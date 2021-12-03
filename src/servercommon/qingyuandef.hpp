#ifndef __QINGYUAN_DEF_HPP__
#define __QINGYUAN_DEF_HPP__

#include "servercommon.h"
#include "servercommon/struct/itemlistparam.h"

const static int QINGYUAN_FB_TEAM_MEMBER_COUNT = 2;				// 情缘队伍人数规定
UNSTD_STATIC_CHECK(QINGYUAN_FB_TEAM_MEMBER_COUNT == 2);

const static int QINGYUAN_SKIP_TO_LAST_WAVE_COUNT = 10;
const static int REWARD_QINGYUAN_VALUE_PER_MONSTER = 30;

const static int QINGYUAN_CARD_MAX_LEVEL = 12;					// 情缘卡牌最大等级
const static int QINGYUAN_CARD_MAX_ID = 19;						// 情缘卡牌最大卡号

const static int QINGYUAN_EQUIP_MAX_STAR = 10;					// 情缘装备最大星星数

const static int QINGYUAN_COUPLE_HALO_MAX_TYPE = 16;			// 夫妻光环类型最大值
const static int QINGYUAN_COUPLE_HALO_MAX_LEVEL = 360;			// 夫妻光环最大等级

const static int QINGYUAN_ADD_BLESS_DAYS_PER = 30;				// 蜜月祝福增加天数

const static int QINGYUAN_HUNLI_MAX_TYPE = 8;					// 婚礼类型个数最大值

const static int QINGYUAN_FB_MAX_REWARD_COUNT = 6;				// 情缘副本奖励最大数量

static const int MAX_ROLE_IN_YANHUI = 250;						// 宴会在场人数

const static int QINGYUAN_MARRY_MAX_LEVEL = 1000;				// 情缘结婚最大等级
const static int QINGYUAN_EQUIPSUIT_MAX_COUNT = 10;				// 情缘装备套装图鉴最大套数
const static int QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT = 8;			// 情缘装备套装图鉴每套最大个数
UNSTD_STATIC_CHECK(8 == QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT);

const static int QING_YUAN_SHENG_DI_TASK_MAX_COUNT = 6;			// 情缘圣地任务最大数量

const static int WEDDING_YUYUE_TIME_MAX = 48;					// 婚宴预约时间段最大个数
const static int WEDDING_GUESTS_MAX_NUM = 50;					// 婚宴宾客最大数(数据库、协议相关)
const static int WEDDING_MAX_LIVENESS_TYPE = 3;					// 婚礼热度最大类型
const static int WEDDING_BLESSING_MAX_CFG_NUM = 3;				// 婚礼类型最大配置数
const static int WEDDING_BLESSSING_MAX_RECORD_NUM = 10;			// 祝福记录最大数
const static int WEDDING_APPLICANT_MAX_NUM = 100;				// 申请者最大数

static const int HUNYAN_XUNYOU_POINT_MAX_NUM = 80;				// 结婚巡游最大点数量
static const int HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM = 10;			// 结婚巡游对象最大数量

const static int QINGYUAN_EQUIP2_MAX_LEVEL = 400;				// 情比金坚装备最大等级
const static int QINGYUAN_GIFT_MAX_COUNT = 8;					// 礼包最大档位

const static int RAND_PERFECT_LOVER_MAX_RECORD_COUPLE_COUNT = 50; // 完美情人最大对数


enum QINGYUAN_EQUIP_TYPE
{
	QINGYUAN_EQUIP_TYPE_1 = 0,
	QINGYUAN_EQUIP_TYPE_2,
	QINGYUAN_EQUIP_TYPE_3,
	QINGYUAN_EQUIP_TYPE_4,

	QINGYUAN_EQUIP_TYPE_MAX,
};

enum YUYUE_HUNYAN_TYPE
{
	YUYUE_HUNYAN_INVALUE = -1,
	YUYUE_HUNYAN_TYPE_0,										// 婚宴类型0(绑元，废弃)
	YUYUE_HUNYAN_TYPE_1,										// 婚宴类型1(元宝)
	YUYUE_HUNYAN_TYPE_2,										// 婚宴类型2(元宝)

	YUYUE_HUNYAN_TYPE_MAX,
};

enum QINGYUAN_MARRY_STATUS
{
	QINGYUAN_MARRY_STATUS_INVAILD = 0,							// 无效状态（结婚前）
	QINGYUAN_MARRY_STATUS_WEDDING_YUYUE,						// 预约状态（结婚后）
	QINGYUAN_MARRY_STATUS_INVITE_GUESTS,						// 准备状态（邀请宾客）
	QINGYUAN_MARRY_STATUS_WEDDING,								// 进行状态（活动开启时）

	QINGYUAN_MARRY_STATUS_MAX,
};

enum HUNYAN_ZHUFU_TYPE
{
	HUNYAN_ZHUFU_TYPE_RED_BAG = 0,								// 送红包 
	HUNYAN_ZHUFU_TYPE_GIVE_FLOWER,								// 送花
	HUNYAN_ZHUFU_TYPE_USE_YANHUA,								// 用烟花

	HUNYAN_ZHUFU_TYPE_MAX,
};

inline bool IsValidYuyueYanhuiType(int yanhui_type)
{
	return yanhui_type > YUYUE_HUNYAN_TYPE_0 && yanhui_type < YUYUE_HUNYAN_TYPE_MAX;
}

#pragma pack(push, 4)

// 情缘全局数据-------------------------------------------------------------

struct QingyuanEquip2Item
{
	QingyuanEquip2Item() { this->Reset(); }

	void Reset()
	{
		equip2_level = 0;
		reserve_sh = 0;
		reserve_int = 0;
	}

	unsigned short equip2_level;
	short reserve_sh;
	int reserve_int;
};

//特殊宝宝类型
enum SPECIAL_BABY_TYPE
{
	SPECIAL_BABY_TYPE_0,
	SPECIAL_BABY_TYPE_1,

	SPECIAL_BABY_TYPE_MAX,
};

// 特殊宝宝品质
enum SPECIAL_BABY_QUALITY
{
	SPECIAL_BABY_QUALITY_VALID,

	SPECIAL_BABY_QUALITY_1,
	SPECIAL_BABY_QUALITY_2,
	SPECIAL_BABY_QUALITY_3,
	SPECIAL_BABY_QUALITY_4,
	SPECIAL_BABY_QUALITY_5,
	SPECIAL_BABY_QUALITY_6,
	SPECIAL_BABY_QUALITY_7,
	SPECIAL_BABY_QUALITY_8,
	SPECIAL_BABY_QUALITY_9,
	SPECIAL_BABY_QUALITY_10,

	SPECIAL_BABY_QUALITY_MAX,
};

struct QingyuanSpecialBaby
{
	QingyuanSpecialBaby() { this->Reset(); }

	void Reset()
	{
		special_baby_level = 0;
		quality = 0;
		reserve_sh = 0;
	}

	unsigned short special_baby_level;
	char quality;
	char reserve_sh;
};

struct QingyuanGlobalParam
{
	QingyuanGlobalParam() { this->Reset(); }

	void Reset()
	{
		lover_id = -1;
		marry_level = 0;
		marry_level_exp = 0;
		today_buy_gift_flag = 0;
		is_add_profess_share_per = 0;
		fight_out_couple_halo_type = -1;
		reserve_ch = 0;

		memset(qingyuan_suit_flag, 0, sizeof(qingyuan_suit_flag));
		is_all_task_complete = 0;

		for (int i = 0; i < QINGYUAN_EQUIP_TYPE_MAX; i++)
		{
			qingyuan_equip_list[i].Reset();
		}

		profess_level = 0;
		reserve_sh1 = 0;
		profess_exp = 0;

		for (int i = 0; i < SPECIAL_BABY_TYPE_MAX; i ++)
		{
			special_baby_list[i].Reset();
		}
	}

	int lover_id;
	int marry_level;																// 结婚等级
	int marry_level_exp;															// 结婚等级经验
	char today_buy_gift_flag;														// 今日购买礼包标记
	char is_add_profess_share_per;													// 是否有表白属性共享加成
	char fight_out_couple_halo_type;												// 出战夫妻光环
	char reserve_ch;

	unsigned char qingyuan_suit_flag[QINGYUAN_EQUIPSUIT_MAX_COUNT];					// 结婚装备套装激活标记位
	short is_all_task_complete;

	ItemDataWrapper qingyuan_equip_list[QINGYUAN_EQUIP_TYPE_MAX];
	QingyuanEquip2Item qingyuan_equip2_list[QINGYUAN_EQUIP_TYPE_MAX];				// 情比金坚装备

	unsigned short profess_level;													// 表白等级
	short reserve_sh1;
	unsigned int profess_exp;														// 表白经验

	QingyuanSpecialBaby special_baby_list[SPECIAL_BABY_TYPE_MAX];					// 特殊宝宝信息
	
};
typedef char QingyuanGlobalParamHex[sizeof(QingyuanGlobalParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(QingyuanGlobalParamHex) < 2048);

struct QingYuanShengDiTask
{
	QingYuanShengDiTask() : task_id(0), is_fetched_reward(0), reserve(0), param(0) {}

	unsigned short task_id;
	char is_fetched_reward;
	char reserve;
	int param;
};

struct WeddingBlessingRecord
{
	WeddingBlessingRecord() : bless_type(0), param(0), timestamp(0)
	{
		memset(role_name, 0, sizeof(role_name));
		memset(to_role_name, 0, sizeof(to_role_name));
	}

	GameName role_name;
	GameName to_role_name;
	int bless_type;									// 祝福类型 0: 红包 1:送花 2: 用烟花 HUNYAN_ZHUFU_TYPE
	int param;										// bless_type为2时，param为主动物品配置param1
	unsigned int timestamp;							// 当前时间戳
};

struct WeddingBlessingRecordInfo
{
	WeddingBlessingRecordInfo() : record_count(0), record_cur_index(0) {}

	int record_count;
	int record_cur_index;
	WeddingBlessingRecord bless_record_list[WEDDING_BLESSSING_MAX_RECORD_NUM];
};

struct WeddingApplicantInfo
{
	WeddingApplicantInfo() { this->Reset(); }

	void Reset()
	{
		user_id = 0;
		memset(user_name, 0, sizeof(GameName));
	}

	int user_id;
	GameName user_name;
};

//情缘数据-----------------------------------------------------------------------------------
struct QingyuanParam
{
	QingyuanParam() { this->Reset(); }

	void Reset()
	{
		join_fb_times = 0;
		is_skip_fb_waves = 0;
		is_hunyan_fb_already_open = 0;
		can_open_hunyan = 0;
		qingyuan_value = 0;
		is_fetch_bless_reward = 0;
		today_hunjie_uplevel_times = 0;
		marry_req_uid = 0;
		bless_days = 0;
		is_first_diamond_yanhui = 0;
		today_hunyan_open_times = 0;
		is_hunyan_fb_open = 0;
		reserve_int = 0;
		yanhui_fb_key = 0;
		marry_count = 0;

		today_yanhui_get_diamonds = 0;
		today_yanhui_get_hunjie_stuffs = 0;
		today_yanhui_get_other_stuffs = 0;
		today_yanhui_get_exps = 0;
		today_hunyan_gather_day_max_stuff_num = 0;
		today_hunyan_join_times = 0;
		today_saxianhua_times = 0;

		hunjie_cur_exp = 0;
		today_tuodan_times = 0;

		memset(card_level_list, 0, sizeof(card_level_list));

		equiped_couple_halo_type = -1;
		cur_tejia_halo_type = 0;
		reserve_sh = 0;
		memset(couple_halo_level_list, 0, sizeof(couple_halo_level_list));
		memset(couple_halo_exp_list, 0, sizeof(couple_halo_exp_list));

		is_fetched_task_other_reward = 0;
		task_info_count = 0;
		memset(task_info_list, 0, sizeof(task_info_list));

		tejie_halo_invalid_time = 0;
		memset(reserve_char_list, 0, sizeof(reserve_char_list));

		today_hunyan_be_zhufu_max_stuff_num = 0;
		today_hunyan_zhufu_max_stuff_num = 0;

		hunyan_type = -1;
		is_today_marry_req_succ = 0;
		is_have_hunyan_times = 0;
		cur_marry_stauts = 0;
		wedding_yuyue_seq = -1;
		marry_type = 0;
		has_marry_huili_type_flag = 0;
		today_xunyou_gather_hongbao_count = 0;
		wedding_index = 0;

		today_xunyou_sa_hongbao_count = 0;
		today_xunyou_sa_hongbao_buy_count = 0;
		today_xunyou_give_flower_count = 0;
		today_xunyou_give_flower_buy_count = 0;
	}

	char join_fb_times;
	char is_skip_fb_waves;
	char is_hunyan_fb_already_open;				// 如果婚宴已经举办=1 否则为0
	char can_open_hunyan;
	int qingyuan_value;
	char is_fetch_bless_reward;
	char is_first_diamond_yanhui;				// 是否是第一次钻石宴会（婚宴）
	char today_hunyan_open_times;				// 当天举行的婚宴次数
	char is_hunyan_fb_open;						// 开启的是否为婚宴
	int reserve_int;							// 保留
	int yanhui_fb_key;							// 宴会副本key
	int marry_count;							// 结婚次数

	int marry_req_uid;							// 求婚玩家uid
	int bless_days;								// 蜜月祝福剩余天数

	int today_yanhui_get_diamonds;
	int today_yanhui_get_hunjie_stuffs;
	int today_yanhui_get_other_stuffs;
	int today_yanhui_get_exps;
	int today_hunyan_gather_day_max_stuff_num;												// 当天婚宴采集次数
	int today_hunyan_join_times;															// 当天参加婚宴次数
	int today_saxianhua_times;																//当天撒鲜花次数

	int today_hunjie_uplevel_times;															// 婚戒当天升级次数
	int hunjie_cur_exp;																		// 婚戒星级当前经验
	int today_tuodan_times;																	// 我要脱单当天的发布次数

	UNSTD_STATIC_CHECK(19 == QINGYUAN_CARD_MAX_ID);
	char card_level_list[QINGYUAN_CARD_MAX_ID + 1];											// 情缘卡牌等级列表

	char equiped_couple_halo_type;															// 已装备的夫妻光环
	char cur_tejia_halo_type;																// 当前特价光环类型
	short reserve_sh;																		// 									
	UNSTD_STATIC_CHECK(QINGYUAN_COUPLE_HALO_MAX_TYPE == 16);
	short couple_halo_level_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];							// 夫妻光环等级列表
	short couple_halo_exp_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];								// 夫妻光环经验列表

	short is_fetched_task_other_reward;														// 是否领取情缘圣地额外奖励
	short task_info_count;
	QingYuanShengDiTask task_info_list[QING_YUAN_SHENG_DI_TASK_MAX_COUNT];					// 情缘圣地任务

	unsigned int tejie_halo_invalid_time;													// 特价光环失效时间				
	char reserve_char_list[1604];															// 保留

	int today_hunyan_be_zhufu_max_stuff_num;												// 当天被祝福累计获得材料数量
	int today_hunyan_zhufu_max_stuff_num;													// 当天祝福累计获得材料数量

	//-------------------------------------------------------------------------------------------
	char hunyan_type;																		// 婚宴类型
	char is_today_marry_req_succ;															// 今日求婚是否成功
	char is_have_hunyan_times;																// 是否有婚宴次数
	char cur_marry_stauts;																	// 当前结婚进度
	char wedding_yuyue_seq;																	// 预约婚礼下标
	char marry_type;																		// 结婚类型(婚宴特效用,多次求婚不变)
	char has_marry_huili_type_flag;															// 已经求过婚的结婚类型
	char today_xunyou_gather_hongbao_count;													// 巡游-当天采集红包次数
	int wedding_index;																		// 全服第几对情侣
	UInt8 today_xunyou_sa_hongbao_count;													// 巡游-当天撒红包次数
	UInt8 today_xunyou_sa_hongbao_buy_count;												// 巡游-当天撒红包购买次数
	UInt8 today_xunyou_give_flower_count;													// 巡游-当天送花次数
	UInt8 today_xunyou_give_flower_buy_count;												// 巡游-当天送花购买次数
};

typedef char QingyuanParamHex[sizeof(QingyuanParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(QingyuanParamHex) < 4096);  

//表白个人数据--------------------------------------------------------------------------------
typedef char ProfessContent[64];

const static int PERSON_PROFESS_MAX_NUM = 12;					// 表白墙个人表白最大数

enum PROFESS_GIFT_TYPE
{
	PROFESS_GIFT_TYPE_CHAMPAGNE = 0,							// 香槟 
	PROFESS_GIFT_TYPE_HOUSEBOAT,								// 游艇
	PROFESS_GIFT_TYPE_ROCKET,									// 火箭

	PROFESS_GIFT_TYPE_MAX,
};

enum PROFESS_WALL_TYPE
{
	PROFESS_WALL_TYPE_SELF = 0,		// 我表白的
	PROFESS_WALL_TYPE_OTHER = 1,	// 表白我的
	PROFESS_WALL_TYPE_GLOBAL = 2,	// 公共表白墙
	PROFESS_WALL_TYPE_MAX
};

struct PersonProfessItem
{
	PersonProfessItem() { this->Reset(); }

	void Reset()
	{
		other_role_id = 0;
		profess_time = 0;
		gift_type = 0;
		memset(content, 0, sizeof(content));
	}

	int				other_role_id;			// 对方id
	unsigned int 	profess_time;			// 表白时间
	int				gift_type;				// 礼物类型
	ProfessContent	content;				// 表白内容
};

struct PersonProfessParam
{
	PersonProfessParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < PERSON_PROFESS_MAX_NUM; i ++)
		{
			profess_to_items[i].Reset();
			profess_from_items[i].Reset();
		}
	}
	PersonProfessItem profess_to_items[PERSON_PROFESS_MAX_NUM];		// 我表白的
	PersonProfessItem profess_from_items[PERSON_PROFESS_MAX_NUM];	// 表白我的
};

typedef char PersonProfessParamHex[sizeof(PersonProfessParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PersonProfessParamHex) < 4096);

#pragma pack(pop)

#endif  // __QINGYUAN_DEF_HPP__

