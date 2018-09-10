#include "HEPDView3D.hh"

ClassImp(HEPDView3D);

using namespace std;

//class

HEPDView3D::HEPDView3D()
{

  //ispressed =false;
  iFile = NULL;
  igFile = NULL;
  iFilePath = NULL;
  gTrackList = NULL;
  gEve = NULL;
  gTextEntry_ntrkhit= NULL;
  gTextEntry_ntofhit= NULL;

  list_tracks = 0;
  list_trk_marker =0;
  list_calo_marker =0;
  list_primary_marker =0;

  trk1 = 0;

  NCOLOR = 6;
  // color = {kOrange+8,kYellow, kWhite, kGreen, kAzure, kMagenta};
  log10_paletteMin = -3; //min signal
  log10_paletteMax = 3.; //max signal
  gStyle->SetPalette(NCOLOR,color,1.);


} // end GView3D::GView3D

/**
 * Default destructor
 */

HEPDView3D::~HEPDView3D(){

  if(iFile) iFile->Close();

  if(list_tracks)delete list_tracks;
  if(list_trk_marker)delete list_trk_marker;
  if(list_calo_marker)delete list_calo_marker;
  if(list_primary_marker)delete list_primary_marker;


} // end destructor


Bool_t HEPDView3D::SetGeoFile(const char* geometryFile){

  igFile = TFile::Open(geometryFile);

    return true;

}

Bool_t HEPDView3D::SetInputFile(const char* iFilePath, Int_t tmpfirstevent, Int_t tmplastevent){

  iFile = TFile::Open(iFilePath);

  TString fileName = "";
  fileName+=iFilePath;

  fileName.Append(":/HEPD");

  dir = (TDirectory*)iFile->Get(fileName);
  dir->GetObject("EventTree",fTree);

  Event = 0;
  
  fTree->SetBranchAddress("Event", &Event, &b_Event);
  nentries = fTree->GetEntries();

  firstevent = tmpfirstevent;

  if (tmplastevent == -1){
    nevents = nentries - firstevent;
    lastevent = nentries-1;
  }
  else
    nevents = lastevent+1 - firstevent;

  cout << "Analyzing " << nevents << " starting at event ";
  cout << firstevent << " ending at event " << lastevent << endl;

  cout << endl <<" <<< done"<<endl;

  return true;

} // end setimput file

void HEPDView3D::TerminateEVE()
{
  
  gSystem->ProcessEvents();
  gApplication->Terminate(0);

}

void HEPDView3D::run_hepd(Bool_t auto_size,const char* geometryFile)
{
  event_id = firstevent;
  minevent = firstevent;
  maxevent = lastevent;

  make_gui();

  // Adapt the main frame to the screen size...
  if (auto_size)
  {
    Int_t qq;
    UInt_t ww, hh;
    gVirtualX->GetWindowSize(gVirtualX->GetDefaultRootWindow(), qq, qq, ww, hh);
    Float_t screen_ratio = (Float_t)ww / (Float_t)hh;
    if (screen_ratio > 1.5) {
      gEve->GetBrowser()->MoveResize(100, 50, ww - 300, hh - 100);
    } else {
      gEve->GetBrowser()->Move(50, 50);
    };
  };

  update_image(event_id,geometryFile);
  load_event(event_id);
 

} // end run_hepd

//_____________________________________________________________________________

//---------------------------------------------//
//LOAD EVENT
//---------------------------------------------//

void HEPDView3D::load_event(Int_t event_id)
{

  if(list_tracks) list_tracks->DestroyElements();
  if(list_trk_marker) list_trk_marker->DestroyElements();
  if(list_calo_marker) list_calo_marker->DestroyElements();
  if(list_primary_marker) list_primary_marker->DestroyElements();

  if(trk1) trk1->DestroyElements();
  if(trk1) trk1->Destroy();

  printf("Loading event %d.\n", event_id);
  gTextEntry->SetTextColor(0xff0000);
  gTextEntry->SetText(Form("Loading event %d...",event_id));
  gSystem->ProcessEvents();

  hepd_esd_read();

  gTextEntry->SetTextColor((Pixel_t)0x000000);
  gTextEntry->SetText(Form("Event %d loaded",event_id));
  printf("Event %d loaded ", event_id);
  
  gProgress->SetPosition(((Double_t)event_id - (Double_t)firstevent)/((Double_t)nevents-(Double_t)firstevent-1.)*100);

  gEve->FullRedraw3D(kTRUE,kTRUE);



} // end load_event()


//___________________________________________________________________
void HEPDView3D::update_image(Int_t event_id,const char* geomFile)
{

 
  gEve->RegisterGeometryAlias("WHOLE",geomFile);

  TGeoManager *gg;

  gg = gEve->GetGeometryByAlias("WHOLE");
  gg->SetMaxVisNodes(1);
  gg->SetVisDensity(100);

  TObjArray*obj = gg->GetListOfVolumes();

  for (int kk=0; kk<obj->GetEntries();kk++){

  TObject*vol = obj->At(kk);

  }

  TGeoNode*node = gg->GetNode(0);

  TEveGeoTopNode*trk1 = new TEveGeoTopNode(gg, node); //gg->GetTopVolume()->FindNode("HEPDBoxThermalBlanketOut0x56348e7b0920"));
  //trk1->SetVisLevel(6);

  gEve->AddGlobalElement(trk1);

  // gProgress->SetPosition(((Double_t)event_id - (Double_t)firstevent)/((Double_t)nevents-(Double_t)firstevent-1.)*100);

  v = gEve->GetDefaultGLViewer();

  gEve->FullRedraw3D(kTRUE,kTRUE); // Reset camera after the first event has been shown.

  v->ColorSet().Background().SetColor(kBlack);
  v->SetCurrentCamera(TGLViewer::kCameraPerspXOY);
  v->SetGuideState(TGLUtil::kAxesNone, kTRUE, kFALSE, 0);

  //v->GetClipSet()->SetClipType(TGLClip::EType(1));
  v->RefreshPadEditor(v);
  v->SetCurrentCamera(TGLViewer::kCameraPerspXOY);
  v->CurrentCamera().RotateRad(-0.5, -2.4);
  //  v->DoDraw();
  v->ColorSet().Background().SetColor(kBlack);
  v->SetGuideState(TGLUtil::kAxesNone, kTRUE, kFALSE, 0);

  v->RefreshPadEditor(v);

 

} // end update_image()

/******************************************************************************/
// GUI
/******************************************************************************/
//______________________________________________________________________________
//

// EvNavHandler class is needed to connect GUI signals.
//class EvNavHandler
//{
//public:

void HEPDView3D::Fwd()
{
  if (event_id < nevents-1) {
    ++event_id;
    load_event(event_id);
  
  } else {
    gTextEntry->SetTextColor(0xff0000);
    gTextEntry->SetText("Already at last event");
    printf("Already at last event.\n");
  }
}

//__________________________
void HEPDView3D::Bck()
{
  if (event_id > firstevent) {
    --event_id;
    load_event(event_id);
  
  } else {
    gTextEntry->SetTextColor(0xff0000);
    gTextEntry->SetText("Already at first event");
    printf("Already at first event.\n");
  }
}

void HEPDView3D::stringcopy(TString s1, const TString s2, Int_t from=0, Int_t to=0){
  if ( to == 0 ){
    Int_t t2length = s2.Length();
    s1 = "";
    to = t2length;
  };
  for (Int_t i = from; i<to; i++){
    s1.Append(s2[i],1);
  };
}


//_____________________________________
void HEPDView3D::CompleteText() {
  TGTextEntry *entry = (TGTextEntry*)gTQSender;
  TString text = entry->GetText();
  Int_t curspos = entry->GetCursorPosition();
  Int_t ncurspos = 0;
  TString textrem;
  stringcopy(textrem,text,curspos,text.Length());
  text.Resize(curspos);
  if ( !strcmp(text.Data(),"") ) {
    stringstream ntext;
    ntext.str("");
    ntext << gSystem->ExpandPathName("$HOME");
    text = ntext.str().c_str();
    curspos=ntext.str().length();
  };
  char *test = gSystem->ExpandPathName(text.Data());
  const char *dir = gSystem->DirName(test);
  const char *base = gSystem->BaseName(test);
  TString lbase = base;
  Int_t lenbase = lbase.Length();
  TSystemDirectory *tsd = new TSystemDirectory("",dir);
  TSystemFile *file;
  TList *lsmatch = new TList;
  Int_t numf = 0;
  Int_t numgo = 0;
  Int_t charmac = 0;
  TString tbase;
  Int_t k = 0;
  stringstream newtext;
  TSystemFile *mfile;
  TSystemFile nfile;
  TList *lsf = new TList;
  const char *ctbase;
  if ( !tsd->GetListOfFiles() ){
    gVirtualX->Bell(0);
    goto end;
  };
  lsf = tsd->GetListOfFiles();
  file = (TSystemFile*)lsf->First();
  //
  while( (TSystemFile*)lsf->After(file) ){
    numf++;
    file = (TSystemFile*)lsf->After(file);
    if ( !strncmp(file->GetName(),base,lenbase) ){
      numgo++;
      lsmatch->AddLast(file);
      const char *ctbase = file->GetName();
      tbase = ctbase;
      charmac = tbase.Length();
    };
  };
  newtext.str("");
  if ( numgo > 0 ){
    mfile = (TSystemFile*)lsmatch->First();
    ctbase = mfile->GetName();
    while ( (!strcmp(ctbase,"..") || !strcmp(ctbase,".")) && (TSystemFile*)lsmatch->After(mfile) ) {
      mfile = (TSystemFile*)lsmatch->After(mfile);
      ctbase = mfile->GetName();
    };
    tbase = ctbase;
    base = ctbase;
    charmac = tbase.Length();;
    while( (TSystemFile*)lsmatch->After(mfile) ){
      mfile = (TSystemFile*)lsmatch->After(mfile);
      ctbase = mfile->GetName();
      while ( (!strcmp(ctbase,"..") || !strcmp(ctbase,".")) && (TSystemFile*)lsmatch->After(mfile) ) {
	mfile = (TSystemFile*)lsmatch->After(mfile);
	ctbase = mfile->GetName();
      };
      tbase = ctbase;
      k = 0;
      for ( Int_t i=lenbase; i<=tbase.Length(); i++){
	if ( !strncmp(tbase.Data(),base,i) && strcmp(tbase.Data(),base) ){
	  k = i;
	};
      };
      if ( k <= charmac){
	charmac = k;
      };
    };
    gVirtualX->Bell(0);
  };
  if ( !charmac ) {
    gVirtualX->Bell(0);
    goto end;
  };
  tbase.Resize(charmac);
  if ( !strcmp(dir,"") || !strcmp(dir,"/") ){
    newtext << "/" << tbase.Data();
  } else {
    newtext << dir << "/" << tbase.Data();
  };
  nfile = TSystemFile(tbase.Data(),dir);
  if ( nfile.IsDirectory() && numgo == 1 ) newtext << "/";
  ncurspos = newtext.str().length();
  if ( textrem.Length() ) newtext << textrem.Data();
  entry->SetText(newtext.str().c_str());
  entry->SetCursorPosition(ncurspos);
 end:
  lsf->Clear();
  lsmatch->Clear();
}

//_________________________________________-

void HEPDView3D::SaveAs() {
  TString text = fText4->GetText();
  Int_t sel = fCombo->GetSelected();
  TString ty;
  switch (sel){
  case (0):
    ty = ".gif";
    break;
  case (1):
    ty = ".jpg";
    break;
  case (2):
    ty = ".tiff";
    break;
  case (3):
    ty = ".xpm";
    break;
  case (4):
    ty = ".xml";
    break;
  case (5):
    ty = ".png";
    break;
  case (6):
    ty = ".svg";
    break;
  case (7):
    ty = ".root";
    break;
  case (8):
    ty = ".C";
    break;
  };
  if ( sel == 0 ) ty = ".png";
  stringstream name;
  if ( strcmp(text.Data(),"") ){
    name.str("");
    name << text.Data();
    name << ty.Data();
    v->SavePicture(name.str().c_str());
    name.str("");
    name << " Saved as ";
    name << text.Data();
    name << ty.Data();
    //DIALOG(0,name.str().c_str());
    printf(" Saved as %s%s \n",text.Data(),ty.Data());
  };
}


void HEPDView3D::jumpto() {
  TGTextEntry *entry = (TGTextEntry*)gTQSender;



  TString text = entry->GetText();
  Int_t j;
  j = atoi(text.Data());
  if ( j < minevent || j > maxevent ) {
    printf("\n You can choose between %i and %i \n",minevent,maxevent);
  } else {

    printf("\n Jumping to event number %i\n\n",j);
    event_id = j;
    load_event(event_id);
   
  };
  entry->SetText("");
}

Int_t HEPDView3D::GetEventID()
{
  cout << "THE EVENT WE ARE STUDYING IS NUMBER " << event_id << endl;
  return event_id;
}

//______________________________________________________________________________

void HEPDView3D::make_gui()
{

  //TEveManager::Create();
  TEveManager::Create(kTRUE,"V");
  TEveBrowser* browser = browser = gEve->GetBrowser();
  browser->StartEmbedding(TRootBrowser::kLeft);
  TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1400, 600);
  frmMain->SetWindowName("GAPS GUI");
  frmMain->SetCleanup(kDeepCleanup);
  browser->Connect("CloseWindow()","HEPDView3D", this, "CloseWindow()");


  TGHorizontalFrame* hf = new TGHorizontalFrame(frmMain);
  {
    TString icondir( Form("%s/icons/", gSystem->Getenv("ROOTSYS")) );

    TGPictureButton *b = new TGPictureButton(hf, gClient->GetPicture(icondir + "GoBack.gif"));

    hf->AddFrame(b, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 10, 2, 10, 10));
    b->Connect("Clicked()", "HEPDView3D", this, "Bck()");

    TGPictureButton *bb = new TGPictureButton(hf, gClient->GetPicture(icondir + "GoForward.gif"));

    hf->AddFrame(bb, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 2, 10, 10, 10));
    bb->Connect("Clicked()", "HEPDView3D", this, "Fwd()");

    gTextEntry = new TGTextEntry(hf);
    gTextEntry->SetEnabled(kFALSE);
    hf->AddFrame(gTextEntry, new TGLayoutHints(kLHintsLeft | kLHintsCenterY  |
					       kLHintsExpandX, 2, 10, 10, 10));
  }

  frmMain->AddFrame(hf, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,20,0));

  gProgress = new TGHProgressBar(frmMain, TGProgressBar::kFancy, nevents);
  gProgress->ShowPosition(kTRUE, kFALSE, "%.0f%% events");
  gProgress->SetBarColor("green");
  frmMain->AddFrame(gProgress, new TGLayoutHints(kLHintsExpandX, 10, 10, 5, 5));

  TGHorizontalFrame* hf_info = new TGHorizontalFrame(frmMain);

  gTextEntry_info = new TGTextEntry(hf_info);
  gTextEntry_info->SetEnabled(kFALSE);
  hf_info->AddFrame(gTextEntry_info, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  gTextEntry_info_2 = new TGTextEntry(hf_info);
  gTextEntry_info_2->SetEnabled(kFALSE);
  hf_info->AddFrame(gTextEntry_info_2, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  frmMain->AddFrame(hf_info, new TGLayoutHints(kLHintsExpandX,10,10,5,5));


  TGHorizontalFrame* hf_nhit = new TGHorizontalFrame(frmMain);

  gTextEntry_nhit = new TGTextEntry(hf_nhit);
  gTextEntry_nhit->SetEnabled(kFALSE);
  hf_nhit->AddFrame(gTextEntry_nhit, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  gTextEntry_totedep = new TGTextEntry(hf_nhit);
  gTextEntry_totedep->SetEnabled(kFALSE);
  hf_nhit->AddFrame(gTextEntry_totedep, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  frmMain->AddFrame(hf_nhit, new TGLayoutHints(kLHintsExpandX,10,10,5,5));

  TGHorizontalFrame* hf_toftrkhit = new TGHorizontalFrame(frmMain);

  gTextEntry_ntofhit = new TGTextEntry(hf_toftrkhit);
  gTextEntry_ntofhit->SetEnabled(kFALSE);
  hf_toftrkhit->AddFrame(gTextEntry_ntofhit, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  gTextEntry_ntrkhit = new TGTextEntry(hf_toftrkhit);
  gTextEntry_ntrkhit->SetEnabled(kFALSE);
  hf_toftrkhit->AddFrame(gTextEntry_ntrkhit, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  frmMain->AddFrame(hf_toftrkhit, new TGLayoutHints(kLHintsExpandX,10,10,5,5));

  // hit in veto

  TGHorizontalFrame* hf_trigger = new TGHorizontalFrame(frmMain);

  gTextEntry_1311 = new TGTextEntry(hf_trigger);
  gTextEntry_1311->SetEnabled(kFALSE);
  hf_trigger->AddFrame(gTextEntry_1311, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  gTextEntry_1312 = new TGTextEntry(hf_trigger);
  gTextEntry_1312->SetEnabled(kFALSE);
  hf_trigger->AddFrame(gTextEntry_1312, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  gTextEntry_1313 = new TGTextEntry(hf_trigger);
  gTextEntry_1313->SetEnabled(kFALSE);
  hf_trigger->AddFrame(gTextEntry_1313, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  frmMain->AddFrame(hf_trigger, new TGLayoutHints(kLHintsExpandX,10,10,5,5));

  // trigger 2 frame

  TGHorizontalFrame* hf_trigger_2 = new TGHorizontalFrame(frmMain);

  gTextEntry_1321 = new TGTextEntry(hf_trigger_2);
  gTextEntry_1321->SetEnabled(kFALSE);
  hf_trigger_2->AddFrame(gTextEntry_1321, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  gTextEntry_1322 = new TGTextEntry(hf_trigger_2);
  gTextEntry_1322->SetEnabled(kFALSE);
  hf_trigger_2->AddFrame(gTextEntry_1322, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  gTextEntry_1323 = new TGTextEntry(hf_trigger_2);
  gTextEntry_1323->SetEnabled(kFALSE);
  hf_trigger_2->AddFrame(gTextEntry_1323, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  frmMain->AddFrame(hf_trigger_2, new TGLayoutHints(kLHintsExpandX,10,10,5,5));


  // angle of trk

  TGHorizontalFrame* hf_angle = new TGHorizontalFrame(frmMain);

  gTextEntry_angle = new TGTextEntry(hf_angle);
  gTextEntry_angle->SetEnabled(kFALSE);
  hf_angle->AddFrame(gTextEntry_ntofhit, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  frmMain->AddFrame(hf_angle, new TGLayoutHints(kLHintsExpandX,10,10,5,5));


  // trigger hits

  TGHorizontalFrame* hf_veto = new TGHorizontalFrame(frmMain);

  gTextEntry_lateralveto = new TGTextEntry(hf_veto);
  gTextEntry_lateralveto->SetEnabled(kFALSE);
  hf_veto->AddFrame(gTextEntry_lateralveto, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  gTextEntry_bottomveto = new TGTextEntry(hf_veto);
  gTextEntry_bottomveto->SetEnabled(kFALSE);
  hf_veto->AddFrame(gTextEntry_bottomveto, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));

  frmMain->AddFrame(hf_veto, new TGLayoutHints(kLHintsExpandX,10,10,5,5));


  //TGHorizontalFrame* hf_palette = new TGHorizontalFrame(frmMain);

  // ENERGY PALETTE --------------------------------------------------

  TGHorizontalFrame* hf_palette = new TGHorizontalFrame(frmMain);

  const TGPicture *ipic=(TGPicture *)gClient->GetPicture("palette.png");
  TGIcon *icon = new TGIcon(hf_palette,ipic,1200,100);

  hf_palette->AddFrame(icon,new TGLayoutHints(kLHintsLeft|kLHintsBottom,1, 15, 1, 1));
  frmMain->AddFrame(hf_palette, new TGLayoutHints(kLHintsExpandX,10,10,5,5));

  //---------------------------------------------------------------------
  
  TGHorizontalFrame* hf_check_vis = new TGHorizontalFrame(frmMain);
  TGButtonGroup* bgr = new TGButtonGroup(hf_check_vis,"View Direction",kHorizontalFrame);
  fR[0] = new TGRadioButton(bgr,new TGHotString("X0Y    "));
  fR[0]->Connect("Clicked()","HEPDView3D", this,"SetLateralVis3()");
  fR[1] = new TGRadioButton(bgr,new TGHotString("Y0Z    "));
  fR[1]->Connect("Clicked()","HEPDView3D", this,"SetLateralVis2()");
  fR[2] = new TGRadioButton(bgr,new TGHotString("Z0X "));
  fR[2]->Connect("Clicked()","HEPDView3D", this,"SetLateralVis1()");
  fR[0]->SetState(kButtonDown);
  bgr->Show();
  hf_check_vis->AddFrame(bgr, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsCenterY,10,5,5,5));
  frmMain->AddFrame(hf_check_vis, new TGLayoutHints(kLHintsExpandX));


  // RESET BUTTON

  TGHorizontalFrame* hf_reset_b = new TGHorizontalFrame(frmMain);

  TGTextButton *reset = new TGTextButton(hf_reset_b,"Reset Camera");
  reset->Connect("Clicked()","HEPDView3D",this,"ResetCameraVis()");
  hf_reset_b->AddFrame(reset, new TGLayoutHints(kLHintsExpandX, 0, 8, 1, 1));
  frmMain->AddFrame(hf_reset_b, new TGLayoutHints(kLHintsExpandX));
  


  TGHorizontalFrame *htexts = new TGHorizontalFrame(frmMain);
  TGHorizontalFrame *htextsl = new TGHorizontalFrame(frmMain);

  fText4 = new TGTextEntry(htexts, new TGTextBuffer(18),-1);
  TGTextButton *save = new TGTextButton(htextsl,"Save");
  // save->SetTextColor(0xffff00,kTRUE);
  save->Connect("Clicked()","TGTextEntry",fText4,"ReturnPressed()");
  fText4->Connect("ReturnPressed()", "HEPDView3D", this,"SaveAs()");
  fText4->Connect("TabPressed()", "HEPDView3D", this,"CompleteText()");
  htexts->AddFrame(fText4, new TGLayoutHints(kLHintsExpandX, 0, 8, 1, 1));
  //
  fCombo = new TGComboBox(htexts, 98);
  fCombo->AddEntry(".gif", 0);
  fCombo->AddEntry(".jpg", 1);
  fCombo->AddEntry(".tiff",2);
  fCombo->AddEntry(".xpm", 3);
  fCombo->AddEntry(".xml", 4);
  fCombo->AddEntry(".png", 5);
  fCombo->AddEntry(".svg", 6);
  fCombo->AddEntry(".root",7);
  fCombo->AddEntry(".C",8);
  fCombo->Select(0);
  fCombo->Resize(50,23);


  htexts->AddFrame(fCombo, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 2, 2, 2, 1));
  //
  htextsl->AddFrame(save, new TGLayoutHints(kLHintsExpandX,0,0,2,1));
  frmMain->AddFrame(htexts, new TGLayoutHints(kLHintsExpandX,0,0,2,5));
  frmMain->AddFrame(htextsl, new TGLayoutHints(kLHintsExpandX,8,0,5,2));


  TGHorizontalFrame* hf_legend = new TGHorizontalFrame(frmMain);

  const TGPicture *ipicleg=(TGPicture *)gClient->GetPicture("legend.png");
  TGIcon *iconleg = new TGIcon(hf_legend,ipicleg,1200,100);

  hf_legend->AddFrame(icon,new TGLayoutHints(kLHintsLeft|kLHintsBottom,1, 15, 1, 1));
  frmMain->AddFrame(hf_legend, new TGLayoutHints(kLHintsExpandX,10,10,5,5));


  // JUMP TO A DIRECT EVENT

  TGHorizontalFrame *htext3_jump = new TGHorizontalFrame(frmMain);

  TGTextEntry *fText3 = new TGTextEntry(htext3_jump, new TGTextBuffer(6),-1);
  TGTextButton *Jump = new TGTextButton(htext3_jump,"go");
  Jump->Connect("Clicked()","TGTextEntry",fText3,"ReturnPressed()");
  fText3->Connect("ReturnPressed()", "HEPDView3D", this,"jumpto()");
  htext3_jump->AddFrame(new TGLabel(htext3_jump, new TGHotString("jump to")), new TGLayoutHints(kLHintsExpandX,0,0,1,1));
  htext3_jump->AddFrame(fText3, new TGLayoutHints(kLHintsExpandX, 0, 8, 0, 0));
  htext3_jump->AddFrame(Jump, new TGLayoutHints(kLHintsExpandX,0,0,0,0));

  //
  frmMain->AddFrame(htext3_jump,new TGLayoutHints(kLHintsExpandX,5,5,5,5));

  // END JUMP TO A DIRECT EVENT
  /*
  TGHorizontalFrame *hf_quit = new TGHorizontalFrame(frmMain);

  TGTextEntry *fText5 = new TGTextEntry(hf_quit, new TGTextBuffer(6),-1);
  TGTextButton *quit = new TGTextButton(hf_quit,"Quit");
  quit->Connect("Clicked()","TGTextEntry",fText5,"ReturnPressed()");
  fText5->Connect("ReturnPressed()", "HEPDView3D", this,"TerminateEVE()");
  hf_quit->AddFrame(quit, new TGLayoutHints(kLHintsExpandX,0,0,0,0));

  frmMain->AddFrame(hf_quit,new TGLayoutHints(kLHintsExpandX,5,5,5,5));
  */
  // merge all frames in the mother frame


  frmMain->MapSubwindows();
  frmMain->Resize();
  frmMain->MapWindow();

  browser->StopEmbedding();
  browser->SetTabTitle("Event control", 0);

  browser->StartEmbedding(TRootBrowser::kBottom);

  TGMainFrame* frmMain2 = new TGMainFrame(gClient->GetRoot(), 1400, 400);
  frmMain2->SetWindowName("PDG");
  //frmMain->SetBackgroundColor(0);
  frmMain2->SetCleanup(kDeepCleanup);

 
}

//_____________________________________________

void HEPDView3D::hepd_esd_read()
{

  
  if (list_tracks == 0){
    list_tracks = new TEveStraightLineSet("Tracks List");
    list_tracks->SetName("Tracks");
    gEve->AddElement(list_tracks);
  }

  
  if (list_trk_marker == 0){
    list_trk_marker = new TEvePointSet(8);
    list_trk_marker->SetName("TRK HITS");

    gEve->AddElement(list_trk_marker);

  }
  

  
  if (list_calo_marker == 0){
    list_calo_marker = new TEvePointSet(8);
    list_calo_marker->SetName("CALO HITS");

    gEve->AddElement(list_calo_marker);

  }
  

  
  if (list_primary_marker == 0){
    list_primary_marker = new TEvePointSet(8);
    list_primary_marker->SetName("PRIMARY");

    gEve->AddElement(list_primary_marker);
  
  }
  

  gProgress->Reset();

  cout<<"\n >>>>> READING FROM MC FILE"<<endl;
  
  fTree->LoadTree(event_id);
  nb = fTree->GetEntry(event_id);

  std::vector<RootTrack> myTracks = Event->GetTracks();
  std::vector<RootVertex> myVertex = Event->GetVertex();
  std::vector<RootCaloHit> myCaloHit = Event->GetCaloHit();
  std::vector<RootCaloHit> myVetoHit = Event->GetVetoHit();
  std::vector<RootTrackerHit> myTrackerHit = Event->GetTrackerHit();
  std::vector<RootPmtHits> myPmtHit = Event->GetPmtHits();

  TString str_beta = "Primary: ";
  str_beta += myTracks.at(0).GetParticleName();

  gTextEntry_info->SetTextColor(kBlack);
  gTextEntry_info->SetText(str_beta);


  TString str_kin = "Pr. kin = ";
  str_kin += Form("%.3f",myTracks[0].GetKinEnergy());

  gTextEntry_info_2->SetTextColor(kBlack);
  gTextEntry_info_2->SetText(str_kin);

  
  TString str_num_trk = "TRK hit = ";
  str_num_trk += myTrackerHit.size();

  gTextEntry_nhit->SetTextColor(kBlack);
  gTextEntry_nhit->SetText(str_num_trk);
   
  TString str_num_calo = "CALO hit = ";
  str_num_calo += myCaloHit.size();

  gTextEntry_ntofhit->SetTextColor(kBlack);
  gTextEntry_ntofhit->SetText(str_num_calo);


  Double_t totecalo=0.;
  Double_t totetrk=0.;

  Int_t hit_1311=0;
  Int_t hit_1312=0;
  Int_t hit_1313=0;
  Int_t hit_1321=0;
  Int_t hit_1322=0;
  Int_t hit_1323=0;

  Int_t hit_bottom=0;
  Int_t hit_lateral=0;

  // LOOP OBVER CALO HITS

  Double_t vrt_x;
  Double_t vrt_y;
  Double_t vrt_z;

  for(int vrt_hit_k=0; vrt_hit_k < myVertex.size(); vrt_hit_k++){

    RootVertex vert = myVertex.at(vrt_hit_k);
    vrt_x = vert.GetPosition().X();
    vrt_y = vert.GetPosition().Y();
    vrt_z = vert.GetPosition().Z();

  

      }

  for(int c_hit_k=0; c_hit_k < myCaloHit.size(); c_hit_k++){
   
     RootCaloHit chit = myCaloHit.at(c_hit_k);
    totecalo += chit.GetTotalEdep();
 
   
    TEvePointSet* caloPoints_edep = new TEvePointSet(8);
    caloPoints_edep->SetMarkerStyle(24);
    caloPoints_edep->SetMarkerSize(2.0);

    caloPoints_edep->SetPoint(0,chit.GetEntryPoint().X()/10.,chit.GetEntryPoint().Y()/10.,chit.GetEntryPoint().Z()/10.);

    int colorIndex = (int) (NCOLOR*(log10(chit.GetTotalEdep())-log10_paletteMin)/(log10_paletteMax-log10_paletteMin) );
    Color_t thisColor = kBlack;
    if(colorIndex<=0)thisColor = kBlack;
    if(colorIndex>0 && colorIndex<NCOLOR)thisColor = color[colorIndex];

    caloPoints_edep->SetMarkerColor(thisColor);
     list_calo_marker->AddElement(caloPoints_edep);
   

     if(chit.GetVolume()==1311)hit_1311++;
     if(chit.GetVolume()==1312)hit_1312++;
     if(chit.GetVolume()==1313)hit_1313++;
     if(chit.GetVolume()==1321)hit_1321++;
     if(chit.GetVolume()==1322)hit_1322++;
     if(chit.GetVolume()==1323)hit_1323++;
     if(chit.GetVolume()==1430)hit_bottom++;
     if(chit.GetVolume()==1411 || chit.GetVolume()==1412 || chit.GetVolume()==1421 || chit.GetVolume()==1422)hit_lateral++;

 }

  TString s_hit_1311 = "trg 1311:";
  s_hit_1311 += hit_1311;
  gTextEntry_1311->SetTextColor(kBlack);
  gTextEntry_1311->SetText(s_hit_1311);

  TString s_hit_1312 = "trg 1312:";
  s_hit_1312 += hit_1312;
  gTextEntry_1312->SetTextColor(kBlack);
  gTextEntry_1312->SetText(s_hit_1312);

  TString s_hit_1313 = "trg 1313:";
  s_hit_1313 += hit_1313;
  gTextEntry_1313->SetTextColor(kBlack);
  gTextEntry_1313->SetText(s_hit_1313);

  TString s_hit_1321 = "trg 1321:";
  s_hit_1321 += hit_1321;
  gTextEntry_1321->SetTextColor(kBlack);
  gTextEntry_1321->SetText(s_hit_1321);

  TString s_hit_1322 = "trg 1322:";
  s_hit_1322 += hit_1322;
  gTextEntry_1322->SetTextColor(kBlack);
  gTextEntry_1322->SetText(s_hit_1322);

  TString s_hit_1323 = "trg 1323: ";
  s_hit_1323 += hit_1323;
  gTextEntry_1323->SetTextColor(kBlack);
  gTextEntry_1323->SetText(s_hit_1323);

  TString s_hit_bottom = "bottom veto hits: ";
  s_hit_bottom += hit_bottom;
  gTextEntry_bottomveto->SetTextColor(kBlack);
  gTextEntry_bottomveto->SetText(s_hit_bottom);

  TString s_hit_lateral = "lateral veto hits: ";
  s_hit_lateral += hit_lateral;
  gTextEntry_lateralveto->SetTextColor(kBlack);
  gTextEntry_lateralveto->SetText(s_hit_lateral);


  gEve->AddElement(list_calo_marker);


  // LOPP OVER TRACKER HITS

  for(int t_hit_k=0; t_hit_k < myTrackerHit.size(); t_hit_k++){
    RootTrackerHit thit = myTrackerHit.at(t_hit_k);

    totetrk += thit.GetELoss();

    TEvePointSet* trkPoints_edep = new TEvePointSet(8);
    trkPoints_edep->SetMarkerStyle(20);
    trkPoints_edep->SetMarkerSize(2.0);

    trkPoints_edep->SetPoint(0,thit.GetEntryPoint().X()/10.,thit.GetEntryPoint().Y()/10.,thit.GetEntryPoint().Z()/10.);

    int colorIndex = (int) (NCOLOR*(log10(thit.GetELoss())-log10_paletteMin)/(log10_paletteMax-log10_paletteMin) );
    Color_t thisColor = kBlack;
    if(colorIndex<=0)thisColor = kRed;
    if(colorIndex>0 && colorIndex<NCOLOR)thisColor = color[colorIndex];

    trkPoints_edep->SetMarkerColor(thisColor);

    list_trk_marker->AddElement(trkPoints_edep);

    TString s_angle = "theta =  ";
    s_angle += Form("%.3f",thit.GetThetaAtEntry());
    gTextEntry_angle->SetTextColor(kBlack);
    gTextEntry_angle->SetText(s_angle);



  }

  gEve->AddElement(list_trk_marker);



  TString str_trk_dep = "TRK Edep = ";
  str_trk_dep += Form("%.2f",totetrk);

  gTextEntry_totedep->SetTextColor(kBlack);
  gTextEntry_totedep->SetText(str_trk_dep);


  TString str_calo_dep = "CALO Edep = ";
  str_calo_dep += Form("%.2f",totecalo);

  gTextEntry_ntrkhit->SetTextColor(kBlack);
  gTextEntry_ntrkhit->SetText(str_calo_dep);


  for(int nt=0; nt < myTracks.size(); nt++){ //loop over tracks

    RootTrack t = myTracks.at(nt);

    int Pdg  = t.GetPDG();
    int parent_ID  = t.GetParentTrackID();   
    int track_ID  = t.GetTrackID();

    Double_t p_vertex_x,p_vertex_y,p_vertex_z;
   
   
    cout << " track_ID=" << track_ID << " parent ID=" << parent_ID << " PDG=" << Pdg  << endl;


    TEveStraightLineSet* tracksXYZ = new TEveStraightLineSet("tracks");
    tracksXYZ->SetLineWidth(2);
    tracksXYZ->SetLineColor(kViolet);
    // bugfix: Set color right away
    if( Pdg == 2112) tracksXYZ->SetLineColor(kGreen); //neutron
    if( Pdg == 2212) tracksXYZ->SetLineColor(kRed); //proton
    if( Pdg == 13 || Pdg == -13) tracksXYZ->SetLineColor(kBlue); //muon-,+
    if(  Pdg == 11) tracksXYZ->SetLineColor(kYellow); //electrons
    if( Pdg == 111 || Pdg == 211 || Pdg == -211 ) tracksXYZ->SetLineColor(kWhite); //pions
    if( Pdg == 22) tracksXYZ->SetLineColor(kMagenta); //photons
    if( Pdg == -11) tracksXYZ->SetLineColor(kAzure); //positrons
    if(Pdg != 2112 && Pdg != 2212 && Pdg != 13  && Pdg != -13 && Pdg != -11 && Pdg != 11 && Pdg != 111 && Pdg != 211 && Pdg != -211 && Pdg != 22)  tracksXYZ->SetLineColor(kBlack);


    //--------------------------------------------------------------------

    for(int c_hit=0; c_hit < myTrackerHit.size(); c_hit++){ 
   
      RootTrackerHit chit = myTrackerHit.at(c_hit);

      //cout << " myTrackerHit.size() = " << myTrackerHit.size() << endl;

   
      TEvePointSet* trkPoints = new TEvePointSet(8);
      trkPoints->SetMarkerColor(2);
      trkPoints->SetMarkerStyle(24);
      trkPoints->SetMarkerSize(1.5);

      TVector3 c_pos_in =  chit.GetEntryPoint();
      TVector3 c_pos_out =  chit.GetExitPoint();

     
    } // end loop over tracker hits

    //-----------------------------------------------

    TVector3 v_pos =  t.GetPosition();

    Double_t vertex_x = v_pos.X();
    Double_t vertex_y = v_pos.Y();
    Double_t vertex_z = v_pos.Z();

    if(parent_ID==0){

      p_vertex_x = v_pos.X();
      p_vertex_y = v_pos.Y();
      p_vertex_z = v_pos.Z();

    }


    TEvePointSet* primary = new TEvePointSet(8);
    primary->SetMarkerColor(0);
    primary->SetMarkerStyle(30);
    primary->SetMarkerSize(2.0);

   
    if(parent_ID==0)primary->SetPoint(0,vertex_x/10.,vertex_y/10.,vertex_z/10.);
    list_primary_marker->AddElement(primary);
    // gEve->AddElement(list_primary_marker);


    if(parent_ID==0)tracksXYZ->AddLine(vrt_x/10.,vrt_y/10.,vrt_z/10.,p_vertex_x/10.,p_vertex_y/10.,p_vertex_z/10.);
    //    if(nt>0)tracksXYZ->AddLine(myTracks.at(nt-1).GetPosition().X()/10.,myTracks.at(nt-1).GetPosition().Y()/10.,myTracks.at(nt-1).GetPosition().Z()/10.,vertex_x/10.,vertex_y/10.,vertex_z/10.);
    if(parent_ID!=0)tracksXYZ->AddLine(vrt_x/10.,vrt_y/10.,vrt_z/10.,vertex_x/10.,vertex_y/10.,vertex_z/10.);

    list_tracks->AddElement(tracksXYZ);
    //    gEve->AddElement(list_tracks);


  } // end loop nt over tracks

  gEve->AddElement(list_tracks);
  gEve->AddElement(list_primary_marker);


  //  gEve->AddElement(list_primary_marker);


} // end analyze 1 event

//_______________________________________________________________________________________________________________

void HEPDView3D::GetPalette()
{

  TCanvas* cc = new TCanvas("cc","cc",1200,100);

  cc = new TCanvas("cc","cc",219,77);


  TBox *palette;
  TLatex lat1,lat2,lat3,lat4,lat5,lat6;

  for(int ic=0; ic<=5; ic++ ){

    //palette[ic] = new TBox(xl+ic*dx,yl,xh+ic*dx,yh);
    palette = new TBox(0.+(ic*0.165),0.45,0.165+(ic*0.165),0.9);
    if(ic ==0)palette->SetFillColor(kOrange+8);
    if(ic ==1)palette->SetFillColor(kYellow);
    if(ic ==2)palette->SetFillColor(kWhite);
    if(ic ==3)palette->SetFillColor(kGreen);
    if(ic ==4)palette->SetFillColor(kAzure);
    if(ic ==5)palette->SetFillColor(kMagenta);

    cc->cd();
    palette->Draw();
    //lat1.DrawLatex(0.,0.15,"10^{-3}");
    lat2.DrawLatex(0.165,0.15,"10^{-2}");
    lat3.DrawLatex(0.330,0.15,"10^{-1}");
    lat4.DrawLatex(0.495,0.15,"1");
    lat5.DrawLatex(0.66,0.15,"10");
    lat6.DrawLatex(0.825,0.15,"10^{2}");

    //lat1.SetTextSize(0.3);
    lat2.SetTextSize(0.3);
    lat3.SetTextSize(0.3);
    lat4.SetTextSize(0.3);
    lat5.SetTextSize(0.3);
    lat6.SetTextSize(0.3);
  }

  cc->SaveAs("palette.png");
  delete cc;

}

void HEPDView3D::GetLegend()
{

  TCanvas* cleg = new TCanvas("cleg","cleg",219,77);

  TLatex lat_p,lat_antip,lat_antid,lat_e_pos,lat_pion,lat_photons, lat_nu;//lat_others;

  cleg->cd();
  cleg->SetFillColor(1);

  lat_p.SetTextSize(0.3);
  lat_p.DrawLatex(0.1,0.70,"#color[2]{p}");

  lat_antip.SetTextSize(0.3);
  lat_antip.DrawLatex(0.25,0.70,"#color[4]{#mu^{+},#mu^{-}}");

  lat_antid.SetTextSize(0.3);
  lat_antid.DrawLatex(0.50,0.70,"#color[3]{n}");

  lat_e_pos.SetTextSize(0.3);
  lat_e_pos.DrawLatex(0.65,0.70,"#color[5]{e^{-}}");

  lat_pion.SetTextSize(0.3);
  lat_pion.DrawLatex(0.1,0.20,"#color[0]{#pi^{-},#pi^{+},#pi^{0}}");

  lat_photons.SetTextSize(0.3);
  lat_photons.DrawLatex(0.35,0.20,"#color[6]{photons}");

  lat_nu.SetTextSize(0.3);
  lat_nu.DrawLatex(0.62,0.20,"#color[7]{e^{+}}");



  cleg->SaveAs("legend.png");
  delete cleg;
}

void HEPDView3D::SetLateralVis1()
{
  v->SetCurrentCamera(TGLViewer::kCameraPerspXOZ);
  v->SetStyle(TGLRnrCtx::kFill); //kFill,kOutline,kWireFrame
  gEve->FullRedraw3D(kTRUE, kTRUE);
}

void HEPDView3D::SetLateralVis2()
{
  v->SetCurrentCamera(TGLViewer::kCameraPerspYOZ);
  v->SetStyle(TGLRnrCtx::kFill); //kFill,kOutline,kWireFrame
  gEve->FullRedraw3D(kTRUE, kTRUE);
}

void HEPDView3D::SetLateralVis3()
{
  v->SetCurrentCamera(TGLViewer::kCameraPerspXOY);
  v->SetStyle(TGLRnrCtx::kFill); //kFill,kOutline,kWireFrame
  gEve->FullRedraw3D(kTRUE, kTRUE);
}

void HEPDView3D::ResetCameraVis()
{
  v->SetCurrentCamera(TGLViewer::kCameraPerspXOY);
  v->SetStyle(TGLRnrCtx::kFill); //kFill,kOutline,kWireFrame
  gEve->FullRedraw3D(kTRUE, kTRUE);
}

void HEPDView3D::CloseWindow()
{

  gApplication->Terminate(0);
}
