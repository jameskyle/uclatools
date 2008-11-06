 /* Extensively hacked by Timothy L. Davis, June 4, 1994 for "xds" input.   */

#define NGR 32     /* Maximum number of ACR-Groups         */
#define NOE  8      /* Maximum number of entries in NUM2 hdr*/

/***************************************************************************/
/*_____ RADGOP IMAGE HEADER STRUCTURE ____________________*/
/* Header record for image files */
struct image_head
{   unsigned long id;
    unsigned long off;
    unsigned long len;
    unsigned short hgh;
    unsigned short wid;
    unsigned short typ;
};

struct RAD_IMAGE_HEAD 
{ struct image_head ih;
  char mc;
};

/***************************************************************************/

struct ACR                 /* Organizational data for the ACR-NEMA Header  */
{
  int   ngrps;             /*  Number of groups                            */
  char *grp[NGR][8];       /*  Group numbers Hex (strings)                 */
  int   siz[NGR];          /*  Group sizes                                 */
  int   off[NGR];          /*  Group offsets into header                   */
};

/***************************************************************************/

struct NUM         /* ORGANIZATIONAL DATA FOR THE SIEMENS NUMARIS2 HEADER  */
{
  int   nents;             /*  Number of entries                           */
  char *nam[NOE][32];      /*  Entry names, and explanations               */
  int   siz[NOE];          /*  Entry sizes                                 */
  int   off[NOE];          /*  Entry offsets                               */
};


struct ENT_TBL             /*  ENTRY TABLE                                 */
{
  ushort n_ent;            /*  Number of entries [14]                      */
  ushort l_ent;            /*  Length of entries (words) [2]               */
  ushort l_blk;            /*  Block length in bytes (hex)                 */
  ushort f_blk;            /*  First free block in image file (hex)        */
  ushort n_blk_inst;       /*  Number of first block of inst+gen info (hex)*/
  ushort l_inst;           /*  Length of inst+gen info in blocks (hex)     */
  ushort n_blk_meas;       /*  Number of first block of measurement info   */
  ushort l_meas;           /*  Length of measurement info in blocks (hex)  */
  ushort n_blk_imtxt;      /*  Number of first block of image text (hex)   */
  ushort l_imtxt;          /*  Length of image text in blocks (hex)        */
  ushort n_blk_im;         /*  Number of first block of
                               /image/raw/adjust/spectroscopy info (hex)   */
  ushort l_im;             /*  Length of ^ in blocks (hex)                 */
  ushort n_blk_corr;       /*  Number of first block of correction info    */
  ushort l_corr;           /*  Length ofcorrection information in blocks   */
  ushort n_blk_adj1;       /*  Number of first block of adjust data-1      */
  ushort l_adj1;           /*  Lengtj of adjust data-1 in blocks (hex)     */
  ushort n_blk_adj2;       /*  Number of first block of adjust data-2 (hex)*/
  ushort l_adj2;           /*  Length of adjust data-2 in blocks (hex)     */
  ushort n_blk_plot;       /*  Number of first block of plotter info (hex) */
  ushort l_plot;           /*  Length of plotter information in blocks     */
  ushort n_blk_card;       /*  Number of first block of cardiac info (hex) */
  ushort l_card;           /*  Length of cardiac information in blocks     */
  ushort n_blk_pat;        /*  Number of first block of patient info (hex) */
  ushort l_pat;            /*  Length of patient information in blocks     */
  ushort n_blk_proc;       /*  Number of first block of image proc info    */
  ushort l_proc;           /*  Length of Image and post processing info    */
  ushort n_blk_spect;      /*  Number of first block of spectroscopy info  */
  ushort l_spect;          /*  Length of spectroscopy information in blocks*/
  ushort n_blk_addmeas;    /*  Number of first block of add. meas info     */
  ushort l_addmeas;        /*  Length of additional measurement info(blks) */
  ushort n_blk_supp;       /*  Number of first block of supplementary text */
  ushort l_supp;           /*  Length of supplementary text in blocks      */
 };

/***************************************************************************/

struct INIT_INFO           /*  INITIALIZATION INFO                         */
 {
  char inst_spec[4];       /*  Installation specification                  */
                           /*  '___0' == foreign
                               '__10' == SIEMENS CT
                               '__20' == SIEMENS MR
                               '__30' == BRUKER                            */

  char inst_code[10];      /*  Installation code                           */
                           /*  Format '______XXXX'
                                  1 - 2000 == SIEMENS CT
                               2001 - 4000 == SIEMENS MR
                               4001 - 5000 == BRUKER
                               5000 - ..,  == For future use               */
  char prot[2];             /*  Prohibited bytes                           */
  char sw_version[8];       /*  Software Version                           */
  char sw_level[8];         /*  Software level                             */
  char ser_no[32];          /*  Serial number                              */
  char reserved1[192];      /*  Reserved area                              */
  ushort patch_id;          /*  Patch identification                       */
  ushort family_nr;         /*  NUMARISFamily number (1 or 2)              */
  ushort n_ver_hdr;         /*  Header Version Number                      */
  ushort n_upd_hdr;         /*  Header Update number                       */
  ushort version_hdr_struct;/*  Version of Header Structure                */
  char reserved2[246];      /*  Reserved area                              */
 };

/***************************************************************************/

struct PAT_INFO            /*  PATIENT INFORMATION                         */
 {
  char name[26];           /*  Patient name                                */
  char prot1[2];           /*  Prohibited area                             */
  char id[12];             /*  Patient ID                                  */
  char birth[10];          /*  Patient date of birth 'yyyy.mm.dd'          */
  char sex[2];             /*  Patient sex 'M_', 'F_' or 'O_'              */
  char prot2[2];           /*  Prohibited area                             */
  char maiden_name[26];    /*  Patient Maiden name                         */
  char prot3[2];           /*  Prohibited area                             */
  char ref_phys[26];       /*  Refering physician                          */
  /* char prot4[402];          Reserved area  not necessary to load        */
 };

/***************************************************************************/

  /*_______________________________________________________________________*/

  struct ADM               /*  AMINISTRATIONAL PARAMETERS                  */
   {
    int n_meas;            /*  Total number of measurements                */
    int date[2];           /*  Date + Time in DEC standard                 */
    char skip1[32];        /*  >>>>>>>>>>>>>>>>>>                          */
    int mode;              /*  Measurement mode                            */
                           /*  100 == SE  (spin echo )
                               200 == IR  (inversion recovery)
                               500 == FID (Free Induction Decay)
                               600 == SEM (spin echo, multi echo, CPMG )
                               800 == FW  (lipid separation)               */
    char skip2[16];        /*  >>>>>>>>>>>>>>>>>>                          */
   };
  
  /*_______________________________________________________________________*/

  struct LINCOL            /*  GENERALS (LINES/COLS)                       */
   {
    char skip1[8];         /*  >>>>>>>>>>>>>>>>>>                          */
    int  n_lines;          /*  Number of valid Image lines                 */
    char skip2[8];         /*  >>>>>>>>>>>>>>>>>>                          */
    int  n_cols;           /*  Number of columns in image                  */
    char skip3[12];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct SEQ_FILE          /*  SEQUENCE FILE PARAMETERS                    */
   {
    char skip1[100];       /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct SEQ_REP           /*  SEQUENCE/REPEAT PARAMETERS                  */
   {
    char skip1[60];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct SEQ_CHA           /*  SEQUENCE/CHANGE PHYSICAL PARAMETERS         */
   {
    int inv_time;          /*  Inversion time in msec                      */
    int rep_time;          /*  Repetition time in msec                     */
    int T1T2_time;         /*  Second echo- / repetition time for
                               standard T2-/ T1 Calculation.
                               Unit is same as TE or TR                    */
    int n_acq;             /*  Number of Acquisitions                      */
    char skip1[44];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct PH_CORR           /*  PHASE CORR MEASUREMENT AND CALCULATION      */
   {
    char skip1[40];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct SLICE             /*  SLICE PARAMETERS                            */
   {
    int n_slices;          /*  Total number of slices [1..64]              */
    int n_slice;           /*  Number of actual slice                      */
    float pos_slice_sag;   /*  Slice pos in sag. dir. from slice cent (mm) */ 
    float pos_slice_cor;   /*  -------""--- cor dir.  -------""----------- */
    float pos_slice_tra;   /*  -------""--- tra dir.  -------""----------- */
    float slice_normal_sag;/*  Slice Normal in Sagittal direction          */
    float slice_normal_cor;/*  --------------- Coronial ---------          */
    float slice_normal_tra;/*  --------------- Transversal ------          */
    float slice_thick;     /*  Slice Thickness in mm                       */
    float slice_rot;       /*  Slice rotation in radians                   */
    float fov;             /*  Field of view in mm                         */
    float slice_pos;       /*  Actual slice position in mm                 */
    float gap;             /*  Slice gap in mm, equidistant slices only    */
    int   slice_pos_hist;  /*  Actual slice positioning history            */
    char skip1[96];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct ECHO              /*  ECHO PARAMETERS                             */
   {
    int   n_echoes;        /*  Total number of echoes                      */
    int   echo_ptr;        /*  Pointer to actual echo time in add_meas     */
    float echo_time;       /*  Actual echo time in ms                      */
    char  skip1[36];       /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct THREED_SPEC       /*  3D SPECIFIC PARAMETERS                      */
   {
    char skip1[48];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct SPECT             /*  SPECTROSCOPY PARAMETERS                     */
   {
    char skip1[76];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct GRAD              /*  GRADIENT PARAMETERS                         */
   {
    char skip1[40];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct PHYS_MEAS         /*  PHYSIOLOGICAL MEASUREMENT PARAMETERS        */
   {
    char skip1[120];       /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct HW                /*  HARDWARE PARAMETERS                         */
   {
    char skip1[40];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct ADJ_TRA           /*  ADJUST PARAMETERS: TRANSMITTER CHANNEL      */
   {
    char skip1[60];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct NUCL_COIL         /*  NUCLEUS AND COIL PARAMETERS                 */
   {
    char  nucleus[8];      /*  Nucleus string                              */
    char  coil[32];        /*  Coil string  e.g "body"                     */
    float coil_fov;        /*  Coil Field of view in mm                    */
    int   coil_id;         /*  Coil ID                                     */
    int   t_nucleus;       /*  Nucleus type                                */
    char  skip1[48];       /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct ADJ_REC           /*  ADJUST PARAMETERS: RECEIVER CHANNEL         */
   {
    char skip1[60];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct IM_SP             /*  IMAGE AND SPECTRUM CALC. PARAMETERS         */
   {
    int cal_mode;          /*  Calculation mode                            */
    float meas_time_fact;  /*  Measurement time mult. factor, timeout fac. */
    int raw_manip_mask_m;  /*  Raw data manipulation mask (main data)      */
    int fft_manip_mask_m;  /*  FFT Manipulation mask (main data)           */
    int img_manip_mask_m;  /*  Image data manipulation mask                */
    int raw_manip_mask;    /*  Raw data manipulation mask (ph. cor. data ) */
    int im_sp_calc;        /*  Image/Spectrum calculation parameters       */
    int fft_manip_mask;    /*  FFT manipulation mask (phase cor. data)     */
    int img_manip_mask;    /*  Image data manipulation mask (ph. corr data */ 
    int refl_rl;           /*  Reflection during im. rec. -1=refl  1=not   */
    int refl_ud;           /*  Reflection during im. rec. -1=refl  1=not   */
    int rot;               /*  Image rotation during im. rec.(deg) 0/90/180*/
    char skip1[56];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct FILTER            /*  FILTER PARAMETERS                           */
   {
    char skip1[96];        /*  >>>>>>>>>>>>>>>>>>                          */
   };

  /*_______________________________________________________________________*/

  struct QUAL              /*  QUALITY PARAMETERS                          */
   {
    int  noise_level;      /*  Noise lecel for this image / spectrum       */
    char skip1[212];       /*  >>>>>>>>>>>>>>>>>>                          */
   };

   /*______________________________________________________________________*/

struct IM_INFO           /*  IMAGE AND POSTPROCESSING INFORMATION          */
 {
  float  norm_fac_cent;  /*  Normalization factor for window center        */
  float  norm_fac_wid;   /*  Normalization factor for windoe width         */
  int    img_manip_mask; /*  Image manipulation mask                       */
  char   skip1[116];     /*  >>>>>>>>>>>>>>>>>>                            */
  ushort img_data_code;  /*  Image text and graphical data                 */
  ushort archive;        /*  Archiving parameter                           */
  ushort arch_code;      /*  Archiving code                                */
  ushort n_txt_lin;      /*  Number of text lines [16]                     */
  ushort n_txt_col;      /*  Number of text columns [25]                   */
  ushort im_mat_x;       /*  Image matrix in X-Direction                   */
  ushort im_mat_y;       /*  Image matrix in Y-Direction                   */
  ushort im_mat;         /*  Image matrix                                  */
  ushort n_bw_lut;       /*  Number of B/W lookup table                    */
  ushort n_col_lut;      /*  Number of colur look up table                 */
  ushort pix_depth;      /*  Pixel depth of image information              */
  ushort gra_bits;       /*  Bit depth of graphical information            */
  ushort gra_reg_code;   /*  Code for usage of particular regions          */
  ushort win_cent_max;   /*  Upper limit for window center:
                             CT:3072  SE:4095 IR:4095                      */
  ushort win_cent_min;   /*  Lower limit for window center:
                             CT:-1024  SE:0  IR:4095                       */
  ushort win_wid_autoA;  /*  Windowing width of window A at aut. windowing */
  ushort win_cent_autoA; /*  windowing centre of win A at auto. windowing  */
  ushort win_wid_autoB;  /*  Windowing width of window B at aut. windowing */
  ushort win_cent_autoB; /*  windowing centre of win B at auto. windowing  */
  ushort win_mode_aut0;  /*  Window mode at automatic windowing            */
  ushort pix_scale_fac;  /*  Scale factor in pixels corresp. to 10cm.      */
  char   skip2[14];      /*  >>>>>>>>>>>>>>>>>>> */
  ushort pix_min_val;    /*  Minimum value of pixel information, absolute  */
  ushort pix_max_val;    /*  Maximum value of pixel information, absolute  */
  ushort pix_mean_val;   /*  Mean    value of pixel information, absolute  */
  ushort qual_minmax;    /*  Quality of Min/Max/Mean value                 */
  ushort compr_code;     /*  Data compression code                         */
  ushort view_dir;       /*  View direction: Cranial=1 Caudal=-1           */
  ushort pat_pos1;       /*  Patient direction: Head first=-1 Feet first=1 */
  ushort pat_pos2;       /*  Patient position2:
                             Suppine position == 1
                             Prone position   == 2
                             Right lateral position ==3
                             Left lateral position == 4  */
  ushort rel_imx_mag;    /*  Relation between image x-axis and mag. field  */
  ushort rel_imy_mag;    /*  Relation between image y-axis and mag. field  */
  ushort refl_lr;        /*  Image reflection left-right 1=done 0=not      */
  ushort refl_ud;        /*  Image reflection up-down    1=done 0=not      */
  ushort img_rot;        /*  Image rotation in degrees                     */
  ushort prot2;          /*  Prohibited area                               */
  ushort exp_lab_fac;    /*  Exposure label factor, index 1...5            */
  char   skip3[290];     /*  <>>>>>>>>>>>>>>>>>>>>>>  */
 };

/***************************************************************************/

struct IM_TXT            /*  IMAGE TEXT */
 {
  ushort nb_txt_page;    /*  Total number of bytes in textpage             */
  char   prot1[14];      /*  Reserved area                                 */
  char   rev_vid_sw[4];  /*  Reverse video switch '<ESC>[7m'               */
  char   img_nr[11];     /*  Image Number                                  */
  char   norm_vid_sw[4]; /*  Normal video switch  '<ESC>[0m'               */
  char   pat_name[25];   /*  Patient name                                  */
  char   prot2[8];       /*  Prohibited area                               */
  char   pat_id[12];     /*  Patient ID                                    */
  char   prot3[13];      /*  Reserved area                                 */
  char   prot4[8];       /*  Prohibited area                               */
  char   pat_sex[5];     /*  Patient sex  's_xxx'                          */
  char   prot5[11];      /*  Reserved area                                 */
  char   date[9];        /*  Measurement date 'dd-mmm-yy'(?)               */
  char   prot6[8];       /*  Prohibited area                               */
  char   pat_whg[8];     /*  Patient wight   'xxxx_xxx'                    */
  char   prot7[9];       /*  Reserved area                                 */
  char   meas_time[8];   /*  Measurement time  'hh:mm:ss'                  */
  char   prot8[8];       /*  Prohibited area                               */
  char   comment[24];    /*  Comment                                       */
  char   prot9;          /*  Reserved area                                 */
  char   prot10[8];      /*  Prohibited area                               */
  char   prot11[25];     /*  Reserved area                                 */
  char   prot12[8];      /*  Prohibited area                               */
  char   seq_type[11];   /*  Sequence type  'xxxxxx_xxxx'                  */
  char   rep_time[11];   /*  Repetition time 'TR____xxxxx'                 */
  char   echo_time[11];  /*  Echo time 'TE_____xxxx'                       */
  char   acq_time[11];   /*  Acquisition time 'TA____cc:xx'                */
  char   n_acq[11];      /*  Number of acquisitions 'Acq____xxxx'          */
  char   FOV[11];        /*  Field of view  'FOV____xx.x'                  */
  char   img_matrix[11]; /*  Image matrix 'M_xxxx*xxxx'                    */
  char   slice_thk[11];  /*  Slice thickness 'Thk___xxx.x'                 */
  char   slice_pos[11];  /*  Slice position  'Pos__xxxx.x'                 */
  char   slice_ori1[11]; /*  Slice orientation1 'aaa>bbb_nnn'              */
  char   slice_ori2[11]; /*  Slice orientation2 '___>ccc_mmm'              */
  char   gat_tr_mode[11];/*  Gating and triger mode 'G/T___a/nnn'          */
  char   prot13[11];     /*  Reserved area                                 */
  char   pat_pos_SW[11]; /*  Patient position and SW Vers. 'xx-x-xx_xx'    */
  char   rev_vid_swi[4]; /*  Reverse vide switch for image '<ESC>[7m'      */
  char   pat_ori_upp[4]; /*  Patient ori. disp. in upper part of im '_xxx' */
  char   norm_vid_swi[4];/*  Normal video switch for  image '<ESC>[0m'     */
  char   rev_vid_swg[4]; /*  Reverse video switch for text '<ESC>[7m'      */
  char   pat_ori_rgh[4]; /*  Patient ori. disp. in right part of im '_xxx' */
  char   norm_vid_swg[4];/*  Normal video switch for text '<ESC>[0m'       */
  char   img_origin[9];  /*  Image origin: 'MAGNETOM' or 'SOMATOM'         */
  char   field_str[6];   /*  Field strength 'x.xx_T'                       */
  char   hosp[25];       /*  Hospital name                                 */
  char   prot14[61];     /*  Reserved area                                 */
 };

/***************************************************************************/
