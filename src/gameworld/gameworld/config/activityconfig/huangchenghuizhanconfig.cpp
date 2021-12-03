#include "huangchenghuizhanconfig.hpp"

#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include <string>

#include "item/itempool.h"


HuangchenghuizhanConfig::HuangchenghuizhanConfig()
{
}

HuangchenghuizhanConfig::~HuangchenghuizhanConfig()
{

}

bool HuangchenghuizhanConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "HuangchenghuizhanConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("refresh");
		if (element.empty())
		{
			*err = configname + ": no [refresh].";
			return false;
		}

		iRet = this->InitRefreshCfg(element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRefreshCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("pos");
		if (element.empty())
		{
			*err = configname + ": no [pos].";
			return false;
		}

		iRet = this->InitPosCfg(element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPosCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int HuangchenghuizhanConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

 	if (!PugiGetSubNodeValue(dataElement, "scene_id", m_other_cfg.sceneid))
 	{
 		return -1;
 	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.sceneid);

	if (!PugiGetSubNodeValue(dataElement, "time", m_other_cfg.time) || m_other_cfg.time <= 0)
	{
		return -2;
	}

	return 0;
}

int HuangchenghuizhanConfig::InitRefreshCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while(!dataElement.empty())
	{
		HchzRefreshCfg item_cfg;

		if (!PugiGetSubNodeValue(dataElement, "monsterid", item_cfg.monsterid) || !MONSTERPOOL->IsMonsterExist(item_cfg.monsterid))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "num", item_cfg.num) || item_cfg.num <= 0)
		{
			return -2;
		}

		m_refresh_vec.push_back(item_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int HuangchenghuizhanConfig::InitPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while(!dataElement.empty())
	{
		Posi pos;
		if (!PugiGetSubNodeValue(dataElement, "posx", pos.x) ||  pos.x <= 0)
		{
			return -1;
		}
		if (!PugiGetSubNodeValue(dataElement, "posy", pos.y) ||  pos.y <= 0)
		{
			return -2;
		}

		m_pos_vec.push_back(pos);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

void HuangchenghuizhanConfig::GetRandomRefresPos(Posi *posi)
{
	const int pos_size = static_cast<int>(m_pos_vec.size());
	if (NULL == posi || pos_size <= 0)
	{
		return;
	}

	int rand_index = RandomNum(pos_size);
	HchzPosVec::const_iterator iter = m_pos_vec.begin() + rand_index;
	posi->x = iter->x;
	posi->y = iter->y;
}

bool HuangchenghuizhanConfig::IsHuangchenghuizhanMonsterId(int monsterid)
{
	for (HchzRefreshVec::const_iterator iter = m_refresh_vec.begin(); m_refresh_vec.end() != iter; ++ iter)
	{
		if (monsterid == iter->monsterid)
		{
			return true;
		}
	}

	return false;
}