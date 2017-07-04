// These values for the HG of "E60MeV_RUN03_HOT" made through kde procedure

#include <string>
#include <vector>
#include "MCcoorPhysicalFrame.hh"

struct meansig {float mean; float sigma;};
typedef std::map<std::string, meansig> pedmap;



std::vector<meansig> CaloPMTpeds= {
 {417.075107296, 4.44430755278 },
 {417.794486216, 18.7611786473 },
 {409.670555296, 13.4624035694 },
 {424.696521739, 9.72433460726 },
 {409.079461572, 3.69943597656 },
 {436.115908507, 37.6078547507 },
 {405.629750271, 10.441610474  },
 {403.936660929, 10.9351136736 },
 {451.898976212, 38.1755071812 },
 {416.856327307, 10.8200093663 },
 {423.080466647, 11.3118490243 },
 {430.63592233 , 8.80742640294 },
 {414.006922258, 10.9429713003 },
 {417.134524696, 10.825623057  },
 {412.918398939, 15.2223886623 },
 {415.033935743, 13.0663680058 },
 {421.667592776, 42.5341304835 },
 {415.062105088, 48.4013268853 },
 {411.013985349, 53.3247928358 },
 {324.455791962, 4.0490267662  },
 {408.069672495, 52.0322560305 },
 {405.847932669, 53.363933793  },
 {319.73877709 , 4.27429653726 },
 {307.449328006, 4.34153866244 },
 {314.579040226, 4.32569561469 },
 {394.501043297, 59.3696789615 },
 {308.164325384, 4.11832282908 },
 {316.59333539 , 3.98206469823 },
 {322.148726852, 4.19419911896 },
 {314.766585432, 4.27217765712 },
 {303.503729355, 3.79265644169 },
 {311.577367719, 4.53403648977 }
};


std::vector<std::string> CaloPMTnames= {
"P1se", "P2sw", "P3se", "P4sw", "P5se", "P6sw", "P7se", "P8sw", "P9se", "P10sw", "P11se", "P12sw", "P13se", "P14sw", "P15se", "P16sw",
"P1nw", "P2ne", "P3nw", "P4ne", "P5nw", "P6ne", "P7nw", "P8ne", "P9nw", "P10ne", "P11nw", "P12ne", "P13nw", "P14ne", "P15nw", "P16ne"};

pedmap getPMThgpeds () {
   pedmap map;
   for (int i=0; i<CaloPMTnames.size(); i++) {
         map[CaloPMTnames[i]]=CaloPMTpeds[i];
   }
   return map;
}



typedef std::map<std::string, TVector3> posmap;

posmap getPosMap () {
   std::vector<float> PMTzMC={312.2,297.42,282.64,267.86,253.08,238.3,223.52,208.74,193.96,179.18,164.814,150.51,136.241,121.944,107.659,93.3685};
   posmap map;
   uint npmt=CaloPMTnames.size();
   for (int i=0; i<npmt; i++) {
      float x=(i%2 == 0)?8.25:-8.25;
      float y=(i < npmt/2)?8.25:-8.25;
      TVector3 vecz=MCtoPhysicalDetectorFrame(TVector3(0, 0, PMTzMC[i]));
      float z=vecz.z();
      map[CaloPMTnames[i]]=TVector3(x, y, z);
   }
   return map;
}
