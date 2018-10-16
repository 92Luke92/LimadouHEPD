#include "GeoManagerClass.hh"
#include "geomag.h"

//ClassImp(GeoManagerClass);
using namespace std; 

#define OBDH_TO_EPOCH_OFFSET_SEC 1230768000
#pragma pack(push, 1)

struct CPUtimestamp_t
{
  unsigned int       OBDH;
  unsigned int       GPS;
  unsigned int       AOCC;
  unsigned int       star_sensor; 
} ;

struct OBDH_struct_t 
{
  unsigned int     sec;
  unsigned short   ms;
  unsigned char    NU[2];
} ;

#pragma pack(pop)

UInt_t GeoManagerClass::GetOBDHstart(TString filename)
{


  f_hepd = new TFile(filename,"read");
  fTreeMD = (TTree*) f_hepd->Get("L2md");
 
  OBDH_struct_t OBDH;
  fTreeMD->SetBranchAddress("OBDH_info",&OBDH.sec);

  unsigned int      CPU_time[2];
  fTreeMD->SetBranchAddress("CPU_time_start_stop_Run[2]", &CPU_time[0]);
  CPUtimestamp_t    timestamp;
  fTreeMD->SetBranchAddress("CPU_timestamp", &timestamp.OBDH);

  fTreeMD->GetEntry(1);
  unsigned int stopRun_sec =  CPU_time[1];
  unsigned int startRun_sec =  CPU_time[0];
  fTreeMD->GetEntry(0);
  UInt_t sec_OBDH = OBDH.sec;
  return (sec_OBDH+(startRun_sec-timestamp.OBDH)/1000);

}

UInt_t GeoManagerClass::GetOBDHstop(TString filename)
{

  f_hepd = new TFile(filename,"read");
  fTreeMD = (TTree*) f_hepd->Get("L2md");

  OBDH_struct_t OBDH;
  fTreeMD->SetBranchAddress("OBDH_info",&OBDH.sec);

  CPUtimestamp_t    timestamp;
  fTreeMD->SetBranchAddress("CPU_timestamp", &timestamp.OBDH);
  unsigned int      CPU_time[2];
  fTreeMD->SetBranchAddress("CPU_time_start_stop_Run[2]", &CPU_time[0]);

  fTreeMD->GetEntry(1);  
  UInt_t startRun_sec =  CPU_time[0];
  UInt_t stopRun_sec =  CPU_time[1];
  fTreeMD->GetEntry(0);
  UInt_t sec_OBDH = OBDH.sec;

  return (sec_OBDH+(stopRun_sec-timestamp.OBDH)/1000);
}


UInt_t GeoManagerClass::GetEpoch(UInt_t year, UInt_t month, UInt_t day, UInt_t hour, UInt_t min, UInt_t sec)
{
  //
  // Print out the unix time abstime (UTC) corresponding to a date
  //
  TTimeStamp t(year,month,day,hour,min,sec,0,1);
  time_t s = t.GetSec();
  
  return (UInt_t) s;
}

void GeoManagerClass::RunTle(Tle tle, double start, double end, double inc, const char*infile, const char* infileL2) //inc = incremento!
{

  UInt_t start_OBDH=GetOBDHstart(infileL2);
  UInt_t stop_OBDH=GetOBDHstop(infileL2);

  std::cout << infileL2 << " " << start_OBDH << " " << stop_OBDH << std::endl;

  bfield = (BField*)calloc(1, sizeof(BField));
  BFieldModel bmodel;
  read_model(&bmodel, "/software/source/LimadouHEPD/OrbitReconstruction/IGRF/IGRF12.COF");
  
  DateTime now_tle = tle.Epoch();
  DateTime now = DateTime(now_tle.Year(),now_tle.Month(),now_tle.Day());


  double field_intensity=0.;
  double field_x=0.;
  double field_y=0.;
  double field_z=0.;
  double B_0_over_B =0.;
  double sin_over_cos =0.;
  double body =0.;
 
  double L_shell =0.; //in Earth radii

  double current = start;
  SGP4 model(tle);
  bool running = true;
  bool first_run = true;

  Double_t talt,tlat,tlon,tB_x,tB_y,tB_z,tB,tL,t_rig;
  UInt_t tabstime;

  f = new TFile(infileL2,"UPDATE");
  tree_out=new TTree("Orbital","Orbital");

  tree_out->Branch("abstime",&tabstime);
  tree_out->Branch("alt",&talt);
  tree_out->Branch("lat",&tlat);
  tree_out->Branch("lon",&tlon);
  tree_out->Branch("B_x",&tB_x);
  tree_out->Branch("B_y",&tB_y);
  tree_out->Branch("B_z",&tB_z);
  tree_out->Branch("B",&tB);
  tree_out->Branch("L",&tL);
  tree_out->Branch("rig",&t_rig);

  tree_out->Write("OrbitalInfo",TObject::kOverwrite);

  while (running)
    {

      bool error = false;
      Vector position;
      Vector velocity;
      double tsince;

      try
	{
	  if (first_run && current != 0.0) 
	    {
	      tsince = 0.0;
	    }
	  else
	    {                 
	      tsince = current;
	    }
	}

      catch (SatelliteException& e)
	{
	  std::cerr << e.what() << std::endl;
	  error = true;
	  running = false;
	}
      catch (DecayedException& e)
	{
	  std::cerr << e.what() << std::endl;

	  position = e.Position();
	  velocity = e.Velocity();

	  if (!first_run)
	    {
	      // print out position on first run
	      error = true;
	    }

	  running = false;
	}
        
      if (!error)
	{
	  DateTime now_incr = now.AddSeconds(tsince);
	  UInt_t now_incr_sec = GeoManagerClass::GetEpoch(now_incr.Year(),now_incr.Month(),now_incr.Day(),now_incr.Hour(),now_incr.Minute(),now_incr.Second());

	  Eci eci = model.FindPosition(now_incr);
	  CoordGeodetic geo = eci.ToGeodetic();

	  int  now_incr_sec_OBDH = now_incr_sec - OBDH_TO_EPOCH_OFFSET_SEC;

	  // mag field

	  double alt = geo.altitude; //Altitude in Kilometers (plus letter DE to be read in Geomag
	  double lat = geo.latitude; //Latitude
	  double lon = geo.longitude; //Longitude
	  const double date_r = julday(now_incr.Month(),now_incr.Day(),now_incr.Year());
	  get_field_components(bfield,&bmodel, alt, kUnitsKilometers,kCoordSysGeodetic,Util::RadiansToDegrees(geo.latitude),Util::RadiansToDegrees(geo.longitude),date_r);
	  field_intensity=bfield->f;
	  field_x=bfield->x;
	  field_y=bfield->y;
	  field_z=bfield->z;
	  //std::cout << " now = " << now << " tsince = " << tsince << " " << " now_incr_sec = " << now_incr_sec << " now_incr_sec_OBDH = " << now_incr_sec_OBDH << " alt = " << alt << " " << field_intensity << std::endl;
	    	  
	  B_0_over_B = (31200./field_intensity);
	  sin_over_cos = sqrt(1 + 3*pow(sin(lat),2))/pow(cos(lat),6);
	  body = B_0_over_B*sin_over_cos;
	  L_shell = cbrt(body); //in Earth radii
       

	  //////////////////////////////////  end mag field /////////////////////
	  talt=geo.altitude;
	  tlat= Util::RadiansToDegrees(geo.latitude);
	  tlon= Util::RadiansToDegrees(geo.longitude);
	  tL=L_shell;
	  tB=field_intensity;
	  tB_x=field_x;
	  tB_y=field_y;
	  tB_z=field_z;
	  tabstime=now_incr_sec_OBDH;
	  t_rig = 15.062*pow(L_shell,-2) -0.363;
	  if(tabstime>=start_OBDH && tabstime<=stop_OBDH)tree_out->Fill();

	 
	}

      //std::cout << "written " << std::endl;


      if ((first_run && current == 0.0) || !first_run)
	{

	  if (current == end)
	    {
	      running = false;
	    }
	  else if (current + inc > end)
	    {
	      current = end;
	    }
	  else
	    {
	      current += inc;
	    }
	}


      first_run = false;

    }

  f->cd();
  tree_out->Write("OrbitalInfo",TObject::kOverwrite);
  f->Close();
  //std::cout << " written to file " << std::endl;
}

/*
  void GeoManagerClass::Associate(const char* infileL2, const char* outfileL2){
  std::cout << " here" << std::endl;

  TFile* fout_out = NULL;
  //TTree* fTree = NULL;

  UInt_t n_abstime;
  Double_t rig, B, lon, lat, alt, L, n_rig, n_B, n_L, n_alt, n_lat, n_lon,B_x,B_y,B_z,n_B_x,n_B_y,n_B_z;

  UInt_t event_index,trigger_index,hepd_time,PMTBoard_trigger_counter,alive_time,dead_time;

  UShort_t orbitZone;
  UInt_t OBDH_info, abstime;

  TFile* fin = new TFile(infileL2,"read");

  std::cout << " here2" << std::endl;

  TTree* fTree = (TTree*)fin->Get("L2");
  std::cout << " here2.01" << std::endl;


  fTreeMD = (TTree*)fin->Get("L2md");
  std::cout << " here2.02" << std::endl;

  fTreeOrb = (TTree*)fin->Get("OrbitalInfo");
  std::cout << " here2.03" << std::endl;

  std::cout << " here2.1" << std::endl;


  fTree->SetBranchAddress("hepd_time",&hepd_time);
  fTreeMD->SetBranchAddress("orbitZone",&orbitZone);

  TBranch*OBDH_info_br = (TBranch*)fTreeMD->GetBranch("OBDH_info");
  TLeaf* leaf=(TLeaf*)OBDH_info_br->FindLeaf("sec_OBDH");
  Int_t sec_OBDH=*((Int_t*)leaf->GetValuePointer());

  fTreeMD->SetBranchAddress("OBDH_info",&OBDH_info);
  OBDH_info_br->SetAddress(&sec_OBDH);


  fTreeOrb->SetBranchAddress("rig",&rig);
  fTreeOrb->SetBranchAddress("abstime",&abstime);
  fTreeOrb->SetBranchAddress("B",&B);
  fTreeOrb->SetBranchAddress("lon",&lon);
  fTreeOrb->SetBranchAddress("lat",&lat);
  fTreeOrb->SetBranchAddress("L",&L);
  fTreeOrb->SetBranchAddress("alt",&alt);
  fTreeOrb->SetBranchAddress("B_x",&B_x);
  fTreeOrb->SetBranchAddress("B_y",&B_y);
  fTreeOrb->SetBranchAddress("B_z",&B_z);

  std::cout << " here2.2" << std::endl;


  TBranch *b_rig = fTree->Branch("n_rig",&n_rig);
  TBranch *b_abstime = fTree->Branch("n_abstime",&n_abstime);
  TBranch *b_B = fTree->Branch("n_B",&n_B);
  TBranch *b_lon = fTree->Branch("n_lon",&n_lon);
  TBranch *b_lat = fTree->Branch("n_lat",&n_lat);
  TBranch *b_alt = fTree->Branch("n_alt",&n_alt);
  TBranch *b_L  = fTree->Branch("n_L",&n_L);
  TBranch *b_B_x = fTree->Branch("n_B_x",&n_B_x);
  TBranch *b_B_y = fTree->Branch("n_B_y",&n_B_y);
  TBranch *b_B_z = fTree->Branch("n_B_z",&n_B_z);

  UInt_t nentriesT = fTree->GetEntries();
  UInt_t nentriesTlt = fTreeMD->GetEntries();
  UInt_t nentriesOrb = fTreeOrb->GetEntries();

  int counts =0;

  std::cout << " here3" << std::endl;


  for(UInt_t entry=0; entry<nentriesT; entry++) {
  fTree->GetEntry(entry);

  double time_s = hepd_time/100000;


  for(UInt_t entryorb=0; entryorb<nentriesOrb; entryorb++) {

  //  fTree->GetEntry(entry);
  fTreeOrb->GetEntry(entryorb);
  fTreeMD->GetEntry(0);

  //      double time_s = hepd_time/100000;
  UInt_t time = time_s+sec_OBDH;
   
  n_abstime = abstime;
  n_rig = rig;
  n_lat = lat;
  n_lon = lon;
  n_L= L;
  n_alt = alt;
  n_B = B;
  n_B_x = B_x;
  n_B_y = B_y;
  n_B_z = B_z;

  //cout << sec_OBDH << " " << time_s << " " << time << " " << n_abstime << endl;

     

  if(time!=abstime) continue;

  b_rig->Fill();
  b_abstime->Fill();
  b_B->Fill();
  b_lon->Fill();
  b_lat->Fill();
  b_alt->Fill();
  b_L->Fill();
  b_B_x->Fill();
  b_B_y->Fill();
  b_B_z->Fill();
   


  }
  }

  //fout_out = new TFile(outfileL2,"recreate");
  fTree->Write();
  fTreeMD->Write();
  //fout_out->Close();
  fin->Close();
  }
*/

void GeoManagerClass::tokenize(const std::string& str, std::vector<std::string>& tokens)
{
  const std::string& delimiters = " ";

  /*
   * skip delimiters at beginning
   */
  std::string::size_type last_pos = str.find_first_not_of(delimiters, 0);

  /*
   * find first non-delimiter
   */
  std::string::size_type pos = str.find_first_of(delimiters, last_pos);

  while (std::string::npos != pos || std::string::npos != last_pos)
    {
      /*
       * add found token to vector
       */
      tokens.push_back(str.substr(last_pos, pos - last_pos));
      /*
       * skip delimiters
       */
      last_pos = str.find_first_not_of(delimiters, pos);
      /*
       * find next non-delimiter
       */
      pos = str.find_first_of(delimiters, last_pos);
    }
};


void GeoManagerClass::PropagateTLE(const char* infileL2, const char* infile)
{

  std::ifstream file;
   
  file.open(infile);


  //TTree *tree_2a=new TTree("Orbital","Orbital");

  if (!file.is_open())
    {
      std::cerr << "Error opening file" << std::endl;
      return;
    }

  bool got_first_line = false;
  std::string line1;
  std::string line2;
  std::string parameters;

  while (!file.eof())
    {
      std::string line;
      std::getline(file, line);

      Util::Trim(line);

      /*
       * skip blank lines or lines starting with #
       */
      if (line.length() == 0 || line[0] == '#')
        {
	  got_first_line = false;
	  continue;
        }

      /*
       * find first line
       */
      if (!got_first_line)
        {
	  try
            {
	      if (line.length() >= Tle::LineLength())
                {
		  //Tle::IsValidLine(line.substr(0, Tle::LineLength()), 1);
		  /*
		   * store line and now read in second line
		   */
		  got_first_line = true;
		  line1 = line;
                }
            }
	  catch (TleException& e)
            {
	      std::cerr << "Error: " << e.what() << std::endl;
	      std::cerr << line << std::endl;
            }
        }
      else
        {
	  got_first_line = false;
        	    
	  line2 = line.substr(0, Tle::LineLength());
	  double start = 0.0; //now.DayOfYear(now.Year(),now.Month(),now.Day());
	  double end = 90000; //(double)GetOBDHstop(infileL2)-(double)GetOBDHstart(infileL2); //86400.; //now.DayOfYear(now.Year(),now.Month(),now.Day()) +1;
	  double inc = 1.; //1 second increment
        
	  if (line.length() > 69)
	    {
	      std::vector<std::string> tokens;
	      parameters = line.substr(Tle::LineLength() + 1,
				       line.length() - Tle::LineLength());
	      tokenize(parameters, tokens);
	      if (tokens.size() >= 3)
		{
		  start = atof(tokens[0].c_str());
		  end = atof(tokens[1].c_str());
		  inc = atof(tokens[2].c_str());
		}
	    }

	  /*
	   * following line must be the second line
	   */
	  try
            {
	      if (line.length() >= Tle::LineLength())
                {
		  Tle tle("Test", line1, line2);
		  RunTle(tle, start, end, inc, infile, infileL2);
                }
            }
	  catch (TleException& e)
            {
	      std::cerr << "Error: " << e.what() << std::endl;
	      std::cerr << line << std::endl;
            }
        }
    }
  /*
   * close file
   */
  file.close();

  return;
};


/*
  void GeoManagerClass::CreateTTreeROOT(const char* outROOTfile) {
 
  //TTree *tree_out=new TTree("Orbital","Orbital");

  std::ifstream inFile;
  inFile.open("HEPD_geo_coord_new.txt");

  Float_t alt,lat,lon,B_x,B_y,B_z,B,L;
  UInt_t abstime;
 
  std::string s;
  TString line;

  while(getline(inFile,s)) {
  line = s.c_str();
  TObjArray* tok = line.Tokenize(" ");

  TString sabstime = ((TObjString*)tok->At(0))->GetString();
  TString salt = ((TObjString*)tok->At(1))->GetString();
  TString slat = ((TObjString*)tok->At(2))->GetString();
  TString slon = ((TObjString*)tok->At(3))->GetString();
  TString sB = ((TObjString*)tok->At(4))->GetString();   
  TString sB_x = ((TObjString*)tok->At(5))->GetString();
  TString sB_y = ((TObjString*)tok->At(6))->GetString();
  TString sB_z = ((TObjString*)tok->At(7))->GetString();
  TString sL = ((TObjString*)tok->At(8))->GetString();


  alt = salt.Atof();
  lat = slat.Atof();
  lon = slon.Atof();
  B_x = sB_x.Atof();
  B_y = sB_y.Atof();
  B_z = sB_z.Atof();
  B = sB.Atof();
  L = sL.Atof();
  abstime = sabstime.Atoi();

  //tree_out->Fill();      

  }

  return;


  }; //end CreateTTreeROOT
*/




