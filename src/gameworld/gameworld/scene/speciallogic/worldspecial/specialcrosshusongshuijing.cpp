#include "specialcrosshusongshuijing.hpp"
#include "servercommon/serverconfig/husongshuijingconfig.hpp"
#include "gameworld/obj/otherobj/gatherobj.h"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/multimount/multimount.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "gameworld/item/knapsack.h"
#include "gameworld/internalcomm.h"
#include "scene/generater/gathergenerater.h"

SpecialCrossHusongShuijing::SpecialCrossHusongShuijing(Scene *scene)
	: SpecialLogic(scene), m_isopen(false), m_finish(false)
{

}

SpecialCrossHusongShuijing::~SpecialCrossHusongShuijing()
{

}

void SpecialCrossHusongShuijing::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);
	if (!m_isopen) return;

	for (int i = 0; i < HusongShuijingConfig::Instance().GetGatherTypeCount() && i < static_cast<int>(m_refresh_control_vec.size()); i++)
	{
		if (0 == m_refresh_control_vec[i].cur_remain_gather_times)
		{
			this->RefreshGatherInfo(m_refresh_control_vec[i].gather_id);
		}
	}

}

void SpecialCrossHusongShuijing::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{

}

void SpecialCrossHusongShuijing::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	Obj * kill_obj = m_scene->GetObj(killer_objid);
	if (NULL == dead_cha || NULL == kill_obj || Obj::OBJ_TYPE_ROLE != dead_cha->GetObjType() || Obj::OBJ_TYPE_ROLE != kill_obj->GetObjType())
	{
		return;
	}

	Role * die_role = dynamic_cast<Role *>(dead_cha);
	Role * killer_role = dynamic_cast<Role *>(kill_obj);
	
	if (NULL == killer_role || Role::MOVE_MODE_FLY == killer_role->GetMoveMode())
	{
		return;
	}

	HusongShuijingUser *die_user = this->GetHusongShuiJingUser(die_role);
	HusongShuijingUser *kill_user = this->GetHusongShuiJingUser(killer_role);

	if (NULL == die_user || NULL == kill_user)
	{
		return;
	}

	if (die_user->hssj_husong_status > HUSONG_SJ_STATUS_NONE) // ½ÙÈ¡
	{
		int die_type = die_user->hssj_husong_type;
		int killer_type = kill_user->hssj_husong_type;
		if (die_type >= 0 && killer_type >= 0 && die_type != killer_type)
		{
			return;
		}

		// ¿´»÷É±¹æÔò
		if (HUSONG_SJ_STATUS_HS_FULL == die_user->hssj_husong_status &&
			HUSONG_SJ_STATUS_NONE == kill_user->hssj_husong_status)
		{
			this->OnRoleHusongStatusChange(die_role, HUSONG_SJ_STATUS_HS_LOST, die_type);
			this->OnRoleHusongStatusChange(killer_role, HUSONG_SJ_STATUS_HS_LOST, die_type);
		}
		else if (HUSONG_SJ_STATUS_HS_FULL == die_user->hssj_husong_status &&
			HUSONG_SJ_STATUS_HS_LOST == kill_user->hssj_husong_status)
		{
			this->OnRoleHusongStatusChange(die_role, HUSONG_SJ_STATUS_HS_LOST, die_type);
			this->OnRoleHusongStatusChange(killer_role, HUSONG_SJ_STATUS_HS_FULL, die_type);
		}

		++kill_user->hssj_rob_count;

		this->NotifyHusongStatusInfo(die_role);
		this->NotifyHusongStatusInfo(killer_role);
		this->SendRoleInfo(die_role);
		this->SendRoleInfo(killer_role);
	}
}

void SpecialCrossHusongShuijing::OnRoleEnterScene(Role *role)
{
	if (NULL == role) return;

	if (m_finish)
	{
		role->BackToOriginalServer();
		return;
	}
	this->SyncShuijingInfo(role);
	this->SendRoleInfo(role);
}

bool SpecialCrossHusongShuijing::SpecialCanMountOn(Role *role)
{
	return true;
}

void SpecialCrossHusongShuijing::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;
	this->OnHusongTimeout(role);
}

bool SpecialCrossHusongShuijing::SpecialIsEnemy(Role *role, Obj *obj)
{
	return SpecialLogic::SpecialIsEnemy(role, obj);
}

void SpecialCrossHusongShuijing::OnAddObj(Obj *obj)
{
	if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
	{
		GatherObj * gather_obj = dynamic_cast<GatherObj*>(obj);
		if (NULL != gather_obj)
		{
			const HusongSJGatherCfg * gather_cfg = HusongShuijingConfig::Instance().GetGatherCfgByGatherID(gather_obj->GatherId());
			if (nullptr != gather_cfg && gather_cfg->gather_type < static_cast<int> (m_refresh_control_vec.size()))
			{
				m_refresh_control_vec[gather_cfg->gather_type].gather_id = gather_cfg->gather_id;
				m_refresh_control_vec[gather_cfg->gather_type].gather_objid = gather_obj->GetId();

				GatherGenerater * generater = gather_obj->GetGatherGenerater();
				if (NULL != generater)
				{
					m_refresh_control_vec[gather_cfg->gather_type].next_try_refresh_time = generater->GetGatherNextFlushTime();
				}

				gather_obj->SetGatherTimesMax(gather_cfg->gather_max_times);
				m_refresh_control_vec[gather_cfg->gather_type].cur_remain_gather_times = gather_obj->GetLeftGatherTimes();

				if (gather_cfg->gather_type == HUSONG_SHUIJING_TYPE_BIG)
				{
					this->SyncShuijingInfo(NULL,Protocol::CS_CROSS_HUSONG_SHUIJING_GATHER_REFRESH);
				}
			}
		}
	}
}

bool SpecialCrossHusongShuijing::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL == role || NULL == gather)
	{
		return false;
	}

	HusongShuijingUser * user = this->GetHusongShuiJingUser(role);
	if (NULL == user)
	{
		return false;
	}

	if (HUSONG_SJ_STATUS_NONE != user->hssj_husong_status)
	{
		role->NoticeNum(errornum::EN_SHUIJING_ALREADY_HAVE);
		return false;
	}

	const HusongSJOtherCfg & other_cfg = HusongShuijingConfig::Instance().GetOtherCfg();
	if (user->hssj_gather_times >= other_cfg.gather_day_count + user->hssj_vip_buy_times)
	{
		role->NoticeNum(errornum::EN_JINGHUA_GATHER_DAYCOUNT_NOT_ENOUGH);
		return false;
	}

	return true;
}

void SpecialCrossHusongShuijing::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (nullptr == role)
	{
		return;
	}

	HusongShuijingUser * user = this->GetHusongShuiJingUser(role);
	if (NULL == user)
	{
		return;
	}

	if (Role::MOVE_MODE_FLY == role->GetMoveMode())
	{
		role->NoticeNum(errornum::EN_JINGHUA_GATHER_FLY_LIMIT);
		return;
	}

	if (HUSONG_SJ_STATUS_NONE != user->hssj_husong_status)
	{
		role->NoticeNum(errornum::EN_SHUIJING_ALREADY_HAVE);
		return;
	}

	Obj *obj = m_scene->GetObj(obj_id);
	GatherObj* gather_obj = dynamic_cast<GatherObj*>(obj);
	if (nullptr == gather_obj || Obj::OBJ_TYPE_GATHER != obj->GetObjType())
	{
		return;
	}

	const HusongSJGatherCfg * gather_cfg = HusongShuijingConfig::Instance().GetGatherCfgByGatherID(gather_id);
	if (nullptr == gather_cfg)
	{
		return;
	}

	if (gather_cfg->gather_type < 0 || gather_cfg->gather_type >= static_cast<int>(m_refresh_control_vec.size()))
	{
		return;
	}

	if (m_refresh_control_vec[gather_cfg->gather_type].cur_remain_gather_times <= 0)
	{
		role->NoticeNum(errornum::EN_SHUIJING_GATHER_NONE);
		return;
	}

	const HusongSJOtherCfg & other_cfg = HusongShuijingConfig::Instance().GetOtherCfg();
	if (user->hssj_gather_times >= other_cfg.gather_day_count + user->hssj_vip_buy_times)
	{
		role->NoticeNum(errornum::EN_JINGHUA_GATHER_DAYCOUNT_NOT_ENOUGH);
		return;
	}
	
	++user->hssj_gather_times;
	--m_refresh_control_vec[gather_cfg->gather_type].cur_remain_gather_times;
	gather_obj->OnGatherChange();

	this->OnRoleHusongStatusChange(role, HUSONG_SJ_STATUS_HS_FULL, gather_cfg->gather_type);

	this->NotifyHusongStatusInfo(role);
	this->SyncShuijingInfo();
	this->SendRoleInfo(role);
}

int SpecialCrossHusongShuijing::GetSpecialParam(Role *role)
{
	HusongShuijingUser * user = this->GetHusongShuiJingUser(role);
	if (NULL == user || HUSONG_SJ_STATUS_NONE == user->hssj_husong_status)
	{
		return 0;
	}

	return user->hssj_husong_type*10 + user->hssj_husong_status;
}

int SpecialCrossHusongShuijing::GetRemainGatherTimes(ObjID obj_id)
{
	for (unsigned int i = 0; i < m_refresh_control_vec.size(); ++i)
	{
		if (m_refresh_control_vec[i].gather_objid == obj_id)
		{
			return m_refresh_control_vec[i].cur_remain_gather_times;
		}
	}

	return 0;
}

bool SpecialCrossHusongShuijing::IsHusongStatus(Role *role)
{
	HusongShuijingUser * husong_user = GetHusongShuiJingUser(role);
	if (NULL != husong_user)
	{
		return husong_user->hssj_husong_status == HUSONG_SJ_STATUS_NONE ? false : true;
	}
	return false;
}

void SpecialCrossHusongShuijing::OnActivityStatusChange(int to_status)
{
	if (ACTIVITY_STATUS_STANDY == to_status)
	{
		this->OnActivityStandy();
	}
	else if (ACTIVITY_STATUS_OPEN == to_status)
	{
		this->OnActivityOpen();
	}
	else
	{
		this->OnActivityClose();
	}
}

HusongShuijingUser * SpecialCrossHusongShuijing::GetHusongShuiJingUser(Role * role)
{
	if (NULL == role)
	{
		return NULL;
	}
	
	HusongShuijingUserMap::iterator itr = m_user_map.find(role->GetUniqueUserID());
	if (itr != m_user_map.end())
	{
		return &itr->second;
	}

	HusongShuijingUser user;
	user.user_id = role->GetUniqueUserID();

	m_user_map.insert(std::make_pair(user.user_id, user));

	itr = m_user_map.find(role->GetUniqueUserID());
	if (itr != m_user_map.end())
	{
		return &itr->second;
	}

	return NULL;
}

void SpecialCrossHusongShuijing::OnActivityStandy()
{
	m_finish = false;
	m_user_map.clear();
	this->StopGenerater();
}

void SpecialCrossHusongShuijing::OnActivityOpen()
{
	m_isopen = true;

	this->StartGenerater();
	int gather_type_count = HusongShuijingConfig::Instance().GetGatherTypeCount();
	for (int i = 0; i < gather_type_count; i++)
	{
		const HusongSJGatherCfg *gather_cfg = HusongShuijingConfig::Instance().GetGatherCfg(i);
		if (nullptr != gather_cfg)
		{
// 			GatherObj * gather_obj = new GatherObj();
// 			gather_obj->Init(NULL, gather_cfg->gather_id, gather_cfg->gather_time * 1000, 0, false);
// 			gather_obj->SetPos(gather_cfg->gather_pos);
// 			gather_obj->SetSpecialGatherType(SPECIAL_GATHER_TYPE_HUSONG_SHUIJING);
// 			m_scene->AddObj(gather_obj);

			HusongShuijingControlParam param;
// 			param.gather_objid = gather_obj->GetId();
// 			param.next_try_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			m_refresh_control_vec.push_back(param);
		}
	}
}

void SpecialCrossHusongShuijing::OnActivityClose()
{
	m_finish = true;

	this->StopGenerater();

// 	int gather_type_count = HusongShuijingConfig::Instance().GetGatherTypeCount();
// 
// 	for (int i = 0; i < gather_type_count && i < static_cast<int>(m_refresh_control_vec.size()); i++)
// 	{
// 		m_refresh_control_vec[i].last_refresh_gather_times = 0;
// 		m_refresh_control_vec[i].cur_remain_gather_times = 0;
// 		const HusongSJGatherCfg * gather_cfg = HusongShuijingConfig::Instance().GetGatherCfg(i);
// 		if (nullptr != gather_cfg)
// 		{
// 			GatherObj *gather_obj = m_scene->GetGatherObj(m_refresh_control_vec[i].gather_objid);
// 			if (nullptr != gather_obj)
// 			{
// 				gather_obj->OnGatherChange();
// 			}
// 		}
// 	}

	for (int i = 0; i < static_cast<int>(m_scene->RoleNum()); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		this->OnHusongTimeout(role);
		if (NULL != role)
		{
			role->BackToOriginalServer();
		}
	}

	m_user_map.clear();
}

void SpecialCrossHusongShuijing::OnCommit(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	HusongShuijingUser * user = this->GetHusongShuiJingUser(role);
	if (NULL == user)
	{
		return;
	}

	const HusongSJOtherCfg & other_cfg = HusongShuijingConfig::Instance().GetOtherCfg();
	if (user->hssj_commit_count >= other_cfg.commit_count + user->hssj_vip_buy_times)
	{
		role->NoticeNum(errornum::EN_JINGHUA_GATHER_DAYCOUNT_NOT_ENOUGH);
		return;
	}

	if (user->hssj_husong_status <= HUSONG_SJ_STATUS_NONE || user->hssj_husong_status >= HUSONG_SJ_STATUS_MAX)
	{
		role->NoticeNum(errornum::EN_SHUIJING_NOT_IN_ROLE);
		return;
	}

	const HusongSJGatherCfg * gather_cfg = HusongShuijingConfig::Instance().GetGatherCfg(user->hssj_husong_type);
	if (NULL == gather_cfg)
	{
		return;
	}

	NPC *npc = role->GetScene()->GetNPCList()->GetNpc(gather_cfg->commit_npcid);
	if (nullptr == npc)
	{
		return;
	}

	if (!npc->CanTouch(role))
	{
		return;
	}

	++user->hssj_commit_count;

	const HusongSJRewardCfg *reward_cfg = HusongShuijingConfig::Instance().GetJingHuaRewardCfg(user->hssj_husong_type, user->hssj_commit_count);
	if (nullptr != reward_cfg)
	{
		const ItemConfigData *reward_item_list = nullptr;
		if (HUSONG_SJ_STATUS_HS_FULL == user->hssj_husong_status)
		{
			reward_item_list = reward_cfg->total_reward_item_list;
		}
		else if (HUSONG_SJ_STATUS_HS_LOST == user->hssj_husong_status)
		{
			reward_item_list = reward_cfg->be_robbed_reward_item_list;
		}

		if (nullptr != reward_item_list)
		{
			role->GetKnapsack()->PutListOrMail(reward_item_list, PUT_REASON_CROSS_HUSONG_SHUIJING);

			role->NoticeNum(noticenum::NT_COMMIT_SUCC);

			std::string get_item_log;
			ItemConfigDataListLog(get_item_log, MAX_ATTACHMENT_ITEM_NUM, reward_item_list);
			gamelog::g_log_activity.printf(LL_ERROR, "SpecialCrossHusongShuijing::OnCommit,user:%lld,name:%s,reward:%s",
				UniqueUserIDToLongLong(role->GetUniqueUserID()), role->GetName(), get_item_log.c_str());
		}

		this->OnRoleHusongStatusChange(role, HUSONG_SJ_STATUS_NONE, -1);
	}

	this->NotifyHusongStatusInfo(role);
	this->SendRoleInfo(role);
}

void SpecialCrossHusongShuijing::SendRoleInfo(Role * role)
{
	if (NULL == role)
	{
		return;
	}

	HusongShuijingUser * user = this->GetHusongShuiJingUser(role);
	if (NULL == user)
	{
		return;
	}

	Protocol::SCCrossHusongShuijingInfo info;

	info.vip_buy_times = user->hssj_vip_buy_times;
	info.gather_times = user->hssj_gather_times;
	info.commit_count = user->hssj_commit_count;
	info.rob_count = user->hssj_rob_count;
	info.husong_type = user->hssj_husong_type;
	info.husong_status = user->hssj_husong_status;
	info.invalid_time = user->hssj_invalid_time;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&info, sizeof(info));
}

void SpecialCrossHusongShuijing::OnRoleHusongStatusChange(Role *role, int status, int type)
{
	if (nullptr == role) return;

	HusongShuijingUser * user = this->GetHusongShuiJingUser(role);
	if (NULL == user)
	{
		return;
	}

	const int old_status = user->hssj_husong_status;
	user->hssj_husong_status = status;
	user->hssj_husong_type = type;

	if (HUSONG_SJ_STATUS_NONE == old_status && HUSONG_SJ_STATUS_NONE != status)
	{
		//ÏÂ×øÆï
		role->GetMountManager()->GoonMount(false, false);
		role->GetFightMountManager()->GoonFightMount(false, false);
		role->GetMultiMount()->MountOn(false);

		this->CheckBuffStatus(role, true);

		const HusongSJGatherCfg *gather_cfg = HusongShuijingConfig::Instance().GetGatherCfg(type);
		if (nullptr != gather_cfg)
		{
			user->hssj_invalid_time = static_cast<UInt32>(EngineAdapter::Instance().Time()) + gather_cfg->husong_timeout;
		}
	}
	else if (HUSONG_SJ_STATUS_NONE != old_status && HUSONG_SJ_STATUS_NONE == status)
	{
		//role->GetMountManager()->GoonMount(true, false, true);
		this->CheckBuffStatus(role, false);
		user->hssj_invalid_time = 0;
	}

	this->SendRoleInfo(role);
}

void SpecialCrossHusongShuijing::CheckBuffStatus(Role *role, bool is_add)
{
	if (nullptr == role) return;

	if (is_add)
	{
		if (this->IsHusongStatus(role))
		{
			const int HUSONG_BUFF_LAST_TIMES_MS = 7200 * 1000;
			int dec_speed_percent = HusongShuijingConfig::Instance().GetOtherCfg().dec_speed_per;

			EffectMoveSpeed *effect_speed = new EffectMoveSpeed(role->GetId(), EffectBase::PRODUCT_ID_JINGHUAHUSONG, HUSONG_BUFF_LAST_TIMES_MS, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_JINGHUA_HUSONG);
			effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			int value = static_cast<int>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_speed_percent * ROLE_ATTR_PER));
			effect_speed->SetMoveSpeed(-value, -dec_speed_percent / 100);
			role->AddEffect(effect_speed);
		}
	}
	else
	{
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_JINGHUA_HUSONG, EffectBase::PRODUCT_ID_JINGHUAHUSONG);
	}
}

void SpecialCrossHusongShuijing::OnHusongTimeout(Role *role)
{
	if (nullptr == role) return;

	HusongShuijingUser * user = this->GetHusongShuiJingUser(role);
	if (NULL == user || HUSONG_SJ_STATUS_NONE == user->hssj_husong_status)
	{
		return;
	}

	++user->hssj_commit_count;
	this->SendRoleInfo(role);

	const HusongSJRewardCfg *reward_cfg = HusongShuijingConfig::Instance().GetJingHuaRewardCfg(user->hssj_husong_type,
		user->hssj_commit_count);
	if (nullptr == reward_cfg)
	{
		return;
	}

	this->OnRoleHusongStatusChange(role, HUSONG_SJ_STATUS_NONE, -1);

	static MailContentParam contentparam; contentparam.Reset();
	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < reward_cfg->be_robbed_reward_item_count; ++i)
	{
		contentparam.AddItem(reward_cfg->be_robbed_reward_item_list[i]);
	}

	int length_1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_jinghua_husong_timeout_subject);
	int length_2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_jinghua_husong_timeout_content);
	if (length_1 > 0 && length_2 > 0)
	{
		MailRoute::MailToRole(role, SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	std::string get_item_log;
	int item_count = reward_cfg->be_robbed_reward_item_count < MAX_ATTACHMENT_ITEM_NUM ? reward_cfg->be_robbed_reward_item_count : MAX_ATTACHMENT_ITEM_NUM;
	ItemConfigDataListLog(get_item_log, item_count, reward_cfg->be_robbed_reward_item_list);
	gamelog::g_log_activity.printf(LL_ERROR, "SpecialCrossHusongShuijing::OnHusongTimeout,user:%lld,name:%s,reward:%s",
		UniqueUserIDToLongLong(role->GetUniqueUserID()),role->GetName(),get_item_log.c_str());
}

void SpecialCrossHusongShuijing::CheckJingHuaHusongTime(Role *role)
{
	if (nullptr == role) return;

	HusongShuijingUser * user = this->GetHusongShuiJingUser(role);
	if(NULL != user && HUSONG_SJ_STATUS_NONE != user->hssj_husong_status )
	{
		if (static_cast<UInt32>(EngineAdapter::Instance().Time()) >= user->hssj_invalid_time || m_finish)
		{
			this->OnHusongTimeout(role);
		}
	}
}

void SpecialCrossHusongShuijing::SyncShuijingInfo(Role * role, int reason)
{
	if (m_refresh_control_vec.size() <= HUSONG_SHUIJING_TYPE_BIG) return;

	Protocol::SCCrossHusongShuijingGatherInfo info;

	info.type = reason;
	info.cur_remain_gather_time_big = m_refresh_control_vec[HUSONG_SHUIJING_TYPE_BIG].cur_remain_gather_times;
	info.next_refresh_time_big = m_refresh_control_vec[HUSONG_SHUIJING_TYPE_BIG].next_try_refresh_time;

	if (Protocol::CS_CROSS_HUSONG_SHUIJING_GATHER_REFRESH == reason)
	{
		InternalComm::Instance().UserProtocalTransferToGame(0, (const char*)&info, sizeof(info));
	}
	else
	{
		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&info, sizeof(info));
		}
		else
		{
			m_scene->SendToRole((const char *)&info, sizeof(info));
		}
	}
}

void SpecialCrossHusongShuijing::SyncShuijingInfo(long long uuid)
{
	if (m_refresh_control_vec.size() <= HUSONG_SHUIJING_TYPE_BIG) return;

	Protocol::SCCrossHusongShuijingGatherInfo info;

	info.type = Protocol::CS_CROSS_HUSONG_SHUIJING_GATHER_DEFAULT;
	info.cur_remain_gather_time_big = m_refresh_control_vec[HUSONG_SHUIJING_TYPE_BIG].cur_remain_gather_times;
	info.next_refresh_time_big = m_refresh_control_vec[HUSONG_SHUIJING_TYPE_BIG].next_try_refresh_time;

	InternalComm::Instance().UserProtocalTransferToGame(uuid,(const char *)&info, sizeof(info));
}

void SpecialCrossHusongShuijing::GmReq(int parm1, int parm2)
{
	if (HUSONG_SHUIJING_TYPE_BIG == parm1)
	{
		if (m_refresh_control_vec.size() > 0 && parm2 > 0)
		{
			GatherObj * gather_obj = m_scene->GetGatherObj(m_refresh_control_vec[0].gather_objid);
			if (NULL != gather_obj)
			{
				m_refresh_control_vec[0].cur_remain_gather_times = parm2;
				gather_obj->SetGatherTimesMax(parm2);
				this->SyncShuijingInfo();
			}
		}
	}
	else if(HUSONG_SHUIJING_TYPE_SMALL == parm1)
	{
		if (m_refresh_control_vec.size() > 1 && parm2 > 0)
		{
			GatherObj * gather_obj = m_scene->GetGatherObj(m_refresh_control_vec[1].gather_objid);
			if (NULL != gather_obj)
			{
				m_refresh_control_vec[1].cur_remain_gather_times = parm2;
				gather_obj->SetGatherTimesMax(parm2);
				this->SyncShuijingInfo();
			}
		}
	}
}

void SpecialCrossHusongShuijing::NotifyHusongStatusInfo(Role *role)
{
	if (NULL == role) return;
	HusongShuijingUser * user = this->GetHusongShuiJingUser(role);

	//»¤ËÍ×´Ì¬£º10*Ë®¾§ÀàÐÍ+×´Ì¬£¬01 ´óË®¾§Âú×´Ì¬ 02 ´óË®¾§ËðÊ§×´Ì¬ 11 Ð¡Ë®¾§Âú×´Ì¬ 12 Ð¡Ë®¾§ËðÊ§×´Ì¬
	Protocol::SCNoticeGatherBuffInfo cmd;
	cmd.obj_id = role->GetId();
	cmd.is_gather_wudi = user->hssj_husong_type *10 + user->hssj_husong_status;
	if (cmd.is_gather_wudi < 0)
	{
		cmd.is_gather_wudi = 0;
	}

	//printf("----------bojid:%d,status:%d\n", cmd.obj_id, cmd.is_gather_wudi);
	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

void SpecialCrossHusongShuijing::StartGenerater()
{
	SceneMonsterLogic::GatherGeneraterList &generater_list = m_scene->GetMonsterLogic()->GetGatherGeneraterList();
	for (SceneMonsterLogic::GatherGeneraterList::iterator gather_it = generater_list.begin(); gather_it != generater_list.end(); gather_it++)
	{
		(*gather_it)->SetPause(false);
		(*gather_it)->RefreshNow();
	}
}

void SpecialCrossHusongShuijing::StopGenerater()
{
	{
		SceneMonsterLogic::GatherGeneraterList &generater_list = m_scene->GetMonsterLogic()->GetGatherGeneraterList();
		for (SceneMonsterLogic::GatherGeneraterList::iterator gather_it = generater_list.begin(); gather_it != generater_list.end(); gather_it++)
		{
			(*gather_it)->SetPause(true);
		}

		int gather_num = (int)m_scene->GatherObjNum();
		for (int i = 0; i < gather_num; i++)
		{
			GatherObj *gather = m_scene->GetGatherObjByIndex(i);
			if (NULL != gather)
			{
				if (NULL != gather->GetGatherGenerater()) gather->GetGatherGenerater()->Gather(NULL);
				m_scene->DeleteObj(gather->GetId());
			}
		}
	}
}

void SpecialCrossHusongShuijing::RefreshGatherInfo(int gather_id)
{
	SceneMonsterLogic::GatherGeneraterList &generater_list = m_scene->GetMonsterLogic()->GetGatherGeneraterList();
	for (SceneMonsterLogic::GatherGeneraterList::iterator gather_it = generater_list.begin(); gather_it != generater_list.end(); gather_it++)
	{
		if ((*gather_it)->GetGatherId() != gather_id)
		{
			continue;
		}

		for (unsigned int i = 0; i < m_refresh_control_vec.size(); ++i)
		{
			if ((*gather_it)->GetGatherId() == m_refresh_control_vec[i].gather_id)
			{
				unsigned int old_refresh_time = m_refresh_control_vec[i].next_try_refresh_time;
				m_refresh_control_vec[i].next_try_refresh_time = (*gather_it)->GetGatherNextFlushTime();
				if (old_refresh_time != m_refresh_control_vec[i].next_try_refresh_time)
				{
					this->SyncShuijingInfo();
				}
			}
		}
	}
}
