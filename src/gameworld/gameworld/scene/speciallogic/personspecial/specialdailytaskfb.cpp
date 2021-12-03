#include "specialdailytaskfb.hpp"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/dailytaskfbcofig.hpp"
#include "other/event/eventhandler.hpp"
#include "monster/monsterpool.h"
#include "ai/monsterai/monsteraiaddrecoverbuff.hpp"
#include "ai/monsterai/monsteraiaddmianshangbuff.hpp"
#include "task/newtaskmanager.h"
#include "protocal/msgfb.h"

SpecialDailyTaskFb::SpecialDailyTaskFb(Scene *scene)
	: SpecialLogic(scene), m_owner_uid(0), m_daily_task_fb_type(0), m_is_pass(false), m_fb_level(0), m_finish_param(0),
	m_is_cancel_monster_static(false)
{

}

SpecialDailyTaskFb::~SpecialDailyTaskFb()
{
}

void SpecialDailyTaskFb::OnRoleEnterScene(Role *role)
{
	if (NULL == role || m_is_pass) return;

	if (m_owner_uid != 0 && m_owner_uid != role->GetUID()) return;

	if (0 == m_owner_uid)
	{
		const DailyFBCfg *fb_cfg = LOGIC_CONFIG->GetDailyTaskFBCfg().GetDailyTaskFBConfig(m_scene->GetSceneID(), role->GetLevel());
		if (NULL == fb_cfg) return;

		m_owner_uid = role->GetUID();
		m_daily_task_fb_type = fb_cfg->branch_fb_type;
		m_is_pass = false;
		m_fb_level = role->GetLevel();

		std::map<int, std::map<int, DailyTaskFBMonsterCfg> > &fb_monster_map = LOGIC_CONFIG->GetDailyTaskFBCfg().GetDailyTaskFBMonsterMap();
		std::map<int, std::map<int, DailyTaskFBMonsterCfg> >::iterator iter = fb_monster_map.find(fb_cfg->scene_id);
		if (iter != fb_monster_map.end())
		{
			std::map<int, DailyTaskFBMonsterCfg> &monster_cfg_map = fb_monster_map[fb_cfg->scene_id];
			std::map<int, DailyTaskFBMonsterCfg>::iterator monster_iter = monster_cfg_map.begin();
			for(; monster_iter != monster_cfg_map.end(); monster_iter++)
			{
				DailyTaskFBMonsterCfg monster_cfg = monster_iter->second;

				if (m_fb_level >= monster_cfg.min_level && m_fb_level <= monster_cfg.max_level)
				{
					if (DAILY_TASK_FB_TYPE_1 == m_daily_task_fb_type)
					{
						SceneMonsterLogic *monster_logic = m_scene->GetMonsterLogic();
						if (NULL != monster_logic)
						{
							monster_logic->AddMonsterGenerate(m_scene, 0, 1, 1000, monster_cfg.create_pos, monster_cfg.monster_id);
						}
					}
					else
					{
						Monster *monster = MONSTERPOOL->CreateMonster(monster_cfg.monster_id, m_scene, monster_cfg.create_pos);
						if (NULL != monster)
						{
							if (0 != monster_cfg.add_mianshang)
							{
								if (MonsterInitParam::AITYPE_STATIC_ADD_MIANSHANG_BUFF == monster->GetAIType())
								{
									MonsterAIAddMianshanBuff *ai = (MonsterAIAddMianshanBuff*)monster->GetMonsterAI();
									ai->SetMianshangPer(monster_cfg.mianshang_per);
									ai->SetEffectType(0 != monster_cfg.mianshang_only_boss);
								}
							}
							else if (0 != monster_cfg.add_recover)
							{
								if (MonsterInitParam::AITYPE_STATCI_ADD_RECOVER_BUFF == monster->GetAIType())
								{
									MonsterAIAddRecoverBuff *ai = (MonsterAIAddRecoverBuff*)monster->GetMonsterAI();
									ai->SetEffectType(0 != monster_cfg.recover_only_boss);
									ai->SetRecoverType(0 != monster_cfg.recover_by_attack);
									ai->SetRecoverPer(monster_cfg.recover_per);
									ai->SetRecoverInterval(monster_cfg.recover_interval);
								}
							}
						}
					}
				}
			}
		}
	}

	this->SyncFBSceneLogicInfo(false);
}

void SpecialDailyTaskFb::OnRoleLeaveScene(Role *role, bool is_logout)
{
}

void SpecialDailyTaskFb::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;

	const DailyFBCfg *fb_cfg = LOGIC_CONFIG->GetDailyTaskFBCfg().GetDailyTaskFBConfig(m_scene->GetSceneID(), m_fb_level);
	if (NULL == fb_cfg) return;

	if (Obj::OBJ_TYPE_MONSTER != dead->GetObjType()) return;

	Monster *dead_monster = (Monster *)dead;

	switch(m_daily_task_fb_type)
	{
	case DAILY_TASK_FB_TYPE_1:
	case DAILY_TASK_FB_TYPE_2:
		{
			if (fb_cfg->monster_1 == dead_monster->GetMonsterId())
			{
				m_finish_param += fb_cfg->param_1;
			}
			else if (fb_cfg->monster_2 == dead_monster->GetMonsterId())
			{
				m_finish_param += fb_cfg->param_2;
			}
			else if (fb_cfg->monster_3 == dead_monster->GetMonsterId())
			{
				m_finish_param += fb_cfg->param_3;
			}
			
			this->SyncFBSceneLogicInfo(false);
		}
		break;

	case DAILY_TASK_FB_TYPE_3:
	case DAILY_TASK_FB_TYPE_4:
	case DAILY_TASK_FB_TYPE_5:
		{
			if (fb_cfg->boss_monster == dead_monster->GetMonsterId())
			{
				m_finish_param++;
			}

			this->SyncFBSceneLogicInfo(false);
		}
		break;
	}
}

void SpecialDailyTaskFb::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	const DailyFBCfg *fb_cfg = LOGIC_CONFIG->GetDailyTaskFBCfg().GetDailyTaskFBConfig(m_scene->GetSceneID(), m_fb_level);
	if (NULL == fb_cfg) return;

	if (m_finish_param >= fb_cfg->finish_param)
	{
		Role * owner_role =m_scene->GetRoleByUID(m_owner_uid);
		if (NULL != owner_role)
		{
			owner_role->GetTaskManager()->OnPassDailyTaskFB(m_scene->GetSceneID());
		}

		m_is_pass = true;
		this->SyncFBSceneLogicInfo(true);
	}
}

void SpecialDailyTaskFb::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster * monster = (Monster*)obj;

		if (!m_is_cancel_monster_static)
		{
			MonsterAIBase *monster_ai = monster->GetMonsterAI();
			if (NULL != monster_ai)
			{
				unsigned now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				monster_ai->SetIsStatic(true);
				monster_ai->SetStaticEndTime(now_sec + 10);
			}
		}
	}
}

void SpecialDailyTaskFb::SpecialCancelMonsterStatic()
{
	if (m_is_cancel_monster_static) return;

	m_is_cancel_monster_static = true;
	UInt32 monster_num = m_scene->MonsterNum();
	for (unsigned int i = 0; i < monster_num; i++)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster)
		{
			MonsterAIBase *monster_ai = monster->GetMonsterAI();
			if (NULL != monster_ai)
			{
				monster_ai->SetIsStatic(false);
				monster_ai->SetStaticEndTime(0);
			}
		}
	}
}

void SpecialDailyTaskFb::SyncFBSceneLogicInfo(bool is_active_leave_fb)
{
	Role *role = m_scene->GetRoleByUID(m_owner_uid);
	if (NULL != role)
	{
		static Protocol::SCFBSceneLogicInfo cmd;
		cmd.reset();

		cmd.time_out_stamp = m_scene->GetSceneTimeOutStamp();
		cmd.scene_type = m_scene->GetSceneType();

		cmd.is_finish = 0;
		cmd.is_pass = m_is_pass ? 1 : 0;
		cmd.is_active_leave_fb = is_active_leave_fb;

		cmd.total_boss_num = 0;
		cmd.total_allmonster_num = 0;
		cmd.kill_boss_num = 0;
		cmd.kill_allmonster_num = 0;

		cmd.pass_time_s = 0;

		cmd.coin = 0;
		cmd.exp = 0;

		cmd.param1 = m_daily_task_fb_type;
		cmd.param2 = m_finish_param;
		cmd.param3 = 0;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}

