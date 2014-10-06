//===========================================================================//
//                                                                           //
// 文件：  PIN_DEF.H                                                         //
// 说明：  BW-DK5438开发板单片机引脚功能定义与IO操作宏定义                   //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//


//***************************************************************************//
//                                                                           //
//                       引脚功能定义                                        //
//                                                                           //
//***************************************************************************//

// P1 引脚定义
#define KP_INT                 BIT0 // 键盘中断 
#define J60_INT                BIT1 // ENC28J60中断
#define BL_CTR                 BIT2 // TFT背光控制
#define TP_INT                 BIT3 // 触摸屏中断
#define DIG5                   BIT4 // 数码管第5位
#define DIG6                   BIT5 // 数码管第6位
#define DIG7                   BIT6 // 数码管第7位
#define DIG8                   BIT7 // 数码管第8位

// P2 引脚定义
#define	DB10   	 	       BIT0 // TFT数据总线高8位         
#define	DB11   	 	       BIT1           
#define	DB12   	 	       BIT2          
#define	DB13   	 	       BIT3           
#define	DB14   	 	       BIT4           
#define	DB15   	 	       BIT5           
#define	DB16   	 	       BIT6           
#define	DB17   	 	       BIT7  

// P3 引脚定义
#define	NCS25  	 	       BIT0  // SST25V片选
#define SI25                   BIT1  // SST25V数据输入
#define	LCD_NRD	               BIT1  // LCD读信号  
#define SO25                   BIT2  // SST25V数据输出
#define	LCD_NRS 	       BIT2  // LCD寄存器选择  
#define	LCD_NWR   	       BIT3  // LCD写信号
#define SC25                   BIT3  // SST25V数据时钟
#define	TP_CS  	 	       BIT4  // XTP2046片选
#define	TP_BUSY	 	       BIT5  // XTP2046忙
#define	LCD_NCS                BIT6  // LCD片选
#define LCD_NRST   	       BIT7  // LCD复位

// P4 引脚定义
#define	DB00   	 	       BIT0  // TFT数据总线低8位        
#define	DB01   	 	       BIT1           
#define	DB02   	 	       BIT2          
#define	DB03   	 	       BIT3           
#define	DB04   	 	       BIT4           
#define	DB05   	 	       BIT5           
#define	DB06   	 	       BIT6           
#define	DB07   	 	       BIT7           

// P5 引脚定义
#define	NWOL   	               BIT0  // ENC28J60网络唤醒
#define	J60_CS                 BIT1  // ENC28J60片选
#define	XT2IN	               BIT2  // XT2输入端
#define	XT2OUT	               BIT3  // XT2输出端
#define POWER 	               BIT4  // 外设电源控制端
#define NC01                   BIT5  // 未占用
#define	TXD_U                  BIT6  // USB串口输出
#define	RXD_U                  BIT7  // USB串口输入

// P6 引脚定义
#define	KPR0 	 	       BIT0  // 键盘0行
#define	KPR1   	               BIT1  // 键盘1行
#define	KPR2	               BIT2  // 键盘2行
#define	KPR3	               BIT3  // 键盘3行
#define KPC0   	 	       BIT4  // 键盘0列
#define	KPC1  	 	       BIT5  // 键盘1列
#define	KPC2  	               BIT6  // 键盘2列
#define	KPC3                   BIT7  // 键盘3列

// P7 引脚定义
#define	XT1IN   	       BIT0  // XT1输入
#define	XT1OUT                 BIT1  // XT1输出  
#define NC02                   BIT2  // 未占用
#define	LED_PWR                BIT3  // LED电源控制
#define DE_485    	       BIT4  // 485数据控制端
#define	TVBTM	               BIT5  // 主电源电压检测输入  
#define	TVBTB                  BIT6  // 备用电池电压检测输入  
#define	BUZZER                 BIT7  // 蜂鸣器控制端

// P8 引脚定义
#define	SEGA    	       BIT0  // 数码管段选
#define	SEGB    	       BIT1  
#define	SEGC    	       BIT2  
#define	SEGD    	       BIT3  
#define	SEGE    	       BIT4  
#define	SEGF    	       BIT5  
#define	SEGG    	       BIT6  
#define	SEGDP    	       BIT7  

#define LED_RED                BIT0  // P8.0 红灯亮起
#define LED_GRE                BIT1  // P8.1 绿灯亮起
#define RF_LPCTL               BIT2  // P8.2 射频卡休眠控制    
#define RF_SS                  BIT3  // p8.3 射频卡从机选择(SS)
#define RF_SCLK                BIT4  // p8.4 射频卡数据时钟输出(SCLK)
#define RF_DATA_OUT            BIT5  // p8.5 射频卡数据输出(MOSI)
#define RF_DATA_IN             BIT6  // p8.6 射频模块输入(MISO) 


// P9 引脚定义
#define DIG1                   BIT0  // 数码管第1位
#define DIG2                   BIT1  // 数码管第2位
#define DIG3                   BIT2  // 数码管第3位
#define DIG4                   BIT3  // 数码管第4位
#define IrDA_OUT               BIT4  // 红外输出
#define IrDA_IN                BIT5  // 红外输入
#define SDA5571                BIT6  // DAC5571数据
#define SCK5571                BIT7  // DAC5571时钟

// P10 引脚定义
#define	PNSS    	       BIT0  // 通用SPI从机片选
#define	PMOSI                  BIT1  // 通用SPI主机输出  
#define	PMISO                  BIT2  // 通用SPI主机输入  
#define	PSCK                   BIT3  // 通用SPI串行时钟  
#define TXD 	               BIT4  // UART发送  
#define	RXD	               BIT5  // UART接收  
#define	DIO_B20                BIT6  // DS18B20数据
#define	NC03                   BIT7  // 未占用

// P11 引脚定义
#define TACK                   BIT0  // ACLK测试端
#define TMCK                   BIT1  // MCLK测试端
#define TSMCK                  BIT2  // SMCLK测试端

//****************************************************************************
//                                                                           *
//                           操作宏定义                                      *
//                                                                           *
//****************************************************************************

//------------------------------- RC522操作宏 ------------------------------//
//#define    RF_POWER_ON             P8OUT &=~RF_PCTL                      // 射频模块上电
//#define    RF_POWER_OFF            P8OUT |= RF_PCTL                      // 射频模块下电
#define    RF_POWER_ON             _NOP()                                // 射频模块上电
#define    RF_POWER_OFF            _NOP()                                // 射频模块下电
#define    RED_LED_ON              P8OUT &=~LED_RED                      // 红灯亮起
#define    RED_LED_OFF             P8OUT |= LED_RED                      // 红灯熄灭
#define    GRE_LED_ON              P8OUT &=~LED_GRE                      // 绿灯亮起
#define    GRE_LED_OFF             P8OUT |= LED_GRE                      // 绿灯熄灭

#define    RF_SLEEP                P8OUT &=~RF_LPCTL                     // 射频模块唤醒
#define    RF_WAKEUP               P8OUT |= RF_LPCTL                     // 射频模块休眠
#define    SCLK_HIGH               P8OUT |= RF_SCLK                      // 数据时钟置高
#define    SCLK_LOW                P8OUT &=~RF_SCLK                      // 数据时钟置低
#define    DATA_OUT_HIGH           P8OUT |= RF_DATA_OUT                  // 数据置高
#define    DATA_OUT_LOW            P8OUT &=~RF_DATA_OUT                  // 数据置低
#define    SS_HIGH                 P8OUT |= RF_SS                        // 从机选择无效
#define    SS_LOW                  P8OUT &=~RF_SS                        // 从机选择有效
#define    DATA_IN                 P8IN  &  RF_DATA_IN                   // 数据输入

#define    NSS522_1                P8OUT |= RF_SS                        // 从机选择无效       
#define    NSS522_0                P8OUT &=~RF_SS                        // 从机选择有效
#define    SCK522_1                P8OUT |= RF_SCLK                      // 数据时钟置高
#define    SCK522_0                P8OUT &=~RF_SCLK                      // 数据时钟置低
#define    SI522_1                 P8OUT |= RF_DATA_OUT                  // 数据置高        
#define    SI522_0                 P8OUT &=~RF_DATA_OUT                  // 数据置低        
#define    SO522                 ((P8IN  &  RF_DATA_IN)>>6)              // 数据输入
#define    RST522_1                P8OUT |= RF_LPCTL
#define    RST522_0                P8OUT &=~RF_LPCTL

//------------------------------- 主电源开关 -------------------------------//
#define MAIN_POWER_ON          P5DIR  |= POWER;\
                               P5OUT  |= POWER
#define MAIN_POWER_OFF         P5OUT  &=~POWER
//------------------------------ LED电源开关 -------------------------------//
#define LED_POWER_ON          P7DIR  |= LED_PWR;\
                              P7OUT  |= LED_PWR
#define LED_POWER_OFF         P7OUT  &=~LED_PWR

//----------------------------- TFT液晶操作宏 ------------------------------//
#define  LCD_POWER_ON          P1OUT  &=~BL_CTR   // LCD背光开
#define  LCD_POWER_OFF         P1OUT  |= BL_CTR   // LCD背光关
#define  SET_LCD_RST           P3OUT  |= LCD_NRST // LCD复位
#define  CLR_LCD_RST           P3OUT  &=~LCD_NRST
#define  SET_LCD_CS            P3OUT  |= LCD_NCS  // LCD片选
#define  CLR_LCD_CS            P3OUT  &=~LCD_NCS
#define  SET_LCD_RS            P3OUT  |= LCD_NRS  // LCD寄存器选择
#define  CLR_LCD_RS            P3OUT  &=~LCD_NRS
#define  SET_LCD_WR            P3OUT  |= LCD_NWR  // LCD写信号
#define  CLR_LCD_WR            P3OUT  &=~LCD_NWR 
#define  SET_LCD_RD            P3OUT  |= LCD_NRD  // LCD读信号
#define  CLR_LCD_RD            P3OUT  &=~LCD_NRD
#define  LOW_BYTE_OUT(data)    P4OUT   = data
#define  HIGH_BYTE_OUT(data)   P2OUT   = data
#define  BUS_IN                P2DIR   = 0x00;\
                               P4DIR   = 0x00     // 
#define  BUS_OUT               P2DIR   = 0xFF;\
                               P4DIR   = 0xFF     
#define  TFT_DATA_HIGH         P2IN
#define  TFT_DATA_LOW          P4IN

//----------------------------- 通用SPI操作宏 ------------------------------//
#define  SET_PNSS              P10OUT |= PNSS
#define  CLR_PNSS              P10OUT &=~PNSS
#define  SET_PSCK              P10OUT |= PSCK
#define  CLR_PSCK              P10OUT &=~PSCK
#define  SET_PMOSI             P10OUT |= PMOSI
#define  CLR_PMOSI             P10OUT &=~PMOSI
#define  PMISO_IS_SET         (P10IN&PMISO)

//------------------------------ 触摸屏操作宏 ------------------------------//
#define  SET_TP_CS             P3OUT  |= TP_CS
#define  CLR_TP_CS             P3OUT  &=~TP_CS
#define  TP_INT_IS_CLR      (!(P1IN&TP_INT))

//------------------------------ 数码管操作宏 ------------------------------//
#define  SET_DIGITS_OUT        P1OUT   = 0xF0;\
                               P1DIR   = 0xF0;\
                               P9OUT   = 0x0F;\
                               P9DIR   = 0x0F
#define  SET_SEGS_OUT          P8DIR   = 0xFF;\
                               P8OUT   = 0x00
#define  MASK_DIGITS           P1OUT  |= 0xF0;\
                               P9OUT  |= 0x0F
#define  LOW_DIGITS_OUT(data)  P9OUT  &= data
#define  HIGH_DIGITS_OUT(data) P1OUT  &= data

//---------------------------------- 键盘操作宏 ----------------------------//
#define KEY_PORT_IN            P6IN 
#define KEY_PORT_OUT           P6OUT 
#define KEY_PORT_DIR           P6DIR 
#define KEY_PORT_REN           P6REN
#define ROW_IN_COL_OUT         P6DIR   =  KPC0+KPC1+KPC2+KPC3      ;\
                               P6OUT   =~(KPC0+KPC1+KPC2+KPC3)     ;\
                               __delay_cycles(20)                      // 键盘列输出行输入                                        
#define CLO_IN_ROW_OUT         P6DIR   =  KPR0+KPR1+KPR2+KPR3      ;\
                               P6OUT   =~(KPR0+KPR1+KPR2+KPR3)     ;\
                               __delay_cycles(20)                      // 键盘行输出列输入
#define INTERNAL_PULL_UP       P6REN   =  0xFF                         // 键盘端口内部上拉电阻使能

//------------------------------- USB端口 ----------------------------//

#define USB_PORT_SEL           P5SEL
#define USB_PORT_DIR           P5DIR

//---------------------------- RS232/485端口 -------------------------//
#define RS_PORT_SEL            P10SEL
#define RS_PORT_DIR            P10DIR
#define RS485_IN               P7OUT &=~DE_485;\
                               P7DIR |= DE_485;\
                               __delay_cycles(5)
#define RS485_OUT              P7OUT |= DE_485;\
                               P7DIR |= DE_485;\
                               __delay_cycles(5)

//------------------------------- 红外端口 ---------------------------//

#define IrDA_PORT_SEL          P9SEL
#define IrDA_PORT_DIR          P9DIR
#define IrDA_PORT_OUT          P9OUT

//---------------------------------- SST25V 操作宏 -------------------------//
#define SET_NCS25              P3OUT  |= NCS25    // SST25VF片选
#define CLR_NCS25              P3OUT  &=~NCS25

//---------------------------------- 键盘操作宏 ----------------------------//
#define ROW_IN_COL_OUT         P6DIR   =  KPC0+KPC1+KPC2+KPC3      ;\
                               P6OUT   =~(KPC0+KPC1+KPC2+KPC3)     ;\
                               __delay_cycles(20)                      // 键盘列输出行输入                                        
#define CLO_IN_ROW_OUT         P6DIR   =  KPR0+KPR1+KPR2+KPR3      ;\
                               P6OUT   =~(KPR0+KPR1+KPR2+KPR3)     ;\
                               __delay_cycles(20)                      // 键盘行输出列输入
#define INTERNAL_PULL_UP       P6REN   =  0xFF                         // 键盘端口内部上拉电阻使能

//------------------------------ ENC68J20操作宏 ----------------------------//

#define CLR_J60_CS             P5OUT  &=~J60_CS
#define SET_J60_CS             P5OUT  |= J60_CS


//---------------------------------- 键码定义-------------------------------//
#define No_key   255
#define F1        10
#define F2        11
#define F3        12
#define Delete    12
#define Backspace 13
#define Enter     14
#define OK        15
#define Up        16
#define Down      17
#define Left      18
#define Right     19
#define Esc       20
#define Cancel    20
#define Power     21
#define Power1    21
#define Power2    22
#define Light     22
#define Update   255

//------------------------------ 操作结果  ---------------------------------------//
#define    SUCCESS                    0
#define    FAILURE                    1
#define    CRC_ERROR                  2
