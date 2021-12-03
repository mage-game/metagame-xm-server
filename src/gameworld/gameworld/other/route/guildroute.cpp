#include "gameworld/engineadapter.h"
#include "servercommon/basedef.h"
#include "internalcomm.h"
#include "obj/character/role.h"

#include "gameworld/protocal/msgguild.hpp"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"

#include "gameworld/item/knapsack.h"
#include "gameworld/item/money.h"

#include "servercommon/campdef.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "guildroute.hpp"
#include "gameworld/gamelog.h"

#include "gameworld/item/itempool.h"

#include "world.h"
#include "gstr.h"

#include "globalconfig/globalconfig.h"

#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/namefilter.hpp"

#include "other/sublock/sublock.hpp"

#include "scene/guildkeyregister/guildkeyregister.hpp"
#include "scene/speciallogic/guildspecial/specialguildstation.hpp"
#include "scene/speciallogic/guildspecial/specialguildfb.hpp"
#include "scene/scene.h"
#include "scene/guildshadow/guildshadow.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/guildfbconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "protocal/msgitem.h"

#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "config/guildconfig.hpp"
#include "config/activityconfig/guildbonfireconfig.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/guild/roleguild.hpp"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "servercommon/serverconfig/localconfig.hpp"

GuildRoute & GuildRoute::Instance()
{
	static GuildRoute guild_route;
	return guild_route;
}

void GuildRoute::CreateGuildCS(Role *role, Protocol::CSCreateGuild *cg)
{
	if (NULL == role)
	{
		return;
	}

	if (0 == cg->guild_name[0] || cg->create_guild_type <= GUILD_CREATE_TYPE_INVALID || 
		cg->create_guild_type >= GUILD_CREATE_TYPE_MAX) 
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now < role->GetLastCreateGuildTime() + 300)
	{//原1秒的容错延长至5分钟
		role->NoticeNum(errornum::EN_GUILD_CREATE_CD_TIME);
		return;
	}


	if (INVALID_GUILD_ID != role->GetGuildID())	
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
		return;
	}

	if (role->GetLevel() < LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().join_guild_level_limit)
	{
		role->NoticeNum(errornum::EN_GUILD_NOT_ENOUGH_LEVEL);
		return;
	}

	if (role->GetSublock()->CheckHasLock()) return;

	bool NAMEFILTER_OPEN_FLAG = ServerConfigPool::Instance().common_config.namefilter_open_flag;
	if (NAMEFILTER_OPEN_FLAG && NameFilter::Instance().HasSensitiveWord(sizeof(cg->guild_name), cg->guild_name)) return;

	if (HasInvalidChar(cg->guild_name))
	{
		role->NoticeNum(errornum::EN_RENAME_NAME_INVALID);
		return;
	}

	static gglobalprotocal::CreateGuild gg_cg;
	gg_cg.db_index = role->GetUserId().db_index;
	gg_cg.role_id = role->GetUserId().role_id;
	F_STRNCPY(gg_cg.guild_name, cg->guild_name, sizeof(gg_cg.guild_name));
	F_STRNCPY(gg_cg.guild_notice, cg->guild_notice, sizeof(gg_cg.guild_notice));
	role->GetName(gg_cg.role_name);
	role->GetPlatName(gg_cg.plat_name);
	gg_cg.level = role->GetLevel();
	gg_cg.sex = role->GetSex();
	gg_cg.prof = role->GetProf();
	gg_cg.camp = role->GetCamp();
	gg_cg.vip_level = role->GetVip()->GetVipLevel();
	gg_cg.capability = role->GetCapability()->GetTotalCapability();
	gg_cg.create_guild_type = cg->create_guild_type;

	if (GUILD_CREATE_TYPE_GOLD == cg->create_guild_type)
	{
		WorldStatusCommonDataParam &world_status = WorldStatus::Instance().GetCommonData();
		if (world_status.free_create_guild_times >= LOGIC_CONFIG->GetGuildConfig().GetGuildFreeCreateGuildTimes())
		{
			PlatName local_plat_name;
			memset(local_plat_name, 0, sizeof(local_plat_name));
			LocalConfig::Instance().GetPlatName(local_plat_name);
			std::string guild_default_plat_name = "default";
			const GuildPlatNameCfg * check_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildPlatNameCfg(local_plat_name);
			const GuildPlatNameCfg * guild_plat_consume_cfg = (nullptr == check_cfg) ? LOGIC_CONFIG->GetGuildConfig().GetGuildPlatNameCfg(guild_default_plat_name) : check_cfg;
			if (nullptr == guild_plat_consume_cfg)
			{
				return;
			}
			// 根据平台类型消耗元宝
			if (guild_plat_consume_cfg->gold > 0 && !role->GetKnapsack()->GetMoney()->UseGold(guild_plat_consume_cfg->gold, "CreateGuild", false))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
			//// 铜币改为元宝
			//long long GUILD_CREATE_COIN_BIND = LOGIC_CONFIG->GetGuildConfig().GetGuildCreateCoin();
			//if (GUILD_CREATE_COIN_BIND > 0 && !role->GetKnapsack()->GetMoney()->UseGold(GUILD_CREATE_COIN_BIND, "CreateGuild",false))
			//{
			//	role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			//	return;
			//}
		}
		else
		{
			world_status.free_create_guild_times++;
		}

		gg_cg.item_wrapper.Reset();
	}
	else
	{
		const ItemID GUILD_CREATE_ITEM_ID = static_cast<ItemID>(LOGIC_CONFIG->GetGuildConfig().GetGuildCreateItemId());
		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
		ItemExtern::ItemConsumeConfig stuff_item;

		stuff_item.item_id = GUILD_CREATE_ITEM_ID;
		stuff_item.num = 1;
		stuff_item.buyable = true;

		if (!ItemExtern::GetConsumeListByOrder(role, true, 1, &stuff_item, &consume_list))
		{
			role->NoticeNum(errornum::EN_GUILD_CREATE_NO_ITEM);
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "GuildRoute::CreateGuildCS"))
		{
			return;
		}
	}
	
	GuildManager::Instance().CreateGuild(role, &gg_cg);

	gamelog::g_log_guild_route.printf(LL_INFO, "guild_route -> create guild[ ret:succ, guild_name:%s, "
		"role[ uid:%d, name:%s, level:%d, prof:%d, camp:%d ] ]"
		"create_guild_type:%d ]",
		(const char*)gg_cg.guild_name, 
		UidToInt(UserID((int)gg_cg.db_index, (int)gg_cg.role_id)), (const char*)gg_cg.role_name, (int)gg_cg.level, 
		(int)gg_cg.prof, (int)gg_cg.camp, 
		(int)gg_cg.create_guild_type);
}

void GuildRoute::DismissGuildCS(Role *role, Protocol::CSDismissGuild *dg)
{
	if (INVALID_GUILD_ID == role->GetGuildID() || dg->guild_id != role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (GUILD_POST_TUANGZHANG != role->GetGuildPost()) // 只有团长才有权限解散军团
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return;
	}

	if (role->GetSublock()->CheckHasLock()) return;

	gglobalprotocal::DismissGuild gg_dg;
	gg_dg.guild_id = dg->guild_id;

	GuildManager::Instance().DismissGuild(role, &gg_dg);

	gamelog::g_log_guild_route.printf(LL_INFO, "guild_route -> dismiss guild[ ret: succ, guild_id:%d, role[ uid:%d,name:%s ] ]",
		gg_dg.guild_id, role->GetUID(), (const char*)role->GetName());
}

void GuildRoute::ApplyForJoinGuildCS(Role *role, Protocol::CSApplyForJoinGuild *afjg)
{
	if (INVALID_GUILD_ID != role->GetGuildID()) 
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
		return;
	}
// 	if (CAMP_TYPE_INVALID == role->GetCamp())	
// 	{
// 		role->NoticeNum(errornum::EN_GUILD_NOT_JOIN_CAMP);
// 		return;
// 	}
	if (role->GetLevel() < LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().join_guild_level_limit)	
	{
		role->NoticeNum(errornum::EN_GUILD_NOT_ENOUGH_LEVEL);
		return;
	}

	if (afjg->is_auto_join != 0)
	{
		unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (now_sec - common_data->auto_join_guild_last_time <= LOGIC_CONFIG->GetGuildConfig().GetGuildAutoJoinCD())
			{
				role->NoticeNum(errornum::EN_GUILD_ATUO_JOIN_TIME_LIMIT);
				return;
			}
			common_data->auto_join_guild_last_time = now_sec;
		}	
	}

	gglobalprotocal::ApplyForJoinGuild gg_afjg;
	gg_afjg.guild_id = afjg->guild_id;
	gg_afjg.db_index = role->GetUserId().db_index;
	gg_afjg.role_id = role->GetUserId().role_id;
	role->GetName(gg_afjg.role_name);
	role->GetPlatName(gg_afjg.plat_name);
	gg_afjg.level = role->GetLevel();
	gg_afjg.sex = role->GetSex();
	gg_afjg.prof = role->GetProf();
	gg_afjg.camp = role->GetCamp();
	gg_afjg.is_auto_join = 0 != afjg->is_auto_join ? 1 : 0; 
	gg_afjg.vip_level = role->GetVip()->GetVipLevel();
	gg_afjg.capability = role->GetCapability()->GetTotalCapability();

	GuildManager::Instance().ApplyForJoin(role, &gg_afjg);
}

void GuildRoute::ApplyForJoinGuildAckCS(Role *role, Protocol::CSApplyForJoinGuildAck *afjga)
{
	if (afjga->count <= 0 || afjga->count > MAX_APPLY_FOR_COUNT) return;

	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != afjga->guild_id) 
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::ApplyForJoinGuildAck gg_ajaf;
	gg_ajaf.guild_id = afjga->guild_id;
	gg_ajaf.result = afjga->result;
	gg_ajaf.db_index = role->GetUserId().db_index;
	gg_ajaf.role_id = role->GetUserId().role_id;

	afjga->count = (afjga->count > MAX_APPLY_FOR_COUNT) ? MAX_APPLY_FOR_COUNT : afjga->count;
	for (int i = 0; i < afjga->count; ++i)
	{
		UserID apply_for_user = IntToUid(afjga->applyfor_uid_list[i]);

		gg_ajaf.apply_for[i].apply_for_db_index = apply_for_user.db_index;
		gg_ajaf.apply_for[i].apply_for_role_id = apply_for_user.role_id;
	}
	gg_ajaf.count = afjga->count;

	GuildManager::Instance().ApplyForJoinAck(role, &gg_ajaf);
}

void GuildRoute::QuitGuildCS(Role *role, Protocol::CSQuitGuild *qg)
{
	if (role->GetSublock()->CheckHasLock()) return;

	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != qg->guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::QuitGuild gg_qg;
	gg_qg.guild_id = qg->guild_id;
	gg_qg.db_index = role->GetUserId().db_index;
	gg_qg.role_id = role->GetUserId().role_id;

	GuildManager::Instance().QuitGuild(role, &gg_qg);
}

void GuildRoute::InviteGuildCS(Role *role, Protocol::CSInviteGuild *ig)
{
	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != ig->guild_id) 
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::InviteGuild gg_ig;
	gg_ig.guild_id = ig->guild_id;
	gg_ig.db_index = role->GetUserId().db_index;
	gg_ig.role_id = role->GetUserId().role_id;

	UserID beinvite_user = IntToUid(ig->beinvite_uid);
	gg_ig.beinvite_db_index = beinvite_user.db_index;
	gg_ig.beinvite_role_id = beinvite_user.role_id;

	GuildManager::Instance().Invite(role, &gg_ig);
}

void GuildRoute::InviteGuildAckCS(Role *role, Protocol::CSInviteGuildAck *iga)
{
	if (INVALID_GUILD_ID != role->GetGuildID()) 
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
		return;
	}
	if (role->GetLevel() < LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().join_guild_level_limit)	
	{
		role->NoticeNum(errornum::EN_GUILD_NOT_ENOUGH_LEVEL);
		return;
	}

	gglobalprotocal::InviteGuildAck gg_ai;
	gg_ai.guild_id = iga->guild_id;
	gg_ai.db_index = role->GetUserId().db_index;
	gg_ai.role_id = role->GetUserId().role_id;
	role->GetName(gg_ai.role_name);
	role->GetPlatName(gg_ai.plat_name);
	gg_ai.level = role->GetLevel();
	gg_ai.sex = role->GetSex();
	gg_ai.prof = role->GetProf();
	gg_ai.camp = role->GetCamp();
	gg_ai.capability = role->GetCapability()->GetTotalCapability();

	UserID invite_user = IntToUid(iga->invite_uid);
	gg_ai.invite_db_index = invite_user.db_index;
	gg_ai.invite_role_id = invite_user.role_id;
	gg_ai.result = iga->result;

	GuildManager::Instance().InviteAck(role, &gg_ai);
}

void GuildRoute::KickoutGuildCS(Role *role, Protocol::CSKickoutGuild *kg)
{
	if (role->GetSublock()->CheckHasLock()) return;

	if (kg->bekicker_count <= 0 || kg->bekicker_count > MAX_MEMBER_COUNT) return;

	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != kg->guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::KickoutGuild gg_kg;
	gg_kg.guild_id = kg->guild_id;
	gg_kg.kicker_db_index = role->GetUserId().db_index;
	gg_kg.kicker_role_id = role->GetUserId().role_id;

	gg_kg.bekicker_count = 0;
	for (int i = 0; i < kg->bekicker_count; ++i)
	{
		UserID bekick_user = IntToUid(kg->bekicker_uid_list[i]);
		gg_kg.bekicker_role_info_list[i].bekicker_db_index = bekick_user.db_index;
		gg_kg.bekicker_role_info_list[i].bekicker_role_id = bekick_user.role_id;

		++gg_kg.bekicker_count;
	}

	GuildManager::Instance().KickoutGuild(role, &gg_kg);
}

void GuildRoute::AppointGuildCS(Role *role, Protocol::CSAppointGuild *ag)
{
	if (ag->post <= GUILD_POST_INVALID || ag->post >= GUILD_POST_MAX) return;

	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != ag->guild_id) 
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::AppointGuild gg_ag;
	gg_ag.guild_id = ag->guild_id;
	gg_ag.db_index = role->GetUserId().db_index;
	gg_ag.role_id = role->GetUserId().role_id;

	UserID beappoint_user = IntToUid(ag->beappoint_uid);
	gg_ag.beappoint_db_index = beappoint_user.db_index;
	gg_ag.beappoint_role_id = beappoint_user.role_id;
	gg_ag.post = ag->post;

	GuildManager::Instance().Appoint(role, &gg_ag);
}

void GuildRoute::LeaveGuildPostCS(Role *role, Protocol::CSLeaveGuildPost *ag)
{
	if (role->GetSublock()->CheckHasLock()) return;

	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != ag->guild_id) 
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::LeavePostGuild gg_lpg;
	gg_lpg.guild_id = ag->guild_id;
	gg_lpg.db_index = role->GetUserId().db_index;
	gg_lpg.role_id = role->GetUserId().role_id;

	GuildManager::Instance().LeavePost(role, &gg_lpg);
}

void GuildRoute::GetGuildInfoCS(Role *role, Protocol::CSGetGuildInfo *agi)
{
	if (agi->guild_info_type <= GUILD_INFO_TYPE_INVALID || agi->guild_info_type >= GUILD_INFO_TYPE_MAX) return;

	switch (agi->guild_info_type)
	{
	case GUILD_INFO_TYPE_ALL_GUILD_BASE_INFO:
		this->OnGetAllGuildBaseInfo(role);
		break;

	case GUILD_INFO_TYPE_GUILD_APPLY_FOR_INFO:
		this->OnGetApplyForList(role, agi->guild_id);
		break;

	case GUILD_INFO_TYPE_GUILD_MEMBER_LIST:
		this->OnGetMemberList(role, agi->guild_id);
		break;

	case GUILD_INFO_TYPE_GUILD_INFO:
		this->OnGetGuildInfo(role, agi->guild_id);
		break;

	case GUILD_INFO_TYPE_GUILD_EVENT_LIST:
		this->OnGetGuildEventList(role, agi->guild_id);
		break;

	case GUILD_INFO_TYPE_APPLY_FOR_JOIN_GUILD_LIST:
		this->OnGetApplyForJoinGuildList(role);
		break;

	case GUILD_INFO_TYPE_INVITE_LIST:
		this->OnGetGuildInviteList(role, agi->guild_id);
		break;

	case GUILD_INFO_TYPE_GONGZI_LIST:
		this->OnGetGuildGongziList(role);
		break;
	}
}

void GuildRoute::GuildChangeNoticeCS(Role *role, Protocol::CSGuildChangeNotice *gsn)
{
	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != gsn->guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::GuildChangeNotice gg_gcn;
	gg_gcn.guild_id = role->GetGuildID();
	gg_gcn.db_index = role->GetUserId().db_index;
	gg_gcn.role_id = role->GetUserId().role_id;
	F_STRNCPY(gg_gcn.notice, gsn->notice, sizeof(gg_gcn.notice));

	GuildManager::Instance().ChangeNotice(role, &gg_gcn);
}

void GuildRoute::GuildMailAllCS(Role *role, Protocol::CSGuildMailAll *gma)
{
	if (gma->contenttxt_len < 0 || gma->contenttxt_len > (int)sizeof(MailContentTxt)) return;

	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != gma->guild_id) 
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::GuildMailAll gg_gma;
	gg_gma.guild_id = gma->guild_id;
	gg_gma.db_index = role->GetUserId().db_index;
	gg_gma.role_id = role->GetUserId().role_id;
	F_STRNCPY(gg_gma.subject, gma->subject, sizeof(gg_gma.subject));
	gg_gma.contenttxt_len = gma->contenttxt_len > static_cast<int>(sizeof(gg_gma.contenttxt) - 1) ? sizeof(gg_gma.contenttxt) - 1: gma->contenttxt_len;
	memcpy(gg_gma.contenttxt, gma->contenttxt, gg_gma.contenttxt_len);
	gg_gma.contenttxt[gg_gma.contenttxt_len] = 0;

	GuildManager::Instance().MailAll(role, &gg_gma);
}

void GuildRoute::OnGetAllGuildBaseInfo(Role *role)
{
	gglobalprotocal::AllGuildBaseInfo gg_agbi;
	gg_agbi.db_index = role->GetUserId().db_index;
	gg_agbi.role_id = role->GetUserId().role_id;

	GuildManager::Instance().GetAllGuildBaseInfo(role, &gg_agbi);
}

void GuildRoute::OnGetApplyForList(Role *role, GuildID guild_id)
{
	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::GetApplyForList gg_gafl;
	gg_gafl.guild_id = guild_id;
	gg_gafl.db_index = role->GetUserId().db_index;
	gg_gafl.role_id = role->GetUserId().role_id;

	// 申请加入我盟的列表
	GuildManager::Instance().GetApplyForList(role, &gg_gafl);
}

void GuildRoute::OnGetApplyForJoinGuildList(Role *role)
{
	if (INVALID_GUILD_ID != role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
		return;
	}
	
	// 我申请加入的盟列表
	GuildManager::Instance().GetApplyForJoinGuildList(role);
}

void GuildRoute::OnGetMemberList(Role *role, GuildID guild_id)
{
	if (INVALID_GUILD_ID == guild_id) return;

	gglobalprotocal::GetMemberList gg_gml;
	gg_gml.guild_id = guild_id;

	GuildManager::Instance().GetMemberList(role, &gg_gml);
}

void GuildRoute::OnGetGuildInfo(Role *role, GuildID guild_id)
{
	if (INVALID_GUILD_ID == guild_id) return;

	gglobalprotocal::GetGuildBaselInfo gg_gi;
	gg_gi.guild_id = guild_id;
	gg_gi.db_index = role->GetUserId().db_index;
	gg_gi.role_id = role->GetUserId().role_id;

	GuildManager::Instance().GetGuildBaseInfo(role, &gg_gi);
}

void GuildRoute::OnGetGuildEventList(Role *role, GuildID guild_id)
{	
	if (INVALID_GUILD_ID == guild_id || role->GetGuildID() != guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::GetGuildEventList gg_ggel;
	gg_ggel.guild_id = guild_id;
	gg_ggel.db_index = role->GetUserId().db_index;
	gg_ggel.role_id = role->GetUserId().role_id;

	GuildManager::Instance().GetGuildEventList(role, &gg_ggel);
}

void GuildRoute::OnGetGuildInviteList(Role *role, GuildID guild_id)
{
	if (INVALID_GUILD_ID != role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
		return;
	}

	gglobalprotocal::GetGuildInviteList gg_ggil;
	gg_ggil.guild_id = guild_id;
	gg_ggil.db_index = role->GetUserId().db_index;
	gg_ggil.role_id = role->GetUserId().role_id;

	GuildManager::Instance().GetGuildInviteList(role, &gg_ggil);
}

void GuildRoute::OnGetGuildGongziList(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}
	
	guild->SendMemberGongziList(role);
}

void GuildRoute::GuildDelateCS(Role *role, Protocol::CSGuildDelate *gd)
{
	if (role->GetSublock()->CheckHasLock()) return;

	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != gd->guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::GuildDelate gg_gd;
	gg_gd.guild_id = gd->guild_id;
	gg_gd.knapsack_index = gd->knapsack_index;

	GuildManager::Instance().Delate(role, &gg_gd);
}

void GuildRoute::OnCreateGuildFailRollback(Role *role, ggprotocal::CreateGuildFailRollback *cgfrb)
{	
	if (nullptr == role || nullptr == cgfrb)
	{
		return;
	}
	if (GUILD_CREATE_TYPE_GOLD == cgfrb->create_guild_type)
	{
		//long long GUILD_CREATE_COIN_BIND = LOGIC_CONFIG->GetGuildConfig().GetGuildCreateCoin();
		PlatName local_plat_name;
		memset(local_plat_name, 0, sizeof(local_plat_name));
		LocalConfig::Instance().GetPlatName(local_plat_name);
		std::string guild_default_plat_name = "default";
		const GuildPlatNameCfg * check_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildPlatNameCfg(local_plat_name);
		const GuildPlatNameCfg * guild_plat_consume_cfg = (nullptr == check_cfg) ? LOGIC_CONFIG->GetGuildConfig().GetGuildPlatNameCfg(guild_default_plat_name) : check_cfg;
		if (nullptr == guild_plat_consume_cfg)
		{
			return;
		}
		if (!role->GetKnapsack()->GetMoney()->AddGold(guild_plat_consume_cfg->gold, "CreateGuildFailRollback"))
		{
			gamelog::g_log_guild_route.printf(LL_ERROR, "guild_route -> OnCreateGuildFailRollback, rollback coin_bind fail"
				" [role[%d %s] coin_bind:%lld ]", UidToInt(role->GetUserId()), role->GetName(), guild_plat_consume_cfg->gold);
		}
	}
	else
	{
		ItemID GUILD_CREATE_ITEM_ID = static_cast<ItemID>(LOGIC_CONFIG->GetGuildConfig().GetGuildCreateItemId());
		if (GUILD_CREATE_ITEM_ID != cgfrb->item_wrapper.item_id || cgfrb->item_wrapper.num != 1) return;

		if (!role->GetKnapsack()->Put(cgfrb->item_wrapper, PUT_REASON_NO_NOTICE))
		{
			gamelog::g_log_guild_route.printf(LL_ERROR, "guild_route -> OnCreateGuildFailRollback, rollback item fail [role[%d %s] item_id:%d is_bind:%d]", 
				UidToInt(role->GetUserId()), role->GetName(), (int)cgfrb->item_wrapper.item_id, cgfrb->item_wrapper.is_bind);
		}
	}
}

void GuildRoute::GuildBackToStationCS(Role *role, Protocol::CSGuildBackToStation *gbts, bool notify)
{
	if (INVALID_GUILD_ID == gbts->guild_id || role->GetGuildID() != gbts->guild_id)
	{
		if (notify) role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (Scene::SCENE_TYPE_GUILD_STATION == role->GetScene()->GetSceneType())
	{
		if (notify) role->NoticeNum(errornum::EN_GUILD_ALREADY_IN_GUILD_STATION);
		return;
	}

	if (!role->GetRoleStatusManager()->CanGuildBackStation(notify)) return;

	int GUILD_STATION_SCENE_ID = LOGIC_CONFIG->GetGuildConfig().GetGuildStationSceneId();
	Posi pos = LOGIC_CONFIG->GetGuildConfig().GetGuildBackStationPos();

	World::GetInstWorld()->GetSceneManager()->GoTo(role, GUILD_STATION_SCENE_ID, role->GetGuildID(), pos);
}

void GuildRoute::GuildResetName(Role *role, Protocol::CSGuildResetName *grn)
{
	if (INVALID_GUILD_ID == grn->guild_id || role->GetGuildID() != grn->guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (GUILD_POST_TUANGZHANG != role->GetGuildPost())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return;
	}

	if (!GuildManager::Instance().CanGuildResetName(role))
	{
		return;
	}

	if (0 == grn->new_name[0]) return;

	grn->new_name[sizeof(grn->new_name) - 1] = 0;

	bool NAMEFILTER_OPEN_FLAG = ServerConfigPool::Instance().common_config.namefilter_open_flag;
	if (NAMEFILTER_OPEN_FLAG && NameFilter::Instance().HasSensitiveWord(sizeof(grn->new_name), grn->new_name)) return;

	GuildName guild_name = {0};
	role->GetGuildName(guild_name);

	{
		static gglobalprotocal::GuildResetName gg_grn;
		gg_grn.guild_id = grn->guild_id;
		gg_grn.role_id = role->GetUserId().role_id;
		gg_grn.db_index = role->GetUserId().db_index;
		F_STRNCPY(gg_grn.new_name, grn->new_name, sizeof(gg_grn.new_name));

		if (GuildManager::Instance().OnGuildResetName(role, &gg_grn))
		{
			gamelog::g_log_guild_route.printf(LL_INFO, "guild_route -> reset name[ ret: succ, guild_id:%d, role[ uid:%d,name:%s ] [ old_name:%s, new_name:%s ] ]",
				grn->guild_id, UidToInt(role->GetUserId()), (const char*)role->GetName(), guild_name, gg_grn.new_name);
		}
	}
}

void GuildRoute::GuildApplySetup(Role *role, Protocol::CSApplyforSetup *afs)
{
	if (INVALID_GUILD_ID == afs->guild_id || role->GetGuildID() != afs->guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::GuildApplyforSetup gg_afs;
	gg_afs.guild_id = afs->guild_id;
	gg_afs.applyfor_setup = afs->applyfor_setup;
	gg_afs.need_capability = afs->need_capability;
	gg_afs.need_level = afs->need_level;

	GuildManager::Instance().OnGuildApplyforSetup(role, &gg_afs);
}

void GuildRoute::GuildAddGuildExp(Role *role, Protocol::CSAddGuildExp *age)
{
	if (NULL == role || NULL == age) return;

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	int add_guild_exp = 0, add_gongxian = 0, use_type = 0, juanxian_value = 0;

	if (Protocol::ADD_GUILD_EXP_TYPE_GOLD == age->type && age->value > 0 && age->times > 0)
	{
		GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
		int day_gold_max = other_cfg.day_juanxian_gold_limit;
		int today_juanxian_gold = role->GetRoleGuild()->GetTodayJuanxianGold();
		
		if (today_juanxian_gold + age->value > day_gold_max)
		{
			gstr::SendError(role, "guild_juanxian_limit");
			return;
		}

		use_type = GUILD_EVENT_ADD_EXP_TYPE_GOLD;
		juanxian_value = age->value;

 		int jx_money = age->value * age->times;
// 		add_guild_exp = jx_money;
		add_gongxian = jx_money * other_cfg.juanxian_gold_add_exp;

		if (add_gongxian <= 0)
		{
			return;
		}

		if (!role->GetKnapsack()->GetMoney()->UseGold(jx_money, "GuildAddGuildExp"))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
		
		role->AddGuildGongxian(add_gongxian, "GuildAddGuildExpByGold");
		role->GetRoleGuild()->AddTodayJuanxianGold(age->value);
	}
	else if (Protocol::ADD_GUILD_EXP_TYPE_ITEM == age->type)
	{	
		use_type = GUILD_EVENT_ADD_EXP_TYPE_ITEM;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		int item_num = 0;
	
		for (int i = 0; i < GUILD_MAX_UPLEVEL_STUFF_COUNT; ++ i)
		{
			Protocol::JxItem *jx_item = &age->item_list[i];
			
			if (jx_item->item_id <= 0 || jx_item->item_num == 0) continue;

			const GuildJuanXianConfig::ConfigItem *config_item = LOGIC_CONFIG->GetGuildConfig().GetJuanXianConfig(jx_item->item_id);
			if (NULL == config_item)
			{
				continue;
			}

			if (item_num < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
			{
				stuff_list[item_num].item_id = jx_item->item_id;
				stuff_list[item_num].buyable = false;
				stuff_list[item_num].num = jx_item->item_num;

				add_guild_exp += config_item->add_guild_exp * jx_item->item_num;
				add_gongxian += config_item->add_gongxian * jx_item->item_num;
				juanxian_value += jx_item->item_num; 

				++item_num;
			}
		}

		if (item_num <= 0)
		{
			return;
		}

		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
		if (!ItemExtern::GetConsumeListByOrder(role, false, item_num, stuff_list, &consume_list)) // 优先消耗非绑定材料 
		{
			return;
		}

		if (!ItemExtern::ConsumeItem(role, consume_list, "GuildRoute::GuildAddGuildExp"))
		{
			return;
		}

		role->AddGuildGongxian(add_gongxian, "GuildAddGuildExpByItem");
	}

	if (add_guild_exp > 0)
	{
		gglobalprotocal::GuildAddExp gae;
		gae.guild_id = role->GetGuildID();
		gae.db_index = role->GetUserId().db_index;
		gae.role_id = role->GetUserId().role_id;
		gae.use_type = use_type;
		gae.value = juanxian_value;
		gae.add_exp = add_guild_exp;
		GuildManager::Instance().OnGuildAddGuildExp(role, &gae);

		Protocol::SCAddGuildExpSucc ages;
		ages.add_gongxian = add_gongxian;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ages, sizeof(ages));
	}

	EventHandler::Instance().OnGuildDonate(role, age->type, juanxian_value);
}

void GuildRoute::GuildUpLevel(Role *role, Protocol::CSGuildUpLevel *msg)
{
}

void GuildRoute::GuildGetBuff(Role *role, Protocol::CSGuildGetBuff *msg)
{
}

void GuildRoute::GuildExchange(Role *role, Protocol::CSGuildExchange *cmd)
{
	role->GetRoleGuild()->GuildExchange(cmd->item_id, cmd->num);
}

void GuildRoute::GuildSetAutoKickoutSetup(Role *role, Protocol::CSGuildSetAutoKickoutSetup *gsaks)
{
	if (gsaks->auto_kickout_setup < 0) return;

	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != gsaks->guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	gglobalprotocal::GuildSetAutoKickoutSetup ggsaks;
	ggsaks.guild_id = role->GetGuildID();
	ggsaks.auto_kickout_setup = gsaks->auto_kickout_setup;

	GuildManager::Instance().SetAutoKickoutSetup(role, &ggsaks);
}

void GuildRoute::GuildCallIn(Role *role, Protocol::CSGuildCallIn *gci)
{
	if (INVALID_GUILD_ID == role->GetGuildID() || role->GetGuildID() != gci->guild_id)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (GUILD_POST_FU_TUANGZHANG != role->GetGuildPost() && GUILD_POST_TUANGZHANG != role->GetGuildPost())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return;
	}

	GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();

	if (role->GetLevel() < other_cfg.guild_call_in_level_limit)
	{
		role->NoticeNum(errornum::EN_GUILD_CALL_IN_LEVEL_LIMIT);
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	int need_gold = 0;
	if (guild->GetCallinTimes() >= other_cfg.guild_call_in_free_time)
	{
		need_gold = other_cfg.guild_call_in_cost_gold;
	}
	else
	{
		guild->AddCallinTimes(1);

		Role *tuanzhan = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(guild->GetGuildMemberManager().GetTuanZhangUid()));
		if (NULL != tuanzhan)
		{
			GuildManager::Instance().SyncGuildBaseInfo(tuanzhan);
		}

		Role *futuanzhan = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(guild->GetGuildMemberManager().GetFuTuanZhangUid()));
		if (NULL != futuanzhan)
		{
			GuildManager::Instance().SyncGuildBaseInfo(futuanzhan);
		}
	}

	if (need_gold > 0 && !role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "GuildCallIn"))
	{
		role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return;
	}

	Protocol::SCGuildOperaSucc gos;
	gos.opera_type = Protocol::SCGuildOperaSucc::OPERA_TYPE_CALL_IN;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&gos, sizeof(gos));

	GuildName guild_name = {0};
	GuildShadow::Instance().GetGuildName(role->GetGuildID(), guild_name);

	int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_call_in, 
		role->GetGuildID(), guild_name, role->GetGuildID());
	if (len > 0) 
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_ONLY_CHAT_WORLD);
	}

	role->NoticeNum(noticenum::EN_GUILD_CALL_IN_SUCC);
}

void GuildRoute::OnGuildFbEnterReq(Role *role)
{
 	if (NULL == role)
 	{
 		return;
 	}
 
 	if (0 == role->GetGuildID())
 	{
 		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
 		return;
 	}
 
 	if (Scene::SCENE_TYPE_GUILD_FB == role->GetScene()->GetSceneType())
 	{
 		role->NoticeNum(errornum::EN_GUILD_ALREADY_IN_GUILD_FB);
 		return;
 	}
 
 	if (!role->GetRoleActivity()->CanEnterGuildFb())
 	{
 		return;
 	}
 
 	if (!role->GetRoleStatusManager()->CanEnterFB()) return;

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}
	int cur_fb_index = guild->GetGuildFbCurIndex();
	GuildFbWaveCfg *guildfb_wavecfg = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbWaveCfg(cur_fb_index);
	if (nullptr == guildfb_wavecfg)
	{
		return;
	}
 
 	SpecialGuildFb *guild_fb = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildFb(guildfb_wavecfg->sceneid, role->GetGuildID());
 	if (NULL != guild_fb)
 	{
		guild->CheckPutHurtRankInfo(role);

		ROLE_LOG_QUICK6(LOG_TYPE_GUILD_FB, role, role->GetLevel(), role->GetGuildID(), "Join", NULL);
		World::GetInstWorld()->GetSceneManager()->GoTo(role, guildfb_wavecfg->sceneid, role->GetGuildID(), gamecommon::GetDisRandPos(guildfb_wavecfg->enter_pos, 5));
 	}
}

void GuildRoute::OnGuildFbOpen(Guild *guild, Role *role)
{
	if (NULL == guild || NULL == role)
	{
		return;
	}

	if (0 == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (!role->GetRoleActivity()->CanEnterGuildFb())
	{
		return;
	}

	int fb_index_count = LOGIC_CONFIG->GetGuildFbConfig().GetGuardWaveCount();

	for (int fb_index = 0; fb_index < fb_index_count; fb_index ++)
	{
		GuildFbWaveCfg *guildfb_wavecfg = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbWaveCfg(fb_index);
		if (nullptr == guildfb_wavecfg)
		{
			return;
		}

		SpecialGuildFb *guild_fb = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildFb(guildfb_wavecfg->sceneid, guild->GetGuildID(), true);
		if (NULL != guild_fb)
		{
			if (0 == fb_index)
			{
				if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_FB))
				{
					guild_fb->OnGuildFbOpen();
				}
				
				if (role->GetRoleStatusManager()->CanEnterFB())
				{
					// 开启活动的人直接传入
					World::GetInstWorld()->GetSceneManager()->GoTo(role, guildfb_wavecfg->sceneid, role->GetGuildID(), gamecommon::GetDisRandPos(guildfb_wavecfg->enter_pos, 5));
				}
			}
		}
	}
}

void GuildRoute::OnGuildFbClose(Guild *guild)
{
	if (NULL == guild)
	{
		return;
	}

	int cur_fb_index = guild->GetGuildFbCurIndex();
	GuildFbWaveCfg *guildfb_wavecfg = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbWaveCfg(cur_fb_index);
	if (nullptr == guildfb_wavecfg)
	{
		return;
	}

	SpecialGuildFb *guild_fb = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildFb(guildfb_wavecfg->sceneid, guild->GetGuildID());
	if (NULL != guild_fb)
	{
		guild_fb->OnGuildFbClose();
	}
}

void GuildRoute::OnGuildFbFinish(GuildID guild_id, bool is_pass)
{
	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		guild->OnGuildFbFinish();
	}
}

void GuildRoute::OnGuildFbActivityOpen()
{
	GuildFbWaveCfg *guildfb_wavecfg = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbWaveCfg(0);
	if (nullptr == guildfb_wavecfg)
	{
		return;
	}
	const GuildManager::GuildMap & guild_map = GuildManager::Instance().GetGuildMap();
	for (GuildManager::GuildMapConstIt itr = guild_map.begin(); itr != guild_map.end(); ++itr)
	{
		if (itr->second == NULL) continue;
		SpecialGuildFb *guild_fb = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildFb(guildfb_wavecfg->sceneid, itr->second->GetGuildID(), false);
		if (NULL != guild_fb)
		{
			guild_fb->OnGuildFbOpen();
			unsigned int guild_fb_finish = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbIntervalS();
			itr->second->SetGuildFbFinishTime(guild_fb_finish);
			itr->second->SyncGuildFbStatus();
		}
	}
}

void GuildRoute::OnAddBonfireMucai(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (0 == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	GuildBonfireInfoMapIt it = m_bonfire_map_register.find(role->GetGuildID());
	if (it != m_bonfire_map_register.end())
	{
		GuildBonfireInfo *bonfire_info = &it->second;

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(bonfire_info->scene_id, bonfire_info->scene_key);
		if (NULL != scene)
		{
			Obj *obj = scene->GetObj(bonfire_info->bonfire_objid);
			if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
			{
				GatherObj *gathar_obj = (GatherObj*)obj;
				if (!gathar_obj->IsBonfire()) return;

				if (gathar_obj->GetBelongGuildID() != role->GetGuildID())
				{
					role->NoticeNum(errornum::EN_ADD_MUCAI_NOT_YOUR_GUILD);
					return;
				}

				if (!gathar_obj->CanAddMuCai(role))
				{
					role->NoticeNum(errornum::EN_GATHAR_ADD_MUCAI_LIMIT);
					return;
				}

				static ItemExtern::ItemConsumeList consume_list; 
				consume_list.Reset();

				ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
				stuff_list[0].item_id = LOGIC_CONFIG->GetGuildBonfireCfg().GetMuCaiItemID();
				stuff_list[0].num = 1;
				stuff_list[0].buyable = false;

				if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list))
				{
					return;
				}

				if (!ItemExtern::ConsumeItem(role, consume_list, "GuildRoute::OnAddBonfireMucai"))
				{
					return;
				}

				role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_GUILD_BONFIRE_ADD_MUCAI);
				gathar_obj->AddMuCai(role);

				role->NoticeNum(errornum::EN_ADD_MUCAI_SUCC);
			}
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_ADD_MUCAI_NOT_YOUR_GUILD);
	}
}

void GuildRoute::OnGotoBonfirePos(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (0 == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	GuildBonfireInfoMapIt it = m_bonfire_map_register.find(role->GetGuildID());
	if (it != m_bonfire_map_register.end())
	{
		GuildBonfireInfo *bonfire_info = &it->second;

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(bonfire_info->scene_id, bonfire_info->scene_key);
		if (NULL != scene)
		{
			Obj *obj = scene->GetObj(bonfire_info->bonfire_objid);
			if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
			{
				GatherObj *gathar_obj = (GatherObj*)obj;
				if (!gathar_obj->IsBonfire()) return;

				const int gathar_id = LOGIC_CONFIG->GetGuildBonfireCfg().GetGatharID();
				if (gathar_obj->GatherId() == gathar_id)
				{
					Posi rand_pos = gamecommon::GetDisRandPos(bonfire_info->bonfire_pos, 8);
					if (!scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, rand_pos.x, rand_pos.y))
					{
						rand_pos = bonfire_info->bonfire_pos;
					}
					
					if (role->GetRoleStatusManager()->CanFlyByShoe(bonfire_info->scene_id))
					{
						World::GetInstWorld()->GetSceneManager()->GoTo(role, bonfire_info->scene_id, bonfire_info->scene_key, rand_pos);
					}
				}
			}
		}
	}
}

void GuildRoute::OnGuildBonfireOpen(Guild *guild, Role *role)
{
	Scene *scene = role->GetScene();
	if (NULL != scene)
	{
		const int gathar_id = LOGIC_CONFIG->GetGuildBonfireCfg().GetGatharID();
		const int gathar_time_s = LOGIC_CONFIG->GetGuildBonfireCfg().GetGatharTimeS();

		GatherObj *gather_obj = new GatherObj();
		gather_obj->SetPos(role->GetPos());
		gather_obj->Init(NULL, gathar_id, gathar_time_s, 0, false);
		gather_obj->InitBonfire(guild->GetGuildID(), guild->GetGuildName());
		gather_obj->SetSpecialGatherType(SPECIAL_GATHER_TYPE_GUILD_BONFIRE);

		scene->AddObj(gather_obj);

		GuildBonfireInfo bonfire_info;
		bonfire_info.bonfire_objid = gather_obj->GetId();
		bonfire_info.scene_id = scene->GetSceneID();
		bonfire_info.scene_key = scene->GetSceneKey();
		bonfire_info.bonfire_pos = gather_obj->GetPos();
		m_bonfire_map_register[guild->GetGuildID()] = bonfire_info;

		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_open_bonfire, 
				guild->GetGuildName(), scene->GetSceneName(), bonfire_info.scene_id, bonfire_info.scene_key,
				bonfire_info.bonfire_pos.x, bonfire_info.bonfire_pos.y);
			if (length > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
				guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
			}
		}
	}
}

void GuildRoute::OnGuildBonfireClose(Guild *guild)
{
	GuildBonfireInfoMapIt it = m_bonfire_map_register.find(guild->GetGuildID());
	if (it != m_bonfire_map_register.end())
	{
		GuildBonfireInfo *bonfire_info = &it->second;

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(bonfire_info->scene_id, bonfire_info->scene_key);
		if (NULL != scene)
		{
			Obj *obj = scene->GetObj(bonfire_info->bonfire_objid);
			if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
			{
				GatherObj *gathar_obj = (GatherObj*)obj;
				if (gathar_obj->IsBonfire())
				{
					const int gathar_id = LOGIC_CONFIG->GetGuildBonfireCfg().GetGatharID();
					if (gathar_obj->GatherId() == gathar_id)
					{
						scene->DeleteObj(gathar_obj->GetId());
					}
				}
			}
		}

		m_bonfire_map_register.erase(it);
	}
}

void GuildRoute::OnStorgePutItem(Role *role, int bag_index, int num)
{
	if (NULL == role)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}
	
	guild->GetStorge()->OnPutItem(role, bag_index, num);
}

void GuildRoute::OnStorgePutItemOneKey(Role *role, Protocol::CSGuildStorgeOneKeyOperate *cmd)
{
	if (NULL == role || NULL == cmd)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	guild->GetStorge()->OnPutItemOneKey(role, cmd);
}

void GuildRoute::OnStorgeDiscardItemOneKey(Role *role, Protocol::CSGuildStorgeOneKeyOperate *cmd)
{
	if (NULL == role || NULL == cmd)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	guild->GetStorge()->OnDiscardItemOneKey(role, cmd);
}

void GuildRoute::OnSetAutoClear(Role *role, short is_auto_clear)
{
	if (NULL == role) return;

	GuildManager::Instance().SetAutoClear(role, is_auto_clear);
}

void GuildRoute::OnStorgeTakeItem(Role *role, int storge_index, int num, ItemID item_id)
{
	if (NULL == role)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	guild->GetStorge()->OnTakeItem(role, storge_index, num, item_id);
}

void GuildRoute::OnStorgeDiscardItem(Role *role, int storge_index, ItemID item_id)
{
	if (NULL == role)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	guild->GetStorge()->OnDiscardItem(role, storge_index, item_id);
}

void GuildRoute::OnReqStorgeInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	guild->GetStorge()->SyncStorgeInfo(role);
	guild->GetStorge()->SyncStorgeLog(role);
}
