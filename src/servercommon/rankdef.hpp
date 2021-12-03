/******************************************************************************************
FileName: rankdef.hpp
Author: 
Description: ���а�ϵͳ 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreanwang    2011/08/26/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __RANK_DEF_HPP__
#define __RANK_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/campdef.hpp"

static const int RANK_DATA_SAVE_TO_DB_TIME = 60;			 // ���а����ݱ��浽DB��ʱ����

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const int PERSON_RANK_LEVEL_LIMIT = 40;				 // �ϰ�ȼ�����
static const int PERSON_RANK_UPDATE_TO_GLOBAL_TIME = 1 * 60; // ͬ�������а��ʱ���� 
static const int PERSON_RANK_UPDATE_TO_CROSS_TIME = 10 * 60; // ͬ����������а��ʱ����(ͬ����Ҫ�������е�ͼ��Init֮��2���ͬ��. ������һ���ҵ�ͬ��)
static const int PERSON_RANK_SNAPSHOT_INTERVAL = 60;		 // �������а����ʱ���� ��

// ���˰�����  
enum PERSON_RANK_TYPE
{
	PERSON_RANK_TYPE_CAPABILITY_ALL = 0,						// �ۺ�ս����
	PERSON_RANK_TYPE_LEVEL = 1,									// �ȼ���
	PERSON_RANK_TYPE_XIANNV_CAPABILITY = 2,						// ��Ůս����
	PERSON_RANK_TYPE_EQUIP = 3,									// װ��ս����
	PERSON_RANK_TYPE_ALL_CHARM = 4,								// �����ܰ�	
	PERSON_RANK_TYPE_CAPABILITY_CAMP_1 = 5,						// ��Ӫ1�ۺ�ս����
	PERSON_RANK_TYPE_CAPABILITY_CAMP_2 = 6,						// ��Ӫ2�ۺ�ս����
	PERSON_RANK_TYPE_CAPABILITY_CAMP_3 = 7,						// ��Ӫ3�ۺ�ս����
	PERSON_RANK_TYPE_MOUNT = 8,									// �������ս����
	PERSON_RANK_TYPE_XIANJIAN = 9,								// �ɽ�ս����
	PERSON_RANK_TYPE_MENTALITY = 10,							// ����ս����
	PERSON_RANK_TYPE_WING = 11,									// �������ս����
	PERSON_RANK_TYPE_CHARM = 12,								// ������
	PERSON_RANK_TYPE_CHARM_MALE = 13,							// ����������
	PERSON_RANK_TYPE_CHARM_FEMALE = 14,							// Ů��������
	PERSON_RANK_TYPE_GONGCHENGZHAN = 15,						// ����ս���а�
	PERSON_RANK_TYPE_QUNXIANLUANDOU = 16,						// Ⱥ���Ҷ����а�
	PERSON_RANK_TYPE_ZHANSHENDIAN = 17,							// ս������а�
	PERSON_RANK_TYPE_KILL_WORLDBOSS = 18,						// ��ɱ����boss 
	PERSON_RANK_TYPE_RA_CHONGZHI = 19,							// ��ֵ���а�
	PERSON_RANK_TYPE_RA_CONSUME_GOLD = 20,						// �������а�
	PERSON_RANK_TYPE_RED_PAPER_CONSUME_GOLD = 21,				// ���������
	PERSON_RANK_TYPE_RED_PAPER_LUCKY_VALUE = 22,				// ������˰�
	PERSON_RANK_TYPE_EVIL = 23,									// ����

	PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP1 = 24,				// ��Ӫ1��ʷ��ս����
	PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP2 = 25,				// ��Ӫ2��ʷ��ս����
	PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP3 = 26,				// ��Ӫ3��ʷ��ս����

	PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP1 = 27,				// ��Ӫ1��ʷװ��ս����
	PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP2 = 28,				// ��Ӫ2��ʷװ��ս����
	PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP3 = 29,				// ��Ӫ3��ʷװ��ս����

	PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP1 = 30,				// ��Ӫ1��ʷ��Ůս����
	PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP2 = 31,				// ��Ӫ2��ʷ��Ůս����
	PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP3 = 32,				// ��Ӫ3��ʷ��Ůս����

	PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP1 = 33,			// ��Ӫ1��ʷ����ս����
	PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP2 = 34,			// ��Ӫ2��ʷ����ս����
	PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP3 = 35,			// ��Ӫ3��ʷ����ս����

	PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP1 = 36,				// ��Ӫ1��ʷ����ս����
	PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP2 = 37,				// ��Ӫ2��ʷ����ս����
	PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP3 = 38,				// ��Ӫ3��ʷ����ս����

	PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP1 = 39 ,				// ��Ӫ1��ʷ����ս����
	PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP2 = 40,				// ��Ӫ2��ʷ����ս����
	PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP3 = 41,				// ��Ӫ3��ʷ����ս����

	PERSON_RANK_TYPE_RA_DAY_CHONGZHI_NUM= 42,					// ����ÿ�ճ�ֵ
	PERSON_RANK_TYPE_RA_DAY_CONSUME_NUM = 43,					// ����ÿ������

	PERSON_RANK_TYPE_HALO = 44,									// �⻷����ս����
	PERSON_RANK_TYPE_SHENGONG = 45,								// �񹭽���ս����
	PERSON_RANK_TYPE_SHENYI = 46,								// �������ս����

	PERSON_RANK_TYPE_CAPABILITY_SHENZHUANG = 47,				// ��װս����
	PERSON_RANK_TYPE_CAPABILITY_JINGLING = 48,					// ����ս����
	PERSON_RANK_TYPE_CAPABILITY_WASH = 49,						// ����ս����

	PERSON_RANK_TYPE_EQUIP_STRENGTH_LEVEL = 50,					// ȫ��װ��ǿ���ܵȼ���
	PERSON_RANK_TYPE_STONE_TOATAL_LEVEL = 51,					// ȫ��ʯ�ܵȼ���
	
	PERSON_RANK_TYPE_FIGHT_MOUNT = 52,							// ս���ܵȼ���
	PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER = 53,					// ������Ŀ�����
	PERSON_RANK_TYPE_FIGHTING_CHALLENGE_SCORE = 54,				// ������ - ���ƻ�������
	PERSON_RANK_TYPE_DAY_CHARM = 55,							// ÿ��������
	PERSON_RANK_FISH_PLACE = 56,								// ��������
	PERSON_RANK_STEAL_FISH_PLACE = 57,							// ͵������
	PERSON_RANK_TYPE_PLANTING_TREE_PLANTING = 58,				// ֲ���ֲ����������
	PERSON_RANK_TYPE_PLANTING_TREE_WATERING = 59,				// ֲ�����ˮ��������
	PERSON_RANK_CRYSTA_DALARAN_STEAL_NUM = 60,					// ����Ȼˮ��͵ȡ��
	PERSON_RANK_CRYSTA_DALARAN_BE_STEALED_NUM = 61,				// ����Ȼˮ����͵��

	PERSON_RANK_TYPE_FABAO = 62,								// ��������ս����
	PERSON_RANK_TYPE_SHIZHUANG = 63,							// ʱװ����ս����
	PERSON_RANK_TYPE_SHIZHUANG_WUQI = 64,						// ʱװ���������������ս����
	PERSON_RANK_TYPE_FOOTPRINT = 65,							// �㼣����ս����
	PERSON_RANK_TYPE_RUNE_TOWER_LAYER = 67,						// ������������

	PERSON_RANK_TYPE_YAOSHI = 68,								// ���ν���ս����
	PERSON_RANK_TYPE_TOUSHI = 69,								// ͷ�ν���ս����
	PERSON_RANK_TYPE_QILINBI = 70,								// ����۽���ս����
	PERSON_RANK_TYPE_MASK = 71,									// ��߽���ս����

	PERSON_RANK_TYPE_LINGZHU = 72,                              // �������ս����
	PERSON_RANK_TYPE_XIANBAO = 73,                              // �ɱ�����ս����
	PERSON_RANK_TYPE_LINGTONG = 74,                             // ��ͯ����ս����
	PERSON_RANK_TYPE_LINGGONG = 75,                             // �鹭����ս����
	PERSON_RANK_TYPE_LINGQI = 76,                               // �������ս����
	PERSON_RANK_TYPE_WEIYAN = 77,                               // β�����ս����
	PERSON_RANK_TYPE_SHOUHUAN = 78,                             // �ֻ�����ս����
	PERSON_RANK_TYPE_WEIBA = 79,                                // β�ͽ���ս����
	PERSON_RANK_TYPE_FEICHONG = 80,                             // �ɳ����ս����

	PERSON_RANK_TYPE_CLOAK = 81,                                // �������а�
	PERSON_RANK_TYPE_SHENBING = 82,                             // �������а�

	PERSON_RANK_TYPE_ROLE_PATA_LAYER = 83,						// ����������������

	PERSON_RANK_TYPE_RA_PROFESS_MALE = 84,						// ��������а�
	PERSON_RANK_TYPE_RA_PROFESS_FEMALE = 85,					// �������Ů��

	PERSON_RANK_TYPE_IMAGE_COMPETITION = 86,                    // �����ƴ

	PERSON_RANK_TYPE_RA_CHONGZHI2 = 87,							// ��ֵ���а�2

	PERSON_RANK_TYPE_MAX
};

static const int PERSON_RANK_MAX_SIZE = 100;				// �ϰ�����100 ���20�� 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���Ű�����  
enum GUILD_RANK_TYPE
{
	GUILD_RANK_TYPE_LEVEL = 0,								// ���ŵȼ���
	GUILD_RANK_TYPE_XIANMENGZHAN,							// ����ս����
	GUILD_RANK_TYPE_KILL_WORLD_BOSS,						// ��ɱ����boss����
	GUILD_RANK_TYPE_XIANMENG_BIPIN_KILL_WORLD_BOSS,			// ���˱�ƴ�ڼ��ɱ����boss����
	GUILD_RANK_TYPE_DAY_INCREASE_CAPABILITY,				// ����ÿ������ս��
	GUILD_RANK_TYPE_CAPABILITY,								// ����ս����
	GUILD_RANK_TYPE_GUILDBATTLE,							// �����������а�
	GUILD_RANK_TYPE_TERRITORYWAR,							// ����ս���а�
	GUILD_RANK_TYPE_KILL_CROSS_BOSS,						// ��ɱ����boss(����+Զ��)

	GUILD_RANK_TYPE_MAX,
};

static const int GUILD_RANK_MAX_SIZE = 120;					// �ϰ���� ���20��
static const int GUILD_RANK_SNAPSHOT_INTERVAL = 60;			// �������а����ʱ���� ��

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ��Ӱ�����
enum TEAM_RANK_TYPE
{
	TEAM_RANK_TYPE_EQUIP_FB = 0,							// װ��
	TEAM_RANK_TYPE_TOWERDEFEND,								// ����
	TEAM_RANK_TYPE_MAX
};

static const int TEAM_RANK_MAX_SIZE = 120;					// �ϰ��¼�� ���20��

static const int TEAM_RANK_SNAPSHOT_INTERVAL = 60;			// ������а����ʱ���� ��

#pragma pack(push, 4)

struct TeamRankMemberInfo									// ������ж�Ա��Ϣ ������Э���� ע�����
{
	TeamRankMemberInfo() : uid(0), camp(CAMP_TYPE_INVALID) { memset(user_name, 0, sizeof(user_name)); }

	void Reset() { uid = 0; camp = CAMP_TYPE_INVALID; memset(user_name, 0, sizeof(user_name)); }

	int uid;												// �û�ID
	GameName user_name;										// �û���
	int camp;												// ����
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ǰʮ���������Ͷ���
enum RANK_TOP_TYPE
{
	RANK_TOP_TYPE_MIN = 0,

	RANK_TOP_TYPE_CAPABILITY = RANK_TOP_TYPE_MIN,		// ս��ǰ10
	RANK_TOP_TYPE_LEVEL,								// �ȼ�ǰ10
	RANK_TOP_TYPE_XIANNV,								// ��Ůǰ10
	RANK_TOP_TYPE_CAPABILITY_CAMP_1,					// ��Ӫ1ս��ǰ10
	RANK_TOP_TYPE_CAPABILITY_CAMP_2,					// ��Ӫ2ս��ǰ10
	RANK_TOP_TYPE_CAPABILITY_CAMP_3,					// ��Ӫ3ս��ǰ10
	RANK_TOP_TYPE_CHARM_MALE,							// ��������ǰ10
	RANK_TOP_TYPE_CHARM_FEMALE,							// Ů������ǰ10
	RANK_TOP_TYPE_QUNXIANLUANDOU,						// ����ս��ǰ10
	RANK_TOP_TYPE_ZHANSHENDIAN,							// ս���ǰ10
	RANK_TOP_TYPE_KILL_WORLDBOSS,						// ��ɱ����bossǰ10
	RANK_TOP_TYPE_RED_PAPER_CONSUME_GOLD,				// �������ǰ10
	RANK_TOP_TYPE_RED_PAPER_LUCKY_VALUE,				// �������ǰ10
	RANK_TOP_TYPE_MOUNT,								// ����ս��
	RANK_TOP_TYPE_WING,									// ����ս��
	RANK_TOP_TYPE_HALO,									// �⻷ս��
	RANK_TOP_TYPE_SHENGONG,								// ��ս��
	RANK_TOP_TYPE_SHENYI,								// ����ս��
	RANK_TOP_TYPE_EQUIPT_STRENGTH_LEVEL,				// ȫ��װ��ǿ���ܵȼ�
	RANK_TOP_TYPE_STONE_TOTAL_LEVEL,					// ȫ��ʯ�ܵȼ�
	RANK_TOP_TYPE_FIGHT_MOUNT,							// ս��ս��
	RANK_TOP_TYPE_CHARM,								// ������
	RANK_TOP_TYPE_WORLD_RIGHT_ANSWER,					// ������Ŀ�����
	RANK_TOP_TYPE_JINGLING,								// ����ս��
	RANK_TOP_TYPE_DAY_CHARM,							// ÿ��������
	RANK_TOP_TYPE_QILINBI,								// �����ս��
	RANK_TOP_TYPE_TOUSHI,								// ͷ��ս��
	RANK_TOP_TYPE_YAOSHI,								// ����ս��
	RANK_TOP_TYPE_LINGTONG,								// ��ͯս��
	RANK_TOP_TYPE_LINGGONG,								// �鹭ս��
	RANK_TOP_TYPE_LINGQI,								// ����ս��
	RANK_TOP_TYPE_FlYPET,								// �ɳ�ս��
	RANK_TOP_TYPE_WEIYAN,								// β��ս��

	RANK_TOP_TYPE_MAX,
};

static const int TOP_TEN = 10;							// ǰ10��

struct TopUidList
{
	TopUidList() { this->Reset(); }	

	void Reset() { memset(uid_list, 0, sizeof(uid_list)); }

	int uid_list[TOP_TEN];
};

struct TopRankParam
{
	TopUidList all_top_rank_list[RANK_TOP_TYPE_MAX];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ������а�����
enum CROSS_PERSON_RANK_TYPE
{
	CROSS_PERSON_RANK_TYPE_CAPABILITY_ALL = 0,							// ���ս����
	CROSS_PERSON_RANK_TYPE_WEEK_ADD_CHARM = 1,							// ���������
	CROSS_PERSON_RANK_TYPE_XIULUO_TOWER = 2,							// ���������
	CROSS_PERSON_RANK_TYPE_1V1_SCORE = 3,								// ���1v1�������а�

	CROSS_PERSON_RANK_TYPE_3V3_SCORE = 4 ,								// ���3v3�������а�
	CROSS_PERSON_RANK_TYPE_ROLE_LEVEL = 5,								// ����ȼ����а�
	CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY = 6,							// �������ս����
	CROSS_PERSON_RANK_TYPE_ADD_CHARM = 7,								// �������������
	CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS = 8,							// ��������ɱ��

	CROSS_PERSON_RANK_TYPE_MAX,									
};

static const int CROSS_COUPLE_RANK_TYPE_BEGIN = 1000;

enum CROSS_COUPLE_RANK_TYPE
{
	CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP = 0,						// �����Ե��
	CROSS_COUPLE_RANK_TYPE_MAX
};

static const int CROSS_COUPLE_RANK_MAX_SIZE = 120;			// �ϰ�����100 ���20�� 

static const int CROSS_RANK_UPDATE_TO_GLOBAL_TIME = 3 * 60; // ͬ�������а��ʱ���� 
static const int CROSS_RANK_SNAPSHOT_INTERVAL = 60;		    // �������а����ʱ���� ��
static const int CROSS_PERSON_RANK_MAX_SIZE = 120;			// �ϰ�����100 ���20�� 

static const int TOP_HUNDRED = 100;							// ǰһ����
static const int CROSS_RANK_WEEK_REWARD_MAX_ROLE_COUNT = 20;// ������а�ÿ�ܽ����������

enum CROSS_RANK_TOP_TYPE
{
	CROSS_RANK_TOP_TYPE_MIN = 0,

	CROSS_RANK_TOP_TYPE_CAPABILITY = CROSS_RANK_TOP_TYPE_MIN,		// ս��ǰһ��
	CROSS_RANK_TOP_TYPE_XIULUO_TOWER,								// ���������
	CROSS_RANK_TOP_TYPE_1V1,										// ���1V1���ְ�
	CROSS_RANK_TOP_TYPE_3V3,										// ���3V3���ְ�

	CROSS_RANK_TOP_TYPE_MAX,
};

struct CrossPersonRankTitleUuidInfo
{
	CrossPersonRankTitleUuidInfo() : rank_type(-1), uuid_count(0) 
	{
		memset(rank_title_uuid_list, 0, sizeof(rank_title_uuid_list));
	}

	int rank_type;
	int uuid_count;
	long long rank_title_uuid_list[CROSS_PERSON_RANK_MAX_SIZE];
};

struct CrossPersonRankTitleParam
{
	CrossPersonRankTitleUuidInfo rank_title_uuid_info_list[CROSS_RANK_TOP_TYPE_MAX];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int COUPLE_RANK_TYPE_MAX_COUNT = 8;

// ��������
enum  COUPLE_RANK_TYPE
{
	COUPLE_RANK_TYPE_MIN = 0,
	COUPLE_RANK_TYPE_QINGYUAN_CAP = COUPLE_RANK_TYPE_MIN,				// ������Ես����
	COUPLE_RANK_TYPE_BABY_CAP,											// ���ޱ���ս����
	COUPLE_RANL_TYPE_LITTLE_PET,										// ����С����ս����
	COUPLE_RANK_TYPE_TOTAL_QINGYUAN_CAP,								// ������Ե��ս����(���ڿ�����а�)

	COUPLE_RANK_TYPE_MAX
};

UNSTD_STATIC_CHECK(COUPLE_RANK_TYPE_MAX_COUNT >= COUPLE_RANK_TYPE_MAX);

static const int COUPLE_RANK_MAX_SIZE = 120;							// �������а������
static const int COUPLE_RANK_TOP_COUNT = 10;							// ��������top ��
static const int COUPLE_RANK_SNASHOT_INTERVAL = 60;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif  // __RANK_DEF_HPP__

