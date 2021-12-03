#ifndef __TASK_DEF_HPP__
#define __TASK_DEF_HPP__

#include <memory>
#include "servercommon/basedef.h"
#include "servercommon/servercommon.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TASK_COLOR
{
	TASK_COLOR_INVALID = 0,

	TASK_COLOR_WHITE,															// ��ɫ
	TASK_COLOR_GREEN,															// ��ɫ
	TASK_COLOR_BLUE,															// ��ɫ
	TASK_COLOR_PURPLE,															// ��ɫ
	TASK_COLOR_ORANGE,															// ��ɫ

	TASK_COLOR_MAX,
};

enum TASK_TYPE
{
	TASK_TYPE_INVALID = -1,
	TASK_TYPE_TRUNK = 0,														// ����
	TASK_TYPE_BRANCH = 1,														// ֧��
	TASK_TYPE_DAILY = 2,														// �ճ�
	TASK_TYPE_HUSONG = 3,														// ����
	TASK_TYPE_GUILD = 4,														// ��������
	TASK_TYPE_CISHA = 5,														// ��ɱ����
	TASK_TYPE_PAOHUAN = 6,														// �ܻ�����
	TASK_TYPE_ZHUANZHI = 8,                                                     // תְ����
	TASK_COUNT,
};

enum TASK_CONDITION
{
	TASK_CON_INVALID = -1,
	TASK_CON_TALK = 0,															// npc�Ի�
	TASK_CON_KILL_MONSTER,														// ���
	TASK_CON_GATHER,															// �ɼ�
	TASK_CON_PASS_FB,															// ��ɸ���
	TASK_CON_ENTER_SCENE,														// ���볡��
	TASK_CON_COMMIT_TASK,														// �ύ����
	TASK_CON_NOTHING,															// �������
	TASK_CON_DO_OPERATOR,														// ִ��ĳ�ֲ���
	TASK_CON_SATISFY_STATUS,													// ����ĳ��״̬
	TASK_CON_ENTER_SCENE_TYPE,													// ����ĳ�ֳ�������
	TASK_CON_PICKUP_ITEM,														// ʰȡĳ����Ʒ
	TASK_CON_KILL_BOSS_TYPE,													// ��ɱĳ�ֹ�������
	TASK_CON_PASS_FB_LAYER,														// ����ͨ�ص�X��
	TASK_CON_PASS_SPECIAL_TASK,													// �������񣬿ͻ��˷�Э��������
	TASK_CON_PASS_DAILY_TASK_FB,												// ����ճ����񸱱�
	TASK_CON_BIANSHEN_TALK,														// ִ��ĳ�ֲ���������NPC�Ի�
	TASK_CON_COMMIT_ITEM,														// �ύ������Ʒ
	TASK_CON_KILL_MONSTER_LEVEL,                                                // ��ɱĳһ�ȼ��Ĺ���
	TASK_CON_MAX,
};

enum TASK_ACCEPT_OP
{
	TASK_ACCEPT_OP_INVALID = 0,
	TASK_ACCEPT_OP_FLY,															// ��������ʱ���У��������������������ģʽ														
	TASK_ACCEPT_OP_MOVE,														// ���͵�ĳ���ص�
	TASK_ACCEPT_OP_FLY_ON_ACCEPT,												// �������������ʱ���ģʽ
	TASK_ACCEPT_OP_MOVE_NORMAL_ON_REMOVE,										// �Ƴ������ʱ�������������ģʽ
	TASK_ACCEPT_OP_ADD_SKILL_ON_ACCEPT,											// �������񼤻��
	TASK_ACCEPT_OP_ADD_SKILL_ON_REMOVE,											// �Ƴ����񼤻��
	TASK_ACCEPT_OP_ENTER_GUIDE_FB,												// ���������������ͻ�����
	TASK_ACCEPT_OP_OPEN_GUIDE_VIEW,												// ���������������棬�ͻ�����
	TASK_ACCEPT_OP_ENTER_DAILY_FB,												// �����ճ��������ͻ�����
	TASK_ACCEPT_OP_CLIENT_PARAM,												// �ͻ���ʹ�ò���������������֤
	TASK_ACCEPT_OP_MAX,
};

enum TASK_OPERATOR_TYPE
{
	TASK_OPERATOR_TYPE_MOUNT_UPGRADE = 1,										// ��������
	TASK_OPERATOR_TYPE_GUILD_DONATE,											// ���˾���
	TASK_OPERATOR_TYPE_STRENGTH,												// װ��ǿ��
	TASK_OPERATOR_TYPE_UPSTAR,													// װ������
	TASK_OPERATOR_TYPE_UPLEVEL,													// װ���ϳ�(������
	TASK_OPERATOR_TYPE_INHERIT,													// װ���̳�
	TASK_OPERATOR_TYPE_EQUIP_RECYCLE,											// װ������
	TASK_OPERATOR_TYPE_LIEMING_CHOUHUN,											// ����һ��
	TASK_OPERATOR_TYPE_LIEMING_HUN_ADD_EXP,										// ��������
	TASK_OPERATOR_TYPE_FULIN,													// װ������
	TASK_OPERATOR_TYPE_KILLBOSS,												// BOSS��ɱһ��
	TASK_OPERATOR_TYPE_ADD_TEAM,												// �������
	TASK_OPERATOR_TYPE_INLAY_STONE,												// ��Ƕ��ʯ
	TASK_OPERATOR_TYPE_JOIN_GUILD,												// ��������
	TASK_OPERATOR_TYPE_SZ_JINJIE,												// ��װ����
	TASK_OPERATOR_TYPE_SZ_SHENZHU,												// ��װ����
	TASK_OPERATOR_TYPE_JL_FEISHENG,												// �������
	TASK_OPERATOR_TYPE_JL_ZHUANGBEI,											// ����װ��
	TASK_OPERATOR_TYPE_MOUNT_UP_STAR,											// ��������
	TASK_OPERATOR_TYPE_WING_UP_STAR,											// ��������
	TASK_OPERATOR_TYPE_HALO_UP_STAR,											// �⻷����
	TASK_OPERATOR_TYPE_SHENGONG_UP_STAR,										// ������
	TASK_OPERATOR_TYPE_SHENYI_UP_STAR,											// ��������
	TASK_OPERATOR_TYPE_FIGHT_MOUNT_UP_STAR,										// ս����������
	TASK_OPERATOR_TYPE_SAILING,													// ������ - ����
	TASK_OPERATOR_TYPE_SAILING_ROB,												// ������ - ���ᷫ��
	TASK_OPERATOR_TYPE_MINING,													// ������ - �ڿ�
	TASK_OPERATOR_TYPE_MINING_ROB,												// ������ - �����ʯ
	TASK_OPERATOR_TYPE_FIGHTING_CHALLENGE,										// ������ - ����
	TASK_OPERATOR_TYPE_XIANNV_UP_ZIZHI,											// Ů����������
	TASK_OPERATOR_TYPE_RUNE_UP_LEVEL,											// ��������
	TASK_OPERATOR_TYPE_CROSS_1V1,												// ������1V1
	TASK_OPERATOR_TYPE_CATCH_FISH,												// ��������
	TASK_OPERATOR_TYPE_FEEDING_FISH,											// ��������
	TASK_OPERATOR_TYPE_JINGLING_UP_LEVEL,										// ����ɳ�����
	TASK_OPERATOR_TYPE_JINGLING_WUXING_UP_LEVEL,								// ������������
	TASK_OPERATOR_TYPE_CHAT_ON_WORLD_CHANNEL,									// ������Ƶ������
	TASK_OPERATOR_TYPE_1V1_FIELD,												// 1V1������
	TASK_OPERATOR_TYPE_FOOTPRINT_UP_STAR,										// �㼣����
	TASK_OPERATOR_TYPE_JINGLINT_CHOUHUN,										// ����Ѱ��
	TASK_OPERATOR_TYPE_WING_UPGRADE,											// �������
	TASK_OPERATOR_TYPE_HALO_UPGRADE,											// �⻷����
	TASK_OPERATOR_TYPE_SHENGONG_UPGRADE,										// �񹭽���
	TASK_OPERATOR_TYPE_SHENYI_UPGRADE,											// �������
	TASK_OPERATOR_TYPE_PUSH_GUILD_SKILL_UP,										// ������˼�������
	TASK_OPERATOR_TYPE_PUT_ON_ZHUANZHI_EQUIP,									// ����תְװ��

	TASK_OPERATOR_TYPE_MAX,
};

enum TASK_SATISFY_STATUS_TYPE
{
	TASK_SATISFY_STATUS_TYPE_MOUNT_GRADE = 1,									// ����׼��ﵽn��
	TASK_SATISFY_STATUS_TYPE_WING_GRADE,										// ����׼��ﵽn��
	TASK_SATISFY_STATUS_TYPE_HALO_GRADE,										// �⻷�׼��ﵽn��
	TASK_SATISFY_STATUS_TYPE_SHENGONG_GRADE,									// �񹭽׼��ﵽn��
	TASK_SATISFY_STATUS_TYPE_SHENYI_GRADE,										// ����׼��ﵽn��
	TASK_SATISFY_STATUS_TYPE_FIGHT_MOUNT_GRADE,									// ս������׼��ﵽn��

	TASK_SATISFY_STATUS_TYPE_MOUNT_STAR,										// �����Ǽ��ﵽn��
	TASK_SATISFY_STATUS_TYPE_WING_STAR,											// �����Ǽ��ﵽn��
	TASK_SATISFY_STATUS_TYPE_HALO_STAR,											// �⻷�Ǽ��ﵽn��
	TASK_SATISFY_STATUS_TYPE_SHENGONG_STAR,										// ���Ǽ��ﵽn��
	TASK_SATISFY_STATUS_TYPE_SHENYI_STAR,										// �����Ǽ��ﵽn��
	TASK_SATISFY_STATUS_TYPE_FIGHT_MOUNT_STAR,									// ս�������Ǽ��ﵽn��

	TASK_SATISFY_STATUS_TYPE_JINGLING_MAX_LEVEL,								// �������ﵽn��
	TASK_SATISFY_STATUS_TYPE_PUT_JINLING_COUNT,									// װ�����������ﵽnֻ

	TASK_SATISFY_STATUS_TYPE_PATA_FB_LEVEL,										// ���������ﵽn��
	TASK_SATISFY_STATUS_TYPE_EXP_FB_WAVE,										// ���鸱������n��

	TASK_SATISFY_STATUS_TYPE_OWN_FRIEND,										// ӵ�к��Ѵﵽn��
	TASK_SATISFY_STATUS_TYPE_CHENGJIU_TITLE,									// �ɾͳɺŴﵽn��

	TASK_SATISFY_STATUS_TYPE_EQUIP_TOTAL_STRENGTH_LEVEL,						// ȫ��װ��ǿ���ܵȼ��ﵽn��
	TASK_SATISFY_STATUS_TYPE_EQUIP_TOTAL_STAR_LEVEL,							// ȫ��װ�������ܵȼ��ﵽn��

	TASK_SATISFY_STATUS_TYPE_ACTIVE_XIANNV,										// ����Ů��n��

	TASK_SATISFY_STATUS_TYPE_INLAY_STONE,										// ��Ƕһ��n���ı�ʯ
	TASK_STATIFY_STATUS_TYPE_TUIFU_NORMAL_FB,									// ��ͼ��ͨ����ͨ��x��x��
	TASK_STATIFY_STATUS_TYPE_TUIFU_HARD_FB,										// ��ͼ��Ӣ����ͨ��x��x��

	TASK_SATISFY_STATUS_TYPE_FOOTPRINT_GRADE,									// �㼣�׼��ﵽn��
	TASK_SATISFY_STATUS_TYPE_FOOTPRINT_STAR,									// �㼣�Ǽ��ﵽn��

	TASK_SATISFY_STATUS_TYPE_RED_EQUIP_WEAR_COUNT,								// ���ϴ���m�׵ĺ�װ�����ﵽn��

	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_TIANMING,								    // תְ����������n��  ��ת   c_param2������
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_CANGLONG,								    // תְ����������n��  ��ת
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENGXIAO,								// תְ������Ф��n��  ��ת
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_YUANQI,								    // תְ����Ԫ����n��  ��ת
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENQI,								    // תְ����������n��  ��ת
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_XINGXIU,								    // תְ�������޵�n��  ʮת
	TASK_SATISFY_STATUS_TYPE_CON_TO_PROF6,								        // �ﵽתְ��ת������
	TASK_SATISFY_STATUS_TYPE_ONE_ZHUAN,											// ���nת
	TASK_SATISFY_STATUS_TYPE_PUT_ON_ZHUANZHI_EQUIP,								// ����n��תְװ��
	TASK_SATISFY_STATUS_TYPE_JINGJIE_LEVEL,										// ����ﵽn��
	TASK_SATISFY_STATUS_TYPE_PHASE_FB_LEVEL,									// ���׸����ﵽn��
	TASK_SATISFY_STATUS_TYPE_PUT_ON_EQUIP,										// ����n��m��װ��
	TASK_SATISFY_STATUS_TYPE_TOWER_DEFEND_FB_ENTER_COUNT,						// ����������������n��
	TASK_SATISFY_STATUS_TYPE_KILL_BOSS_TYPE,									// ��ɱn����boss m ֻ
	
	TASK_SATISFY_STATUS_TYPE_MAX,
};

static const int MAX_NEXT_TASK_COUNT = 100;


#pragma pack(push, 4)

struct TaskInfo
{
	TaskInfo() : task_id(0), task_ver(0), task_condition(0), progress_num(0), reserve_sh3(0), accept_time(0), reserve_int1(0), reserve_int2(0) {}

	unsigned short task_id;
	char task_ver;
	char task_condition;
	short progress_num;
	short reserve_sh3;
	unsigned int accept_time;
	int reserve_int1;
	int reserve_int2;
};

struct TaskParam
{
	static const int MAX_ACCEPTED_TASK_NUM = 40;

	TaskParam() { this->Reset(); }

	void Reset()
	{
		reserve_sh1 = reserve_int2 = reserve_int = 0;
		task_count = 0;
		memset(taskinfo_list, 0, sizeof(taskinfo_list));
	}

	TaskInfo * GetTaskInfo(unsigned short task_id, int *t_index = NULL)
	{
		for (int i = 0; i < task_count; ++i)
		{
			if (taskinfo_list[i].task_id == task_id)
			{
				if (NULL != t_index) *t_index = i;
				return &taskinfo_list[i];
			}
		}

		return NULL;
	}

	bool AddTask(const TaskInfo &task_info)
	{
		if (task_count >= MAX_ACCEPTED_TASK_NUM)
		{
			return false;
		}

		taskinfo_list[task_count ++] = task_info;
		return true;
	}

	void RemoveByID(unsigned short task_id)
	{
		for (int i = 0; i < task_count; ++i)
		{
			if (taskinfo_list[i].task_id == task_id)
			{
				taskinfo_list[i] = taskinfo_list[task_count - 1];
				-- task_count;
				return;
			}
		}
	}

	void RemoveByIndex(int t_index)
	{
		if (t_index >= 0 && t_index < task_count)
		{
			taskinfo_list[t_index] = taskinfo_list[task_count - 1];
			-- task_count;
		}
	}

	int reserve_int;
	int reserve_int2;
	short reserve_sh1;
	short task_count;
	TaskInfo taskinfo_list[MAX_ACCEPTED_TASK_NUM];
};

typedef char TaskParamHex[sizeof(TaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TaskParamHex) < 2048);


static const int MAX_TASK_RECORD_NUM = 1000;
static const int MAX_TASK_RECORD_BUFF_LEN = sizeof(short) + sizeof(short) * MAX_TASK_RECORD_NUM;

//struct TaskRecordParam
//{
//	short task_count;
//	unsigned short task_item_list[MAX_TASK_RECORD_NUM];
//};
//UNSTD_STATIC_CHECK(sizeof(TaskRecordParam) * 2 < 4096);
//////////////////////////////////////////////////////////  �ɰ��������  /////////////////////////////////////////////////////////////////
static const int XIANBANG_LEVEL_INTERVAL_MAX = 40;										// �ɰ����� �ȼ������� ��С 

static const int XIANBANG_OPEN_LEVEL = 41;												// �ɰ����� ���ŵȼ� 
static const int XIANBANG_ONEDAY_ACCEPT_MAX = 10;										// �ɰ����� ÿ�������ȡ����
static const int XIANBANG_ACCEPTED_LIST_MAX = 3;										// �ɰ����� �ѽ������б� ��С
static const int XIANBANG_FLUSH_LIST_MAX = 5;											// �ɰ����� ˢ�������б� ��С

static const int XIANBANG_FREE_FLUSH_INTERVAL_S = 30 * 60;								// �ɰ����� ���ˢ�¼��

struct XianBangTaskItem
{
	XianBangTaskItem() { this->Reset(); } 

	void Reset() { task_id = 0; task_color = 0; unit_type = 0; }
	bool Invalid() { return 0 == task_id; }

	UInt16 task_id;																		// ����ID
	char task_color;																	// ������ɫ
	char unit_type;																		// ��������
};

struct XianBangTaskParam
{
	XianBangTaskParam() { this->Reset(); }

	void Reset()
	{
		next_free_flush_time = 0;

		memset(accepted_list, 0, sizeof(accepted_list));
		memset(flush_list, 0, sizeof(flush_list));
	}

	unsigned int next_free_flush_time;													// �´����ˢ��ʱ��
	XianBangTaskItem accepted_list[XIANBANG_ACCEPTED_LIST_MAX];							// �ѽ������б�
	XianBangTaskItem flush_list[XIANBANG_FLUSH_LIST_MAX];								// ˢ���б�
};

typedef char XianBangTaskParamHex[sizeof(XianBangTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XianBangTaskParamHex) < 128);

///////////////////////////////////////////////////////////  �ճ��������  /////////////////////////////////////////////////////////////
static const int DAILY_LEVEL_INTERVAL_MAX = 60;											// 
static const int DAILY_LEVEL_TEMP_TASK_MAX = 60;										// 

static const int DAILY_ONEDAY_COMMIT_MAX = 10;											// ÿ�տ��ύ�ճ������������
static const int DAILY_GOLD_COMMIT_REWARD_TIMES = 2;									// Ԫ���ύ����������

enum DAILY_TASK_REWARD_TYPE
{
	DAILY_TASK_REWARD_TYPE_VALID = 0,

	DAILY_TASK_REWARD_TYPE_NORMAL,		// ��������
	DAILY_TASK_REWARD_TYPE_DOUBLE,		// ˫������
	DAILY_TASK_REWARD_TYPE_TREBLE,		// ��������

	DAILY_TASK_REWARD_TYPE_MAX,
};

///////////////////////////////////////////////////////////  �ܻ��������  /////////////////////////////////////////////////////////////
static const int PAOHUAN_ONEDAY_COMMIT_MAX = 100;										// ÿ�տ��ύ�ܻ������������
static const int PAOHUAN_LEVEL_INTERVAL_MAX = 60;										// �ܻ�����ȼ��������
static const int PAOHUAN_LEVEL_TEMP_TASK_MAX = 60;										// 

///////////////////////////////////////////////////////////  �����������  /////////////////////////////////////////////////////////////
static const int HUSONG_ONEDAY_ACCEPT_MAX = 3;											// ����ÿ��������
static const int HUSONG_LEVEL_REWARD_CFG_MAX_COUNT = 110;								// �������� �ȼ������������ֵ
static const int HUSONG_BUY_TIMES_CFG_MAX_COUNT = 32;									// �������� ��������������ֵ
static const int HUSONG_TASK_LIMIT_TIME_S = 900;										// �����������ʱ��

struct HusongTaskParam
{
	HusongTaskParam() { this->Reset(); }

	void Reset() { task_id = 0; task_color = 0; is_first_refresh = 0; accpet_activitytime = 0; refresh_count = 0; is_use_hudun = false; reserve_1 = 0; reserve_2 = 0; }
	void Clear() { task_id = 0; task_color = 0; accpet_activitytime = 0; is_use_hudun = 0;}
	bool Invalid() { return 0 == task_id; }
	bool IsFirstRefresh() { return 0 == is_first_refresh; }

	UInt16 task_id;																		// ����ID 
	char task_color;																	// ������ɫ
	char is_first_refresh;																// ˢ�´���
	unsigned int accpet_activitytime;													// ��������ʱ �ʱ��
	int refresh_count;																	// ��������ˢ�´���
	char is_use_hudun;																	// �Ƿ�ʹ�ù�����
	char reserve_1;
	short reserve_2;
};

typedef char HusongTaskParamHex[sizeof(HusongTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(HusongTaskParamHex) < 64);



//////////////////////////////////////////////////////////  ����  ����  /////////////////////////////////////////////////////////////////

static const int FAIRY_BUDDHA_CARD_START_LEVEL = 150;								    // ���𿨿����ȼ�

static const int FAIRY_BUDDHA_CARD_START_DAY = 2;										// ���𿨿���ʱ�䣨�����ڼ��죩

static const int FAIRY_BUDDHA_CARD_CONTINUE_TIME = 60 * 60 * 24 * 30;					// ���𿨳���ʱ��Ϊ30��

static const int  FAIRY_BUDDHA_CARD_COUNT = 3;                                          // ���𿨼������α�Ϊ���ü���  

enum FairyBuddhaCardType
{
	FAIRY_BUDDHA_CARD_INVALID = 0,
	FAIRY_BUDDHA_CARD_BRONZE,             //��ͭ��
	FAIRY_BUDDHA_CARD_SILVER,             //������
	FAIRY_BUDDHA_CARD_JEWEL,              //��ʯ��
	FAIRY_BUDDHA_CARD_ALL,                //һ������
	FAIRY_BUDDHA_CARD_MAX,
};

struct FairyBuddhaCard
{
	FairyBuddhaCard() { this->Reset(); }
	void Reset() {
		bronze_timestamp = 0;
		silver_timestamp = 0;
		jewel_timestamp = 0;
		is_forever_open = 0;
		fairy_buddha_card_is_activate = 0;
		bronze_activate_count = 0;
		silver_activate_count = 0;
		jewel_activate_count = 0;
		gold_bind_is_get_flage = 0;
	}
	unsigned int bronze_timestamp;       // ��ͭ��ʱ���
	unsigned int silver_timestamp;       // ������ʱ���
	unsigned int jewel_timestamp;        // ��ʯ��ʱ���
	char is_forever_open;                // �Ƿ����ü���
	char fairy_buddha_card_is_activate;  // �����Ƿ񼤻�
	unsigned short bronze_activate_count;// ��ͭ���������
	unsigned short silver_activate_count;// �������������
	unsigned short jewel_activate_count; // ��ʯ���������
	int gold_bind_is_get_flage;          // ÿ�հ�Ԫ�Ƿ���ȡ
};

//////////////////////////////////////////////////////////  �����������  /////////////////////////////////////////////////////////////////
static const int MAX_GUILD_TASK_COLOR = 5;												// �����ɫ

static const int MAX_GUILD_REWARD_ITEM_COUNT = 64;

struct GuildTaskParam
{
	GuildTaskParam() { this->Reset(); }

	void Reset()
	{
		
		double_reward_flag = 0;
		is_finish = 0;
		first_task = 0;
		complete_task_count = 0;
		guild_task_max_count = 0;
	}

	char double_reward_flag;													// ˫��������ȡ���
	char is_finish;																// �������������Ƿ����
	short complete_task_count;													// �ܹ����������ͳ��

	int first_task;                  											// ��һ������ID

	int guild_task_max_count;													// ��������������

};

typedef char GuildTaskParamHex[sizeof(GuildTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GuildTaskParamHex) < 128);

//////////////////////////////////////////////////////////  Լ���������  /////////////////////////////////////////////////////////////////
struct DatingTaskParam
{
	DatingTaskParam() { this->Reset(); }

	void Reset()
	{
		task_id = 0;
	}

	int task_id;															// �ѽ�����ID
};

typedef char DatingTaskParamHex[sizeof(DatingTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(DatingTaskParamHex) < 32);

//////////////////////////////////////////////////////////  תְ�������  ////////////////////////////////////////////////////////////////
static const int ROLE_ZHUANZHI_TASK_TOTAL_COUNT_MAX = 33;					//��ɫ��תְ����������

struct ZhuanzhiTaskParam
{
	ZhuanzhiTaskParam() { this->Reset(); }

	void Reset()
	{
		first_task = 0;
		reserve_sh = 0;
		complete_task_count = 0;
	}

	short complete_task_count;													// �ܹ����������ͳ��
	short reserve_sh;
	int first_task;																// ��һ������ID
};

typedef char ZhuanzhiTaskParamHex[sizeof(ZhuanzhiTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ZhuanzhiTaskParamHex) < 64);

#pragma pack(pop)

#endif // __TASK_DEF_HPP__



