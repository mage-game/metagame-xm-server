/******************************************************************************************
FileName: countrydef.hpp
Author: dreamwang
Description: ������ض���  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __GUILD_DEF_HPP__
#define __GUILD_DEF_HPP__

#include <memory>
#include <map>
#include <vector>

#include "servercommon.h"
	
static const GuildID INVALID_GUILD_ID = 0;						// ��Ч�ľ���ID
static const int MAX_ONCE_GUILD_COUNT = 100;					// һ�β�������������
static const int GUILD_SAVE_INTERVAL = 60;						// ���ű�����
static const int GUILD_CLEAR_MEMBER_INTERVAL = 3;				// �Զ��������Ա���

static const int GUILD_LEVEL_MAX = 10;							// ������ߵȼ�

const static int GUILD_MAX_UPLEVEL_STUFF_COUNT = 8;				// ���������������

const static int GUILD_MAX_EXCHANGE_ITEM_COUNT = 30;			// �������һ���Ʒ����

static const int GUILD_FULI_GIFT_MAX = 6;						// ���6�����������ȡ

static const int GUILD_WARNING_DISMISS_DAY = 1;					// ���Ž�ɢ����Ϊ1��

static const int MAX_MEMBER_COUNT = 60;							// һ�������������
static const int MAX_APPLY_FOR_COUNT = 40;						// һ�����������б��������
static const int MAX_INVITE_COUNT = 40;							// ���ͬʱ��������

static const int MAX_COMMON_EVENT_COUNT = 50;					// ������ͨ�¼�����
static const int MAX_BIG_EVENT_COUNT = 0;						// ���Ŵ��¼�����
static const int MAX_STORAGE_EVENT_COUNT = 30;					// ���˲ֿ���־����
// �����¼�������  �洢˳��Ϊ��ͨ�¼�+���¼�+�ֿ���־�¼�
static const int MAX_ALL_GUILD_EVENT_COUNT = MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT + MAX_STORAGE_EVENT_COUNT;

static const int GUILD_UNION_MAX_GUILD_NUM = 2;					// �����������ľ��Ÿ���
static const int GUILD_UNION_APPLYFOR_MAX = 10;					// �������������б��С
static const int GUILD_UNION_ONCE_OPER_COUNT = 512;				// һ�β���������������
static const int GUILD_UNION_SAVE_INTERVAL = 60;				// �������˱�����

static const int GUILD_STORAGE_MAX_GRID_NUM = 102;				// ���Ųֿ������
static const int GUILD_STORAGE_CFG_MAX_NUM = 10;				// ���Ųֿ����������

static const int GUILD_LOCK_RECOVER_TIME = 3;					// �������ָ�ʱ��

static const int GUILD_ACHIEVE_ID_MAX = 64;						// ���ųɾ�ID���ֵ
static const int GUILD_ACHIEVE_REWARD_ITEM_MAX = 3;				// ���ųɾͽ�����Ʒ�����

static const int GUILD_JIANSHE_COIN_ONCE_CONSUME = 1000;		// ���Ž���� ͭ��һ������1000

static const int GUILD_AUTO_KICKOUT_NEED_MEMBER_COUNT = 10;		// �����Զ�������Ҫ��Ա����
static const int GUILD_AUTO_KICKOUT_OFFLINE_TIME = 1800;		// �����Զ�������������ʱ��

static const int GUILD_BOSS_MAX_LEVEL = 20;						// ����boss���ȼ�

static const int GUILD_DAY_INVITE_COUNT = 5;					// ����ÿ�ճɹ������������

static const int GUILD_SKILL_COUNT = 7;							// ���˼�����
static const int MAX_GUILD_SKILL_LEVEL = 200;					// ���˼������ȼ�
static const int MAX_GUILD_TOTEM_LEVEL = 100;					// ����ͼ�����ȼ�

static const int MAX_GUILD_SAIZI_REWARD = 10;					// ���ӽ��������
static const int GUILD_SEND_SAIZI_RANK_MSG_INTERVAL = 7200;		// �������ӵĹ�����Ϣ��ʱ����

static const int GUILD_ENEMY_RANK_MAX_COUNT = 100;				// ���˳��������������

//static const char *GUILD_DEFAULT_PLAT_NAME = "default";				// ��������ʱ��Ĭ��ƽ̨��

// ����ְλ����
enum GUILD_POST_TYPE
{
	GUILD_POST_INVALID = 0,

	GUILD_POST_CHENG_YUAN,										// ��Ա
	GUILD_POST_ZHANG_LAO,										// ����
	GUILD_POST_FU_TUANGZHANG,									// ���ų�
	GUILD_POST_TUANGZHANG,										// �ų�
	GUILD_POST_JINGYING,										// ��Ӣ��Ա
	GUILD_POST_HUFA,											// ����

	GUILD_POST_MAX,
};

// ��ȡ��Ϣ����
enum GUILD_INFO_TYPE
{
	GUILD_INFO_TYPE_INVALID,

	GUILD_INFO_TYPE_ALL_GUILD_BASE_INFO,						// ���о��Ż�����Ϣ
	GUILD_INFO_TYPE_GUILD_APPLY_FOR_INFO,						// ���������б�
	GUILD_INFO_TYPE_GUILD_MEMBER_LIST,							// ���ų�Ա�б�
	GUILD_INFO_TYPE_GUILD_INFO,									// ������Ϣ
	GUILD_INFO_TYPE_GUILD_EVENT_LIST,							// ������־�б�
	GUILD_INFO_TYPE_APPLY_FOR_JOIN_GUILD_LIST,					// ���������ľ����б�
	GUILD_INFO_TYPE_INVITE_LIST,								// �����б�
	GUILD_INFO_TYPE_GONGZI_LIST,								// �����б�

	GUILD_INFO_TYPE_MAX
};

// ���Ŵ�����ʽ
enum GUILD_CREATE_TYPE
{
	GUILD_CREATE_TYPE_INVALID = 0,

	GUILD_CREATE_TYPE_GOLD,										// Ԫ��
	GUILD_CREATE_TYPE_ITEM,										// ������

	GUILD_CREATE_TYPE_MAX
};

// ������������
enum GUILD_APPLYFOR_SETUP_TYPE
{
	GUILD_APPLYFOR_SETUP_NEED_AGREE = 0,						// ��Ҫͬ��
	GUILD_APPLYFOR_SETUP_FORBIT,								// ��ֹ����
	GUILD_APPLYFOR_CONDITION,									// ��������ͨ��
	
	GUILD_APPLYFOR_TYPE_MAX,
};

// ����buff����
enum GUILD_BUFF_TYPE											
{
	GUILD_BUFF_TYPE_INVALID = 0,

	GUILD_BUFF_TYPE_ADD_HP_PER,									// ������Ѫ����
	GUILD_BUFF_TYPE_ADD_GONGJI_PER,								// ���ӹ���
	GUILD_BUFF_TYPE_ADD_FANGYU_PER,								// ���ӷ���
	GUILD_BUFF_TYPE_ADD_HP_GONGJI_FANGYU_PER,					// ������Ѫ,����������

	GUILD_BUFF_TYPE_MAX,
};

// ��ɫ������״̬����
enum GUILD_ROLE_RED_PAPER_STATE
{
	ROLE_RED_PAPER_INVALID = 0,
	ROLE_RED_PAPER_HAS_AUTHORITY,									// �з������Ȩ��,��δ�й��ᣬ���ܷ�
	ROLE_RED_PAPER_CAN_CREATE,										// �ڹ��ᣬ�ܹ����ͺ��
};

// ������״̬����
enum GUILD_RED_PAPER_STATE
{
	RED_PAPER_INVALID = 0,
	RED_PAPER_CAN_CREATE,											// �ܹ����ĺ��
	RED_PAPER_CAN_FETCH,											// �ܹ����ĺ��
	RED_PAPER_HAS_FETCH,											// �����ĺ��
};

enum GUILD_CREATE_CONSUME_TYPE
{
	GUILD_CREATE_CONSUME_TYPE_BIND_GOLD = 0,						// ��Ԫ(����ʱ����Ԫ������)
	GUILD_CREATE_CONSUME_TYPE_GOLD = 1,								// Ԫ��
};

// ��ɫ�������
struct RoleGuildParam
{
	RoleGuildParam() { this->Reset(); }

	void Reset()
	{
		ClearExchangeRecord();

		memset(skill_level_list, 0, sizeof(skill_level_list));
		territorywar_reward_flag = 0;

		last_leave_guild_time = 0;
		last_join_guild_time = 0;
		daily_gongxian = 0;
		storage_score = 0;
		guild_boss_activity_totem_total_exp = 0;
		be_kill_last_notice_time = 0;
		maze_reward_layer = 0;
		
		memset(guild_red_paper_list, 0, sizeof(guild_red_paper_list));

		guild_tianci_tongbi_reward_limit = 0;
		today_paosaizi_times = 0;
		reserve_sh = 0;
		join_tiancitongbi_guild_id = 0;
		today_juanxian_gold = 0;

		world_ques_rank = -1;
		charm_rank = -1;
		profess_rank = -1;
		qingyuan_rank = -1;
		baby_rank = -1;
		li_pet_rank = -1;
		kill_count = 0;
		killed_count = 0;
		next_notice_level = 0;
	}

	void ClearExchangeRecord()
	{
		exchange_record_total_num = 0;
		memset(exchange_record_list, 0, sizeof(exchange_record_list));
	}

	struct ExchangeRecordItem
	{
		ExchangeRecordItem() : item_id(0), item_num(0) {}

		ItemID item_id;
		short item_num;
	};

	void SetTerritoryWelfFlag(int index)
	{
		territorywar_reward_flag |= (1 << index);
	}

	bool IsFetchtTerritoryWelfReward(int index)
	{
		return 0 != (territorywar_reward_flag & (1 << index));
	}

	struct GuildRedPaper
	{
		GuildRedPaper(): red_paper_state(0), red_paper_seq(0), invalid_time(0){}

		short red_paper_state;
		short red_paper_seq;
		unsigned int invalid_time;
	};

	int exchange_record_total_num;
	ExchangeRecordItem exchange_record_list[GUILD_MAX_EXCHANGE_ITEM_COUNT];

	short skill_level_list[GUILD_SKILL_COUNT];
	short territorywar_reward_flag;

	unsigned int last_leave_guild_time;
	unsigned int last_join_guild_time;
	int daily_gongxian;
	int storage_score;
	long long guild_boss_activity_totem_total_exp;						// ����boss�����ͼ�ھ���
	unsigned int be_kill_last_notice_time;
	int maze_reward_layer;

	GuildRedPaper guild_red_paper_list[MAX_ROLE_GUILD_RED_PAPER];	// �������б�

	char guild_tianci_tongbi_reward_limit;							// ���ͭ�Ҳɼ���þ������
	unsigned char today_paosaizi_times;								// ���������Ӵ���
	short reserve_sh;						
	GuildID join_tiancitongbi_guild_id;								// ����μӹ����ͭ�ҵİ���id
	int today_juanxian_gold;										// �����Ѿ���Ԫ��

	char world_ques_rank;											//������������������
	char charm_rank;												//���������������
	char profess_rank;												//������߱������
	char qingyuan_rank;												//���������Ե����
	char baby_rank;													//������߱�������
	char li_pet_rank;												//�������С��������
	short kill_count;												//Ұ���ɱ��Ҵ���
	short killed_count;												//Ұ�ⱻ��ɱ����
	short next_notice_level;										//��һ�ν������Ž�ɫ�ȼ�
};

typedef char RoleGuildParamHex[sizeof(RoleGuildParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleGuildParamHex) < 2048);

//����
static const int GUILD_BOSS_GRADE_MAX = 3;						// ���޽׶���
static const int GUILD_BOSS_LEVEL_MAX = 200;					// ����������ߵȼ�

static const int GUILD_BOSS_LEVEL_CFG_MAX_NUM = 10;				// ����������ø���10�ȼ���

static const int GUILD_BOSS_LIVE_INTERVAL_S = 1800;				// �������޴��ʱ��
static const int GUILD_BOSS_DROP_ITEM_CFG_MAX_NUM = 18;			// ������Ʒ������ø���
static const int GUILD_BOSS_DROP_ITEM_MAX_NUM = 10;				// �������޵�����Ʒ������
UNSTD_STATIC_CHECK(GUILD_BOSS_DROP_ITEM_MAX_NUM <= GUILD_BOSS_DROP_ITEM_CFG_MAX_NUM);

static const int GUILD_BOSS_GIVE_ITEM_ROLL_INTERVAL_S = 10;		// ���޵�����Ʒҡ����
static const int GUILD_BOSS_ROLL_SCORE_MAX = 100;				// ���޵���ҡ�����ֵ
static const int GUILD_BOSS_DPS_RANK_NUM = 5;					// �����˺�������Ŀ
static const int GUILD_BOSS_TOP_DPS_REWARD_NUM = 3;				// �����˺�ǰ���������⾭�齱������Ʒ����Ĭ�ϸ���
static const int GUILD_CHANGE_NOTICE_CD_TIME = 2;				// �޸����˹���CD
static const int GUILD_MAIL_ALL_CD_TIME = 2;					// �����ʼ�CD

// ����״̬
enum GUILD_BOSS_STATE
{
	GUILD_BOSS_STATE_INVALID = 0,
	GUILD_BOSS_STATE_CALL,										// ���ٻ�״̬��
	GUILD_BOSS_STATE_MAX,
};

// ����ι����ʽ
enum GUILD_BOSS_FEED_TYPE
{
	GUILD_BOSS_FEED_TYPE_INVALID = 0,
	GUILD_BOSS_FEED_TYPE_NORMAL,								// ��ͨι��
	GUILD_BOSS_FEED_TYPE_GOLD,									// Ԫ��ι��
	GUILD_BOSS_FEED_TYPE_MAX,
};

struct GuildBossDPSUser
{
	GuildBossDPSUser() : obj_id(0), user_id(INVALID_USER_ID), user_level(0), user_camp(0), hurt(0), roll_timestamp(0), roll_score(0), rank_pos(0), have_got_item(false)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	ObjID obj_id;
	UserID user_id;
	GameName user_name;
	int user_level;
	int user_camp;
	int hurt;
	unsigned int roll_timestamp;
	int roll_score;
	int rank_pos;
	bool have_got_item;
};
typedef std::map<UserID, GuildBossDPSUser> GuildBossDPSUserMap;
typedef std::map<UserID, GuildBossDPSUser>::iterator GuildBossDPSUserMapIt;

struct GuildBossDPSRankItem
{
	GuildBossDPSRankItem() : uid(0), hurt(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int hurt;
};
typedef std::vector<GuildBossDPSRankItem> GuildBossDPSRankItemVec;						
typedef std::vector<GuildBossDPSRankItem>::iterator GuildBossDPSRankItemVecIt;

// ����ǩ����ʽ
enum GUILD_SIGNIN_TYPE
{
	GUILD_SIGNIN_TYPE_INVALID = 0,
	GUILD_SIGNIN_TYPE_PUTONG,									// ��ͨǩ��
	GUILD_SIGNIN_TYPE_BAIJIN,									// �׽�ǩ��
	GUILD_SIGNIN_TYPE_ZHIZUN,									// ����ǩ��
	GUILD_SIGNIN_TYPE_MAX
};

// ������־����
enum GUILD_EVENT_TYPE
{
	GUILD_EVENT_TYPE_INVALID = 0,

	GUILD_EVENT_TYPE_CREATE,									// ��������
	GUILD_EVENT_TYPE_MEMBER_ADD,								// ��Ա����
	GUILD_EVENT_TYPE_MEMBER_QUIT,								// �˳�����
	GUILD_EVENT_TYPE_MEMBER_KICK,								// �߳�����
	GUILD_EVENT_TYPE_MEMBER_APPOINT,							// ��Ա����
	GUILD_EVENT_TYPE_MEMBER_LEAVE_POST,							// ж��
	GUILD_EVENT_TYPE_TRANSFER_TUANZHANG,						// ת�þ��ų�
	GUILD_EVENT_TYPE_DELATE_TUANZHANG,							// �������ų�
	GUILD_EVENT_TYPE_UPGRADE_LEVEL,								// ��������
	GUILD_EVENT_TYPE_STORAGE_OPERATE,							// �ֿ����		// 2019-04-18 ��Ϊ�ֿ������־�¼�
	GUILD_EVENT_TYPE_SET_STORAGESHOP_USE,						// ����Ȩ������
	GUILD_EVENT_TYPE_CALL_BOSS,									// �ٻ�����
	GUILD_EVENT_TYPE_GUILD_UNION,								// ���˽�� ���
	GUILD_EVENT_TYPE_FEED_BOSS,									// ι������
	GUILD_EVENT_TYPE_MEMBER_SIGNIN,								// ��Աǩ��
	GUILD_EVENT_TYPE_DOWNGRADE_LEVEL,							// ���Ž���
	GUILD_EVENT_TYPE_ADD_EXP,									// ��Ա����
	GUILD_EVENT_TYPE_RUMOR_UPLEVEL = 18,						// ��������-��ɫ����
	GUILD_EVENT_TYPE_RUMOR_ZHUANZHI = 19,						// ��������-��ɫתְ
	GUILD_EVENT_TYPE_RUMOR_MARRY = 20,							// ��������-��ɫ���
	GUILD_EVENT_TYPE_RUMOR_DIVORCE = 21,						// ��������-��ɫ���
	GUILD_EVENT_TYPE_RUMOR_BABY = 22,							// ��������-������
	GUILD_EVENT_TYPE_RUMOR_BUY_HOUSE = 23,						// ��������-��
	GUILD_EVENT_TYPE_RUMOR_WEAPON_FB = 24,						// ��������-������ͨ�ص�X��
	GUILD_EVENT_TYPE_RUMOR_ARMDEFEND_FB = 25,					// ��������-���߱�ͨ�ص�X��
	GUILD_EVENT_TYPE_RUMOR_PATA_FB = 26,						// ��������-������ͨ�ص�X��
	GUILD_EVENT_TYPE_RUMOR_RUNE_TOWER_FB = 27,					// ��������-ս����ͨ�ص�X��
	GUILD_EVENT_TYPE_RUMOR_GET_RED_EQUIP = 28,					// ��������-�����Xװ��
	GUILD_EVENT_TYPE_RUMOR_KILLED = 29,							// ��������-��ɱX��
	GUILD_EVENT_TYPE_RUMOR_KILL = 30,							// ��������-��ɱX��
	GUILD_EVENT_TYPE_RUMOR_BE_FAMOUS_MAN = 31,					// ��������-������
	GUILD_EVENT_TYPE_RUMOR_BE_LOVER_RANK = 32,					// ��������-���°�
	GUILD_EVENT_TYPE_RUMOR_BE_BABY_RANK = 33,					// ��������-������
	GUILD_EVENT_TYPE_RUMOR_LITTLE_PET_RANK = 34,				// ��������-С�����
	GUILD_EVENT_TYPE_RUMOR_CHARM_RANK = 35,						// ��������-������
	GUILD_EVENT_TYPE_RUMOR_WORLD_QUESTION_RANK = 36,			// ��������-��������
	GUILD_EVENT_TYPE_RUMOR_EXPRESS_RANK = 37,					// ��������-��װ�
	GUILD_EVENT_TYPE_RUMOR_GUILD_FB = 38,						// ��������-����������ʼ
	GUILD_EVENT_TYPE_RUMOR_GUILD_FB_RESULT = 39,				// ��������-��������������X�������Y��
	GUILD_EVENT_TYPE_RUMOR_GUILD_QUESTION = 40,					// ��������-���˴��⿪ʼ
	GUILD_EVENT_TYPE_RUMOR_GUILD_QUESTION_RESULT = 41,			// ��������-���˴��⣬������ȡ�õ�X����Y�Ǵ�����������˳�Ա
	GUILD_EVENT_TYPE_RUMOR_GUILD_YUNBIAO = 42,					// ��������-�������ڿ�ʼ
	GUILD_EVENT_TYPE_RUMOR_YUNBIAO_SUCC = 43,					// ��������-���ڳɹ�
	GUILD_EVENT_TYPE_RUMOR_YUNBIAO_ROBBED = 44,					// ��������-������
	GUILD_EVENT_TYPE_RUMOR_GUILD_QIANGQI = 45,					// ��������-�������쿪ʼ
	GUILD_EVENT_TYPE_RUMOR_GUILD_QIANGQI_RESULT = 46,			// ��������-�����������������˻�õ�X����Y�Ǳ��˻�������
	GUILD_EVENT_TYPE_RUMOR_GONGCHENG_START = 47	,				// ��������-���˹��ǿ�ʼ
	GUILD_EVENT_TYPE_RUMOR_GONGCHENG_SUCC = 48,					// ��������-���˹��ǳɹ���X��Ϊ������Y�ǻ�������Ա
	GUILD_EVENT_TYPE_RUMOR_GONGCHENG_FAIL = 49,					// ��������-���˹���ʧ��,Y�ǻ�������Ա
	GUILD_EVENT_TYPE_RUMOR_CROSS_GUILD_BATTLE = 50,				// ��������-�������ս��ʼ
	GUILD_EVENT_TYPE_RUMOR_CROSS_GUILD_BATTLE_SUCC = 51,		// ��������-�������ս�ɹ����ɹ�ռ��X��Y�ǻ�������Ա
	GUILD_EVENT_TYPE_RUMOR_CROSS_GUILD_BATTLE_FAIL = 52,		// ��������-�������սʧ�ܣ� Y�ǻ�������Ա
	GUILD_EVENT_TYPE_RUMOR_FB_TOWER_DEFEND = 53,				// ��������-�ػ���
	GUILD_EVENT_TYPE_RUMOR_FB_CHALLENGE = 54,					// ��������-Ʒ�ʱ�

	GUILD_EVENT_TYPE_MAX
};

enum GUILD_EVENT_ADD_EXP_TYPE
{
	GUILD_EVENT_ADD_EXP_TYPE_INVALID = 0,

	GUILD_EVENT_ADD_EXP_TYPE_GOLD,								// Ԫ������
	GUILD_EVENT_ADD_EXP_TYPE_ITEM,								// ��Ʒ����

	GUILD_EVENT_ADD_EXP_TYPE_MAX,
};

// ���˲ֿ��������
enum GUILD_STORE_OPTYPE
{
	GUILD_STORE_OPTYPE_TAKEOUT = 0,								// �Ӿ��Ųֿ�ȡ��
	GUILD_STORE_OPTYPE_PUTIN,									// ������Ųֿ�
	//GUILD_STORE_OPTYPE_DESTROY,								// ���پ��Ųֿ���Ʒ
};

enum GUILD_JIANZHU_TYPE
{
	GUILD_JIANZHU_TYPE_SHENLU = 0,								// ���Ž��� ��¯ �����0��ʼ
	GUILD_JIANZHU_TYPE_SHOP,									// ���Ž��� �̵�
	GUILD_JIANZHU_TYPE_CANGKU,									// ���Ž��� �ֿ�
	GUILD_JIANZHU_TYPE_YAOJIANG,								// ���Ž��� ҡ��

	GUILD_JIANZHU_TYPE_MAX,									
};

struct GuildJianzhuInfo
{
	GuildJianzhuInfo() : level(0), exp(0) {}

	int level;													// ���� �ȼ�
	int exp;													// ���� ����
};

// ���ųɾ�����
enum GUILD_ACHIEVE_TYPE
{
	GUILD_ACHIEVE_TYPE_INVALID = 0,

	GUILD_ACHIEVE_TYPE_GUILD_LEVEL,								// ���ŵȼ�
	GUILD_ACHIEVE_TYPE_MEMBER_COUNT,							// ���ų�Ա��
	GUILD_ACHIEVE_TYPE_KILL_WORLD_BOSS,							// ��ɱ����BOSS
	GUILD_ACHIEVE_TYPE_KILL_GUILD_BOSS,							// ��ɱ��������
	GUILD_ACHIEVE_TYPE_MEMBER_SIGNIN,							// ���ų�Ա�ۼ�ǩ������
	GUILD_ACHIEVE_TYPE_FEED_GUILD_BOSS,							// �ۼ�ι���������޴���

	GUILD_ACHIEVE_TYPE_MAX
};

// ���Ų�������
enum GUILD_OPER_TYPE
{
	GUILD_OPER_TYPE_INVALID = 0,
	GUILD_OPER_TYPE_DISMISS,									// ��ɢ
	GUILD_OPER_TYPE_QUIT,										// �˳�
	GUILD_OPER_TYPE_KICK,										// ����
	GUILD_OPER_TYPE_CREATE,										// ����
	GUILD_OPER_TYPE_APPOINT,									// ����

	GUILD_OPER_TYPE_MAX
};

// ����GM��������
enum GUILD_GM_OPER_TYPE
{
	GUILD_GM_OPER_TYPE_INVALID = 0,
	GUILD_GM_OPER_TYPE_DISMISS,									// ��ɢ
	GUILD_GM_OPER_TYPE_CLEAR_NOTICE,							// �������

	GUILD_GM_OPER_TYPE_MAX
};

// ���Ž�ɢ����
enum GUILD_DISMISS_TYPE
{
	GUILD_DISMISS_TYPE_DEFAULT = 0,								// �ֶ���ɢ
	GUILD_DISMISS_TYPE_AUTO,									// �Զ���ɢ
	GUILD_DISMISS_TYPE_GM,										// GM ��ɢ
};

typedef char GuildEventStrParam[32];

UNSTD_STATIC_CHECK(sizeof(GuildEventStrParam) >= sizeof(GameName));
UNSTD_STATIC_CHECK(sizeof(GuildEventStrParam) >= sizeof(GuildName));

enum GUILD_SOS_TYPE
{
	GUILD_SOS_TYPE_DEFAULT,										// Ĭ�����
	GUILD_SOS_TYPE_HUSONG,										// �������
	GUILD_SOS_TYPE_HUSONG_BE_ATTACK,							// ������� - ������
	GUILD_SOS_TYPE_GUILD_BATTLE,								// ��������
	GUILD_SOS_TYPE_GONGCHENGZHAN,								// ����ս
	GUILD_SOS_TYPE_CROSS_GUILD_BATTLE,							// ����
	GUILD_SOS_TYPE_CROSS_BIANJINGZHIDI,							// ����߾�֮��
	GUILD_SOS_TYPE_CROSS_BOSS,									// ���Զ��BOSS
	GUILD_SOS_TYPE_CROSS_MIZANG_BOSS,							// �������BOSS
	GUILD_SOS_TYPE_CROSS_VIP_BOSS,								// ���VIPBOSS
};

#pragma pack(push, 4)

struct GuildSyncInfo
{
	GuildSyncInfo() : guild_id(0), tuanzhang_uid(0), guild_level(0), camp(0), member_count(0), guild_task_score(0), 
		personal_tower_defend_max_wave(0), personal_equip_fb_max_layer(0), today_call_shenjiang_flag(0)
	{
		memset(guild_name, 0, sizeof(guild_name));
		memset(tuanzhang_name, 0, sizeof(tuanzhang_name));
	}

	GuildID guild_id;
	GuildName guild_name;
	int tuanzhang_uid;
	GameName tuanzhang_name;
	int guild_level;
	int camp;
	int member_count;
	int guild_task_score;
	int personal_tower_defend_max_wave;
	int personal_equip_fb_max_layer;
	int today_call_shenjiang_flag;

	GuildJianzhuInfo jianzhu_info_list[GUILD_JIANZHU_TYPE_MAX];		
};

struct GuildXianshuParam
{
	GuildXianshuParam() { this->Reset(); }

	void Reset()
	{
		xianshu_level = 0;
		reserve = 0;
	}

	short xianshu_level;														// �����ȼ�
	short reserve;																// ����
};

typedef char GuildXianshuParamHex[sizeof(GuildXianshuParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GuildXianshuParamHex) < 16);

// ��������
static const int GUILD_TOWER_DEFEND_DAY_COUNT_MAX = 1;							// ��������ÿ�մ���
static const int MAX_TOWER_DEFEND_DIRECTION = 4;								// ����������������
static const int MAX_TOWER_DEFEND_WAVE = 80;									// �������������ﲨ��
static const int MAX_TOWER_DEFEND_SKILL_COUNT = 4;								// ����������������

struct GuildTowerDefendUser
{
	GuildTowerDefendUser() : obj_id(0), user_id(INVALID_USER_ID), total_longhun(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(skill_cd_list, 0, sizeof(skill_cd_list));
	}

	ObjID obj_id;
	UserID user_id;
	GameName user_name;
	int total_longhun;
	unsigned int skill_cd_list[MAX_TOWER_DEFEND_SKILL_COUNT];
};
typedef std::map<UserID, GuildTowerDefendUser> GuildTowerDefendUserMap;
typedef std::map<UserID, GuildTowerDefendUser>::iterator GuildTowerDefendUserMapIt;

struct GuildStuffItem
{
	GuildStuffItem() : item_id(0), reserve(0), count(0) {}

	ItemID item_id;
	short reserve;
	
	int count;
};

struct GuildBossItem
{
	GuildBossItem() : killer_roleid(0), reserve1(0)
	{
		memset(killer_name, 0, sizeof(killer_name));
	}

	bool IsBeKilled()
	{
		return killer_roleid != 0;
	}
	
	int killer_roleid;
	GameName killer_name;

	int reserve1;
};

// ���˱���
static const int MAX_GUILD_BOX_COUNT = 8;										// ���˱����������
static const int MAX_GUILD_BOX_LEVEL = 4;										// ���˱������ȼ�
static const int MAX_TERRITORY_WELF_COUNT = 10;									// ����������������
static const int MAX_GUILD_BOX_BE_ASSIST_COUNT = 4;								// ���˱���ɱ�Э��������

struct GuildBoxItem
{
	GuildBoxItem() : box_level(0), is_reward(0), open_time(0) 
	{
		memset(assist_uid, 0, sizeof(assist_uid));
		memset(is_thank_assist_uid, 0, sizeof(is_thank_assist_uid));
	}

	short box_level;
	short is_reward;								// �Ƿ���ȡ
	int assist_uid[MAX_GUILD_BOX_BE_ASSIST_COUNT];
	char is_thank_assist_uid[MAX_GUILD_BOX_BE_ASSIST_COUNT]; // �Ƿ��Ѹ�л
	unsigned int open_time;
};

struct GuildBoxInfo
{
	GuildBoxInfo() : dayid(0), uplevel_count(0), assist_count(0), be_assist_count(0), reserve_sh(0), assist_cd_end_time(0) {}

	void Reset()
	{
		dayid = 0;
		uplevel_count = 0;
		assist_count = 0;
		assist_cd_end_time = 0;
		be_assist_count = 0;
		reserve_sh = 0;

		memset(&info_list, 0, sizeof(info_list));
	}

	unsigned int dayid;
	short uplevel_count;
	short assist_count;
	short be_assist_count;
	short reserve_sh;
	unsigned int assist_cd_end_time;
	GuildBoxItem info_list[MAX_GUILD_BOX_COUNT];
};
typedef char GuildBoxInfoHex[sizeof(GuildBoxInfo) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GuildBoxInfoHex) < 512);

// ��������-------------------------------------------------------------------------

enum GUILD_YUNBIAO_RESULT_TYPE
{
	GUILD_YUNBIAO_RESULT_TYPE_FAIL = 0,		// ����ʧ�� 
	GUILD_YUNBIAO_RESULT_TYPE_SUCC,			// ���ڳɹ�
	GUILD_YUNBIAO_RESULT_TYPE_ROB_SUCC,		// �������ٳɹ�
};

// ���ͭ�Ҳɼ�������---------------------------------------------------------------
enum GUILD_TIANCI_TONGBI_GATHER_TYPE
{
	GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID = 0,
	GUILD_TIANCI_TONGBI_GATHER_TYPE_XIAPING,					// ���ͭ�Ҳɼ�����Ʒ
	GUILD_TIANCI_TONGBI_GATHER_TYPE_ZHONGPING,					// ���ͭ�Ҳɼ�����Ʒ
	GUILD_TIANCI_TONGBI_GATHER_TYPE_SHANGPING,					// ���ͭ�Ҳɼ�����Ʒ
	GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX,
};

static const int GUILD_TIANCI_TONGBI_DAY_COUNT_MAX = 1;				// �������ͭ��ÿ�մ���
static const int GUILD_TIANCI_TONGBI_GATHER_REFRESH_POS_MAX = 30;	// �������ͭ�Ҳɼ���ˢ�µ�����
static const int GUILD_TIANCI_TONGBI_BIGCOIN_FLUSH_POS_MAX = 30;	// �������ͭ��ͭ��ˢ�µ�����
static const int GUILD_TIANCI_TONGBI_GAHTER_NUM_MAX = 30;			// �������ͭ�Ҳɼ����������
static const int GUILD_TIANCI_TONGBI_RANK_NUM = 5;					// �������ͭ��������Ŀ

struct GuildTianCiTongBiUser
{
	GuildTianCiTongBiUser() : obj_id(0), user_id(INVALID_USER_ID), user_level(0), user_camp(0), /*reward_item_id(0),*/ reward_mojing_num(0), gold_bind_num(0), total_gather_exp_count(0), total_exp(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(gather_count_list, 0, sizeof(gather_count_list));
	}

	ObjID obj_id;
	UserID user_id;
	GameName user_name;
	int user_level;
	int user_camp;
	//int reward_item_id;
	int reward_mojing_num;
	int gold_bind_num;
	int total_gather_exp_count;
	long long total_exp;
	int gather_count_list[GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX];
};
typedef std::map<UserID, GuildTianCiTongBiUser> GuildTianCiTongBiUserMap;
typedef std::map<UserID, GuildTianCiTongBiUser>::iterator GuildTianCiTongBiUserMapIt;

struct GuildTianCiTongBiRankItem
{
	GuildTianCiTongBiRankItem() : uid(0), reward_mojing_num(0), coin_bind(0), rank_info(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int reward_mojing_num;
	int coin_bind;
	int total_gather_exp_count;
	int rank_info;
};
typedef std::vector<GuildTianCiTongBiRankItem> GuildTianCiTongBiRankItemVec;
typedef std::vector<GuildTianCiTongBiRankItem>::iterator GuildTianCiTongBiRankItemVecIt;

// ��������---------------------------------------------------------------
struct HurtMonsterInfo
{
	HurtMonsterInfo() : role_id(0), hurt_val(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	bool operator <(const HurtMonsterInfo &other) const
	{
		return hurt_val > other.hurt_val;
	}

	int role_id;
	GameName user_name;
	long long hurt_val;
};

// ���˹���---------------------------------------------------------------

// ���ʻ������
enum GUILD_GONGZI_GIVE_TYPE
{
	GUILD_GONGZI_GIVE_TYPE_INVALID = 0,

	GUILD_GONGZI_GIVE_TYPE_NORMAL,						// ���ˡ�����
	GUILD_GONGZI_GIVE_TYPE_ONLY_GUILD,					// ������

	GUILD_GONGZI_GIVE_TYPE_MAX,
};

struct MemberGongziInfo
{
	MemberGongziInfo()
	{
		memset(role_name, 0, sizeof(role_name));
	}

	bool operator <(const MemberGongziInfo &other) const
	{
		return gongzi > other.gongzi;
	}

	int				uid = 0;
	GameName		role_name;
	char			post = 0;
	char			reserve_ch = 0;
	short			gongzi_rate = 0;		// ���ű���
	int				gongzi = 0;				// ����
	int				gold_bind = 0;			// ��Ԫ
};


// ���˳��� ---------------------------------------------------------------

// ���˳�����Ϣ
struct GuildEnemyInfo
{
	GuildEnemyInfo() { this->Reset(); }

	void Reset()
	{
		enemy_uuid = 0;
		memset(enemy_name, 0, sizeof(enemy_name));
		kill_score = 0;
		last_kill_timestamp = 0;

		reserve_ll = 0;
		reserve_ll2 = 0;
	}

	long long		enemy_uuid;		// ����uuid
	GameName		enemy_name;		// ��������
	int				kill_score;		// ���ֵ
	unsigned int	last_kill_timestamp; // �����ϴλ�ɱ��Ա��ʱ��

	long long reserve_ll;
	long long reserve_ll2;
};


#pragma pack(pop)

#endif  // __GUILD_DEF_HPP__

