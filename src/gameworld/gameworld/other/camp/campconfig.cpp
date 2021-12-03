#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/attribute.hpp"
#include "servercommon/servercommon.h"
#include "campconfig.hpp"
#include "gamedef.h"
#include "skill/skillpool.hpp"
#include "servercommon/activitydef.hpp"

CampConfig::CampConfig() : m_max_beast_level(0)
{

}

CampConfig::~CampConfig()
{

}

bool CampConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CampConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	// 其他
	{
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

	// 师门普通夺宝
	{
		PugiXmlNode root_element = RootElement.child("normalduobao");
		if (root_element.empty())
		{
			*err = path + ": no [normalduobao].";
			return false;
		}

		iRet = this->InitNormalDuoBaoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitNormalDuoBaoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 师门称号
	{
		PugiXmlNode root_element = RootElement.child("camptitle");
		if (root_element.empty())
		{
			*err = path + ": no [camptitle].";
			return false;
		}

		iRet = this->InitCampTitleCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCampTitleCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	
	// 雕象所在位置
	{
		PugiXmlNode root_element = RootElement.child("diaoxiang_pos");
		if (root_element.empty())
		{
			*err = path + ": no [diaoxiang_pos].";
			return false;
		}

		iRet = this->InitDiaoXiangPosCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDiaoXiangPosCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 雕象属性
	{
		PugiXmlNode root_element = RootElement.child("diaoxiang_attr");
		if (root_element.empty())
		{
			*err = path + ": no [diaoxiang_attr].";
			return false;
		}

		iRet = this->InitDiaoXiangAttrCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDiaoXiangAttrCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode rootElement = RootElement.child("gongxun_rank_reward");
		if (rootElement.empty())
		{
			*err = path +": xml no gongxun_rank_reward node.";
			return false;
		}

		iRet = this->InitGongXunRankRewardCfg(rootElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGongXunRankRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode rootElement = RootElement.child("gongxun_value_reward");
		if (rootElement.empty())
		{
			*err = path +": xml no gongxun_value_reward node.";
			return false;
		}

		iRet = this->InitGongXunValueRewardCfg(rootElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGongXunValueRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 师门装备魂炼
		PugiXmlNode hunlian_element = RootElement.child("camp_equip_hunlian");
		if (hunlian_element.empty())
		{
			*err = path + ": no [camp_equip_hunlian].";
			return false;
		}

		iRet = InitCampHunLianCfg(hunlian_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCampHunLianCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 师门装备回收
		PugiXmlNode equipment_recyle = RootElement.child("camp_equip_recyle");
		if (NULL == equipment_recyle)
		{
			*err = path + ": no [camp_equip_recyle].";
			return false;
		}

		iRet = InitCampEquipmentRecyle(equipment_recyle);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCampEquipmentRecyle failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 师门神兽
		PugiXmlNode camp_beast = RootElement.child("camp_beast");
		if (NULL == camp_beast)
		{
			*err = path + ": no [camp_beast].";
			return false;
		}

		iRet = InitCampBeast(camp_beast);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCampBeast failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CampConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "gjdb_sceneid", m_othercfg.gjdb_sceneid))
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_othercfg.gjdb_sceneid);

	if (!PugiGetSubNodeValue(data_element, "gjdb_level_limit", m_othercfg.gjdb_level_limit) || m_othercfg.gjdb_level_limit < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "gjdb_enterposx", m_othercfg.gjdb_enterpos.x) || m_othercfg.gjdb_enterpos.x <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "gjdb_enterposy", m_othercfg.gjdb_enterpos.y) || m_othercfg.gjdb_enterpos.y <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "gjdb_costitemid", m_othercfg.gjdb_costitemid) || NULL == ITEMPOOL->GetItem(m_othercfg.gjdb_costitemid))
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "gjdb_timeslimit", m_othercfg.gjdb_timeslimit) || m_othercfg.gjdb_timeslimit <= 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(data_element, "first_act_mountimgid", m_othercfg.first_act_mountimgid) || m_othercfg.first_act_mountimgid < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "dx_addextraexp_once", m_othercfg.dx_addextraexp_once) || m_othercfg.dx_addextraexp_once <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "dx_sf_addgongxun_once", m_othercfg.dx_sf_addgongxun_once) || m_othercfg.dx_sf_addgongxun_once <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "dx_gf_addgongxun_once", m_othercfg.dx_gf_addgongxun_once) || m_othercfg.dx_gf_addgongxun_once <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "dx_killrole_addgongxun", m_othercfg.dx_killrole_addgongxun) || m_othercfg.dx_killrole_addgongxun <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "dx_killrole_timegap", m_othercfg.dx_killrole_timegap) || m_othercfg.dx_killrole_timegap <= 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "dx_killdiaoxiang_addgongxun", m_othercfg.dx_killdiaoxiang_addgongxun) || m_othercfg.dx_killdiaoxiang_addgongxun <= 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "dx_sceneid1", m_othercfg.dx_sceneid1) || m_othercfg.dx_sceneid1 <= 0)
	{
		return -16;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_othercfg.dx_sceneid1);

	if (!PugiGetSubNodeValue(data_element, "dx_sceneid2", m_othercfg.dx_sceneid2) || m_othercfg.dx_sceneid2 <= 0)
	{
		return -17;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_othercfg.dx_sceneid2);

	if (!PugiGetSubNodeValue(data_element, "dx_sceneid3", m_othercfg.dx_sceneid3) || m_othercfg.dx_sceneid3 <= 0)
	{
		return -18;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_othercfg.dx_sceneid3);

	return 0;
}

int CampConfig::InitNormalDuoBaoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int sceneid = 0;

		if (!PugiGetSubNodeValue(data_element, "sceneid", sceneid) || sceneid < 0)
		{
			return -1;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(sceneid);

		CampNormalDuoBaoLayerCfg &item_cfg = m_normal_duobao_layer_dic[sceneid];
		item_cfg.sceneid = sceneid;

		if (!PugiGetSubNodeValue(data_element, "camp", item_cfg.camp) || item_cfg.camp <= CAMP_TYPE_INVALID || item_cfg.camp >= CAMP_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "enterposx", item_cfg.enterpos.x) || item_cfg.enterpos.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "enterposy", item_cfg.enterpos.y) || item_cfg.enterpos.y <= 0)
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CampConfig::InitCampTitleCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		CampTitleItemCfg item_cfg;

		if (!PugiGetSubNodeValue(data_element, "camp", item_cfg.camp) || item_cfg.camp <= CAMP_TYPE_INVALID || item_cfg.camp >= CAMP_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "rank", item_cfg.rank) || item_cfg.rank < 0 || item_cfg.rank > MAX_CAMP_TOPRANK3)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "titleid", item_cfg.titleid) || item_cfg.titleid <= 0)
		{
			return -3;
		}

		m_title_vec.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CampConfig::InitDiaoXiangPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int count = 0;
	while (!data_element.empty())
	{
		if (count >= MAX_CAMP_RECORD_RANK3_NUM)
		{
			return -100;
		}

		CampDiaoXiangPosItemCfg &item_cfg = m_diaoxiang_pos_list[count];
		++ count;

		if (!PugiGetSubNodeValue(data_element, "sceneid", item_cfg.sceneid) || item_cfg.sceneid <= 0)
		{
			return -1;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(item_cfg.sceneid);

		if (!PugiGetSubNodeValue(data_element, "posx", item_cfg.pos.x) || item_cfg.pos.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "posy", item_cfg.pos.y) || item_cfg.pos.y <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CampConfig::InitDiaoXiangAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_level = -1;
	while (!data_element.empty())
	{
		CampDiaoXiangAttrItemCfg item_cfg;

		if (!PugiGetSubNodeValue(data_element, "worldlevel", item_cfg.worldlevel) || item_cfg.worldlevel < 0 || item_cfg.worldlevel <= last_level)
		{
			return -1;
		}
		last_level = item_cfg.worldlevel;

		if (!PugiGetSubNodeValue(data_element, "maxhp", item_cfg.maxhp) || item_cfg.maxhp <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", item_cfg.fangyu) || item_cfg.fangyu < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", item_cfg.shanbi) || item_cfg.shanbi < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "dikangshanghai", item_cfg.dikangshanghai) || item_cfg.dikangshanghai < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "kangbao_per", item_cfg.kangbao_per) || item_cfg.kangbao_per < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "mianshang_per", item_cfg.mianshang_per) || item_cfg.mianshang_per < 0)
		{
			return -7;
		}

		m_diaoxiang_attr_vec.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CampConfig::InitGongXunRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int seq = 0;
	int last_lank = 0;

	while (!data_element.empty())
	{
		if (seq >= GONG_XUN_RANDK_NUM)
		{
			return -100;
		}

		GongXunRankRewardCfg &item_cfg = rand_reward_cfg_list[seq];

		if (!PugiGetSubNodeValue(data_element, "rank", item_cfg.rank) || 1 != item_cfg.rank - last_lank)
		{
			return -1;
		}
		last_lank = item_cfg.rank;

		PugiXmlNode item_list_element = data_element.child("reward_item_list");
		if (item_list_element.empty())
		{
			return -2;
		}

		PugiXmlNode item_element = item_list_element.child("reward_item");
		if (item_element.empty())
		{
			return -3;
		}

		while (item_element)
		{
			if (item_cfg.item_count >= HONOR_REWARD_ITEM_NUM)
			{
				return -103;
			}

			if (!item_cfg.reward_item_list[item_cfg.item_count].ReadConfig(item_element))
			{
				return -104;
			}

			++ item_cfg.item_count;
			item_element = item_element.next_sibling();
		}

		++ seq;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CampConfig::InitGongXunValueRewardCfg(PugiXmlNode RootElement)
{

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int last_need_gongxun = 0;
	int seq = 0;

	while (!data_element.empty())
	{
		if (seq >= GONG_XUN_VALUE_NUM)
		{
			return -1;
		}

		GongXunValueRewardCfg &item_cfg = value_reward_cfg_list[seq];

		if (!PugiGetSubNodeValue(data_element, "need_gongxun", item_cfg.need_gongxun) || item_cfg.need_gongxun <= last_need_gongxun)
		{
			return -2;
		}
		last_need_gongxun = item_cfg.need_gongxun;

		PugiXmlNode item_list_element = data_element.child("reward_item_list");
		if (item_list_element.empty())
		{
			return -102;
		}

		PugiXmlNode item_element = item_list_element.child("reward_item");
		if (item_element.empty())
		{
			return -103;
		}

		while (item_element)
		{
			if (item_cfg.item_count >= HONOR_REWARD_ITEM_NUM)
			{
				return -104;
			}

			if (!item_cfg.reward_item_list[item_cfg.item_count].ReadConfig(item_element))
			{
				return -105;
			}

			++ item_cfg.item_count;
			item_element = item_element.next_sibling();
		}

		++ seq;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CampConfig::InitCampHunLianCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	int equip_index = -1;
	int level = -1;

	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "equip_index", equip_index) || equip_index < 0 || equip_index >= MAX_CAMP_EQUIP_INDEX) 
		{
			return -1;
		}
		CampEquipHunLianCfg *hunlian_index_cfg = &m_hunlian_cfg_list[equip_index];
		hunlian_index_cfg->equip_index = equip_index;
		
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_HUNLIAN_LEVEL) 
		{
			return -2;
		}
		HunlianCfg *cfg = &m_hunlian_cfg_list[equip_index].level_cfg[level];
		cfg->level = level;

		if (!PugiGetSubNodeValue(dataElement, "coin", cfg->coin) || cfg->coin <= 0) 
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg->stuff_id) || NULL == ITEMPOOL->GetItem(cfg->stuff_id)) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_count", cfg->stuff_count) || cfg->stuff_count <= 0) 
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "base_succ_rate", cfg->base_succ_rate) || cfg->base_succ_rate <= 0 || cfg->base_succ_rate > 100) 
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_succ_rate", cfg->add_succ_rate) || cfg->add_succ_rate < 0) 
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_downlevel_on_fail", cfg->is_downlevel_on_fail) || cfg->is_downlevel_on_fail < 0 || cfg->is_downlevel_on_fail > 1) 
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "hunliandan_id", cfg->hunliandan_id) || NULL == ITEMPOOL->GetItem(cfg->hunliandan_id)) 
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "hunliandan_count", cfg->hunliandan_count) || cfg->hunliandan_count < 0) 
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg->gongji) || cfg->gongji < 0) 
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_hp", cfg->max_hp) || cfg->max_hp < 0) 
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg->fangyu) || cfg->fangyu < 0) 
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg->mingzhong) || cfg->mingzhong < 0) 
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg->shanbi) || cfg->shanbi < 0) 
		{
			return -15;
		}

		++ equip_index;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CampConfig::InitCampEquipmentRecyle(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	int seq = 0;
	int last_seq = -1;
	int last_color = 0;
	int last_limit_level = 0;
	int last_exp = 0;

	while (!dataElement.empty())
	{
		CampEquipmentRecyleCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq != last_seq + 1) 
		{
			return -1;
		}
		last_seq = seq;
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(dataElement, "limit_level", cfg.limit_level) || cfg.limit_level <= 0) 
		{
			return -2;
		}
		
		if (cfg.limit_level != last_limit_level)
		{
			last_limit_level = cfg.limit_level;
			last_color = 0;
			last_exp = 0;
		}

		if (!PugiGetSubNodeValue(dataElement, "color", cfg.color) || cfg.color != last_color + 1) 
		{
			return -3;
		}
		last_color = cfg.color;

		if (!PugiGetSubNodeValue(dataElement, "experience", cfg.exp) || cfg.exp <= last_exp)
		{
			return -4;
		}
		last_exp =cfg.exp;

		m_equipment_recyle_cfg_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CampConfig::InitCampBeast(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if ( dataElement.empty())
	{
		return -1000;
	}

	//从0级开始
	m_max_beast_level = -1;
	int last_need_exp = -1;

	while (!dataElement.empty())
	{

		CampBeastCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "level", cfg.level) || cfg.level !=  m_max_beast_level + 1) 
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_exp", cfg.need_exp) || cfg.need_exp < last_need_exp) 
		{
			return -2;
		}
		last_need_exp = cfg.need_exp;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) 
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_hp", cfg.max_hp) || cfg.max_hp < 0) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) 
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) 
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) 
		{
			return -7;
		}
			
		m_max_beast_level = cfg.level;

		m_beast_cfg_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const CampNormalDuoBaoLayerCfg *CampConfig::GetNormalDuoBaoLayerCfg(int sceneid)
{
	NormalDuoBaoLayerDic::const_iterator iter = m_normal_duobao_layer_dic.find(sceneid);
	if (m_normal_duobao_layer_dic.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

int CampConfig::GetCampDefendSceneIdByActType(int activity_type)
{
	if (ACTIVITY_TYPE_CAMP_DEFEND1 == activity_type)
	{
		return m_othercfg.dx_sceneid1;
	}

	else if (ACTIVITY_TYPE_CAMP_DEFEND2 == activity_type)
	{
		return m_othercfg.dx_sceneid2;
	}

	else if (ACTIVITY_TYPE_CAMP_DEFEND3 == activity_type)
	{
		return m_othercfg.dx_sceneid3;
	}

	return 0;
}

int CampConfig::GetCampBySceneId(int sceneid)
{
	if (m_othercfg.dx_sceneid1 == sceneid)
	{
		return CAMP_TYPE_FEIXING;
	}

	else if (m_othercfg.dx_sceneid2 == sceneid)
	{
		return CAMP_TYPE_ZHURI;
	}

	else if (m_othercfg.dx_sceneid3 == sceneid)
	{
		return CAMP_TYPE_ZHUIYUE;
	}

	return CAMP_TYPE_INVALID;
}

const CampDiaoXiangPosItemCfg *CampConfig::GetDiaoXiangPosItemCfg(int index)
{
	if (index < 0 || index >= MAX_CAMP_RECORD_RANK3_NUM)
	{
		return NULL;
	}

	return &m_diaoxiang_pos_list[index];
}

int CampConfig::GetTitleId(int camp, int rank)
{
	for (CampTitleVec::const_iterator iter = m_title_vec.begin(); m_title_vec.end() != iter; ++ iter)
	{
		if (iter->camp == camp && iter->rank == rank)
		{
			return iter->titleid;
		}
	}

	return 0;
}

const CampDiaoXiangAttrItemCfg *CampConfig::GetDiaoXiangAttrItemCfg(int world_level)
{
	if (m_diaoxiang_attr_vec.size() <= 0) return NULL;

	int size = static_cast<int>(m_diaoxiang_attr_vec.size());
	for (int i = size - 1; i >= 0; -- i)
	{
		if (world_level >= m_diaoxiang_attr_vec[i].worldlevel)
		{
			return &m_diaoxiang_attr_vec[i];
		}
	}

	return NULL;
}

const GongXunRankRewardCfg * CampConfig::GetGongXunRankRewardCfg(int seq)
{
	if (seq < 0 || seq >= GONG_XUN_RANDK_NUM)
	{
		return NULL;
	}

	return &rand_reward_cfg_list[seq];
}

const GongXunValueRewardCfg * CampConfig::GetGongXunValueRewardCfg(int gongxun_value)
{
	if (gongxun_value <= 0)
	{
		return NULL;
	}

	for(int i = GONG_XUN_VALUE_NUM - 1; i >= 0; -- i)
	{
		if (gongxun_value >= value_reward_cfg_list[i].need_gongxun && value_reward_cfg_list[i].need_gongxun > 0)
		{
			return &value_reward_cfg_list[i];
		}
	}

	return NULL;
}

const HunlianCfg *CampConfig::GetEquipHunlianCfg(const int equip_index, const int level)
{
	if (equip_index < 0 || equip_index >= MAX_CAMP_EQUIP_INDEX || level <= 0 || level > MAX_HUNLIAN_LEVEL)
	{
		return NULL;
	}

	CampEquipHunLianCfg * cfg = &m_hunlian_cfg_list[equip_index];
	if (cfg->equip_index != equip_index)
	{
		return NULL;
	}

	if (level != cfg->level_cfg[level].level)
	{
		return NULL;
	}

	return &cfg->level_cfg[level];
}

const int CampConfig::GetEquipRecyleCfg(const int limit_level, int color)
{
	CampEquipmentRecyleCfgVec::const_iterator iter = m_equipment_recyle_cfg_vec.begin();

	while (iter != m_equipment_recyle_cfg_vec.end())
	{
		if (iter->limit_level == limit_level && iter->color == color)
		{
			return iter->exp;
		}

		++ iter;
	}

	return -1;
}

const CampBeastCfg *CampConfig::GetBeastCfg(const int level)
{
	CampBeastCfgVec::const_iterator iter = m_beast_cfg_vec.begin();
	while (iter != m_beast_cfg_vec.end())
	{
		if (iter->level == level)
		{
			return &(*iter);
		}

		++ iter;
	}

	return NULL;
}