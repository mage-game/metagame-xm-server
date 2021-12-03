#include "specialzhanshendian.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/zhanshendianconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/fb/rolefbzhanshendian.hpp"
#include "gamelog.h"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "engineadapter.h"

SpecialZhanShenDian::SpecialZhanShenDian(Scene *scene) : SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID), 
														m_is_finish(false), m_is_pass(false), m_monster_id(0)
{
}

SpecialZhanShenDian::~SpecialZhanShenDian()
{
}

void SpecialZhanShenDian::OnRoleEnterScene(Role *role)
{
	if (NULL != role)
	{
		bool is_first_enter = (INVALID_USER_ID == m_owner_user_id);
		if (is_first_enter)
		{
			int pass_level = role->GetZhanShenDian()->GetPassLevel();
			const ZhanShenDianLevelCfg *zhanshendian_cfg = LOGIC_CONFIG->GetZhanShenDianCfg().GetZhanShenDianLevelCfgByLevel(pass_level + 1);
			if (NULL != zhanshendian_cfg)
			{
				m_monster_id = zhanshendian_cfg->monster_id;
				MONSTERPOOL->CreateMonster(zhanshendian_cfg->monster_id, role->GetScene(), zhanshendian_cfg->monster_pos);
			}
		}

		m_owner_user_id = role->GetUserId();
		m_owner_objid = role->GetId();

		EventHandler::Instance().OnEnterZhanShenDian(role);
	}
}

bool SpecialZhanShenDian::OnTimeout()
{
	if (!m_is_finish)
	{
		this->OnFinish(false);
	}

	return m_is_pass;
}

void SpecialZhanShenDian::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_is_finish)
	{
		this->CheckSceneMonsterStatus(interval, now_second);
	}
}

Role * SpecialZhanShenDian::GetOwnerRole()
{
	Obj *obj = m_scene->GetObj(m_owner_objid);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (m_owner_user_id == role->GetUserId()) 
		{
			return role;
		}
	}

	return NULL;
}

void SpecialZhanShenDian::OnFinish(bool is_pass)
{
	if (!m_is_finish)
	{
		m_is_finish = true;
		m_is_pass = is_pass;

		Role *role = this->GetOwnerRole();
		if (NULL != role)
		{
			this->NotifyBossHPPercent(m_monster_id);

			if (m_is_pass)
			{
				const ZhanShenDianLevelCfg *zhanshendian_cfg = LOGIC_CONFIG->GetZhanShenDianCfg().GetZhanShenDianLevelCfgByMonster(m_monster_id);
				if (NULL != zhanshendian_cfg)
				{
					if (zhanshendian_cfg->level > role->GetZhanShenDian()->GetPassLevel())
					{
						role->GetKnapsack()->PutOrMail(zhanshendian_cfg->pass_raward, PUT_REASON_ZHANSHENDIAN_PASS_REWARD);

						role->GetZhanShenDian()->SetPassLevel(zhanshendian_cfg->level);

						{
							// »’÷æ
							int use_time = static_cast<int>(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());

							gamelog::g_log_fb.printf(LL_INFO, "Zhanshendian::OnFinish user[%d, %s], role_level[%d], level[%d], use_time[%d]", 
								role->GetUID(), role->GetName(), role->GetLevel(), zhanshendian_cfg->level, use_time);
						}
					}

					EventHandler::Instance().OnPassZhanshendian(role, zhanshendian_cfg->level);
				}

				role->Recover();
			}

			Protocol::SCZhanShenDianFBResult zsdfbr;
			zsdfbr.is_pass = (m_is_pass ? 1 : 0);
			zsdfbr.time_cost = static_cast<unsigned int>(EngineAdapter::Instance().Time()) - this->GetScene()->GetCreateTime();
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&zsdfbr, sizeof(zsdfbr));
		}
	}
}

void SpecialZhanShenDian::OnAllMonsterDead()
{
	this->OnFinish(true);
}

void SpecialZhanShenDian::OnReqNextLevel()
{
	if (!m_is_finish || !m_is_pass)
	{
		return;
	}

	if (!this->IsAllMonsterDead())
	{
		return;
	}

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	int pass_level = role->GetZhanShenDian()->GetPassLevel();
	const ZhanShenDianLevelCfg *zhanshendian_cfg = LOGIC_CONFIG->GetZhanShenDianCfg().GetZhanShenDianLevelCfgByLevel(pass_level + 1);
	if (NULL == zhanshendian_cfg)
	{
		return;
	}

	if (!m_scene->ResetScene(role))
	{
		return;
	}

	if (NULL != MONSTERPOOL->CreateMonster(zhanshendian_cfg->monster_id, role->GetScene(), zhanshendian_cfg->monster_pos))
	{
		m_is_finish = false;
		m_is_pass = false;
		m_monster_id = zhanshendian_cfg->monster_id;
	}
}

