#ifndef __TERRITORY_WAR_CONFIG_HPP__
#define __TERRITORY_WAR_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>
#include <vector>
#include "obj/character/role.h"

static const int MAX_TRRITORYWAR_ROOM = 5;

struct TerritroyWarOtherCfg
{
	TerritroyWarOtherCfg():scene_id(0),add_credit_interval_s(0),add_credit(0),blue_relive_pos(0,0),red_relive_pos(0,0),
		kill_player_credit(0), kill_player_guild_credit(0), kill_car_credit(0), kill_car_guild_credit(0),
		kill_player_assist_credit(0), kill_car_assist_credit(0), assist_vaild_time(0), huizhang_extra_reward_item_count(0),
		ice_landmine_num_limit(0), fire_landmine_num_limit(0), center_relive_point_id(0), red_fortress_id(0), blue_fortress_id(0),
		attack_notice_interval(0), lucky_interval(0), luck_people_num(0){}

	int scene_id;
	unsigned int add_credit_interval_s;
	short add_credit;
	Posi blue_relive_pos;
	Posi red_relive_pos;
	short kill_player_credit;
	short kill_player_guild_credit;
	short kill_car_credit;
	short kill_car_guild_credit;
	short kill_player_assist_credit;
	short kill_car_assist_credit;
	unsigned int assist_vaild_time;
	int huizhang_extra_reward_item_count;
	short ice_landmine_num_limit;
	short fire_landmine_num_limit;
	unsigned short center_relive_point_id;
	unsigned short red_fortress_id;
	unsigned short blue_fortress_id;
	unsigned int attack_notice_interval;
	ItemConfigData huizhang_extra_reward[MAX_ATTACHMENT_ITEM_NUM];
	int lucky_interval;					// 幸运奖励间隔时间
	ItemConfigData lucky_item;			// 幸运奖励物品
	int luck_people_num;				// 幸运人数
};

struct TerritroyWarBuildingCfg
{
	TerritroyWarBuildingCfg():building_id(0), building_pos(0,0), guild_credit_reward(0),personal_credit_reward(0),side(0),preposition_monster_1(0), preposition_monster_2(0),
		building_index(0), kill_assist_credit(0), assist_vaild_time(0), tower_attack_man_per(0), tower_attack_fight_car_per(0), tower_attack_fang_car_per(0), tower_attack_cure_car_per(0){}
	
	int building_id;
	Posi building_pos;
	short guild_credit_reward;
	short personal_credit_reward;
	short side;
	int preposition_monster_1;
	int preposition_monster_2;
	short building_index;
	short kill_assist_credit;
	unsigned int assist_vaild_time;
	int tower_attack_man_per;
	int tower_attack_fight_car_per;
	int tower_attack_fang_car_per;
	int tower_attack_cure_car_per;
};

enum FIGHT_SHOP_GOODS_TYPE
{
	BIAN_SHEN = 0,
	MEDICAMENT,
	LANDMINE,

	FIGHT_SHOP_MAX_TYPE,	
};

enum FIGHT_SHOP_TYPE_0_INDDEX
{
	FIGHT_CAR_GONG = 0,
	FIGHT_CAR_FANG = 1,
	FIGHT_CAR_CURE = 2,

	TYPE_0_MAX_INDEX,
};

enum FIGHT_SHOP_TYPE_1_INDDEX
{
	EDGE_MEDICINE = 0,
	RELIVE_MEDICINE = 1,

	TYPE_1_MAX_INDEX,
};

enum FIGHT_SHOP_TYPE_2_INDDEX
{
	FIER_LANDMINE = 0,
	ICE_LANDMINE = 1,

	TYPE_2_MAX_INDEX,
};

struct TerritroyWarFightShopCfg
{
	TerritroyWarFightShopCfg():type(0),goods_id(0),cost_credit(0),guild_credit_reward(0),personal_credit_reward(0),param1(0),param2(0),param3(0),param4(0),param5(0),param6(0){}
	
	short type;
	short goods_id;
	short cost_credit;
	short guild_credit_reward;
	short personal_credit_reward;
	int param1;
	int param2;
	int param3;
	int param4;
	int param5;
	int param6;
};

enum RELIVE_SHOP_GOODS
{
	TOWER_WUDI = 0,
	TRANSPORT = 1,
	TRANSPORT_TO_CENTER = 2,

	MAX_RLIVE_SHOP_INDEX,	
};

struct TerritroyWarReliveShopCfg
{
	TerritroyWarReliveShopCfg():goods_id(0),cost_credit(0),param1(0),param2(0),param3(0){}
	
	short goods_id;
	short cost_credit;
	int param1;
	int param2;
	int param3;
};

struct TerritroyWarSkillItemCfg
{
	TerritroyWarSkillItemCfg(): skill_index(0), skill_level(0), distance(0), attack_range(0), enemy_num(0), cd_s(0), hurt_percent(0), fix_hurt(0), fix_hurt_on_fight_car(0),
		fix_hurt_on_fang_car(0),fix_hurt_on_cure_car(0),param_a(0), param_b(0), param_c(0), param_d(0) {}

	int skill_index;
	short skill_level;
	short distance;
	short attack_range;
	short enemy_num;
	unsigned int cd_s;
	int hurt_percent;
	int fix_hurt;
	int fix_hurt_on_fight_car;
	int fix_hurt_on_fang_car;
	int fix_hurt_on_cure_car;
	
	int param_a;
	int param_b;
	int param_c;
	int param_d;
};

enum TW_SKILL_INDEX
{
	TW_SKILL_INVALID = 0,
	TW_SKILL_INDEX1,		// 攻城车-主攻-普工
	TW_SKILL_INDEX2,		// 攻城车-主防-普工
	TW_SKILL_INDEX3,		// 攻城车-主治-普工
	TW_SKILL_INDEX4,		// 攻城车-主攻-技能
	TW_SKILL_INDEX5,		// 攻城车-主防-技能
	TW_SKILL_INDEX6,		// 攻城车-主治-技能

	TW_SKILL_MAX,
};

struct TerritroyWarPersonalRewardCfg
{
	TerritroyWarPersonalRewardCfg():reward_index(0),person_credit_min(0),person_credit_max(0), banggong(0), shengwang(0){}
	
	short reward_index;
	int person_credit_min;
	int person_credit_max;
	ItemConfigData item_1;
	ItemConfigData item_2;
	ItemConfigData item_3;
	int banggong;
	int shengwang;
};

struct TerritroyWarGuajiMonsterCfg
{
	TerritroyWarGuajiMonsterCfg():monster_id(0),kill_credit_reward(0){}

	int monster_id;
	short kill_credit_reward;

};

struct TerritroyWarActivityEndCfg
{
	TerritroyWarActivityEndCfg():room_index(0),reward_index(0), banggong(0), shengwang(0){}

	int room_index;
	short reward_index;

	ItemConfigData reward_item_1;
	ItemConfigData reward_item_2;
	ItemConfigData reward_item_3;
	int banggong;
	int shengwang;
};

struct TerritoryWarFixHurtCfg
{
	TerritoryWarFixHurtCfg():man_attack_fight_car_per(0), man_attack_fang_car_per(0), man_attack_cure_car_per(0),fix_hurt_on_tower(0), fix_hurt_on_relive(0), fix_hurt_on_fortress(0){}

	int man_attack_fight_car_per;
	int man_attack_fang_car_per;
	int man_attack_cure_car_per;
	int fix_hurt_on_tower;
	int fix_hurt_on_relive;
	int fix_hurt_on_fortress;
};

class TerritoryWarConfig : public ConfigBase
	{
public:

	TerritoryWarConfig();
	~TerritoryWarConfig();

	bool Init(const std::string &configname, std::string *er);

	int GetTerritoryWarEnterInfo(Role *role,Posi *pos);

	GuildID GetGuildIDByRank(int rank);
	bool CanEnterRoom(Role *role, int room_index);

	const TerritroyWarOtherCfg& GetOtherCfg(){return m_other_cfg;}
	const TerritoryWarFixHurtCfg& GetFixHurtCfg(){return m_fixhurt_cfg;}
	const TerritroyWarBuildingCfg * GetBuildingCfg(int building_id) const;
	const std::map<int, TerritroyWarBuildingCfg> & GetBuildingCfgMap(){return m_building_cfg_list;}
	const TerritroyWarFightShopCfg * GetFightShopCfg(short type,short goods_id) const;
	const TerritroyWarReliveShopCfg * GetReliveShopCfg(short goods_id) const;
	const TerritroyWarSkillItemCfg * GetSkillItemCfg(int skill_index) const;
	const TerritroyWarPersonalRewardCfg * GetPersonalCreditCfg(int credit)const;
	const TerritroyWarPersonalRewardCfg * GetPersonalCreditCfgByIndex(int index)const;
	const TerritroyWarGuajiMonsterCfg* GetGuajiMonsterCfg(int monster_id) const;
	const std::map<int, TerritroyWarGuajiMonsterCfg> & GetGuaJiMonsterMap(){return m_guaji_monster_cfg_list;}
	const TerritroyWarActivityEndCfg * GetActivityEndCfg(int room_index,short reward_index) const;

private:

	int InitOtherCfg(PugiXmlNode RootElement);
	int InitbBuildingCfg(PugiXmlNode RootElement);
	int InitFightShopCfg(PugiXmlNode RootElement);
	int InitReliveShopCfg(PugiXmlNode RootElement);
	int InitSkill(PugiXmlNode RootElement);
	int InitPersonaleRewardCfg(PugiXmlNode RootElement);
	int InitGuajiMonster(PugiXmlNode RootElement);
	int InitActivityEndCfg(PugiXmlNode RootElement);
	int InitFixHurtCfg(PugiXmlNode RootElement);
	
	TerritroyWarOtherCfg m_other_cfg;
	TerritoryWarFixHurtCfg m_fixhurt_cfg;
	std::map<int, TerritroyWarBuildingCfg> m_building_cfg_list;
	std::map<short, TerritroyWarFightShopCfg> m_fight_shop_cfg_list[FIGHT_SHOP_MAX_TYPE];
	std::map<short, TerritroyWarReliveShopCfg> m_relive_shop_cfg_list;
	std::map<int, TerritroyWarSkillItemCfg> m_skill_map;
	std::map<int, TerritroyWarPersonalRewardCfg> m_person_credit_reward_cfg_list;
	std::map<int, TerritroyWarGuajiMonsterCfg> m_guaji_monster_cfg_list;
	
	std::map<int, TerritroyWarActivityEndCfg> m_activity_reward_cfg_list[MAX_TRRITORYWAR_ROOM];
};


#endif // __TERRITORY_WAR_CONFIG_HPP__
