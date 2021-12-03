#ifndef _FBEQUIPCONFIG_HPP_
#define _FBEQUIPCONFIG_HPP_

#include "servercommon/fbdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "gamedef.h"

#include <map>
#include "servercommon/configbase.h"

// ������������� F-����-�������.xls

struct FBEquipJumpLayerCfg
{
	FBEquipJumpLayerCfg() : target_layer(0), limit_time(0), enter_pos(0, 0) {}

	bool CanJump() const { return target_layer > 0; } 

	int target_layer;											// ���� Ŀ���
	int limit_time;												// ���� ��ʱ
	ItemConfigData gift_item;									// �������
	Posi enter_pos;												// Ŀ����������
};

struct FBEquipLayerCfg
{
	FBEquipLayerCfg() : layer(0), scene_id(0), is_mystery(true), show_layer(0), pass_exp(0), is_pass_notice(false)
	{
		dropid_list.clear();
	}

	int layer;													// ��
	int scene_id;												// ����ID
	bool is_mystery;											// �Ƿ������ز�
	int show_layer;												// ��ʾ��
	long long pass_exp;												// ͨ�ؾ���
	bool is_pass_notice;
	FBEquipJumpLayerCfg jump_layer_cfg;							// ��������

	std::vector<UInt16> dropid_list;							// ����ID�б�(���������֮ǰ�Ĳ����Ľ���)
};

struct FBEquipMysteryShopItem
{
	FBEquipMysteryShopItem() : mystery_layer(0), gold_price(0), buy_limit(0) {}

	int mystery_layer;											// ��Ӧ�����ز�
	ItemConfigData item_cfg_data;								// ��Ʒ��Ӧ����Ʒ��Ϣ
	int gold_price;												// �۸� �ǰ�Ԫ��
	int buy_limit;												// �޹�
};

struct FBEquipOtherCfg
{
	FBEquipOtherCfg() : team_double_cost_gold(0), trial_task_id(0), 
		guild_add_gongji_percent(0), join_times(0), kick_time_s(0),
		buy_double_reward_need_gold(0), buy_triple_reward_need_gold(0), double_reward_need_vip_level(0), triple_reward_need_vip_level(0){}

	int team_double_cost_gold;									// ���˫������Ԫ��
	int trial_task_id;											// �������� ����id
	int guild_add_gongji_percent;								// ���˼ӹ����ٷֱ�
	int join_times;												// �������
	int kick_time_s;                                            // �߳�ʱ��
	int buy_double_reward_need_gold;
	int buy_triple_reward_need_gold;
	int double_reward_need_vip_level;
	int triple_reward_need_vip_level;

	std::vector<UInt16> dropid_list;							// ���⽱��
};

class TiXmlElement;

class FBEquipConfig : public ConfigBase
{
public:
	FBEquipConfig();
	~FBEquipConfig();

	bool Init(const std::string &configname, std::string *err);

	const FBEquipOtherCfg & GetOtherCfg() { return m_other_cfg; }

	const FBEquipLayerCfg * GetTeamLayerCfgByLayer(int layer) const;
	const FBEquipLayerCfg * GetTeamLayerCfgBySceneID(int scene_id) const;
	const FBEquipJumpLayerCfg * GetTeamJumpCfgByLayer(int layer) const;
	const FBEquipJumpLayerCfg * GetTeamJumpCfgBySceneID(int scene_id) const;
	const FBEquipMysteryShopItem * GetTeamMysteryShopCfg(int shop_item_seq) const;
	int GetTeamSceneIDByLayer(int layer);
	int GetTeamLayerBySceneID(int scene_id);
	const int GetTeamMaxLayer() { return m_team_max_layer; }
	
private:
	int InitOtherCfg(TiXmlElement *RootElement);
	int InitTeamLayerCfg(TiXmlElement *RootElement);
	int InitTeamMysteryShopCfg(TiXmlElement *RootElement);

	// ��������
	FBEquipOtherCfg m_other_cfg;

	// ��� ���볡����Ӧ��ϵ
	std::map<int, FBEquipLayerCfg> m_team_scene_to_layer_map;
	int m_team_max_layer;
	FBEquipLayerCfg m_team_layer_cfg[FB_EQUIP_MAX_LAYER_ID + 1];

	// ��� ���ز��̵�
	FBEquipMysteryShopItem m_team_mystery_shop_list[FB_EQUIP_MAX_GOODS_SEQ];

};

#endif
