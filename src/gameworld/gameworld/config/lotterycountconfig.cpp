#include "lotterycountconfig.hpp"

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/configbase.h"
#include "item/itempool.h"
#include "config/logicconfigmanager.hpp"
#include "global/globalrole/litttlepet/littlepetconfig.hpp"
#include "other/shop/mysteriousshopinmallconfig.hpp"
#include "other/shengesystem/shengesystemconfig.hpp"
#include "other/shenzhouweapon/shenzhouweaponconfig.hpp"
#include "other/chinesezodiac/chinesezodiacconfig.hpp"

LotteryCountConfig::LotteryCountConfig()
{
	
}

LotteryCountConfig::~LotteryCountConfig()
{

}

bool LotteryCountConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "LotteryCountConfig::Init", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	// ÆäËûÅäÖÃ
	{
		PugiXmlNode element = RootElement.child("replace_list");
		if (element.empty())
		{
			*err = configname + ": no [replace_list].";
			return false;
		}

		iRet = this->InitReplaceListCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitReplaceListCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	return true;
}

const LotteryCountReplaceSequenceCfg * LotteryCountConfig::GetReplaceListCfg(int type, int seq)
{
	if (type <= LOTTERY_REPLACE_TYPE_INVALID || type >= LOTTERY_REPLACE_TYPE_MAX || seq < 0 || seq >= MAX_LOTTERY_REPLACE_SEQUENCE_COUNT)
	{
		return nullptr;
	}
	return &lottery_count_replace_sequence_cfg[type][seq];
}

int LotteryCountConfig::InitReplaceListCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	int type = 0;
	int seq = 0;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "type", type) || type <= LOTTERY_REPLACE_TYPE_INVALID || type >= LOTTERY_REPLACE_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq < 0 || seq >= MAX_LOTTERY_REPLACE_SEQUENCE_COUNT)
		{
			return -2;
		}

		LotteryCountReplaceSequenceCfg &cfg = lottery_count_replace_sequence_cfg[type][seq];

		if (!PugiGetSubNodeValue(dataElement, "can_replace_need_count_person", cfg.can_replace_need_count_person) || cfg.can_replace_need_count_person < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "replace_need_count_person", cfg.replace_need_count_person) || cfg.replace_need_count_person < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "replace_need_count_world", cfg.replace_need_count_world) || cfg.replace_need_count_world < 0)
		{
			return -5;
		}

		for (int i = 0; i < LOTTERY_SEQUENCE_TYPE_MAX; ++i)
		{
			char column_name[32] = { 0 };
			SNPRINTF(column_name, sizeof(column_name), "replace_sequence_%d", i);
			if (!PugiGetSubNodeValue(dataElement, column_name, cfg.replace_sequence[i]))
			{
				return -10 - i;
			}
			int check_result = this->CheckReplaceSequence(type, cfg.replace_sequence[i]);
			if (check_result < 1)
			{
				return -100 + check_result;
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int LotteryCountConfig::CheckReplaceSequence(int type, LotterySequence replace_sequence)
{
	switch (type)
	{
	case LOTTERY_REPLACE_TYPE_LITTERY_PET:
	{
		if (!LOGIC_CONFIG->GetLittlePetCfg().CheckSeq(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP:
	{
		if (NULL == ITEMPOOL->GetItem(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP1:
	{
		if (NULL == ITEMPOOL->GetItem(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP2:
	{
		if (NULL == ITEMPOOL->GetItem(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_CHEST_SHOP_JINGLING:
	{
		if (NULL == ITEMPOOL->GetItem(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP3:
	{
		if (NULL == ITEMPOOL->GetItem(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_6:
	{
		// todo
	}
	break;
	case LOTTERY_REPLACE_TYPE_7:
	{
		// todo
	}
	break;
	case LOTTERY_REPLACE_TYPE_8:
	{
		// todo
	}
	break;
	case LOTTERY_REPLACE_TYPE_CHINESE_ZODIAC:
	{
		if (!LOGIC_CONFIG->GetChineseZodiacCfg().CheckTypeIndex(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_SHENGESYS:
	{
		if (!LOGIC_CONFIG->GetShengesystemCfg().CheckSeq(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_SHENZHOUWEAPON:
	{
		if (!LOGIC_CONFIG->GetShenzhouWeaponCfg().CheckSeq(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_MYSTERIOUS_SHOP:
	{
		if (!LOGIC_CONFIG->GetMysteriousShopInMallCfg().CheckSeq(replace_sequence))
		{
			return -type;
		}
	}
	break;
	case LOTTERY_REPLACE_TYPE_13:
	{
		// todo
	}
	break;
	case LOTTERY_REPLACE_TYPE_14:
	{
		// todo
	}
	break;
	default:break;
	}
	return 1;
}

