#include "configcommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
//#include "monster/monsterpool.h"

int AttrConfig::ReadConfig(TiXmlElement *data_element)
{
	if (GetSubNodeValue(data_element, "maxhp", maxhp) && maxhp < 0)
	{
		return -101;
	}
	if (GetSubNodeValue(data_element, "gongji", gongji) && gongji < 0)
	{
		return -102;
	}
	if (GetSubNodeValue(data_element, "fangyu", fangyu) && fangyu < 0)
	{
		return -103;
	}
	if (GetSubNodeValue(data_element, "mingzhong", mingzhong) && mingzhong < 0)
	{
		return -104;
	}
	if (GetSubNodeValue(data_element, "shanbi", shanbi) && shanbi < 0)
	{
		return -105;
	}
	if (GetSubNodeValue(data_element, "baoji", baoji) && baoji < 0)
	{
		return -106;
	}
	if (GetSubNodeValue(data_element, "jianren", jianren) && jianren < 0)
	{
		return -107;
	}

	return 0;
}

int AttrConfig::ReadConfig(pugi::xml_node &data_element)
{
	if (PugiGetSubNodeValue(data_element, "maxhp", maxhp) && maxhp < 0)
	{
		return -101;
	}
	if (PugiGetSubNodeValue(data_element, "gongji", gongji) && gongji < 0)
	{
		return -102;
	}
	if (PugiGetSubNodeValue(data_element, "fangyu", fangyu) && fangyu < 0)
	{
		return -103;
	}
	if (PugiGetSubNodeValue(data_element, "mingzhong", mingzhong) && mingzhong < 0)
	{
		return -104;
	}
	if (PugiGetSubNodeValue(data_element, "shanbi", shanbi) && shanbi < 0)
	{
		return -105;
	}
	if (PugiGetSubNodeValue(data_element, "baoji", baoji) && baoji < 0)
	{
		return -106;
	}
	if (PugiGetSubNodeValue(data_element, "jianren", jianren) && jianren < 0)
	{
		return -107;
	}

	return 0;
}

bool NumListConfig::ReadConfig(pugi::xml_node data_element, const pugi::char_t* node)
{
	std::string tmp_str;
	if (!PugiGetSubNodeValue(data_element, node, tmp_str))
	{
		error_desc = "no " + std::string(node);
		return false;
	}

	char buffer[1024] = { 0 };
	if (SNPRINTF(buffer, sizeof(buffer), "%s", tmp_str.c_str()) < 0)
	{
		error_desc = "no " + std::string(node);
		return false;
	}

	std::vector<std::string> result;
	int count = ParseParam(buffer, result, "|");

	for (int i = 0; i < count; ++i)
	{
		int num = 0;
		if (!StringToBaseType(result[i].c_str(), num))
		{
			error_desc = "read " + std::string(node) + " fail";
			return false;
		}

		num_list.emplace_back(num);
	}

	return true;
}

bool NumListConfig::Check(int policy, int param1, int param2)
{
	if (CHECK_POLICY_NONE == policy) return true;

	if (CHECK_POLICY_UP_ORDER == policy)
	{
		if ((int)num_list.size() > 1)
		{
			for (int i = 1; i < (int)num_list.size(); ++i)
			{
				if (num_list[i] < num_list[i - 1])
				{
					error_desc = std::to_string(num_list[i]) + " can not <= " + std::to_string(num_list[i - 1]);
					return false;
				}
			}
		}
		return true;
	}

	for (std::vector<int>::iterator it = num_list.begin(); it != num_list.end(); ++it)
	{
		const int &value = *it;
		switch (policy)
		{
		case CHECK_POLICY_GRANTER_0:
		{
			if (value <= 0)
			{
				error_desc = std::to_string(value) + " can not <= 0";
				return false;
			}
		}
		break;

		case CHECK_POLICY_GRANTER_EQUAL_0:
		{
			if (value < 0)
			{
				error_desc = std::to_string(value) + " can not < 0";
				return false;
			}
		}
		break;

		case CHECK_POLICY_RANGE:
		{
			// 合法区间：[r, l)
			int r = param1;
			int l = param2;

			if (value >= l || value < r)
			{
				error_desc = std::to_string(value) + " must >= " + std::to_string(r) + " and < " + std::to_string(l);
				return false;
			}
		}
		break;

		/*case CHECK_POLICY_MONSTER:
		{
			if (NULL == MONSTERPOOL->GetMonsterParam(value))
			{
				error_desc = ToString(value) + " is not valid monster_id";
				return false;
			}
		}
		break;*/
		}
	}

	return true;
}

int NumListConfig::ToCArray(int *arr, int len) const
{
	// vector拷贝给数组
	int count = 0;
	for (std::vector<int>::const_iterator it = num_list.begin(); it != num_list.end(); ++it)
	{
		if (count >= len) break;

		arr[count++] = *it;
	}

	return count;
}

bool PointConfig::ReadConfig(TiXmlElement * data_element, const char *node)
{
	std::string tmp_str;
	if (!GetSubNodeValue(data_element, node, tmp_str))
	{
		return false;
	}

	char buffer[1024] = { 0 };
	if (SNPRINTF(buffer, sizeof(buffer), "%s", tmp_str.c_str()) < 0)
	{
		return false;
	}

	if (!ParsePos(buffer, pos))
	{
		return false;
	}

	return true;
}

bool PointConfig::ReadConfig(pugi::xml_node data_element, const pugi::char_t* name)
{
	std::string tmp_str;

	if (!PugiGetSubNodeValue(data_element, name, tmp_str))
	{
		return false;
	}

	char buffer[1024] = { 0 };
	if (SNPRINTF(buffer, sizeof(buffer), "%s", tmp_str.c_str()) < 0)
	{
		return false;
	}

	if (!ParsePos(buffer, pos))
	{
		return false;
	}

	return true;
}

bool PointListConfig::ReadConfig(TiXmlElement * data_element, const char *node)
{
	std::string tmp_str;
	if (!GetSubNodeValue(data_element, node, tmp_str))
	{
		return false;
	}

	char buffer[1024] = { 0 };
	if (SNPRINTF(buffer, sizeof(buffer), "%s", tmp_str.c_str()) < 0)
	{
		return false;
	}

	std::vector<std::string> result;
	int count = ParseParam(buffer, result, "|");

	for (int i = 0; i < count; ++i)
	{
		Posi pos;
		if (!PointConfig::ParsePos(result[i].c_str(), pos))
		{
			return false;
		}

		point_list.emplace_back(pos);
	}

	return true;
}

bool PointListConfig::ReadConfig(pugi::xml_node data_element, const pugi::char_t* name)
{
	std::string tmp_str;

	if (!PugiGetSubNodeValue(data_element, name, tmp_str))
	{
		return false;
	}

	char buffer[1024] = { 0 };
	if (SNPRINTF(buffer, sizeof(buffer), "%s", tmp_str.c_str()) < 0)
	{
		return false;
	}

	std::vector<std::string> result;
	int count = ParseParam(buffer, result, "|");

	for (int i = 0; i < count; ++i)
	{
		Posi pos;
		if (!PointConfig::ParsePos(result[i].c_str(), pos))
		{
			return false;
		}

		point_list.emplace_back(pos);
	}

	return true;
}