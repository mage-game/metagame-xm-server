#include "gatherobj.h"

#include "obj/character/role.h"
#include "monster/monsterpool.h"
#include "monster/monsterinitparam.h"
#include "monster/drop/droppool.hpp"
#include "scene/scene.h"
#include "scene/generater/gathergenerater.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "item/knapsack.h"
#include "item/itempool.h"

#include "config/logicconfigmanager.hpp"
#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include "globalconfig/globalconfig.h"

#include "engineadapter.h"
#include "internalcomm.h"

#include "world.h"

#include "protocal/msgactivity.hpp"
#include "servercommon/activitydef.hpp"

#include "skill/skillbase.hpp"
#include "protocal/msgscene.h"
#include "scene/speciallogic/worldspecial/jinghuahusongmgr.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "config/activityconfig/millionaireconfig.hpp"
#include "config/activityconfig/guildbonfireconfig.hpp"
#include "other/shenzhouweapon/shenzhouweaponconfig.hpp"
#include "other/daycounter/daycounter.hpp"
#include "servercommon/serverconfig/husongshuijingconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrosshusongshuijing.hpp"

GatherObj::GatherObj()
	: Obj(OBJ_TYPE_GATHER), m_gather_id(0), m_gather_time(0), m_gather_generater(NULL), m_evil_add(0), 
	m_disappear_after_gather(true), m_has_gather(false), m_disappear_timestamp(0), m_disappear_gather_max(0), m_cur_gather_count(0), m_data(0),
	m_bonfire_next_reward_time(0), m_belong_guildid(0), m_special_gather_type(0), m_gather_ing_obj_id(INVALID_OBJ_ID)
{
	memset(m_belong_guildname, 0, sizeof(m_belong_guildname));
}

GatherObj::~GatherObj()
{

}

void GatherObj::Init(GatherGenerater *gather_generater, int gather_id, int gather_time, int evil_add, bool disappear_after_gather, 
					 int disappear_s, int disappear_gather_max)
{
	m_gather_generater = gather_generater;

	m_gather_id = gather_id;
	m_gather_time = gather_time;
	m_evil_add = evil_add;
	m_disappear_after_gather = disappear_after_gather;

	if (disappear_s > 0)
	{
		m_disappear_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + disappear_s;
	}

	if (disappear_gather_max > 0)
	{
		m_disappear_gather_max = disappear_gather_max;
	}
}

void GatherObj::Gather(Role *role)
{
	if (NULL == role) return;

	m_gather_ing_obj_id = INVALID_OBJ_ID;
	EventHandler::Instance().OnGather(role, m_gather_id, m_obj_id, m_posi);
	
	if (this->IsBonfire())
	{
		this->OnGatharBonfire(role);
	}
	else if (this->IsMillionaireBox())
	{
		// 大富豪最初设计是普通场景刷的，策划又提需求要改成活动场景刷，所以采集处理流程放在了这里
		// 不要学，场景活动逻辑应该放在场景逻辑里面
		if (role->GetLevel() < LOGIC_CONFIG->GetMillionaireConfig().GetOhterCfg()->level_limit)
		{
			role->NoticeNum(errornum::EN_MILLIONAIRE_GATHER_LEVEL_LIMIT);
			return;
		}

		LOGIC_CONFIG->GetMillionaireConfig().CalcGatherReward(role, m_gather_id);
		role->SendMillionaireInfo();
		EventHandler::Instance().OnMillionaireGatherBox(role);
	}

	++ m_cur_gather_count;

	{
		Protocol::SCGatherBeGather gbg;
		gbg.gather_role_obj_id = role->GetId();
		gbg.gather_obj_id = m_obj_id;
		gbg.gather_id = m_gather_id;
		gbg.left_gather_times = this->GetLeftGatherTimes();

		m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&gbg, sizeof(gbg));
	}

	this->RemoveOnGather(role);
}

bool GatherObj::CanGather(Role *gather_role)
{
	if (NULL == gather_role)
	{
		return false;
	}

	if (this->IsBonfire())
	{
		return this->CanGatherBonfire(gather_role);
	}
	else if (this->IsMillionaireBox())
	{
		if (!LOGIC_CONFIG->GetMillionaireConfig().IsCanGather(gather_role))
			return false;
	}

	return true;
}

void GatherObj::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	if (NULL == m_scene) return;

	if (0 != m_disappear_timestamp && now_second >= m_disappear_timestamp && !m_has_gather)		// 这句放最后 因为UpdateLingMai里面会改变m_disappear_time 不然有严重bug
	{
		m_scene->DeleteObj(m_obj_id);
		m_has_gather = true;
		m_gather_ing_obj_id = INVALID_OBJ_ID;

		if (this->IsShenzhouGather())
		{
			ActivityShadow::Instance().ChangeShenzhouGatherNum(-1);
			ActivityShadow::Instance().SynceShenzhouGatherInfoToAll();
		}
	}

	if (this->IsBonfire())
	{
		this->UpdateBonfire(interval, now_second, now_dayid);
	}
}

void GatherObj::InitCrossFishingGather(Int64 unique_user_id, const GameName name)
{
	m_special_gather_type = SPEICAL_GATHER_TYPE_CROSS_FISHING;
	m_data = unique_user_id;
	F_STRNCPY(m_own_name, name, sizeof(m_own_name));
}

void GatherObj::InitCrossGuildBattleBoss(GuildName guild_name)
{ 

	F_STRNCPY(m_belong_guildname, guild_name, sizeof(GuildName)); 
}

void GatherObj::RemoveOnGather(Role *gathar_role)
{
	if (m_disappear_after_gather && m_cur_gather_count >= m_disappear_gather_max)
	{
		if (NULL != m_gather_generater)
		{
			m_gather_generater->Gather(this);
		}

		m_scene->DeleteObj(m_obj_id);
		m_has_gather = true;

		if (this->IsShenzhouGather())
		{
			ActivityShadow::Instance().ChangeShenzhouGatherNum(-1);
			ActivityShadow::Instance().SynceShenzhouGatherInfoToAll();
		}
	}
}

void GatherObj::InitBonfire(GuildID guild_id, GuildName guild_name)
{
	m_belong_guildid = guild_id;
	F_STRNCPY(m_belong_guildname, guild_name, sizeof(GuildName));
}

bool GatherObj::CanAddMuCai(Role *role)
{
	if (NULL == role) return false;

	if (!role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_GUILD_BONFIRE_ADD_MUCAI, LOGIC_CONFIG->GetGuildBonfireCfg().GetAddMucaiMaxCount()))
	{
		return false;
	}

	return true;
}

void GatherObj::AddMuCai(Role *role)
{
	if (NULL == role) return;

	ItemConfigData mucai_add_reward[MAX_ATTACHMENT_ITEM_NUM];

	LOGIC_CONFIG->GetGuildBonfireCfg().GetAddMucaiReward(mucai_add_reward);
	role->GetKnapsack()->PutListOrMail(mucai_add_reward, PUT_REASON_GUILD_BONFIRE_ADD_MUCAI);
	
	int add_bangong = LOGIC_CONFIG->GetGuildBonfireCfg().GetAddMucaiBangong();
	role->AddGuildGongxian(add_bangong, "GuildBonfireAddMucai");
}

bool GatherObj::CanGatherBonfire(Role *gathar_role, bool notice)
{
	if (gathar_role->GetGuildID() <= 0)
	{
		if (notice) gathar_role->NoticeNum(errornum::EN_GATHAR_GUILD_LIMIT);
		return false;
	}

	if (gathar_role->GetGuildID() == m_belong_guildid)
	{
		const int gathar_self_max = LOGIC_CONFIG->GetGuildBonfireCfg().GetGatharSelfMax();
		if (!gathar_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_GATHAR_SELF_BONFIRE, gathar_self_max))
		{
			if (notice) gathar_role->NoticeNum(errornum::EN_GATHAR_SELF_BONFIRE_LIMIT);
			return false;
		}
	}
	
	const int gathar_max = LOGIC_CONFIG->GetGuildBonfireCfg().GetGatharMax();
	if (!gathar_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_GATHAR_BONFIRE_TOTAL, gathar_max))
	{
		if (notice) gathar_role->NoticeNum(errornum::EN_GATHAR_BONFIRE_LIMIT);
		return false;
	}

	return true;
}

void GatherObj::OnGatharBonfire(Role *gathar_role)
{
	if (!this->CanGatherBonfire(gathar_role))
	{
		return;
	}

	if (gathar_role->GetGuildID() == m_belong_guildid)
	{
		gathar_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_GATHAR_SELF_BONFIRE);
	}
	gathar_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_GATHAR_BONFIRE_TOTAL);
	EventHandler::Instance().OnGatherBonfire(gathar_role);

	const GuildBonfireLvCfg * gather_reward_cfg = LOGIC_CONFIG->GetGuildBonfireCfg().GetGatharRewardByLv(gathar_role->GetLevel());
	if (NULL != gather_reward_cfg)
	{
		gathar_role->AddExp(gather_reward_cfg->base_exp, "GatherObj::OnGatharBonfire", Role::EXP_ADD_REASON_DEFAULT);
		gathar_role->GetKnapsack()->PutListOrMail(gather_reward_cfg->gather_reward, PUT_REASON_GUILD_BONFIRE_GATHER);
	}

	ROLE_LOG_QUICK6(LOG_TYPE_GUILD_BONFIRE, gathar_role, gathar_role->GetLevel(), m_belong_guildid, "Gather", NULL);
}

bool GatherObj::IsBonfire()
{
	const int bonfire_gathar_id = LOGIC_CONFIG->GetGuildBonfireCfg().GetGatharID();

	return m_gather_id == bonfire_gathar_id;
}

bool GatherObj::IsMillionaireBox()
{
	const std::map<int, GatherCfg> & gather_map = LOGIC_CONFIG->GetMillionaireConfig().GetGatherCfgMap();
	if (gather_map.find(m_gather_id) != gather_map.end())
	{
		return true;
	}

	return false;
}

bool GatherObj::IsShenzhouGather()
{
	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();

	return m_gather_id == other_cfg.gather_id;
}

void GatherObj::UpdateBonfire(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	const int timereward_cd = LOGIC_CONFIG->GetGuildBonfireCfg().GetTimeRewardCDs();
	const int timereward_range = LOGIC_CONFIG->GetGuildBonfireCfg().GetTimeRewardRange();

	if (0 == m_bonfire_next_reward_time)
	{
		m_bonfire_next_reward_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + timereward_cd);
	}
	else if (now_second >= m_bonfire_next_reward_time)
	{
		m_bonfire_next_reward_time += timereward_cd;

		{
			static ObjID target_list[256] = {INVALID_OBJ_ID};
			int count = SkillBase::GetSceneObjByArea(m_scene, m_posi, timereward_range, target_list, sizeof(target_list) / sizeof(target_list[0]));

			for (int i = 0; i < count; ++i)
			{
				Obj *tmp_obj = m_scene->GetObj(target_list[i]);
				if (NULL != tmp_obj && Obj::OBJ_TYPE_ROLE == tmp_obj->GetObjType())
				{
					Role *tmp_role = (Role*)tmp_obj;
					if (tmp_role->GetGuildID() == m_belong_guildid)
					{
						const GuildBonfireTimesCfg * time_reward_cfg = LOGIC_CONFIG->GetGuildBonfireCfg().GetTimeRewardByLv(tmp_role->GetLevel());
						if (NULL != time_reward_cfg)
						{
							tmp_role->AddExp(time_reward_cfg->base_exp, "GatherObj::UpdateBonfire", Role::EXP_ADD_REASON_DEFAULT);
							tmp_role->AddGuildGongxian(time_reward_cfg->base_bangong, "GatherObj::UpdateBonfire");
						}
					}
				}
			}
		}
	}
}

void GatherObj::OnGatherChange()
{
	static Protocol::SCGatherChange gc;
	gc.obj_id = this->GetId();
	gc.special_gather_type = this->GetSpecialGatherType();
	gc.gather_id = this->GatherId();
	gc.pos_x = (short)this->GetPos().x;
	gc.pos_y = (short)this->GetPos().y;
	
	if (SPECIAL_GATHER_TYPE_JINGHUA == gc.special_gather_type)
	{
		gc.param_0 = JingHuaHuSongMgr::Instance().GetRemainGatherTimes();
	}
	else if (SPECIAL_GATHER_TYPE_HUSONG_SHUIJING == gc.special_gather_type)
	{
		const HusongSJOtherCfg other_cfg = HusongShuijingConfig::Instance().GetOtherCfg();
		SpecialCrossHusongShuijing * special_logic = World::GetInstWorld()->GetSceneManager()->GetSpcialCrossHusongShuijing(other_cfg.scene_id, COMMON_SCENE_KEY, false);
		if (NULL != special_logic)
		{
			gc.param_0 = special_logic->GetRemainGatherTimes(this->GetId());
		}
	}

	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&gc, sizeof(Protocol::SCGatherChange));
}