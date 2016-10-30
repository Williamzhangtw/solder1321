#include "menu.h"
void Real_Disp(void);
BOOL MenuEnter_flag =0;
 MENU_ITEM  *PMenu; 
 //uint8_t MenuItemIndex =0;
  uint16_t menu_index = 0; 
  uint8_t quit_is_set_flag =0;
 

U8 menu_in_flag = NO ;

  void quit_menu(  void);
  void DevBackLightAjust ( void );
	void TempDispValueMoveAdjust(void);
  void RestrictRangeAjust(void);
	void TempArcCall(void);
	void SettingReset (void);
MENU_ITEM   MainMenu[];  //第一层菜单目录项(结构体数组)        
//={
////  { 0, MENU01, quit_menu }, //GUI_DispDecMinp
////  { 1, MENU02, DevBackLightAjust }, 
////  { 2, MENU03, TempDispValueMoveAdjust }, 
////  { 3, MENU04, RestrictRangeAjust }, //
////  { 4, MENU05, TempArcCall }, 
////  { 5, MENU06, NULL }, //
////  { 6, MENU07, SettingReset  }, 
////  { 7, MENU08, NULL }, //
////  { 8, MENU09, NULL }, 
////  { 9, MENU10, NULL }, 
// };
  

//菜单指针初始化
void MenuInit(void)  
{
  PMenu =MainMenu;

  
//  PMenu =NULL;
//    settion_max =hoter1321temp_max;
//  settion_min =hoter1321temp_min;
//  settion =250 ;
//   PrevMenu =NULL;
//   PMParam =NULL;
}

void MenuDeInit()
{
}



void MoveItem(uint16_t *p,uint16_t max,uint16_t min,BOOL dir )
{   
   //改变选择项
    if(dir)
   {  
      if(++(*p)  >=max) *p =max;  //索引下移
   }     
   else
   {
      if(*p <=min)  *p= min ;   //索引上移
      else --(*p);              
   }
}


//菜单界面参数初始化
void InitMenuItem(void)
{
//   uchar id_end;  //查看密码等级范围菜单最后ID号
//   
//   MenuItemTotal = 0;  
//   MenuItemIndex = 0;  
//   MenuSelectItem = -1;          
//   
//   //if( PMenu == NULL)  //
//   //return;
//                    
//   //计算有几个菜单项(同步查看密码等级), [在password.c中打开, 在soldering.c中关闭, 在menu.c中引用]
//   
//   if(code_level ==0) id_end =   CODE_USER_ID_END;  //普通用户(查看密码等级范围菜单最末ID号)
//   if(code_level ==1) id_end =CODE_FACTORY_ID_END;  //工厂调校
//   if(code_level ==2) id_end =  CODE_SUPER_ID_END;  //超级用户
//   
//   //定总项数
//   while(PMenu[++MenuItemTotal].id !=id_end);    //先++
//   
//   //while(PMenu[MenuItemTotal++].id !=id_end);  //后++
//   //if(MenuItemTotal >1) MenuItemTotal -=1; //去掉最后一行才正确(因MenuItemIndex是从'0'开始的)

//   RefreshMenu(1); //重新加载菜单         
}

void RefreshMenu(uint8_t col)  //col, 0:不可反白显示, 1:可控反
{ 
 // GUI_Clear();
 // if (PMenu ==MainMenu )
//  GUI_Disp_Pic(PMenu[menu_index].name);
}

 

//执行菜单索引载入
void SelectMenuItem(void)
{
//   if( PMenu == NULL)
//   return;
//                    
//   MenuSelectItem =MenuItemIndex;  //执行索引载入当前菜单项的索引号(当前菜单项索引号==当前ID号)
//   //MenuEvent();  //放在main.c, 除按键操作可打开菜单外, 也可通过其他方式启动菜单(屏幕)
}

////键盘处理事件
void onMenuKeyDown(uint16_t key)  //通过函数指针 onKeyUp 调用
{      
//  switch(key)
//  {
//    case HT_UP_KEY:
//    
//     MoveItem(&menu_index,6,0,0);
//      break;
//    case HT_DOWN_KEY:
//     MoveItem(&menu_index,6,0,1); 
//     break;
//    case HT_ENTER_KEY : menu_in_flag = YES ; break ;//HAL_Delay (1000);
//    default :
//      break;
//  
//  }
RefreshMenu(1); //重新加载菜单 
} 

//指针影射设置
void SetMenu(MENU_ITEM  *p)  //菜单设置,  参数项: 菜单结构指针  (struct MENU_ITEM *p)的typedef
{
   //if( p == PMenu)
   //        return;
                    
//   PrevMenu = PMenu;          //保存上一层的菜单  (菜单结构指针)
//   PMenu = p;                 //当前菜单          (菜单结构指针)

//   InitMenuItem();           //菜单界面初始化
//   PTouchKey = onMenuKeyDown;  //建立键盘映射       (函数指针)
//   
   
             //函数的指针载入(程序进入死循环前的第一次推动 (函数指针赋入函数名))  
             //通过按键响应调用void onMenuKeyDown(uchar key)函数
}

//处理菜单命令(下拉菜单及操作)
void MenuEvent(void)
{
  
     if((PMenu !=NULL)&&(PMenu[menu_index ].func != NULL))
     {
       PMenu[menu_index].func()  ;
     }
   
}

void Real_Disp(void)
{
  PMenu = NULL ;
//  
//  settion_max =hoter1321temp_max;
//  settion_min =hoter1321temp_min;
//  settion =250 ;

}

/************************************* Funtion*****************************/
void quit_menu(  void)
{
  quit_is_set_flag = 1;
}

//功能：背光调节函数
//参数：按键键值
void MoveBackLightItem (uint16_t  key)
{
	
    switch(key)
  {
    case HT_UP_KEY:
    
     MoveItem(&data_saved[3],7,0,0);
      break;
    case HT_DOWN_KEY:
     MoveItem(&data_saved[3],7,0,1); 
   
     break;
    case HT_ENTER_KEY :
			menu_in_flag =NO;
			
		APP_DataSaveIsError(FLASH_USER_START_ADDR,data_saved,10);
			HAL_Delay (500);
		break ; 
    default :
      break;
  
  }
  if(hal_100ms_flag )
  {
    hal_100ms_flag =0;
    GUI_DispDecMin(data_saved[3]);
    GUI_BackLight(data_saved[3]);
  }
  
}


void DevBackLightAjust(void)
{	if(PID_PressDelay(&key_10ms_flag,1))
	GUI_PID_Scan(SCAN_SPEED_MIN );
  MoveBackLightItem(0);
}



//功能：数码显示控制函数
//参数：按键键值
void TempDispValueItem (uint16_t  key)
{
	
    switch(key)
  {
    case HT_UP_KEY:
    
     MoveItem(&data_saved[4],1,0,0);//实时
      break;
    case HT_DOWN_KEY:
     MoveItem(&data_saved[4],1,0,1); //限幅
   
     break;
    case HT_ENTER_KEY :
			menu_in_flag =NO;
			
		APP_DataSaveIsError(FLASH_USER_START_ADDR,data_saved,10);
			HAL_Delay (500);
		break ; 
    default :
      break;
  
  }
	if( GUI_DispDelay (&gui_disp_100ms_flag ,2))
  {
		if(data_saved[4]==1)
    GUI_Disp_Pic   (xianfu);
		if(!data_saved[4])
    GUI_Disp_Pic   (_888 );;
  }

}

//数字显示模式 调整函数
void TempDispValueMoveAdjust(void)
{//	if(PID_PressDelay(&key_10ms_flag,1))
//  TempDispValueItem(GUI_PID_Scan(SCAN_SPEED_MIN ));
}


//功能：背光调节函数
//参数：按键键值
void RestrictRangeItem (uint16_t  key)
{
	
    switch(key)
  {
    case HT_UP_KEY:
    
     MoveItem(&data_saved[6],30,2,0);
      break;
    case HT_DOWN_KEY:
     MoveItem(&data_saved[6],30,2,1); 
   
     break;
    case HT_ENTER_KEY :
			menu_in_flag =NO;
			
		APP_DataSaveIsError(FLASH_USER_START_ADDR,data_saved,10);
			HAL_Delay (500);
		break ;
    default :
      break;
  
  }
  if( GUI_DispDelay (&gui_disp_100ms_flag ,2))
  {
    GUI_DispDecMin(data_saved[6]);
    
  }
  
}


void RestrictRangeAjust(void)
{
//	if(PID_PressDelay(&key_10ms_flag,1))
//  RestrictRangeItem(GUI_PID_Scan(SCAN_SPEED_MIN ));
}


void TempArcCall(void)
{
	static uint8_t n =0;
 
 
	
	switch (n)
	{
		case 0:hotter_state = 19;n =1;
			break ;
		case 1:HotterWorkStateDisp( ); 
			break ;
		default :
			
			break ;
	}
	
	
}

void SettingReset (void)
{
		if(PID_PressDelay(&key_10ms_flag,1))
		{
//			if(GUI_PID_Scan(SCAN_SPEED_MIN ) ==HT_ENTER_KEY ) 
			{
				para_set();
				menu_in_flag =NO;
			}
		}
		if( HalTimeDelaySimple (&gui_disp_10ms_flag ,8)) //8--20
		{
			GUI_BackLight (GUI_BreathLightPWM() );//存放 明亮度数值
		}
	
}


