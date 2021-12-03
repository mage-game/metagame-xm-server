#include "sceneregister.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "scene/scene.h"

#include "engineadapter.h"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/configcommon.h"

#include "gameworld/config/doorconfig.h"

SceneRegister::SceneRegister()
{

}

SceneRegister::~SceneRegister()
{

}

SceneRegister & SceneRegister::Instance()
{
	static SceneRegister sr;
	return sr;
}

bool SceneRegister::LoadSceneLineConfig(const std::string &path, std::string *err)
{
	TiXmlDocument document;

	if (path == "" || !document.LoadFile(path))
	{
		*err = path + ": LoadSceneLineConfig Error,\n " + document.ErrorDesc();
		return false;
	}

	TiXmlElement *root_element = document.RootElement();
	if (NULL == root_element)
	{
		*err = path + ": xml node error in root";
		return false;
	}

	TiXmlElement *line_element = root_element->FirstChildElement("line_config");
	if (NULL == line_element)
	{
		*err = path + ": xml node error in line_config";
		return false;
	}

	TiXmlElement *current_element = line_element->FirstChildElement("data");
	while (NULL != current_element)
	{
		int scene_id = 0;
		if (!GetSubNodeValue(current_element, "scene_id", scene_id) || !this->CheckSceneIsExist(scene_id))
		{
			*err = path + ": xml node error in scene_id";
			return false;
		}

		int max_line = 0;
		if (!GetSubNodeValue(current_element, "max_line", max_line) || max_line <= 0)
		{
			*err = path + ": xml node error in max_line";
			return false;
		}

		int line_role_count = 0;
		if (!GetSubNodeValue(current_element, "line_role_count", line_role_count) || line_role_count <= 0)
		{
			*err = path + ": xml node error in line_role_count";
			return false;
		}

		this->AddLine(scene_id, max_line, line_role_count);

		current_element = current_element->NextSiblingElement("data");
	}

	return true;
}

void SceneRegister::GetAllObjMoveInfo(Role *role, Protocol::CSGetAllObjMoveInfoReq *gaomir)
{
	Scene *scene = role->GetScene();
	if (NULL != scene && this->CanGetAllObjMoveInfo(scene->GetSceneType(), scene->GetSceneID(), role->GetUserId()))
	{
		static Protocol::SCAllObjMoveInfo aomi;
		aomi.count = 0;

		{
			for (int i = 0; i < (int)scene->RoleNum() && aomi.count < Protocol::SC_ALL_OBJ_MOVE_INFO_MAX; i++)
			{
				Role *temp_role = scene->GetRoleByIndex(i);
				if (NULL != temp_role)
				{
					Protocol::SCAllObjMoveInfo::ObjMoveInfo *omi = &aomi.all_obj_moveinfo_list[aomi.count];
					omi->obj_id = temp_role->GetId();
					omi->obj_type = temp_role->GetObjType();
					omi->type_special_id = UidToInt(temp_role->GetUserId());
					omi->dir = temp_role->GetDir();
					omi->distance = temp_role->GetDirDistance();
					omi->pos_x = temp_role->GetPos().x; omi->pos_y = temp_role->GetPos().y;
					omi->move_speed = (int)temp_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
					omi->monster_key = role->IsEnemy(temp_role) ? 1 : 0;
					++ aomi.count;
				}
			}
			UInt32 monster_num = scene->MonsterNum();
			for (int i = 0; i < (int)monster_num && aomi.count < Protocol::SC_ALL_OBJ_MOVE_INFO_MAX; i++)
			{
				Monster *monster = scene->GetMonsterByIndex(i);
				if (NULL != monster && monster->IsAlive())
				{
					Protocol::SCAllObjMoveInfo::ObjMoveInfo *omi = &aomi.all_obj_moveinfo_list[aomi.count];
					omi->obj_id = monster->GetId();
					omi->obj_type = monster->GetObjType();
					omi->type_special_id = static_cast<int>(monster->GetMonsterId());
					omi->dir = monster->GetDir();
					omi->distance = monster->GetDirDistance();
					omi->pos_x = monster->GetPos().x; omi->pos_y = monster->GetPos().y;
					omi->move_speed = (int)monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
					omi->monster_key = monster->GetMonsterKey();
					++ aomi.count;
				}
			}
		}

		int sendlen = sizeof(aomi) - (Protocol::SC_ALL_OBJ_MOVE_INFO_MAX - aomi.count) * sizeof(Protocol::SCAllObjMoveInfo::ObjMoveInfo);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&aomi, sendlen);
	}
}

bool SceneRegister::AddSceneLevelLimit(int scene_id, int level_limit)
{
	std::map<int, int>::iterator it = m_scene_level_limit_map.find(scene_id);
	if (it != m_scene_level_limit_map.end() && it->second != level_limit) 
	{
		return false;
	}

	m_scene_level_limit_map[scene_id] = level_limit;

	return true;
}

bool SceneRegister::CheckSceneLevelLimit(Role *role, int scene_id)
{
	std::map<int, int>::iterator it = m_scene_level_limit_map.find(scene_id);
	if (it != m_scene_level_limit_map.end())
	{
		if (!role->CheckEnterSceneLevelLimit((int)it->second))
		{
			return false;
		}
	}

	return true;
}

void SceneRegister::AddScene(int scene_id)
{
	m_all_scene_set.insert(scene_id);
}

bool SceneRegister::CheckSceneIsExist(int scene_id)
{
	std::set<int>::iterator it = m_all_scene_set.find(scene_id);
	if (it != m_all_scene_set.end())
	{
		return true;
	}

	return false;
}

bool SceneRegister::IsThisGSScene(int total_server, int server_index, int scene_id)
{
	/*if (m_special_scene_set.find(scene_id) != m_special_scene_set.end())
	{
		if (total_server != server_index) 
		{
			return false;
		}
	}
	else
	{
		if (total_server > 0 && scene_id % total_server != server_index)
		{
			return false;
		}
	}*/

	return true;
}

bool SceneRegister::CanGetAllObjMoveInfo(int scene_type, int scene_id, const UserID &user_id)
{
	//if (Scene::IsStaticScene(scene_type)) return false; 

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	UserGetAllObjOperTimeMapIt it = m_getallobj_opertime_map.find(user_id);
	if (it != m_getallobj_opertime_map.end())
	{
		if (now_second < it->second + 2) return false;
	}

	m_getallobj_opertime_map[user_id] = now_second;

	return true;
}

void SceneRegister::AddWorldBossScene(int scene_id)
{
	m_worldboss_scene_set.insert(scene_id);
}

bool SceneRegister::IsWorldBossScene(int scene_id)
{
	return m_worldboss_scene_set.find(scene_id) != m_worldboss_scene_set.end();
}

void SceneRegister::AddDoor(int scene_id, ConfigSceneDoor door)
{
	m_door_map[(scene_id << 16) + door.id] = door;
}

const ConfigSceneDoor *SceneRegister::GetDoor(int scene_id, int door_id)
{
	int key = (scene_id << 16) + door_id;
	std::map<int, ConfigSceneDoor>::iterator it = m_door_map.find(key);
	if (it != m_door_map.end())
	{
		return &it->second;
	}
	return NULL;
}

void SceneRegister::AddLine(int scene_id, int max_line, int line_role_count)
{
	SceneLineInfo &line_info = m_line_map[scene_id];
	line_info.max_line = max_line;
	line_info.line_role_count = line_role_count;
}

SceneLineInfo *SceneRegister::GetSceneLineInfo(int scene_id)
{
	std::map<int, SceneLineInfo>::iterator it = m_line_map.find(scene_id);
	if (it == m_line_map.end())
	{
		return NULL;
	}

	return &it->second;
}

