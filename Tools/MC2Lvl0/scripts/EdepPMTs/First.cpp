void First()
{
	MyAnalyzer* anal = new MyAnalyzer();
	anal->SetFileChain("fichiers_test.txt");
	//anal->Edep_Plots();
	anal->LVL0Converter();
}	
