#include <string>
#include <map>


// 1st digit -> detector (1 = CALO, 2 = Tracker)
// 2nd digit -> detector type (1 = cristals, 2 = scint. planes, 3 = S1, 4 = veto)
// 3rd, 4th  -> sensitive volume (plane, cristal, paddle) inside a detector type
// Coord indicated = orientation chosed for simulation
// Axis unreported : plane considered centered with respect to this axis
// With axis origin the external wall of the rear veto, Z + n corrresponds to greater Z if n>0

const  std::map<int, std::string> detectorID = {

{1111, "LYSO Crystal [ -X, -Y ]"},
{1112, "LYSO Crystal [ 0, -Y ]"},
{1113, "LYSO Crystal [ X, -Y ]"},
{1121, "LYSO Crystal [ -X, 0 ]"},
{1122, "LYSO Crystal [ 0, 0 ]"},
{1123, "LYSO Crystal [ X, 0 ]"},
{1131, "LYSO Crystal [ -X, Y ]"},
{1132, "LYSO Crystal [ 0, Y ]"},
{1133, "LYSO Crystal [ X, Y ]"},

{1201, "Scintillator Layer [ Z ]"},
{1202, "Scintillator Layer[ Z+1 ]"},
{1203, "Scintillator Layer[ Z+2 ]"},
{1204, "Scintillator Layer[ Z+3 ]"},
{1205, "Scintillator Layer[ Z+4 ]"},
{1206, "Scintillator Layer[ Z+5 ]"},
{1207, "Scintillator Layer[ Z+6 ]"},
{1208, "Scintillator Layer[ Z+7 ]"},
{1209, "Scintillator Layer[ Z +8]"},
{1210, "Scintillator Layer[ Z+9 ]"},
{1211, "Scintillator Layer[ Z+10 ]"},
{1212, "Scintillator Layer[ Z+11 ]"},
{1213, "Scintillator Layer[ Z+12 ]"},
{1214, "Scintillator Layer[ Z+13 ]"},
{1215, "Scintillator Layer[ Z+14 ]"},
{1216, "Scintillator Layer[ Z +15]"},

{1311, "S1 [ -Y ]"},
{1312, "S1 [ -Y +1 ]"},
{1313, "S1 [ -Y +2 ]"},
{1321, "S1 [ Y ]"},
{1322, "S1 [ Y +1 ]"},
{1323, "S1 [ Y +2 ]"},

{1411, "VETO [ - X ]"},
{1412, "VETO [ X ]"},
{1421, "VETO [ - Y ]"},
{1422, "VETO [ Y ]"},
{1430, "VETO Bottom"},

{2111, "Silicon[ Z , - X , - Y]"},
{2112, "Silicon[ Z , - X , Y]"},
{2121, "Silicon[ Z , 0 , - Y]"},
{2122, "Silicon[ Z , 0 , Y]"},
{2131, "Silicon[ Z , X , - Y]"},
{2132, "Silicon[ Z , X , Y]"},
{2211, "Silicon[ Z+1 , - X , - Y]"},
{2212, "Silicon[ Z+1 , - X , Y]"},
{2221, "Silicon[ Z+1 , 0 , - Y]"},
{2222, "Silicon[ Z+1 , 0 , Y]"},
{2231, "Silicon[ Z+1 , X , - Y]"},
{2232, "Silicon[ Z+1 , X , Y]"}

};


