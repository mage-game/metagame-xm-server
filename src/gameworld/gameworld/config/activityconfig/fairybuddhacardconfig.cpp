#include "fairybuddhacardconfig.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/vipdef.hpp"

FairyBuddhaCardConfig::FairyBuddhaCardConfig() : use_gold(0)
{

}
FairyBuddhaCardConfig::~FairyBuddhaCardConfig()
{

}


bool FairyBuddhaCardConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	TiXmlDocument document;

	if (configname == "" || !document.LoadFile(configname))
	{
		sprintf(errinfo,"%s: Load FairyBuddhaCardConfig Error,\n %s.", configname.c_str(), document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	// ÏÉ×ð¿¨ÅäÖÃ
	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = configname + ": xml root node error";
		return false;
	}

	{
		TiXmlElement *CommonElement = RootElement->FirstChildElement("fairy_buddha_card");
		if (NULL == CommonElement)
		{
			*err = configname + ": xml reward node error in [root->fairy_buddha_card].";
			return false;
		}

		TiXmlElement *DataElement = CommonElement->FirstChildElement("data");
		if (NULL == DataElement)
		{
			*err = configname + ": xml reward node error in [root->fairy_buddha_card->data].";
			return false;
		}

		{
			while(NULL != DataElement)
			{
				int card_type = 0;
				if (!GetSubNodeValue(DataElement, "card_type", card_type) || card_type <= 0 || card_type >= FAIRY_BUDDHA_CARD_TYPE_MAX)
				{
					*err = configname + ": xml node error in [root->fairy_buddha_card->data->card_type].";
					return false;
				}

				FairyBuddhaCardCfg &cfg_card = m_fairy_buddha_card_cfg[card_type];

				if (!GetSubNodeValue(DataElement, "use_gold", cfg_card.use_gold) || cfg_card.use_gold < 0)
				{
					*err = configname + ": xml node error in [root->fairy_buddha_card->data->use_gold].";
					return false;
				}

				if (!GetSubNodeValue(DataElement, "add_gongji", cfg_card.add_gongji) ||  cfg_card.add_gongji < 0)
				{
					*err = configname + ": xml node error in [root->fairy_buddha_card->data->add_gongji].";
					return false;
				}

				if (!GetSubNodeValue(DataElement, "title_id", cfg_card.title_id) ||  cfg_card.title_id < 0)
				{
					*err = configname + ": xml node error in [root->fairy_buddha_card->data->title_id].";
					return false;
				}

				if (!GetSubNodeValue(DataElement, "add_mojing_per", cfg_card.add_mojing_per) ||  cfg_card.add_mojing_per < 0)
				{
					*err = configname + ": xml node error in [root->fairy_buddha_card->data->add_mojing_per].";
					return false;
				}

				if (!GetSubNodeValue(DataElement, "monster_exp", cfg_card.monster_exp) ||  cfg_card.monster_exp < 0)
				{
					*err = configname + ": xml node error in [root->fairy_buddha_card->data->monster_exp].";
					return false;
				}

				if (!GetSubNodeValue(DataElement, "day_gold_bind", cfg_card.day_gold_bind) ||  cfg_card.day_gold_bind < 0)
				{
					*err = configname + ": xml node error in [root->fairy_buddha_card->data->day_gold_bind].";
					return false;
				}

				if (!GetSubNodeValue(DataElement, "useful_time", cfg_card.useful_time) || cfg_card.useful_time < 0)
				{
					*err = configname + ": xml node error in [root->fairy_buddha_card->data->useful_time].";
					return false;
				}

				TiXmlElement *Itemelement = DataElement->FirstChildElement("stuff_item");
				if (!cfg_card.stuff_item.ReadConfig(Itemelement))
				{
					*err = configname + ": xml node error in [root->fairy_buddha_card->data->stuff_item].";
					return false;
				}

				DataElement = DataElement->NextSiblingElement();
			}
		}
	}


	//{
	//	TiXmlElement *CommonElement = RootElement->FirstChildElement("other");
	//	if (NULL == CommonElement)
	//	{
	//		*err = configname + ": xml reward node error in [root->other].";
	//		return false;
	//	}

	//	TiXmlElement *DataElement = CommonElement->FirstChildElement("data");
	//	if (NULL == DataElement)
	//	{
	//		*err = configname + ": xml reward node error in [root->other->data].";
	//		return false;
	//	}

	//	if(NULL != DataElement)
	//	{
	//		if (!GetSubNodeValue(DataElement, "use_gold", use_gold) ||  use_gold <= 0)
	//		{
	//			*err = configname + ": xml node error in [root->other->data->use_gold_bind].";
	//			return false;
	//		}
	//	}
	//}

	return true;
}


const  FairyBuddhaCardCfg * FairyBuddhaCardConfig::GetFairyBuddhaCardCfg(int card_type)
{
	if(card_type <= 0 || card_type >= FAIRY_BUDDHA_CARD_TYPE_MAX) return NULL;

	return &m_fairy_buddha_card_cfg[card_type];
}

const int FairyBuddhaCardConfig::GetFairyBuddhaAllGoldBind()
{
	if(use_gold < 0) return 0;

	return use_gold;
}
