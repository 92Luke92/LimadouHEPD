/*
 * pmtenum.hh
 * Enum of all PMTs and a convenience iterator
 *
 *
 */


#ifndef PMTENUM_HH
#define PMTENUM_HH

#include <array>




enum PMTenum {T1e, T2e, T3e, T4e, T5e, T6e,
            P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw, P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
            VNu, VEu, VSu, VWu, VBne, L9sw, L7nw, L1ne, L8w, L5c,
            T1w, T2w, T3w, T4w, T5w, T6w,
            P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne, P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne,
            VNd, VEd, VSd, VWd, VBsw, L3se, L2e, L6s, L4n, NC
           };

const std::array<const PMTenum, NPMT> PMTiterator = {T1e, T2e, T3e, T4e, T5e, T6e,
                                                   P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw, P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
                                                   VNu, VEu, VSu, VWu, VBne, L9sw, L7nw, L1ne, L8w, L5c,
                                                   T1w, T2w, T3w, T4w, T5w, T6w,
                                                   P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne, P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne,
                                                   VNd, VEd, VSd, VWd, VBsw, L3se, L2e, L6s, L4n, NC
                                                  };

const std::array <PMTenum, 32 > scintPMT = {P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw,
                              P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
                              P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne,
                              P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne
                             };



#endif /* PMTENUM_HH */
