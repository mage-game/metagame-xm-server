/************************************************************************
 *																		* 
 * ˵����һЩͨ�õ����ýṹ�ͷ���										*
 *																		*
 ************************************************************************/

#ifndef __GAME_CONFIG_COMMON_H__
#define __GAME_CONFIG_COMMON_H__

#include <vector>
#include <string>

#include "servercommon/configcommon.h"
#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "common/typetransform.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Character;
struct AttributesConfig;
class CharIntAttrs;

//-------------------------------------------------------------------------------------------

// ��������ת����std::string
#define ToString(X) std::string(TypeTransform(X).ToString())

//-------------------------------------------------------------------------------------------

// ����

// ���ָ����ֽ��ַ������浽result����طֽ���ַ�������
// inline int ParseParam(const char *src, std::vector<std::string> &result, const char *delim)
// {
// 	static char temp[1024] = {};
// 
// 	STRNCPY(temp, src, sizeof(temp));
// 	result.clear();
// 
// 	char *token = strtok(temp, delim);
// 
// 	while (NULL != token)
// 	{
// 		result.emplace_back(token);
// 		token = strtok(NULL, delim);
// 	}
// 
// 	return (int)result.size();
// }

//-------------------------------------------------------------------------------------------

// �������ýṹ��

// ����10|10
struct StrListConfig
{
	StrListConfig() {}

	bool ReadConfig(TiXmlElement * data_element, const char *node);

	bool ReadConfig(pugi::xml_node data_element, const pugi::char_t* name);

	enum CHECK_POLICY
	{
		CHECK_POLICY_NONE,				// ��
		CHECK_POLICY_GRANTER_0,			// ����0
		CHECK_POLICY_MONSTER,			// ���monster
	};
	bool Check(int policy);

	int Count()
	{
		return (int)str_list.size();
	}

	const std::string* Value(int i) const
	{
		if (i < 0 || i >= (int)str_list.size())
		{
			return NULL;
		}
		return &str_list[i];
	}

	const int* ValueInt(int i) const
	{
		if (i < 0 || i >= (int)str_list.size())
		{
			return NULL;
		}

		static int n = 0;
		if (StringToBaseType(str_list[i], n))
		{
			return &n;
		}

		return NULL;
	}

private:
	std::vector<std::string> str_list;
};

// ��������
struct AttributesConfig
{
	friend AttributesConfig operator-(const AttributesConfig &lhs, const AttributesConfig &rhs);
	friend AttributesConfig operator+(const AttributesConfig &lhs, const AttributesConfig &rhs);
	bool operator!=(const AttributesConfig &rhs) const;

	AttributesConfig() { this->Reset(); }
	void Reset()
	{
		maxhp = 0;
		gongji = 0;
		fangyu = 0;
		mingzhong = 0;
		shanbi = 0;
		baoji = 0;
		jianren = 0;
	}

	int ReadConfig(PugiXmlNode  data_element);
	void AddAttrs(CharIntAttrs &attrs, double factor = 1.0) const;			// config_attrs �ӵ� base_attrs
	void AddFightAttrs(CharIntAttrs &attrs, double factor = 1.0) const;		// config_attrs �ӵ� fight_attrs

	Attribute maxhp;					// ���Ѫ��
	Attribute gongji;					// ����
	Attribute fangyu;					// ����
	Attribute mingzhong;				// ����
	Attribute shanbi;					// ����
	Attribute baoji;					// ����
	Attribute jianren;					// ���ͣ�������

};

//////////////////////////////////////////////////////////////////////////
static constexpr int DRAW_ITEM_MAX_COUNT = 100;					// �齱��Ŀ�������


//////////////////////////////////////////////////////////////////////////
// ��������Ʒ
struct ConsumeItemConfig
{
	bool ReadConfig(const pugi::xml_node &data_element, const char *item_id_name, const char *item_num_name)
	{
		if (!PugiGetSubNodeValue(data_element, item_id_name, item_id) || NULL == ITEMPOOL->GetItem(item_id)) return false;
		if (!PugiGetSubNodeValue(data_element, item_num_name, item_num) || item_num <= 0) return false;

		return true;
	}

	bool IsValid() const
	{
		return ITEMPOOL->GetItem(item_id) && item_num > 0;
	}

	ItemID item_id = 0;
	short item_num = 0;
};

// �����Ŀ���������ڳ齱��
struct RandItemConfig
{
	struct RandItem
	{
		RandItem() = default;
		RandItem(int _seq, int _weight) : seq(_seq), weight(_weight) {}

		int seq = 0;
		int weight = 0;
	};

	bool ReadConfig(const pugi::xml_node &data_element, const char *weight_str)
	{
		RandItem ri;
		if (!PugiGetSubNodeValue(data_element, "seq", ri.seq) || ri.seq < 0) return false;
		if (!PugiGetSubNodeValue(data_element, weight_str, ri.weight) || ri.weight < 0) return false;
		rand_item_list.emplace_back(ri);
		return true;
	}

	bool ReadConfig(const pugi::xml_node &data_element, const char *seq_str, const char *weight_str)
	{
		RandItem ri;
		if (!PugiGetSubNodeValue(data_element, seq_str, ri.seq) || ri.seq < 0) return false;
		if (!PugiGetSubNodeValue(data_element, weight_str, ri.weight) || ri.weight < 0) return false;
		rand_item_list.emplace_back(ri);
		return true;
	}

	void Insert(int seq, int weight)
	{
		rand_item_list.emplace_back(seq, weight);
	}

	void Insert(const RandItem &rand_item)
	{
		rand_item_list.emplace_back(rand_item);
	}

	bool Empty() const { return rand_item_list.empty(); }
	int Size() const { return (int)rand_item_list.size(); }

	RandItem& operator[](int i) { return rand_item_list[i]; }

	// �����һ����Ŀ
	const RandItem * Rand() const;

	std::vector<RandItem> rand_item_list;
};

// ��item
inline bool ReadItemConfig(const pugi::xml_node &data_element, const char *item_str, ItemConfigData &item)
{
	pugi::xml_node item_element = data_element.child(item_str);
	if (item_element.empty())
	{
		return false;
	}
	if (!item.ReadConfig(item_element))
	{
		return false;
	}
	return true;
}

#endif
