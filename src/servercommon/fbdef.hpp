#ifndef __FB_DEF_HPP__
#define __FB_DEF_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push, 4)

//////////////////////////////////////////////////////////  װ������  //////////////////////////////////////////////////////////
static const int FB_EQUIP_MAX_MYSTERYLAYER_NUM = 16;																// ���ز�����
static const int FB_EQUIP_MAX_GOODS_NUM_PER_MYSTERYLAYER = 6;														// ÿ�����ر�������
static const int FB_EQUIP_MAX_LAYER_ID = 71;																		// �����
static const int FB_EQUIP_MAX_GOODS_SEQ = FB_EQUIP_MAX_MYSTERYLAYER_NUM * FB_EQUIP_MAX_GOODS_NUM_PER_MYSTERYLAYER;	// ������ز���Ʒ���

//////////////////////////////////////////////////////////  ������������  //////////////////////////////////////////////////////////
static const int AUTO_FB_FREE_WITH_LEVEL = 70;																		// ָ���ȼ����ϣ�����ɨ�����

struct EquipFBInfo
{
	enum EFB_FLAG
	{
		EFB_FLAG_DOUBLE_DROP = 0x1,	
	};

	EquipFBInfo() { this->Reset(); }

	void Reset()
	{
		max_layer_today_entered = 0;
		max_passed_real_layer = 0;
		flag = 0;
		max_layer_today_pass = 0;
		today_multiple_buy_times = 0;
		member_min_layer = 0;
		can_give_prelayer_reward = 1;
		is_record_today_max_pass_level = 1;
		memset(mysterylayer_list, 0, sizeof(mysterylayer_list));
		reserve_sh_1 = 0;
		reserve_sh_2 = 0;
	}

	void DayReset()
	{
		max_layer_today_entered = 0;
		flag = 0;
		max_layer_today_pass = 0;
		today_multiple_buy_times = 0;
		is_record_today_max_pass_level = 1;
		memset(mysterylayer_list, 0, sizeof(mysterylayer_list));
	}

	char max_layer_today_entered;																					// ��������������
	char max_passed_real_layer;																						// ͨ�ع�����߲㣬���������ز�
	short flag;																										// ��λ��EFB_FLAG
	char mysterylayer_list[FB_EQUIP_MAX_GOODS_SEQ];																	// ���ز���Ʒ�����¼
	char max_layer_today_pass;																						// �������ͨ�ز���
	char today_multiple_buy_times;																					// ����౶�����������
	char member_min_layer;																							// ��ӳ�Ա����С��ʷͨ����
	char can_give_prelayer_reward;																					// �ܷ�����ʷ���ͨ�ز㽱�������븱��ʱ�����������Сͨ����ȷ������������
	char is_record_today_max_pass_level;																			// �Ƿ��¼�������ͨ�ز����������౶�������߻����󣬸��ӽ���İ������һ�β��븱����ͨ�ز�����������
	char reserve_sh_1;
	short reserve_sh_2;
};

struct EquipFBParam
{
	EquipFBParam() { this->Reset(); }

	void Reset()
	{
		personal_equipfb_info.Reset();
		team_equipfb_info.Reset();
	}

	EquipFBInfo personal_equipfb_info;																				// ����װ������Ϣ
	EquipFBInfo team_equipfb_info;																					// ���װ������Ϣ
};

typedef char EquipFBParamHex[sizeof(EquipFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(EquipFBParamHex) < 512);

//////////////////////////////////////////////////////////  �ɻ���  //////////////////////////////////////////////////////////
static const int MAX_FBXHT_NORMAL_XIANHUNTA_LAYER = 80;																// �ɻ�����ͨ���ؿ�
static const int MAX_FBXHT_JINGYING_XIANHUNTA_LAYER = 7;															// �ɻ�����Ӣ���ؿ�
static const int MAX_FBXHT_REWARD_ITEM_COUNT = 3;																	// �ؿ��������Ʒ����

enum EN_FBXIANHUNTA_TYPE
{
	EN_FBXIANHUNTA_TYPE_NORMAL = 1,																					// ��ͨ�ɻ���
	EN_FBXIANHUNTA_TYPE_JINGYING,																					// ��Ӣ�ɻ���
};

struct XianhuntaFBPassNewsItem
{
	XianhuntaFBPassNewsItem() : news_id(0), uid(0), capability(0), pass_time(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int news_id;																					// ��¼id��Խ��Խ��
	int uid;																						// uid
	GameName user_name;																				// ��ɫ��
	int capability;																					// ս����
	int pass_time;																					// ͨ��ʱ��
};

struct XianhuntaFBParam
{
	XianhuntaFBParam() { this->Reset(); }

	void Reset()
	{
		passed_normal_level = 0; passed_jingying_level = 0;
		is_fetched_reward = 0;
		left_xianhun = 0; left_reward_item_count = 0;
		today_normal_fail_times = 0; today_jingying_fail_times = 0;
		normal_enterable_time = 0; jingying_enterable_time = 0;
	}

	short passed_normal_level;																		// �Ѿ�ͨ�ص���ͨ�ؿ�
	char passed_jingying_level;																		// �Ѿ�ͨ�صľ�Ӣ�ؿ�
	char is_fetched_reward;																			// �Ƿ���ȡ������

	int left_xianhun;																				// ���۵��ɻ�
	short left_reward_item_count;																	// ���۵ĵ�����

	char today_normal_fail_times;																	// ��ǰ��ͨ�ؿ�����ʧ�ܴ���
	char today_jingying_fail_times;																	// ��ǰ��Ӣ�ؿ�����ʧ�ܴ���
	unsigned int normal_enterable_time;																// ��ǰ��ͨ�ؿ��ɽ���ʱ��
	unsigned int jingying_enterable_time;															// ��ǰ��Ӣ�ؿ��ɽ���ʱ��
};

typedef char XianhuntaParamHex[sizeof(XianhuntaFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XianhuntaParamHex) < 64);


//////////////////////////////////////////////////////////  VIP����  //////////////////////////////////////////////////////////
static const int FB_VIP_NUM_MAX = 16;								// VIP�����������

struct VipFBParam
{
	VipFBParam() { this->Reset(); }

	void Reset()
	{
		memset(today_times_list, 0, sizeof(today_times_list));
		is_pass_flag = 0;
	}

	char today_times_list[FB_VIP_NUM_MAX];
	int is_pass_flag;
};

typedef char VipFBParamHex[sizeof(VipFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(VipFBParamHex) < 64);

////////////////////////////////////////////////////////// ��Ӹ���/////////////////////////////////////////////////////////
static const int FB_TEAM_ROOM_MAX_NUM = 50;

enum TEAM_FB_MODE													// ����ģʽ
{
	TEAM_FB_MODE_EASY = 0,											// ��									
	TEAM_FB_MODE_NORMAL,											// ��ͨ
	TEAM_FB_MODE_HARD,												// ����

	TEAM_FB_MAX_MODE,
};

//////////////////////////////////////////////////////////  ����  //////////////////////////////////////////////////////////
struct TowerDefendFBParam
{
	TowerDefendFBParam() { this->Reset(); }

	void Reset()
	{
		personal_max_pass_level = -1;
		personal_join_times = 0;
		personal_buy_join_times = 0;
		personal_auto_fb_free_times = 0;
		personal_item_buy_join_time = 0;
		personal_last_level_star = 0;

		team_attrtype = 0;
		reserve_ch = 0;
		team_member_min_pass = 0;
		team_max_wave_passed = 0;

		personal_holiday_guard_join_times = 0;
		reserver_sh = 0;
		personal_holiday_guard_kill_monster = 0;
		today_multiple_buy_times = 0;
		is_new_player = 0;
		reserve_ch_2 = 0;
	}

	void OnResetData()
	{
		personal_join_times = 0;
		personal_buy_join_times = 0;
		personal_auto_fb_free_times = 0;
		personal_item_buy_join_time = 0;
		personal_holiday_guard_join_times = 0;
		today_multiple_buy_times = 0;
	}

	char personal_max_pass_level;							// �������� ���ͨ����
	char personal_join_times;								// �������� �������
	char personal_buy_join_times;							// �������� ����������
	char personal_auto_fb_free_times;						// �������� ���ɨ������

	short personal_item_buy_join_time;						// �������� ���߹���������
	short personal_last_level_star;							// ��ǰ��߲�ε��Ǽ�		

	char team_attrtype;										// ��������ӳ�����
	char reserve_ch;
	short team_member_min_pass;								// ����ػ���Ա��С������
	int	team_max_wave_passed;								// �����������������

	char personal_holiday_guard_join_times;					// �����ػ� �������
	char reserver_sh;
	short personal_holiday_guard_kill_monster;				// �����ػ� ��ɱ����

	char today_multiple_buy_times;							// ����౶�����������
	char is_new_player;
	short reserve_ch_2;
};

typedef char TowerDefendParamHex[sizeof(TowerDefendFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TowerDefendParamHex) < 64);

////////////////////////////////////////////////////////// ���޼�̳/////////////////////////////////////////////////////////
static const int YAOSHOUJITAN_TEAMFB_MAX_ATTR_NUM = 4;
struct YaoShouJiTanTeamFbRoleAttrInfo
{
	YaoShouJiTanTeamFbRoleAttrInfo() : uid(0), attr(0) {}

	void Reset()
	{
		uid = 0;
		attr = 0;
	}

	bool IsEmpty()
	{ 
		return uid == 0;
	}

	int uid;
	int attr;
};

////////////////////////////////////////////////////////// �Թ��ɸ�/////////////////////////////////////////////////////////

static const int MIGONGXIANFU_NORMAL_LAYER_NUM = 5;		// �Թ��ɸ���ͨ������
static const int MIGONGXIANFU_LAYER_NUM = 7;			// �Թ��ɸ�������
static const int MIGONGXIANFU_DOOR_NUM = 5;				// �Թ��ɸ����͵������
static const int MIGONGXIANFU_LAYER_FIRST_INDEX = 0;	// ��1������
static const int MIGONGXIANFU_BOSS_LAYER_INDEX = 5;		// Boss������	
static const int MIGONGXIANFU_HIDE_LAYER_INDEX = 6;		// ���ز�����

UNSTD_STATIC_CHECK(MIGONGXIANFU_NORMAL_LAYER_NUM < MIGONGXIANFU_LAYER_NUM);

enum MIGONGXIANFU_STATUS_TYPE
{
	MGXF_DOOR_STATUS_NONE = 0,
	MGXF_DOOR_STATUS_TO_PRVE,
	MGXF_DOOR_STATUS_TO_HERE,
	MGXF_DOOR_STATUS_TO_NEXT,
	MGXF_DOOR_STATUS_TO_HIDE,
	MGXF_DOOR_STATUS_TO_BOSS,
	MGXF_DOOR_STATUS_TO_FIRST,
};

enum MIGONGXIANFU_LAYER_TYPE
{
	MGXF_LAYER_TYPE_NORMAL = 0,								// ��ͨ��
	MGXF_LAYER_TYPE_BOSS,									// Boss��
	MGXF_LAYER_TYPE_HIDE,									// ���ز�
};

//////////////////////////////////////////////////////////  ��װ����(������)  //////////////////////////////////////////////////////////
const static int NEQFB_MAX_CHAPTER = 24;                     // ��x�½�
const static int NEQFB_MAX_LEVEL_PER_CHAPTER = 8;            // ÿ���½�x��
const static int NEQFB_MAX_STAR = 3;
const static int NEQFB_PASS_LAYER_BASE_VAL = 10000;
const static int NEQFB_MAX_DROP_COUNT = 32;

const static int NEQFB_ROLL_ITYPE_0_ITEM_COUNT = 2;
const static int NEQFB_ROLL_ITYPE_1_ITEM_COUNT = 5;
const static int NEQFB_ROLL_ITYPE_2_ITEM_COUNT = 1;
const static int NEQFB_ROLLPOOL_TOTAL_COUNT = 8;            // ���ƽ����ܸ���

const static int NEQFB_PICK_FALL_ITEM_RECORD_COUNT = 8;     // ��¼����ʰȡ����ĸ���

UNSTD_STATIC_CHECK(NEQFB_ROLLPOOL_TOTAL_COUNT == NEQFB_ROLL_ITYPE_0_ITEM_COUNT + NEQFB_ROLL_ITYPE_1_ITEM_COUNT + NEQFB_ROLL_ITYPE_2_ITEM_COUNT);

const static int NEQFB_CAN_ROLL_ITYPE_1_LIMIT_COUNT = 2;
const static int NEQFB_MAX_ROLL_TIMES = 8;
const static int NEQFB_MAX_GOLD_ROLL_TIMES = 3;
const static int NEQFB_MAX_FREE_ROLL_TIMES = 5;

UNSTD_STATIC_CHECK(NEQFB_MAX_ROLL_TIMES == NEQFB_MAX_GOLD_ROLL_TIMES + NEQFB_MAX_FREE_ROLL_TIMES);
UNSTD_STATIC_CHECK(NEQFB_ROLL_ITYPE_0_ITEM_COUNT >= NEQFB_CAN_ROLL_ITYPE_1_LIMIT_COUNT);
UNSTD_STATIC_CHECK(NEQFB_ROLLPOOL_TOTAL_COUNT >= NEQFB_MAX_ROLL_TIMES);

struct NewEquipFBLevel
{
	NewEquipFBLevel() { this->Reset(); }

	void Reset()
	{
		join_times = 0;
		max_star = 0;
		reserve_sh = 0;
	}

	char join_times;
	char max_star;
	short reserve_sh;
};

struct NewEquipFBChapter
{
	NewEquipFBChapter() { this->Reset(); }

	void Reset()
	{
		reward_flag = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		memset(level_list, 0, sizeof(level_list));
	}

	char reward_flag;
	char reserve_ch;
	short reserve_sh;
	NewEquipFBLevel level_list[NEQFB_MAX_LEVEL_PER_CHAPTER];
};

struct NewEquipFBParam
{
	NewEquipFBParam() { this->Reset(); }

	void Reset()
	{
		today_vip_buy_times = 0;
		today_item_buy_times = 0;
		today_fight_all_times = 0;
		reserve_sh_3 = 0;
		reserve_int = 0;
		memset(chapter_list, 0, sizeof(chapter_list));
	}

	short today_vip_buy_times;
	short today_item_buy_times;
	short today_fight_all_times;
	short reserve_sh_3;
	int reserve_int;
	NewEquipFBChapter chapter_list[NEQFB_MAX_CHAPTER];
};

typedef char NewEquipFBParamHex[sizeof(NewEquipFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(NewEquipFBParamHex) < 2048);

//////////////////////////////////////////////////���߲��ϱ�/////////////////////////////
static const int ARMORDEFEND_SKILL_MAX_COUNT = 2;			// ���������������
struct ArmorDefendFBParam
{
	ArmorDefendFBParam() { this->Reset(); }

	void Reset()
	{
		personal_max_pass_level = -1;
		personal_join_times = 0;
		personal_buy_join_times = 0;
		personal_auto_fb_free_times = 0; 
		personal_item_buy_join_time = 0;
		reserve_sh = 0;
	}

	void OnResetData()
	{
		personal_join_times = 0;
		personal_buy_join_times = 0;
		personal_auto_fb_free_times = 0;
		personal_item_buy_join_time = 0;
	}

	char personal_max_pass_level;						// �������� ���ͨ����
	char personal_join_times;							// �������� �������
	char personal_buy_join_times;						// �������� ����������
	char personal_auto_fb_free_times;					// �������� ���ɨ������

	short personal_item_buy_join_time;					// �������� ���߹���������
	short reserve_sh;
};

typedef char ArmorDefendParamHex[sizeof(ArmorDefendFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ArmorDefendParamHex) < 64);

//////////////////////////////////////////////////////////  ���鸱��  //////////////////////////////////////////////////////////
static const int FB_STORY_MAX_COUNT = 20;			// ���鱾�������

struct StoryFBParam
{
	struct FbItem
	{
		FbItem() { this->Reset(); }

		void Reset()
		{
			is_pass = 0;
			today_times = 0;
			reserved = 0;
		}

		char is_pass;
		char today_times;
		short reserved;
	};

	StoryFBParam() { this->Reset(); }
	void Reset()
	{
		curr_fb_index = 0;
		for (int i = 0; i < FB_STORY_MAX_COUNT; ++i)
		{
			fb_list[i].Reset();
		}
	}

	int curr_fb_index;
	FbItem fb_list[FB_STORY_MAX_COUNT];
};

typedef char StoryFBParamHex[sizeof(StoryFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(StoryFBParamHex) < 2048);

//////////////////////////////////////////////////////////  ħ�丱��  //////////////////////////////////////////////////////////
static const int MOJIE_FB_HUMAN_PIAN_MAX_COUNT = 8;						// ����ƪ�������ֵ
static const int MOJIE_FB_AIREN_PIAN_MAX_COUNT = 8;						// ����ƪ�������ֵ
static const int MOJIE_FB_SKILL_PIAN_ZHANGJIE_MAX_COUNT = 8;			// �����½����ֵ
static const int MOJIE_FB_JINGLING_PIAN_ZHANGJIE_TASK_MAX_COUNT = 10;	// �����½ڵ����������ֵ
static const int MOJIE_FB_SKILL_MAX_COUNT = 30;							// ħ�似�����ֵ

// ���������½�
enum MOJIEFB_JINGLINGPIAN_CHAPTER_TYPE
{
	MOJIEFB_FANGYU_TASK = 0,		// ��������
	MOJIEFB_ZHUANGBEI_TASK,			// װ������
	MOJIEFB_BOSS_TASK,				// ��ɱboss
	MOJIEFB_SHENZHU_TASK,			// �����ﵽ
};

// ħ�似��
enum MOJIEFB_JINENG
{
	MOJIEFB_SKILL_SHEHUN = 0,		// ��꣺��ɱ��������20%���� 
	MOJIEFB_SKILL_HUIFU,			// �ָ�������ʱ��5%���ʻظ�1000����Ѫ
	MOJIEFB_SKILL_CHIDUN,			// �ٶۣ�����ʱ��10%����ʹĿ���ٶȼ���40%������2��
	MOJIEFB_SKILL_CHUMO,			// ��ħ���Թ����˺�����10% 
	MOJIEFB_SKILL_JIANSHANG,		// ���ˣ�ͬʱ�ܵ�3�����﹥��ʱ�������˺�����30%
	MOJIEFB_SKILL_ZHUFU,			// ף������ɱ����װ����������20%
	MOJIEFB_SKILL_JUEJING,			// ��������Χ��5������ʱ�����м����˺��Թ������20%
	MOJIEFB_SKILL_YANSHOU,			// ���أ�Ѫ������30%ʱ���յ��˺�����20%
	MOJIEFB_SKILL_ZUIJI,			// ׷������Ѫ������30%��ҽ��й���ʱ���˺�����20%
	MOJIEFB_SKILL_DONGCHA,			// ���죺�ܵ�����ʱ��20%���ʼ���20%���˺�
	MOJIEFB_SKILL_KUANGMBAO,		// �񱩣�����ʱ��20%��������10%�˺�
	MOJIEFB_SKILL_JIANSHE,			// ���䣺����ʱ��10%�������Գ�Ŀ�������3����ɫ���30%���˺�
	MOJIEFB_SKILL_SHUNSHI,			// ˳�ƣ����ˣ���boss����Ѫ��5%����ʱ������ʱ��10%����ֱ�ӻ�ɱ
	MOJIEFB_SKILL_TUJING,			// ͻ��������ʱ��5%���ʶ�Ŀ��������2000���˺�
	MOJIEFB_SKILL_POJIA,			// �Ƽף�����ʱ��10%�������ӶԷ�30%����
	MOJIEFB_SKILL_KUANGLV,			// ��ŭ���Լ���Ѫ����10%ʱ��������������3%
	MOJIEFB_SKILL_GUANGHUI,		// ��ԣ�pvpʱ��ɵ��˺�����8%���յ��˺�����8%
	MOJIEFB_SKILL_ZHIBAO,			// �α����յ�����ʱ����20%���ʽ���30%�˺�
};

struct MojieFBParam
{

	MojieFBParam(){this->Reset(); }
	void Reset()
	{
		mojie_human_pian_task_mark = -1;
		mojie_airen_pian_task_mark = -1;
		mojie_jingling_pian_task_mark = -1;
	}

	int mojie_human_pian_task_mark;						// ħ������ƪ��ɱ��
	int mojie_airen_pian_task_mark;						// ħ�䰫��ƪ��ɱ��
	int mojie_jingling_pian_task_mark;					// ħ�侫��ƪ��ɱ��
};

typedef char MojieFBParamHex[sizeof(MojieFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MojieFBParamHex) < 128);

//////////////////////////////////////////////////////////  ��ս����  //////////////////////////////////////////////////////////
struct ChallengeFBParam
{
	const static int LEVEL_MAX_COUNT = 30;
	
	struct Level
	{
		Level()	{ this->Reset(); }

		void Reset()
		{
			pass_time = 0;
			is_pass = 0;
			state = 0;
			fight_layer = 0;
			history_max_reward = 0;
			use_count = 0;
			res_ch = 0;
		}

		void OnResetData()
		{
			pass_time = 0;
			fight_layer = 0;
			use_count = 0;
		}

		unsigned int pass_time;
		char is_pass;
		char fight_layer;
		short state;
		short history_max_reward;//�����Ǽ�
		char use_count;
		char res_ch;
	};

	ChallengeFBParam() { this->Reset(); }

	void Reset()
	{
		enter_count = 0;
		buy_count = 0;

		for (int i = 0; i < LEVEL_MAX_COUNT; ++ i)
		{
			level_list[i].Reset();
		}
	}

	void OnResetData()
	{
		enter_count = 0;
		buy_count = 0;

		for (int i = 0; i < LEVEL_MAX_COUNT; ++ i)
		{
			level_list[i].OnResetData();
		}
	}

	Level level_list[LEVEL_MAX_COUNT];
	int enter_count;
	int buy_count;
};

typedef char ChallengeFBParamHex[sizeof(ChallengeFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeFBParamHex) < 768);

//////////////////////////////////////////////////////////  �ճ�����  //////////////////////////////////////////////////////////
static const int FB_RECORD_DROP_ITEM_MAX_NUM = 7;

static const int FB_EXP_MAX_WAVE = 300;			// ���鱾�����
static const int MAX_DAILY_FB_NUM = 15;

enum DAILYFB_TYPE
{
	DAILYFB_TYPE_EXP = 0,		// ���鱾
	DAILYFB_TYPE_COIN,			// ͭ�ұ�
	DAILYFB_TYPE_DAOJU,			// ���߱�
};

struct DailyFbParam
{
	DailyFbParam() { this->Reset(); }

	void Reset()
	{
		expfb_history_pass_max_wave = 0;
		expfb_today_pay_times = 0;
		expfb_today_enter_times = 0;
		expfb_history_enter_times = 0;
		last_quit_fb_time = 0;
		expfb_last_exp = 0;
		expfb_record_max_exp = 0;
	}
	
	short expfb_history_pass_max_wave;				// ��ʷ�����
	short expfb_today_pay_times;					// ����Ĺ������
	short expfb_today_enter_times;					// ����������
	unsigned short expfb_history_enter_times;		// ��ʷ�������
	unsigned int last_quit_fb_time;					// ���һ�ε��˳�ʱ��
	long long expfb_last_exp;						// ��һ�λ�õľ��飬�����һ���
	long long expfb_record_max_exp;					// ͨ����ʷ��þ������
};
typedef char DailyFbParamHex[sizeof(DailyFbParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(DailyFbParamHex) < 512);

//////////////////////////////////////////////////////////  �׶θ���  //////////////////////////////////////////////////////////
static const int FB_PHASE_MAX_COUNT = 32;		// �׶α��������
static const int FB_PHASE_MAX_LEVEL = 30;		// ÿ�����ױ�С�ؿ������

struct PhaseFbParam
{
	struct FbItem
	{
		FbItem() { this->Reset(); }

		void Reset()
		{
			is_pass = 0;
			today_times = 0;
			today_buy_times = 0;
			reserved = 0;
		}

		char is_pass;			// �Ѿ�ͨ���˵ڼ���(����)
		char today_times;		// �����Ѿ��õ��Ĵ���
		char today_buy_times;	// ���չ���Ĵ���
		char reserved;
	};

	PhaseFbParam() { this->Reset(); }
	void Reset()
	{
		curr_fb_index = 0;
		for (int i = 0; i < FB_PHASE_MAX_COUNT; ++i)
		{
			fb_list[i].Reset();
		}
		curr_fb_level = 0;
	}

	int curr_fb_index;
	FbItem fb_list[FB_PHASE_MAX_COUNT];
	int curr_fb_level;
};

typedef char PhaseFbParamHex[sizeof(PhaseFbParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PhaseFbParamHex) < 2048);

//////////////////////////////////////////////////////////  �����Ӹ���  //////////////////////////////////////////////////////////
static const int NEW_FB_DROP_ITEM_MAX = 15;								// ������������Ʒ��
static const int NEW_FB_DELAY_SEND_TIME = 3;							// �����ӳٷ�����ʱ�䣨�룩
static const int CROSS_TEAM_FB_GONGJI_BUFF_ADDON = 10;					// ��������˹����ӳɰٷֱ�

//////////////////////////////////////////////////////////  ���װ������  //////////////////////////////////////////////////////////
static const int MAX_TEAM_EQUIP_FB_LAYER = 20;							// ��Ӹ���������
static const int TEAM_EQUIP_FB_ROLE_MAX = 4;							// ��Ӹ�����������ɫ��
static const int TEAM_EQUIP_FB_GONGJI_BUFF_ADDON = 10;					// ��������˹����ӳɰٷֱ�
static const int TEAM_EQUIP_FB_DELAY_SEND_TIME = 3;						// �����ӳٷ�����ʱ�䣨�룩

enum NEW_FB_STATE_TYPE
{
	NEW_FB_STATE_TYPE_DEFAULT = 0,										// Ĭ��
	NEW_FB_STATE_TYPE_SUCCEED,											// ͨ�سɹ�
	NEW_FB_STATE_TYPE_FAIL,												// ͨ��ʧ��
};

struct FBDropItemInfo
{
	FBDropItemInfo() : item_id(0), is_bind(0), is_first(0), num(0) {}

	ItemID item_id;
	char is_bind;
	char is_first;
	int num;
};

//////////////////////////////////////////////////////////  ��ͼ����  //////////////////////////////////////////////////////////
static const int TUTUI_FB_TYPE_NUM_MAX = 2;								// ��ͼ�������������
static const int TUITU_FB_CHAPER_NUM_MAX = 50;							// ��ͼ�����½������
static const int TUITU_FB_LEVLE_NUM_MAX = 20;							// ��ͼ�����ؿ������
static const int TUITU_FB_CHAPTER_STAR_REWARD_NUM_MAX = 16;				// ��ͼ�����½��Ǽ��������������

enum TUITU_FB_TYPE
{
	TUITU_FB_TYPE_NORMAL = 0,
	TUITU_FB_TYPE_HARD,

	TUITU_FB_TYPE_MAX
};

UNSTD_STATIC_CHECK(TUTUI_FB_TYPE_NUM_MAX >= TUITU_FB_TYPE_MAX);

struct TuituFbParam
{
	struct LayerInfo
	{
		LayerInfo() : pass_star(0), reward_flag(0), reserve_sh(0){}

		char pass_star;
		char reward_flag;
		short reserve_sh;
	};

	struct ChapterInfo
	{
		ChapterInfo() : is_pass_chapter(0), reserve_ch(0), reserve_sh(0), total_star(0), star_reward_flag(0) {}

		char is_pass_chapter;
		char reserve_ch;
		short reserve_sh;
		short total_star;
		short star_reward_flag;
		LayerInfo level_list[TUITU_FB_LEVLE_NUM_MAX];
	};

	struct FbInfo
	{
		FbInfo() { this->Reset(); }
		void Reset()
		{
			pass_chapter = -1;
			pass_level = -1;
			today_join_times = 0;
			buy_join_times = 0;

			memset(chapter_list, 0, sizeof(chapter_list));
		}

		short pass_chapter;				//��ͨ������½�
		short pass_level;				//��ͨ�����ؿ��ȼ�
		short today_join_times;			//���ս������
		short buy_join_times;			//�������
		ChapterInfo chapter_list[TUITU_FB_CHAPER_NUM_MAX];
	};

	TuituFbParam() { this->Reset(); }
	void Reset()
	{
		enter_fb_type = 0;
		enter_chapter = 0;
		enter_level = 0;
		reserve_ch = 0;
		for (int i = 0; i < TUTUI_FB_TYPE_NUM_MAX; ++i)
		{
			fb_list[i].Reset();
		}
	}

	void ResetData()
	{
		for (int i = 0; i < TUTUI_FB_TYPE_NUM_MAX; ++i)
		{
			fb_list[i].today_join_times = 0;
			fb_list[i].buy_join_times = 0;

			for (int j = 0; j < TUITU_FB_CHAPER_NUM_MAX; ++j)
			{
				for (int k = 0; k < TUITU_FB_LEVLE_NUM_MAX; ++k)
				{
					fb_list[i].chapter_list[j].level_list[i].reward_flag = 0;
				}
			}
		}
	}

	char enter_fb_type;
	char enter_chapter;
	char enter_level;
	char reserve_ch;
	FbInfo fb_list[TUTUI_FB_TYPE_NUM_MAX];
};

typedef char TuituFbParamHex [sizeof(TuituFbParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TuituFbParamHex) < 20480);

//////////////////////////////////////////////// �ز�boss	//////////////////////////////////////

struct PreciousBossParam
{
	PreciousBossParam()
	{
		this->Reset();
	}

	enum PRECIOUS_BOSS_TASK_TYPE
	{
		PRECIOUS_BOSS_TASK_TYPE_BOSS = 0,
		PRECIOUS_BOSS_TASK_TYPE_COMMON_MONSTER,
		PRECIOUS_BOSS_TASK_TYPE_GATHER,
		PRECIOUS_BOSS_TASK_TYPE_GREAT_MONSTER,

		PRECIOUS_BOSS_TASK_TYPE_MAX,
	};

	void Reset()
	{
		this->ResetTaskList();
	}

	void ResetTaskList()
	{
		for (int i = 0; i < PRECIOUS_BOSS_TASK_TYPE_MAX; ++i)
		{
			task_list[i].Reset();
		}
	}

	struct TaskParam
	{
		TaskParam()
		{
			this->Reset();
		}

		void Reset()
		{
			task_condition = 0;
			task_type = -1;
			is_finish = 0;
		}

		short task_condition;					// ������������
		char task_type;							// ��������
		char is_finish;							// �Ƿ����
	};

	TaskParam task_list[PRECIOUS_BOSS_TASK_TYPE_MAX];
};

typedef char PreciousBossParamHex[sizeof(PreciousBossParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PreciousBossParamHex) < 64);

/////////////////////////// ����boss //////////////////////////////////////////
static const int MAX_PERSON_BOSS_LAYER = 40;     // ���˸���������

struct PersonBossParam
{
	PersonBossParam()
	{
		this->Reset();
	}

	void Reset() 
	{
		memset(person_boss_today_enter_times, 0, sizeof(person_boss_today_enter_times));
	}
	char person_boss_today_enter_times[MAX_PERSON_BOSS_LAYER + 1];    // ����boss��ÿ��boss��������
};

typedef char PersonBossParamHex[sizeof(PersonBossParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PersonBossParamHex) < 128);
////////////////////////////////��ħboss/////////////////////////////////////////////////////////////
static const int GODMAGIC_BOSS_MAX_HISTROY_RECROD = 5;						    // ��ħboss����ɱ��¼
static const int MAX_GODMAGIC_BOSS_PER_SCENE = 20;							    // ÿ������boss������
static const int GODMAGIC_BOSS_MAX_MONSTER_NUM = 200;							// ��ħbossС���������
static const int GODMAGIC_BOSS_MAX_ORDINARY_CRYSTAL = 60;						// ��ħboss�����ͨˮ��
static const int GODMAGIC_BOSS_MAX_TREASURE_CRYSTAL_POS_NUM = 60;				// ��ħboss�����ϧˮ��
static const int GODMAGIC_BOSS_SCENE_MAX = 10;									// ��ħboss����������
static const int GODMAGIC_BOSS_MAX_DROP_RECROD = 20;							// ��ħboss�������¼

#pragma pack(pop)

#endif // __FB_DEF_HPP__



