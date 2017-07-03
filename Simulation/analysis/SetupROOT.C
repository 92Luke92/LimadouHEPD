/*
SetupRoot.C allows to load the HEPDSW library, the source files related to the detector and all the useful analysis files.
The latter allow to recall all the contained functions typing simply the name of them.
In ROOT, SetupRoot.C can be activated by typing ".X SetupRoot.C"
*/

{
gSystem->Load("../lib/libhepdsw.so");	// for Linux users!
//gSystem->Load("../lib/libhepdsw.dylib");	// for Mac OSX users!	
gSystem->AddIncludePath(" -I../source/Dataformats/include");

gROOT->LoadMacro("Analyzer.C");
gROOT->LoadMacro("aiglong4_anal.C");
gROOT->LoadMacro("aiglon_g4_hist.C");
gROOT->LoadMacro("aiglon_g4_Bethe.C");
}
