// ��ģ�����ڴ洢���gsȫ������

#ifndef __WORLD_STATUS2_PARAM_HPP__
#define __WORLD_STATUS2_PARAM_HPP__

#include "servercommon/servercommon.h"
#include "common/tlvprotocol.h"
#include "servercommon/droploglistdef.hpp"
#include "servercommon/qingyuandef.hpp"
#include "servercommon/crossdef.hpp"

static const int MAX_UPGRADE_LEVEL = 10;			// �����׼�¼��
static const int MAX_UPGRADE_TYPE = 16;				// ������������,ĿǰUPGRADE_TYPE_MAX��9��Ԥ��һ���λ��������չ

static const int MAX_SERVER_ACQUISITION_RECORD_NUM = 100;	// ȫ���չ���¼������

// ����ԤԼ---------------------------------------------------------------
struct WeddingYuYueInfo
{
	WeddingYuYueInfo() { this->Reset(); }

	void Reset()
	{
		role_id = 0;
		lover_role_id = 0;
		wedding_type = 0;
		can_invite_guest_num = 0;
		wedding_yuyue_seq = 0;
		is_has_done = 0;
		wedding_index = 0;
		memset(guest_role_id_list, 0, sizeof(guest_role_id_list));
	}

	int role_id;
	int lover_role_id;
	char wedding_type;
	char can_invite_guest_num;  //�ܹ�����ı��������
	char wedding_yuyue_seq;
	char is_has_done;
	int wedding_index;
	int guest_role_id_list[WEDDING_GUESTS_MAX_NUM]; // �����������(role_id)
};

struct WeddingParam
{
	WeddingParam() { this->Reset(); }

	void Reset()
	{
		total_wedding_num = 0;
		wedding_yuyue_flag = 0;

		for (int i = 0; i < WEDDING_YUYUE_TIME_MAX; i++)
		{
			yuyue_info[i].Reset();
		}
	}

	int total_wedding_num;
	long long wedding_yuyue_flag;
	WeddingYuYueInfo yuyue_info[WEDDING_YUYUE_TIME_MAX];			//����ԤԼ��Ϣ
};

// ���ǽȫ������---------------------------------------------------------------
static const int GLOBAL_PROFESS_MAX_NUM = 30;	// ���ǽ�����ౣ��ı����

struct GlobalProfessItem
{
	int				role_id_from;		// �����
	int				role_id_to;			// �������
	int				gife_type;			// ��������
	unsigned int	profess_time;		// ���ʱ��
	ProfessContent	content;			// �������

	bool IsVaild() const { return role_id_from != 0 && role_id_to != 0 && profess_time > 0; }
};

struct GlobalProfessParam
{
	GlobalProfessParam() { this->Reset(); }

	short earliest_profess_index;	// �����һ����׵�����
	short reserve_sh;
	GlobalProfessItem profession_item_list[GLOBAL_PROFESS_MAX_NUM];

	void Reset()
	{
		earliest_profess_index = 0;
		reserve_sh = 0;
		memset(profession_item_list, 0, sizeof(profession_item_list));
	}
};

// ��������---------------------------------------------------------------
struct WorldStatus2CommonDataParam
{
	WorldStatus2CommonDataParam() { this->Reset(); }

	void Reset()
	{
		cross1v1_season_count = 1;
		memset(cross1v1_rank_min_score, 0, sizeof(cross1v1_rank_min_score));

		cross3v3_season_count = 1;
		cross3v3_day_index = 0;
		memset(cross3v3_rank_min_score, 0, sizeof(cross3v3_rank_min_score));

		perfect_lover_couple_count = 0;
		memset(perfect_lover_role_id_list, 0, sizeof(perfect_lover_role_id_list));
		memset(total_upgrade_fabao_record_list, 0, sizeof(total_upgrade_fabao_record_list));
		memset(total_upgrade_shizhuang_record_list, 0, sizeof(total_upgrade_shizhuang_record_list));
		memset(total_upgrade_shenbing_record_list, 0, sizeof(total_upgrade_shenbing_record_list));
		memset(total_upgrade_footprint_record_list, 0, sizeof(total_upgrade_footprint_record_list));

		memset(mess_battle_rank_first, 0, sizeof(mess_battle_rank_first));
		memset(mess_battle_rank_second, 0, sizeof(mess_battle_rank_second));
		memset(mess_battle_rank_third, 0, sizeof(mess_battle_rank_third));

		memset(night_fight_rank_first, 0, sizeof(night_fight_rank_first));
		memset(night_fight_rank_second, 0, sizeof(night_fight_rank_second));
		memset(night_fight_rank_third, 0, sizeof(night_fight_rank_third));

		memset(cross_xiuluotower_rank_title, 0, sizeof(cross_xiuluotower_rank_title));

		memset(total_upgrade_qilinbi_record_list, 0, sizeof(total_upgrade_qilinbi_record_list));
		memset(total_upgrade_toushi_record_list, 0, sizeof(total_upgrade_toushi_record_list));
		memset(total_upgrade_yaoshi_record_list, 0, sizeof(total_upgrade_yaoshi_record_list));
		memset(total_upgrade_fight_mount_record_list, 0, sizeof(total_upgrade_fight_mount_record_list));
		memset(total_upgrade_record_list, 0, sizeof(total_upgrade_record_list));

		image_competition_competition_type = 0;
		image_competition_competition_image = 0;
		ling_kun_win_guild_id = 0;

		cross_1v1_cur_join_season_dayid = 0;
		cross_3v3_cur_join_season_dayid = 0;
	}

	void ResetMessbattleRankTop()
	{
		memset(mess_battle_rank_first, 0, sizeof(mess_battle_rank_first));
		memset(mess_battle_rank_second, 0, sizeof(mess_battle_rank_second));
		memset(mess_battle_rank_third, 0, sizeof(mess_battle_rank_third));
	}
	
	void ResetNightFightRankTop()
	{
		memset(night_fight_rank_first, 0, sizeof(night_fight_rank_first));
		memset(night_fight_rank_second, 0, sizeof(night_fight_rank_second));
		memset(night_fight_rank_third, 0, sizeof(night_fight_rank_third));
	}
	// ���1v1-------------------------------------------------------------------------------------
	int cross1v1_season_count;                                                      // 1v1�ڼ�����
	int cross1v1_rank_min_score[CROSS_1V1_SEASON_MAX];                              // 1v1���а���ͷ�

	// ���3v3-------------------------------------------------------------------------------------
	int cross3v3_season_count;                                                      // 3v3�ڼ�����
	int cross3v3_day_index;															// 3v3��ǰ������������ʱû�ã�
	int cross3v3_rank_min_score[CROSS_3V3_SEASON_MAX];                              // 3v3���а���ͷ�

	//���� ��������
	int perfect_lover_couple_count;
	int perfect_lover_role_id_list[RAND_PERFECT_LOVER_MAX_RECORD_COUPLE_COUNT * 2];	// ���� �������� ȫ����¼

	int total_upgrade_fabao_record_list[MAX_UPGRADE_LEVEL];						// ��������������¼
	int total_upgrade_shizhuang_record_list[MAX_UPGRADE_LEVEL];					// ʱװ����������¼
	int total_upgrade_shenbing_record_list[MAX_UPGRADE_LEVEL];					// �������������¼
	int total_upgrade_footprint_record_list[MAX_UPGRADE_LEVEL];					// �㼣����������¼

	int mess_battle_rank_first[CROSS_MESS_BATTLE_MAX_SCENE_NUM];						// �Ҷ�ս����һ��
	int mess_battle_rank_second[CROSS_MESS_BATTLE_MAX_SCENE_NUM];						// �Ҷ�ս���ڶ���
	int mess_battle_rank_third[CROSS_MESS_BATTLE_MAX_SCENE_NUM];						// �Ҷ�ս��������

	int night_fight_rank_first[CROSS_NIGHT_FIGHT_MAX_SCENE_NUM];						//ҹս���ǵ�һ��
	int night_fight_rank_second[CROSS_NIGHT_FIGHT_MAX_SCENE_NUM];						//ҹս���ǵڶ���
	int night_fight_rank_third[CROSS_NIGHT_FIGHT_MAX_SCENE_NUM];						//ҹս���ǵ�����
	
	int cross_xiuluotower_rank_title[CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM];		// ���������ǰ����

	int total_upgrade_qilinbi_record_list[MAX_UPGRADE_LEVEL];					// ����۽���������¼
	int total_upgrade_toushi_record_list[MAX_UPGRADE_LEVEL];					// ͷ�ν���������¼
	int total_upgrade_yaoshi_record_list[MAX_UPGRADE_LEVEL];					// ���ν���������¼
	int total_upgrade_fight_mount_record_list[MAX_UPGRADE_LEVEL];				// ս�����������¼
	int total_upgrade_record_list[MAX_UPGRADE_TYPE][MAX_UPGRADE_LEVEL];			// ����ϵͳ����������¼

	int image_competition_competition_type;                                  // �����ƴ��ƴ����
	int image_competition_competition_image;                                 // �����ƴ��ƴ����id

	int ling_kun_win_guild_id;
	int cross_1v1_cur_join_season_dayid;									 // ���������μӿ��1v1�������
	int cross_3v3_cur_join_season_dayid;									 // ���������μӿ��3v3�������

};

// ȫ���չ���¼
struct WorldAcquisitionLogItem
{
	WorldAcquisitionLogItem() { this->Reset(); }

	void Reset()
	{
		log_type = 0;
		log_str_id = 0;
		item_id = 0;
		memset(xianpin_type_list, 0, sizeof(xianpin_type_list));

		timestamp = 0;
		role_uid = 0;

		memset(reserve_arr, 0, sizeof(reserve_arr));
	}

	bool IsValid() const { return (role_uid > 0 && item_id > 0); }

	enum ACQU_LOG_TYPE
	{
		ACQU_LOG_TYPE_ACQU = 0,	// �չ�
	};

	char log_type;			// ����
	char log_str_id;		// ��־�ַ���ID
	ItemID item_id;			// ��ƷID
	unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // ��Ʒ��������

	unsigned int timestamp;	// ��¼ʱ���
	int	role_uid;			// ��ɫuid

	int reserve_arr[3];		// �����ֶ�2
};
struct WorldAcquisitionLogParam
{
	WorldAcquisitionLogParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < MAX_SERVER_ACQUISITION_RECORD_NUM; ++i)
		{
			world_acquisition_log_list[i].Reset();
		}
	}

	WorldAcquisitionLogItem world_acquisition_log_list[MAX_SERVER_ACQUISITION_RECORD_NUM];
};

class WorldStatus2Param
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	DropLogListParam drop_log_list_param;
	WorldStatus2CommonDataParam common_data_2;
	WeddingParam wedding_param;
	GlobalProfessParam global_profess_param;
	DropLogFbParam drop_log_fb_param;

	WorldAcquisitionLogParam world_acquisition_log_param;
};

typedef char DropLogListParamHex[sizeof(DropLogListParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(DropLogListParamHex) < 8192);

typedef char WorldStatus2CommonDataParamHex[sizeof(WorldStatus2CommonDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WorldStatus2CommonDataParamHex) < 8192);

typedef char WeddingParamHex[sizeof(WeddingParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WeddingParamHex) < 20992);

typedef char GlobalProfessParamHex[sizeof(GlobalProfessParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GlobalProfessParamHex) < 5120);

typedef char DropLogFbParamHex[sizeof(DropLogFbParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(DropLogFbParamHex) < 7680);

typedef char WorldAcquisitionLogParamHex[sizeof(WorldAcquisitionLogParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WorldAcquisitionLogParamHex) < 8000);

#endif // __WORLD_STATUS2_PARAM_HPP__
