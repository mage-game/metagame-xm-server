#ifndef __SERVERCOMMON_H__
#define __SERVERCOMMON_H__

// 服务器之间公用的相关定义 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <string>
#include <limits.h>
#include <cassert>

#include "basedef.h"
#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "servercommon/logindef.h"

static const char *INTERNAL_NETWORK = "InternalNetwork";

namespace ggprotocal
{
	static const int MAX_GGPROTOCAL_LENGTH = 4096;
}

namespace crossgameprotocal
{
	static const int MAX_CROSSGAMEPROTOCAL_LENGTH = 4 * 1024 * 1024;
}

// 生成登陆场景服务器的sessionkey
void GetSessionKey(const std::string &key_base, int scene_id, int scene_key, int last_scene_id, 
				   int role_id, GameName role_name, unsigned int time, char first_login, PlatName pname, SessionKey key);

// 生成聊天服务器的验证key
unsigned int GetChatLoginKey(int uid);

// 聊天相关id资源划分
namespace chatdef
{
	enum ChannelType
	{
		CHANNEL_TYPE_WORLD = 0,
		CHANNEL_TYPE_CAMP,
		CHANNEL_TYPE_SCENE,
		CHANNEL_TYPE_TEAM,
		CHANNEL_TYPE_GUILD,
		CHANNEL_TYPE_SINGLE = 5,	// 私聊
		CHANNEL_TYPE_WORLD_QUESTION = 10,
		CHANNEL_TYPE_GUILD_QUESTION = 11,
		CHANNEL_TYPE_GUILD_SYSTEM = 12,

		CHANNEL_TYPE_MAX,
	};

	// 来源类型
	enum ORIGIN_TYPE
	{
		ORIGIN_TYPE_NORMAL_CHAT = 0,		// 普通聊天
		ORIGIN_TYPE_FALLING_ITEM = 1,		// 掉落物品后自动发的消息
		ORIGIN_TYPE_GUILD_SYSTEM_CHAT = 2,		// 系统发出仙盟聊天消息
		ORIGIN_TYPE_GUILD_SYSTEM_MSG = 3,		// 系统发出仙盟系统消息
	};

	enum ChannleIDBegin
	{
		CHANNEL_ID_WORLD_BEGIN = 0,				// 世界频道id为0
		CHANNEL_ID_CAMP_BEGIN = 100,			// 阵营频道id为100+阵营编号
		CHANNEL_ID_SCENE_BEGIN = 10000,			// 场景频道id为10000 + 服务器id * 10000 + SceneIndex
		CHANNEL_ID_TEAM_BEGIN = 200000,			// 组队聊天id为20W + team_id
		CHANNEL_ID_GUILD_BEGIN = 1000000,		// 军团聊天id为100W以上 保证给队伍足够的ID
	};
}

#define LL_ONE_BIT ((long long)0x0000000000000001)

static const int ROLE_RESET_MODULE_DATA_TM_HOUR = 6;
const static int DIANFENG_MIN_LEVEL = 370;						// 巅峰等级的分界线（这个等级之上，称为“巅峰N级”，N = level - DIANFENG_MIN_LEVEL）
static const int MAX_ROLE_LEVEL = 1000;	
static const int MAX_ROLE_LEVEL_VERSION_LIMIT = 998;
static const int MAX_ROLE_FLOOR_LEVEL = MAX_ROLE_LEVEL / 10;
static const int MAX_ZHUAN_SHENG_LEVEL = 9;
static const int ZHUANSHENG_LEVEL_STEP = 100;

static const int DB_INDEX_MARK_BIT = 20;		// db_index 最多是2047 role_id最多到1048575
static const int MAX_DB_IDNEX = 2047;
static const int MAX_ROLE_ID = 1048575;

static const int TWO_BILLION = 2000000000;
static const int MAX_XIANHUN_VAL = 2000000000;	// 仙魂最大值 20亿
static const int MAX_YUANLI_VAL = 2000000000;	// 元力最大值 20亿
static const int MAX_NVWASHI_VAL = 2000000000;	// 女娲石最大值 20亿
static const int MAX_CROSSHONOR_VAL = 2000000000;// 跨服荣誉最大值 20亿

static const int SECOND_PER_DAY = 86400;		// 一天有86400秒
static const int SECOND_PER_HOUR = 3600;		// 一小时有3600秒
static const int SECOND_PER_MIN = 60;			// 一分钟有60秒

static const int DAY_PER_WEEK = 7;				// 一周有7天

static const long long LONG_LONG_MAX_VALUE = 9223372036854775807;// long long 最大值 

static const int LUADATA_DB_PAGE_LENGTH = 4096;

static const int MAX_ROLE_GUILD_RED_PAPER = 20;		// 角色公会红包最大值
static const int MAX_GUILD_RED_PAPER = 100;			// 公会红包最大值

static const int MAX_BUY_COUNT = 999;               // 限制客户端传上来的购买数量的最大值

inline int UidToInt(const UserID &uid)
{
	int role_id = (uid.db_index << DB_INDEX_MARK_BIT)  + uid.role_id;
	return role_id;
}

inline UserID IntToUid(int role_id)
{
	int d = role_id >> DB_INDEX_MARK_BIT;
	int r = role_id - (d << DB_INDEX_MARK_BIT);
	return UserID(d, r);
}

inline long long TranslateInt64(int a, int b)
{
	return ((long long)(a) << 32) + b;
}

long long ConvertParamToLongLong(int param_0, int param_1);
long long ConvertParamToLongLong(int param_0, short param_1, short param_2);
bool HasInvalidChar(char *str);
void InvalidCharFilte(char *str);
void RoleNameCatPostfix(GameName rolename, short server);
void PlatNameCatPostfix(PlatName pname, short server);
void GuildNameCatPostfix(GuildName guildname, short server);
char * GetCurVersion(unsigned int unVer);
void PlatNameReCatPostfix(PlatName pname);

struct ItemDataWrapper;
struct SaleItemParam;
struct MailParam;
struct MailVirtualItemList;
struct ItemConfigData;

static const int LOG_MAX_ITEM_NUM = 8;
static const int LOG_BASE_LENGTH = 1024;

void ItemConfigDataListLog(std::string &log_str, int item_count, const ItemConfigData *item_cfglist);
const char * ItemConfigDataLog(const ItemConfigData *item_cfg, char log_str[LOG_BASE_LENGTH]);
const char * ItemDataWrapperLog(const ItemDataWrapper *item_wrapper, int index = 0);		// 连续使用作为函数参数 要传索引 小于LOG_MAX_ITEM_NUM
const char * ItemDataQuickLog(const ItemDataWrapper *item_wrapper);
const char * SaleItemParamLog(const SaleItemParam *param);
const char * MailParamLog(const MailParam *param);
const char * MailVirtualItemLog(const MailVirtualItemList *param);

int GetDayIndex(time_t begin_time, time_t t);
int GetWeekIndex(time_t begin_time, time_t t);
int GetMonthIndex(time_t begin_time, time_t t);
time_t GetZeroTime(time_t time_stamp);
unsigned int GetTodaySecond(unsigned int now_sec);

bool ConvertTimeStringToUnixTime(const char *time_str, time_t *timestamp);

int RandomNum(int max_num);
int RandomNum(int min_num, int max_num);

enum SYS_MSG_TYPE
{
	SYS_MSG_ONLY_CHAT_WORLD = 0,													// 只添加到聊天世界频道
	SYS_MSG_ONLY_CHAT_GUILD = 1,													// 只添加到聊天仙盟频道系统
	SYS_MSG_CENTER_ROLL = 2,														// 屏幕中央, 滚动播放
	SYS_MSG_CENTER_NOTICE = 3,														// 屏幕中央, 弹出播放	
	SYS_MSG_ACTIVE_NOTICE = 4,														// 活动公告，只在活动场景弹出播放
	SYS_MSG_CENTER_PERSONAL_NOTICE = 5,												// 屏幕中央, 个人信息弹出播放
	SYS_MSG_ONLY_WORLD_QUESTION = 6,												// 只添加到世界答题
	SYS_MSG_ONLY_GUILD_QUESTION = 7,												// 只添加到公会答题
	SYS_MSG_CENTER_NOTICE_2 = 8,												    // 添加到系统频道+屏幕中央弹出
	SYS_MSG_EVENT_TYPE_COMMON_NOTICE = 9,											// 系统普通事件提醒
	SYS_MSG_EVENT_TYPE_SPECIAL_NOTICE = 10,											// 系统特殊事件提醒(中间战场传闻，不可叠加)
	SYS_MSG_CHAT_GUILD_PERSONAL = 11,												// 添加到聊天仙盟频道个人	
	SYS_MSG_CENTER_NOTICE_NOT_CHAT = 12,											// 屏幕中央, 弹出播放，不添加到聊天频道	
	SYS_MSG_ACTIVITY_SPECIAL = 13,													// 战场播报传闻，专用框弹出（右下方弹出）
	SYS_MSG_CENTER_ROLL_2 = 14,														// 屏幕中央, 滚动播放(A类传闻)
	SYS_MSG_CENTER_NOTICE_3 = 15,													// 屏幕中央, 弹出播放(B类传闻)
	SYS_MSG_TYPE_MAX
};
																				// 职业定义
enum PROF_TYPE
{
	PROF_TYPE_INVALID = 0,														// 新手 
	
																				//        新      -  旧
	PROF_TYPE_PROF_1,															// 职业1 剑士(男) - 剑灵
	PROF_TYPE_PROF_2,															// 职业2 双剑(女) - 刺客
	PROF_TYPE_PROF_3,															// 职业3 琴师(男) - 笔仙
	PROF_TYPE_PROF_4,															// 职业4 铃铛(女) - 法尊
	PROF_TYPE_PROF_NO_LIMIT,													// 可用于代表所有职业都都能接的任务等！

	PROF_TYPE_PROF_ZHUANZHI_BASE = 10,
	PROF_TYPE_PROF_ZHUANZHI_1 = PROF_TYPE_PROF_1 + PROF_TYPE_PROF_ZHUANZHI_BASE,// 转职职业1
	PROF_TYPE_PROF_ZHUANZHI_2,													// 转职职业2
	PROF_TYPE_PROF_ZHUANZHI_3,													// 转职职业3
	PROF_TYPE_PROF_ZHUANZHI_4,													// 转职职业4

	PROF_TYPE_PROF_SECOND_ZHUANZHI_1 = 21,										// 2次转职职业1
	PROF_TYPE_PROF_SECOND_ZHUANZHI_2,											// 2次转职职业2
	PROF_TYPE_PROF_SECOND_ZHUANZHI_3,											// 2次转职职业3
	PROF_TYPE_PROF_SECOND_ZHUANZHI_4,											// 2次转职职业4	

	PROF_TYPE_PROF_THIRD_ZHUANZHI_1 = 31,										// 3次转职职业1
	PROF_TYPE_PROF_THIRD_ZHUANZHI_2,											// 3次转职职业2
	PROF_TYPE_PROF_THIRD_ZHUANZHI_3,											// 3次转职职业3
	PROF_TYPE_PROF_THIRD_ZHUANZHI_4,											// 3次转职职业4

	PROF_TYPE_PROF_FOURTH_ZHUANZHI_1 = 41,										// 4次转职职业1
	PROF_TYPE_PROF_FOURTH_ZHUANZHI_2,											// 4次转职职业2
	PROF_TYPE_PROF_FOURTH_ZHUANZHI_3,											// 4次转职职业3
	PROF_TYPE_PROF_FOURTH_ZHUANZHI_4,											// 4次转职职业4

	PROF_TYPE_PROF_FIFTH_ZHUANZHI_1 = 51,										// 5次转职职业1
	PROF_TYPE_PROF_FIFTH_ZHUANZHI_2,											// 5次转职职业2
	PROF_TYPE_PROF_FIFTH_ZHUANZHI_3,											// 5次转职职业3
	PROF_TYPE_PROF_FIFTH_ZHUANZHI_4,											// 5次转职职业4

	PROF_TYPE_PROF_SIXTH_ZHUANZHI_1 = 61,										// 6次转职职业1
	PROF_TYPE_PROF_SIXTH_ZHUANZHI_2,											// 6次转职职业2
	PROF_TYPE_PROF_SIXTH_ZHUANZHI_3,											// 6次转职职业3
	PROF_TYPE_PROF_SIXTH_ZHUANZHI_4,											// 6次转职职业4

	PROF_TYPE_PROF_SEVENTH_ZHUANZHI_1 = 71,										// 7次转职职业1
	PROF_TYPE_PROF_SEVENTH_ZHUANZHI_2,											// 7次转职职业2
	PROF_TYPE_PROF_SEVENTH_ZHUANZHI_3,											// 7次转职职业3
	PROF_TYPE_PROF_SEVENTH_ZHUANZHI_4,											// 7次转职职业4

	PROF_TYPE_PROF_EIGHTH_ZHUANZHI_1 = 81,										// 8次转职职业1
	PROF_TYPE_PROF_EIGHTH_ZHUANZHI_2,											// 8次转职职业2
	PROF_TYPE_PROF_EIGHTH_ZHUANZHI_3,											// 8次转职职业3
	PROF_TYPE_PROF_EIGHTH_ZHUANZHI_4,											// 8次转职职业4

	PROF_TYPE_MAX,																
};

//最大转职等级
static const int MAX_PROF_LEVEL = (PROF_TYPE_MAX - PROF_TYPE_PROF_NO_LIMIT) / PROF_TYPE_PROF_ZHUANZHI_BASE;

const static int PROF_BASE_TYPE_MAX = PROF_TYPE_PROF_4;							// 基础职业的最大类型
																				// 角色颜色定义
enum NAME_COLOR
{
	NAME_COLOR_WHITE = 0,														// 白色
	NAME_COLOR_RED_1,															// 红色1
	NAME_COLOR_RED_2,															// 红色2
	NAME_COLOR_RED_3,															// 红色3
	NAME_COLOR_MAX,
};

enum DISCONNECT_NOTICE_TYPE
{
	DISCONNECT_NOTICE_TYPE_INVALID,
	DISCONNECT_NOTICE_TYPE_LOGIN_OTHER_PLACE,									// 玩家在别处登录
	DISCONNECT_NOTICE_TYPE_CLIENT_REQ,											// 客户端请求
};

enum ROLE_ONLINE_TYPE
{
	ROLE_ONLINE_TYPE_OFF = 0,		// 离线
	ROLE_ONLINE_TYPE_ON = 1,		// 在线
	ROLE_ONLINE_TYPE_CROSS = 2,		// 跨服
};

static const int BIRTH_MAX_HP_BASE = 200;										// 初始血量
static const int BIRTH_MAX_MP_BASE = 50;										// 初始魔法
static const int BIRTH_BAOJI_BASE = 5;											// 初始暴击
static const int BIRTH_JIANREN_BASE = 10;										// 初始坚韧
static const int BIRTH_MOVESPEED_BASE = 900;									// 初始速度

static const int BIRTH_FIRST_ATTR_LILIANG = 5;									// 一级属性 力量
static const int BIRTH_FIRST_ATTR_TIZHI = 5;									// 一级属性 体质
static const int BIRTH_FIRST_ATTR_JINGZHUN = 5;									// 一级属性 精准
static const int BIRTH_FIRST_ATTR_MINJIE = 5;									// 一级属性 敏捷

static const ItemID COIN_ITEM_ID = ItemID(-1);									// 铜币的物品ID
static const ItemID GOLD_ITEM_ID = ItemID(-2);									// 元宝的物品ID
static const ItemID BIND_GOLD_ITEM_ID = ItemID(-3);								// 绑定元宝的物品ID

static const int ZHENQI_VIRTUAL_ITEM_ID = 90072;								// 精元物品ID（虚拟物品）

static const int BIT_COUNT_OF_BYTE = 8 * sizeof(unsigned char);					// 单字节大小
#define ALIGN_VALUE	4															// 对齐值

#define UNSTD_STATIC_CHECK(Expr)  				static_assert(Expr, "UNSTD_STATIC_CHECK");
//#define UNSTD_STATIC_CHECK(Expr)  				UNSTD_STATIC_CHECK1(Expr, __LINE__)
//#define UNSTD_STATIC_CHECK1(Expr, Line)			UNSTD_STATIC_CHECK2(Expr, Line)
//#define UNSTD_STATIC_CHECK2(Expr, Line)			typedef char UnName##Line[(Expr) ? 1 : -1];

#define UNSTD_OFFSETOFF(TYPE, MEMBER)			((size_t)((char *)&(((TYPE *)0x10)->MEMBER) - (char*)0x10))

#define ARRAY_ITEM_COUNT(a) (static_cast<int>(sizeof(a) / sizeof(*a)))

template <class T, std::size_t N>
constexpr std::size_t static_array_size(const T(&array)[N]) noexcept
{
	return N;
}

// 字符串拷贝 转long long是为了屏蔽LINUX下的警告
#define STRNCPY(dst_str, src_str, dst_len)                     \
{                                                              \
	if (NULL != (dst_str) && (int)(dst_len) > 0)               \
	{                                                          \
		if (NULL != (src_str) && ((long long)dst_str) != ((long long)src_str))       \
		{                                                      \
			strncpy((dst_str), (src_str), (dst_len) - 1);      \
			(dst_str)[(dst_len) - 1] = '\0';                   \
		}										               \
		else                                                   \
		{                                                      \
			(dst_str)[0] = '\0';                               \
		}                                                      \
	}                                                          \
}

// 定长字符串数组拷贝
#define F_STRNCPY(dst_str, src_str, len)                       \
{                                                              \
	if (NULL != (dst_str) && (int)(len) > 0)                   \
	{                                                          \
		if (NULL != (src_str) && (dst_str) != (src_str))       \
		{                                                      \
			memcpy((dst_str), (src_str), (len) - 1);           \
			(dst_str)[(len) - 1] = '\0';                       \
		}                                                      \
		else                                                   \
		{                                                      \
			(dst_str)[0] = '\0';                               \
		}                                                      \
	}                                                          \
}


// 字符串按字符切割成数组
#define F_STRSPLIT(str_list, o_str, pattern)				   \
{															   \
	int i = 20000;											   \
	int comma_n = 0;										   \
	do														   \
	{														   \
		std::string tmp_s = "";								   \
		comma_n = static_cast<int>(o_str.find(pattern));	   \
		if( -1 == comma_n )									   \
		{													   \
			tmp_s = o_str.substr( 0, o_str.length() );		   \
			str_list.push_back( tmp_s );					   \
			break;											   \
		}													   \
		tmp_s = o_str.substr( 0, comma_n );					   \
		o_str.erase( 0, comma_n+1 );						   \
		str_list.push_back( tmp_s );						   \
															   \
		i--;												   \
	}														   \
	while(i > 0);											   \
}

// 字符串格式化操作函数
inline int SNPRINTF(char *dst, int len, const char *format, ...)
{
	if (NULL == dst || len <= 0 || NULL == format) return -1;

	va_list va;
	va_start(va, format);

	int ret = vsnprintf(dst, len, format, va);
	if (ret >= len) ret = -1;

	va_end(va);

	dst[len - 1] = '\0';

	return ret;
}

#ifndef _WIN32
	#define USE_VAR(x)    ((void) &(x)) 
#else 
	#define USE_VAR(x) 
#endif 

#define USE_DATAACCESS_VAR()						\
inline void UseDataaccessVar()						\
{													\
	USE_VAR(dataaccessrmi::LOGIN);					\
	USE_VAR(dataaccessrmi::ROLE);					\
	USE_VAR(dataaccessrmi::SYSTEM);					\
	USE_VAR(dataaccessrmi::GLOBAL);					\
	USE_VAR(dataaccessrmi::CROSS);					\
}

#define USE_INTERNAL_NETWORK()						\
inline void UseInternalNetwork()					\
{													\
	USE_VAR(INTERNAL_NETWORK);						\
}		

#define OLD_USE_INTERNAL_NETWORK()

#define USE_DATAACCESS_ROLE_VAR()					\
	inline void UseDataaccessRoleVar()				\
{													\
	USE_VAR(dataaccessrmi::role::GET_ROLE_INFO);	\
	USE_VAR(dataaccessrmi::role::ROLE_INIT);		\
	USE_VAR(dataaccessrmi::role::ROLE_SAVE);		\
	USE_VAR(dataaccessrmi::role::CREATE_ROLE);		\
	USE_VAR(dataaccessrmi::role::DESTROY_ROLE);		\
	USE_VAR(dataaccessrmi::role::ROLE_SAVE_STATUS);	\
	USE_VAR(dataaccessrmi::role::FRIEND_SAVE);		\
	USE_VAR(dataaccessrmi::role::CREATE_CROSS_ROLE); \
	USE_VAR(dataaccessrmi::role::ROLE_QUERY);		\
}
		
#define USE_DATAACCESS_LOGIN_VAR()							\
	inline void UseDataaccessLoginVar()						\
{															\
	USE_VAR(dataaccessrmi::login::GET_PROF_NUM);			\
	USE_VAR(dataaccessrmi::login::USER_LOGIN);				\
	USE_VAR(dataaccessrmi::login::ADD_ROLE);				\
	USE_VAR(dataaccessrmi::login::REMOVE_ROLE);				\
	USE_VAR(dataaccessrmi::login::FORBID);					\
	USE_VAR(dataaccessrmi::login::GET_ROLE);				\
	USE_VAR(dataaccessrmi::login::CHANGE_ROLE);				\
	USE_VAR(dataaccessrmi::login::ADD_NAME_INFO);			\
	USE_VAR(dataaccessrmi::login::UPDATE_NAME_INFO);		\
	USE_VAR(dataaccessrmi::login::DELETE_NAME_INFO);		\
	USE_VAR(dataaccessrmi::login::DELETE_NAME_INFO_BY_NAME);\
	USE_VAR(dataaccessrmi::login::QUERY_NAME_EXIST);\
	USE_VAR(dataaccessrmi::login::RESET_ROLE_NAME);\
	USE_VAR(dataaccessrmi::login::INIT_WORLD_STATUS);\
	USE_VAR(dataaccessrmi::login::SAVE_WORLD_STATUS);\
}

USE_INTERNAL_NETWORK();

// ---------------------------------------------------------------------------------------------------------------

inline bool ItemCanMerge(ItemID item_id, int num, bool has_param, UInt32 invalid_time)
{
	return (0 != item_id) && (num > 0) && (num < 99) && (!has_param) && (0 == invalid_time);
}

static const int MAX_TEAM_MEMBER_NUM = 3;						// 队伍最大人数

enum TEAM_ASSIGN_MODE
{	
	TEAM_ASSIGN_MODE_INVALID = 0,
	TEAM_ASSIGN_MODE_ATTACK,									// 谁杀谁得分配模式
	TEAM_ASSIGN_MODE_RANDOM,									// 随机分配模式
	TEAM_ASSIGN_MODE_MAX
};

enum WEEKDAY
{
	SUNDAY = 0,													// 周日
	MONDAY,														// 周一
	TUESDAY,													// 周二
	WEDNESDAY,													// 周三
	THURSDAY,													// 周四
	FRIDAY,														// 周五
	SATURDAY													// 周六
};

static const int CHANNEL_CD_TIME_WORLD = 10;					// 世界频道冷却时间 s
static const int CHANNEL_CD_TIME_CAMP = 10;						// 国家频道冷却时间
static const int CHANNEL_CD_TIME_SCENE = 10;					// 场景频道冷却时间
static const int CHANNEL_CD_TIME_TEAM = 0;						// 组队频道冷却时间
static const int CHANNEL_CD_TIME_GUILD = 0;						// 军团频道冷却时间

UNSTD_STATIC_CHECK(LUADATA_DB_PAGE_LENGTH * 2 <= 8192);
// ---------------------------------------------------------------------------------------------------------------

namespace gamecommon
{
	const Posi GetDisRandPos(const Posi &curr_pos, int rand_dis);
}

// ---------------------------------------------------------------------------------------------------------------

struct NetAddress
{
	NetAddress(const char *_ipstr = "", unsigned short _port = 0) : ipstr(_ipstr), port(_port) {}

	bool Invalid() const { return (0 == port || ipstr.empty()); }
	bool operator==(const NetAddress &r) const { return (port == r.port && ipstr == r.ipstr); }
	bool operator!=(const NetAddress &r) const { return (port != r.port || ipstr != r.ipstr); }

	std::string ipstr;			// IP字符串
	unsigned short port;		// 端口
};

struct ClockReturnDayItem // EngineAdapter里m_clock->DayID()的返回值类型
{
	unsigned short year;
	unsigned char  mon;
	unsigned char  day;
};

int CalcDayIndexWithClockDayItem(ClockReturnDayItem *day1, ClockReturnDayItem *day2); // 计算day1到day2经过的天数，0代表同一天，可以为负数

static const int PROBABILITY_TEN_MILLION = 10000000;			// 概率千万分比分母
static const int PROBABILITY_TEN_THOUSAND = 10000;				// 概率万分比分母
static const int PROBABILITY_HUNDRED = 100;						// 概率百分比分母

// 把hhmm与当前时间戳转换
UInt32 HHMMToNow(UInt32 zero_time, short hhmm);
short  NowToHHMM(UInt32 now_sec);

bool ConvertTimeStringToUnixTime2(const char *time_str, time_t *timestamp);
																	// 简化数组拷贝，加了静态检查
#define ARRAYCPY(dst, src)\
do\
{\
	memcpy((dst), (src), sizeof(src));\
	UNSTD_STATIC_CHECK(sizeof(src) == sizeof(dst));\
	UNSTD_STATIC_CHECK(sizeof((src)[0]) == sizeof((dst)[0]));\
} while(0)


// ---------------------------------------------------------------------------------------------------------------
// DATA_PAGE
// Use for role_attr_detail10 表
static const int DATA_PAGE_LEN = 1024 * 1024 * 1;		// 目前采取 BLOB 存储，但不可以超过2MB，详细请见 tabledefcreator 项目 operator.cpp:314
static const int DATA_PAGE_LENGTH_HEX = DATA_PAGE_LEN * 2 + 1;
UNSTD_STATIC_CHECK(DATA_PAGE_LENGTH_HEX <= 1024 * 1024 * 2 + 1)

#pragma  pack(push, 4)
struct DataPageParam
{
	DataPageParam()
	{
		this->Reset();
	}

	void Reset()
	{
		len = 0;
		memset(data, 0, sizeof(data));
	}

	int len;
	char data[DATA_PAGE_LEN];
};
#pragma  pack(pop)


#endif // __SERVERCOMMON_H__

