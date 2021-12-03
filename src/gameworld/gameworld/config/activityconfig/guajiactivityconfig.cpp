#include "guajiactivityconfig.hpp"
#include "servercommon/configcommon.h"
#include "gameworld/gameworld/checkresourcecenter.hpp"

GuaJiActivityConfig::GuaJiActivityConfig()
	: m_reward_interval(0), m_reward_times(0), m_scene_id_count(0)
{
	memset(m_scene_id_list,0,sizeof(m_scene_id_list));
}

GuaJiActivityConfig::~GuaJiActivityConfig()
{

}

bool GuaJiActivityConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;

	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "GuaJiActivityConfig", *err);
	
	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + ": xml no other node";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 挂机活动场景ID配置
	{
		PugiXmlNode root_element = RootElement.child("guaji_activity");
		if (root_element.empty())
		{
			*err = path + ": xml no guaji_activity node";
			return false;
		}

		iRet = this->InitGuaJiSceneIdCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGuaJiSceneIdCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}


int GuaJiActivityConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "reward_interval", m_reward_interval) || m_reward_interval <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "reward_times", m_reward_times) || m_reward_times <= 0)
	{
		return -2;
	}

	return 0;
}

int GuaJiActivityConfig::InitGuaJiSceneIdCfg(PugiXmlNode RootElement)
{
	PugiXmlNode DataElement = RootElement.child("data");
	if (DataElement.empty())
	{
		return -3;
	}
	
	int count = 0;
	while (!DataElement.empty())
	{
		if (count >= GUAJI_SCENE_ID_MAX)
		{
			return -4;
		}

		int scene_id = 0;
		if (!PugiGetSubNodeValue(DataElement, "scene_id", scene_id) || scene_id <= 0)
		{
			return -5;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(scene_id);

		if (this->IsGuaJiSceneID(scene_id))
		{
			return -6;
		}
		m_scene_id_set.insert(scene_id);

		m_scene_id_list[count ++] = scene_id;

		DataElement = DataElement.next_sibling();
	}

	m_scene_id_count = count;

	return 0;
}

bool GuaJiActivityConfig::IsGuaJiSceneID(int scene_id)
{
	std::set<int>::iterator it = m_scene_id_set.find(scene_id);
	if (it != m_scene_id_set.end())
	{
		return true;
	}

	return false;
}

int GuaJiActivityConfig::GetGuaJiSceneIdByIndex(int index)
{
	if (index >= 0 && index < GUAJI_SCENE_ID_MAX)
	{
		return m_scene_id_list[index];
	}

	return 0;
}


