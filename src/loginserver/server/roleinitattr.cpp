#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "servercommon/struct/itemlistparam.h"

#include "roleinitattr.h"
#include "engineadapter.h"
#include "config/loginconfig.h"

RoleInitAttr::RoleInitAttr()
	: m_scenecount(0)
{
	memset(&m_role_init_attr, 0, sizeof(m_role_init_attr));
	memset(m_newbie_scene, 0, sizeof(m_newbie_scene));
}

RoleInitAttr::~RoleInitAttr()
{

}

RoleInitAttr &RoleInitAttr::Instance()
{
	static RoleInitAttr ria;
	return ria;
}

bool RoleInitAttr::Init(const std::string& path, const std::string& configname, std::string* err)
{
	memset(&m_role_init_attr, 0, sizeof(m_role_init_attr));

	STRNCPY(m_role_init_attr.role_name, "role name", sizeof(m_role_init_attr.role_name));
	m_role_init_attr.sex = FEMALE;

	TiXmlDocument document;
	if (path == "" || !document.LoadFile(path+configname))
	{
		*err = path + configname + ": RoleInitAttr::Inite Error: " + document.ErrorDesc();
		return false;
	}

	std::string configpath;

	TiXmlElement *RootElement = document.RootElement();
	if (!RootElement)
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	TiXmlElement *cur_element = RootElement->FirstChildElement("scenes");
	if (!cur_element)
	{
		*err = configname + ": xml node error in [config -> scenes]";
		return false;
	}

	cur_element = cur_element->FirstChildElement("scene");
	if (!cur_element)
	{
		*err = configname + ": xml node error in [config->scenes->scene]";
		return false;
	}

	int scenenum = 0;
	while (cur_element)
	{
		if (scenenum >= NEWBIE_SCENE_NUM)
		{
			*err = configname + ": too much nodes in [config->scenes->scene]";
			return false;
		}

		if (!GetSubNodeValue(cur_element, "scene_id", m_newbie_scene[scenenum].id) || m_newbie_scene[scenenum].id <= 0)
		{
			*err = configname + ": xml node error in [config->scenes->scene->scene_id]";
			return false;
		}

		if (!GetSubNodeValue(cur_element, "scene_x", m_newbie_scene[scenenum].x) || m_newbie_scene[scenenum].x < 0)
		{
			*err = configname + ": xml node error in [config->scenes->scene->scene_x]";
			return false;
		}

		if (!GetSubNodeValue(cur_element, "scene_y", m_newbie_scene[scenenum].y) || m_newbie_scene[scenenum].y < 0)
		{
			*err = configname + ": xml node error in [config->scenes->scene->scene_y]";
			return false;
		}

		if (!GetSubNodeValue(cur_element, "range", m_newbie_scene[scenenum].range) || m_newbie_scene[scenenum].range <= 0)
		{
			*err = configname + ": xml node error in [config->scenes->scene->range]";
			return false;
		}

		++scenenum;
		cur_element = cur_element->NextSiblingElement();
	}

	m_scenecount = scenenum;

	if (m_scenecount <= 0)
	{
		*err = configname + ": xml node error in [config->scenecount <= 0]";
		return false;
	}
	
	m_role_init_attr.scene_id = m_newbie_scene[0].id;
	m_role_init_attr.scene_x =  m_newbie_scene[0].x;
	m_role_init_attr.scene_y =  m_newbie_scene[0].y;

	if (!GetSubNodeValue(RootElement, "knapsack_valid_num", m_role_init_attr.knapsack_valid_num))
	{
		*err = configname + ": xml node error in knapsack_valid_num";
		return false;
	}
	if (ItemNamespace::DEFAULT_KNAPSACK_GRID_NUM != m_role_init_attr.knapsack_valid_num)
	{
		*err = configname + ": xml node error in knapsack_valid_num, default_knapsack_grid_num is unfit!";
		return false;
	}

	if (!GetSubNodeValue(RootElement, "storage_valid_num", m_role_init_attr.storage_valid_num))
	{
		*err = configname + ": xml node error in storage_valid_num";
		return false;
	}
	if (ItemNamespace::DEFAULT_STORAGE_GRID_NUM != m_role_init_attr.storage_valid_num)
	{
		*err = configname + ": xml node error in storage_valid_num, default_storage_grid_num is unfit!";
		return false;
	}

	if (!GetSubNodeValue(RootElement, "gold_bind", m_role_init_attr.gold_bind))
	{
		*err = configname + ": xml node error in gold_bind";
		return false;
	}
	if (!GetSubNodeValue(RootElement, "coin_bind", m_role_init_attr.coin_bind))
	{
		*err = configname + ": xml node error in coin_bind";
		return false;
	}
	if (!GetSubNodeValue(RootElement, "coin", m_role_init_attr.coin))
	{
		*err = configname + ": xml node error in coin";
		return false;
	}
	if (!GetSubNodeValue(RootElement, "gold", m_role_init_attr.gold))
	{
		*err = configname + ": xml node error in gold";
		return false;
	}

	m_role_init_attr.max_hp = BIRTH_MAX_HP_BASE; m_role_init_attr.hp = m_role_init_attr.max_hp;  
	m_role_init_attr.max_mp = BIRTH_MAX_MP_BASE; m_role_init_attr.mp = m_role_init_attr.max_mp;
	m_role_init_attr.move_speed = BIRTH_MOVESPEED_BASE;

	m_role_init_attr.level = 1;


	return true;
}

const RoleInitParam & RoleInitAttr::GetInitParam(int prof)
{
	if (m_scenecount <= 0) return m_role_init_attr;

	int r = RandomNum(m_scenecount);

	m_role_init_attr.scene_id = m_newbie_scene[r].id;
	m_role_init_attr.scene_x = m_newbie_scene[r].x + (m_newbie_scene[r].range == 0 ? 0 : RandomNum(m_newbie_scene[r].range));
	m_role_init_attr.scene_y = m_newbie_scene[r].y + (m_newbie_scene[r].range == 0 ? 0 : RandomNum(m_newbie_scene[r].range));
	m_role_init_attr.create_time = EngineAdapter::Instance().Time();
	m_role_init_attr.prof = prof;

	m_role_init_attr.last_online_dayid = EngineAdapter::Instance().DayID();

	return m_role_init_attr;
}

