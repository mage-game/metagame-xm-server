#include "specialqingyuanshengdi.hpp"

#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"

#include "engineadapter.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"

#include "other/qingyuan/qingyuan.hpp"
#include "protocal/msgother.h"

SpecialQingYuanShengDi::SpecialQingYuanShengDi(Scene *scene) 
	: SpecialLogic(scene), m_is_init(false), m_last_update_time(0)
{

}

SpecialQingYuanShengDi::~SpecialQingYuanShengDi()
{

}

void SpecialQingYuanShengDi::Update(unsigned long interval, time_t now_second)
{
	if (!m_is_init)
	{
		m_is_init = true;
		this->InitRefresh();
	}

	if (now_second > m_last_update_time)
	{
		m_last_update_time = (unsigned int)now_second;
		this->RefreshMonster((unsigned int)now_second);
		this->RefreshGather((unsigned int)now_second);
	}
}

void SpecialQingYuanShengDi::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;

		Role *role = (Role *)m_scene->GetObj(killer_id);
		if (NULL != role && Obj::OBJ_TYPE_ROLE == role->GetObjType())
		{
			int task_type = QYSD_TASK_TYPE_MONSTER;
			if (m_scene_monster_map.find(monster->GetId()) != m_scene_monster_map.end())
			{
				task_type = monster->IsBossMonster() ? QYSD_TASK_TYPE_BOSS : QYSD_TASK_TYPE_JINGYING_MONSTER;
			}
			role->GetQingyuan()->OnQingYuanShengDiKillMonster(task_type, monster->GetMonsterId());
		}
	}
}

void SpecialQingYuanShengDi::OnRoleEnterScene(Role *role)
{
	this->SyncMonsterInfo(role);
}

void SpecialQingYuanShengDi::OnRemoveObj(Obj *obj)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster *)obj;

		std::map<ObjID, MonsterInfo>::iterator it = m_scene_monster_map.find(monster->GetId());
		if (it != m_scene_monster_map.end())
		{
			MonsterInfo &info = it->second;

			this->AddPos(info.cfg.pos_type, info.pos);

			info.next_refresh_time = (unsigned int)(EngineAdapter::Instance().Time() + info.cfg.refresh_time);
			m_refresh_monster_vec.push_back(info);

			m_scene_monster_map.erase(it);

			if (monster->IsBossMonster())
			{
				this->SyncMonsterInfo();
			}
		}
	}
	else if (Obj::OBJ_TYPE_GATHER == obj->GetObjType())
	{
		GatherObj *gather_obj = (GatherObj *)obj;

		std::map<ObjID, GatherInfo>::iterator it = m_scene_gather_map.find(gather_obj->GetId());
		if (it != m_scene_gather_map.end())
		{
			GatherInfo &info = it->second;

			this->AddPos(info.cfg.pos_type, info.pos);

			info.next_refresh_time = (unsigned int)(EngineAdapter::Instance().Time() + info.cfg.refresh_time);
			m_refresh_gather_vec.push_back(info);

			m_scene_gather_map.erase(it);
		}
	}
}

void SpecialQingYuanShengDi::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	role->GetQingyuan()->OnQingYuanShengDiGether(gather_id);
}

void SpecialQingYuanShengDi::InitRefresh()
{
	const QYSDLayerCfg *cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDLayerCfg(m_scene->GetSceneKey());
	if (NULL == cfg)
	{
		return;
	}

	{
		MonsterInfo info;
		for (size_t i = 0; i < cfg->monster_vec.size(); i++)
		{
			info.cfg = cfg->monster_vec[i];

			const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(info.cfg.monster_id);
			if (NULL != monster_param)
			{
				info.is_boss = 0 != monster_param->monster_type;

				for (int j = 0; j < info.cfg.monster_count && j < 1000; j++)
				{
					m_refresh_monster_vec.push_back(info);
				}
			}
		}
	}

	{
		GatherInfo info;
		for (size_t i = 0; i < cfg->gather_vec.size(); i++)
		{
			info.cfg = cfg->gather_vec[i];
			for (int j = 0; j < info.cfg.gather_count && j < 1000; j++)
			{
				m_refresh_gather_vec.push_back(info);
			}
		}
	}
}

void SpecialQingYuanShengDi::RefreshMonster(unsigned int now_second)
{
	bool is_create_boss = false;

	for (std::vector<MonsterInfo>::iterator it = m_refresh_monster_vec.begin(); it != m_refresh_monster_vec.end();)
	{
		MonsterInfo &info = *it;
		if (now_second >= info.next_refresh_time)
		{
			if (this->GetPos(info.cfg.pos_type, &info.pos))
			{
				Monster * monster = MONSTERPOOL->CreateMonster(info.cfg.monster_id, m_scene, info.pos);
				if (NULL != monster)
				{
					info.obj_id = monster->GetId();
					m_scene_monster_map[info.obj_id] = info;

					if (monster->IsBossMonster())
					{
						is_create_boss = true;
					}
				}
			}

			it = m_refresh_monster_vec.erase(it);
		}
		else
		{
			it++;
		}
	}

	if (is_create_boss)
	{
		this->SyncMonsterInfo();
	}
}

void SpecialQingYuanShengDi::RefreshGather(unsigned int now_second)
{
	for (std::vector<GatherInfo>::iterator it = m_refresh_gather_vec.begin(); it != m_refresh_gather_vec.end();)
	{
		GatherInfo &info = *it;
		if (now_second >= info.next_refresh_time)
		{
			if (this->GetPos(info.cfg.pos_type, &info.pos))
			{
				GatherObj *gather_obj = new GatherObj();
				gather_obj->Init(NULL, info.cfg.gather_id, info.cfg.gather_time * 1000, 0, true);
				gather_obj->SetPos(info.pos);
				m_scene->AddObj(gather_obj);

				info.obj_id = gather_obj->GetId();
				m_scene_gather_map[info.obj_id] = info;
			}

			it = m_refresh_gather_vec.erase(it);
		}
		else
		{
			it++;
		}
	}
}

bool SpecialQingYuanShengDi::GetPos(int pos_type, Posi *pos)
{
	std::map<int, std::vector<Posi> >::iterator it = m_pos_map.find(pos_type);
	if (it == m_pos_map.end())
	{
		const std::vector<Posi> *cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDPosCfg(pos_type);
		if (NULL == cfg)
		{
			return false;
		}

		m_pos_map[pos_type] = *cfg;

		it = m_pos_map.find(pos_type);
	}

	if (it == m_pos_map.end())
	{
		return false;
	}

	std::vector<Posi> &pos_vec = it->second;
	int pos_count = (int)pos_vec.size();
	if (pos_count <= 0)
	{
		return false;
	}

	int index = RandomNum(pos_count);

	*pos = pos_vec[index];
	pos_vec[index] = pos_vec[pos_count - 1];
	pos_vec.pop_back();

	return true;
}

void SpecialQingYuanShengDi::AddPos(int pos_type, const Posi &pos)
{
	std::map<int, std::vector<Posi> >::iterator it = m_pos_map.find(pos_type);
	if (it == m_pos_map.end())
	{
		return;
	}

	it->second.push_back(pos);
}

void SpecialQingYuanShengDi::SyncMonsterInfo(Role *role)
{
	Protocol::SCQingYuanShengDiBossInfo pro;
	pro.boss_count = 0;

	for (std::map<ObjID, MonsterInfo>::iterator it = m_scene_monster_map.begin(); it != m_scene_monster_map.end(); it++)
	{
		if (pro.boss_count >= Protocol::QING_YUAN_SHENG_DI_BOSS_COUNT_MAX) break;

		MonsterInfo &info = it->second;
		if (info.is_boss)
		{
			pro.boss_list[pro.boss_count].boss_id = info.cfg.monster_id;
			pro.boss_list[pro.boss_count].pos_x = info.pos.x;
			pro.boss_list[pro.boss_count].pos_y = info.pos.y;
			pro.boss_list[pro.boss_count].next_refresh_time = 0;
			pro.boss_count++;
		}
	}

	for (std::vector<MonsterInfo>::iterator it = m_refresh_monster_vec.begin(); it != m_refresh_monster_vec.end(); it++)
	{
		if (pro.boss_count >= Protocol::QING_YUAN_SHENG_DI_BOSS_COUNT_MAX) break;

		MonsterInfo &info = *it;
		if (info.is_boss)
		{
			pro.boss_list[pro.boss_count].boss_id = info.cfg.monster_id;
			pro.boss_list[pro.boss_count].pos_x = 0;
			pro.boss_list[pro.boss_count].pos_y = 0;
			pro.boss_list[pro.boss_count].next_refresh_time = info.next_refresh_time;
			pro.boss_count++;
		}
	}

	int length = sizeof(pro) - sizeof(pro.boss_list[0]) * (Protocol::QING_YUAN_SHENG_DI_BOSS_COUNT_MAX - pro.boss_count);

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, length);
	}
	else
	{
		UInt32 role_num = m_scene->RoleNum();
		for (UInt32 i = 0; i < role_num; i++)
		{
			Role *tmp_role = m_scene->GetRoleByIndex(i);
			if (NULL != tmp_role)
			{
				EngineAdapter::Instance().NetSend(tmp_role->GetNetId(), (const char*)&pro, length);
			}
		}
	}
}
