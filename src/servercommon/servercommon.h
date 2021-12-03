#ifndef __SERVERCOMMON_H__
#define __SERVERCOMMON_H__

// ������֮�乫�õ���ض��� 

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

// ���ɵ�½������������sessionkey
void GetSessionKey(const std::string &key_base, int scene_id, int scene_key, int last_scene_id, 
				   int role_id, GameName role_name, unsigned int time, char first_login, PlatName pname, SessionKey key);

// �����������������֤key
unsigned int GetChatLoginKey(int uid);

// �������id��Դ����
namespace chatdef
{
	enum ChannelType
	{
		CHANNEL_TYPE_WORLD = 0,
		CHANNEL_TYPE_CAMP,
		CHANNEL_TYPE_SCENE,
		CHANNEL_TYPE_TEAM,
		CHANNEL_TYPE_GUILD,
		CHANNEL_TYPE_SINGLE = 5,	// ˽��
		CHANNEL_TYPE_WORLD_QUESTION = 10,
		CHANNEL_TYPE_GUILD_QUESTION = 11,
		CHANNEL_TYPE_GUILD_SYSTEM = 12,

		CHANNEL_TYPE_MAX,
	};

	// ��Դ����
	enum ORIGIN_TYPE
	{
		ORIGIN_TYPE_NORMAL_CHAT = 0,		// ��ͨ����
		ORIGIN_TYPE_FALLING_ITEM = 1,		// ������Ʒ���Զ�������Ϣ
		ORIGIN_TYPE_GUILD_SYSTEM_CHAT = 2,		// ϵͳ��������������Ϣ
		ORIGIN_TYPE_GUILD_SYSTEM_MSG = 3,		// ϵͳ��������ϵͳ��Ϣ
	};

	enum ChannleIDBegin
	{
		CHANNEL_ID_WORLD_BEGIN = 0,				// ����Ƶ��idΪ0
		CHANNEL_ID_CAMP_BEGIN = 100,			// ��ӪƵ��idΪ100+��Ӫ���
		CHANNEL_ID_SCENE_BEGIN = 10000,			// ����Ƶ��idΪ10000 + ������id * 10000 + SceneIndex
		CHANNEL_ID_TEAM_BEGIN = 200000,			// �������idΪ20W + team_id
		CHANNEL_ID_GUILD_BEGIN = 1000000,		// ��������idΪ100W���� ��֤�������㹻��ID
	};
}

#define LL_ONE_BIT ((long long)0x0000000000000001)

static const int ROLE_RESET_MODULE_DATA_TM_HOUR = 6;
const static int DIANFENG_MIN_LEVEL = 370;						// �۷�ȼ��ķֽ��ߣ�����ȼ�֮�ϣ���Ϊ���۷�N������N = level - DIANFENG_MIN_LEVEL��
static const int MAX_ROLE_LEVEL = 1000;	
static const int MAX_ROLE_LEVEL_VERSION_LIMIT = 998;
static const int MAX_ROLE_FLOOR_LEVEL = MAX_ROLE_LEVEL / 10;
static const int MAX_ZHUAN_SHENG_LEVEL = 9;
static const int ZHUANSHENG_LEVEL_STEP = 100;

static const int DB_INDEX_MARK_BIT = 20;		// db_index �����2047 role_id��ൽ1048575
static const int MAX_DB_IDNEX = 2047;
static const int MAX_ROLE_ID = 1048575;

static const int TWO_BILLION = 2000000000;
static const int MAX_XIANHUN_VAL = 2000000000;	// �ɻ����ֵ 20��
static const int MAX_YUANLI_VAL = 2000000000;	// Ԫ�����ֵ 20��
static const int MAX_NVWASHI_VAL = 2000000000;	// Ů�ʯ���ֵ 20��
static const int MAX_CROSSHONOR_VAL = 2000000000;// ����������ֵ 20��

static const int SECOND_PER_DAY = 86400;		// һ����86400��
static const int SECOND_PER_HOUR = 3600;		// һСʱ��3600��
static const int SECOND_PER_MIN = 60;			// һ������60��

static const int DAY_PER_WEEK = 7;				// һ����7��

static const long long LONG_LONG_MAX_VALUE = 9223372036854775807;// long long ���ֵ 

static const int LUADATA_DB_PAGE_LENGTH = 4096;

static const int MAX_ROLE_GUILD_RED_PAPER = 20;		// ��ɫ���������ֵ
static const int MAX_GUILD_RED_PAPER = 100;			// ���������ֵ

static const int MAX_BUY_COUNT = 999;               // ���ƿͻ��˴������Ĺ������������ֵ

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
const char * ItemDataWrapperLog(const ItemDataWrapper *item_wrapper, int index = 0);		// ����ʹ����Ϊ�������� Ҫ������ С��LOG_MAX_ITEM_NUM
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
	SYS_MSG_ONLY_CHAT_WORLD = 0,													// ֻ��ӵ���������Ƶ��
	SYS_MSG_ONLY_CHAT_GUILD = 1,													// ֻ��ӵ���������Ƶ��ϵͳ
	SYS_MSG_CENTER_ROLL = 2,														// ��Ļ����, ��������
	SYS_MSG_CENTER_NOTICE = 3,														// ��Ļ����, ��������	
	SYS_MSG_ACTIVE_NOTICE = 4,														// ����棬ֻ�ڻ������������
	SYS_MSG_CENTER_PERSONAL_NOTICE = 5,												// ��Ļ����, ������Ϣ��������
	SYS_MSG_ONLY_WORLD_QUESTION = 6,												// ֻ��ӵ��������
	SYS_MSG_ONLY_GUILD_QUESTION = 7,												// ֻ��ӵ��������
	SYS_MSG_CENTER_NOTICE_2 = 8,												    // ��ӵ�ϵͳƵ��+��Ļ���뵯��
	SYS_MSG_EVENT_TYPE_COMMON_NOTICE = 9,											// ϵͳ��ͨ�¼�����
	SYS_MSG_EVENT_TYPE_SPECIAL_NOTICE = 10,											// ϵͳ�����¼�����(�м�ս�����ţ����ɵ���)
	SYS_MSG_CHAT_GUILD_PERSONAL = 11,												// ��ӵ���������Ƶ������	
	SYS_MSG_CENTER_NOTICE_NOT_CHAT = 12,											// ��Ļ����, �������ţ�����ӵ�����Ƶ��	
	SYS_MSG_ACTIVITY_SPECIAL = 13,													// ս���������ţ�ר�ÿ򵯳������·�������
	SYS_MSG_CENTER_ROLL_2 = 14,														// ��Ļ����, ��������(A�ഫ��)
	SYS_MSG_CENTER_NOTICE_3 = 15,													// ��Ļ����, ��������(B�ഫ��)
	SYS_MSG_TYPE_MAX
};
																				// ְҵ����
enum PROF_TYPE
{
	PROF_TYPE_INVALID = 0,														// ���� 
	
																				//        ��      -  ��
	PROF_TYPE_PROF_1,															// ְҵ1 ��ʿ(��) - ����
	PROF_TYPE_PROF_2,															// ְҵ2 ˫��(Ů) - �̿�
	PROF_TYPE_PROF_3,															// ְҵ3 ��ʦ(��) - ����
	PROF_TYPE_PROF_4,															// ְҵ4 ����(Ů) - ����
	PROF_TYPE_PROF_NO_LIMIT,													// �����ڴ�������ְҵ�����ܽӵ�����ȣ�

	PROF_TYPE_PROF_ZHUANZHI_BASE = 10,
	PROF_TYPE_PROF_ZHUANZHI_1 = PROF_TYPE_PROF_1 + PROF_TYPE_PROF_ZHUANZHI_BASE,// תְְҵ1
	PROF_TYPE_PROF_ZHUANZHI_2,													// תְְҵ2
	PROF_TYPE_PROF_ZHUANZHI_3,													// תְְҵ3
	PROF_TYPE_PROF_ZHUANZHI_4,													// תְְҵ4

	PROF_TYPE_PROF_SECOND_ZHUANZHI_1 = 21,										// 2��תְְҵ1
	PROF_TYPE_PROF_SECOND_ZHUANZHI_2,											// 2��תְְҵ2
	PROF_TYPE_PROF_SECOND_ZHUANZHI_3,											// 2��תְְҵ3
	PROF_TYPE_PROF_SECOND_ZHUANZHI_4,											// 2��תְְҵ4	

	PROF_TYPE_PROF_THIRD_ZHUANZHI_1 = 31,										// 3��תְְҵ1
	PROF_TYPE_PROF_THIRD_ZHUANZHI_2,											// 3��תְְҵ2
	PROF_TYPE_PROF_THIRD_ZHUANZHI_3,											// 3��תְְҵ3
	PROF_TYPE_PROF_THIRD_ZHUANZHI_4,											// 3��תְְҵ4

	PROF_TYPE_PROF_FOURTH_ZHUANZHI_1 = 41,										// 4��תְְҵ1
	PROF_TYPE_PROF_FOURTH_ZHUANZHI_2,											// 4��תְְҵ2
	PROF_TYPE_PROF_FOURTH_ZHUANZHI_3,											// 4��תְְҵ3
	PROF_TYPE_PROF_FOURTH_ZHUANZHI_4,											// 4��תְְҵ4

	PROF_TYPE_PROF_FIFTH_ZHUANZHI_1 = 51,										// 5��תְְҵ1
	PROF_TYPE_PROF_FIFTH_ZHUANZHI_2,											// 5��תְְҵ2
	PROF_TYPE_PROF_FIFTH_ZHUANZHI_3,											// 5��תְְҵ3
	PROF_TYPE_PROF_FIFTH_ZHUANZHI_4,											// 5��תְְҵ4

	PROF_TYPE_PROF_SIXTH_ZHUANZHI_1 = 61,										// 6��תְְҵ1
	PROF_TYPE_PROF_SIXTH_ZHUANZHI_2,											// 6��תְְҵ2
	PROF_TYPE_PROF_SIXTH_ZHUANZHI_3,											// 6��תְְҵ3
	PROF_TYPE_PROF_SIXTH_ZHUANZHI_4,											// 6��תְְҵ4

	PROF_TYPE_PROF_SEVENTH_ZHUANZHI_1 = 71,										// 7��תְְҵ1
	PROF_TYPE_PROF_SEVENTH_ZHUANZHI_2,											// 7��תְְҵ2
	PROF_TYPE_PROF_SEVENTH_ZHUANZHI_3,											// 7��תְְҵ3
	PROF_TYPE_PROF_SEVENTH_ZHUANZHI_4,											// 7��תְְҵ4

	PROF_TYPE_PROF_EIGHTH_ZHUANZHI_1 = 81,										// 8��תְְҵ1
	PROF_TYPE_PROF_EIGHTH_ZHUANZHI_2,											// 8��תְְҵ2
	PROF_TYPE_PROF_EIGHTH_ZHUANZHI_3,											// 8��תְְҵ3
	PROF_TYPE_PROF_EIGHTH_ZHUANZHI_4,											// 8��תְְҵ4

	PROF_TYPE_MAX,																
};

//���תְ�ȼ�
static const int MAX_PROF_LEVEL = (PROF_TYPE_MAX - PROF_TYPE_PROF_NO_LIMIT) / PROF_TYPE_PROF_ZHUANZHI_BASE;

const static int PROF_BASE_TYPE_MAX = PROF_TYPE_PROF_4;							// ����ְҵ���������
																				// ��ɫ��ɫ����
enum NAME_COLOR
{
	NAME_COLOR_WHITE = 0,														// ��ɫ
	NAME_COLOR_RED_1,															// ��ɫ1
	NAME_COLOR_RED_2,															// ��ɫ2
	NAME_COLOR_RED_3,															// ��ɫ3
	NAME_COLOR_MAX,
};

enum DISCONNECT_NOTICE_TYPE
{
	DISCONNECT_NOTICE_TYPE_INVALID,
	DISCONNECT_NOTICE_TYPE_LOGIN_OTHER_PLACE,									// ����ڱ𴦵�¼
	DISCONNECT_NOTICE_TYPE_CLIENT_REQ,											// �ͻ�������
};

enum ROLE_ONLINE_TYPE
{
	ROLE_ONLINE_TYPE_OFF = 0,		// ����
	ROLE_ONLINE_TYPE_ON = 1,		// ����
	ROLE_ONLINE_TYPE_CROSS = 2,		// ���
};

static const int BIRTH_MAX_HP_BASE = 200;										// ��ʼѪ��
static const int BIRTH_MAX_MP_BASE = 50;										// ��ʼħ��
static const int BIRTH_BAOJI_BASE = 5;											// ��ʼ����
static const int BIRTH_JIANREN_BASE = 10;										// ��ʼ����
static const int BIRTH_MOVESPEED_BASE = 900;									// ��ʼ�ٶ�

static const int BIRTH_FIRST_ATTR_LILIANG = 5;									// һ������ ����
static const int BIRTH_FIRST_ATTR_TIZHI = 5;									// һ������ ����
static const int BIRTH_FIRST_ATTR_JINGZHUN = 5;									// һ������ ��׼
static const int BIRTH_FIRST_ATTR_MINJIE = 5;									// һ������ ����

static const ItemID COIN_ITEM_ID = ItemID(-1);									// ͭ�ҵ���ƷID
static const ItemID GOLD_ITEM_ID = ItemID(-2);									// Ԫ������ƷID
static const ItemID BIND_GOLD_ITEM_ID = ItemID(-3);								// ��Ԫ������ƷID

static const int ZHENQI_VIRTUAL_ITEM_ID = 90072;								// ��Ԫ��ƷID��������Ʒ��

static const int BIT_COUNT_OF_BYTE = 8 * sizeof(unsigned char);					// ���ֽڴ�С
#define ALIGN_VALUE	4															// ����ֵ

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

// �ַ������� תlong long��Ϊ������LINUX�µľ���
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

// �����ַ������鿽��
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


// �ַ������ַ��и������
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

// �ַ�����ʽ����������
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

static const int MAX_TEAM_MEMBER_NUM = 3;						// �����������

enum TEAM_ASSIGN_MODE
{	
	TEAM_ASSIGN_MODE_INVALID = 0,
	TEAM_ASSIGN_MODE_ATTACK,									// ˭ɱ˭�÷���ģʽ
	TEAM_ASSIGN_MODE_RANDOM,									// �������ģʽ
	TEAM_ASSIGN_MODE_MAX
};

enum WEEKDAY
{
	SUNDAY = 0,													// ����
	MONDAY,														// ��һ
	TUESDAY,													// �ܶ�
	WEDNESDAY,													// ����
	THURSDAY,													// ����
	FRIDAY,														// ����
	SATURDAY													// ����
};

static const int CHANNEL_CD_TIME_WORLD = 10;					// ����Ƶ����ȴʱ�� s
static const int CHANNEL_CD_TIME_CAMP = 10;						// ����Ƶ����ȴʱ��
static const int CHANNEL_CD_TIME_SCENE = 10;					// ����Ƶ����ȴʱ��
static const int CHANNEL_CD_TIME_TEAM = 0;						// ���Ƶ����ȴʱ��
static const int CHANNEL_CD_TIME_GUILD = 0;						// ����Ƶ����ȴʱ��

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

	std::string ipstr;			// IP�ַ���
	unsigned short port;		// �˿�
};

struct ClockReturnDayItem // EngineAdapter��m_clock->DayID()�ķ���ֵ����
{
	unsigned short year;
	unsigned char  mon;
	unsigned char  day;
};

int CalcDayIndexWithClockDayItem(ClockReturnDayItem *day1, ClockReturnDayItem *day2); // ����day1��day2������������0����ͬһ�죬����Ϊ����

static const int PROBABILITY_TEN_MILLION = 10000000;			// ����ǧ��ֱȷ�ĸ
static const int PROBABILITY_TEN_THOUSAND = 10000;				// ������ֱȷ�ĸ
static const int PROBABILITY_HUNDRED = 100;						// ���ʰٷֱȷ�ĸ

// ��hhmm�뵱ǰʱ���ת��
UInt32 HHMMToNow(UInt32 zero_time, short hhmm);
short  NowToHHMM(UInt32 now_sec);

bool ConvertTimeStringToUnixTime2(const char *time_str, time_t *timestamp);
																	// �����鿽�������˾�̬���
#define ARRAYCPY(dst, src)\
do\
{\
	memcpy((dst), (src), sizeof(src));\
	UNSTD_STATIC_CHECK(sizeof(src) == sizeof(dst));\
	UNSTD_STATIC_CHECK(sizeof((src)[0]) == sizeof((dst)[0]));\
} while(0)


// ---------------------------------------------------------------------------------------------------------------
// DATA_PAGE
// Use for role_attr_detail10 ��
static const int DATA_PAGE_LEN = 1024 * 1024 * 1;		// Ŀǰ��ȡ BLOB �洢���������Գ���2MB����ϸ��� tabledefcreator ��Ŀ operator.cpp:314
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

