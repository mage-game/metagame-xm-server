#ifndef _CAMP_CONFG_HPP_
#define _CAMP_CONFG_HPP_

#include <string>
#include <map>
#include <vector>
#include "servercommon/serverdef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "obj/character/attribute.hpp"
#include "equipment/equipment.h"
#include "item/itembase.h"
#include "item/itemgriddata.h"
#include "servercommon/campdef.hpp"
#include "servercommon/configbase.h"

struct CampOtherCfg
{
	CampOtherCfg() : gjdb_sceneid(0), gjdb_level_limit(0), gjdb_enterpos(0, 0), gjdb_costitemid(0), gjdb_timeslimit(0), first_act_mountimgid(0), 
					dx_addextraexp_once(0), dx_sf_addgongxun_once(0), dx_gf_addgongxun_once(0), 
					dx_killrole_addgongxun(0), dx_killrole_timegap(0), dx_killdiaoxiang_addgongxun(0), dx_sceneid1(0),
					dx_sceneid2(0), dx_sceneid3(0) {}

	int gjdb_sceneid;
	int gjdb_level_limit;
	Posi gjdb_enterpos;
	ItemID gjdb_costitemid;
	int gjdb_timeslimit;

	int first_act_mountimgid;

	int dx_addextraexp_once;
	int dx_sf_addgongxun_once;
	int dx_gf_addgongxun_once;
	int dx_killrole_addgongxun;
	unsigned int dx_killrole_timegap;
	int dx_killdiaoxiang_addgongxun;

	int dx_sceneid1;
	int dx_sceneid2;
	int dx_sceneid3;
};

struct CampNormalDuoBaoLayerCfg
{
	CampNormalDuoBaoLayerCfg() : camp(0), sceneid(0), enterpos(0, 0), monster_relivetime(0) {}

	int camp;
	int sceneid;
	Posi enterpos;
	int monster_relivetime;
};

struct CampTitleItemCfg
{
	CampTitleItemCfg() : camp(0), rank(0), titleid(0) {}

	int camp;
	int rank;
	int titleid;
};

struct CampDiaoXiangPosItemCfg
{
	CampDiaoXiangPosItemCfg() : sceneid(0), pos(0, 0) {}

	int sceneid;
	Posi pos;
};

struct CampDiaoXiangAttrItemCfg
{
	CampDiaoXiangAttrItemCfg() : worldlevel(0), maxhp(0), fangyu(0), shanbi(0), dikangshanghai(0), kangbao_per(0), mianshang_per(0) {}

	int worldlevel;
	int maxhp;
	int	fangyu;
	int shanbi;
	int dikangshanghai;
	int kangbao_per;
	int	mianshang_per;
};

struct GongXunRankRewardCfg
{
	GongXunRankRewardCfg() : rank(0), item_count(0) {}

	int rank;
	int item_count;

	ItemConfigData reward_item_list[HONOR_REWARD_ITEM_NUM];
};

struct GongXunValueRewardCfg
{
	GongXunValueRewardCfg() : need_gongxun(0), item_count(0) {}

	int need_gongxun;
	int item_count;

	ItemConfigData reward_item_list[HONOR_REWARD_ITEM_NUM];
};

//////魂炼配置////////////////////////////////////////////////////
struct HunlianCfg
{
	HunlianCfg() : level(0), coin(0), stuff_id(0), stuff_count(0), base_succ_rate(0),
		is_downlevel_on_fail(0), hunliandan_id(0), hunliandan_count(0), add_succ_rate(0), max_hp(0),
		gongji(0), fangyu(0), mingzhong(0), shanbi(0)
	{

	}

	int level;
	int coin;
	ItemID stuff_id;
	int stuff_count;
	int base_succ_rate;
	int is_downlevel_on_fail;
	ItemID hunliandan_id;
	int hunliandan_count;
	int add_succ_rate;
	int max_hp;
	int gongji; 
	int fangyu;
	int mingzhong;
	int shanbi;
};

struct CampEquipHunLianCfg
{
	CampEquipHunLianCfg() : equip_index(0)
	{

	}

	int equip_index;
	HunlianCfg level_cfg[MAX_HUNLIAN_LEVEL + 1];
};

struct CampEquipmentRecyleCfg
{
	CampEquipmentRecyleCfg() : seq(0), color(0), limit_level(0), exp(0)
	{

	}

	int seq;
	int color;
	int limit_level;
	int exp;
};

struct CampBeastCfg
{
	CampBeastCfg() : level(0), need_exp(0), max_hp(0), fangyu(0), gongji(0), shanbi(0), mingzhong(0)
	{

	}

	int level;
	int need_exp;
	int max_hp;
	int fangyu;
	int gongji;
	int shanbi;
	int mingzhong;
};

class CampConfig : public ConfigBase
{
public:
	CampConfig();
	~CampConfig();

	bool Init(const std::string &configname, std::string *err);

	const CampNormalDuoBaoLayerCfg *GetNormalDuoBaoLayerCfg(int sceneid);
	const CampOtherCfg &GetOtherCfg() { return m_othercfg; }
	int GetCampDefendSceneIdByActType(int activity_type);
	int GetCampBySceneId(int sceneid);
	int GetTitleId(int camp, int rank);
	const CampDiaoXiangPosItemCfg *GetDiaoXiangPosItemCfg(int index);
	const CampDiaoXiangAttrItemCfg *GetDiaoXiangAttrItemCfg(int world_level);
	const GongXunRankRewardCfg *GetGongXunRankRewardCfg(int seq);
	const GongXunValueRewardCfg *GetGongXunValueRewardCfg(int gongxun_value); 
	const HunlianCfg *GetEquipHunlianCfg(const int equip_index, const int level); 

	const int GetEquipRecyleCfg(const int limit_level, const int color);
	const CampBeastCfg *GetBeastCfg(const int level);
	int GetBeastMaxLevel() { return m_max_beast_level; }

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitNormalDuoBaoCfg(PugiXmlNode RootElement);
	int InitCampTitleCfg(PugiXmlNode RootElement);
	int InitDiaoXiangPosCfg(PugiXmlNode RootElement);
	int InitDiaoXiangAttrCfg(PugiXmlNode RootElement);
	int InitGongXunRankRewardCfg(PugiXmlNode RootElement);
	int InitGongXunValueRewardCfg(PugiXmlNode RootElement);
	int InitCampHunLianCfg(PugiXmlNode RootElement);
	int InitCampEquipmentRecyle(PugiXmlNode RootElement);
	int InitCampBeast(PugiXmlNode RootElement);

	CampOtherCfg m_othercfg;

	typedef std::map<int, CampNormalDuoBaoLayerCfg> NormalDuoBaoLayerDic;
	NormalDuoBaoLayerDic m_normal_duobao_layer_dic;

	typedef std::vector<CampTitleItemCfg> CampTitleVec;
	CampTitleVec m_title_vec;

	CampDiaoXiangPosItemCfg m_diaoxiang_pos_list[MAX_CAMP_RECORD_RANK3_NUM];

	typedef std::vector<CampDiaoXiangAttrItemCfg> DiaoXiangAttrVec;
	DiaoXiangAttrVec m_diaoxiang_attr_vec;

	GongXunRankRewardCfg rand_reward_cfg_list[GONG_XUN_RANDK_NUM];
	GongXunValueRewardCfg value_reward_cfg_list[GONG_XUN_VALUE_NUM];

	//师门魂炼相关
	CampEquipHunLianCfg m_hunlian_cfg_list[MAX_CAMP_EQUIP_INDEX];

	//师门装备回收
	typedef std::vector<CampEquipmentRecyleCfg> CampEquipmentRecyleCfgVec;
	CampEquipmentRecyleCfgVec m_equipment_recyle_cfg_vec;

	typedef std::vector<CampBeastCfg> CampBeastCfgVec;
	CampBeastCfgVec m_beast_cfg_vec;

	int m_max_beast_level;

};

#endif


