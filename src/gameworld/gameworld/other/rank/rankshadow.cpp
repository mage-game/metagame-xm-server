#include "rankshadow.h"

#include "obj/character/role.h"
#include "globalconfig/globalconfig.h"

#include "servercommon/rankdef.hpp"
#include "servercommon/campdef.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "internalcomm.h"
#include "engineadapter.h"
#include "other/event/eventhandler.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/globalrole/globalrole.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/struct/historycapparam.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "gameworld/gameworld/config/sharedconfig/sharedconfig.h"
#include "equipment/equipmentmanager.h"
#include "other/jingling/jinglingmanager.h"
#include "other/xiannv/xiannvmanager.hpp"
#include "friend/friendmanager.h"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "equipment/stonemananger.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/footprint/footprint.hpp"
#include "other/runesystem/runesystem.hpp"
#include "other/cross/rolecross.h"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "other/cloak/cloakmanager.hpp"
#include "other/shenbing/shenbingmanager.hpp"
#include "other/fb/rolepatafb.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"

RankShadow::RankShadow()
	: m_role(NULL), m_next_update_time(0), m_next_update_to_cross_time(0)
{
}

RankShadow::~RankShadow()
{
}

void RankShadow::Init(Role *role)
{
	m_role = role;

	m_next_update_time = EngineAdapter::Instance().Time() + 2;
	m_next_update_to_cross_time = m_next_update_time;
}

void RankShadow::Update(unsigned long interval, time_t now_second)
{
	if (now_second > m_next_update_time)
	{
		this->UpdateFunc();

		m_next_update_time = now_second + PERSON_RANK_UPDATE_TO_GLOBAL_TIME;
	}

	if (now_second > m_next_update_to_cross_time)
	{
		this->UpdateToCross();
		m_next_update_to_cross_time = now_second + PERSON_RANK_UPDATE_TO_CROSS_TIME;
	}
}

void RankShadow::UpdateFunc()
{
	this->UpdateToGlobal();
//	this->UpdateToCross();

	GlobalRole *g_role = GlobalRoleManager::Instance().GetGlobalRole(m_role->GetUID());
	if (NULL != g_role)
	{
		g_role->UpdateToRank(m_role);
	}
}

void RankShadow::OnRoleLeaveScene()
{//调用已被注释
	this->UpdateToGlobal();
	this->UpdateToCross();
}

void RankShadow::UpdataOneToGlobal(int rank_type, long long rank_value, int record_index, GameName flexible_name, int flexible_int, long long flexible_ll)
{
	if (m_role->IsInCross()) 
	{
		return;
	}

	static SyncPersonRankInfo rank_info;

	UserID user_id = m_role->GetUserId();
	rank_info.db_index = user_id.db_index;
	rank_info.role_id = user_id.role_id;
	rank_info.exp = m_role->GetExp();
	rank_info.level = m_role->GetLevel();
	m_role->GetName(rank_info.user_name);
	m_role->GetGuildName(rank_info.guild_name);
	rank_info.reserve_ch = 0;
	rank_info.sex = m_role->GetSex();
	rank_info.prof = m_role->GetProf();
	rank_info.camp = m_role->GetCamp();
	rank_info.vip_level = m_role->GetVip()->GetVipLevel();
	rank_info.jingjie_level = 0;
	rank_info.reserved2 = 0;

	rank_info.items[0].rank_type = rank_type;
	rank_info.items[0].rank_value = rank_value;
	rank_info.items[0].record_index = record_index;
	rank_info.items[0].flexible_int = flexible_int;
	rank_info.items[0].flexible_ll = flexible_ll;

	if (NULL != flexible_name)
	{
		F_STRNCPY(rank_info.items[0].flexible_name, flexible_name, sizeof(GameName));
	}
	else
	{
		memset(rank_info.items[0].flexible_name, 0, sizeof(0));
	}

	rank_info.item_count = 1;

	//int sendlen = sizeof(rank_info) - (SYNC_PERSON_RANK_INFO_SIZE - rank_info.item_count) * sizeof(rank_info.items[0]);

}

void RankShadow::UpdateToGlobal()
{
	if (m_role->IsInCross()) 
	{
		return;
	}

	static SyncPersonRankInfo rank_info;
	rank_info.ResetItems();

	UserID user_id = m_role->GetUserId();
	rank_info.db_index = user_id.db_index;
	rank_info.role_id = user_id.role_id;
	rank_info.exp = m_role->GetExp();
	rank_info.level = m_role->GetLevel();
	m_role->GetName(rank_info.user_name);
	m_role->GetGuildName(rank_info.guild_name);
	rank_info.reserve_ch = 0;
	rank_info.sex = m_role->GetSex();
	rank_info.prof = m_role->GetProf();
	rank_info.camp = m_role->GetCamp();
	rank_info.vip_level = m_role->GetVip()->GetVipLevel();
	rank_info.jingjie_level = m_role->GetJingJieLevel();
	rank_info.reserved2 = 0;

	int item_count = 0;

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 综合战力榜
	{
		rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_CAPABILITY_ALL;
		rank_info.items[item_count].rank_value = m_role->GetCapability()->GetTotalCapability();
		m_role->GetGuildName(rank_info.items[item_count].flexible_name);
		rank_info.items[item_count].record_index = 0;
		rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetTotalCapability();

		UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));

		++item_count;
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 阵营战力榜
	{
		int camp_type = m_role->GetCamp();
		if (camp_type > CAMP_TYPE_INVALID && camp_type < CAMP_TYPE_MAX)
		{
			const static int camp_to_ranktype[CAMP_TYPE_MAX] = 
			{
				-1, 
				PERSON_RANK_TYPE_CAPABILITY_CAMP_1, 
				PERSON_RANK_TYPE_CAPABILITY_CAMP_2, 
				PERSON_RANK_TYPE_CAPABILITY_CAMP_3
			};

			int rank_type = camp_to_ranktype[camp_type];
			if (rank_type > 0)
			{
				rank_info.items[item_count].rank_type = rank_type;
				rank_info.items[item_count].rank_value = m_role->GetCapability()->GetTotalCapability();
				rank_info.items[item_count].record_index = 0; 

				UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
				m_role->GetGuildName(rank_info.items[item_count].flexible_name);

				++item_count;
			}
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 等级榜
	{
		rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_LEVEL;
		rank_info.items[item_count].rank_value = m_role->GetLevel();
		rank_info.items[item_count].record_index = 0;

		UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
		m_role->GetGuildName(rank_info.items[item_count].flexible_name);

		++item_count;
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 仙女榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_XIANNV) > 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_XIANNV_CAPABILITY;
			rank_info.items[item_count].rank_value = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_XIANNV);
			rank_info.items[item_count].record_index = 0;
			m_role->GetXiannvManager()->GetRankXnData(&rank_info.items[item_count].flexible_int, &rank_info.items[item_count].flexible_ll);

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 精灵榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING) > 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_CAPABILITY_JINGLING;
			rank_info.items[item_count].rank_value = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING);
			short jingling_huanhua_image = 0;
			ItemID jingling_id = 0;
			m_role->GetJingLingMgr()->GetUsedJingLingInfo(&jingling_id, NULL, rank_info.items[item_count].flexible_name , NULL, &jingling_huanhua_image);
			rank_info.items[item_count].flexible_int = jingling_huanhua_image;
			rank_info.items[item_count].flexible_ll = jingling_id;
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 坐骑榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MOUNT) > 0 && m_role->GetMountManager()->GetMountParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_MOUNT;
			rank_info.items[item_count].rank_value = m_role->GetMountManager()->GetMountParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MOUNT);
			rank_info.items[item_count].record_index = 0; 

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 羽翼榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_WING) > 0 && m_role->GetWing()->GetWingParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_WING;
			rank_info.items[item_count].rank_value = m_role->GetWing()->GetWingParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_WING);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)  															// 魅力榜
	{
		rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_CHARM;
		rank_info.items[item_count].rank_value = m_role->GetFriendManager()->GetAllCharm();
		rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetTotalCapability();
		rank_info.items[item_count].record_index = 0;

		UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
		m_role->GetGuildName(rank_info.items[item_count].flexible_name);

		++item_count;
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 魅力榜(男/女)
	{
		int rank_type = (FEMALE == m_role->GetSex() ? PERSON_RANK_TYPE_CHARM_FEMALE : PERSON_RANK_TYPE_CHARM_MALE);

		rank_info.items[item_count].rank_type = rank_type;
		rank_info.items[item_count].rank_value = m_role->GetFriendManager()->GetAllCharm();
		rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetTotalCapability();
		rank_info.items[item_count].record_index = 0;

		UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
		m_role->GetGuildName(rank_info.items[item_count].flexible_name);

		++item_count;
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 击杀世界boss
	{
		rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_KILL_WORLDBOSS;
		rank_info.items[item_count].rank_value = m_role->GetKillWorldBossCount();
		rank_info.items[item_count].record_index = 0;

		UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
		m_role->GetGuildName(rank_info.items[item_count].flexible_name);

		++item_count;
	}

	{
		// 随机活动
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_RANK))
		{
			if (item_count < SYNC_PERSON_RANK_INFO_SIZE)													// 充值排行榜
			{
				rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_RA_CHONGZHI;
				rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetRAChongzhiRankChongzhiNum();
				rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - m_role->GetRoleActivity()->GetRAChongzhiRankChongzhiTime();
				rank_info.items[item_count].record_index = 0;

				UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
				m_role->GetGuildName(rank_info.items[item_count].flexible_name);

				++item_count;
			}
		}

		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK))
		{
			if (item_count < SYNC_PERSON_RANK_INFO_SIZE)													// 消费排行榜
			{
				rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_RA_CONSUME_GOLD;
				rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetRAConsumeGoldRankConsumeGoldNum();
				rank_info.items[item_count].flexible_ll = m_role->GetRoleActivity()->GetRAConsumeGoldRankConsumeGoldConsumeTime();
				rank_info.items[item_count].record_index = 0;

				UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
				m_role->GetGuildName(rank_info.items[item_count].flexible_name);

				++item_count;
			}
		}

		if (item_count < SYNC_PERSON_RANK_INFO_SIZE)													// 红包土豪榜
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_RED_PAPER_CONSUME_GOLD;
			rank_info.items[item_count].rank_value = m_role->GetRedPaperConsumeGold();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}

		if (item_count < SYNC_PERSON_RANK_INFO_SIZE)													// 红包幸运榜
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_RED_PAPER_LUCKY_VALUE;
			rank_info.items[item_count].rank_value = m_role->GetRedPaperLuckyValue();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}

		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_RANK2))
		{
			if (item_count < SYNC_PERSON_RANK_INFO_SIZE)													// 充值排行榜2
			{
				rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_RA_CHONGZHI2;
				rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetRAChongzhiRank2ChongzhiNum();
				rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - m_role->GetRoleActivity()->GetRAChongzhiRank2ChongzhiTime();
				rank_info.items[item_count].record_index = 0;

				UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
				m_role->GetGuildName(rank_info.items[item_count].flexible_name);

				++item_count;
			}
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
	{
		int capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_EQUIPMENT);
		capability += m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_GEM);
		
		rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_EQUIP;
		rank_info.items[item_count].rank_value = capability;
		rank_info.items[item_count].record_index = 0;

		UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
		m_role->GetGuildName(rank_info.items[item_count].flexible_name);

		++item_count;
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
	{
		rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_ALL_CHARM;
		rank_info.items[item_count].rank_value = m_role->GetFriendManager()->GetAllCharm();
		rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetTotalCapability();
		rank_info.items[item_count].record_index = 0;

		UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
		m_role->GetGuildName(rank_info.items[item_count].flexible_name);

		++item_count;
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK))
	{
		if (m_role->GetRoleActivity()->GetRADayChongzhiRankGoldNum() > 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_RA_DAY_CHONGZHI_NUM;
			rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetRADayChongzhiRankGoldNum();
			rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - m_role->GetRoleActivity()->GetRADayChongzhiRankLastChongzhiTimestamp();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK))
	{
		if (m_role->GetRoleActivity()->GetRADayConsumeRankGoldNum() > 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_RA_DAY_CONSUME_NUM;
			rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetRADayConsumeRankGoldNum();
			rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - m_role->GetRoleActivity()->GetRADayConsumeRankLastConsumeTimestamp();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (m_role->GetEvil() > 0)																							// 罪恶榜
	{
		if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_EVIL;
			rank_info.items[item_count].rank_value = m_role->GetEvil();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	{
		int camp_type = m_role->GetCamp();
		if (camp_type > CAMP_TYPE_INVALID && camp_type < CAMP_TYPE_MAX)
		{
			UNSTD_STATIC_CHECK(10 == HISTORY_CAPABILITY_TYPE_MAX);
			UNSTD_STATIC_CHECK(4 == CAMP_TYPE_MAX);

			const static int camp1_historycap_ranktype_map[HISTORY_CAPABILITY_TYPE_MAX] = 
			{
				PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP1,
				PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP1,
				PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP1,
				PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP1,
				PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP1,
				PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP1,
			};

			const static int camp2_historycap_ranktype_map[HISTORY_CAPABILITY_TYPE_MAX] = 
			{
				PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP2,
				PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP2,
				PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP2,
				PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP2,
				PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP2,
				PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP2,
			};

			const static int camp3_historycap_ranktype_map[HISTORY_CAPABILITY_TYPE_MAX] = 
			{
				PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP3,
				PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP3,
				PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP3,
				PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP3,
				PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP3,
				PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP3,
			};

			const static int *camp_to_ranktype_array[CAMP_TYPE_MAX] = 
			{
				NULL, 
				camp1_historycap_ranktype_map,
				camp2_historycap_ranktype_map,
				camp3_historycap_ranktype_map,
			};

			const static int historycap_randactivity_type[HISTORY_CAPABILITY_TYPE_MAX] = 
			{
				RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL,
				RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP,
				RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV,
				RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI,
				RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT,
				RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING,
				RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG,
				RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING,
				RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH,
			};

			const int *camp_to_ranktype = camp_to_ranktype_array[camp_type];
			if (NULL != camp_to_ranktype)
			{
				for (int history_cap_type = 0; history_cap_type < HISTORY_CAPABILITY_TYPE_MAX; ++ history_cap_type)
				{
					int randactivity_type = historycap_randactivity_type[history_cap_type];
					if (RandActivityManager::Instance().IsSyncRankData(randactivity_type))
					{
						const RandActivityRankBipinCfg *bipin_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(randactivity_type);
						if (NULL != bipin_cfg)
						{
							const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(history_cap_type);
							if (NULL != history_cap_item && history_cap_item->max_cap > 0 && history_cap_item->max_cap >= bipin_cfg->need_capability)
							{
								if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
								{
									int rank_type = camp_to_ranktype[history_cap_type];
									if (rank_type > 0 && rank_type < PERSON_RANK_TYPE_MAX)
									{
										rank_info.items[item_count].rank_type = rank_type;
										rank_info.items[item_count].rank_value = history_cap_item->max_cap;
										rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - history_cap_item->reach_timestamp;
										rank_info.items[item_count].record_index = 0; 

										UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
										m_role->GetGuildName(rank_info.items[item_count].flexible_name);

										++item_count;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 光环榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_HALO) > 0 && m_role->GetHalo()->GetHaloParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_HALO;
			rank_info.items[item_count].rank_value = m_role->GetHalo()->GetHaloParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_HALO);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 法宝榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FABAO) > 0 && m_role->GetFabaoManager()->GetParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_FABAO;
			rank_info.items[item_count].rank_value = m_role->GetFabaoManager()->GetParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FABAO);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 时装榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHIZHUANG) > 0 && m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_BODY].grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_SHIZHUANG;
			rank_info.items[item_count].rank_value = m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_BODY].grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHIZHUANG);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 时装武器(神兵)榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHIZHUANG_WUQI) > 0 && m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_WUQI].grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_SHIZHUANG_WUQI;
			rank_info.items[item_count].rank_value = m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_WUQI].grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHIZHUANG_WUQI);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 足迹榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FOOTPRINT) > 0 && m_role->GetFootPrint()->GetFootprintParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_FOOTPRINT;
			rank_info.items[item_count].rank_value = m_role->GetFootPrint()->GetFootprintParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FOOTPRINT);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 神弓榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENGONG) > 0 && m_role->GetShengong()->GetShengongParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_SHENGONG;
			rank_info.items[item_count].rank_value = m_role->GetShengong()->GetShengongParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENGONG);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 神翼榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENYI) > 0 && m_role->GetShenyi()->GetShenyiParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_SHENYI;
			rank_info.items[item_count].rank_value = m_role->GetShenyi()->GetShenyiParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENYI);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 全身装备强化总等级榜
	{
		if (m_role->GetEquipmentManager()->GetTotalStrengthen() > 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_EQUIP_STRENGTH_LEVEL;
			rank_info.items[item_count].rank_value = m_role->GetEquipmentManager()->GetTotalStrengthen();
			rank_info.items[item_count].flexible_int = m_role->GetEquipmentManager()->GetTotalStrengthen();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 全身宝石总等级榜
	{
		if (m_role->GetStoneManager()->GetTotalStoneLevel() > 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_STONE_TOATAL_LEVEL;
			rank_info.items[item_count].rank_value = m_role->GetStoneManager()->GetTotalStoneLevel();
			rank_info.items[item_count].flexible_int = m_role->GetStoneManager()->GetTotalStoneLevel();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 战骑榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FIGHT_MOUNT) > 0 && m_role->GetFightMountManager()->GetFightMountParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_FIGHT_MOUNT;
			rank_info.items[item_count].rank_value = m_role->GetFightMountManager()->GetFightMountParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FIGHT_MOUNT);
			rank_info.items[item_count].record_index = 0; 

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 世界题目对题榜
	{
		if (m_role->GetRightWorldAnswerNum() > 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER;
			rank_info.items[item_count].rank_value = m_role->GetRightWorldAnswerNum();
			rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - m_role->GetRightWorldAnswerNumChangeTime();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)									// 每日魅力榜
	{
		rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_DAY_CHARM;
		rank_info.items[item_count].rank_value = m_role->GetFriendManager()->GetDayAddCharm();
		rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetTotalCapability();
		rank_info.items[item_count].record_index = 0;

		UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
		m_role->GetGuildName(rank_info.items[item_count].flexible_name);

		++item_count;
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_FISHING))
	{
		//钓鱼榜
		if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_FISH_PLACE;
			rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetFishingscore();
			rank_info.items[item_count].record_index = 0;
			rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - m_role->GetRoleActivity()->GetFishingFishingTimestamp();

			++item_count;
		}

		//偷鱼榜
		if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_STEAL_FISH_PLACE;
			rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetStealfishscore();
			rank_info.items[item_count].record_index = 0;
			rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - m_role->GetRoleActivity()->GetFishingStealTimestamp();

			++item_count;
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PLANTING_TREE))
	{
		if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
		{
			int plangting_times = m_role->GetRoleActivity()->GetRAPlantingTreePlantingTimes();
			if (plangting_times > 0)
			{
				rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_PLANTING_TREE_PLANTING;
				rank_info.items[item_count].rank_value = plangting_times;
				rank_info.items[item_count].record_index = 0;

				++item_count;
			}
		}

		if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
		{
			int watering_times = m_role->GetRoleActivity()->GetRAPlantingTreeWateringTimes();
			if (watering_times > 0)
			{
				rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_PLANTING_TREE_WATERING;
				rank_info.items[item_count].rank_value = watering_times;
				rank_info.items[item_count].record_index = 0;

				++item_count;
			}
		}
	}

	{
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN))
		{
			if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
			{
				rank_info.items[item_count].rank_type = PERSON_RANK_CRYSTA_DALARAN_STEAL_NUM;
				rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetCrystalDalaranStealCount();
				rank_info.items[item_count].record_index = 0;
				rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - m_role->GetRoleActivity()->GetCrystalDalaranStealTimestamp();

				++item_count;
			}

			if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
			{
				rank_info.items[item_count].rank_type = PERSON_RANK_CRYSTA_DALARAN_BE_STEALED_NUM;
				rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetCrystalDalaranBeStealedCount();
				rank_info.items[item_count].record_index = 0;
				rank_info.items[item_count].flexible_ll = LONG_LONG_MAX_VALUE - m_role->GetRoleActivity()->GetCrystalDalaranBeStealedTimestamp();

				++item_count;
			}
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 符文塔层数榜
	{
		if (m_role->GetRuneSystem()->GetRuneTowerPassLayer() > 0 && m_role->GetRuneSystem()->GetRuneTowerTodayLayer() + 1 > 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_RUNE_TOWER_LAYER;
			rank_info.items[item_count].rank_value = m_role->GetRuneSystem()->GetRuneTowerPassLayer();
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetTotalCapability();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 腰饰榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_YAOSHI) > 0 && m_role->GetYaoShiManager()->GetYaoShiParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_YAOSHI;
			rank_info.items[item_count].rank_value = m_role->GetYaoShiManager()->GetYaoShiParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_YAOSHI);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 头饰榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_TOUSHI) > 0 && m_role->GetTouShiManager()->GetTouShiParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_TOUSHI;
			rank_info.items[item_count].rank_value = m_role->GetTouShiManager()->GetTouShiParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_TOUSHI);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 麒麟臂榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_QILINBI) > 0 && m_role->GetQiLinBiManager()->GetQiLinBiParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_QILINBI;
			rank_info.items[item_count].rank_value = m_role->GetQiLinBiManager()->GetQiLinBiParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_QILINBI);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 面具榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MASK) > 0 && m_role->GetMaskManager()->GetMaskParam()->grade > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_MASK;
			rank_info.items[item_count].rank_value = m_role->GetMaskManager()->GetMaskParam()->grade;
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MASK);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 灵珠榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_0) > 0 && m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_0) > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_LINGZHU;
			rank_info.items[item_count].rank_value = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_0);
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_0);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 仙宝榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_1) > 0 && m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_1) > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_XIANBAO;
			rank_info.items[item_count].rank_value = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_1);
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_1);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 灵童榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_2) > 0 && m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_2) > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_LINGTONG;
			rank_info.items[item_count].rank_value = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_2);
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_2);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 灵弓榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_3) > 0 && m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_3) > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_LINGGONG;
			rank_info.items[item_count].rank_value = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_3);
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_3);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 灵骑榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_4) > 0 && m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_4) > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_LINGQI;
			rank_info.items[item_count].rank_value = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_4);
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_4);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 尾焰榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_5) > 0 && m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_5) > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_WEIYAN;
			rank_info.items[item_count].rank_value = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_5);
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_5);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 手环榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_6) > 0 && m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_6) > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_SHOUHUAN;
			rank_info.items[item_count].rank_value = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_6);
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_6);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 尾巴榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_7) > 0 && m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_7) > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_WEIBA;
			rank_info.items[item_count].rank_value = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_7);
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_7);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 飞宠榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_8) > 0 && m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_8) > 1)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_FEICHONG;
			rank_info.items[item_count].rank_value = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_8);
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + UPGRADE_TYPE_8);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 披风榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_CLOAK) > 0 && m_role->GetCloakManager()->GetCloakLevel() > 0)
		{
			// 该等级有形象激活才上榜
			const CloakUpLevelCfg * curr_level_cfg = LOGIC_CONFIG->GetCloakConfig().GetUpLevelCfg(m_role->GetCloakManager()->GetCloakLevel());
			if (curr_level_cfg && curr_level_cfg->active_image > 0)
			{
				rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_CLOAK;
				rank_info.items[item_count].rank_value = m_role->GetCloakManager()->GetCloakLevel();
				rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_CLOAK);
				rank_info.items[item_count].record_index = 0;

				UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
				m_role->GetGuildName(rank_info.items[item_count].flexible_name);

				++item_count;
			}
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)													// 灵刃榜
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENBING) > 0 && m_role->GetShenBingManager()->GetShenBingLevel() > 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_SHENBING;
			rank_info.items[item_count].rank_value = m_role->GetShenBingManager()->GetShenBingLevel();
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENBING);
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 个人爬塔层数榜
	{
		if (m_role->GetRolePataFB()->GetPassLevel() > 0 && m_role->GetRolePataFB()->GetTodayLevel() >= 0)
		{
			rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_ROLE_PATA_LAYER;
			rank_info.items[item_count].rank_value = m_role->GetRolePataFB()->GetPassLevel();
			rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetTotalCapability();
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PROFESS_RANK))					    // 表白排行榜（男/女）活动
	{
		int rank_type = (FEMALE == m_role->GetSex() ? PERSON_RANK_TYPE_RA_PROFESS_FEMALE : PERSON_RANK_TYPE_RA_PROFESS_MALE);
		if (m_role->GetRoleActivity()->GetRAProfessScore() > 0 && item_count < SYNC_PERSON_RANK_INFO_SIZE)
		{
			int total_profess_num = m_role->GetRoleActivity()->GetRAProfessToNum() + m_role->GetRoleActivity()->GetRAProfessFromNum();
			rank_info.items[item_count].rank_type = rank_type;
			rank_info.items[item_count].rank_value = m_role->GetRoleActivity()->GetRAProfessScore();
			rank_info.items[item_count].flexible_int = total_profess_num;
			rank_info.items[item_count].record_index = 0;

			UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
			m_role->GetGuildName(rank_info.items[item_count].flexible_name);

			++item_count;
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_IMAGE_COMPETITION))   // 形象比拼
	{
		if (item_count < SYNC_PERSON_RANK_INFO_SIZE)
		{
			int rank_value = 0; long long flexible_ll = 0;
			if (ActivityShadow::Instance().GetImageCompetitionRankInfo(m_role, &rank_value, &flexible_ll))
			{
				rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_IMAGE_COMPETITION;
				rank_info.items[item_count].rank_value = rank_value;
				rank_info.items[item_count].flexible_ll = flexible_ll;
				rank_info.items[item_count].record_index = 0;

				UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));
				m_role->GetGuildName(rank_info.items[item_count].flexible_name);

				++item_count;
			}
		}
	}

	rank_info.item_count = item_count;
	if (rank_info.item_count > 0)
	{
		RankManager::Instance().OnSyncPersonRankInfo(m_role->GetUserId(), &rank_info);
	}
}

void RankShadow::UpdateToCross()
{
	if (m_role->IsInCross())
	{
		return;
	}

	static crossgameprotocal::GameCrossSyncCrossPersonRankInfo rank_info;

	rank_info.plat_type = m_role->GetRoleCross()->GetOriginPlatType();
	rank_info.role_id = UidToInt(m_role->GetUserId());
	rank_info.exp = m_role->GetExp();
	rank_info.level = m_role->GetLevel();
	m_role->GetName(rank_info.name);
	rank_info.sex = m_role->GetSex();
	rank_info.prof = m_role->GetProf();
	rank_info.camp = m_role->GetCamp();
	rank_info.vip_level = static_cast<char>(m_role->GetVip()->GetVipLevel());

	int item_count = 0;

	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	int limit_open_day = 0;
	if (item_count < crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE)
	{	
		limit_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_CAPABILITY_ALL);
		if (limit_open_day <= 0 || opengame_day >= limit_open_day)
		{
			rank_info.items[item_count].rank_type = CROSS_PERSON_RANK_TYPE_CAPABILITY_ALL;
			rank_info.items[item_count].rank_value = m_role->GetCapability()->GetTotalCapability();
			rank_info.items[item_count].record_index = 0;
			++item_count;
		}
	}

	if (item_count < crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE)
	{
		limit_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_WEEK_ADD_CHARM);
		if (limit_open_day <= 0 || opengame_day >= limit_open_day)
		{
			if (m_role->GetFriendManager()->GetWeekAddCharm() > 0)
			{
				rank_info.items[item_count].rank_type = CROSS_PERSON_RANK_TYPE_WEEK_ADD_CHARM;
				rank_info.items[item_count].rank_value = m_role->GetFriendManager()->GetWeekAddCharm();
				rank_info.items[item_count].record_index = 0;
				++item_count;
			}
		}
	}

	if (item_count < crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE)
	{
		limit_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_ROLE_LEVEL);
		if (limit_open_day <= 0 || opengame_day >= limit_open_day)
		{
			rank_info.items[item_count].rank_type = CROSS_PERSON_RANK_TYPE_ROLE_LEVEL;
			rank_info.items[item_count].rank_value = m_role->GetLevel();
			rank_info.items[item_count].record_index = 0;
			++item_count;
		}
	}

	if (item_count < crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE)
	{
		limit_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY);
		if (limit_open_day <= 0 || opengame_day >= limit_open_day)
		{
			if (m_role->GetCommonDataParam()->add_cap_for_rank > 0)
			{
				rank_info.items[item_count].rank_type = CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY;
				rank_info.items[item_count].rank_value = m_role->GetCommonDataParam()->add_cap_for_rank;
				rank_info.items[item_count].record_index = 0;
				++item_count;
			}
		}
	}

	if (item_count < crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE)
	{
		limit_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_ADD_CHARM);
		if (limit_open_day <= 0 || opengame_day >= limit_open_day)
		{
			if (m_role->GetCommonDataParam()->add_charm_for_rank > 0)
			{
				rank_info.items[item_count].rank_type = CROSS_PERSON_RANK_TYPE_ADD_CHARM;
				rank_info.items[item_count].rank_value = m_role->GetCommonDataParam()->add_charm_for_rank;
				rank_info.items[item_count].record_index = 0;
				++item_count;
			}
		}
	}

	rank_info.item_count = item_count;
	if (rank_info.item_count > 0)
	{
		// 发往跨服排行榜所在战区，正常为1战区
		int sendlen = sizeof(rank_info) - (crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE - rank_info.item_count) * sizeof(rank_info.items[0]);
		InternalComm::Instance().SendToCross((const char *)&rank_info, sendlen, CROSS_FUNCTION_TYPE_CROSS_RANK);
	}

	//////////////////////////////////////////////////////////////////////////
	item_count = 0;

	// 3v3 需要发往对应战区
	if (item_count < crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE)
	{
		limit_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_3V3_SCORE);
		if (limit_open_day <= 0 || opengame_day >= limit_open_day)
		{
			rank_info.items[item_count].rank_type = CROSS_PERSON_RANK_TYPE_3V3_SCORE;
			rank_info.items[item_count].rank_value = m_role->GetRoleCross()->GetCrossMultiuserChallengeScore();
			rank_info.items[item_count].flexible_ll = m_role->GetCapability()->GetTotalCapability();
			rank_info.items[item_count].record_index = 0;
			++item_count;
		}
	}
	rank_info.item_count = item_count;
	if (rank_info.item_count > 0)
	{
		int sendlen = sizeof(rank_info) - (crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE - rank_info.item_count) * sizeof(rank_info.items[0]);
		InternalComm::Instance().SendToCross((const char *)&rank_info, sendlen, CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE);
	}



	//////////////////////////////////////////////////////////////////////////
	item_count = 0;

	// 1v1 需要发往对应战区
	if (item_count < crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE)
	{
		limit_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_1V1_SCORE);
		if (limit_open_day <= 0 || opengame_day >= limit_open_day)
		{
			rank_info.items[item_count].rank_type = CROSS_PERSON_RANK_TYPE_1V1_SCORE;
			rank_info.items[item_count].rank_value = m_role->GetRoleCross()->GetCross1v1Score();
			rank_info.items[item_count].flexible_ll = m_role->GetCapability()->GetTotalCapability();
			rank_info.items[item_count].record_index = 0;
			++item_count;
		}
	}
	rank_info.item_count = item_count;
	if (rank_info.item_count > 0)
	{
		int sendlen = sizeof(rank_info) - (crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE - rank_info.item_count) * sizeof(rank_info.items[0]);
		InternalComm::Instance().SendToCross((const char*)& rank_info, sendlen, CROSS_ACTIVITY_TYPE_CROSS_1V1);
	}
}


