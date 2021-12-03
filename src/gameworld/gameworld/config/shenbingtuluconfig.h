#ifndef __SHENBING_TULU_CONFIG_H__
#define __SHENBING_TULU_CONFIG_H__

#include <string>
#include "servercommon/configbase.h"
#include "servercommon/configcommon.h"
#include "gameworld/config/gameconfigcommon/gameconfigcommon.h"
#include "servercommon/struct/redequipcollectdef.hpp"
#include "servercommon/struct/orangeequipcollectdef.hpp"

struct ShenBingTuLuOtherConfig
{
	ShenBingTuLuOtherConfig() : red_equip_collect_active_puton_count(0), orange_equip_collect_active_puton_count(0) {}

	int red_equip_collect_active_puton_count;
	int orange_equip_collect_active_puton_count;						
};

//-------------------------------------------------------------------------------------------

// ��װ�ռ�
struct RedEquipCollectConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _prof)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			prof = _prof;

			if (!equip_items.ReadConfig(data_element, "equip_items")) return -2;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// ��װ����
		int prof = 0;					// ְҵ
		ListConfig<ItemID> equip_items;	// ��װ����Ʒ
	};

	CfgItem cfg_item_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT * PROF_TYPE_PROF_NO_LIMIT];
};

// ��װ�ռ�-���Լӳ�
struct RedEquipCollectAttrConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _collect_count)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			collect_count = _collect_count;

			if (attr_cfg.ReadConfig(data_element) < 0)
			{
				return -2;
			}

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// ��װ����
		int collect_count = 0;			// �ռ�����
		AttributesConfig attr_cfg;		// ���Լӳ�

	};

	CfgItem cfg_item_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT * RED_EQUIP_COLLECT_SLOT_COUNT];
};

// ��װ�ռ�-����
struct RedEquipCollectOtherConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _pro)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			pro = _pro;
			if (!PugiGetSubNodeValue(data_element, "order", order)) return -2;
			if (!PugiGetSubNodeValue(data_element, "color", color)) return -3;
			if (!PugiGetSubNodeValue(data_element, "level", level)) return -4;
			if (!PugiGetSubNodeValue(data_element, "star_add_attr_percent", star_add_attr_percent)) return -5;
			if (!PugiGetSubNodeValue(data_element, "reward_title_id", reward_title_id)) return -6;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// ��װ����
		int pro = 0;					// ְҵ����
		int order = 0;					// ��Ӧװ����
		int color = 0;					// ��Ӧװ����ɫ
		int level = 0;					// ����ȼ�
		int star_add_attr_percent = 0;	// �Ǽ��ӳ����԰ٷֱ�
		int reward_title_id = 0;		// �����ƺ�id
	};

	CfgItem cfg_item_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT][RED_EQUIP_COLLECT_PRO_MAX];
};

// ��װ�ռ�-�����
struct RedEquipCollectActConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			if (!PugiGetSubNodeValue(data_element, "collect_count", collect_count)) return -2;
			if (!reward_item.ReadConfig(data_element.child("reward_item"))) return -3;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// ���
		int collect_count = 0;			// ������װ��
		ItemConfigData reward_item;		// ������Ʒ
	};

	CfgItem cfg_item_list[RED_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT];
};

// ----------------------------------------------------------------------------------------
// ��װ�ռ�
struct OrangeEquipCollectConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _prof)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			prof = _prof;

			if (!equip_items.ReadConfig(data_element, "equip_items")) return -2;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// ��װ����
		int prof = 0;					// ְҵ
		ListConfig<ItemID> equip_items;	// ��װ����Ʒ
	};

	CfgItem cfg_item_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT * PROF_TYPE_PROF_NO_LIMIT];
};

// ��װ�ռ�-���Լӳ�
struct OrangeEquipCollectAttrConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq, int _collect_count)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			collect_count = _collect_count;

			if (attr_cfg.ReadConfig(data_element) < 0) return -2;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// ��װ����
		int collect_count = 0;			// �ռ�����
		AttributesConfig attr_cfg;		// ���Լӳ�

	};

	CfgItem cfg_item_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT * ORANGE_EQUIP_COLLECT_SLOT_COUNT];
};

// ��װ�ռ�-����
struct OrangeEquipCollectOtherConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			if (!PugiGetSubNodeValue(data_element, "active_role_level", active_level)) return -2;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// ��װ����
		int active_level;				// ����ȼ�
	};

	CfgItem cfg_item_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT];
};

// ��װ�ռ�-�����
struct OrangeEquipCollectActConfig
{
	struct CfgItem
	{
		int ReadConfig(PugiXmlNode data_element, int _seq)
		{
			if (has_read) return -1;
			has_read = 1;

			seq = _seq;
			if (!PugiGetSubNodeValue(data_element, "collect_count", collect_count)) return -2;
			if (!reward_item.ReadConfig(data_element.child("reward_item"))) return -3;

			return 0;
		}

		int has_read = 0;
		int seq = 0;					// ���
		int collect_count = 0;			// ������װ��
		ItemConfigData reward_item;		// ������Ʒ
	};

	CfgItem cfg_item_list[ORANGE_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT];
};

class ShenBingTuLuConfig : public ConfigBase
{
public:
	ShenBingTuLuConfig();
	~ShenBingTuLuConfig();

	bool Init(const std::string &path, std::string *err);
	int InitOtherCfg(PugiXmlNode RootElement);

	const ShenBingTuLuOtherConfig & GetOtherCfg() { return m_other_cfg; }

	const RedEquipCollectAttrConfig::CfgItem * GetRedEquipCollectAttrCfg(int seq, int cnt);
	const RedEquipCollectConfig & GetRedEquipCollectConfig() { return m_red_equip_collect_cfg; }
	const RedEquipCollectConfig::CfgItem * GetRedEquipCollectItemCfg(int seq, int prof);
	const RedEquipCollectOtherConfig::CfgItem * GetRedEquipCollectOtherCfg(int seq, int pro);
	const RedEquipCollectActConfig::CfgItem * GetRedEquipCollectActCfg(int seq);

	//----------------------------------------------------------------------------------------
	const OrangeEquipCollectAttrConfig::CfgItem * GetOrangeEquipCollectAttrCfg(int seq, int cnt);
	const OrangeEquipCollectConfig & GetOrangeEquipCollectConfig() { return m_orange_equip_collect_cfg; }
	const OrangeEquipCollectConfig::CfgItem * GetOrangeEquipCollectItemCfg(int seq, int prof);
	const OrangeEquipCollectOtherConfig::CfgItem * GetOrangeEquipCollectOtherCfg(int seq);
	const OrangeEquipCollectActConfig::CfgItem * GetOrangeEquipCollectActCfg(int seq);

public:
	int InitRedEquipCollectCfg(PugiXmlNode root_element);
	int InitRedEquipCollectAttrCfg(PugiXmlNode root_element);
	int InitRedEquipCollectOtherCfg(PugiXmlNode root_element);
	int InitRedEquipCollectActCfg(PugiXmlNode root_element);

	//-------------------------------------------------------------------------------------------
	int InitOrangeEquipCollectCfg(PugiXmlNode root_element);
	int InitOrangeEquipCollectAttrCfg(PugiXmlNode root_element);
	int InitOrangeEquipCollectOtherCfg(PugiXmlNode root_element);
	int InitOrangeEquipCollectActCfg(PugiXmlNode root_element);

	//-------------------------------------------------------------------------------------------

	bool IsValidRedEquipCollectAttrConfigSeqCount(int seq, int cnt, bool is_init = false);	// �Ƿ��ǺϷ��ĺ�װ�ռ�-���Լӳ�����
	bool IsValidRedEquipCollectConfigSeqProf(int seq, int prof, bool is_init = false);		// �Ƿ��ǺϷ��ĺ�װ�ռ�����
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------

	bool IsValidOrangeEquipCollectAttrConfigSeqCount(int seq, int cnt, bool is_init = false);	// �Ƿ��ǺϷ��ĳ�װ�ռ�-���Լӳ�����
	bool IsValidOrangeEquipCollectConfigSeqProf(int seq, int prof, bool is_init = false);		// �Ƿ��ǺϷ��ĳ�װ�ռ�����
	//-------------------------------------------------------------------------------------------

	ShenBingTuLuOtherConfig m_other_cfg;

	RedEquipCollectConfig m_red_equip_collect_cfg;
	RedEquipCollectAttrConfig m_red_equip_collect_attr_cfg;
	RedEquipCollectOtherConfig m_red_equip_collect_other_cfg;
	RedEquipCollectActConfig m_red_equip_collect_act_cfg;

	OrangeEquipCollectConfig m_orange_equip_collect_cfg;
	OrangeEquipCollectAttrConfig m_orange_equip_collect_attr_cfg;
	OrangeEquipCollectOtherConfig m_orange_equip_collect_other_cfg;
	OrangeEquipCollectActConfig m_orange_equip_collect_act_cfg;
};


#endif // __SHENBING_TULU_CONFIG_H__

