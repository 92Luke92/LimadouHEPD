/**
 * =============================================================================
 *
 * Created by Alessandro Sotgiu, Francesco Palma e Simona Bartocci

 * FILENAME: housekeeping.cc
 *
 * DESCRIPTION:  Script to generate Housekeeping Quicklook xml files   
 *                 
 * DATE:           June 14, 2017
 *     
 *
 =============================================================================
**/

#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <TString.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "housekeeping.hh"
#include <TDatime.h>


void HVPSMonitorToXML(TString rootname, TString outPath, TString xslPath = "")
{

  LEvRec0File rootfile(rootname.Data());
  LEvRec0 ev;
  LEvRec0Md metaData;
  LEvRec0Conf dummyPKT;
  LEvRec0HVpmt HVpkt;
  rootfile.SetTheEventPointer(ev);
  rootfile.SetTmdPointer(metaData);
  

  if (rootfile.IsTConf()){
     
     rootfile.SetTHVpmtPointer(HVpkt);
     cout << endl << "Processing HVPS Monitor to xml file:" << rootname << endl;

  
     int THVpmt_entries = rootfile.GetTHVpmtEntries(); 
     // cout << "Number of THVpmt entries: " << THVpmt_entries << endl;
  
     TString filename = outPath;
     const char * _temp = rootname;
     filename += "/";
     filename += basename(_temp);
     filename.ReplaceAll(".root", 5, "_HVPSMonitor.xml", 16);

     ofstream outputFile;
     outputFile.open(filename.Data(), ios::trunc);


     if (!outputFile.is_open()){
	printf("Cannot open the file %s for the output", filename.Data());
	exit(0);
     }

     outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
     outputFile << "<!-- Prologo XML -->\n";
     outputFile << "<?xml-stylesheet type='text/xsl' href='" << xslPath.Data() << "'?>\n";
     outputFile << "<ROOT_SOURCE>\n";

     Double_t HV_pmt_mon_table[10];
     Double_t HV_sil_mon_table[2];
  
  
  for(int j=0;j<THVpmt_entries;j++)
    {
      
      rootfile.GetTHVpmtEntry(j);
      
      for(int h=0;h<10;h++)
	{
	  HV_pmt_mon_table[h]=(double)HVpkt.HV_pmt_mon[h]*0.24-0.78;
	}

      for(int l=0;l<2;l++)
	{
      	 HV_sil_mon_table[l]= (double)HVpkt.HV_sil_mon[l]*0.030-0.10;
	}

      outputFile << "<HVPSMONITOR>\n";
      
      outputFile << "\t<HEADER>"<< 0 << "</HEADER>\n";
      
      if(j%2==0) 
	outputFile << "\t<HEADER>" << 1 << "</HEADER>\n";

      outputFile << "\t<HEADER_VAL>" << "H" << "</HEADER_VAL>\n";
      outputFile << "\t<TAIL_VAL>" << "T" << "</TAIL_VAL>\n";
      
      outputFile << "\t<HV_MON_PLANE_A_error>"  <<  0 << "</HV_MON_PLANE_A_error>\n";
      outputFile << "\t<HV_MON_PLANE_B_error>"  <<  0 << "</HV_MON_PLANE_B_error>\n";
      outputFile << "\t<HV_MON_PMT0_error>"  <<  0 << "</HV_MON_PMT0_error>\n";
      outputFile << "\t<HV_MON_PMT1_error>"  <<  0 << "</HV_MON_PMT1_error>\n";
      outputFile << "\t<HV_MON_PMT2_error>"  <<  0 << "</HV_MON_PMT2_error>\n";
      outputFile << "\t<HV_MON_PMT3_error>"  <<  0 << "</HV_MON_PMT3_error>\n";
      outputFile << "\t<HV_MON_PMT4_error>"  <<  0 << "</HV_MON_PMT4_error>\n";
      outputFile << "\t<HV_MON_PMT5_error>"  <<  0 << "</HV_MON_PMT5_error>\n";
      outputFile << "\t<HV_MON_PMT6_error>"  <<  0 << "</HV_MON_PMT6_error>\n";
      outputFile << "\t<HV_MON_PMT7_error>"  <<  0 << "</HV_MON_PMT7_error>\n";
      outputFile << "\t<HV_MON_PMT8_error>"  <<  0 << "</HV_MON_PMT8_error>\n";
      outputFile << "\t<HV_MON_PMT9_error>"  <<  0 << "</HV_MON_PMT9_error>\n"; 
      
      outputFile << "\t<BOOT_NR>" << HVpkt.boot_nr << "</BOOT_NR>\n";
      outputFile << "\t<RUN_NR>"  << HVpkt.run_id  << "</RUN_NR>\n";

      if (HV_sil_mon_table[0]<0 || HV_sil_mon_table[0]>100)
	outputFile << "\t<HV_MON_PLANE_A_error>"  <<  1   << "</HV_MON_PLANE_A_error>\n";

      if (HV_sil_mon_table[1]<0 || HV_sil_mon_table[1]>100)
	outputFile << "\t<HV_MON_PLANE_B_error>"  <<  1  << "</HV_MON_PLANE_B_error>\n";

      if (HV_pmt_mon_table[0]<0 || HV_pmt_mon_table[0]>980)
	outputFile << "\t<HV_MON_PMT0_error>"  <<  1 << "</HV_MON_PMT0_error>\n";

      if (HV_pmt_mon_table[1]<0 || HV_pmt_mon_table[1]>980)
	outputFile << "\t<HV_MON_PMT1_error>"  <<  1 << "</HV_MON_PMT1_error>\n";

      if (HV_pmt_mon_table[2]<0 || HV_pmt_mon_table[2]>980)
	outputFile << "\t<HV_MON_PMT2_error>"  <<  1 << "</HV_MON_PMT2_error>\n";

      if (HV_pmt_mon_table[3]<0 || HV_pmt_mon_table[3]>980)
	outputFile << "\t<HV_MON_PMT3_error>"  <<  1 << "</HV_MON_PMT3_error>\n";

      if (HV_pmt_mon_table[4]<0 || HV_pmt_mon_table[4]>980)
	outputFile << "\t<HV_MON_PMT4_error>"  <<  1 << "</HV_MON_PMT4_error>\n";

      if (HV_pmt_mon_table[5]<0 || HV_pmt_mon_table[5]>980)
	outputFile << "\t<HV_MON_PMT5_error>"  <<  1 << "</HV_MON_PMT5_error>\n";

      if (HV_pmt_mon_table[6]<0 || HV_pmt_mon_table[6]>980)
	outputFile << "\t<HV_MON_PMT6_error>"  <<  1 << "</HV_MON_PMT6_error>\n";

      if (HV_pmt_mon_table[7]<0 || HV_pmt_mon_table[7]>980)
	outputFile << "\t<HV_MON_PMT7_error>"  <<  1 << "</HV_MON_PMT7_error>\n";

      if (HV_pmt_mon_table[8]<0 || HV_pmt_mon_table[8]>980)
	outputFile << "\t<HV_MON_PMT8_error>"  <<  1 << "</HV_MON_PMT8_error>\n";

      if (HV_pmt_mon_table[9]<0 || HV_pmt_mon_table[9]>980)
	outputFile << "\t<HV_MON_PMT9_error>"  <<  1 << "</HV_MON_PMT9_error>\n";

      
      outputFile << setprecision(3) << "\t<HV_MON_PLANE_A>"  << HV_sil_mon_table[0] << "</HV_MON_PLANE_A>\n";
      outputFile << setprecision(3) << "\t<HV_MON_PLANE_B>"  << HV_sil_mon_table[1] << "</HV_MON_PLANE_B>\n";

      outputFile << setprecision(4) << "\t<HV_MON_PMT0>" <<  HV_pmt_mon_table[0] << "</HV_MON_PMT0>\n";
      outputFile << setprecision(4) << "\t<HV_MON_PMT1>" <<  HV_pmt_mon_table[1] << "</HV_MON_PMT1>\n";
      outputFile << setprecision(4) << "\t<HV_MON_PMT2>" <<  HV_pmt_mon_table[2] << "</HV_MON_PMT2>\n";
      outputFile << setprecision(4) << "\t<HV_MON_PMT3>" <<  HV_pmt_mon_table[3] << "</HV_MON_PMT3>\n";
      outputFile << setprecision(4) << "\t<HV_MON_PMT4>" <<  HV_pmt_mon_table[4] << "</HV_MON_PMT4>\n";
      outputFile << setprecision(4) << "\t<HV_MON_PMT5>" <<  HV_pmt_mon_table[5] << "</HV_MON_PMT5>\n";
      outputFile << setprecision(4) << "\t<HV_MON_PMT6>" <<  HV_pmt_mon_table[6] << "</HV_MON_PMT6>\n";
      outputFile << setprecision(4) << "\t<HV_MON_PMT7>" <<  HV_pmt_mon_table[7] << "</HV_MON_PMT7>\n";
      outputFile << setprecision(4) << "\t<HV_MON_PMT8>" <<  HV_pmt_mon_table[8] << "</HV_MON_PMT8>\n";
      outputFile << setprecision(4) << "\t<HV_MON_PMT9>" <<  HV_pmt_mon_table[9] << "</HV_MON_PMT9>\n";

      outputFile << "</HVPSMONITOR>\n";

    }
  outputFile << "</ROOT_SOURCE>\n";
  outputFile.close();

    }
    else
    return;
}


void DUMPConfigToXML(TString rootname, TString outPath, TString xslPath = "")
{
  

  LEvRec0File rootfile(rootname.Data());
  LEvRec0 ev;
  LEvRec0Md metaData;
  LEvRec0Conf dummyPKT;
  rootfile.SetTheEventPointer(ev);
  rootfile.SetTmdPointer(metaData);


  if (rootfile.IsTConf()){
     
     rootfile.SetTConfPointer(dummyPKT);
     cout << endl << "Processing DUMP Config to xml file:" << rootname << endl;

  
     int TConf_entries = rootfile.GetTConfEntries(); 
     //cout << "Number of TConf entries: " << TConf_entries << endl;
  
     TString filename = outPath;
     const char * _temp = rootname;
     filename += "/";
     filename += basename(_temp);
     filename.ReplaceAll(".root", 5, "_DUMPConfig.xml", 15);

     ofstream outputFile;
     outputFile.open(filename.Data(), ios::trunc);


     if (!outputFile.is_open()){
	printf("Cannot open the file %s for the output", filename.Data());
	exit(0);
     }

     outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
     outputFile << "<!-- Prologo XML -->\n";
     outputFile << "<?xml-stylesheet type='text/xsl' href='" << xslPath.Data() << "'?>\n";
     outputFile << "<ROOT_SOURCE>\n";
  


     for(int j=0;j<TConf_entries;j++) {

	rootfile.GetTConfEntry(j);

     
	outputFile << "\t<INDEX>"<< 0 << "</INDEX>\n";  
	outputFile << "\t<INDEX_2>"<< 0 << "</INDEX_2>\n";
    
	for(int i=0; i<5; ++i) {
      
	   outputFile << "<DUMP_CONFIG>\n";
      
	   outputFile << "\t<BOOT_NR>" << dummyPKT.dummy_pkt.boot_nr << "</BOOT_NR>\n";
	   outputFile << "\t<RUN_NR>"  << dummyPKT.dummy_pkt.run_id  << "</RUN_NR>\n";
      
	   if (i==0) {      
	      outputFile << "\t<ORBIT_CONF>"    << "SAA"  << "</ORBIT_CONF>\n";
	      outputFile << "\t<LADDER_MASK_TOP>"    << dummyPKT.dummy_pkt.orbit_conf[0].ladder_mask[0] << "</LADDER_MASK_TOP>\n";
	      outputFile << "\t<LADDER_MASK_CENTR>"    << dummyPKT.dummy_pkt.orbit_conf[0].ladder_mask[1] << "</LADDER_MASK_CENTR>\n";
	      outputFile << "\t<LADDER_MASK_BOTT>"    << dummyPKT.dummy_pkt.orbit_conf[0].ladder_mask[2] << "</LADDER_MASK_BOTT>\n";
	      outputFile << "\t<TRIGGER_MASK_VETO>"    << (short)dummyPKT.dummy_pkt.orbit_conf[0].trigger_mask[0] << "</TRIGGER_MASK_VETO>\n";
	      outputFile << "\t<TRIGGER_MASK>"    << (short)dummyPKT.dummy_pkt.orbit_conf[0].trigger_mask[1] << "</TRIGGER_MASK>\n";
	      outputFile << "\t<RUN_DURATION>"    << dummyPKT.dummy_pkt.orbit_conf[0].run_duration << "</RUN_DURATION>\n";
	      outputFile << "\t<LATITUDE_RANGE>"<< "[" <<(double)dummyPKT.dummy_pkt.orbit_conf[0].lat1*pow(10,-7)<< ";" << (double)dummyPKT.dummy_pkt.orbit_conf[0].lat2*pow(10,-7) <<"]" << "</LATITUDE_RANGE>\n";
	      outputFile << "\t<LONGITUDE_RANGE>"<< "[" <<(double)dummyPKT.dummy_pkt.orbit_conf[0].long1*pow(10,-7)<< ";" << (double)dummyPKT.dummy_pkt.orbit_conf[0].long2*pow(10,-7) << "]" << "</LONGITUDE_RANGE>\n";

	      outputFile << "\t<INDEX_2>"<< 1 << "</INDEX_2>\n";
	   }
      
	   if (i==1) {       
	      outputFile << "\t<ORBIT_CONF>"    << "EQUATORIAL"  << "</ORBIT_CONF>\n";
	      outputFile << "\t<LADDER_MASK_TOP>"    << dummyPKT.dummy_pkt.orbit_conf[1].ladder_mask[0] << "</LADDER_MASK_TOP>\n";
	      outputFile << "\t<LADDER_MASK_CENTR>"    << dummyPKT.dummy_pkt.orbit_conf[1].ladder_mask[1] << "</LADDER_MASK_CENTR>\n";
	      outputFile << "\t<LADDER_MASK_BOTT>"    << dummyPKT.dummy_pkt.orbit_conf[1].ladder_mask[2] << "</LADDER_MASK_BOTT>\n";
	      outputFile << "\t<TRIGGER_MASK_VETO>"    << (short)dummyPKT.dummy_pkt.orbit_conf[1].trigger_mask[0] << "</TRIGGER_MASK_VETO>\n";
	      outputFile << "\t<TRIGGER_MASK>"    << (short)dummyPKT.dummy_pkt.orbit_conf[1].trigger_mask[1] << "</TRIGGER_MASK>\n";
	      outputFile << "\t<RUN_DURATION>"    << dummyPKT.dummy_pkt.orbit_conf[1].run_duration << "</RUN_DURATION>\n";
	      outputFile << "\t<LATITUDE_RANGE>"<< "[" <<(double)dummyPKT.dummy_pkt.orbit_conf[1].lat1*pow(10,-7)<< ";"<< (double)dummyPKT.dummy_pkt.orbit_conf[1].lat2*pow(10,-7)<< "]" << "</LATITUDE_RANGE>\n";
	      outputFile << "\t<LONGITUDE_RANGE>"<< "[" <<(double)dummyPKT.dummy_pkt.orbit_conf[1].long1*pow(10,-7) <<";"<< (double)dummyPKT.dummy_pkt.orbit_conf[1].long2*pow(10,-7) <<"]" << "</LONGITUDE_RANGE>\n";
	   }
        
	   if (i==2) {       
	      outputFile << "\t<ORBIT_CONF>"    << "SOUTH POLAR"  << "</ORBIT_CONF>\n";
	      outputFile << "\t<LADDER_MASK_TOP>"    << dummyPKT.dummy_pkt.orbit_conf[2].ladder_mask[0] << "</LADDER_MASK_TOP>\n";
	      outputFile << "\t<LADDER_MASK_CENTR>"    << dummyPKT.dummy_pkt.orbit_conf[2].ladder_mask[1] << "</LADDER_MASK_CENTR>\n";
	      outputFile << "\t<LADDER_MASK_BOTT>"    << dummyPKT.dummy_pkt.orbit_conf[2].ladder_mask[2] << "</LADDER_MASK_BOTT>\n";
	      outputFile << "\t<TRIGGER_MASK_VETO>"    << (short)dummyPKT.dummy_pkt.orbit_conf[2].trigger_mask[0] << "</TRIGGER_MASK_VETO>\n";
	      outputFile << "\t<TRIGGER_MASK>"    << (short)dummyPKT.dummy_pkt.orbit_conf[2].trigger_mask[1] << "</TRIGGER_MASK>\n";
	      outputFile << "\t<RUN_DURATION>"    << dummyPKT.dummy_pkt.orbit_conf[2].run_duration << "</RUN_DURATION>\n";
	      outputFile << "\t<LATITUDE_RANGE>"<< "[" <<(double)dummyPKT.dummy_pkt.orbit_conf[2].lat1*pow(10,-7)<< ";"<< (double)dummyPKT.dummy_pkt.orbit_conf[2].lat2*pow(10,-7) << "]" << "</LATITUDE_RANGE>\n";
	      outputFile << "\t<LONGITUDE_RANGE>"<< "[" <<(double)dummyPKT.dummy_pkt.orbit_conf[2].long1*pow(10,-7)<< ";"<< (double)dummyPKT.dummy_pkt.orbit_conf[2].long2*pow(10,-7) << "]" << "</LONGITUDE_RANGE>\n";
	   }

	   if (i==3) {    
	      outputFile << "\t<ORBIT_CONF>"    << "NORTH POLAR"  << "</ORBIT_CONF>\n";
	      outputFile << "\t<LADDER_MASK_TOP>"    << dummyPKT.dummy_pkt.orbit_conf[3].ladder_mask[0] << "</LADDER_MASK_TOP>\n";
	      outputFile << "\t<LADDER_MASK_CENTR>"    << dummyPKT.dummy_pkt.orbit_conf[3].ladder_mask[1] << "</LADDER_MASK_CENTR>\n";
	      outputFile << "\t<LADDER_MASK_BOTT>"    << dummyPKT.dummy_pkt.orbit_conf[3].ladder_mask[2] << "</LADDER_MASK_BOTT>\n";
	      outputFile << "\t<TRIGGER_MASK_VETO>"    << (short)dummyPKT.dummy_pkt.orbit_conf[3].trigger_mask[0] << "</TRIGGER_MASK_VETO>\n";
	      outputFile << "\t<TRIGGER_MASK>"    << (short)dummyPKT.dummy_pkt.orbit_conf[3].trigger_mask[1] << "</TRIGGER_MASK>\n";
	      outputFile << "\t<RUN_DURATION>"    << dummyPKT.dummy_pkt.orbit_conf[3].run_duration << "</RUN_DURATION>\n";
	      outputFile << "\t<LATITUDE_RANGE>"<< "["<< (double)dummyPKT.dummy_pkt.orbit_conf[3].lat1*pow(10,-7)<< ";"<<(double)dummyPKT.dummy_pkt.orbit_conf[3].lat2*pow(10,-7)<< "]" << "</LATITUDE_RANGE>\n";
	      outputFile << "\t<LONGITUDE_RANGE>"<< "[" <<(double)dummyPKT.dummy_pkt.orbit_conf[3].long1*pow(10,-7) <<";" <<(double)dummyPKT.dummy_pkt.orbit_conf[3].long2*pow(10,-7)<< "]" << "</LONGITUDE_RANGE>\n";
	   }

	   if (i==4) {       
	      outputFile << "\t<ORBIT_CONF>"    << "DEFAULT"  << "</ORBIT_CONF>\n";
	      outputFile << "\t<LADDER_MASK_TOP>"    << dummyPKT.dummy_pkt.orbit_conf[4].ladder_mask[0] << "</LADDER_MASK_TOP>\n";
	      outputFile << "\t<LADDER_MASK_CENTR>"    << dummyPKT.dummy_pkt.orbit_conf[4].ladder_mask[1] << "</LADDER_MASK_CENTR>\n";
	      outputFile << "\t<LADDER_MASK_BOTT>"    << dummyPKT.dummy_pkt.orbit_conf[4].ladder_mask[2] << "</LADDER_MASK_BOTT>\n";
	      outputFile << "\t<TRIGGER_MASK_VETO>"    << (short)dummyPKT.dummy_pkt.orbit_conf[4].trigger_mask[0] << "</TRIGGER_MASK_VETO>\n";
	      outputFile << "\t<TRIGGER_MASK>"    << (short)dummyPKT.dummy_pkt.orbit_conf[4].trigger_mask[1] << "</TRIGGER_MASK>\n";
	      outputFile << "\t<RUN_DURATION>"    << dummyPKT.dummy_pkt.orbit_conf[4].run_duration << "</RUN_DURATION>\n";
	      outputFile << "\t<LATITUDE_RANGE>"<< "[" <<(double)dummyPKT.dummy_pkt.orbit_conf[4].lat1*pow(10,-7) <<";"<<(double)dummyPKT.dummy_pkt.orbit_conf[4].lat2*pow(10,-7) <<"]" << "</LATITUDE_RANGE>\n";
	      outputFile << "\t<LONGITUDE_RANGE>"<< "[" <<(double)dummyPKT.dummy_pkt.orbit_conf[4].long1*pow(10,-7) <<";" <<(double)dummyPKT.dummy_pkt.orbit_conf[4].long2*pow(10,-7) <<"]" << "</LONGITUDE_RANGE>\n";
      
	      outputFile << "\t<INDEX>"<< 1 << "</INDEX>\n";

	      outputFile << "\t<ORBITAL_SETTINGS>"    << dummyPKT.dummy_pkt.user_orbital_settings << "</ORBITAL_SETTINGS>\n";
    	      outputFile << showbase << hex << uppercase <<  "\t<CONFIG_ID>"    <<  dummyPKT.dummy_pkt.WO_config_ID << "</CONFIG_ID>\n";
	      outputFile << dec << "\t<CALIB_PERIOD>"    << dummyPKT.dummy_pkt.calib_period << "</CALIB_PERIOD>\n";
	      outputFile << dec << "\t<SAFE_MODE>" << dummyPKT.dummy_pkt.safe_mode << "</SAFE_MODE>\n";
 
	   }
	   outputFile << "</DUMP_CONFIG>\n"; 
	}
  

     }
     outputFile << "</ROOT_SOURCE>\n"; 
     outputFile.close();
  
  }  
  else
    return;
  
 
  }



void BroadcastToXML(TString rootname, TString outPath, TString xslPath= "", TString xslPath2= "")
{
  
  cout << endl << "Processing Broadcast to xml files:" << rootname << endl;

  TString filename = outPath;
  const char * _temp = rootname;
  filename += "/";
  filename += basename(_temp);
  filename.ReplaceAll(".root", 5, "_Time.xml",9 );

  ofstream outputFile;
  outputFile.open(filename.Data(), ios::trunc);
  if (!outputFile.is_open()){
    printf("Cannot open the file %s for the output", filename.Data());
    exit(0);
  }

  TString filename2 = outPath;
  filename2 += "/";
  filename2 += basename(_temp);
  filename2.ReplaceAll(".root", 5, "_GPS.xml",8 );

  ofstream outputFile2;
  outputFile2.open(filename2.Data(), ios::trunc); 
  if (!outputFile2.is_open()){
    printf("Cannot open the file2 %s for the output", filename2.Data());
    exit(0);
  }
 
   
  LEvRec0File rootfile(rootname.Data());
  LEvRec0 ev;
  LEvRec0Md metaData;
  rootfile.SetTheEventPointer(ev);
  rootfile.SetTmdPointer(metaData);
   
  outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
  outputFile << "<!-- Prologo XML -->\n";
  outputFile << "<?xml-stylesheet type='text/xsl' href='" << xslPath.Data() << "'?>\n";
  outputFile << "<ROOT_SOURCE>\n";

  outputFile2 << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
  outputFile2 << "<!-- Prologo XML -->\n";
  outputFile2 << "<?xml-stylesheet type='text/xsl' href='" << xslPath2.Data() << "'?>\n";
  outputFile2 << "<ROOT_SOURCE>\n"; 
  
  // Metadata
  int Tmd_entries = rootfile.GetTmdEntries(); 
  //cout << "Number of Tmd entries: " << Tmd_entries << endl;

  UInt_t OBDH_ms_vect[Tmd_entries];
  UInt_t  OBDH_sec_vect[Tmd_entries];
  UInt_t OBDH_timestamp_vect[Tmd_entries];
  UInt_t CPU_Time[Tmd_entries];

  for(int j=0;j<Tmd_entries;j++)
    {
      rootfile.GetTmdEntry(j);
       
      OBDH_ms_vect[j] = metaData.broadcast.OBDH.ms;
      OBDH_sec_vect[j]=  metaData.broadcast.OBDH.sec;
      OBDH_timestamp_vect[j] = metaData.timestamp.OBDH;
      

      if (j%2!=0) {
	CPU_Time[j-1]=metaData.CPU_time[0];
	CPU_Time[j]=metaData.CPU_time[1];
       }

    }


  for(int t=0;t<Tmd_entries;t++)
    {
      rootfile.GetTmdEntry(t);
      
      outputFile << "<BROADCAST>\n";
      outputFile2 << "<BROADCAST2>\n";

      outputFile << "\t<BROADCAST_OBDH_SEC_Y>"  <<  0 << "</BROADCAST_OBDH_SEC_Y>\n";
      outputFile << "\t<BROADCAST_OBDH_MS_Y>"   <<  0 << "</BROADCAST_OBDH_MS_Y>\n";
      outputFile << "\t<TIMESTAMP_OBDH_Y>"  <<  0 << "</TIMESTAMP_OBDH_Y>\n";
      outputFile << "\t<ABS_TIME_Y>"   <<  0 << "</ABS_TIME_Y>\n";
      outputFile << "\t<AOCC_S_Y>"   <<  0 << "</AOCC_S_Y>\n";
      outputFile << "\t<AOCC_US_Y>"   <<  0 << "</AOCC_US_Y>\n";
      outputFile << "\t<AOCC_Timestamp_Y>"   <<  0 << "</AOCC_Timestamp_Y>\n";
      outputFile2 << "\t<GPS_broadcast_Y>"   <<  0 << "</GPS_broadcast_Y>\n";
      outputFile2 << "\t<GPS_timestamp_Y>"   <<  0 << "</GPS_timestamp_Y>\n";

      outputFile << "\t<LATITUDE_error>"   <<  0 << "</LATITUDE_error>\n";
      outputFile << "\t<LONGITUDE_error>"   <<  0 << "</LONGITUDE_error>\n";
      
      outputFile << "\t<HEADER>"<< 0 << "</HEADER>\n";
      outputFile2 << "\t<HEADER>"<< 0 << "</HEADER>\n";

      if(t%2==0)
	outputFile << "\t<HEADER>" << 1 << "</HEADER>\n";

      outputFile << "\t<HEADER_VAL>" << "H" << "</HEADER_VAL>\n";
      outputFile << "\t<TAIL_VAL>" << "T" << "</TAIL_VAL>\n";

      if(t%2==0) 
	outputFile2 << "\t<HEADER>" << 1 << "</HEADER>\n";

      outputFile2 << "\t<HEADER_VAL>" << "H" << "</HEADER_VAL>\n";
      outputFile2 << "\t<TAIL_VAL>" << "T" << "</TAIL_VAL>\n";

      double longitude = (double)(metaData.broadcast.GPS.lon)*pow(10,-7);
      double latitude  = (double)(metaData.broadcast.GPS.lat)*pow(10,-7);

      if(latitude > 90 && latitude < -90)
	 outputFile << "\t<LATITUDE_error>"   <<  1 << "</LATITUDE_error>\n";

      if(longitude > 180 && longitude < -180)
	 outputFile << "\t<LONGITUDE_error>"   <<  0 << "</LONGITUDE_error>\n";
      
      UShort_t ABS_diff_sec[Tmd_entries];
      UShort_t ABS_Time_Run_ms[Tmd_entries];
      UShort_t ABS_Time_Run_sec[Tmd_entries];


      if (t%2==0) {
       	ABS_diff_sec[t]=(CPU_Time[t]-OBDH_timestamp_vect[t]+OBDH_ms_vect[t])/1000;
       	ABS_Time_Run_ms[t]=(CPU_Time[t]-OBDH_timestamp_vect[t]+OBDH_ms_vect[t])%1000;  	
       	ABS_Time_Run_sec[t]=OBDH_sec_vect[t]+ABS_diff_sec[t];
      }

      if (t%2!=0) {
       	ABS_diff_sec[t]=(CPU_Time[t]-OBDH_timestamp_vect[t]+OBDH_ms_vect[t])/1000;
       	ABS_Time_Run_ms[t]=(CPU_Time[t]-OBDH_timestamp_vect[t]+OBDH_ms_vect[t])%1000;  	
       	ABS_Time_Run_sec[t]=OBDH_sec_vect[t]+ABS_diff_sec[t];
      }
      
      TDatime h;
      TDatime GPS;
      TDatime AOCC;
      TDatime absolute;


      Int_t timestamp_OBDH_2009;
      Int_t timestamp_GPS_2009;
      Int_t timestamp_AOCC_2009;
      Int_t timestamp_ABS_2009;
      

      timestamp_OBDH_2009 = 1230764400+metaData.broadcast.OBDH.sec;
      timestamp_GPS_2009 = 1230764400+metaData.broadcast.GPS.sec;
      timestamp_AOCC_2009 = 1230764400+metaData.broadcast.AOCC.sec;
      timestamp_ABS_2009 = 1230764400+ABS_Time_Run_sec[t];
      
     	
      h.Set(timestamp_OBDH_2009);
      GPS.Set(timestamp_GPS_2009);
      AOCC.Set(timestamp_AOCC_2009);
      absolute.Set(timestamp_ABS_2009);
    
    
      
      if (metaData.broadcast.OBDH.sec == 0xBBBBBBBB){
	outputFile << "\t<OBDH_S>" << "N.A." << "</OBDH_S>\n";
	outputFile << "\t<ABS_TIME>" <<  "N.A." << "</ABS_TIME>\n";
	outputFile << "\t<ABS_TIME_ms>" << "N.A."       << "</ABS_TIME_ms>\n";
	outputFile << "\t<BROADCAST_OBDH_SEC_Y>"  <<  1 << "</BROADCAST_OBDH_SEC_Y>\n";
	outputFile << "\t<ABS_TIME_Y>"   <<  1 << "</ABS_TIME_Y>\n";
      }
            
      if (metaData.broadcast.OBDH.ms == 0xBBBB){
	outputFile << "\t<OBDH_MS>" << "N.A." << "</OBDH_MS>\n";
	outputFile << "\t<ABS_TIME>" <<  "N.A." << "</ABS_TIME>\n";
	outputFile << "\t<ABS_TIME_ms>" << "N.A."       << "</ABS_TIME_ms>\n";
	outputFile << "\t<BROADCAST_OBDH_MS_Y>"   <<  1 << "</BROADCAST_OBDH_MS_Y>\n";
	outputFile << "\t<ABS_TIME_Y>"   <<  1 << "</ABS_TIME_Y>\n";
      }

      if (metaData.timestamp.OBDH == 0xCCCCCCCC){
	outputFile << "\t<TIMESTAMP_OBDH>" << "N.A." << "</TIMESTAMP_OBDH>\n";
	outputFile << "\t<TIMESTAMP_OBDH_Y>"  <<  1 << "</TIMESTAMP_OBDH_Y>\n";
	outputFile << "\t<ABS_TIME>" <<  "N.A." << "</ABS_TIME>\n";
	outputFile << "\t<ABS_TIME_ms>" << "N.A."       << "</ABS_TIME_ms>\n";
	outputFile << "\t<ABS_TIME_Y>"   <<  1 << "</ABS_TIME_Y>\n";
	outputFile << "\t<BROADCAST_OBDH_MS_Y>"   <<  1 << "</BROADCAST_OBDH_MS_Y>\n";
      }
      
      if (metaData.broadcast.AOCC.sec == 0xBBBBBBBB){
	outputFile << "\t<AOCC_S>" << "N.A." << "</AOCC_S>\n";
      	outputFile << "\t<AOCC_S_Y>"   <<  1 << "</AOCC_S_Y>\n";
        }
      
      if (metaData.broadcast.AOCC.us == 0xBBBBBBBB ){
	outputFile << "\t<AOCC_US>" << "N.A." << "</AOCC_US>\n";
	outputFile << "\t<AOCC_US_Y>"   <<  1 << "</AOCC_US_Y>\n";
      }
      if (metaData.timestamp.AOCC == 0xCCCCCCCC){
	outputFile << "\t<TIMESTAMP_AOCC>" << "N.A." << "</TIMESTAMP_AOCC>\n";
	outputFile << "\t<AOCC_Timestamp_Y>"   <<  1 << "</AOCC_Timestamp_Y>\n";
      }
      if (metaData.broadcast.GPS.sec == 0xBBBBBBBB){
	outputFile2 << "\t<GPS_S>" << "N.A." << "</GPS_S>\n";
	outputFile2 << "\t<GPS_broadcast_Y>"   <<  1 << "</GPS_broadcast_Y>\n";
      }
      if (metaData.timestamp.GPS == 0xCCCCCCCC){
	outputFile2 << "\t<TIMESTAMP_GPS>" << "N.A." << "</TIMESTAMP_GPS>\n";
	outputFile2 << "\t<GPS_timestamp_Y>"   <<  1 << "</GPS_timestamp_Y>\n";
      }
	  
      outputFile << "\t<BOOT_NR>" << metaData.boot_nr << "</BOOT_NR>\n";
      outputFile << "\t<RUN_NR>"  << metaData.run_id   << "</RUN_NR>\n";
      outputFile << "\t<OBDH_S>" << h.AsSQLString() << "</OBDH_S>\n";
      outputFile <<  "\t<OBDH_MS>"  << metaData.broadcast.OBDH.ms  << "</OBDH_MS>\n";    
      outputFile << "\t<TIMESTAMP_OBDH>"<< metaData.timestamp.OBDH<< "</TIMESTAMP_OBDH>\n";
      outputFile << "\t<AOCC_S>" << AOCC.AsSQLString() << "</AOCC_S>\n";
      outputFile <<  "\t<AOCC_US>"  << metaData.broadcast.AOCC.us*10  << "</AOCC_US>\n";    
      outputFile << "\t<TIMESTAMP_AOCC>"<< metaData.timestamp.AOCC << "</TIMESTAMP_AOCC>\n";
      
      if (t%2==0)
	outputFile << "\t<TIME_RUN>" <<  "Start Run" << "</TIME_RUN>\n";


      if (t%2!=0)
	outputFile << "\t<TIME_RUN>" <<  "Stop Run" << "</TIME_RUN>\n";

      outputFile << "\t<ABS_TIME>" <<  absolute.AsSQLString() << "</ABS_TIME>\n";
      outputFile << "\t<ABS_TIME_ms>" << ABS_Time_Run_ms[t]       << "</ABS_TIME_ms>\n";
      outputFile << "\t<CPU_TIME>" << CPU_Time[t] << "</CPU_TIME>\n";
      
        
      outputFile2 << "\t<BOOT_NR>" << metaData.boot_nr << "</BOOT_NR>\n";
      outputFile2 << "\t<RUN_NR>"  << metaData.run_id  << "</RUN_NR>\n";
      outputFile2 << "\t<GPS_S>" << GPS.AsSQLString() << "</GPS_S>\n";
      outputFile2 << "\t<TIMESTAMP_GPS>"<< metaData.timestamp.GPS << "</TIMESTAMP_GPS>\n";
      outputFile2 << "\t<GPS_POS_X>" << metaData.broadcast.GPS.axis_pos[0]*0.1 << "</GPS_POS_X>\n";
      outputFile2 << "\t<GPS_POS_Y>" << metaData.broadcast.GPS.axis_pos[1]*0.1 << "</GPS_POS_Y>\n";
      outputFile2 << "\t<GPS_POS_Z>" << metaData.broadcast.GPS.axis_pos[2]*0.1 << "</GPS_POS_Z>\n";
      outputFile2 << "\t<GPS_VEL_X>" << metaData.broadcast.GPS.axis_vel[0]*0.01 << "</GPS_VEL_X>\n";
      outputFile2 << "\t<GPS_VEL_Y>" << metaData.broadcast.GPS.axis_vel[1]*0.01 << "</GPS_VEL_Y>\n";
      outputFile2 << "\t<GPS_VEL_Z>" << metaData.broadcast.GPS.axis_vel[2]*0.01 << "</GPS_VEL_Z>\n";
      outputFile2 << "\t<LONGITUDE>" << longitude << "</LONGITUDE>\n";
      outputFile2 << "\t<LATITUDE>"  << latitude  << "</LATITUDE>\n";
      outputFile << "</BROADCAST>\n";
      outputFile2 << "</BROADCAST2>\n"; 
  
    }
  outputFile << "</ROOT_SOURCE>\n";
  outputFile2 << "</ROOT_SOURCE>\n";
  outputFile.close();
  outputFile2.close();
}



void CPUTimeTempToXML(TString rootname, TString outPath, TString xslPath = "")
{
  
  cout << endl << "Processing CPU Time to xml file:" << rootname << endl;

  TString filename = outPath;
  const char * _temp = rootname;
  filename += "/";
  filename += basename(_temp);
  filename.ReplaceAll(".root", 5, "_CPUTimeTemp.xml", 16);

  ofstream outputFile;
  outputFile.open(filename.Data(), ios::trunc);
  if (!outputFile.is_open()){
    printf("Cannot open the file %s for the output", filename.Data());
    exit(0);
  }

  Short_t CPU_temp_start;
  Short_t CPU_temp_stop;
  Short_t PMT_temp_start;
  Short_t PMT_temp_stop;
  UInt_t REAL_run_duration;
  LEvRec0File rootfile(rootname.Data());
  LEvRec0 ev;
  LEvRec0Md metaData;
  rootfile.SetTheEventPointer(ev);
  rootfile.SetTmdPointer(metaData);
   
  
  outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
  outputFile << "<!-- Prologo XML -->\n";
  outputFile << "<?xml-stylesheet type='text/xsl' href='" << xslPath.Data() << "'?>\n";
  outputFile << "<ROOT_SOURCE>\n";

  // Metadata
  int Tmd_entries = rootfile.GetTmdEntries(); 
  //cout << "Number of Tmd entries: " << Tmd_entries << endl;

  for(int j=0;j<Tmd_entries;j++)
    {
      rootfile.GetTmdEntry(j);   
       
      if(j%2!=0)
	{
	
	  outputFile << "<CPUTIMETEMP>\n";

	  outputFile << "\t<CPU_TEMP_START_Y>"  <<  0 << "</CPU_TEMP_START_Y>\n";
	  outputFile << "\t<CPU_TEMP_STOP_Y>"   <<  0 << "</CPU_TEMP_STOP_Y>\n";
	  outputFile << "\t<PMT_TEMP_START_Y>"  <<  0 << "</PMT_TEMP_START_Y>\n";
	  outputFile << "\t<PMT_TEMP_STOP_Y>"   <<  0 << "</PMT_TEMP_STOP_Y>\n";
	  outputFile << "\t<CPU_TEMP_START_R>"  <<  0 << "</CPU_TEMP_START_R>\n";
	  outputFile << "\t<CPU_TEMP_STOP_R>"   <<  0 << "</CPU_TEMP_STOP_R>\n";
	  outputFile << "\t<PMT_TEMP_START_R>"  <<  0 << "</PMT_TEMP_START_R>\n";
	  outputFile << "\t<PMT_TEMP_STOP_R>"   <<  0 << "</PMT_TEMP_STOP_R>\n";
	 
	  outputFile << dec <<  "\t<BOOT_NR>"   <<  metaData.boot_nr << "</BOOT_NR>\n";
	  outputFile << dec << "\t<RUN_ID>"     <<  metaData.run_id  << "</RUN_ID>\n";

	  CPU_temp_start = metaData.CPU_temp[0]*0.0625;
	  CPU_temp_stop = metaData.CPU_temp[1]*0.0625;
	  PMT_temp_start = metaData.PMT_temp[0]*0.25;
	  PMT_temp_stop = metaData.PMT_temp[1]*0.25;
	  REAL_run_duration= (metaData.CPU_time[1]-metaData.CPU_time[0])/1000;

	 
	  if ((CPU_temp_start <-10) && (CPU_temp_start >-20))
	    outputFile << "\t<CPU_TEMP_START_Y>"  <<  1  << "</CPU_TEMP_START_Y>\n";
	  
	  if ((CPU_temp_start <45) && (CPU_temp_start >35))
	    outputFile << "\t<CPU_TEMP_START_Y>" <<  1   << "</CPU_TEMP_START_Y>\n";

	  if ((CPU_temp_stop <-10) && (CPU_temp_stop >-20))
	    outputFile << "\t<CPU_TEMP_STOP_Y>"   <<  1   << "</CPU_TEMP_STOP_Y>\n";
	  
	  if ((CPU_temp_stop <45) && (CPU_temp_stop >35))
	    outputFile << "\t<CPU_TEMP_STOP_Y>"   <<  1   << "</CPU_TEMP_STOP_Y>\n";

	  if ((PMT_temp_start <-10) && (PMT_temp_start >-20))
	    outputFile << "\t<PMT_TEMP_START_Y>"  <<  1   << "</PMT_TEMP_START_Y>\n";
	  
	  if ((PMT_temp_start <45) && (PMT_temp_start >35))
	    outputFile << "\t<PMT_TEMP_START_Y>"  <<  1   << "</PMT_TEMP_START_Y>\n";

	  if ((PMT_temp_stop <-10) && (PMT_temp_stop >-20))
	    outputFile << "\t<PMT_TEMP_STOP_Y>"  <<  1    << "</PMT_TEMP_STOP_Y>\n";
	  
	  if ((PMT_temp_stop <45) && (PMT_temp_stop >35))
	    outputFile << "\t<PMT_TEMP_STOP_Y>"  <<  1    << "</PMT_TEMP_STOP_Y>\n";	   

	  if (CPU_temp_start >45)
	    outputFile << "\t<CPU_TEMP_START_R>" <<  1 << "</CPU_TEMP_START_R>\n";
	   
	  if (CPU_temp_start<-20)
	    outputFile << "\t<CPU_TEMP_START_R>"  <<  1 << "</CPU_TEMP_START_R>\n";

	  if (CPU_temp_stop >45)
	    outputFile << "\t<CPU_TEMP_STOP_R>"  <<  1 << "</CPU_TEMP_STOP_R>\n";
	  
	  if (CPU_temp_stop<-20)
	    outputFile << "\t<CPU_TEMP_STOP_R>"  <<  1  << "</CPU_TEMP_STOP_R>\n";

	  if (PMT_temp_start >45)
	    outputFile << "\t<PMT_TEMP_START_R>"  <<  1 << "</PMT_TEMP_START_R>\n";
	  
	  if (PMT_temp_start<-20)
	    outputFile << "\t<PMT_TEMP_START_R>"  <<  1 << "</PMT_TEMP_START_R>\n";

	  if (PMT_temp_stop >45)
	    outputFile << "\t<PMT_TEMP_STOP_R>"  <<  1 << "</PMT_TEMP_STOP_R>\n";
	  
	  if (PMT_temp_stop<-20)
	    outputFile << "\t<PMT_TEMP_STOP_R>"  <<  1 << "</PMT_TEMP_STOP_R>\n";

	  outputFile << "\t<CPU_START_TIME>"<< metaData.CPU_time[0] << "</CPU_START_TIME>\n";
	  outputFile << "\t<CPU_STOP_TIME>" << metaData.CPU_time[1] << "</CPU_STOP_TIME>\n";
	  outputFile << "\t<REAL_RUN_DURATION>" << REAL_run_duration << "</REAL_RUN_DURATION>\n";
	  outputFile << "\t<CPU_TEMP_START>" <<  CPU_temp_start  << "</CPU_TEMP_START>\n";
	  outputFile << "\t<CPU_TEMP_STOP>"  <<  CPU_temp_stop  << "</CPU_TEMP_STOP>\n";
	  outputFile << "\t<PMT_TEMP_START>" <<  PMT_temp_start << "</PMT_TEMP_START>\n";
	  outputFile << "\t<PMT_TEMP_STOP>"  <<  PMT_temp_stop  << "</PMT_TEMP_STOP>\n";

	  outputFile << "</CPUTIMETEMP>\n";   
 
	}
    } 

  
  outputFile << "</ROOT_SOURCE>\n";
  outputFile.close();

}


void HVPSConfigToXML(TString rootname, TString outPath, TString xslPath = "")
{
  
  cout << endl <<  "Processing HVPS to xml file:" << rootname << endl;

  TString filename = outPath;
  const char * _temp = rootname;
  filename += "/";
  filename += basename(_temp);
  filename.ReplaceAll(".root", 5, "_HVPSConfig.xml", 15);

  ofstream outputFile;
  outputFile.open(filename.Data(), ios::trunc);
  
  if (!outputFile.is_open()){
    printf("Cannot open the file %s for the output", filename.Data());
    exit(0);
  }
  
  outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
  outputFile << "<!-- Prologo XML -->\n";
  outputFile << "<?xml-stylesheet type='text/xsl' href='"<< xslPath.Data()<<"'?>\n";
  outputFile << "<ROOT_SOURCE>\n";

  Double_t HV_value_table[10];
  Double_t Plane_value_table[2];
  
  LEvRec0File rootfile(rootname.Data());
  LEvRec0 ev;
  LEvRec0Md metaData;
  rootfile.SetTheEventPointer(ev);
  rootfile.SetTmdPointer(metaData);

  // Metadata
  int Tmd_entries = rootfile.GetTmdEntries(); 
  //cout << "Number of Tmd entries: " << Tmd_entries << endl;
  
  for(int j=0;j<Tmd_entries;j++)
    {
      rootfile.GetTmdEntry(j);   
      for(int h=0;h<10;h++)
	{
	  HV_value_table[h]=(double)(metaData.HV_value[h]+1)/1024*1200;
	}

      for(int l=0;l<2;l++)
	{
	  Plane_value_table[l]= (double)(metaData.silConfig.plane_HV[l]+1)/1024*150.8;
	}

      outputFile << "<HVPSCONFIG>\n";
      
      outputFile << "\t<HEADER>"<< 0 << "</HEADER>\n";
      
      if(j%2==0) 
	outputFile << "\t<HEADER>" << 1 << "</HEADER>\n";

      outputFile << "\t<HEADER_VAL>" << "H" << "</HEADER_VAL>\n";
      outputFile << "\t<TAIL_VAL>" << "T" << "</TAIL_VAL>\n";
      
      outputFile << "\t<HV_PLANE_A_error>"  <<  0 << "</HV_PLANE_A_error>\n";
      outputFile << "\t<HV_PLANE_B_error>"  <<  0 << "</HV_PLANE_B_error>\n";
      outputFile << "\t<HV_PMT0_error>"  <<  0 << "</HV_PMT0_error>\n";
      outputFile << "\t<HV_PMT1_error>"  <<  0 << "</HV_PMT1_error>\n";
      outputFile << "\t<HV_PMT2_error>"  <<  0 << "</HV_PMT2_error>\n";
      outputFile << "\t<HV_PMT3_error>"  <<  0 << "</HV_PMT3_error>\n";
      outputFile << "\t<HV_PMT4_error>"  <<  0 << "</HV_PMT4_error>\n";
      outputFile << "\t<HV_PMT5_error>"  <<  0 << "</HV_PMT5_error>\n";
      outputFile << "\t<HV_PMT6_error>"  <<  0 << "</HV_PMT6_error>\n";
      outputFile << "\t<HV_PMT7_error>"  <<  0 << "</HV_PMT7_error>\n";
      outputFile << "\t<HV_PMT8_error>"  <<  0 << "</HV_PMT8_error>\n";
      outputFile << "\t<HV_PMT9_error>"  <<  0 << "</HV_PMT9_error>\n"; 
      
      outputFile << "\t<BOOT_NR>" << metaData.boot_nr << "</BOOT_NR>\n";
      outputFile << "\t<RUN_NR>"  << metaData.run_id  << "</RUN_NR>\n";

      if (Plane_value_table[0]<0 || Plane_value_table[0]>150.8)
	outputFile << "\t<HV_PLANE_A_error>"  <<  1   << "</HV_PLANE_A_error>\n";

      if (Plane_value_table[1]<0 || Plane_value_table[1]>150.8)
	outputFile << "\t<HV_PLANE_B_error>"  <<  1  << "</HV_PLANE_B_error>\n";

      if (HV_value_table[0]<0 || HV_value_table[0]>1200)
	outputFile << "\t<HV_PMT0_error>"  <<  1 << "</HV_PMT0_error>\n";

      if (HV_value_table[1]<0 || HV_value_table[1]>1200)
	outputFile << "\t<HV_PMT1_error>"  <<  1 << "</HV_PMT1_error>\n";

      if (HV_value_table[2]<0 || HV_value_table[2]>1200)
	outputFile << "\t<HV_PMT2_error>"  <<  1 << "</HV_PMT2_error>\n";

      if (HV_value_table[3]<0 || HV_value_table[3]>1200)
	outputFile << "\t<HV_PMT3_error>"  <<  1 << "</HV_PMT3_error>\n";

      if (HV_value_table[4]<0 || HV_value_table[4]>1200)
	outputFile << "\t<HV_PMT4_error>"  <<  1 << "</HV_PMT4_error>\n";

      if (HV_value_table[5]<0 || HV_value_table[5]>1200)
	outputFile << "\t<HV_PMT5_error>"  <<  1 << "</HV_PMT5_error>\n";

      if (HV_value_table[6]<0 || HV_value_table[6]>1200)
	outputFile << "\t<HV_PMT6_error>"  <<  1 << "</HV_PMT6_error>\n";

      if (HV_value_table[7]<0 || HV_value_table[7]>1200)
	outputFile << "\t<HV_PMT7_error>"  <<  1 << "</HV_PMT7_error>\n";

      if (HV_value_table[8]<0 || HV_value_table[8]>1200)
	outputFile << "\t<HV_PMT8_error>"  <<  1 << "</HV_PMT8_error>\n";

      if (HV_value_table[9]<0 || HV_value_table[9]>1200)
	outputFile << "\t<HV_PMT9_error>"  <<  1 << "</HV_PMT9_error>\n";

      
      outputFile << setprecision(3) << "\t<HV_PLANE_A>"  << Plane_value_table[0]
		 << "</HV_PLANE_A>\n";
      outputFile << setprecision(3) << "\t<HV_PLANE_B>"  << Plane_value_table[1]
		 << "</HV_PLANE_B>\n";

      outputFile << setprecision(4) << "\t<HV_PMT0>"
		 <<  HV_value_table[0] << "</HV_PMT0>\n";
      outputFile << setprecision(4) << "\t<HV_PMT1>"
		 <<  HV_value_table[1] << "</HV_PMT1>\n";
      outputFile << setprecision(4) << "\t<HV_PMT2>"
		 <<  HV_value_table[2]  << "</HV_PMT2>\n";
      outputFile << setprecision(4) << "\t<HV_PMT3>"
		 <<  HV_value_table[3] << "</HV_PMT3>\n";
      outputFile << setprecision(4) << "\t<HV_PMT4>"
		 <<  HV_value_table[4] << "</HV_PMT4>\n";
      outputFile << setprecision(4) << "\t<HV_PMT5>"
		 <<  HV_value_table[5] << "</HV_PMT5>\n";
      outputFile << setprecision(4) << "\t<HV_PMT6>"
		 <<  HV_value_table[6] << "</HV_PMT6>\n";
      outputFile << setprecision(4) << "\t<HV_PMT7>"
		 <<  HV_value_table[7] << "</HV_PMT7>\n";
      outputFile << setprecision(4) << "\t<HV_PMT8>"
		 <<  HV_value_table[8] << "</HV_PMT8>\n";
      outputFile << setprecision(4) << "\t<HV_PMT9>"
		 << HV_value_table[9] << "</HV_PMT9>\n";

      if(metaData.HV_mask[10] == 0)	
	outputFile << "\t<MASK_PLANE_A>" << "ON" << "</MASK_PLANE_A>\n";
      if(metaData.HV_mask[10] == 1)
	outputFile << "\t<MASK_PLANE_A>" << "OFF" << "</MASK_PLANE_A>\n";   
      if(metaData.HV_mask[11] == 0)
	outputFile << "\t<MASK_PLANE_B>" << "ON"  << "</MASK_PLANE_B>\n";
      if(metaData.HV_mask[11] == 1)
	outputFile << "\t<MASK_PLANE_B>" << "OFF" << "</MASK_PLANE_B>\n";

      if(metaData.HV_mask[0] == 0)
	outputFile << "\t<MASK_PMT0>"  << "ON"  << "</MASK_PMT0>\n";
      if(metaData.HV_mask[0] == 1)
	outputFile << "\t<MASK_PMT0>"  << "OFF" << "</MASK_PMT0>\n";
      if(metaData.HV_mask[1] == 0)
	outputFile << "\t<MASK_PMT1>"  << "ON"  << "</MASK_PMT1>\n";
      if(metaData.HV_mask[1] == 1)
	outputFile << "\t<MASK_PMT1>"  << "OFF"  << "</MASK_PMT1>\n";
      if(metaData.HV_mask[2] == 0)
	outputFile << "\t<MASK_PMT2>"  << "ON"  << "</MASK_PMT2>\n";
      if(metaData.HV_mask[2] == 1)
	outputFile << "\t<MASK_PMT2>"  << "OFF" << "</MASK_PMT2>\n";
      if(metaData.HV_mask[3] == 0)
	outputFile << "\t<MASK_PMT3>"  << "ON"  << "</MASK_PMT3>\n";
      if(metaData.HV_mask[3] == 1)
	outputFile << "\t<MASK_PMT3>"  << "OFF" << "</MASK_PMT3>\n";
      if(metaData.HV_mask[4] == 0)
	outputFile << "\t<MASK_PMT4>"  << "ON"  << "</MASK_PMT4>\n";
      if(metaData.HV_mask[4] == 1)
	outputFile << "\t<MASK_PMT4>"  << "OFF" << "</MASK_PMT4>\n";
      if(metaData.HV_mask[5] == 0)
	outputFile << "\t<MASK_PMT5>"  << "ON"  << "</MASK_PMT5>\n";
      if(metaData.HV_mask[5] == 1)
	outputFile << "\t<MASK_PMT5>" << "OFF"  << "</MASK_PMT5>\n";
      if(metaData.HV_mask[6] == 0)
	outputFile << "\t<MASK_PMT6>" << "ON"   << "</MASK_PMT6>\n";
      if(metaData.HV_mask[6] == 1)
	outputFile << "\t<MASK_PMT6>" << "OFF"  << "</MASK_PMT6>\n";
      if(metaData.HV_mask[7] == 0)
	outputFile << "\t<MASK_PMT7>" << "ON"   << "</MASK_PMT7>\n";
      if(metaData.HV_mask[7] == 1)
	outputFile << "\t<MASK_PMT7>" << "OFF" << "</MASK_PMT7>\n";
      if(metaData.HV_mask[8] == 0)
	outputFile << "\t<MASK_PMT8>" << "ON"  << "</MASK_PMT8>\n";
      if(metaData.HV_mask[8] == 1)
	outputFile << "\t<MASK_PMT8>" << "OFF" << "</MASK_PMT8>\n";
      if(metaData.HV_mask[9] == 0)
	outputFile << "\t<MASK_PMT9>" << "ON" << "</MASK_PMT9>\n";
      if(metaData.HV_mask[9] == 1)
	outputFile << "\t<MASK_PMT9>" << "OFF" << "</MASK_PMT9>\n";

      outputFile << "</HVPSCONFIG>\n";

    }
  outputFile << "</ROOT_SOURCE>\n";
  outputFile.close();
}

void RunInfoToXML(TString rootname, TString outPath, TString xslPath = "")
{
  
  cout << endl << "Processing Run Info to xml file:" << rootname << endl;
  
  TString filename = outPath;
  const char * _temp = rootname;
  filename += "/";
  filename += basename(_temp);
  filename.ReplaceAll(".root", 5, "_RunInfo.xml", 12);

  ofstream outputFile;
  outputFile.open(filename.Data(), ios::trunc);


  if (!outputFile.is_open()){
    printf("Cannot open the file %s for the output", filename.Data());
    exit(0);
  }


  LEvRec0File rootfile(rootname.Data());
  LEvRec0 ev;
  LEvRec0Md metaData;
  rootfile.SetTheEventPointer(ev);
  rootfile.SetTmdPointer(metaData);

  outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
  outputFile << "<!-- Prologo XML -->\n";

  outputFile << "<?xml-stylesheet type='text/xsl' href='"<< xslPath.Data()<<"'?>\n";
  outputFile << "<ROOT_SOURCE>\n";
  
  // Metadata
  int Tmd_entries = rootfile.GetTmdEntries(); 
  //cout << "Number of Tmd entries: " << Tmd_entries << endl;
  
  UShort_t run_type_vect[Tmd_entries];
  UShort_t run_duration_vect[Tmd_entries];
  UShort_t orbitZone_vect[Tmd_entries];
  UShort_t run_id_vect[Tmd_entries];
  UShort_t boot_nr_vect[Tmd_entries];

  
  for(int j=0;j<Tmd_entries;j++)
    {

      rootfile.GetTmdEntry(j);   

      run_type_vect[j] = metaData.run_type;
      run_duration_vect[j] = metaData.run_duration;
      orbitZone_vect[j] = metaData.orbit_Zone;
      
      run_id_vect[j]=metaData.run_id;
      boot_nr_vect[j]=metaData.boot_nr;
      
    }

  for(int t=0;t<Tmd_entries;t++) {
       
    outputFile << "<RUN_INFO>\n";

    outputFile << "\t<HEADER>"<< 0 << "</HEADER>\n";
      
            
    outputFile <<  dec << "\t<RUN_TYPE_error>"<< 0 << "</RUN_TYPE_error>\n";
    outputFile << "\t<RUN_DURATION_error>"<< 0 << "</RUN_DURATION_error>\n";
    outputFile << dec << "\t<ORBIT_error>"<< 0 << "</ORBIT_error>\n";
    outputFile << dec << "\t<ORBIT_error_yellow>"<< 0 << "</ORBIT_error_yellow>\n";

    int applied_orbit=orbitZone_vect[t] & 0x00FF;
    int calculated_orbit=orbitZone_vect[t] >> 8;

       
    if (t%2==0){

	
      if (run_type_vect[t]!=run_type_vect[t+1]) 
	outputFile << dec << "\t<RUN_TYPE_error>"<< 1 << "</RUN_TYPE_error>\n";
	    
    	    
      if (run_duration_vect[t]!=run_duration_vect[t+1]) 
	outputFile << "\t<RUN_DURATION_error>"<< 1 << "</RUN_DURATION_error>\n";
	  
         
      if (applied_orbit != 0 && applied_orbit != 1 && applied_orbit != 2 && applied_orbit != 3 && applied_orbit != 4 && applied_orbit != 5) 
	outputFile << dec << "\t<ORBIT_error>"<< 1 << "</ORBIT_error>\n";
          
      if (calculated_orbit == 170) 
	outputFile << dec << "\t<ORBIT_error_yellow>"<< 1 << "</ORBIT_error_yellow>\n";   
      
    }
      
    if (t%2!=0){
	
      if (run_type_vect[t]!=run_type_vect[t-1]) 
	outputFile << dec << "\t<RUN_TYPE_error>"<< 1 << "</RUN_TYPE_error>\n";
	   	    
      if (run_duration_vect[t]!=run_duration_vect[t-1]) 
	outputFile << "\t<RUN_DURATION_error>"<< 1 << "</RUN_DURATION_error>\n";  
	         
     if (applied_orbit != 0 && applied_orbit != 1 && applied_orbit != 2 && applied_orbit != 3 && applied_orbit != 4 && applied_orbit != 5) 
	outputFile << dec << "\t<ORBIT_error>"<< 1 << "</ORBIT_error>\n";
          
      if (orbitZone_vect[t] >> 8 == 170) 
	outputFile << dec << "\t<ORBIT_error_yellow>"<< 1 << "</ORBIT_error_yellow>\n";
	 
    }
      
    if(t%2==0) 
      outputFile << "\t<HEADER>" << 1 << "</HEADER>\n";

    outputFile << "\t<HEADER_VAL>" << "H" << "</HEADER_VAL>\n";
    outputFile << "\t<TAIL_VAL>" << "T" << "</TAIL_VAL>\n";
      
    outputFile << dec << "\t<BOOT_NR>"    << boot_nr_vect[t]  << "</BOOT_NR>\n";
    outputFile << dec << "\t<RUN_NR>"     << run_id_vect[t]  << "</RUN_NR>\n";

    if (run_type_vect[t]==0)
      outputFile <<  "\t<RUN_TYPE>"   << "NO RUN"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==27)
      outputFile <<  "\t<RUN_TYPE>"   << "STD CALIB"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==45)
      outputFile <<  "\t<RUN_TYPE>"   << "FAST CALIB"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==54)
      outputFile <<  "\t<RUN_TYPE>"   << "ZERO-SUPPRESSED"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==78)
      outputFile <<  "\t<RUN_TYPE>"   << "VIRGIN RAW"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==85)
      outputFile <<  "\t<RUN_TYPE>"   << "ZERO-SUPPRESSED COMMON NOISE"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==99)
      outputFile <<  "\t<RUN_TYPE>"   << "MIXED"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==120)
      outputFile <<  "\t<RUN_TYPE>"   << "GENERIC"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==135)
      outputFile <<  "\t<RUN_TYPE>"   << "STOP"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==156)
      outputFile <<  "\t<RUN_TYPE>"   << "DUMP CALIB"  << "</RUN_TYPE>\n";

    if (run_type_vect[t]==170)
      outputFile <<  "\t<RUN_TYPE>"   << "DUMP INFO"  << "</RUN_TYPE>\n";

    outputFile << dec << "\t<RUN_DURATION>" << run_duration_vect[t] << "</RUN_DURATION>\n";



    if (calculated_orbit == 170)
      outputFile <<  "\t<ORBIT_ZONE_Calculated>"   << "BROADCAST NOT AVAIBLE" << "</ORBIT_ZONE_Calculated>\n";
      
    if (calculated_orbit == 04)
      outputFile <<  "\t<ORBIT_ZONE_Calculated>"   << "DEFAULT" << "</ORBIT_ZONE_Calculated>\n";

    if (applied_orbit == 04)
      outputFile <<  "\t<ORBIT_ZONE_Applied>"   << "DEFAULT" << "</ORBIT_ZONE_Applied>\n";
      
    if (calculated_orbit == 0)
      outputFile <<  "\t<ORBIT_ZONE_Calculated>"   << "SAA" << "</ORBIT_ZONE_Calculated>\n";
      
    if (applied_orbit == 0)
      outputFile <<  "\t<ORBIT_ZONE_Applied>"   << "SAA" << "</ORBIT_ZONE_Applied>\n";
          
    if (calculated_orbit == 1)
      outputFile <<  "\t<ORBIT_ZONE_Calculated>"   << "EQUATORIAL" << "</ORBIT_ZONE_Calculated>\n";

    if (applied_orbit == 1)
      outputFile <<  "\t<ORBIT_ZONE_Applied>"   << "EQUATORIAL" << "</ORBIT_ZONE_Applied>\n";

    if (calculated_orbit == 2)
      outputFile <<  "\t<ORBIT_ZONE_Calculated>"   << "SOUTH POLE" << "</ORBIT_ZONE_Calculated>\n";
    
    if (applied_orbit == 2)
      outputFile <<  "\t<ORBIT_ZONE_Applied>"   << "SOUTH POLE" << "</ORBIT_ZONE_Applied>\n";
	
    if (calculated_orbit == 3)
      outputFile <<  "\t<ORBIT_ZONE_Calculated>"   << "NORTH POLE" << "</ORBIT_ZONE_Calculated>\n";

    if (applied_orbit == 3)
      outputFile <<  "\t<ORBIT_ZONE_Applied>"   << "NORTH POLE" << "</ORBIT_ZONE_Applied>\n";

    if (calculated_orbit == 5)
      outputFile <<  "\t<ORBIT_ZONE_Calculated>"   << "STANDBY ZONE" << "</ORBIT_ZONE_Calculated>\n";

    if (applied_orbit == 5)
      outputFile <<  "\t<ORBIT_ZONE_Applied>"   << "STANDBY ZONE" << "</ORBIT_ZONE_Applied>\n";
	

    outputFile << "</RUN_INFO>\n";
      
      
  }  
   
  outputFile << "</ROOT_SOURCE>\n";
 
  outputFile.close();
}


void FastInfoToXML(TString rootname, TString outPath, TString xslPath = ""){
  
  
   TString filename = outPath;
   const char * _temp = rootname;
   filename += "/";
   filename += basename(_temp);
   filename.ReplaceAll(".root", 5, "_FastInfo.xml", 13);

   ofstream outputFile;
   outputFile.open(filename.Data(), ios::trunc);


   if (!outputFile.is_open()){
      printf("Cannot open the file %s for the output", filename.Data());
      exit(0);
   }


   LEvRec0File rootfile(rootname.Data());
   LEvRec0 ev;
   LEvRec0Md metaData;
   rootfile.SetTheEventPointer(ev);
   rootfile.SetTmdPointer(metaData);

   cout << endl << "Processing Fast Info to xml file:" << rootname << endl;

   outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
   outputFile << "<!-- Prologo XML -->\n";

   outputFile << "<?xml-stylesheet type='text/xsl' href='"<< xslPath.Data()<<"'?>\n";
   outputFile << "<ROOT_SOURCE>\n";
  
   // Metadata
   int Tmd_entries = rootfile.GetTmdEntries();
   //TTree *T=(TTree*)rootfile->Get("T");
   int n_events = 0;
   //cout << "Number of Tmd entries: " << Tmd_entries << endl;
   rootfile.GetEntry(0);
   rootfile.GetTmdEntry(0);
   int start_bootnr = metaData.boot_nr;
   int new_bootnr = ev.boot_nr;
   int start_runid = ev.run_id;
   int new_runid = ev.run_id;
   int Tentries = rootfile.GetEntries();
   int nEntriesForRun[Tmd_entries/2];
   int runInMergedFile = 0;

// calculating the number of events for each single run
   for (int nev=0; nev<Tentries; nev++ )
   {
      rootfile.GetEntry(nev);
      new_runid = ev.run_id;
      new_bootnr = ev.boot_nr;
      n_events++;
      
      if ((new_runid != start_runid) || (new_bootnr   != start_bootnr))
      {
	 nEntriesForRun[runInMergedFile] = (n_events-1);
	 n_events = 1;
	 start_runid = new_runid;
	 start_bootnr = new_bootnr;
	 // cout << "Events in Run "  << runInMergedFile << " = " << nEntriesForRun[runInMergedFile] << endl;
	 runInMergedFile++;

      }

      if (nev == (Tentries -1))
      {
	 nEntriesForRun[runInMergedFile] = n_events;
	 // cout << "Events in Run "  << runInMergedFile << " = " << nEntriesForRun[runInMergedFile] << endl;
      }

   }
   
  
  
   UShort_t run_type_vect[Tmd_entries];
   UShort_t run_duration_vect[Tmd_entries];
   UShort_t orbitZone_vect[Tmd_entries];
   UShort_t run_id_vect[Tmd_entries];
   UShort_t boot_nr_vect[Tmd_entries];
   Short_t CPU_temp_start[Tmd_entries];
   Short_t CPU_temp_stop[Tmd_entries];
   double long_vect_start[Tmd_entries];
   double long_vect_stop[Tmd_entries];
   double lat_vect_start[Tmd_entries];
   double lat_vect_stop[Tmd_entries];
  
   Short_t REAL_run_duration[Tmd_entries];
  
   int e = 0;
   int g = 0;
  
   for(int j=0;j<Tmd_entries;j++)
   {

      rootfile.GetTmdEntry(j);   

      run_type_vect[j] = metaData.run_type;
      run_duration_vect[j] = metaData.run_duration;
      orbitZone_vect[j] = metaData.orbit_Zone;
      
      run_id_vect[j]=metaData.run_id;
      boot_nr_vect[j]=metaData.boot_nr;

      CPU_temp_start[j] = metaData.CPU_temp[0]*0.0625;
      CPU_temp_stop[j] = metaData.CPU_temp[1]*0.0625;
      
      
      if(j%2==0) {
	 long_vect_start[e] = (double)(metaData.broadcast.GPS.lon)*pow(10,-7);
	 lat_vect_start[e]  = (double)(metaData.broadcast.GPS.lat)*pow(10,-7);
	 e = e+1;
	 //cout << "longitude   " <<  (double)(metaData.broadcast.GPS.lon)*pow(10,-7) << endl;
	
      }
      if(j%2!=0) {
	
	 long_vect_stop[g] = (double)(metaData.broadcast.GPS.lon)*pow(10,-7);
	 lat_vect_stop[g]  = (double)(metaData.broadcast.GPS.lat)*pow(10,-7);
	 g=g+1;
      }
      
   }

   int x=0;

   for(int t=0;t<Tmd_entries;t++) {
    
      rootfile.GetTmdEntry(t);
            
      if (t%2!=0){

	 outputFile << "<RUN_INFO2>\n";

	 TString current_mask="(";
	 int veto = (short)metaData.trigger_mask[0];

	 outputFile << "\t<LATITUDE_error>"   <<  0 << "</LATITUDE_error>\n";
	 outputFile << "\t<LONGITUDE_error>"   <<  0 << "</LONGITUDE_error>\n";      
	 outputFile <<  dec << "\t<RUN_TYPE_error>"<< 0 << "</RUN_TYPE_error>\n";
	 outputFile << "\t<RUN_DURATION_error>"<< 0 << "</RUN_DURATION_error>\n";
	 outputFile << dec << "\t<ORBIT_error>"<< 0 << "</ORBIT_error>\n";
	 outputFile << dec << "\t<ORBIT_error_yellow>"<< 0 << "</ORBIT_error_yellow>\n";


	 int applied_orbit=orbitZone_vect[t] & 0x00FF;
	 int calculated_orbit=orbitZone_vect[t] >> 8;
	 REAL_run_duration[t]= (metaData.CPU_time[1]-metaData.CPU_time[0])/1000;
    
      
	 if(lat_vect_start[t] > 90 && lat_vect_start[t] < -90)
	    outputFile << "\t<LATITUDE_error>"   <<  1 << "</LATITUDE_error>\n";
	 if(lat_vect_stop[t] > 90 && lat_vect_stop[t] < -90)
	    outputFile << "\t<LATITUDE_error>"   <<  1 << "</LATITUDE_error>\n";

	 if(long_vect_start[t] > 180 && long_vect_start[t] < -180)
	    outputFile << "\t<LONGITUDE_error>"   <<  0 << "</LONGITUDE_error>\n";
	 if(long_vect_stop[t] > 180 && long_vect_stop[t] < -180)
	    outputFile << "\t<LONGITUDE_error>"   <<  0 << "</LONGITUDE_error>\n";
      
	 if (applied_orbit != 0 && applied_orbit != 1 && applied_orbit != 2 && applied_orbit != 3 && applied_orbit != 4 && applied_orbit != 5) 
	    outputFile << dec << "\t<ORBIT_error>"<< 1 << "</ORBIT_error>\n";
          
	 if (calculated_orbit == 170) 
	    outputFile << dec << "\t<ORBIT_error_yellow>"<< 1 << "</ORBIT_error_yellow>\n";   
      

	         
	 if (applied_orbit != 0 && applied_orbit != 1 && applied_orbit != 2 && applied_orbit != 3 && applied_orbit != 4 && applied_orbit != 5) 
	    outputFile << dec << "\t<ORBIT_error>"<< 1 << "</ORBIT_error>\n";
          
	 if (orbitZone_vect[t] >> 8 == 170) 
	    outputFile << dec << "\t<ORBIT_error_yellow>"<< 1 << "</ORBIT_error_yellow>\n";
	 
      
	 outputFile << dec << "\t<BOOT_NR>"    << boot_nr_vect[t]  << "</BOOT_NR>\n";
	 outputFile << dec << "\t<RUN_NR>"     << run_id_vect[t]  << "</RUN_NR>\n";
       
	 if (run_type_vect[t]==0)
	    outputFile <<  "\t<RUN_TYPE>"   << "NO RUN"  << "</RUN_TYPE>\n";

	 if (run_type_vect[t]==27)
	    outputFile <<  "\t<RUN_TYPE>"   << "STD CALIB"  << "</RUN_TYPE>\n";

	 if (run_type_vect[t]==45)
	    outputFile <<  "\t<RUN_TYPE>"   << "FAST CALIB"  << "</RUN_TYPE>\n";

	 if (run_type_vect[t]==54)
	    outputFile <<  "\t<RUN_TYPE>"   << "ZERO-SUPPRESSED"  << "</RUN_TYPE>\n";

	 if (run_type_vect[t]==78)
	    outputFile <<  "\t<RUN_TYPE>"   << "VIRGIN"  << "</RUN_TYPE>\n";

	 if (run_type_vect[t]==85)
	    outputFile <<  "\t<RUN_TYPE>"   << "ZERO-SUPPRESSED CN"  << "</RUN_TYPE>\n";

	 if (run_type_vect[t]==99)
	    outputFile <<  "\t<RUN_TYPE>"   << "MIXED"  << "</RUN_TYPE>\n";

	 if (run_type_vect[t]==120)
	    outputFile <<  "\t<RUN_TYPE>"   << "GENERIC"  << "</RUN_TYPE>\n";
       
	 if (run_type_vect[t]==135)
	    outputFile <<  "\t<RUN_TYPE>"   << "STOP"  << "</RUN_TYPE>\n";

	 if (run_type_vect[t]==156)
	    outputFile <<  "\t<RUN_TYPE>"   << "DUMP CALIB"  << "</RUN_TYPE>\n";

	 if (run_type_vect[t]==170)
	    outputFile <<  "\t<RUN_TYPE>"   << "DUMP INFO"  << "</RUN_TYPE>\n";

	 outputFile << "\t<N_EVENTS>"  << nEntriesForRun[t/2]  << "</N_EVENTS>\n";
	 outputFile << dec << "\t<RUN_DURATION>" << REAL_run_duration[t] << "</RUN_DURATION>\n";


	 if (applied_orbit == 04)
	    outputFile <<  "\t<Applied_ORBIT_ZONE>"   << "DEFAULT" << "</Applied_ORBIT_ZONE>\n";
         
	 if (applied_orbit == 0)
	    outputFile <<  "\t<Applied_ORBIT_ZONE>"   << "SAA" << "</Applied_ORBIT_ZONE>\n";
          
	 if (applied_orbit == 1)
	    outputFile <<  "\t<Applied_ORBIT_ZONE>"   << "EQUATORIAL" << "</Applied_ORBIT_ZONE>\n";
    
	 if (applied_orbit == 2)
	    outputFile <<  "\t<Applied_ORBIT_ZONE>"   << "SOUTH POLE" << "</Applied_ORBIT_ZONE>\n";
  
	 if (applied_orbit == 3)
	    outputFile <<  "\t<Applied_ORBIT_ZONE>"   << "NORTH POLE" << "</Applied_ORBIT_ZONE>\n";

	 if (applied_orbit == 5)
	    outputFile <<  "\t<Applied_ORBIT_ZONE>"   << "STANDBY ZONE" << "</Applied_ORBIT_ZONE>\n";

	 outputFile << "\t<LATITUDE>" << "[" << lat_vect_start[x]<< "; "<< lat_vect_stop[x] <<"]"<< "</LATITUDE>\n";
	 outputFile << "\t<LONGITUDE>" << "[" << long_vect_start[x]<< "; "<< long_vect_stop[x] <<"]"<< "</LONGITUDE>\n";

	 x=x+1;
  
	 int on=0;
	 if(metaData.PMT_mask[0]==1 || metaData.PMT_mask[32]==1){
	    on=1;
	    current_mask+="T1 ";
	 }
	 if(metaData.PMT_mask[1]==1 || metaData.PMT_mask[33]==1){
	    if(on==1)
	       current_mask+="or T2";
	    if(on==0){
	       current_mask+="T2 ";
	       on=1;
	    }
	 }
	 if(metaData.PMT_mask[2]==1 || metaData.PMT_mask[34]==1){
	    if(on==1)
	       current_mask+="or T3";
	    if(on==0){
	       current_mask+="T3 ";
	       on=1;
	    }
	 }
	 if(metaData.PMT_mask[3]==1 || metaData.PMT_mask[35]==1){
	    if(on==1)
	       current_mask+="or T4";
	    if(on==0){
	       current_mask+="T4 ";
	       on=1;
	    }
	 }
	 if(metaData.PMT_mask[4]==1 || metaData.PMT_mask[36]==1){
	    if(on==1)
	       current_mask+="or T5";
	    if(on==0){
	       current_mask+="T5 ";
	       on=1;
	    }
	 }
	 if(metaData.PMT_mask[5]==1 || metaData.PMT_mask[37]==1){
	    if(on==1)
	       current_mask+="or T6";
	    if(on==0){
	       current_mask+="T6 ";
	       on=1;
	    }
	 }
	 current_mask+=")";

	 if((short)metaData.trigger_mask[1]==1)
	    current_mask+=" and P1";

	 if((short)metaData.trigger_mask[1]==2)
	    current_mask+=" and (P1 or P2)";

	 if((short)metaData.trigger_mask[1]==3)
	    current_mask+=" and (P1 or P2)";

	 if((short)metaData.trigger_mask[1]==4)
	    current_mask+=" and P1 and P2";

	 if((short)metaData.trigger_mask[1]==5)
	    current_mask+=" and P1 and P2 and P3";

	 if((short)metaData.trigger_mask[1]==6)
	    current_mask+=" and (P1 or P2) and (P15 or P16)";

	 if((short)metaData.trigger_mask[1]==7)
	    current_mask+=" and (P1 or P2) and L";
     
	 //cout << "current mask  "<< current_mask << endl;

	 if((short)metaData.trigger_mask[1]==8)
	    outputFile << "\t<TRIGGER>"  << "GENERIC TRIGGER MASK" << "</TRIGGER>\n";
	 else
	    outputFile << "\t<TRIGGER>"  <<   current_mask  << "</TRIGGER>\n";

	 if(veto==0)
            outputFile << "\t<VETO>"  << "NO" << "</VETO>\n";
	 else if(veto==1)
	    outputFile << "\t<VETO>"  << "LATERAL" << "</VETO>\n";
	 else if(veto==2)
	    outputFile << "\t<VETO>"  << "BOTTOM" << "</VETO>\n";
	 else if(veto==3)
	    outputFile << "\t<VETO>"  << "ALL" << "</VETO>\n";

      
	 outputFile << "</RUN_INFO2>\n";
      }
     

   }  
   
   outputFile << "</ROOT_SOURCE>\n";
 
   outputFile.close();
}


void ScintConfigToXML(TString rootname, TString outPath, TString xslPath = "")
{
  
  cout << endl <<  "Processing Scint to xml file:" << rootname << endl;

  TString filename = outPath;
  const char * _temp = rootname;
  filename += "/";
  filename += basename(_temp);
  filename.ReplaceAll(".root", 5, "_ScintConfig.xml", 16);

  ofstream outputFile;
  outputFile.open(filename.Data(), ios::trunc);

  
  if (!outputFile.is_open()){
    printf("Cannot open the file %s for the output", filename.Data());
    exit(0);
  }
  
  LEvRec0File rootfile(rootname.Data());
  LEvRec0 ev;
  LEvRec0Md metaData;
  rootfile.SetTheEventPointer(ev);
  rootfile.SetTmdPointer(metaData);

  outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
  outputFile << "<!-- Prologo XML -->\n";
  outputFile << "<?xml-stylesheet type='text/xsl' href='" << xslPath.Data() << "'?>\n";
  outputFile << "<ROOT_SOURCE>\n";
  
  // Metadata
  int Tmd_entries = rootfile.GetTmdEntries(); 
  //cout << "Number of Tmd entries: " << Tmd_entries << endl;

        
  
  for(int j=0;j<Tmd_entries;j++)
    {
      rootfile.GetTmdEntry(j);   
       
      if(j%2 == 0){

	outputFile << "<SCINTCONFIG>\n";
	outputFile << "\t<BOOT_NR>" << metaData.boot_nr << "</BOOT_NR>\n";
	outputFile << "\t<RUN_NR>"  << metaData.run_id  << "</RUN_NR>\n";
      
	outputFile << "\t<VETO_CONFIG>"  << (short)metaData.trigger_mask[0] << "</VETO_CONFIG>\n";
	outputFile << "\t<CURRENT_TRIGGER_MASK>" << (short)metaData.trigger_mask[1] << "</CURRENT_TRIGGER_MASK>\n";
	outputFile <<  showbase << hex << uppercase <<"\t<EASIROC_1>" << (short)metaData.easiroc_config[0] << "</EASIROC_1>\n";
	outputFile <<  showbase << hex << uppercase <<"\t<EASIROC_2>" << (short)metaData.easiroc_config[30] << "</EASIROC_2>\n";

       
	outputFile << dec <<  "/<MASK_PMT_0>" << metaData.PMT_mask[0] << "</MASK_PMT_0>/n";
	outputFile << dec << "/<MASK_PMT_1>" << metaData.PMT_mask[1] << "</MASK_PMT_1>/n";
	outputFile << dec << "/<MASK_PMT_2>" << metaData.PMT_mask[2] << "</MASK_PMT_2>/n";
	outputFile << dec <<"/<MASK_PMT_3>" << metaData.PMT_mask[3] << "</MASK_PMT_3>/n";
	outputFile << dec <<"/<MASK_PMT_4>" << metaData.PMT_mask[4] << "</MASK_PMT_4>/n";
	outputFile << dec <<"/<MASK_PMT_5>" << metaData.PMT_mask[5] << "</MASK_PMT_5>/n";
	outputFile << dec <<"/<MASK_PMT_6>" << metaData.PMT_mask[6]  << "</MASK_PMT_6>/n";
	outputFile << dec <<"/<MASK_PMT_7>" << metaData.PMT_mask[7] << "</MASK_PMT_7>/n";
	outputFile << dec <<"/<MASK_PMT_8>" << metaData.PMT_mask[8]  << "</MASK_PMT_8>/n";
	outputFile << dec <<"/<MASK_PMT_9>" << metaData.PMT_mask[9]  << "</MASK_PMT_9>/n";
	outputFile << dec <<"/<MASK_PMT_10>" << metaData.PMT_mask[10]  << "</MASK_PMT_10>/n";
	outputFile << dec <<"/<MASK_PMT_11>" << metaData.PMT_mask[11]  << "</MASK_PMT_11>/n";
	outputFile << dec <<"/<MASK_PMT_12>" << metaData.PMT_mask[12]  << "</MASK_PMT_12>/n";
	outputFile << dec <<"/<MASK_PMT_13>" << metaData.PMT_mask[13]  << "</MASK_PMT_13>/n";
	outputFile << dec <<"/<MASK_PMT_14>" << metaData.PMT_mask[14]  << "</MASK_PMT_14>/n";
	outputFile << dec <<"/<MASK_PMT_15>" << metaData.PMT_mask[15]  << "</MASK_PMT_15>/n";
	outputFile << dec <<"/<MASK_PMT_16>" << metaData.PMT_mask[16]  << "</MASK_PMT_16>/n";
	outputFile << dec <<"/<MASK_PMT_17>" << metaData.PMT_mask[17]  << "</MASK_PMT_17>/n";
	outputFile << dec <<"/<MASK_PMT_18>" << metaData.PMT_mask[18]  << "</MASK_PMT_18>/n";
	outputFile << dec <<"/<MASK_PMT_19>" << metaData.PMT_mask[19]  << "</MASK_PMT_19>/n";
	outputFile << dec <<"/<MASK_PMT_20>" << metaData.PMT_mask[20]  << "</MASK_PMT_20>/n";
	outputFile << dec <<"/<MASK_PMT_21>" << metaData.PMT_mask[21] << "</MASK_PMT_21>/n";
	outputFile << dec <<"/<MASK_PMT_22>" << metaData.PMT_mask[22] << "</MASK_PMT_22>/n";
	outputFile << dec <<"/<MASK_PMT_23>" << metaData.PMT_mask[23] << "</MASK_PMT_23>/n";
	outputFile << dec <<"/<MASK_PMT_24>" << metaData.PMT_mask[24] << "</MASK_PMT_24>/n";
	outputFile << dec <<"/<MASK_PMT_25>" << metaData.PMT_mask[25] << "</MASK_PMT_25>/n";
	outputFile << dec <<"/<MASK_PMT_26>" << metaData.PMT_mask[26]  << "</MASK_PMT_26>/n";
	outputFile << dec <<"/<MASK_PMT_27>" << metaData.PMT_mask[27]  << "</MASK_PMT_27>/n";
	outputFile << dec <<"/<MASK_PMT_28>" << metaData.PMT_mask[28] << "</MASK_PMT_28>/n";
	outputFile << dec <<"/<MASK_PMT_29>" << metaData.PMT_mask[29] << "</MASK_PMT_29>/n";
	outputFile << dec <<"/<MASK_PMT_30>" << metaData.PMT_mask[30]  << "</MASK_PMT_30>/n";
	outputFile << dec <<"/<MASK_PMT_31>" << metaData.PMT_mask[31]  << "</MASK_PMT_31>/n";

	outputFile << dec <<"/<MASK2_PMT_0>" << metaData.PMT_mask[32] << "</MASK2_PMT_0>/n";
	outputFile << dec <<"/<MASK2_PMT_1>" << metaData.PMT_mask[33] << "</MASK2_PMT_1>/n";
	outputFile << dec <<"/<MASK2_PMT_2>" << metaData.PMT_mask[34] << "</MASK2_PMT_2>/n";
	outputFile << dec <<"/<MASK2_PMT_3>" << metaData.PMT_mask[35] << "</MASK2_PMT_3>/n";
	outputFile << dec <<"/<MASK2_PMT_4>" << metaData.PMT_mask[36] << "</MASK2_PMT_4>/n";
	outputFile << dec <<"/<MASK2_PMT_5>" << metaData.PMT_mask[37] << "</MASK2_PMT_5>/n";
	outputFile << dec <<"/<MASK2_PMT_6>" << metaData.PMT_mask[38] << "</MASK2_PMT_6>/n";
	outputFile << dec <<"/<MASK2_PMT_7>" << metaData.PMT_mask[39] << "</MASK2_PMT_7>/n";
	outputFile << dec <<"/<MASK2_PMT_8>" << metaData.PMT_mask[40] << "</MASK2_PMT_8>/n";
	outputFile << dec <<"/<MASK2_PMT_9>" << metaData.PMT_mask[41] << "</MASK2_PMT_9>/n";
	outputFile << dec <<"/<MASK2_PMT_10>" << metaData.PMT_mask[42]  << "</MASK2_PMT_10>/n";
	outputFile << dec <<"/<MASK2_PMT_11>" << metaData.PMT_mask[43]  << "</MASK2_PMT_11>/n";
	outputFile << dec <<"/<MASK2_PMT_12>" << metaData.PMT_mask[44] << "</MASK2_PMT_12>/n";
	outputFile << dec <<"/<MASK2_PMT_13>" << metaData.PMT_mask[45] << "</MASK2_PMT_13>/n";
	outputFile << dec <<"/<MASK2_PMT_14>" << metaData.PMT_mask[46]  << "</MASK2_PMT_14>/n";
	outputFile << dec <<"/<MASK2_PMT_15>" << metaData.PMT_mask[47] << "</MASK2_PMT_15>/n";
	outputFile << dec <<"/<MASK2_PMT_16>" << metaData.PMT_mask[48]  << "</MASK2_PMT_16>/n";
	outputFile << dec <<"/<MASK2_PMT_17>" << metaData.PMT_mask[49]  << "</MASK2_PMT_17>/n";
	outputFile << dec <<"/<MASK2_PMT_18>" << metaData.PMT_mask[50] << "</MASK2_PMT_18>/n";
	outputFile << dec <<"/<MASK2_PMT_19>" << metaData.PMT_mask[51] << "</MASK2_PMT_19>/n";
	outputFile << dec <<"/<MASK2_PMT_20>" << metaData.PMT_mask[52] << "</MASK2_PMT_20>/n";
	outputFile << dec <<"/<MASK2_PMT_21>" << metaData.PMT_mask[53] << "</MASK2_PMT_21>/n";
	outputFile << dec <<"/<MASK2_PMT_22>" << metaData.PMT_mask[54]  << "</MASK2_PMT_22>/n";
	outputFile << dec <<"/<MASK2_PMT_23>" << metaData.PMT_mask[55] << "</MASK2_PMT_23>/n";
	outputFile << dec <<"/<MASK2_PMT_24>" << metaData.PMT_mask[56] << "</MASK2_PMT_24>/n";
	outputFile << dec <<"/<MASK2_PMT_25>" << metaData.PMT_mask[57] << "</MASK2_PMT_25>/n";
	outputFile << dec <<"/<MASK2_PMT_26>" << metaData.PMT_mask[58] << "</MASK2_PMT_26>/n";
	outputFile << dec <<"/<MASK2_PMT_27>" << metaData.PMT_mask[59] << "</MASK2_PMT_27>/n";
	outputFile << dec <<"/<MASK2_PMT_28>" << metaData.PMT_mask[60] << "</MASK2_PMT_28>/n";
	outputFile << dec <<"/<MASK2_PMT_29>" << metaData.PMT_mask[61] << "</MASK2_PMT_29>/n";
	outputFile << dec <<"/<MASK2_PMT_30>" << metaData.PMT_mask[62]  << "</MASK2_PMT_30>/n";

	outputFile << dec <<"/<GEN_TRIG_MASK_0>" << metaData.gen_trig_mask[0]  << "</GEN_TRIG_MASK_0>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_1>" << metaData.gen_trig_mask[1]  << "</GEN_TRIG_MASK_1>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_2>" << metaData.gen_trig_mask[2]  << "</GEN_TRIG_MASK_2>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_3>" << metaData.gen_trig_mask[3]  << "</GEN_TRIG_MASK_3>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_4>" << metaData.gen_trig_mask[4]  << "</GEN_TRIG_MASK_4>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_5>" << metaData.gen_trig_mask[5]  << "</GEN_TRIG_MASK_5>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_6>" << metaData.gen_trig_mask[6]  << "</GEN_TRIG_MASK_6>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_7>" << metaData.gen_trig_mask[7]  << "</GEN_TRIG_MASK_7>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_8>" << metaData.gen_trig_mask[8]  << "</GEN_TRIG_MASK_8>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_9>" << metaData.gen_trig_mask[9]  << "</GEN_TRIG_MASK_9>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_10>" << metaData.gen_trig_mask[10]  << "</GEN_TRIG_MASK_10>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_11>" << metaData.gen_trig_mask[11]  << "</GEN_TRIG_MASK_11>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_12>" << metaData.gen_trig_mask[12]  << "</GEN_TRIG_MASK_12>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_13>" << metaData.gen_trig_mask[13]  << "</GEN_TRIG_MASK_13>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_14>" << metaData.gen_trig_mask[14]  << "</GEN_TRIG_MASK_14>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_15>" << metaData.gen_trig_mask[15]  << "</GEN_TRIG_MASK_15>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_16>" << metaData.gen_trig_mask[16]  << "</GEN_TRIG_MASK_16>/n";
	outputFile << dec <<"/<GEN_TRIG_MASK_17>" << metaData.gen_trig_mask[17]  << "</GEN_TRIG_MASK_17>/n";
	
	

	outputFile << "</SCINTCONFIG>\n";
      }   
  
    }
  
  outputFile << "</ROOT_SOURCE>\n";
  outputFile.close();
}


void SilConfigToXML(TString rootname, TString outPath, TString xslPath = "")
{
  
  cout << endl << "Processing Silicon Config to xml file:" << rootname << endl;

  TString filename = outPath;
  const char * _temp = rootname;
  filename += "/";
  filename += basename(_temp);
  filename.ReplaceAll(".root", 5, "_SilConfig.xml", 14);

  ofstream outputFile;
  outputFile.open(filename.Data(), ios::trunc);

  
  if (!outputFile.is_open()){
    printf("Cannot open the file %s for the output",filename.Data());
    exit(0);
  }
  
 
  LEvRec0File rootfile(rootname.Data());
  LEvRec0 ev;
  LEvRec0Md metaData;
  rootfile.SetTheEventPointer(ev);
  rootfile.SetTmdPointer(metaData);
 
  outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
  outputFile << "<!-- Prologo XML -->\n";
  outputFile << "<?xml-stylesheet type='text/xsl' href='"<<xslPath<<"'?>\n";
  outputFile << "<ROOT_SOURCE>\n";
  
  // Metadata
  int Tmd_entries = rootfile.GetTmdEntries(); 
  //cout << "Number of Runs " << Tmd_entries << endl;
  
  Bool_t ladder_on_top[Tmd_entries];
  Bool_t ladder_on_central[Tmd_entries];
  Bool_t ladder_on_bottom[Tmd_entries];
  Bool_t ladder_mask_top[Tmd_entries];
  Bool_t ladder_mask_central[Tmd_entries];
  Bool_t ladder_mask_bottom[Tmd_entries];
  UShort_t adj_strip_vect[Tmd_entries];
  UShort_t zero_supp_vect[Tmd_entries];
  Short_t CN_high_vect[Tmd_entries];
  Short_t CN_low_vect[Tmd_entries];
  UShort_t event_RMS_vect[Tmd_entries];
  UShort_t  event_ped_vect[Tmd_entries];
  UShort_t event_gauss_vect[Tmd_entries];
  UShort_t event_CN_vect[Tmd_entries];
  UShort_t gauss_check_vect[Tmd_entries];
  UShort_t run_id_vect[Tmd_entries];
  UShort_t boot_nr_vect[Tmd_entries];

  for(int j=0;j<Tmd_entries;j++)
    {
      rootfile.GetTmdEntry(j);   

      ladder_on_top[j] = metaData.silConfig.ladder_on[0];
      ladder_on_central[j] = metaData.silConfig.ladder_on[1];
      ladder_on_bottom[j] = metaData.silConfig.ladder_on[2];
      ladder_mask_top[j] = metaData.silConfig.ladder_mask[0];
      ladder_mask_central[j] = metaData.silConfig.ladder_mask[1];
      ladder_mask_bottom[j] = metaData.silConfig.ladder_mask[2];

      adj_strip_vect[j] = metaData.silConfig.adj_strip;
      zero_supp_vect[j] = metaData.silConfig.zero_supp_thrd;
      CN_high_vect[j] = (short)metaData.silConfig.thrd_CN_HIGH;
      CN_low_vect[j] = (short)metaData.silConfig.thrd_CN_LOW;
      event_RMS_vect[j] = metaData.silConfig.calib_event_RMS;
      event_ped_vect[j] = metaData.silConfig.calib_event_ped;
      event_gauss_vect[j] = metaData.silConfig.calib_event_gauss;
      event_CN_vect[j] = metaData.silConfig.calib_event_CN;
      gauss_check_vect[j] = metaData.silConfig.gauss_check;
      run_id_vect[j]=metaData.run_id;
      boot_nr_vect[j]=metaData.boot_nr;
      
    }

  for(int t=0;t<Tmd_entries;t++) {
         
    outputFile << "<SILCONFIG>\n";
      
    outputFile << "\t<HEADER>"<< 0 << "</HEADER>\n";
     
    outputFile << "\t<LADDER_error_top>"<< 0 << "</LADDER_error_top>\n";
    outputFile << "\t<LADDER_error_central>"<< 0 << "</LADDER_error_central>\n";
    outputFile << "\t<LADDER_error_bottom>"<< 0 << "</LADDER_error_bottom>\n";
    outputFile << "\t<LADDER_MASK_error_top>"<< 0 << "</LADDER_MASK_error_top>\n";
    outputFile << "\t<LADDER_MASK_error_central>"<< 0 << "</LADDER_MASK_error_central>\n";
    outputFile << "\t<LADDER_MASK_error_bottom>"<< 0 << "</LADDER_MASK_error_bottom>\n";
    outputFile << "\t<ADJ_STRIP_error>"<< 0 << "</ADJ_STRIP_error>\n";
    outputFile << "\t<ZERO_SUPP_error>"<< 0 << "</ZERO_SUPP_error>\n";
    outputFile << "\t<CN_HIGH_error>"<< 0 << "</CN_HIGH_error>\n";
    outputFile << "\t<CN_LOW_error>"<< 0 << "</CN_LOW_error>\n";
    outputFile << "\t<EVENT_RMS_error>"<< 0 << "</EVENT_RMS_error>\n";
    outputFile << "\t<EVENT_PED_error>"<< 0 << "</EVENT_PED_error>\n";
    outputFile << "\t<EVENT_GAUSS_error>"<< 0 << "</EVENT_GAUSS_error>\n";
    outputFile << "\t<EVENT_CN_error>"<< 0 << "</EVENT_CN_error>\n";
    outputFile << "\t<GAUSS_CHECK_error>"<< 0 << "</GAUSS_CHECK_error>\n";

    if (t%2==0){
	
      if (ladder_on_top[t]!=ladder_on_top[t+1])
	outputFile << "\t<LADDER_error_top>"<< 1 << "</LADDER_error_top>\n";

      if (ladder_on_central[t]!=ladder_on_central[t+1])
	outputFile << "\t<LADDER_error_central>"<< 1 << "</LADDER_error_central>\n";

      if (ladder_on_bottom[t]!=ladder_on_bottom[t+1])
	outputFile << "\t<LADDER_error_bottom>"<< 1 << "</LADDER_error_bottom>\n";

      if (ladder_mask_top[t]!=ladder_mask_top[t+1])
	outputFile << "\t<LADDER_MASK_error_top>"<< 1 << "</LADDER_MASK_error_top>\n";

      if (ladder_mask_central[t]!=ladder_mask_central[t+1])
	outputFile << "\t<LADDER_MASK_error_central>"<< 1 << "</LADDER_MASK_error_central>\n";

      if (ladder_mask_bottom[t]!=ladder_mask_bottom[t+1])
	outputFile << "\t<LADDER_MASK_error_bottom>"<< 1 << "</LADDER_MASK_error_bottom>\n";
	   
      if (adj_strip_vect[t]!=adj_strip_vect[t+1])
	outputFile << "\t<ADJ_STRIP_error>"<< 1 << "</ADJ_STRIP_error>\n";

      if (zero_supp_vect[t]!=zero_supp_vect[t+1])
	outputFile << "\t<ZERO_SUPP_error>"<< 1 << "</ZERO_SUPP_error>\n";

      if (CN_high_vect[t]!=CN_high_vect[t+1])
	outputFile << "\t<CN_HIGH_error>"<< 1 << "</CN_HIGH_error>\n";

      if (CN_low_vect[t]!=CN_low_vect[t+1])
	outputFile << "\t<CN_LOW_error>"<< 1 << "</CN_LOW_error>\n";

      if (event_RMS_vect[t]!=event_RMS_vect[t+1])
	outputFile << "\t<EVENT_RMS_error>"<< 1 << "</EVENT_RMS_error>\n";

      if (event_ped_vect[t]!=event_ped_vect[t+1])
	outputFile << "\t<EVENT_PED_error>"<< 1 << "</EVENT_PED_error>\n";

      if (event_gauss_vect[t]!=event_gauss_vect[t+1])
	outputFile << "\t<EVENT_GAUSS_error>"<< 1 << "</EVENT_GAUSS_error>\n";

      if (event_CN_vect[t]!=event_CN_vect[t+1])
	outputFile << "\t<EVENT_CN_error>"<< 1 << "</EVENT_CN_error>\n";

      if (gauss_check_vect[t]!=gauss_check_vect[t+1])
	outputFile << "\t<GAUSS_CHECK_error>"<< 1 << "</GAUSS_CHECK_error>\n";
	  
    }
       
       
    if (t%2!=0)
      {

	if (ladder_on_top[t]!=ladder_on_top[t-1])
	  outputFile << "\t<LADDER_error_top>"<< 1 << "</LADDER_error_top>\n";

	if (ladder_on_central[t]!=ladder_on_central[t-1])
	  outputFile << "\t<LADDER_error_central>"<< 1 << "</LADDER_error_central>\n";

	if (ladder_on_bottom[t]!=ladder_on_bottom[t-1])
	  outputFile << "\t<LADDER_error_bottom>"<< 1 << "</LADDER_error_bottom>\n";

	if (ladder_mask_top[t]!=ladder_mask_top[t-1])
	  outputFile << "\t<LADDER_MASK_error_top>"<< 1 << "</LADDER_MASK_error_top>\n";

	if (ladder_mask_central[t]!=ladder_mask_central[t-1])
	  outputFile << "\t<LADDER_MASK_error_central>"<< 1 << "</LADDER_MASK_error_central>\n";

	if (ladder_mask_bottom[t]!=ladder_mask_bottom[t-1])
	  outputFile << "\t<LADDER_MASK_error_bottom>"<< 1 << "</LADDER_MASK_error_bottom>\n";
	   
	if (adj_strip_vect[t]!=adj_strip_vect[t-1])
	  outputFile << "\t<ADJ_STRIP_error>"<< 1 << "</ADJ_STRIP_error>\n";

	if (zero_supp_vect[t]!=zero_supp_vect[t-1])
	  outputFile << "\t<ZERO_SUPP_error>"<< 1 << "</ZERO_SUPP_error>\n";

	if (CN_high_vect[t]!=CN_high_vect[t-1])
	  outputFile << "\t<CN_HIGH_error>"<< 1 << "</CN_HIGH_error>\n";

	if (CN_low_vect[t]!=CN_low_vect[t-1])
	  outputFile << "\t<CN_LOW_error>"<< 1 << "</CN_LOW_error>\n";

	if (event_RMS_vect[t]!=event_RMS_vect[t-1])
	  outputFile << "\t<EVENT_RMS_error>"<< 1 << "</EVENT_RMS_error>\n";

	if (event_ped_vect[t]!=event_ped_vect[t-1])
	  outputFile << "\t<EVENT_PED_error>"<< 1 << "</EVENT_PED_error>\n";

	if (event_gauss_vect[t]!=event_gauss_vect[t-1])
	  outputFile << "\t<EVENT_GAUSS_error>"<< 1 << "</EVENT_GAUSS_error>\n";

	if (event_CN_vect[t]!=event_CN_vect[t-1])
	  outputFile << "\t<EVENT_CN_error>"<< 1 << "</EVENT_CN_error>\n";

	if (gauss_check_vect[t]!=gauss_check_vect[t-1])
	  outputFile << "\t<GAUSS_CHECK_error>"<< 1 << "</GAUSS_CHECK_error>\n";
      }

    if(t%2==0) 
      outputFile << "\t<HEADER>" << 1 << "</HEADER>\n";

    outputFile << "\t<HEADER_VAL>" << "H" << "</HEADER_VAL>\n";

    outputFile << "\t<TAIL_VAL>" << "T" << "</TAIL_VAL>\n";
	
    outputFile << "\t<BOOT_NR>" << boot_nr_vect[t] << "</BOOT_NR>\n";
    outputFile << "\t<RUN_NR>"  << run_id_vect[t]  << "</RUN_NR>\n";

    if(ladder_on_top[t] == 0)
      outputFile << "\t<LADDERS_LV_top>" << "OFF" << "</LADDERS_LV_top>\n";
    if(ladder_on_top[t] == 1)
      outputFile << "\t<LADDERS_LV_top>" << "ON" << "</LADDERS_LV_top>\n";

    if(ladder_on_central[t] == 0)
      outputFile << "\t<LADDERS_LV_central>" << "OFF" << "</LADDERS_LV_central>\n";
    if(ladder_on_central[t] == 1)
      outputFile << "\t<LADDERS_LV_central>" << "ON" << "</LADDERS_LV_central>\n";


    if(ladder_on_bottom[t] == 0)
      outputFile << "\t<LADDERS_LV_bottom>" << "OFF" << "</LADDERS_LV_bottom>\n";
    if(ladder_on_bottom[t] == 1)
      outputFile << "\t<LADDERS_LV_bottom>" << "ON" << "</LADDERS_LV_bottom>\n";

    if(ladder_mask_top[t] == 0)
      outputFile << "\t<CURRENT_SD_top>" << "Excl." << "</CURRENT_SD_top>\n";
    if(ladder_mask_top[t] == 1)
      outputFile << "\t<CURRENT_SD_top>" << "Incl." << "</CURRENT_SD_top>\n";
    
    if(ladder_mask_central[t] == 0)
      outputFile << "\t<CURRENT_SD_central>" << "Excl." << "</CURRENT_SD_central>\n";
    if(ladder_mask_central[t] == 1)
      outputFile << "\t<CURRENT_SD_central>" << "Incl." << "</CURRENT_SD_central>\n";

    if(ladder_mask_bottom[t] == 0)
      outputFile << "\t<CURRENT_SD_bottom>" << "Excl." << "</CURRENT_SD_bottom>\n";
    if(ladder_mask_bottom[t] == 1)
      outputFile << "\t<CURRENT_SD_bottom>" << "Incl." << "</CURRENT_SD_bottom>\n";
    
    outputFile << "\t<ADIACENT_STRIP>" << adj_strip_vect[t]
	       << "</ADIACENT_STRIP>\n";
    outputFile << "\t<ZERO_SUPP_THRD>" << zero_supp_vect[t]
	       << "</ZERO_SUPP_THRD>\n";
    outputFile << "\t<THRD_CN_HIGH>"  << CN_high_vect[t]
	       << "</THRD_CN_HIGH>\n";
    outputFile << "\t<THRD_CN_LOW>"   << CN_low_vect[t]
	       << "</THRD_CN_LOW>\n";
    outputFile << "\t<CALIB_EVENT_RMS>" << event_RMS_vect[t]
	       << "</CALIB_EVENT_RMS>\n";
    outputFile << "\t<CALIB_EVENT_PED>" << event_ped_vect[t]
	       << "</CALIB_EVENT_PED>\n";
    outputFile << "\t<CALIB_EVENT_GAUSS>" << event_gauss_vect[t]
	       << "</CALIB_EVENT_GAUSS>\n";
    outputFile << "\t<CALIB_EVENT_CN>" << event_CN_vect[t]
	       << "</CALIB_EVENT_CN>\n";
    outputFile << "\t<GAUSS_CHECK>" << gauss_check_vect[t]
	       << "</GAUSS_CHECK>\n";

    outputFile << "</SILCONFIG>\n";    
  
  }
  outputFile << "</ROOT_SOURCE>\n";
  outputFile.close();
}


void TelemetryToXML(TString rootname, TString outPath,  TString xslPath = "")
{
  
  cout << endl << "Processing Telemetry to xml file:" << rootname << endl;
  
  TString filename = outPath;
  const char * _temp = rootname;
  filename += "/";
  filename += basename(_temp);
  filename.ReplaceAll(".root", 5, "_Telemetry.xml", 14);

  ofstream outputFile;
  outputFile.open(filename.Data(), ios::trunc);


  if (!outputFile.is_open()){
    printf("Cannot open the file %s for the output", filename.Data());
    exit(0);
  }
  LEvRec0File rootfile(rootname.Data());
  LEvRec0 ev;
  LEvRec0Md metaData;
  rootfile.SetTheEventPointer(ev);
  rootfile.SetTmdPointer(metaData);
  
  outputFile << "<?xml version='1.0' encoding='ISO-8859-1'?>\n";
  outputFile << "<!-- Prologo XML -->\n";
  outputFile << "<?xml-stylesheet type='text/xsl' href='"<< xslPath.Data() << "'?>\n";
  outputFile << "<ROOT_SOURCE>\n";
    
  // Metadata
  int Tmd_entries =  rootfile.GetTmdEntries(); 
  int k_start;
  int k_stop;
  
  int start_ind;
  int stop_ind;
  
  start_ind=0;
  stop_ind=0;
  
  if (Tmd_entries%2 ==0) {
    k_start= Tmd_entries/2;
    k_stop = Tmd_entries/2;
    
  }
  
  else  {

    k_start=((Tmd_entries-1)/2)+1;
    k_stop=(Tmd_entries-1)/2;   
  
  }
  
  UShort_t vect_start_status_DAQ[k_start];
  UShort_t vect_start_status_PMT[k_start];
  UShort_t vect_start_status_TM_TC[k_start];
  UShort_t vect_start_status_HVPS[k_start];
  UShort_t vect_start_status_CPU_boot[k_start];
  UShort_t vect_start_status_CPU_FSM[k_start];
  UShort_t vect_stop_status_DAQ[k_stop];
  UShort_t vect_stop_status_PMT[k_stop];
  UShort_t vect_stop_status_TM_TC[k_stop];
  UShort_t vect_stop_status_HVPS[k_stop];
  UShort_t vect_stop_status_CPU_boot[k_stop];
  UShort_t vect_stop_status_CPU_FSM[k_stop];
  UShort_t vect_boot_nr[k_start];
  UShort_t vect_run_id[k_start];

  //cout << "Number of Tmd entries: " << Tmd_entries << endl;
   
  for(int j=0;j<Tmd_entries;j++)
    {
      rootfile.GetTmdEntry(j);   


      if(j%2 == 0){    
	vect_start_status_DAQ[start_ind] = metaData.status_reg.statusDAQ;
	vect_start_status_PMT[start_ind] = metaData.status_reg.statusPMT;
	vect_start_status_TM_TC[start_ind] = metaData.status_reg.statusTM_TC;
	vect_start_status_HVPS[start_ind] = metaData.status_reg.statusHV_PS;
	vect_start_status_CPU_boot[start_ind] = metaData.status_reg.CPU_board_boot;
	vect_start_status_CPU_FSM[start_ind] = metaData.status_reg.statusCPU;
	vect_boot_nr[start_ind] = metaData.boot_nr;
	vect_run_id[start_ind]= metaData.run_id;
       
	start_ind=start_ind+1;	  
      }      
      else {
	vect_stop_status_DAQ[stop_ind] = metaData.status_reg.statusDAQ;
	vect_stop_status_PMT[stop_ind] = metaData.status_reg.statusPMT;
	vect_stop_status_TM_TC[stop_ind] = metaData.status_reg.statusTM_TC;
	vect_stop_status_HVPS[stop_ind] = metaData.status_reg.statusHV_PS;
	vect_stop_status_CPU_boot[stop_ind] = metaData.status_reg.CPU_board_boot;
	vect_stop_status_CPU_FSM[stop_ind] = metaData.status_reg.statusCPU;
          
	stop_ind=stop_ind+1;
	  
      } 	 
    
    } 

  
  for(int t=0;t<k_start;t++)
    {
      
      outputFile << dec << "\t<DAQ_STOP_error>"     << 0 << "</DAQ_STOP_error>\n";
      outputFile << dec << "\t<PMT_STOP_error>"     << 0 << "</PMT_STOP_error>\n";
      outputFile << dec << "\t<TM_TC_STOP_error>"   << 0 << "</TM_TC_STOP_error>\n";
      outputFile << dec << "\t<HVPS_STOP_error>"    << 0 << "</HVPS_STOP_error>\n";
      outputFile << dec << "\t<CPUboot_STOP_error>"     << 0 << "</CPUboot_STOP_error>\n";
      outputFile << dec << "\t<CPU_FSM_STOP_error>"     << 0 << "</CPU_FSM_STOP_error>\n";
           
      outputFile << "<TELEMETRY>\n";

      outputFile << dec << "\t<DAQ_START_error>"     <<  0 << "</DAQ_START_error>\n";
      outputFile << dec << "\t<PMT_START_error>"    <<  0 << "</PMT_START_error>\n";
      outputFile << dec << "\t<TM_TC_START_error>"  <<  0 << "</TM_TC_START_error>\n";
      outputFile << dec << "\t<HVPS_START_error>"    <<  0 << "</HVPS_START_error>\n";
      outputFile << dec << "\t<CPUboot_START_error>"     <<  0 << "</CPUboot_START_error>\n";
      outputFile << dec << "\t<CPU_FSM_START_error>"     <<  0 << "</CPU_FSM_START_error>\n";

      outputFile << dec <<  "\t<BOOT_NR>" <<   vect_boot_nr[t] << "</BOOT_NR>\n";
      outputFile << dec << "\t<RUN_ID>"  <<   vect_run_id[t] << "</RUN_ID>\n";  

      if (vect_start_status_DAQ[t]!= 0x50A1)
	outputFile << dec << "\t<DAQ_START_error>" << 1 << "</DAQ_START_error>\n";

      
      if (vect_start_status_PMT[t]!= 0x0D00 && vect_start_status_PMT[t]!=0x0D05) 
	outputFile << dec <<  "\t<PMT_START_error>" << 1 << "</PMT_START_error>\n";

           
      if (vect_start_status_TM_TC[t]!=0x075c && vect_start_status_TM_TC[t]!=0x0ffd)
	outputFile << dec << "\t<TM_TC_START_error>" << 1 << "</TM_TC_START_error>\n";

      if (vect_start_status_HVPS[t] != 0x0000 && vect_start_status_HVPS[t] !=0x0004)
	outputFile << dec << "\t<HVPS_START_error>"  <<  1 << "</HVPS_START_error>\n";

      if (vect_start_status_CPU_boot[t]!=0x0704 && vect_start_status_CPU_boot[t]!=0x0705)
	outputFile << dec << "\t<CPUboot_START_error>"   <<  1 << "</CPUboot_START_error>\n";

      if (vect_start_status_CPU_FSM[t]!=0x0300 && vect_start_status_CPU_FSM[t]!= 0x0200 && vect_start_status_CPU_FSM[t]!= 0x0100 && vect_start_status_CPU_FSM[t]!= 0)
	outputFile << dec <<"\t<CPU_FSM_START_error>"   <<  1 << "</CPU_FSM_START_error>\n";

      
      outputFile << setprecision(4)<<showbase << hex << uppercase << "\t<DAQ_START>" << vect_start_status_DAQ[t] << "</DAQ_START>\n";
      outputFile << setprecision(4) << showbase << hex << "\t<PMT_START>" << vect_start_status_PMT[t]  << "</PMT_START>\n";
      outputFile  << setprecision(4)<<showbase << hex << "\t<TM_TC_START>" << vect_start_status_TM_TC[t] << "</TM_TC_START>\n";
      outputFile << setprecision(4)<<showbase <<  hex << "\t<HVPS_START>" <<   vect_start_status_HVPS[t] << "</HVPS_START>\n";
      outputFile << setprecision(4)<<showbase << hex << "\t<CPUboot_START>" <<   vect_start_status_CPU_boot[t] << "</CPUboot_START>\n";
      outputFile << setprecision(4)<<showbase << hex << "\t<CPU_FSM_START>" <<   vect_start_status_CPU_FSM[t] << "</CPU_FSM_START>\n";

      stringstream ss;
      ss << hex << vect_stop_status_PMT[t];
      
      TString PMT_Status = ss.str();
      TString goodstatus;
 
      if(PMT_Status.Length() == 3)
	goodstatus=PMT_Status(1,2);
      
      if(PMT_Status.Length() == 4)
	goodstatus=PMT_Status(2,2);
    
      ss.str("");
   
    
      if (vect_stop_status_DAQ[t]!= 0x80A3 && vect_stop_status_DAQ[t]!= 0x70A2 && vect_stop_status_DAQ[t]!=0x60A2 && vect_stop_status_DAQ[t]!=0x50A1)
	outputFile << dec << "\t<DAQ_STOP_error>" << 1  << "</DAQ_STOP_error>\n";
	  
      if (goodstatus!="1f" && goodstatus!="2f" && goodstatus!="05")
	outputFile <<dec <<  "\t<PMT_STOP_error>" <<  1 << "</PMT_STOP_error>\n";

      if (vect_stop_status_TM_TC[t]!=0x075c && vect_stop_status_TM_TC[t]!=0x0ffd)
	outputFile << dec <<  "\t<TM_TC_STOP_error>"  <<  1 << "</TM_TC_STOP_error>\n";

      if (vect_stop_status_HVPS[t]!=0 && vect_stop_status_HVPS[t]!=4)
	outputFile << dec << "\t<HVPS_STOP_error>" <<  1 << "</HVPS_STOP_error>\n";

      if (vect_stop_status_CPU_boot[t]!=0x0705 && vect_stop_status_CPU_boot[t]!=0x0704)
	outputFile << dec << "\t<CPUboot_STOP_error>" << 1 << "</CPUboot_STOP_error>\n";

      if (vect_stop_status_CPU_FSM[t]!=1 && vect_stop_status_CPU_FSM[t]!=0x0301 && vect_stop_status_CPU_FSM[t]!=0x0302 && vect_stop_status_CPU_FSM[t]!=2 && vect_stop_status_CPU_FSM[t]!= 0)
	outputFile << dec << "\t<CPU_FSM_STOP_error>" << 1 << "</CPU_FSM_STOP_error>\n";
    
      if (t < k_stop)
	{
	  outputFile << setprecision(4) << hex << "\t<DAQ_STOP>"  <<  vect_stop_status_DAQ[t] << "</DAQ_STOP>\n";
	  outputFile << setprecision(4) << hex << "\t<PMT_STOP>" << vect_stop_status_PMT[t] << "</PMT_STOP>\n";
	  outputFile << setprecision(4) <<  hex << "\t<TM_TC_STOP>" << vect_stop_status_TM_TC[t] << "</TM_TC_STOP>\n";
	  outputFile << setprecision(4) << hex << "\t<HVPS_STOP>" << vect_stop_status_HVPS[t] << "</HVPS_STOP>\n";
	  outputFile << setprecision(4) << hex << "\t<CPUboot_STOP>"  << vect_stop_status_CPU_boot[t] << "</CPUboot_STOP>\n";
	  outputFile << setprecision(4) << hex << "\t<CPU_FSM_STOP>"  << vect_stop_status_CPU_FSM[t] << "</CPU_FSM_STOP>\n";
	}
      
      outputFile << "</TELEMETRY>\n";   
   
    }

  
  outputFile << "</ROOT_SOURCE>\n";
  outputFile.close();

}
