//=====================================================================//
//函数：void UpDate_Menu(UCHAR First_Index,UCHAR Menu_Index,UINT** Menu_List)
//功能：刷新整屏菜单显示函数
//参数：First_Index -  显示的第一条菜单号
//	    Menu_Index  -  处于选择状态的菜单项
//	    Menu_List   -  菜单资源链表指针
//返回：无
//日期:        2010.01.20        
//=====================================================================//
void RunMenu(void)                                                         ;

void CreateEditDig(unsigned char ID,unsigned int x_s   ,unsigned int y_s,
                   unsigned char width,unsigned char height)                                 ;
void IniEditDig(unsigned char ID   ,unsigned char type  ,unsigned char font,
                unsigned char frame,unsigned char limit ,char* p             )               ;
void SetEditDig(unsigned char ID,char* p)                                                    ;				
void ActiveEditDig(unsigned char ID   ,unsigned char Active)  ;	
void EditDigMessage(unsigned char Message,char * p)           ;
void RedrawEditDig(unsigned char Index)                       ;                    
void ReadEditDig(unsigned char ID,unsigned char *Ret_Buf)     ;
void DeleteEditDig(unsigned char ID)                          ;
void UpDate_Menu(unsigned char First_Index,
                 unsigned char Menu_Index  )                  ;
void Update_ScrollBar(unsigned int x,unsigned int y,
                      unsigned char height,
                      unsigned char position,
					  unsigned char total                    );
void Redraw_Menu(unsigned char First_Index,
                 unsigned char Menu_Index  )                  ;
void Initial_Menu(void)                                       ;
void MenuMessage(unsigned char Message)                       ;                    


void UpDate_Menu(unsigned char First_Index,unsigned char Menu_Index)
{
  unsigned int  y_Index                                                                ;
  unsigned char i                                                                      ;
  _DINT()                                                                              ;
  if(Dis_Menu_Num+First_Index> Menu.MenuConfig[0])
    Dis_Menu_Num =  Menu.MenuConfig[0] - First_Index                                   ; 
  y_Index  = 0                                                                         ; // 
  if(First_Index_old!=First_Index)                                                       // window内首条菜单项改变 
  {
    if(y_Index_old==0xFF)                                                                // 初始绘制菜单
    {
      DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                                       ; // 清空窗口 
      Color    = TITLE_COLOR                                                           ; // 显示标题条
      Color_BK = TITLE_BK_COLOR                                                        ;
      DrawRectFill(0,30,240,30,Color_BK)                                               ;
      PutString24M(120-25*strlen((char*)Menu.MenuTitle)/4,33,Menu.MenuTitle)          ;
      y_Index = Y_OFFSET-1                                                             ; // 由上而下显示各菜单项
      for(i=First_Index;i<Dis_Menu_Num+First_Index;i++)
      {
        if(i==Menu_Index)
		{
          DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)           ; // 显示选择条		
          ShowMenu_Item(y_Index,i+1,1)                                               ;
		}
	    else
          ShowMenu_Item(y_Index,i+1,0)                                               ; // 显示非被选菜单项
	    y_Index += Y_WIDTH_MENU                                                      ;
      }        

      y_Index_old     = 0                                                              ;
      First_Index_old = 0                                                              ;
    }
    else                                                                                 // 首菜单项改变 
    {
      Color_BK = WINDOW_BK_COLOR                                                       ;            
      if(First_Index_old<First_Index)                                                    // 菜单向上滚动
      {
        y_Index = Y_OFFSET-1+Y_WIDTH_MENU*(Dis_Menu_Num-1)                             ;
        DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)               ; // 清选中菜单项内容
        DrawRectFill(0,Y_OFFSET-1,Dis_X_MAX,Y_WIDTH_MENU*(Dis_Menu_Num-1),
		             WINDOW_BK_COLOR                                      )    ; // 清空被选菜单项上部内容
      	y_Index = Y_OFFSET-1 + Y_WIDTH_MENU*(Dis_Menu_Num-1)                           ; // 自下而上显示被选菜单项
        for(i=0;i<Dis_Menu_Num;i++)
        {
		  if(i==0)
            ShowMenu_Item(y_Index,Menu_Index-i+1,1)                                    ;
	      else
            ShowMenu_Item(y_Index,Menu_Index-i+1,0)                                    ; // 显示非被选菜单项
	      y_Index -= Y_WIDTH_MENU                                                  ;
        }        
      }
      else
      {
	y_Index = Y_OFFSET-1                                                           ;
        DrawRectFill(0,Y_WIDTH_MENU+Y_OFFSET-1,Dis_X_MAX,Y_WIDTH_MENU*(Dis_Menu_Num-1),
		             WINDOW_BK_COLOR                                          ); // 清空被选菜单项下部内容
        DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)               ; // 清选中菜单项内容
        for(i=First_Index;i<Dis_Menu_Num+First_Index;i++)
        {
          if(i==Menu_Index)
            ShowMenu_Item(y_Index,i+1,1)                                               ;
	      else
            ShowMenu_Item(y_Index,i+1,0)                                               ; // 显示非被选菜单项
	      y_Index += Y_WIDTH_MENU                                                      ;
        }        
      }
    }
    First_Index_old = First_Index                                                      ;
  }
  else                                                                                   // 首菜单项无变化
  {
    y_Index = y_Index_old-First_Index_old                                              ;
    y_Index = y_Index*Y_WIDTH_MENU+Y_OFFSET-1                                          ;
    DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,WINDOW_BK_COLOR)                   ;            
    ShowMenu_Item(y_Index,y_Index_old+1,0)                                             ;
    y_Index = Menu_Index-First_Index                                                   ;
    y_Index = y_Index*Y_WIDTH_MENU+Y_OFFSET-1                                          ;
//    DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,WINDOW_COLOR)                      ;
    DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)                   ;
    ShowMenu_Item(y_Index,Menu_Index+1,1)                                              ;
  }
  y_Index_old = Menu_Index                                                             ;
  Update_ScrollBar(Dis_X_MAX+2,Y_OFFSET-5,289-Y_OFFSET,Menu_Index,Menu.MenuConfig[0])  ;
  _EINT()                                                                              ;
}



//=====================================================================//
//函数：void Redraw_Menu(UCHAR First_Index,UCHAR Menu_Index,UINT** Menu_List)
//描述：刷新整屏菜单显示函数
//参数：First_Index  当面显示页的第一条菜单号
//		Menu_Index   当前处于选用的菜单项
//		Menu_List    菜单资源链表指针
//=====================================================================//
void Redraw_Menu(unsigned char First_Index,unsigned char Menu_Index)
{
  First_Index_old   = 0xFF                           ;
  y_Index_old       = 0xFF                           ;
//  Font_GB = *(Menu_Config+1)                         ;
//  Font_String = *(Menu_Config+2)                     ;
//  Y_WIDTH_MENU = *(Menu_Config+3)                    ;
//  X_SPACE_FRONT = *(Menu_Config+4)                   ;
  Dis_Menu_Num = (Dis_Y_MAX+1-Y_OFFSET)/Y_WIDTH_MENU ;  
  UpDate_Menu(First_Index,Menu_Index)                ;  
}
//=====================================================================//
//函数：void Initial_Menu(UINT** Menu_List)
//功能：刷新整屏菜单显示函数
//参数：Menu_List - 菜单资源链表指针
//=====================================================================//
void Initial_Menu(void)
{
  First_Index_old   = 0xFF                                ;
  y_Index_old       = 0xFF                                ;
//  Font_GB           = *(Menu_Config+1)                    ; // 中文字体大小
//  Font_String       = *(Menu_Config+2)                    ; // 西文字体大小
//  Y_WIDTH_MENU      = *(Menu_Config+3)                    ; // 菜单项占用纵轴点数
//  X_SPACE_FRONT     = *(Menu_Config+4)                    ; // 菜单项横轴缩进点数
  Dis_Menu_Num      = (Dis_Y_MAX+1-Y_OFFSET)/Y_WIDTH_MENU ;       
//  FontSet_cn(Font_GB)                                     ;
//  FontSet_en(Font_String)                                 ;
//  Active_Index = Menu.MenuConfig[6]                       ;
  Active_Index = 0x00                                     ;
  First_Index  = Active_Index                             ;
  if(First_Index<Dis_Menu_Num)
    First_Index    = 0x00                                 ;
  while(First_Index>Dis_Menu_Num)
    First_Index   -= (Dis_Menu_Num-1)                     ;
  UpDate_Menu(First_Index,Active_Index)                   ;
  Color_BK = STATUS_BK_COLOR                              ;
  Color    = STATUS_COLOR                                 ;
  PutStringCN24(10,294,"确认")                            ; // 底栏
  if(Menu.MenuIndex>0)
    PutStringCN24(185,294,"返回")                         ; 
  else
    DrawRectFill(185,294,54,25,STATUS_BK_COLOR)           ;
}

void Update_ScrollBar(unsigned int x,unsigned int y,unsigned char height,
                      unsigned char position,unsigned char total         )
{
  unsigned int temp                                       ;
  DrawRectFill(x,y,8,height,WINDOW_BK_COLOR)              ; // 外框
  DrawRect(x,y,8,height,WINDOW_COLOR)                     ; // 外框
  temp = y+(unsigned int)(height-44)*position/(total-1)   ;
  DrawRectFill(x,temp,8,44,WINDOW_COLOR)                  ; // 滑块
}

void SetColor(unsigned long int color)
{
  Color    = RGB888to565(color)                                   ;
}

void SetBKColor(unsigned long int color)
{
  Color_BK = RGB888to565(color)                                   ;
}


//***************************************************************************************
//																	                    *
// 		void MenuMessage(): 菜单消息管理                    		        *
//																	                    *
//***************************************************************************************
void MenuMessage(unsigned char Message)                    
{
  _DINT()                                                               ;
  switch(Message)
  {
  case Update:
    Redraw_Menu(First_Index,Active_Index)                               ;
    break                                                               ;
  case Up:
    if(Active_Index==0) 
      break                                                             ;
    else 
      Active_Index--                                                    ;
    if(Active_Index<First_Index)
      if(First_Index>0)       First_Index --                            ;
    UpDate_Menu(First_Index,Active_Index)                               ;
    break                                                               ;
  case Down:
    if(Active_Index==Menu.MenuConfig[0]-1)
      break                                                             ;
    else 
    {
      Active_Index++                                                    ;
      if(Active_Index>First_Index+6)
        First_Index++                                                   ;
      UpDate_Menu(First_Index,Active_Index)                             ;
    }
    break                                                               ;
  case Left:
    if(First_Index-7>0)
    {
      First_Index   -= 7                                                ;
      Active_Index   = First_Index + 6                                  ;  
      Redraw_Menu(First_Index,Active_Index)                             ;
    }
    else if(First_Index!=0)
    {
      First_Index    = 0                                                ;
      Active_Index   = First_Index                                      ;  
      Redraw_Menu(First_Index,Active_Index)                             ;
    }
    break                                                               ;
  case Right:
    if(First_Index+Dis_Menu_Num<Menu.MenuConfig[0])
    {
      First_Index   += Dis_Menu_Num                                     ;
      Active_Index   = First_Index                                      ;
      if(First_Index+7>Menu.MenuConfig[0])
      {
        Active_Index = Menu.MenuConfig[0]-1                             ;
        First_Index  = Active_Index-6                                   ;
      }
      Redraw_Menu(First_Index,Active_Index)                             ;
    }
    break                                                               ;
  }
  MenuUpdated = 0x00                                                    ;
  _EINT()                                                               ;
}




void RunMenu(unsigned char Menu_Index)
{
  unsigned char key = 0xFF , menu_index  , SubMenuIndex                   ;
  unsigned char First_Item , Active_Item , Active_Item_Index              ;
  unsigned char CallBackIndex                                             ;

  Clear_LCD(Color_BK)                                                     ;
  DrawRectFill(0,0,240,25,Blue)                                           ;
  DrawRectFill(0,295,240,25,Blue)                                         ;
  Color    = 0xFFFF                                                       ;
  Color_BK = Blue                                                         ;
  PutStringCN16(4,4,Menus[Menu_Index].MenuTitle)                          ;
  Color    = Black                                                        ;
  Color_BK = WINDOW_BK_COLOR                                              ;  
  
  Clear_LCD(STATUS_BK_COLOR)                                              ;
  Color_BK = STATUS_BK_COLOR                                              ;
  PutStringCN24(10,294,"确认")                                            ; // 底栏
  PutStringCN24(185,294,"取消")                                           ; 
  LoadMenu(MAIN_MENU,PERMISSION)                                          ;
  Initial_Menu()                                                          ;
  for(;;)
  {
    key = GetKeyPress()                                                   ;
    switch(key)
    {
    case Up: case Down: case Left: case Right:
      MenuMessage(key)                                                    ;
      break                                                               ;
    case  8:
      MenuMessage(Up)                                                     ;
      break                                                               ;
    case  0:
      MenuMessage(Down)                                                   ;
      break                                                               ;
    case Esc:
      {
//        if(Current_Menu[0][6]!=EMPTY)
//          Item_OP[Current_Menu[0][6]](0,0)                                ; // 菜单执行函数
        menu_index = GetMenuIndex()                                       ;
        if(Menus[menu_index].MasterMenu!=EMPTY)
//          return Menus[menu_index].MasterMenu                             ;
        {
          LoadMenu(Menus[menu_index].MasterMenu,PERMISSION)               ;
          Initial_Menu()                                                  ;
        }
        break                                                             ;
      }
    case OK: case Enter:
      {
        First_Item          = GetFirstItem()                              ;
        Active_Item         = GetActiveItem()                             ;
        Active_Item_Index   = GetActiveItemIndex()                        ;        
        if(MENU_ITEM[Active_Item_Index].CallBackFuncIndex!=EMPTY)           // 有操作函数
        {
          CallBackIndex= MENU_ITEM[Active_Item_Index].CallBackFuncIndex   ;
          SubMenuIndex = MENU_ITEM[Active_Item_Index].SubMenuIndex        ;
          if(SubMenuIndex!=EMPTY)                                           // 有操作函数且有动态子菜单
          {
            SubMenuIndex = MENU_ITEM[Active_Item_Index].SubMenuIndex      ; 
            if(Item_OP[CallBackIndex](0,Active_Item_Index  )==0x00)         // 生成子菜单成功
              Initial_Menu()                                              ; // 初始化动态子菜单
            else
              Redraw_Menu(First_Item,Active_Item)                         ; // 未生成子菜单，重画原菜单
          }
          else
          {
            Item_OP[CallBackIndex](0,Active_Item_Index  )                 ; // 无动态子菜单
            Redraw_Menu(First_Item,Active_Item)                           ; // 重画菜单
          }
        }
        
/*        
        if(MENU_ITEM[Item_Index].CallBackFuncIndex!=EMPTY)
        {
          Item_OP[MENU_ITEM[Item_Index].CallBackFuncIndex](0,Active_Index);
          if(  (MENU_ITEM[Item_Index].SubMenuIndex!=EMPTY)
             &&(Master_Or_Sub==GOTO_SUB                  ))
            return MENU_ITEM[Item_Index].SubMenuIndex                     ;
          Redraw_Menu(First_Index,Active_Index,(UCHAR**)P_Current_Menu)   ;          
        }
        else
*/      
        else  
        if(MENU_ITEM[Active_Item_Index].SubMenuIndex!=EMPTY)
//          return MENU_ITEM[Item_Index].SubMenuIndex                       ;
        {
          LoadMenu(MENU_ITEM[Active_Item_Index].SubMenuIndex,PERMISSION)  ;
          Initial_Menu()                                                  ;
        }
        break                                                             ;          
      }
    case Power:
      return                                                              ;
    default:
      break                                                               ;
    }
  }
}  




//***************************************************************************************
//																	                    *
// 		void CreateEditDig(): 创建数字编辑控件                           		        *
//																	                    *
//***************************************************************************************
void CreateEditDig(unsigned char ID   ,
                   unsigned int x_s   ,unsigned int y_s,
                   unsigned char width,unsigned char height)
{
  unsigned char i                           ;
  Color    = WINDOW_COLOR                   ; 
  Color_BK = 0xFFFF                         ;   
  for(i=0;i<6;i++)
  {
    if(Edit_Dig[i].status&EDIT_VALID) 
	  continue                          ;
    else
    {
      Edit_Dig[i].status  |= EDIT_VALID     ; // 设置有效标志
      Edit_Dig[i].id       = ID             ; // 设置标识
      Edit_Dig[i].x        = x_s            ; // 设置坐标
      Edit_Dig[i].y        = y_s            ;
      Edit_Dig[i].width    = width          ; // 设置大小
      Edit_Dig[i].height   = height         ;
      break                                 ;
    }
  }
}
//***************************************************************************************
//																	                    *
// 		void IniEditDig(): 初始化数字编辑控件                           		        *
//																	                    *
//***************************************************************************************
void IniEditDig(unsigned char ID   ,unsigned char type  ,unsigned char font,
                unsigned char frame,unsigned char limit ,char* p            )
{
  unsigned char i,j                                                   ;
  Color    = WINDOW_COLOR                                             ; 
  Color_BK = 0xFFFF                                                   ;   
  for(i=0;i<6;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                    ;
	else
	{ 
	  if(!(Edit_Dig[i].status&EDIT_VALID))
	    break                                                     ;	  
	  if(type!=0)
	    Edit_Dig[i].status    |= EDIT_MODE                        ; // 设置编辑模式
	  else
	    Edit_Dig[i].status    &=~EDIT_MODE                        ; 
	  if(font!=0)
	    Edit_Dig[i].status    |= EDIT_FONT                        ; // 设置字体
	  else
	    Edit_Dig[i].status    &=~EDIT_FONT                        ; 
	  Edit_Dig[i].property     = frame<<5                         ; // 设置边框属性
	  Edit_Dig[i].property    += limit                            ; // 设置最大字符数  
	  for(j=0;j<strlen(p);j++)
	    Edit_Dig[i].string[j]  = p[j]                             ;	    
//   	  Edit_Dig[i].string[limit]= 0x00                             ; 
   	  Edit_Dig[i].string[j]= 0x00                                 ; 
	  RedrawEditDig(i)                                            ;    
	  break                                                       ;
	}
  }
}
//***************************************************************************************
//																	                    *
// 		void IniEditDig(): 初始化数字编辑控件                           		        *
//																	                    *
//***************************************************************************************
void SetEditDig(unsigned char ID,char* p)
{
  unsigned char i,j                                                   ;
  Color    = WINDOW_COLOR                                             ; 
  Color_BK = 0xFFFF                                                   ;   
  for(i=0;i<6;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                    ;
	else
	{ 
          if(Edit_Dig[i].status&EDIT_ACTIVE)
	    Active_Digit           = 0                                ;
	  if(!(Edit_Dig[i].status&EDIT_VALID))
	    break                                                     ;	  
	  for(j=0;j<strlen(p);j++)
	    Edit_Dig[i].string[j]  = p[j]                             ;	    
   	  Edit_Dig[i].string[j]    = 0x00                             ; 
	  RedrawEditDig(i)                                            ;    
	  break                                                       ;
	}
  }
}
//***************************************************************************************
//																	                    *
// 		void ActiveEditDig(): 设置数字编辑控件活动状态                     		        *
//																	                    *
//***************************************************************************************
void ActiveEditDig(unsigned char ID   ,unsigned char Active)
{
  unsigned char i                                                           ;
  Color    = WINDOW_COLOR                                                   ; 
  Color_BK = 0xFFFF                                                         ;   
  for(i=0;i<6;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                          ;
    else
    {
      if(!(Edit_Dig[i].status&EDIT_VALID))
	break                                                           ;
      else
      {
        if(Active)
        {
          if(Active_Edit_Dig!=0xFF)
	  {
            Edit_Dig[Active_Edit_Dig].status |= EDIT_ACTIVE             ; // 如有活动控件，取消其活动状态并消隐其光标显示
	    RedrawEditDig(Active_Edit_Dig)                              ;                                               		  
	  }
          Active_Edit_Dig     = i                                       ;
	  Edit_Dig[i].status |= EDIT_ACTIVE                             ; // 设置激活状态
	  Active_Digit = strlen(Edit_Dig[i].string)                     ; // 光标设置在最后一位
        }
        else                                                              // 取消控件活动状态
	{
          Edit_Dig[i].status &=~EDIT_ACTIVE                             ; 
          if(Active_Edit_Dig == i)
            Active_Edit_Dig  =  0xFF                                    ;
	}
      }
    }
  }
}
//***************************************************************************************
//																	                    *
// 		void ReadEditDig(): 设置数字编辑控件活动状态                     		        *
//																	                    *
//***************************************************************************************
void ReadEditDig(unsigned char ID,unsigned char *Ret_Buf)
{
  unsigned char i                                                           ;
  for(i=0;i<6;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                          ;
	else
	{	  
	  if(!(Edit_Dig[i].status&EDIT_VALID))
	    break                                                           ;
	  else
	  {	
            strcpy((char *)Ret_Buf,Edit_Dig[i].string)                              ;
	    Edit2Read   = i                                                 ; // 读数据
            ReadDigit   = 0x00                                              ; // 读数据位
	  }
	}
  }
}



//***************************************************************************************
//																	                    *
// 		void ActiveEditDig(): 设置数字编辑控件活动状态                     		        *
//																	                    *
//***************************************************************************************
void EditDigMessage(unsigned char Message,char * p)                    
{
  unsigned char i,temp,k                                                ;
  _DINT()                                                               ;  
  Color    = WINDOW_COLOR                                               ; 
  Color_BK = 0xFFFF                                                     ;   
  temp   = Edit_Dig[Active_Edit_Dig].x
	  +Edit_Dig[Active_Edit_Dig].width/2
	  -strlen(Edit_Dig[Active_Edit_Dig].string)*6                   ;
  temp  += Active_Digit*13-1-4                                          ; // 计算光标位置   
  i      = Active_Edit_Dig                                              ;
  DrawVerticalLine( temp,
	            Edit_Dig[Active_Edit_Dig].y
		   +Edit_Dig[Active_Edit_Dig].height/2-10,
		    22,Color_BK                           )             ; // 消隐光标
  if(Message==129)                                                        // 输入了中文字符
  {
    if(strlen(Edit_Dig[i].string)+1<(Edit_Dig[i].property&EDIT_LIMIT))
    {
      Edit_Dig[i].string[strlen(Edit_Dig[i].string)+2] = 0x00           ; // 插入字符
      for(k=strlen(Edit_Dig[i].string)+1;k>Active_Digit;k--)                
        Edit_Dig[i].string[k] = Edit_Dig[i].string[k-2]                 ; 
      Edit_Dig[i].string[Active_Digit++] = *p                           ;
      Edit_Dig[i].string[Active_Digit++] = *(++p)                       ;
      RedrawEditDig(i)                                                  ;
    }
  }
  switch(Message)
  {
  case 128:
    RedrawEditDig(i)                                                    ;
  case 0xFF:
    break                                                               ;
  case Left:
    if(Active_Digit==0) 
      Active_Digit = strlen(Edit_Dig[i].string)                         ; 
    else                
      Active_Digit--                                                    ;
    if(Edit_Dig[i].string[Active_Digit]>128)
      Active_Digit--                                                    ;
    break                                                               ;
  case Right:
    if(Active_Digit==strlen(Edit_Dig[i].string))
      Active_Digit = 0                                                  ; 
    else              
      Active_Digit++                                                    ;
    if(Edit_Dig[i].string[Active_Digit-1]>128)
      Active_Digit++                                                    ;
    break                                                               ;
  case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:case 0:
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': 
  case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': 
  case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': 
  case 'Y': case 'Z':
    if(strlen(Edit_Dig[i].string)<(Edit_Dig[i].property&EDIT_LIMIT))
    {
      Edit_Dig[i].string[strlen(Edit_Dig[i].string)+1] = 0x00           ; // 插入字符
      for(k=strlen(Edit_Dig[i].string);k>Active_Digit;k--)                
        Edit_Dig[i].string[k] = Edit_Dig[i].string[k-1]                 ; 
      if(Message<=9)
        Edit_Dig[i].string[Active_Digit] = Message+0x30                 ;
      else
        Edit_Dig[i].string[Active_Digit] = Message                      ;
      Active_Digit++                                                    ;
      RedrawEditDig(i)                                                  ;
    }
    break                                                               ;
  case Delete:
    if(strlen(Edit_Dig[i].string)>0)
    {
      temp = strlen(Edit_Dig[i].string)                                 ;
     if(Edit_Dig[i].string[Active_Digit]>128)
        for(k=Active_Digit;k<temp-1;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+2]               ; // 删除下一中文字符
      else  
        for(k=Active_Digit;k<temp;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+1]               ; // 删除下一西文字符
      RedrawEditDig(i)                                                  ;
    }
    break                                                               ;
  case Backspace:
    if(Active_Digit==0)     break                                       ;
    Active_Digit--                                                      ;
    if(Edit_Dig[i].string[Active_Digit]>128)
      Active_Digit--                                                    ;
    if(strlen(Edit_Dig[i].string)>0)
    {
      temp = strlen(Edit_Dig[i].string)                                 ;
      if(Edit_Dig[i].string[Active_Digit]>128)
        for(k=Active_Digit;k<temp-1;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+2]               ; // 删除下一中文字符
      else
        for(k=Active_Digit;k<temp;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+1]               ; // 删除下一字符
      RedrawEditDig(i)                                                  ;
    }
    break                                                               ;
  default:
    break                                                               ;
  }
  Event         |= SHOULD_FLICKER                                       ;
  flicker_status = 0x00                                                 ;
  _EINT()                                                               ;  
}

//***************************************************************************************
//																	                    *
// 		void ActiveEditDig(): 设置数字编辑控件活动状态                     		        *
//																	                    *
//***************************************************************************************
void RedrawEditDig(unsigned char Index)                    
{
  unsigned char  i,j                                                    ;
  char* p                                                               ;
  unsigned char string[20]                                              ;
  i      = Index                                                        ;
  p      = Edit_Dig[i].string                                           ;
  DrawRectFill(Edit_Dig[i].x,Edit_Dig[i].y,
	       Edit_Dig[i].width, Edit_Dig[i].height+1,Color_BK)        ; // 清除显示 
  if(Edit_Dig[i].property&EDIT_FRAME)                                     // 需绘制边框
    DrawRect(Edit_Dig[i].x,Edit_Dig[i].y,
	     Edit_Dig[i].width,
	     Edit_Dig[i].height,Color    )                              ; 
  if(DISP_MODE==0x00)
    PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,            // 居中显示
	        Edit_Dig[i].y+Edit_Dig[i].height/2-12,(unsigned char*)p);
  else
  {
    for(j=0;j<strlen(p);j++)
      string[j]  = '*'                                                  ;
    string[j]    = 0x00                                                 ;
    PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,            // 居中显示
	        Edit_Dig[i].y+Edit_Dig[i].height/2-12,string    )       ;
  }
  Event         |= SHOULD_FLICKER                                       ;
  flicker_status = 0x00                                                 ;
}
//***************************************************************************************
//																	                    *
// 		void DeleteEditDig(): 设置数字编辑控件活动状态                     		        *
//																	                    *
//***************************************************************************************
void DeleteEditDig(unsigned char ID)
{
  unsigned char i                                                           ;
  for(i=0;i<6;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                          ;
	else
	{	  
	  if(!(Edit_Dig[i].status&EDIT_VALID))
	    break                                                           ;
	  else
	  {	
	    Edit_Dig[i].status   &=~EDIT_VALID                              ; // 清除有效状态
	    if(Edit_Dig[i].status&EDIT_ACTIVE)		  
	      Active_Edit_Dig = 0xFF                                        ; // 消隐光标
	    Edit_Dig[i].status   &=~EDIT_ACTIVE                             ; // 清除活动状态		
	    DrawRectFill(Edit_Dig[i].x,Edit_Dig[i].y,
	                 Edit_Dig[i].width, Edit_Dig[i].height+1,Color_BK)  ; // 清除显示 		
	  }
	}
  }
}

//--------------------------- 载入指定菜单 ----------------------------//

void LoadMenu(unsigned char Menu_Index,unsigned char Permission)
{
  unsigned char i,j,k                                                                ; // 导入菜单配置项
//  unsigned char temp,temp1,temp2                                                     ;
//  temp  = Menu.MenuConfig[0]                                                         ; // 备份菜单项数目
//  temp1 = Menu.MenuConfig[8]                                                         ; // 备份页数
//  temp2 = Menu.MenuConfig[9]                                                         ;
  Menu.MenuIndex =  Menu_Index                                                       ;
  str_cpy((char *)Menu.MenuConfig,(char *)Menus[Menu_Index].MenuConfig,10)           ; // 拷贝菜单配置项
  strcpy((char *)Menu.MenuTitle,(char *)Menus[Menu_Index].MenuTitle)                 ; // 拷贝菜单标题
  Menu.MenuConfig[0] = 0                                                             ; // 菜单项数清0
  for(i=1;i< Menus[Menu_Index].MenuConfig[0]+1;i++)                                    // 导入菜单标题字符串
  {
//    temp = Menus[Menu_Index].ItemIndex[i-1]                                          ;
//    temp = MENU_ITEM[temp].AccessLevel                                               ;
//    if(Permission>=temp)                                                             
    if(Permission>=(MENU_ITEM[Menus[Menu_Index].ItemIndex[i-1]].AccessLevel))
    {
      j = Menu.MenuConfig[0]++                                                       ; // 计算菜单项数目
      Menu.ItemIndex[j] = Menus[Menu_Index].ItemIndex[i-1]                           ; // 记录菜单项索引
      for(k=0;k<20;k++)
        Menu.ItemTitle[j][k] = 0x00                                                  ;
      strcpy((char *)Menu.ItemTitle[j],
             (char *)MENU_ITEM[Menus[Menu_Index].ItemIndex[i-1]].ItemTitle)          ; // 拷贝菜单项标题
      Menu.ItemConfig[j] =  MENU_ITEM[Menus[Menu_Index].ItemIndex[i-1]].Config       ;     // 拷贝菜单项配置字      
    }    
  }
//  Menu.MenuConfig[0] = temp                                                          ;
//  Menu.MenuConfig[8] = temp1                                                         ;
//  Menu.MenuConfig[9] = temp2                                                         ;
}

unsigned char GetFirstItem(void)
{
  return First_Index                      ;
}
unsigned char GetActiveItem(void)
{
  return Active_Index                     ;
}
unsigned char GetActiveItemIndex(void)
{
  return Menu.ItemIndex[Active_Index]     ;
}

unsigned char GetMenuIndex(void)
{
  return Menu.MenuIndex                   ;
}

void Flicker_Cursor(void)
{
  unsigned int temp                                                   ;
  static unsigned char flicker_status = 0x00                          ;                   
  if((Active_Edit_Dig!=0xFF))
  {
    temp   = Edit_Dig[Active_Edit_Dig].x
	    +Edit_Dig[Active_Edit_Dig].width/2
	    -strlen(Edit_Dig[Active_Edit_Dig].string)*6               ;
    temp  += Active_Digit*13-1-4                                      ; // 计算光标横坐标
    if(flicker_status++==0x00)
      DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-10,22,Color)    ; // 绘制光标
    else
    {
      DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-10,22,Color_BK) ; // 消隐光标
      flicker_status = 0x00                                           ;
    }
  }
}




//---------------------------------- LCD 颜色 --------------------------------------//
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define EDIT_VALID            BIT7                // 编辑控件状态        ：1 - 有效    0 - 无效
#define EDIT_ACTIVE           BIT6                // 编辑控件状态        ：1 - 活动    0 - 非活动 
#define EDIT_FONT             BIT5                // 编辑控件字体        ：1 - 24点阵  0 - 16点阵
#define EDIT_MODE             BIT4                // 编辑控件模式        ：1 - 删除式  0 - 插入式
#define EDIT_CURSOR           0x0F                // 编辑控件光标位置    
#define EDIT_FRAME            0xE0                // 编辑控件边框属性    : 
#define EDIT_LIMIT            0x1F                // 编辑控件最大字符数  

#define Horizontal        0x00
#define Vertical          0x01

#define TABLE_HZK24       0x000000                // HZK24字库首地址
#define TABLE_ASCII24     0x0927F0                // ASCII24字库首地址
#define TABLE_HZK16       0x0939F0                // HZK16字库首地址 
#define TABLE_ASCII20     0x0D4F50                // ASCII20字库首地址
#define TABLE_ASCII18     0x0D5E50                // ASCII20字库首地址
#define TABLE_ASCII16     0x0D6BD0                // ASCII20字库首地址
#define TABLE_HZK20       0x0D71D0                // HZK20字库首地址

#define USER_DATA         0x150000                // 用户数据起始地址

typedef struct 
{
  unsigned char     MenuIndex                   ;
  unsigned char     MenuConfig[40]              ; // 菜单配置
  unsigned char     MenuTitle[20]               ; // 菜单标题
  unsigned char     ItemTitle[25][20]           ; // 菜单项
  unsigned char     ItemIndex[25]               ; // 菜单项索引
  unsigned char     ItemConfig[25]              ; // 菜单项配置字
} MENU                                          ; 

typedef struct
{
  unsigned char id                              ; // 识别号
  unsigned int  x                               ; // 起始位置横坐标x
  unsigned int  y                               ; // 起始位置纵坐标y
  unsigned char width                           ; // 宽度
  unsigned char height                          ; // 高度
  unsigned char property                        ; // 属性
  unsigned char status                          ; // 状态
  unsigned char limit                           ; // 最大字符数
  char string[50]                               ; // 字符串
} EDIT_DIG                                      ; // 编辑控件


//#define RX_FRAME_RECEIVED     BIT0            // 收到有效通讯数据帧
#define SHOULD_FLICKER        BIT1              // 应闪烁
#define MENU_LOADED           BIT2              // 菜单已下载

//*
// 界面颜色
#define WINDOW_BK_COLOR         0xEF9F          // 窗口背景色
#define WINDOW_COLOR            Black           // 窗口前景色

#define TITLE_BK_COLOR          Blue            // 标题栏背景色
#define TITLE_COLOR             White           // 标题栏前景色
#define STATUS_BK_COLOR         Blue            // 状态栏背景色
#define STATUS_COLOR            White           // 状态栏前景色

#define Dis_X_MAX		       228
#define Dis_Y_MAX		       289


unsigned char BLight_Deg       = 0xFF                     ; // 背光亮度等级
unsigned int  Color            = Yellow                   ; // 前景颜色
unsigned int  Color_BK         = Blue                     ; // 背景颜色
unsigned char Active_Edit_Dig  = 0xFF                     ; // 活动的数字编辑控件
unsigned char Active_Digit     = 0xFF                     ;
unsigned char flicker_status                              ;
unsigned char Edit2Read        = 0xFF                     ; // 读数据
unsigned char ReadDigit        = 0xFF                     ; // 读数据位
unsigned char Font_CN          = 24                       ; // 中文字体
unsigned char Font_EN          = 24                       ; // 西文字体
unsigned char MenuNum          = 0xFF                     ; // 需上传的菜单数组数量
unsigned char MenuUpdated      = 0xFF                     ; // 菜单更新，需上传当前菜单选项
unsigned char First_Index_old  = 0xFF                     ;
unsigned char y_Index_old      = 0xFF                     ;
unsigned char First_Index      = 0x00                     ;
unsigned char Active_Index     = 0x00                     ;
unsigned char Dis_Menu_Num                                ;