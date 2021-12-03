// 此模块用于存储跨服gs全局数据

#ifndef __WORLD_STATUS2_PARAM_HPP__
#define __WORLD_STATUS2_PARAM_HPP__

#include "servercommon/servercommon.h"
#include "common/tlvprotocol.h"
#include "servercommon/droploglistdef.hpp"
#include "servercommon/qingyuandef.hpp"
#include "servercommon/crossdef.hpp"

static const int MAX_UPGRADE_LEVEL = 10;			// 最大进阶记录数
static const int MAX_UPGRADE_TYPE = 16;				// 最大进阶类型数,目前UPGRADE_TYPE_MAX是9，预留一半空位给后面扩展

static const int MAX_SERVER_ACQUISITION_RECORD_NUM = 100;	// 全服收购记录最大个数

// 婚宴预约---------------------------------------------------------------
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
	char can_invite_guest_num;  //能够邀请的宾客最大数
	char wedding_yuyue_seq;
	char is_has_done;
	int wedding_index;
	int guest_role_id_list[WEDDING_GUESTS_MAX_NUM]; // 婚宴宾客名单(role_id)
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
	WeddingYuYueInfo yuyue_info[WEDDING_YUYUE_TIME_MAX];			//婚礼预约信息
};

// 表白墙全局数据---------------------------------------------------------------
static const int GLOBAL_PROFESS_MAX_NUM = 30;	// 表白墙最多最多保存的表白数

struct GlobalProfessItem
{
	int				role_id_from;		// 表白者
	int				role_id_to;			// 被表白者
	int				gife_type;			// 礼物类型
	unsigned int	profess_time;		// 表白时间
	ProfessContent	content;			// 表白内容

	bool IsVaild() const { return role_id_from != 0 && role_id_to != 0 && profess_time > 0; }
};

struct GlobalProfessParam
{
	GlobalProfessParam() { this->Reset(); }

	short earliest_profess_index;	// 最早的一个表白的索引
	short reserve_sh;
	GlobalProfessItem profession_item_list[GLOBAL_PROFESS_MAX_NUM];

	void Reset()
	{
		earliest_profess_index = 0;
		reserve_sh = 0;
		memset(profession_item_list, 0, sizeof(profession_item_list));
	}
};

// 公共数据---------------------------------------------------------------
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
	// 跨服1v1-------------------------------------------------------------------------------------
	int cross1v1_season_count;                                                      // 1v1第几赛季
	int cross1v1_rank_min_score[CROSS_1V1_SEASON_MAX];                              // 1v1排行榜最低分

	// 跨服3v3-------------------------------------------------------------------------------------
	int cross3v3_season_count;                                                      // 3v3第几赛季
	int cross3v3_day_index;															// 3v3当前赛季天数（暂时没用）
	int cross3v3_rank_min_score[CROSS_3V3_SEASON_MAX];                              // 3v3排行榜最低分

	//随机活动 完美情人
	int perfect_lover_couple_count;
	int perfect_lover_role_id_list[RAND_PERFECT_LOVER_MAX_RECORD_COUPLE_COUNT * 2];	// 随机活动 完美情人 全服记录

	int total_upgrade_fabao_record_list[MAX_UPGRADE_LEVEL];						// 法宝进阶人数记录
	int total_upgrade_shizhuang_record_list[MAX_UPGRADE_LEVEL];					// 时装进阶人数记录
	int total_upgrade_shenbing_record_list[MAX_UPGRADE_LEVEL];					// 神兵进阶人数记录
	int total_upgrade_footprint_record_list[MAX_UPGRADE_LEVEL];					// 足迹进阶人数记录

	int mess_battle_rank_first[CROSS_MESS_BATTLE_MAX_SCENE_NUM];						// 乱斗战场第一名
	int mess_battle_rank_second[CROSS_MESS_BATTLE_MAX_SCENE_NUM];						// 乱斗战场第二名
	int mess_battle_rank_third[CROSS_MESS_BATTLE_MAX_SCENE_NUM];						// 乱斗战场第三名

	int night_fight_rank_first[CROSS_NIGHT_FIGHT_MAX_SCENE_NUM];						//夜战王城第一名
	int night_fight_rank_second[CROSS_NIGHT_FIGHT_MAX_SCENE_NUM];						//夜战王城第二名
	int night_fight_rank_third[CROSS_NIGHT_FIGHT_MAX_SCENE_NUM];						//夜战王城第三名
	
	int cross_xiuluotower_rank_title[CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM];		// 跨服修罗塔前三名

	int total_upgrade_qilinbi_record_list[MAX_UPGRADE_LEVEL];					// 麒麟臂进阶人数记录
	int total_upgrade_toushi_record_list[MAX_UPGRADE_LEVEL];					// 头饰进阶人数记录
	int total_upgrade_yaoshi_record_list[MAX_UPGRADE_LEVEL];					// 腰饰进阶人数记录
	int total_upgrade_fight_mount_record_list[MAX_UPGRADE_LEVEL];				// 战骑进阶人数记录
	int total_upgrade_record_list[MAX_UPGRADE_TYPE][MAX_UPGRADE_LEVEL];			// 进阶系统进阶人数记录

	int image_competition_competition_type;                                  // 形象比拼比拼类型
	int image_competition_competition_image;                                 // 形象比拼比拼形象id

	int ling_kun_win_guild_id;
	int cross_1v1_cur_join_season_dayid;									 // 本服务器参加跨服1v1活动的日期
	int cross_3v3_cur_join_season_dayid;									 // 本服务器参加跨服3v3活动的日期

};

// 全服收购记录
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
		ACQU_LOG_TYPE_ACQU = 0,	// 收购
	};

	char log_type;			// 类型
	char log_str_id;		// 日志字符串ID
	ItemID item_id;			// 物品ID
	unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // 仙品属性类型

	unsigned int timestamp;	// 记录时间戳
	int	role_uid;			// 角色uid

	int reserve_arr[3];		// 保留字段2
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
