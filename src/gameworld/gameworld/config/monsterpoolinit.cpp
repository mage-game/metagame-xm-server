#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "monster/monsterpool.h"
#include "monster/monsterinitparam.h"
#include "skill/skillpool.hpp"
#include "monster/drop/droppool.hpp"
#include "gameworld/checkresourcecenter.hpp"

bool MonsterInitParam::Init(const std::string &configname, std::string &err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "MonsterInitParam::Init", err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		err = configname + ": xml node error in root";
		return false;
	}

	// ¿ªÊ¼¼ÓÔØ
	if (!PugiGetSubNodeValue(RootElement, "type", monster_type) || monster_type < MONSTER_TYPE_NORMAL || monster_type >= MONSTER_TYPE_MAX)
	{
		err = configname + ": xml node error in node [monsters->monster->monster_type] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "id", monster_id) || monster_id <= 0)
	{
		err = configname + ": xml node error in node [monsters->monster->id] ";
		return false;
	}

	std::string tmpname;
	if (!PugiGetSubNodeValue(RootElement, "name", tmpname))
	{
		err = configname + ": xml node error in node [monsters->monster->name] ";
		return false;
	}
	if (tmpname == "" || tmpname.length() >= sizeof(monster_name))
	{
		err = configname + ": xml node error in node [monsters->monster->name]: length error";
		return false;
	}
	STRNCPY(monster_name, tmpname.c_str(), sizeof(monster_name));

	if (!PugiGetSubNodeValue(RootElement, "monsterai", ai_type))
	{
		err = configname + ": xml node error in node [monsters->monster->monsterai] ";
		return false;
	}
	if(AITYPE_FOOL != ai_type && AITYPE_STATIC != ai_type && AITYPE_PASSIVE != ai_type && AITYPE_INITIATIVE != ai_type && 
		AITYPE_STAT_INITIAT != ai_type && AITYPE_JUST_RUN != ai_type && AITYPE_JUST_FLEE != ai_type && 
		AITYPE_SPECIALAI_BATTLEFIELD != ai_type && AITYPE_SPECIALAI_GONGCHENGZHAN != ai_type && 
		AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK != ai_type &&
		AITYPE_CISHA_CAMP1 != ai_type && AITYPE_CISHA_CAMP2 != ai_type && AITYPE_CISHA_CAMP3 != ai_type && 
		AITYPE_GUILDFB_GUARD != ai_type && AITYPE_GUILDFB_ENEMY != ai_type && AITYPE_COIN != ai_type && AITYPE_CROSS_PASTURE_ANIMAL != ai_type &&
		AITYPE_SPECIALAI_TERRITORYWAR_TOWER != ai_type && AITYPE_STATIC_ADD_MIANSHANG_BUFF != ai_type && AITYPE_STATCI_ADD_RECOVER_BUFF != ai_type&&
		AITYPE_BUILD_TOWER_RUNNING != ai_type && AITYPE_BUILD_TOWER_TOWER != ai_type &&
		AITYPE_SPECIALAI_GIFT_HARVEST != ai_type && AITYPE_SPECIALAI_TOWERDEFEND_TEAM_SHOUHU != ai_type)
	{
		err = configname + ": xml node error in node [monsters->monster->monsterai] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "colddown", cold_down) || cold_down <= 0)
	{
		err = configname + ": xml node error in node [monsters->monster->colddown] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "capability", capability) || capability < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->capability] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "hp", hp) || hp <= 0)
	{
		err = configname + ": xml node error in node [monsters->monster->hp] ";
		return false;
	}
	if (!PugiGetSubNodeValue(RootElement, "boss_type", boss_type) || boss_type <= BOSS_TYPE_INVALID || boss_type >= BOSS_TYPE_MAX)
	{
		err = configname + ": xml node error in node [monsters->monster->boss_type] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "gongji", gongji) || gongji < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->gongji] ";
		return false;
	}
	if (!PugiGetSubNodeValue(RootElement, "fangyu", fangyu) || fangyu < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->fangyu] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "mingzhong", mingzhong) || mingzhong < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->mingzhong] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "shanbi", shanbi) || shanbi < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->shanbi] ";
		return false;
	}
	if (!PugiGetSubNodeValue(RootElement, "baoji", baoji) || baoji < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->baoji] ";
		return false;
	}
	if (!PugiGetSubNodeValue(RootElement, "jianren", jianren) || jianren < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->jianren] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "per_jingzhun", per_jingzhun) || per_jingzhun < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->per_jingzhun] ";
		return false;
	}
	if (!PugiGetSubNodeValue(RootElement, "per_baoji", per_baoji) || per_baoji < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->per_baoji] ";
		return false;
	}
	if (!PugiGetSubNodeValue(RootElement, "per_kangbao", per_kangbao) || per_kangbao < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->per_kangbao] ";
		return false;
	}
	if (!PugiGetSubNodeValue(RootElement, "per_pofang", per_pofang) || per_pofang < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->per_pofang] ";
		return false;
	}
	if (!PugiGetSubNodeValue(RootElement, "per_mianshang", per_mianshang) || per_mianshang < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->per_mianshang] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "level", level) || level <= 0)
	{
		err = configname + ": xml node error in node [monsters->monster->level] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "speed", move_speed) || (0 != move_speed && move_speed < 150))
	{
		err = configname + ": xml node error in node [monsters->monster->speed] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "skillid", skillid))
	{
		err = configname + ": xml node error in node [monsters->monster->skillid] ";
		return false;
	}
	if (0 != skillid && NULL == SKILLPOOL->GetSkill(skillid))
	{
		sprintf(errinfo, "%s xml node error in node [monsters->monster->skillid not exist :%d]", configname.c_str(), skillid);
		err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "boss_skill_cond_id", boss_skill_cond_id))
	{
		err = configname + ": xml node error in node [monsters->monster->boss_skill_cond_id] ";
		return false;
	}
	if (0 != boss_skill_cond_id)
	{
		CheckResourceCenter::Instance().GetBossCondSkillCheck()->Add(monster_id, boss_skill_cond_id);
	}

	if (MONSTER_TYPE_BOSS == monster_type && 0 == boss_skill_cond_id)
	{
		err = configname + ": xml node error in node [monsters->monster->boss_skill_cond_id == 0 but is boss] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "run_range", run_range) || run_range < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->run_range] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "aoi_range", aoi_range) || aoi_range < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->aoi_range] ";
		return false;
	}

	if (aoi_range > run_range)
	{
		err = configname + ": xml node error in node [monsters->monster->aoi_range], run_range must be larger then aoi_rang!";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "drop_type", drop_type) || drop_type < BOSS_DROP_OWN_TYPE_NONE || drop_type >= BOSS_DROP_OWN_TYPE_MAX)
	{
		err = configname + ": xml node error in node [monsters->monster->drop_type] ";
		return false;
	}

	{
		PugiXmlNode dropidListElement = RootElement.child("dropid_list");
		if (dropidListElement.empty())
		{
			err = configname + ": xml node error in node [monsters->monster->dropid_list NULL]";
			return false;
		}
		if (!this->ReadDropIDList(dropidListElement, dropid_list))
		{
			err = configname + ": xml node error in node [monsters->monster->dropid_list ReadDropIDList FAIL]";
			return false;
		}
	}

	{
		PugiXmlNode firsthitdropidListElement = RootElement.child("firsthit_dropid_list");
		if (firsthitdropidListElement.empty())
		{
			err = configname + ": xml node error in node [monsters->monster->firsthit_dropid_list NULL]";
			return false;
		}
		if (!this->ReadDropIDList(firsthitdropidListElement, firsthit_dropid_list))
		{
			err = configname + ": xml node error in node [monsters->monster->firsthit_dropid_list ReadDropIDList FAIL]";
			return false;
		}
	}

// 	{
// 		PugiXmlNode dpsdropidListElement = RootElement.child("dps_dropid_list");
// 		if (dpsdropidListElement.empty())
// 		{
// 			err = configname + ": xml node error in node [monsters->monster->dps_dropid_list NULL]";
// 			return false;
// 		}
// 		if (!this->ReadDropIDList(dpsdropidListElement, dps_dropid_list))
// 		{
// 			err = configname + ": xml node error in node [monsters->monster->dps_dropid_list ReadDropIDList FAIL]";
// 			return false;
// 		}
// 	}

	{
		PugiXmlNode randdropidListElement = RootElement.child("rand_dropid_list");
		if (randdropidListElement.empty())
		{
			err = configname + ": xml node error in node [monsters->monster->rand_dropid_list NULL]";
			return false;
		}
		if (!this->ReadDropIDList(randdropidListElement, rand_dropid_list))
		{
			err = configname + ": xml node error in node [monsters->monster->rand_dropid_list ReadDropIDList FAIL]";
			return false;
		}
	}
	

	{
		PugiXmlNode taskdropidListElement = RootElement.child("task_dropid_list");
		if (taskdropidListElement.empty())
		{
			err = configname + ": xml node error in node [monsters->monster->task_dropid_list NULL]";
			return false;
		}
		if (!this->ReadDropIDList(taskdropidListElement, task_dropid_list))
		{
			err = configname + ": xml node error in node [monsters->monster->task_dropid_list ReadDropIDList FAIL]";
			return false;
		}
	}

	{
		if (!PugiGetSubNodeValue(RootElement, "money_drop_count", drop_money_count))
		{
			err = configname + ": xml node error in node [monsters->monster->money_drop_count]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "money_drop_prob", drop_money_prob))
		{
			err = configname + ": xml node error in node [monsters->monster->money_drop_prob]";
			return false;
		}
	}

	if (!PugiGetSubNodeValue(RootElement, "exp_reward", exp_reward) || exp_reward < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->exp_reward] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "yuanli_reward", yuanli_reward) || yuanli_reward < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->yuanli_reward] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "disappear_time", disappear_time) || disappear_time < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->disappear_time] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "die_notice", die_notice))
	{
		err = configname + ": xml node error in node [monsters->monster->die_notice] ";
		return false;
	}
	
	if (!PugiGetSubNodeValue(RootElement, "dead_notice", dead_notice))
	{
		err = configname + ": xml node error in node [monsters->monster->dead_notice] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "forbid_capture_by_skill", forbid_capture_by_skill))
	{
		err = configname + ": xml node error in node [monsters->monster->forbid_capture_by_skill] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "is_fix_hurt_on_attack", is_fix_hurt_on_attack))
	{
		err = configname + ": xml node error in node [monsters->monster->is_fix_hurt_on_attack] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "is_forbid_recover", is_forbid_recover))
	{
		err = configname + ": xml node error in node [monsters->monster->is_forbid_recover] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "flush_notice", flush_notice))
	{
		err = configname + ": xml node error in node [monsters->monster->flush_notice] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "walk_point_num", walk_point_num) || walk_point_num < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->walk_point_num] ";
		return false;
	}
 
	if (!PugiGetSubNodeValue(RootElement, "walk_range", walk_range) || walk_range < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->walk_range] ";
		return false;
	}

	if (walk_range > aoi_range)
	{
		err = configname + ": xml node error in node [monsters->monster->walk_range], aoi_rang must be larger then walk_range!";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "auto_reover_per", auto_reover_per) || auto_reover_per < 0 || auto_reover_per > 10000)
	{
		err = configname + ": xml node error in node [monsters->monster->auto_reover_per] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "auto_reover_time_gap", auto_reover_time_gap) || auto_reover_time_gap < 0)
	{
		err = configname + ": xml node error in node [monsters->monster->auto_reover_time_gap] ";
		return false;
	}

	return true;
}

bool MonsterInitParam::ReadDropIDList(PugiXmlNode dropListElement, std::vector<UInt16> &dropid_list)
{
	PugiXmlNode dropidElement = dropListElement.child("dropid");
	if (dropidElement.empty()) return false;
	
	while (!dropidElement.empty())
	{
		UInt16 dropid = 0;
		if (!PugiGetNodeValue(dropidElement, dropid)) return false;

		if (0 != dropid) 
		{
			if (NULL == DROPPOOL->GetDropConfig(dropid)) return false;

			dropid_list.push_back(dropid);
			if ((int)dropid_list.size() >= DROP_LIST_MAX_COUNT) return false;
		}

		dropidElement = dropidElement.next_sibling();
	}

	return true;
}

