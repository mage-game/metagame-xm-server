#include "friendgiftconfig.hpp"
#include "obj/character/attribute.hpp"
#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"

FriendGiftConfig::FriendGiftConfig()
{

}

FriendGiftConfig::~FriendGiftConfig()
{

}

bool FriendGiftConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "FriendGiftConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode elem = RootElement.child("other");
		if (NULL == elem)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitOther(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitOther fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("friend_heli_cond");
		if (NULL == elem)
		{
			sprintf(errinfo,"%s:xml node error in node [config->friend_heli_cond]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitFriendHeliCfg(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitFriendHeliCfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int FriendGiftConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "shou_gift_count_limit", m_other_cfg.receive_gift_count_limit) || m_other_cfg.receive_gift_count_limit < 0 || m_other_cfg.receive_gift_count_limit > FriendGiftParam::FRIEND_GIFT_MAX_GIFT_COUNT)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "add_intimacy", m_other_cfg.add_intimacy) || m_other_cfg.add_intimacy < 0)
	{
		return -2;
	}

	PugiXmlNode Itemelement = data_element.child("gift_item");
	ItemID itemid = 0;
	if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
	{
		if (!m_other_cfg.gift_item.ReadConfig(Itemelement))
		{
			return -3;
		}
	}
	else
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "free_song_hua_times", m_other_cfg.free_song_hua_times) || m_other_cfg.free_song_hua_times <= 0)
	{
		return -5;
	}
	
	{
		PugiXmlNode Itemelement = data_element.child("free_song_hua_reward");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
		{
			if (!m_other_cfg.free_song_hua_reward.ReadConfig(Itemelement))
			{
				return -6;
			}
		}
		else
		{
			return -7;
		}
	}

	if (!PugiGetSubNodeValue(data_element, "heli_exp", m_other_cfg.heli_exp) || m_other_cfg.heli_exp <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "heli_notice_time_interval", m_other_cfg.heli_notice_time_interval) || m_other_cfg.heli_notice_time_interval <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "start_level", m_other_cfg.start_level) || m_other_cfg.start_level < 0)
	{
		return -10;
	}

	return 0;
}

int FriendGiftConfig::InitFriendHeliCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}
	
	int index = 0;

	while (!data_element.empty())
	{
		if (index < 0 || index >= FRIENDNUM_HELI_COND_TYPE_MAX_COUNT)
		{
			return -1001;
		}

		FriendHeliCfg & cfg = m_friend_heli_cfg[index];

		if (!PugiGetSubNodeValue(data_element, "type", cfg.type) || cfg.type < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "value", cfg.value) || cfg.value < 0)
		{
			return -2;
		}
		
		index++;

		data_element = data_element.next_sibling();
	}

	return 0;
}

bool FriendGiftConfig::IsReqFriendHeli(int type, int value)
{
	for (int i = 0; i < FRIENDNUM_HELI_COND_TYPE_MAX_COUNT; i++)
	{
		if (m_friend_heli_cfg[i].type == type && m_friend_heli_cfg[i].value == value)
		{
			return true;
		}
	}

	return false;
}