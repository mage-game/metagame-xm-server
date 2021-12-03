#ifndef __MARRY_DEF_HPP__
#define __MARRY_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/serverdef.h"

#pragma pack(push, 4)

enum MARRY_LEVEL_TYPE
{
	MARRY_LEVEL_TYPE_INVALID = 0,
	MARRY_LEVEL_TYPE_POTONG,									// ���� ��ͨ�����꣩
	MARRY_LEVEL_TYPE_HAOHUA,									// ���� ��������ʯ��
	MARRY_LEVEL_TYPE_MAX,
};

enum MARRY_STATUS
{
	MARRY_STATUS_INVALID = 0,
	MARRY_STATUS_ZHUN_BEI,										// ����׼��
	MARRY_STATUS_BAI_TIAN_DI_ZHUN_BEI,							// �����׼��
	MARRY_STATUS_BAI_TIAN_DI,									// �����
	MARRY_STATUS_XUN_YOU,										// Ѳ��
	MARRY_STATUS_HUN_YAN_ZHUN_BEI,								// ����׼��				
	MARRY_STATUS_HUN_YAN,										// ����
	MARRY_STATUS_FINISH,										// �������
};

enum MARRY_XUNYOU_OPERA_TYPE
{
	MARRY_XUNYOU_OPERA_TYPE_INVALID = 0,
	MARRY_XUNYOU_OPERA_TYPE_SA_HONGBAO,							// Ѳ�������
	MARRY_XUNYOU_OPERA_TYPE_RANFANG_YANHUA,						// Ѳ��ȼ���̻�
	MARRY_XUNYOU_OPERA_TYPE_MEIGUI_BIAOBAI,						// Ѳ��õ����
	MARRY_XUNYOU_OPERA_TYPE_MAX,
};

static const int MARRY_MAX_DAILY_MERRY_NUM = 24;				// ÿ����е���������Ŀ
static const int MARRY_COUPLE_USER_LIMIT_NUM = 2;				// һ������ֻ����������
static const int MARRY_ROLE_LEVEL_LIMIT = 50;					// �������ȼ�
static const int MARRY_REWARD_ITEM_MAX_NUM = 2;					// ��齱����Ʒ�������
static const int MARRY_XUNYOU_POINT_MAX_NUM = 30;				// ���Ѳ����������
static const int MARRY_HUNYAN_INVITE_USER_MAX_NUM = 50;			// ������ϲ�������������
static const int MARRY_HUNYAN_BLESS_GIFT_MAX_NUM = 6;			// ������ף�������������
static const int MARRY_MARRYOBJ_MAX_NUM = 7;					// �������������
static const int MARRY_HUNYAN_SCENE_KEY = 1;					// ��鳡��key
static const int MARRY_LEVEL_REWARD_SHIZHUANG_NUM = 8;			// ��齱��ʱװ������
static const int MARRY_HUNYAN_OPERA_LANGMAN_WUSHUANG_DIS = 20;	// ������������˫Ҫ��������ɫ�ľ���
static const int PER_DAY_JOIN_YANHUI_MAX_COUNT = 3;				// ÿ�����μ�������

static const int MARRY_MIN_INTERVAL_FROM_DIVORCE = 5 * 24 * 3600;// �ٴν����Сʱ����
static const int DIVORCE_MIN_INTERVAL_FROM_MARRY = 24 * 3600;	// ��������Сʱ����

struct SyncMarryInfo
{
	SyncMarryInfo() { this->Reset(); }

	void Reset()
	{
		memset(uid_list, 0, sizeof(uid_list));
		memset(user_name_list, 0, sizeof(user_name_list));

		marry_level = 0;
		marry_status = 0;

		memset(camp_list, 0, sizeof(camp_list));

		total_marry_count = 0;
		marryobjpos_x = 0;
		marryobjpos_y = 0;

		marry_start_time = 0;
	}

	int uid_list[MARRY_COUPLE_USER_LIMIT_NUM];					// ���˫��ID
	GameName user_name_list[MARRY_COUPLE_USER_LIMIT_NUM];		// ���˫������
	char marry_level;											// ���ĵ���
	char marry_status;											// ��鵱ǰ״̬
	char camp_list[MARRY_COUPLE_USER_LIMIT_NUM];				// ˫����Ӫ
	int total_marry_count;										// ȫ������ܳ���
	short marryobjpos_x;										// ����������X
	short marryobjpos_y;										// ����������Y
	unsigned int marry_start_time;								// ����ʼʱ��
};

struct MarryReserve
{
	MarryReserve() { this->Reset(); }

	void Reset()
	{
		memset(uid_list, 0, sizeof(uid_list));
		memset(user_name_list, 0, sizeof(user_name_list));

		marry_level = 0;
		is_already_start = 0;
		
		memset(camp_list, 0, sizeof(camp_list));
		reserve_ll = 0;
	}

	bool Invalid() { return MARRY_LEVEL_TYPE_INVALID == marry_level; }
	bool IsAlreadyStart() { return 0 != is_already_start; }

	int uid_list[MARRY_COUPLE_USER_LIMIT_NUM];					// ���˫��ID
	GameName user_name_list[MARRY_COUPLE_USER_LIMIT_NUM];		// ���˫������
	char marry_level;											// ���ĵ���
	char is_already_start;										// �Ƿ��ѽ���
	char camp_list[MARRY_COUPLE_USER_LIMIT_NUM];				// ���˫����Ӫ
	long long reserve_ll;										// ����3
};

struct MarryReserveParam
{
	MarryReserveParam() : total_marry_count(0) { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < MARRY_MAX_DAILY_MERRY_NUM; i++)
		{
			reserve_list[i].Reset();
		}
	}

	int total_marry_count;										// ȫ�������ܳ���
	MarryReserve reserve_list[MARRY_MAX_DAILY_MERRY_NUM];		// ÿ��Ԥ���Ļ���
};

struct MarryHunyanInviteList
{
	MarryHunyanInviteList() : uid(0), count(0)
	{
		memset(be_invite_uid_list, 0, sizeof(be_invite_uid_list));
	}

	int uid;
	int count;
	int be_invite_uid_list[MARRY_HUNYAN_INVITE_USER_MAX_NUM];
};

struct MarryHunyanBlessNewsItem
{
	MarryHunyanBlessNewsItem() : news_id(0), uid(0), user_camp(0), bless_gift_index(0), be_bless_uid(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int news_id;
	int uid;
	GameName user_name;
	short user_camp;
	short bless_gift_index; 
	int be_bless_uid;
};

#pragma pack(pop)

#endif // __MARRY_DEF_HPP__

