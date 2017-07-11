#include "detector_const.hh"
#include "analysis_const.hh"

#ifndef __LEVREC0__
#define __LEVREC0__ 1

class LEvRec0 {

  public:
    LEvRec0();
    void Reset();

    unsigned short   runType;
    unsigned short   boot_nr;
    unsigned short   run_id;
    unsigned int     event_index;
    unsigned short   event_length;
    unsigned int     trigger_index;
    unsigned int     hepd_time;

    unsigned short   pmt_high[NPMT];
    unsigned short   pmt_low[NPMT];
    unsigned int     PMTBoard_trigger_counter;
    unsigned short   lost_trigger;
    unsigned short   rate_meter[NRATEMETER];
    bool             trigger_flag[NPMT];
    unsigned int     alive_time;
    unsigned int     dead_time;

    short            strip[NCHAN];


    unsigned short   run_duration;
    unsigned short   orbitZone;


    void DumpStrip (void) const;
    void DumpEventIndex() const;
    bool IsZeroSuppressed() const;
    bool IsVirgin() const;


};


class LEvRec0md {

  public:
     LEvRec0md();
    void Reset();

    struct sil_conf_out_t {
        bool             ladder_on[3];
        bool             ladder_mask[3];
        unsigned short   plane_HV[2];
        unsigned short   adj_strip;
        unsigned short   zero_supp_thrd;
        unsigned char    thrd_CN_high;
        unsigned char    thrd_CN_low;
        unsigned short   calib_event_CN;
        unsigned short   calib_event_ped;
        unsigned short   calib_event_RMS;
        unsigned short   calib_event_gauss;
        unsigned short   gauss_check;
        sil_conf_out_t()
        {
            Reset();
        }
        void Reset()
        {
            for (int i = 0; i < 3; ++i) {
                ladder_on[i] = 0;
                ladder_mask[i] = 0;
            }
            for (int i = 0; i < 2; ++i) plane_HV[i] = 0;
            adj_strip = 0;
            zero_supp_thrd = 0;
            thrd_CN_high = 0;
            thrd_CN_low = 0;
            calib_event_CN = 0;
            calib_event_ped = 0;
            calib_event_RMS = 0;
            calib_event_gauss = 0;
            gauss_check = 0;
        }
    };


    unsigned char    trigger_mask[2];
    unsigned short   easiroc_conf[60];
    bool             PMT_mask[NPMT];
    bool             HV_mask[12];
    unsigned short   HV_value[10];
    bool             gen_trig_mask[18];

    struct OBDH_struct_t {
        unsigned int     sec_OBDH;
        unsigned short   ms_OBDH;
        unsigned char    NU[2];
        OBDH_struct_t ()
        {
            Reset();
        }
        void Reset()
        {
            sec_OBDH = 0;
            ms_OBDH = 0;
        }
    };

    struct GPS_struct_t {
        unsigned int     sec_GPS;
        int              axis_pos[3];
        int              axis_vel[3];
        unsigned char    dataID;
        int              lon;
        int              lat;
        int              NU[3];
        GPS_struct_t ()
        {
            Reset();
        }
        void Reset ()
        {
            sec_GPS = 0;
            for (int i = 0; i < 3; ++i) {
                axis_pos[i] = 0;
                axis_vel[i] = 0;
            }
            dataID = 0;
            lon = 0;
            lat = 0;
        }
    };

    struct AOCC_struct_t {
        unsigned int     sec_AOCC;
        unsigned int     us_AOCC;
        unsigned char    x_coord[3];
        unsigned char    y_coord[3];
        unsigned char    z_coord[3];
        unsigned char    roll_angle[3];
        unsigned char    pitch_angle[3];
        unsigned char    yaw_angle[3];
        unsigned short   roll_angl_vel;
        unsigned short   pitch_angl_vel;
        unsigned short   yaw_angl_vel;
        unsigned short   attitude_det;
        unsigned char    magn_torquer;
        unsigned short   magn_torquer_pulse[3];
        unsigned int     q1;
        unsigned int     q2;
        unsigned int     q3;
        AOCC_struct_t ()
        {
            Reset();
        }
        void Reset()
        {
            sec_AOCC = 0;
            us_AOCC = 0;
            for (int i = 0; i < 3; ++i) {
                x_coord[i] = 0;
                y_coord[i] = 0;
                z_coord[i] = 0;
                roll_angle[i] = 0;
                pitch_angle[i] = 0;
                yaw_angle[i] = 0;
            }
            roll_angl_vel = 0;
            pitch_angl_vel = 0;
            yaw_angl_vel = 0;
            attitude_det = 0;
            magn_torquer = 0;
            for (int i = 0; i < 3; ++i) magn_torquer_pulse[i] = 0;
            q1 = 0;
            q2 = 0;
            q3 = 0;
        }
    };

    struct star_sensor_struct_t {
        unsigned int     sec_ss11;
        unsigned int     us_ss11;
        int              att_quat_ss11[4];
        unsigned int     sec_ss21;
        unsigned int     us_ss21;
        int              att_quat_ss21[4];
        unsigned int     sec_ss31;
        unsigned int     us_ss31;
        int              att_quat_ss31[4];
        unsigned char    quat_valID_x1;
        unsigned int     sec_ss12;
        unsigned int     us_ss12;
        int              att_quat_ss12[4];
        unsigned int     sec_ss22;
        unsigned int     us_ss22;
        int              att_quat_ss22[4];
        unsigned int     sec_ss23;
        unsigned int     us_ss23;
        int              att_quat_ss23[4];
        unsigned char    quat_valID_x2;
        unsigned char    attitude_det;
        unsigned char        NU;
        star_sensor_struct_t ()
        {
            Reset();
        }
        void Reset()
        {
            sec_ss11 = 0;
            us_ss11 = 0;
            sec_ss21 = 0;
            us_ss21 = 0;
            sec_ss31 = 0;
            us_ss31 = 0;
            quat_valID_x1 = 0;
            sec_ss12 = 0;
            us_ss12 = 0;
            sec_ss22 = 0;
            us_ss22 = 0;
            sec_ss23 = 0;
            us_ss23 = 0;
            for (int i = 0; i < 4; ++i) {
                att_quat_ss11[i] = 0;
                att_quat_ss21[i] = 0;
                att_quat_ss31[i] = 0;
                att_quat_ss12[i] = 0;
                att_quat_ss22[i] = 0;
                att_quat_ss23[i] = 0;
            }
            quat_valID_x2 = 0;
        }

    };

    unsigned short   PMT_rate_meter[NPMT + 1];
    short            CPU_temp_start_stop_Run[2];
    short            PMT_temp_start_stop_Run[2];
    unsigned int     CPU_time_start_stop_Run[2];

    struct CPU_timestamp_t {
        unsigned int     OBDH;
        unsigned int     GPS;
        unsigned int     AOCC;
        unsigned int     star_sensor;
        CPU_timestamp_t ()
        {
            Reset();
        }
        void Reset()
        {
            OBDH = 0;
            GPS = 0;
            AOCC = 0;
            star_sensor = 0;
        }
    };

    struct status_register_t {
        unsigned short   statusDAQ;
        unsigned short   statusPMT;
        unsigned short   statusTM_TC;
        unsigned short   statusHV_PS;
        unsigned short   CPU_board_boot;
        unsigned short   statusCPU;
        status_register_t()
        {
            Reset();
        }
        void Reset()
        {
            statusDAQ = 0;
            statusPMT = 0;
            statusTM_TC = 0;
            statusHV_PS = 0;
            CPU_board_boot = 0;
            statusCPU = 0;
        }
    };

    sil_conf_out_t   sil_conf;
    OBDH_struct_t    OBDH_info;
    GPS_struct_t     GPS_info;
    AOCC_struct_t    AOCC_info;
    star_sensor_struct_t    star_sensor_info;
    CPU_timestamp_t  CPU_timestamp;
    status_register_t status_register;

};


#endif
