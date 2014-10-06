// 菜单常量定义
#define MAX_ITEM                           18  
#define ALL_ITEMS                          60
#define EMPTY                            0xFF  // 无效标识
#define DYNAMIC_OP                       0xFE  // 动态可执行菜单标识
#define DYNAMIC_VIEW                     0xFD  // 动态视图菜单标识
#define STATIC                           0xFC  // 静态菜单
#define SINGLE_PAGE                      0x00
#define MULTI_PAGE                       0x11
#define NEXT_PAGE                        0x11
#define PREVIOUS_PAGE                    0x22
#define CHECK_RECORD                     0x11
#define ITEM_ACCESS_HIDDEN               0x04 // 尚未开放
#define ITEM_ACCESS_SYSTEM               0x03 // 系统级
#define ITEM_ACCESS_MANAGER              0x02 // 管理员级
#define ITEM_ACCESS_OPERATOR             0x01 // 操作员级

// 菜单索引定义
#define MAIN_MENU                        0x00  // 主菜单索引
#define HUB_MANAGE_MENU                  0x01  // 集中器管理菜单索引
#define METER_MANAGE_MENU                0x02  // 水表管理菜单索引
#define DATA_MANAGE_MENU                 0x03  // 水表数据管理菜单索引
#define HANDSET_MANAGE_MENU              0x04  // 手持机管理菜单索引
#define HELP_MENU                        0x05  // 帮助菜单索引
#define VIEW_INFO_MENU                   0x06  // 查看水表信息菜单索引(视图菜单)
#define VIEW_CHECK_RECORD_MENU           0x07  // 查看数据记录菜单索引(视图菜单)
#define SELECT_DATA_INQ_MODE             0x08  // 选择检查记录查看方式菜单索引
#define CHECK_RECORD_LIST_MENU           0x09  // 检查记录列表菜单索引
#define FAULT_STA_MENU                   0x0A  // 故障统计菜单
#define VIEW_HUB_INFO_MENU               0x0B  // 查看集中器信息菜单(视图菜单)
#define VIEW_METER_INFO_MENU             0x0C  // 查看水表信息菜单(视图菜单)
#define VIEW_HUB_REC_MENU                0x0D  // 查看集中器信息菜单(视图菜单)
#define SELECT_DATA_READ_MODE            0x0E  // 选择数据读取模式
#define VIEW_DOWNLOAD_RESULT_MENU        0x0F  // 查看下载结果菜单(视图菜单)
#define SELECT_INFO_INQ_MODE             0x10  // 查看水表信息方式选择菜单
#define VIEW_HUB_INFO_INQ_MODE           0x11  // 查看集中器信息方式选择菜单
// 一级菜单项[0] :　主菜单
#define ITEM_HUB_MANAGE                  0x00  // 集中器管理
#define ITEM_METER_MANAGE                0x01  // 水表管理
#define ITEM_DATA_MANAGE                 0x02  // 数据管理
#define ITEM_HANDSET_MANAGE              0x03  // 手持器管理
#define ITEM_HELP                        0x04  // 水表数据管理
#define ITEM_RESERVE_00                  0x05  // 保留
#define ITEM_RESERVE_01                  0x06  // 保留
#define ITEM_RESERVE_02                  0x07  // 保留
#define ITEM_RESERVE_03                  0x08  // 保留

// 二级菜单项[1] :　集中器管理菜单
#define ITEM_SET_HUB_INFO                0x09  // 设置集中器信息
#define ITEM_CHECK_HUB_INFO              0x0A  // 查看集中器信息
#define ITEM_DELETE_HUB_INFO             0x0B  // 删除集中器信息
#define ITEM_ADD_METER_MANUAL            0x0C  // 手工添加子表
#define ITEM_ADD_METER_AUTO              0x0D  // 自动添加子表
#define ITEM_HUB_DEL_METER               0x0E  // 删除子表
#define ITEM_CHECK_SUB_INFO              0x0F  // 查询子表信息
#define ITEM_SET_HUB_CLOCK               0x10  // 集中器校时
#define ITEM_HUB_SET_AUTO_DATE           0x11  // 设置自动抄表
#define ITEM_HUB_CHECK_AUTO_DATE         0x12  // 查看自动抄表设置
#define ITEM_RESERVE_04                  0x13  // 保留
#define ITEM_RESERVE_05                  0x14  // 保留
#define ITEM_RESERVE_06                  0x15  // 保留
#define ITEM_RESERVE_07                  0x16  // 保留

// 二级菜单项[2] :　水表管理菜单
#define ITEM_CHECK_METER_INFO            0x17  // 查看水表信息
#define ITEM_METER_INIT                  0x18  // 水表初始化
#define ITEM_SET_METER_NO                0x19  // 设置水表表号
#define ITEM_READ_METER_NO               0x1A  // 读取水表表号
#define ITEM_SET_READOUT                 0x1B  // 设置水表示数
#define ITEM_SET_METER_TIME              0x1C  // 水表广播校时
#define ITEM_READ_METER_DATA             0x1D  // 读取水表数据
#define ITEM_SET_AVAILABLE               0x1E  // 设置水表出厂
#define ITEM_ONE_KEY                     0x1F  // 水表一键出厂
#define ITEM_METER_FORCE_OPEN            0x20  // 强制打开阀门
#define ITEM_METER_FORCE_CLOSE           0x21  // 强制关闭阀门
#define ITEM_CANCEL_FORCE                0x22  // 取消强制状态
#define ITEM_MAINTAIN                    0x23  // 阀门维护
#define ITEM_SET_AUTO                    0x24  // 设置结算日期
#define ITEM_CHECK_AUTO                  0x25  // 查看结算日期
#define ITEM_FAULT_STA                   0x26  // 故障表统计
#define ITEM_PROGRAM                     0x27  // 下载水表程序
#define ITEM_MAINTAIN_OPEN               0x28  // 阀门维护开，兼容旧表
#define ITEM_READ_STATUS                 0x29  // 读取状态信息
#define ITEM_RESERVE_10                  0x2A  // 保留
#define ITEM_RESERVE_11                  0x2B  // 保留
#define ITEM_RESERVE_12                  0x2C  // 保留
#define ITEM_RESERVE_13                  0x2D  // 保留


// 二级菜单项[3] :　数据管理菜单
#define ITEM_VIEW_DATA_RECORD            0x2E  // 查看水表数据
#define ITEM_UPLOAD_DATA_RECORD          0x2F  // 上传水表数据
#define ITEM_DELETE_DATA_RECORD          0x30  // 删除水表数据
#define ITEM_VIEW_HUB_DATA_RECORD        0x31  // 查看集中器信息
#define ITEM_EDIT_HUB_DATA_RECORD        0x32  // 编辑集中器信息
#define ITEM_DOWNLOAD_RECORD             0x33  // 下载水表数据
#define ITEM_EMPTY_RECORD                0x34  // 清空水表数据
#define ITEM_UPLOAD_RECORD               0x35  // 上传水表数据
#define ITEM_RESERVE_19                  0x36  // 保留

// 二级菜单项[4] :　手持机管理菜单
#define ITEM_HANDSET_INFO                0x37  // 查看手持机信息
#define ITEM_HANDSET_TIME                0x38  // 设置手持机时间
#define ITEM_HANDSET_LED                 0x39  // 设置手持机背光
#define ITEM_CREATE_USER                 0x3A  // 创建用户
#define ITEM_DELETE_USER                 0x3B  // 删除用户                                       
#define ITEM_VIEW_LOG                    0x3C  // 查看操作日志
#define ITEM_UPLOAD_LOG                  0x3D  // 上传操作日志
#define ITEM_DELETE_LOG                  0x3E  // 清空操作日志
#define ITEM_INI_MEMORY                  0x3F  // 初始化存储器
#define ITEM_PORT_SET                    0x40  // 端口设置
#define ITEM_SET_PRE_NO                  0x41  // 设置表号补足
#define ITEM_UPDATE_FIRMWARE             0x42  // 更新固件程序
#define ITEM_RESERVE_22                  0x43  // 保留
#define ITEM_RESERVE_23                  0x44  // 保留
#define ITEM_RESERVE_24                  0x45  // 保留
#define ITEM_RESERVE_25                  0x46  // 保留

// 二级菜单项[5] :　帮助菜单
#define ITEM_HUB_INFO_BY_METER_NO        0x47  // 按集中器号查询
#define ITEM_HUB_INFO_ALL                0x48  // 查看所有集中器
#define ITEM_RESERVE_28                  0x49  // 保留
#define ITEM_RESERVE_29                  0x4A  // 保留
#define ITEM_RESERVE_30                  0x4B  // 保留
#define ITEM_RESERVE_31                  0x4C  // 保留

// 三级菜单项[6] :　水表故障统计菜单
#define ITEM_VALVE_FAULT                 0x4D  // 阀门故障表统计
#define ITEM_LOW_POWER_FAULT             0x4E  // 低电故障表统计
#define ITEM_MEMORY_FAULT                0x4F  // 存储器故障表统计
#define ITEM_SENSOR_FAULT                0x50  // 传感器故障表统计
#define ITEM_OUTOF_WATER_FAULT           0x51  // 低水量故障表统计
#define ITEM_ALL_FAULT                   0x52  // 所有故障表统计
#define ITEM_RESERVE_32                  0x53  // 保留
#define ITEM_RESERVE_33                  0x54  // 保留
#define ITEM_RESERVE_34                  0x55  // 保留
#define ITEM_RESERVE_35                  0x56  // 保留

// 三级菜单项[7] :　检查记录查讯方式菜单
#define ITEM_CHECK_INQ_BY_METER_NO       0x57  // 按表号查询
#define ITEM_CHECK_INQ_BY_HUB_NO         0x58  // 按用户号查询
#define ITEM_VIEW_ALL_RECORD             0x59  // 按查看所有记录
#define ITEM_CHECK_INQ_ALL               0x5A  // 查看所有记录
#define VITEM_CHECK_REC_NO               0x5B  // 数据记录编号
#define ITEM_CHECK_RECORD_LIST           0x5C  // 检查记录列表
#define ITEM_READ_BY_METER_NO            0x5D  // 按表号读取水表数据
#define ITEM_READ_BY_HOUSE_NO            0x5E  // 按房间号读取水表数据
#define ITEM_READ_ALL_IN_HUB             0x5F  // 读取集中器下所有水表数据
#define ITEM_CHECK_INFO_BY_METER_NO      0x60  // 按表号查看水表信息
#define ITEM_CHECK_INFO_BY_HOUSE_NO      0x61  // 按房间号查看水表信息
#define ITEM_CHECK_INFO_ALL              0x62  // 查看集中器下所有子表信息


// 动态视图菜单项 
#define VITEM_DYNAMIC                    0x63  // 水表表号

// 视图菜单项[8] :　查看水表信息
#define VITEM_METER_NO                   0x64  // 子表编号
#define VITEM_SUB_STA                    0x65  // 子表状态
#define VITEM_CUSTOMER_ID                0x66  // 客户代码
#define VITEM_METER_DATE                 0x67  // 水表日期
#define VITEM_METER_TIME                 0x68  // 子表时间
#define VITEM_REC_AMOUNT                 0x69  // 充值水量
#define VITEM_REC_NUM                    0x6A  // 充值序次
#define VITEM_REM_AMOUNT                 0x6B  // 剩余水量
#define VITEM_TOTAL_AMOUNT               0x6C  // 累计水量
#define VITEM_VALVE_STA                  0x6D  // 阀门状态
#define VITEM_DISTURB_NUM                0x6E  // 干扰次数
#define VITEM_VALVE_FAULT_STA            0x6F  // 阀门故障
#define VITEM_SENSOR_STA                 0x70  // 传感器故障
#define VITEM_BATTERY_STA                0x71  // 电池电量
#define VITEM_REM_STA                    0x72  // 水量状态
#define VITEM_MEMORY_STA                 0x73  // 存储器
#define ITEM_RESERVE_42                  0x74  // 保留
#define ITEM_RESERVE_43                  0x75  // 保留
#define ITEM_RESERVE_44                  0x76  // 保留
#define ITEM_RESERVE_45                  0x77  // 保留
#define ITEM_RESERVE_46                  0x78  // 保留
#define ITEM_RESERVE_47                  0x79  // 保留


// 操作函数定义

#define FUNC_SET_HUB_INFO                0x00  // 设置集中器信息
#define FUNC_CHECK_HUB_INFO              0x01  // 设置集中器信息
#define FUNC_SET_HUB_TIME                0x02  // 设置集中器时间
#define FUNC_HUB_ADD_METER               0x03  // 集中器添加子表
#define FUNC_HUB_DEL_METER               0x04  // 集中器删除子表
#define FUNC_CHECK_SUB_INFO              0x05  // 查询子表信息
#define FUNC_HUB_SET_AUTO_DATE           0x06  // 设置集中器自动抄表日期
#define FUNC_HUB_CHECK_AUTO_DATE         0x07  // 查看集中器自动抄表日期
#define FUNC_READ_METER_NO               0x08  // 读水表表号
#define FUNC_INI_METER                   0x09  // 初始化水表
#define FUNC_SET_AVAILABLE               0x0A  // 水表出厂
#define FUNC_READ_METER_DATA             0x0B  // 读取水表数据
#define FUNC_SET_METER_NO                0x0C  // 设置表号函数
#define FUNC_SET_READOUT                 0x0D  // 设置水表读数
#define FUNC_SET_METER_TIME              0x0E  // 设置水表时钟函数
#define FUNC_CHECK_METER_INFO            0x0F  // 查看水表信息函数
#define FUNC_METER_FORCE_OPEN            0x10  // 强制打开阀门
#define FUNC_METER_FORCE_CLOSE           0x11  // 强制关闭阀门
#define FUNC_CANCEL_FORCE                0x12  // 取消强制状态
#define FUNC_MANTAIN                     0x13  // 阀门维护
#define FUNC_VIEW_REC_BY_METERNO         0x14  // 按表号查看水表记录
#define FUNC_INI_CHECK_RECORD_LIST       0x15  // 初始化查询记录列表
#define FUNC_UPDATE_CHECK_RECORD_LIST    0x16  // 更新检查数据列表
#define FUNC_INI_REFUND_RECORD_LIST      0x17  // 初始化退费记录列表
#define FUNC_UPDATE_REFUND_RECORD_LIST   0x18  // 更新检查数据列表
#define FUNC_VIEW_REFUND_RECORD          0x19  // 查看退费记录
#define FUNC_SET_HANDSET_CLOCK           0x1A  // 设置手持机时钟
#define FUNC_VIEW_HUB_REC                0x1B  // 查看集中器记录
#define FUNC_DEL_DATA_RECORD             0x1C  // 删除水表记录
#define FUNC_INI_MEMORY                  0x1D  // 初始化存储器
#define FUNC_DOWNLOAD_METER_DATA         0x1E  // 下载水表数据
#define FUNC_VIEW_REC_BY_HUBNO           0x1F  // 按集中器号查看水表记录
#define FUNC_VIEW_REC_BY_DATE            0x20  // 按日期查看水表记录
#define FUNC_SET_AUTO                    0x21  // 设置水表自动抄表日期
#define FUNC_CHECK_AUTO                  0x22  // 查看水表自动抄表日期
#define FUNC_READ_BY_METER_NO            0x0B  // 按表号读取水表数据
#define FUNC_READ_BY_HOUSE_NO            0x23  // 按房间号读取水表数据
#define FUNC_READ_ALL_IN_HUB             0x24  // 读取集中器所有子表数据
#define FUNC_SET_PRE_NO                  0x25  // 设置补充表号
#define FUNC_EMPTY_METER_DATA            0x26  // 清空水表数据
#define FUNC_VIEW_ALL_RECORD             0x27  // 查看所有水表记录
#define FUNC_CHECK_INFO_BY_METER_NO      0x28  // 按表号查看水表信息
#define FUNC_CHECK_INFO_BY_HOUSE_NO      0x29  // 按房间号查看水表信息
#define FUNC_CHECK_INFO_ALL              0x2A  // 查看集中器下所有子表信息
#define FUNC_VIEW_HANDSET_INFO           0x2B  // 查看手持机信息
#define FUNC_SHOW_HELP                   0x2C  // 查看帮助信息
#define FUNC_VIEW_ALL_HUB_REC            0x2D  // 查看所有集中器记录
#define FUNC_EDIT_HUB_RECORD             0x2E  // 编辑集中器信息
#define FUNC_DEL_HUB_RECORD              0x2F  // 删除集中器信息
#define FUNC_MAINTAIN_OPEN               0x30  // 阀门维护开，兼容旧表
#define FUNC_UPLOAD_RECORD               0x31  // 上传水表数据
#define FUNC_READ_STATUS                 0x32  // 读取水表状态信息
#define FUNC_UPDATE_FIRMWARE             0x33  // 更新固件程序
//extern char Current_Menu[MAX_ITEM+1][26]                                  ;
extern unsigned char (*Item_OP[ALL_ITEMS])(unsigned int,unsigned char)    ;
void str_cpy(char* Destination, char* Source,unsigned char Num)           ;

typedef  struct 
{
  unsigned char  ItemIndex                                             ; // 菜单项目索引
  unsigned char  SubMenuIndex                                          ; // 子菜单索引
  unsigned char  CallBackFuncIndex                                     ; // 回调函数索引
  unsigned char  AccessLevel                                           ; // 菜单项权限
  unsigned char  Config                                                ; // 菜单项类别
  char           ItemTitle[20]                                         ; // 菜单项名称
} ITEM                                                                 ;

typedef struct 
{
  unsigned char     MenuIndex                                          ; // 菜单索引 
  unsigned char     MasterMenu                                         ; // 父菜单索引  
  unsigned char     MasterItem                                         ; // 父菜单项索引
  unsigned char     MenuTitle[20]                                      ; // 菜单标题
  unsigned char     MenuConfig[40]                                     ; // 菜单配置
  unsigned char     ItemIndex[MAX_ITEM]                                ; // 菜单项索引
} MENU_RES                                                             ; // 菜单资源

//***************************MenuConfig 定义**********************************//
//   MenuConfig[0] ：菜单项数目
//   MenuConfig[1] ：中文字体大小
//   MenuConfig[2] ：西文字体大小
//   MenuConfig[3] ：每一菜单项所占用的Y轴的点数
//   MenuConfig[4] ：X轴固定偏移
//   MenuConfig[5] ：分页模式
//   MenuConfig[6] ：退出菜单调用函数(一般为视图菜单用以保存数据)
//   MenuConfig[7] ：分页函数索引
//   MenuConfig[8] ：菜单页数
//   MenuConfig[9] ：辅助参数，由应用函数定义
//         .                      .
//         .                      .
//         .                      .
//   MenuConfig[15]：多页菜单，菜单页数，供返回时使用
//****************************************************************************//

const ITEM MENU_ITEM[] = 
{
  
//--------------------------------- 主菜单[0] --------------------------------//
  
  {ITEM_HELP                    ,
   EMPTY                        ,
   FUNC_SHOW_HELP               ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "帮助信息"                    } ,  // [00]帮助
   
  {ITEM_METER_MANAGE            ,
   METER_MANAGE_MENU            ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "水表管理"                    } ,  // [01]水表管理
   
  {ITEM_DATA_MANAGE             ,
   DATA_MANAGE_MENU             ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "数据管理"                    } ,  // [02]数据管理
   
  {ITEM_HANDSET_MANAGE          ,
   HANDSET_MANAGE_MENU          ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "手持机管理"                  } ,  // [03]手持机管理
   
  {ITEM_HUB_MANAGE              ,
   HUB_MANAGE_MENU              ,   
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "集中器管理"                  } ,  // [04]集中器管理
     
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [05]保留

  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [06]保留

  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [07]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [08]保留
   
  
//------------------二级菜单项[1]：集中器管理菜单 ------------------------------//

  {ITEM_SET_HUB_INFO            ,
   EMPTY                        ,
   FUNC_SET_HUB_INFO            ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "设置集中器信息"              } ,  // [09]设置集中器信息
   
  {ITEM_CHECK_HUB_INFO          ,
   VIEW_HUB_INFO_MENU           ,
   FUNC_CHECK_HUB_INFO          ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "读取集中器信息"              } ,  // [0A]查看集中器信息
   
  {ITEM_DELETE_HUB_INFO         ,
   EMPTY                        ,
   FUNC_DEL_HUB_RECORD          ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "删除集中器信息"              } ,  // [0B]删除集中器信息
   
  {ITEM_ADD_METER_MANUAL        ,
   EMPTY                        ,
   FUNC_HUB_ADD_METER           ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "添加子表"                } ,  // [0C]手工添加子表
   
  {ITEM_ADD_METER_AUTO          ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "自动添加子表"                } ,  // [0D]自动添加子表
   
  {ITEM_HUB_DEL_METER           ,
   EMPTY                        ,
   FUNC_HUB_DEL_METER           ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "删除子表"                    } ,  // [0E]删除子表
   
  {ITEM_CHECK_SUB_INFO          ,
   EMPTY                        ,
   FUNC_CHECK_SUB_INFO          ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查看子表信息"                } ,  // [0F]查看水表信息
   
  {ITEM_SET_HUB_CLOCK           ,
   EMPTY                        ,
   FUNC_SET_HUB_TIME            ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "集中器校时"                  } ,  // [10]集中器校时
   
  {ITEM_HUB_SET_AUTO_DATE       ,
   EMPTY                        ,
   FUNC_HUB_SET_AUTO_DATE       ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "设置自动抄表"                } ,  // [11]设置自动抄表
   
  {ITEM_HUB_CHECK_AUTO_DATE     ,
   EMPTY                        ,
   FUNC_HUB_CHECK_AUTO_DATE     ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查看抄表设置"                } ,  // [12]查看抄表设置
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [13]保留

  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [14]保留

  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [15]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [16]保留
   
  
//------------------二级菜单项[2]：水表管理菜单 ------------------------------//

//  菜单项目索引                   子菜单索引            操作函数
  {ITEM_CHECK_METER_INFO        ,
   SELECT_INFO_INQ_MODE         ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查看水表信息"                } ,  // [17]查看水表信息
   
  {ITEM_METER_INIT              ,
   EMPTY                        ,
   FUNC_INI_METER               ,
   ITEM_ACCESS_SYSTEM           ,
   STATIC                       ,
   "水表初始化"                  } ,  // [18]水表初始化
   
  {ITEM_SET_METER_NO            ,
   EMPTY                        ,
   FUNC_SET_METER_NO            ,
   ITEM_ACCESS_SYSTEM           ,
   STATIC                       ,
   "设置水表表号"                } ,  // [19]设置水表表号
   
  {ITEM_READ_METER_NO           ,
   EMPTY                        ,
   FUNC_READ_METER_NO           ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "读取水表表号"                } ,  // [1A]读取水表表号
   
  {ITEM_SET_READOUT             ,
   EMPTY                        ,
   FUNC_SET_READOUT             ,
   ITEM_ACCESS_SYSTEM           ,
   STATIC                       ,
   "设置水表示数"                } ,  // [1B]设置水表示数
   
  {ITEM_SET_METER_TIME          ,  // 水表广播校时
   EMPTY                        ,
   FUNC_SET_METER_TIME          ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "水表广播校时"                } ,  // [1C]水表广播校时
   
  {ITEM_READ_METER_DATA         ,
   SELECT_DATA_READ_MODE        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "读取水表数据"                } ,  // [1D]读取水表数据
   
  {ITEM_SET_AVAILABLE           ,
   EMPTY                        ,
   FUNC_SET_AVAILABLE           ,
   ITEM_ACCESS_SYSTEM           ,
   STATIC                       ,
   "设置水表出厂"                } ,  // [22]设置水表出厂  
   
  {ITEM_ONE_KEY                 ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_HIDDEN           ,
   STATIC                       ,
   "水表一键出厂"                } ,  // [23]水表一键出厂 
   
  {ITEM_METER_FORCE_OPEN        ,
   EMPTY                        ,
   FUNC_METER_FORCE_OPEN        ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "强制打开阀门"                } ,  // [1E]强制打开阀门
   
  {ITEM_METER_FORCE_CLOSE       ,
   EMPTY                        ,
   FUNC_METER_FORCE_CLOSE       ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "强制阀门关闭"                } ,  // [1F]阀门强制关闭
   
  {ITEM_CANCEL_FORCE            ,
   EMPTY                        ,
   FUNC_CANCEL_FORCE            ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "取消强制状态"                } ,  // [20]取消强制状态
   
  {ITEM_MAINTAIN                ,
   EMPTY                        ,
   FUNC_MANTAIN                 ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "阀门维护"                    } ,  // [21]阀门维护  
   
  {ITEM_SET_AUTO                ,
   EMPTY                        ,
   FUNC_SET_AUTO                ,
   ITEM_ACCESS_OPERATOR          ,
   STATIC                       ,
   "设置抄表日期"                } ,  // [24]设置自动抄表
   
  {ITEM_CHECK_AUTO              ,
   EMPTY                        ,
   FUNC_CHECK_AUTO              ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查看抄表日期"                } ,  // [25]查看抄表设置
   
  {ITEM_PROGRAM                 ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_SYSTEM           ,
   STATIC                       ,
   "故障表统计"                  } ,  // [26]故障表统计
   
  {ITEM_PROGRAM                 ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_HIDDEN           ,
   STATIC                       ,
   "下载水表程序"                } ,  // [27]下载水表程序
   
  {ITEM_MAINTAIN_OPEN           ,
   EMPTY                        ,
   FUNC_MAINTAIN_OPEN           ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "维护开阀"                    } ,  // [28]阀门维护开，兼容旧表
   
  {ITEM_READ_STATUS             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "读取状态信息"                } ,  // [29]读取状态信息
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [2A]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [2B]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [2C]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [2D]保留
   

//---------------- 二级菜单项[3] : 数据管理菜单 -----------------------------//

  {ITEM_VIEW_DATA_RECORD        ,
   SELECT_DATA_INQ_MODE         ,
//   FUNC_VIEW_METER_RECORD       ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查询水表数据"                } ,  // [2E]查询水表数据
   
  {ITEM_UPLOAD_DATA_RECORD      ,
   EMPTY                        ,
   FUNC_UPLOAD_RECORD           ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "上传水表数据"                } ,  // [2F]上传水表数据
   
  {ITEM_DELETE_DATA_RECORD      ,
   EMPTY                        ,
   FUNC_DEL_DATA_RECORD         ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "删除水表数据"                } ,  // [30]删除水表数据
   
  {ITEM_VIEW_HUB_DATA_RECORD    ,
   VIEW_HUB_INFO_INQ_MODE       ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查看集中器信息"              } ,  // [31]查看集中器数据
   
  {ITEM_EDIT_HUB_DATA_RECORD    ,
   EMPTY                        ,
   FUNC_EDIT_HUB_RECORD         ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "编辑集中器信息"              } ,  // [32]保留
   
  {ITEM_DOWNLOAD_RECORD         ,
   EMPTY                        ,
   FUNC_DOWNLOAD_METER_DATA     ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "下载水表数据"                } ,  // [33]保留
   
  {ITEM_EMPTY_RECORD            ,
   EMPTY                        ,
   FUNC_EMPTY_METER_DATA        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "清空水表数据"                } ,  // [34]清空水表数据
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [35]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [36]保留
   

//---------------- 二级菜单项[4] : 手持机管理菜单 -------------------------------//

  {ITEM_HANDSET_INFO            ,
   EMPTY                        ,
   FUNC_VIEW_HANDSET_INFO       ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查看手持机信息"              } ,  // [37]查看手持机信息
   
  {ITEM_HANDSET_TIME            ,
   EMPTY                        ,   
   FUNC_SET_HANDSET_CLOCK       ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "设置手持机时间"              } ,  // [38]设置手持机时间
   
  {ITEM_HANDSET_LED             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "设置手持机背光"              } ,  // [39]设置手持机背光
   
  {ITEM_CREATE_USER             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "创建用户"                    } ,  // [3A]创建用户
   
  {ITEM_DELETE_USER             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "删除用户"                    } ,  // [3B]删除用户
   
  {ITEM_VIEW_LOG                ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR          ,
   STATIC                       ,
   "查看操作日志"                } ,  // [3C]查看操作日志
   
  {ITEM_UPLOAD_LOG              ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "上传操作日志"                } ,  // [3D]上传操作日志
   
  {ITEM_DELETE_LOG              ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_MANAGER          ,
   STATIC                       ,
   "清空操作日志"                } ,  // [3E]清空操作日志
   
  {ITEM_INI_MEMORY              ,
   EMPTY                        ,
   FUNC_INI_MEMORY              ,
   ITEM_ACCESS_SYSTEM           ,
   STATIC                       ,
   "初始化存储器"                } ,  // [3F]初始化存储器
   
  {ITEM_PORT_SET                ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "端口设置"                    } ,  // [40]端口设置
   
  {ITEM_SET_PRE_NO              ,
   EMPTY                        ,
   FUNC_SET_PRE_NO              ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "设置补充表号"                } ,  // [41]设置表号补足
   
  {ITEM_UPDATE_FIRMWARE         ,
   EMPTY                        ,
   FUNC_UPDATE_FIRMWARE         ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "更新固件程序"                } ,  // [42]更新固件程序
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [43]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [44]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [45]保留
    
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [46]保留
    
//----------------------二级菜单项[5]：帮助菜单 ------------------------------//
  {ITEM_HUB_INFO_BY_METER_NO    ,
   EMPTY                        ,
   FUNC_VIEW_HUB_REC            ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "按集中器号查询"              } ,  // [47]按集中器号查询
   
  {ITEM_HUB_INFO_ALL            ,
   EMPTY                        ,
   FUNC_VIEW_ALL_HUB_REC        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查看所有集中器"              } ,  // [48]查看所有集中器
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [49]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [4A]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [4B]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [4C]保留
    

//------------------二级菜单项[3]：水表故障统计菜单 ------------------------------//
  
  {ITEM_VALVE_FAULT             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "阀门故障"                    } ,  // [4D]阀门故障表统计
   
  {ITEM_LOW_POWER_FAULT         ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "低电故障表统计"              } ,  // [4E]低电故障表统计
   
  {ITEM_MEMORY_FAULT            ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "存储器故障"                  } ,  // [4F]存储器故障表统计
  
  {ITEM_SENSOR_FAULT            ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "传感器故障"                  } ,  // [50]传感器故障表统计
   
  {ITEM_OUTOF_WATER_FAULT       ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "低水量故障"                  } ,  // [51]低水量故障表统计
   
  {ITEM_ALL_FAULT               ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "所有故障"                    } ,  // [52]所有故障表统计
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [53]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [54]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [55]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [56]保留
       
   
//---------------- 三级菜单项[11] : 检查记录查询方式选择 ---------------------------//

  {ITEM_CHECK_INQ_BY_METER_NO   ,
   EMPTY                        ,
   FUNC_VIEW_REC_BY_METERNO     ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "按表号查询"                  } ,  // [57]按表号查询
   
  {ITEM_CHECK_INQ_BY_HUB_NO     ,
   EMPTY                        ,
   FUNC_VIEW_REC_BY_HUBNO       ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "按集中器号查询"              } ,  // [58]按用户号查询
   
  {ITEM_VIEW_ALL_RECORD         ,
   EMPTY                        ,
   FUNC_VIEW_ALL_RECORD         ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查询所有记录"                } ,  // [59]查询所有记录
   
  {ITEM_CHECK_INQ_ALL           ,
   CHECK_RECORD_LIST_MENU       ,
   FUNC_INI_CHECK_RECORD_LIST   ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查看所有记录"                } ,  // [5A]查看所有记录
   
  {VITEM_CHECK_REC_NO           ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "记录项菜单"                  } ,  // [5B]记录项
   
  {ITEM_CHECK_RECORD_LIST       ,
   VIEW_CHECK_RECORD_MENU       ,
//   FUNC_VIEW_CHECK_RECORD       ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "记录项菜单列表"              } ,  // [5C]记录项列表                    
          
  {ITEM_READ_BY_METER_NO        ,     // [5D]按表号读取水表数据
   EMPTY                        ,
   FUNC_READ_BY_METER_NO        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "按表号读取"                  } ,  
   
  {ITEM_READ_BY_HOUSE_NO        ,     // [5E]按房间号读取水表数据
   EMPTY                        ,
   FUNC_READ_BY_HOUSE_NO        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "按房间号读取"                } ,  
   
  {ITEM_READ_ALL_IN_HUB         ,     // [5F]读取集中器所有子表数据
   EMPTY                        ,
   FUNC_READ_ALL_IN_HUB         ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "读取所有子表"                } ,   
 
  {ITEM_CHECK_INFO_BY_METER_NO  ,
   EMPTY                        ,
   FUNC_CHECK_METER_INFO        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "按表号查询"                  } ,  // [60]按表号查询水表信息
   
  {ITEM_CHECK_INFO_BY_HOUSE_NO  ,
   EMPTY                        ,
   FUNC_CHECK_INFO_BY_HOUSE_NO  ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "按房间号查询"                } ,  // [61]按房间号查看水表信息
   
  {ITEM_CHECK_INFO_ALL          ,
   EMPTY                        ,
   FUNC_CHECK_INFO_ALL          ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "查看所有子表"                } ,  // [62]查看集中器下所有子表信息
   
  
//---------------- 视图菜单项[6] : 查看水表信息菜单(视图) -------------------------------//

  {VITEM_DYNAMIC                ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   DYNAMIC_VIEW                 ,
   ""                            } ,  // [63]动态视图菜单

  {VITEM_METER_NO               ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "子表编号"                    } ,  // [64]子表编号
   
  {VITEM_SUB_STA                ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "子表状态"                    } ,  // [65]子表状态
   
  {VITEM_CUSTOMER_ID            ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "客户代码"                    } ,  // [66]客户代码
   
  {VITEM_METER_DATE             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "水表日期"                    } ,  // [67]水表日期
   
  {VITEM_METER_TIME             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "子表时间"                    } ,  // [68]子表时间
   
  {VITEM_REC_AMOUNT             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "充值水量"                    } ,  // [69]充值水量
   
  {VITEM_REC_NUM                ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "充值序次"                    } ,  // [6A]充值序次
   
  {VITEM_REM_AMOUNT             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "剩余水量"                    } ,  // [6B]剩余水量
   
  {VITEM_TOTAL_AMOUNT           ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "累计水量"                    } ,  // [6C]累计水量
   
  {VITEM_VALVE_STA              ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "阀门状态"                    } ,  // [6D]阀门状态
   
  {VITEM_DISTURB_NUM            ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "干扰次数"                    } ,  // [6E]干扰次数  
   
  {VITEM_VALVE_FAULT_STA        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "阀门故障"                    } ,  // [6F]阀门故障
   
  {VITEM_SENSOR_STA             ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "传感器故障"                  } ,  // [70]传感器故障
   
  {VITEM_BATTERY_STA            ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "电池电量"                    } ,  // [71]电池电量
   
  {VITEM_REM_STA                ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "水量状态"                    } ,  // [72]水量状态 
   
  {VITEM_MEMORY_STA             ,
   EMPTY                        ,  
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "存储器状态"                  } ,  // [73]存储器状态  
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [5D]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [5E]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [5F]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [60]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [61]保留
   
  {EMPTY                        ,
   EMPTY                        ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "保留"                        } ,  // [62]保留
         
}                                                                       ;

const MENU_RES Menus[] =                                           // 管理员级设置菜单
{
//--------------------------- Menus[0]： 主菜单 -----------------------------------//  
  {
    MAIN_MENU                                      ,                         // 菜单索引
    EMPTY                                          ,
    EMPTY                                          ,
    "BW-DK5438"                                    ,
    {5,24,24,28,15,SINGLE_PAGE,EMPTY}              ,  
    {
      ITEM_HUB_MANAGE                              ,  // 集中器管理
      ITEM_METER_MANAGE                            ,  // 水表管理
      ITEM_DATA_MANAGE                             ,  // 数据管理
      ITEM_HANDSET_MANAGE                          ,  // 手持器管理
      ITEM_HELP                                    ,  // 水表数据管理
    }                                            
  }                                                ,
//--------------------------- Menus[1]： 集中器管理菜单 ---------------------------//    
  {
    HUB_MANAGE_MENU                                ,                         // 菜单索引
    MAIN_MENU                                      ,
    ITEM_HUB_MANAGE                                ,
    "集中器管理"                                   ,
    {8,24,24,28,15,SINGLE_PAGE,EMPTY}             ,      
    {
      ITEM_SET_HUB_INFO                            ,  // 设置集中器信息
      ITEM_CHECK_HUB_INFO                          ,  // 查看集中器信息
//      ITEM_DELETE_HUB_INFO                         ,  // 删除集中器信息
      ITEM_ADD_METER_MANUAL                        ,  // 手工添加子表
//      ITEM_ADD_METER_AUTO                          ,  // 自动添加子表
      ITEM_HUB_DEL_METER                           ,  // 删除子表
      ITEM_CHECK_SUB_INFO                          ,  // 查询子表信息
      ITEM_SET_HUB_CLOCK                           ,  // 集中器校时
      ITEM_HUB_SET_AUTO_DATE                       ,  // 设置自动抄表
      ITEM_HUB_CHECK_AUTO_DATE                     ,  // 查看自动抄表设置
    }
  }                                                ,    
//--------------------------- Menus[2]： 水表管理菜单 -----------------------------//   
  {
    METER_MANAGE_MENU                              ,                         // 菜单索引
    MAIN_MENU                                      ,
    ITEM_METER_MANAGE                              ,
    "水表管理"                                     ,
    {18,24,24,28,15,SINGLE_PAGE,EMPTY}              ,      
    {
      ITEM_CHECK_METER_INFO                        ,  // 查看水表信息
      ITEM_METER_INIT                              ,  // 水表初始化
      ITEM_SET_METER_NO                            ,  // 设置水表表号
      ITEM_READ_METER_NO                           ,  // 读取水表表号
      ITEM_SET_READOUT                             ,  // 设置水表示数
      ITEM_SET_METER_TIME                          ,  // 水表广播校时
      ITEM_READ_METER_DATA                         ,  // 读取水表数据
      ITEM_SET_AVAILABLE                           ,  // 设置水表出厂
      ITEM_ONE_KEY                                 ,  // 水表一键出厂
      ITEM_METER_FORCE_OPEN                        ,  // 强制打开阀门
      ITEM_METER_FORCE_CLOSE                       ,  // 强制关闭阀门
      ITEM_CANCEL_FORCE                            ,  // 取消强制状态
      ITEM_MAINTAIN                                ,  // 阀门维护
      ITEM_MAINTAIN_OPEN                           ,  // 阀门维护开，兼容旧表
      ITEM_SET_AUTO                                ,  // 设置结算日期
      ITEM_CHECK_AUTO                              ,  // 查看结算日期
      ITEM_PROGRAM                                 ,  // 下载水表程序
      ITEM_READ_STATUS                                // 读取状态信息
    }
  }                                                ,                               
//---------------------------Menus[3]：  数据管理菜单 ---------------------------//   
  {
    DATA_MANAGE_MENU                               ,                         // 菜单索引
    MAIN_MENU                                      ,
    ITEM_DATA_MANAGE                               ,
    "数据管理"                                     ,
    {8,24,24,28,15,SINGLE_PAGE,EMPTY}              ,      
    {
      ITEM_VIEW_HUB_DATA_RECORD                    ,    // 查看集中器信息
      ITEM_EDIT_HUB_DATA_RECORD                    ,    // 编辑集中器信息
      ITEM_DELETE_HUB_INFO                         ,    // 删除集中器信息
      ITEM_DOWNLOAD_RECORD                         ,    // 下载水表数据
      ITEM_VIEW_DATA_RECORD                        ,    // 查看水表数据 
      ITEM_UPLOAD_DATA_RECORD                      ,    // 上传水表数据
      ITEM_DELETE_DATA_RECORD                      ,    // 删除水表数据
      ITEM_EMPTY_RECORD                            ,    // 清空水表数据
    }
  }                                                ,                                 
//---------------------------Menus[4]： 手持机管理菜单  -------------------------//   
  {
    HANDSET_MANAGE_MENU                            ,    // 菜单索引
    MAIN_MENU                                      ,
    ITEM_HANDSET_MANAGE                            ,
    "手持机管理"                                   ,
    {5,24,24,28,15,SINGLE_PAGE,EMPTY}              ,      
    {
      ITEM_HANDSET_INFO                            ,    // 查看手持机信息
      ITEM_HANDSET_TIME                            ,    // 设置手持机时间
//      ITEM_HANDSET_LED                             ,    // 设置手持机背光
      ITEM_SET_PRE_NO                              ,    // 设置表号补足
      ITEM_INI_MEMORY                              ,    // 初始化存储器
      ITEM_UPDATE_FIRMWARE                         ,    // 更新固件程序
    }                                            
  }                                                ,
//---------------------------Menus[5]： 帮助菜单  -------------------------//   
  {
    HELP_MENU                                      ,    // 菜单索引
    MAIN_MENU                                      ,
    ITEM_HELP                                      ,
    "帮助"                                         ,
    {4,24,24,28,15,SINGLE_PAGE,EMPTY}              ,      
    {
      ITEM_HANDSET_INFO                            ,    // 查看手持机信息
      ITEM_HANDSET_TIME                            ,    // 设置手持机时间
      ITEM_HANDSET_LED                             ,    // 设置手持机背光
      ITEM_INI_MEMORY                              ,    // 初始化存储器
    }                                            
  }                                                ,  
//---------------------------Menus[6]：  故障统计菜单 ---------------------------//   
  {
    FAULT_STA_MENU                                 ,                         // 菜单索引
    METER_MANAGE_MENU                              ,
    ITEM_FAULT_STA                                 ,
    "故障表统计"                                   ,    
    {6,24,24,28,15,SINGLE_PAGE,EMPTY}              ,      
    {
      ITEM_VALVE_FAULT                             ,    // 阀门故障表统计 
      ITEM_LOW_POWER_FAULT                         ,    // 低电故障表统计
      ITEM_MEMORY_FAULT                            ,    // 存储器故障表统计
      ITEM_SENSOR_FAULT                            ,    // 传感器故障表统计
      ITEM_OUTOF_WATER_FAULT                       ,    // 低水量故障表统计
      ITEM_ALL_FAULT                                    // 所有故障表统计        
    }
  }                                                ,                                 
//---------------------------Menus[7]： 查看水表信息菜单  -----------------------//   
  {
    VIEW_INFO_MENU                                 ,    // 菜单索引
    METER_MANAGE_MENU                              ,
    ITEM_CHECK_METER_INFO                          ,
    "查看水表信息"                                 ,
//    {16,24,24,28,8,SINGLE_PAGE,FUNC_SAVE_CHECK_INFO} ,      
    {16,24,24,28,8,SINGLE_PAGE,EMPTY} ,      
    {
      VITEM_METER_NO                               ,    // 水表表号
      VITEM_SUB_STA                                ,    // 子表状态
      VITEM_CUSTOMER_ID                            ,    // 客户代码
      VITEM_METER_DATE                             ,    // 水表日期
      VITEM_METER_TIME                             ,    // 子表时间
      VITEM_REC_AMOUNT                             ,    // 充值水量
      VITEM_REC_NUM                                ,    // 充值序次
      VITEM_REM_AMOUNT                             ,    // 剩余水量
      VITEM_TOTAL_AMOUNT                           ,    // 累计水量
      VITEM_VALVE_STA                              ,    // 阀门状态
      VITEM_DISTURB_NUM                            ,    // 干扰次数  
      VITEM_VALVE_FAULT_STA                        ,    // 阀门故障
      VITEM_SENSOR_STA                             ,    // 传感器故障
      VITEM_BATTERY_STA                            ,    // 电池电量
      VITEM_REM_STA                                ,    // 水量状态 
      VITEM_MEMORY_STA                                  // 存储器状态
    }                                            
  }                                                ,                               
//---------------------------Menus[8]： 检查记录查询方式选择菜单  -----------------------//   
  {
    SELECT_DATA_INQ_MODE                           ,    // 菜单索引    
    DATA_MANAGE_MENU                               ,
    ITEM_VIEW_DATA_RECORD                          ,
    "查询记录"                                     ,
    {3,24,24,28,8,SINGLE_PAGE,EMPTY}               ,      
    {
      ITEM_CHECK_INQ_BY_METER_NO                   ,    // 按表号查询
      ITEM_CHECK_INQ_BY_HUB_NO                     ,    // 按用户号查询
      ITEM_VIEW_ALL_RECORD                         ,    // 按日期查询
      ITEM_CHECK_INQ_ALL                           ,    // 查看所有记录
    }
  }                                                ,
//---------------------------Menus[9]： 检查数据记录菜单  -----------------------//   
  {
    VIEW_CHECK_RECORD_MENU                         ,    // 菜单索引
    CHECK_RECORD_LIST_MENU                         ,
    ITEM_CHECK_RECORD_LIST                         ,
    "数据记录"                                     ,
    {17,24,24,28,8,SINGLE_PAGE,EMPTY}              ,      
    {
      VITEM_CHECK_REC_NO                           ,    // 记录编号
      VITEM_METER_NO                               ,    // 水表表号
      VITEM_SUB_STA                                ,    // 子表状态
      VITEM_CUSTOMER_ID                            ,    // 客户代码
      VITEM_METER_DATE                             ,    // 水表日期
      VITEM_METER_TIME                             ,    // 子表时间
      VITEM_REC_AMOUNT                             ,    // 充值水量
      VITEM_REC_NUM                                ,    // 充值序次
      VITEM_REM_AMOUNT                             ,    // 剩余水量
      VITEM_TOTAL_AMOUNT                           ,    // 累计水量
      VITEM_VALVE_STA                              ,    // 阀门状态
      VITEM_DISTURB_NUM                            ,    // 干扰次数  
      VITEM_VALVE_FAULT_STA                        ,    // 阀门故障
      VITEM_SENSOR_STA                             ,    // 传感器故障
      VITEM_BATTERY_STA                            ,    // 电池电量
      VITEM_REM_STA                                ,    // 水量状态 
      VITEM_MEMORY_STA                                  // 存储器状态
    }                                            
  }                                                ,                     
//---------------------------Menus[10]： 检查记录列表菜单  -----------------------//   
  {
    CHECK_RECORD_LIST_MENU                         ,    // 菜单索引
    SELECT_DATA_INQ_MODE                           ,
    ITEM_CHECK_INQ_BY_METER_NO                     ,
    {7,24,24,28,8,MULTI_PAGE,EMPTY,FUNC_UPDATE_CHECK_RECORD_LIST,0,0,0}  ,      
    {
      ITEM_CHECK_RECORD_LIST                       ,    // 检查记录列表
      ITEM_CHECK_RECORD_LIST                       ,   
      ITEM_CHECK_RECORD_LIST                       ,    
      ITEM_CHECK_RECORD_LIST                       ,    
      ITEM_CHECK_RECORD_LIST                       ,   
      ITEM_CHECK_RECORD_LIST                       ,  
      ITEM_CHECK_RECORD_LIST                       ,  
    }                                            
  }                                                ,      
//---------------------------Menus[11]： 动态视图菜单  ------------------------//
  {
    VIEW_HUB_INFO_MENU                             ,    // 菜单索引
    HUB_MANAGE_MENU                                ,
    ITEM_CHECK_HUB_INFO                            ,
    ""                                             ,
    {10,24,24,28,8,SINGLE_PAGE,EMPTY,EMPTY,0,0,0}  ,      
    {
      VITEM_DYNAMIC                                ,    // 检查记录列表
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,     
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,   
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,   
    }                                            
  }                                                ,      

//---------------------------Menus[12]： 动态视图菜单  ------------------------//
  {
    VIEW_METER_INFO_MENU                           ,    // 菜单索引
    METER_MANAGE_MENU                              ,
    ITEM_CHECK_METER_INFO                          ,
    ""                                             ,
    {10,24,24,28,8,SINGLE_PAGE,EMPTY,EMPTY,0,0,0}  ,      
    {
      VITEM_DYNAMIC                                ,    // 检查记录列表
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,     
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,   
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,   
    }                                            
  }                                                , 
//---------------------------Menus[13]： 动态视图菜单  ------------------------//
 {
    VIEW_HUB_REC_MENU                              ,    // 菜单索引
    DATA_MANAGE_MENU                               ,
    ITEM_VIEW_HUB_DATA_RECORD                      ,
    ""                                             ,
    {10,24,24,28,8,SINGLE_PAGE,EMPTY,EMPTY,0,0,0}  ,      
    {
      VITEM_DYNAMIC                                ,    // 检查记录列表
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,     
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,   
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,   
    }                                            
  }                                                ,      
//---------------------------Menus[14]： 水表数据读取方式选择菜单  -----------------------//   
  {
    SELECT_DATA_READ_MODE                          ,    // 菜单索引    
    METER_MANAGE_MENU                              ,
    ITEM_VIEW_DATA_RECORD                          ,
    "读取水表数据"                                 ,
    {3,24,24,28,8,SINGLE_PAGE,EMPTY}               ,      
    {
      ITEM_READ_BY_METER_NO                        ,    // 按表号读取
      ITEM_READ_BY_HOUSE_NO                        ,    // 按房间号查询
      ITEM_READ_ALL_IN_HUB                         ,    // 读取集中器所有子表数据
    }
  }                                                ,  
//---------------------------Menus[15]： 动态视图菜单  ------------------------//
 {
    VIEW_DOWNLOAD_RESULT_MENU                      ,    // 菜单索引
    DATA_MANAGE_MENU                               ,
    ITEM_DOWNLOAD_RECORD                           ,
    "下载完毕"                                     ,
    {20,24,24,28,8,SINGLE_PAGE,EMPTY,EMPTY,0,0,0}  ,      
    {
      VITEM_DYNAMIC                                ,    // 检查记录列表
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,     
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,   
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,   
      VITEM_DYNAMIC                                ,    // 检查记录列表
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,     
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
      VITEM_DYNAMIC                                ,    
    }                                            
  }                                                ,      
//---------------------------Menus[16]： 检查水表信息方式选择菜单  -----------------------//   
  {
    SELECT_INFO_INQ_MODE                           ,    // 菜单索引    
    METER_MANAGE_MENU                              ,
    ITEM_CHECK_METER_INFO                          ,
    "查看水表信息"                                 ,
    {2,24,24,28,8,SINGLE_PAGE,EMPTY}               ,      
    {
      ITEM_CHECK_INFO_BY_METER_NO                  ,    // 按表号查询
      ITEM_CHECK_INFO_BY_HOUSE_NO                  ,    // 按用户号查询
      ITEM_CHECK_INFO_ALL                          ,    // 按日期查询
    }
  }                                                ,
//---------------------------Menus[17]： 集中器信息查询方式选择菜单  -----------------------//   
  {
    VIEW_HUB_INFO_INQ_MODE                         ,    // 菜单索引    
    DATA_MANAGE_MENU                               ,
    ITEM_VIEW_HUB_DATA_RECORD                      ,
    "查看集中器信息"                               ,
    {2,24,24,28,8,SINGLE_PAGE,EMPTY}               ,      
    {
      ITEM_HUB_INFO_BY_METER_NO                    ,    // 按集中器号查询
      ITEM_HUB_INFO_ALL                            ,    // 查看所有集中器
    }
  }                                                ,
}                                                                     ;

