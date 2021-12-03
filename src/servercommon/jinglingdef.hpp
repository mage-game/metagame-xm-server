#ifndef __JINGLING_DEF_HPP__
#define __JINGLING_DEF_HPP__

#include "servercommon.h"
#include "servercommon/struct/itemlistparam.h"

#pragma pack(push, 4)

const static int JINGLING_MAX_LEVEL = 200;										// �������ȼ�
const static int JINGLING_MAX_TAKEON_NUM = 4;									// ���鴩�������
const static int JINGLING_MAX_GROUP_IN_ONECE = 6;								// һ���Ծ�����������

const static int JINGLING_CARD_MAX_TYPE = 16;									// ���鿨���������
const static int JINGLING_CARD_ATTR_COUNT = 3;									// ���鿨��������������
const static int JINGLING_CARD_MAX_LEVEL = 200;									// ���鿨�����ȼ�
const static int JINGLING_CARD_STUFF_ADD_EXP = 1;								// ���鿨�Ʋ������Ӿ���ֵ

static const int JINGLING_SHUXINGDAN_MAX_TYPE = 3;								// ���Ե�
static const int JINGLING_MAX_IMAGE_ID = 10;									// �������ID
static const int JINGLING_MAX_GRADE = 10;										// ������
static const int JINGLING_EQUIP_MAX_PART = 8;									// װ�����λ

static const int JINGLING_PTHANTOM_MAX_TYPE = 128;								// �û����� 
static const int JINGLING_PTHANTOM_MAX_BYTE = JINGLING_PTHANTOM_MAX_TYPE / BIT_COUNT_OF_BYTE;			// ������������ֽ�
static const int JINGLING_PTHANTOM_MAX_LEVEL = 150;								// ���û��ȼ�

static const int JINGLING_SOUL_MAX_LEVEL = 10;									// �������ȼ�

static const int HALO_MAX_IMAGEID = 10;											// ����⻷������
static const int JINGLING_HALO_MAX_LEVEL = 60;									// ����⻷������

static const int JING_LING_WU_XING_LEVEL_MAX = 100;								// �����������ȼ�
static const int JING_LING_XIAN_ZHEN_LEVEL_MAX = 100;							// �����������ȼ�
static const int JING_LING_XIAN_ZHEN_HUN_YU_LEVEL_MAX = 50;						// ��������������ȼ�
static const int JING_LING_SKILL_ID_MAX = 252;									// ���鼼��ID���ֵ
static const int JING_LING_SKILL_STORAGE_MAX = 50;								// ���鼼�ֿܲ��������
static const int JING_LING_SKILL_REFRESH_SKILL_MAX = 11;						// ���鼼ˢ�� ��������
static const int JING_LING_SKILL_REFRESH_ITEM_MAX = 4;							// ���鼼ˢ�� ����

static const int JING_LING_EXPLORE_MODE_COUNT = 3;								// ����̽��ģʽ����
static const int JING_LING_EXPLORE_LEVEL_COUNT = 6;								// ����̽�չؿ�����
static const int JING_LING_COLOR_COUNT = 7;										// ����Ʒ���������


UNSTD_STATIC_CHECK(0 == JINGLING_EQUIP_MAX_PART % 2);
UNSTD_STATIC_CHECK(4 == JINGLING_MAX_TAKEON_NUM);
UNSTD_STATIC_CHECK(128 == JINGLING_PTHANTOM_MAX_TYPE);

//�츳����
enum JINGLING_TALENT_TYPE
{
	JL_TALENT_INVALID_TYPE = 0,

	JL_TALENT_GONGJI,
	JL_TALENT_FANGYU,
	JL_TALENT_MAXHP,
	JL_TALENT_MINGZHONG,
	JL_TALENT_SHANGBI,
	JL_TALENT_BAOJI,
	JL_TALENT_JIANREN,
	JL_TALENT_JINGZHUN_PER,
	JL_TALENT_BAOJI_PER,
	JL_TALENT_POFANG_PER,
	JL_TALENT_MIANSHANG_PER,

	JL_TALENT_MAX_TYPE,
};

// ������;��
enum JINGLING_GETWAY
{
	JL_GETWAY_MIN = 0,

	JL_GETWAY_DEFGIFT = 1,		// �ӹ̶�����л��

	JL_GETWAY_MAX,
};

// ��������
enum XIAN_ZHEN_HUN_YU_TYPE
{
	XIAN_ZHEN_HUN_YU_TYPE_MAXHP = 0,
	XIAN_ZHEN_HUN_YU_TYPE_GONGJI,
	XIAN_ZHEN_HUN_YU_TYPE_FANGYU,
	XIAN_ZHEN_HUN_YU_TYPE_MAX,
};

struct JingLingCardParamItem
{
	JingLingCardParamItem ()
	{
		this->Reset();
	}

	void Reset()
	{
		level = 0;
		reserve = 0;
		exp = 0;
	}

	short level;
	short reserve;
	int exp;
};

struct JingLingParamItem
{
	JingLingParamItem()
	{
		this->Reset();
	}

	void Reset()
	{
		item_id = 0;
		is_bind = 0;
		reserve_sh = 0;
		level = 0;
		wuxing_level = 0;
		max_wuxing_level = 0;
		ling_jing = 0;

		memset(talent_list, 0, sizeof(talent_list));
		memset(jing_ling_skill_list, 0, sizeof(jing_ling_skill_list));
	}

	ItemID item_id;
	short is_bind;
	short reserve_sh;
	short level;
	short wuxing_level;
	short max_wuxing_level;
	int ling_jing;

	short talent_list[ItemNamespace::MAX_XIANPIN_NUM];
	JingLingSkillInfo jing_ling_skill_list[JING_LING_SKILL_COUNT_MAX];
};

struct JingLingRefreshItem
{
	JingLingRefreshItem()
	{
		this->Reset();
	}

	void Reset()
	{
		refresh_count = 0;
		is_active = 0;
		memset(skill_list, 0, sizeof(skill_list));
	}

	int refresh_count;
	short is_active;
	short skill_list[JING_LING_SKILL_REFRESH_SKILL_MAX];
};

struct JingLingExploreInfo
{
	JingLingExploreInfo() : capability(0), hp(0), jingling_id(0), name_id(0), reward_times(0), reserved(0) {}

	int capability;
	int hp;
	ItemID jingling_id;
	short name_id;
	short reward_times;
	short reserved;
};

struct JingLingParam
{
	JingLingParam() 
	{
		this->Reset();
	}

	void Reset()
	{
		use_jingling_id = 0;
		reserve_sh = 0;

		for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
		{
			jingling_list[i].Reset();
		}

		for (int i = 0; i < JINGLING_CARD_MAX_TYPE; ++ i)
		{
			card_list[i].Reset();
		}

		memset(jingling_name, 0, sizeof(jingling_name));
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));

		jingling_grade = 0;
		grade_bless = 0;
		memset(equip_strength_list, 0, sizeof(equip_strength_list));

		active_image_flag = 0;
		used_imageid = 0;
		memset(phantom_level_list, 0, sizeof(phantom_level_list));
		memset(phantom_image_flag, 0, sizeof(phantom_image_flag));
		phantom_imageid = -1;

		memset(soul_level_list, 0, sizeof(soul_level_list));
	
		halo_active_image_flag = 0;
		halo_level = 0;
		use_halo_img = 0;
		halo_bless_val = 0;

		xianzhen_level = 0;
		xianzhen_exp = 0;
		xianzhen_up_count = 0;
		memset(hunyu_level_list, 0, sizeof(hunyu_level_list));
		memset(skill_storage_list, 0, sizeof(skill_storage_list));

		for (int i = 0; i < JING_LING_SKILL_REFRESH_ITEM_MAX; ++i)
		{
			skill_refresh_item_list[i].Reset();
		}

		history_max_capability = 0;
		explore_maxhp = 0;
		explore_hp = 0;
		explore_mode = 0;
		explore_buy_buff_count = 0;
		memset(explore_info_list, 0, sizeof(explore_info_list));
		history_max_chengzhang = 0;
		history_max_wuxing = 0;

		skill_last_perform_time_lei_ting = 0;
		skill_last_perform_time_xixue = 0;
		skill_last_perform_time_zhi_ming = 0;
		skill_last_perform_time_fan_tan = 0;
	}

	int GetUseJingLingLevel()
	{
		for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
		{
			if (jingling_list[i].item_id == use_jingling_id)
			{
				return jingling_list[i].level;
			}
		}

		return 0;
	}

	void GetUseJingLingTalentList(short talent_list[ItemNamespace::MAX_XIANPIN_NUM])
	{
		for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; ++ i)
		{
			if (jingling_list[i].item_id == use_jingling_id)
			{
				int len = sizeof(talent_list[0]) * ItemNamespace::MAX_XIANPIN_NUM;
				if (len == sizeof(jingling_list[i].talent_list))
				{
					memcpy(talent_list, jingling_list[i].talent_list, len);
				}
			}
		}
	}

	ItemID use_jingling_id;
	short reserve_sh;

	JingLingParamItem jingling_list[JINGLING_MAX_TAKEON_NUM];

	JingLingCardParamItem card_list[JINGLING_CARD_MAX_TYPE];

	GameName jingling_name;
	int shuxingdan_list[JINGLING_SHUXINGDAN_MAX_TYPE];

	int jingling_grade;
	int	grade_bless;
	short equip_strength_list[JINGLING_EQUIP_MAX_PART];

	int active_image_flag;
	int used_imageid;

	short phantom_level_list[JINGLING_PTHANTOM_MAX_TYPE];				// �û��ȼ�
	unsigned char phantom_image_flag[JINGLING_PTHANTOM_MAX_BYTE];
	short phantom_imageid;												// �û�����id
	short reserve;

	short soul_level_list[JINGLING_CARD_MAX_TYPE];						// ���Ǿ���ȼ�

	UNSTD_STATIC_CHECK(HALO_MAX_IMAGEID < 32);
	int halo_active_image_flag;											// �⻷������������б�

	short halo_level;													// �⻷�ȼ�
	short use_halo_img;													// �⻷��ǰʹ�õ�����
	int halo_bless_val;													// �⻷ף��ֵ

	int xianzhen_level;
	int xianzhen_exp;
	int xianzhen_up_count;
	int hunyu_level_list[XIAN_ZHEN_HUN_YU_TYPE_MAX];
	JingLingSkillInfo skill_storage_list[JING_LING_SKILL_STORAGE_MAX];		// ���鼼�ֿܲ�
	JingLingRefreshItem skill_refresh_item_list[JING_LING_SKILL_REFRESH_ITEM_MAX];

	int history_max_capability;												// ��ʷ���ս��
	int explore_maxhp;
	int explore_hp;
	short explore_mode;
	short explore_buy_buff_count;;
	JingLingExploreInfo explore_info_list[JING_LING_EXPLORE_LEVEL_COUNT];	// ����̽����Ϣ
	int history_max_chengzhang;												// ��ʷ���ɳ�
	int history_max_wuxing;													// ��ʷ�������

	unsigned int skill_last_perform_time_lei_ting;							// �����ϴ��ͷ�ʱ��
	unsigned int skill_last_perform_time_xixue;
	unsigned int skill_last_perform_time_zhi_ming;
	unsigned int skill_last_perform_time_fan_tan;
};

typedef char JingLingParamHex[sizeof(JingLingParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(JingLingParamHex) < 3072);

// �����԰
static const int JING_LING_HOME_REWARD_ITEM_MAX = 40;
static const int JING_LING_HOME_ROB_RECORD_MAX = 20;

struct JingLingHomeItem
{
	JingLingHomeItem() : item_id(0), reward_times(0), capability(0), reward_beging_time(0), last_get_time(0), reward_lingjing(0), reward_hunli(0) {}

	struct RewardItem
	{
		RewardItem() : item_id(0), item_num(0) {}
		ItemID item_id;
		short item_num;
	};

	ItemID item_id;
	unsigned short reward_times;
	int capability;
	unsigned int reward_beging_time;
	unsigned int last_get_time;
	int reward_lingjing;
	int reward_hunli;
	RewardItem reward_item_list[JING_LING_HOME_REWARD_ITEM_MAX];
};

struct JingLingHomeRobRecord
{
	JingLingHomeRobRecord() : role_id(0), rob_time(0) {}

	int role_id;
	unsigned int rob_time;
};

struct JingLingHomeParam
{
	JingLingHomeParam()
	{
		this->Reset();
	}

	void Reset()
	{
		read_rob_record_time = 0;
		memset(jingling_list, 0, sizeof(jingling_list));
		memset(be_rob_record_list, 0, sizeof(be_rob_record_list));
		memset(today_record_list, 0, sizeof(today_record_list));
	}

	unsigned int read_rob_record_time;
	JingLingHomeItem jingling_list[JINGLING_MAX_TAKEON_NUM];
	JingLingHomeRobRecord be_rob_record_list[JING_LING_HOME_ROB_RECORD_MAX];	// ������¼
	int today_record_list[JING_LING_HOME_ROB_RECORD_MAX];						// ���˼�¼
};

typedef char JingLingHomeParamHex[sizeof(JingLingHomeParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(JingLingHomeParamHex) < 2048);

#pragma pack(pop)
#endif

