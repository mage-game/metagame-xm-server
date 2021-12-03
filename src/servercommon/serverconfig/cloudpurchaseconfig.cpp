#include "cloudpurchaseconfig.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/crossdef.hpp"
#include <algorithm>
#include "servercommon/rolecrossrandactivitydef.hpp"

CloudPurchaseConfig::CloudPurchaseConfig() : m_ticket_cfg_count(0), m_item_count(0), m_convert_count(0)
{

}

CloudPurchaseConfig::~CloudPurchaseConfig()
{

}

CloudPurchaseConfig &CloudPurchaseConfig::Instance()
{
	static CloudPurchaseConfig instance;
	return instance;
}

bool CloudPurchaseConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};
	PugiXmlDocument document;

	PUGI_XML_LOAD_AND_CHECK(document, path, "CloudPurchaseConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load CloudPurchaseConfig config [%s] Error.cannot find root node.", path.c_str());
		*err = errinfo;
		return false;
	}

	int iRet = 0;

	{
		// otherÅäÖÃ
		PugiXmlNode otherElem = RootElement.child("other");
		if (otherElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->other]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitOther(otherElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitOther fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// È¯ÅäÖÃ
		PugiXmlNode ItemElem = RootElement.child("ticket_type");
		if (ItemElem.empty())
		{
			sprintf(errinfo, "%s:xml node error in node [config->ticket_type]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitTicketCfg(ItemElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitTicketCfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ÎïÆ·ÅäÖÃ
		PugiXmlNode ItemElem = RootElement.child("item_cfg");
		if (ItemElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->item_cfg]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitItemCfg(ItemElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitItemCfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¶Ò»»ÅäÖÃ
		PugiXmlNode ConvertElem = RootElement.child("convert_cfg");
		if (ConvertElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->convert_cfg]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitConvertCfg(ConvertElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitConvertCfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CloudPurchaseConfig::InitTicketCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	m_ticket_cfg_count = 0;

	while (!dataElem.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(dataElem, "seq", seq) || seq < 0 || seq >= CLOUD_PURCHASE_TICKET_TYPE_COUNT_MAX || seq != m_ticket_cfg_count)
		{
			return -1;
		}

		CloudPurchaseTicketCfg &ticket_cfg = m_ticket_cfg_list[seq];
		ticket_cfg.seq = seq;

		if (!PugiGetSubNodeValue(dataElem, "gold_price", ticket_cfg.gold_price) || ticket_cfg.gold_price <= 0 || 0 != ticket_cfg.gold_price % m_other_cfg.qianggou_ticket_gold_price)
		{
			return -2;
		}

		++m_ticket_cfg_count;
		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int CloudPurchaseConfig::InitItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int last_seq = -1;
	while (!dataElem.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(dataElem, "seq", seq) || seq < 0 || seq >= CLOUD_PURCHASE_ITEM_COUNT_MAX || (seq != last_seq + 1))
		{
			return -1;
		}

		last_seq = seq;
		CloudPurchaseItemCfg &cfg = m_item_cfg_list[seq];

		{
			PugiXmlNode reward_item = dataElem.child("reward_item");
			if (reward_item.empty())
			{
				return -3;
			}

			if (!cfg.reward_item.ReadConfigNoCheck(reward_item))
			{
				return -4;
			}
		}

		if (!PugiGetSubNodeValue(dataElem, "need_count", cfg.draw_lottery_need_buy_count) || cfg.draw_lottery_need_buy_count <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "need_ticket_num", cfg.need_ticket_num) || cfg.need_ticket_num <= 0)
		{
			return -6;
		}

		++ m_item_count;
		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int CloudPurchaseConfig::InitConvertCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int limit_convert_count = 0;
	int last_seq = -1;
	while (!dataElem.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(dataElem, "seq", seq) || seq < 0 || seq >= CLOUD_PURCHASE_CONVERT_ITEM_COUNT || (seq != last_seq + 1))
		{
			return -1;
		}

		last_seq = seq;
		CloudPurchaseConvertCfg &cfg = m_convert_cfg_list[seq];

		if (!PugiGetSubNodeValue(dataElem, "cost_score", cfg.cost_score) || cfg.cost_score <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "item_id", cfg.item_id) || cfg.item_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "convert_count_limit", cfg.count_limit) || cfg.count_limit < 0)
		{
			return -4;
		}

		if (cfg.count_limit > 0)
		{
			limit_convert_count ++;

			if (limit_convert_count > MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT)
			{
				return -5;
			}
		}

		++ m_convert_count;
		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int CloudPurchaseConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElem, "draw_lottery_time_interval", m_other_cfg.draw_lottery_interval) || m_other_cfg.draw_lottery_interval < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElem, "ticket_gold_price", m_other_cfg.qianggou_ticket_gold_price) || m_other_cfg.qianggou_ticket_gold_price <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElem, "score_per_gold", m_other_cfg.score_per_gold) || m_other_cfg.score_per_gold <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElem, "buy_interval", m_other_cfg.buy_interval) || m_other_cfg.buy_interval < 0)
	{
		return -5;
	}
	
	return 0;
}

const CloudPurchaseItemCfg *CloudPurchaseConfig::GetItemCfg(int seq)
{
	if (seq < 0 || seq >= CLOUD_PURCHASE_ITEM_COUNT_MAX)
	{
		return NULL;
	}

	return &m_item_cfg_list[seq];
}

const CloudPurchaseConvertCfg *CloudPurchaseConfig::GetConvertCfg(int seq)
{
	if (seq < 0 || seq >= CLOUD_PURCHASE_CONVERT_ITEM_COUNT)
	{
		return NULL;
	}
	
	return &m_convert_cfg_list[seq];
}

const CloudPurchaseTicketCfg * CloudPurchaseConfig::GetCloudPurchaseTicketCfg(int seq)
{
	if (seq < 0 || seq >= m_ticket_cfg_count || seq >= CLOUD_PURCHASE_TICKET_TYPE_COUNT_MAX)
	{
		return nullptr;
	}

	if (m_ticket_cfg_list[seq].seq != seq)
	{
		return nullptr;
	}

	return &m_ticket_cfg_list[seq];
}