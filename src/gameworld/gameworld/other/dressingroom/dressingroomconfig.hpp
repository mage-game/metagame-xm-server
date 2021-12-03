#ifndef __DRESSING_ROOM_CONFIG_HPP__
#define __DRESSING_ROOM_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>
#include <functional>
#include <set>

class Role;

struct SuitIDCfg
{
	bool operator< (const SuitIDCfg &rhs) const
	{
		if (suit_system_type != rhs.suit_system_type)
		{
			return suit_system_type < rhs.suit_system_type;
		}

		return img_id < rhs.img_id;
	}

	int suit_system_type = -1;
	int img_id = 0;
	int suit_index = -1;
	int sub_index = -1;
	int img_item_id = 0;
	int need_exchange_ticket_num = 0;
};

struct DressingRoomSuitCfg
{
	struct SuitAttrCfg
	{
		int img_count = 0;
		Attribute hp = 0;
		Attribute gongji = 0;
		Attribute fangyu = 0;
		Attribute mingzhong = 0;
		Attribute shanbi = 0;
		Attribute baoji = 0;
		Attribute jianren = 0;

		int hp_per = 0;
		int gongji_per = 0;
		int fangyu_per = 0;
		int mingzhong_per = 0;
		int shanbi_per = 0;
		int baoji_per = 0;
		int jianren_per = 0;
	};

	int suit_index = -1;
	unsigned int open_timestamp = 0;		// 开放时间
	std::vector<SuitIDCfg> id_cfg_vec;
	std::vector<SuitAttrCfg> attr_cfg_vec;
};

struct DressRoomOtherCfg
{
	int exchange_ticket_item_id = 0;
};

struct SystemFuncHelper
{
	SystemFuncHelper() : check_special_img_func(nullptr)
	{}

	using CheckSystemSpecialImgActiveFunc = std::function<bool(Role *role, int img_id)>;
	CheckSystemSpecialImgActiveFunc check_special_img_func;
};

class DressingRoomConfig : public ConfigBase
{
public:
	DressingRoomConfig();
	~DressingRoomConfig();

	enum SPECIAL_IMG_TYPE
	{
		SPECIAL_IMG_TYPE_MOUNT = 0,						// 坐骑
		SPECIAL_IMG_TYPE_WING = 1,						// 羽翼
		SPECIAL_IMG_TYPE_SHIZHUANG_PART_1 = 2,			// 时装-部位1 时装
		SPECIAL_IMG_TYPE_SHIZHUANG_PART_0 = 3,			// 时装-部位0 神兵
		SPECIAL_IMG_TYPE_FABAO = 4,						// 法宝
		SPECIAL_IMG_TYPE_HALO = 5,						// 光环
		SPECIAL_IMG_TYPE_FOOTPRINT = 6,					// 足迹
		SPECIAL_IMG_TYPE_FIGHT_MOUNT = 7,				// 战斗坐骑
		SPECIAL_IMG_TYPE_XIANNV = 8,					// 仙女
		SPECIAL_IMG_TYPE_SHENGONG = 9,					// 仙女光环
		SPECIAL_IMG_TYPE_SHENYI = 10,					// 仙女法阵
		SPECIAL_IMG_TYPE_JINGLING = 11,					// 仙宠
		SPECIAL_IMG_TYPE_TOUSHI = 12,					// 头饰
		SPECIAL_IMG_TYPE_YAOSHI = 13,					// 腰饰
		SPECIAL_IMG_TYPE_MASK = 14,						// 面饰
		SPECIAL_IMG_TYPE_QILINBI = 15,					// 麒麟臂
		SPECIAL_IMG_TYPE_MULIT_MOUNT = 16,				// 双骑
		SPECIAL_IMG_TYPE_LINGGONG = 17,					// 灵弓
		SPECIAL_IMG_TYPE_LINGQI = 18,					// 灵骑
		SPECIAL_IMG_TYPE_WEIYAN = 19,					// 尾焰
		SPECIAL_IMG_TYPE_SHOUHUAN = 20,					// 手环
		SPECIAL_IMG_TYPE_TAIL = 21,						// 尾巴
		SPECIAL_IMG_TYPE_FLYPET = 22,					// 飞宠
		SPECIAL_IMG_TYPE_LINGZHU = 23,					// 灵珠
		SPECIAL_IMG_TYPE_XIANBAO = 24,					// 仙宝
		SPECIAL_IMG_TYPE_LINGTONG = 25,					// 灵童				
		SPECIAL_IMG_TYPE_MAX =26
	};

	bool Init(const std::string &configname, std::string *err);

	int GetSuitCount() { return static_cast<int>(m_suit_cfg_vec.size()); }

	const DressingRoomSuitCfg::SuitAttrCfg * GetSuitAttrCfg(int suit_index, int suit_count);
	const std::vector<SuitIDCfg> * GetSuitIDCfgVec(int suit_index);
	const SystemFuncHelper * GetFuncHelper(int type);
	const SuitIDCfg * GetSuitInfo(int system_type, int img_id);
	const SuitIDCfg * GetSuitInfoByIndex(int suit_index, int sub_index);
	const DressRoomOtherCfg & GetOtherCfg() { return m_other_cfg; }

	unsigned int GetSuitOpenTimeStamp(int suit_index);

private:
	int InitSuitIDCfg(PugiXmlNode root_element);
	int InitSuitAttrCfg(PugiXmlNode root_element);
	int InitOtherCfg(PugiXmlNode root_element);
	int InitSuitOpenTimeCfg(PugiXmlNode root_element);
	
	void InitFuncHelper();

	std::set<SuitIDCfg>					m_suit_id_cfg_set;
	std::vector<DressingRoomSuitCfg>	m_suit_cfg_vec;
	DressRoomOtherCfg					m_other_cfg;

	SystemFuncHelper					m_func_list[SPECIAL_IMG_TYPE_MAX];
};

#endif //__DRESSING_ROOM_CONFIG_HPP__
