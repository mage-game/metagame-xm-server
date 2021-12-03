#include <algorithm>
#include "specialcrossfishing.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/servercommon.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "protocal/msgactivity.hpp"
#include "global/team/team.hpp"
#include "other/route/mailroute.hpp"
#include "other/event/eventhandler.hpp"
#include "item/itempool.h"
#include "gamelog.h"
#include "internalcomm.h"
#include "servercommon/string/crossstr.h"
#include "gameworld/gameworld/world.h"
#include "gameworld/gameworld/gstr.h"
#include "gameworld/scene/activityshadow/activityshadow.hpp"
#include "servercommon/string/crossstr.h"
#include "other/roleactivity/roleactivity.hpp"
#include "other/cross/rolecross.h"
#include "other/specialappearance/specialappearance.hpp"
#include "gameworld/gameworld/item/knapsack.h"
#include "obj/otherobj/gatherobj.h"
#include "friend/teaminfo.hpp"
#include "other/shop/roleshop.hpp"
#include "servercommon/userprotocal/msgsystem.h"
#include "gameworld/gameworld/protocal/msgfb.h"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "config/logicconfigmanager.hpp"

SpecialCrossFishing::SpecialCrossFishing(Scene *scene)
	: SpecialLogic(scene), m_isopen(false), m_has_notice_act_close(false), m_activity_end_time(0), m_broadcast_times(0), m_next_broadcast_time(0)
{
}

SpecialCrossFishing::~SpecialCrossFishing()
{
}

void SpecialCrossFishing::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_isopen)
	{
		return;
	}

	static time_t next_update_timestamp = 0;

	time_t now_timestamp = EngineAdapter::Instance().Time();
	if (next_update_timestamp > 0 && now_timestamp < next_update_timestamp)
	{
		return;
	}

	for (FishingUserIt it = m_user_list.begin(); it != m_user_list.end(); ++it)
	{
		CrossFishingUserInfo &user_info = it->second;

		if (user_info.fishing_status == CrossFishingUserInfo::FISHING_STATUS_IDLE)
		{
			continue;
		}

		Role *role = this->GetScene()->GetRoleByUUID(LongLongToUniqueUserID(user_info.uuid));
		if (NULL == role)
		{
			user_info.fishing_status = CrossFishingUserInfo::FISHING_STATUS_IDLE;
			user_info.ClearSpecialStatus(CrossFishingUserInfo::SPECIAL_STATUS_AUTO_FISHING);

			continue;
		}

		bool is_change = false;

		// 检测一般状态
		if (user_info.fishing_status == CrossFishingUserInfo::FISHING_STATUS_HOOKED && user_info.auto_pull_timestamp > 0 && now_timestamp >= user_info.auto_pull_timestamp)
		{
			user_info.Reset();
			is_change = true;

			role->NoticeNum(errornum::EN_FISHING_GET_FISH_FAIL);
		}
		else if (user_info.fishing_status == CrossFishingUserInfo::FISHING_STATUS_CAST && user_info.event_trigger_timestamp > 0 && now_timestamp >= user_info.event_trigger_timestamp)
		{
			// 鱼儿上钩，触发事件
			this->OnEventTrigger(role);
			is_change = true;
		}

		// 检测特殊状态
		if (user_info.IsSpecialStatusSet(CrossFishingUserInfo::SPECIAL_STATUS_OIL))
		{
			if (now_timestamp >= user_info.special_status_oil_end_timestamp)
			{
				user_info.ClearSpecialStatus(CrossFishingUserInfo::SPECIAL_STATUS_OIL);
				is_change = true;
			}
		}

		if (is_change)
		{
			this->SendUserInfo(role);
			this->NotifyStatuChange(role);
		}
	}

	next_update_timestamp = EngineAdapter::Instance().Time() + 1;

	if (!m_has_notice_act_close)
	{
		UInt32 act_close_timestamp = ActivityShadow::Instance().GetCrossActivityEndTime(CROSS_ACTIVITY_TYPE_FISHING);
		if (next_update_timestamp >= act_close_timestamp - SECOND_PER_MIN)
		{
			this->NoticeActStatus();
		}
	}

	do
	{
		if (m_broadcast_times < CrossFishingConfig::Instance().GetBroadCastTimes() && 0 != m_next_broadcast_time && static_cast<unsigned int>(now_second) > m_next_broadcast_time)
		{
			unsigned int to_end_interval_s = m_activity_end_time - m_next_broadcast_time;
			int to_end_interval_min = to_end_interval_s / 60;

			int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_fishing_end_notice", to_end_interval_min);
			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;
				if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE, 0, 0))
				{
					m_scene->SendToRole((const char *)&sm, sendlen);
				}
			}

			++m_broadcast_times;

			unsigned int interval = CrossFishingConfig::Instance().GetBroadCastInterval(m_broadcast_times);
			if (0 == interval)
			{
				break;
			}

			m_next_broadcast_time += interval;
		}

	} while (false);
}

bool SpecialCrossFishing::CanDestroy()
{
	if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_FISHING))
	{
		return true;
	}
	return false;
}

int SpecialCrossFishing::GetSpecialParam(Role *role)
{
	if (nullptr == role)
	{
		return CrossFishingUserInfo::FISHING_STATUS_IDLE;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (nullptr == user_info)
	{
		return CrossFishingUserInfo::FISHING_STATUS_IDLE;
	}

	return user_info->fishing_status;
}

void SpecialCrossFishing::OnRoleEnterScene(Role *role)
{
	//初始化一些数据
	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		// 加入新玩家
		this->AddUserInfo(role);
	}

	user_info = this->GetUserInfo(uuid);
	if (nullptr == user_info)
	{
		return;
	}

	if (0 == user_info->is_init)
	{
		const CrossFishingFishBaitConfig *bait_cfg = CrossFishingConfig::Instance().GetFishBaitCfg(0);
		if (NULL == bait_cfg)
		{
			return;
		}

		//if (!role->GetKnapsack()->HaveSpace())
		//{
		//	role->NoticeNum(errornum::EN_FISHING_KNAPSACK_SPACE_NOT_ENOUGH);
		//	return;
		//}

		// 发放初级鱼饵
		ItemConfigData item_to_give(bait_cfg->item_id, true, CrossFishingConfig::Instance().GetOtherCfg().give_bait_count);
		role->GetKnapsack()->PutOrMail(item_to_give, PUT_REASON_FISHING_GIVE_BAIT);
		user_info->least_count_cfg_index = CrossFishingConfig::Instance().GetRandomLeastCountCfgIndex();

		// 活动开启时，清除法宝数据
//		role->GetRoleActivity()->SyncFishingGearNumList(user_info->gear_num_list, true);

		user_info->is_init = 1;
	}
	else
	{
		user_info->Reset();
	}

	this->SendUserInfo(role);
	this->SendStageScoreInfo(role);
	this->SendFishingRankListToRole(role);

	role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPEARANCE_TYPE_CROSS_FISHING, role->GetSex(), 0, true);
	//role->SetAttackMode(Role::ATTACK_MODE_PEACE, true, false);

	EventHandler::Instance().OnJoinFishingActivity(role);
}

void SpecialCrossFishing::OnRoleLeaveScene(Role *role, bool is_logout)
{
	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	GatherIdMap::iterator it = m_gather_id_map.find(uuid);
	if (it != m_gather_id_map.end())
	{
		Obj *obj = m_scene->GetObj(it->second);
		if (obj != nullptr && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			m_scene->DeleteObj(obj->GetId());
			it = m_gather_id_map.erase(it);
		}
	}

	role->GetSpecialAppearance()->SetFbAppearance(0, 0, 0, true);

	role->BackToOriginalServer();
}

bool SpecialCrossFishing::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (nullptr == role || nullptr == gather)
	{
		return false;
	}

	const CrossFishingOtherConfig &other_cfg = CrossFishingConfig::Instance().GetOtherCfg();

	//不能采集自己的鱼篓
	if (gather->GatherId() == other_cfg.fishing_gather_id &&
		UniqueUserIDToLongLong(role->GetUniqueUserID()) == gather->GetData())
	{
		return false;
	}

 	Role *target_role = m_scene->GetRoleByUUID(LongLongToUniqueUserID(gather->GetData()));
 	if (target_role != nullptr)
 	{
 		//target_role->NoticeNum(noticenum::NT_FISHING_WATCH_YOUR_FISH);

		Protocol::SCFishingCheckEventResult fcer;
		fcer.event_type = CrossFishingEventConfig::EVENT_TYPE_NOTICE;
		fcer.param1 = 1;

		EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char*)&fcer, sizeof(fcer));
 	}

	return true;
}

void SpecialCrossFishing::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	const CrossFishingOtherConfig &other_cfg = CrossFishingConfig::Instance().GetOtherCfg();
	if (other_cfg.fishing_gather_id == gather_id)
	{
		Obj *obj = m_scene->GetObj(obj_id);
		if (nullptr == obj || obj->GetObjType() != Obj::OBJ_TYPE_GATHER)
		{
			return;
		}

		GatherObj *gather = reinterpret_cast<GatherObj*>(obj);
		long long target_uuid = gather->GetData();

		this->OnStealFishReq(role, target_uuid);
	}
}

void SpecialCrossFishing::OnActivityStatusChange(int status)
{
	if (ACTIVITY_STATUS_OPEN == status)
	{
		this->OnActivityStart();

		m_broadcast_times = 0;
		m_next_broadcast_time = 0;

		unsigned int interval = CrossFishingConfig::Instance().GetBroadCastInterval(m_broadcast_times);
		if (interval > 0)
		{
			m_next_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + interval;
		}
	}
	else if (ACTIVITY_STATUS_CLOSE == status)
	{
		this->OnActivityClose();
	}
}

void SpecialCrossFishing::OnActivityClose()
{
	m_isopen = false;

	m_next_broadcast_time = 0;
	m_broadcast_times = 0;
	m_activity_end_time = 0;

	this->CheckRewardOnClose();

	m_user_list.clear();
	m_score_rank_vec.clear();

	for (auto& gather_it : m_gather_id_map)
	{
		Obj* obj = m_scene->GetObj(gather_it.second);
		if (obj != nullptr && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			m_scene->DeleteObj(obj->GetId());
		}
	}
	m_gather_id_map.clear();

	//踢回原服
	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			role->BackToOriginalServer();
		}
	}
}

void SpecialCrossFishing::ResetUserInfoOnFail(Role *role, const char *reason)
{
	if (NULL == role || NULL == reason)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	user_info->Reset();
	user_info->ClearSpecialStatus(CrossFishingUserInfo::SPECIAL_STATUS_AUTO_FISHING);

	this->SendUserInfo(role);
	this->NotifyStatuChange(role);

	gamelog::g_log_fishing.printf(LL_INFO, "ResetUserInfoOnFail user[%lld %s], fishing_status[%d], special_status_flag[%d], reason[%s]",
		user_info->uuid, role->GetName(), user_info->fishing_status, user_info->special_status_flag, reason);
}

static const int MAX_POS_NUM = 8;
static const int GATHER_POS_LIST[MAX_POS_NUM][2] =
{
	{ 1, 1 }, { 1, 0 }, { 1, -1 },
	{ 0, 1 }, { 0, -1 },            // 不能在自己脚下
	{ -1, 1 },{ -1, 0 },{ -1, -1 }
};

bool SpecialCrossFishing::StartFishing(Role *role)
{
	if (role == nullptr || !m_isopen)
	{
		return false;
	}

	if (NULL == this->GetScene()->GetRoleByUUID(role->GetUniqueUserID()))
	{
		role->NoticeNum(errornum::EN_FISHING_NOT_FISHING_AREA);
		return false;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return false;
	}

	//生成采集物
	GatherObj *gather = new GatherObj();
	if (gather != nullptr)
	{
		Posi pos;
		const int GET_POS_LOOPTIMES = 10;

		for (int i = 0; i < GET_POS_LOOPTIMES; ++i)
		{
			int rand_pos_seq = RandomNum(MAX_POS_NUM);
			pos.x = role->GetPos().x + (GATHER_POS_LIST[rand_pos_seq][0]);		
			pos.y = role->GetPos().y + (GATHER_POS_LIST[rand_pos_seq][1]);

			if (m_scene->GetMap()->IsValidatePos(Obj::OBJ_TYPE_ROLE, pos.x, pos.y))
			{
				break;
			}
		}

		const CrossFishingOtherConfig &other_cfg = CrossFishingConfig::Instance().GetOtherCfg();

		gather->Init(nullptr, other_cfg.fishing_gather_id, other_cfg.fishing_gather_times, 0, false);
		gather->InitCrossFishingGather(uuid, role->GetName());

		gather->SetPos(pos);
		m_scene->AddObj(gather);

		//更新玩家的鱼篓采集物id
		GatherIdMap::iterator gather_iter = m_gather_id_map.find(uuid);
		if (gather_iter != m_gather_id_map.end())
		{
			Obj *obj = m_scene->GetObj(gather_iter->second);
			if (obj != nullptr && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
			{
				m_scene->DeleteObj(obj->GetId());
			}
		}

		m_gather_id_map[uuid] = gather->GetId();
	}

	user_info->fishing_status = CrossFishingUserInfo::FISHING_STATUS_WAITING;

	// 发送玩家信息，玩家队友信息
	this->SendUserInfo(role);
/*	this->SendTeamMemberInfo(role);*/
	this->NotifyStatuChange(role);

	return true;
}

bool SpecialCrossFishing::CastingRods(Role *role, int bait_type)
{
	if (role == nullptr)
	{
		return false;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return false;
	}

	const CrossFishingFishBaitConfig *bait_cfg = CrossFishingConfig::Instance().GetFishBaitCfg(bait_type);
	if (NULL == bait_cfg)
	{
		return false;
	}

	if (user_info->fishing_status != CrossFishingUserInfo::FISHING_STATUS_WAITING && user_info->special_status_flag != 1)
	{
		//role->NoticeNum(errornum::EN_FISHING_CANT_CASTING_RODS);
		return false;
	}

	// 检查鱼饵
	if (!role->GetKnapsack()->HasItem(bait_cfg->item_id))
	{
		role->NoticeNum(errornum::EN_FISHING_FISH_BAIT_NOT_ENOUGH);
		return false;
	}

	// 计算事件触发事件
	{
		const int MIN_CD_S = CrossFishingConfig::Instance().GetOtherCfg().event_min_cd_s;
		const int MAX_CD_S = CrossFishingConfig::Instance().GetOtherCfg().event_max_cd_s;
		if (MIN_CD_S <= 0 || MAX_CD_S <= 0)
		{
			return false;
		}

		int rand_cd_base = ::RandomNum(MIN_CD_S, MAX_CD_S + 1);
		if (user_info->IsSpecialStatusSet(CrossFishingUserInfo::SPECIAL_STATUS_OIL))
		{
			if (role->GetTeamInfo()->InTeam())
			{
				rand_cd_base -= CrossFishingConfig::Instance().GetOtherCfg().oil_reduce_event_cd_s * 2;
			}
			else
			{
				rand_cd_base -= CrossFishingConfig::Instance().GetOtherCfg().oil_reduce_event_cd_s;
			}
		}

		double reduce_cd_percent = 0;
		Team *team = role->GetTeamInfo()->GetMyTeam();
		if (NULL != team)
		{
			if (team->GetMemberNum() == 2)
			{
				reduce_cd_percent = CrossFishingConfig::Instance().GetOtherCfg().reduce_cd_percent_ingroup_2 / 100.0;
			}
			else if (team->GetMemberNum() >= 3)
			{
				reduce_cd_percent = CrossFishingConfig::Instance().GetOtherCfg().reduce_cd_percent_ingroup_3 / 100.0;
			}
		}

		int real_cd_s = (int)(rand_cd_base * (1 - reduce_cd_percent));
		if (real_cd_s < MIN_CD_S)
		{
			real_cd_s = MIN_CD_S;
		}

		user_info->event_trigger_timestamp = EngineAdapter::Instance().Time() + real_cd_s;
	}

	// 设置状态
	user_info->fishing_status = CrossFishingUserInfo::FISHING_STATUS_CAST;
	user_info->use_bait_type = bait_type;

	// 发送
	this->SendUserInfo(role);
	this->NotifyStatuChange(role);

	// log
	gamelog::g_log_fishing.printf(LL_INFO, "CastingRods user[%lld %s], bait_type[%d], event_trigger_timestamp[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), bait_type, (unsigned int)user_info->event_trigger_timestamp, user_info->fishing_status);

	return true;
}

bool SpecialCrossFishing::PullRods(Role *role, bool is_perfect)
{
	if (role == nullptr)
	{
		return false;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return false;
	}

	if (user_info->fishing_status != CrossFishingUserInfo::FISHING_STATUS_HOOKED && user_info->special_status_flag != 1)
	{
		//role->NoticeNum(errornum::EN_FISHING_CANT_PULL_RODS);
		return false;
	}

	// 生成一个随机事件
	user_info->event_type = CrossFishingConfig::Instance().GetRandomEventType(role->GetTeamInfo()->InTeam(), is_perfect);

	// 如果触发了传说中的大鱼
	if (user_info->event_type == CrossFishingEventConfig::EVENT_TYPE_BIGFISH)
	{
		static Protocol::SCFishingEventBigFish febf;
		febf.owner_uid = role->GetUID();

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&febf, sizeof(febf));

		// 发给队友
		Role *member_list[MAX_TEAM_MEMBER_NUM] = { NULL };
		int member_count = role->GetTeamInfo()->GetOtherMemberInTeam(member_list, sizeof(member_list) / sizeof(member_list[0]));
		if (member_count > 0)
		{
			for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
			{
				if (NULL != member_list[i])
				{
					EngineAdapter::Instance().NetSend(member_list[i]->GetNetId(), (const char*)&febf, sizeof(febf));
				}
			}
		}
	}

	// 检查事件
	switch (user_info->event_type)
	{
	case CrossFishingEventConfig::EVENT_TYPE_GET_FISH:
	{
		this->OnCheckEventGetFish(role, user_info);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_TREASURE:
	{
		this->OnCheckEventTreasure(role, user_info);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_YUWANG:
	{
		this->OnCheckEventGotYuwang(role, user_info);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_YUCHA:
	{
		this->OnCheckEventGotYucha(role, user_info);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_OIL:
	{
		this->OnCheckEventGotOil(role, user_info);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_ROBBER:
	{
		this->OnCheckEventRubber(role, user_info);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_BIGFISH:
	{
		this->OnCheckEventBigFish(role, user_info);
	}
	break;
	}

	user_info->fishing_status = CrossFishingUserInfo::FISHING_STATUS_PULLED;

	// 发送
	this->SendUserInfo(role);
	this->NotifyStatuChange(role);

	// log
	gamelog::g_log_fishing.printf(LL_INFO, "PullRods user[%lld %s], bait_type[%d], event_type[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), user_info->use_bait_type, user_info->event_type, user_info->fishing_status);

	return true;
}

void SpecialCrossFishing::OnEventTrigger(Role *role)
{
	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	if (user_info->fishing_status != CrossFishingUserInfo::FISHING_STATUS_CAST)
	{
		return;
	}

	// 消耗鱼饵
	const CrossFishingFishBaitConfig *bait_cfg = CrossFishingConfig::Instance().GetFishBaitCfg(user_info->use_bait_type);
	if (NULL == bait_cfg)
	{
		user_info->Reset();
		this->NotifyStatuChange(role);
		return;
	}

	if (!role->GetKnapsack()->ConsumeItem(bait_cfg->item_id, 1, "SpecialCrossFishing::PullRods"))
	{
		user_info->Reset();
		this->NotifyStatuChange(role);
		role->NoticeNum(errornum::EN_FISHING_FISH_BAIT_NOT_ENOUGH);
		return;
	}

	user_info->auto_pull_timestamp = EngineAdapter::Instance().Time() + CrossFishingConfig::Instance().GetOtherCfg().pull_count_down_s;
	user_info->fishing_status = CrossFishingUserInfo::FISHING_STATUS_HOOKED;

	// log
	gamelog::g_log_fishing.printf(LL_INFO, "OnEventTrigger user[%lld %s], auto_pull_timestamp[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), (unsigned int)user_info->auto_pull_timestamp, user_info->fishing_status);
}

void SpecialCrossFishing::OnCheckEventGetFish(Role *role, CrossFishingUserInfo *user_info)
{
	// 获取一个随机鱼
	int fish_type = CrossFishingConfig::Instance().GetRandomFishType(user_info->use_bait_type);

	const CrossFishingFishConfig *fish_cfg = CrossFishingConfig::Instance().GetFishCfg(fish_type);
	if (NULL == fish_cfg)
	{
		return;
	}

	// 设置
	user_info->get_fish_type = fish_type;
	if (fish_type != FISHING_FISH_TYPE_NOFISH)
	{
		//role->NoticeNum(noticenum::NT_FISHING_EVENT_GET_FISH);
	}
	else
	{
		role->NoticeNum(errornum::EN_FISHING_GET_FISH_FAIL);
	}

	// 发送事件结果
	this->SendCheckEventResult(role, fish_type, 1, 0);

	Protocol::SCFishingGetFishBrocast fgfb;
	fgfb.uid = role->GetUID();
	fgfb.get_fish_type = fish_type;

	if (NULL != m_scene) m_scene->SendToRole((const char*)&fgfb, sizeof(fgfb));

	if (1 == fish_cfg->is_rare)
	{
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_fishing_get_fish", role->GetUID(), role->GetName(), role->GetCamp(), fish_type);
		if (length > 0)
		{
			m_scene->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	// log
	gamelog::g_log_fishing.printf(LL_INFO, "OnCheckEventGetFish user[%lld %s], fish_type[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), fish_type, user_info->fishing_status);
}

void SpecialCrossFishing::OnCheckEventTreasure(Role *role, CrossFishingUserInfo *user_info)
{
	// 发送事件结果
	this->SendCheckEventResult(role, 0, 0, 0);

	{
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_fishing_get_treasure", role->GetUID(), role->GetName(), role->GetCamp());
		if (length > 0)
		{
			//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			m_scene->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}
	}

	// log
	gamelog::g_log_fishing.printf(LL_INFO, "OnCheckEventTreasure user[%lld %s], fishing_status[%d]",
		UniqueUserIDToLongLong(role->GetUniqueUserID()), role->GetName(), user_info->fishing_status);
}

void SpecialCrossFishing::OnCheckEventGotYuwang(Role *role, CrossFishingUserInfo *user_info)
{
	++user_info->gear_num_list[FISHING_GEAR_NET];

	// 同步存储
	role->GetRoleActivity()->SyncFishingGearNumList(user_info->gear_num_list);

	// 发送事件结果
	this->SendCheckEventResult(role, FISHING_GEAR_NET, 1, 0);

	gamelog::g_log_fishing.printf(LL_INFO, "OnCheckEventFisher user[%lld %s], get_gear[%d], cur_gear_count[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), FISHING_GEAR_NET, user_info->gear_num_list[FISHING_GEAR_NET], user_info->fishing_status);
}

void SpecialCrossFishing::OnCheckEventGotYucha(Role *role, CrossFishingUserInfo *user_info)
{
	++user_info->gear_num_list[FISHING_GEAR_SPEAR];

	// 同步存储
	role->GetRoleActivity()->SyncFishingGearNumList(user_info->gear_num_list);

	// 发送事件结果
	this->SendCheckEventResult(role, FISHING_GEAR_SPEAR, 1, 0);

	gamelog::g_log_fishing.printf(LL_INFO, "OnCheckEventFisher user[%lld %s], get_gear[%d], cur_gear_count[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), FISHING_GEAR_SPEAR, user_info->gear_num_list[FISHING_GEAR_SPEAR], user_info->fishing_status);
}

void SpecialCrossFishing::OnCheckEventGotOil(Role *role, CrossFishingUserInfo *user_info)
{
	++user_info->gear_num_list[FISHING_GEAR_OIL];

	// 同步存储
	role->GetRoleActivity()->SyncFishingGearNumList(user_info->gear_num_list);

	// 发送事件结果
	this->SendCheckEventResult(role, FISHING_GEAR_OIL, 1, 0);

	gamelog::g_log_fishing.printf(LL_INFO, "OnCheckEventFisher user[%lld %s], get_gear[%d], cur_gear_count[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), FISHING_GEAR_OIL, user_info->gear_num_list[FISHING_GEAR_OIL], user_info->fishing_status);
}

void SpecialCrossFishing::OnCheckEventRubber(Role *role, CrossFishingUserInfo *user_info)
{
	int tmp_count = 0;
	int tmp_fish_num_list[FISHING_FISH_TYPE_MAX_COUNT] = { 0 };

	for (int j = 0; j < FISHING_FISH_TYPE_MAX_COUNT; ++j)
	{
		if (user_info->fish_num_list[j] > 0)
		{
			tmp_fish_num_list[tmp_count++] = j;
		}
	}

	int rand_fish_type = FISHING_FISH_TYPE_NOFISH;
	int rand_robbed_count = 0;

	if (tmp_count > 0)
	{
		int rand_index = RandomNum(tmp_count);
		rand_fish_type = tmp_fish_num_list[rand_index];

		const int max_robbed_count = CrossFishingConfig::Instance().GetOtherCfg().max_robbed_count;
		if (max_robbed_count <= 0)//不可能
		{
			return;
		}

		rand_robbed_count = RandomNum(max_robbed_count) + 1;
		rand_robbed_count = user_info->fish_num_list[rand_fish_type] > rand_robbed_count ? rand_robbed_count : user_info->fish_num_list[rand_fish_type];

		this->ChangeFishNum(role, rand_fish_type, -rand_robbed_count);
	}

	// 发送事件结果
	this->SendCheckEventResult(role, rand_fish_type, rand_robbed_count, 0);

	if (rand_fish_type > FISHING_FISH_TYPE_NOFISH)
	{
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_fishing_be_robbed", role->GetUID(), role->GetName(), role->GetCamp(), rand_fish_type, rand_robbed_count);
		if (length > 0)
		{
			//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			m_scene->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}
	}

	gamelog::g_log_fishing.printf(LL_INFO, "OnCheckEventRubber user[%lld %s], rubber_fish_type[%d], rubber_count[%d], cur_count[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), rand_fish_type, rand_robbed_count, user_info->fish_num_list[rand_fish_type], user_info->fishing_status);
}

void SpecialCrossFishing::OnCheckEventBigFish(Role *role, CrossFishingUserInfo *user_info)
{
	// 是否成功
	int helper_count = 0;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (user_info->helper_uuid_list[i] > 0)
		{
			++helper_count;
		}
	}

	int succ_rate = 0;

	if (helper_count == 1)
	{
		succ_rate = CrossFishingConfig::Instance().GetOtherCfg().get_bigfish_rate_2;
	}
	else if (helper_count > 1)
	{
		succ_rate = CrossFishingConfig::Instance().GetOtherCfg().get_bigfish_rate_3;
	}
	else
	{
		succ_rate = CrossFishingConfig::Instance().GetOtherCfg().get_bigfish_rate_1;
	}

	bool is_succ = RandomNum(100) < succ_rate;
	int get_fish_type = FISHING_FISH_TYPE_NOFISH;

	if (!is_succ)
	{
		user_info->get_fish_type = FISHING_FISH_TYPE_NOFISH;
		role->NoticeNum(errornum::EN_FISHING_BIG_FISH_PULL_FAIL);
	}
	else
	{
		// 获取一个随机鱼
		get_fish_type = CrossFishingConfig::Instance().GetRandomFishType(user_info->use_bait_type);
		if (get_fish_type == FISHING_FISH_TYPE_NOFISH)
		{
			++get_fish_type;
		}

		const CrossFishingFishConfig *fish_cfg = CrossFishingConfig::Instance().GetFishCfg(get_fish_type);
		if (NULL != fish_cfg)
		{
			user_info->get_fish_type = get_fish_type;
			role->NoticeNum(noticenum::NT_FISHING_EVENT_GET_BIG_FISH_SUCC);
		}
	}

	// 发送事件结果
	this->SendCheckEventResult(role, get_fish_type, 2, 0);

	// log
	gamelog::g_log_fishing.printf(LL_INFO, "OnCheckEventBigFish user[%lld %s], get_fish_type[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), get_fish_type, user_info->fishing_status);
}

bool SpecialCrossFishing::ConfirmEvent(Role *role, int param1)
{
	if (role == nullptr)
	{
		return false;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return false;
	}

	if (user_info->fishing_status != CrossFishingUserInfo::FISHING_STATUS_PULLED)
	{
		role->NoticeNum(errornum::EN_FISHING_CANT_CONFIRM_RESULT);
		return false;
	}

	switch (user_info->event_type)
	{
	case CrossFishingEventConfig::EVENT_TYPE_GET_FISH:
	{
		this->OnConfirmEventGetFish(role, user_info, param1);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_TREASURE:
	{
		this->OnConfirmEventTreasure(role, user_info, 0 != param1);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_YUWANG:
	case CrossFishingEventConfig::EVENT_TYPE_YUCHA:
	case CrossFishingEventConfig::EVENT_TYPE_OIL:
	{
		this->OnConfirmEventFisher(role, user_info);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_ROBBER:
	{
		this->OnConfirmEventRubber(role, user_info);
	}
	break;

	case CrossFishingEventConfig::EVENT_TYPE_BIGFISH:
	{
		this->OnConfirmEventBigFish(role, user_info, param1);
	}
	break;
	}

	int event_type_to_log = user_info->event_type;

	// 确认完毕，重置
	user_info->Reset();

	this->SendUserInfo(role);
	this->NotifyStatuChange(role);
	this->SendTeamMemberInfo(role);

	gamelog::g_log_fishing.printf(LL_INFO, "ConfirmEvent user[%lld %s], confirmed_event_type[%d], fishing_status[%d]",
		user_info->uuid, role->GetName(), event_type_to_log, user_info->fishing_status);

	return true;
}

void SpecialCrossFishing::OnConfirmEventGetFish(Role *role, CrossFishingUserInfo *user_info, int give_uid)
{
	if (user_info->get_fish_type <= FISHING_FISH_TYPE_NOFISH || user_info->get_fish_type >= FISHING_FISH_TYPE_MAX_COUNT)
	{
		return;
	}

	bool is_given_succ = false;

	// 给队友
	Role *given_role = this->GetScene()->GetRoleByUID(give_uid);;
	CrossFishingUserInfo *given_user_info = NULL;

	if (given_role != nullptr)
	{
		long long given_uuid = UniqueUserIDToLongLong(given_role->GetUniqueUserID());
		given_user_info = this->GetUserInfo(given_uuid);

		if (given_user_info != NULL)
		{
			if (role->GetTeamInfo()->IsMember(IntToUid(give_uid)))
			{
				this->ChangeFishNum(given_role, user_info->get_fish_type, 1);

				//role->NoticeNum(noticenum::NT_FISHING_CONFIRM_GET_FISH_GIVE_SUCC);
				given_role->NoticeNum(noticenum::NT_FISHING_CONFIRM_GET_FISH_BE_GIVIEN);

				is_given_succ = true;
			}
		}
	}

	// 给自己
	if (!is_given_succ)
	{
		if (give_uid > 0)
		{
			role->NoticeNum(errornum::EN_FISHING_CONFIRM_GET_FISH_MEMBER_NOT_FISHING);
		}
		else
		{
//			role->NoticeNum(noticenum::NT_FISHING_CONFIRM_GET_FISH);
		}

		this->ChangeFishNum(role, user_info->get_fish_type, 1);
	}

	// log
	if (is_given_succ)
	{
		this->SendFishInfoToTeam(given_role);

		gamelog::g_log_fishing.buff_printf("OnConfirmEventGetFish be_given, giver[%lld %s], given_role[%d %s], get_fish_type[%d], given_user_fish_num_list[",
			user_info->uuid, role->GetName(), give_uid, given_role->GetName(), user_info->get_fish_type);

		for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
		{
			gamelog::g_log_fishing.buff_printf("(%d)", given_user_info->fish_num_list[i]);
		}
		gamelog::g_log_fishing.buff_printf("], fishing_status[%d]", given_user_info->fishing_status);
		gamelog::g_log_fishing.commit_buff(LL_INFO);
	}
	else
	{
		this->SendFishInfoToTeam(role);

		gamelog::g_log_fishing.buff_printf("OnConfirmEventGetFish user[%lld %s], give_uid[%d], is_given[%d], get_fish_type[%d], fish_num_list[",
			user_info->uuid, role->GetName(), give_uid, is_given_succ ? 1 : 0, user_info->get_fish_type);

		for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
		{
			gamelog::g_log_fishing.buff_printf("(%d)", user_info->fish_num_list[i]);
		}
		gamelog::g_log_fishing.buff_printf("], fishing_status[%d]", user_info->fishing_status);
		gamelog::g_log_fishing.commit_buff(LL_INFO);
	}
}

void SpecialCrossFishing::OnConfirmEventTreasure(Role *role, CrossFishingUserInfo *user_info, bool is_open)
{
	// 1代表打开宝箱，否则为丢弃
	if (is_open)
	{
		// 发放随机奖励，背包不足则邮件发送
		const CrossFishingTreasureConfig *rand_cfg = CrossFishingConfig::Instance().GetRandomTreasureReward();
		if (NULL == rand_cfg)
		{
			return;
		}

		if (role->GetKnapsack()->HaveSpace())
		{
			role->GetKnapsack()->Put(rand_cfg->reward_item, PUT_REASON_FISHING_GIVE_TREASURE_REWARD);
			this->AddRewardItem(user_info->uuid, &rand_cfg->reward_item, 1);
		}
		else
		{
			static MailContentParam contentparam; contentparam.Reset();
			const ItemBase *reward_item_base = ITEMPOOL->GetItem(rand_cfg->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = rand_cfg->reward_item.item_id;
				contentparam.item_list[0].num = rand_cfg->reward_item.num;
				contentparam.item_list[0].is_bind = (rand_cfg->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_fishing_treasure_reward_content", CROSS_ACTIVITY_TYPE_FISHING);
				if (length > 0)
				{
					MailRoute::MailToCrossUser(role->GetUniqueUserID(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}

				role->NoticeNum(errornum::EN_FISHING_TREASURE_REWARD_NO_SPACE);
			}
		}

		gamelog::g_log_fishing.printf(LL_INFO, "OnConfirmEventTreasure user[%lld %s], open, reward_item[%s], fishing_status[%d]",
			user_info->uuid, role->GetName(), ItemConfigDataLog(&rand_cfg->reward_item, NULL), user_info->fishing_status);

		Protocol::SCFishingConfirmResult fcr;

		fcr.confirm_type = user_info->event_type;
		fcr.param_1 = rand_cfg->reward_item.item_id;
		fcr.param_2 = rand_cfg->reward_item.num;
		fcr.param_3 = rand_cfg->reward_item.is_bind;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fcr, sizeof(fcr));
	}
	else
	{
		gamelog::g_log_fishing.printf(LL_INFO, "OnConfirmEventTreasure user[%lld %s], no_open, fishing_status[%d]", user_info->uuid, role->GetName(), user_info->fishing_status);
	}
}

void SpecialCrossFishing::OnConfirmEventFisher(Role *role, CrossFishingUserInfo *user_info)
{
	gamelog::g_log_fishing.printf(LL_INFO, "OnConfirmEventFisher user[%lld %s], fishing_status[%d]", user_info->uuid, role->GetName(), user_info->fishing_status);
}

void SpecialCrossFishing::OnConfirmEventRubber(Role *role, CrossFishingUserInfo *user_info)
{
	gamelog::g_log_fishing.printf(LL_INFO, "OnConfirmEventRubber user[%lld %s], fishing_status[%d]", user_info->uuid, role->GetName(), user_info->fishing_status);
}

void SpecialCrossFishing::OnConfirmEventBigFish(Role *role, CrossFishingUserInfo *user_info, int give_uid)
{
	if (user_info->get_fish_type == FISHING_FISH_TYPE_NOFISH)
	{
		gamelog::g_log_fishing.printf(LL_INFO, "OnConfirmEventBigFish no_fish user[%lld %s]", user_info->uuid, role->GetName());
		return;
	}

	if (user_info->get_fish_type <= 0 || user_info->get_fish_type >= FISHING_FISH_TYPE_MAX_COUNT)
	{
		return;
	}

	bool is_give_other = false;

	const int GIVE_NUM = 2;

	// 给队友
	Role *given_role = this->GetScene()->GetRoleByUID(give_uid);
	CrossFishingUserInfo *given_user_info = NULL;

	if (given_role != nullptr)
	{
		long long given_uuid = UniqueUserIDToLongLong(given_role->GetUniqueUserID());
		given_user_info = this->GetUserInfo(given_uuid);

		if (NULL != given_user_info)
		{
			if (role->GetTeamInfo()->IsMember(IntToUid(give_uid)))
			{
				this->ChangeFishNum(given_role, user_info->get_fish_type, GIVE_NUM);

				//role->NoticeNum(noticenum::NT_FISHING_CONFIRM_GET_FISH_GIVE_SUCC);
				given_role->NoticeNum(noticenum::NT_FISHING_CONFIRM_GET_FISH_BE_GIVIEN);

				is_give_other = true;
			}
		}
	}

	// 给自己
	if (!is_give_other)
	{
		this->ChangeFishNum(role, user_info->get_fish_type, GIVE_NUM);

//		role->NoticeNum(noticenum::NT_FISHING_CONFIRM_GET_FISH);
	}

	// 给helper
	Role *helper_role_list[MAX_TEAM_MEMBER_NUM] = { NULL };
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		CrossFishingUserInfo *helper_user_info = this->GetUserInfo(user_info->helper_uuid_list[i]);
		if (NULL != helper_user_info)
		{
			Role *helper_role = this->GetScene()->GetRoleByUUID(LongLongToUniqueUserID(helper_user_info->uuid));
			if (NULL != helper_role)
			{
				this->ChangeFishNum(helper_role, user_info->get_fish_type, GIVE_NUM);

				this->SendFishInfoToTeam(helper_role);
				helper_role->NoticeNum(noticenum::NT_FISHING_CONFIRM_BIG_FISH_TO_HELPER);

				helper_role_list[i] = helper_role;

				// log for helper
				gamelog::g_log_fishing.buff_printf("OnConfirmEventBigFish helper_reward, helper_role[%lld %s], get_fish_type[%d], helper_user_fish_num_list[",
					helper_user_info->uuid, helper_role->GetName(), user_info->get_fish_type);
				for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
				{
					gamelog::g_log_fishing.buff_printf("(%d)", helper_user_info->fish_num_list[i]);
				}
				gamelog::g_log_fishing.buff_printf("], fishing_status[%d]", helper_user_info->fishing_status);
				gamelog::g_log_fishing.commit_buff(LL_INFO);
			}
		}
	}

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (helper_role_list[i] != NULL)
		{
			this->SendTeamMemberInfo(helper_role_list[i]);
		}
	}

	// log
	if (is_give_other && given_user_info != NULL)
	{
		this->SendTeamMemberInfo(role);

		gamelog::g_log_fishing.buff_printf("OnConfirmEventBigFish be_given, giver[%lld %s], given_role[%d %s], get_fish_type[%d], given_user_fish_num_list[",
			user_info->uuid, role->GetName(), given_role->GetUID(), given_role->GetName(), user_info->get_fish_type);

		for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
		{
			gamelog::g_log_fishing.buff_printf("(%d)", given_user_info->fish_num_list[i]);
		}
		gamelog::g_log_fishing.buff_printf("], fishing_status[%d]", given_user_info->fishing_status);
		gamelog::g_log_fishing.commit_buff(LL_INFO);
	}
	else
	{
		this->SendFishInfoToTeam(role);

		gamelog::g_log_fishing.buff_printf("OnConfirmEventBigFish user[%lld %s], give_uid[%d], is_given[%d], get_fish_type[%d], fish_num_list[",
			user_info->uuid, role->GetName(), give_uid, is_give_other ? 1 : 0, user_info->get_fish_type);

		for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
		{
			gamelog::g_log_fishing.buff_printf("(%d)", user_info->fish_num_list[i]);
		}
		gamelog::g_log_fishing.buff_printf("], fishing_status[%d]", user_info->fishing_status);
		gamelog::g_log_fishing.commit_buff(LL_INFO);
	}
}


bool SpecialCrossFishing::AddUserInfo(Role *role)
{
	if (nullptr == role)
	{
		return false;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());

	CrossFishingUserInfo *user_info = &m_user_list[uuid];
	user_info->uuid = uuid;
	user_info->prof = role->GetProf();
	role->GetName(user_info->user_name);
	user_info->fishing_status = CrossFishingUserInfo::FISHING_STATUS_IDLE;

	return true;
}

void SpecialCrossFishing::HelpBigFish(Role *helper_role, int helped_uid)
{
	if (NULL == helper_role)
	{
		return;
	}

	if (helper_role->GetUID() == helped_uid)
	{
		return;
	}

	long long helper_uuid = UniqueUserIDToLongLong(helper_role->GetUniqueUserID());
	CrossFishingUserInfo *helper_user = this->GetUserInfo(helper_uuid);
	if (NULL == helper_user)
	{
		return;
	}

	if (helper_user->fishing_status == CrossFishingUserInfo::FISHING_STATUS_IDLE)
	{
		return;
	}

	Role *be_helped_role = m_scene->GetRoleByUID(helped_uid);
	if (NULL == be_helped_role)
	{
		return;
	}

	long long be_helped_uuid = UniqueUserIDToLongLong(be_helped_role->GetUniqueUserID());
	CrossFishingUserInfo *helped_user = this->GetUserInfo(be_helped_uuid);
	if (NULL == helped_user)
	{
		return;
	}

	if (helped_user->event_type != CrossFishingEventConfig::EVENT_TYPE_BIGFISH)
	{
		return;
	}

	if (helped_user->fishing_status != CrossFishingUserInfo::FISHING_STATUS_HOOKED)
	{
		helper_role->NoticeNum(errornum::EN_FISHING_THE_HELPED_PULLED);
		return;
	}

	if (!helper_role->GetTeamInfo()->IsMember(IntToUid(helped_uid)))
	{
		return;
	}

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (helped_user->helper_uuid_list[i] == helper_uuid)
		{
			helper_role->NoticeNum(errornum::EN_FISHING_BIG_FISH_ALREADY_HELPED);
			return;
		}
	}

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (helped_user->helper_uuid_list[i] == 0)
		{
			helped_user->helper_uuid_list[i] = helper_uuid;
			helper_role->NoticeNum(noticenum::NT_FISHING_BIG_FISH_HELP_SUCC);
			be_helped_role->NoticeNum(noticenum::NT_FISHING_BIG_FISH_BE_HELPED_SUCC);

			break;
		}
	}
}

void SpecialCrossFishing::StopFishing(Role *role)
{
	if (role == nullptr)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	//去掉相关采集物
	GatherIdMap::iterator it = m_gather_id_map.find(uuid);
	if (it != m_gather_id_map.end())
	{
		Obj* obj = m_scene->GetObj(it->second);
		if (obj != nullptr && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			m_scene->DeleteObj(obj->GetId());
			it->second = INVALID_OBJ_ID;
		}
	}

	user_info->Reset();
	user_info->fishing_status = CrossFishingUserInfo::FISHING_STATUS_IDLE;
	user_info->ClearSpecialStatus(CrossFishingUserInfo::SPECIAL_STATUS_AUTO_FISHING);

	this->SendUserInfo(role);
	this->NotifyStatuChange(role);

	gamelog::g_log_fishing.printf(LL_INFO, "StopFishing user[%lld %s], fishing_status[%d], special_status_flag[%d]",
		user_info->uuid, role->GetName(), user_info->fishing_status, user_info->special_status_flag);
}

void SpecialCrossFishing::AutoFishingReq(Role *role, int is_set, int special_status)
{
	if (NULL == role || special_status < 0 || special_status > CrossFishingUserInfo::SPECIAL_STATUS_QUICK_FISHING)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	if (is_set > 0 && CrossFishingUserInfo::SPECIAL_STATUS_QUICK_FISHING == special_status && user_info->IsSpecialStatusSet(special_status))
	{
		//role->NoticeNum(errornum::EN_FISHING_QUICK_FISHING_HAS_ACTIVED);
		return;
	}

	if (0 == is_set)
	{
		// 取消自动钓鱼状态
		user_info->ClearSpecialStatus(special_status);
	}
	else
	{
		// 检查并消耗
		if (!user_info->is_consumed_auto_fishing && CrossFishingUserInfo::SPECIAL_STATUS_QUICK_FISHING == special_status)
		{
			const int need_gold = CrossFishingConfig::Instance().GetOtherCfg().auto_fishing_need_gold;
			if (!role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "SpecialCrossFishing::AutoFishingReq"))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			user_info->is_consumed_auto_fishing = 1;
		}

		// 设置自动钓鱼状态
		user_info->SetSpecialStatus(special_status);
	}

	this->SendUserInfo(role);

	gamelog::g_log_fishing.printf(LL_INFO, "AutoFishingReq user[%lld %s], is_set[%d], special_status_flag[%d], is_consumed_auto_fishing[%d]",
		user_info->uuid, role->GetName(), is_set ? 1 : 0, user_info->special_status_flag, user_info->is_consumed_auto_fishing);
}

void SpecialCrossFishing::OnRandUserReq(Role *role)
{
	if (m_user_list.empty() || role == nullptr)
	{
		return;
	}

	std::set<long long> check_uuid_list;

	int random_count = 0, temp_num = 0;

	static Protocol::SCFishingRandUserInfo frui;

	while (random_count++ < 20 && temp_num < FISHING_RAND_ROLE_NUM)
	{
		int rand_index = RandomNum(static_cast<int>(m_user_list.size()));

		FishingUserIt it = m_user_list.begin();
		std::advance(it, rand_index);

		if (check_uuid_list.end() != check_uuid_list.find(it->first) || (it->first == UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()))) continue;

		Role *role = this->GetScene()->GetRoleByUUID(LongLongToUniqueUserID(it->second.uuid));
		if (NULL == role)
		{
			continue;
		}

		check_uuid_list.insert(it->first);

		frui.user_info_list[temp_num].uid = it->second.uuid;
		frui.user_info_list[temp_num].prof = it->second.prof;
		frui.user_info_list[temp_num].least_count_cfg_index = it->second.least_count_cfg_index;
		F_STRNCPY(frui.user_info_list[temp_num].user_name, it->second.user_name, sizeof(GameName));
		memcpy(frui.user_info_list[temp_num].fish_num_list, it->second.fish_num_list, sizeof(frui.user_info_list[temp_num].fish_num_list));

		++temp_num;
	}

	frui.user_count = temp_num;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&frui, sizeof(frui));
}


bool SpecialCrossFishing::IsCanStealFish(Role *role, long long target_uuid)
{
	if (nullptr == role)
	{
		return false;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());

	if (uuid == target_uuid)
	{
		return false;
	}

	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		// 第一次进入场景，会先触发进入视野，所以加入新玩家信息
		if (!AddUserInfo(role))
		{
			return false;
		}
	}

	user_info = this->GetUserInfo(uuid);
	if (nullptr == user_info)
	{
		return false;
	}

	Role *target_role = this->GetScene()->GetRoleByUUID(LongLongToUniqueUserID(target_uuid));
	if (NULL == target_role)
	{
		return false;
	}

	CrossFishingUserInfo *target_user_info = this->GetUserInfo(target_uuid);
	if (NULL == target_user_info)
	{
		return false;
	}

	if (user_info->steal_fish_count >= (CrossFishingConfig::Instance().GetOtherCfg().steal_count + user_info->buy_steal_count))
	{
		return false;
	}

	if (target_user_info->be_stealed_fish_count >= CrossFishingConfig::Instance().GetOtherCfg().be_stealed_count)
	{
		return false;
	}

	int total_steal_rate = 0;
	for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
	{
		if (target_user_info->fish_num_list[i] > 0)
		{
			const CrossFishingFishConfig *fish_config = CrossFishingConfig::Instance().GetFishCfg(i);
			if (NULL != fish_config)
			{
				total_steal_rate += fish_config->be_stealed_rate;
			}
		}
	}

	if (total_steal_rate <= 0)
	{
		return false;
	}

	return true;
}

void SpecialCrossFishing::OnStealFishReq(Role *role, long long target_uuid)
{
	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	Role *target_role = this->GetScene()->GetRoleByUUID(LongLongToUniqueUserID(target_uuid));
	if (NULL == target_role)
	{
		role->NoticeNum(errornum::EN_FISHINT_STEAL_TARGET_NOT_IN_SCENE);
		return;
	}

	CrossFishingUserInfo *target_user_info = this->GetUserInfo(target_uuid);
	if (NULL == target_user_info)
	{
		return;
	}

	if (user_info->steal_fish_count >= (CrossFishingConfig::Instance().GetOtherCfg().steal_count + user_info->buy_steal_count))
	{
		role->NoticeNum(errornum::EN_FISHINT_STEAL_COUNT_LIMIT);
		return;
	}

	if (target_user_info->be_stealed_fish_count >= CrossFishingConfig::Instance().GetOtherCfg().be_stealed_count)
	{
		role->NoticeNum(errornum::EN_FISHINT_BE_STEALED_COUNT_LIMIT);
		return;
	}

	Protocol::SCFishingStealResult fsr;

	int total_steal_rate = 0;
	int steal_rate_list[FISHING_FISH_TYPE_MAX_COUNT] = { 0 };
	for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
	{
		if (target_user_info->fish_num_list[i] > 0)
		{
			const CrossFishingFishConfig *fish_config = CrossFishingConfig::Instance().GetFishCfg(i);
			if (NULL != fish_config)
			{
				steal_rate_list[i] = fish_config->be_stealed_rate;
				total_steal_rate += fish_config->be_stealed_rate;
			}
		}
	}

	if (total_steal_rate <= 0)
	{
		role->NoticeNum(errornum::EN_FISHING_STEAL_TARGET_HAS_NO_FISH);
		return;
	}

	if (RandomNum(100) >= CrossFishingConfig::Instance().GetOtherCfg().steal_succ_rate)
	{
		fsr.is_succ = 0;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fsr, sizeof(fsr));

		role->NoticeNum(errornum::EN_FISHINT_STEAL_FISH_FAIL);
		return;
	}

	int steal_fish_type = 0;
	int rand_rate = RandomNum(total_steal_rate);
	for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; i++)
	{
		if (steal_rate_list[i] <= 0)
		{
			continue;
		}

		if (rand_rate > steal_rate_list[i])
		{
			rand_rate -= steal_rate_list[i];
			continue;
		}

		int be_steal_num = RandomNum(1, 3);
		if (be_steal_num > target_user_info->fish_num_list[i])
		{
			be_steal_num = target_user_info->fish_num_list[i];
		}

		//被偷日志
		if (0 <= target_user_info->news_count && target_user_info->news_count < FISHING_NEWS_MAX)
		{
			role->GetName(target_user_info->news_list[target_user_info->news_count].user_name);
			target_user_info->news_list[target_user_info->news_count].fish_type = i;
			target_user_info->news_list[target_user_info->news_count].news_type = FISHING_NEWS_TYPE_BE_STEAL;
			target_user_info->news_list[target_user_info->news_count].fish_num = be_steal_num;
			++target_user_info->news_count;
		}
		else
		{
			for (int j = 0; j < FISHING_NEWS_MAX - 1; ++j)
			{
				target_user_info->news_list[j] = target_user_info->news_list[j + 1];
			}

			role->GetName(target_user_info->news_list[FISHING_NEWS_MAX - 1].user_name);
			target_user_info->news_list[FISHING_NEWS_MAX - 1].fish_type = i;
			target_user_info->news_list[FISHING_NEWS_MAX - 1].news_type = FISHING_NEWS_TYPE_BE_STEAL;
			target_user_info->news_list[FISHING_NEWS_MAX - 1].fish_num = be_steal_num;
		}

		//偷鱼日志
		if (0 <= user_info->news_count && user_info->news_count < FISHING_NEWS_MAX)
		{
			target_role->GetName(user_info->news_list[user_info->news_count].user_name);
			user_info->news_list[user_info->news_count].fish_type = i;
			user_info->news_list[user_info->news_count].news_type = FISHING_NEWS_TYPE_STEAL;
			user_info->news_list[user_info->news_count].fish_num = be_steal_num;
			++user_info->news_count;
		}
		else
		{
			for (int j = 0; j < FISHING_NEWS_MAX - 1; ++j)
			{
				user_info->news_list[j] = user_info->news_list[j + 1];
			}

			target_role->GetName(user_info->news_list[FISHING_NEWS_MAX - 1].user_name);
			user_info->news_list[FISHING_NEWS_MAX - 1].fish_type = i;
			user_info->news_list[FISHING_NEWS_MAX - 1].news_type = FISHING_NEWS_TYPE_STEAL;
			user_info->news_list[FISHING_NEWS_MAX - 1].fish_num = be_steal_num;
		}

		this->ChangeFishNum(role, i, be_steal_num,true,true);
		++user_info->steal_fish_count;

		this->ChangeFishNum(target_role, i, -be_steal_num,false,true);
		++target_user_info->be_stealed_fish_count;

		{
			Protocol::SCFishingStealInfo fsi;
			role->GetName(fsi.thief_name);
			target_role->GetName(fsi.be_stolen_name);
			fsi.fish_type = i;
			fsi.fish_num = be_steal_num;

			EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char *)&fsi, sizeof(fsi));

			this->NotifyStatuChange(target_role);
		}

		steal_fish_type = i;

		this->SendUserInfo(target_role);
		this->SendUserInfo(role);
		role->NoticeNum(noticenum::NT_FISHING_STEAL_FISH_SUCC);

		fsr.is_succ = 1;
		fsr.fish_type = i;
		fsr.fish_num = be_steal_num;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fsr, sizeof(fsr));

		break; //偷到了就break
	}

	{
		//传闻
		const CrossFishingFishConfig *fish_config = CrossFishingConfig::Instance().GetFishCfg(steal_fish_type);
		if (NULL != fish_config && (1 == fish_config->is_rare))
		{
			int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_fishing_steal_fish", role->GetUID(), role->GetName(), role->GetCamp(), target_role->GetUID(), target_role->GetName(), target_role->GetCamp(), steal_fish_type);
			if (length > 0)
			{
				m_scene->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE);
			}
		}
	}
}

void SpecialCrossFishing::OnBuyStealCountReq(Role *role)
{
	if (role == nullptr)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	if (user_info->buy_steal_count >= CrossFishingConfig::Instance().GetMaxBuyStealTimes() || user_info->buy_steal_count >= FISHING_STEAL_COUNT_BUY_MAX)
	{
		role->NoticeNum(errornum::EN_FB_DAILYTIMES_BUY_TIMES_MAX);
		return;
	}

	int consume_gold = CrossFishingConfig::Instance().GetStealBuyCountGold(user_info->buy_steal_count);
	if (consume_gold <= 0)
	{
		return;
	}
	if (consume_gold > 0 && !role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "OnCrossFinshingBuyStealCount"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	++user_info->buy_steal_count;
	this->SendUserInfo(role);

	gamelog::g_log_fishing.printf(LL_INFO, "OnBuyStealCountReq user[%lld %s], consume_gold[%d], buy_steal_count[%d]",
		user_info->uuid, role->GetName(), consume_gold, user_info->buy_steal_count);
}

void SpecialCrossFishing::OnExChange(Role* role, int combination_index)
{
	if (role == nullptr)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		role->NoticeNum(errornum::EN_FISHING_FISH_NOT_ENOUGH);
		return;
	}

	const CrossFishingFishLeastCountConfig *least_cfg = CrossFishingConfig::Instance().GetLeastCountCfg(combination_index);
	if (nullptr == least_cfg) return;

	bool is_satisfied = true;
	for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
	{
		if (user_info->fish_num_list[i] < least_cfg->least_count_list[i])
		{
			is_satisfied = false;
			break;
		}
	}

	if (!is_satisfied)
	{
		role->NoticeNum(errornum::EN_FISHING_FISH_NOT_ENOUGH);
		return;
	}

	for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
	{
		this->ChangeFishNum(role, i, -least_cfg->least_count_list[i], false);
	}

	//user_info->total_score += least_cfg->reward_score;
	this->UpdateScoreRank(role);

	role->GetKnapsack()->PutOrMail(least_cfg->reward_item, PUT_REASON_FISHING_EXCHANGE);
	this->AddRewardItem(user_info->uuid, &least_cfg->reward_item, 1);
	role->NoticeNum(noticenum::NT_FISHING_EXCHANGE_SUCC);

	this->SendUserInfo(role);
}

void SpecialCrossFishing::OnBuyBait(Role* role, int bait_type, int num)
{
	if (role == nullptr) return;

	if (num > MAX_BUY_COUNT)
	{
		role->NoticeNum(errornum::EN_BUY_TOO_MUCH_ONCE);
		return;
	}

	const CrossFishingFishBaitConfig *bait_cfg = CrossFishingConfig::Instance().GetFishBaitCfg(bait_type);
	if (nullptr == bait_cfg) return;

	const ItemBase *item_base = ITEMPOOL->GetItem(bait_cfg->item_id);
	if (nullptr == item_base) return;

	int need_grid = num / item_base->GetPileLimit();
	if (num % item_base->GetPileLimit() != 0)
	{
		++need_grid;
	}

	int total_gold = bait_cfg->gold_price * num;
	if (total_gold <= 0 || !role->GetKnapsack()->GetMoney()->AllGoldMoreThan(total_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(total_gold, "SpecialCrossFishing::OnBuyBait"))
	{
		return;
	}

	ItemConfigData put_item(bait_cfg->item_id, true, num);
	role->GetKnapsack()->Put(put_item, PUT_REASON_FISHING_BUY_BAIT);
	role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);
}

void SpecialCrossFishing::OnFetchScoreReward(Role * role)
{
	if (role == nullptr)
	{
		return;
	}

	CrossFishingUserInfo *user_info = this->GetUserInfo(UniqueUserIDToLongLong(role->GetUniqueUserID()));
	if (!user_info)
	{
		return;
	}

	auto reward_cfg = CrossFishingConfig::Instance().GetFishingScoreReward(user_info->cur_score_stage);
	if (!reward_cfg)
	{
		//role->NoticeNum(errornum::EN_FB_EXP_NOT_REWARD_TO_FETCH);
		return;
	}

	const int BIT_COUNT = BIT_COUNT_OF_BYTE * sizeof(user_info->score_stage_fetch_flag);
	if (user_info->cur_score_stage < 0 || user_info->cur_score_stage >= BIT_COUNT)
	{
		return;
	}

	if ((user_info->score_stage_fetch_flag & (1 << user_info->cur_score_stage)) != 0)
	{
		//role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	if (user_info->total_score < reward_cfg->need_score)
	{
		//role->NoticeNum(errornum::EN_SCORE_NOT_ENOUGH);
		return;
	}

	user_info->score_stage_fetch_flag |= (1 << user_info->cur_score_stage);
	//long long reward_exp = role->RecalcExp(reward_cfg->exp_reward);

	//策划需求，修改增加经验规则（2018-1-3）
	long long reward_exp = static_cast<long long>(reward_cfg->exp_factor_stage * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));

	if (role->AddExp(reward_exp, "FishingScoreReward", Role::EXP_ADD_REASON_EXP_FISH_POOL) && role->GetKnapsack()->PutListOrMail(reward_cfg->item_list, PUT_REASON_FISH_SCORE_REWARD))
	{
		this->AddRewardItem(user_info->uuid, reward_cfg->item_list, reward_cfg->item_count);

		auto next_stage_reward_cfg = CrossFishingConfig::Instance().GetFishingScoreReward(user_info->cur_score_stage + 1);
		if (next_stage_reward_cfg)
		{
			++user_info->cur_score_stage;
			//user_info->total_score += next_stage_reward_cfg->reward_score;
		}

		this->UpdateScoreRank(role);
	}
	this->SendStageScoreInfo(role);
}

CrossFishingUserInfo * SpecialCrossFishing::GetUserInfo(long long uid)
{
	FishingUserIt it = m_user_list.find(uid);
	if (it != m_user_list.end())
	{
		return &it->second;
	}

	return NULL;
}

void SpecialCrossFishing::SendUserInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	static Protocol::SCFishingUserInfo fui;

	fui.uuid = uuid;
	fui.fishing_status = user_info->fishing_status;
	fui.special_status_flag = user_info->special_status_flag;
	fui.least_count_cfg_index = user_info->least_count_cfg_index;
	fui.is_fish_event = (user_info->event_type == CrossFishingEventConfig::EVENT_TYPE_GET_FISH) ? 1 : 0;
	fui.is_consumed_auto_fishing = user_info->is_consumed_auto_fishing;
	memset(fui.reserve_ch, 0, sizeof(fui.reserve_ch));
	fui.auto_pull_timestamp = (unsigned int)user_info->auto_pull_timestamp;
	fui.special_status_oil_end_timestamp = (unsigned int)user_info->special_status_oil_end_timestamp;

	memcpy(fui.fish_num_list, user_info->fish_num_list, sizeof(fui.fish_num_list));
	memcpy(fui.gear_num_list, user_info->gear_num_list, sizeof(fui.gear_num_list));

	fui.steal_fish_count = user_info->steal_fish_count;
	fui.be_stealed_fish_count = user_info->be_stealed_fish_count;
	fui.buy_steal_count = user_info->buy_steal_count;

	fui.news_count = user_info->news_count;
	memcpy(fui.news_list, user_info->news_list, sizeof(fui.news_list));

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fui, sizeof(fui));
}

void SpecialCrossFishing::SendTeamMemberInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCFishingTeamMemberInfo ftmi;

	ftmi.member_uid_1 = 0;
	ftmi.least_count_cfg_index_1 = 0;
	memset(ftmi.fish_num_list_1, 0, sizeof(ftmi.fish_num_list_1));

	ftmi.member_uid_2 = 0;
	ftmi.least_count_cfg_index_2 = 0;
	memset(ftmi.fish_num_list_2, 0, sizeof(ftmi.fish_num_list_2));

	ftmi.member_uid_3 = 0;
	ftmi.least_count_cfg_index_3 = 0;
	memset(ftmi.fish_num_list_3, 0, sizeof(ftmi.fish_num_list_3));


	Role *member_list[MAX_TEAM_MEMBER_NUM] = { NULL };
	int member_count = role->GetTeamInfo()->GetOtherMemberInTeam(member_list, sizeof(member_list) / sizeof(member_list[0]));
	ftmi.member_count = member_count;
	if (member_count > 0)
	{
		if (NULL != member_list[0])
		{
			ftmi.member_uid_1 = member_list[0]->GetUID();

			CrossFishingUserInfo *member_user_info_1 = this->GetUserInfo(member_list[0]->GetUID());
			if (NULL != member_user_info_1)
			{
				ftmi.least_count_cfg_index_1 = member_user_info_1->least_count_cfg_index;
				memcpy(ftmi.fish_num_list_1, member_user_info_1->fish_num_list, sizeof(ftmi.fish_num_list_1));
			}
		}

		if (NULL != member_list[1])
		{
			ftmi.member_uid_2 = member_list[1]->GetUID();

			CrossFishingUserInfo *member_user_info_2 = this->GetUserInfo(member_list[1]->GetUID());
			if (NULL != member_user_info_2)
			{
				ftmi.least_count_cfg_index_2 = member_user_info_2->least_count_cfg_index;
				memcpy(ftmi.fish_num_list_2, member_user_info_2->fish_num_list, sizeof(ftmi.fish_num_list_2));
			}
		}

		if (NULL != member_list[2])
		{
			ftmi.member_uid_3 = member_list[2]->GetUID();

			CrossFishingUserInfo *member_user_info_3 = this->GetUserInfo(member_list[2]->GetUID());
			if (NULL != member_user_info_3)
			{
				ftmi.least_count_cfg_index_3 = member_user_info_3->least_count_cfg_index;
				memcpy(ftmi.fish_num_list_3, member_user_info_3->fish_num_list, sizeof(ftmi.fish_num_list_3));
			}
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ftmi, sizeof(ftmi));
}

void SpecialCrossFishing::SendFishInfoToTeam(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	static Protocol::SCFishingFishInfo ffi;
	ffi.uid = role->GetUID();
	ffi.least_count_cfg_index = user_info->least_count_cfg_index;
	memcpy(ffi.fish_num_list, user_info->fish_num_list, sizeof(ffi.fish_num_list));

	Role *member_list[MAX_TEAM_MEMBER_NUM] = { NULL };
	int member_count = role->GetTeamInfo()->GetOtherMemberInTeam(member_list, sizeof(member_list) / sizeof(member_list[0]));
	if (member_count > 0)
	{
		for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			if (member_list[i] != NULL)
			{
				EngineAdapter::Instance().NetSend(member_list[i]->GetNetId(), (const char*)&ffi, sizeof(ffi));
			}
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ffi, sizeof(ffi));
}

void SpecialCrossFishing::SendCheckEventResult(Role *role, int param1, int param2, int param3)
{
	if (NULL == role)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	static Protocol::SCFishingCheckEventResult fcer;
	fcer.event_type = user_info->event_type;
	fcer.param1 = param1;
	fcer.param2 = param2;
	fcer.param3 = param3;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fcer, sizeof(fcer));
}

void SpecialCrossFishing::UseFishingGear(Role *role, int type)
{
	if (role == nullptr)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	if (user_info->fishing_status == CrossFishingUserInfo::FISHING_STATUS_IDLE)
	{
		return;
	}

	if (type < 0 || type >= FISHING_GEAR_MAX_COUNT)
	{
		return;
	}

	// 检查并消耗法宝
	if (user_info->gear_num_list[type] <= 0)
	{
		if (FISHING_GEAR_NET == type)
		{
			role->NoticeNum(errornum::EN_FISHING_NET_NOT_ENOUGH);
		}
		else if (FISHING_GEAR_SPEAR == type)
		{
			role->NoticeNum(errornum::EN_FISHING_SPEAR_NOT_ENOUGH);
		}
		else if (FISHING_GEAR_OIL == type)
		{
			role->NoticeNum(errornum::EN_FISHING_OIL_NOT_ENOUGH);
		}
		else
		{
			role->NoticeNum(errornum::EN_FISHING_GEAR_NOT_ENOUGH);
		}

		return;
	}
	--user_info->gear_num_list[type];

	// 同步存储
	role->GetRoleActivity()->SyncFishingGearNumList(user_info->gear_num_list);

	static Protocol::SCFishingGearUseResult fgui;
	fgui.gear_type = type;
	fgui.param1 = 0;
	fgui.param2 = 0;
	fgui.param3 = 0;

	// 使用
	switch (type)
	{
	case FISHING_GEAR_NET:
	{
		// 渔网，随机获取N条同类鱼
		const int get_max_count = CrossFishingConfig::Instance().GetOtherCfg().net_get_fish_max_count;

		//int rand_fish_type = RandomNum(FISHING_FISH_TYPE_MAOXIE) + 1;			//当前版本只有1-5
		// 策划要求 按照鱼饵类型0的概率来算
		int rand_fish_type = CrossFishingConfig::Instance().GetRandomFishType(0);

		fgui.param1 = rand_fish_type;
		fgui.param2 = get_max_count;

		this->ChangeFishNum(role, rand_fish_type, get_max_count);
	}
	break;

	case FISHING_GEAR_SPEAR:
	{
		// 鱼叉，获取一条鱼
		const int get_max_count = CrossFishingConfig::Instance().GetOtherCfg().spear_get_fish_max_count;

		// 策划要求 按照鱼饵类型0的概率来算
		//int rand_fish_type = RandomNum(FISHING_FISH_TYPE_MAOXIE) + 1;		//当前版本只有1-5
		int rand_fish_type = CrossFishingConfig::Instance().GetRandomFishType(0);

		fgui.param1 = rand_fish_type;
		fgui.param2 = get_max_count;

		this->ChangeFishNum(role, rand_fish_type, get_max_count);
	}
	break;

	case FISHING_GEAR_OIL:
	{
		// 香油，设置特殊状态
		const CrossFishingOtherConfig &other_cfg = CrossFishingConfig::Instance().GetOtherCfg();
		user_info->SetSpecialStatus(CrossFishingUserInfo::SPECIAL_STATUS_OIL);
		user_info->special_status_oil_end_timestamp = user_info->special_status_oil_end_timestamp > EngineAdapter::Instance().Time() ? 
					user_info->special_status_oil_end_timestamp + other_cfg.special_status_oil_duration_s : EngineAdapter::Instance().Time() + other_cfg.special_status_oil_duration_s;

		fgui.param1 = 1;
	}
	break;
	}

	// 发送
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fgui, sizeof(fgui));
	this->SendUserInfo(role);
	this->SendFishInfoToTeam(role);

	// log
	gamelog::g_log_fishing.buff_printf("UseFishingGear user[%lld %s], type[%d], special_status_flag[%d], special_status_oil_end_timestamp[%d], fish_num_list[",
		user_info->uuid, role->GetName(), type, user_info->special_status_flag, (unsigned int)user_info->special_status_oil_end_timestamp);

	for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
	{
		gamelog::g_log_fishing.buff_printf("(%d)", user_info->fish_num_list[i]);
	}
	gamelog::g_log_fishing.buff_printf("], gear_num_list[");

	for (int i = 0; i < FISHING_GEAR_MAX_COUNT; ++i)
	{
		gamelog::g_log_fishing.buff_printf("(%d)", user_info->gear_num_list[i]);
	}
	gamelog::g_log_fishing.buff_printf("]");

	gamelog::g_log_fishing.commit_buff(LL_INFO);
}

void SpecialCrossFishing::UpdateScoreRank(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	FishingUserIt it = m_user_list.find(uuid);
	if (it == m_user_list.end())
	{
		return;
	}

	CrossFishingUserInfo &user_info = it->second;

	// 更新排行榜数据并排序
	bool is_new = true;

	for (auto& score_it : m_score_rank_vec)
	{
		if (score_it.uuid == uuid)
		{
			score_it.total_score = user_info.total_score;
			is_new = false;
		}
	}

	if (is_new)
	{
		ScoreRankItem rank_item;
		rank_item.uuid = user_info.uuid;
		rank_item.total_score = user_info.total_score;

		m_score_rank_vec.push_back(rank_item);
	}

	std::sort(m_score_rank_vec.begin(), m_score_rank_vec.end(), [](const ScoreRankItem& v1, const ScoreRankItem& v2) { return v1.total_score > v2.total_score; });

	this->SendFishingRankListToRole();
}

void SpecialCrossFishing::SendFishingRankListToRole(Role *role)
{
	static Protocol::SCFishingScoreRankList cfsrl;
	cfsrl.fish_rank_count = 0;

	for (auto& rank_it : m_score_rank_vec)
	{
		if (cfsrl.fish_rank_count >= FISHING_SCORE_MAX_RANK)
		{
			break;
		}

		CrossFishingUserInfo *p_info = this->GetUserInfo(rank_it.uuid);
		if (NULL == p_info)
		{
			continue;
		}

		cfsrl.fish_rank_list[cfsrl.fish_rank_count].uid = rank_it.uuid;
		cfsrl.fish_rank_list[cfsrl.fish_rank_count].total_score = rank_it.total_score;
		F_STRNCPY(cfsrl.fish_rank_list[cfsrl.fish_rank_count].name, p_info->user_name, sizeof(cfsrl.fish_rank_list[cfsrl.fish_rank_count].name));
		++cfsrl.fish_rank_count;
	}

	int send_len = sizeof(cfsrl) - (FISHING_SCORE_MAX_RANK - cfsrl.fish_rank_count) * sizeof(cfsrl.fish_rank_list[0]);

	if (role != nullptr)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cfsrl, send_len);
	}
	else
	{
		m_scene->SendToRole((const char *)&cfsrl, send_len);
	}
}

void SpecialCrossFishing::GmAddGear(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (NULL == user_info)
	{
		return;
	}

	for (int i = 0; i < FISHING_GEAR_MAX_COUNT; ++i)
	{
		user_info->gear_num_list[i] = 100;
	}

	this->SendUserInfo(role);
}

void SpecialCrossFishing::SendStageScoreInfo(Role *role)
{
	if (nullptr == role) return;

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (nullptr == user_info)
	{
		return;
	}

	Protocol::SCFishingScoreStageInfo fssi;

	fssi.cur_score_stage = user_info->cur_score_stage;
	fssi.fishing_score = user_info->total_score;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fssi, sizeof(fssi));
}

void SpecialCrossFishing::NotifyStatuChange(Role *role)
{
	if (nullptr == role) return;

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (nullptr == user_info)
	{
		return;
	}

	Protocol::SCFishingNotifyStatusChange fnsc;
	fnsc.uuid = uuid;
	fnsc.obj_id = role->GetId();
	fnsc.status = user_info->fishing_status;
	fnsc.be_stealed_fish_count = user_info->be_stealed_fish_count;

	UNSTD_STATIC_CHECK(sizeof(fnsc.fish_num_list) == sizeof(user_info->fish_num_list));
	memcpy(fnsc.fish_num_list, user_info->fish_num_list, sizeof(fnsc.fish_num_list));

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), &fnsc, sizeof(Protocol::SCFishingNotifyStatusChange));
}

void SpecialCrossFishing::ChangeFishNum(Role *role, int fish_type, int num, bool is_chg_score, bool is_steal)
{
	if (nullptr == role || fish_type <= 0 || fish_type >= FISHING_FISH_TYPE_MAX_COUNT || 0 == num)
	{
		return;
	}

	CrossFishingUserInfo *user_info = this->GetUserInfo(UniqueUserIDToLongLong(role->GetUniqueUserID()));
	if (nullptr == user_info)
	{
		return;
	}

	const CrossFishingFishConfig *fish_cfg = CrossFishingConfig::Instance().GetFishCfg(fish_type);
	if (nullptr == fish_cfg)
	{
		return;
	}

	user_info->fish_num_list[fish_type] += num;
	if (is_chg_score)
	{
		if (is_steal)
		{
			user_info->total_score += num * fish_cfg->steal_score;
		}
		else
		{
			user_info->total_score += num * fish_cfg->score;
		}
		this->OnFetchScoreReward(role);
	}

	this->UpdateScoreRank(role);

	this->SendStageScoreInfo(role);
	this->SendUserInfo(role);
}

void SpecialCrossFishing::NoticeActStatus()
{
	if (m_has_notice_act_close)
	{
		return;
	}

	int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_fishing_activity_notice_content", CROSS_ACTIVITY_TYPE_FISHING);

	Protocol::SCSystemMsg sm;
	SceneManager::CreateSystemMsg(&sm, &length, gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL_2);

	m_scene->SendToRole((const char*)&sm, length);

	m_has_notice_act_close = true;
}

void SpecialCrossFishing::CheckRewardOnClose()
{
	//排行名次通知原服
	crossgameprotocal::GameCrossFishingInfoSync gcfis;

	std::sort(m_score_rank_vec.begin(), m_score_rank_vec.end(), [](const ScoreRankItem &v1, const ScoreRankItem &v2) { return v1.total_score > v2.total_score; });
	int rank = 0;
	for (auto &rank_it : m_score_rank_vec)
	{
		gcfis.unique_user_id = rank_it.uuid;
		gcfis.rank = rank + 1;
		++rank;

		InternalComm::Instance().SendToCross((const char *)&gcfis, sizeof(gcfis));
	}

	if (m_score_rank_vec.size() == 1)
	{
		CrossFishingUserInfo * rank_user = GetUserInfo(m_score_rank_vec[0].uuid);
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_frishing_rank_top1", rank_user->user_name);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL_2);
		}
	}
	else if(m_score_rank_vec.size() == 2)
	{
		CrossFishingUserInfo * rank_user = GetUserInfo(m_score_rank_vec[0].uuid);
		CrossFishingUserInfo * rank_user2 = GetUserInfo(m_score_rank_vec[1].uuid);
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_frishing_rank_top2",  rank_user->user_name,rank_user2->user_name);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL_2);
		}
	}
	else if (m_score_rank_vec.size() >= 3)
	{
		CrossFishingUserInfo * rank_user = GetUserInfo(m_score_rank_vec[0].uuid);
		CrossFishingUserInfo * rank_user2 = GetUserInfo(m_score_rank_vec[1].uuid);
		CrossFishingUserInfo * rank_user3 = GetUserInfo(m_score_rank_vec[2].uuid);

		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_frishing_rank_top3",  rank_user->user_name,
			rank_user2->user_name,rank_user3->user_name);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL_2);
		}
	}
// 	static MailContentParam contentparam;

// 	// 发放日常兑换积分
// 	for (FishingUserIt it = m_user_list.begin(); it != m_user_list.end(); ++it)
// 	{
// 		CrossFishingUserInfo &user_info = it->second;
// 
// 		if (user_info.total_score > 0)
// 		{
// 			Role *role = this->GetScene()->GetRoleByUUID(LongLongToUniqueUserID(user_info.uuid));
// 			if (NULL != role)
// 			{
// 				role->GetRoleShop()->AddChestShopDailyScore(user_info.total_score, __FUNCTION__);
// 			}
// 			else
// 			{
// 				contentparam.Reset();
// 				contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_CONVERTSHOP_DAILYSCORE] = user_info.total_score;
//				int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_fishing_score_reward_mail_content", CROSS_ACTIVITY_TYPE_FISHING, user_info.total_score);
// 
// 				MailRoute::MailToCrossUser(LongLongToUniqueUserID(user_info.uuid), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 			}
// 
// 			// log
// 			gamelog::g_log_fishing.printf(LL_INFO, "OnActivityClose GiveScoreReward user[%lld], is_reward_by_mail[%d], get_score[%d].",
// 				user_info.uuid, (NULL != role ? 0 : 1), user_info.total_score);
// 		}

// 		//积分奖励
// 		{
// 			contentparam.Reset();
// 
// 			SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_fishing_score_item_reward_mail_content,
// 				CROSS_ACTIVITY_TYPE_FISHING, user_info.total_score);
// 
// 			const int max_stage_cfg_count = CrossFishingConfig::Instance().GetFishingScoreCfgCount();
// 			for (int stage = user_info.cur_score_stage; stage < max_stage_cfg_count && stage < CROSS_FISHING_SCORE_REWARD_CFG_MAX_NUM; ++stage)
// 			{
// 				if ((user_info.score_stage_fetch_flag & (1 << stage)) != 0)
// 				{
// 					continue;
// 				}
// 
// 				auto reward_cfg = CrossFishingConfig::Instance().GetFishingScoreReward(stage);
// 				if (!reward_cfg)
// 				{
// 					break;
// 				}
// 
// 				if (user_info.total_score < reward_cfg->need_score)
// 				{
// 					break;
// 				}
// 
// 				user_info.score_stage_fetch_flag |= (1 << stage);
// 
// 				//合并物品邮件补发，
// 				bool is_put = false;
// 				for (int i = 0; i < reward_cfg->item_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
// 				{
// 					is_put = false;
// 
// 					for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
// 					{
// 						if (contentparam.item_list[j].item_id == reward_cfg->item_list[i].item_id)
// 						{
// 							contentparam.item_list[j].num += reward_cfg->item_list[i].num;
// 							is_put = true;
// 						}
// 
// 						if (!is_put && contentparam.item_list[j].Invalid())
// 						{
// 							contentparam.AddItem(reward_cfg->item_list[i].item_id, reward_cfg->item_list[i].num, reward_cfg->item_list[i].is_bind);
// 							is_put = true;
// 						}
// 
// 						if (is_put) break;
// 					}
// 
// 					if (!is_put)
// 					{
//  						MailRoute::MailToCrossUser(LongLongToUniqueUserID(user_info.uuid), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 						contentparam.Reset();
//  
// 						int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_fishing_score_item_reward_mail_content", CROSS_ACTIVITY_TYPE_FISHING, user_info.total_score);
// 
// 						contentparam.AddItem(reward_cfg->item_list[i].item_id, reward_cfg->item_list[i].num, reward_cfg->item_list[i].is_bind);
// 					}
// 
// 					this->AddRewardItem(user_info.uuid, reward_cfg->item_list, reward_cfg->item_count);
// 				}
// 
// 				user_info.cur_score_stage = stage;
// 			}
// 
// 			if (contentparam.item_count > 0)
// 			{
// 				MailRoute::MailToCrossUser(LongLongToUniqueUserID(user_info.uuid), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 			}
// 		}

// 		// 奖励下发
// 		Role *temp_role = m_scene->GetRoleByUUID(LongLongToUniqueUserID(user_info.uuid));
// 		if (temp_role != nullptr)
// 		{
// 			Protocol::SCCommonFbGetRewardInfo cmd;
// 
// 			cmd.fb_type = Protocol::COMMON_FB_GET_REWARD_TYPE_CROSS_FISHING;
// 			cmd.param_1 = user_info.total_score;
// 			cmd.param_2 = 0;
// 
// 			cmd.item_count = 0;
// 			for (auto &item : user_info.get_reward_item_vec)
// 			{
// 				cmd.item_list[cmd.item_count].item_id = item.item_id;
// 				cmd.item_list[cmd.item_count].num = item.num;
// 				cmd.item_list[cmd.item_count].is_bind = item.is_bind;
// 				++cmd.item_count;
// 				if (cmd.item_count >= Protocol::SCCommonFbGetRewardInfo::MAX_ITEM_COUNT)
// 				{
// 					break;
// 				}
// 			}
// 
// 			for (int i = 0; i < CROSS_MINING_TOP_RANK_COUNT_MAX && i < (int)m_score_rank_vec.size() &&
// 				cmd.item_count < Protocol::SCCommonFbGetRewardInfo::MAX_ITEM_COUNT; ++i)
// 			{
// 				if (user_info.uuid == m_score_rank_vec[i].uuid)
// 				{
// 					cmd.item_list[cmd.item_count].item_id = CrossFishingConfig::Instance().GetOtherCfg().rank_title_item_id;
// 					cmd.item_list[cmd.item_count].num = 1;
// 					cmd.item_list[cmd.item_count].is_bind = 1;
// 					++cmd.item_count;
// 					break;
// 				}
// 			}
// 
// 			int send_len = UNSTD_OFFSETOFF(Protocol::SCCommonFbGetRewardInfo, item_list) + sizeof(cmd.item_list[0]) * cmd.item_count;
// 			EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char *)&cmd, send_len);
// 		}
//	}
}

void SpecialCrossFishing::AddRewardItem(long long uuid, const ItemConfigData *reward_item_list, int put_count)
{
	if (nullptr == reward_item_list)
	{
		return;
	}

	CrossFishingUserInfo *user_info = this->GetUserInfo(uuid);
	if (nullptr == user_info)
	{
		return;
	}

	if (user_info->get_reward_item_vec.empty())
	{
		return;
	}

	bool is_put = false;
	for (int i = 0; i < put_count && i < 100; ++i)
	{
		is_put = false;

		for (auto &item_it : user_info->get_reward_item_vec)
		{
			if (item_it.item_id == reward_item_list[i].item_id)
			{
				is_put = true;
				item_it.num += reward_item_list[i].num;
				break;
			}
		}

		if (!is_put)
		{
			user_info->get_reward_item_vec.push_back(reward_item_list[i]);
		}
	}
}
