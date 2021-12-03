#include "specialcrossshuijing.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"

#include "scene/activityshadow/activityshadow.hpp"

#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "engineadapter.h"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "config/logicconfigmanager.hpp"
#include "globalconfig/otherconfig.h"
#include "globalconfig/globalconfig.h"

#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/scenemanager.h"

#include "other/event/eventhandler.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "internalcomm.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "gamelog.h"
#include "item/knapsack.h"
#include "other/shop/roleshop.hpp"
#include "other/cross/rolecross.h"
#include "other/event/eventhandler.hpp"

SpecialCrossShuiJing::SpecialCrossShuiJing(Scene *scene) : SpecialLogic(scene), m_big_shuijing_num(0)
{
}

SpecialCrossShuiJing::~SpecialCrossShuiJing()
{

}

void SpecialCrossShuiJing::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *add_role = (Role *)obj;
		UniqueUserID uuid = add_role->GetRoleCross()->GetCrossOriginalUniqueUserID();

		CrossShuijingPlayerInfo *player_info = this->GetPlayerInfo(uuid);
		if (NULL == player_info) // 新来的玩家
		{
			player_info = &m_user_info_cache[uuid];
			player_info->uuid = uuid;
			F_STRNCPY(player_info->name, add_role->GetName(), sizeof(player_info->name));
			player_info->cur_limit_gather_times = CrossShuiJingConfig::Instance().GetOtherCfg().gather_max_times;

			gamelog::g_log_cross.printf(LL_INFO, "SpecialCrossShuiJing First Join user[%d,%s] uuid[%lld] plat_name[%s]",
				add_role->GetUID(), add_role->GetName(), UniqueUserIDToLongLong(uuid), add_role->GetPlatName());
		}

		this->SendPlayerInfoToPlayer(add_role);
		EventHandler::Instance().OnEnterCrossShuijing(add_role);
	}
	else if (Obj::OBJ_TYPE_GATHER == obj->GetObjType())
	{
		GatherObj *gather = (GatherObj *)obj;
		if (CrossShuiJingConfig::Instance().GetBigShuiJingId() == gather->GatherId())
		{
			this->UpdateBigGatherNum();
		}
	}
}

void SpecialCrossShuiJing::OnRemoveObj(Obj *obj)
{
	if (Obj::OBJ_TYPE_GATHER == obj->GetObjType())
	{
		GatherObj *gather = (GatherObj *)obj;
		if (CrossShuiJingConfig::Instance().GetBigShuiJingId() == gather->GatherId())
		{
			this->UpdateBigGatherNum();
		}
	}
}

bool SpecialCrossShuiJing::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL == role)
	{
		return false;
	}

	CrossShuijingPlayerInfo *p_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == p_info)
	{
		return false;
	}

	if (p_info->cur_limit_gather_times <= 0)
	{
		role->NoticeNum(errornum::EN_CROSS_SHUIJING_GATHER_TIMES_LIMIT);
		return false;
	}

	return true;
}

void SpecialCrossShuiJing::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	const CrossShuiJingGatherCfg *gather_cfg = CrossShuiJingConfig::Instance().GetGatherCfgByGatherId(gather_id);
	if (NULL == gather_cfg) return;

	CrossShuijingPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL != player_info)
	{
		player_info->total_bind_gold += gather_cfg->bind_gold;
		role->GetKnapsack()->GetMoney()->AddGoldBind(gather_cfg->bind_gold, "CrossShuijing");

		player_info->total_mojing += gather_cfg->mojing;
		role->GetRoleShop()->AddChestShopMojing(gather_cfg->mojing, "CrossShuijing");

		player_info->total_cross_honor += gather_cfg->cross_honor;
		role->AddCrossHonor(gather_cfg->cross_honor, "CrossShuijing");

		-- player_info->cur_limit_gather_times;

		this->SendPlayerInfoToPlayer(role);
	}
}

bool SpecialCrossShuiJing::SpecialRelive(Role *role, int realive_type)
{
	if (NULL == role)
	{
		return true;
	}

	CrossShuijingPlayerInfo *p_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == p_info)
	{
		return true;
	}

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		// 免费复活，有倒计时限制
		const CrossShuiJingOtherCfg &other_cfg = CrossShuiJingConfig::Instance().GetOtherCfg();
		if (role->GetDeadTime() < other_cfg.relive_time * 1000 - 1000)
		{
			return true;
		}

		role->ReAlive(100, 100, other_cfg.realive_pos);
	}
	else
	{
		// 消耗次数复活
		if (role->GetRoleCross()->GetCrossBossReliveTimes() > 0)
		{
			if (!role->ReAlive(100, 100))
			{
				return true;
			}

			role->GetRoleCross()->AddCrossBossReliveTimes(-1, "CrossShuiJing", false);
			this->SendPlayerInfoToPlayer(role);
		}
		else
		{
			UniqueUserID uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
			CrossShuijingPlayerInfo *p_info = this->GetPlayerInfo(uuid);
			if (NULL == p_info) return true;

			int relive_need_gold = LOGIC_CONFIG->GetOtherCfg().GetOtherCfg().cross_relive_gold;
			if (!role->GetKnapsack()->GetMoney()->UseAllGold(relive_need_gold, "SpecialCrossShuiJing::SpecialRelive"))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return true;
			}

			role->ReAlive(100, 100);
		}
	}

	return true;
}

void SpecialCrossShuiJing::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	role->BackToOriginalServer();
}
	
bool SpecialCrossShuiJing::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (NULL == role || NULL == obj)
	{
		return false;
	}

	if (role->GetId() == obj->GetId())
	{
		return false;
	}

	return true;
}

bool SpecialCrossShuiJing::SpecialStopGahterOnAttack(Role *role)
{
	CrossShuijingPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == player_info)
	{
		return true;
	}

	if (player_info->gather_buff_time >= (unsigned int)(EngineAdapter::Instance().Time()))
	{
		return false;
	}

	return true;
}

void SpecialCrossShuiJing::OnActivityOpen()
{

}

void SpecialCrossShuiJing::OnActivityClose()
{
	this->DelayKickOutAllRole();
	m_user_info_cache.clear();
}

void SpecialCrossShuiJing::OnBuyGatherBuff(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	CrossShuijingPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == player_info)
	{
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseGold(CrossShuiJingConfig::Instance().GetOtherCfg().gather_buff_gold, "CrossShuiJingBuyBuff"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int buff_time = CrossShuiJingConfig::Instance().GetOtherCfg().gather_buff_time;
	player_info->gather_buff_time = (std::max)(player_info->gather_buff_time + buff_time, (unsigned int)(EngineAdapter::Instance().Time()) + buff_time);

	this->SendPlayerInfoToPlayer(role);
}

Role *SpecialCrossShuiJing::GetRole(ObjID obj_id)
{
	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return  (Role *)obj;
	}

	return NULL;
}

CrossShuijingPlayerInfo *SpecialCrossShuiJing::GetPlayerInfo(UniqueUserID uuid)
{
	UserInfoMap::iterator it = m_user_info_cache.find(uuid);
	if (it != m_user_info_cache.end())
	{
		return &it->second;
	}

	return NULL;
}

void SpecialCrossShuiJing::SendPlayerInfoToPlayer(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	CrossShuijingPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL != player_info)
	{
		static Protocol::SCCrossShuijingPlayerInfo spi;
		spi.total_bind_gold = player_info->total_bind_gold;
		spi.total_mojing = player_info->total_mojing;
		spi.total_cross_honor = player_info->total_cross_honor;
		spi.total_relive_times = role->GetRoleCross()->GetCrossBossReliveTimes();
		spi.cur_limit_gather_times = player_info->cur_limit_gather_times;
		spi.gather_buff_time = player_info->gather_buff_time;
		spi.big_shuijing_num = m_big_shuijing_num;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&spi, sizeof(spi));
	}
}

void SpecialCrossShuiJing::UpdateBigGatherNum()
{
	int old_num = m_big_shuijing_num;

	m_big_shuijing_num = 0;
	int gather_num = m_scene->GatherObjNum();
	for (int i = 0; i < gather_num; i++)
	{
		GatherObj *gather_obj = m_scene->GetGatherObjByIndex(i);
		if (NULL != gather_obj && CrossShuiJingConfig::Instance().GetBigShuiJingId() == gather_obj->GatherId())
		{
			m_big_shuijing_num++;
		}
	}

	if (old_num != m_big_shuijing_num)
	{
		int role_num = m_scene->RoleNum();
		for (int i = 0; i < role_num; i++)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				this->SendPlayerInfoToPlayer(role);
			}
		}
	}
}
