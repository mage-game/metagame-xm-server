#include "activityworship.hpp"
#include "protocal/msgactivity.hpp"
#include "world.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/gongchengzhanconfig.hpp"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "other/event/eventhandler.hpp"

ActivityWorship::ActivityWorship(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_WORSHIP), m_auto_worship_next_addexp_timestamp(0)
{
	
}

ActivityWorship::~ActivityWorship()
{

}

void ActivityWorship::OnActivityStatusChange(int from_status, int to_status)
{
	Activity::OnActivityStatusChange(from_status, to_status);

	switch (to_status)
	{
	case ACTIVITY_STATUS_CLOSE:
		{
			m_worship_user_map.clear();
		}
		break;

	default:
		break;
	}
}

void ActivityWorship::Update(unsigned long interval, time_t now_second)
{
	Activity::Update(interval, now_second);

	if (m_activity_status == ACTIVITY_STATUS_OPEN)
	{
		this->CheckWorshipAddexp(now_second);
	}
}

void ActivityWorship::OnWorship(Role* role)
{
	if (nullptr == role) return;

	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

	if (nullptr == role->GetScene() || role->GetScene()->GetSceneID() != other_cfg.worship_scene_id) return;

	if (std::abs(role->GetPos().x - other_cfg.worship_pos.x) > other_cfg.worship_range || std::abs(role->GetPos().y - other_cfg.worship_pos.y) > other_cfg.worship_range)
	{
		return;
	}

	WorshipItem &item = m_worship_user_map[role->GetUID()];
	if (item.manual_worship_time >= other_cfg.worship_click_time_limit)
	{
		return;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (item.next_can_manual_worship_timestamp > now_second)
	{
		return;
	}

	++item.manual_worship_time;
	item.next_can_manual_worship_timestamp = now_second + other_cfg.worship_click_cd;
	item.manual_worship_add_exp_timestamp = now_second + other_cfg.worship_gather_time;

	this->SendWorshipInfo(role);
	EventHandler::Instance().OnEnterWorship(role);
}

void ActivityWorship::CheckWorshipAddexp(time_t now_second)
{
	UInt32 now_sec = static_cast<UInt32>(now_second);
	// 手动加经验
	for (auto &it : m_worship_user_map)
	{
		if (0 == it.second.manual_worship_add_exp_timestamp || now_sec < it.second.manual_worship_add_exp_timestamp)
		{
			continue;
		}

		it.second.manual_worship_add_exp_timestamp = 0;

		Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(it.first));
		if (nullptr == temp_role)
		{
			continue;
		}

		//const GongChengZhanWorshipExpConfig *exp_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetWorshipExpCfg(temp_role->GetLevel());
		//if (NULL == exp_cfg)
		//{
		//	continue;
		//}

		//temp_role->AddExp(exp_cfg->click_addexp, "manual_worship_add_exp", Role::EXP_ADD_WORSHIP);

		//策划需求，修改增加经验规则（2018-1-3）
		long long add_exp = static_cast<long long>(LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().exp_factor_click * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(temp_role->GetLevel(), 0)));
		temp_role->AddExp(add_exp, "manual_worship_add_exp", Role::EXP_ADD_WORSHIP);
	}

	// 自动加经验
	if (0 == m_auto_worship_next_addexp_timestamp || now_sec >= m_auto_worship_next_addexp_timestamp)
	{
		const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

		m_auto_worship_next_addexp_timestamp = now_sec + other_cfg.worship_addexp_interval;

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.worship_scene_id, 0);
		if (NULL == scene) return;

		const int MAX_GET_ROLE_COUNT = 512;
		ObjID objid_list[MAX_GET_ROLE_COUNT] = { 0 };
		int get_obj_count = scene->GetZoneMatrix()->GetObjByArea(other_cfg.worship_pos, other_cfg.worship_range, other_cfg.worship_range, objid_list, MAX_GET_ROLE_COUNT);

		for (int i = 0; i < get_obj_count && i < MAX_GET_ROLE_COUNT; ++i)
		{
			Obj *obj = scene->GetObj(objid_list[i]);
			if (obj != NULL && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
			{
				Role *tmp_role = (Role*)obj;

				//const GongChengZhanWorshipExpConfig *exp_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetWorshipExpCfg(tmp_role->GetLevel());
				//if (NULL == exp_cfg) continue;

				//tmp_role->AddExp(exp_cfg->interval_addexp, __FUNCTION__, Role::EXP_ADD_WORSHIP);

				//策划需求，修改增加经验规则（2018-1-3）
				long long add_exp = static_cast<long long>(LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().exp_factor_time * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(tmp_role->GetLevel(), 0)));
				tmp_role->AddExp(add_exp, __FUNCTION__, Role::EXP_ADD_WORSHIP);
			}
		}
	}
}

void ActivityWorship::SendWorshipInfo(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCGCZWorshipInfo gczwi;

	WorshipItem &item = m_worship_user_map[role->GetUID()];

	gczwi.worship_time = item.manual_worship_time;
	gczwi.next_worship_timestamp = item.next_can_manual_worship_timestamp;
	gczwi.next_interval_addexp_timestamp = m_auto_worship_next_addexp_timestamp;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&gczwi, sizeof(gczwi));
}

