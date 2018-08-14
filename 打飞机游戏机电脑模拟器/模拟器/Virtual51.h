/*--------------------------------------------------------------------------
Virtual51.h
Header file for the Virtual51.
Copyright(c)2004-2006,Macrocai.All rights reserved.
--------------------------------------------------------------------------*/

sfr P0      = 0x80;
sfr SP      = 0x81;
sfr DPL     = 0x82;
sfr DPH     = 0x83;
sfr PCON    = 0x87;
sfr TCON    = 0x88;
sfr TMOD    = 0x89;
sfr TL0     = 0x8A;
sfr TL1     = 0x8B;
sfr TH0     = 0x8C;
sfr TH1     = 0x8D;
sfr P1      = 0x90;
sfr SCON    = 0x98;
sfr SBUF    = 0x99;
sfr TGCON   = 0x9B;
sfr TFR     = 0x9C;
sfr KEYPAD  = 0x9F;
sfr P2      = 0xA0;
sfr LCDDR   = 0xA3;
sfr LCDCMR  = 0xA4;
sfr RTCON   = 0xA5;
sfr XRAMSH  = 0xA6;
sfr XRAMSL  = 0xA7;
sfr IE      = 0xA8;
sfr P3      = 0xB0;
sfr IP      = 0xB8;
//sfr T2CON   = 0xC8;
//sfr T2MOD   = 0xC9;
//sfr RCAP2L  = 0xCA;
//sfr RCAP2H  = 0xCB;
//sfr TL2     = 0xCC;
//sfr TH2     = 0xCD;
sfr PSW     = 0xD0;
sfr ACC     = 0xE0;
sfr B       = 0xF0;
sfr LPTDTR  = 0xD8;
sfr LPTSTR  = 0xE8;
sfr LPTCTR  = 0xF8;

/*------------------------------------------------
P0 Bit Registers
------------------------------------------------*/
sbit P0_0 = 0x80;
sbit P0_1 = 0x81;
sbit P0_2 = 0x82;
sbit P0_3 = 0x83;
sbit P0_4 = 0x84;
sbit P0_5 = 0x85;
sbit P0_6 = 0x86;
sbit P0_7 = 0x87;

/*------------------------------------------------
TCON Bit Registers
------------------------------------------------*/
sbit IT0  = 0x88;
sbit IE0  = 0x89;
sbit IT1  = 0x8A;
sbit IE1  = 0x8B;
sbit TR0  = 0x8C;
sbit TF0  = 0x8D;
sbit TR1  = 0x8E;
sbit TF1  = 0x8F;

/*------------------------------------------------
PCON Bit Values
------------------------------------------------*/
#define IDL_    0x01
#define GF0_    0x04
#define GF1_    0x08
#define SMOD_   0x80

/*------------------------------------------------
TMOD Bit Values
------------------------------------------------*/
#define T0_M0_   0x01
#define T0_M1_   0x02
#define T0_CT_   0x04
#define T0_GATE_ 0x08
#define T1_M0_   0x10
#define T1_M1_   0x20
#define T1_CT_   0x40
#define T1_GATE_ 0x80

#define T1_MASK_ 0xF0
#define T0_MASK_ 0x0F

/*------------------------------------------------
P1 Bit Registers
------------------------------------------------*/
sbit P1_0 = 0x90;
sbit P1_1 = 0x91;
sbit P1_2 = 0x92;
sbit P1_3 = 0x93;
sbit P1_4 = 0x94;
sbit P1_5 = 0x95;
sbit P1_6 = 0x96;
sbit P1_7 = 0x97;

/*------------------------------------------------
SCON Bit Registers
------------------------------------------------*/
sbit RI   = 0x98;
sbit TI   = 0x99;
sbit RB8  = 0x9A;
sbit TB8  = 0x9B;
sbit REN  = 0x9C;
sbit SM2  = 0x9D;
sbit SM1  = 0x9E;
sbit SM0  = 0x9F;

/*------------------------------------------------
P2 Bit Registers
------------------------------------------------*/
sbit P2_0 = 0xA0;
sbit P2_1 = 0xA1;
sbit P2_2 = 0xA2;
sbit P2_3 = 0xA3;
sbit P2_4 = 0xA4;
sbit P2_5 = 0xA5;
sbit P2_6 = 0xA6;
sbit P2_7 = 0xA7;

/*------------------------------------------------
IE Bit Registers
------------------------------------------------*/
sbit EX0  = 0xA8;
sbit ET0  = 0xA9;
sbit EX1  = 0xAA;
sbit ET1  = 0xAB;
sbit ES   = 0xAC;
//sbit ET2  = 0xAD;
sbit EA   = 0xAF;

/*------------------------------------------------
P3 Bit Registers (Mnemonics & Ports)
------------------------------------------------*/
sbit P3_0 = 0xB0;
sbit P3_1 = 0xB1;
sbit P3_2 = 0xB2;
sbit P3_3 = 0xB3;
sbit P3_4 = 0xB4;
sbit P3_5 = 0xB5;
sbit P3_6 = 0xB6;
sbit P3_7 = 0xB7;

sbit RXD  = 0xB0;
sbit TXD  = 0xB1;
sbit INT0 = 0xB2;
sbit INT1 = 0xB3;
sbit T0   = 0xB4;
sbit T1   = 0xB5;
sbit WR   = 0xB6;
sbit RD   = 0xB7;

/*------------------------------------------------
IP Bit Registers
------------------------------------------------*/
sbit PX0  = 0xB8;
sbit PT0  = 0xB9;
sbit PX1  = 0xBA;
sbit PT1  = 0xBB;
sbit PS   = 0xBC;
//sbit PT2  = 0xBD;

/*------------------------------------------------
T2CON Bit Registers
------------------------------------------------
sbit CP_RL2= 0xC8;
sbit C_T2 = 0xC9;
sbit TR2  = 0xCA;
sbit EXEN2= 0xCB;
sbit TCLK = 0xCC;
sbit RCLK = 0xCD;
sbit EXF2 = 0xCE;
sbit TF2  = 0xCF;
*/

/*------------------------------------------------
T2MOD Bit Values
------------------------------------------------
#define DCEN_   0x01
#define T2OE_   0x02
*/

/*------------------------------------------------
PSW Bit Registers
------------------------------------------------*/
sbit P    = 0xD0;
sbit FL   = 0xD1;
sbit OV   = 0xD2;
sbit RS0  = 0xD3;
sbit RS1  = 0xD4;
sbit F0   = 0xD5;
sbit AC   = 0xD6;
sbit CY   = 0xD7;

/*------------------------------------------------
LPT Data Port Bit Registers
------------------------------------------------*/
sbit LPT_Pin02    = 0xD8;//2-Input/Output
sbit LPT_Pin03    = 0xD9;//3-Input/Output
sbit LPT_Pin04    = 0xDA;//4-Input/Output
sbit LPT_Pin05    = 0xDB;//5-Input/Output
sbit LPT_Pin06    = 0xDC;//6-Input/Output
sbit LPT_Pin07    = 0xDD;//7-Input/Output
sbit LPT_Pin08    = 0xDE;//8-Input/Output
sbit LPT_Pin09    = 0xDF;//9-Input/Output

/*------------------------------------------------
LPT Status Port Bit Registers
------------------------------------------------*/
sbit LPT_Pin15    = 0xEB;//15-Input only
sbit LPT_Pin13    = 0xEC;//13-Input only
sbit LPT_Pin12    = 0xED;//12-Input only
sbit LPT_Pin10    = 0xEE;//10-Input only
sbit LPT_Pin11    = 0xEF;//11-Input only

/*------------------------------------------------
LPT Control Port Bit Registers
------------------------------------------------*/
sbit LPT_Pin01    = 0xF8;//01-Output only
sbit LPT_Pin14    = 0xF9;//14-Output only
sbit LPT_Pin16    = 0xFA;//16-Output only
sbit LPT_Pin17    = 0xFB;//17-Output only
sbit LPT_IOctrl   = 0xFD;//  -Read/Write

/*------------------------------------------------
TGCON Bit Values
------------------------------------------------*/
#define TGE      0x80

/*------------------------------------------------
LCDCMR Command
Read Status:x-x-ON/OFF#-INV-AINC-CP2-CP1-CP0
------------------------------------------------*/
#define SET_COL  0x00
#define SET_PAGE 0xA0
#define INC_dis  0xB0
#define INC_enb  0xB1
#define INV_dis  0xB2
#define INV_enb  0xB3
#define LCD_off  0xB4
#define LCD_on   0xB5
#define Cls      0xB6

/*------------------------------------------------
Real Time
------------------------------------------------*/
#define Msec_BCD  0x00
#define Sec_BCD   0x01
#define Min_BCD   0x02
#define Hou_BCD   0x03
#define Day_BCD   0x04
#define Week_BCD  0x05
#define Mon_BCD   0x06
#define Year_BCD  0x07

#define Msec_BIN  0x08
#define Sec_BIN   0x09
#define Min_BIN   0x0A
#define Hou_BIN   0x0B
#define Day_BIN   0x0C
#define Week_BIN  0x0D
#define Mon_BIN   0x0E
#define Year_BIN  0x0F
