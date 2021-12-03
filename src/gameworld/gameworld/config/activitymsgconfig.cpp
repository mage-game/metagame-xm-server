#include "activitymsgconfig.hpp"
#include <string>
#include "gameworld/scene/scene.h"

bool ActivityMsgConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int IRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ActivityMsgConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + "xml root node error";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + "no [other]";
			return false;
		}

		IRet = this->InitOtherCfg(other_element);
		if (IRet)
		{
			sprintf(errinfo, "%s :InitOtherCfg failed : %d", configname.c_str(), IRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode fun_element = RootElement.child("function_open");
		if (fun_element.empty())
		{
			*err = configname + "no [function_open]";
			return false;
		}

		IRet = this->InitFunctionOpenCfg(fun_element);
		if (IRet)
		{
			sprintf(errinfo, "%s :InitFunctionOpenCfg failed : %d", configname.c_str(), IRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode role_dead_element = RootElement.child("role_dead_cfg");
		if (role_dead_element.empty())
		{
			*err = configname + "no [role_dead_cfg]";
			return false;
		}

		IRet = this->InitRoleDeadCfg(role_dead_element);
		if (IRet)
		{
			sprintf(errinfo, "%s :InitRoleDeadCfg  failed :%d", configname.c_str(), IRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode monster_attack_element = RootElement.child("monster_attack_cfg");
		if (monster_attack_element.empty())
		{
			*err = configname + "no [monster_attack_cfg]";
			return false;
		}

		IRet = this->InitMonsterAttackCfg(monster_attack_element);
		if (IRet)
		{
			sprintf(errinfo, "%s :InitMonsterAttackCfg:%d", configname.c_str(), IRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode lianzhan_element = RootElement.child("lian_zhan_cfg");
		if (lianzhan_element.empty())
		{
			*err = configname + "no [lian_zhan_cfg]";
			return false;
		}

		IRet = this->InitLianZhanCfg(lianzhan_element);
		if (IRet)
		{
			sprintf(errinfo, "%s :InitLianZhanCfg:%d", configname.c_str(), IRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode monster_dead_element = RootElement.child("monster_dead_cfg");
		if (monster_dead_element.empty())
		{
			*err = configname + "no [monster_dead_cfg]";
			return false;
		}

		IRet = this->InitMonsterDeadCfg(monster_dead_element);
		if (IRet)
		{
			sprintf(errinfo, "%s :InitMonsterDeadCfg:%d", configname.c_str(), IRet);
			*err = errinfo;
			return false;
		}
	}
	return true;
}
ActivityMsg* ActivityMsgConfig::GetActivityMsgType(int scene_type)
{
	std::map<int, ActivityMsg>::iterator itr = m_function_open.find(scene_type);
	if (itr == m_function_open.end())
	{
		return nullptr;
	}

	return &itr->second;
}

bool ActivityMsgConfig::CheckRoleDeadCount(const int scene_type, const int dead_count) const
{
	std::map<int, RoleDeadCfg>::const_iterator itr = m_role_dead_cfg.find(scene_type);
	if (m_role_dead_cfg.end() != itr)
	{
		for (int i = 0; i < static_cast<int>(itr->second.dead_num.size()); ++i)
		{
			if (itr->second.dead_num[i] == dead_count)
			{
				return true;
			}
		}
	}
	return false;
}

int ActivityMsgConfig::GetMonsterAttackNotifyCD(const int scene_type, const int monster_id) const
{
	std::map<int, MonsterAttackCfg>::const_iterator itr = m_monster_attack_cfg.find(scene_type);
	if (m_monster_attack_cfg.end() == itr)
	{
		return -1;
	}

	for (int i = 0; i < static_cast<int>(itr->second.monster_id.size()); ++i)
	{
		if (monster_id == itr->second.monster_id[i])
		{
			return itr->second.notify_attack_cd;
		}
	}

	return -1;
}

bool ActivityMsgConfig::CheckRoleLianZhanCount(const int scene_type, const int lianzhan)const
{
	std::map<int, LianZhanCfg>::const_iterator itr = m_lian_zhan_cfg.find(scene_type);
	if(m_lian_zhan_cfg.end() != itr)
	{
		for (int i = 0; i < static_cast<int> (itr->second.lianzhan_num.size()); ++i)
		{
			if (lianzhan == itr->second.lianzhan_num[i])
			{
				return true;
			}
		}
	}
	return false;
}

bool ActivityMsgConfig::CheckRoleLianZhanEnd(const int scene_type, const int lianzhan)const
{
	std::map<int, LianZhanCfg>::const_iterator itr = m_lian_zhan_cfg.find(scene_type);
	if (m_lian_zhan_cfg.end() != itr && lianzhan >= itr->second.lianzhan_end)
	{
		return true;
	}
	return false;
}

bool ActivityMsgConfig::NeedMonsterHpNotify(const int scene_type, const int monster_id)const
{
	std::map<int, MonsterAttackCfg>::const_iterator itr = m_monster_attack_cfg.find(scene_type);
	if (m_monster_attack_cfg.end() == itr)
	{
		return false;
	}

	for (int i = 0; i < static_cast<int>(itr->second.monster_hp.size()); ++i)
	{
		if (monster_id == itr->second.monster_hp[i])
		{
			return true;
		}
	}
	return false;
}

int ActivityMsgConfig::CheckMonsterHpPercent(const int scene_type, const int old_percent, const int now_percent)const
{
	std::map<int, MonsterAttackCfg>::const_iterator itr = m_monster_attack_cfg.find(scene_type);
	if (m_monster_attack_cfg.end() == itr)
	{
		return -1;
	}
	
	for (int i = 0; i < static_cast<int>(itr->second.hp_percent.size()); ++i)
	{
		if (now_percent < itr->second.hp_percent[i] && old_percent >= itr->second.hp_percent[i])
		{
			return itr->second.hp_percent[i];
		}
	}

	return -1;
}

int ActivityMsgConfig::NeedMonsterDeadMsg(const int scene_type, const int monster_id)const
{
	std::map<int, MonsterDeadCfg>::const_iterator itr = m_monster_dead_cfg.find(scene_type);
	if (m_monster_dead_cfg.end() == itr)
	{
		return 0;
	}

	for (int i = 0; i < static_cast<int>(itr->second.monster_id.size()); ++i)
	{
		if (monster_id == itr->second.monster_id[i])
		{
			return 1;
		}
	}

	return 0;
}


int ActivityMsgConfig::ReadConfig(PugiXmlNode dataElement, const char * name , std::vector<int> &veci)
{
	if (dataElement.empty())
	{
		return 0;
	}
	std::string str;
	char str_buff[1024] = {0};

	if (!PugiGetSubNodeValue(dataElement, name, str))
	{
		return -101;
	}
	
	if (SNPRINTF(str_buff, sizeof(str_buff), "%s",str.c_str()) < 0 )
	{
		return -102;
	}
	
	char *token = strtok(str_buff, "|");
	while (nullptr != token)
	{
		veci.push_back(atoi(token));
		token = strtok(NULL, "|");
	}

	return 0;
}

int ActivityMsgConfig::InitOtherCfg(PugiXmlNode rootElement)
{
	PugiXmlNode dataElement = rootElement.child("data");
	if(dataElement.empty())
	{
		return -1000;
	}
	
	if (!PugiGetSubNodeValue(dataElement, "guild_time", m_other_cfg.guild_time) || m_other_cfg.guild_time <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "rank_top_interval", m_other_cfg.rank_top_interval) || m_other_cfg.rank_top_interval <= 0)
	{
		return -2;
	}

	return 0;
}

int ActivityMsgConfig::InitFunctionOpenCfg(PugiXmlNode rootElement)
{
	if (rootElement.empty())
	{
		return -1000;
	}

	PugiXmlNode dataElement = rootElement.child("data");
	while (!dataElement.empty())
	{
		int scene_type = -1;
		if (!PugiGetSubNodeValue(dataElement,"scene_type",scene_type) || scene_type < Scene::SCENE_TYPE_COMMON_SCENE || scene_type >= Scene::SCENE_TYPE_COUNT)
		{
			return -1;
		}

		ActivityMsg act_msg;
		if (!PugiGetSubNodeValue(dataElement, "guilder_dead", act_msg.guilder_die) || act_msg.guilder_die < MSG_TYPE_BROADCAST_INVILIDE || 
			act_msg.guilder_die >= MSG_TYPE_BROADCAST_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_dead", act_msg.role_die) || act_msg.role_die < MSG_TYPE_BROADCAST_INVILIDE || 
			act_msg.role_die >= MSG_TYPE_BROADCAST_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_attack", act_msg.monster_attack) || act_msg.monster_attack < MSG_TYPE_BROADCAST_INVILIDE || 
			act_msg.monster_attack >= MSG_TYPE_BROADCAST_MAX)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "lian_zhan", act_msg.lian_zhan) || act_msg.lian_zhan < MSG_TYPE_BROADCAST_INVILIDE ||
			act_msg.lian_zhan >= MSG_TYPE_BROADCAST_MAX)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_dead", act_msg.monster_die) || act_msg.monster_die < MSG_TYPE_BROADCAST_INVILIDE ||
			act_msg.monster_die >= MSG_TYPE_BROADCAST_MAX)
		{
			return -6;
		}
		m_function_open[scene_type] = act_msg;
		dataElement = dataElement.next_sibling();
	}
	return 0;
}
int ActivityMsgConfig::InitRoleDeadCfg(PugiXmlNode rootElement)
{
	if (rootElement.empty())
	{
		return -1000;
	}

	PugiXmlNode dataElement = rootElement.child("data");
	while (!dataElement.empty())
	{
		int scene_type = -1;
		if (!PugiGetSubNodeValue(dataElement, "scene_type", scene_type) || scene_type < Scene::SCENE_TYPE_COMMON_SCENE || scene_type >= Scene::SCENE_TYPE_COUNT)
		{
			return -1;
		}
		std::map<int, ActivityMsg>::iterator itr = m_function_open.find(scene_type);
		if (m_function_open.end() == itr || itr->second.role_die <= MSG_TYPE_BROADCAST_INVILIDE)
		{
			return -2;
		}
		RoleDeadCfg role_dead;
		int ret = this->ReadConfig(dataElement, "dead_num", role_dead.dead_num);
		if (ret)
		{
			return ret;
		}

		m_role_dead_cfg[scene_type] = role_dead;
		dataElement = dataElement.next_sibling();
	}
	return 0;
}
int ActivityMsgConfig::InitMonsterAttackCfg(PugiXmlNode rootElement)
{
	if (rootElement.empty())
	{
		return -1000;
	}
	PugiXmlNode dataElement = rootElement.child("data");
	while (!dataElement.empty())
	{
		int scene_type = -1;
		if (!PugiGetSubNodeValue(dataElement, "scene_type", scene_type) || scene_type < Scene::SCENE_TYPE_COMMON_SCENE || scene_type >= Scene::SCENE_TYPE_COUNT)
		{
			return -1;
		}

		std::map<int, ActivityMsg>::iterator itr = m_function_open.find(scene_type);
		if (m_function_open.end() == itr || itr->second.monster_attack <= MSG_TYPE_BROADCAST_INVILIDE)
		{
			return -2;
		}
		
		MonsterAttackCfg monster_attack;

		if (!PugiGetSubNodeValue(dataElement, "attack_cd", monster_attack.notify_attack_cd)  || monster_attack.notify_attack_cd <0 )
		{
			return -3;
		}

		int ret = this->ReadConfig(dataElement, "monster_id", monster_attack.monster_id);
		if (ret)
		{
			return ret;
		}
		
		ret = this->ReadConfig(dataElement, "monster_hp", monster_attack.monster_hp);
		if (ret)
		{
			return ret;
		}

		ret = this->ReadConfig(dataElement, "hp_percent", monster_attack.hp_percent);
		if (ret)
		{
			return ret;
		}
		m_monster_attack_cfg[scene_type] = monster_attack;
		dataElement = dataElement.next_sibling();
	}
	return 0;
}
int ActivityMsgConfig::InitLianZhanCfg(PugiXmlNode rootElement)
{
	if (rootElement.empty())
	{
		return -1000;
	}
	PugiXmlNode dataElement = rootElement.child("data");
	while (!dataElement.empty())
	{
		int scene_type = -1;
		if (!PugiGetSubNodeValue(dataElement, "scene_type", scene_type) || scene_type < Scene::SCENE_TYPE_COMMON_SCENE || scene_type >= Scene::SCENE_TYPE_COUNT)
		{
			return -1;
		}

		std::map<int, ActivityMsg>::iterator itr = m_function_open.find(scene_type);
		if (m_function_open.end() == itr || itr->second.lian_zhan <= MSG_TYPE_BROADCAST_INVILIDE)
		{
			return -2;
		}

		LianZhanCfg lianzhan;
		int ret = this->ReadConfig(dataElement, "lianzhan_num", lianzhan.lianzhan_num);
		if (ret)
		{
			return ret;
		}

		if (!PugiGetSubNodeValue(dataElement, "lianzhan_end", lianzhan.lianzhan_end) || lianzhan.lianzhan_end <= 0)
		{
			return -3;
		}
		m_lian_zhan_cfg[scene_type] = lianzhan;
		dataElement = dataElement.next_sibling();
	}
	return 0;
}
int ActivityMsgConfig::InitMonsterDeadCfg(PugiXmlNode rootElement)
{
	if (rootElement.empty())
	{
		return -1000;
	}

	PugiXmlNode dataElement = rootElement.child("data");
	while (!dataElement.empty())
	{
		int scene_type = -1;
		if (!PugiGetSubNodeValue(dataElement, "scene_type", scene_type) || scene_type < Scene::SCENE_TYPE_COMMON_SCENE || scene_type >= Scene::SCENE_TYPE_COUNT)
		{
			return -1;
		}

		std::map<int, ActivityMsg>::iterator itr = m_function_open.find(scene_type);
		if (m_function_open.end() == itr || itr->second.monster_die <= MSG_TYPE_BROADCAST_INVILIDE)
		{
			return -2;
		}

		MonsterDeadCfg monster_dead ;
		int ret = this->ReadConfig(dataElement, "monster_id", monster_dead.monster_id);
		if (ret)
		{
			return ret;
		}
		m_monster_dead_cfg[scene_type] = monster_dead;
		dataElement = dataElement.next_sibling();
	}
	return 0;
}