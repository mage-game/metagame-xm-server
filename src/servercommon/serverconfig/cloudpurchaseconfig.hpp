#ifndef __CLOUDPURCHASE_CONFIG_HPP__
#define __CLOUDPURCHASE_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/crossrandactivitydef.hpp"

static const int CLOUD_PURCHASE_CONVERT_ITEM_COUNT = 30;				// ��������һ���Ʒ����

struct CloudPurchaseOtherCfg
{
	CloudPurchaseOtherCfg() : buy_interval(0), draw_lottery_interval(0), qianggou_ticket_gold_price(0), score_per_gold(0) {}

	int buy_interval;								// ������
	int draw_lottery_interval;						// �������
	int qianggou_ticket_gold_price;					// ����ȯԪ���۸�
	int score_per_gold;								// ÿԪ������x����
};

struct CloudPurchaseTicketCfg
{
	CloudPurchaseTicketCfg() : seq(0), gold_price(0) {}

	int seq;
	int gold_price;
};

struct CloudPurchaseItemCfg
{
	CloudPurchaseItemCfg() : draw_lottery_need_buy_count(0), need_ticket_num(0) {}

	int draw_lottery_need_buy_count;		// ������Ҫ�������
	int need_ticket_num;					// ����һ����Ҫȯ��
	ItemConfigData reward_item;				// ��Ʒ				
};

struct CloudPurchaseConvertCfg
{
	CloudPurchaseConvertCfg() : cost_score(0), item_id(0), count_limit(0) {}

	int cost_score;					// �һ��������
	ItemID item_id;					// �һ���Ʒid
	int count_limit;				// �һ�����
};

class CloudPurchaseConfig
{
public:
	CloudPurchaseConfig();
	~CloudPurchaseConfig();

	static CloudPurchaseConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CloudPurchaseOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const CloudPurchaseItemCfg * GetItemCfg(int seq);
	const CloudPurchaseConvertCfg * GetConvertCfg(int seq);

	int GetItemCount() { return m_item_count; }
	const int GetCloudPurchaseTicketCount() { return m_ticket_cfg_count; }
	const CloudPurchaseTicketCfg * GetCloudPurchaseTicketCfg(int seq);
	 
private:
	int InitOther(PugiXmlNode RootElement);
	int InitTicketCfg(PugiXmlNode RootElement);
	int InitItemCfg(PugiXmlNode RootElement);
	int InitConvertCfg(PugiXmlNode RootElement);

	CloudPurchaseOtherCfg m_other_cfg;

	int m_ticket_cfg_count;
	CloudPurchaseTicketCfg m_ticket_cfg_list[CLOUD_PURCHASE_TICKET_TYPE_COUNT_MAX];

	int m_item_count;
	CloudPurchaseItemCfg m_item_cfg_list[CLOUD_PURCHASE_ITEM_COUNT_MAX];

	int m_convert_count;
	CloudPurchaseConvertCfg m_convert_cfg_list[CLOUD_PURCHASE_CONVERT_ITEM_COUNT];
};

#endif	//__CLOUDPURCHASE_CONFIG_HPP__

