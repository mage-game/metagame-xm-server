#include "dressingroomconfig.hpp"
#include "servercommon/configcommon.h"
#include "obj/character/role.h"
#include "other/cloak/cloakmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/halo/halomanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/jingling/jinglingmanager.h"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/multimount/multimount.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/multimount/multimount.hpp" 
#include "other/upgrade/upgrademanager.hpp"
#include "other/upgrade/upgrade.hpp"
#include "item/itempool.h"


#define INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(system_type, func) \
{ \
	m_func_list[system_type].check_special_img_func = [](Role *role, int img_id) -> bool \
	{ \
		if (nullptr == role) \
		{ \
			return false; \
		} \
		return func(img_id); \
	}; \
}

DressingRoomConfig::DressingRoomConfig()
{
	
}

DressingRoomConfig::~DressingRoomConfig()
{
	
}

bool DressingRoomConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	PUGI_XML_LOAD_CONFIG("suit_cfg", InitSuitIDCfg);
	PUGI_XML_LOAD_CONFIG("suit_attr", InitSuitAttrCfg);
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	PUGI_XML_LOAD_CONFIG("suit_open_time", InitSuitOpenTimeCfg);
	
	this->InitFuncHelper();

	return true;
}

const DressingRoomSuitCfg::SuitAttrCfg * DressingRoomConfig::GetSuitAttrCfg(int suit_index, int suit_count)
{
	if (suit_index < 0 || suit_index >= static_cast<int>(m_suit_cfg_vec.size()))
	{
		return nullptr;
	}

	const auto &vec = m_suit_cfg_vec[suit_index].attr_cfg_vec;
// 	for (auto iter = vec.rbegin(); iter != vec.rend(); ++iter)
// 	{
// 		if (suit_count >= iter->img_count)
// 		{
// 			return &(*iter);
// 		}
// 	}
	for (int i = vec.size() - 1; i >= 0; --i)
	{
		if (suit_count >= vec[i].img_count)
		{
			return &vec[i];
		}
	}

	return nullptr;
}

const std::vector<SuitIDCfg> * DressingRoomConfig::GetSuitIDCfgVec(int suit_index)
{
	if (suit_index < 0 || suit_index >= static_cast<int>(m_suit_cfg_vec.size()))
	{
		return nullptr;
	}

	return &m_suit_cfg_vec[suit_index].id_cfg_vec;
}

const SystemFuncHelper * DressingRoomConfig::GetFuncHelper(int type)
{
	if (type < 0 || type >= SPECIAL_IMG_TYPE_MAX)
	{
		return nullptr;
	}

	return &m_func_list[type];
}

const SuitIDCfg * DressingRoomConfig::GetSuitInfo(int system_type, int img_id)
{
	SuitIDCfg cfg;
	cfg.suit_system_type = system_type;
	cfg.img_id = img_id;

	auto iter = m_suit_id_cfg_set.find(cfg);
	if (iter != m_suit_id_cfg_set.end())
	{
		return &(*iter);
	}

	return nullptr;
}

const SuitIDCfg * DressingRoomConfig::GetSuitInfoByIndex(int suit_index, int sub_index)
{
	for (auto &iter : m_suit_id_cfg_set)
	{
		if (suit_index == iter.suit_index && sub_index == iter.sub_index)
		{
			return &(iter);
		}
	}
	return nullptr;
}

unsigned int DressingRoomConfig::GetSuitOpenTimeStamp(int suit_index)
{
	if (suit_index < 0 || suit_index >= static_cast<int>(m_suit_cfg_vec.size()))
	{
		return 0;
	}

	return m_suit_cfg_vec[suit_index].open_timestamp;
}


int DressingRoomConfig::InitSuitIDCfg(PugiXmlNode root_element)
{
	PugiXmlNode dataElement = root_element.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_suit_index = -1;
	while (!dataElement.empty())
	{
		int suit_index = -1;
		if (!PugiGetSubNodeValue(dataElement, "suit_index", suit_index) || suit_index < 0 || suit_index > static_cast<int>(m_suit_cfg_vec.size()))
		{
			return -1;
		}

		SuitIDCfg cfg;
		if (!PugiGetSubNodeValue(dataElement, "suit_system_type", cfg.suit_system_type) || cfg.suit_system_type < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "img_id", cfg.img_id) || cfg.img_id < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "sub_index", cfg.sub_index) || cfg.sub_index < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "suit_index", cfg.suit_index) || cfg.suit_index < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "img_item_id", cfg.img_item_id) || nullptr == ITEMPOOL->GetItem(cfg.img_item_id))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_exchange_ticket_num", cfg.need_exchange_ticket_num)) // 可以为-1
		{
			return -7;
		}

		if (!m_suit_id_cfg_set.insert(cfg).second)
		{
			return -8;
		}

		if (last_suit_index != suit_index)
		{
			DressingRoomSuitCfg new_suit_cfg;
			new_suit_cfg.suit_index = suit_index;
			new_suit_cfg.id_cfg_vec.push_back(cfg);
			m_suit_cfg_vec.push_back(new_suit_cfg);
			if (static_cast<int>(m_suit_cfg_vec.size()) != suit_index + 1)
			{
				return -9;
			}
		}
		else
		{
			auto &last_suit_cfg = m_suit_cfg_vec.back();
			if (last_suit_cfg.suit_index != suit_index)
			{
				return -10;
			}

			auto &last_suit_id_cfg = last_suit_cfg.id_cfg_vec.back();
			if (last_suit_id_cfg.sub_index + 1 != cfg.sub_index)
			{
				return -11;
			}

			last_suit_cfg.id_cfg_vec.push_back(cfg);
		}

		last_suit_index = suit_index;

		dataElement = dataElement.next_sibling();
	}

	//一个套装最多只能有两件下列物品(仙女，仙宠，飞宠，仙宝，灵童)
	int suit_type[5] = { SPECIAL_IMG_TYPE_XIANNV, SPECIAL_IMG_TYPE_JINGLING,SPECIAL_IMG_TYPE_FLYPET,SPECIAL_IMG_TYPE_XIANBAO,SPECIAL_IMG_TYPE_LINGTONG};
	for (int i = 0; i < static_cast<int>(m_suit_cfg_vec.size()); ++i)
	{
		int count = 0;
		for (int j = 0; j < static_cast<int>(m_suit_cfg_vec[i].id_cfg_vec.size()); ++j)
		{
			for (int k = 0; k < 5; ++k)
			{
				if (m_suit_cfg_vec[i].id_cfg_vec[j].suit_system_type == suit_type[k])
				{
					count++;
				}
			}
		}
		if (count > 2)
		{
			return -100;
		}
	}
	return 0;
}

int DressingRoomConfig::InitSuitAttrCfg(PugiXmlNode root_element)
{
	PugiXmlNode dataElement = root_element.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_suit = 0; int  min_count = 0;
	while (!dataElement.empty())
	{
		int suit_index = -1;
		if (!PugiGetSubNodeValue(dataElement, "suit_index", suit_index) || suit_index < 0)
		{
			return -1;
		}

		if (suit_index < 0 || suit_index >= static_cast<int>(m_suit_cfg_vec.size()))
		{
			return -2;
		}

		DressingRoomSuitCfg &cfg = m_suit_cfg_vec[suit_index]; 
		DressingRoomSuitCfg::SuitAttrCfg attr_cfg;

		if (!PugiGetSubNodeValue(dataElement, "img_count_min", attr_cfg.img_count) || attr_cfg.img_count <= 0)
		{
			return -3;
		}

		if (last_suit == suit_index && attr_cfg.img_count < min_count)
		{
			return -100;
		}
		last_suit = suit_index;
		min_count = attr_cfg.img_count;

		if (!cfg.attr_cfg_vec.empty() && cfg.attr_cfg_vec.back().img_count >= attr_cfg.img_count)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "hp", attr_cfg.hp) || attr_cfg.hp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", attr_cfg.gongji) || attr_cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", attr_cfg.fangyu) || attr_cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", attr_cfg.mingzhong) || attr_cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", attr_cfg.shanbi) || attr_cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", attr_cfg.baoji) || attr_cfg.baoji < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", attr_cfg.jianren) || attr_cfg.jianren < 0)
		{
			return -11;
		}

		if (PugiGetSubNodeValue(dataElement, "hp_per", attr_cfg.hp_per) && attr_cfg.hp_per < 0)
		{
			return -12;
		}

		if (PugiGetSubNodeValue(dataElement, "gongji_per", attr_cfg.gongji_per) && attr_cfg.gongji_per < 0)
		{
			return -13;
		}

		if (PugiGetSubNodeValue(dataElement, "fangyu_per", attr_cfg.fangyu_per) && attr_cfg.fangyu_per < 0)
		{
			return -14;
		}

		if (PugiGetSubNodeValue(dataElement, "mingzhong_per", attr_cfg.mingzhong_per) && attr_cfg.mingzhong_per < 0)
		{
			return -15;
		}

		if (PugiGetSubNodeValue(dataElement, "shanbi_per", attr_cfg.shanbi_per) && attr_cfg.shanbi_per < 0)
		{
			return -16;
		}

		if (PugiGetSubNodeValue(dataElement, "baoji_per", attr_cfg.baoji_per) && attr_cfg.baoji_per < 0)
		{
			return -17;
		}

		if (PugiGetSubNodeValue(dataElement, "jianren_per", attr_cfg.jianren_per) && attr_cfg.jianren_per < 0)
		{
			return -18;
		}

		cfg.attr_cfg_vec.push_back(attr_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int DressingRoomConfig::InitOtherCfg(PugiXmlNode root_element)
{
	PugiXmlNode dataElement = root_element.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	{
		if (!PugiGetSubNodeValue(dataElement, "exchange_ticket_item_id", m_other_cfg.exchange_ticket_item_id) || nullptr == ITEMPOOL->GetItem(m_other_cfg.exchange_ticket_item_id))
		{
			return -1;
		}
	}

	return 0;
}

int DressingRoomConfig::InitSuitOpenTimeCfg(PugiXmlNode root_element)
{
	PugiXmlNode dataElement = root_element.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int suit_index = -1;
		if (!PugiGetSubNodeValue(dataElement, "suit_index", suit_index) || suit_index < 0)
		{
			return -1;
		}

		if (suit_index < 0 || suit_index >= static_cast<int>(m_suit_cfg_vec.size()))
		{
			return -2;
		}
		DressingRoomSuitCfg &cfg = m_suit_cfg_vec[suit_index];

		PugiXmlNode OpenTimeElement = dataElement.child("open_date");
		if (OpenTimeElement.empty())
		{
			return -4;
		}

		time_t open_time = 0;
		if (!ConvertTimeStringToUnixTime(OpenTimeElement.first_child().value(), &open_time))
		{
			return -5;
		}
		cfg.open_timestamp = static_cast<unsigned int>(open_time);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

void DressingRoomConfig::InitFuncHelper()
{

	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_MOUNT, role->GetMountManager()->GetMountParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_WING, role->GetWing()->GetWingParam()->IsSpecialImageActive);

	// 时装-部位0
	{
		m_func_list[SPECIAL_IMG_TYPE_SHIZHUANG_PART_0].check_special_img_func = [](Role *role, int img_id) -> bool
		{
			if (nullptr == role)
			{
				return false;
			}

			return role->GetShizhuang()->IsActive(SHIZHUANG_TYPE_WUQI, img_id);
		};
	}

	// 时装-部位1
	{
		m_func_list[SPECIAL_IMG_TYPE_SHIZHUANG_PART_1].check_special_img_func = [](Role *role, int img_id) -> bool
		{
			if (nullptr == role)
			{
				return false;
			}

			return role->GetShizhuang()->IsActive(SHIZHUANG_TYPE_BODY, img_id);
		};
	}
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_FABAO, role->GetFabaoManager()->GetParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_HALO, role->GetHalo()->GetHaloParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_FIGHT_MOUNT, role->GetFightMountManager()->GetFightMountParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_FOOTPRINT, role->GetFootPrint()->GetFootprintParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_XIANNV, role->GetXiannvManager()->GetXiannvParam()->IsHuanHuaActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_SHENGONG, role->GetShengong()->GetShengongParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_SHENYI, role->GetShenyi()->GetShenyiParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_JINGLING, role->GetJingLingMgr()->IsActivedPhantomImg);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_MASK, role->GetMaskManager()->GetMaskParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_QILINBI, role->GetQiLinBiManager()->GetQiLinBiParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_TOUSHI, role->GetTouShiManager()->GetTouShiParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_YAOSHI, role->GetYaoShiManager()->GetYaoShiParam()->IsSpecialImageActive);

	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_MULIT_MOUNT, role->GetMultiMount()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_LINGGONG, role->GetUpgradeManager()->GetUpgrade(UPGRADE_TYPE_3)->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_LINGQI, role->GetUpgradeManager()->GetUpgrade(UPGRADE_TYPE_4)->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_WEIYAN, role->GetUpgradeManager()->GetUpgrade(UPGRADE_TYPE_5)->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_SHOUHUAN, role->GetUpgradeManager()->GetUpgrade(UPGRADE_TYPE_6)->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_TAIL, role->GetUpgradeManager()->GetUpgrade(UPGRADE_TYPE_7)->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_FLYPET, role->GetUpgradeManager()->GetUpgrade(UPGRADE_TYPE_8)->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_LINGZHU, role->GetUpgradeManager()->GetUpgrade(UPGRADE_TYPE_0)->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_XIANBAO, role->GetUpgradeManager()->GetUpgrade(UPGRADE_TYPE_1)->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(SPECIAL_IMG_TYPE_LINGTONG, role->GetUpgradeManager()->GetUpgrade(UPGRADE_TYPE_2)->IsImageActive);

	UNSTD_STATIC_CHECK(SPECIAL_IMG_TYPE_MAX == 26)		//要记得添加函数进去
}
