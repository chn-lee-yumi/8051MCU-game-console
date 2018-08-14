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
         佛祖保佑       永无BUG 
*/
/*打飞机游戏机*/
#include "Virtual51API.h"
//下面定义场景
#define SCENE_MENU_MAIN 1
#define SCENE_PLAYING 5
#define SCENE_GAME_OVER 6
#define SCENE_GAME_WIN 7
unsigned char scene = SCENE_MENU_MAIN;//场景，按键执行功能时用
//下面定义游戏菜单
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
#define MENU_ITEM_START 1 //开始游戏
//下面声明各种函数
//游戏相关
void StartGame();//开始游戏
void ButtonManage();//按键处理
void GameShoot();//发射子弹
void GameMoveBullet();//移动子弹
void GameMoveEnemy();//移动敌机
void GameMoveAerolite();//移动陨石
void GameOver();//游戏结束
void GameWin();//游戏胜利
//LCD相关
void DrawScreen(unsigned char x,unsigned char y,unsigned char drawing_model);//游戏显示
//下面定义各种变量
unsigned char display_mem_buff[2]={0x00,0x00};//显示缓存
unsigned char my_plane_x,my_plane_y;//我的飞机坐标。
unsigned int game_speed=5;//游戏速度，控制定时器1初值，范围1~10，定时器1初值为65535-2500+200*game_speed
unsigned int game_shoot_speed_counter=0;//发射子弹的延时计数器，Timer1每中断一次这个值加1
unsigned int game_shoot_speed;//game_shoot_speed_counter到达该值就发射子弹
unsigned int game_bullet_speed_counter=0;//子弹移动的延时计数器，Timer1每中断一次这个值加1
unsigned int game_bullet_speed;
unsigned int game_enemy_speed_counter=0;//敌人移动的延时计数器，Timer1每中断一次这个值加1
unsigned int game_enemy_speed;
unsigned char game_map[17][8];//8行16列的地图，第17列用来放准备出来的敌机
unsigned char game_progress;//游戏进度，表示飞机走到地图哪里了
unsigned char game_level_selected=4;//当前选中的关卡，在模拟器里只有1关，这个用来控制BOSS强度，范围1~13。
unsigned char game_boss_hp;//BOSS血量
unsigned char game_boss_attack_speed_counter=0;//BOSS攻击速度
unsigned char game_aerolite_speed;//陨石速度（BOSS召唤陨石攻击）
game_aerolite_speed_counter=0;
//下面定义游戏模型
const unsigned char code model[11][8]={
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},{0x18,0x18,0x19,0xFF,0xFF,0x19,0x18,0x18},
    {0x18,0x38,0x19,0xFF,0xFF,0x19,0x38,0x18},{0x38,0x38,0x19,0xFF,0xFF,0x19,0x38,0x38},
    {0x38,0x78,0x19,0xFF,0xFF,0x19,0x78,0x38},{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
    {0x18,0x7E,0x7E,0xFF,0xFF,0x7E,0x7E,0x18},{0x3C,0x3D,0x19,0xFF,0xFF,0x19,0x3D,0x3C},
    {0x3C,0x1D,0x19,0xFF,0xFF,0x19,0x1D,0x3C},{0x00,0x00,0x00,0x7E,0x7E,0x00,0x00,0x00},
    {0x18,0x18,0x9C,0xFF,0xFF,0x9C,0x18,0x18}
};//8x8的模型，我的飞机，敌人飞机1，敌人飞机2，敌人飞机3，敌人飞机4，障碍物，陨石，BOSS，残血BOSS，子弹
#define MY_PLANE 10 //我的飞机
#define ENEMY_PLANE1 1 //敌人飞机1
#define ENEMY_PLANE2 2 //敌人飞机2
#define ENEMY_PLANE3 3 //敌人飞机3
#define ENEMY_PLANE4 4 //敌人飞机4
#define OBSTACLE 5 //障碍物
#define AEROLITE 6 //陨石
#define ENEMY_BOSS 7 //BOSS
#define ENEMY_BOSS_DILAPIDATED 8 //残血BOSS
#define BULLET 9 //子弹
#define NOTHING 0 //空
//下面定义地图
#define MAP_LENGTH 30 //因为一个扇区保存一个地图，所以地图长度最大可以为512/8=64，不过暂时不写这么大，编地图好累的
const unsigned char code game_map_level[1][MAP_LENGTH][8]= //关卡数，长度，地图宽度。
{
    {//模拟器只有1关
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
};//地图到此结束

char virtualkey_delay=0;//用于模拟器的虚拟键盘延时，为双数时把virtualkey置1
char virtualkey=0;
/************************************************************************************************
*************************************************************************************************
***************************************下面开始主程序********************************************
*************************************************************************************************
************************************************************************************************/

void main(){
	//初始化LCD
	sendLCDCmd(LCD_on);
	clearAll();
	drawLargeImage(20,0,menu_main,WIDTH,20);
	refreshScreen();
    //初始化定时器
    TMOD=0x11;//定时器1为16位定时计数器，用于游戏速度
    EA=1;
    ET0=1;
    ET1=1;
    TH0=(65535-10000)/255;//50ms
    TL0=(65535-10000)%255;
    TR0=1;
    //循环检测按键
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
    //控制游戏速度的定时器
    game_shoot_speed_counter++;
    game_bullet_speed_counter++;
    game_enemy_speed_counter++;
    game_aerolite_speed_counter++;
    TH1=(65535-2500+200*game_speed)/255;
    TL1=(65535-2500+200*game_speed)%255;
}

void Timer0() interrupt 1{
    //控制虚拟键盘延时的定时器。需要100ms。
    virtualkey_delay++;
    if(virtualkey_delay==2){
        virtualkey_delay=0;
        virtualkey=1;
    }
    TH0=(65535-10000)/255;//50ms
    TL0=(65535-10000)%255;
}

void StartGame(){
    //开始游戏
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
            GameMoveBullet();//移动子弹
            game_bullet_speed_counter=0;
        }
        if(game_enemy_speed_counter>=game_enemy_speed){
            GameMoveEnemy();//移动敌人
            game_enemy_speed_counter=0;
        }
        if(game_shoot_speed_counter>=game_shoot_speed){
            GameShoot();//发射子弹
            game_shoot_speed_counter=0;
        }
        if(game_aerolite_speed_counter>=game_aerolite_speed){
            GameMoveAerolite();//移动陨石
            game_aerolite_speed_counter=0;
        }
	    refreshScreen();
    }
}

void ButtonManage(){
    //按键处理。思路：判断按钮是否按下，如果按下了，判断当前的场景（scene），然后进行相应操作
    //主菜单按任意键开始游戏。
    switch(KEYPAD){//读取虚拟键盘的按键
        case 0x5e://上
            //drawString(32,32,"up");refreshScreen();
            if(scene==SCENE_MENU_MAIN){//主菜单
                scene=SCENE_PLAYING;
            }else if(scene==SCENE_PLAYING){//正在游戏
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
        case 0x56://下
            //drawString(32,32,"down");refreshScreen();
            if(scene==SCENE_MENU_MAIN){//主菜单
                scene=SCENE_PLAYING;
            }else if(scene==SCENE_PLAYING){//正在游戏
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
        case 0x3c://左
            //drawString(32,32,"left");refreshScreen();
            if(scene==SCENE_MENU_MAIN){//主菜单
                scene=SCENE_PLAYING;
            }else if(scene==SCENE_PLAYING){//正在游戏
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
        case 0x3e://右
            //drawString(32,32,"right");refreshScreen();
            if(scene==SCENE_MENU_MAIN){//主菜单
                scene=SCENE_PLAYING;
            }else if(scene==SCENE_PLAYING){//正在游戏
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
    //发射子弹
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
        //不产生子弹
    }else{
        game_map[my_plane_x+1][my_plane_y]=BULLET;
        DrawScreen(my_plane_x+1,my_plane_y,BULLET);
    }
}

void GameMoveBullet(){
    //移动子弹
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
                            DrawScreen(i+1,j,ENEMY_BOSS_DILAPIDATED);//残血BOSS的模型
                        }else{
                            DrawScreen(i+1,j,ENEMY_BOSS);
                        }
                    }else if(game_map[i+1][j]==OBSTACLE||game_map[i+1][j]==AEROLITE){
                        //子弹撞没了
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
    //移动敌机
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
    //出现敌机
    if(game_progress<MAP_LENGTH){//如果没走完地图
        for(j=0;j<=7;j++){
            game_map[16][j]=game_map_level[0][game_progress][j];
        }
    }
    //出现BOSS
    if(game_progress==MAP_LENGTH+2){//走完地图过一会儿
        game_map[15][game_bullet_speed_counter%7]=ENEMY_BOSS;
    }
    if(game_progress<MAP_LENGTH+9) game_progress++;
    //移动BOSS
    for(j=0;j<=7;j++){
        if(game_map[15][j]==ENEMY_BOSS){
            if((game_bullet_speed_counter+game_boss_hp)%5==0){//上移
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
            }else if((game_bullet_speed_counter+game_boss_hp)%5==1){//下移
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
            }else{//不动
                
            }
            break;
        }
        if(game_map[15][j]==ENEMY_BOSS_DILAPIDATED){//残血BOSS更加机智
            if((game_bullet_speed_counter+game_boss_hp)%3==0){//上移
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
            }else if((game_bullet_speed_counter+game_boss_hp)%3==1){//下移
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
            }else{//不动
                
            }
            break;
        }
    }
    //BOSS攻击
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
    //移动陨石
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
    //游戏显示
    unsigned char i,j;//for循环临时变量
    //下面这段代码开始绘图(按列绘图)
	for(i=0;i<=7;i++){
    	if(drawing_model==NOTHING){
        	g_ScreenMatrix[y][x*8+i]=0x00;
    	}else{
    	    //旋转模型，因为LCD驱动方式不同，需要顺时针旋转90度。不过这样做导致画面较卡。
    	    //新模型第一列为旧模型最后一行（第i列为旧模型第7-i行）
    	    unsigned char g_ScreenMatrix_tmp=0x00;
    	    for(j=0;j<=7;j++){//行
    	        //取模型每一行的7-i位，赋值到tmp的第j位。
    	        if(model[drawing_model][j]&(1<<(7-i))){
    	            g_ScreenMatrix_tmp|=1<<j;
    	        }
    	    }
        	g_ScreenMatrix[y][x*8+i]=g_ScreenMatrix_tmp;
        	//g_ScreenMatrix[y][x*8+i]=model[drawing_model][i];
    	}
	}
}

