
#include "itemlistparam.h"
#include "servercommon/servercommon.h"
#include <servercommon/configcommon.h>

bool ItemConfigData::ReadConfigNoCheck(const PugiXmlNode & element)
{
	if (!PugiGetSubNodeValue(element, "item_id", item_id)) return false;

	if (!PugiGetSubNodeValue(element, "is_bind", is_bind)) return false;

	if (!PugiGetSubNodeValue(element, "num", num) || num <= 0) return false;

	return true;
}

int ItemConfigData::ReadConfigListNoCheck(const PugiXmlNode & element, const std::string & element_name, ItemConfigData * item_list, int max_count)
{
	if (NULL == element || NULL == item_list)
	{
		return -1;
	}

	int item_count = 0;

	PugiXmlNode item_list_element = element.child((element_name + "_list").c_str());
	if (item_list_element.empty())
	{
		return -2;
	}

	if (!item_list_element.empty())
	{
		PugiXmlNode item_element = item_list_element.child(element_name.c_str());
		while (!item_element.empty())
		{
			if (item_count >= max_count)
			{
				return -3;
			}

			ItemConfigData &cfg = item_list[item_count];

			if (PugiGetSubNodeValue(item_element, "item_id", cfg.item_id) && 0 != cfg.item_id)
			{
				if (!PugiGetSubNodeValue(item_element, "is_bind", cfg.is_bind))
				{
					return -5;
				}

				if (!PugiGetSubNodeValue(item_element, "num", cfg.num) || cfg.num <= 0)
				{
					return -6;
				}

				item_count++;
			}

			item_element = item_element.next_sibling();
		}
	}

	return item_count;
}

int ItemConfigData::ReadConfigByNameNoCheck(const PugiXmlNode &data_element, const char *element_name)
{
	PugiXmlNode item_element = data_element.child(element_name);
	if (item_element.empty())
	{
		return -10000000;
	}

	if (!this->ReadConfigNoCheck(item_element))
	{
		return -10000000 - this->item_id;
	}

	return 0;
}

int ItemConfigData::ReadConfigByNameNoCheck(const PugiXmlNode &data_element, const std::string &element_name)
{
	return this->ReadConfigByNameNoCheck(data_element, element_name.c_str());
}

bool ItemConfigData::ReadConfigNoCheck(TiXmlElement *element)
{
	if (!GetSubNodeValue(element, "item_id", item_id)) return false;

	if (!GetSubNodeValue(element, "is_bind", is_bind)) return false;

	if (!GetSubNodeValue(element, "num", num) || num <= 0) return false;

	return true;
}

int ItemConfigData::ReadConfigListNoCheck(TiXmlElement *element, const std::string &element_name, ItemConfigData *item_list, int max_count)
{
	if (NULL == element || NULL == item_list)
	{
		return -1;
	}

	int item_count = 0;

	TiXmlElement *item_list_element = element->FirstChildElement(element_name + "_list");
	if (NULL == item_list_element)
	{
		return -2;
	}

	if (NULL != item_list_element)
	{
		TiXmlElement *item_element = item_list_element->FirstChildElement(element_name);
		while (NULL != item_element)
		{
			if (item_count >= max_count)
			{
				return -3;
			}

			ItemConfigData &cfg = item_list[item_count];

			if (GetSubNodeValue(item_element, "item_id", cfg.item_id) && 0 != cfg.item_id)
			{
				if (!GetSubNodeValue(item_element, "is_bind", cfg.is_bind))
				{	
					return -5;
				}

				if (!GetSubNodeValue(item_element, "num", cfg.num) || cfg.num <= 0)
				{	
					return -6;
				}

				item_count++;
			}

			item_element = item_element->NextSiblingElement(element_name);
		}
	}

	return item_count;
}

bool ItemListParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret)
	{
		return false;
	}

	for (int i = 0; i < count && i < ItemNamespace::MAX_GRID_NUM; ++i)
	{
		ret = outstream.Push(item_list[i].change_state_item) 
			&& outstream.Push(item_list[i].index);
		if (!ret)
		{
			return false;
		}

		if (!item_list[i].item_wrapper.Serialize(outstream)) return false;
	}

	return ret;
}

bool ItemListParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret)
	{
		return false;
	}

	for (int i = 0; i < count && i < ItemNamespace::MAX_GRID_NUM; ++i)
	{
		ret = instream.Pop(&item_list[i].change_state_item)
			&& instream.Pop(&item_list[i].index);
		if (!ret)
		{
			return false;
		}

		if (!item_list[i].item_wrapper.Unserialize(instream)) return false;
	}

	return ret;
}

