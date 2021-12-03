#include "guildmanager.hpp"
#include "guild.hpp"

#include "global/guild/impl/guildauthority.hpp"
#include "global/activity/activitymanager.hpp"

#include "global/rmibackobjdef.h"
#include "internalcomm.h"
#include "engineadapter.h"

#include "servercommon/string/globalstr.h"

#include "servercommon/internalprotocal/gcprotocal.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "global/guild/impl/guildmember.hpp"
#include "global/guild/impl/guildapplyformanager.hpp"
#include "global/guild/roleguild.hpp"
#include "gamelog.h"

#include "world.h"
#include "gstr.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "serverlogic.h"
#include "obj/character/role.h"
#include "global/mail/mailmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "global/offlineregister/offlineregistermanager.hpp"
#include "other/route/guildroute.hpp"
#include "other/route/mailroute.hpp"
#include "other/event/eventhandler.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/guildfbconfig.hpp"
#include "config/activityconfig/guildbonfireconfig.hpp"
#include "other/vip/vipconfig.hpp"

#include "servercommon/userprotocal/msgsystem.h"
#include "protocal/msgscguildunion.hpp"
#include "protocal/msgchatmsg.h"

#include "servercommon/string/gameworldstr.h"
#include "global/worldstatus/worldstatus.hpp"
#include "global/worldstatus3/worldstatus3.hpp"
#include "gameworld/gameworld/scene/activityshadow/activityshadow.hpp"
#include "item/knapsack.h"
#include "item/money.h"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/sublock/sublock.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "global/combineserveractivity/combineserveractivitymanager.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "gameworld/gameworld/chat/chatmanager.h"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "global/rank/rankmanager.hpp"

#include <algorithm>

GuildManager::GuildManager()
: m_load_flag(false), m_guild_box_is_gm_start(false)
{
}

GuildManager::~GuildManager()
{
	for (GuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); it != end; ++it)
	{
		delete it->second;
	}

	m_guild_map.clear();
}

bool GuildManager::OnServerStart()
{
	RMIInitGuildBackObjectImpl *impl = new RMIInitGuildBackObjectImpl();

	RMIGlobalClient ac;
	ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return ac.InitGuildAsyn(0, impl);
}

void GuildManager::OnServerStop()
{
	for (GuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); it != end; ++it)
	{
		it->second->Save();
	}

	std::map<GuildID, Guild*>::iterator guild_iter = m_guild_map.begin();
	for (; guild_iter != m_guild_map.end() && NULL != guild_iter->second; guild_iter++)
	{
		guild_iter->second->OnServerStop();
	}
}

void RMIInitGuildBackObjectImpl::InitGuildRet(int ret, const AllGuildParam &guild_list)
{
	GuildManager::Instance().Init(ret, guild_list);
}

GuildManager & GuildManager::Instance()
{
	static GuildManager gm;
	return gm;
}

void GuildManager::Init(int ret, const AllGuildParam &guild_list)
{
	if (0 != ret)
	{
		printf("GuildManager::Init Failed \n");
		ServerLogic::GetInstServerLogic()->StopServer();
	}
	else 
	{
		GuildID next_guild_id_from = 0; 

		for (int i = 0; i < guild_list.count && i < MAX_ONCE_GUILD_COUNT; i++)
		{
			if (guild_list.guild_list[i].member_param.count > 0) 
			{
				this->AddGuild(guild_list.guild_list[i], false);

				if (0 == guild_list.guild_list[i].base_param.guild_name[0])
				{
					printf("Empty Guild Name, Guild ID: %d\n", guild_list.guild_list[i].base_param.guild_id);
					gamelog::g_log_serious_error.printf(LL_CRITICAL, "[GuildManager::Init INVALID GUILD NAME] [guild_id:%d]", (int)guild_list.guild_list[i].base_param.guild_id);
				}
			}
			else
			{
				printf("GuildManager::Init : AddGuild Failed Guild ID: %d\n", guild_list.guild_list[i].base_param.guild_id);
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[GuildManager::Init INVALID MEMBER COUNT] [guild_id:%d]", (int)guild_list.guild_list[i].base_param.guild_id);
			}

			if (guild_list.guild_list[i].base_param.guild_id > next_guild_id_from)
			{
				next_guild_id_from = guild_list.guild_list[i].base_param.guild_id;
			}
		}

		if (guild_list.count >= MAX_ONCE_GUILD_COUNT)
		{
			RMIInitGuildBackObjectImpl *impl = new RMIInitGuildBackObjectImpl();

			RMIGlobalClient ac;
			ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
			ac.InitGuildAsyn(next_guild_id_from, impl);
		}
		else 
		{
			printf("Init Guild Succ, Guild Count: %d\n", static_cast<int>(m_guild_map.size()));

			m_load_flag = true;

			std::map<GuildID, Guild*>::iterator guild_iter = m_guild_map.begin();
			for (; guild_iter != m_guild_map.end() && NULL != guild_iter->second; guild_iter++)
			{
				guild_iter->second->OnServerStart();
			}
		}
	}
}

void GuildManager::CreateGuildFailRollback(Role *role, int create_guild_type, const ItemDataWrapper &item_wrapper)
{
	if (NULL == role)
	{
		return;
	}

	ggprotocal::CreateGuildFailRollback cgfrb;
	cgfrb.create_guild_type = create_guild_type;
	cgfrb.item_wrapper = item_wrapper;

	GuildRoute::Instance().OnCreateGuildFailRollback(role, &cgfrb);
}

bool GuildManager::DismissGuild(Guild *guild, const UserID &user_id, int dismiss_type)
{
	RMIDismissGuildBackObjectImpl *dgbo = new RMIDismissGuildBackObjectImpl();

	dgbo->guild_id = guild->GetGuildID();
	dgbo->user_id = user_id;
	dgbo->dismiss_type = dismiss_type;

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.DismissGuildAsyn(dgbo->guild_id, dgbo);
}

bool GuildManager::ExtendGuildMember(Role *role, bool can_use_gold, short num)
{
	if (NULL == role || num <= 0)
	{
		return false;
	}

	Guild * guild = this->GetGuild(role->GetUserId());
	if (NULL == guild)  // 1：没有军团
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
		return false;
	}

	GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member) // 2：必须是该军团成员
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return false;
	}

	if (!GuildAuthority::CanExtendMember(member)) // 3：必须是有权限
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return false;
	}

	if (role->GetSublock()->CheckHasLock()) return false;

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	GuildCommonDataParam *guild_common_data = guild->GetGuildCommonDataParam();
	if (NULL == guild_common_data)
	{
		return false;
	}

	short curr_member_count = guild_common_data->default_member_count + guild_common_data->extend_member_count;
	int max_num = MAX_MEMBER_COUNT;

	if (curr_member_count >= max_num)
	{
		role->NoticeNum(errornum::EN_GUILD_EXTEND_MEMBER_MAX);
		return false;
	}

	if (curr_member_count + num > max_num)
	{
		num = max_num - curr_member_count;
	}

	// 检查消耗
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	ItemID consume_item_id = LOGIC_CONFIG->GetGuildConfig().GetGuildExtendMemberItemID();
	{
		int total_need_item_num = 0;
		{
			for (int i = 0; i < num; ++ i)
			{
				int extend_member_count = curr_member_count + i;
				int need_item_count = LOGIC_CONFIG->GetGuildConfig().GetGuildExtendMemberNeedItemCount(extend_member_count);

				if (need_item_count <= 0)
				{
					return false;
				}

				total_need_item_num += need_item_count;
			} 
		}

		{
			ItemExtern::ItemConsumeConfig comsume_stuff;
			comsume_stuff.item_id = consume_item_id;
			comsume_stuff.num = total_need_item_num;
			comsume_stuff.buyable = can_use_gold;

			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, &comsume_stuff, &consume_list))
			{
				role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				return false;
			}

			if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(consume_list.need_gold_num))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return false;
			}
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "ExtendGuildMemberNum"))
		{
			return false;
		}
	}

	guild_common_data->extend_member_count += num;

	this->SendGuildMemberCountInfo(role);

	gamelog::g_log_guild_route.printf(LL_INFO, "ExtendMemberNum user[%d, %s], extern_num[%d], curr_member_num[%d], consume_all_gold[%d], consume_item[id:%d, num:%d]",
		role->GetUID(), role->GetName(), num, guild_common_data->extend_member_count, consume_list.need_gold_num,
		consume_item_id, consume_list.bind_item_num + consume_list.nobind_item_num);

	return true;
}

void GuildManager::SendGuildMemberCountInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	Guild * guild = this->GetGuild(role->GetUserId());
	if (NULL == guild)  // 1：没有军团
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
		return;
	}

	GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member) // 2：必须是该军团成员
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	GuildCommonDataParam *guild_common_data = guild->GetGuildCommonDataParam();
	if (NULL == guild_common_data)
	{
		return;
	}

	Protocol::SCGuildMemberNum kmgn;
	kmgn.max_guild_member_num = guild_common_data->default_member_count + guild_common_data->extend_member_count;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&kmgn, sizeof(kmgn));
}

void GuildManager::OnAddGuild(Guild *guild, bool create)
{
	if (NULL == guild)
	{
		return;
	}
}

void GuildManager::OnRemoveGuild(Guild *guild)
{
	if (NULL == guild)
	{
		return;
	}
}

void GuildManager::Update(unsigned long interval, time_t now_second)
{
	if (!m_load_flag) return;

	for (GuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); it != end; ++it)
	{
		Guild *guild = it->second;
		guild->Update(interval, now_second);
	}
}

void GuildManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (!m_load_flag) return;

	OfflineRegisterManager::Instance().GuildBoxResetAll();
	OnGuildKillCrossBossRankReward();

	for (GuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); it != end; ++it)
	{
		Guild *guild = it->second;
		if (NULL != guild)
		{
			guild->GetGuildCommonDataParam()->kill_csa_boss_num = 0;
			guild->ClearKillCrossBossNum();
		}
	}
	
	GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
	if (NULL != guild_rank)
	{
		guild_rank->ReInitGuildRank(GUILD_RANK_TYPE_KILL_CROSS_BOSS);
	}
}

void GuildManager::OnUserLogin(Role *role)
{
	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(user_id);
	if (NULL != guild)
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL != member)
		{
			GuildName guild_name;
			memset(guild_name, 0, sizeof(guild_name));

			guild->GetGuildName(guild_name);
			this->SyncToRole(role, guild->GetGuildID(), guild_name, member->GetPost());

			member->SetOnline(true);

			guild->OnMemberLogin(member, role);

			// 角色上线  如果活动已开始通知其天赐铜币活动状态
			guild->IsTianCiTongBiOpen(role);

			return;
		}
	}

	this->SyncToRole(role, INVALID_GUILD_ID, "", 0);
}

void GuildManager::OnUserLogout(Role *role)
{
	Guild *guild = this->GetGuild(role->GetUserId());
	if (NULL == guild) return;

	GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL != member)
	{
		member->SetLastLoginTime(EngineAdapter::Instance().Time());
		member->SetOnline(false);

		guild->OnMemberLogout(member);
	}
}

void RMICreateGuildBackObjectImpl::CreateGuildRet(int ret, GuildID guild_id)
{
	const char *log_ret = "succeed";

	if (0 == ret && guild_id > 0) // 1：DB创建成功
	{
		guild_base.guild_id = guild_id;

		static AllGuildParam::GuildParam guild_param;

		guild_param.apply_for_param.count = 0;
		guild_param.member_param.count = 0;
		guild_param.event_param.count = 0;
		memcpy(&guild_param.base_param, &guild_base, sizeof(guild_param.base_param));

		Guild *guild = GuildManager::Instance().AddGuild(guild_param, true);
		if (NULL != guild) // 2：系统不出错 
		{
			Protocol::SCCreateGuild create_rsp;

			create_rsp.ret = 0;
			create_rsp.guild_id = guild_id;
			F_STRNCPY(create_rsp.guild_name, guild_base.guild_name, sizeof(create_rsp.guild_name));

			World::GetInstWorld()->GetSceneManager()->SendToUser(user_id, (const char*)&create_rsp, sizeof(create_rsp));

			guild->GetGuildMemberManager().AddMember(user_id, GUILD_POST_TUANGZHANG);
			guild->GetGuildEventManager().OnCreate(role_name);

			/*ggprotocal::AchieveInfo achieve_info;
			achieve_info.achieve_type = ACHIEVE_TYPE_GUILD_CREATE;
			guild_manager->GetGlobalServer()->SendToUserGameWorld(user_id, (const char*)&achieve_info, sizeof(achieve_info));*/

			int length = 0;
			Role *creator = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);

			if (guild_base.guild_level >= 2)
			{
				guild->AddGuildExp(LOGIC_CONFIG->GetGuildConfig().GetGuildInitExp2());

				if (NULL != creator) creator->AddGuildGongxian(LOGIC_CONFIG->GetGuildConfig().GetGuildCreatorGetGongXian2(), "create guild level2");

				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_create_lv2_guild_succ_public_notice, 
					UidToInt(user_id), role_name, camp, guild_param.base_param.guild_name, guild_base.guild_level, guild_id);
			}
			else
			{
				guild->AddGuildExp(LOGIC_CONFIG->GetGuildConfig().GetGuildInitExp1());

				if (NULL != creator) creator->AddGuildGongxian(LOGIC_CONFIG->GetGuildConfig().GetGuildCreatorGetGongXian1(), "create guild level1");

				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_create_guild_succ_public_notice, 
					UidToInt(user_id), role_name, camp, guild_param.base_param.guild_name, guild_id);
			}

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE, 0, 0);
			}

			// 屏蔽创会邮件
			// 			static MailContentParam contentparam; contentparam.Reset();
			// 			UserMailManager::Instance().MailToUser(MAIL_TYPE_SYSTEM, user_id, SYSTEM_MAIL_REASON_GUILD_CREATE, contentparam);

			log_ret = "succeed";

			if (creator != NULL)
			{	
				guild->GetGuildCommonDataParam()->daily_guild_relive_times = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().craete_guild_init_relive_times;
				guild->SyncGulidReliveTimesInfo(creator);
				creator->SyncSCGulidBossRedbagInfo();
			}

			// 帮派答题处理
			if (INVALID_GUILD_ID != guild_id)
			{
				ActivityShadow::Instance().GuildQuestionQuickSetQuestion(guild_id);
			}
		}
		else 
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL != role)
			{
				role->NoticeNum(errornum::EN_GUILD_SYSTEM_ERROR);
				GuildManager::Instance().CreateGuildFailRollback(role, create_guild_type, item_wrapper);
			}
			else
			{
				gamelog::g_log_guild.printf(LL_INFO, "GuildManager::CreateGuild AddGuild fail and role(%d) null", UidToInt(user_id));
			}

			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[GuildManager::CreateGuild AddGuild SYSTEM ERROR] [uid:%d guild_id:%d]", UidToInt(user_id), (int)guild_id);

			log_ret = "add_failed";
		}
	}
	else
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
		if (NULL != role)
		{
			role->NoticeNum(errornum::EN_GUILD_SYSTEM_ERROR);
			GuildManager::Instance().CreateGuildFailRollback(role, create_guild_type, item_wrapper);
		}
		else
		{
			gamelog::g_log_guild.printf(LL_INFO, "GuildManager::CreateGuild db fail and role(%d) null", UidToInt(user_id));
		}

		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[GuildManager::CreateGuild db SYSTEM ERROR] [uid:%d guild_id:%d]", UidToInt(user_id), (int)guild_id);

		log_ret = "system_error";
	}

	gamelog::g_log_guild.printf(LL_INFO, "guild -> create guild ret[ ret:%s, guild_id:%d]", (const char*)log_ret, (int)guild_id);
}

void GuildManager::CreateGuild(Role *role, gglobalprotocal::CreateGuild *create_req)
{
	if (NULL == role)
	{
		return;
	}

	create_req->role_name[sizeof(create_req->role_name) - 1] = '\0';
	create_req->plat_name[sizeof(create_req->plat_name) - 1] = '\0';
	create_req->guild_name[sizeof(create_req->guild_name) - 1] = '\0';
	create_req->guild_notice[sizeof(create_req->guild_notice) - 1] = '\0';

	const char *log_ret = "succeed";

	UserID user_id = role->GetUserId();

	if (!ActivityManager::Instance().IsActivityLimitGuildOper(GUILD_OPER_TYPE_CREATE, role))
	{
		Guild * guild = this->GetGuild(user_id);
		if (NULL == guild)  // 1：没有军团
		{
			if (0 == create_req->guild_name[0]) return;
			InvalidCharFilte(create_req->guild_name);
			GuildNameCatPostfix(create_req->guild_name, user_id.db_index);

			bool repeat_name = false;
			for (GuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); it != end; ++it)
			{
				if (0 == strncmp(it->second->GetGuildName(), create_req->guild_name, sizeof(GuildName)))
				{
					repeat_name = true;
					break;
				}
			}

			//发起仙盟
			static std::set<std::string> guild_name_cache;
			std::string the_guild_name(create_req->guild_name, sizeof(create_req->guild_name));
			auto guild_name_cache_it = guild_name_cache.find(the_guild_name);
			if (guild_name_cache_it != guild_name_cache.end())
			{
				repeat_name = true;
			}
			else
			{
				guild_name_cache.insert(std::move(the_guild_name));
			}


			if (!repeat_name) // 2：名字可用 
			{
				RMICreateGuildBackObjectImpl *cgbo = new RMICreateGuildBackObjectImpl();
				//设置本次创建仙盟的时间. 5分钟内只能成功创建1次
				role->SetLastCreateGuildTime(static_cast<unsigned int>(EngineAdapter::Instance().Time()));
				cgbo->user_id = user_id;
				F_STRNCPY(cgbo->role_name, create_req->role_name, sizeof(cgbo->role_name));
				F_STRNCPY(cgbo->plat_name, create_req->plat_name, sizeof(cgbo->plat_name));
				cgbo->level = create_req->level;
				cgbo->sex = create_req->sex;
				cgbo->prof = create_req->prof;
				cgbo->camp = create_req->camp;
				cgbo->vip_level = create_req->vip_level;
				cgbo->capability = create_req->capability;
				cgbo->create_guild_type = create_req->create_guild_type;
				cgbo->item_wrapper = create_req->item_wrapper;

				memset(&cgbo->guild_base, 0, sizeof(cgbo->guild_base));
				cgbo->guild_base.common_data_param.is_auto_clear = 1;

				F_STRNCPY(cgbo->guild_base.guild_name, create_req->guild_name, sizeof(cgbo->guild_base.guild_name));
				F_STRNCPY(cgbo->guild_base.notice, create_req->guild_notice, sizeof(cgbo->guild_base.notice));
				cgbo->guild_base.create_time = EngineAdapter::Instance().Time();
				cgbo->guild_base.camp = create_req->camp;
				cgbo->guild_base.guild_level = (GUILD_CREATE_TYPE_ITEM == create_req->create_guild_type ? 2 : 1);
				cgbo->guild_base.auto_kickout_setup = 1;
				cgbo->guild_base.applyfor_setup = GUILD_APPLYFOR_CONDITION;
				cgbo->guild_base.applyfor_need_capability = 100;
				cgbo->guild_base.applyfor_need_level = 20;
				cgbo->guild_base.active_degree = 0;

				int guild_member_count = 0;
				const char *plat = LocalConfig::Instance().GetPlatName();
				if (NULL!= plat)
				{
					const std::string plat_name(plat);
					guild_member_count = LOGIC_CONFIG->GetGuildConfig().GetGuildInitMemberByPlatName(plat_name);
				}
				else
				{
					guild_member_count = LOGIC_CONFIG->GetGuildConfig().GetGuildInitMemberByPlatName("default");
				}
		

				cgbo->guild_base.common_data_param.default_member_count = guild_member_count;
				
				RMIGlobalClient gc;
				gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
				bool ret = gc.CreateGuildAsyn(user_id.db_index, cgbo->guild_base, cgbo);
				if (!ret)
				{
					role->NoticeNum(errornum::EN_GUILD_SYSTEM_ERROR);
					this->CreateGuildFailRollback(role, create_req->create_guild_type, create_req->item_wrapper);

					log_ret = "system_error";
				}
				else
				{
					log_ret = "succeed";
				}
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_INVILD_NAME);
				this->CreateGuildFailRollback(role, create_req->create_guild_type, create_req->item_wrapper);

				log_ret = "invalid_name";
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
			this->CreateGuildFailRollback(role, create_req->create_guild_type, create_req->item_wrapper);

			log_ret = "already_in_guild";
		}
	}
	else
	{
		this->CreateGuildFailRollback(role, create_req->create_guild_type, create_req->item_wrapper);

		log_ret = "activity_limit";
	}

	gamelog::g_log_guild.printf(LL_INFO, "guild -> create guild[ ret:%s, guild_name:%s, "
		"role[ uid:%d, name:%s, level:%d, prof:%d, camp:%d ], money[coin:%d, gold:%d ] "
		"create_guild_type:%d ]",
		(const char*)log_ret, (const char*)create_req->guild_name, 
		UidToInt(UserID((int)create_req->db_index, (int)create_req->role_id)), (const char*)create_req->role_name, 
		(int)create_req->level, (int)create_req->prof, (int)create_req->camp, 0, 0,
		(int)create_req->create_guild_type);
}

void RMIDismissGuildBackObjectImpl::DismissGuildRet(int ret)
{
	const char *log_ret = "succeed";

	if (0 == ret) // 1：DB解散成功
	{
		Guild *guild = GuildManager::Instance().GetGuild(guild_id);
		if (NULL != guild)
		{
			static MailSubject mail_subject = {0};
			static MailContentTxt contenttxt = {0};

			if (GUILD_DISMISS_TYPE_AUTO == dismiss_type)
			{
				int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_auto_dimiss_guild_mail_subject);
				int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_auto_dimiss_guild_mail_content);
				if (length1 > 0 && length2 > 0)
				{
					guild->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
				}
			}
			else if (GUILD_DISMISS_TYPE_GM == dismiss_type)
			{
				Role *gm_role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
				if (NULL != gm_role)
				{
					int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_gm_dimiss_guild_mail_subject);
					int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_gm_dimiss_guild_mail_content, gm_role->GetName());
					if (length1 > 0 && length2 > 0)
					{
						guild->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
					}
				}
			}
			else
			{
				GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
				if (NULL != member)
				{
					UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(user_id));
					if (NULL != uc)
					{
						int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_dimiss_guild_mail_subject);
						int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_dimiss_guild_mail_content, uc->user_name);
						if (length1 > 0 && length2 > 0)
						{
							guild->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
						}
					}
				}
			}
		}

		if (GUILD_DISMISS_TYPE_DEFAULT == dismiss_type)
		{
			Protocol::SCDismissGuild dismiss_rsp;
			dismiss_rsp.ret = 0;
			World::GetInstWorld()->GetSceneManager()->SendToUser(user_id, (const char*)&dismiss_rsp, sizeof(dismiss_rsp));
		}

		GuildManager::Instance().RemoveGuild(guild_id);

		log_ret = "succeed";
	}
	else // 2：系统出错
	{
		if (GUILD_DISMISS_TYPE_AUTO == dismiss_type)
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[GuildManager::AutoDismissGuild SYSTEM ERROR] [guild_id:%d]", (int)guild_id);
		}
		else if (GUILD_DISMISS_TYPE_GM == dismiss_type)
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[GuildManager::GmDismissGuild SYSTEM ERROR] [guild_id:%d]", (int)guild_id);
		}
		else
		{
			//guild_manager->GetGlobalServer()->NoticeToUser(errornum::EN_GUILD_SYSTEM_ERROR);

			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[GuildManager::DismissGuild SYSTEM ERROR] [uid:%d guild_id:%d]", UidToInt(user_id), (int)guild_id);
		}

		log_ret = "system_error";
	}

	if (GUILD_DISMISS_TYPE_AUTO == dismiss_type)
	{
		gamelog::g_log_guild.printf(LL_INFO, "guild -> autodismiss guild ret[ ret:%s, guild_id:%d]", (const char*)log_ret, (int)guild_id);
	}
	else if (GUILD_DISMISS_TYPE_GM == dismiss_type)
	{
		gamelog::g_log_guild.printf(LL_INFO, "guild -> gmdismiss guild ret[ ret:%s, guild_id:%d]", (const char*)log_ret, (int)guild_id);
	}
	else
	{
		gamelog::g_log_guild.printf(LL_INFO, "guild -> dismiss guild ret[ ret:%s, guild_id:%d]", (const char*)log_ret, (int)guild_id);
	}
}

void GuildManager::DismissGuild(Role *role, gglobalprotocal::DismissGuild *dismiss_req)
{
	if (NULL == role)
	{
		return;
	}

	const char *log_ret = "succeed";

	UserID user_id = role->GetUserId();

	if (!ActivityManager::Instance().IsActivityLimitGuildOper(GUILD_OPER_TYPE_DISMISS, role))
	{
		Guild *guild = this->GetGuild(dismiss_req->guild_id);
		if (NULL != guild) // 1：必须有军团
		{
			GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
			if (NULL != member) // 2：必须是该军团成员 
			{
				if (GuildAuthority::CanDismiss(member)) // 3：必须有权力
				{
					if (!guild->IsTianCiTongBiOpen(role)) // 判断是否处于天赐铜币活动开始状态
					{
						if (guild->CanDismiss(role))
						{
							if (!this->DismissGuild(guild, user_id, GUILD_DISMISS_TYPE_DEFAULT))
							{
								role->NoticeNum(errornum::EN_GUILD_SYSTEM_ERROR);
							}
							else
							{
								CombineServerActivityManager::Instance().OnCSABossDismissGuild(guild->GetGuildID());
							}
						}
					}
					else
					{
						gstr::SendError(role, "en_guild_tianci_tongbi_limit");
					}
				}
				else 
				{
					role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);

					log_ret = "not_have_power";
				}
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);

				log_ret = "not_in_guild";
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_NO_EXIST);

			log_ret = "not_exist";
		}
	}
	else
	{
		if (ActivityManager::Instance().IsActivityOpen(ACTIVITY_TYPE_GUILD_QUESTION))
		{
			gstr::SendError(role, "guild_question_dismiss_activity_limit");
		}

		log_ret = "activity_limit";
	}

	gamelog::g_log_guild.printf(LL_INFO, "guild -> dismiss guild[ ret:%s, guild_id:%d, role[uid:%d] ]",
		(const char*)log_ret, dismiss_req->guild_id, role->GetUID());
}

void GuildManager::ApplyForJoin(Role *role, gglobalprotocal::ApplyForJoinGuild *join_req)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	if (NULL == this->GetGuild(user_id)) // 1：必须没有军团
	{
		if (1 != join_req->is_auto_join)
		{
			Guild *guild = this->GetGuild(join_req->guild_id);
			if (NULL != guild) // 2：军团必须存在
			{
				// 			if (guild->GetCamp() == join_req->camp) // 3：要在同一阵营
				// 			{
				switch (guild->GetApplyforSetup())
				{
				case GUILD_APPLYFOR_SETUP_NEED_AGREE: // 5：需要认证
					{
						if (NULL == guild->GetGuildApplyForManager().GetApplayFor(user_id))  // 5.1：没有重复申请
						{
							bool ret = guild->GetGuildApplyForManager().AddApplyFor(user_id);
							if (ret) // 5：申请列表未满
							{
								this->OnUserAddApplyFor(user_id, guild->GetGuildID());

								Protocol::SCApplyForJoinGuild join_rsp;
								join_rsp.ret = 0;
								join_rsp.guild_id = join_req->guild_id;
								EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&join_rsp, sizeof(join_rsp));

								guild->OnNewApplyFor(role);
							}
							else
							{
								role->NoticeNum(errornum::EN_GUILD_APPLY_FOR_FULL);
							}
						}
						else 
						{
							role->NoticeNum(errornum::EN_GUILD_REPEAT_APPLY_FOR);
						}	
					}
					break;

				case GUILD_APPLYFOR_SETUP_FORBIT: // 6：禁止申请
					{
						role->NoticeNum(errornum::EN_GUILD_FORBIT_APPLYFOR);
					}
					break;

				case GUILD_APPLYFOR_CONDITION: // 7：满足条件自动通过
					{
						if (join_req->capability >= guild->GetApplyforNeedCapability() && join_req->level >= guild->GetApplyforNeedLevel())
						{
							gglobalprotocal::InviteGuildAck iga;
							iga.guild_id = join_req->guild_id;
							iga.db_index = join_req->db_index;
							iga.role_id = join_req->role_id;
							F_STRNCPY(iga.role_name, join_req->role_name, sizeof(GameName));
							F_STRNCPY(iga.plat_name, join_req->plat_name, sizeof(PlatName));
							iga.level = join_req->level;
							iga.prof = join_req->prof;
							iga.camp = join_req->camp;
							iga.sex = join_req->sex;
							iga.vip_level = join_req->vip_level;
							iga.capability = join_req->capability;

							{
								GameName tuanzhang_name = {0};
								UserID tuanzhang_userid = guild->GetGuildRegister().GetGuildTuanZhangInfo(tuanzhang_name);
								iga.invite_db_index = tuanzhang_userid.db_index;
								iga.invite_role_id = tuanzhang_userid.role_id;
							}

							iga.result = 0;

							this->InviteAck(role, &iga, true);
						}
						else
						{
							role->NoticeNum(errornum::EN_GUILD_JOIN_GUILD_LIMIT);
						}
					}
					break;
				}
				// 			}
				// 			else
				// 			{
				// 				role->NoticeNum(errornum::EN_GUILD_NOT_IN_SAME_CAMP);
				// 			}
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
			}
		}
		else
		{
			GuildID better_guild_id = INVALID_GUILD_ID;
			int better_guild_cap = 0;

			GuildMapIt iter = m_guild_map.begin();
			for(; iter != m_guild_map.end(); iter++)
			{
				Guild* guild = iter->second;
				if (NULL != guild)
				{
					// 			if (guild->GetCamp() == join_req->camp) // 3：要在同一阵营
					// 			{
					switch (guild->GetApplyforSetup())
					{
					case GUILD_APPLYFOR_SETUP_NEED_AGREE: // 5：需要认证
						{
							if (NULL == guild->GetGuildApplyForManager().GetApplayFor(user_id))  // 5.1：没有重复申请
							{
								bool ret = guild->GetGuildApplyForManager().AddApplyFor(user_id);
								if (ret) // 5：申请列表未满
								{
									this->OnUserAddApplyFor(user_id, guild->GetGuildID());

									Protocol::SCApplyForJoinGuild join_rsp;
									join_rsp.ret = 0;
									join_rsp.guild_id = join_req->guild_id;
									EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&join_rsp, sizeof(join_rsp));

									guild->OnNewApplyFor(role);
								}
							}
						}
						break;

					case GUILD_APPLYFOR_CONDITION: // 7：满足条件自动通过
						{
							if (join_req->capability >= guild->GetApplyforNeedCapability() && join_req->level >= guild->GetApplyforNeedLevel())
							{
								if (guild->GetGuildRegister().GetGuildMemberCount() < guild->GetGuildMaxMemberCount())
								{
									int guild_cap = guild->GetCapability();
									if (guild_cap > better_guild_cap)
									{
										better_guild_cap = guild_cap;
										better_guild_id = guild->GetGuildID();
									}
								}
							}
						}
						break;
					}
					// 			}
					// 			else
					// 			{
					// 				role->NoticeNum(errornum::EN_GUILD_NOT_IN_SAME_CAMP);
					// 			}
				}
			}

			Guild *better_guild = this->GetGuild(better_guild_id);
			if(NULL != better_guild)
			{
				gglobalprotocal::InviteGuildAck iga;
				iga.guild_id = better_guild_id;
				iga.db_index = join_req->db_index;
				iga.role_id = join_req->role_id;
				F_STRNCPY(iga.role_name, join_req->role_name, sizeof(GameName));
				F_STRNCPY(iga.plat_name, join_req->plat_name, sizeof(PlatName));
				iga.level = join_req->level;
				iga.prof = join_req->prof;
				iga.camp = join_req->camp;
				iga.sex = join_req->sex;
				iga.vip_level = join_req->vip_level;
				iga.capability = join_req->capability;

				{
					GameName tuanzhang_name = {0};
					UserID tuanzhang_userid = better_guild->GetGuildRegister().GetGuildTuanZhangInfo(tuanzhang_name);
					iga.invite_db_index = tuanzhang_userid.db_index;
					iga.invite_role_id = tuanzhang_userid.role_id;
				}

				iga.result = 0;

				this->AutoJoinInviteAck(role, &iga, true);
			}
			else
			{
				role->NoticeNum(errornum::EN_GUILD_NO_ATUO_JOIN_GUILD);
			}
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
	}
}

void GuildManager::ApplyForJoinAck(Role *role, gglobalprotocal::ApplyForJoinGuildAck *join_ack)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	// role对这个申请列表里的玩家做批量操作
	Guild *guild = this->GetGuild(join_ack->guild_id); // 1：必须有该军团
	if (NULL != guild)
	{
		GuildMember *verify_member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL != verify_member) // 2：必须是该军团成员
		{
			if (GuildAuthority::CanVerify(verify_member)) // 3：必须是有权限
			{
				for (int i = 0; i < join_ack->count && i < MAX_APPLY_FOR_COUNT; ++i)
				{
					UserID apply_user_id(join_ack->apply_for[i].apply_for_db_index, join_ack->apply_for[i].apply_for_role_id);

					GuildApplyForManager::ApplyFor *apply_record = guild->GetGuildApplyForManager().GetApplayFor(apply_user_id); 
					if (NULL != apply_record) // 4：必须是有申请过
					{
						if (0 == join_ack->result)
						{
							if (NULL == this->GetGuild(apply_user_id)) // 5：对方必须不在其他军团
							{
								GuildMember *applyfor_member = guild->GetGuildMemberManager().AddMember(apply_user_id, GUILD_POST_CHENG_YUAN);
								if (NULL == applyfor_member) // 6：人满了 
								{
									role->NoticeNum(errornum::EN_GUILD_ROLE_COUNT_LIMIT);
									World::GetInstWorld()->GetSceneManager()->NoticeToUser(apply_user_id, errornum::EN_GUILD_ROLE_COUNT_LIMIT);
								}
								else
								{
									guild->GetGuildEventManager().OnMemberAdd(applyfor_member, verify_member);

									guild->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_MEMBER_ADD);

									guild->IsTianCiTongBiOpen(role); // 如果活动开始 通知新加入成员
								}
							}
							else 
							{
								role->NoticeNum(errornum::EN_GUILD_OTHER_ALREADY_IN_GUILD);
							}
						}
						else
						{
							static Protocol::SCApplyForJoinGuildAck afjga;
							afjga.result = join_ack->result;
							afjga.guild_id = join_ack->guild_id;
							guild->GetGuildName(afjga.guild_name);
							World::GetInstWorld()->GetSceneManager()->SendToUser(apply_user_id, (const char *)&afjga, sizeof(afjga));
						}
					}
					else 
					{
						role->NoticeNum(errornum::EN_GUILD_APPLY_FOR_NOT_EXIST);
					}

					guild->GetGuildApplyForManager().RemoveApplyFor(apply_user_id);

					this->OnUserRemoveApplyFor(apply_user_id, guild->GetGuildID());
				}

				gglobalprotocal::GetApplyForList apply_info_req;
				apply_info_req.guild_id = join_ack->guild_id;
				apply_info_req.db_index = role->GetUserId().db_index;
				apply_info_req.role_id = role->GetUserId().role_id;

				GuildManager::Instance().GetApplyForList(role, &apply_info_req);
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::QuitGuild(Role *role, gglobalprotocal::QuitGuild *quit_req)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(quit_req->guild_id); // 1：必须有该军团
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
		return;
	}

	GuildMember *quit_member = guild->GetGuildMemberManager().GetMember(user_id); // 2：必须是该军团成员
	if (NULL == quit_member)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (ActivityManager::Instance().IsActivityLimitGuildOper(GUILD_OPER_TYPE_QUIT, role)) // 活动限制
	{
		return;
	}

	if (!ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_LIEKUN_FB))
	{
		role->NoticeNum(errornum::EN_GUILD_LIEKUN_ZHUMO_LIMIT);
		return ;
	}

	if (!ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE))
	{
		role->NoticeNum(errornum::EN_CROSS_GUILD_BATTLE_LIMIT);
		return ;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (CrossGuildBattleConfig::Instance().IsZhuMoTime(now_second))
	{
		role->NoticeNum(errornum::EN_GUILD_LIEKUN_ZHUMO_LIMIT);
		return ;
	}

	if (!guild->CanQuit(role)) // 能否退出
	{
		return;
	}

	if (!GuildAuthority::CanQuit(quit_member))  // 3：团长不能直接退出
	{
		role->NoticeNum(errornum::EN_GUILD_CANNOT_QUIT);
		return;
	}

	Protocol::SCQuitGuild quit_rsp;
	quit_rsp.ret = 0;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&quit_rsp, sizeof(quit_rsp));

	guild->GetGuildEventManager().OnMemberQuit(quit_member);

	guild->GetGuildMemberManager().RemoveMember(user_id);

	guild->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_MEMBER_REMOVE);
}

void GuildManager::Invite(Role *role, gglobalprotocal::InviteGuild *invite_req)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(invite_req->guild_id); // 1：军团必须存在
	if (NULL != guild) 
	{
		if (guild->IsGuildFull())
		{
			role->NoticeNum(errornum::EN_GUILD_CAN_NOT_INVITE_FOR_MEMBER_FULL);
			return;
		}

		if (guild->GetGuildMemberManager().GetInviteDayCount() >= GUILD_DAY_INVITE_COUNT)
		{
			role->NoticeNum(errornum::EN_GUILD_DAY_INVITE_COUNT_LIMIT);
			return;
		}

		GuildMember *invite_member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL != invite_member) // 2：邀请方必须是该军团成员
		{
			if (GuildAuthority::CanInvite(invite_member)) // 3：邀请方必须有邀请权限
			{
				UserID beinvite_user_id(invite_req->beinvite_db_index, invite_req->beinvite_role_id);

				Role *target_user = World::GetInstWorld()->GetSceneManager()->GetRole(beinvite_user_id);
				if (NULL != target_user) // 4：被邀请方必须在线
				{
					if (target_user->GetGuildID() <= 0) // 5：被邀请方必须没有加入任何军团
					{
						// 						if (guild->GetCamp() == target_user->GetCamp()) // 6：两者必须在同一阵营
						// 						{
						if (target_user->GetLevel() >= LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().join_guild_level_limit) // 7：被邀请方必须达到加入军团的等级
						{
							Protocol::SCInviteGuild invite_guild;
							invite_guild.ret = 0;
							EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&invite_guild, sizeof(invite_guild));

							role->NoticeNum(noticenum::NT_GUILD_INVITE_SUCC);

							guild->GetGuildApplyForManager().AddInvite(beinvite_user_id);

							Protocol::SCInviteNotify invite_notify;			
							invite_notify.guild_id = invite_req->guild_id;
							invite_notify.invite_uid = UidToInt(user_id);
							role->GetName(invite_notify.invite_name);
							guild->GetGuildName(invite_notify.guild_name);
							EngineAdapter::Instance().NetSend(target_user->GetNetId(), (const char*)&invite_notify, sizeof(invite_notify));
						}
						else
						{
							role->NoticeNum(errornum::EN_GUILD_CAN_NOT_INVITE_TARGET_LEVEL_LIMIT);
						}
						// 						}
						// 						else
						// 						{
						// 							role->NoticeNum(errornum::EN_GUILD_CAN_NOT_INVITE_FOR_CAMP_NOT_SAME);
						// 						}
					}
					else
					{
						role->NoticeNum(errornum::EN_GUILD_OTHER_ALREADY_IN_GUILD);
					}
				}
				else
				{
					role->NoticeNum(errornum::EN_ROLE_NOT_EXIST);
				}
			}
			else
			{
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::InviteAck(Role *role, gglobalprotocal::InviteGuildAck *invite_ack, bool is_autojoin)
{
	if (NULL == role)	
	{
		return;
	}

	// 被邀请人role回应邀请

	UserID user_id = role->GetUserId();

	if (NULL == this->GetGuild(user_id)) // 1：没有加入军团
	{
		Guild *guild = this->GetGuild(invite_ack->guild_id); // 2：军团必须存在
		if (NULL != guild)
		{
			UserID invite_user(invite_ack->invite_db_index, invite_ack->invite_role_id);
			GuildMember *invite_member = guild->GetGuildMemberManager().GetMember(invite_user);
			if (NULL != invite_member) // 3：邀请方必须在军团中
			{
				if (is_autojoin || guild->GetGuildApplyForManager().RemoveInvite(user_id)) // 4：邀请必须存在 或者是 自动加入
				{
					if (0 == invite_ack->result)
					{
						GuildMember * beinvite_member = guild->GetGuildMemberManager().AddMember(user_id, GUILD_POST_CHENG_YUAN);
						if (NULL == beinvite_member) // 5：人满了
						{
							role->NoticeNum(errornum::EN_GUILD_ROLE_COUNT_LIMIT);
						}
						else
						{
							guild->GetGuildEventManager().OnMemberAdd(beinvite_member, invite_member);

							guild->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_MEMBER_ADD);

							guild->IsTianCiTongBiOpen(role);		// 如果活动开始 通知新加入成员 
						}
					}
					else
					{
						Role *invite_role = World::GetInstWorld()->GetSceneManager()->GetRole(invite_user);
						if (NULL != invite_role)
						{
							invite_role->NoticeNum(errornum::EN_HE_IS_REJUCT);
						}
					}
				}
				else 
				{
					role->NoticeNum(errornum::EN_GUILD_INVITE_INVALID);
				}
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_INVITE_INVALID);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
		}
	}
	else
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
	}
}

bool GuildManager::AutoJoinInviteAck(Role *role, gglobalprotocal::InviteGuildAck *invite_ack, bool is_autojoin)
{
	if (NULL == role)	
	{
		return false;
	}

	// 被邀请人role回应邀请

	UserID user_id = role->GetUserId();

	if (NULL == this->GetGuild(user_id)) // 1：没有加入军团
	{
		Guild *guild = this->GetGuild(invite_ack->guild_id); // 2：军团必须存在
		if (NULL != guild)
		{
			UserID invite_user(invite_ack->invite_db_index, invite_ack->invite_role_id);
			GuildMember *invite_member = guild->GetGuildMemberManager().GetMember(invite_user);
			if (NULL != invite_member) // 3：邀请方必须在军团中
			{
				if (is_autojoin || guild->GetGuildApplyForManager().RemoveInvite(user_id)) // 4：邀请必须存在 或者是 自动加入
				{
					// 					if (guild->GetCamp() == invite_ack->camp) // 5：必须在同一阵营
					// 					{
					if (0 == invite_ack->result)
					{
						GuildMember * beinvite_member = guild->GetGuildMemberManager().AddMember(user_id, GUILD_POST_CHENG_YUAN);
						if (NULL == beinvite_member) // 5：人满了
						{
							return false;
						}
						else
						{
							guild->GetGuildEventManager().OnMemberAdd(beinvite_member, invite_member);

							guild->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_MEMBER_ADD);

							guild->IsTianCiTongBiOpen(role); // 如果活动开始 通知新加入成员

							return true;
						}
					}
					// 					}
					// 					else
					// 					{
					// 						role->NoticeNum(errornum::EN_GUILD_NOT_IN_SAME_CAMP);
					// 					}
				}
			}
		}
	}

	return false;
}

void GuildManager::KickoutGuild(Role *role, gglobalprotocal::KickoutGuild *kick_req)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	if (!ActivityManager::Instance().IsActivityLimitGuildOper(GUILD_OPER_TYPE_KICK, role))
	{
		Guild *guild = this->GetGuild(kick_req->guild_id); // 1：军团必须存在
		if (NULL != guild) 
		{
			GuildMember *kicker_member = guild->GetGuildMemberManager().GetMember(user_id);
			if (NULL != kicker_member) // 2：踢人的家伙必须是该军团的
			{
				for (int i = 0; i < kick_req->bekicker_count && i < MAX_MEMBER_COUNT; ++i)
				{
					UserID bekicker_user(kick_req->bekicker_role_info_list[i].bekicker_db_index, kick_req->bekicker_role_info_list[i].bekicker_role_id);

					GuildMember *bekicker_member = guild->GetGuildMemberManager().GetMember(bekicker_user);
					if (NULL != bekicker_member) // 3：被踢的家伙必须是该军团的
					{
						if (!GuildAuthority::CanKickMember(kicker_member, bekicker_member)) // 4：必须有权限
						{
							role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
						}
						else
						{
							Protocol::SCKickoutGuild kickout_guild;
							kickout_guild.bekick_uid = UidToInt(bekicker_user);
							kickout_guild.ret = 0;
							EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&kickout_guild, sizeof(kickout_guild));

							guild->GetGuildEventManager().OnMemberKick(bekicker_member, kicker_member);

							guild->GetGuildMemberManager().RemoveMember(bekicker_user);

							guild->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_MEMBER_REMOVE);

							static MailContentParam contentparam; contentparam.Reset();
							UserMailManager::Instance().MailToUser(MAIL_TYPE_SYSTEM, bekicker_user, SYSTEM_MAIL_REASON_GUILD_BEKICKED, contentparam);
						}
					}
					else
					{
						role->NoticeNum(errornum::EN_GUILD_OTHER_NOT_IN_YOUR_GUILD);
					}
				}
			}
			else 
			{	
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
		}
	}
	else
	{
		if (ActivityManager::Instance().IsActivityOpen(ACTIVITY_TYPE_GUILD_QUESTION))
		{
			gstr::SendError(role, "guild_question_dismiss_activity_limit");
		}
	}
}

void GuildManager::Appoint(Role *role, gglobalprotocal::AppointGuild *appoint_req)
{
	if (NULL == role) return;

	if (!ActivityManager::Instance().IsActivityLimitGuildOper(GUILD_OPER_TYPE_APPOINT, role))
	{
		Guild *guild = this->GetGuild(appoint_req->guild_id); // 1：军团必须存在
		if (NULL != guild) 
		{
			GuildMember *appoint_member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
			if (NULL != appoint_member) // 2：任命的家伙必须是该军团的
			{
				UserID beappoint_user(appoint_req->beappoint_db_index, appoint_req->beappoint_role_id);

				GuildMember *beappoint_member = guild->GetGuildMemberManager().GetMember(beappoint_user);
				if (NULL != beappoint_member) // 3：被任命的家伙必须是该军团的
				{
					if (GuildAuthority::CanAppoint(appoint_member, beappoint_member, appoint_req->post)) // 4：必须有权限 
					{
						UserCacheNode *beappoint_uc = UserCacheManager::Instance().GetUserNode(UidToInt(beappoint_user));
						if (NULL == beappoint_uc)
						{
							role->NoticeNum(errornum::EN_ROLE_NOT_IN_CACHE);
							return;
						}

						GameName tuanzhang_name; memset(tuanzhang_name, 0, sizeof(tuanzhang_name));
						guild->GetGuildRegister().GetGuildTuanZhangInfo(tuanzhang_name);

						int errorno = guild->GetGuildMemberManager().Appoint(appoint_member, beappoint_member, appoint_req->post);
						if (0 == errorno)
						{
							Protocol::SCAppointGuild appoint_guild;
							appoint_guild.guild_id = appoint_req->guild_id;
							appoint_guild.beappoint_uid = UidToInt(beappoint_user);
							appoint_guild.post = appoint_req->post;
							appoint_guild.ret = 0;
							EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&appoint_guild, sizeof(appoint_guild));

							if (GUILD_POST_TUANGZHANG == appoint_req->post)
							{
								guild->GetGuildEventManager().OnTransferTuanzhang(appoint_member, beappoint_member);
								guild->OnBaseInfoChange();

								gamelog::g_log_guild.printf(LL_INFO, "guild->appoint guild_id[%d], guild_name[%s], old_tuanzhang[%d, %s], new_tuanzhang[%d, %s]",
									guild->GetGuildID(), guild->GetGuildName(), UidToInt(appoint_member->GetUserID()), tuanzhang_name, role->GetUID(), role->GetName());
							}
							else
							{
								guild->GetGuildEventManager().OnMemberAppoint(beappoint_member, appoint_member);
							}

							static MailSubject mail_subject = {0}; memset(mail_subject, 0, sizeof(mail_subject));
							static MailContentTxt contenttxt = {0}; memset(contenttxt, 0, sizeof(contenttxt));

							switch (appoint_req->post)
							{
							case GUILD_POST_TUANGZHANG:
								{
									int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_guild_tuanzhang_appointed_subject);
									int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_guild_tuanzhang_appointed_content, 
										role->GetName(), beappoint_uc->user_name);
									if (length1 > 0 && length2 > 0)
									{
										guild->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
									}
								}
								break;

							case GUILD_POST_FU_TUANGZHANG:
								{
									int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_msg_appointed_subject);
									int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_msg_futuanzhang_be_appointed, 
										appoint_req->beappoint_role_id, beappoint_uc->user_name, guild->GetCamp());
									if (length1 > 0 && length2 > 0)
									{
										guild->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
									}
								}
								break;

							case GUILD_POST_ZHANG_LAO:
								{
// 									int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_msg_appointed_subject);
// 									int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_msg_zhanglao_be_appointed,
// 										appoint_req->beappoint_role_id, beappoint_uc->user_name, guild->GetCamp());
// 									if (length1 > 0 && length2 > 0)
// 									{
// 										guild->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
// 									}
								}
								break;

							case GUILD_POST_JINGYING:
								{
// 									//任命精英去掉邮件通知
// 									int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_msg_appointed_subject);
// 									int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_msg_jingying_be_appointed, 
// 										appoint_req->beappoint_role_id, beappoint_uc->user_name, guild->GetCamp());
// 									if (length1 > 0 && length2 > 0)
// 									{
// 										guild->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
// 									}
								}
								break;

							case GUILD_POST_HUFA:
								{
// 									int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_msg_appointed_subject);
// 									int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_msg_hufa_be_appointed, 
// 										appoint_req->beappoint_role_id, beappoint_uc->user_name, guild->GetCamp());
// 									if (length1 > 0 && length2 > 0)
// 									{
// 										guild->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
// 									}
								}
								break;
							}

							gglobalprotocal::GetMemberList gml;			// 更新成员列表信息 给该玩家
							gml.guild_id = appoint_req->guild_id;
							this->GetMemberList(role, &gml, true);
						}
						else 
						{
							role->NoticeNum(errorno);
						}
					}
					else
					{
						role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
					}
				}
				else 
				{
					role->NoticeNum(errornum::EN_GUILD_OTHER_NOT_IN_YOUR_GUILD);
				}
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
		}
	}
	else
	{

	}
}

void GuildManager::LeavePost(Role *role, gglobalprotocal::LeavePostGuild *leavepost_req)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(leavepost_req->guild_id); // 1：必须有该军团
	if (NULL != guild) 
	{
		GuildMember *leavepost_member = guild->GetGuildMemberManager().GetMember(user_id); // 2：必须是该军团成员
		if (NULL != leavepost_member) 
		{
			if (GuildAuthority::CanLeavePost(leavepost_member)) // 3：必须有权限 
			{
				guild->GetGuildEventManager().OnMemberLeavePost(leavepost_member);

				guild->GetGuildMemberManager().LeavePost(leavepost_member);

				Protocol::SCLeavePost lp;
				lp.guild_id = leavepost_req->guild_id;
				lp.ret = 0;
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&lp, sizeof(lp));

				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_leave_post, 
					role->GetUID(), role->GetName(), guild->GetCamp());
				if (length > 0)
				{
					guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
				}

				gglobalprotocal::GetMemberList gml;			// 立即更新成员列表信息 给该玩家
				gml.guild_id = leavepost_req->guild_id;
				this->GetMemberList(role, &gml, true);
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_CANNOT_LEAVE_POST);
			}
		}
	}
}

void GuildManager::MailAll(Role *role, gglobalprotocal::GuildMailAll *mail_all_req)
{
	if (NULL == role)
	{
		return;
	}

	Guild *guild = this->GetGuild(mail_all_req->guild_id);
	if (NULL != guild) // 1：军团必须存在
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
		if (NULL != member) // 2：必须是该军团成员
		{
			if (GuildAuthority::CanMailAll(member)) // 3：必须是有权限
			{
				unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				if (now_second >= member->GetLastMailAllTime() + GUILD_MAIL_ALL_CD_TIME)
				{
					member->SetLastMailAllTime(now_second);

					Protocol::SCGuildMailAll mail_all;
					mail_all.guild_id = mail_all_req->guild_id;
					mail_all.ret = 0;
					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&mail_all, sizeof(mail_all));

					guild->GetGuildMemberManager().SendMailToAll(role, mail_all_req);
				}
				else
				{
					role->NoticeNum(errornum::EN_GUILD_OPERATOR_TOO_FAST);
				}
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::ChangeNotice(Role *role, gglobalprotocal::GuildChangeNotice *chg_notice_req)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(chg_notice_req->guild_id);
	if (NULL != guild) // 1：军团必须存在
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL != member) // 2：必须是该军团成员
		{
			if (GuildAuthority::CanChangeNotice(member)) // 3：必须是有权限
			{
				unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

				if (now_second >= member->GetLastChangeNoticeTime() + GUILD_CHANGE_NOTICE_CD_TIME)
				{
					member->SetLastChangeNoticeTime(now_second);

					chg_notice_req->notice[sizeof(chg_notice_req->notice) - 1] = 0; // 字符串一定要补0

					guild->ChangeNotice(chg_notice_req->notice);

					Protocol::SCChangeNotice set_notice;
					set_notice.guild_id = chg_notice_req->guild_id;
					set_notice.ret = 0;
					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&set_notice, sizeof(set_notice));

					ROLE_LOG_QUICK6(LOG_TYPE_CHAT_RECORD, role, LOG_CHAT_TYPE_GUILD_NOTICE, chg_notice_req->guild_id, chg_notice_req->notice, NULL);
				}
				else
				{
					role->NoticeNum(errornum::EN_GUILD_OPERATOR_TOO_FAST);
				}
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::SetAutoKickoutSetup(Role *role, gglobalprotocal::GuildSetAutoKickoutSetup *gsaks)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(gsaks->guild_id);
	if (NULL != guild) // 1：军团必须存在
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL != member) // 2：必须是该军团成员
		{
			if (GuildAuthority::CanSetup(member)) // 3：必须是有权限
			{
				guild->SetAutoKickoutSetup(gsaks->auto_kickout_setup);
				role->NoticeNum(noticenum::NT_GUILD_SET_AUTO_KICKOUT_SETUP_SUCC);
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::Delate(Role *role, gglobalprotocal::GuildDelate *gd)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(gd->guild_id);
	if (NULL != guild) // 1：军团必须存在
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL != member) // 2：必须是该军团成员
		{
			if (GuildAuthority::CanDelate(member)) // 3：必须是有权限
			{

				ItemID GUILD_DELATE_ITEM_ID = static_cast<ItemID>(LOGIC_CONFIG->GetGuildConfig().GetGuildDelateItemId());

				ItemGridData item_data;
				if (!role->GetKnapsack()->GetItemGridData(gd->knapsack_index, &item_data) || GUILD_DELATE_ITEM_ID != item_data.item_id)
				{
					role->NoticeNum(errornum::EN_GUILD_DELATE_NO_ITEM);
					return;
				}

				if (!role->GetKnapsack()->ConsumeItemByIndex(gd->knapsack_index, 1, "guild_delate"))
				{
					role->NoticeNum(errornum::EN_GUILD_DELATE_NO_ITEM);
					return;
				}

				GameName tuanzhang_name; memset(tuanzhang_name, 0, sizeof(tuanzhang_name));
				GuildMember *tuanzhang_member = guild->GetGuildMemberManager().GetMember(guild->GetGuildRegister().GetGuildTuanZhangInfo(tuanzhang_name));
				if (NULL != tuanzhang_member)
				{
					long long off_line_time_s = EngineAdapter::Instance().Time() - tuanzhang_member->GetLastLoginTime();
					if (!tuanzhang_member->IsOnline() && off_line_time_s >= 3 * 24 * 3600)
					{
						guild->GetGuildEventManager().OnDelateTuanzhang(member);

						tuanzhang_member->BeAppoint(GUILD_POST_CHENG_YUAN);
						member->BeAppoint(GUILD_POST_TUANGZHANG);

						static MailSubject mail_subject = {0};
						static MailContentTxt contenttxt = {0};
						int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_guild_tuanzhang_delated_subject);
						int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_guild_tuanzhang_delated_content,
							tuanzhang_name, role->GetName());
						if (length1 > 0 && length2 > 0)
						{
							guild->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);

							static Protocol::SCSystemMsg sm;
							int sendlen = 0;
							if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &sendlen, contenttxt, length2, SYS_MSG_ONLY_CHAT_GUILD))
							{
								guild->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_BOSS_CALL, (const char *)&sm, sendlen);	
							}
						}

						gamelog::g_log_guild.printf(LL_INFO, "guild->delate ret succ, guild_id[%d], guild_name[%s], new_tuanzhang[%d, %s], old_tuanzhang[%d, %s] off_line_time[%lld]", 
							guild->GetGuildID(), guild->GetGuildName(), UidToInt(tuanzhang_member->GetUserID()), tuanzhang_name, role->GetUID(), role->GetName(), off_line_time_s);
					}
					else 
					{
						role->NoticeNum(errornum::EN_GUILD_DELATE_FAIL);
					}
				}
				else
				{
					role->NoticeNum(errornum::EN_GUILD_OTHER_NOT_IN_YOUR_GUILD);
				}
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::OnCheckCanDelate(Role *role, Protocol::CSGuildCheckCanDelate *gccd)
{
	if (NULL == role)
	{
		return;
	}

	Protocol::SCGuildCheckCanDelateAck gccda;
	gccda.can_delate = 0;

	Guild *guild = this->GetGuild(gccd->guild_id);
	if (NULL != guild) // 1：军团必须存在
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
		if (NULL != member) // 2：必须是该军团成员
		{
			if (GuildAuthority::CanDelate(member)) // 3：必须是有权限
			{
				GameName tuanzhang_name; memset(tuanzhang_name, 0, sizeof(tuanzhang_name));
				GuildMember *tuanzhang_member = guild->GetGuildMemberManager().GetMember(guild->GetGuildRegister().GetGuildTuanZhangInfo(tuanzhang_name));
				if (NULL != tuanzhang_member)
				{
					if (!tuanzhang_member->IsOnline() && EngineAdapter::Instance().Time() - tuanzhang_member->GetLastLoginTime() >= 3 * 24 * 3600)
					{
						gccda.can_delate = 1;
					}
				}
			}
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gccda, sizeof(gccda));
}

void GuildManager::OnGuildAddGuildExp(Role *role, gglobalprotocal::GuildAddExp *gae)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(gae->guild_id);
	if (NULL != guild)
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL != member) // 2：必须是该军团成员
		{
			guild->AddGuildExp(gae->add_exp);

// 			if (gae->add_exp >= 10)
// 			{
// 				int length = 0;
// 				if (GUILD_EVENT_ADD_EXP_TYPE_ITEM == gae->use_type || GUILD_EVENT_ADD_EXP_TYPE_GOLD == gae->use_type)
// 				{
// 					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_juanxian_byitem, 
// 						UidToInt(user_id), role->GetName(), guild->GetCamp(), gae->add_exp);
// 				}
// 
// 				if (length > 0)
// 				{
// 					guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
// 				}
// 			}

			guild->GetGuildEventManager().OnMemberAddExp(member, gae->use_type, gae->value);

			this->SyncGuildBaseInfo(role);
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
}

void GuildManager::OnGuildMemberSos(Role *role, gglobalprotocal::GuildMemberSos *gms)
{
	if (NULL == role)
	{
		return;
	}

	Guild *guild = this->GetGuild(gms->guild_id);
	if (NULL != guild)
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
		if (NULL != member)
		{
			guild->OnMemberSos(role, gms);
		}
	}
}

bool GuildManager::OnGuildResetName(Role *role, gglobalprotocal::GuildResetName *grn)
{
	if (NULL == role || NULL == grn)
	{
		return false;
	}

	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(grn->guild_id);
	if (NULL != guild)
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL == member)
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
			return false;
		}

		if (0 == grn->new_name[0]) return false;

		grn->new_name[sizeof(grn->new_name) - 1] = 0;
		InvalidCharFilte(grn->new_name);
		GuildNameCatPostfix(grn->new_name, grn->db_index);

		bool repeat_name = false;
		for (GuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); it != end; ++it)
		{
			if (0 == strncmp(it->second->GetGuildName(), grn->new_name, sizeof(GuildName)))
			{
				repeat_name = true;
				break;
			}
		}

		if (!repeat_name)
		{
			static const ItemID XIANMENG_CHANGE_NAME_CARD_ID = 26922;

			// 检查消耗
			static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
			ItemExtern::ItemConsumeConfig comsume_stuff;
			comsume_stuff.item_id = XIANMENG_CHANGE_NAME_CARD_ID;
			comsume_stuff.num = 1;
			comsume_stuff.buyable = false;

			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, &comsume_stuff, &consume_list))
			{
				role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				return false;
			}

			// 消耗
			if (!ItemExtern::ConsumeItem(role, consume_list, "OnGuildResetName"))
			{
				return false;
			}

			// 生效
			static Protocol::SCGuildResetName scgrn;
			scgrn.guild_id = guild->GetGuildID();
			guild->GetGuildName(scgrn.old_name);
			guild->OnResetName(grn->new_name);
			guild->GetGuildName(scgrn.new_name);
			guild->GetGuildMemberManager().OnRefreshGuildAppearanceInfo();
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scgrn, sizeof(scgrn));

			role->NoticeNum(noticenum::NT_GUILD_RESET_NAME_SUCC);

			return true;
		}
		else
		{
			role->NoticeNum(errornum::EN_GUILD_INVILD_NAME);
		}
	}

	return false;
}

bool GuildManager::CanGuildResetName(Role *role)
{
	if (NULL == role)
	{
		return false;
	}

	if (!ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_GONGCHENGZHAN))
	{
		role->NoticeNum(errornum::EN_GUILD_GCZ_LIMIT);
		return false;
	}

	if (!ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_GUILDBATTLE))
	{
		role->NoticeNum(errornum::EN_GUILD_BATTLE_LIMIT);
		return false;
	}

	if (!ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_GUILD_FB))
	{
		gstr::SendError(role, "guild_fb_limit");
		return false;
	}

	if (!ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_GUILD_QUESTION))
	{
		gstr::SendError(role, "guild_question_limit");
		return false;
	}

	if (!ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_LIEKUN_FB))
	{
		role->NoticeNum(errornum::EN_GUILD_LIEKUN_ZHUMO_LIMIT);
		return false;
	}

	if (!ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE))
	{
		role->NoticeNum(errornum::EN_CROSS_GUILD_BATTLE_LIMIT);
		return false;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (CrossGuildBattleConfig::Instance().IsZhuMoTime(now_second))
	{
		role->NoticeNum(errornum::EN_GUILD_LIEKUN_ZHUMO_LIMIT);
		return false;
	}

	return true;
}

void GuildManager::OnGuildApplyforSetup(Role *role, gglobalprotocal::GuildApplyforSetup *afs)
{
	if (NULL == role)
	{
		return;
	}

	UserID user_id = role->GetUserId();

	Guild *guild = this->GetGuild(afs->guild_id);
	if (NULL != guild) // 1：军团必须存在
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL != member) // 2：必须是该军团成员
		{
			if (GuildAuthority::CanSetup(member)) // 3：必须是有权限
			{
				if (guild->SetApplyforSetup(afs->applyfor_setup, afs->need_capability, afs->need_level))
				{
					Protocol::SCGuildOperaSucc gos;
					gos.opera_type = Protocol::SCGuildOperaSucc::OPERA_TYPE_APPLY_SET;
					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&gos, sizeof(gos));

					role->NoticeNum(noticenum::NT_GUILD_APPLYFOR_SETUP_SAVE_SUCC);

					// 如果设置为条件满足自动通过，就把已经申请且满足条件的成员自动加入仙盟，其他干掉
					if (GUILD_APPLYFOR_CONDITION == afs->applyfor_setup)
					{
						GuildMember *verify_member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
						if (NULL != verify_member && GuildAuthority::CanVerify(verify_member))
						{
							UserID apply_for_userid_list[MAX_APPLY_FOR_COUNT];
							int apply_for_count = guild->GetGuildApplyForManager().GetApplyForList(apply_for_userid_list);

							for (int i = 0; i < apply_for_count && i < MAX_APPLY_FOR_COUNT; ++i)
							{
								UserID apply_user_id = apply_for_userid_list[i];

								GuildApplyForManager::ApplyFor *apply_record = guild->GetGuildApplyForManager().GetApplayFor(apply_user_id); 
								if (NULL != apply_record)									// 必须是有申请过
								{
									bool can_join = false;
									{
										if (NULL == this->GetGuild(apply_user_id))			// 对方不在其他军团
										{
											Role *apply_role = World::GetInstWorld()->GetSceneManager()->GetRole(apply_user_id);
											if (NULL != apply_role)
											{
												if (apply_role->GetLevel() >= afs->need_level && apply_role->GetCapability()->GetTotalCapability() >= afs->need_capability)
												{
													can_join = true;
												}
											}
											else
											{
												UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(apply_user_id);
												if (NULL != uc && uc->level >= afs->need_level && uc->capability >= afs->need_capability)
												{
													can_join = true;
												}
											}
										}
									}

									int join_result = 0;

									if (can_join)
									{
										GuildMember *applyfor_member = guild->GetGuildMemberManager().AddMember(apply_user_id, GUILD_POST_CHENG_YUAN);
										if (NULL == applyfor_member)						// 人满了 
										{
											World::GetInstWorld()->GetSceneManager()->NoticeToUser(apply_user_id, errornum::EN_GUILD_ROLE_COUNT_LIMIT);
										}
										else
										{
											join_result = 1;
											guild->GetGuildEventManager().OnMemberAdd(applyfor_member, verify_member);

											guild->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_MEMBER_ADD);

											guild->IsTianCiTongBiOpen(role); // 如果活动开始 通知新加入成员
										}
									}

									{
										static Protocol::SCApplyForJoinGuildAck afjga;
										afjga.result = join_result;
										afjga.guild_id = guild->GetGuildID();
										guild->GetGuildName(afjga.guild_name);
										World::GetInstWorld()->GetSceneManager()->SendToUser(apply_user_id, (const char *)&afjga, sizeof(afjga));
									}
								}

								guild->GetGuildApplyForManager().RemoveApplyFor(apply_user_id);
								this->OnUserRemoveApplyFor(apply_user_id, guild->GetGuildID());
							}

							if (apply_for_count > 0)
							{
								gglobalprotocal::GetApplyForList apply_info_req;
								apply_info_req.guild_id = guild->GetGuildID();
								apply_info_req.db_index = role->GetUserId().db_index;
								apply_info_req.role_id = role->GetUserId().role_id;

								GuildManager::Instance().GetApplyForList(role, &apply_info_req);
							}
						}
					}
				}
				else 
				{
					role->NoticeNum(errornum::EN_GUILD_APPLYFOR_SETUP_ERROR);
				}
			}
			else 
			{
				role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
			}
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::GetAllGuildBaseInfo(Role *role, gglobalprotocal::AllGuildBaseInfo *all_guild_req)
{
	if (NULL == role) return;

	static Protocol::SCAllGuildBaseInfo agbi;
	agbi.free_create_guild_times = WorldStatus::Instance().GetFreeCreateGuildTimes();
	agbi.is_first = 1;

	if (m_guild_map.empty())
	{
		agbi.count = 0;

		int sendlen = sizeof(agbi) - (MAX_ONCE_GUILD_COUNT - agbi.count) * sizeof(Protocol::SCAllGuildBaseInfo::GuildBaseInfo);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&agbi, sendlen);
	}
	else
	{
		std::set<GuildID> *apply_guild_id_set = NULL;
		UserApplyForMapIt it_apply = m_user_applyfor_mgr.find(role->GetUserId());
		if (m_user_applyfor_mgr.end() != it_apply)
		{
			apply_guild_id_set = &(it_apply->second);
		}

		int count = 0;

		for (GuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); it != end; ++it)
		{
			Guild *guild = it->second;

			agbi.guild_list[count].guild_id = guild->GetGuildID();
			guild->GetGuildName(agbi.guild_list[count].guild_name);
			UserID tuanzhang_user_id = guild->GetGuildRegister().GetGuildTuanZhangInfo(agbi.guild_list[count].tuanzhang_name);
			agbi.guild_list[count].tuanzhang_uid = UidToInt(tuanzhang_user_id);
			agbi.guild_list[count].create_time = static_cast<unsigned int>(guild->GetGuildCreateTime());
			agbi.guild_list[count].guild_level = guild->GetGuildLevel();
			agbi.guild_list[count].cur_member_count = guild->GetGuildRegister().GetGuildMemberCount();
			agbi.guild_list[count].max_member_count = guild->GetGuildMaxMemberCount();
			agbi.guild_list[count].camp = guild->GetCamp();
			agbi.guild_list[count].vip_type = guild->GetGuildRegister().GetGuildTuanzhangVipLevel();
			agbi.guild_list[count].applyfor_setup = static_cast<short>(guild->GetApplyforSetup());
			agbi.guild_list[count].applyfor_need_capability = guild->GetApplyforNeedCapability();
			agbi.guild_list[count].applyfor_need_level = guild->GetApplyforNeedLevel();
			agbi.guild_list[count].active_degree = guild->GetActiveDegree();
			agbi.guild_list[count].total_capability = guild->GetGuildMaxCapability();
			agbi.guild_list[count].is_apply = 0;

			if (NULL != apply_guild_id_set)
			{
				std::set<GuildID>::iterator it_set = apply_guild_id_set->find(guild->GetGuildID());
				if (apply_guild_id_set->end() != it_set)
				{
					agbi.guild_list[count].is_apply = 1;
				}
			}

			++count;
			if (count >= MAX_ONCE_GUILD_COUNT)
			{
				//// 为方便客户端，进行排序
				//std::stable_sort(std::begin(agbi.guild_list), std::end(agbi.guild_list), [](const Protocol::SCAllGuildBaseInfo::GuildBaseInfo &cmp_item1, const Protocol::SCAllGuildBaseInfo::GuildBaseInfo &cmp_item2) {
				//	return cmp_item1.total_capability > cmp_item2.total_capability; });

				agbi.count = count;

				int sendlen = sizeof(agbi) - (MAX_ONCE_GUILD_COUNT - count) * sizeof(Protocol::SCAllGuildBaseInfo::GuildBaseInfo);
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&agbi, sendlen);

				agbi.is_first = 0;

				count = 0;
			}
		}

		//// count 必须大于等于0 小于等于static_array_size(agbi.guild_lis)
		//// 为方便客户端，进行排序
		//std::stable_sort(agbi.guild_list, agbi.guild_list + count, [](const Protocol::SCAllGuildBaseInfo::GuildBaseInfo &cmp_item1, const Protocol::SCAllGuildBaseInfo::GuildBaseInfo &cmp_item2) {
		//	return cmp_item1.total_capability > cmp_item2.total_capability; });

		if (count >= 0)
		{
			agbi.count = count;

			int sendlen = sizeof(agbi) - (MAX_ONCE_GUILD_COUNT - count) * sizeof(Protocol::SCAllGuildBaseInfo::GuildBaseInfo);
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&agbi, sendlen);
		}
	}
}

void GuildManager::GetApplyForList(Role *role, gglobalprotocal::GetApplyForList *apply_info_req)
{
	if (NULL == role) return;

	Guild *guild = this->GetGuild(apply_info_req->guild_id);
	if (NULL != guild) // 1：军团必须存在
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
		if (NULL != member) // 2：必须是该军团成员
		{
			static Protocol::SCGuildGetApplyForList ggaf;
			guild->GetGuildApplyForManager().GetApplyForList(&ggaf);

			int sendlen = sizeof(ggaf) - (MAX_APPLY_FOR_COUNT - ggaf.count) * sizeof(Protocol::SCGuildGetApplyForList::ApplyFor);
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ggaf, sendlen);
		}
		else
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::GetApplyForJoinGuildList(Role *role)
{
	if (NULL == role) return;

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		static Protocol::SCApplyForJoinGuildList afjgl;
		afjgl.count = 0;

		UserApplyForMapIt it = m_user_applyfor_mgr.find(role->GetUserId());
		if (it != m_user_applyfor_mgr.end())
		{
			for (std::set<GuildID>::iterator it_set = it->second.begin(), it_set_end = it->second.end(); it_set != it_set_end; ++ it_set)
			{
				if (afjgl.count >= Protocol::SCApplyForJoinGuildList::MAX_APPLY_FOR_JOIN_GUILD_COUNT) break;

				afjgl.applyfor_join_guild_list[afjgl.count] = *it_set;
				++ afjgl.count;
			}
		}

		int send_len = sizeof(afjgl) - (Protocol::SCApplyForJoinGuildList::MAX_APPLY_FOR_JOIN_GUILD_COUNT - afjgl.count) * sizeof(afjgl.applyfor_join_guild_list[0]);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&afjgl, send_len);
	}
	else
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
	}
}

void GuildManager::GetMemberList(Role *role, gglobalprotocal::GetMemberList *memberlist_info_req, bool force_get)
{
	if (NULL == role) return;

	Guild *guild = this->GetGuild(memberlist_info_req->guild_id);
	if (NULL != guild) // 1：军团必须存在
	{
		static Protocol::SCGuildMemberList gml;
		gml.guild_id = guild->GetGuildID();
		guild->GetGuildMemberManager().GetMemberList(&gml);

		int sendlen = sizeof(gml) - (MAX_MEMBER_COUNT - gml.count) * sizeof(Protocol::SCGuildMemberList::_Member);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gml, sendlen);
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::GetGuildBaseInfo(Role *role, gglobalprotocal::GetGuildBaselInfo *base_info_req)
{
	if (NULL == role) return;

	Guild *guild = this->GetGuild(base_info_req->guild_id);
	if (NULL != guild) // 1：军团必须存在 
	{
		static Protocol::SCGuildBaseInfo gi;

		gi.guild_id = guild->GetGuildID();
		guild->GetGuildName(gi.guild_name);
		gi.guild_level = guild->GetGuildLevel();
		gi.guild_exp = guild->GetGuildExp(&gi.guild_max_exp);
		gi.totem_level = guild->GetTotemLevel();
		gi.totem_exp = guild->GetTotemExp();
		gi.cur_member_count = guild->GetGuildRegister().GetGuildMemberCount();
		gi.max_member_count = guild->GetGuildMaxMemberCount();
		UserID tuanzhang_user_id = guild->GetGuildRegister().GetGuildTuanZhangInfo(gi.tuanzhang_name);	
		gi.tuanzhang_uid = UidToInt(tuanzhang_user_id);
		gi.create_time = static_cast<unsigned int>(guild->GetGuildCreateTime());
		gi.camp = guild->GetCamp();
		gi.vip_level = guild->GetGuildRegister().GetGuildTuanzhangVipLevel();
		gi.applyfor_setup = guild->GetApplyforSetup();
		guild->GetNotice(gi.guild_notice);
		gi.auto_kickout_setup = guild->GetAutoKickoutSetup();
		gi.applyfor_need_capability = guild->GetApplyforNeedCapability();
		gi.applyfor_need_level = guild->GetApplyforNeedLevel();
		gi.callin_times = guild->GetCallinTimes();
		gi.active_degree = guild->GetActiveDegree();
		gi.total_capability = guild->GetGuildMaxCapability();
		gi.rank = guild->GetRank();
		gi.totem_exp_today = guild->GetTotemExpToday();
		gi.is_auto_clear = guild->GetIsAutoClear();
		gi.is_today_biaoche_start = (guild->IsBiaoCheHadEnd() ? 1 : 0);
		gi.avatar_timestamp = guild->GetGuildAvatar();
		gi.guild_total_gongzi = guild->GetGuildGongzi();
		gi.avatar_changed = 1;
		if (guild->GetGuildAvatar() == 0 && role->GetCommonDataParam()->avatar_changed == 0)
		{
			gi.avatar_changed = 0;
		}

		GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
		if (NULL != member)
		{
			gi.my_lucky_color = member->GetLuckyColor();
		}

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gi, sizeof(gi));
	}
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
	}
}

void GuildManager::GetGuildEventList(Role *role, gglobalprotocal::GetGuildEventList *eventlist_info_req)
{
	if (NULL == role) return;

	Guild *guild = this->GetGuild(eventlist_info_req->guild_id);
	if (NULL != guild) // 1：军团必须存在
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
		if (NULL != member) // 2：必须是该军团成员
		{
			static Protocol::SCGuildEventList gel;
			guild->GetGuildEventManager().GetEventList(&gel);

			int sendlen = sizeof(gel) - (MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT - gel.count) * sizeof(gel.event_for_list[0]);
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gel, sendlen);
		}
		else 
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			role->NoticeNum(errornum::EN_GUILD_NO_EXIST);
		}
	}
}

void GuildManager::GetGuildInviteList(Role *role, gglobalprotocal::GetGuildInviteList *invitelist_info_req)
{
	if (NULL == role) return;

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		static Protocol::SCInviteGuildList igl;
		igl.count = 0;

		for (GuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); it != end; ++it)
		{
			Guild *guild = it->second;
			if (guild->GetGuildApplyForManager().IsbeInvite(role->GetUserId())) // 如果该军团有邀请我 
			{
				igl.invite_guild_list[igl.count].guild_id = guild->GetGuildID();
				guild->GetGuildName(igl.invite_guild_list[igl.count].guild_name);
				igl.invite_guild_list[igl.count].guild_level = guild->GetGuildLevel();
				igl.invite_guild_list[igl.count].member_count = guild->GetGuildRegister().GetGuildMemberCount();

				{
					GameName tuanzhang_name = {0};
					UserID tuanzhang_userid = guild->GetGuildRegister().GetGuildTuanZhangInfo(tuanzhang_name);
					igl.invite_guild_list[igl.count].invite_uid = UidToInt(tuanzhang_userid);
				}

				++ igl.count;
				if (igl.count >= Protocol::SCInviteGuildList::MAX_INVITE_GUILD_COUNT) break;
			}
		}

		int sendlen = sizeof(igl) - (Protocol::SCInviteGuildList::MAX_INVITE_GUILD_COUNT - igl.count) * sizeof(igl.invite_guild_list[0]);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&igl, sendlen);
	}
	else
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_ALREADY_IN_GUILD);
	}
}

void GuildManager::OnUserAddApplyFor(const UserID &user_id, GuildID guild_id)
{
	UserApplyForMapIt it = m_user_applyfor_mgr.find(user_id);
	if (it != m_user_applyfor_mgr.end())
	{
		it->second.insert(guild_id);
	}
	else 
	{
		std::set<GuildID> temp_set;
		temp_set.insert(guild_id);

		m_user_applyfor_mgr[user_id] = temp_set;
	}
}

void GuildManager::OnUserRemoveApplyFor(const UserID &user_id, GuildID guild_id)
{
	UserApplyForMapIt it = m_user_applyfor_mgr.find(user_id);
	if (it != m_user_applyfor_mgr.end())
	{
		it->second.erase(guild_id);

		if (it->second.empty())
		{
			m_user_applyfor_mgr.erase(it);
		}
	}
}

void GuildManager::OnUserJoinGuildRemoveApplyFor(const UserID &user_id, GuildID guild_id)
{
	UserApplyForMapIt it = m_user_applyfor_mgr.find(user_id);
	if (it != m_user_applyfor_mgr.end())
	{
		for (std::set<GuildID>::iterator it_set = it->second.begin(), it_set_end = it->second.end(); it_set != it_set_end; ++ it_set)
		{
			Guild *guild = this->GetGuild(*it_set);
			if (NULL != guild)
			{
				guild->GetGuildApplyForManager().RemoveApplyFor(user_id);
			}
		}

		m_user_applyfor_mgr.erase(it);
	}
}

Guild * GuildManager::GetGuild(GuildID guild_id)
{
	GuildMapIt it = m_guild_map.find(guild_id);
	if (it == m_guild_map.end()) return NULL;

	return it->second;
}

Guild * GuildManager::GetGuild(const UserID &user_id)
{
	for (GuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); it != end; ++it)
	{
		Guild *guild = it->second;
		if (NULL != guild->GetGuildMemberManager().GetMember(user_id))
		{
			return guild;
		}
	}

	return NULL;
}

Guild * GuildManager::AddGuild(const AllGuildParam::GuildParam &guild_param, bool is_create)
{
	GuildMapIt it = m_guild_map.find(guild_param.base_param.guild_id);
	if (it != m_guild_map.end()) 
	{
		printf("GuildManager::AddGuild  Failed\n");
		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[GuildManager::AddGuild GUILD REPEAT] [guild_id:%d]", (int)guild_param.base_param.guild_id);
		return NULL;
	}

	Guild *guild = new Guild(this);
	guild->Init(guild_param);

	m_guild_map[guild_param.base_param.guild_id] = guild;

	this->OnAddGuild(guild, is_create);

	return guild;
}

void GuildManager::RemoveGuild(GuildID guild_id)
{
	GuildMapIt it = m_guild_map.find(guild_id);
	if (it == m_guild_map.end()) return;

	Guild *guild = it->second;

	guild->GetGuildMemberManager().Dismiss();
	this->OnRemoveGuild(it->second);

	Role * tuanzhang = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(guild->GetTuanzhuangRoleId()));

	delete it->second;
	m_guild_map.erase(it);

	if (tuanzhang)
	{
		//获取公会列表, 只有会长可以操作解散,解散后会长的加入仙盟列表包含已经解散的仙盟。
		Protocol::CSGetGuildInfo agi;
		agi.guild_info_type = GUILD_INFO_TYPE_ALL_GUILD_BASE_INFO;
		GuildRoute::Instance().GetGuildInfoCS(tuanzhang, &agi);
	}

	if (WorldStatus::Instance().GetGCZWinGuild() == guild_id)  //解散仙盟时，攻城胜方也取消
	{
		WorldStatus::Instance().OnSyncGCZInfo(INVALID_GUILD_ID, 0);
	}

	if (WorldStatus::Instance().GetKillCrossBossRankGuild() == guild_id)
	{
		WorldStatus::Instance().SyncKillCrossBossRankGuild(0, INVALID_GUILD_ID);
	}
}

int GuildManager::GetGuildCamp(GuildID guild_id)
{
	Guild *guild = this->GetGuild(guild_id);
	if (NULL != guild)
	{
		return guild->GetCamp();
	}

	return CAMP_TYPE_INVALID;
}

void GuildManager::UserResetName(const UserID &user_id, GameName name)
{
	Guild *guild = this->GetGuild(user_id);
	if (NULL != guild)
	{
		if (guild->GetGuildRegister().IsTuanZhang(user_id))
		{
			guild->OnBaseInfoChange();
		}
	}
}

long long GuildManager::GetCampCapability(int camp, int level_limit, int login_time_limit)
{
	long long camp_capability = 0;

	for (GuildMapIt it = m_guild_map.begin(); it != m_guild_map.end(); ++it)
	{
		Guild *guild = it->second;
		if (NULL != guild && guild->GetCamp() == camp)
		{
			camp_capability += guild->GetGuildMemberManager().GetCapability(level_limit, login_time_limit);
		}
	}

	return camp_capability;
}

void GuildManager::SyncToRole(const UserID &user_id, GuildID guild_id, const GuildName guild_name, int post)
{
	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
	if (NULL != role)
	{
		this->SyncToRole(role, guild_id, guild_name, post);
	}
}

void GuildManager::SyncToRole(Role *role, GuildID guild_id, const GuildName guild_name, int post)
{
	if (NULL != role)
	{
		role->OnSyncGuildInfo(guild_id, guild_name, post);

		UserCacheManager::Instance().UpdateGuildId(role->GetUID(), guild_id);
	}
}

void GuildManager::SyncGuildBaseInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	gglobalprotocal::GetGuildBaselInfo ggbi; // 更新面板 给该玩家
	ggbi.guild_id = role->GetGuildID();
	ggbi.db_index = role->GetUserId().db_index;
	ggbi.role_id = role->GetUserId().role_id;
	this->GetGuildBaseInfo(role, &ggbi);
}

void GuildManager::ClearXianmengzhanRankValue()
{
	for (GuildMapIt it = m_guild_map.begin(); it != m_guild_map.end(); ++ it)
	{
		Guild *guild = it->second;
		if (NULL != guild)
		{
			guild->ClearXianmengzhanRankValue();
		}
	}
}

void GuildManager::ClearGuildBattleRank()
{
	for (GuildMapIt it = m_guild_map.begin(); it != m_guild_map.end(); ++ it)
	{
		Guild *guild = it->second;
		if (NULL != guild)
		{
			guild->ClearGuildBattleRank();
		}
	}
}

void GuildManager::ClearTerritoryWarRank()
{
	for (GuildMapIt it = m_guild_map.begin(); it != m_guild_map.end(); ++ it)
	{
		Guild *guild = it->second;
		if (NULL != guild)
		{
			guild->ClearTerritoryWarRank();
		}
	}
}

void GuildManager::ClearXianmengBipinKillBossCount()
{
	for (GuildMapIt it = m_guild_map.begin(); it != m_guild_map.end(); ++ it)
	{
		Guild *guild = it->second;
		if (NULL != guild)
		{
			guild->ClearXianmengBipinKillWorldBossCount();
		}
	}
}

void GuildManager::RecordYesterdayCapability()
{
	for (GuildMapIt it = m_guild_map.begin(); it != m_guild_map.end(); ++ it)
	{
		Guild *guild = it->second;
		if (NULL != guild)
		{
			guild->RecordYesterdayCapability();
		}
	}
}

void GuildManager::OnStartGuildFbReq(Role *role)
{
	if (NULL == role) 
	{
		return;
	}

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}

	Guild *guild = this->GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	if (guild->GetGuildLevel() < LOGIC_CONFIG->GetGuildFbConfig().GetGuildLvLimit())
	{
		role->NoticeNum(errornum::EN_GUILD_FB_LV_LIMIT);
		return;
	}

// 	if (GUILD_POST_FU_TUANGZHANG != role->GetGuildPost() && GUILD_POST_TUANGZHANG != role->GetGuildPost())
// 	{
// 		role->NoticeNum(errornum::EN_GUILD_FB_START_TUANZHANG_LIMIT);
// 		return;
// 	}

	if (guild->IsGuildFbOpen(static_cast<unsigned int>(EngineAdapter::Instance().Time())))
	{
		role->NoticeNum(errornum::EN_GUILD_FB_IS_OPEN);
		return;
	}

	if (guild->IsGuildFbAlreadyOpen())
	{
		role->NoticeNum(errornum::EN_GUILD_FB_ALREADY_START_ONCE);
		return;
	}

	if (!role->GetRoleActivity()->CanEnterGuildFb()) // 之前在别的仙盟进过 也不能开启
	{
		return;
	}

	const int guildexp_cost = LOGIC_CONFIG->GetGuildFbConfig().GetGuildExpCost();
	if (guild->GetExp() < guildexp_cost)
	{
		role->NoticeNum(errornum::EN_GUILD_FB_OPEN_EXP_LIMIT);
		return;
	}

	if (guild->DecGuildExp(guildexp_cost))
	{
		guild->StartGuildFb(role);
	}
}

void GuildManager::OnStartGuildBonfireReq(Role *role)
{
	if (NULL == role) 
	{
		return;
	}

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}

	Guild *guild = this->GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	if (GUILD_POST_FU_TUANGZHANG != role->GetGuildPost() && GUILD_POST_TUANGZHANG != role->GetGuildPost())
	{
		role->NoticeNum(errornum::EN_GUILD_BONFIRE_START_TUANZHANG_LIMIT);
		return;
	}

	if (!ActivityManager::Instance().IsActivityOpen(ACTIVITY_TYPE_GUILD_BONFIRE))
	{
		role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	if (Role::MOVE_MODE_FLY == role->GetMoveMode())
	{
		role->NoticeNum(errornum::EN_GUILD_BONFIRE_MOVE_MODE_LIMIT);
		return;
	}

	if (guild->IsGuildBonfireOpen(static_cast<unsigned int>(EngineAdapter::Instance().Time())))
	{
		role->NoticeNum(errornum::EN_GUILD_BONFIRE_IS_OPEN);
		return;
	}

	if (guild->IsGuildBonfireAlreadyOpen())
	{
		role->NoticeNum(errornum::EN_GUILD_BONFIRE_ALREADY_START_ONCE);
		return;
	}

	if (NULL == role->GetScene())
	{
		return;
	}

	if (NULL != role->GetScene() && !LOGIC_CONFIG->GetGuildBonfireCfg().IsOpenFireScene(role->GetScene()->GetSceneID()))
	{
		role->NoticeNum(errornum::EN_GUILD_BONFIRE_SCENE_LIMIT);
		return;
	}

	if (NULL != role->GetScene() && role->GetScene()->GetMap()->SaveArea(role->GetPos().x, role->GetPos().y))
	{
		role->NoticeNum(errornum::EN_GUILD_BONFIRE_SAVE_AREA_LIMIT);
		return;
	}

	const int guildexp_cost = LOGIC_CONFIG->GetGuildBonfireCfg().GetGuildExpCost();
	if (guild->GetExp() < guildexp_cost)
	{
		role->NoticeNum(errornum::EN_GUILD_BONFIRE_OPEN_EXP_LIMIT);
		return;
	}
	if (guild->DecGuildExp(guildexp_cost))
	{
		guild->StartGuildBonfire(role);
	}
}

void GuildManager::OnZhuFuMemberLuckyReq(Role *role, int be_zhufu_uid)
{
	if (NULL == role) 
	{
		return;
	}

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}

	Guild *guild = this->GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	if (!role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_GUILD_ZHUFU_TIMES, LOGIC_CONFIG->GetGuildConfig().GetGuildLuckyZhufuTimes()))
	{
		role->NoticeNum(errornum::EN_GUILD_ZHUFU_LUCKY_TIMES_IS_OUT);
		return;
	}

	GuildMember *be_zhufu_member = guild->GetGuildMemberManager().GetMember(IntToUid(be_zhufu_uid));
	if (NULL != be_zhufu_member)
	{
		int lucky_color = be_zhufu_member->GetLuckyColor();
		if (lucky_color >= GUILD_MEMBER_LUCKY_COLOR_MAX)
		{
			role->NoticeNum(errornum::EN_GUILD_ZHUFU_LUCKY_IS_FULL);
		}
		else 
		{
			MemberLuckBlessingCfg *luck_cfg = LOGIC_CONFIG->GetGuildConfig().GetMemberLuckBlessingCfg(lucky_color);
			if (NULL != luck_cfg)
			{
				int flush_color = GUILD_MEMBER_LUCKY_COLOR_INVALID;

				int rand_value = RandomNum(100);
				for (int i = GUILD_MEMBER_LUCKY_COLOR_WHITE; i < GUILD_MEMBER_LUCKY_COLOR_MAX; ++i)
				{
					if (rand_value < luck_cfg->upgrade_prob_list[i])
					{
						flush_color = i;
						break;
					}

					rand_value -= luck_cfg->upgrade_prob_list[i];
				}

				if (flush_color > lucky_color) // 更好的运势就修改 
				{
					be_zhufu_member->SetLuckyColor(flush_color);
					this->SyncLuckyInfo(guild, role, be_zhufu_uid, flush_color);
					role->NoticeNum(errornum::EN_GUILD_ZHUFU_LUCKY_FLUSH_UPGRADE);

					{
						if (flush_color >= GUILD_MEMBER_LUCKY_COLOR_ORANGE) // 祝福等级大于等于橙色发送传闻
						{
							UserCacheNode *be_zhufu_user_node = UserCacheManager::Instance().GetUserNode(be_zhufu_member->GetUserID());
							if (NULL != be_zhufu_user_node)
							{
								int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_lucky_info_orange_notice, 
									role->GetUID(), role->GetName(), (int)role->GetCamp(), be_zhufu_user_node->uid, be_zhufu_user_node->user_name, be_zhufu_user_node->camp,
									be_zhufu_user_node->uid, be_zhufu_user_node->user_name, be_zhufu_user_node->camp);
								if (len > 0) 
								{
									guild->GuildSystemMsg(len, gamestring::GAMESTRING_BUF);
								}
							}
						}

						{
							Role *be_zhufu_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(be_zhufu_uid));
							if (NULL != be_zhufu_role) // 给被祝福的家伙同步运势 
							{
								this->SyncLuckyInfo(guild, be_zhufu_role, be_zhufu_uid, flush_color);

								Protocol::SCGuildLuckyChangeNotice glcn;
								glcn.bless_uid = role->GetUID();
								role->GetName(glcn.bless_name);
								glcn.to_color = flush_color;
								EngineAdapter::Instance().NetSend(be_zhufu_role->GetNetId(), (const char *)&glcn, sizeof(glcn));
							}
						}
					}
				}
				else 
				{
					role->NoticeNum(errornum::EN_GUILD_ZHUFU_LUCKY_FLUSH_ISSAME);
				}

				role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_GUILD_ZHUFU_TIMES);
			}
		}
	}	
	else 
	{
		role->NoticeNum(errornum::EN_GUILD_ZHUFU_LUCKY_NOT_IN_YOURGUILD);
	}
}

void GuildManager::OnGetAllMemberLuckyInfoReq(Role *role)
{
	if (NULL == role) 
	{
		return;
	}

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}

	Guild *guild = this->GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	this->SyncLuckyInfo(guild, role);
}

void GuildManager::OnInviteZhuFuLuckyReq(Role *role, int invite_uid)
{
	if (NULL == role) 
	{
		return;
	}

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}

	Guild *guild = this->GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	static Protocol::SCInviteLuckyZhufu ilz;
	ilz.req_invite_uid = role->GetUID();
	role->GetName(ilz.req_invite_name);

	if (0 != invite_uid)
	{
		Role *invite_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(invite_uid));
		if (NULL != invite_role)
		{
			EngineAdapter::Instance().NetSend(invite_role->GetNetId(), (const char*)&ilz, sizeof(ilz));
		}
	}
	else 
	{
		guild->GetGuildMemberManager().SendMessageToPrincipal(Protocol::GUILD_NOTIFY_TYPE_GUILD_LUCKY, (const char*)&ilz, sizeof(ilz));
	}

	role->NoticeNum(noticenum::NT_INVITE_LUCKY_ZHUFU_SUCC);
}

void GuildManager::OnCallBeastReq(Role *role)
{
	if (NULL == role) 
	{
		return;
	}

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}

	Guild *guild = this->GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	char role_post = role->GetGuildPost();
	if (GUILD_POST_TUANGZHANG != role_post && GUILD_POST_FU_TUANGZHANG != role_post)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return;
	}

	guild->StartGuildBeast(role);
}

void GuildManager::OnActiveDegreeInfoReq(Role *role)
{
	if (NULL == role) 
	{
		return;
	}

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}

	Guild *guild = this->GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	guild->SyncGuildBeastStatus(role->GetNetId());
}

void GuildManager::OnFetchGuildBossRedbag(Role *role, int index)
{
	// 屏蔽公会boss红包
// 	if (NULL == role) return;
// 
// 	BossGuildRedbagCfg * boss_redbag_cfg = LOGIC_CONFIG->GetGuildConfig().GetBossGuildRedbagCfg(index);
// 	if (NULL == boss_redbag_cfg) return;
// 	
// 	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
// 	if (NULL == guild)
// 	{
// 		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
// 		return;
// 	}
// 
// 	GuildCommonDataParam *guild_common_data = guild->GetGuildCommonDataParam();
// 	if (NULL == guild_common_data)
// 	{
// 		return;
// 	}
// 
// 	if (guild_common_data->daily_kill_redbag_boss_times >= boss_redbag_cfg->kill_boss_times)
// 	{
// 		if (0 == (role->GetDailyBossRedbagRewardFetchFlag() & (1 << index)))
// 		{
// 			role->ChangeDailyBossRedbagRewardFetchFlag(index);
// 
// 			if (boss_redbag_cfg->max_diamond <= 0) return;
// 			int gold_num = RandomNum(boss_redbag_cfg->max_diamond);
// 			role->GetKnapsack()->GetMoney()->AddGoldBind(gold_num, "GuildBossRedbagFieldBindGold");
// 			role->NoticeNum(noticenum::EN_GUILD_FETCH_REDBAG_SUCC);
// 			role->SyncSCGulidBossRedbagInfo();
// 			guild->OnMemberFetchRedbag(role, gold_num);
// 		}
// 	}
// 
// 	guild->SendBossRedbagInfo(role, index);
}

void GuildManager::GuildBoxOperate(Role *role, int operate_type, int param1, int param2)
{
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}

	this->GuildBoxUpdate(role);

	switch (operate_type)
	{
	case Protocol::GBOT_QUERY_SELF:
		this->GuildBoxSendInfo(role, NULL);
		break;

	case Protocol::GBOT_UPLEVEL:
		this->GuildBoxUplevel(role, param1);
		break;

	case Protocol::GBOT_OPEN:
		this->GuildBoxOpen(role, param1);
		break;

	case Protocol::GBOT_FETCH:
		this->GuildBoxFetch(role, param1);
		break;

	case Protocol::GBOT_QUERY_NEED_ASSIST:
		this->GuildBoxSendNeedAssistInfo(role);
		break;

	case Protocol::GBOT_ASSIST:
		this->GuildBoxAssist(role, param1, param2);
		break;

	case Protocol::GBOT_CLEAN_ASSIST_CD:
		this->GuildBoxCleanCD(role);
		break;

	case Protocol::GBOT_INVITE_ASSIST:
		this->GuildInviteAssist(role, param1);
		break;

	case Protocol::GBOT_THANK_ASSIST:
		this->GuildthankAssist(role, param1, param2);
		break;
	}
}

void GuildManager::GuildBoxSendInfo(Role *role, GuildBoxInfo *guild_box_info)
{
	if (role == nullptr) return;

	if (NULL == guild_box_info)
	{
		guild_box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId());
	}

	static Protocol::SCGuildBoxInfo pro;
	if (NULL != guild_box_info)
	{
		pro.uplevel_count = guild_box_info->uplevel_count;
		pro.assist_count = guild_box_info->assist_count;
		pro.be_assist_count = guild_box_info->be_assist_count;
		pro.assist_cd_end_time = guild_box_info->assist_cd_end_time;
		for (int i = 0; i < MAX_GUILD_BOX_COUNT; i++)
		{
			pro.info_list[i].box_level = guild_box_info->info_list[i].box_level;
			pro.info_list[i].is_reward = guild_box_info->info_list[i].is_reward;
			pro.info_list[i].open_time = guild_box_info->info_list[i].open_time;

			for(int k = 0; k < MAX_GUILD_BOX_BE_ASSIST_COUNT; k++)
			{
				pro.info_list[i].assist_uid[k] = guild_box_info->info_list[i].assist_uid[k];
				pro.info_list[i].is_thank_assist_uid[k] = guild_box_info->info_list[i].is_thank_assist_uid[k];

				if (0 != pro.info_list[i].assist_uid[k])
				{
					UserCacheManager::Instance().GetUserName(pro.info_list[i].assist_uid[k], pro.info_list[i].assist_name[k]);
				}
				else
				{
					memset(pro.info_list[i].assist_name[k], 0, sizeof(pro.info_list[i].assist_name[k]));
				}
			}
		}
	}
	else
	{
		pro.uplevel_count = 0;
		pro.assist_count = 0;
		pro.assist_cd_end_time = 0;
		memset(pro.info_list, 0, sizeof(pro.info_list));
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void GuildManager::GuildBoxUplevel(Role *role, int box_index)
{
	if (box_index < 0 || box_index >= MAX_GUILD_BOX_COUNT || role == nullptr) return;
	if (!this->GuildBoxIsStart(role, true))
	{
		return;
	}

	if (box_index >= LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_GUILD_BOX_COUNT))
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_UPLEVEL_VIP_COUNT_LIMIT);
		return;
	}

	GuildBoxInfo *box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId(), true);
	if (NULL == box_info) return;

	GuildBoxCfg::ConfigItem *box_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildBoxCfg(box_info->info_list[box_index].box_level);
	if (NULL == box_cfg) return;

	GuildBoxCfg::ConfigItem *next_box_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildBoxCfg(box_info->info_list[box_index].box_level + 1);
	if (NULL == next_box_cfg)
	{
		gstr::SendError(role, "en_guild_box_uplevel_vip_count_limit");
		return;
	}

	if (box_info->info_list[box_index].open_time > 0)
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_OPENING);
		return;
	}

	GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
	if ((time_t)(role->GetRoleGuild()->GetLastLeaveGuildTime()) + other_cfg.box_limit_time > EngineAdapter::Instance().Time())
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_UPLEVEL_TIME_LIMIT);
		return;
	}

	if (box_info->uplevel_count >= other_cfg.box_free_up_count)
	{
		if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.box_up_gold, "GuildBoxUpLevel"))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	box_info->uplevel_count++;
	if (RandomNum(100) >= box_cfg->uplevel_rate)
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_UPLEVEL_FAIL);
	}
	else
	{
		box_info->info_list[box_index].box_level++;

		role->NoticeNum(noticenum::NT_GUILD_BOX_UPLEVEL_SUCC);
	}

	this->GuildBoxSendInfo(role, box_info);
	OfflineRegisterManager::Instance().OnGuildBoxInfoChange(role->GetUserId());
}

void GuildManager::GuildBoxOpen(Role *role, int box_index)
{
	if (box_index < 0 || box_index >= MAX_GUILD_BOX_COUNT || role == nullptr) return;
	if (!this->GuildBoxIsStart(role, true))
	{
		return;
	}

	if (box_index >= LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_GUILD_BOX_COUNT))
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_OPEN_VIP_COUNT_LIMIT);
		return;
	}

	GuildBoxInfo *box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId(), true);
	if (NULL == box_info) return;

	if (box_index > 0)
	{
		if (0 == box_info->info_list[box_index - 1].is_reward)
		{
			role->NoticeNum(errornum::EN_GUILD_BOX_PRE_BOX_OPEN_LIMIT);
			return;
		}
	}

	if (0 != box_info->info_list[box_index].is_reward)
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_REWARDED);
		return;
	}

	if (box_info->info_list[box_index].open_time > 0)
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_OPENING);
		return;
	}

	GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
	if ((time_t)(role->GetRoleGuild()->GetLastLeaveGuildTime()) + other_cfg.box_limit_time > EngineAdapter::Instance().Time())
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_OPEN_TIME_LIMIT);
		return;
	}

	GuildBoxCfg::ConfigItem *box_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildBoxCfg(box_info->info_list[box_index].box_level);
	if (NULL == box_cfg) return;

	box_info->info_list[box_index].open_time = (unsigned int)(EngineAdapter::Instance().Time() + box_cfg->box_open_time);

	this->GuildBoxSendInfo(role, box_info);
	OfflineRegisterManager::Instance().OnGuildBoxInfoChange(role->GetUserId());

	EventHandler::Instance().OnOpenGuildBox(role);
}

void GuildManager::GuildBoxFetch(Role *role, int box_index, bool is_notice)
{
	if (box_index < 0 || box_index >= MAX_GUILD_BOX_COUNT) return;

	GuildBoxInfo *box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId());
	if (NULL == box_info) return;

	if (0 != box_info->info_list[box_index].is_reward || box_info->info_list[box_index].open_time <= 0)
	{	
		if (is_notice)
		{
			role->NoticeNum(errornum::EN_GUILD_BOX_REWARDED);
		}
		return;
	}

	if (box_info->info_list[box_index].open_time > (unsigned int)(EngineAdapter::Instance().Time()))
	{
		if (is_notice)
		{
			role->NoticeNum(errornum::EN_GUILD_BOX_OPENING);
		}
		return;
	}

	GuildBoxCfg::ConfigItem *box_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildBoxCfg(box_info->info_list[box_index].box_level);
	if (NULL == box_cfg) return;

	box_info->info_list[box_index].is_reward = 1;
	OfflineRegisterManager::Instance().OnGuildBoxInfoChange(role->GetUserId());

	EventHandler::Instance().OnFetchGuildBox(role);

	//role->GetKnapsack()->PutOrMail(box_cfg->item_reward, PUT_REASON_GUILD_BOX_OPEN);

	{
		for (int i = 0; i < box_cfg->item_reward_count; i++)
		{
			if (!role->GetKnapsack()->HaveSpace())
			{
				role->NoticeNum(noticenum::NT_GUILD_BOX_OPEN);
			}
			role->GetKnapsack()->PutOrMail(box_cfg->item_reward_list[i], PUT_REASON_GUILD_BOX_OPEN);
		}

		if (RandomNum(100) < box_cfg->probability)
		{
			role->GetKnapsack()->PutOrMail(box_cfg->random_reward, PUT_REASON_GUILD_BOX_OPEN);
		}
	}

	for(int i = 0; i < MAX_GUILD_BOX_BE_ASSIST_COUNT; i++)
	{
		if (0 != box_info->info_list[box_index].assist_uid[i])
		{
			role->GetKnapsack()->GetMoney()->AddGoldBind(box_cfg->be_assist_reward_bind_gold, "GuildBoxFetch");
			break;
		}
	}
}

void GuildManager::GuildBoxMialReward(Role *role, int box_index)
{
	if (box_index < 0 || box_index >= MAX_GUILD_BOX_COUNT || role == nullptr) return;

	GuildBoxInfo *box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId());
	if (NULL == box_info) return;

	if (0 != box_info->info_list[box_index].is_reward || box_info->info_list[box_index].open_time <= 0)
	{
		return;
	}

	if (box_info->info_list[box_index].open_time > (unsigned int)(EngineAdapter::Instance().Time()))
	{
		return;
	}

	GuildBoxCfg::ConfigItem *box_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildBoxCfg(box_info->info_list[box_index].box_level);
	if (NULL == box_cfg) return;

	box_info->info_list[box_index].is_reward = 1;
	OfflineRegisterManager::Instance().OnGuildBoxInfoChange(role->GetUserId());

	EventHandler::Instance().OnFetchGuildBox(role);

	static MailContentParam contentparam; contentparam.Reset();
	//contentparam.AddItem(box_cfg->item_reward.item_id, box_cfg->item_reward.num, box_cfg->item_reward.is_bind);
	for (int i = 0; i < box_cfg->item_reward_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
	{
		contentparam.AddItem(box_cfg->item_reward_list[i].item_id, box_cfg->item_reward_list[i].num, box_cfg->item_reward_list[i].is_bind);
	}


	for (int i = 0; i < MAX_GUILD_BOX_BE_ASSIST_COUNT; i++)
	{
		if (0 != box_info->info_list[box_index].assist_uid[i])
		{
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, box_cfg->be_assist_reward_bind_gold);
			break;
		}
	}

	int length1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_guild_box_open_content);
	if (length1 > 0)
	{
		MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	if (RandomNum(100) < box_cfg->probability)
	{
		static MailContentParam contentparam_random_reward; contentparam_random_reward.Reset();

		contentparam_random_reward.AddItem(box_cfg->random_reward.item_id, box_cfg->random_reward.num, box_cfg->random_reward.is_bind);

		int length2 = SNPRINTF(contentparam_random_reward.contenttxt, sizeof(contentparam_random_reward.contenttxt), gamestring::g_guild_box_open_content);
		if (length2 > 0)
		{
			MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam_random_reward);
		}
	}
}

void GuildManager::GuildBoxSendNeedAssistInfo(Role *role)
{
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild) return;

	static Protocol::SCGuildBoxNeedAssistInfo pro;
	pro.box_count = 0;

	static int member_uid_list[MAX_MEMBER_COUNT];
	int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
	for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; i++)
	{
		if (member_uid_list[i] == role->GetUID()) continue;

		GuildBoxInfo *box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(IntToUid(member_uid_list[i]));
		if (NULL == box_info) continue;

		for (int k = 0; k < MAX_GUILD_BOX_COUNT; k++)
		{
			if (box_info->info_list[k].open_time > 0
				&& static_cast<unsigned int>(EngineAdapter::Instance().Time()) < box_info->info_list[k].open_time
				&& 0 == box_info->info_list[k].is_reward)
			{
				int assist_count = 0;
				bool is_has_assist = false;
				for(int n = 0; n < MAX_GUILD_BOX_BE_ASSIST_COUNT; n++)
				{
					if (0 != box_info->info_list[k].assist_uid[n])
					{
						if (box_info->info_list[k].assist_uid[n] == role->GetUID())
						{
							is_has_assist = true;
						}

						assist_count++;
					}
				}

				GuildBoxCfg::ConfigItem *box_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildBoxCfg(box_info->info_list[k].box_level);
				if (NULL == box_cfg) continue;

				if (is_has_assist)
				{
					continue;
				}

				if (assist_count >= box_cfg->can_be_assist_max_count)
				{
					continue;
				}

				pro.info_list[pro.box_count].uid = member_uid_list[i];
				pro.info_list[pro.box_count].box_index = k;
				pro.info_list[pro.box_count].box_level = box_info->info_list[k].box_level;
				pro.info_list[pro.box_count].open_time = box_info->info_list[k].open_time;
				UserCacheManager::Instance().GetUserName(member_uid_list[i], pro.info_list[pro.box_count].user_name);
				pro.box_count++;

				if (pro.box_count >= Protocol::SCGuildBoxNeedAssistInfo::MAX_BOX_ASSIST_COUNT) break;
			}
		}

		if (pro.box_count >= Protocol::SCGuildBoxNeedAssistInfo::MAX_BOX_ASSIST_COUNT) break;
	}

	int length = sizeof(pro) - (Protocol::SCGuildBoxNeedAssistInfo::MAX_BOX_ASSIST_COUNT - pro.box_count) * sizeof(pro.info_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, length);
}

void GuildManager::GuildBoxAssist(Role *role, int box_index, int target_uid)
{
	if (box_index < 0 || box_index >= MAX_GUILD_BOX_COUNT || role == nullptr) return;
	if (!this->GuildBoxIsStart(role, true))
	{
		return;
	}

	GuildBoxInfo *guild_box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId(), true);
	if (NULL == guild_box_info) return;

	GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
	if (guild_box_info->assist_count >= other_cfg.box_assist_max_count)
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_ASSIST_COUNT_LIMIT);
		return;
	}

	unsigned int time_now = (unsigned int)EngineAdapter::Instance().Time();
	if (guild_box_info->assist_cd_end_time > time_now && guild_box_info->assist_cd_end_time >= time_now + other_cfg.box_assist_cd_limit)
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_ASSIST_TIME_LIMIT);
		return;
	}

	if (target_uid == role->GetUID()) return;

// 	if (!role->GetKnapsack()->HaveSpace())
// 	{
// 		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return;
// 	}

	GuildBoxInfo *target_box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(IntToUid(target_uid));
	if (NULL == target_box_info) return;

	GuildBoxCfg::ConfigItem *box_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildBoxCfg(target_box_info->info_list[box_index].box_level);
	if (NULL == box_cfg) return;

	int cur_assit_count = 0;
	int first_invalid_index = -1;
	for (int i = 0; i < MAX_GUILD_BOX_BE_ASSIST_COUNT; i++)
	{
		if (0 != target_box_info->info_list[box_index].assist_uid[i])
		{
			if (role->GetUID() == target_box_info->info_list[box_index].assist_uid[i])
			{
				role->NoticeNum(errornum::EN_GUILD_BOX_HAVE_ASSIST_LIMIT);
				return;
			}

			cur_assit_count++;
		}
		else
		{
			if (-1 == first_invalid_index)
			{
				first_invalid_index = i;
			}
		}
	}

	if (cur_assit_count >= box_cfg->can_be_assist_max_count || first_invalid_index < 0 || first_invalid_index >= MAX_GUILD_BOX_BE_ASSIST_COUNT)
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_BE_ASSIST_TIMES_LIMIT);
		return;
	}

	if (0 != target_box_info->info_list[box_index].is_reward) return;

	if (target_box_info->info_list[box_index].open_time < static_cast<unsigned int>(EngineAdapter::Instance().Time()))
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_CAN_OPEN);
		return;
	}

	guild_box_info->assist_count++;
	if (guild_box_info->assist_cd_end_time > time_now)
	{
		guild_box_info->assist_cd_end_time += other_cfg.box_assist_cd;
	}
	else
	{
		guild_box_info->assist_cd_end_time = time_now + other_cfg.box_assist_cd;
	}
	this->GuildBoxSendInfo(role, guild_box_info);
	OfflineRegisterManager::Instance().OnGuildBoxInfoChange(role->GetUserId());

	target_box_info->info_list[box_index].assist_uid[first_invalid_index] = role->GetUID();
	target_box_info->info_list[box_index].open_time -= box_cfg->box_assist_dec_time;
	target_box_info->be_assist_count++;

	OfflineRegisterManager::Instance().OnGuildBoxInfoChange(IntToUid(target_uid));

	// 如果在线，通知对方
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(target_uid));
	if (nullptr != target_role)
	{
		this->GuildBoxSendInfo(target_role, target_box_info);
	}

//	role->GetKnapsack()->Put(box_cfg->assist_reward, PUT_REASON_GUILD_BOX_ASSIST);

	this->GuildBoxSendNeedAssistInfo(role);
}

void GuildManager::GuildBoxCleanCD(Role *role)
{
	GuildBoxInfo *guild_box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId());
	if (NULL == guild_box_info) return;

	unsigned int time_now = (unsigned int)EngineAdapter::Instance().Time();
	if (guild_box_info->assist_cd_end_time > time_now)
	{
		int minute = (guild_box_info->assist_cd_end_time - time_now + 59) / 60;
		int need_gold = minute * LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().box_assist_cd_gold;
		if (!role->GetKnapsack()->GetMoney()->UseGold(need_gold, "GuildBoxCleanCD"))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		guild_box_info->assist_cd_end_time = 0;
		this->GuildBoxSendInfo(role, guild_box_info);
		OfflineRegisterManager::Instance().OnGuildBoxInfoChange(role->GetUserId());
	}
}

void GuildManager::GuildInviteAssist(Role *role, int target_uid)
{
	if (role == nullptr)return;

	GuildBoxInfo *guild_box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId(), true);
	if (NULL == guild_box_info) return;
	bool box_assist = false;
	for (int i = 0; i < MAX_GUILD_BOX_COUNT; i++)
	{
		if (guild_box_info->info_list[i].open_time > 0 && !guild_box_info->info_list[i].is_reward)
		{
			box_assist = true;
			break;
		}
	}
	if (!box_assist)
	{
		role->NoticeNum(errornum::EN_GUILD_BOX_ASSIST_NOT_HAVE);
		return;
	}

// 	int content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_box_invite_assist,
// 		role->GetUID(), role->GetName(), role->GetCamp());
// 
// 	int sendlen = 0;
// 	Protocol::SCSystemMsg sm;
// 
// 	Guild * guild = GetGuild(role->GetGuildID());
// 	if (guild != NULL && SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_CHAT_GUILD_PERSONAL))
// 	{
// 		guild->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_BOX_ASSIST, (const char *)&sm, sendlen);
// 		role->NoticeNum(noticenum::NT_HUNYAN_INVATE_SUCC);
// 	}

	// 邀请自动私聊
	Protocol::CSSingleChat *sc = (Protocol::CSSingleChat *)gamestring::GAMESTRING_BUF;
	sc->msg_length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSSingleChat),
		sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSSingleChat), "guild_box_invite_assist");
	if (sc->msg_length > 0)
	{
		sc->to_uid = target_uid;
		ChatManager::Instance().OnSingleChat(role, sc, sizeof(Protocol::CSSingleChat) + sc->msg_length, true);
	}

	gstr::SendNotice(role, "guild_box_invite_assist_succ");
}

void GuildManager::GuildthankAssist(Role *role, int box_index, int target_uid)
{
	if (role == nullptr || box_index < 0 || box_index >= MAX_GUILD_BOX_COUNT || target_uid <= 0)return;

	GuildBoxInfo *guild_box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId());
	if (NULL == guild_box_info) return;

	unsigned int max_count = static_array_size(guild_box_info->info_list[box_index].assist_uid);

	for (unsigned int i = 0; i < max_count; i ++)
	{
		if (target_uid == guild_box_info->info_list[box_index].assist_uid[i])
		{
			if (0 <= i && i < static_array_size(guild_box_info->info_list[box_index].is_thank_assist_uid))
			{
				guild_box_info->info_list[box_index].is_thank_assist_uid[i] = 1;
			}
			break;
		}
	}

	// 感谢自动私聊
	Protocol::CSSingleChat *sc = (Protocol::CSSingleChat *)gamestring::GAMESTRING_BUF;
	sc->msg_length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSSingleChat),
		sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSSingleChat), "guild_box_thank_assist");
	if (sc->msg_length > 0)
	{
		sc->to_uid = target_uid;
		ChatManager::Instance().OnSingleChat(role, sc, sizeof(Protocol::CSSingleChat) + sc->msg_length, true);
	}

	this->GuildBoxSendInfo(role, NULL);

	gstr::SendNotice(role, "guild_box_thank_assist_succ");
}

void GuildManager::GuildBoxUpdate(Role *role)
{
	if (role == nullptr) return;

	if (!this->GuildBoxIsStart(role, false))
	{
		return;
	}

	GuildBoxInfo *guild_box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId(), true);
	if (NULL == guild_box_info)
	{
		return;
	}

	if (EngineAdapter::Instance().DayID() == guild_box_info->dayid)
	{
		return;
	}

	for (int i = 0; i < MAX_GUILD_BOX_COUNT; i++)
	{
		GuildManager::Instance().GuildBoxMialReward(role, i);
	}

	guild_box_info->Reset();
	guild_box_info->dayid = EngineAdapter::Instance().DayID();

	for (int i = 0; i < MAX_GUILD_BOX_COUNT; i++)
	{
		guild_box_info->info_list[i].box_level = LOGIC_CONFIG->GetGuildConfig().RandGuildBoxLevel();
	}
	OfflineRegisterManager::Instance().OnGuildBoxInfoChange(role->GetUserId());
}

bool GuildManager::GuildBoxIsStart(Role *role, bool is_notice)
{
	if (!m_guild_box_is_gm_start)
	{
		int daytime = 24 * 3600 - EngineAdapter::Instance().NextDayInterval(0, 0, 0);
		if (daytime < LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().box_start_time)
		{
			if (is_notice && role != nullptr) role->NoticeNum(errornum::EN_GUILD_BOX_NOT_START);
			return false;
		}
	}

	return true;
}

void GuildManager::GuildBoxGmOpenNow(Role *role, int box_index)
{
	if (box_index < 0 || box_index >= MAX_GUILD_BOX_COUNT) return;

	GuildBoxInfo *guild_box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId());
	if (NULL == guild_box_info) return;

	guild_box_info->info_list[box_index].open_time = (unsigned int)EngineAdapter::Instance().Time();
	this->GuildBoxSendInfo(role, guild_box_info);
	OfflineRegisterManager::Instance().OnGuildBoxInfoChange(role->GetUserId());
}

void GuildManager::GuildBoxGmCleanCD(Role *role)
{
	GuildBoxInfo *guild_box_info = OfflineRegisterManager::Instance().GetUserGuildBoxInfo(role->GetUserId());
	if (NULL == guild_box_info) return;

	guild_box_info->assist_cd_end_time = 0;
	this->GuildBoxSendInfo(role, guild_box_info);
	OfflineRegisterManager::Instance().OnGuildBoxInfoChange(role->GetUserId());
}

void GuildManager::GuildBoxGmStart(bool is_start)
{
	m_guild_box_is_gm_start = is_start;
}

void GuildManager::OnFetchDailyReward(Role *role)
{
	if (role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_GUILD_REWARD) > 0)
	{
		role->NoticeNum(errornum::EN_GUILD_REWARD_FETCHED);
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	const GuildLevelConfig::ConfigItem *cfg = LOGIC_CONFIG->GetGuildConfig().GetLevelConfig(guild->GetGuildLevel());
	if (NULL == cfg)
	{
		return;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!role->GetKnapsack()->Put(cfg->daily_reward_item, PUT_REASON_GUILD_REWARD))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_GUILD_REWARD);
}

void GuildManager::OnFetchTerritoryReward(Role *role, int param1)
{
	if (NULL == role) return;

	if (INVALID_GUILD_ID == role->GetGuildID())
		return;

	if (param1 <= Protocol::INVALID_PHASE || param1 >= Protocol::MAX_PAHSE) return;

	if (role->GetRoleGuild()->GetRoleGuildParam()->IsFetchtTerritoryWelfReward(param1))
	{
		role->NoticeNum(errornum::EN_GUILD_TERRITORY_REWARD_LIMIT);
		return;
	}

	int rank = -1;
	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL != guild)
	{
		rank = guild->GetTerritoryWarRank();
	}

	GuildTerritoryWelfCfg::ConfigItem* reward = NULL;
	if (-1 == rank)
	{
		reward = LOGIC_CONFIG->GetGuildConfig().GetGuildTerritoryWelf(0);
	}
	else
	{
		reward = LOGIC_CONFIG->GetGuildConfig().GetGuildTerritoryWelf(rank);
	}

	if (NULL == reward) return;

	if (rank == -1 && Protocol::ONE_PHASE != param1)
	{
		return;
	}

	switch(param1)
	{
	case Protocol::ONE_PHASE:
		{
			if (role->GetRoleGuild()->GetDailyGongXian() < reward->banggong_one_limit)
			{
				role->NoticeNum(errornum::EN_GUILD_TERRITORY_REWARD_DAY_BANGGONG_LIMIT);
				return;
			}

			role->GetKnapsack()->PutOrMail(reward->phase_one_reward, PUT_REASON_GUILD_TERRITORY_WELF);
			role->GetRoleGuild()->GetRoleGuildParam()->SetTerritoryWelfFlag(Protocol::ONE_PHASE);
			role->GetRoleGuild()->SendRoleGuildInfo();
		}
		break;

	case Protocol::TWO_PHASE:
		{
			if (role->GetRoleGuild()->GetDailyGongXian() < reward->banggong_two_limit)
			{
				role->NoticeNum(errornum::EN_GUILD_TERRITORY_REWARD_DAY_BANGGONG_LIMIT);
				return;
			}

			role->GetKnapsack()->PutOrMail(reward->phase_two_reward, PUT_REASON_GUILD_TERRITORY_WELF);
			role->GetRoleGuild()->GetRoleGuildParam()->SetTerritoryWelfFlag(Protocol::TWO_PHASE);
			role->GetRoleGuild()->SendRoleGuildInfo();
		}
		break;

	case Protocol::THREE_PHASE:
		{
			if (role->GetRoleGuild()->GetDailyGongXian() < reward->banggong_three_limit)
			{
				role->NoticeNum(errornum::EN_GUILD_TERRITORY_REWARD_DAY_BANGGONG_LIMIT);
				return;
			}

			role->GetKnapsack()->PutOrMail(reward->phase_three_reward, PUT_REASON_GUILD_TERRITORY_WELF);
			role->GetRoleGuild()->GetRoleGuildParam()->SetTerritoryWelfFlag(Protocol::THREE_PHASE);
			role->GetRoleGuild()->SendRoleGuildInfo();
		}
		break;

	case Protocol::FOUR_PHASE:
		{
			if (role->GetRoleGuild()->GetDailyGongXian() < reward->banggong_four_limit)
			{
				role->NoticeNum(errornum::EN_GUILD_TERRITORY_REWARD_DAY_BANGGONG_LIMIT);
				return;
			}

			role->GetKnapsack()->PutOrMail(reward->phase_four_reward, PUT_REASON_GUILD_TERRITORY_WELF);
			role->GetRoleGuild()->GetRoleGuildParam()->SetTerritoryWelfFlag(Protocol::FOUR_PHASE);
			role->GetRoleGuild()->SendRoleGuildInfo();
		}
		break;

	default:
		break;
	}

	return;
}

void GuildManager::OnFetchTerritoryExtraReward(Role *role)
{
	if(NULL == role) return;

	if (INVALID_GUILD_ID == role->GetGuildID())
		return;

	if (GUILD_POST_TUANGZHANG != role->GetGuildPost())
	{
		role->NoticeNum(errornum::EN_GUILD_TERRITORY_EXTRA_LIMIT);
		return;
	}

	if (role->GetRoleGuild()->GetRoleGuildParam()->IsFetchtTerritoryWelfReward(Protocol::PRESIDENT_EXTRA_REWARD))
		return;

	int rank = -1;
	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL != guild)
	{
		rank = guild->GetTerritoryWarRank();
	}

	GuildTerritoryWelfCfg::ConfigItem* reward = NULL;
	if (-1 == rank)
	{
		reward = LOGIC_CONFIG->GetGuildConfig().GetGuildTerritoryWelf(0);
	}
	else
	{
		reward = LOGIC_CONFIG->GetGuildConfig().GetGuildTerritoryWelf(rank);
	}

	if (NULL == reward) return;

	role->GetKnapsack()->PutOrMail(reward->guild_president_extra_reward, PUT_REASON_GUILD_TERRITORY_WELF);
	role->GetRoleGuild()->GetRoleGuildParam()->SetTerritoryWelfFlag(Protocol::PRESIDENT_EXTRA_REWARD);
	role->GetRoleGuild()->SendRoleGuildInfo();
}

void GuildManager::SyncLuckyInfo(Guild *guild, Role *role, int change_uid, int flush_color)
{
	if (NULL == guild || NULL == role)
	{
		return;
	}

	static Protocol::SCGuildLuckyInfo gli;
	gli.member_count = 0;

	if (0 != change_uid)
	{
		gli.reason = Protocol::SCGuildLuckyInfo::NOTIFY_REASON_CHANGE;

		gli.member_count = 1;
		gli.member_luckyinfo_list[0].uid = change_uid;
		gli.member_luckyinfo_list[0].lucky_color = flush_color;
		memset(gli.member_luckyinfo_list[0].user_name, 0, sizeof(gli.member_luckyinfo_list[0].user_name));

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(change_uid);
		if (NULL != user_node)
		{
			user_node->GetName(gli.member_luckyinfo_list[0].user_name);
		}
	}
	else
	{
		gli.reason = Protocol::SCGuildLuckyInfo::NOTIFY_REASON_GET;
		guild->GetGuildMemberManager().GetMemberLuckyInfo(&gli);
	}

	int sendlen = sizeof(gli) - sizeof(gli.member_luckyinfo_list[0]) * (MAX_MEMBER_COUNT - gli.member_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gli, sendlen);
}

int GuildManager::OnGmCommand(GuildID guild_id, int oper_type)
{
	Guild *guild = this->GetGuild(guild_id);

	if (NULL != guild)
	{
		switch (oper_type)
		{
		case GUILD_GM_OPER_TYPE_DISMISS:
			{
				return this->GmDismissGuild(guild);
			}
			break;

		case GUILD_GM_OPER_TYPE_CLEAR_NOTICE:
			{
				return this->GmClearNotice(guild);
			}
			break;
		}
	}

	return -1;
}

void GuildManager::SendGuildRedPaperlistInfo(Role *role)
{
	if (NULL == role || INVALID_GUILD_ID == role->GetGuildID())
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	static Protocol::SCGuildRedPaperListInfo cmd;
	guild->GetGuildRedPaperList(role, &cmd);

	int sendlen = sizeof(cmd) - (Protocol::SCGuildRedPaperListInfo::MAX_PAPER_COUNT - cmd.count) * sizeof(Protocol::SCGuildRedPaperListInfo::PaperItem);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sendlen);
}

void GuildManager::SingalChatRedPaperRole(Role *role, int red_paper_index)
{
	if (NULL == role || INVALID_GUILD_ID == role->GetGuildID())
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	role->NoticeNum(noticenum::NT_RED_PAPER_NOTICE_SUCC);

	guild->SignalChatGuildPaperRole(role, red_paper_index);
}

void GuildManager::SetAutoClear(Role *role, short is_auto_clear)
{
	if (NULL == role) return;

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	guild->SetAutoClear(role, is_auto_clear);
}

void GuildManager::OnCrossGuildBattleBegin()
{
	for (GuildMapIt it = m_guild_map.begin(); it != m_guild_map.end(); ++it)
	{
		Guild *guild = it->second;
		if (NULL != guild)
		{
			guild->SetJoinGuildBattleFlag(0);
			guild->ClearGuildBattleJoinFlag();
		}
	}
}

void GuildManager::SendGuildBattleJoinReward(const std::set<GuildID> &ignore_guild_list)
{
	for (GuildMapIt it = m_guild_map.begin(); it != m_guild_map.end(); ++it)
	{
		Guild *guild = it->second;
		// 如果当前帮派参与了帮派战并且不在忽略列表，才发奖励
		if (NULL != guild && ignore_guild_list.find(guild->GetGuildID()) == ignore_guild_list.end() && guild->GetJoinGuildBattleFlag() > 0)
		{
			guild->SendBattleJoinReward();
		}
	}
}

void GuildManager::ChangeAvatar(Role *role, unsigned long long avatar_timestamp)
{
	Guild *guild = this->GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_XIANMENGZHAN_NO_GUILD);
		return;
	}

	GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member)
	{
		return;
	}

	if (!GuildAuthority::CanChangeAvatar(member))
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return;
	}

	GuildCommonDataParam *param = guild->GetGuildCommonDataParam();
	if (NULL != param)
	{
		param->avatar_timestamp = avatar_timestamp;
	}

	auto commdata = role->GetCommonDataParam();

	if (param->avatar_changed == 0 && commdata->avatar_changed == 0)
	{//第一次变更头像 发奖励 一个仙盟只能领取一次， 一个人只能领取一次
		++param->avatar_changed;
		++commdata->avatar_changed;
		int bind_coin = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().first_change_avatar_give_bind_coin;
		if (bind_coin > 0)
		{
			role->GetKnapsack()->GetMoney()->AddGoldBind(bind_coin, "first_change_avatar_give_bind_coin");
		}
	}

	this->SyncGuildBaseInfo(role);
	guild->GetGuildMemberManager().OnRefreshGuildAppearanceInfo();
}

void GuildManager::OnBiaoCheStart(Role *role, GuildID guild_id)
{
	if (nullptr == role)
	{
		return;
	}

	Guild *guild = this->GetGuild(guild_id);
	if (NULL == guild) // 1：军团必须存在
	{
		return;
	}

	GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member) // 2：必须是该军团成员
	{
		return;
	}

	if (!GuildAuthority::CanBiaoCheStart(member)) // 3：必须是有权限
	{
		World::GetInstWorld()->GetSceneManager()->NoticeToUser(role->GetUserId(), errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return;
	}

	if (guild->IsBiaoCheHadStart()) // 4：今天运镖活动还没开始过
	{
		gstr::SendError(role, "en_guild_yunbiao_is_start");
		return;
	}

	if (guild->IsHadBiaoChe())	  // 5.是否开启中
	{
		gstr::SendError(role, "en_guild_yunbiao_starting");
		return;
	}

//	const tm * local_tm = EngineAdapter::Instance().LocalTime();

	if (!ActivityManager::Instance().IsActivityOpen(ACTIVITY_TYPE_GUILD_YUNBIAO))
//		!LOGIC_CONFIG->GetGuildConfig().NowCanStartGuildYunBiao(local_tm->tm_hour, local_tm->tm_min)) // 6：检查活动时间
	{
		gstr::SendError(role, "en_guild_yunbiao_time_limit");
		return;
	}

	const int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();
	if (role->GetScene()->GetSceneID() != biaoche_sceneid)
	{
		gstr::SendError(role, "en_guild_yunbiao_open_scene_limit");
		return;
	}

	guild->SetBiaoCheStart();	// 7：设置运镖活动开始，然后通知gameworld （通知所有gameworld，如果只是给这个家伙发，当心他下线了）

	static gglobalprotocal::GuildYunBiaoStart ybs;
	ybs.guild_id = guild->GetGuildID();
	ybs.guild_level = guild->GetGuildLevel();
	GuildShadow::Instance().OnGuildYunBiaoStart(&ybs);
	int biaoche_objid = GuildShadow::Instance().GetGuildBiaocheObjId(guild->GetGuildID());

	guild->SetOpenBiaoCheUID(role->GetUID());
	guild->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_START, NULL, biaoche_objid, role->GetUID());

	Posi biaoche_pos(0, 0);
	if (LOGIC_CONFIG->GetGuildConfig().GetBiaoChePos(0, &biaoche_pos))
	{
		int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guild_biaoche_start_notice", guild->GetGuildName(),
			biaoche_sceneid, biaoche_pos.x, biaoche_pos.y);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}
}

void GuildManager::OnTransToGuildBiaoCheReq(Role *role, GuildID guild_id)
{
	if (nullptr == role)
	{
		return;
	}

	Guild *guild = this->GetGuild(guild_id);
	if (NULL != guild)
	{
		guild->OnTransToBiaoCheReq(role);
	}
}

void GuildManager::OnYunBiaoStatus(gglobalprotocal::GuildYunBiaoStatus *gybs)
{
	Guild *guild = this->GetGuild(gybs->guild_id);
	if (NULL != guild)
	{
		guild->OnYunBiaoStatus(gybs);
	}
}

void GuildManager::OnGetBiaoChePos(Role *role, GuildID guild_id)
{
	if (nullptr == role)
	{
		return;
	}

	Guild *guild = this->GetGuild(guild_id);
	if (NULL != guild)
	{
		guild->OnGetBiaoChePos(role);
	}
}

void GuildManager::OnGuildKillCrossBossRankReward()
{
	int count = 0;
	GuildID win_guild[3] = {0};
	GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
	if (NULL != guild_rank)
	{
		count = guild_rank->GetRankTopGuild(GUILD_RANK_TYPE_KILL_CROSS_BOSS, 3, win_guild);
	}

	if (1 == count)
	{
		Guild * guild1 = this->GetGuild(win_guild[0]);
		if (NULL == guild1) return;
		int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guild_cross_boss_rank_notice1", guild1->GetGuildName());
		if (lenth > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
		}
	}
	else if (2 == count)
	{
		Guild * guild1 = this->GetGuild(win_guild[0]);
		if (NULL == guild1) return;
		Guild * guild2 = this->GetGuild(win_guild[1]);
		if (NULL == guild2) return;
		int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guild_cross_boss_rank_notice2", guild1->GetGuildName(), guild2->GetGuildName());
		if (lenth > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
		}
	}
	else if (3 == count)
	{
		Guild * guild1 = this->GetGuild(win_guild[0]);
		if (NULL == guild1) return;
		Guild * guild2 = this->GetGuild(win_guild[1]);
		if (NULL == guild2) return;
		Guild * guild3 = this->GetGuild(win_guild[2]);
		if (NULL == guild3) return;
		int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guild_cross_boss_rank_notice3",
			guild1->GetGuildName(), guild2->GetGuildName(), guild3->GetGuildName());
		if (lenth > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
		}
	}

	Guild * guild = this->GetGuild(win_guild[0]);
	if (NULL != guild)
	{
		WorldStatus::Instance().SyncKillCrossBossRankGuild(guild->GetTuanzhuangRoleId(), guild->GetGuildID());

		MailContentParam contenetparam; contenetparam.Reset();
		GuildOtherConfig & other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			contenetparam.AddItem(other_cfg.kill_cross_boss_reward[i]);
		}
		
		int lenth1 = gstr::MailContent(contenetparam.subject, sizeof(contenetparam.subject), "guild_kill_cross_boss_first_subject");
		int lenth2 = gstr::MailContent(contenetparam.contenttxt, sizeof(contenetparam.contenttxt), "guild_kill_cross_boss_first_content");

		if (lenth1 > 0 && lenth2 > 0)
		{
			int member_uid_list[MAX_MEMBER_COUNT] = {0};
			int count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
			for (int i = 0; i < count && i < MAX_MEMBER_COUNT; ++i)
			{
				MailRoute::MailToUser(IntToUid(member_uid_list[i]), SYSTEM_MAIL_REASON_INVALID,contenetparam);
			}
		}

		int sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guild_cross_boss_rank_top", guild->GetGuildName());
		if (sendlen > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, sendlen, SYS_MSG_CENTER_ROLL_2);
		}
	}
	else
	{
		WorldStatus::Instance().SyncKillCrossBossRankGuild(0, 0);
	}
}

void GuildManager::OnGiveGongzi(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	Guild *guild = this->GetGuild(role->GetGuildID());
	if (NULL != guild)
	{
		guild->OnGiveGongzi(role);
		this->SyncGuildBaseInfo(role);
	}
}

bool GuildManager::OnAddGongzi(GuildID guild_id, int uid, int add_gongzi_type, int add_val, const char* reason)
{
	if (nullptr == reason)
	{
		return false;
	}

	Guild *guild = this->GetGuild(guild_id);
	if (NULL == guild)
	{
		return false;
	}

	switch (add_gongzi_type)
	{
	case GUILD_GONGZI_GIVE_TYPE_NORMAL:
	{
		guild->AddGuildGongzi(add_val);
		guild->AddMemberGongzi(uid, add_val);
	}
	break;

	case GUILD_GONGZI_GIVE_TYPE_ONLY_GUILD:
	{
		guild->AddGuildGongzi(add_val);
	}
	break;

	default:
		return false;
		break;
	}

	gamelog::g_log_guild.printf(LL_INFO, "GuildManager::OnAddGongzi guild_id[%d], add_reason[%s], add_gongzi_val[%d]",
		guild->GetGuildID(), reason, add_val);

	return true;
}

void GuildManager::OnAddGuildKillBossNum(GuildID guild_id, int num)
{
	Guild *guild = this->GetGuild(guild_id);
	if (NULL == guild)
	{
		return;
	}

	guild->AddGuildKillBossNum(num);
}

void GuildManager::OnSyncCrossGuildKillRank(GuildID guild_id)
{
	Guild *guild = this->GetGuild(guild_id);
	if (NULL == guild)
	{
		return;
	}

	static int member_uid_list[MAX_MEMBER_COUNT]; memset(member_uid_list, 0, sizeof(member_uid_list));

	int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
	for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
	{
		Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
		if (NULL != temp_role)
		{
			temp_role->GetTitleManager()->CheckAddCrossGuildKillRankTitle(SYNC_REASON_RANK);
		}
	}

	guild->ResetGuildKillBossNum();
}

int GuildManager::GmDismissGuild(Guild *guild)
{
	if (NULL == guild)
	{
		return -10;
	}

	const char *log_ret = "succeed";

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	// 有仙盟活动，不能强制解散
	if (guild->IsGuildFbOpen(now_second) || guild->IsGuildBonfireOpen(now_second) || 
		guild->IsGuildStationHasActivity() || ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GONGCHENGZHAN))
	{
		return -20;
	}
	else 
	{
		GameName mengzhu_name = {0};
		UserID mengzhu_userid = guild->GetGuildRegister().GetGuildTuanZhangInfo(mengzhu_name);

		GuildMember *member = guild->GetGuildMemberManager().GetMember(mengzhu_userid);
		if (NULL != member)
		{
			if (GuildAuthority::CanDismiss(member))
			{
				this->DismissGuild(guild, mengzhu_userid, GUILD_DISMISS_TYPE_GM);
			}
		}

	}

	gamelog::g_log_guild.printf(LL_INFO, "guild -> GmDismissGuild guild[ ret:%s, guild_id:%d, dismiss_type:%d]",
		(const char*)log_ret, guild->GetGuildID(), GUILD_DISMISS_TYPE_GM);

	return 0;
}

int GuildManager::GmClearNotice(Guild *guild)
{
	if (NULL == guild)
	{
		return -100;
	}

	const char *log_ret = "succeed";

	GuildNotice empty_notice;
	memset(empty_notice, 0, sizeof(empty_notice));

	guild->ChangeNotice(empty_notice);

	gamelog::g_log_guild.printf(LL_INFO, "guild -> GmClearNotice guild[ ret:%s, guild_id:%d]",
		(const char*)log_ret, guild->GetGuildID());

	return 0;
}