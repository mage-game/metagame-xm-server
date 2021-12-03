#ifndef __CHALLENGE_FIELD_DEF_HPP__
#define __CHALLENGE_FIELD_DEF_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"

#include "servercommon/struct/skilllistparam.h"
#include "servercommon/xianjiandef.hpp"
#include "servercommon/maildef.hpp"

#include "gameworld/gameworld/gamedef.h"

static const int CHALLENGE_FIELD_MAX_USER = 50000;						// �����Ҹ���
static const int CHALLENGE_FIELD_OPPONENT_MAX = 4;						// ���ָ���
const static int CHALLENGE_FIELD_MAX_USER_NUM = 500;					// ����������������������Э�飩

static const int CHALLENGE_FIELD_SAVE_INTERVAL = 30;					// ������

static const int CHALLENGE_FIELD_ONCE_OPER_USER_COUNT = 256;			// ������һ�β�����Ҹ���
static const int CHALLENGE_FIELD_INCREASE_STEP = 512;					// ����������б�����������

static const int CHALLENGE_FIELD_WAITING_TIME = 12;						// �������ȴ���ʱʱ��
static const int CHALLENGE_FIELD_READY_TIME = 4;						// ������׼��ʱ��
static const int CHALLENGE_FIELD_FIGHT_TIME = 60;						// ����������ʱ��
static const int CHALLENGE_FIELD_DELAY_KICK_TIME = 10;					// �������ӳ�����ʱ��

static const int CHALLENGE_FIELD_TOTAL_TIME = CHALLENGE_FIELD_READY_TIME + CHALLENGE_FIELD_FIGHT_TIME;

static const int CHALLENGE_FIELD_REPORT_MAX_COUNT = 20;					// ����ս���������
static const int CHALLENGE_FIELD_JIFEN_REWARD_MAX_COUNT = 32;			// ���ֽ��������
static const int CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT = 10;			// �������������
static const int CHALLENGE_FIELD_TOP_RANKER_ITEM_NUM = 100;				// ��ע��ǰ100

static const int CHALLENGE_FIELD_GIVE_RANK_REWARD_HOUR = 19;			// ��������������Сʱ��19�㣩

const static int CHALLENGE_FIELD_ROBOT_SERVER_ID = 2047;				// �����˱��
const static int CHALLENGE_FIELD_ROBOT_PLAT_TYPE = 0;					// ������ƽ̨���

static const int CHALLENGE_FIELD_RECORD_RANK_POS_MAX_COUNT = 10;		// ��¼�����������

/////////////////////////////////////// ��������� //////////////////////////////////////////////////////
static const int CROSS_CHALLENGE_FIELD_MAX_USER = 1000;					// ��������������Ҹ���
static const int CROSS_CHALLENGE_FIELD_SAVE_INTERVAL = 1;				// ���������ͬ�����
static const int CROSS_CHALLENGE_FIELD_ONCE_OPER_USER_COUNT = 3;		// ���������һ��ͬ��������Ҹ���

enum CHALLENGE_FIELD_SIDE
{
	CHALLENGE_FIELD_SIDE_SPONSOR = 0,					// ����
	CHALLENGE_FIELD_SIDE_OPPONENT,						// �Է�
	CHALLENGE_FIELD_SIDE_MAX,
};

enum CHALLENGE_FIELD_STATE
{
	CHALLENGE_FIELD_STATE_WAITING = 0,					// ���𷽵ȴ���
	CHALLENGE_FIELD_STATE_READY,						// ׼��
	CHALLENGE_FIELD_STATE_FIGHTING,						// ս����ʼ
	CHALLENGE_FIELD_STATE_FINISH,						// ս�����
};

#pragma pack(push, 4)

struct ChallengeParam
{
	ChallengeParam() { this->Reset(); }

	void Reset()
	{
		plat_type = 0;
		role_id = 0;
		memset(role_name, 0, sizeof(role_name));
		avatar = 0;
		sex = 0;
		prof = 0;
		camp = 0;
		level = 0;

		capability = 0;

		max_hp = 0;
		gongji = 0;
		fangyu = 0;
		mingzhong = 0;
		shanbi = 0;
		baoji = 0;
		jianren = 0;
		move_speed = 0;
		fujiashanghai = 0;
		dikangshanghai = 0;

		jingzhun_per = 0;
		baoji_per = 0;
		kangbao_per = 0;
		pofang_per = 0;
		mianshang_per = 0;

		constant_zengshang = 0;
		constant_mianshang = 0;

		vip_level = 0;
		guild_post = 0;
		guild_id = 0;

		memset(guild_name, 0, sizeof(guild_name));

		server_id = 0;

		avatar_timestamp = 0;
		reserve_char = -1;
		reserve_ch = 0;

		memset(resreve_ch_list, 0, sizeof(resreve_ch_list));
	}

	struct SkillInfo
	{
		SkillInfo() { this->Reset(); }

		void Reset() 
		{
			skill_id = 0;
			level = 0;
		}

		unsigned short skill_id;						// ����ID
		unsigned short level;							// ���ܵȼ�
	};

	int				plat_type;							// ƽ̨����
	int				role_id;							// ID
	GameName		role_name;							// ����
	char			avatar;								// ͷ��
	char			sex;								// �Ա�
	char			prof;								// ְҵ
	char			camp;								// ��Ӫ
	int				level;								// �ȼ�

	int				capability;							// ս����

	Attribute		max_hp;								// Ѫ��
	Attribute		gongji;								// ����
	Attribute		fangyu;								// ����
	Attribute		mingzhong;							// ����
	Attribute		shanbi;								// ����
	Attribute		baoji;								// ����
	Attribute		jianren;							// ����
	Attribute		move_speed;							// �ƶ��ٶ�

	Attribute		fujiashanghai;
	Attribute		dikangshanghai;
	Attribute		jingzhun_per;						// ��׼��ֱ�
	Attribute		baoji_per;							// ������ֱ�
	Attribute		kangbao_per;						// ������ֱ�
	Attribute		pofang_per;							// �Ʒ���ֱ�
	Attribute		mianshang_per;						// ������ֱ�

	Attribute		constant_zengshang;					// �̶�����
	Attribute		constant_mianshang;					// �̶�����

	char			vip_level;							// vip�ȼ�
	char			guild_post;							// ����ְλ
	char			reserve_char;					
	char			reserve_ch;							// ����

	GuildID			guild_id;							// ����id
	GuildName		guild_name;							// ��������

	int				server_id;							// ���ڷ�����id
	char			resreve_ch_list[68];				// ����

	long long		avatar_timestamp;					// ͷ���޸�ʱ���

	SkillInfo		skill_list[SkillListParam::MAX_SKILL_NUM]; // ������Ϣ
};

typedef char ChallengeParamHex[sizeof(ChallengeParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeParamHex) < 2048);

struct ChallengeUserCommonData
{
	ChallengeUserCommonData() { this->Reset(); }

	void Reset() 
	{
		continue_wins = 0;
		best_rankpos = -1;
		join_times = 0;
		buy_join_times = 0;
		jifen = 0;
		jifen_reward_flag = 0;
		last_online_time = 0;
		reward_guanghui = 0;
		reward_bind_gold = 0;
		liansheng = 0;
		buy_buff_times = 0;
		reward_day = 0;
		memset(reward_item_list, 0, sizeof(reward_item_list));
	}

	void OnResetData()
	{
		join_times = 0;
		buy_join_times = 0;
		jifen = 0;
		jifen_reward_flag = 0;
		buy_buff_times = 0;
	}

	int continue_wins;
	int best_rankpos;
	int join_times;
	int buy_join_times;
	int jifen;
	int jifen_reward_flag;
	long long last_online_time;
	int reward_guanghui;
	int reward_bind_gold;
	int liansheng;
	int buy_buff_times;
	int reward_day;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

typedef char ChallengeUserCommonDataHex[sizeof(ChallengeUserCommonData) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeUserCommonDataHex) < 256);

struct ChallengeUserReport								// ս��
{
	struct ReportItem
	{
		ReportItem() { this->Reset(); }

		void Reset() 
		{
			challenge_time = 0;
			target_uid = 0;
			memset(target_name, 0, sizeof(target_name));
			is_sponsor = 0;
			is_win = 0;
			reserve_sh = 0;
			old_rankpos = 0;
			new_rankpos = 0;
		}

		bool Invalid() const { return (0 == challenge_time); }

		unsigned int		challenge_time;					// ʱ��
		long long			target_uid;						// �Է�id
		GameName			target_name;					// �Է�����
		char				is_sponsor;						// �Լ��Ƿ�Ϊ��ս��
		char				is_win;							// �Ƿ�ʤ��
		short				reserve_sh;						// ����
		unsigned short		old_rankpos;					// ����ǰ����
		unsigned short		new_rankpos;					// ����������
	};

	ChallengeUserReport()
	{
		this->Reset();
	}

	void Reset()
	{
		reprot_count = 0;
	}

	short reprot_count;
	ReportItem report_list[CHALLENGE_FIELD_REPORT_MAX_COUNT];
};

typedef char ChallengeUserReportHex[sizeof(ChallengeUserReport) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeUserReportHex) < 2304);

struct ChallengeFieldDataParam
{
	struct LuckyUserItem
	{
		LuckyUserItem() { this->Reset(); }

		void Reset()
		{
			rank_pos = 0;
			user_id = 0;
			memset(user_name, 0, sizeof(user_name));
		}

		int rank_pos;
		int user_id;
		GameName user_name;
	};

	ChallengeFieldDataParam() { this->Reset(); }

	void Reset()
	{
		next_give_luckyreward_timestamp = 0;
		is_luckyreward_gived = 0;
		is_rankreward_gived = 0;
		rankreward_percent = 0;
		next_give_rankreward_timestamp = 0;

		for (int i = 0; i < CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT; ++ i)
		{
			luck_rank_pos_list[i] = -1;
		}
		memset(reward_rank_uid_list, 0, sizeof(reward_rank_uid_list));

		reward_opengame_day = 0;
		clear_data_version = 0;
	}

	unsigned int next_give_luckyreward_timestamp;
	char is_luckyreward_gived;
	char is_rankreward_gived;
	short rankreward_percent;
	unsigned int next_give_rankreward_timestamp;

	UNSTD_STATIC_CHECK(10 == CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT);

	int luck_rank_pos_list[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT];
	LuckyUserItem lucky_user_list[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT];
	int reward_rank_uid_list[CHALLENGE_FIELD_RECORD_RANK_POS_MAX_COUNT];				// ��¼����ǰ10�����UID 

	int reward_opengame_day;															// ���㿪���ڼ���Ľ���
	char clear_data_version;
}; 

typedef char ChallengeFieldDataParamHex[sizeof(ChallengeFieldDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeFieldDataParamHex) < 1024);

#pragma pack(pop)

#endif // __CHALLENGE_FIELD_DEF_HPP__

