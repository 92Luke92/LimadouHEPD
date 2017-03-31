#include <iostream>
#include "LTrackerQuicklook.hh"
#include <stdlib.h>
#include <TSystem.h>


int main(int argc, char **argv){

   std::cout << "Running quicklook: " << std::endl;

   TString xslPath = "./";
   TString outDir = "./";
   
   TString broad1;
   TString broad2;
   TString cpu;
   TString hvps;
   TString run;
   TString scint;
   TString sil;
   TString tel;


   if (argc < 2)
   {
      PrintHelp();
      return -1;
   }
   else
      if (!strcmp(argv[1], "--help") ||
	  !strcmp(argv[1], "-h")        )
      {
	 PrintHelp();
	 return -1;
      }
   
   for (int i = 2; i < argc; i++)
   {
    
      if (!strcmp(argv[i], "-x"))
      {
	 if (++i >= argc)
	 {
	    std::cout <<  "\n-x needs arguments." << std::endl;
	    PrintHelp();
	    return -1;
	 } else {
	    xslPath = argv[i];
	    continue;
	 }
      }
      // if (!strcmp(argv[i], "-o"))
      // {
      // 	 if (++i >= argc)
      // 	 {
      // 	    std::cout <<  "\n-o needs arguments." << std::endl;
      // 	    PrintHelp();
      // 	    return -1;
      // 	 } else {
      // 	    outDir = argv[i];
      // 	    continue;
      // 	 }
      // }

   }

   std::cout << "xsl path = " << xslPath << std::endl;
   std::cout << "output dir = " << outDir << std::endl;

   if (TrackerQuickLook(argv[1]) == -1 )
      std::cout << "Not Virgin run: " << std::endl;
   
   TriggerScan(argv[1]);
   PMTScan(argv[1]);

   broad1 = xslPath+"TimeBroadcastTemplate.xsl";
   broad2 = xslPath+"GPSBroadcastTemplate.xsl";

   cpu = xslPath+"CPUTimeTempTemplate.xsl";
   hvps = xslPath+"HVPSConfigTemplate.xsl";
   run = xslPath+"RunInfoTemplate.xsl";
   scint = xslPath+"ScintConfigTemplate.xsl";
   sil = xslPath+"SilConfigTemplate.xsl";
   tel = xslPath+"TelemetryTemplate.xsl";

   BroadcastToXML(argv[1], broad1, broad2);
   CPUTimeTempToXML(argv[1], cpu);
   HVPSConfigToXML(argv[1], hvps);
   RunInfoToXML(argv[1], run);
   ScintConfigToXML(argv[1], scint);
   SilConfigToXML(argv[1], sil);
   TelemetryToXML(argv[1], tel);
           
   
  return 0; 
}


void PrintHelp(void)
{
   std::cout << "\nUsage: ./QuickLook <root file directory>  -x <xsl folder>\n" << std::endl;
   std::cout <<  "\t -h or --help print this help and exit \n" << std::endl;
   std::cout << "\t -x <path> set the path to xsl template files [default ./] "
	     << std::endl;
   //std::cout << "\t -o <path> set the path for the output files [default ./]" << std::endl;
}

