// These values for the HG of "E60MeV_RUN03_HOT" made through kde procedure

#include <string>
#include <vector>
#include "MCcoorPhysicalFrame.hh"

const  std::vector<std::string> PMTID=
{"T1e", "T2e", "T3e", "T4e", "T5e", "T6e",
"P1se", "P2sw", "P3se", "P4sw", "P5se", "P6sw", "P7se", "P8sw",
"P9se", "P10sw", "P11se", "P12sw", "P13se", "P14sw", "P15se", "P16sw",
"VNu", "VEu", "VSu", "VWu", "VBne", "L9sw", "L7nw", "L1ne", "L8w", "L5c",
"T1w", "T2w", "T3w", "T4w", "T5w", "T6w",
"P1nw", "P2ne", "P3nw", "P4ne", "P5nw", "P6ne", "P7nw", "P8ne",
"P9nw", "P10ne", "P11nw", "P12ne", "P13nw", "P14ne", "P15nw", "P16ne",
"VNd", "VEd", "VSd", "VWd", "VBsw", "L3se", "L2e", "L6s", "L4n", "NC"};




struct meansig {float mean; float sigma;};
typedef std::map<std::string, meansig> pedmap;



pedmap CaloPMTpedsHG={
{"T1e"  ,  { 412.881081 ,  5.093883 }},
{"T2e"  ,  { 414.176163 ,  4.326234 }},
{"T3e"  ,  { 415.425746 ,  4.121837 }},
{"T4e"  ,  { 407.145891 ,  4.621238 }},
{"T5e"  ,  { 425.449938 ,  6.050231 }},
{"T6e"  ,  { 428.115108 ,  4.650233 }},
{"P1se" ,  { 430.355283 , 16.186345 }},
{"P2sw" ,  { 417.464629 ,  6.610456 }},
{"P3se" ,  { 412.608072 ,  9.512528 }},
{"P4sw" ,  { 430.881204 ,  6.871393 }},
{"P5se" ,  { 428.139805 ,  5.186284 }},
{"P6sw" ,  { 416.522330 ,  5.849863 }},
{"P7se" ,  { 409.647628 ,  5.147244 }},
{"P8sw" ,  { 410.832036 ,  6.252032 }},
{"P9se" ,  { 418.328410 ,  6.430319 }},
{"P10sw",  { 423.420672 ,  6.879046 }},
{"P11se",  { 430.422299 ,  7.082736 }},
{"P12sw",  { 438.338769 ,  6.347400 }},
{"P13se",  { 421.560884 ,  6.770046 }},
{"P14sw",  { 423.465648 ,  9.530436 }},
{"P15se",  { 417.392465 ,  5.913402 }},
{"P16sw",  { 420.224073 ,  6.112400 }},
{"VNu"  ,  { 424.774791 ,  6.582503 }},
{"VEu"  ,  { 429.370026 ,  6.455723 }},
{"VSu"  ,  { 428.145046 , 13.088752 }},
{"VWu"  ,  { 429.907221 , 18.225902 }},
{"VBne" ,  { 423.299925 ,  6.750211 }},
{"L9sw" ,  { 438.722418 , 19.493861 }},
{"L7nw" ,  { 422.969108 , 15.874327 }},
{"L1ne" ,  { 415.129171 , 25.826501 }},
{"L8w"  ,  { 413.458845 , 11.736555 }},
{"L5c"  ,  { 430.865169 ,  7.090569 }},
{"T1w"  ,  { 312.678994 ,  5.263946 }},
{"T2w"  ,  { 318.003454 ,  4.578126 }},
{"T3w"  ,  { 310.250425 ,  4.322035 }},
{"T4w"  ,  { 307.053455 ,  4.358171 }},
{"T5w"  ,  { 315.675663 ,  4.549825 }},
{"T6w"  ,  { 309.535722 ,  4.305248 }},
{"P1nw" ,  { 402.542064 , 65.379367 }},
{"P2ne" ,  { 328.774567 ,  9.286540 }},
{"P3nw" ,  { 321.547513 ,  7.840695 }},
{"P4ne" ,  { 332.553483 ,  7.773640 }},
{"P5nw" ,  { 328.824463 ,  7.317108 }},
{"P6ne" ,  { 326.966478 ,  7.821378 }},
{"P7nw" ,  { 327.450339 ,  6.574737 }},
{"P8ne" ,  { 318.161227 ,  8.743401 }},
{"P9nw" ,  { 324.454721 ,  8.411402 }},
{"P10ne",  { 315.770293 ,  7.612869 }},
{"P11nw",  { 321.138785 ,  8.858730 }},
{"P12ne",  { 328.406985 ,  8.270837 }},
{"P13nw",  { 334.606293 ,  8.780044 }},
{"P14ne",  { 325.424411 ,  6.694034 }},
{"P15nw",  { 312.662737 ,  6.885861 }},
{"P16ne",  { 320.643009 ,  5.980882 }},
{"VNd"  ,  { 329.124665 ,  7.170878 }},
{"VEd"  ,  { 337.051129 ,  6.779592 }},
{"VSd"  ,  { 331.479400 ,  6.541011 }},
{"VWd"  ,  { 347.221667 ,  6.750870 }},
{"VBsw" ,  { 341.079814 ,  7.080330 }},
{"L3se" ,  { 331.311224 ,  6.669596 }},
{"L2e"  ,  { 334.087855 ,  7.406568 }},
{"L6s"  ,  { 336.917577 ,  6.948810 }},
{"L4n"  ,  { 337.570442 ,  5.134804 }},
{"NC"   ,  { 331.487573 ,  6.142241 }}
};

pedmap CaloPMTpedsLG={
{"T1e"  , {399.008924 ,  3.268237 }},
{"T2e"  , {399.584691 ,  3.384526 }},
{"T3e"  , {522.220569 , 53.151085 }},
{"T4e"  , {392.300114 ,  3.097322 }},
{"T5e"  , {418.923692 ,  2.974240 }},
{"T6e"  , {400.779195 ,  3.118238 }},
{"P1se" , {432.192769 ,  6.757755 }},
{"P2sw" , {430.285032 , 11.007667 }},
{"P3se" , {431.893790 ,  8.433011 }},
{"P4sw" , {442.374399 , 17.982996 }},
{"P5se" , {403.787713 ,  2.949230 }},
{"P6sw" , {426.182138 , 13.230503 }},
{"P7se" , {447.339483 , 24.364488 }},
{"P8sw" , {434.475100 , 21.708419 }},
{"P9se" , {420.429008 , 13.965482 }},
{"P10sw", {435.153799 , 20.107288 }},
{"P11se", {439.867632 , 18.227464 }},
{"P12sw", {443.912112 , 14.300461 }},
{"P13se", {446.087428 , 22.311186 }},
{"P14sw", {463.972819 , 24.098870 }},
{"P15se", {467.835340 , 29.908638 }},
{"P16sw", {459.237594 , 33.453373 }},
{"VNu"  , {395.663711 ,  4.076380 }},
{"VEu"  , {396.964784 ,  3.217283 }},
{"VSu"  , {416.334958 ,  3.104289 }},
{"VWu"  , {396.684153 ,  2.932975 }},
{"VBne" , {402.246254 ,  3.154824 }},
{"L9sw" , {401.651129 ,  3.065235 }},
{"L7nw" , {397.538343 ,  3.182374 }},
{"L1ne" , {407.914169 ,  3.105616 }},
{"L8w"  , {392.045656 ,  3.218268 }},
{"L5c"  , {411.536058 ,  3.010167 }},
{"T1w"  , {304.345074 ,  3.609370 }},
{"T2w"  , {300.802398 , 19.557175 }},
{"T3w"  , {355.291697 ,  8.634542 }},
{"T4w"  , {304.661661 ,  3.563051 }},
{"T5w"  , {298.446013 ,  3.482449 }},
{"T6w"  , {309.383422 ,  3.482624 }},
{"P1nw" , {335.841188 , 13.207743 }},
{"P2ne" , {328.413679 , 15.605322 }},
{"P3nw" , {370.772189 , 24.145603 }},
{"P4ne" , {359.434118 , 20.855813 }},
{"P5nw" , {335.937379 , 10.226122 }},
{"P6ne" , {327.667156 ,  9.556422 }},
{"P7nw" , {347.850633 , 20.023048 }},
{"P8ne" , {359.138266 , 22.355495 }},
{"P9nw" , {341.254559 , 19.109021 }},
{"P10ne", {342.170585 , 18.816923 }},
{"P11nw", {380.749801 , 22.141895 }},
{"P12ne", {343.777956 , 18.753798 }},
{"P13nw", {358.998911 , 24.947332 }},
{"P14ne", {384.513106 , 32.911301 }},
{"P15nw", {364.697157 ,  8.446660 }},
{"P16ne", {395.070524 , 38.498187 }},
{"VNd"  , {311.771857 ,  3.560220 }},
{"VEd"  , {314.996470 , 18.185264 }},
{"VSd"  , {308.885617 ,  3.508393 }},
{"VWd"  , {314.055274 ,  3.325341 }},
{"VBsw" , {308.673573 ,  3.521295 }},
{"L3se" , {313.814653 ,  3.519594 }},
{"L2e"  , {318.283583 ,  3.499119 }},
{"L6s"  , {298.484384 ,  9.275479 }},
{"L4n"  , {308.245027 ,  3.764058 }},
{"NC"   , {309.820007 ,  4.063671 }},
};



std::vector<std::string> CaloPMTnames= {
"P1se", "P2sw", "P3se", "P4sw", "P5se", "P6sw", "P7se", "P8sw", "P9se", "P10sw", "P11se", "P12sw", "P13se", "P14sw", "P15se", "P16sw",
"P1nw", "P2ne", "P3nw", "P4ne", "P5nw", "P6ne", "P7nw", "P8ne", "P9nw", "P10ne", "P11nw", "P12ne", "P13nw", "P14ne", "P15nw", "P16ne"};



std::vector<std::string> ScintPlanesNames () {
   std::vector<std::string> planenames;
   for (int ns=0; ns<2; ns++)
      for (int il=CaloPMTnames.size()/2-1; il>=0; il--) {
         std::string offset="Scintillator Layer[ Z";
         if (il > 0 ) offset+="+"+std::to_string(il);
         planenames.push_back(offset+" ]");

      }
   //for (auto v : planenames) std::cout << v << std::endl;
   return planenames;
}





typedef std::map<std::string, TVector3> posmap;

posmap getPosMap () {
   std::vector<float> PMTzMC={312.2,297.42,282.64,267.86,253.08,238.3,223.52,208.74,193.96,179.18,164.814,150.51,136.241,121.944,107.659,93.3685};

   posmap map;
   uint npmt=CaloPMTnames.size();
   for (int i=0; i<npmt; i++) {
      float x=(i%2 == 0)?82.5:-82.5; //mm
      float y=(i < npmt/2)?82.5:-82.5; //mm
      TVector3 vecz=MCtoPhysicalDetectorFrame(TVector3(0, 0, PMTzMC[i]));
      float z=vecz.z();
      map[CaloPMTnames[i]]=TVector3(x, y, z);
   }
   return map;
}
