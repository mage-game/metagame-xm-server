#ifndef __CARD_ZU_CONFIG_H__
#define __CARD_ZU_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "servercommon/cardzudef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "common/tinyxml/tinyxml.h"
#include <set>

struct CardzuOtherConfig
{
	CardzuOtherConfig() : choucard_need_coin(0), choucard_need_gold(0), choucard_batch_need_gold(0), choucard_need_gold_bind(0), coin_choucard_lingli(0), gold_choucard_lingli(0), 
		batch_gold_choucard_lingli(0), gold_bind_choucard_lingli(0), coin_chouka_max_time(0), gold_bind_chouka_max_time(0), choucard_batch_need_item_id(0) {}

	int choucard_need_coin;													// ͭ�ҳ鿨����
	int choucard_need_gold;													// Ԫ����������
	int choucard_batch_need_gold;											// Ԫ������������
	int choucard_need_gold_bind;											// ���굥������
	int coin_choucard_lingli;												// ͭ�ҳ鿨�������
	int gold_choucard_lingli;												// Ԫ������������
	int batch_gold_choucard_lingli;											// Ԫ��ʮ����������
	int gold_bind_choucard_lingli;											// ���굥��������
	int coin_chouka_max_time;												// ͭ�ҳ鿨ÿ�����ƴ���
	int gold_bind_chouka_max_time;											// ����鿨ÿ�����ƴ���
	int choucard_batch_need_item_id;										// ������������Ʒ
};

struct CardzuActiveConfig
{
	CardzuActiveConfig() : cardzu_id(0), need_active_count(0) {}

	int cardzu_id;															// ����ID
	int need_active_count;													// ������Ҫ�ϲ������
};

struct CardzuCardConfig
{
	CardzuCardConfig() : card_id(0), cardzu_id(0), star_count(0), sell_lingli(0), need_lingli(0), coin_weight(0), gold_weight(0), gold_batch_weight(0), gold_bind_weight(0), broadcast(false) {}

	int card_id;															// ����ID
	int cardzu_id;															// ��������ID
	int star_count;															// ����
	int sell_lingli;														// ����ɵ�
	int need_lingli;														// ��������

	int coin_weight;														// ��ѻ�ȡȨ��
	int gold_weight;														// Ԫ����ȡȨ��
	int gold_batch_weight;													// Ԫ��ʮ����Ȩ��
	int gold_bind_weight;													// �����ȡȨ��

	bool broadcast;						 									// �Ƿ񹫸�
};

struct CardzuZuheConfig
{
	struct AttributeConfig
	{
		AttributeConfig() : zuhe_id(0), zuhe_level(0), upgrade_need_lingli(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

		int zuhe_id;														// �������id
		int zuhe_level;														// ������ϵȼ�
		int upgrade_need_lingli;											// ������������

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
	};

	CardzuZuheConfig() : zuhe_id(0), cardzu_id(0), need_card1_id(0), need_card1_num(0), need_card2_id(0), need_card2_num(0),
		need_card3_id(0), need_card3_num(0), zuhe_max_level(0) {}

	const AttributeConfig *GetAttributeCfg(int zuhe_level) const;

	int zuhe_id;															// �������id
	int cardzu_id;															// ��������ID

	int need_card1_id;														// ������迨��1 id
	int need_card1_num;														// ������迨��1 ����
	int need_card2_id;														// ������迨��2 id
	int need_card2_num;														// ������迨��2 ����
	int need_card3_id;														// ������迨��3 id
	int need_card3_num;														// ������迨��3 ����

	int zuhe_max_level;														// �������Ը���
	AttributeConfig attribute_cfglist[CARDZU_ZUHE_MAX_LEVEL + 1];			// ���������б���ͬ�ȼ���
};

class CardzuConfig : public ConfigBase
{
public:
	typedef std::set<int> CardzuCardSet;
	typedef std::set<int>::iterator CardzuCardSetIt;

	typedef std::set<int> CardzuZuheSet;
	typedef std::set<int>::iterator CardzuZuheSetIt;

	CardzuConfig();
	~CardzuConfig();

	bool Init(const std::string &configname, std::string *err);

	const int GetCardzuIDWithCardID(int card_idx) const;					// ���ݿ���ID��ȡ�ÿ�����������ID
	const int GetCardzuIDWithZuheID(int zuhe_idx) const;					// ���ݿ������ID��ȡ�ÿ�����������ID

	const CardzuOtherConfig & GetOtherCfg() { return m_other_cfg; }
	int GetCardzuNeedActiveCount(int carzu_id);
	const CardzuCardConfig * GetCardCfg(int card_idx);
	const CardzuZuheConfig * GetZuheCfg(int zuhe_idx);
	const CardzuZuheConfig::AttributeConfig * GetZuheAttributeCfg(int zuhe_idx, int zuhe_level);
	CardzuZuheSet *GetZuheSet(int cardzu_id);
	const CardzuCardConfig *GetRandomCard(int choutcard_type, int cardzu_id, int card_star_num_min, int card_star_num_max);

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitCardzuInfoCfg(PugiXmlNode RootElement);
	int InitCardInfoCfg(PugiXmlNode RootElement);
	int InitZuheInfoCfg(PugiXmlNode RootElement);
	int InitZuheUpgradeCfg(PugiXmlNode RootElement);

	CardzuOtherConfig m_other_cfg;
	CardzuActiveConfig m_cardzu_active_cfglist[CARDZU_TYPE_MAX_COUNT];			// ������Ϣ

	int m_max_card_id;
	CardzuCardConfig m_card_cfglist[CARDZU_MAX_CARD_ID + 1];					// ������Ϣ

	int m_max_zuhe_id;
	CardzuZuheConfig m_zuhe_cfglist[CARDZU_MAX_ZUHE_ID + 1];					// ���������Ϣ

	CardzuCardSet m_cardzu_cardset_list[CARDZU_TYPE_MAX_COUNT];					// ����ӳ�����ݣ����飩
	CardzuZuheSet m_cardzu_zuheset_list[CARDZU_TYPE_MAX_COUNT];					// �������ӳ�����ݣ����飩
};

#endif

