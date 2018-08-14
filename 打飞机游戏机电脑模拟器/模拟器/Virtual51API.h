//Virtual51 API Head File
//Copyright (C) 2010.3.22 Tan KaiShuai Inc.
//All Rights Reserved By Tan KaiShuai

#ifndef VIRTUAL51API_H
#define VIRTUAL51API_H

#include "Virtual51.h"

//Some DataType Define
typedef char INT8;
typedef unsigned char UINT8;
typedef short INT16;
typedef unsigned short UINT16;
typedef long INT32;
typedef unsigned long UINT32;
typedef int  INT;
typedef unsigned int  UINT;
typedef float FLOAT;
typedef double DOUBLE;
#define VOID void
typedef void *LPVOID;

//LCD SCREEN WIDTH
#define WIDTH 128

//LCD SCREEN HEIGHT
#define HEIGHT 64

//CHAR WIDTH
#define CHARWIDTH 4

//CHAR HEIGHT
#define CHARHEIGHT 8

//CHARS FONT MATRIX
UINT8 code g_CharsMatrix[][4]={
{0x00,0x00,0x00,0x00},/*" ",00*/
{0x00,0x00,0x5E,0x00},/*"!",01*/
{0x00,0x04,0x06,0x02},/*""",02*/
{0x7C,0x28,0x7C,0x00},/*"#",03*/
{0x00,0x6C,0xFE,0x64},/*"$",04*/
{0x1C,0x5C,0x70,0x7C},/*"%",05*/
{0x60,0x5C,0x3C,0x48},/*"&",06*/
{0x04,0x02,0x00,0x00},/*"'",07*/
{0x00,0x00,0x7C,0x82},/*"(",08*/
{0x00,0xC6,0x38,0x00},/*")",09*/
{0x00,0x18,0x3C,0x18},/*"*",10*/
{0x08,0x08,0x3E,0x08},/*"+",11*/
{0x80,0x40,0x00,0x00},/*",",12*/
{0x08,0x08,0x08,0x00},/*"-",13*/
{0x00,0x40,0x00,0x00},/*".",14*/
{0xC0,0x38,0x06,0x00},/*"/",15*/
{0x38,0x44,0x38,0x00},/*"0",16*/
{0x00,0x48,0x78,0x40},/*"1",17*/
{0x6C,0x54,0x4C,0x00},/*"2",18*/
{0x6C,0x4C,0x74,0x00},/*"3",19*/
{0x10,0x28,0x7C,0x00},/*"4",20*/
{0x7C,0x54,0x74,0x00},/*"5",21*/
{0x38,0x54,0x54,0x70},/*"6",22*/
{0x00,0x04,0x7C,0x04},/*"7",23*/
{0x6C,0x54,0x6C,0x00},/*"8",24*/
{0x1C,0x54,0x3C,0x00},/*"9",25*/
{0x00,0x00,0x48,0x00},/*":",26*/
{0x00,0x00,0xC8,0x00},/*";",27*/
{0x00,0x10,0x28,0x44},/*"<",28*/
{0x28,0x28,0x28,0x00},/*"=",29*/
{0x00,0x44,0x28,0x10},/*">",30*/
{0x06,0x52,0x0E,0x00},/*"?",31*/
{0x38,0x74,0x6C,0x78},/*"@",32*/
{0x40,0x7C,0x78,0x40},/*"A",33*/
{0x00,0x7C,0x4C,0x74},/*"B",34*/
{0x38,0x44,0x44,0x00},/*"C",35*/
{0x00,0x7C,0x44,0x38},/*"D",36*/
{0x00,0x7C,0x5C,0x44},/*"E",37*/
{0x00,0x7C,0x1C,0x04},/*"F",38*/
{0x38,0x64,0x24,0x20},/*"G",39*/
{0x00,0x7C,0x10,0x7C},/*"H",40*/
{0x00,0x44,0x7C,0x44},/*"I",41*/
{0x80,0x84,0xFC,0x04},/*"J",42*/
{0x00,0x7C,0x7C,0x44},/*"K",43*/
{0x00,0x7C,0x40,0x40},/*"L",44*/
{0x7C,0x7F,0x7C,0x00},/*"M",45*/
{0x00,0x7C,0x7C,0x7C},/*"N",46*/
{0x38,0x44,0x44,0x38},/*"O",47*/
{0x00,0x7C,0x14,0x1C},/*"P",48*/
{0x38,0x64,0x64,0xB8},/*"Q",49*/
{0x00,0x7C,0x0C,0x3C},/*"R",50*/
{0x4C,0x54,0x64,0x00},/*"S",51*/
{0x04,0x04,0x7C,0x04},/*"T",52*/
{0x00,0x7C,0x40,0x7C},/*"U",53*/
{0x04,0x3C,0x60,0x3C},/*"V",54*/
{0x0C,0x74,0x1C,0x7C},/*"W",55*/
{0x44,0x7C,0x7C,0x44},/*"X",56*/
{0x04,0x4C,0x70,0x4C},/*"Y",57*/
{0x44,0x7C,0x4C,0x00},/*"Z",58*/
{0x00,0x00,0xFE,0x82},/*"[",59*/
{0x00,0x1C,0xE0,0x00},/*"\",60*/
{0x00,0x82,0xFE,0x00},/*"]",61*/
{0x00,0x04,0x02,0x04},/*"^",62*/
{0x80,0x80,0x80,0x80},/*"_",63*/
{0x04,0x02,0x00,0x00},/*"'",64*/
{0x68,0x58,0x78,0x40},/*"a",65*/
{0x00,0x7C,0x48,0x78},/*"b",66*/
{0x70,0x48,0x48,0x00},/*"c",67*/
{0x78,0x48,0x7C,0x00},/*"d",68*/
{0x00,0x70,0x58,0x58},/*"e",69*/
{0x48,0x7C,0x4C,0x00},/*"f",70*/
{0x00,0xF8,0xD8,0xD8},/*"g",71*/
{0x00,0x7C,0x08,0x78},/*"h",72*/
{0x00,0x48,0x7C,0x40},/*"i",73*/
{0x00,0x88,0xFC,0x00},/*"j",74*/
{0x00,0x7C,0x78,0x48},/*"k",75*/
{0x00,0x44,0x7C,0x40},/*"l",76*/
{0x78,0x78,0x78,0x00},/*"m",77*/
{0x00,0x78,0x08,0x78},/*"n",78*/
{0x30,0x48,0x48,0x30},/*"o",79*/
{0x00,0xF8,0xC8,0x78},/*"p",80*/
{0x78,0xC8,0xF8,0x00},/*"q",81*/
{0x48,0x78,0x48,0x08},/*"r",82*/
{0x00,0x58,0x68,0x68},/*"s",83*/
{0x08,0x7C,0x40,0x00},/*"t",84*/
{0x00,0x78,0x40,0x78},/*"u",85*/
{0x08,0x38,0x40,0x38},/*"v",86*/
{0x18,0x68,0x18,0x78},/*"w",87*/
{0x48,0x78,0x78,0x48},/*"x",88*/
{0x88,0xD8,0x38,0x08},/*"y",89*/
{0x00,0x68,0x78,0x48},/*"z",90*/
{0x00,0x10,0xFE,0x82},/*"{",91*/
{0x00,0x00,0xFF,0x00},/*"|",92*/
{0x82,0xFE,0x10,0x00},/*"}",93*/
{0x03,0x01,0x02,0x03},/*"~",94*/
};

//SCREEN BUFFER
UINT8 xdata g_ScreenMatrix[HEIGHT/8][WIDTH];

//User Interface
//VOID setLocation(UINT8 x,UINT8 y);
VOID sendLCDCmd(UINT8 cmd);
//VOID delayTime(UINT mstime);
VOID clearAll();
//VOID inverseAll();
VOID refreshScreen(); //Only Shell it can get the result on LCD
//VOID drawRect(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2);
//VOID drawRectEx(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2);
//VOID fillRect(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2);
//VOID clearRect(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2);
//VOID drawCharImage(UINT8 x,UINT8 y,UINT8 pic[]);
VOID drawLargeImage(UINT8 x,UINT8 y,UINT8 pic[][WIDTH],UINT8 width,UINT8 height);
//VOID drawPoint(UINT8 x,UINT8 y);
//VOID clearPoint(UINT8 x,UINT8 y);
//VOID drawLine(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2);
VOID drawChar(UINT8 x,UINT8 y,UINT8 ch);
//VOID drawCharEx(UINT8 x,UINT8 y,UINT8 ch);
VOID drawString(UINT8 x,UINT8 y,UINT8 str[]);
//VOID drawStringEx(UINT8 x,UINT8 y,UINT8 str[]);
//UINT8 getScreenInfo(UINT8 page,UINT8 coln);
//VOID setScreenInfo(UINT8 page,UINT8 coln,UINT8 val);
//User Interface

//Set Current Location
/*
VOID setLocation(UINT8 x,UINT8 y){
	LCDCMR=SET_PAGE|(x/8);
	LCDCMR=SET_COL|y;
}*/

//Send LCD Command To Execute
VOID sendLCDCmd(UINT8 cmd){
	LCDCMR=cmd;
}

//Delay MillSecond with a Timer	in MCS-51
/*
VOID delayTime(UINT mstime){
	UINT clk=0;
	UINT8 preTR0,preTMOD,preTH0,preTL0;
	preTMOD=TMOD;preTR0=TR0;preTH0=TH0;preTL0=TL0;
	TMOD&=0xf0;TMOD|=0x01;TR0=1;
	while(clk<mstime){
		TH0=0;TL0=20;while(TH0<4){};++clk;
	}
	TH0=preTH0;TL0=preTL0;TMOD=preTMOD;TR0=preTR0;
}*/

//Clear All Screen Buffer
VOID clearAll(){
	UINT8 page,coln;
	for(page=0;page<HEIGHT/8;++page){
		for(coln=0;coln<WIDTH;++coln){
			g_ScreenMatrix[page][coln]=0x00;
		}
	}
}

//Inverse All Screen Buffer
/*
VOID inverseAll(){
	UINT8 page,coln;
	for(page=0;page<HEIGHT/8;++page){
		for(coln=0;coln<WIDTH;++coln){
			g_ScreenMatrix[page][coln]=~g_ScreenMatrix[page][coln];
		}
	}
}*/

//Refresh Screen to display
VOID refreshScreen(){
	UINT8 page,coln;
	LCDCMR=INC_enb;
	for(page=0;page<HEIGHT/8;++page){
		LCDCMR=SET_PAGE|page;
	    LCDCMR=SET_COL|0;
		for(coln=0;coln<WIDTH;++coln){
			LCDDR=g_ScreenMatrix[page][coln];
		}
	}
	LCDCMR=INC_dis;
}

/*
//Draw Rectangle
VOID drawRect(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2){
	UINT8 line,coln;
	//Draw Vertical Lines
	for(line=x1;line<=x2;++line){
		g_ScreenMatrix[line/8][y1]|=0x01<<(7-line%8);
		g_ScreenMatrix[line/8][y2]|=0x01<<(7-line%8);
	}
	//Draw Horztal Lines
	for(coln=y1;coln<=y2;++coln){
		g_ScreenMatrix[x1/8][coln]|=0x01<<(7-x1%8);
		g_ScreenMatrix[x2/8][coln]|=0x01<<(7-x2%8);
	}
}

//Draw Rectangle Ex
//Use while drawRect not working well
VOID drawRectEx(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2){
	UINT8 line,coln;
	//Draw Vertical Lines
	for(line=x1;line<=x2;++line){
		g_ScreenMatrix[line/8][y1]|=0x01<<(7-line%8);
		g_ScreenMatrix[line/8][y2]|=0x01<<(7-line%8);
	}
	//Draw Horztal Lines
	for(coln=y1;coln<=y2;++coln){
		g_ScreenMatrix[x1/8-1][coln]|=0x01<<(7-x1%8);
		g_ScreenMatrix[x2/8-1][coln]|=0x01<<(7-x2%8);
	}
}

//Fill Rectangle
VOID fillRect(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2){
	UINT8 line,coln;
	for(line=x1;line<=x2;++line){
		for(coln=y1;coln<=y2;++coln){
			g_ScreenMatrix[line/8][coln]|=0x01<<(7-line%8);
		}
	}
}

//Clear Rect
VOID clearRect(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2){
	UINT8 line,coln;
	for(line=x1;line<=x2;++line){
		for(coln=y1;coln<=y2;++coln){
			g_ScreenMatrix[line/8][coln]&=~(0x01<<(7-line%8));
		}
	}
}

//The Char Image must have a size of CHARWIDTH*CHARHEIGHT
VOID drawCharImage(UINT8 x,UINT8 y,UINT8 pic[]){
	UINT8 line,coln;
	for(line=0;line<CHARHEIGHT;++line){
		for(coln=0;coln<CHARWIDTH;++coln){
			g_ScreenMatrix[(x+line)/8][y+coln]|=pic[coln]&(0x01<<(7-line));
		}
	}
}
*/

//Draw Large Image must have a height of 8 at least
VOID drawLargeImage(UINT8 x,UINT8 y,UINT8 pic[][WIDTH],UINT8 width,UINT8 height){
	UINT8 page,page0,coln;
	for(page=page0=x/8;page<(x+height)/8;++page){
		for(coln=y;coln<(y+width);++coln){
			g_ScreenMatrix[page][coln]|=pic[page-page0][coln];
		}
	}
}

/*
//Draw Point
VOID drawPoint(UINT8 x,UINT8 y){
   g_ScreenMatrix[(x/8)%(HEIGHT/8)][y%WIDTH]|=0x01<<(7-x%8);
}

//Clear Point
VOID clearPoint(UINT8 x,UINT8 y){
   g_ScreenMatrix[(x/8)%(HEIGHT/8)][y%WIDTH]&=~(0x01<<(7-x%8));
}

//Draw Line
VOID drawLine(UINT8 x1,UINT8 y1,UINT8 x2,UINT8 y2){
	UINT8 line,coln;DOUBLE k;
	if(y1==y2){
		for(line=x1;line<=x2;++line){
			g_ScreenMatrix[line/8][y1]|=0x01<<(7-line%8);
		}
	}else if(x1==x2){
		for(coln=y1;coln<=y2;++coln){
			//g_ScreenMatrix[x1/8][coln]|=0x01<<(7-x1%8);
			g_ScreenMatrix[x1/8][coln]|=0x01<<x1%8;
		}
	}else{
		k=(DOUBLE)(x2-x1)/(DOUBLE)(y2-y1);
		for(coln=y1;coln<=y2;++coln){
			line=x1+(UINT8)(k*(coln-y1));
			//g_ScreenMatrix[line/8][coln]|=0x01<<(7-line%8);
			g_ScreenMatrix[line/8][coln]|=0x01<<line%8;
		}
	}
}*/

//Draw Char
VOID drawChar(UINT8 x,UINT8 y,UINT8 ch){
	UINT8 line,coln;
	ch=(ch>32&&ch<128)?(ch-32):0;
	//drawCharImage(x,y,g_CharsMatrix[ch]);
	for(line=0;line<CHARHEIGHT;++line){
		for(coln=0;coln<CHARWIDTH;++coln){
			g_ScreenMatrix[(x+line)/8][y+coln]|=g_CharsMatrix[ch][coln]&(0x01<<(7-line));
		}
	}
}

//Draw Char	Ex
/*
VOID drawCharEx(UINT8 x,UINT8 y,UINT8 ch){
	UINT8 page,coln;
	ch=(ch>32&&ch<128)?(ch-32):0;
	for(page=x/8,coln=0;coln<4;++coln){
		g_ScreenMatrix[page][y+coln]|=g_CharsMatrix[ch][coln];
	}
}*/

//Draw String
VOID drawString(UINT8 x,UINT8 y,UINT8 str[]){
	UINT8 ii=0;
  	while(str[ii]!='\0'){
		drawChar(x,y,str[ii]);++ii;y+=CHARWIDTH;
	}
}

/*
//Draw String Ex
VOID drawStringEx(UINT8 x,UINT8 y,UINT8 str[]){
	UINT8 ii=0;
  	while(str[ii]!='\0'){
		drawCharEx(x,y,str[ii]);++ii;y+=CHARWIDTH;
	}
}


UINT8 getScreenInfo(UINT8 page,UINT8 coln){
	return g_ScreenMatrix[page][coln];
}


VOID setScreenInfo(UINT8 page,UINT8 coln,UINT8 val){
	g_ScreenMatrix[page][coln]=val;
}*/

#endif  //VIRTUAL51API_H
