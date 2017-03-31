/**
 * =============================================================================
 *
 * Created by 
 * FILENAME:       
 *
 * DESCRIPTION:    
 *
 * INSTRUCTIONS:   
 *                 
 * DATE:           March 14, 2017
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
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "housekeeping.hh"


void BroadcastToXML(TString rootname, TString xslPath= "", TString xslPath2= "")
{
  
   cout << endl << "Broadcast to xml:" << rootname << endl;

   TString filename = rootname;
   filename.ReplaceAll(".root", 5, "_Time.xml",9 );

   ofstream outputFile;
   outputFile.open(filename.Data(), ios::trunc);
   if (!outputFile.is_open()){
      printf("Cannot open the file %s for the output", filename.Data());
      exit(0);
   }

   TString filename2 = rootname;
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
   cout << "Number of Tmd entries: " << Tmd_entries << endl;
   for(int j=0;j<Tmd_entries;j++)
   {
      rootfile.GetEntry(j);   
      outputFile << "<BROADCAST>\n";
      outputFile2 << "<BROADCAST2>\n";
      outputFile << "\t<BOOT_NR>" << metaData.boot_nr << "</BOOT_NR>\n";
      outputFile << "\t<RUN_NR>"  << metaData.run_id   << "</RUN_NR>\n";    
      outputFile << "\t<OBDH_MS>"  << metaData.broadcast.OBDH.ms  << "</OBDH_MS>\n";    
      outputFile << "\t<TIMESTAMP_OBDH>"<< metaData.timestamp.OBDH<< "</TIMESTAMP_OBDH>\n";
      outputFile << "\t<ABS_START_RUN>" << -9999 << "</ABS_START_RUN>\n";
      outputFile << "\t<ABS_STOP_RUN>" << -9999 << "</ABS_STOP_RUN>\n";
      outputFile << "\t<RELATIVE_START_RUN>" << metaData.CPU_time[0]
		 << "</RELATIVE_START_RUN>\n";
      outputFile << "\t<RELATIVE_STOP_RUN>" << metaData.CPU_time[1]
		 << "</RELATIVE_STOP_RUN>\n";   
      outputFile2 << "\t<BOOT_NR>" << metaData.boot_nr << "</BOOT_NR>\n";
      outputFile2 << "\t<RUN_NR>"  << metaData.run_id  << "</RUN_NR>\n";
      outputFile2 << "\t<OBDH_MS>"  << metaData.broadcast.OBDH.ms  << "</OBDH_MS>\n";
      outputFile2 << "\t<TIMESTAMP_GPS>" << metaData.timestamp.GPS << "</TIMESTAMP_GPS>\n";
      outputFile2 << "\t<GPS_POS_X>" << metaData.broadcast.GPS.axis_pos[0]
		  << "</GPS_POS_X>\n";
      outputFile2 << "\t<GPS_POS_Y>" << metaData.broadcast.GPS.axis_pos[1]
		  << "</GPS_POS_Y>\n";
      outputFile2 << "\t<GPS_POS_Z>" << metaData.broadcast.GPS.axis_pos[2]
		  << "</GPS_POS_Z>\n";
      outputFile2 << "\t<GPS_VEL_X>" << metaData.broadcast.GPS.axis_vel[0]
		  << "</GPS_VEL_X>\n";
      outputFile2 << "\t<GPS_VEL_Y>" << metaData.broadcast.GPS.axis_vel[1]
		  << "</GPS_VEL_Y>\n";
      outputFile2 << "\t<GPS_VEL_Z>" << metaData.broadcast.GPS.axis_vel[2]
		  << "</GPS_VEL_Z>\n";
      outputFile2 << "\t<LONGITUDE>" << metaData.broadcast.GPS.lon << "</LONGITUDE>\n";
      outputFile2 << "\t<LATITUDE>"  << metaData.broadcast.GPS.lat << "</LATITUDE>\n";
      outputFile << "</BROADCAST>\n";
      outputFile2 << "</BROADCAST2>\n"; 
  
   }
   outputFile << "</ROOT_SOURCE>\n";
   outputFile2 << "</ROOT_SOURCE>\n";
   outputFile.close();
   outputFile2.close();
}



void CPUTimeTempToXML(TString rootname, TString xslPath = "")
{
  
  cout << endl << "CPU Time to xml:" << rootname << endl;

  TString filename = rootname;
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
  cout << "Number of Tmd entries: " << Tmd_entries << endl;

  for(int j=0;j<Tmd_entries;j++)
    {
       rootfile.GetEntry(j);   
       
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

	  
	  outputFile << "\t<CPU_START_TIME>"<<metaData.CPU_time[0]<< "</CPU_START_TIME>\n";
	  outputFile << "\t<CPU_STOP_TIME>" <<metaData.CPU_time[1]<< "</CPU_STOP_TIME>\n";
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


void HVPSConfigToXML(TString rootname, TString xslPath = "")
{
  
  cout << endl <<  "HVPS to xml:" << rootname << endl;

  TString filename = rootname;
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
  cout << "Number of Tmd entries: " << Tmd_entries << endl;
  
  for(int j=0;j<Tmd_entries;j++)
  {
     rootfile.GetEntry(j);   
     for(int h=0;h<10;h++)
     {
	HV_value_table[h]=(double)(metaData.HV_value[h]+1)/1024*1200;
     }

     for(int l=0;l<2;l++)
     {
	Plane_value_table[l]= (double)(metaData.silConfig.plane_HV[l]+1)/1024*150.8;
     }

     outputFile << "<HVPSCONFIG>\n";
      
     outputFile << "\t<BOOT_NR>" << metaData.boot_nr << "</BOOT_NR>\n";
     outputFile << "\t<RUN_NR>"  << metaData.run_id  << "</RUN_NR>\n";
     
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

void RunInfoToXML(TString rootname, TString xslPath = "")
{
  
   cout << endl << "Run Info to xml:" << rootname << endl;
  
   TString filename = rootname;
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

   outputFile << "<?xml-stylesheet type='text/xsl' href='" << xslPath.Data() << "'?>\n";
   outputFile << "<ROOT_SOURCE>\n";
  
   // Metadata
   int Tmd_entries = rootfile.GetTmdEntries(); 
   cout << "Number of Tmd entries: " << Tmd_entries << endl;
  
   UShort_t run_type_vect[Tmd_entries];
   UShort_t run_duration_vect[Tmd_entries];
   UShort_t orbitZone_vect[Tmd_entries];
  
  
   for(int j=0;j<Tmd_entries;j++)
   {

      rootfile.GetEntry(j);   

      run_type_vect[j] = metaData.run_type;
      run_duration_vect[j] = metaData.run_duration;
      orbitZone_vect[j] = metaData.orbit_Zone;
       
      outputFile << "<RUN_INFO>\n";
      
      outputFile << "\t<RUN_TYPE_error>"<< 0 << "</RUN_TYPE_error>\n";
      outputFile << "\t<RUN_DURATION_error>"<< 0 << "</RUN_DURATION_error>\n";
      outputFile << "\t<ORBIT_error>"<< 0 << "</ORBIT_error>\n";
       
      if (j%2!=0)
      {
	 if (run_type_vect[j]!=run_type_vect[j-1])
	    outputFile << "\t<RUN_TYPE_error>"<< 1 << "</RUN_TYPE_error>\n";
	  
	 if (run_duration_vect[j]!=run_duration_vect[j-1])
	    outputFile << "\t<RUN_DURATION_error>"<< 1 << "</RUN_DURATION_error>\n";
	         
	 if (orbitZone_vect[j]!=orbitZone_vect[j-1])
	    outputFile << "\t<ORBIT_error>"<< 1 << "</ORBIT_error>\n";
      }	     
     
      outputFile << "\t<BOOT_NR>"    << metaData.boot_nr  << "</BOOT_NR>\n";
      outputFile << "\t<RUN_NR>"     << metaData.run_id   << "</RUN_NR>\n";
      outputFile << "\t<RUN_TYPE>"   << run_type_vect[j]  << "</RUN_TYPE>\n";
      outputFile << "\t<RUN_DURATION>" << metaData.run_duration << "</RUN_DURATION>\n";
      outputFile << "\t<ORBIT_ZONE>"   << metaData.orbit_Zone  << "</ORBIT_ZONE>\n";

      outputFile << "</RUN_INFO>\n";
      
      
   }
   
   outputFile << "</ROOT_SOURCE>\n";
 
   outputFile.close();
}




void ScintConfigToXML(TString rootname, TString xslPath = "")
{
  
  cout << endl <<  "Scint to xml:" << rootname << endl;

  TString filename = rootname;
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
  cout << "Number of Tmd entries: " << Tmd_entries << endl;
  
  for(int j=0;j<Tmd_entries;j++)
    {
       rootfile.GetEntry(j);   
       
       if(j%2 == 0){

	outputFile << "<SCINTCONFIG>\n";
	outputFile << "\t<BOOT_NR>" << metaData.boot_nr << "</BOOT_NR>\n";
	outputFile << "\t<RUN_NR>"  << metaData.run_id  << "</RUN_NR>\n";
      
	outputFile << "\t<VETO_CONFIG>"  << (short)metaData.trigger_mask[0]
		   << "</VETO_CONFIG>\n";
	outputFile << "\t<CURRENT_TRIGGER_MASK>" << (short)metaData.trigger_mask[1]
		   << "</CURRENT_TRIGGER_MASK>\n";
	outputFile << "\t<EASIROC_1>" << (short)metaData.easiroc_config[0]
		   << "</EASIROC_1>\n";
	outputFile << "\t<EASIROC_2>" << (short)metaData.easiroc_config[60]
		   << "</EASIROC_2>\n";

       
	outputFile << "/<MASK_PMT_0>" << metaData.PMT_mask[0] << "</MASK_PMT_0>/n";
	outputFile << "/<MASK_PMT_1>" << metaData.PMT_mask[1] << "</MASK_PMT_1>/n";
	outputFile << "/<MASK_PMT_2>" << metaData.PMT_mask[2] << "</MASK_PMT_2>/n";
	outputFile << "/<MASK_PMT_3>" << metaData.PMT_mask[3] << "</MASK_PMT_3>/n";
	outputFile << "/<MASK_PMT_4>" << metaData.PMT_mask[4] << "</MASK_PMT_4>/n";
	outputFile << "/<MASK_PMT_5>" << metaData.PMT_mask[5] << "</MASK_PMT_5>/n";
	outputFile << "/<MASK_PMT_6>" << metaData.PMT_mask[6]  << "</MASK_PMT_6>/n";
	outputFile << "/<MASK_PMT_7>" << metaData.PMT_mask[7] << "</MASK_PMT_7>/n";
	outputFile << "/<MASK_PMT_8>" << metaData.PMT_mask[8]  << "</MASK_PMT_8>/n";
	outputFile << "/<MASK_PMT_9>" << metaData.PMT_mask[9]  << "</MASK_PMT_9>/n";
	outputFile << "/<MASK_PMT_10>" << metaData.PMT_mask[10]  << "</MASK_PMT_10>/n";
	outputFile << "/<MASK_PMT_11>" << metaData.PMT_mask[11]  << "</MASK_PMT_11>/n";
	outputFile << "/<MASK_PMT_12>" << metaData.PMT_mask[12]  << "</MASK_PMT_12>/n";
	outputFile << "/<MASK_PMT_13>" << metaData.PMT_mask[13]  << "</MASK_PMT_13>/n";
	outputFile << "/<MASK_PMT_14>" << metaData.PMT_mask[14]  << "</MASK_PMT_14>/n";
	outputFile << "/<MASK_PMT_15>" << metaData.PMT_mask[15]  << "</MASK_PMT_15>/n";
	outputFile << "/<MASK_PMT_16>" << metaData.PMT_mask[16]  << "</MASK_PMT_16>/n";
	outputFile << "/<MASK_PMT_17>" << metaData.PMT_mask[17]  << "</MASK_PMT_17>/n";
	outputFile << "/<MASK_PMT_18>" << metaData.PMT_mask[18]  << "</MASK_PMT_18>/n";
	outputFile << "/<MASK_PMT_19>" << metaData.PMT_mask[19]  << "</MASK_PMT_19>/n";
	outputFile << "/<MASK_PMT_20>" << metaData.PMT_mask[20]  << "</MASK_PMT_20>/n";
	outputFile << "/<MASK_PMT_21>" << metaData.PMT_mask[21] << "</MASK_PMT_21>/n";
	outputFile << "/<MASK_PMT_22>" << metaData.PMT_mask[22] << "</MASK_PMT_22>/n";
	outputFile << "/<MASK_PMT_23>" << metaData.PMT_mask[23] << "</MASK_PMT_23>/n";
	outputFile << "/<MASK_PMT_24>" << metaData.PMT_mask[24] << "</MASK_PMT_24>/n";
	outputFile << "/<MASK_PMT_25>" << metaData.PMT_mask[25] << "</MASK_PMT_25>/n";
	outputFile << "/<MASK_PMT_26>" << metaData.PMT_mask[26]  << "</MASK_PMT_26>/n";
	outputFile << "/<MASK_PMT_27>" << metaData.PMT_mask[27]  << "</MASK_PMT_27>/n";
	outputFile << "/<MASK_PMT_28>" << metaData.PMT_mask[28] << "</MASK_PMT_28>/n";
	outputFile << "/<MASK_PMT_29>" << metaData.PMT_mask[29] << "</MASK_PMT_29>/n";
	outputFile << "/<MASK_PMT_30>" << metaData.PMT_mask[30]  << "</MASK_PMT_30>/n";
	outputFile << "/<MASK_PMT_31>" << metaData.PMT_mask[31]  << "</MASK_PMT_31>/n";

	outputFile << "/<MASK2_PMT_0>" << metaData.PMT_mask[32] << "</MASK2_PMT_0>/n";
	outputFile << "/<MASK2_PMT_1>" << metaData.PMT_mask[33] << "</MASK2_PMT_1>/n";
	outputFile << "/<MASK2_PMT_2>" << metaData.PMT_mask[34] << "</MASK2_PMT_2>/n";
	outputFile << "/<MASK2_PMT_3>" << metaData.PMT_mask[35] << "</MASK2_PMT_3>/n";
	outputFile << "/<MASK2_PMT_4>" << metaData.PMT_mask[36] << "</MASK2_PMT_4>/n";
	outputFile << "/<MASK2_PMT_5>" << metaData.PMT_mask[37] << "</MASK2_PMT_5>/n";
	outputFile << "/<MASK2_PMT_6>" << metaData.PMT_mask[38] << "</MASK2_PMT_6>/n";
	outputFile << "/<MASK2_PMT_7>" << metaData.PMT_mask[39] << "</MASK2_PMT_7>/n";
	outputFile << "/<MASK2_PMT_8>" << metaData.PMT_mask[40] << "</MASK2_PMT_8>/n";
	outputFile << "/<MASK2_PMT_9>" << metaData.PMT_mask[41] << "</MASK2_PMT_9>/n";
	outputFile << "/<MASK2_PMT_10>" << metaData.PMT_mask[42]  << "</MASK2_PMT_10>/n";
	outputFile << "/<MASK2_PMT_11>" << metaData.PMT_mask[43]  << "</MASK2_PMT_11>/n";
	outputFile << "/<MASK2_PMT_12>" << metaData.PMT_mask[44] << "</MASK2_PMT_12>/n";
	outputFile << "/<MASK2_PMT_13>" << metaData.PMT_mask[45] << "</MASK2_PMT_13>/n";
	outputFile << "/<MASK2_PMT_14>" << metaData.PMT_mask[46]  << "</MASK2_PMT_14>/n";
	outputFile << "/<MASK2_PMT_15>" << metaData.PMT_mask[47] << "</MASK2_PMT_15>/n";
	outputFile << "/<MASK2_PMT_16>" << metaData.PMT_mask[48]  << "</MASK2_PMT_16>/n";
	outputFile << "/<MASK2_PMT_17>" << metaData.PMT_mask[49]  << "</MASK2_PMT_17>/n";
	outputFile << "/<MASK2_PMT_18>" << metaData.PMT_mask[50] << "</MASK2_PMT_18>/n";
	outputFile << "/<MASK2_PMT_19>" << metaData.PMT_mask[51] << "</MASK2_PMT_19>/n";
	outputFile << "/<MASK2_PMT_20>" << metaData.PMT_mask[52] << "</MASK2_PMT_20>/n";
	outputFile << "/<MASK2_PMT_21>" << metaData.PMT_mask[53] << "</MASK2_PMT_21>/n";
	outputFile << "/<MASK2_PMT_22>" << metaData.PMT_mask[54]  << "</MASK2_PMT_22>/n";
	outputFile << "/<MASK2_PMT_23>" << metaData.PMT_mask[55] << "</MASK2_PMT_23>/n";
	outputFile << "/<MASK2_PMT_24>" << metaData.PMT_mask[56] << "</MASK2_PMT_24>/n";
	outputFile << "/<MASK2_PMT_25>" << metaData.PMT_mask[57] << "</MASK2_PMT_25>/n";
	outputFile << "/<MASK2_PMT_26>" << metaData.PMT_mask[58] << "</MASK2_PMT_26>/n";
	outputFile << "/<MASK2_PMT_27>" << metaData.PMT_mask[59] << "</MASK2_PMT_27>/n";
	outputFile << "/<MASK2_PMT_28>" << metaData.PMT_mask[60] << "</MASK2_PMT_28>/n";
	outputFile << "/<MASK2_PMT_29>" << metaData.PMT_mask[61] << "</MASK2_PMT_29>/n";
	outputFile << "/<MASK2_PMT_30>" << metaData.PMT_mask[62]  << "</MASK2_PMT_30>/n";

        outputFile << "/<GEN_TRIG_MASK_0>" << -99999  << "</GEN_TRIG_MASK_0>/n";
	outputFile << "/<GEN_TRIG_MASK_1>" << -99999  << "</GEN_TRIG_MASK_1>/n";
	outputFile << "/<GEN_TRIG_MASK_2>" << -99999  << "</GEN_TRIG_MASK_2>/n";
	outputFile << "/<GEN_TRIG_MASK_3>" << -99999  << "</GEN_TRIG_MASK_3>/n";
	outputFile << "/<GEN_TRIG_MASK_4>" << -99999  << "</GEN_TRIG_MASK_4>/n";
	outputFile << "/<GEN_TRIG_MASK_5>" << -99999  << "</GEN_TRIG_MASK_5>/n";
	outputFile << "/<GEN_TRIG_MASK_6>" << -99999  << "</GEN_TRIG_MASK_6>/n";
	outputFile << "/<GEN_TRIG_MASK_7>" << -99999  << "</GEN_TRIG_MASK_7>/n";
	outputFile << "/<GEN_TRIG_MASK_8>" << -99999  << "</GEN_TRIG_MASK_8>/n";
	outputFile << "/<GEN_TRIG_MASK_9>" << -99999  << "</GEN_TRIG_MASK_9>/n";
	outputFile << "/<GEN_TRIG_MASK_10>" << -99999  << "</GEN_TRIG_MASK_10>/n";
	outputFile << "/<GEN_TRIG_MASK_11>" << -99999  << "</GEN_TRIG_MASK_11>/n";
	outputFile << "/<GEN_TRIG_MASK_12>" << -99999  << "</GEN_TRIG_MASK_12>/n";
	outputFile << "/<GEN_TRIG_MASK_13>" << -99999  << "</GEN_TRIG_MASK_13>/n";
	outputFile << "/<GEN_TRIG_MASK_14>" << -99999  << "</GEN_TRIG_MASK_14>/n";
	outputFile << "/<GEN_TRIG_MASK_15>" << -99999  << "</GEN_TRIG_MASK_15>/n";
	outputFile << "/<GEN_TRIG_MASK_16>" << -99999  << "</GEN_TRIG_MASK_16>/n";
	outputFile << "/<GEN_TRIG_MASK_17>" << -99999  << "</GEN_TRIG_MASK_17>/n";

	outputFile << "</SCINTCONFIG>\n";
      }   
  
    }
  
  outputFile << "</ROOT_SOURCE>\n";
  outputFile.close();
}


void SilConfigToXML(TString rootname, TString xslPath = "")
{
  
  cout << endl << "Sil to xml:" << rootname << endl;

  TString filename = rootname;
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
  cout << "Number of Runs " << Tmd_entries << endl;
  
  Short_t ladder_on_vect[Tmd_entries];
  Short_t ladder_mask_vect[Tmd_entries];
  UShort_t adj_strip_vect[Tmd_entries];
  UShort_t zero_supp_vect[Tmd_entries];
  Short_t CN_high_vect[Tmd_entries];
  Short_t CN_low_vect[Tmd_entries];
  UShort_t event_RMS_vect[Tmd_entries];
  UShort_t  event_ped_vect[Tmd_entries];
  UShort_t event_gauss_vect[Tmd_entries];
  UShort_t event_CN_vect[Tmd_entries];
  UShort_t gauss_check_vect[Tmd_entries];

  for(int j=0;j<Tmd_entries;j++)
    {
       rootfile.GetEntry(j);   

      ladder_on_vect[j] = (short)metaData.silConfig.ladder_on;
      ladder_mask_vect[j] = (short)metaData.silConfig.ladder_mask;
      adj_strip_vect[j] = metaData.silConfig.adj_strip;
      zero_supp_vect[j] = metaData.silConfig.zero_supp_thrd;
      CN_high_vect[j] = (short)metaData.silConfig.thrd_CN_HIGH;
      CN_low_vect[j] = (short)metaData.silConfig.thrd_CN_LOW;
      event_RMS_vect[j] = metaData.silConfig.calib_event_RMS;
      event_ped_vect[j] = metaData.silConfig.calib_event_ped;
      event_gauss_vect[j] = metaData.silConfig.calib_event_gauss;
      event_CN_vect[j] = metaData.silConfig.calib_event_CN;
      gauss_check_vect[j] = metaData.silConfig.gauss_check;

	
      outputFile << "<SILCONFIG>\n";
      
      outputFile << "\t<BOOT_NR>" << metaData.boot_nr << "</BOOT_NR>\n";
      outputFile << "\t<RUN_NR>"  << metaData.run_id  << "</RUN_NR>\n";


       outputFile << "\t<LADDER_error>"<< 0 << "</LADDER_error>\n";
       outputFile << "\t<LADDER_MASK_error>"<< 0 << "</LADDER_MASK_error>\n";
       outputFile << "\t<ADJ_STRIP_error>"<< 0 << "</ADJ_STRIP_error>\n";
       outputFile << "\t<ZERO_SUPP_error>"<< 0 << "</ZERO_SUPP_error>\n";
       outputFile << "\t<CN_HIGH_error>"<< 0 << "</CN_HIGH_error>\n";
       outputFile << "\t<CN_LOW_error>"<< 0 << "</CN_LOW_error>\n";
       outputFile << "\t<EVENT_RMS_error>"<< 0 << "</EVENT_RMS_error>\n";
       outputFile << "\t<EVENT_PED_error>"<< 0 << "</EVENT_PED_error>\n";
       outputFile << "\t<EVENT_GAUSS_error>"<< 0 << "</EVENT_GAUSS_error>\n";
       outputFile << "\t<EVENT_CN_error>"<< 0 << "</EVENT_CN_error>\n";
       outputFile << "\t<GAUSS_CHECK_error>"<< 0 << "</GAUSS_CHECK_error>\n";
       
       
      if (j%2!=0)
	{
	  if (ladder_on_vect[j]!=ladder_on_vect[j-1])
	      outputFile << "\t<LADDER_error>"<< 1 << "</LADDER_error>\n";

	   if (ladder_mask_vect[j]!=ladder_mask_vect[j-1])
	      outputFile << "\t<LADDER_MASK_error>"<< 1 << "</LADDER_MASK_error>\n";
	   
	   if (adj_strip_vect[j]!=adj_strip_vect[j-1])
	      outputFile << "\t<ADJ_STRIP_error>"<< 1 << "</ADJ_STRIP_error>\n";

	   if (zero_supp_vect[j]!=zero_supp_vect[j-1])
	      outputFile << "\t<ZERO_SUPP_error>"<< 1 << "</ZERO_SUPP_error>\n";

	   if (CN_high_vect[j]!=CN_high_vect[j-1])
	      outputFile << "\t<CN_HIGH_error>"<< 1 << "</CN_HIGH_error>\n";

	    if (CN_low_vect[j]!=CN_low_vect[j-1])
	      outputFile << "\t<CN_LOW_error>"<< 1 << "</CN_LOW_error>\n";

	    if (event_RMS_vect[j]!=event_RMS_vect[j-1])
	      outputFile << "\t<EVENT_RMS_error>"<< 1 << "</EVENT_RMS_error>\n";

	    if (event_ped_vect[j]!=event_ped_vect[j-1])
	      outputFile << "\t<EVENT_PED_error>"<< 1 << "</EVENT_PED_error>\n";

	    if (event_gauss_vect[j]!=event_gauss_vect[j-1])
	      outputFile << "\t<EVENT_GAUSS_error>"<< 1 << "</EVENT_GAUSS_error>\n";

	    if (event_CN_vect[j]!=event_CN_vect[j-1])
	      outputFile << "\t<EVENT_CN_error>"<< 1 << "</EVENT_CN_error>\n";

	     if (gauss_check_vect[j]!=gauss_check_vect[j-1])
	      outputFile << "\t<GAUSS_CHECK_error>"<< 1 << "</GAUSS_CHECK_error>\n";
	}
	           

      if((short)metaData.silConfig.ladder_on == 0)
	outputFile << "\t<LADDERS_LV>" << "OFF" << "</LADDERS_LV>\n";

      if((short)metaData.silConfig.ladder_on == 1)
	outputFile << "\t<LADDERS_LV>" << "ON" << "</LADDERS_LV>\n";
      
      outputFile << "\t<CURRENT_SD>" << (short)metaData.silConfig.ladder_mask
		 << "</CURRENT_SD>\n";
      
      outputFile << "\t<ADIACENT_STRIP>" << metaData.silConfig.adj_strip
		 << "</ADIACENT_STRIP>\n";
      outputFile << "\t<ZERO_SUPP_THRD>" << metaData.silConfig.zero_supp_thrd
		 << "</ZERO_SUPP_THRD>\n";
      outputFile << "\t<THRD_CN_HIGH>"  << (short)metaData.silConfig.thrd_CN_HIGH
		 << "</THRD_CN_HIGH>\n";
      outputFile << "\t<THRD_CN_LOW>"   << (short)metaData.silConfig.thrd_CN_LOW
		 << "</THRD_CN_LOW>\n";
      outputFile << "\t<CALIB_EVENT_RMS>" << metaData.silConfig.calib_event_RMS
		 << "</CALIB_EVENT_RMS>\n";
      outputFile << "\t<CALIB_EVENT_PED>" << metaData.silConfig.calib_event_ped
		 << "</CALIB_EVENT_PED>\n";
      outputFile << "\t<CALIB_EVENT_GAUSS>" << metaData.silConfig.calib_event_gauss
		 << "</CALIB_EVENT_GAUSS>\n";
      outputFile << "\t<CALIB_EVENT_CN>" << metaData.silConfig.calib_event_CN
		 << "</CALIB_EVENT_CN>\n";
      outputFile << "\t<GAUSS_CHECK>" << metaData.silConfig.gauss_check
		 << "</GAUSS_CHECK>\n";

      outputFile << "</SILCONFIG>\n";    
  
    }
  outputFile << "</ROOT_SOURCE>\n";
  outputFile.close();
}


void TelemetryToXML(TString rootname, TString xslPath = "")
{
  
  cout << endl << "Telemetry to xml:" << rootname << endl;
  
  TString filename = rootname;
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
    cout << " k_start: " << k_start  << endl;
    cout << " k_stop " << k_stop  << endl;   
  }
  
  else  {

    k_start=((Tmd_entries-1)/2)+1;
    k_stop=(Tmd_entries-1)/2;   
    cout << " k_start: " << k_start  << endl;
    cout << " k_stop " << k_stop  << endl;  
  }
  
  UShort_t vect_start_status_DAQ[k_start];
  UShort_t vect_start_status_PMT[k_start];
  UShort_t vect_start_status_TM_TC[k_start];
  UShort_t vect_start_status_HVPS[k_start];
  UShort_t vect_start_status_CPU[k_start];
  UShort_t vect_stop_status_DAQ[k_stop];
  UShort_t vect_stop_status_PMT[k_stop];
  UShort_t vect_stop_status_TM_TC[k_stop];
  UShort_t vect_stop_status_HVPS[k_stop];
  UShort_t vect_stop_status_CPU[k_stop];
  UShort_t vect_boot_nr[k_start];
  UShort_t vect_run_id[k_start];

  cout << "Number of Tmd entries: " << Tmd_entries << endl;
   
  for(int j=0;j<Tmd_entries;j++)
    {
       rootfile.GetEntry(j);   


      if(j%2 == 0){    
	vect_start_status_DAQ[start_ind] = metaData.status_reg.statusDAQ;
	vect_start_status_PMT[start_ind] = metaData.status_reg.statusPMT;
	vect_start_status_TM_TC[start_ind] = metaData.status_reg.statusTM_TC;
	vect_start_status_HVPS[start_ind] = metaData.status_reg.statusHV_PS;
	vect_start_status_CPU[start_ind] = metaData.status_reg.CPU_board_boot;
	vect_boot_nr[start_ind] = metaData.boot_nr;
	vect_run_id[start_ind]= metaData.run_id;
       
	start_ind=start_ind+1;	  
      }      
      else {
        vect_stop_status_DAQ[stop_ind] = metaData.status_reg.statusDAQ;
        vect_stop_status_PMT[stop_ind] = metaData.status_reg.statusPMT;
        vect_stop_status_TM_TC[stop_ind] = metaData.status_reg.statusTM_TC;
        vect_stop_status_HVPS[stop_ind] = metaData.status_reg.statusHV_PS;
        vect_stop_status_CPU[stop_ind] = metaData.status_reg.CPU_board_boot;
          
	stop_ind=stop_ind+1;
	  
      } 	 
    
    } 

  
  for(int t=0;t<k_start;t++)
    {

      outputFile << dec << "\t<DAQ_STOP_error>"     << 0 << "</DAQ_STOP_error>\n";
      outputFile << dec << "\t<PMT_STOP_error>"     << 0 << "</PMT_STOP_error>\n";
      outputFile << dec << "\t<TM_TC_STOP_error>"   << 0 << "</TM_TC_STOP_error>\n";
      outputFile << dec << "\t<HVPS_STOP_error>"    << 0 << "</HVPS_STOP_error>\n";
      outputFile << dec << "\t<CPU_STOP_error>"     << 0 << "</CPU_STOP_error>\n";
           
      outputFile << "<TELEMETRY>\n";

      outputFile << "\t<DAQ_START_error>"     <<  0 << "</DAQ_START_error>\n";
      outputFile <<  "\t<PMT_START_error>"    <<  0 << "</PMT_START_error>\n";
      outputFile <<  "\t<TM_TC_START_error>"  <<  0 << "</TM_TC_START_error>\n";
      outputFile << "\t<HVPS_START_error>"    <<  0 << "</HVPS_START_error>\n";
      outputFile << "\t<CPU_START_error>"     <<  0 << "</CPU_START_error>\n";

      outputFile << dec <<  "\t<BOOT_NR>" <<   vect_boot_nr[t] << "</BOOT_NR>\n";
      outputFile << dec << "\t<RUN_ID>"  <<   vect_run_id[t] << "</RUN_ID>\n";  

      if (vect_start_status_DAQ[t]!=20641)
	outputFile <<  "\t<DAQ_START_error>" << 1 << "</DAQ_START_error>\n";

      if (vect_start_status_PMT[t]!=3333)
	outputFile <<  "\t<PMT_START_error>" << 1 << "</PMT_START_error>\n";

      if (vect_start_status_TM_TC[t]!=4093)
	outputFile <<  "\t<TM_TC_START_error>" << 1 << "</TM_TC_START_error>\n";

      if (vect_start_status_HVPS[t] !=0)
	outputFile << "\t<HVPS_START_error>"  <<  1 << "</HVPS_START_error>\n";

      if (vect_start_status_CPU[t]!=1796)
	outputFile << "\t<CPU_START_error>"   <<  1 << "</CPU_START_error>\n";

      outputFile << showbase << hex << uppercase << "\t<DAQ_START>"
		 << vect_start_status_DAQ[t] << "</DAQ_START>\n";
      outputFile << showbase << hex << "\t<PMT_START>" <<
	 vect_start_status_PMT[t]  << "</PMT_START>\n";
      outputFile << showbase << hex << "\t<TM_TC_START>" <<
	 vect_start_status_TM_TC[t] << "</TM_TC_START>\n";
      outputFile << hex << "\t<HVPS_START>" <<   vect_start_status_HVPS[t]
		 << "</HVPS_START>\n";
      outputFile << hex << "\t<CPU_START>" <<   vect_start_status_CPU[t]
		 << "</CPU_START>\n";
    
      if (vect_stop_status_DAQ[t]!=32931)
	outputFile << dec << "\t<DAQ_STOP_error>" << 1  << "</DAQ_STOP_error>\n";
	  
      if (vect_stop_status_PMT[t]!=3871)
	outputFile <<dec <<  "\t<PMT_STOP_error>" <<  1 << "</PMT_STOP_error>\n";

      if (vect_stop_status_TM_TC[t]!=4093)
	outputFile << dec <<  "\t<TM_TC_STOP_error>"  <<  1 << "</TM_TC_STOP_error>\n";

      if (vect_stop_status_HVPS[t]!=0)
	outputFile << dec << "\t<HVPS_STOP_error>" <<  1 << "</HVPS_STOP_error>\n";

      if (vect_stop_status_CPU[t]!=1796)
	outputFile << dec << "\t<CPU_STOP_error>" << 1 << "</CPU_STOP_error>\n";
    
      if (t < k_stop)
	{
	  outputFile << hex << "\t<DAQ_STOP>"  <<  vect_stop_status_DAQ[t]
		     << "</DAQ_STOP>\n";
	  outputFile << hex << "\t<PMT_STOP>" << vect_stop_status_PMT[t]
		     << "</PMT_STOP>\n";
	  outputFile << hex << "\t<TM_TC_STOP>" << vect_stop_status_TM_TC[t]
		     << "</TM_TC_STOP>\n";
	  outputFile << hex << "\t<HVPS_STOP>" << vect_stop_status_HVPS[t]
		     << "</HVPS_STOP>\n";
	  outputFile << hex << "\t<CPU_STOP>"  << vect_stop_status_CPU[t]
		     << "</CPU_STOP>\n";	  
	}
      
      outputFile << "</TELEMETRY>\n";   
   
    }

  
  outputFile << "</ROOT_SOURCE>\n";
  outputFile.close();

}
