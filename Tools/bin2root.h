#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <TVector3.h>
#include <TTimeStamp.h>
#include <TTree.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TBranch.h>
#include <iostream>


#pragma pack(push)
#pragma pack(1)

#define ADC_HIGHGAIN_1_BEG       4 // prime 4 word = 2 di index + 2 di timer (10 us)
#define ADC_HIGHGAIN_1_END       36
#define ADC_HIGHGAIN_2_BEG       68
#define ADC_HIGHGAIN_2_END       100
#define ADC_LOWGAIN_1_BEG        36
#define ADC_LOWGAIN_1_END        68
#define ADC_LOWGAIN_2_BEG        100
#define ADC_LOWGAIN_2_END        132

#define HEADER_RUN               (438-16)
#define EASIROC_CH               64                   
#define PMTADC                   128 // (word)
#define PMTINFO                  24  // (word)
#define PMTLEN                   (PMTADC+PMTINFO)
#define SILLEN                   4608
#define SiBLOCK                  384
#define STRIPNUM                 7
#define TRIG_CONFIG_NUM          9

#define PMT_RATE_METER_SIZE      65
#define GEN_TRIG_MASK_SIZE       17

#define CRC_SEED                 0xffffffffUL
#define RUNID_STRING_LEN         6                    
#define MIXED                    0x63
#define VIRGIN                   0x4E
#define COMPRESSED               0x36
#define COMPRESSED_NOCN          0x55
#define GENERIC                  0x78
   

#define HTONS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))
#define HTONL(n) (((((unsigned long)(n) & 0xFF)) << 24) |       \
                  ((((unsigned long)(n) & 0xFF00)) << 8) |      \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) |    \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))


#define CRC32TABLE					      \
{  /* 0x00 */ 0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, \
   /* 0x04 */ 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3, \
   /* 0x08 */ 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, \
   /* 0x0C */ 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, \
   /* 0x10 */ 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, \
   /* 0x14 */ 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, \
   /* 0x18 */ 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, \
   /* 0x1C */ 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5, \
   /* 0x20 */ 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, \
   /* 0x24 */ 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, \
   /* 0x28 */ 0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, \
   /* 0x2C */ 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59, \
   /* 0x30 */ 0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, \
   /* 0x34 */ 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F, \
   /* 0x38 */ 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, \
   /* 0x3C */ 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, \
   /* 0x40 */ 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, \
   /* 0x44 */ 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433, \
   /* 0x48 */ 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, \
   /* 0x4C */ 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01, \
   /* 0x50 */ 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, \
   /* 0x54 */ 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, \
   /* 0x58 */ 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, \
   /* 0x5C */ 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65, \
   /* 0x60 */ 0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, \
   /* 0x64 */ 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, \
   /* 0x68 */ 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, \
   /* 0x6C */ 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, \
   /* 0x70 */ 0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, \
   /* 0x74 */ 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F, \
   /* 0x78 */ 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, \
   /* 0x7C */ 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD, \
   /* 0x80 */ 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, \
   /* 0x84 */ 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, \
   /* 0x88 */ 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, \
   /* 0x8C */ 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, \
   /* 0x90 */ 0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, \
   /* 0x94 */ 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7, \
   /* 0x98 */ 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, \
   /* 0x9C */ 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, \
   /* 0xA0 */ 0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, \
   /* 0xA4 */ 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B, \
   /* 0xA8 */ 0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, \
   /* 0xAC */ 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79, \
   /* 0xB0 */ 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, \
   /* 0xB4 */ 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, \
   /* 0xB8 */ 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, \
   /* 0xBC */ 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D, \
   /* 0xC0 */ 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, \
   /* 0xC4 */ 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713, \
   /* 0xC8 */ 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, \
   /* 0xCC */ 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, \
   /* 0xD0 */ 0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, \
   /* 0xD4 */ 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777, \
   /* 0xD8 */ 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, \
   /* 0xDC */ 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45, \
   /* 0xE0 */ 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, \
   /* 0xE4 */ 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, \
   /* 0xE8 */ 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, \
   /* 0xEC */ 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9, \
   /* 0xF0 */ 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, \
   /* 0xF4 */ 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF, \
   /* 0xF8 */ 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, \
   /* 0xFC */ 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D  \
}

/* 같같같같같같같같같같같같같같같같같*/
struct evHeader_s
{
   unsigned int    ev_packetCode;
   unsigned short  ev_boot_nr;
   unsigned short  ev_run_id;
   unsigned int    ev_index;
   unsigned short  ev_len;
};


/* 같같같같같같같같같같같같같같같같같*/
struct event_s
{
   struct evHeader_s   eventHeader;
   unsigned short      pmtDATA[PMTLEN]; 
} ;
typedef struct event_s event_t;



/* 같같같같같같같같같같같같같같같같같*/
struct calib_s
{
   struct evHeader_s   eventHeader;
   unsigned short      silDATA[SILLEN];
   unsigned short      pmtDATA[PMTADC];
   unsigned short      crc32[2]; 
} ;
typedef struct calib_s calib_t;



/* 같같같같같같같같같같같같같같같같같같같같같같같*/
struct EASIROC_s
{
   unsigned char    config[116];
};
typedef struct EASIROC_s EASIROC_t;


/* 같같같같같같같같같같같같같같같같같같같같같같같*/
struct OBDH_struct_s // 8 bytes
{
   unsigned int     sec;
   unsigned short   ms;
   unsigned char    NU[2];
} ;
typedef struct OBDH_struct_s OBDH_struct_t;


/* 같같같같같같같같같같같같같같같같같같같같같같같*/
struct GPS_struct_s // 40 bytes
{
   unsigned int     sec;
   int              axis_pos[3];
   int              axis_vel[3];
   unsigned char    dataID;
   int              lon;
   int              lat;
   unsigned char    NU[3];
} ;
typedef struct GPS_struct_s GPS_struct_t;


/* 같같같같같같같같같같같같같같같같같같같같같같같*/
struct AOCC_struct_s // 52 bytes
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
typedef struct AOCC_struct_s AOCC_struct_t;


/* 같같같같같같같같같같같같같같같같같같같같같같같*/
struct quaternion_s // 16 bytes
{
   int q0;
   int q1;
   int q2;
   int q3;
};
typedef struct quaternion_s quaternion_t;



/* 같같같같같같같같같같같같같같같같같같같같같같같*/
struct star_sens_struct_s // 148 bytes
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
typedef struct star_sens_struct_s star_sens_struct_t;


/* 같같같같같같같같같같같같같같같같같같같같같같같*/
struct BroadCast_s
{
   OBDH_struct_t        OBDH;   // 8 bytes
   GPS_struct_t         GPS;         // 40 bytes 
   AOCC_struct_t        AOCC;        // 52 bytes
   star_sens_struct_t   star_sensor;  // 148 bytes
} ;
typedef struct BroadCast_s BroadCast_t;





/* 같같같같같같같같같같같같같같같같같*/
struct CPUtimestamp_s
{
   unsigned int       OBDH;
   unsigned int       GPS;
   unsigned int       AOCC;
   unsigned int       star_sensor; // 16 bytes
} ;
typedef struct CPUtimestamp_s CPUtimestamp_t;




/* 같같같같같같같같같같같같같같같같같*/
struct Status_Register_s
{
   unsigned short      statusDAQ;
   unsigned short      statusPMT;   
   unsigned short      statusTM_TC;  
   unsigned short      statusHV_PS; 
   unsigned short      CPU_board_boot;
   unsigned short      statusCPU;   
} ;
typedef struct Status_Register_s Status_Register_t;



/* 같같같같같같같같같같같같같같같같같*/
struct scientific_data_s
{
   unsigned short     PMT_rate_meter[PMT_RATE_METER_SIZE];
   short              CPU_temp_start_stop[2];   // first 4 bits unused
   short              PMT_temp_start_stop[2]; 
   unsigned int       CPU_time_start_stop[2];
} ;
typedef struct scientific_data_s scientific_data_t;


/* 같같같같같같같같같같같같같같같같같*/
struct silicon_config_s  // 0x1D6 bytes (Tail = 0x1D8 bytes)
{  
   unsigned char      ladder_on; // byte
   unsigned char      ladder_mask; 
   unsigned short     plane_HV[2];
   unsigned short     adj_strip;
   unsigned short     zero_supp_thrd; 
   unsigned char      thrd_CN_HIGH;
   unsigned char      thrd_CN_LOW;
   unsigned short     calib_event_CN;
   unsigned short     calib_event_ped;
   unsigned short     calib_event_RMS;
   unsigned short     calib_event_gauss;
   unsigned short     gauss_check;
}   ;
typedef struct  silicon_config_s silicon_config_t;


/* 같같같같같같같같같같같같같같같같같*/
struct header_run_s  // 0x1D6 bytes (Tail = 0x1D8 bytes)
{
   unsigned short     boot_nr;
   unsigned short     run_id;
   unsigned short     run_type;
   unsigned short     run_duration;
   unsigned short     orbit_Zone;
   silicon_config_t   silConfig;
   unsigned char      trigger_mask[4]; // 38 bytes
   unsigned char      easiroc_config[118];
   unsigned int       PMT_mask[2]; // 1 = excluded
   unsigned int       HV_mask; // 1 = OFF, 0 = ON
   unsigned short     HV_value[10]; // 10 bits for each of the 10 power supply
   unsigned int       gen_trig_mask; 

   BroadCast_t        broadcast;  // 248 bytes
   CPUtimestamp_t     timestamp;   // 16 bytes
   Status_Register_t  status_reg;
};
typedef struct header_run_s header_run_t;



/* 같같같같같같같같같같같같같같같같같*/
struct tail_run_s  // Tail = 0x1D8 bytes
{
   unsigned short     boot_nr;
   unsigned short     run_id;
   unsigned short     run_type;
   unsigned short     run_duration;
   unsigned short     orbit_Zone;
   silicon_config_t   silConfig;
   unsigned char      trigger_mask[4]; // 38 bytes
   unsigned char      easiroc_config[120];
   unsigned int       PMT_mask[2]; // 1 = excluded
   unsigned int       HV_mask; // 0 = ON, 1 = OFF
   unsigned short     HV_value[10]; // 10 bits for each of the 10 power supply
   unsigned int       gen_trig_mask; 

   BroadCast_t        broadcast;  // 248 bytes
   CPUtimestamp_t     timestamp;   // 16 bytes
   Status_Register_t  status_reg;
};
typedef struct tail_run_s tail_run_t;


/* 같같같같같같같같같같같같같같같같같같같같같같같*/
struct silicon_cluster5_s
{
   unsigned short    max_index;
   short             strip_data[5];
};
typedef struct silicon_cluster5_s silClust5_t;



/* 같같같같같같같같같같같같같같같같같같같같같같같*/
struct silicon_cluster3_s
{
   unsigned short    max_index;
   short             strip_data[3];
};
typedef struct silicon_cluster3_s silClust3_t;




int RunHandler(TString filename, unsigned char *dataIn, int position);
int bin2root(TString filename);

void FillHeader(header_run_t * head_run);
void FillTail(tail_run_t * tail_run, header_run_t * head_run);
void FillScientDataPacket(scientific_data_t *scient_data);
bool getBit(unsigned char byte, int pos){return (byte >> pos) & 0x1; } // position in range 0-7
int silicon_handling(int runtype, short *strip, unsigned char *datain,
		     unsigned int silstart, unsigned int sillen, unsigned int NrAdjStrips,
		     unsigned short laddermask);
void virgin_raw(short *strip, unsigned char *datain,
		unsigned int silstart, unsigned int sillen, unsigned short laddermask);
int compressed_run(short *strip, unsigned char *datain,
		   unsigned int silstart, unsigned int sillen, unsigned int NrAdjStrips);

unsigned  int crc32PC(unsigned char *data, unsigned int dataLen, unsigned  int crc);
