#include "shenbingtuluconfig.h"
#include "servercommon/configcommon.h"
#include "servercommon/struct/orangeequipcollectdef.hpp"

ShenBingTuLuConfig::ShenBingTuLuConfig()
{
}

ShenBingTuLuConfig::~ShenBingTuLuConfig()
{

}

bool ShenBingTuLuConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = { 0 };
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ShenBingTuLuConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 红装收集
		PugiXmlNode root_element = RootElement.child("red_equip_collect");
		if (root_element.empty())
		{
			*err = path + ": no [red_equip_collect].";
			return false;
		}

		iRet = this->InitRedEquipCollectCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRedEquipCollectCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 红装收集属性配置
		PugiXmlNode root_element = RootElement.child("red_equip_collect_attr");
		if (root_element.empty())
		{
			*err = path + ": no [red_equip_collect_attr].";
			return false;
		}

		iRet = this->InitRedEquipCollectAttrCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRedEquipCollectAttrCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 红装收集其他
		PugiXmlNode root_element = RootElement.child("red_equip_collect_other");
		if (root_element.empty())
		{
			*err = path + ": no [red_equip_collect_other].";
			return false;
		}

		iRet = this->InitRedEquipCollectOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRedEquipCollectOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 红装收集活动
		PugiXmlNode root_element = RootElement.child("red_equip_collect_act");
		if (root_element.empty())
		{
			*err = path + ": no [red_equip_collect_act].";
			return false;
		}

		iRet = this->InitRedEquipCollectActCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRedEquipCollectActCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	//--------------------------------------------------
	{
		// 橙装收集
		PugiXmlNode root_element = RootElement.child("orange_equip_collect");
		if (root_element.empty())
		{
			*err = path + ": no [orange_equip_collect].";
			return false;
		}

		iRet = this->InitOrangeEquipCollectCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOrangeEquipCollectCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 橙装收集属性配置
		PugiXmlNode root_element = RootElement.child("orange_equip_collect_attr");
		if (root_element.empty())
		{
			*err = path + ": no [orange_equip_collect_attr].";
			return false;
		}

		iRet = this->InitOrangeEquipCollectAttrCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOrangeEquipCollectAttrCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 橙装收集其他
		PugiXmlNode root_element = RootElement.child("orange_equip_collect_other");
		if (root_element.empty())
		{
			*err = path + ": no [orange_equip_collect_other].";
			return false;
		}

		iRet = this->InitOrangeEquipCollectOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOrangeEquipCollectOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 橙装收集活动
		PugiXmlNode root_element = RootElement.child("orange_equip_collect_act");
		if (root_element.empty())
		{
			*err = path + ": no [orange_equip_collect_act].";
			return false;
		}

		iRet = this->InitOrangeEquipCollectActCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOrangeEquipCollectActCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const RedEquipCollectAttrConfig::CfgItem * ShenBingTuLuConfig::GetRedEquipCollectAttrCfg(int seq, int cnt)
{
	if (!IsValidRedEquipCollectAttrConfigSeqCount(seq, cnt))
	{
		return nullptr;
	}

	return &m_red_equip_collect_attr_cfg.cfg_item_list[seq * RED_EQUIP_COLLECT_SLOT_COUNT + cnt];
}

const RedEquipCollectConfig::CfgItem * ShenBingTuLuConfig::GetRedEquipCollectItemCfg(int seq, int prof)
{
	if (!IsValidRedEquipCollectConfigSeqProf(seq, prof))
	{
		return nullptr;
	}

	return &m_red_equip_collect_cfg.cfg_item_list[seq * PROF_TYPE_PROF_NO_LIMIT + prof];
}

const RedEquipCollectOtherConfig::CfgItem * ShenBingTuLuConfig::GetRedEquipCollectOtherCfg(int seq, int pro)
{
	if (!IsValidRedEquipCollectSeq(seq) || !IsValidRedEquipCollectPro(pro) || !m_red_equip_collect_other_cfg.cfg_item_list[seq][pro].has_read)
	{
		return nullptr;
	}

	return &m_red_equip_collect_other_cfg.cfg_item_list[seq][pro];
}

const RedEquipCollectActConfig::CfgItem * ShenBingTuLuConfig::GetRedEquipCollectActCfg(int seq)
{
	if (!IsValidRedEquipActRewardSeq(seq))
	{
		return nullptr;
	}

	const auto &cfg_item = m_red_equip_collect_act_cfg.cfg_item_list[seq];
	if (!cfg_item.has_read)
	{
		return nullptr;
	}

	return &cfg_item;
}

//---------------------------------------------------------------------------------------------------
const OrangeEquipCollectAttrConfig::CfgItem * ShenBingTuLuConfig::GetOrangeEquipCollectAttrCfg(int seq, int cnt)
{
	if (!IsValidOrangeEquipCollectAttrConfigSeqCount(seq, cnt))
	{
		return nullptr;
	}

	return &m_orange_equip_collect_attr_cfg.cfg_item_list[seq * ORANGE_EQUIP_COLLECT_SLOT_COUNT + cnt];
}

const OrangeEquipCollectConfig::CfgItem * ShenBingTuLuConfig::GetOrangeEquipCollectItemCfg(int seq, int prof)
{
	if (!IsValidOrangeEquipCollectConfigSeqProf(seq, prof))
	{
		return nullptr;
	}

	return &m_orange_equip_collect_cfg.cfg_item_list[seq * PROF_TYPE_PROF_NO_LIMIT + prof];
}

const OrangeEquipCollectOtherConfig::CfgItem * ShenBingTuLuConfig::GetOrangeEquipCollectOtherCfg(int seq)
{
	if (!IsValidOrangeEquipCollectSeq(seq) || !m_orange_equip_collect_other_cfg.cfg_item_list[seq].has_read)
	{
		return nullptr;
	}

	return &m_orange_equip_collect_other_cfg.cfg_item_list[seq];
}

const OrangeEquipCollectActConfig::CfgItem * ShenBingTuLuConfig::GetOrangeEquipCollectActCfg(int seq)
{
	if (!IsValidOrangeEquipActRewardSeq(seq))
	{
		return nullptr;
	}

	const auto &cfg_item = m_orange_equip_collect_act_cfg.cfg_item_list[seq];
	if (!cfg_item.has_read)
	{
		return nullptr;
	}

	return &cfg_item;
}

int ShenBingTuLuConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "red_equip_collect_active_puton_count", m_other_cfg.red_equip_collect_active_puton_count) || m_other_cfg.red_equip_collect_active_puton_count <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "orange_equip_collect_active_puton_count", m_other_cfg.orange_equip_collect_active_puton_count) || m_other_cfg.orange_equip_collect_active_puton_count <= 0)
	{
		return -2;
	}

	return 0;
}


int ShenBingTuLuConfig::InitRedEquipCollectCfg(PugiXmlNode root_element)
{
	auto data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0, prof = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !PugiGetSubNodeValue(data_element, "prof", prof)) return -1000;

		if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_PROF_NO_LIMIT)
		{
			return -1001;
		}

		if (!this->IsValidRedEquipCollectConfigSeqProf(seq, prof, true)) return -1002;

		int ret = m_red_equip_collect_cfg.cfg_item_list[seq * PROF_TYPE_PROF_NO_LIMIT + prof].ReadConfig(data_element, seq, prof);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenBingTuLuConfig::InitRedEquipCollectAttrCfg(PugiXmlNode root_element)
{
	auto data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0, collect_count = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !PugiGetSubNodeValue(data_element, "collect_count", collect_count)) return -1000;

		if (!this->IsValidRedEquipCollectAttrConfigSeqCount(seq, collect_count, true)) return -1001;

		int ret = m_red_equip_collect_attr_cfg.cfg_item_list[seq * RED_EQUIP_COLLECT_SLOT_COUNT + collect_count].ReadConfig(data_element, seq, collect_count);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenBingTuLuConfig::InitRedEquipCollectOtherCfg(PugiXmlNode root_element)
{
	auto data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !IsValidRedEquipCollectSeq(seq)) return -1000;
		int pro = 0;
		if (!PugiGetSubNodeValue(data_element, "prof", pro) || !IsValidRedEquipCollectPro(pro)) return -2000;

		int ret = m_red_equip_collect_other_cfg.cfg_item_list[seq][pro].ReadConfig(data_element, seq, pro);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenBingTuLuConfig::InitRedEquipCollectActCfg(PugiXmlNode root_element)
{
	auto data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !IsValidRedEquipActRewardSeq(seq)) return -1000;

		int ret = m_red_equip_collect_act_cfg.cfg_item_list[seq].ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

//-------------------------------------------------------------------------------------------

inline bool ShenBingTuLuConfig::IsValidRedEquipCollectAttrConfigSeqCount(int seq, int cnt, bool is_init)
{
	if (seq < 0 || seq >= RED_EQUIP_COLLECT_SEQ_MAX_COUNT) return false;
	if (cnt <= 0 || cnt >= RED_EQUIP_COLLECT_SLOT_COUNT) return false;
	if (!is_init && !m_red_equip_collect_attr_cfg.cfg_item_list[seq * RED_EQUIP_COLLECT_SLOT_COUNT + cnt].has_read) return false;
	return true;
}

inline bool ShenBingTuLuConfig::IsValidRedEquipCollectConfigSeqProf(int seq, int prof, bool is_init)
{
	if (seq < 0 || seq >= RED_EQUIP_COLLECT_SEQ_MAX_COUNT) return false;
	if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_PROF_NO_LIMIT) return false;
	if (!is_init && !m_red_equip_collect_cfg.cfg_item_list[seq * PROF_TYPE_PROF_NO_LIMIT + prof].has_read) return false;
	return true;
}

//------------------------------------------------------------------------------------------------
int ShenBingTuLuConfig::InitOrangeEquipCollectCfg(PugiXmlNode root_element)
{
	auto data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0, prof = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !PugiGetSubNodeValue(data_element, "prof", prof)) return -1000;

		if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_PROF_NO_LIMIT)
		{
			return -1001;
		}

		if (!this->IsValidOrangeEquipCollectConfigSeqProf(seq, prof, true)) return -1002;

		int ret = m_orange_equip_collect_cfg.cfg_item_list[seq * PROF_TYPE_PROF_NO_LIMIT + prof].ReadConfig(data_element, seq, prof);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenBingTuLuConfig::InitOrangeEquipCollectAttrCfg(PugiXmlNode root_element)
{
	auto data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0, collect_count = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !PugiGetSubNodeValue(data_element, "collect_count", collect_count)) return -1000;

		if (!this->IsValidOrangeEquipCollectAttrConfigSeqCount(seq, collect_count, true)) return -1001;

		int ret = m_orange_equip_collect_attr_cfg.cfg_item_list[seq * ORANGE_EQUIP_COLLECT_SLOT_COUNT + collect_count].ReadConfig(data_element, seq, collect_count);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenBingTuLuConfig::InitOrangeEquipCollectOtherCfg(PugiXmlNode root_element)
{
	auto data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !IsValidOrangeEquipCollectSeq(seq)) return -1000;

		int ret = m_orange_equip_collect_other_cfg.cfg_item_list[seq].ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShenBingTuLuConfig::InitOrangeEquipCollectActCfg(PugiXmlNode root_element)
{
	auto data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !IsValidOrangeEquipActRewardSeq(seq)) return -1000;

		int ret = m_orange_equip_collect_act_cfg.cfg_item_list[seq].ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

inline bool ShenBingTuLuConfig::IsValidOrangeEquipCollectAttrConfigSeqCount(int seq, int cnt, bool is_init)
{
	if (seq < 0 || seq >= ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT) return false;
	if (cnt <= 0 || cnt >= ORANGE_EQUIP_COLLECT_SLOT_COUNT) return false;
	if (!is_init && !m_orange_equip_collect_attr_cfg.cfg_item_list[seq * ORANGE_EQUIP_COLLECT_SLOT_COUNT + cnt].has_read) return false;
	return true;
}

inline bool ShenBingTuLuConfig::IsValidOrangeEquipCollectConfigSeqProf(int seq, int prof, bool is_init)
{
	if (seq < 0 || seq >= ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT) return false;
	if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_PROF_NO_LIMIT) return false;
	if (!is_init && !m_orange_equip_collect_cfg.cfg_item_list[seq * PROF_TYPE_PROF_NO_LIMIT + prof].has_read) return false;
	return true;
}