#include "giftreplaceconfig.hpp"

#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "item/itembase.h"

GiftReplaceConfig::GiftReplaceConfig()
{

}

GiftReplaceConfig::~GiftReplaceConfig()
{

}

bool GiftReplaceConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "GiftReplaceConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 
		PugiXmlNode root_element = RootElement.child("re_gift_by_role_level");
		if (root_element.empty())
		{
			*err = path + " xml not re_gift_by_role_level node ";
			return false;
		}

		iRet = this->InitGiftReplaceCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGiftReplaceCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int GiftReplaceConfig::InitGiftReplaceCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	while (!data_element.empty())
	{
		GiftReplaceCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "gift_id", cfg.gift_id) || cfg.gift_id <= 0)
		{
			return -1;
		}

		const ItemBase *gift_cfg = ITEMPOOL->GetItem(cfg.gift_id);
		if (gift_cfg == nullptr || gift_cfg->GetItemType() != ItemBase::I_TYPE_GIFT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "role_min_level", cfg.role_min_level) || cfg.role_min_level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "role_max_level", cfg.role_max_level) || cfg.role_max_level <= 0 || 
			cfg.role_max_level < cfg.role_min_level)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "regift_id", cfg.re_gift_id) || cfg.re_gift_id <= 0)
		{
			return -5;
		}

		const ItemBase *regift_cfg = ITEMPOOL->GetItem(cfg.re_gift_id);
		if (regift_cfg == nullptr || regift_cfg->GetItemType() != ItemBase::I_TYPE_GIFT)
		{
			return -6;
		}

		GiftReplaceVec &temp_vec = m_gift_replace_map[cfg.gift_id];
		temp_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

bool GiftReplaceConfig::GetReplaceGiftId(ItemID &gift_id, int role_level)
{
	GiftReplaceMapIt it = m_gift_replace_map.find(gift_id);
	if (it != m_gift_replace_map.end())
	{
		GiftReplaceVec replace_vec = it->second;
		for (int i = 0; i < (int)replace_vec.size(); ++i)
		{
			if (role_level >= replace_vec[i].role_min_level && role_level <= replace_vec[i].role_max_level)
			{
				gift_id = replace_vec[i].re_gift_id;
				return true;
			}
		}
	}

	return false;
}