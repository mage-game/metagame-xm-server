
#include "servercommon/servercommon.h"
#include "shenqiconfig.hpp"
#include "gameworld/gameworld/item/itempool.h"

static int ConvertParamInt(short param1, short param2)
{
	union ParamToInt
	{
		struct
		{
			short m_param1;
			short m_param2;
		};

		int val;
	};

	ParamToInt param;

	param.m_param1 = param1;
	param.m_param2 = param2;

	return param.val;
}

void ShenqiTeXiaoExtraAttributes::AddAttrs(CharIntAttrs &attrs, double factor) const
{
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += (Attribute)(pojia_per * factor);
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER] += (Attribute)(zhufu_per * factor);
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] += (Attribute)(jianshang_per * factor);
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER] += (Attribute)(shanbi_per * factor);

// 	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += (Attribute)(per_pofang * factor);
// 	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += (Attribute)(per_mianshang * factor);

	// 下面特殊属性用乘法
	attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, static_cast<Attribute>(per_mianshang * factor));
	attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, static_cast<Attribute>(per_pofang * factor));

	attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(per_gongji * factor*ROLE_ATTR_PER);
	attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(per_fangyu * factor*ROLE_ATTR_PER);
	attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(per_maxhp * factor*ROLE_ATTR_PER);
}

int ShenqiTeXiaoExtraAttributes::ReadConfig(PugiXmlNode & data_element)
{
	if (!PugiGetSubNodeValue(data_element, "pojia_per", pojia_per) || pojia_per < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "zhufu_per", zhufu_per) || zhufu_per < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "jianshang_per",jianshang_per) || jianshang_per < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "shangbi_per", shanbi_per) || shanbi_per < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element,"per_fangyu", per_fangyu) || per_fangyu < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "per_gongji", per_gongji) || per_gongji < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "per_maxhp", per_maxhp) || per_maxhp < 0)
	{
		return -7;
	}
	if (!PugiGetSubNodeValue(data_element, "per_pofang", per_pofang) || per_pofang < 0)
	{
		return -8;
	}
	if (!PugiGetSubNodeValue(data_element, "per_mianshang", per_mianshang) || per_mianshang < 0)
	{
		return -9;
	}

	return 0;
}

ShenqiConfig::ShenqiConfig() : m_shenbing_uplevel_cfg_count(0), m_baojia_uplevel_cfg_count(0), m_shenbing_active_texiao_cfg_count(0), m_baojia_active_texiao_cfg_count(0),
	m_uplevel_addexp_cfg_count(0), m_uplevel_addexp_total_weight(0)
{
	
}

bool ShenqiConfig::Init(const std::string& config_path, std::string* err)
{
	int iRet = 0;
	char error_info[512] = { 0 };
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, config_path, "ShenqiConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = config_path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other");
		if (nullptr == other_element)
		{
			*err = config_path + " xml no [other] node.";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if(iRet)
		{
			sprintf(error_info, "%s:InitOtherCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode decompose_element = RootElement.child("decompose");
		if (nullptr == decompose_element)
		{
			*err = config_path + " xml no [decompose] node.";
			return false;
		}

		iRet = this->InitDecomposeCfg(decompose_element);
		if (iRet)
		{
			sprintf(error_info, "%s:InitDecomposeCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode shenbing_inlay_element = RootElement.child("shenbing_inlay");
		if (nullptr == shenbing_inlay_element)
		{
			*err = config_path + " xml no [shenbing_inlay] node.";
			return false;
		}

		iRet = this->InitShenbingInlayCfg(shenbing_inlay_element);
		if (iRet)
		{
			sprintf(error_info, "%s:InitShenbingInlayCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode shenbing_image_element = RootElement.child("shenbing_image");
		if (nullptr == shenbing_image_element)
		{
			*err = config_path + " xml no [shenbing_image] node.";
			return false;
		}

		iRet = this->InitShenbingImageCfg(shenbing_image_element);
		if (iRet)
		{
			sprintf(error_info, "%s:InitShenbingImageCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode shenbing_uplevel_element = RootElement.child("shenbing_uplevel");
		if (nullptr == shenbing_uplevel_element)
		{
			*err = config_path + " xml no [shenbing_uplevel] node.";
			return false;
		}

		iRet = this->InitShenbingUplevelCfg(shenbing_uplevel_element);
		if (iRet)
		{
			sprintf(error_info, "%s:InitShenbingUplevelCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode baojia_inlay_element = RootElement.child("baojia_inlay");
		if (nullptr == baojia_inlay_element)
		{
			*err = config_path + " xml no [baojia_inlay] node.";
			return false;
		}

		iRet = this->InitBaojiaInlayCfg(baojia_inlay_element);
		if (iRet)
		{
			sprintf(error_info, "%s:InitBaojiaInlayCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode baojia_image_element = RootElement.child("baojia_image");
		if (nullptr == baojia_image_element)
		{
			*err = config_path + " xml no [baojia_image] node.";
			return false;
		}

		iRet = this->InitBaojiaImageCfg(baojia_image_element);
		if (iRet)
		{
			sprintf(error_info, "%s:InitBaojiaImageCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode baojia_uplevel = RootElement.child("baojia_uplevel");
		if (nullptr == baojia_uplevel)
		{
			*err = config_path + " xml no [baojia_uplevel] node.";
			return false;
		}

		iRet = this->InitBaojiaUplevelCfg(baojia_uplevel);
		if (iRet)
		{
			sprintf(error_info, "%s:InitBaojiaUplevelCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("uplevel_addexp");
		if (nullptr == root_element)
		{
			*err = config_path + " xml no [uplevel_addexp] node.";
			return false;
		}

		iRet = this->InitUplevelAddexpCfg(root_element);
		if (iRet)
		{
			sprintf(error_info, "%s:InitUplevelAddexpCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("shenbing_texiao");
		if (nullptr == root_element)
		{
			*err = config_path + " xml no [shenbing_texiao] node.";
			return false;
		}

		iRet = this->InitShenbingActiveTeXiaoCfg(root_element);
		if (iRet)
		{
			sprintf(error_info, "%s:InitShenbingActiveTeXiaoCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("baojia_texiao");
		if (nullptr == root_element)
		{
			*err = config_path + " xml no [baojia_texiao] node.";
			return false;
		}

		iRet = this->InitBaojiagActiveTeXiaoCfg(root_element);
		if (iRet)
		{
			sprintf(error_info, "%s:InitBaojiagActiveTeXiaoCfg failed [%d]", config_path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}


	return true;
}

const ShenqiDecomposeCfg* ShenqiConfig::GetDecomposeCfg(const int item_id) const
{
	if (m_decompose_cfg_map.find(item_id) == m_decompose_cfg_map.end()) return nullptr;

	auto it = m_decompose_cfg_map.find(item_id);
	return &it->second;
}

const ShenqiInlayCfg* ShenqiConfig::GetShenBingInlayCfg(const short id, const short part_type) const
{
	int key = ConvertParamInt(id, part_type);
	if (m_shenbing_inlay_cfg_map.find(key) == m_shenbing_inlay_cfg_map.end()) return nullptr;

	auto it = m_shenbing_inlay_cfg_map.find(key);
	return &it->second;
}

const ShenqiImageCfg* ShenqiConfig::GetShenbingImageCfg(const short id) const
{
	if (id <= 0 || id >= SHENQI_MAX_ID)
	{
		return nullptr;
	}

	if (m_shenbing_image_cfg_list[id].id != id)
	{
		return nullptr;
	}

	return &m_shenbing_image_cfg_list[id];
}

const ShenqiUplevelCfg* ShenqiConfig::GetShenBingUplevelCfg(const int level) const
{
	for (int i = 0; i < m_shenbing_uplevel_cfg_count && i < SHENQI_UPLEVEL_MAX_LEVEL; ++i)
	{
		if (m_shenbing_uplevel_cfg_list[i].level == level)
		{
			return &m_shenbing_uplevel_cfg_list[i];
		}
	}

	return nullptr;
}

const ShenqiInlayCfg* ShenqiConfig::GetBaojiaInlayCfg(const short id, const short part_type) const
{
	int key = ConvertParamInt(id, part_type);
	if (m_baojia_inlay_cfg_map.find(key) == m_baojia_inlay_cfg_map.end()) return nullptr;

	auto it = m_baojia_inlay_cfg_map.find(key);
	return &it->second;
}

const ShenqiImageCfg* ShenqiConfig::GetBaojiaImageCfg(const short id) const
{
	if (id <= 0 || id >= SHENQI_MAX_ID)
	{
		return nullptr;
	}

	if (m_baojia_image_cfg_list[id].id != id)
	{
		return nullptr;
	}

	return &m_baojia_image_cfg_list[id];
}

const ShenqiUplevelCfg* ShenqiConfig::GetBaojieUplevelCfg(const int level) const
{
	for (int i = 0; i < m_baojia_uplevel_cfg_count && i < SHENQI_UPLEVEL_MAX_LEVEL; ++i)
	{
		if (m_baojia_uplevel_cfg_list[i].level == level)
		{
			return &m_baojia_uplevel_cfg_list[i];
		}
	}

	return nullptr;
}

const int ShenqiConfig::GetUplevelRandAddexp() const
{
	int rand_val = RandomNum(m_uplevel_addexp_total_weight);

	for (int i = 0; i < m_uplevel_addexp_cfg_count && i < SHENQI_UPLEVEL_ADDEXP_CFG_NUM_MAX; ++i)
	{
		if (rand_val < m_uplevel_addexp_cfg_list[i].weight)
		{
			return m_uplevel_addexp_cfg_list[i].addexp;
		}

		rand_val -= m_uplevel_addexp_cfg_list[i].weight;
	}

	return 0;
}

const ShenqiActiveTeXiaoCfg * ShenqiConfig::GetShenbingActiveTeXiaoCfg(const short shenqi_id, int quality) const
{
	for (int i = 0; i < SHENQI_MAX_ID; ++i)
	{
		if (m_shenbing_active_texiao_list[i].shenqi_id == shenqi_id && m_shenbing_active_texiao_list[i].trigger_quality == quality)
		{
			return &m_shenbing_active_texiao_list[i];
		}
	}

	return nullptr;
}

const ShenqiActiveTeXiaoCfg * ShenqiConfig::GetBaojiaActiveTeXiaoCfg(const short shenqi_id, int quality ) const
{
	for (int i = 0; i < SHENQI_MAX_ID; ++i)
	{
		if (m_baojia_active_texiao_list[i].shenqi_id == shenqi_id && m_baojia_active_texiao_list[i].trigger_quality == quality)
		{
			return &m_baojia_active_texiao_list[i];
		}
	}

	return nullptr;
}

int ShenqiConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	if (!PugiGetSubNodeValue(data_element, "shenbing_uplevel_stuff", m_other_cfg.shenbing_uplevel_stuff_id) ||
		nullptr == ITEMPOOL->GetItem(m_other_cfg.shenbing_uplevel_stuff_id))
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "baojia_uplevel_stuff_id", m_other_cfg.baojia_uplevel_stuff_id) ||
		nullptr == ITEMPOOL->GetItem(m_other_cfg.baojia_uplevel_stuff_id))
	{
		return -3;
	}

	//if (!PugiGetSubNodeValue(data_element, "shenbing_active_texiao_stuff_id", m_other_cfg.shenbing_active_texiao_stuff_id) ||
	//	m_other_cfg.shenbing_active_texiao_stuff_id < 0)
	//{
	//	return -4;
	//}
	//if (!PugiGetSubNodeValue(data_element, "shenbing_active_texiao_stuff_count", m_other_cfg.shenbing_active_texiao_stuff_count) ||
	//	m_other_cfg.shenbing_active_texiao_stuff_count < 0)
	//{
	//	return -4;
	//}

	if (!PugiGetSubNodeValue(data_element, "shenbing_suit_trigger_quality", m_other_cfg.shenbing_suit_trigger_quality) || 
		m_other_cfg.shenbing_suit_trigger_quality <= SHENQI_QUALITY_TYPE_INVALID || 
		m_other_cfg.shenbing_suit_trigger_quality >= SHENQI_QUALITY_TYPE_MAX)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "shenbing_suit_trigger_per", m_other_cfg.shenbing_suit_trigger_per) || 
		m_other_cfg.shenbing_suit_trigger_per < 0 || m_other_cfg.shenbing_suit_trigger_per > 100)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "shenbing_suit_duration_time", m_other_cfg.shenbing_suit_duration_time) || 
		m_other_cfg.shenbing_suit_duration_time < 0)
	{
		return -7;
	}

	//if (!PugiGetSubNodeValue(data_element, "baojia_active_texiao_stuff_id", m_other_cfg.baojia_active_texiao_stuff_id) ||
	//	m_other_cfg.baojia_active_texiao_stuff_id < 0)
	//{
	//	return -8;
	//}
	//if (!PugiGetSubNodeValue(data_element, "baojia_active_texiao_stuff_count", m_other_cfg.baojia_active_texiao_stuff_count) ||
	//	m_other_cfg.baojia_active_texiao_stuff_count < 0)
	//{
	//	return -8;
	//}

	if (!PugiGetSubNodeValue(data_element, "baojia_suit_trigger_quality", m_other_cfg.baojia_suit_trigger_quality) ||
		m_other_cfg.baojia_suit_trigger_quality <= SHENQI_QUALITY_TYPE_INVALID ||
		m_other_cfg.baojia_suit_trigger_quality >= SHENQI_QUALITY_TYPE_MAX)
	{
		return -9;
	}

	return 0;
}

int ShenqiConfig::InitDecomposeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	while (!data_element.empty())
	{
		int stuff_id = 0;
		if (!PugiGetSubNodeValue(data_element, "stuff_id", stuff_id) || nullptr == ITEMPOOL->GetItem(stuff_id) ||
			m_decompose_cfg_map.find(stuff_id) != m_decompose_cfg_map.end())
		{
			return -1;
		}

		ShenqiDecomposeCfg& cfg = m_decompose_cfg_map[stuff_id];
		cfg.item_id = stuff_id;

		cfg.get_item_count = ItemConfigData::ReadConfigList(data_element, "get_item", cfg.get_item_list, MAX_ATTACHMENT_ITEM_NUM);

		if (cfg.get_item_count <= 0)
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenqiConfig::InitShenbingInlayCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	int last_key = 0;
	int last_quality = 0;

	while (!data_element.empty())
	{
		short id = -1;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id < 0 || id >= SHENQI_SUIT_NUM_MAX)
		{
			return -1;
		}

		short part_type = 0;
		if (!PugiGetSubNodeValue(data_element, "part_type", part_type) || part_type < SHENQI_PART_TYPE_0 || part_type >= SHENQI_PART_TYPE_MAX)
		{
			return -2;
		}

		int key = ConvertParamInt(id, part_type);
		if (key != last_key)
		{
			last_key = key;
			last_quality = 0;
		}

		ShenqiInlayCfg& cfg = m_shenbing_inlay_cfg_map[key];
		cfg.id = id;
		cfg.part_type = part_type;

		int quality = -1;
		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality != last_quality + 1 || quality <= SHENQI_QUALITY_TYPE_INVALID || quality >= SHENQI_QUALITY_TYPE_MAX)
		{
			return -5;
		}
		last_quality = quality;

		if (!PugiGetSubNodeValue(data_element, "inlay_stuff_id", cfg.inlay_stuff_list[quality]) || nullptr == ITEMPOOL->GetItem(cfg.inlay_stuff_list[quality]))
		{
			return -6;
		}

		int IRet = cfg.attibute_config_list[quality].ReadConfig(data_element);
		if (IRet < 0 )
		{
			return -7 + IRet;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenqiConfig::InitShenbingImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	int last_id = 0;

	while (!data_element.empty())
	{
		short id = 0;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id != last_id + 1 || id >= SHENQI_MAX_ID)
		{
			return -1;
		}
		last_id = id;

		ShenqiImageCfg &cfg = m_shenbing_image_cfg_list[id];
		cfg.id = id;

		if (!PugiGetSubNodeValue(data_element, "need_quality", cfg.need_quality) || cfg.need_quality <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "active_image_id", cfg.active_image_id) || cfg.active_image_id <= 0)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenqiConfig::InitShenbingUplevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	m_shenbing_uplevel_cfg_count = 0;
	int last_level = 0;

	while (!data_element.empty())
	{
		if (m_shenbing_uplevel_cfg_count >= SHENQI_UPLEVEL_MAX_LEVEL)
		{
			return -100;
		}

		ShenqiUplevelCfg& cfg = m_shenbing_uplevel_cfg_list[m_shenbing_uplevel_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level != last_level + 1)
		{
			return -1;
		}
		last_level = cfg.level;

		if (!PugiGetSubNodeValue(data_element, "need_exp", cfg.need_exp) || cfg.need_exp <= 0)
		{
			return -2;
		}

		//if (PugiGetSubNodeValue(data_element, "active_texiao_id", cfg.active_texiao_id))
		//{
		//	if (cfg.active_texiao_id <= 0 || cfg.active_texiao_id >= SHENQI_SUIT_NUM_MAX)
		//	{
		//		return -3;
		//	}
		//}
		int IRet = cfg.attibute_config.ReadConfig(data_element);
		if (IRet <0 )
		{
			return -4 + IRet;
		}

		++m_shenbing_uplevel_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenqiConfig::InitBaojiaInlayCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	int last_key = 0;
	int last_quality = 0;

	while (!data_element.empty())
	{
		short id = -1;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id <= 0 || id >= SHENQI_SUIT_NUM_MAX)
		{
			return -1;
		}

		short part_type = 0;
		if (!PugiGetSubNodeValue(data_element, "part_type", part_type) || part_type < SHENQI_PART_TYPE_0 || part_type >= SHENQI_PART_TYPE_MAX)
		{
			return -2;
		}

		int key = ConvertParamInt(id, part_type);
		if (key != last_key)
		{
			last_key = key;
			last_quality = 0;
		}

		ShenqiInlayCfg& cfg = m_baojia_inlay_cfg_map[key];
		cfg.id = id;
		cfg.part_type = part_type;

		int quality = -1;
		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality != last_quality + 1 || quality <= SHENQI_QUALITY_TYPE_INVALID || quality >= SHENQI_QUALITY_TYPE_MAX)
		{
			return -5;
		}
		last_quality = quality;

		if (!PugiGetSubNodeValue(data_element, "inlay_stuff_id", cfg.inlay_stuff_list[quality]) || nullptr == ITEMPOOL->GetItem(cfg.inlay_stuff_list[quality]))
		{
			return -6;
		}
		int IRet = cfg.attibute_config_list[quality].ReadConfig(data_element);
		if (IRet < 0)
		{
			return -7 - IRet;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenqiConfig::InitBaojiaImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	int last_id = 0;

	while (!data_element.empty())
	{
		short id = 0;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id != last_id + 1 || id >= SHENQI_MAX_ID)
		{
			return -1;
		}
		last_id = id;

		ShenqiImageCfg &cfg = m_baojia_image_cfg_list[id];
		cfg.id = id;

		if (!PugiGetSubNodeValue(data_element, "need_quality", cfg.need_quality) || cfg.need_quality <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "active_image_id", cfg.active_image_id) || cfg.active_image_id <= 0)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenqiConfig::InitBaojiaUplevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	m_baojia_uplevel_cfg_count = 0;
	int last_level = 0;

	while (!data_element.empty())
	{
		if (m_baojia_uplevel_cfg_count >= SHENQI_UPLEVEL_MAX_LEVEL)
		{
			return -100;
		}

		ShenqiUplevelCfg& cfg = m_baojia_uplevel_cfg_list[m_baojia_uplevel_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level != last_level + 1)
		{
			return -1;
		}
		last_level = cfg.level;

		if (!PugiGetSubNodeValue(data_element, "need_exp", cfg.need_exp) || cfg.need_exp <= 0)
		{
			return -2;
		}

		//if (PugiGetSubNodeValue(data_element, "active_texiao_id", cfg.active_texiao_id))
		//{
		//	if (cfg.active_texiao_id <= 0 || cfg.active_texiao_id >= SHENQI_SUIT_NUM_MAX)
		//	{
		//		return -3;
		//	}
		//}
		int IRet = cfg.attibute_config.ReadConfig(data_element);
		if (IRet < 0)
		{
			return -4 + IRet;
		}

		++m_baojia_uplevel_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenqiConfig::InitUplevelAddexpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	m_uplevel_addexp_total_weight = 0;
	m_uplevel_addexp_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_uplevel_addexp_cfg_count >= SHENQI_UPLEVEL_ADDEXP_CFG_NUM_MAX)
		{
			return -100;
		}

		ShenqiUplevelAddexpCfg& cfg = m_uplevel_addexp_cfg_list[m_uplevel_addexp_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "addexp", cfg.addexp) || cfg.addexp <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -2;
		}

		m_uplevel_addexp_total_weight += cfg.weight;

		++m_uplevel_addexp_cfg_count;
		data_element = data_element.next_sibling();
	}

	if (m_uplevel_addexp_total_weight <= 0) return -200;

	return 0;
}

int ShenqiConfig::InitShenbingActiveTeXiaoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	m_shenbing_active_texiao_cfg_count = 0;

	int last_seq = 0;
	while (!data_element.empty())
	{
		if (m_shenbing_active_texiao_cfg_count >= SHENQI_SUIT_NUM_MAX)
		{
			return -100;
		}

		ShenqiActiveTeXiaoCfg &cfg = m_shenbing_active_texiao_list[m_shenbing_active_texiao_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg.seq) || cfg.seq < 0 )
		{
			return -1;
		}
		if (cfg.seq != last_seq + 1 && last_seq != 0)
		{
			return -101;
		}
		last_seq = cfg.seq;

		if (!PugiGetSubNodeValue(data_element, "id", cfg.shenqi_id)|| cfg.shenqi_id <= 0 || cfg.shenqi_id >= SHENQI_SUIT_NUM_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "trigger_quality", cfg.trigger_quality) || cfg.trigger_quality <= 0 )
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "active_texiao_stuff_id", cfg.active_texiao_stuff_id) || cfg.active_texiao_stuff_id < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "active_texiao_stuff_count", cfg.active_texiao_stuff_count) || cfg.active_texiao_stuff_count <= 0)
		{
			return -5;
		}

		int IRet = cfg.attibute_config.ReadConfig(data_element);
		if (IRet < 0)
		{
			return -102 + IRet;
		}

		IRet = cfg.extra_attibute.ReadConfig(data_element);
		if (IRet < 0)
		{
			return -200 + IRet;
		}

		++m_shenbing_active_texiao_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenqiConfig::InitBaojiagActiveTeXiaoCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	m_baojia_active_texiao_cfg_count = 0;

	int last_seq = 0;
	while (!data_element.empty())
	{
		if (m_baojia_active_texiao_cfg_count >= SHENQI_UPLEVEL_MAX_LEVEL)
		{
			return -100;
		}

		ShenqiActiveTeXiaoCfg &cfg = m_baojia_active_texiao_list[m_baojia_active_texiao_cfg_count];
		if (!PugiGetSubNodeValue(data_element, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -1;
		}
		if (cfg.seq != last_seq + 1 && last_seq != 0)
		{
			return -101;
		}
		last_seq = cfg.seq;

		if (!PugiGetSubNodeValue(data_element, "id", cfg.shenqi_id) || cfg.shenqi_id <= 0 || cfg.shenqi_id >= SHENQI_SUIT_NUM_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "trigger_quality", cfg.trigger_quality) || cfg.trigger_quality <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "active_texiao_stuff_id", cfg.active_texiao_stuff_id) || cfg.active_texiao_stuff_id < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "active_texiao_stuff_count", cfg.active_texiao_stuff_count) || cfg.active_texiao_stuff_count <= 0)
		{
			return -5;
		}

		int IRet = cfg.attibute_config.ReadConfig(data_element);
		if (IRet < 0)
		{
			return -102 + IRet;
		}

		IRet = cfg.extra_attibute.ReadConfig(data_element);
		if (IRet < 0)
		{
			return -200 + IRet;
		}

		++m_baojia_active_texiao_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}
