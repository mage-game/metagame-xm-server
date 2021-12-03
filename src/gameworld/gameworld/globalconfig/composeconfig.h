#ifndef COMPOSECONFIG_H
#define COMPOSECONFIG_H

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include "item/itemfunction.h"
#include "item/itembase.h"

static const int MAX_COMPOSE_STUFF_TYPE = 4;						// �ϳɲ�����������

struct ProductConfig
{
	ProductConfig() : m_product_seq(0), m_product_id(0), m_coin(0), m_nv_wa_shi(0)
	{
		memset(m_stuff_id_list, 0, sizeof(m_stuff_id_list));
		memset(m_stuff_count_list, 0, sizeof(m_stuff_count_list));
	}

	int m_product_seq;
	ItemID m_product_id;											// ����ID
	int m_coin;														// ����ͭǮ
	int m_nv_wa_shi;												// Ů�ʯ
	ItemID m_stuff_id_list[MAX_COMPOSE_STUFF_TYPE];					// ����
	int m_stuff_count_list[MAX_COMPOSE_STUFF_TYPE];					// ����
	int is_broadcast;
};

class ComposeConfig : public ConfigBase
{
public:
	ComposeConfig();
	~ComposeConfig();

	bool Init(const std::string &configname, std::string *err);
	const ProductConfig * GetProductCfg(int produect_seq) const;

private:
	int InitProductCfg(PugiXmlNode RootElement);
	std::map<int, ProductConfig> m_product_map;
};


#endif
