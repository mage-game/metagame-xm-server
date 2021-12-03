#include "titlemanager.hpp"
#include "obj/character/role.h"
#include "gamelog.h"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "other/goldvip/goldvipconfig.hpp"
#include "other/achieve/title/titleconfig.hpp"
#include "other/famousman/famousmanconfig.hpp"
#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "protocal/msgtitle.hpp"
#include "scene/scene.h"
#include "servercommon/string/gameworldstr.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "world.h"
#include "global/worldstatus/worldstatus.hpp"
#include "global/worldstatus3/worldstatus3.hpp"
#include "global/rank/topranker.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/rank/crosspersonrankshadow.hpp"
#include "other/camp/campconfig.hpp"
#include "other/jingling/jinglingmanager.h"
#include "item/itemextern.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/goldvip/goldvip.hpp"
#include "other/cross/rolecross.h"
#include "global/guild/guildmanager.hpp"
#include "config/guildactivityconfig.hpp"
#include "global/guild/guild.hpp"
#include "global/challengefield/challengefield.hpp"
#include "other/crossroleglobal/crossroleglobal.hpp"
#include "servercommon/serverconfig/crossfishingconfig.hpp"
#include "servercommon/serverconfig/crosspastureconfig.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"
#include "config/activityconfig/qunxianluandouconfig.hpp"
#include "global/usercache/usercache.hpp"
#include "config/activityconfig/gongchengzhanconfig.hpp"
#include "config/activityconfig/yizhandaodiconfig.hpp"
#include "other/fb/messbattle/messbattleconfig.hpp"
#include "other/fb/nightfight/nightfightfbconfig.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "gameworld/gstr.h"
#include "other/route/mailroute.hpp"
#include "servercommon/serverconfig/crossxiuluotowercommonconfig.hpp"
#include "other/redequipcollect/redequipcollect.hpp"
#include "config/activityconfig/guildbattleconfignew.hpp"
#include "other/mount/mountmanager.hpp"
#include "servercommon/serverconfig/crossliekunfbconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "global/challengefield/challengefieldconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"


TitleManager::TitleManager() : m_role(NULL), m_other_title_count(0), m_use_title_count(0), m_use_jingling_titleid(0), m_last_update_time(0)
{
	memset(m_use_title_list, 0, sizeof(m_use_title_list));
}

TitleManager::~TitleManager()
{

}

void TitleManager::Init(Role *role, const TitleParam &title_param)
{
	m_role = role;

	// 称号进阶保存的称号
	for (int i = 0; i < title_param.saved_count && i < MAX_TITLE_COUNT_TO_SAVE; ++i)
	{
		if (m_other_title_count >= MAX_OTHER_TITLE_COUNT) break;

		m_other_title_list[m_other_title_count++] = title_param.saved_title_list[i];
	}

	// 清掉动态计算的称号
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		m_role->GetCrossRoleGlobal()->ClearSaveInGlobalModule(ADJUST_CAP_TYPE_TITLE);
	}

// 	{
// 		// 算出跨服六界称号
// 		CrossGuildBattleGuildItem old_cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
// 		WorldStatus::Instance().GetCrossGuildBattleInfo(old_cross_guildbattle_list);
// 		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
// 		{
// 			if (old_cross_guildbattle_list[i].plat_type != m_role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type)
// 			{
// 				continue;
// 			}
// 
// 			if (old_cross_guildbattle_list[i].guild_id <= 0)
// 			{
// 				continue;
// 			}
// 
// 			if (m_role->GetGuildID() == old_cross_guildbattle_list[i].guild_id)
// 			{
// 				int cross_guildbattle_first = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_CROSS_GUILDBATTLE_FIRST);
// 				int cross_guildbattle_other = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_CROSS_GUILDBATTLE_OTHER);
// 				if (0 == i && 0 != cross_guildbattle_first)
// 				{
// 					if (this->AddTitle(cross_guildbattle_first, false, false, false, true))
// 					{
// 						this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, true, cross_guildbattle_first, i);
// 					}
// 				}
// 				else
// 				{
// 					if (0 != cross_guildbattle_other)
// 					{
// 						bool auto_use = !this->HasOtherTitle(cross_guildbattle_first);
// 						if (this->AddTitle(cross_guildbattle_other, false, false, false, auto_use))
// 						{
// 							this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, true, cross_guildbattle_other, i);
// 						}
// 					}
// 					break;
// 				}
// 			}
// 		}//for
// 	}

	// 算出攻城战称号
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		GuildID gongchengzhan_guild_id = WorldStatus::Instance().GetGCZWinGuild();
		int gongchengzhan_guild_title_id = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().cz_guild_member_title_id;
		GuildID my_guild_id = m_role->GetGuildID();
		if (my_guild_id != INVALID_GUILD_ID)
		{
			if (my_guild_id == gongchengzhan_guild_id)
			{
				int chengzhu_id = WorldStatus::Instance().GetGCZChengzhuID();
				if (m_role->GetUID() != chengzhu_id && m_role->GetLoverUid() != chengzhu_id)
				{
					if (this->AddTitle(gongchengzhan_guild_title_id, false, false, false, true))
					{
						this->MailOnTileStatusChange(ACTIVITY_TYPE_GONGCHENGZHAN, true, gongchengzhan_guild_title_id);
					}
				}
			}
			else
			{
				if (this->RemoveTitle(gongchengzhan_guild_title_id, false, false, true))
				{
					this->MailOnTileStatusChange(ACTIVITY_TYPE_GONGCHENGZHAN, false, gongchengzhan_guild_title_id);
				}
			}
		}
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		{
			// 算出天宫试练称号
			/*int pass_level = m_role->GetRolePataFB()->GetPassLevel();
			unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetPataFbTitle(pass_level);
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT)
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}*/
		}

		{
			// 算出精灵卡牌称号
			int card_level = m_role->GetJingLingMgr()->GetCardTotalLevel();
			unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetJinglingCardTitle(card_level);
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
				m_use_jingling_titleid = title_id;	//默认使用
			}

			// 算精灵王称号(同时装备冰凤和火凤)
			if (m_role->GetJingLingMgr()->HasSameJingLing(JINGLING_BINGFENG_ID) && m_role->GetJingLingMgr()->HasSameJingLing(JINGLING_HUOFENG_ID))
			{
				if (m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(JINGLINGWANG_TITLE_ID))
				{
					m_other_title_list[m_other_title_count++].title_id = JINGLINGWANG_TITLE_ID;
				}
			}
		}

		{
			// 算出名人堂称号
			for(int famous_type = FAMOUS_MAN_ROLE_LEVEL; famous_type < FAMOUS_MAN_MAX_TYPE; famous_type++)
			{
				// 获取玩家身上是否已经获取过名人堂称号，若获取过，即便当前公共数据那边没有，也给玩家称号
				CommonDataParam *role_common_param = m_role->GetCommonDataParam();
				bool aleady_has_title = (role_common_param->famous_man_flag & (1 << famous_type)) > 0;

				int famous_man_uid = WorldStatus::Instance().GetFamousManID(famous_type);
				if (aleady_has_title || famous_man_uid == m_role->GetUID())
				{
					unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetFamousManTitle(famous_type);
					if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
					{
						m_other_title_list[m_other_title_count++].title_id = title_id;
					}
				}
			}
		}

		{
			// 算出黄金vip称号
			if (m_role->GetGoldVip()->IsvalidGoldVip())
			{
				const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipTitleID());
				if (NULL != title_cfg)
				{
					if (0 != title_cfg->title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_cfg->title_id))
					{
						m_other_title_list[m_other_title_count++].title_id = title_cfg->title_id;
					}
				}
			}
		}

// 		{
// 			// 算出公会签到称号
// 			Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
// 			if (NULL != guild)
// 			{
// 				int member_signin_count = guild->GetGuildMemberManager().GetMemberSigninTotalCountToday();
// 				const GuildActivityOther &other_cfg = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg();
// 				if (member_signin_count >= other_cfg.signin_title_need_num)
// 				{
// 					int sign_title_id = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_GUILD_SIGN);
// 					if (0 != sign_title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(sign_title_id))
// 					{
// 						m_other_title_list[m_other_title_count++].title_id = sign_title_id;
// 						m_role->GetCrossRoleGlobal()->SetSaveInGlobalModule(ADJUST_CAP_TYPE_TITLE, sign_title_id);
// 					}
// 				}
// 			}
// 		}

		{
			// 各种成就称号
			unsigned short title_id = 0;
			int condition = 0;

			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_XUNBAO, m_role->GetEquipXunbaoTimes());
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}

			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_LIEQU, m_role->GetJLXunbaoTimes());
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}

			condition = m_role->GetShizhuang()->GetHasActiveShizhuang(SHIZHUANG_TYPE_BODY);
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_SHIZHUANG, condition);
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}

			condition = m_role->GetShizhuang()->GetHasActiveShizhuang(SHIZHUANG_TYPE_WUQI);
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_WEAPON, condition);
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}

			condition = m_role->GetJingLingMgr()->GetJingLingPhantomCount();
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITILE_TYPE_PHANTOM, condition);
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}

			condition = m_role->GetHCHZKillCount();
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_HCHZ_KILLER, condition);
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}

			condition = m_role->GetGCZKillCount();
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_GCZ_100_KILLER, condition);
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}

			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_GCZ_1000_KILLER, condition);
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}

			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_GCZ_10000_KILLER, condition);
			if (0 != title_id && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(title_id))
			{
				m_other_title_list[m_other_title_count++].title_id = title_id;
			}	
		}

		{
			// 魅力排行榜
			this->OnChangeCharmRankTitle(SYNC_REASON_ENTER_SCENE, true);
			// 世界答题
			this->OnChangeWorldQuestionRankTitle(SYNC_REASON_ENTER_SCENE, true);
		}

		{
			// 活动称号需要在RecalAttr(init)之前加到m_other_title_list
			WorldStatus::Instance().SynActivityTitleInfo(role, SYNC_REASON_ENTER_SCENE);
		}

		{
			// 其他杂项称号需要在RecalAttr(init)之前加到m_other_title_list
			WorldStatus::Instance().SynOtherTitleInfo(role, SYNC_REASON_ENTER_SCENE);
		}

		{
			// 排行榜称号需要在RecalAttr(init)之前加到m_other_title_list
			PersonRankInfo personrank_info;
			TopRanker::Instance().GetTopRankInfo(role->GetUID(), &personrank_info);
			this->OnTopRankInfo(personrank_info, SYNC_REASON_ENTER_SCENE);
		}

		{
			// 跨服排行榜称号需要在RecalAttr(init)之前加到m_other_title_list
			CrossPersonRankInfo cross_personrank_info;
			CrossPersonRankShadow::Instance().GetTopRankInfo(role->GetUID(), &cross_personrank_info);
			this->OnCrossTopRankInfo(cross_personrank_info, SYNC_REASON_ENTER_SCENE);

			this->CheckAddCrossGuildKillRankTitle(SYNC_REASON_ENTER_SCENE);
		}
		{
			// 竞技场排名称号. 改为结算时通过邮件发放 modify 2019-03-10
			//this->OnChallengeFieldRankInfo(ChallengeField::Instance().GetRoleRewardRankPos(role->GetUID()), SYNC_REASON_ENTER_SCENE);
		}

		{   //乱斗战场，怒战九霄
			WorldStatus2::Instance().SyncRoomActivityTitleInfo(role, SYNC_REASON_ENTER_SCENE);
		}

		{
			WorldStatus2::Instance().SyncCrossActivityTitleInfo(role, SYNC_REASON_ENTER_SCENE);
		}

	}

	{
		// 红装收集称号
		std::vector<unsigned short> title_list;
		bool ret = m_role->GetRedEquipCollect()->GetActiveTitleList(title_list);
		if (ret)
		{
			for (std::vector<unsigned short>::iterator it = title_list.begin(); it != title_list.end() && m_other_title_count < MAX_OTHER_TITLE_COUNT; ++it)
			{
				if (!HasOtherTitle(*it))
				{
					m_other_title_list[m_other_title_count++].title_id = *it;
				}
			}
		}
	}

	//如果在跨服，检查自己在本服是否有该称号，如果有，则加上
	if (CrossConfig::Instance().IsHiddenServer())
	{
		const std::vector<CapAdjustKeyObj> &cap_vec = m_role->GetCrossRoleGlobal()->GetKeyObj(ADJUST_CAP_TYPE_TITLE);
		for (const auto &key : cap_vec)
		{
			if (key.param_0 > 0 && m_other_title_count < MAX_OTHER_TITLE_COUNT && !HasOtherTitle(key.param_0))
			{
				m_other_title_list[m_other_title_count++].title_id = key.param_0;
			}
		}
	}

	{
		// 顶在头上的称号
		// 以前能佩戴3个称号， 现在改成只能佩戴一个称号，如果要改回三个将m_use_title_count < 1去掉
		//策划需求，外部称号获得时自动佩戴，故此处特殊处理，切换场景时获得称号佩戴后不从数据库初始化m_use_title_list，如果要改回佩戴三个称号，去掉条件语句即可(2019-1-7)
		if (!(m_use_title_list[0] != 0 && m_use_title_count == 1))	//仅适用于只能佩戴一个称号
		{
			m_use_title_count = 0;
		}
		for (int i = 0; i < MAX_USE_TITLE_COUNT && m_use_title_count < 1 && m_use_title_count < MAX_USE_TITLE_COUNT; ++ i)
		{
			const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_param.used_title_list[i]);
			if (NULL != title_cfg && this->HasOtherTitle(title_param.used_title_list[i]) &&
				TITLESOURCETYPE_JINGLINGCARD != title_cfg->source_type)		// 精灵称号不会进使用列表，之前的设计精灵称号会进使用列表，所以这里对旧角色进行处理.新项目时可删
			{
				m_use_title_list[m_use_title_count] = title_param.used_title_list[i];
				++ m_use_title_count;
			}
		}
	}

	m_last_update_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
}

void TitleManager::GetOtherInitParam(TitleParam *param)
{
	param->Reset();

	for (int i = 0; i < m_use_title_count && i < MAX_USE_TITLE_COUNT; ++ i)
	{
		param->used_title_list[i] = m_use_title_list[i];
	}

	for (int i = 0; i < m_other_title_count && i < MAX_OTHER_TITLE_COUNT && param->saved_count < MAX_TITLE_COUNT_TO_SAVE; ++i)
	{
		const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(m_other_title_list[i].title_id);
		if (NULL == title_cfg) continue;

		// 要保存的类型陆续在这添加
		//if (TITLESOURCETYPE_TITLECARD == title_cfg->source_type || TITLESOURCETYPE_ACTIVITY == title_cfg->source_type ||
		//	TITLESOURCETYPE_OTHER == title_cfg->source_type || TITLESOURCETYPE_CHALLENGE_OUTSIDE_CFG == title_cfg->source_type)
		{
			param->saved_title_list[param->saved_count++] = m_other_title_list[i];
		}
	}
}

void TitleManager::OnRoleLogin()
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		this->CheckShowChallengeFieldRank();
	}
	
}

void TitleManager::Update(time_t now_second)
{
	if (now_second - m_last_update_time >= 1)
	{
		bool own_chged = false;
		bool used_chged = false;

		for (int i = 0; i < m_other_title_count && i < MAX_OTHER_TITLE_COUNT; ++i)
		{
			if (m_other_title_list[i].expired_time > 0 && m_other_title_list[i].expired_time <= now_second)
			{
				if (this->IsUsed(m_other_title_list[i].title_id))
				{
					used_chged = true;
				}

				this->RemoveTitle(m_other_title_list[i].title_id, false, false, true);
				own_chged = true;
			}
			else if (m_other_title_list[i].title_id == CrossLieKunFbConfig::Instance().GetOtherCfg().title_id && !m_role->IsInCross())
			{
				int guild_id = WorldStatus2::Instance().GetCommonData2().ling_kun_win_guild_id;
				if (m_role->GetGuildID() != guild_id)
				{
					used_chged = true;
				}
				if (used_chged)
				{
					this->RemoveTitle(m_other_title_list[i].title_id, false, false, true);
					own_chged = true;
					this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_LIEKUN_FB, false, CrossLieKunFbConfig::Instance().GetOtherCfg().title_id);
				}
			}
		}

		{
			GuildID guildbattle_mengzhu_guild_id = WorldStatus::Instance().GetGBMengzhuGuildID();
			if (guildbattle_mengzhu_guild_id != INVALID_GUILD_ID)
			{
				int guildbattle_title_id = LOGIC_CONFIG->GetTitlePool().GetActivityTitle(ACTIVITY_TYPE_GUILDBATTLE);
				if (m_role->GetGuildID() == guildbattle_mengzhu_guild_id)
				{
					if (this->AddTitle(guildbattle_title_id, false, false, false, true))
					{
						own_chged = true;
						//this->MailOnTileStatusChange(ACTIVITY_TYPE_GUILDBATTLE,true,guildbattle_title_id);

						EventHandler::Instance().OnGetSceneActivityReward(m_role, ACTIVITY_TYPE_GUILDBATTLE, guildbattle_title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
					}

					const GuildBattleOtherCfg  & othe_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg();
					if (m_role->GetGuildPost() == GUILD_POST_TUANGZHANG)
					{
						bool is_change = false;
						is_change = m_role->GetMountManager()->ActiveSpecialImage(othe_cfg.mount_special_image_id, false);
						if (is_change)
						{
							EventHandler::Instance().OnGetSceneActivityReward(m_role, ACTIVITY_TYPE_GUILDBATTLE, othe_cfg.mount_special_image_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_MOUNT);
						}
					}
					else
					{
						m_role->GetMountManager()->UnActiveSpecialImage(othe_cfg.mount_special_image_id);
					}
				}
				else
				{
					if (this->RemoveTitle(guildbattle_title_id, false, false, true))
					{
						own_chged = true;
						this->MailOnTileStatusChange(ACTIVITY_TYPE_GUILDBATTLE, false, guildbattle_title_id);
						const GuildBattleOtherCfg  & othe_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg();
						m_role->GetMountManager()->UnActiveSpecialImage(othe_cfg.mount_special_image_id);
					}
				}
			}
		}

		if (!CrossConfig::Instance().IsHiddenServer())
		{
			int sign_title_id = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_GUILD_SIGN);
			Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
			if (NULL == guild)
			{
				if (this->RemoveTitle(sign_title_id, false, false, true))
				{
					own_chged = true;
				}
			}
			else
			{
				int member_signin_count = guild->GetGuildMemberManager().GetMemberSigninTotalCountToday();
				const GuildActivityOther &other_cfg = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg();
				if (member_signin_count >= other_cfg.signin_title_need_num)
				{
					if (this->AddTitle(sign_title_id, false, false, false, true))
					{
						own_chged = true;
					}
				}
				else
				{
					if (this->RemoveTitle(sign_title_id, false, false, true))
					{
						own_chged = true;
					}
				}
			}
		}

		//if (!CrossConfig::Instance().IsHiddenServer())
		{
			//跨服六界
			CrossGuildBattleGuildItem old_cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
			WorldStatus::Instance().GetCrossGuildBattleInfo(old_cross_guildbattle_list);
			int cross_guildbattle_first = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_CROSS_GUILDBATTLE_FIRST);
			int cross_guildbattle_other = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_CROSS_GUILDBATTLE_OTHER);
			bool have_first = false; bool have_other = false;
			for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
			{
				if (old_cross_guildbattle_list[i].plat_type != m_role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type)
				{
					continue;
				}

				if (old_cross_guildbattle_list[i].guild_id <= 0)
				{
					continue;
				}

				if (m_role->GetGuildID() == old_cross_guildbattle_list[i].guild_id)
				{
					if (0 == i)
					{
						have_first = true;
						if (this->AddTitle(cross_guildbattle_first, false, false, false, true))
						{
							own_chged = true;
							//this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, true, cross_guildbattle_first, 0);

							EventHandler::Instance().OnGetSceneActivityReward(m_role, CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, cross_guildbattle_first, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
						}
					}
					else
					{
						have_other = true;
						bool auto_use = !this->HasOtherTitle(cross_guildbattle_first);
						if (this->AddTitle(cross_guildbattle_other, false, false, false, auto_use))
						{
							own_chged = true;
							//this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, true, cross_guildbattle_other, i);

							if (auto_use)
							{
								EventHandler::Instance().OnGetSceneActivityReward(m_role, CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, cross_guildbattle_other, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
							}
						}
						break;
					}
				}
			}

			if (!have_first && 0 != cross_guildbattle_first)
			{
				if (this->RemoveTitle(cross_guildbattle_first, false, false, true))
				{
					own_chged = true;
					this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, false, cross_guildbattle_first);
				}

			}

			if(!have_other && 0 != cross_guildbattle_other)
			{
				if (this->RemoveTitle(cross_guildbattle_other, false, false, true))
				{
					own_chged = true;
					this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, false, cross_guildbattle_other);
				}
			}
		}

		if(!CrossConfig::Instance().IsHiddenServer())
		{
			GuildID gongchengzhan_guild_id = WorldStatus::Instance().GetGCZWinGuild();
			GuildID my_guild_id = m_role->GetGuildID();
			int gongchengzhan_guild_title_id = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().cz_guild_member_title_id;
			if (my_guild_id != INVALID_GUILD_ID)
			{
				if (my_guild_id == gongchengzhan_guild_id)
				{
					int chengzhu_id = WorldStatus::Instance().GetGCZChengzhuID();
					if (m_role->GetUID() == chengzhu_id || m_role->GetLoverUid() == chengzhu_id)
					{
						this->RemoveTitle(gongchengzhan_guild_title_id, false, false, false);
					}
					else if (this->AddTitle(gongchengzhan_guild_title_id, false, false, false, true))
					{
						own_chged = true;
						//this->MailOnTileStatusChange(ACTIVITY_TYPE_GONGCHENGZHAN, true, gongchengzhan_guild_title_id);

						EventHandler::Instance().OnGetSceneActivityReward(m_role, ACTIVITY_TYPE_GONGCHENGZHAN, gongchengzhan_guild_title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
					}
				}
				else
				{
					if (this->RemoveTitle(gongchengzhan_guild_title_id, false, false, true))
					{
						own_chged = true;
						this->MailOnTileStatusChange(ACTIVITY_TYPE_GONGCHENGZHAN, false, gongchengzhan_guild_title_id);
					}
				}
			}
			else if (this->RemoveTitle(gongchengzhan_guild_title_id, false, false, true))
			{
				own_chged = true;
				this->MailOnTileStatusChange(ACTIVITY_TYPE_GONGCHENGZHAN, false, gongchengzhan_guild_title_id);
			}
		}

		{
			int gold_vip_title_id = LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipTitleID();
			if (m_role->GetGoldVip()->IsvalidGoldVip())
			{
				if (this->AddTitle(gold_vip_title_id, false, false, false, true))
				{
					own_chged = true;
				}
			}
			else
			{
				if (this->RemoveTitle(gold_vip_title_id, false, false, true))
				{
					own_chged = true;
				}
			}
		}

		if (own_chged)
		{
			if (used_chged) this->BroadCastUsedTitle();

			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
			this->SendAllTitle();
		}

		m_last_update_time = static_cast<unsigned int>(now_second);
	}
}

void TitleManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		unsigned short title_id_list[MAX_OWN_TITLE_COUNT];
		short title_count = this->GetAllTitleList(title_id_list);

		for (int i = 0; i < title_count; ++i)
		{
			this->AttachAttr(m_attrs_add, m_other_title_list[i].title_id, m_other_title_list[i].title_grade);
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_TITLE, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

bool TitleManager::AddTitle(unsigned short title_id, bool syn_title_info, bool need_recal, bool notify_client, bool auto_use)
{
	const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_id);
	if (NULL == title_cfg) return false;

	if (m_other_title_count >= MAX_OTHER_TITLE_COUNT)
	{
		if (notify_client) m_role->NoticeNum(errornum::EN_TITLE_ALREADY_FULL);
		return false;
	}
	
	if (this->HasOtherTitle(title_id))
	{
		if (notify_client) m_role->NoticeNum(errornum::EN_TITLE_ALREADY_HAVE);

		// 检查是否要加到跨服数据里去
		m_role->GetCrossRoleGlobal()->SetSaveInGlobalModule(ADJUST_CAP_TYPE_TITLE, title_id);
		return false;
	}

	m_other_title_list[m_other_title_count].title_id = title_id;
	m_other_title_list[m_other_title_count].expired_time = 0;
	unsigned int expired_time = 0;

	if (title_cfg->valid_secs > 0)
	{
		m_other_title_list[m_other_title_count].expired_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + title_cfg->valid_secs;
		expired_time = m_other_title_list[m_other_title_count].expired_time;
	}
	
	++ m_other_title_count;

	if (auto_use)
	{
		if (TITLESOURCETYPE_JINGLINGCARD == title_cfg->source_type)
		{
			if(m_use_jingling_titleid != title_id)
			{
				m_use_jingling_titleid = title_id;
				this->BroadCastUsedTitle();
			}
		}
		else
		{
			// 以前能佩戴3个称号， 现在改成只能佩戴一个称号，如果要改回三个将m_use_title_count < 1去掉
			memset(m_use_title_list, 0, sizeof(m_use_title_list));
			m_use_title_count = 0;
			if (m_use_title_count < 1 && m_use_title_count < MAX_USE_TITLE_COUNT && !this->IsUsed(title_id))
			{
				m_use_title_list[m_use_title_count] = title_id;
				++ m_use_title_count;
				this->BroadCastUsedTitle();
			}
		}
	}

	if (syn_title_info) this->SendAllTitle();
	if (notify_client) this->OnTitleOwnStatusChange(TITLE_ACTION_TYPE_GET, title_id);
	if (need_recal)m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);

	// 检查是否要加到跨服数据里去
	m_role->GetCrossRoleGlobal()->SetSaveInGlobalModule(ADJUST_CAP_TYPE_TITLE, title_id);

	gamelog::g_log_title.printf(LL_INFO, "Title::AddNewTitle Succ user[%d %s], new_title_id[%d], new_title_expired_time[%d]",
		m_role->GetUID(), m_role->GetName(), title_id, expired_time);

	return true;
}

bool TitleManager::RemoveTitle(unsigned short title_id, bool syn_title_info, bool need_recal, bool notify_client)
{
	if (!this->HasOtherTitle(title_id))
	{
		return false;
	}
	
	//如果在隐藏服，并且原服有这个称号，则不移除
	if (CrossConfig::Instance().IsHiddenServer() && m_role->GetCrossRoleGlobal()->CheckHaveSaveInGlobalModule(ADJUST_CAP_TYPE_TITLE, title_id))
	{
		return false;
	}

	for (int i = 0; i < m_other_title_count; ++ i)
	{
		if (title_id == m_other_title_list[i].title_id)
		{
			m_other_title_list[i] = m_other_title_list[m_other_title_count - 1];
			-- m_other_title_count;
		}
	}

	if (this->IsUsed(title_id))
	{
		for (int i = 0; i < m_use_title_count; ++ i)
		{
			if (title_id == m_use_title_list[i])
			{
				m_use_title_list[i] = m_use_title_list[m_use_title_count - 1];
				-- m_use_title_count;
				if (m_use_title_count == 0)
				{
					m_use_title_list[0] = 0;
				}
				this->BroadCastUsedTitle();
			}
		}
	}

	if (syn_title_info) this->SendAllTitle();
	if (notify_client) this->OnTitleOwnStatusChange(TITLE_ACTION_TYPE_LOST, title_id);
	if (need_recal)m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		// 同步过去
		m_role->GetCrossRoleGlobal()->ClearSaveInGlobalModule(ADJUST_CAP_TYPE_TITLE, title_id);
	}

	gamelog::g_log_title.printf(LL_INFO, "Title::RemoveTitle Succ user[%d %s], remove_title_id[%d]",
		m_role->GetUID(), m_role->GetName(), title_id);

	return true;
}

void TitleManager::UseTitleList(unsigned short used_title_list[MAX_USE_TITLE_COUNT])
{
	m_use_title_count = 0;
	// 以前能佩戴3个称号， 现在改成只能佩戴一个称号，如果要改回三个将m_use_title_count < 1去掉
	for (int i = 0; i < MAX_USE_TITLE_COUNT && m_use_title_count < 1 && m_use_title_count < MAX_USE_TITLE_COUNT; ++ i)
	{
		if (this->HasOtherTitle(used_title_list[i]))
		{
			m_use_title_list[m_use_title_count] = used_title_list[i];
			++ m_use_title_count;
		}
	}

	this->BroadCastUsedTitle();
	m_role->NoticeNum(noticenum::NT_TITLE_USE_SUCC);
}

int TitleManager::GetUseTitleList(unsigned short used_title_list[MAX_USE_TITLE_COUNT])
{
	for (int i = 0; i < m_use_title_count && i < MAX_USE_TITLE_COUNT; ++ i)
	{
		used_title_list[i] = m_use_title_list[i];
	}

	return m_use_title_count;
}

short TitleManager::GetAllTitleList(unsigned short title_id_list[MAX_OWN_TITLE_COUNT])
{
	if (NULL == title_id_list) return 0;

	int count = 0;
	for (int i = 0; i < m_other_title_count && count < MAX_OWN_TITLE_COUNT; ++i)
	{
		title_id_list[count] = m_other_title_list[i].title_id;
		++ count;
	}

	return count;
}

bool TitleManager::IsUsed(unsigned short title_id)
{
	if (title_id > 0) 
	{
		for (int i = 0; i < m_use_title_count; ++ i)
		{
			if (title_id == m_use_title_list[i])
			{
				return true;
			}
		}
	}

	return false;
}

void TitleManager::SendUsedTitle()
{
	static Protocol::SCUsedTitleList utl;
	utl.use_jingling_title = m_use_jingling_titleid;
	utl.use_title_count = static_cast<short>(m_use_title_count);
	memcpy(utl.use_title_list, m_use_title_list, sizeof(utl.use_title_list));

	int send_len = sizeof(utl) - sizeof(utl.use_title_list[0]) * (MAX_USE_TITLE_COUNT - utl.use_title_count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&utl, send_len);
}

void TitleManager::SendAllTitle()
{
	static Protocol::SCTitleList tl;
	tl.title_count = 0;
	int title_grade_count = 0;
	memset(tl.title_upgrade_list, 0, sizeof(tl.title_upgrade_list));
	memset(tl.title_id_list, 0, sizeof(tl.title_id_list));

	for (int i = 0; i < m_other_title_count && tl.title_count < MAX_OWN_TITLE_COUNT; ++i)
	{
		tl.title_id_list[tl.title_count].title_id = m_other_title_list[i].title_id;
		tl.title_id_list[tl.title_count].reserve = 0;
		tl.title_id_list[tl.title_count].expired_time = m_other_title_list[i].expired_time;
		++ tl.title_count;

		const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(m_other_title_list[i].title_id);
		if (NULL != title_cfg)
		{
			if (TITLESOURCETYPE_TITLECARD == title_cfg->source_type) 
			{
				tl.title_upgrade_list[title_grade_count].title_id = m_other_title_list[i].title_id;
				tl.title_upgrade_list[title_grade_count].grade = m_other_title_list[i].title_grade;
				title_grade_count++;
			}
		}
	}

	int sendlen = sizeof(tl) - sizeof(Protocol::SCTitleList::TitleInfo) * (MAX_OWN_TITLE_COUNT - tl.title_count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tl, sendlen);
}

void TitleManager::BroadCastUsedTitle()
{
	Protocol::SCRoleUsedTitleChange utc;
	utc.obj_id = m_role->GetId();
	utc.use_jingling_title = m_use_jingling_titleid;
	utc.use_title_count = m_use_title_count;
	memcpy(utc.used_title_list, m_use_title_list, sizeof(utc.used_title_list));

	if (NULL != m_role->GetScene()) 
	{
		int send_len = sizeof(utc) - sizeof(utc.used_title_list[0]) * (MAX_USE_TITLE_COUNT - utc.use_title_count);
		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&utc, send_len);
	}
	SendUsedTitle();
}

void TitleManager::OnCrossTopRankInfo(const CrossPersonRankInfo &cross_personrank_info, int reason)
{
	std::set<unsigned short> old_rank_title_set;
	std::set<unsigned short> new_rank_title_set;

	{
		// 收集已有的排行榜称号 和 现在应该有的排行榜称号
		for (int i = 0; i < m_other_title_count; ++i)
		{
			const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(m_other_title_list[i].title_id);
			if (NULL == title_cfg) continue;

			if (TITLESOURCETYPE_CROSS_RANK == title_cfg->source_type) 
			{
				old_rank_title_set.insert(m_other_title_list[i].title_id);
			}
		}

		for (int top_rank_type = CROSS_RANK_TOP_TYPE_MIN; top_rank_type < CROSS_RANK_TOP_TYPE_MAX; ++top_rank_type)
		{
			int rank = cross_personrank_info.rank_list[top_rank_type];
			if (rank > 0 && rank <= CROSS_PERSON_RANK_MAX_SIZE)
			{
				unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetCrossTopRankTitle(top_rank_type, rank);

				const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_id);
				if (NULL == title_cfg) continue;

				new_rank_title_set.insert(title_id);
			}
		}
	}

	bool title_chged = false;
	{
		// 删掉排行之后失去的称号
		std::set<unsigned short>::const_iterator old_iter;
		for (old_iter = old_rank_title_set.begin(); old_rank_title_set.end() != old_iter; ++old_iter)
		{
			unsigned short old_title_id = *old_iter;

			// 之前有，现在没有，干掉
			if (new_rank_title_set.end() == new_rank_title_set.find(old_title_id))
			{
				this->RemoveTitle(old_title_id, false, false, SYNC_REASON_ENTER_SCENE != reason);
				title_chged = true;
			}
		}
	}

	{
		// 添加新增排行榜称号
		std::set<unsigned short>::const_iterator new_iter;
		for (new_iter = new_rank_title_set.begin(); new_rank_title_set.end() != new_iter; ++new_iter)
		{
			unsigned short new_title_id = *new_iter;

			// 之前没有，现在有，增加
			if (old_rank_title_set.end() == old_rank_title_set.find(new_title_id))
			{
				this->AddTitle(new_title_id, false, false, SYNC_REASON_ENTER_SCENE != reason, SYNC_REASON_ENTER_SCENE != reason);
				title_chged = true;
			}
		}
	}

	if (title_chged)
	{
		if (SYNC_REASON_ENTER_SCENE != reason)
		{
			this->SendAllTitle();
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
		}
	}
}

void TitleManager::OnTopRankInfo(const PersonRankInfo &personrank_info, int reason)
{
	std::set<unsigned short> old_rank_title_set;
	std::set<unsigned short> new_rank_title_set;

	{
		// 收集已有的排行榜称号 和 现在应该有的排行榜称号
		for (int i = 0; i < m_other_title_count; ++i)
		{
			const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(m_other_title_list[i].title_id);
			if (NULL == title_cfg) continue;

			if (TITLESOURCETYPE_RANK == title_cfg->source_type) 
			{
				old_rank_title_set.insert(m_other_title_list[i].title_id);
			}
		}

		for (int top_rank_type = RANK_TOP_TYPE_MIN; top_rank_type < RANK_TOP_TYPE_MAX; ++top_rank_type)
		{
			int rank = personrank_info.person_rank_list[top_rank_type];
			if (rank > 0 && rank <= TOP_TEN)
			{
				unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetTopRankTitle(top_rank_type, rank,m_role->GetLevel());

				const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_id);
				if (NULL == title_cfg) continue;

				new_rank_title_set.insert(title_id);
			}
		}
	}

	bool title_chged = false;
	{
		// 删掉排行之后失去的称号
		std::set<unsigned short>::const_iterator old_iter;
		for (old_iter = old_rank_title_set.begin(); old_rank_title_set.end() != old_iter; ++old_iter)
		{
			unsigned short old_title_id = *old_iter;

			// 之前有，现在没有，干掉
			if (new_rank_title_set.end() == new_rank_title_set.find(old_title_id))
			{
				this->RemoveTitle(old_title_id, false, false, SYNC_REASON_ENTER_SCENE != reason);
				title_chged = true;
			}
		}
	}

	{
		// 添加新增排行榜称号
		std::set<unsigned short>::const_iterator new_iter;
		for (new_iter = new_rank_title_set.begin(); new_rank_title_set.end() != new_iter; ++new_iter)
		{
			unsigned short new_title_id = *new_iter;

			// 之前没有，现在有，增加
			if (old_rank_title_set.end() == old_rank_title_set.find(new_title_id))
			{
				this->AddTitle(new_title_id, false, false, SYNC_REASON_ENTER_SCENE != reason, SYNC_REASON_ENTER_SCENE != reason);
				title_chged = true;
			}
		}
	}

	if (title_chged)
	{
		if (SYNC_REASON_ENTER_SCENE != reason)
		{
			this->SendAllTitle();
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
		}
	}
}

void TitleManager::OnActivityTitleInfo(const ActivityTitleInfo &activity_info, int reason)
{
	const int role_id = UidToInt(m_role->GetUserId());
	bool title_chged = false;

	int xianmengzhan_title_id = LOGIC_CONFIG->GetTitlePool().GetActivityTitle(ACTIVITY_TYPE_XIANMENGZHAN);
	this->OnOneTitle(xianmengzhan_title_id, role_id == activity_info.xianmengzhan_mengzhu_id, reason, &title_chged);

	{
		bool gcz_chg = false;
		int gongchengzhan_title_id = LOGIC_CONFIG->GetTitlePool().GetActivityTitle(ACTIVITY_TYPE_GONGCHENGZHAN);
		this->OnOneTitle(gongchengzhan_title_id, role_id == activity_info.gongchengzhan_mengzhu_id, reason, &gcz_chg);
		if(gcz_chg)
		{
			title_chged = true;
			if (role_id != activity_info.gongchengzhan_mengzhu_id)
			{
				this->MailOnTileStatusChange(ACTIVITY_TYPE_GONGCHENGZHAN, role_id == activity_info.gongchengzhan_mengzhu_id, gongchengzhan_title_id, 1);
			}

			if (role_id == activity_info.gongchengzhan_mengzhu_id)
			{
				EventHandler::Instance().OnGetSceneActivityReward(m_role, ACTIVITY_TYPE_GONGCHENGZHAN, gongchengzhan_title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
			}
		}
	}

	//一战到底称号处理
	{
		bool first_chg = false;
		int yizhandaodi_first_title = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetRankTitle(0);
		this->OnOneTitle(yizhandaodi_first_title, role_id == activity_info.yizhandaodi_win_user_id, reason, &first_chg);
		if (first_chg)
		{
			title_chged = true;
			this->MailOnTileStatusChange(ACTIVITY_TYPE_YIZHANDAODI, role_id == activity_info.yizhandaodi_win_user_id, yizhandaodi_first_title, 1);
		}
		bool second_chg = false;
		int yizhandaodi_second_title = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetRankTitle(1);
		this->OnOneTitle(yizhandaodi_second_title, role_id == activity_info.yizhandaodi_second_user_id, reason, &second_chg);
		if (second_chg)
		{
			title_chged = true;
			this->MailOnTileStatusChange(ACTIVITY_TYPE_YIZHANDAODI, role_id == activity_info.yizhandaodi_second_user_id, yizhandaodi_second_title, 2);
		}

		bool third_chg = false;
		int yizhandaodi_third_title = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetRankTitle(2);
		this->OnOneTitle(yizhandaodi_third_title, role_id == activity_info.yizhandaodi_third_user_id, reason, &third_chg);
		if (third_chg)
		{
			title_chged = true;
			this->MailOnTileStatusChange(ACTIVITY_TYPE_YIZHANDAODI, role_id == activity_info.yizhandaodi_third_user_id, yizhandaodi_third_title, 3);
		}
	}

	UserCacheNode *user_cache = UserCacheManager::Instance().GetUserNode(activity_info.gongchengzhan_mengzhu_id);
	if (nullptr != user_cache)
	{
		int gongchengzhan_lover_title_id = 0;
		if (FEMALE == m_role->GetSex())
		{
			gongchengzhan_lover_title_id = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().cz_wife_title_id;
		}
		else
		{
			gongchengzhan_lover_title_id = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().cz_husband_title_id;
		}

		this->OnOneTitle(gongchengzhan_lover_title_id, m_role->GetLoverUid() == user_cache->uid, reason, &title_chged);

		if (title_chged && m_role->GetLoverUid() == user_cache->uid)
		{
			EventHandler::Instance().OnGetSceneActivityReward(m_role, ACTIVITY_TYPE_GONGCHENGZHAN, gongchengzhan_lover_title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
		}
	}

	//群仙乱斗称号处理
	{
		for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; ++i)
		{
			bool qun_xian_chg = false;
			int title_id = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetTitleID(i);
			this->OnOneTitle(title_id, role_id == activity_info.qunxian_luandou_user_id_list[i], reason, &qun_xian_chg);
			if (qun_xian_chg)
			{
				title_chged = true;

				if (role_id != activity_info.qunxian_luandou_user_id_list[i])
				{
					this->MailOnTileStatusChange(ACTIVITY_TYPE_QUNXIANLUANDOU, role_id == activity_info.qunxian_luandou_user_id_list[i], title_id, 1);
				}

				if (role_id == activity_info.qunxian_luandou_user_id_list[i])
				{
					EventHandler::Instance().OnGetSceneActivityReward(m_role, ACTIVITY_TYPE_QUNXIANLUANDOU, title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
				}
			}
		}
	}

// 	if (activity_info.guildbattle_mengzhu_guild_id != INVALID_GUILD_ID)
// 	{
// 		if (activity_info.guildbattle_mengzhu_guild_id == m_role->GetGuildID())
// 		{
// 			bool gd_title_chg = false;
// 			int guildbattle_title_id = LOGIC_CONFIG->GetTitlePool().GetActivityTitle(ACTIVITY_TYPE_GUILDBATTLE);
// 			if (guildbattle_title_id > 0)
// 			{
// 				this->OnOneTitle(guildbattle_title_id, true, reason, &gd_title_chg);
// 			}
// 			if(gd_title_chg)
// 			{
// 				title_chged = true;
// 				this->MailOnTileStatusChange(ACTIVITY_TYPE_GUILDBATTLE,true,guildbattle_title_id);
// 			}
// 		}
// 	}

	if (title_chged && SYNC_REASON_ENTER_SCENE != reason)
	{
		this->SendAllTitle();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
	}
}

void TitleManager::OnChangeCharmRankTitle(int reason, bool is_init)
{
	bool title_chged = false;
	
	int title = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_MAN_CHARM);
	if (FEMALE == m_role->GetSex())
	{
		title = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_GIRL_CHARM);
	}

	if (0 == title) return;

	int uid = m_role->GetUID();
	int charm_top1_uid = WorldStatus::Instance().GetCommonData().charm_top1_title_uid;
	this->OnOneTitle(title, charm_top1_uid == uid, reason, &title_chged);
	
	if (title_chged && SYNC_REASON_ENTER_SCENE != reason)
	{
		if (!is_init)
		{
			this->SendAllTitle();
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
		}
	}
}

void TitleManager::OnChangeWorldQuestionRankTitle(int reason, bool is_init)
{
	bool title_chged = false;
	int title_id = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_WORLD_QUESTION);
	if (0 >= title_id) return;

	this->OnOneTitle(title_id, WorldStatus::Instance().GetCommonData().world_question_top1_title_uid == m_role->GetUID(), reason, &title_chged);

	if (title_chged && SYNC_REASON_ENTER_SCENE != reason)
	{
		if (!is_init)
		{
			this->SendAllTitle();
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
		}
	}
}

void TitleManager::OnOtherTitleInfo(const OtherTitleInfo &other_info, int reason)
{
	const int role_id = UidToInt(m_role->GetUserId());
	bool title_chged = false;

// 	int xianmengzhan_title_id = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(TITLE_OTHER_TYPE_CHALLENGE_FIELD_KING);
// 	this->OnOneTitle(xianmengzhan_title_id, role_id == other_info.challengefield_king_userid, reason, &title_chged);

	// 师门排行
	{
		bool is_gettitle = false;
	
		for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
		{
			if (MAX_CAMP_TOPRANK3 <= 0)
			{
				continue;
			}

			is_gettitle = (other_info.camp_toprank3_uid[i] == role_id);
			
			const int camp = i / MAX_CAMP_TOPRANK3 + 1;
			if (camp != m_role->GetCamp())
			{
				is_gettitle = false;
			}
			
			int titleid = LOGIC_CONFIG->GetCampCfg().GetTitleId(camp, i % MAX_CAMP_TOPRANK3 + 1);
			this->OnOneTitle(titleid, is_gettitle, reason, &title_chged);
		}
	}

	{
		// 跨服钓鱼Top
		bool is_add = false;
		int rank = 0;
		for (int i = 0; i < CROSS_FISHING_TOP_RANK_COUNT_MAX; ++i)
		{
			if (role_id == other_info.cross_fishing_top_rank_uid_list[i])
			{
				is_add = true;
				rank = i + 1;
				break;
			}
		}

		const Int32 title_id = CrossFishingConfig::Instance().GetOtherCfg().rank_title_id;
		bool title_chged = false;

		this->OnOneTitle(title_id, is_add, reason, &title_chged);
		
		if (title_id > 0 && title_chged)
		{
			if (!is_add)
			{
				this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_FISHING, is_add, title_id, rank);
			}

			if (is_add)
			{
				EventHandler::Instance().OnGetSceneActivityReward(m_role, CROSS_ACTIVITY_TYPE_FISHING, title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
			}
		}
	}

	{
		// 跨服牧场Top
		bool is_add = false;

		for (int i = 0; i < CROSS_PASTURE_TOP_RANK_COUNT_MAX; ++i)
		{
			if (role_id == other_info.cross_pasture_top_rank_uid_list[i])
			{
				is_add = true;
				break;
			}
		}

		const Int32 title_id = CrossPastureConfig::Instance().GetOtherCfg().rank_title_id;
		bool title_chged = false;

		this->OnOneTitle(title_id, is_add, reason, &title_chged);

	}

	{
		// 夫妻排行榜
		{
			bool is_add = false;
			int qingyuan_title_id = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_QINGYUAN_CAP_TOP);
			if (other_info.qingyuan_cap_top_female_uid == m_role->GetUID() || other_info.qingyuan_cap_top_male_uid == m_role->GetUID())
			{
				is_add = true;
			}

			this->OnOneTitle(qingyuan_title_id, is_add, reason, &title_chged);
		}

		{
			bool is_add = false;
			int babay_title_id = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_BABY_CAP_TOP);
			if (other_info.baby_cap_top_female_uid == m_role->GetUID() || other_info.baby_cap_top_male_uid == m_role->GetUID())
			{
				is_add = true;
			}

			this->OnOneTitle(babay_title_id, is_add, reason, &title_chged);
		}

		{
			bool is_add = false;
			int little_title_id = LOGIC_CONFIG->GetTitlePool().GetOtherTitle(OTHER_TITLE_TYPE_LITTLE_PET_TOP);
			if (other_info.littlepet_cap_top_male_uid == m_role->GetUID() || other_info.littlepet_cap_top_female_uid == m_role->GetUID())
			{
				is_add = true;
			}

			this->OnOneTitle(little_title_id, is_add, reason, &title_chged);

			/*if (reason != SYNC_REASON_ENTER_SCENE && title_chged && is_add)
			{
				mail_param.Reset();

				int length = SNPRINTF(mail_param.contenttxt, sizeof(mail_param.contenttxt), gamestring::g_llitte_pet_top_rank_get_title_content, little_title_id);
				if (length > 0)
				{
					MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, mail_param);
				}
			}*/
		}
	}

	if (title_chged && SYNC_REASON_ENTER_SCENE != reason)
	{
		this->SendAllTitle();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
	}
}

void TitleManager::OnRoomActivityTitleInfo(const RoomActivityInfo & info, int reason)
{
	const int role_id = UidToInt(m_role->GetUserId());

	bool mess_chg = false;
	// 乱斗战场Top
	{
		bool is_add = false;
		int rank = 0;
		for (int i = 0; i < CROSS_MESS_BATTLE_MAX_SCENE_NUM; ++i)
		{
			if (role_id == info.mess_battle_rank_first[i])
			{
				is_add = true;
				rank = 1;
				break;
			}
			if (role_id == info.mess_battle_rank_second[i])
			{
				is_add = true;
				rank = 2;
				break;
			}
			if (role_id == info.mess_battle_rank_third[i])
			{
				is_add = true;
				rank = 3;
				break;
			}
		}

		if (is_add)
		{
			const Int32 title_id = LOGIC_CONFIG->GetMessBattleFbConfig().GetTitleIdByRank(rank);
			this->OnOneTitle(title_id, is_add, reason, &mess_chg);
			if (title_id >0 && mess_chg)
			{
				//this->MailOnTileStatusChange(ACTIVITY_TYPE_MESS_BATTLE_FB, is_add, title_id, rank);

				EventHandler::Instance().OnGetSceneActivityReward(m_role, CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB, title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
			}
		}
		else
		{
			for (int i = 0; i < MESS_BATTLE_TOP_RANK_MAX; ++i)
			{
				const Int32 title_id = LOGIC_CONFIG->GetMessBattleFbConfig().GetTitleIdByRank(i+1);
				this->OnOneTitle(title_id, is_add, reason, &mess_chg);
				if (title_id > 0 && mess_chg)
				{
					this->MailOnTileStatusChange(ACTIVITY_TYPE_MESS_BATTLE_FB, is_add, title_id, i+1);
					break;
				}
			}
		}
	}

	bool night_chg = false;
	{  //夜战王城
		bool is_add = false;
		int rank = 0;
		for (int i = 0; i < CROSS_NIGHT_FIGHT_MAX_SCENE_NUM; ++i)
		{
			if (role_id == info.night_fight_rank_first[i])
			{
				is_add = true;
				rank = 1;
				break;
			}
			if (role_id == info.night_fight_rank_second[i])
			{
				is_add = true;
				rank = 2;
				break;
			}
			if (role_id == info.night_fight_rank_third[i])
			{
				is_add = true;
				rank = 3;
				break;
			}
		}

		if (is_add)
		{
			const Int32 title_id = LOGIC_CONFIG->GetNightFightFbConfig().GetTitleIdByRank(rank);
			this->OnOneTitle(title_id, is_add, reason, &night_chg);
			if (title_id> 0 && night_chg)
			{
				//this->MailOnTileStatusChange(ACTIVITY_TYPE_NIGHT_FIGHT_FB, is_add, title_id, rank);

				EventHandler::Instance().OnGetSceneActivityReward(m_role, CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB, title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
			}
		}
		else
		{
			for (int i = 0; i < NIGHT_FIGHT_RANK_TOP_MAX; ++i)
			{
				const Int32 title_id = LOGIC_CONFIG->GetNightFightFbConfig().GetTitleIdByRank(i+1);
				this->OnOneTitle(title_id, is_add, reason, &night_chg);
				if (title_id > 0 && night_chg)
				{
					this->MailOnTileStatusChange(ACTIVITY_TYPE_NIGHT_FIGHT_FB, is_add, title_id, i+1);
					break;
				}
			}
		}
	}

	if ((mess_chg || night_chg ) && SYNC_REASON_ENTER_SCENE != reason)
	{
		this->SendAllTitle();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
	}
}

void TitleManager::OnCrossActivityTitleInfo(const CrossActivityTitleInfo & info, int reason)
{
	bool is_add = false;
	int rank = 0;
	for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM; ++i)
	{
		if (m_role->GetUID() == info.cross_xiuluotower_rank_title[i])
		{
			is_add = true;
			rank = i + 1;
			break;
		}
	}
	bool title_chged = false;

	if (is_add)
	{
		const Int32 title_id = CrossXiuluoTowerCommonConfig::Instance().GetXiuluoTowerRankTitleId(rank);
		this->OnOneTitle(title_id, is_add, reason, &title_chged);
		if (title_chged)
		{
			//this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER, is_add, title_id, rank);

			EventHandler::Instance().OnGetSceneActivityReward(m_role, CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER, title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
		}
	}
	else
	{
		for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM; ++i)
		{
			const Int32 title_id = CrossXiuluoTowerCommonConfig::Instance().GetXiuluoTowerRankTitleId(i+1);
			this->OnOneTitle(title_id, is_add, reason, &title_chged);
			if (title_chged)
			{
				this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER, is_add, title_id, i+1);
				break;
			}
		}
	}

	if (title_chged && SYNC_REASON_ENTER_SCENE != reason)
	{
		this->SendAllTitle();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
	}

}

void TitleManager::RecalPataFbTitle()
{
	int pass_level = m_role->GetRolePataFB()->GetPassLevel();
	unsigned short new_title_id = LOGIC_CONFIG->GetTitlePool().GetPataFbTitle(pass_level);
	unsigned short old_title_id = 0;

	if (new_title_id > 0)
	{
		for (int i = 0; i < m_other_title_count && i < MAX_OTHER_TITLE_COUNT; ++ i)
		{
			const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(m_other_title_list[i].title_id);
			if (NULL == title_cfg)
			{
				continue;
			}

			if (TITLESOURCETYPE_PATAFB != title_cfg->source_type)
			{
				continue;
			}

			old_title_id = title_cfg->title_id;

			break;
		}

		if (old_title_id != new_title_id)
		{
			this->RemoveTitle(old_title_id, false, false, false);
			this->AddTitle(new_title_id, true, true, true, true);

			if (pass_level >= 10)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_title_get_pata_title_notice, m_role->GetUID(),
					m_role->GetName(), (int)m_role->GetCamp(), pass_level, (int)new_title_id);
				if (length > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}
}

void TitleManager::RecalJinglingCardTitle()
{
	int total_level = m_role->GetJingLingMgr()->GetCardTotalLevel();
	unsigned short new_title_id = LOGIC_CONFIG->GetTitlePool().GetJinglingCardTitle(total_level);
	unsigned short old_title_id = 0;

	if (new_title_id > 0)
	{
		for (int i = 0; i < m_other_title_count && i < MAX_OTHER_TITLE_COUNT; ++ i)
		{
			const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(m_other_title_list[i].title_id);
			if (NULL == title_cfg)
			{
				continue;
			}

			if (TITLESOURCETYPE_JINGLINGCARD != title_cfg->source_type)
			{
				continue;
			}

			old_title_id = title_cfg->title_id;

			break;
		}

		if (old_title_id != new_title_id)
		{
			this->RemoveTitle(old_title_id, false, false, false);
			this->AddTitle(new_title_id, true, true, true, true);

			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_jingling_card,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), total_level);
			if (sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}
}

void TitleManager::RecalJinglingTitle()
{
	if (m_role->GetJingLingMgr()->HasSameJingLing(JINGLING_BINGFENG_ID) && m_role->GetJingLingMgr()->HasSameJingLing(JINGLING_HUOFENG_ID)
		&& !this->HasOtherTitle(JINGLINGWANG_TITLE_ID))
	{
		this->AddTitle(JINGLINGWANG_TITLE_ID, true, true, true,true);
	}
}

void TitleManager::CheckAddCrossGuildKillRankTitle(int reason)
{
	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (nullptr == guild)
	{
		return;
	}

	std::set<unsigned short> old_rank_title_set;
	std::set<unsigned short> new_rank_title_set;

	int rank = 0;
	int tuanzhang_id = guild->GetTuanzhuangRoleId();
	bool is_tuanzhang = (tuanzhang_id == m_role->GetUID());
	int guild_list[CROSS_RANK_REWARD_COUNT] = { 0 };
	WorldStatus3::Instance().GetCrossGuildKillRankFirst(guild_list);

	for (int i = 0 ; i < CROSS_RANK_REWARD_COUNT; i ++)
	{
		if (guild_list[i] == m_role->GetGuildID())
		{
			rank = i + 1;
			break;
		}
	}

	if (rank <= 0)
	{
		return;
	}

	{
		// 
		for (int i = 0; i < m_other_title_count; ++i)
		{
			const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(m_other_title_list[i].title_id);
			if (NULL == title_cfg) continue;

			if (TITLESOURCETYPE_CROSS_GUILD_RANK == title_cfg->source_type)
			{
				old_rank_title_set.insert(m_other_title_list[i].title_id);
			}
		}

		unsigned short title_id = 0;
		const CrossGuildKillBossRankReward *reward_cfg = CrossRankRewardConfig::Instance().GetCrossGuildRankReward(CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS, rank);
		if (nullptr != reward_cfg)
		{
			title_id = is_tuanzhang ? reward_cfg->tuanzhang_title_id : reward_cfg->member_title_id;
		}

		const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_id);
		if (NULL == title_cfg) return;

		new_rank_title_set.insert(title_id);
	}

	bool title_chged = false;
	{
		// 删掉排行之后失去的称号
		std::set<unsigned short>::const_iterator old_iter;
		for (old_iter = old_rank_title_set.begin(); old_rank_title_set.end() != old_iter; ++old_iter)
		{
			unsigned short old_title_id = *old_iter;

			// 之前有，现在没有，干掉
			if (new_rank_title_set.end() == new_rank_title_set.find(old_title_id))
			{
				this->RemoveTitle(old_title_id, false, false, SYNC_REASON_ENTER_SCENE != reason);
				title_chged = true;
			}
		}
	}

	{
		// 添加新增排行榜称号
		std::set<unsigned short>::const_iterator new_iter;
		for (new_iter = new_rank_title_set.begin(); new_rank_title_set.end() != new_iter; ++new_iter)
		{
			unsigned short new_title_id = *new_iter;

			// 之前没有，现在有，增加
			if (old_rank_title_set.end() == old_rank_title_set.find(new_title_id))
			{
				this->AddTitle(new_title_id, false, false, SYNC_REASON_ENTER_SCENE != reason, SYNC_REASON_ENTER_SCENE != reason);
				title_chged = true;

				EventHandler::Instance().OnGetSceneActivityReward(m_role, Protocol::SCSceneActivityRewardInfo::REWARD_SOURCE_ID_CROSS_GUILD_KILL_BOSSP, new_title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
			}
		}
	}

	if (title_chged)
	{
		if (SYNC_REASON_ENTER_SCENE != reason)
		{
			this->SendAllTitle();
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
		}
	}
}

bool TitleManager::HasOtherTitle(unsigned short title_id)
{
	for (int i = 0; i < m_other_title_count && i < MAX_OTHER_TITLE_COUNT; ++i) 
	{
		if (m_other_title_list[i].title_id == title_id) return true;
	}

	return false;
}

void TitleManager::AttachAttr(CharIntAttrs &base_add, unsigned short title_id, char title_grade)
{
	const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_id);
	if (NULL == title_cfg) return;

	if (TITLESOURCETYPE_TITLECARD == title_cfg->source_type)
	{
		if (title_grade == 1)
		{
			base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += title_cfg->gongji;
			base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += title_cfg->fangyu;
			base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += title_cfg->maxhp;
			base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(title_cfg->gongji_add_per * ROLE_ATTR_PER);
			base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(title_cfg->fangyu_add_per * ROLE_ATTR_PER);
			base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(title_cfg->max_hp_add_per * ROLE_ATTR_PER);
		}
		else
		{
			const TitleUpgradeConfig *upgrade_cfg = LOGIC_CONFIG->GetTitlePool().GetUpgradeCfg(title_id, title_grade);
			if (NULL != upgrade_cfg)
			{
				base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += upgrade_cfg->gongji;
				base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += upgrade_cfg->fangyu;
				base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += upgrade_cfg->max_hp;
				base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(upgrade_cfg->gongji_add_per * ROLE_ATTR_PER);
				base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(upgrade_cfg->fangyu_add_per * ROLE_ATTR_PER);
				base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(upgrade_cfg->max_hp_add_per * ROLE_ATTR_PER);
			}
		}
	}
	else if (TITLESOURCETYPE_RED_EQUIP_COLLECT == title_cfg->source_type)    //套装收集，每多一颗星加成10%
	{
		int attr_factor = m_role->GetRedEquipCollect()->GetRedCollectStarAttrPer((int)title_id);

		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(title_cfg->gongji * (1 + attr_factor * 0.01));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(title_cfg->fangyu * (1 + attr_factor * 0.01));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(title_cfg->maxhp * (1 + attr_factor * 0.01));
		base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(title_cfg->gongji_add_per * ROLE_ATTR_PER * (1 + attr_factor * 0.01));
		base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(title_cfg->fangyu_add_per * ROLE_ATTR_PER * (1 + attr_factor * 0.01));
		base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(title_cfg->max_hp_add_per * ROLE_ATTR_PER * (1 + attr_factor * 0.01));
	}
	else
	{
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += title_cfg->gongji;
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += title_cfg->fangyu;
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += title_cfg->maxhp;
		base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(title_cfg->gongji_add_per * ROLE_ATTR_PER);
		base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(title_cfg->fangyu_add_per * ROLE_ATTR_PER);
		base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(title_cfg->max_hp_add_per * ROLE_ATTR_PER);
	}
}

void TitleManager::OnOneTitle(unsigned short title_id, bool is_add, int reason, bool *title_chged)
{
	const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_id);
	if (NULL == title_cfg)
	{
		return;
	}

	bool is_title_change = false;

	if (is_add)
	{
		if (!this->HasOtherTitle(title_id))
		{
			//策划需求，称号类型为11的在切场景也可自动使用，2019-1-4
			if (title_cfg->source_type == TITLESOURCETYPE_CHALLENGE_OUTSIDE_CFG)
			{
				is_title_change = this->AddTitle(title_id, SYNC_REASON_ENTER_SCENE != reason, SYNC_REASON_ENTER_SCENE != reason,
					SYNC_REASON_ENTER_SCENE != reason, true);
			}
			else
			{
				is_title_change = this->AddTitle(title_id, SYNC_REASON_ENTER_SCENE != reason, SYNC_REASON_ENTER_SCENE != reason,
					SYNC_REASON_ENTER_SCENE != reason, SYNC_REASON_ENTER_SCENE != reason);
			}
		}
	}
	else 
	{
		is_title_change = this->RemoveTitle(title_id, SYNC_REASON_ENTER_SCENE != reason, SYNC_REASON_ENTER_SCENE != reason, SYNC_REASON_ENTER_SCENE != reason);
	}

	if (NULL != title_chged) *title_chged = is_title_change;
}

void TitleManager::OnTitleOwnStatusChange(int action_type, unsigned short title_id)
{
	if (action_type <= TITLE_ACTION_TYPE_INVALID || action_type >= TITLE_ACTION_TYPE_MAX)
	{
		return;
	}

	const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_id);
	if (NULL == title_cfg)
	{
		return;
	}

	int length = 0;
	if (TITLE_ACTION_TYPE_GET == action_type)
	{
		length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_title_get_title, title_cfg->title_name);
	}
	else
	{
		length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_title_lost_title, title_cfg->title_name);
	}

	if (length > 0)
	{
		m_role->SendPersonalMsg(length, gamestring::GAMESTRING_BUF);
	}
}

void TitleManager::MailOnTileStatusChange(int activity_type, bool is_add, int title_id, int param1)
{
	int lenth = 0;
	MailContentParam contentparam; contentparam.Reset();
	if (is_add)
	{
		if (ACTIVITY_TYPE_MESS_BATTLE_FB == activity_type || ACTIVITY_TYPE_YIZHANDAODI == activity_type ||
			ACTIVITY_TYPE_QUNXIANLUANDOU == activity_type || ACTIVITY_TYPE_NIGHT_FIGHT_FB == activity_type ||
			CROSS_ACTIVITY_TYPE_FISHING == activity_type || CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER == activity_type)
		{
			lenth = gstr::SysMsgContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "fight_activity_title_notice",
				activity_type, param1, title_id);
		}
		else if (ACTIVITY_TYPE_GONGCHENGZHAN == activity_type || ACTIVITY_TYPE_GUILDBATTLE == activity_type)
		{
			lenth = gstr::SysMsgContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "guild_activity_title_notice",
				activity_type, title_id);
		}
		else if (CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE ==activity_type)
		{
			lenth = gstr::SysMsgContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_guild_battle_title_notice",
				param1, title_id);
		}
		else if(CROSS_ACTIVITY_TYPE_LIEKUN_FB ==activity_type)
		{
			lenth = gstr::SysMsgContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_liekunfb_top_reward");
		}
	}
	else
	{
		lenth = gstr::SysMsgContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "acitivity_title_reset_notice",
				activity_type, title_id);
	}
	if (lenth > 0) {
		MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
	}
}

void TitleManager::CheckAllTitleCondition(int type)
{
	unsigned short title_id = 0;
	switch(type)
	{
	case TITLE_TYPE_XUNBAO:
		{
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_XUNBAO, m_role->GetEquipXunbaoTimes());
		}
		break;

	case TITLE_TYPE_LIEQU:
		{
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_LIEQU, m_role->GetJLXunbaoTimes());
		}
		break;

	case TITLE_TYPE_SHIZHUANG:
		{
			int condition = m_role->GetShizhuang()->GetHasActiveShizhuang(SHIZHUANG_TYPE_BODY);
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_SHIZHUANG, condition);
		}
		break;

	case TITLE_TYPE_WEAPON:
		{
			int condition = m_role->GetShizhuang()->GetHasActiveShizhuang(SHIZHUANG_TYPE_WUQI);
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_WEAPON, condition);
		}
		break;

	case TITILE_TYPE_PHANTOM:
		{
			int condition = m_role->GetJingLingMgr()->GetJingLingPhantomCount();
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITILE_TYPE_PHANTOM, condition);
		}
		break;

	case TITLE_TYPE_HCHZ_KILLER:
		{
			int condition = m_role->GetHCHZKillCount();
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_HCHZ_KILLER, condition);
		}
		break;

	case TITLE_TYPE_GCZ_100_KILLER:
		{
			int condition = m_role->GetGCZKillCount();
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_GCZ_100_KILLER, condition);
		}
		break;

	case TITLE_TYPE_GCZ_1000_KILLER:
		{
			int condition = m_role->GetGCZKillCount();
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_GCZ_1000_KILLER, condition);
		}
		break;

	case TITLE_TYPE_GCZ_10000_KILLER:
		{
			int condition = m_role->GetGCZKillCount();
			title_id = LOGIC_CONFIG->GetTitlePool().GetConditionTitle(TITLE_TYPE_GCZ_10000_KILLER, condition);
		}
		break;
	}

	if (title_id > 0 && (!this->HasOtherTitle(title_id)))
	{
		this->AddTitle(title_id, true, true, true, true);
	}
}

bool TitleManager::OnActive(unsigned short title_id, bool syn_title_info, bool need_recal, bool notify_client, bool auto_use)
{
	const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(title_id);
	if (NULL == title_cfg || TITLESOURCETYPE_TITLECARD != title_cfg->source_type) return false;

	if (this->HasOtherTitle(title_id))
	{
		m_role->NoticeNum(errornum::EN_TITLE_HAS_ACTIVE);
		return false;
	}

	m_other_title_list[m_other_title_count].title_id = title_id;
	m_other_title_list[m_other_title_count].title_grade = 1;
	m_other_title_list[m_other_title_count].expired_time = 0;

	if (title_cfg->valid_secs > 0)
	{
		m_other_title_list[m_other_title_count].expired_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + title_cfg->valid_secs;
	}

	++m_other_title_count;

	if (auto_use)
	{
		// 以前能佩戴3个称号， 现在改成只能佩戴一个称号，如果要改回三个将m_use_title_count < 1去掉
		memset(m_use_title_list, 0, sizeof(m_use_title_list));
		m_use_title_count = 0;
		if (m_use_title_count < 1 && m_use_title_count < MAX_USE_TITLE_COUNT && !this->IsUsed(title_id))
		{
			m_use_title_list[m_use_title_count] = title_id;
			++ m_use_title_count;
			this->BroadCastUsedTitle();
		}
	}

	if (syn_title_info) this->SendAllTitle();
	if (notify_client) this->OnTitleOwnStatusChange(TITLE_ACTION_TYPE_GET, title_id);
	if (need_recal)m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);

	gamelog::g_log_title.printf(LL_INFO, "Title::OnActive user[%d, %s], level[%d], title_id[%d]", 
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), title_id);

	return true;
}

void TitleManager::OnUpGradeReq(int title_id)
{
	if (0 > title_id)
	{
		return;
	}

	int index = this->GetTitleIndex(title_id);
	if (0 > index || index >= MAX_OTHER_TITLE_COUNT)
	{
		return;
	}

	const int grade = m_other_title_list[index].title_grade;
	if (grade <= 0)
	{
		return;
	}

	if (grade >= MAX_TITLE_GRADE)
	{
		m_role->NoticeNum(errornum::EN_TITLE_GRADE_LIMIT);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

	const TitleUpgradeConfig *upgrade_cfg = LOGIC_CONFIG->GetTitlePool().GetUpgradeCfg(title_id, grade + 1);
	if (NULL == upgrade_cfg)
	{
		return;
	}

	stuff_list[0].item_id = upgrade_cfg->stuff_id;
	stuff_list[0].num = upgrade_cfg->stuff_num;
	stuff_list[0].buyable = false;

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return;
	}

	if (consume_list.count <= 0 && consume_list.need_gold_num <= 0)
	{
		return;
	}

	{
		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "TitleOnUpGradeComsume"))
		{
			return;
		}

		// 生效
		++ m_other_title_list[index].title_grade;
		m_role->NoticeNum(noticenum::NT_TITLE_UPGRADE_SUCC);
	}

	this->SendAllTitle();
	//升级不提示获取称号
	//this->OnTitleOwnStatusChange(TITLE_ACTION_TYPE_GET, title_id);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
}

int TitleManager::GetTitleIndex(int title_id)
{
	if (0 > title_id)
	{
		return -1;
	}

	for (int i = 0; i < m_other_title_count; i++)
	{
		if (title_id == m_other_title_list[i].title_id)
		{
			return i;
		}
	}

	return -2;
}

void TitleManager::OnChallengeFieldRankInfo(int rank_pos, int reason)
{
	return; // 改为结算时通过邮件发放 modify 2019-03-10

	unsigned short old_title_id = 0;
	for (int idx = 0; idx < m_other_title_count; ++idx)
	{
		const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(m_other_title_list[idx].title_id);
		if (NULL == title_cfg) continue;

		if (TITLESOURCETYPE_CHALLENGE_FIELD == title_cfg->source_type)
		{
			old_title_id = m_other_title_list[idx].title_id;
			this->RemoveTitle(old_title_id, false, false, SYNC_REASON_ENTER_SCENE != reason);
		}
	}

	unsigned short new_title_id = LOGIC_CONFIG->GetTitlePool().GetChallengeFieldRankTitle(rank_pos);
	const TitleCfg *title_cfg = LOGIC_CONFIG->GetTitlePool().GetTitleCfg(new_title_id);
	if (NULL != title_cfg)
	{
		this->AddTitle(new_title_id, false, false, SYNC_REASON_ENTER_SCENE != reason, SYNC_REASON_ENTER_SCENE != reason);
	}

	if (old_title_id != new_title_id)
	{
		if (SYNC_REASON_ENTER_SCENE != reason)
		{
			this->SendAllTitle();
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
		}
	}
}

void TitleManager::CheckShowChallengeFieldRank()
{
	int rank = ChallengeField::Instance().GetChallengeUserRankPos(m_role);
	bool is_open = ChallengeField::Instance().IsChallengeOpen();
	if (rank > 0 && !is_open)
	{
		int reward_opengame_day = ChallengeField::Instance().GetEndRankRewardOpenday();
		const ChallengeFieldRankRewardConfig::ConfigItem *reward_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetRankRewardCfgByRank(reward_opengame_day, rank - 1);
		if (NULL != reward_cfg)
		{
			if (reward_cfg->title_id > 0 && !this->HasOtherTitle(reward_cfg->title_id))
			{
				EventHandler::Instance().OnGetSceneActivityReward(m_role, Protocol::SCSceneActivityRewardInfo::REWARD_SOURCE_ID_CHALLENGEFIELD, reward_cfg->title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE, rank);
			}
		}
	}
}

void TitleManager::OnChangeLieKunTitle(int title_id, int reason)
{
	bool is_add = false;

	int ling_kun_win_guild_id = WorldStatus2::Instance().GetCommonData2().ling_kun_win_guild_id;
	if (ling_kun_win_guild_id != INVALID_GUILD_ID && m_role->GetGuildID() == ling_kun_win_guild_id)
	{
		is_add = true;
	}

	bool title_chged = false;

	if (is_add)
	{
		this->OnOneTitle(title_id, is_add, reason, &title_chged);
		if (title_chged)
		{
			//this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_LIEKUN_FB, is_add, title_id);

			EventHandler::Instance().OnGetSceneActivityReward(m_role, CROSS_ACTIVITY_TYPE_LIEKUN_FB, title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE);
		}
	}
	else
	{
		this->OnOneTitle(title_id, is_add, reason, &title_chged);
		if (title_chged)
		{
			this->MailOnTileStatusChange(CROSS_ACTIVITY_TYPE_LIEKUN_FB, is_add, title_id);
		}
	}

	if (title_chged && SYNC_REASON_ENTER_SCENE != reason)
	{
		this->SendAllTitle();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);
	}
}
