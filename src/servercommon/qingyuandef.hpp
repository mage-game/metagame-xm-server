#ifndef __QINGYUAN_DEF_HPP__
#define __QINGYUAN_DEF_HPP__

#include "servercommon.h"
#include "servercommon/struct/itemlistparam.h"

const static int QINGYUAN_FB_TEAM_MEMBER_COUNT = 2;				// ��Ե���������涨
UNSTD_STATIC_CHECK(QINGYUAN_FB_TEAM_MEMBER_COUNT == 2);

const static int QINGYUAN_SKIP_TO_LAST_WAVE_COUNT = 10;
const static int REWARD_QINGYUAN_VALUE_PER_MONSTER = 30;

const static int QINGYUAN_CARD_MAX_LEVEL = 12;					// ��Ե�������ȼ�
const static int QINGYUAN_CARD_MAX_ID = 19;						// ��Ե������󿨺�

const static int QINGYUAN_EQUIP_MAX_STAR = 10;					// ��Եװ�����������

const static int QINGYUAN_COUPLE_HALO_MAX_TYPE = 16;			// ���޹⻷�������ֵ
const static int QINGYUAN_COUPLE_HALO_MAX_LEVEL = 360;			// ���޹⻷���ȼ�

const static int QINGYUAN_ADD_BLESS_DAYS_PER = 30;				// ����ף����������

const static int QINGYUAN_HUNLI_MAX_TYPE = 8;					// �������͸������ֵ

const static int QINGYUAN_FB_MAX_REWARD_COUNT = 6;				// ��Ե���������������

static const int MAX_ROLE_IN_YANHUI = 250;						// ����ڳ�����

const static int QINGYUAN_MARRY_MAX_LEVEL = 1000;				// ��Ե������ȼ�
const static int QINGYUAN_EQUIPSUIT_MAX_COUNT = 10;				// ��Եװ����װͼ���������
const static int QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT = 8;			// ��Եװ����װͼ��ÿ��������
UNSTD_STATIC_CHECK(8 == QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT);

const static int QING_YUAN_SHENG_DI_TASK_MAX_COUNT = 6;			// ��Եʥ�������������

const static int WEDDING_YUYUE_TIME_MAX = 48;					// ����ԤԼʱ���������
const static int WEDDING_GUESTS_MAX_NUM = 50;					// ������������(���ݿ⡢Э�����)
const static int WEDDING_MAX_LIVENESS_TYPE = 3;					// �����ȶ��������
const static int WEDDING_BLESSING_MAX_CFG_NUM = 3;				// �����������������
const static int WEDDING_BLESSSING_MAX_RECORD_NUM = 10;			// ף����¼�����
const static int WEDDING_APPLICANT_MAX_NUM = 100;				// �����������

static const int HUNYAN_XUNYOU_POINT_MAX_NUM = 80;				// ���Ѳ����������
static const int HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM = 10;			// ���Ѳ�ζ����������

const static int QINGYUAN_EQUIP2_MAX_LEVEL = 400;				// ��Ƚ��װ�����ȼ�
const static int QINGYUAN_GIFT_MAX_COUNT = 8;					// ������λ

const static int RAND_PERFECT_LOVER_MAX_RECORD_COUPLE_COUNT = 50; // ��������������


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
	YUYUE_HUNYAN_TYPE_0,										// ��������0(��Ԫ������)
	YUYUE_HUNYAN_TYPE_1,										// ��������1(Ԫ��)
	YUYUE_HUNYAN_TYPE_2,										// ��������2(Ԫ��)

	YUYUE_HUNYAN_TYPE_MAX,
};

enum QINGYUAN_MARRY_STATUS
{
	QINGYUAN_MARRY_STATUS_INVAILD = 0,							// ��Ч״̬�����ǰ��
	QINGYUAN_MARRY_STATUS_WEDDING_YUYUE,						// ԤԼ״̬������
	QINGYUAN_MARRY_STATUS_INVITE_GUESTS,						// ׼��״̬��������ͣ�
	QINGYUAN_MARRY_STATUS_WEDDING,								// ����״̬�������ʱ��

	QINGYUAN_MARRY_STATUS_MAX,
};

enum HUNYAN_ZHUFU_TYPE
{
	HUNYAN_ZHUFU_TYPE_RED_BAG = 0,								// �ͺ�� 
	HUNYAN_ZHUFU_TYPE_GIVE_FLOWER,								// �ͻ�
	HUNYAN_ZHUFU_TYPE_USE_YANHUA,								// ���̻�

	HUNYAN_ZHUFU_TYPE_MAX,
};

inline bool IsValidYuyueYanhuiType(int yanhui_type)
{
	return yanhui_type > YUYUE_HUNYAN_TYPE_0 && yanhui_type < YUYUE_HUNYAN_TYPE_MAX;
}

#pragma pack(push, 4)

// ��Եȫ������-------------------------------------------------------------

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

//���ⱦ������
enum SPECIAL_BABY_TYPE
{
	SPECIAL_BABY_TYPE_0,
	SPECIAL_BABY_TYPE_1,

	SPECIAL_BABY_TYPE_MAX,
};

// ���ⱦ��Ʒ��
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
	int marry_level;																// ���ȼ�
	int marry_level_exp;															// ���ȼ�����
	char today_buy_gift_flag;														// ���չ���������
	char is_add_profess_share_per;													// �Ƿ��б�����Թ���ӳ�
	char fight_out_couple_halo_type;												// ��ս���޹⻷
	char reserve_ch;

	unsigned char qingyuan_suit_flag[QINGYUAN_EQUIPSUIT_MAX_COUNT];					// ���װ����װ������λ
	short is_all_task_complete;

	ItemDataWrapper qingyuan_equip_list[QINGYUAN_EQUIP_TYPE_MAX];
	QingyuanEquip2Item qingyuan_equip2_list[QINGYUAN_EQUIP_TYPE_MAX];				// ��Ƚ��װ��

	unsigned short profess_level;													// ��׵ȼ�
	short reserve_sh1;
	unsigned int profess_exp;														// ��׾���

	QingyuanSpecialBaby special_baby_list[SPECIAL_BABY_TYPE_MAX];					// ���ⱦ����Ϣ
	
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
	int bless_type;									// ף������ 0: ��� 1:�ͻ� 2: ���̻� HUNYAN_ZHUFU_TYPE
	int param;										// bless_typeΪ2ʱ��paramΪ������Ʒ����param1
	unsigned int timestamp;							// ��ǰʱ���
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

//��Ե����-----------------------------------------------------------------------------------
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
	char is_hunyan_fb_already_open;				// ��������Ѿ��ٰ�=1 ����Ϊ0
	char can_open_hunyan;
	int qingyuan_value;
	char is_fetch_bless_reward;
	char is_first_diamond_yanhui;				// �Ƿ��ǵ�һ����ʯ��ᣨ���磩
	char today_hunyan_open_times;				// ������еĻ������
	char is_hunyan_fb_open;						// �������Ƿ�Ϊ����
	int reserve_int;							// ����
	int yanhui_fb_key;							// ��ḱ��key
	int marry_count;							// ������

	int marry_req_uid;							// ������uid
	int bless_days;								// ����ף��ʣ������

	int today_yanhui_get_diamonds;
	int today_yanhui_get_hunjie_stuffs;
	int today_yanhui_get_other_stuffs;
	int today_yanhui_get_exps;
	int today_hunyan_gather_day_max_stuff_num;												// �������ɼ�����
	int today_hunyan_join_times;															// ����μӻ������
	int today_saxianhua_times;																//�������ʻ�����

	int today_hunjie_uplevel_times;															// ��䵱����������
	int hunjie_cur_exp;																		// ����Ǽ���ǰ����
	int today_tuodan_times;																	// ��Ҫ�ѵ�����ķ�������

	UNSTD_STATIC_CHECK(19 == QINGYUAN_CARD_MAX_ID);
	char card_level_list[QINGYUAN_CARD_MAX_ID + 1];											// ��Ե���Ƶȼ��б�

	char equiped_couple_halo_type;															// ��װ���ķ��޹⻷
	char cur_tejia_halo_type;																// ��ǰ�ؼ۹⻷����
	short reserve_sh;																		// 									
	UNSTD_STATIC_CHECK(QINGYUAN_COUPLE_HALO_MAX_TYPE == 16);
	short couple_halo_level_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];							// ���޹⻷�ȼ��б�
	short couple_halo_exp_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];								// ���޹⻷�����б�

	short is_fetched_task_other_reward;														// �Ƿ���ȡ��Եʥ�ض��⽱��
	short task_info_count;
	QingYuanShengDiTask task_info_list[QING_YUAN_SHENG_DI_TASK_MAX_COUNT];					// ��Եʥ������

	unsigned int tejie_halo_invalid_time;													// �ؼ۹⻷ʧЧʱ��				
	char reserve_char_list[1604];															// ����

	int today_hunyan_be_zhufu_max_stuff_num;												// ���챻ף���ۼƻ�ò�������
	int today_hunyan_zhufu_max_stuff_num;													// ����ף���ۼƻ�ò�������

	//-------------------------------------------------------------------------------------------
	char hunyan_type;																		// ��������
	char is_today_marry_req_succ;															// ��������Ƿ�ɹ�
	char is_have_hunyan_times;																// �Ƿ��л������
	char cur_marry_stauts;																	// ��ǰ������
	char wedding_yuyue_seq;																	// ԤԼ�����±�
	char marry_type;																		// �������(������Ч��,�����鲻��)
	char has_marry_huili_type_flag;															// �Ѿ������Ľ������
	char today_xunyou_gather_hongbao_count;													// Ѳ��-����ɼ��������
	int wedding_index;																		// ȫ���ڼ�������
	UInt8 today_xunyou_sa_hongbao_count;													// Ѳ��-�������������
	UInt8 today_xunyou_sa_hongbao_buy_count;												// Ѳ��-����������������
	UInt8 today_xunyou_give_flower_count;													// Ѳ��-�����ͻ�����
	UInt8 today_xunyou_give_flower_buy_count;												// Ѳ��-�����ͻ��������
};

typedef char QingyuanParamHex[sizeof(QingyuanParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(QingyuanParamHex) < 4096);  

//��׸�������--------------------------------------------------------------------------------
typedef char ProfessContent[64];

const static int PERSON_PROFESS_MAX_NUM = 12;					// ���ǽ���˱�������

enum PROFESS_GIFT_TYPE
{
	PROFESS_GIFT_TYPE_CHAMPAGNE = 0,							// ���� 
	PROFESS_GIFT_TYPE_HOUSEBOAT,								// ��ͧ
	PROFESS_GIFT_TYPE_ROCKET,									// ���

	PROFESS_GIFT_TYPE_MAX,
};

enum PROFESS_WALL_TYPE
{
	PROFESS_WALL_TYPE_SELF = 0,		// �ұ�׵�
	PROFESS_WALL_TYPE_OTHER = 1,	// ����ҵ�
	PROFESS_WALL_TYPE_GLOBAL = 2,	// �������ǽ
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

	int				other_role_id;			// �Է�id
	unsigned int 	profess_time;			// ���ʱ��
	int				gift_type;				// ��������
	ProfessContent	content;				// �������
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
	PersonProfessItem profess_to_items[PERSON_PROFESS_MAX_NUM];		// �ұ�׵�
	PersonProfessItem profess_from_items[PERSON_PROFESS_MAX_NUM];	// ����ҵ�
};

typedef char PersonProfessParamHex[sizeof(PersonProfessParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PersonProfessParamHex) < 4096);

#pragma pack(pop)

#endif  // __QINGYUAN_DEF_HPP__

