#include "detector_const.hh"
#include "analysis_const.hh"
#include "LEvRec0File.hh"

#ifndef __LEVREC0__
#define __LEVREC0__ 1

const unsigned short __MAXCLUSTERNR__ = 100;


class LEvRec0 {

friend class LEvRec0File;

public:
  LEvRec0();
  ~LEvRec0();

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

  short            *strip;
  unsigned short   clust_nr;
  short            *cluster[__MAXCLUSTERNR__];     
  
  void DumpStrip(void) const;
  void DumpEventIndex() const;
  bool IsZeroSuppressed() const;
  bool IsVirgin() const;
  inline unsigned short GetNAdjacentStrips(void) const {return __adj_strip;};

  const int trigger(const int i, const int j) const;
  const int plane(const int i, const int j) const;
  const int lyso(const int i) const;
  const int veto (const int i, const int j) const;

private:
  unsigned short __adj_strip;
  void SetVirginMode(void);
  void SetZeroSuppressedMode(const unsigned short adj_strip);
};


#pragma pack(push, 1)
struct silicon_config_t
{  
   bool               ladder_on[N_COLUMN]; 
   bool               ladder_mask[N_COLUMN]; 
   unsigned short     plane_HV[N_SIDES];
   unsigned short     adj_strip;
   unsigned short     zero_supp_thrd; 
   unsigned char      thrd_CN_HIGH;
   unsigned char      thrd_CN_LOW;
   unsigned short     calib_event_CN;
   unsigned short     calib_event_ped;
   unsigned short     calib_event_RMS;
   unsigned short     calib_event_gauss;
   unsigned short     gauss_check;
} ;


struct OBDH_struct_t 
{
   unsigned int     sec;
   unsigned short   ms;
   unsigned char    NU[2];
} ;


struct GPS_struct_t
{
   unsigned int     sec;
   int              axis_pos[3];
   int              axis_vel[3];
   unsigned char    dataID;
   int              lon;
   int              lat;
   unsigned char    NU[3];
} ;


struct AOCC_struct_t
{
   unsigned int     sec;
   unsigned int     us;
   unsigned char    x_coord[3];
   unsigned char    y_coord[3];
   unsigned char    z_coord[3];
   unsigned char    roll_angle[3];
   unsigned char    pitch_angle[3];
   unsigned char    yaw_angle[3];
   unsigned short   roll_angl_vel;
   unsigned short   pitch_angl_vel;
   unsigned short   yaw_angl_vel;
   unsigned char    attitude_det;
   unsigned char    magn_torquer;
   unsigned short   magn_torquer_pulse[3];
   unsigned int     q1;
   unsigned int     q2;
   unsigned int     q3;
} ;


struct quaternion_t
{
   int q0;
   int q1;
   int q2;
   int q3;
};


struct star_sens_struct_t
{
   unsigned int     sec_ss11;
   unsigned int     us_ss11; 
   quaternion_t     att_quat_ss11;
                                  
   unsigned int     sec_ss21;     
   unsigned int     us_ss21;
   quaternion_t     att_quat_ss21;
   
   unsigned int     sec_ss31;
   unsigned int     us_ss31;
   quaternion_t     att_quat_ss31;
   unsigned char    quat_valID_x1;
   
   unsigned int     sec_ss12;
   unsigned int     us_ss12;
   quaternion_t     att_quat_ss12;
   
   unsigned int     sec_ss22;
   unsigned int     us_ss22;
   quaternion_t     att_quat_ss22;
   
   unsigned int     sec_ss32;
   unsigned int     us_ss32;
   quaternion_t     att_quat_ss32;
   unsigned char    quat_valID_x2;
   
   unsigned char    attitude_det;
   unsigned char    NU;
} ;


struct BroadCast_t
{
   OBDH_struct_t        OBDH;        
   GPS_struct_t         GPS;         
   AOCC_struct_t        AOCC;        
   star_sens_struct_t   star_sensor; 
} ;


struct CPUtimestamp_t
{
   unsigned int       OBDH;
   unsigned int       GPS;
   unsigned int       AOCC;
   unsigned int       star_sensor; 
} ;


struct Status_Register_t
{
   unsigned short      statusDAQ;
   unsigned short      statusPMT;   
   unsigned short      statusTM_TC;  
   unsigned short      statusHV_PS; 
   unsigned short      CPU_board_boot;
   unsigned short      statusCPU;   
} ;


class LEvRec0Md {

public:
  LEvRec0Md();
   
   unsigned short     boot_nr;
   unsigned short     run_id;
   unsigned short     run_type;
   unsigned short     run_duration;
   unsigned short     orbit_Zone;
   
   silicon_config_t   silConfig;

   unsigned char      trigger_mask[2]; // 0 = startrun, 1 stoprun
   unsigned short     easiroc_config[EASIROC_CONF];
   bool               PMT_mask[NPMT]; 
   bool               HV_mask[NPS]; 
   unsigned short     HV_value[NPSPMT]; 
   bool               gen_trig_mask[NCALOPLANE];

   BroadCast_t        broadcast;  
   CPUtimestamp_t     timestamp;  

   unsigned short     PMT_rate_meter[SINGLE_PMT_RM];
   unsigned int       CPU_time[2]; // 0 = startrun, 1 stoprun
   short              CPU_temp[2]; // 0 = startrun, 1 stoprun
   short              PMT_temp[2]; // 0 = startrun, 1 stoprun

   Status_Register_t  status_reg;

   void               Dump(void) const;
};

#pragma pack(pop)


// Added according to QuickLook
struct orbit_conf_out_t           
{
   bool             ladder_mask[3];
   unsigned char    trigger_mask[2];
   unsigned short   run_duration;
   int     lat1;
   int     lat2;
   int     long1;
   int     long2;
} ;
// Added according to QuickLook
struct dummy_packet_out_t
{
   unsigned short    len;
   unsigned short    boot_nr;
   unsigned short    run_id;
   orbit_conf_out_t  orbit_conf[5];
   bool              user_orbital_settings;
   unsigned short    WO_config_ID;
   unsigned short    calib_period;
   bool              safe_mode;
} ;

class LEvRec0Conf {
   
public:
   LEvRec0Conf();
   
   dummy_packet_out_t    dummy_pkt;
   
};

class LEvRec0HVpmt {
   
public:
   LEvRec0HVpmt();
   
   unsigned short     boot_nr;
   unsigned short     run_id;
   unsigned short     HV_pmt_mon[10];
   unsigned short     HV_sil_mon[2]; 
   
};

#endif
