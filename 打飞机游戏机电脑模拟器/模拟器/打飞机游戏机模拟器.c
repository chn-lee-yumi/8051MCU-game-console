/* 
                   _ooOoo_ 
                  o8888888o 
                  88" . "88 
                  (| -_- |) 
                  O\  =  /O 
               ____/`---'\____ 
             .'  \\|     |//  `. 
            /  \\|||  :  |||//  \ 
           /  _||||| -:- |||||-  \ 
           |   | \\\  -  /// |   | 
           | \_|  ''\---/''  |   | 
           \  .-\__  `-`  ___/-. / 
         ___`. .'  /--.--\  `. . __ 
      ."" '<  `.___\_<|>_/___.'  >'"". 
     | | :  `- \`.;`\ _ /`;.`/ - ` : | | 
     \  \ `-.   \_ __\ /__ _/   .-` /  / 
======`-.____`-.___\_____/___.-`____.-'====== 
                   `=---=' 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 
         ���汣��       ����BUG 
*/
/*��ɻ���Ϸ��*/
#include "Virtual51API.h"
//���涨�峡��
#define SCENE_MENU_MAIN 1
#define SCENE_PLAYING 5
#define SCENE_GAME_OVER 6
#define SCENE_GAME_WIN 7
unsigned char scene = SCENE_MENU_MAIN;//����������ִ�й���ʱ��
//���涨����Ϸ�˵�
const unsigned char code menu_main[][WIDTH]={
0x00,0xC0,0x00,0x00,0x80,0x80,0x90,0xB0,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x20,0xA0,0x20,
0x20,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0xC0,0x00,
0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xA0,0xAC,
0x24,0xDB,0x04,0x01,0x16,0x08,0x78,0xC4,0x88,0xF6,0x08,0x09,0x08,0x00,0x00,0x00,
0x62,0x32,0x12,0x1A,0x0A,0x0C,0x00,0x00,0x00,0x02,0x06,0x04,0x08,0x08,0xF7,0x08,
0xE8,0x15,0x00,0x08,0x08,0x08,0x0C,0x00,0x04,0x04,0x84,0x79,0xC2,0x62,0x3D,0x02,
0x02,0x02,0x77,0xE4,0x24,0x34,0x15,0x14,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x03,0x05,0x00,0x00,0x00,0x02,0x03,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x01,0x00,
0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x01,0x01,0x00,0x03,0x04,0x04,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
#define MENU_ITEM_START 1 //��ʼ��Ϸ
//�����������ֺ���
//��Ϸ���
void StartGame();//��ʼ��Ϸ
void ButtonManage();//��������
void GameShoot();//�����ӵ�
void GameMoveBullet();//�ƶ��ӵ�
void GameMoveEnemy();//�ƶ��л�
void GameMoveAerolite();//�ƶ���ʯ
void GameOver();//��Ϸ����
void GameWin();//��Ϸʤ��
//LCD���
void DrawScreen(unsigned char x,unsigned char y,unsigned char drawing_model);//��Ϸ��ʾ
//���涨����ֱ���
unsigned char display_mem_buff[2]={0x00,0x00};//��ʾ����
unsigned char my_plane_x,my_plane_y;//�ҵķɻ����ꡣ
unsigned int game_speed=5;//��Ϸ�ٶȣ����ƶ�ʱ��1��ֵ����Χ1~10����ʱ��1��ֵΪ65535-2500+200*game_speed
unsigned int game_shoot_speed_counter=0;//�����ӵ�����ʱ��������Timer1ÿ�ж�һ�����ֵ��1
unsigned int game_shoot_speed;//game_shoot_speed_counter�����ֵ�ͷ����ӵ�
unsigned int game_bullet_speed_counter=0;//�ӵ��ƶ�����ʱ��������Timer1ÿ�ж�һ�����ֵ��1
unsigned int game_bullet_speed;
unsigned int game_enemy_speed_counter=0;//�����ƶ�����ʱ��������Timer1ÿ�ж�һ�����ֵ��1
unsigned int game_enemy_speed;
unsigned char game_map[17][8];//8��16�еĵ�ͼ����17��������׼�������ĵл�
unsigned char game_progress;//��Ϸ���ȣ���ʾ�ɻ��ߵ���ͼ������
unsigned char game_level_selected=4;//��ǰѡ�еĹؿ�����ģ������ֻ��1�أ������������BOSSǿ�ȣ���Χ1~13��
unsigned char game_boss_hp;//BOSSѪ��
unsigned char game_boss_attack_speed_counter=0;//BOSS�����ٶ�
unsigned char game_aerolite_speed;//��ʯ�ٶȣ�BOSS�ٻ���ʯ������
game_aerolite_speed_counter=0;
//���涨����Ϸģ��
const unsigned char code model[11][8]={
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},{0x18,0x18,0x19,0xFF,0xFF,0x19,0x18,0x18},
    {0x18,0x38,0x19,0xFF,0xFF,0x19,0x38,0x18},{0x38,0x38,0x19,0xFF,0xFF,0x19,0x38,0x38},
    {0x38,0x78,0x19,0xFF,0xFF,0x19,0x78,0x38},{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
    {0x18,0x7E,0x7E,0xFF,0xFF,0x7E,0x7E,0x18},{0x3C,0x3D,0x19,0xFF,0xFF,0x19,0x3D,0x3C},
    {0x3C,0x1D,0x19,0xFF,0xFF,0x19,0x1D,0x3C},{0x00,0x00,0x00,0x7E,0x7E,0x00,0x00,0x00},
    {0x18,0x18,0x9C,0xFF,0xFF,0x9C,0x18,0x18}
};//8x8��ģ�ͣ��ҵķɻ������˷ɻ�1�����˷ɻ�2�����˷ɻ�3�����˷ɻ�4���ϰ����ʯ��BOSS����ѪBOSS���ӵ�
#define MY_PLANE 10 //�ҵķɻ�
#define ENEMY_PLANE1 1 //���˷ɻ�1
#define ENEMY_PLANE2 2 //���˷ɻ�2
#define ENEMY_PLANE3 3 //���˷ɻ�3
#define ENEMY_PLANE4 4 //���˷ɻ�4
#define OBSTACLE 5 //�ϰ���
#define AEROLITE 6 //��ʯ
#define ENEMY_BOSS 7 //BOSS
#define ENEMY_BOSS_DILAPIDATED 8 //��ѪBOSS
#define BULLET 9 //�ӵ�
#define NOTHING 0 //��
//���涨���ͼ
#define MAP_LENGTH 30 //��Ϊһ����������һ����ͼ�����Ե�ͼ����������Ϊ512/8=64��������ʱ��д��ô�󣬱��ͼ���۵�
const unsigned char code game_map_level[1][MAP_LENGTH][8]= //�ؿ��������ȣ���ͼ��ȡ�
{
    {//ģ����ֻ��1��
        {0,0,5,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,1,1,0,0,0},
        {0,5,0,0,0,0,0,0},
        {0,0,1,0,0,1,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,1,0,0,1,0,0},
        {0,0,0,0,0,0,0,0},
        {0,1,0,1,1,0,1,0},
        {0,0,0,0,0,0,0,0},
        {1,0,1,0,0,1,0,1},
        {0,0,0,0,0,0,0,0},
        {0,0,1,1,1,1,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,1,0,0,1,0,0},
        {0,0,0,1,1,0,0,0},
        {0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0},
        {0,0,1,0,0,1,0,0},
        {1,0,2,0,2,0,2,0},
        {0,2,0,2,0,1,0,2},
        {2,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,1},
        {0,0,3,0,0,3,0,0},
        {0,3,0,3,3,0,3,0},
        {0,0,0,0,0,0,0,0},
        {4,0,4,0,0,4,0,4},
        {0,0,0,5,0,0,0,0},
        {0,4,0,0,0,5,4,0}
    }
};//��ͼ���˽���

char virtualkey_delay=0;//����ģ���������������ʱ��Ϊ˫��ʱ��virtualkey��1
char virtualkey=0;
/************************************************************************************************
*************************************************************************************************
***************************************���濪ʼ������********************************************
*************************************************************************************************
************************************************************************************************/

void main(){
	//��ʼ��LCD
	sendLCDCmd(LCD_on);
	clearAll();
	drawLargeImage(20,0,menu_main,WIDTH,20);
	refreshScreen();
    //��ʼ����ʱ��
    TMOD=0x11;//��ʱ��1Ϊ16λ��ʱ��������������Ϸ�ٶ�
    EA=1;
    ET0=1;
    ET1=1;
    TH0=(65535-10000)/255;//50ms
    TL0=(65535-10000)%255;
    TR0=1;
    //ѭ����ⰴ��
    while(1){
        if(virtualkey==1){
            ButtonManage();
        }
        if(scene==SCENE_PLAYING){
            StartGame();
        }
    }
}

void Timer1() interrupt 3{
    //������Ϸ�ٶȵĶ�ʱ��
    game_shoot_speed_counter++;
    game_bullet_speed_counter++;
    game_enemy_speed_counter++;
    game_aerolite_speed_counter++;
    TH1=(65535-2500+200*game_speed)/255;
    TL1=(65535-2500+200*game_speed)%255;
}

void Timer0() interrupt 1{
    //�������������ʱ�Ķ�ʱ������Ҫ100ms��
    virtualkey_delay++;
    if(virtualkey_delay==2){
        virtualkey_delay=0;
        virtualkey=1;
    }
    TH0=(65535-10000)/255;//50ms
    TL0=(65535-10000)%255;
}

void StartGame(){
    //��ʼ��Ϸ
    unsigned char i,j;
    for(i=0;i<=16;i++){
        for(j=0;j<=7;j++){
            game_map[i][j]=NOTHING;
        }
    }
    clearAll();
    game_shoot_speed=400;
    game_bullet_speed=100;
    game_enemy_speed=800;
    game_aerolite_speed=200;
    game_boss_hp=10+game_level_selected*3;
    game_progress=0;
    my_plane_x=0;
    my_plane_y=2;
    game_map[my_plane_x][my_plane_y]=MY_PLANE;
    DrawScreen(my_plane_x,my_plane_y,MY_PLANE);
    TH1=(65535-1000)/255;
    TL1=(65535-1000)%255;
    TR1=1;
    while(scene==SCENE_PLAYING){
        if(virtualkey==1){
            ButtonManage();
        }
        if(game_bullet_speed_counter>=game_bullet_speed){
            GameMoveBullet();//�ƶ��ӵ�
            game_bullet_speed_counter=0;
        }
        if(game_enemy_speed_counter>=game_enemy_speed){
            GameMoveEnemy();//�ƶ�����
            game_enemy_speed_counter=0;
        }
        if(game_shoot_speed_counter>=game_shoot_speed){
            GameShoot();//�����ӵ�
            game_shoot_speed_counter=0;
        }
        if(game_aerolite_speed_counter>=game_aerolite_speed){
            GameMoveAerolite();//�ƶ���ʯ
            game_aerolite_speed_counter=0;
        }
	    refreshScreen();
    }
}

void ButtonManage(){
    //��������˼·���жϰ�ť�Ƿ��£���������ˣ��жϵ�ǰ�ĳ�����scene����Ȼ�������Ӧ����
    //���˵����������ʼ��Ϸ��
    switch(KEYPAD){//��ȡ������̵İ���
        case 0x5e://��
            //drawString(32,32,"up");refreshScreen();
            if(scene==SCENE_MENU_MAIN){//���˵�
                scene=SCENE_PLAYING;
            }else if(scene==SCENE_PLAYING){//������Ϸ
                if(my_plane_y>0){
                    if(game_map[my_plane_x][my_plane_y-1]!=NOTHING){
                        GameOver();
                    }else{
                        game_map[my_plane_x][my_plane_y]=NOTHING;
                        DrawScreen(my_plane_x,my_plane_y,NOTHING);
                        my_plane_y--;
                        game_map[my_plane_x][my_plane_y]=MY_PLANE;
                        DrawScreen(my_plane_x,my_plane_y,MY_PLANE);
                    }
                }
            }
            break;
        case 0x56://��
            //drawString(32,32,"down");refreshScreen();
            if(scene==SCENE_MENU_MAIN){//���˵�
                scene=SCENE_PLAYING;
            }else if(scene==SCENE_PLAYING){//������Ϸ
                if(my_plane_y<7){
                    if(game_map[my_plane_x][my_plane_y+1]!=NOTHING){
                        GameOver();
                    }else{
                        game_map[my_plane_x][my_plane_y]=NOTHING;
                        DrawScreen(my_plane_x,my_plane_y,NOTHING);
                        my_plane_y++;
                        game_map[my_plane_x][my_plane_y]=MY_PLANE;
                        DrawScreen(my_plane_x,my_plane_y,MY_PLANE);
                    }
                }
            }
            break;
        case 0x3c://��
            //drawString(32,32,"left");refreshScreen();
            if(scene==SCENE_MENU_MAIN){//���˵�
                scene=SCENE_PLAYING;
            }else if(scene==SCENE_PLAYING){//������Ϸ
                if(my_plane_x>0){
                    if(game_map[my_plane_x-1][my_plane_y]!=NOTHING){
                        GameOver();
                    }else{
                        game_map[my_plane_x][my_plane_y]=NOTHING;
                        DrawScreen(my_plane_x,my_plane_y,NOTHING);
                        my_plane_x--;
                        game_map[my_plane_x][my_plane_y]=MY_PLANE;
                        DrawScreen(my_plane_x,my_plane_y,MY_PLANE);
                    }
                }
            }
            break;
        case 0x3e://��
            //drawString(32,32,"right");refreshScreen();
            if(scene==SCENE_MENU_MAIN){//���˵�
                scene=SCENE_PLAYING;
            }else if(scene==SCENE_PLAYING){//������Ϸ
                if(my_plane_x<15){
                    if(game_map[my_plane_x+1][my_plane_y]!=NOTHING && game_map[my_plane_x+1][my_plane_y]!=BULLET){
                        GameOver();
                    }else{
                        game_map[my_plane_x][my_plane_y]=NOTHING;
                        DrawScreen(my_plane_x,my_plane_y,NOTHING);
                        my_plane_x++;
                        game_map[my_plane_x][my_plane_y]=MY_PLANE;
                        DrawScreen(my_plane_x,my_plane_y,MY_PLANE);
                    }
                }
            }
            break;
    }
}

void GameShoot(){
    //�����ӵ�
    if(game_map[my_plane_x+1][my_plane_y]==ENEMY_PLANE1){
        game_map[my_plane_x+1][my_plane_y]=NOTHING;
        DrawScreen(my_plane_x+1,my_plane_y,NOTHING);
    }else if(game_map[my_plane_x+1][my_plane_y]==ENEMY_PLANE2){
        game_map[my_plane_x+1][my_plane_y]=ENEMY_PLANE1;
        DrawScreen(my_plane_x+1,my_plane_y,ENEMY_PLANE1);
    }else if(game_map[my_plane_x+1][my_plane_y]==ENEMY_PLANE3){
        game_map[my_plane_x+1][my_plane_y]=ENEMY_PLANE2;
        DrawScreen(my_plane_x+1,my_plane_y,ENEMY_PLANE2);
    }else if(game_map[my_plane_x+1][my_plane_y]==ENEMY_PLANE4){
        game_map[my_plane_x+1][my_plane_y]=ENEMY_PLANE3;
        DrawScreen(my_plane_x+1,my_plane_y,ENEMY_PLANE3);
    }else if(game_map[my_plane_x+1][my_plane_y]==ENEMY_BOSS||game_map[my_plane_x+1][my_plane_y]==ENEMY_BOSS_DILAPIDATED){
        game_boss_hp--;
        if(game_boss_hp==0){
            GameWin();
        }else if(game_boss_hp<=5){
            game_map[my_plane_x+1][my_plane_y]=ENEMY_BOSS_DILAPIDATED;
            DrawScreen(my_plane_x+1,my_plane_y,ENEMY_BOSS_DILAPIDATED);
        }else{
            game_map[my_plane_x+1][my_plane_y]=ENEMY_BOSS;
            DrawScreen(my_plane_x+1,my_plane_y,ENEMY_BOSS);
        }
    }else if(game_map[my_plane_x+1][my_plane_y]==OBSTACLE||game_map[my_plane_x+1][my_plane_y]==AEROLITE){
        //�������ӵ�
    }else{
        game_map[my_plane_x+1][my_plane_y]=BULLET;
        DrawScreen(my_plane_x+1,my_plane_y,BULLET);
    }
}

void GameMoveBullet(){
    //�ƶ��ӵ�
    char i,j;
    for(i=15;i>=0;i--){
        for(j=0;j<=7;j++){
            if(game_map[i][j]==BULLET){
                game_map[i][j]=NOTHING;
                DrawScreen(i,j,NOTHING);
                if(i==15){
                    if(game_map[i+1][j]==ENEMY_PLANE1){
                        game_map[i+1][j]=NOTHING;
                    }else if(game_map[i+1][j]==ENEMY_PLANE2){
                        game_map[i+1][j]=ENEMY_PLANE1;
                    }else if(game_map[i+1][j]==ENEMY_PLANE3){
                        game_map[i+1][j]=ENEMY_PLANE2;
                    }else if(game_map[i+1][j]==ENEMY_PLANE4){
                        game_map[i+1][j]=ENEMY_PLANE3;
                    }
                }else{
                    if(game_map[i+1][j]==ENEMY_PLANE1){
                        game_map[i+1][j]=NOTHING;
                        DrawScreen(i+1,j,NOTHING);
                    }else if(game_map[i+1][j]==ENEMY_PLANE2){
                        game_map[i+1][j]=ENEMY_PLANE1;
                        DrawScreen(i+1,j,ENEMY_PLANE1);
                    }else if(game_map[i+1][j]==ENEMY_PLANE3){
                        game_map[i+1][j]=ENEMY_PLANE2;
                        DrawScreen(i+1,j,ENEMY_PLANE2);
                    }else if(game_map[i+1][j]==ENEMY_PLANE4){
                        game_map[i+1][j]=ENEMY_PLANE3;
                        DrawScreen(i+1,j,ENEMY_PLANE3);
                    }else if(game_map[i+1][j]==ENEMY_BOSS||game_map[i+1][j]==ENEMY_BOSS_DILAPIDATED){
                        game_boss_hp--;
                        if(game_boss_hp==0){
                            game_map[i+1][j]=NOTHING;
                            DrawScreen(i+1,j,NOTHING);
                            GameWin();
                        }else if(game_boss_hp<=5){
                            game_map[i+1][j]=ENEMY_BOSS_DILAPIDATED;
                            DrawScreen(i+1,j,ENEMY_BOSS_DILAPIDATED);//��ѪBOSS��ģ��
                        }else{
                            DrawScreen(i+1,j,ENEMY_BOSS);
                        }
                    }else if(game_map[i+1][j]==OBSTACLE||game_map[i+1][j]==AEROLITE){
                        //�ӵ�ײû��
                    }else{
                        game_map[i+1][j]=BULLET;
                        DrawScreen(i+1,j,BULLET);
                    }
                }
            }
        }
    }
}

void GameMoveEnemy(){
    //�ƶ��л�
    char i,j;
    for(i=0;i<=16;i++){
        for(j=0;j<=7;j++){
            if(i==0 && (game_map[i][j]==ENEMY_PLANE1||game_map[i][j]==ENEMY_PLANE2||game_map[i][j]==ENEMY_PLANE3||game_map[i][j]==ENEMY_PLANE4||game_map[i][j]==OBSTACLE)){
                game_map[i][j]=NOTHING;
                DrawScreen(i,j,NOTHING);
            }else{
                if(game_map[i][j]==ENEMY_PLANE1){
                    game_map[i][j]=NOTHING;
                    if(i!=16) DrawScreen(i,j,NOTHING);
                    if(game_map[i-1][j]==BULLET){
                        game_map[i-1][j]=NOTHING;
                        DrawScreen(i-1,j,NOTHING);
                    }else if(game_map[i-1][j]==MY_PLANE){
                        GameOver();
                    }else{
                        game_map[i-1][j]=ENEMY_PLANE1;
                        DrawScreen(i-1,j,ENEMY_PLANE1);
                    }
                }
                if(game_map[i][j]==ENEMY_PLANE2){
                    game_map[i][j]=NOTHING;
                    if(i!=16) DrawScreen(i,j,NOTHING);
                    if(game_map[i-1][j]==BULLET){
                        game_map[i-1][j]=ENEMY_PLANE1;
                        DrawScreen(i-1,j,ENEMY_PLANE1);
                    }else if(game_map[i-1][j]==MY_PLANE){
                        GameOver();
                    }else{
                        game_map[i-1][j]=ENEMY_PLANE2;
                        DrawScreen(i-1,j,ENEMY_PLANE2);
                    }
                }
                if(game_map[i][j]==ENEMY_PLANE3){
                    game_map[i][j]=NOTHING;
                    if(i!=16) DrawScreen(i,j,NOTHING);
                    if(game_map[i-1][j]==BULLET){
                        game_map[i-1][j]=ENEMY_PLANE2;
                        DrawScreen(i-1,j,ENEMY_PLANE2);
                    }else if(game_map[i-1][j]==MY_PLANE){
                        GameOver();
                    }else{
                        game_map[i-1][j]=ENEMY_PLANE3;
                        DrawScreen(i-1,j,ENEMY_PLANE3);
                    }
                }
                if(game_map[i][j]==ENEMY_PLANE4){
                    game_map[i][j]=NOTHING;
                    if(i!=16) DrawScreen(i,j,NOTHING);
                    if(game_map[i-1][j]==BULLET){
                        game_map[i-1][j]=ENEMY_PLANE3;
                        DrawScreen(i-1,j,ENEMY_PLANE3);
                    }else if(game_map[i-1][j]==MY_PLANE){
                        GameOver();
                    }else{
                        game_map[i-1][j]=ENEMY_PLANE4;
                        DrawScreen(i-1,j,ENEMY_PLANE4);
                    }
                }
                if(game_map[i][j]==OBSTACLE){
                    game_map[i][j]=NOTHING;
                    if(i!=16) DrawScreen(i,j,NOTHING);
                    if(game_map[i-1][j]==MY_PLANE){
                        GameOver();
                    }else{
                        game_map[i-1][j]=OBSTACLE;
                        DrawScreen(i-1,j,OBSTACLE);
                    }
                }
            }
        }
    }
    //���ֵл�
    if(game_progress<MAP_LENGTH){//���û�����ͼ
        for(j=0;j<=7;j++){
            game_map[16][j]=game_map_level[0][game_progress][j];
        }
    }
    //����BOSS
    if(game_progress==MAP_LENGTH+2){//�����ͼ��һ���
        game_map[15][game_bullet_speed_counter%7]=ENEMY_BOSS;
    }
    if(game_progress<MAP_LENGTH+9) game_progress++;
    //�ƶ�BOSS
    for(j=0;j<=7;j++){
        if(game_map[15][j]==ENEMY_BOSS){
            if((game_bullet_speed_counter+game_boss_hp)%5==0){//����
                if(j==0){
                    if(game_map[15][1]!=BULLET){
                        game_map[15][j]=NOTHING;
                        DrawScreen(15,j,NOTHING);
                        game_map[15][1]=ENEMY_BOSS;
                        DrawScreen(15,1,ENEMY_BOSS);
                    }
                }else if(game_map[15][j-1]!=BULLET){
                    game_map[15][j]=NOTHING;
                    DrawScreen(15,j,NOTHING);
                    game_map[15][j-1]=ENEMY_BOSS;
                    DrawScreen(15,j-1,ENEMY_BOSS);
                }
            }else if((game_bullet_speed_counter+game_boss_hp)%5==1){//����
                if(j==7){
                    if(game_map[15][6]!=BULLET){
                        game_map[15][j]=NOTHING;
                        DrawScreen(15,j,NOTHING);
                        game_map[15][6]=ENEMY_BOSS;
                        DrawScreen(15,6,ENEMY_BOSS);
                    }
                }else if(game_map[15][j+1]!=BULLET){
                    game_map[15][j]=NOTHING;
                    DrawScreen(15,j,NOTHING);
                    game_map[15][j+1]=ENEMY_BOSS;
                    DrawScreen(15,j+1,ENEMY_BOSS);
                }
            }else{//����
                
            }
            break;
        }
        if(game_map[15][j]==ENEMY_BOSS_DILAPIDATED){//��ѪBOSS���ӻ���
            if((game_bullet_speed_counter+game_boss_hp)%3==0){//����
                if(j==0){
                    if(game_map[15][1]!=BULLET){
                        game_map[15][j]=NOTHING;
                        DrawScreen(15,j,NOTHING);
                        game_map[15][1]=ENEMY_BOSS_DILAPIDATED;
                        DrawScreen(15,1,ENEMY_BOSS_DILAPIDATED);
                    }
                }else if(game_map[15][j-1]!=BULLET){
                    game_map[15][j]=NOTHING;
                    DrawScreen(15,j,NOTHING);
                    game_map[15][j-1]=ENEMY_BOSS_DILAPIDATED;
                    DrawScreen(15,j-1,ENEMY_BOSS_DILAPIDATED);
                }
            }else if((game_bullet_speed_counter+game_boss_hp)%3==1){//����
                if(j==7){
                    if(game_map[15][6]!=BULLET){
                        game_map[15][j]=NOTHING;
                        DrawScreen(15,j,NOTHING);
                        game_map[15][6]=ENEMY_BOSS_DILAPIDATED;
                        DrawScreen(15,6,ENEMY_BOSS_DILAPIDATED);
                    }
                }else if(game_map[15][j+1]!=BULLET){
                    game_map[15][j]=NOTHING;
                    DrawScreen(15,j,NOTHING);
                    game_map[15][j+1]=ENEMY_BOSS_DILAPIDATED;
                    DrawScreen(15,j+1,ENEMY_BOSS_DILAPIDATED);
                }
            }else{//����
                
            }
            break;
        }
    }
    //BOSS����
    if(game_progress==MAP_LENGTH+9){
        if(game_boss_attack_speed_counter++==7-game_level_selected/4){
            for(j=0;j<=7;j++){
                if(game_map[15][j]==ENEMY_BOSS||game_map[15][j]==ENEMY_BOSS_DILAPIDATED){
                    if(game_map[14][j]==MY_PLANE){
                        GameOver();
                    }else{
                        game_map[14][j]=AEROLITE;
                        DrawScreen(14,j,AEROLITE);
                    }
                    game_boss_attack_speed_counter=0;
                    break;
                }
            }
        }
    }
}

void GameMoveAerolite(){
    //�ƶ���ʯ
    char i,j;
    for(i=0;i<=14;i++){
        for(j=0;j<=7;j++){
            if(i==0 && game_map[i][j]==AEROLITE){
                game_map[i][j]=NOTHING;
                DrawScreen(i,j,NOTHING);
            }else{
                if(game_map[i][j]==AEROLITE){
                    game_map[i][j]=NOTHING;
                    DrawScreen(i,j,NOTHING);
                    if(game_map[i-1][j]==MY_PLANE){
                        GameOver();
                    }else{
                        game_map[i-1][j]=AEROLITE;
                        DrawScreen(i-1,j,AEROLITE);
                    }
                }
            }
        }
    }
}

void GameOver(){
    scene=SCENE_GAME_OVER;
    clearAll();
	drawString(32,32,"Game over!");
	refreshScreen();
}

void GameWin(){
    scene=SCENE_GAME_WIN;
    clearAll();
	drawString(32,32,"You win!");
	refreshScreen();
}

void DrawScreen(unsigned char x,unsigned char y,unsigned char drawing_model){
    //��Ϸ��ʾ
    unsigned char i,j;//forѭ����ʱ����
    //������δ��뿪ʼ��ͼ(���л�ͼ)
	for(i=0;i<=7;i++){
    	if(drawing_model==NOTHING){
        	g_ScreenMatrix[y][x*8+i]=0x00;
    	}else{
    	    //��תģ�ͣ���ΪLCD������ʽ��ͬ����Ҫ˳ʱ����ת90�ȡ��������������»���Ͽ���
    	    //��ģ�͵�һ��Ϊ��ģ�����һ�У���i��Ϊ��ģ�͵�7-i�У�
    	    unsigned char g_ScreenMatrix_tmp=0x00;
    	    for(j=0;j<=7;j++){//��
    	        //ȡģ��ÿһ�е�7-iλ����ֵ��tmp�ĵ�jλ��
    	        if(model[drawing_model][j]&(1<<(7-i))){
    	            g_ScreenMatrix_tmp|=1<<j;
    	        }
    	    }
        	g_ScreenMatrix[y][x*8+i]=g_ScreenMatrix_tmp;
        	//g_ScreenMatrix[y][x*8+i]=model[drawing_model][i];
    	}
	}
}

