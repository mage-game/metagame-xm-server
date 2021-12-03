#ifndef __WORLD_STATUS_PARAM_HPP__
#define __WORLD_STATUS_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/basedef.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/rankdef.hpp"
#include "servercommon/vipdef.hpp"

#include "servercommon/marrydef.hpp"
#include "servercommon/challengefielddef.hpp"
#include "servercommon/worldeventdef.hpp"
#include "servercommon/campdef.hpp"
#include "servercommon/chatserverdef.h"
#include "servercommon/crossrandactivitydef.hpp"

static const int MAX_UPGRADE_RECORD_COUNT = 10;			// �����׼�¼��
static const int MAX_CHONGJIHAOLI_RECORD_COUNT = 30;	// �弶��������ȼ�ȫ������ȡ��¼�����
static const int MAX_CHEST_SHOP_RECORD_COUNT = 30;		// Ѱ����¼�����

static const int MAX_SHENGXINGZHULI_SPECIAL_DAYS_COUNT = 7;	// ��������ǰ7��Ҫ���⴦��

struct YsgcSytTopUserInfo
{
	YsgcSytTopUserInfo() 
		: quanfu_topscore(0), quanfu_topscore_uid(0)
	{
		memset(quanfu_topscore_name, 0, sizeof(quanfu_topscore_name));
	}

	int quanfu_topscore;
	int	quanfu_topscore_uid;
	GameName quanfu_topscore_name;
};

struct WorldStatusCommonDataParam
{
	WorldStatusCommonDataParam() { this->Reset(); }

	struct ChestRecord
	{
		ChestRecord() : uid(0), item_id(0), reserve(0)
		{}

		int uid;
		ItemID item_id;
		short reserve;
	};
	
	void Reset() 
	{
		buy_month_card_times = 0;
		xunbao_times = 0;
		
		memset(vip_level_role_num, 0, sizeof(vip_level_role_num));
		memset(reserve_int_list, 0, sizeof(reserve_int_list));

		gcz_chengzhu_id = 0;
		gcz_win_guild = INVALID_GUILD_ID;
		gcz_series_win = 0;

		last_reset_role_module_dayid = 0;
		last_reset_role_modlue_timestamp = 0;

		memset(oga_putonequipment_fetch_times, 0, sizeof(oga_putonequipment_fetch_times));
		memset(oga_capability_fetch_times, 0, sizeof(oga_capability_fetch_times));
		memset(oga_rolelevel_fetch_times, 0, sizeof(oga_rolelevel_fetch_times));

		memset(camp_toprank3_uid, 0, sizeof(camp_toprank3_uid));
		memset(camp_diaoxiang_status, 0, sizeof(camp_diaoxiang_status));

		today_chongzhi_role_count = 0;
		memset(total_upgrade_mount_record_list, 0, sizeof(total_upgrade_mount_record_list));
		memset(total_upgrade_holo_record_list, 0, sizeof(total_upgrade_holo_record_list));
		memset(total_upgrade_wing_record_list, 0, sizeof(total_upgrade_wing_record_list));
		memset(total_upgrade_shengong_record_list, 0, sizeof(total_upgrade_shengong_record_list));
		memset(total_upgrade_shenyi_record_list, 0, sizeof(total_upgrade_shenyi_record_list));

		new_cornucopia_total_value = 0;

		open_server_qunxian_uid = 0;
		open_server_guildbatte_uid = 0;
		open_server_gongchengzhan_uid = 0;
		open_server_territorywar_uid = 0;

		memset(total_chongjihaoli_record_list, 0, sizeof(total_chongjihaoli_record_list));

		guildbattle_mengzhu_guild_id = 0;

		famous_man_type_1_uid = 0;
		famous_man_type_2_uid = 0;
		famous_man_type_3_uid = 0;
		famous_man_type_4_uid = 0;
		famous_man_type_5_uid = 0;
		famous_man_type_6_uid = 0;

		free_create_guild_times = 0;
		role_create_count = 0;
		last_send_fake_recharge_msg_time = 0;
		charm_top1_title_uid = 0;

		yizhandaodi_win_user_id = 0;

		chestshop_index = 0;
		chestshop_index1 = 0;
		chestshop_index2 = 0;
		chestshop_jl_index = 0;

		memset(chestshop_record_list, 0, sizeof(chestshop_record_list));
		memset(chestshop_jl_record_list, 0, sizeof(chestshop_jl_record_list));

		marry_all_times = 0;
		marry_zhufu_count = 0;

		world_question_top1_title_uid = 0;
		gold_turntable_pool = 0;

		yesterday_chongzhi_role_count = 0;

		is_gongchengzhan_first = 0;

		total_join_darknight_role_level = 0;
		total_join_darknight_role_count = 0;
		total_join_darknight_average_level = 0;

		memset(qunxian_luandou_user_id_list, 0, sizeof(qunxian_luandou_user_id_list));

		qingyuan_cap_top_male_uid = 0;						// ��Ե��ս����һ
		qingyuan_cap_top_female_uid = 0;					// ��ԵŮս����һ
		baby_cap_top_male_uid = 0;							// ������ս����һ
		baby_cap_top_female_uid = 0;						// ����Ůս����һ
		little_pet_cap_rank_top_male_uid = 0;					// С����ս���е�һ
		little_pet_cap_rank_top_female_uid = 0;					// С����ս��Ů��һ
		
		memset(cross_fishing_top_rank_uid_list, 0, sizeof(cross_fishing_top_rank_uid_list));
		memset(cross_pasture_top_rank_uid_list, 0, sizeof(cross_pasture_top_rank_uid_list));

		yizhandaodi_second_user_id = 0;						// һս���׵ڶ���id
		yizhandaodi_third_user_id = 0;						// һս���׵�����id

		reserve_int = 0;
		memset(chestshop_record3_list, 0, sizeof(chestshop_record3_list));
		chestshop_index3 = 0;

		chat_limit_condition_type_flag = 0;
		set_chat_limit_cond_type_flag = 0;
		is_forbid_audio_chat = 0;
		reserve_ch = 0;
		reserve_sh = 0;

		memset(channel_vip_level_limit_list, 0, sizeof(channel_vip_level_limit_list));
		memset(channel_level_limit_list, 0, sizeof(channel_level_limit_list));

		adjust_server_role_num = 0;
		kill_cross_boss_rank_mengzhu = 0;
		kill_corss_boss_rank_guild = 0;
		first_charge_fake_num = 0;
	}

	int buy_month_card_times;						// �¿���������
	int xunbao_times;								// Ѱ������

	UNSTD_STATIC_CHECK(20 == MAX_VIP_LEVEL);

	int vip_level_role_num[MAX_VIP_LEVEL + 1];		// �����ȼ�vip����
	int reserve_int_list[1];						// �����ֽڣ�Ԥ���Ժ�vip�ȼ�����

	int gcz_chengzhu_id;							// ����ս����id
	int gcz_win_guild;								// ����սʤ������
	int gcz_series_win;								// ����ս��ʤ����

	unsigned int last_reset_role_module_dayid;		// ��������ɫ����������ݵ�dayid
	unsigned int last_reset_role_modlue_timestamp;	// ��������ɫ����ֵ���ݵ�ʱ��
	
	YsgcSytTopUserInfo ysgc_topuserinfo;			// ���޹㳡��߻���
	YsgcSytTopUserInfo syt_topuserinfo;				// ��������߻���
	
	unsigned short oga_putonequipment_fetch_times[OGA_PUTONEQUIPMENT_REWARD_MAX_SEQ];	// ȫ���ռ�װ����ȡ��������
	unsigned short oga_capability_fetch_times[OGA_CAPABILITY_REWARD_MAX_SEQ];			// ȫ��ս�������ȡ��������
	unsigned short oga_rolelevel_fetch_times[OGA_ROLELEVEL_REWARD_MAX_SEQ];				// ȫ���ȼ������ȡ��������

	UNSTD_STATIC_CHECK(9 == MAX_CAMP_RECORD_RANK3_NUM);
	int camp_toprank3_uid[MAX_CAMP_RECORD_RANK3_NUM];									// ǰ3����. 3����Ӫ
	int camp_diaoxiang_status[MAX_CAMP_RECORD_RANK3_NUM];								// ʦ������ս���, ǰ3������ 3����Ӫ ((0.�����1����))

	int today_chongzhi_role_count;										// �����״γ�ֵ����
	int total_upgrade_mount_record_list[MAX_UPGRADE_RECORD_COUNT];		// �������������¼
	int total_upgrade_holo_record_list[MAX_UPGRADE_RECORD_COUNT];		// �⻷����������¼
	int total_upgrade_wing_record_list[MAX_UPGRADE_RECORD_COUNT];		// �������������¼
	int total_upgrade_shengong_record_list[MAX_UPGRADE_RECORD_COUNT];	// �񹭽���������¼
	int total_upgrade_shenyi_record_list[MAX_UPGRADE_RECORD_COUNT];		// �������������¼

	int new_cornucopia_total_value;

	int open_server_qunxian_uid;							// �������Ի-Ԫ��ս��
	int open_server_guildbatte_uid;							// �������Ի-��������
	int open_server_gongchengzhan_uid;						// �������Ի-����ս
	int open_server_territorywar_uid;						// �������Ի-����ս

	int total_chongjihaoli_record_list[MAX_CHONGJIHAOLI_RECORD_COUNT];	// �弶��������ȼ�ȫ������ȡ��¼

	GuildID guildbattle_mengzhu_guild_id;				//�������԰���id

	int famous_man_type_1_uid;							// ������-����1
	int famous_man_type_2_uid;							// ������-����2
	int famous_man_type_3_uid;							// ������-����3
	int famous_man_type_4_uid;							// ������-����4
	int famous_man_type_5_uid;							// ������-����5
	int famous_man_type_6_uid;							// ������-����6

	int free_create_guild_times;						// ��Ѵ����ܴ���
	int role_create_count;								// ������ɫ��
	unsigned int last_send_fake_recharge_msg_time;		// ���һ�η��ͼ��׳崫�ŵ�ʱ��
	int charm_top1_title_uid;							// ����������ֵ�����ƺŵĽ�ɫuid

	int yizhandaodi_win_user_id;						// һս���׵�һ��id

	unsigned char chestshop_index;
	unsigned char chestshop_index1;
	unsigned char chestshop_index2;
	unsigned char chestshop_jl_index;
	ChestRecord chestshop_record_list[MAX_CHEST_SHOP_RECORD_COUNT];			// װ��Ѱ����¼
	ChestRecord chestshop_record1_list[MAX_CHEST_SHOP_RECORD_COUNT];		// װ��Ѱ��1��¼
	ChestRecord chestshop_record2_list[MAX_CHEST_SHOP_RECORD_COUNT];		// װ��Ѱ��2��¼
	ChestRecord chestshop_jl_record_list[MAX_CHEST_SHOP_RECORD_COUNT];		// ����Ѱ����¼

	int marry_all_times;													// ����ܴ���
	int marry_zhufu_count;													// ����ף������
	long long gold_turntable_pool;											// Ԫ��ת��

	int world_question_top1_title_uid;					// �����������������һ

	int yesterday_chongzhi_role_count;					// �����ֵ����

	unsigned int is_gongchengzhan_first;				// �Ѿ��״ι���ս��ʵ��Ϊ��

	int total_join_darknight_role_level;				// ����ºڷ�ߵ��λ��������ܵȼ�
	short total_join_darknight_role_count;				// ����ºڷ�ߵ��λ�������������
	short total_join_darknight_average_level;			// ����ºڷ�����ƽ���ȼ�
	
	int qunxian_luandou_user_id_list[QUNXIANLUANDOU_SIDE_MAX];				// Ⱥ���Ҷ�����������һ

	int qingyuan_cap_top_male_uid;						// ��Ե��ս����һ
	int qingyuan_cap_top_female_uid;					// ��ԵŮս����һ
	int baby_cap_top_male_uid;							// ������ս����һ
	int baby_cap_top_female_uid;						// ����Ůս����һ
	int little_pet_cap_rank_top_male_uid;				// С����ս���е�һ
	int little_pet_cap_rank_top_female_uid;				// С����ս��Ů��һ

	UNSTD_STATIC_CHECK(3 == CROSS_FISHING_TOP_RANK_COUNT_MAX);
	Int32 cross_fishing_top_rank_uid_list[CROSS_FISHING_TOP_RANK_COUNT_MAX];			// �������ǰ����
	Int32 cross_pasture_top_rank_uid_list[CROSS_PASTURE_TOP_RANK_COUNT_MAX];			// ��������ǰ����

	int yizhandaodi_second_user_id;						// һս���׵ڶ���id
	int yizhandaodi_third_user_id;						// һս���׵�����id
	unsigned int reserve_int;							// ����

	ChestRecord chestshop_record3_list[MAX_CHEST_SHOP_RECORD_COUNT];		// װ��Ѱ��3��¼
	int chestshop_index3;											// װ��Ѱ��3����

	short chat_limit_condition_type_flag;							// ����������������
	short set_chat_limit_cond_type_flag;							// �Ƿ��̨���ù�����������������
	char is_forbid_audio_chat;										// �Ƿ��ֹ��������
	char reserve_ch;
	short reserve_sh;

	UNSTD_STATIC_CHECK(8 == OPENLEVEL_LIMIT_TYPE_MAX);
	char channel_vip_level_limit_list[OPENLEVEL_LIMIT_TYPE_MAX];				// Ƶ��vip�ȼ�����
	unsigned short channel_level_limit_list[OPENLEVEL_LIMIT_TYPE_MAX];			// Ƶ���ȼ�����
	unsigned int adjust_server_role_num;								// ��̬���������
	int kill_cross_boss_rank_mengzhu;									// ��ɱ����boss��һ������������
	int kill_corss_boss_rank_guild;										// ��ɱ����boss��һ����
	int first_charge_fake_num;											// �׳��Ź�������
};

struct BossSpecialRefreshParam
{
	struct RefreshItem
	{
		RefreshItem() : boss_id(0), refresh_interval(0) {}
		int boss_id;
		int refresh_interval;
	};

	static const int REFRESH_ITEM_MAX = 50;
	RefreshItem refresh_item_list[REFRESH_ITEM_MAX];
};

struct CrossGuildBattleGuildItem
{
	CrossGuildBattleGuildItem() { this->Reset(); }

	void Reset()
	{
		plat_type = 0;
		guild_id = 0;
		memset(guild_name, 0, sizeof(guild_name));
		memset(guild_tuanzhang_name, 0, sizeof(guild_tuanzhang_name));
	}

	int plat_type;
	int guild_id;
	GuildName guild_name;
	GuildName guild_tuanzhang_name;
};

struct CrossGuildBattleTuanzhangInfo
{
	CrossGuildBattleTuanzhangInfo() { this->Reset(); }

	void Reset()
	{
		tuanzhang_prof = 0;
		tuanzhang_sex = 0;

		reserve_sh = 0;
	}
	char tuanzhang_prof;
	char tuanzhang_sex;

	short reserve_sh;
};
struct CrossGuildBattleApperanceInfo //���������Ϣ
{
	CrossGuildBattleApperanceInfo() { this->Reset(); }
	void Reset()
	{
		wuqi_id = 0;
		shizhuang_wuqi = 0;
		shizhuang_wuqi_is_special = 0;
		shizhuang_body = 0;
		shizhuang_body_is_special = 0;
		halo_used_imageid = 0;
		toushi_used_imageid = 0;
		qilinbi_used_imageid = 0;
		mask_used_imageid = 0;
		reserve_sh = 0;
	}
	ItemID wuqi_id;              // ����ID
	char shizhuang_wuqi;          // ʱװ����
	char shizhuang_wuqi_is_special;      // ʱװ����
	char shizhuang_body;          // ʱװ����
	char shizhuang_body_is_special;      // ʱװ����
	short halo_used_imageid;        // �⻷����
	short toushi_used_imageid;        // ͷ������id
	short qilinbi_used_imageid;        // ���������id
	short mask_used_imageid;        // �������id
	short reserve_sh;
};
struct CrossGuildBattleParam
{
	CrossGuildBattleParam() { this->Reset(); }

	void Reset()
	{
		reward_calc_time = 0;
		average_level = 0;

		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
		{
			cross_guildbattle_list[i].Reset();
			info_list[i].Reset();
			apperance_info_list[i].Reset();
		}
	}

	CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	int reward_calc_time;
	int average_level;
	CrossGuildBattleTuanzhangInfo info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	CrossGuildBattleApperanceInfo apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
};

class WorldStatusParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	long long id_system;							// ������ʼ����ʱ������ 

	unsigned int start_time;
	unsigned int world_time;
	unsigned int last_update_dayid;
	int xianmengzhan_mengzhu_id;

	ActivityOpenRecords activity_open_records;

	WorldStatusCommonDataParam common_data;
	ChallengeFieldDataParam challenge_field_data;
	WorldEventParam world_event_param;

	CombineServerActivityRankParam csa_activity_rank_param;
	CombineServerActivityOtherParam csa_activity_other_param;

	BossSpecialRefreshParam boss_refresh_param;

	CrossGuildBattleParam cross_guildbattle_param;

	CombineServerActivityBossRankParam csa_boss_rank_param;

	CrossRandActivityParam cross_rand_activity_param;
	CrossRandActivityBigParam cross_rand_activity_big_param;
	CrossRandActivityBigParam2 cross_rand_activity_big_param2;
	CrossCloudPurchaseParam cross_cloudpurchase_param;
};

typedef char WorldStatusCommonDataParamHex[sizeof(WorldStatusCommonDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WorldStatusCommonDataParamHex) < 4800);

typedef char ActivityOpenRecordsHex[sizeof(ActivityOpenRecords) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ActivityOpenRecordsHex) < 96);

typedef char TotalRoleProfCountListHex[sizeof(int) * 5 * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TotalRoleProfCountListHex) < 64);

typedef char MarryReserveDataHex[sizeof(MarryReserveParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MarryReserveDataHex) < 5120);

typedef char WorldEventParamHex[sizeof(WorldEventParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WorldEventParamHex) < 512);

typedef char CombineServerActivityRankParamHex[sizeof(CombineServerActivityRankParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CombineServerActivityRankParamHex) < 2048);

typedef char CombineServerActivityOtherParamHex[sizeof(CombineServerActivityOtherParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CombineServerActivityOtherParamHex) < 256);

typedef char BossSpecialRefreshParamHex[sizeof(BossSpecialRefreshParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(BossSpecialRefreshParamHex) < 1024);

typedef char CrossGuildBattleParamHex[sizeof(CrossGuildBattleParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossGuildBattleParamHex) < 2048);

typedef char CombineServerActivityBossRankParamHex[sizeof(CombineServerActivityBossRankParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CombineServerActivityBossRankParamHex) < 512);

typedef char CrossRandActivityParamHex[sizeof(CrossRandActivityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossRandActivityParamHex) < 1024);

typedef char CrossRandActivityBigParamHex[sizeof(CrossRandActivityBigParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossRandActivityBigParamHex) < 1024);

typedef char CrossRandActivityBigParam2Hex[sizeof(CrossRandActivityBigParam2) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossRandActivityBigParam2Hex) < 4096);

typedef char CrossCloudPurchaseParamHex[sizeof(CrossCloudPurchaseParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossCloudPurchaseParamHex) < 30720);

#endif // __WORLD_STATUS_PARAM_HPP__

