#include "rolestatusmanager.hpp"

#include "servercommon/errornum.h"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/marryshadow/marryshadow.h"

#include "task/taskpool.h"

#include "globalconfig/globalconfig.h"
#include "global/bossfamily/bossfamilymanager.hpp"

#include "engineadapter.h"
#include "gstr.h"

#include "config/logicconfigmanager.hpp"
#include "scene/speciallogic/worldspecial/jinghuahusongmgr.hpp"
#include "config/fbsceneconfig.hpp"
#include "other/shenzhouweapon/shenzhouweapon.hpp"
#include "friend/teaminfo.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "task/other/husongtask.hpp"
#include "other/cross/rolecross.h"
#include "other/multimount/multimount.hpp"
#include "other/jinglingadvantage/jinglingadvantage.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/greatesoldier/greatesoldier.hpp"

RoleStatusManager::RoleStatusManager()
	: m_role(NULL), m_last_fight_time(0), m_fight_end_time(0)
{

}

RoleStatusManager::~RoleStatusManager()
{

}

void RoleStatusManager::Init(Role *role)
{
	m_role = role;
}

void RoleStatusManager::Update(unsigned int now_second)
{
	if (now_second > m_fight_end_time)
	{
		m_role->OnStopFight();
	}
}

int RoleStatusManager::GetRoleActionStatus()
{
	switch (m_role->GetRoleStatus())
	{
	case Role::ROLE_STATUS_CHARACTER:
		{
			switch (m_role->GetActionStatus())
			{
			case Character::ACTION_IDLE:
				return ROLE_ACTION_STATUS_IDLE;

			case Character::ACTION_RUN:
				return ROLE_ACTION_STATUS_MOVE;

			case Character::ACTION_ATTACK:
				return ROLE_ACTION_STATUS_FIGHT;
			}
		}
		break;

	case Role::ROLE_STATUS_GATHER:
		{
			return ROLE_ACTION_STATUS_GATHER;
		}
		break;

	case Role::ROLE_STATUS_REST:
		{
			return ROLE_ACTION_STATUS_REST;
		}
		break;
	}

	return ROLE_ACTION_STATUS_INVALID;
}

bool RoleStatusManager::CanMountOn()
{
	if (m_role->GetHusongTask()->IsHusongStatus() || JingHuaHuSongMgr::Instance().IsHusongStatus(m_role))
	{
		/*m_role->NoticeNum(errornum::EN_MOUNT_CANNOT_MOUNT_IN_HUSONG);*/
		return false;
	}

	if (this->IsInFightStatus() && Role::ATTACK_MODE_PEACE != m_role->GetAttackMode())
	{
		return false;
	}

	if (NULL != m_role->GetScene())
	{
		const FbSceneConfig * fb_scenc_cfg = LOGIC_CONFIG->GetFbSceneConfig().GetFbSceneCfg(m_role->GetScene()->GetSceneType()); 
		if (0 != fb_scenc_cfg->pb_mount)
		{
			return false;
		}
	}

	if (GREATE_SOLDIER(m_role)->IsBianshenStatus())
	{
		return false;
	}

	return true;
}

bool RoleStatusManager::CanFightMountOn()
{
	if (m_role->GetHusongTask()->IsHusongStatus() || JingHuaHuSongMgr::Instance().IsHusongStatus(m_role))
	{
		/*m_role->NoticeNum(errornum::EN_MOUNT_CANNOT_MOUNT_IN_HUSONG);*/
		return false;
	}

	if (NULL != m_role->GetScene())
	{
		const FbSceneConfig * fb_scenc_cfg = LOGIC_CONFIG->GetFbSceneConfig().GetFbSceneCfg(m_role->GetScene()->GetSceneType()); 
		if (0 != fb_scenc_cfg->pb_fightmount)
		{
			return false;
		}
	}

	if (GREATE_SOLDIER(m_role)->IsBianshenStatus())
	{
		return false;
	}

	return true;
}


bool RoleStatusManager::CanEnterFB(bool check_acitivty, bool notify)
{
	if (this->IsInFB())
	{
		if (notify)
		{
			m_role->NoticeNum(errornum::EN_FB_ENTER_CAN_NOT_IN_FB);
		}

		return false;
	}

	if (check_acitivty)
	{
		if (!m_role->GetRoleCross()->IsInCross1v1Matching())
		{
			if (notify)
			{
				m_role->NoticeNum(errornum::EN_1V1_MATCHING_CANNOT_ENTER_FB);
			}
			return false;
		}

		if (m_role->GetRoleCross()->IsInCross3v3Matching())
		{
			if (notify)
			{
				m_role->NoticeNum(errornum::EN_1V1_MATCHING_CANNOT_ENTER_FB);
			}
			return false;
		}
	}

	if (m_role->GetQingyuan()->IsInXunyouStatus())
	{
		if (notify)
		{
			gstr::SendError(m_role, "xunyou_enter_fb_limit");
		}

		return false;
	}
	else if (Scene::IsBossFamilyScene(m_role->GetScene()->GetSceneID()) || Scene::IsBossDongKuScene(m_role->GetScene()->GetSceneID()) ||
			Scene::IsDabaoBossScene(m_role->GetScene()->GetSceneID()) || Scene::IsActiveBossScene(m_role->GetScene()->GetSceneID()) ||
			Scene::IsCampNormalDuoBaoScene(m_role->GetScene()->GetSceneID() || Scene::IsPerciousBossScene(m_role->GetScene()->GetSceneID())))
	{
		if (notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_SCENE_OPERATE_LIMIT);
		}
		return false;
	}

	return this->CanTransmit(notify);
}

bool RoleStatusManager::CanGather(GatherObj *gather)
{
	if (NULL == gather) return false;

	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		m_role->NoticeNum(errornum::EN_CAN_NOT_GATHER_IN_FLY);
		return false;
	}

	Scene *scene = m_role->GetScene();
	if (NULL != scene)
	{
		if (!scene->GetSpecialLogic()->SpecialCanGather(m_role, gather))
		{
			return false;
		}
	}

	if (!m_role->GetShenzhouWeapon()->CanGather(gather->GatherId())) // 神州六器判断能否采集
	{
		return false;
	}

	if (!JinglingAdvantage::Instance().CanGather(m_role, gather->GatherId()))				// 精灵奇遇判断能否采集
	{
		return false;
	}

	if (!m_role->GetQingyuan()->CheckCanGatherXunyouHongbao(gather->GatherId()))		// 婚宴-巡游红包能否采集
	{
		return false;
	}

	if (!gather->CanGather(m_role))
	{
		return false;
	}

	if (!m_role->GetRoleActivity()->CanRAPlantingTreeWatering(gather->GatherId(), gather->GetId()))	// 植树活动判断能否采集（浇水）
	{
		return false;
	}

	return true;
}

bool RoleStatusManager::CanRest(bool is_notify)
{
	if (!m_role->CanCharacterAction())
	{
		if (is_notify) m_role->NoticeNum(errornum::EN_STATUS_CAN_NOT_REST);
		return false;
	}

	if (this->IsInFightStatus())
	{
		if (is_notify) m_role->NoticeNum(errornum::EN_ROLE_REST_FORBID_IN_FIGHT);
		return false;
	}

	if (NULL != m_role->GetScene() && !m_role->GetScene()->GetSpecialLogic()->SpecialCanRest(m_role))
	{
		return false;
	}

	return true;
}

bool RoleStatusManager::CanPerformSkill()
{
	if (NULL != m_role->GetScene() && !m_role->GetScene()->GetSpecialLogic()->SpecialCanPerformSkill(m_role))
	{
		return false;
	}

	return true;
}

bool RoleStatusManager::CanFight(ObjID target_id)
{
	if (!m_role->CanCharacterAction(true))
	{
		return false;
	}

	switch (m_role->GetMoveMode())
	{
	case Role::MOVE_MODE_FLY:
		m_role->NoticeNum(errornum::EN_NOT_FIGHT_IN_FLY);
		return false;
	case Role::MOVE_MODE_JUMP:
		m_role->NoticeNum(errornum::EN_NOT_FIGHT_IN_JUMP);
		return false;
	case Role::MOVE_MODE_JUMP2:
		return false;
		m_role->NoticeNum(errornum::EN_NOT_FIGHT_IN_JUMP2);
	default:
		break;
	}
 
	return true;
}

bool RoleStatusManager::CanSupplyEffect(int supply_type)
{
	if (NULL != m_role->GetScene() && !m_role->GetScene()->GetSpecialLogic()->SpecialCanSupplyEffect(m_role, supply_type))
	{
		return false;
	}

	return true;
}

bool RoleStatusManager::CanUseItem(ItemID item_id)
{
	if (!m_role->IsAlive())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_USE_DEAD);
		return false;
	}

	if (m_role->IsXuanYun())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_USE_XUANYUN_STATE);
		return false;
	}

	if (NULL != m_role->GetScene() && !m_role->GetScene()->GetSpecialLogic()->SpecialCanUseItem(m_role, item_id))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_USE_SCENE_LIMIT);
		return false;
	}

	return true;
}

bool RoleStatusManager::CanGuildBackStation(bool notify)
{
	if (this->IsInFB())
	{
		if (notify) m_role->NoticeNum(errornum::EN_GUILD_UNION_BACK_STATION_FB);
		return false;
	}

	if (this->IsInBoss())
	{
		if (notify) m_role->NoticeNum(errornum::EN_GUILD_UNION_BACK_STATION_BOSS);
		return false;
	}

	return this->CanTransmit();
}

bool RoleStatusManager::CanTransmit(bool is_notify)
{
	if (m_role->GetHusongTask()->IsHusongStatus() || JingHuaHuSongMgr::Instance().IsHusongStatus(m_role))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_FB_ENTER_HOSONG_LIMIT);
		}

		return false;
	}

	if (!m_role->IsAlive())
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_TRANSMIT_LIMIT);
		}

		return false;
	}

	if (m_role->IsXuanYun())
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_NOT_TANSMIT_XUANYUN);
		}
		
		return false;
	}

	if (m_role->IsDingShen())
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_NOT_TANSMIT_DINGSHEN);
		}

		return false;
	}

	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_NOT_TRANSMIT_FLY_MODE);
		}

		return false;
	}

	if (this->IsInFightStatus() && Role::ATTACK_MODE_PEACE != m_role->GetAttackMode())
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_ROLE_TRANSMIT_FORBID_IN_FIGHT);
		}

		return false;
	}

	// 新游戏可传送
	//if (NULL != m_role->GetScene() && SceneRegister::Instance().IsWorldBossScene(m_role->GetScene()->GetSceneID()))
	//{
	//	if (is_notify)
	//	{
	//		m_role->NoticeNum(errornum::EN_TRANSMIT_LIMIT_IN_THIS_SCENE);
	//	}

	//	return false;
	//}
// 
// 	if (NULL != m_role->GetScene() && GLOBALCONFIG->IsOtherCampScene(m_role->GetCamp(), m_role->GetScene()->GetSceneID()))
// 	{
// 		if (is_notify)
// 		{
// 			m_role->NoticeNum(errornum::EN_TRANSMIT_LIMIT_IN_THIS_SCENE);
// 		}
// 		
// 		return false;
// 	}

	if (m_role->GetQingyuan()->IsInXunyouStatus())
	{
		m_role->NoticeNum(errornum::EN_TRANSMIT_LIMIT_IN_MARRY);
		return false;
	}

	return true;
}

bool RoleStatusManager::CanFlyByShoe(int target_scene_id)
{
	if (NULL != m_role->GetScene() && !m_role->GetScene()->GetSpecialLogic()->SpecialCanFlyByShoe(m_role))
	{
		m_role->NoticeNum(errornum::EN_NOT_FLY_BY_SHOE_SCENE_LIMIT);
		return false;
	}

	if (this->IsInFB())
	{
		m_role->NoticeNum(errornum::EN_NOT_FLY_BY_SHOE_IN_FB);
		return false;
	}

	if (!this->CanAttackModeEnterHelper(target_scene_id))
	{
		m_role->NoticeNum(errornum::EN_ATTACK_MODE_NOT_ALLOW_IN);
		return false;
	}

	if (!this->CheckOtherSceneCanFly(target_scene_id))
	{
		m_role->NoticeNum(errornum::EN_NOT_FLY_BY_SHOE_SCENE_LIMIT);
		return false;
	}

	if (!BossFamilyManager::Instance().CanFlyByShoe(m_role, target_scene_id))
	{
		return false;
	}

	if (Scene::IsCampNormalDuoBaoScene(target_scene_id))  // 防止客户端直接发协议进去打宝地图，玩家进入打宝地图应该走另外条扣钱的协议
	{
		m_role->NoticeNum(errornum::EN_NOT_FLY_BY_SHOE_CAMPDUOBAO_LIMIT);
		return false;
	}

	if (Scene::IsShangGuYiJi(m_role->GetScene()->GetSceneID()))
	{
		m_role->NoticeNum(errornum::EN_NOT_FLY_BY_SHOE_SCENE_LIMIT);
		return false;
	}

	//if (GLOBALCONFIG->IsHangUpScene(target_scene_id))
	//{
	//	m_role->NoticeNum(errornum::EN_CAN_NOT_FLY_TO_HANGUP_SCENE);
	//	return false;
	//}

	return this->CanTransmit();
}

bool RoleStatusManager::CanTransportTo(int target_scene_id)
{
	if (!this->CanAttackModeEnterHelper(target_scene_id))
	{
		m_role->NoticeNum(errornum::EN_ATTACK_MODE_NOT_ALLOW_IN);
		return false;
	}

	return true;
}

bool RoleStatusManager::CanAcceptSpecialTask(TaskID task_id)
{
	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		m_role->NoticeNum(errornum::EN_TASK_ACCEPT_FLY_MODE_LIMIT);
		return false;
	}

	return true;
}

bool RoleStatusManager::CanDating(bool is_notify)
{
	//if (this->IsInFB())
	//{
	//	if (is_notify) m_role->NoticeNum(errornum::EN_DATING_IN_FB);
	//	return false;
	//}

	return true;
}

bool RoleStatusManager::CanStartCross()
{
	if (this->IsInFB())
	{
		m_role->NoticeNum(errornum::EN_NOT_START_CROSS_IN_FB);
		return false;
	}

	return this->CanTransmit();
}

bool RoleStatusManager::CanChangeAttackMode(int attack_mode)
{
	Scene *scene = m_role->GetScene();
	if (NULL == scene) return false;

	return true;
}

bool RoleStatusManager::CanGuildSos()
{
	return m_role->GetHusongTask()->IsHusongStatus();
}

int RoleStatusManager::GetGuildSosType()
{
	int sos_type = GUILD_SOS_TYPE_DEFAULT;

	if (m_role->GetHusongTask()->IsHusongStatus())
	{
		sos_type = GUILD_SOS_TYPE_HUSONG_BE_ATTACK;
	}

	return sos_type;
}

bool RoleStatusManager::CanRecordLastKillMeUserInfo()
{
	return true;
}

bool RoleStatusManager::IsInFB()
{
	if (NULL != m_role->GetScene() && !m_role->GetScene()->InStaticScene())
	{
		return true;
	}

	return false;
}

bool RoleStatusManager::IsInBoss()
{
	Scene* scene = m_role->GetScene();
	int scene_id = m_role->GetScene()->GetSceneID();
	if (NULL != scene && (scene->IsWorldBossScene(scene_id) || scene->IsBossFamilyScene(scene_id) || 
		scene->IsBossDongKuScene(scene_id) || scene->IsDabaoBossScene(scene_id) || 
		scene->IsActiveBossScene(scene_id) || scene->IsPerciousBossScene(scene_id)))
	{
		return true;
	}
	
	return false;
}

bool RoleStatusManager::IsHaveTeam()
{
	if (m_role->GetTeamInfo()->InTeam())
	{
		return true;
	}

	return false;
}

void RoleStatusManager::OnEnterFB()
{
	if (m_role->GetMountManager()->IsOnMount() && NULL != m_role->GetScene())
	{
		const FbSceneConfig * fb_scece_cfg = LOGIC_CONFIG->GetFbSceneConfig().GetFbSceneCfg(m_role->GetScene()->GetSceneType());
		if (NULL != fb_scece_cfg)
		{
			if (0 != fb_scece_cfg->pb_mount)
			{
				m_role->GetMountManager()->GoonMount(false, false);

				if (m_role->GetMultiMount()->IsOnMount())
				{
					m_role->GetMultiMount()->MountOn(false);
				}
			}
		}
	}

	if (m_role->GetFightMountManager()->IsOnFightMount() && NULL != m_role->GetScene())
	{
		const FbSceneConfig * fb_scece_cfg = LOGIC_CONFIG->GetFbSceneConfig().GetFbSceneCfg(m_role->GetScene()->GetSceneType());
		if (NULL != fb_scece_cfg)
		{
			if (0 != fb_scece_cfg->pb_fightmount)
			{
				m_role->GetFightMountManager()->GoonFightMount(false, false);

				if (m_role->GetMultiMount()->IsOnMount())
				{
					m_role->GetMultiMount()->MountOn(false);
				}
			}
		}
	}
}

void RoleStatusManager::OnFly()
{
	
}

// 是否在战斗状态
bool RoleStatusManager::IsInFightStatus()
{
	if (0 == m_last_fight_time) return false;

	unsigned long now = EngineAdapter::Instance().GetGameTime();

	return now < m_last_fight_time + GlobalConfig::Instance()->GetRoleInFightMs();
}

bool RoleStatusManager::IsHpSupplyRewardExtra()
{
	int scene_type = m_role->GetScene()->GetSceneType();
	if (Scene::SCENE_TYPE_PERSONAL_TOWERDEFEND == scene_type || Scene::IsPersonalScene(scene_type, 0) || Scene::IsPersonalScene(scene_type, 0))
	{
		return false;
	}

	if (m_last_fight_time > 0)
	{
		if (EngineAdapter::Instance().GetGameTime() < m_last_fight_time + GlobalConfig::Instance()->GetRoleInFightMs() + 30000)
		{
			return false;
		}
	}

	 return true;
}

// 进入战斗，记录最后一次进入战斗的时间
void RoleStatusManager::OnFight()
{
	m_last_fight_time = EngineAdapter::Instance().GetGameTime();
	m_fight_end_time = (unsigned int)(EngineAdapter::Instance().Time() + GlobalConfig::Instance()->GetRoleInFightMs()/1000);
}

bool RoleStatusManager::CanAttackModeEnterHelper(int target_scene_id)
{
	return true;
}

bool RoleStatusManager::CheckOtherSceneCanFly(int target_scene_id)
{
	// BOSS洞窟、打宝地图、世界boss、boss之家、活跃boss 内 相同地图不给飞行
	if (Scene::IsBossDongKuScene(m_role->GetScene()->GetSceneID()) || 
		Scene::IsDabaoBossScene(m_role->GetScene()->GetSceneID()) || 
		Scene::IsActiveBossScene(m_role->GetScene()->GetSceneID()) ||
		SceneRegister::Instance().IsWorldBossScene(m_role->GetScene()->GetSceneID()) || 
		Scene::IsCampNormalDuoBaoScene(m_role->GetScene()->GetSceneID()) ) 
	{
		if (m_role->GetScene()->GetSceneID() == target_scene_id)
		{
			return false;
		}
	}

	return true;
}

bool RoleStatusManager::CanUpdateGiveMP()
{
	if (this->IsInFightStatus()) return false;

	return true;
}



