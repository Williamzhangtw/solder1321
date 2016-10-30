
#ifndef __MENU_H
#define __MENU_H
#include "stm32f0xx_hal.h"
 



typedef struct  //菜单参数结构体声明
{  
   unsigned int param;       //菜单参数项
   unsigned char x1, s1;     //参数, 字符项列距   x1,x2; (进入参数调整时,s1 → 0: 反白显示参数, x: 反白显示字串的个数且屏蔽显示的参数)
   //unsigned char __flash *s;        //参数的字符注释项
   unsigned char s[8]; 
   
}M_ITEM_PARAM;

typedef struct  //菜单结构体声明
{  
   uint8_t id;                //菜单ID号
   const uint8_t  *name;          //菜单项最大为8个汉字长度
   //unsigned char x,y;               //显示屏坐标
  // void (*func)(unsigned char id);  //功能程序
  void (*func)(void);//(unsigned char id); 
} MENU_ITEM;



//全局变量
extern  MENU_ITEM  *PMenu;  

extern U8 quit_is_set_flag  ;

extern U8 menu_in_flag  ;
extern BOOL MenuEnter_flag;
extern MENU_ITEM   MainMenu[];

extern void onMenuKeyDown(uint16_t  key);
extern void MenuEvent(void);
extern void MenuInit(void)  ;
extern void RefreshMenu(U8 col) ;
extern void MoveMenuItem(BOOL dir);
extern void MoveItem(uint16_t *p,uint16_t max,uint16_t min,BOOL dir );
#endif
