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
//TODO:蜂鸣器音乐（定时器2），飞机性能升级功能，LVDF判断低电压
/*打飞机游戏机*/
#include <reg51.h>
#include <intrins.h>
//sbit wela=P2^6;sbit dula=P2^7;//开发板才需要这个，实际使用要注释掉
//下面定义LCD
#define LCD_PORT P1//LCD数据口
sbit lcd_rs  =  P3^5;//指令/数据选择信号
sbit lcd_rw  =  P3^6;//读写选择信号
sbit lcd_en  =  P3^4;//使能信号
//sbit lcd_psb =  P3^7;//串并行接口选择：1为并行，0为串行
#define READ 'r'
#define WRITE 'w'
#define EXEC_CMD 'c'
sbit LCD_IS_BUSY = LCD_PORT^7;//数据口最高位，判断LCD是否繁忙时从该为读取数据
//下面定义按钮
sbit button_up = P2^0;
sbit button_down = P2^1;
sbit button_left = P2^2;
sbit button_right = P2^3;
sbit button_a = P2^4;
sbit button_b = P2^5;
#define BUTTON_NONE 0
#define BUTTON_UP 1
#define BUTTON_DOWN 2
#define BUTTON_LEFT 3
#define BUTTON_RIGHT 4
#define BUTTON_A 5
#define BUTTON_B 6
unsigned char button_last_pressed = BUTTON_NONE;//上一次按下的按钮，用于按键去抖
unsigned char button_last_last_pressed = BUTTON_NONE;//上上一次按下的按钮，用于检测按键是否一直按着
//下面定义场景
#define SCENE_MENU_MAIN 1
#define SCENE_SELECT_LEVEL 2
#define SCENE_SETTINGS 3
#define SCENE_EDITING_MAP 4
#define SCENE_CHOOSE_MAP 8
#define SCENE_PLAYING 5
#define SCENE_GAME_OVER 6
#define SCENE_GAME_WIN 7
unsigned char scene = SCENE_MENU_MAIN;//场景，按键执行功能时用
//下面定义游戏菜单
const unsigned char code menu_main[64]={"      菜单          地图编辑器    > 开始游戏        游戏设置    "};
const unsigned char code menu_select_level[64]={"    选择关卡        第 01 关                                    "};
const unsigned char code menu_game_over[64]={"                                   GAME OVER!                   "};
const unsigned char code menu_game_win[64]={"                                    YOU WIN!                    "};
const unsigned char code menu_settings[64]={"    游戏速度        <  05  >                                    "};
const unsigned char code menu_choose_map[64]={"    地图选择        <  01  >                                    "};
#define MENU_ITEM_START 1 //开始游戏
#define MENU_ITEM_EDITOR 2 //地图编辑器
#define MENU_ITEM_SETTINGS 3 //游戏设置
unsigned char menu_item_selected = MENU_ITEM_START;//当前选中的选项
//下面定义游戏模型
const unsigned char code model[10][8]={
    {0x18,0x18,0x9C,0xFF,0xFF,0x9C,0x18,0x18},{0x18,0x18,0x19,0xFF,0xFF,0x19,0x18,0x18},
    {0x18,0x38,0x19,0xFF,0xFF,0x19,0x38,0x18},{0x38,0x38,0x19,0xFF,0xFF,0x19,0x38,0x38},
    {0x38,0x78,0x19,0xFF,0xFF,0x19,0x78,0x38},{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
    {0x18,0x7E,0x7E,0xFF,0xFF,0x7E,0x7E,0x18},{0x3C,0x3D,0x19,0xFF,0xFF,0x19,0x3D,0x3C},
    {0x3C,0x1D,0x19,0xFF,0xFF,0x19,0x1D,0x3C},{0x00,0x00,0x00,0x7E,0x7E,0x00,0x00,0x00}
};//8x8的模型，我的飞机，敌人飞机1，敌人飞机2，敌人飞机3，敌人飞机4，BOSS，残血BOSS，陨石，障碍物，子弹
#define MY_PLANE 0
#define ENEMY_PLANE1 1
#define ENEMY_PLANE2 2
#define ENEMY_PLANE3 3
#define ENEMY_PLANE4 4
#define OBSTACLE 5
#define AEROLITE 6
#define ENEMY_BOSS 7
#define ENEMY_BOSS_DILAPIDATED 8
#define BULLET 9
#define NOTHING 255
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
void LoadSettings();//加载游戏设置
void SaveSettings();//保存游戏设置
//LCD相关
unsigned char OperateLCD(unsigned char method,unsigned char parm);//LCD操作函数
void DisplayMenu();//显示游戏菜单，采用文字方式
void ModifyMenu(unsigned char address,unsigned char dat[]);//修改文字菜单内容
void ClearMenu();//清空菜单内容
void DrawScreen(unsigned char x,unsigned char y,unsigned char drawing_model);//游戏显示
void DrawScreenReverse(unsigned char x,unsigned char y,unsigned char drawing_model);//游戏反色显示，用于地图编辑器的光标，其实可以和上面合并，不过要改的代码太多，干脆复制一个改一下好了
void ReadScreen(unsigned char x,unsigned char y);//读取显示内容
void WriteScreen(unsigned char x,unsigned char y);//写入显示内容
void ClearScreen();//清空显示内容
//地图编辑器相关
void EditMap();//编辑地图
void DrawMap();//绘制自定义地图
void LoadCustomMap();//加载自定义地图
void SaveCustonMap();//保存自定义地图
//下面定义各种变量
unsigned char display_mem_buff[2]={0x00,0x00};//显示缓存
unsigned char my_plane_x,my_plane_y;//我的飞机坐标。
unsigned int game_speed;//游戏速度，控制定时器1初值，范围1~10，定时器1初值为65535-2500+200*game_speed
unsigned int game_shoot_speed_counter=0;//发射子弹的延时计数器，Timer1每中断一次这个值加1
unsigned int game_shoot_speed;//game_shoot_speed_counter到达该值就发射子弹
unsigned int game_bullet_speed_counter=0;//子弹移动的延时计数器，Timer1每中断一次这个值加1
unsigned int game_bullet_speed;
unsigned int game_enemy_speed_counter=0;//敌人移动的延时计数器，Timer1每中断一次这个值加1
unsigned int game_enemy_speed;
//unsigned int game_my_plane_speed_counter=0;//自己移动的延时计数器，Timer1每中断一次这个值加1
//unsigned int game_my_plane_speed;
unsigned char game_map[17][8];//8行16列的地图，第17列用来放准备出来的敌机
unsigned char game_progress;//游戏进度，表示飞机走到地图哪里了
unsigned char game_level_selected;//当前选中的关卡
unsigned char game_boss_hp;//BOSS血量
unsigned char map_editing=11;//当前正在编辑的地图
unsigned char cursor_x,cursor_y;//地图编辑器光标
unsigned char game_boss_attack_speed_counter=0;//BOSS攻击速度
unsigned char game_aerolite_speed;//陨石速度（BOSS召唤陨石攻击）
game_aerolite_speed_counter=0;
//下面定义地图
#define MAP_LENGTH 30 //因为一个扇区保存一个地图，所以地图长度最大可以为512/8=64，不过暂时不写这么大，编地图好累的
unsigned char game_custom_map[MAP_LENGTH][8];//玩家自定义的地图
const unsigned char code game_map_level[10][MAP_LENGTH][8]= //关卡数，长度，地图宽度。下面的十个地图随便写的……我为什么一开始要设计10关这么多QwQ
{
    {//第1关
        {255,255,1,255,255,1,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,1,1,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,1,255,255,1,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,1,255,255,1,255,255},
        {255,255,255,255,255,255,255,255},
        {255,1,255,1,1,255,1,255},
        {255,255,255,255,255,255,255,255},
        {1,255,1,255,255,1,255,1},
        {255,255,255,255,255,255,255,255},
        {255,255,1,1,1,1,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,1,255,255,1,255,255},
        {255,255,255,1,1,255,255,255},
        {255,255,255,255,255,255,255,255},
        {1,1,1,1,1,1,1,1},
        {255,255,255,255,255,255,255,255},
        {255,255,1,255,255,1,255,255},
        {1,255,1,255,1,255,1,255},
        {255,1,255,1,255,1,255,1},
        {1,255,1,255,1,255,1,255},
        {255,1,255,1,255,1,255,1},
        {255,255,1,255,255,1,255,255},
        {255,1,255,1,1,255,1,255},
        {255,255,255,255,255,255,255,255},
        {2,255,2,255,255,2,255,2},
        {255,255,255,255,255,255,255,255},
        {255,2,255,255,255,255,2,255}
    },
    {//第2关
        {5,255,1,255,255,1,255,5},
        {255,5,255,255,255,255,5,255},
        {255,255,255,1,1,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,1,255,255,1,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,1,255,255,1,255,255},
        {255,255,255,255,255,255,255,255},
        {255,1,255,1,1,255,1,255},
        {255,255,255,255,255,255,255,255},
        {1,255,1,255,255,1,255,1},
        {255,255,255,255,255,255,255,255},
        {255,255,1,1,1,1,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,1,255,255,1,255,255},
        {255,255,255,1,1,255,255,255},
        {255,255,255,255,255,255,255,255},
        {1,1,1,1,1,1,1,1},
        {255,255,255,255,255,255,255,255},
        {255,255,1,255,255,1,255,255},
        {1,255,1,255,1,255,1,255},
        {255,1,255,1,255,1,255,1},
        {1,255,1,255,1,255,1,255},
        {255,1,255,1,255,1,255,1},
        {255,255,1,255,255,1,255,255},
        {255,1,255,1,1,255,1,255},
        {255,255,255,255,255,255,255,255},
        {2,255,2,255,255,2,255,2},
        {255,255,255,255,255,255,255,255},
        {255,2,255,255,255,255,2,255}
    },
    {//第3关
        {255,255,2,255,255,2,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,2,2,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,2,255,255,2,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,2,255,255,2,255,255},
        {255,255,255,255,255,255,255,255},
        {255,2,255,2,2,255,2,255},
        {255,255,255,255,255,255,255,255},
        {2,255,2,255,255,2,255,2},
        {255,255,255,255,255,255,255,255},
        {255,255,2,2,2,2,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,2,255,255,2,255,255},
        {255,255,255,2,2,255,255,255},
        {255,255,255,255,255,255,255,255},
        {2,2,2,2,2,2,2,2},
        {255,255,255,255,255,255,255,255},
        {255,255,2,255,255,2,255,255},
        {2,255,2,255,2,255,2,255},
        {255,2,255,2,255,2,255,2},
        {2,255,2,255,2,255,2,255},
        {255,2,255,2,255,2,255,2},
        {255,255,2,255,255,2,255,255},
        {255,2,255,2,2,255,2,255},
        {255,255,255,255,255,255,255,255},
        {3,255,3,255,255,3,255,3},
        {255,255,255,255,255,255,255,255},
        {255,3,255,255,255,255,3,255}
    },
    {//第4关
        {255,255,2,255,255,2,255,255},
        {255,5,255,255,255,255,255,255},
        {255,5,5,2,2,5,5,5},
        {255,255,255,255,255,255,255,255},
        {255,255,2,255,255,2,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,2,255,255,2,255,255},
        {255,255,255,255,255,255,255,255},
        {255,2,255,2,2,255,2,255},
        {255,255,255,255,255,255,255,255},
        {2,255,2,255,255,2,255,2},
        {255,255,255,255,255,255,255,255},
        {255,255,2,2,2,2,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,2,255,255,2,255,255},
        {255,255,255,2,2,255,255,255},
        {255,255,255,255,255,255,255,255},
        {2,2,2,2,2,2,2,2},
        {255,255,255,255,255,255,255,255},
        {255,255,2,255,255,2,255,255},
        {2,255,2,255,2,255,2,255},
        {255,2,255,2,255,2,255,2},
        {2,255,2,255,2,255,2,255},
        {255,2,255,2,255,2,255,2},
        {255,255,2,255,255,2,255,255},
        {255,2,255,2,2,255,2,255},
        {255,255,255,255,255,255,255,255},
        {3,255,3,255,255,3,255,3},
        {255,255,255,255,255,255,255,255},
        {255,3,255,255,255,255,3,255}
    },
    {//第5关
        {255,255,3,255,255,3,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,3,3,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,3,255,255,3,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,3,255,255,3,255,255},
        {255,255,255,255,255,255,255,255},
        {255,3,255,3,3,255,3,255},
        {255,255,255,255,255,255,255,255},
        {3,255,3,255,255,3,255,3},
        {255,255,255,255,255,255,255,255},
        {255,255,3,3,3,3,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,3,255,255,3,255,255},
        {255,255,255,3,3,255,255,255},
        {255,255,255,255,255,255,255,255},
        {3,3,3,3,3,3,3,3},
        {255,255,255,255,255,255,255,255},
        {255,255,3,255,255,3,255,255},
        {3,255,3,255,3,255,3,255},
        {255,3,255,3,255,3,255,3},
        {3,255,3,255,3,255,3,255},
        {255,3,255,3,255,3,255,3},
        {255,255,3,255,255,3,255,255},
        {255,3,255,3,3,255,3,255},
        {255,255,255,255,255,255,255,255},
        {4,255,4,255,255,4,255,4},
        {255,255,255,255,255,255,255,255},
        {255,4,255,255,255,255,4,255}
    },
    {//第6关
        {255,255,3,255,255,3,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,3,3,255,255,255},
        {5,5,255,5,5,255,255,255},
        {255,255,3,255,255,3,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,3,255,255,3,255,255},
        {255,255,255,255,255,255,255,255},
        {255,3,255,3,3,255,3,255},
        {255,255,255,255,255,255,255,255},
        {3,255,3,255,255,3,255,3},
        {255,255,255,255,255,255,255,255},
        {255,255,3,3,3,3,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,3,255,255,3,255,255},
        {255,255,255,3,3,255,255,255},
        {255,255,255,255,255,255,255,255},
        {3,3,3,3,3,3,3,3},
        {255,255,255,255,255,255,255,255},
        {255,255,3,255,255,3,255,255},
        {3,255,3,255,3,255,3,255},
        {255,3,255,3,255,3,255,3},
        {3,255,3,255,3,255,3,255},
        {255,3,255,3,255,3,255,3},
        {255,255,3,255,255,3,255,255},
        {255,3,255,3,3,255,3,255},
        {255,255,255,255,255,255,255,255},
        {4,255,4,255,255,4,255,4},
        {255,255,255,255,255,255,255,255},
        {255,4,255,255,255,255,4,255}
    },
    {//第7关
        {255,255,4,255,255,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,4,4,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,4,255,255,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,4,255,255,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,4,255,4,4,255,4,255},
        {255,255,255,255,255,255,255,255},
        {4,255,4,255,255,4,255,4},
        {255,255,255,255,255,255,255,255},
        {255,255,4,4,4,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,4,255,255,4,255,255},
        {255,255,255,4,4,255,255,255},
        {255,255,255,255,255,255,255,255},
        {4,4,4,4,4,4,4,4},
        {255,255,255,255,255,255,255,255},
        {255,255,4,255,255,4,255,255},
        {4,255,4,255,4,255,4,255},
        {255,4,255,4,255,4,255,4},
        {4,255,4,255,4,255,4,255},
        {255,4,255,4,255,4,255,4},
        {255,255,4,255,255,4,255,255},
        {255,4,255,4,4,255,4,255},
        {255,255,255,255,255,255,255,255},
        {4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4}
    },
    {//第8关
        {255,255,4,255,255,4,255,255},
        {5,5,5,255,255,5,5,5},
        {5,5,5,4,4,5,5,5},
        {5,5,255,255,255,255,5,5},
        {255,255,4,255,255,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,4,255,255,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,4,255,4,4,255,4,255},
        {255,255,255,255,255,255,255,255},
        {4,255,4,255,255,4,255,4},
        {255,255,255,255,5,255,255,255},
        {5,255,4,4,4,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,4,255,255,4,255,255},
        {255,255,255,4,4,255,255,255},
        {255,255,255,255,255,255,255,255},
        {4,4,4,4,4,4,4,4},
        {255,255,255,255,255,255,255,255},
        {255,255,4,255,255,4,255,255},
        {4,255,4,255,4,255,4,255},
        {255,4,255,4,255,4,255,4},
        {4,255,4,255,4,255,4,255},
        {255,4,255,4,255,4,255,4},
        {255,255,4,255,255,4,255,255},
        {255,4,255,4,4,255,4,255},
        {255,255,255,255,255,255,255,255},
        {4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4}
    },
    {//第9关
        {4,255,4,255,255,4,255,4},
        {255,255,255,255,255,255,255,255},
        {255,4,255,4,4,255,4,255},
        {255,255,255,255,255,255,255,255},
        {255,255,4,5,255,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,5,4,255,255,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,4,255,4,4,5,4,255},
        {255,255,255,255,255,255,255,255},
        {4,255,4,5,255,4,255,4},
        {255,255,255,255,255,255,255,255},
        {255,5,4,4,4,4,255,255},
        {255,5,255,255,255,255,255,255},
        {255,5,4,255,255,4,255,255},
        {255,255,255,4,4,255,255,255},
        {255,255,255,255,255,255,255,255},
        {4,4,4,4,4,4,4,4},
        {255,255,255,255,255,255,255,255},
        {255,255,4,255,255,4,255,255},
        {4,255,4,255,4,255,4,255},
        {255,4,255,4,255,4,255,5},
        {4,255,4,255,4,5,4,255},
        {255,4,255,4,255,4,255,4},
        {255,255,4,255,255,4,255,255},
        {255,4,255,4,4,255,4,255},
        {255,255,255,255,255,255,255,255},
        {4,4,5,5,4,5,5,4},
        {4,4,5,5,4,5,5,4},
        {4,4,5,5,4,5,5,4}
    },
    {//第10关
        {4,255,4,255,255,4,255,4},
        {255,255,255,255,255,255,255,255},
        {255,4,255,4,4,255,4,255},
        {255,255,255,255,255,255,255,255},
        {255,255,4,5,255,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,5,4,255,255,4,255,255},
        {255,255,255,255,255,255,255,255},
        {255,4,255,4,4,5,4,255},
        {255,255,255,255,255,255,255,255},
        {4,255,4,5,255,4,255,4},
        {255,255,255,255,255,255,255,255},
        {255,5,4,4,4,4,255,255},
        {255,5,255,255,255,255,255,255},
        {255,5,4,255,255,4,255,255},
        {255,255,255,4,4,255,255,255},
        {255,255,255,255,255,255,255,255},
        {4,4,4,4,4,4,4,4},
        {255,255,255,255,255,255,255,255},
        {255,255,4,255,255,4,255,255},
        {4,255,4,255,4,255,4,255},
        {255,4,255,4,255,4,255,5},
        {4,255,4,255,4,5,4,255},
        {255,4,255,4,255,4,255,4},
        {255,255,4,255,255,4,255,255},
        {255,4,255,4,4,255,4,255},
        {255,255,255,255,255,255,255,255},
        {4,5,4,5,4,5,5,4},
        {4,5,4,5,4,5,5,4},
        {4,5,5,5,5,5,5,4}
    }
};//地图到此结束

//下面是STC12C5A60S2的EEPROM操作代码，参考官网文档
sfr IAP_DATA = 0xC2;//数据寄存器，读写的时候数据都放在这
sfr IAP_ADDRH = 0xC3;//地址高八位
sfr IAP_ADDRL = 0xC4;//地址低八位    IAP操作完成后，地址寄存器内容不变
sfr IAP_CMD = 0xC5;//命令寄存器，只有最后两位有用。00为无操作，01为读，10位编程，11为擦除
sfr IAP_TRIG= 0xC6;//IAPEN=1时，先对IAP_TRIG写入0x5A,再写入0xA5，IAP命令才生效
sfr IAP_CONTR = 0xC7;//控制寄存器，从高位到低位分别为：IAPEN SWBS SWRST CMD_FAIL - WT2 WT1 WT0
                     //IAPEN为IAP功能允许位，为0时禁止IAP操作，为1时允许。WT210为CPU等待时间，根据系统时钟设置，官网文档建议12MHz设置为011。其它位这里用不到，全给0。
//固件版本在v7.1及以上的芯片的EEPROM : 2048字节(0000H-07FFH)
//固件版本低于v7.1的芯片的EEPROM     : 1024字节(0000H-03FFH)
//这里使用STC12C5A60S2的固件版本号: 7.1.4I，所以有四个扇区可用
unsigned char IAPReadByte(unsigned char addrh,unsigned char addrl){
    //读字节
    unsigned char dat;
    IAP_CMD=0x01;
    IAP_ADDRL=addrl;
    IAP_ADDRH=addrh;
    IAP_TRIG=0x5a;
    IAP_TRIG=0xa5;
    _nop_();
    dat=IAP_DATA;
    return dat;
}
void IAPProgramByte(unsigned char addrh,unsigned char addrl,unsigned char dat){
    //编程字节
    IAP_CMD=0x02;
    IAP_ADDRL=addrl;
    IAP_ADDRH=addrh;
    IAP_DATA=dat;
    IAP_TRIG=0x5a;
    IAP_TRIG=0xa5;
    _nop_();
}
void IAPEraseSector(unsigned char addrh,unsigned char addrl){
    //擦除扇区，擦除地址所在扇区的所有字节
    IAP_CMD=0x03;
    IAP_ADDRL=addrl;
    IAP_ADDRH=addrh;
    IAP_TRIG=0x5a;
    IAP_TRIG=0xa5;
    _nop_();
}
//EEPROM操作代码到此结束
/************EEPROM空间分配***********
第一扇区（000H-1FFH） 游戏设置
000H game_level_selected
001H game_speed
第二扇区（200H-3FFH）
200H-2EFH 玩家自定义地图1（30x8=240字节）
第三扇区（400H-5FFH）
400H-4EFH 玩家自定义地图2
第四扇区（600H-7FFH）
600H-6EFH 玩家自定义地图3
*************************************/


/************************************************************************************************
*************************************************************************************************
***************************************下面开始主程序********************************************
*************************************************************************************************
************************************************************************************************/

void main(){
    //wela=0;dula=0;//开发板才需要这个，实际使用要注释掉
    //初始化IAP，载入游戏设置
    IAP_CONTR=0x83;
    LoadSettings();
	//初始化LCD   
    //lcd_psb = 1;//设置为并口方式
    OperateLCD(EXEC_CMD,0x30);//基本指令
    OperateLCD(EXEC_CMD,0x0C);//显示开，关光标
    DisplayMenu();	
    //初始化定时器
    TMOD=0x11;//定时器0为16位定时计数器，用于按键检测；定时器1为16位定时计数器，用于游戏速度
    EA=1;
    ET0=0;//不开中断，采用查询法
    ET1=1;
    TH0=(65535-10000)/255;//假设按键抖动时间为10ms
    TL0=(65535-10000)%255;
    TR0=1;
    //循环检测按键
    while(1){
        if(TF0==1){
            ButtonManage();
            TL0=(65535-10000)%255;
            TH0=(65535-10000)/255;
            TF0=0;
        }
        if(scene==SCENE_PLAYING){
            StartGame();
        }
        if(scene==SCENE_EDITING_MAP){
            EditMap();
        }
    }
}

void Timer1() interrupt 3{
    //控制游戏速度的定时器
    game_shoot_speed_counter++;
    game_bullet_speed_counter++;
    game_enemy_speed_counter++;
    game_aerolite_speed_counter++;
    //game_my_plane_speed_counter++;
    TH1=(65535-2500+200*game_speed)/255;
    TL1=(65535-2500+200*game_speed)%255;
}

void StartGame(){
    //开始游戏
    unsigned char i,j;
    ClearMenu();
    ClearScreen();
    SaveSettings();//保存当前关卡
    for(i=0;i<=16;i++){
        for(j=0;j<=7;j++){
            game_map[i][j]=NOTHING;
        }
    }
    game_shoot_speed=400;
    game_bullet_speed=100;
    game_enemy_speed=800;
    game_aerolite_speed=200;
    //game_my_plane_speed=200;
    game_boss_hp=10+game_level_selected*3;
    game_progress=0;
    my_plane_x=0;
    my_plane_y=2;
    game_map[my_plane_x][my_plane_y]=MY_PLANE;
    DrawScreen(my_plane_x,my_plane_y,MY_PLANE);
    TH1=(65535-1000)/255;
    TL1=(65535-1000)%255;
    TR1=1;
    if(game_level_selected>10){//11-13关是用户自定义关卡，需要加载用户的地图
        map_editing=game_level_selected;
        LoadCustomMap();
    }
    while(scene==SCENE_PLAYING){
        if(TF0==1){
            ButtonManage();
            TL0=(65535-10000)%255;
            TH0=(65535-10000)/255;
            TF0=0;
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
    }
}

void ButtonManage(){
    //按键处理。思路：判断按钮是否按下，如果按下了，判断当前的场景（scene），然后进行相应操作
    unsigned char dat[2]="  ";//修改文字菜单内容是用到的数组
    if(button_up==0){
        if(button_last_pressed==BUTTON_UP){
            if(scene==SCENE_MENU_MAIN && button_last_last_pressed!=button_last_pressed){//主菜单
                /*一开始的开始菜单显示为：
                    菜单
                  > 开始游戏
                    地图编辑器
                    游戏设置
                箭头所在menu_text的位置从上往下分别为17，9，25
                */
                switch(menu_item_selected){
                    case MENU_ITEM_START:
                        ModifyMenu(17,"  ");
                        ModifyMenu(25,"> ");
                        menu_item_selected=MENU_ITEM_SETTINGS;
                        break;
                    case MENU_ITEM_EDITOR:
                        ModifyMenu(9,"  ");
                        ModifyMenu(17,"> ");
                        menu_item_selected=MENU_ITEM_START;
                        break;
                    case MENU_ITEM_SETTINGS:
                        ModifyMenu(25,"  ");
                        ModifyMenu(9,"> ");
                        menu_item_selected=MENU_ITEM_EDITOR;
                        break;
                }
            }else if(scene==SCENE_PLAYING && button_last_last_pressed!=button_last_pressed){//正在游戏
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
            }else if(scene==SCENE_EDITING_MAP && button_last_last_pressed!=button_last_pressed){//正在编辑地图
                if(cursor_y>0){
                    DrawScreen(cursor_x,cursor_y,game_custom_map[cursor_x+game_progress][cursor_y]);
                    cursor_y--;
                    DrawScreenReverse(cursor_x,cursor_y,game_custom_map[cursor_x+game_progress][cursor_y]);
                }
            }
        }
        button_last_last_pressed=button_last_pressed;
        button_last_pressed=BUTTON_UP;
    }
    if(button_down==0){
        if(button_last_pressed==BUTTON_DOWN){
            if(scene==SCENE_MENU_MAIN && button_last_last_pressed!=button_last_pressed){//主菜单
                switch(menu_item_selected){
                    case MENU_ITEM_START:
                        ModifyMenu(17,"  ");
                        ModifyMenu(9,"> ");
                        menu_item_selected=MENU_ITEM_EDITOR;
                        break;
                    case MENU_ITEM_EDITOR:
                        ModifyMenu(9,"  ");
                        ModifyMenu(25,"> ");
                        menu_item_selected=MENU_ITEM_SETTINGS;
                        break;
                    case MENU_ITEM_SETTINGS:
                        ModifyMenu(25,"  ");
                        ModifyMenu(17,"> ");
                        menu_item_selected=MENU_ITEM_START;
                        break;
                }
            }else if(scene==SCENE_PLAYING && button_last_last_pressed!=button_last_pressed){//正在游戏。
                if(my_plane_y<7){//             ↑↑↑↑↑↑↑↑↑ TODO：删掉button_last_last_pressed!=button_last_pressed的判断条件，按下时飞机可以一直移动，速度通过game_my_plane_speed控制。
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
            }else if(scene==SCENE_EDITING_MAP && button_last_last_pressed!=button_last_pressed){//正在编辑地图
                if(cursor_y<7){
                    DrawScreen(cursor_x,cursor_y,game_custom_map[cursor_x+game_progress][cursor_y]);
                    cursor_y++;
                    DrawScreenReverse(cursor_x,cursor_y,game_custom_map[cursor_x+game_progress][cursor_y]);
                }
            }
        }
        button_last_last_pressed=button_last_pressed;
        button_last_pressed=BUTTON_DOWN;
    }
    if(button_left==0){
        if(button_last_pressed==BUTTON_LEFT){
            if(scene==SCENE_SELECT_LEVEL && button_last_last_pressed!=button_last_pressed){//关卡选择菜单
                if(game_level_selected==1){
                    game_level_selected=13;
                }else{
                    game_level_selected--;
                }
                dat[0]=' ';
                dat[1]=game_level_selected/10+48;
                ModifyMenu(11,dat);
                dat[0]=game_level_selected%10+48;
                dat[1]=' ';
                ModifyMenu(12,dat);
            }else if(scene==SCENE_PLAYING && button_last_last_pressed!=button_last_pressed){//正在游戏
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
            }else if(scene==SCENE_SETTINGS && button_last_last_pressed!=button_last_pressed){//游戏设置菜单
                if(game_speed==1){
                    game_speed=10;
                }else{
                    game_speed--;
                }
                dat[0]=' ';
                dat[1]=game_speed/10+48;
                ModifyMenu(11,dat);
                dat[0]=game_speed%10+48;
                dat[1]=' ';
                ModifyMenu(12,dat);
            }else if(scene==SCENE_CHOOSE_MAP && button_last_last_pressed!=button_last_pressed){//地图选择菜单
                if(map_editing==11){
                    map_editing=13;
                }else{
                    map_editing--;
                }
                dat[0]=' ';
                dat[1]=map_editing/10+48;
                ModifyMenu(11,dat);
                dat[0]=map_editing%10+48;
                dat[1]=' ';
                ModifyMenu(12,dat);
            }else if(scene==SCENE_EDITING_MAP && button_last_last_pressed!=button_last_pressed){//正在编辑地图
                if(cursor_x>0){
                    DrawScreen(cursor_x,cursor_y,game_custom_map[cursor_x+game_progress][cursor_y]);
                    cursor_x--;
                    DrawScreenReverse(cursor_x,cursor_y,game_custom_map[cursor_x+game_progress][cursor_y]);
                }else if(cursor_x==0&&game_progress>0){
                    game_progress--;
                    DrawMap();
                }
            }
        }
        button_last_last_pressed=button_last_pressed;
        button_last_pressed=BUTTON_LEFT;
    }
    if(button_right==0){
        if(button_last_pressed==BUTTON_RIGHT){
            if(scene==SCENE_SELECT_LEVEL && button_last_last_pressed!=button_last_pressed){
                if(game_level_selected==13){
                    game_level_selected=1;
                }else{
                    game_level_selected++;
                }
                dat[0]=' ';
                dat[1]=game_level_selected/10+48;
                ModifyMenu(11,dat);
                dat[0]=game_level_selected%10+48;
                dat[1]=' ';
                ModifyMenu(12,dat);
            }else if(scene==SCENE_PLAYING && button_last_last_pressed!=button_last_pressed){
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
            }else if(scene==SCENE_SETTINGS && button_last_last_pressed!=button_last_pressed){
                if(game_speed==10){
                    game_speed=1;
                }else{
                    game_speed++;
                }
                dat[0]=' ';
                dat[1]=game_speed/10+48;
                ModifyMenu(11,dat);
                dat[0]=game_speed%10+48;
                dat[1]=' ';
                ModifyMenu(12,dat);
            }else if(scene==SCENE_CHOOSE_MAP && button_last_last_pressed!=button_last_pressed){
                if(map_editing==13){
                    map_editing=11;
                }else{
                    map_editing++;
                }
                dat[0]=' ';
                dat[1]=map_editing/10+48;
                ModifyMenu(11,dat);
                dat[0]=map_editing%10+48;
                dat[1]=' ';
                ModifyMenu(12,dat);
            }else if(scene==SCENE_EDITING_MAP && button_last_last_pressed!=button_last_pressed){
                if(cursor_x<15){
                    DrawScreen(cursor_x,cursor_y,game_custom_map[cursor_x+game_progress][cursor_y]);
                    cursor_x++;
                    DrawScreenReverse(cursor_x,cursor_y,game_custom_map[cursor_x+game_progress][cursor_y]);
                }else if(cursor_x==15&&game_progress<MAP_LENGTH-16){
                    game_progress++;
                    DrawMap();
                }
            }
        }
        button_last_last_pressed=button_last_pressed;
        button_last_pressed=BUTTON_RIGHT;
    }
    if(button_a==0){
        if(button_last_pressed==BUTTON_A){
            if(scene==SCENE_MENU_MAIN && button_last_last_pressed!=button_last_pressed){
                switch(menu_item_selected){
                    case MENU_ITEM_START:
                        scene=SCENE_SELECT_LEVEL;
                        DisplayMenu();
                        dat[0]=' ';
                        dat[1]=game_level_selected/10+48;
                        ModifyMenu(11,dat);
                        dat[0]=game_level_selected%10+48;
                        dat[1]=' ';
                        ModifyMenu(12,dat);
                        break;
                    case MENU_ITEM_EDITOR:
                        scene=SCENE_CHOOSE_MAP;
                        DisplayMenu();
                        dat[0]=' ';
                        dat[1]=map_editing/10+48;
                        ModifyMenu(11,dat);
                        dat[0]=map_editing%10+48;
                        dat[1]=' ';
                        ModifyMenu(12,dat);
                        break;
                    case MENU_ITEM_SETTINGS:
                        scene=SCENE_SETTINGS;
                        DisplayMenu();
                        dat[0]=' ';
                        dat[1]=game_speed/10+48;
                        ModifyMenu(11,dat);
                        dat[0]=game_speed%10+48;
                        dat[1]=' ';
                        ModifyMenu(12,dat);
                        break;
                }
            }else if(scene==SCENE_SELECT_LEVEL && button_last_last_pressed!=button_last_pressed){
                scene=SCENE_PLAYING;
            }else if(scene==SCENE_CHOOSE_MAP && button_last_last_pressed!=button_last_pressed){
                scene=SCENE_EDITING_MAP;
            }else if(scene==SCENE_EDITING_MAP && button_last_last_pressed!=button_last_pressed){
                if(game_custom_map[cursor_x+game_progress][cursor_y]==NOTHING){
                    game_custom_map[cursor_x+game_progress][cursor_y]=ENEMY_PLANE1;
                }else if(game_custom_map[cursor_x+game_progress][cursor_y]>=OBSTACLE){
                    game_custom_map[cursor_x+game_progress][cursor_y]=NOTHING;
                }else{
                    game_custom_map[cursor_x+game_progress][cursor_y]++;
                }
                DrawScreenReverse(cursor_x,cursor_y,game_custom_map[cursor_x+game_progress][cursor_y]);
            }
        }
        button_last_last_pressed=button_last_pressed;
        button_last_pressed=BUTTON_A;
    }
    if(button_b==0){
        if(button_last_pressed==BUTTON_B){
            if(scene==SCENE_SELECT_LEVEL && button_last_last_pressed!=button_last_pressed){
                scene=SCENE_MENU_MAIN;
                menu_item_selected=MENU_ITEM_START;
                DisplayMenu();
            }else if(scene==SCENE_SETTINGS && button_last_last_pressed!=button_last_pressed){
                scene=SCENE_MENU_MAIN;
                menu_item_selected=MENU_ITEM_START;
                DisplayMenu();
                SaveSettings();
            }else if(scene==SCENE_EDITING_MAP && button_last_last_pressed!=button_last_pressed){
                ClearScreen();
                scene=SCENE_CHOOSE_MAP;
                DisplayMenu();
                dat[0]=' ';
                dat[1]=map_editing/10+48;
                ModifyMenu(11,dat);
                dat[0]=map_editing%10+48;
                dat[1]=' ';
                ModifyMenu(12,dat);
                SaveCustonMap();
            }else if(scene==SCENE_CHOOSE_MAP && button_last_last_pressed!=button_last_pressed){
                scene=SCENE_MENU_MAIN;
                menu_item_selected=MENU_ITEM_START;
                DisplayMenu();
            }else if(scene==SCENE_PLAYING && button_last_last_pressed!=button_last_pressed){
                GameOver();
            }else if((scene==SCENE_GAME_OVER || scene==SCENE_GAME_WIN) && button_last_last_pressed!=button_last_pressed){//游戏结束
                ClearScreen();
                scene=SCENE_SELECT_LEVEL;
                DisplayMenu();
                dat[0]=' ';
                dat[1]=game_level_selected/10+48;
                ModifyMenu(11,dat);
                dat[0]=game_level_selected%10+48;
                dat[1]=' ';
                ModifyMenu(12,dat);
            }
        }
        button_last_last_pressed=button_last_pressed;
        button_last_pressed=BUTTON_B;
    }
    if(button_up==1&&button_down==1&&button_left==1&&button_right==1&&button_a==1&&button_b==1){
        button_last_last_pressed=button_last_pressed;
        button_last_pressed=BUTTON_NONE;//没有按键按下
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
        if(game_level_selected<=10){//前10关为自带关卡
            for(j=0;j<=7;j++){
                game_map[16][j]=game_map_level[game_level_selected-1][game_progress][j];
            }
        }else{
            for(j=0;j<=7;j++){//后3关为用户自定义关卡
                game_map[16][j]=game_custom_map[game_progress][j];
            }
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
    DisplayMenu();
}

void GameWin(){
    scene=SCENE_GAME_WIN;
    DisplayMenu();
}

void LoadSettings(){
    //加载游戏设置
    game_level_selected=IAPReadByte(0x00,0x00);
    game_speed=IAPReadByte(0x00,0x01);
    //如果没数据或者数据损坏了
    if(game_speed>10 || game_level_selected>10){
        if(game_speed>10) game_speed=5;
        if(game_level_selected>10) game_level_selected=1;
        SaveSettings();
    }
}

void SaveSettings(){
    //保存游戏设置
    IAPEraseSector(0x00,0x00);//先擦再写
    IAPProgramByte(0x00,0x00,game_level_selected);
    IAPProgramByte(0x00,0x01,game_speed);
}

/*LCD使用方法 参考：http://blog.sina.com.cn/s/blog_61b6e08b01016xif.html
****绘图****
先执行两次设置地址的指令，行地址（0-31）和列地址（0-15）
然后写入两次数据，共16位
读取的时候先假读一次，然后读取两次
OperateLCD(EXEC_CMD,0x36);//扩充指令，开绘图显示
OperateLCD(EXEC_CMD,0x34);//扩充指令，关绘图显示
OperateLCD(EXEC_CMD,0x80+行/列地址);
****显示文字****
先执行一次设置地址的指令
然后写入两次数据，共16位
OperateLCD(EXEC_CMD,0x30);//基本指令
OperateLCD(EXEC_CMD,0x0C);//显示开，关光标
OperateLCD(EXEC_CMD,0x80+地址);
*/

void ClearMenu(){
    //清空菜单内容
    OperateLCD(EXEC_CMD,0x30);//基本指令
    OperateLCD(EXEC_CMD,0x01);//清空菜单内容
}

void DisplayMenu(){
    //显示游戏菜单，采用文字方式
    int i;//for循环临时变量
    OperateLCD(EXEC_CMD,0x30);//基本指令
    OperateLCD(EXEC_CMD,0x80);//从第一个地址开始写入文字
    for(i=0;i<=63;i++){
        if(scene==SCENE_MENU_MAIN) OperateLCD(WRITE,menu_main[i]);
        if(scene==SCENE_SELECT_LEVEL) OperateLCD(WRITE,menu_select_level[i]);
        if(scene==SCENE_GAME_OVER) OperateLCD(WRITE,menu_game_over[i]);
        if(scene==SCENE_GAME_WIN) OperateLCD(WRITE,menu_game_win[i]);
        if(scene==SCENE_SETTINGS) OperateLCD(WRITE,menu_settings[i]);
        if(scene==SCENE_CHOOSE_MAP) OperateLCD(WRITE,menu_choose_map[i]);
    }
}

void ModifyMenu(unsigned char address,unsigned char dat[]){
    //修改文字菜单内容
    OperateLCD(EXEC_CMD,0x80+address);
    OperateLCD(WRITE,dat[0]);
    OperateLCD(WRITE,dat[1]);
}

void DrawScreen(unsigned char x,unsigned char y,unsigned char drawing_model){
    //游戏显示
    unsigned char i;//for循环临时变量
    unsigned char x2,y2;
    OperateLCD(EXEC_CMD,0x36);//扩充指令，开绘图显示
    //ClearScreen();
	//坐标转换
	if(y<=3){
        x2=x/2;
        y2=y*8;
    }else{
        x2=x/2+8;
        y2=y*8-32;
    }
    //下面这段代码开始绘图
	for(i=0;i<=7;i++){
	    ReadScreen(x2,y2+i);
    	if(drawing_model==NOTHING){
        	if(x%2==0){
    		    display_mem_buff[0]=0x00;
    		}else{
        		display_mem_buff[1]=0x00;
    		}
    	}else{
        	if(x%2==0){
    		    display_mem_buff[0]=model[drawing_model][i];
    		}else{
        		display_mem_buff[1]=model[drawing_model][i];
    		}
    	}
    	WriteScreen(x2,y2+i);
	}
}

void DrawScreenReverse(unsigned char x,unsigned char y,unsigned char drawing_model){
    //游戏反色显示，用于地图编辑器的光标
    unsigned char i;//for循环临时变量
    unsigned char x2,y2;
    OperateLCD(EXEC_CMD,0x36);//扩充指令，开绘图显示
    //ClearScreen();
	//坐标转换
	if(y<=3){
        x2=x/2;
        y2=y*8;
    }else{
        x2=x/2+8;
        y2=y*8-32;
    }
    //下面这段代码开始绘图
	for(i=0;i<=7;i++){
	    ReadScreen(x2,y2+i);
    	if(drawing_model==NOTHING){
        	if(x%2==0){
    		    display_mem_buff[0]=~0x00;
    		}else{
        		display_mem_buff[1]=~0x00;
    		}
    	}else{
        	if(x%2==0){
    		    display_mem_buff[0]=~model[drawing_model][i];
    		}else{
        		display_mem_buff[1]=~model[drawing_model][i];
    		}
    	}
    	WriteScreen(x2,y2+i);
	}
}

void ReadScreen(unsigned char x,unsigned char y){
    //读取显示内容
    OperateLCD(EXEC_CMD,0x80+y);
    OperateLCD(EXEC_CMD,0x80+x);
    OperateLCD(READ,0);//假读
	display_mem_buff[0]=OperateLCD(READ,0);
	display_mem_buff[1]=OperateLCD(READ,0);
}

void WriteScreen(unsigned char x,unsigned char y){
    //写入显示内容
    OperateLCD(EXEC_CMD,0x80+y);
    OperateLCD(EXEC_CMD,0x80+x);
    OperateLCD(WRITE,display_mem_buff[0]);
    OperateLCD(WRITE,display_mem_buff[1]);
}

void ClearScreen(){
    //清空显示内容
    unsigned char i,j;//for循环临时变量
    OperateLCD(EXEC_CMD,0x36);//扩充指令，开绘图显示
    //下面这段代码是清屏
	for(i=0;i<=31;i++){
		OperateLCD(EXEC_CMD,0x80+i);
		OperateLCD(EXEC_CMD,0x80);
		for (j=0;j<=15;j++){
			OperateLCD(WRITE,0x00);
			OperateLCD(WRITE,0x00);
		}
	}
}

unsigned char OperateLCD(unsigned char method,unsigned char parm){
    //LCD操作函数
    //method：读取：READ，写入：WRITE，执行指令：EXEC_CMD
    //parm为传给P0口的数据，仅对WRITE和EXEC_CMD模式有效，READ模式无效。
    unsigned char lcd_read_data;//从LCD数据口读取的数据
    bit lcd_is_busy;//表示LCD是否繁忙，1为是，0为否
    //下面这段代码判断LCD是否繁忙，直到LCD不繁忙才进行操作
    do{
        lcd_rs = 0;
        lcd_rw = 1;
    	LCD_IS_BUSY=1;//读引脚前先置高电平
        lcd_en = 1;
        _nop_();
        lcd_is_busy = LCD_IS_BUSY;
        lcd_en = 0;
    }while(lcd_is_busy);
    //下面的代码对LCD进行操作
    switch(method){
        case READ:
            lcd_rs = 1;
            lcd_rw = 1;
            break;
        case WRITE:
            lcd_rs = 1;
            lcd_rw = 0;
            break;
        case EXEC_CMD:
            lcd_rs = 0;
            lcd_rw = 0;
            break;
    }
    lcd_en = 1;
    if(method==WRITE||method==EXEC_CMD){
        LCD_PORT = parm;
        _nop_();
        lcd_en = 0;
        return 0;
    }
    if(method==READ){
        LCD_PORT = 0xff;//读引脚前先置高电平
        lcd_read_data = LCD_PORT;
        lcd_en = 0;
        return lcd_read_data;
    }
}

void EditMap(){
    ClearScreen();
    ClearMenu();
    game_progress=0;//地图横向偏移
    cursor_x=0;//光标x
    cursor_y=0;//光标y
    LoadCustomMap();
    DrawMap();
    while(scene==SCENE_EDITING_MAP){
        ButtonManage();
    }
}

void DrawMap(){
    unsigned char i,j;
    for(i=0;i<16;i++){
        for(j=0;j<8;j++){
            if(i==cursor_x&&j==cursor_y){//光标选中的反色绘图
                DrawScreenReverse(i,j,game_custom_map[i+game_progress][j]);
            }else{
                DrawScreen(i,j,game_custom_map[i+game_progress][j]);
            }
        }
    }
}

void LoadCustomMap(){
    //加载自定义地图
    unsigned char i,j;
    unsigned char addrl,addrh;
    if(map_editing==11){
        addrl=0x00;
        addrh=0x02;
    }else if(map_editing==12){
        addrl=0x00;
        addrh=0x04;
    }else if(map_editing==13){
        addrl=0x00;
        addrh=0x06;
    }
    for(i=0;i<MAP_LENGTH;i++){
        for(j=0;j<8;j++){
            game_custom_map[i][j]=IAPReadByte(addrh,addrl);
            if(game_custom_map[i][j]>OBSTACLE&&game_custom_map[i][j]!=NOTHING) game_custom_map[i][j]=NOTHING;
            addrl++;
            if(addrl==0x00) addrh++;
        }
    }
}

void SaveCustonMap(){
    //保存自定义地图
    unsigned char i,j;
    unsigned char addrl,addrh;
    if(map_editing==11){
        addrl=0x00;
        addrh=0x02;
    }else if(map_editing==12){
        addrl=0x00;
        addrh=0x04;
    }else if(map_editing==13){
        addrl=0x00;
        addrh=0x06;
    }
    IAPEraseSector(addrh,addrl);
    for(i=0;i<MAP_LENGTH;i++){
        for(j=0;j<8;j++){
            IAPProgramByte(addrh,addrl,game_custom_map[i][j]);
            addrl++;
            if(addrl==0x00) addrh++;
        }
    }
}
