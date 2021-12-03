#include "lifeskill.hpp"

#include "obj/character/role.h"
#include "gameworld/gamelog.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "lifeskillconfig.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "item/itempool.h"
#include "item/itemgriddata.h"
#include "item/itembase.h"
#include "globalconfig/globalconfig.h"
#include "equipment/equipmentmanager.h"
#include "item/knapsack.h"
#include "protocal/msgrole.h"

LifeSkillMgr::LifeSkillMgr()
	: m_role(NULL)
{
}

LifeSkillMgr::~LifeSkillMgr()
{

}

void LifeSkillMgr::Init(Role *role, const LifeSkillParam &p)
{
	m_role = role;
	
	m_skill_p = p;
}

void LifeSkillMgr::GetInitParam(LifeSkillParam *p)
{
	*p = m_skill_p;
}

void LifeSkillMgr::UpLevelSkill(int skill_type)
{
	if (skill_type < 0 || skill_type >= LIFE_SKIL_TYPE_MAX)
	{
		return;
	}

	LifeSkill *life_skill = &m_skill_p.life_skill_list[skill_type];

	if (life_skill->skill_lv >= LIFE_SKILL_LV_MAX)
	{
		m_role->NoticeNum(errornum::EN_LIFESKILL_MAX_SKLL_LEVEL);
		return;
	}

	const LifeSkillItemCfg *item_cfg = LOGIC_CONFIG->GetLifeSkillConfg().GetSkillItemCfg(skill_type);
	if (NULL == item_cfg)
	{
		return;
	}

	const int need_huoli = item_cfg->need_huoli_list[life_skill->skill_lv + 1];
	if (!m_role->HuoliMoreThan(need_huoli))
	{
		m_role->NoticeNum(errornum::EN_LIFESKILL_HUOLI_NOT_ENOUGH);
		return;
	}

	m_role->UseHuoli(need_huoli, "UpLevelLifeSkill");
	++ life_skill->skill_lv;

	this->SyncLifeSkillInfo();

	m_role->NoticeNum(noticenum::NT_OPERATE_SUCC);
}

void LifeSkillMgr::Make(int index)
{
	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

 	const LifeSkillMakeItemCfg *make_item_cfg = LOGIC_CONFIG->GetLifeSkillConfg().GetMakeCfg(index);
 	if (NULL == make_item_cfg)
 	{
 		return;
 	}
 
 	if (make_item_cfg->type < 0 || make_item_cfg->type >= LIFE_SKIL_TYPE_MAX)
 	{
 		return;
 	}
 
 	LifeSkill *life_skill = &m_skill_p.life_skill_list[make_item_cfg->type];
 	
 	if (life_skill->skill_lv < make_item_cfg->need_skill_level)
 	{
 		m_role->NoticeNum(errornum::EN_LIFESKILL_LEVEL_LIMIT);
 		return;
 	}
 
 	if (!m_role->HuoliMoreThan(make_item_cfg->need_huoli))
 	{
 		m_role->NoticeNum(errornum::EN_LIFESKILL_HUOLI_NOT_ENOUGH);
 		return;
 	}
 
	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

 	//检查材料
 	{
 		short stuff_count = 0;
 		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
 		
 		if (make_item_cfg->stuff1 > 0 && make_item_cfg->count1 > 0)
 		{
 			stuff_list[stuff_count].item_id = make_item_cfg->stuff1;
 			stuff_list[stuff_count].num = make_item_cfg->count1;
 			stuff_list[stuff_count].buyable = false;
 			++ stuff_count;
 		}

		if (make_item_cfg->stuff2 > 0 && make_item_cfg->count2 > 0)
		{
			stuff_list[stuff_count].item_id = make_item_cfg->stuff2;
			stuff_list[stuff_count].num = make_item_cfg->count2;
			stuff_list[stuff_count].buyable = false;
			++ stuff_count;
		}

 		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list))
 		{
 			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
 			return;
 		}
 	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "LifeSkillMake"))
	{
		return;
	}

	m_role->UseHuoli(make_item_cfg->need_huoli, "LifeSkillMake");
	
	// 熟练度
	{
		int real_add_shuliandu = 0;
		if (make_item_cfg->shuliandu_xisu >= life_skill->shuliandu_lv)
		{
			real_add_shuliandu = make_item_cfg->add_shuliandu;
		}
		else
		{
			real_add_shuliandu = static_cast<int>(make_item_cfg->add_shuliandu * (0.9f - 0.2f * (life_skill->shuliandu_lv - make_item_cfg->shuliandu_xisu)));
			if (real_add_shuliandu < 0)
			{
				real_add_shuliandu = 0;
			}
		}

		this->AddShuliandu(make_item_cfg->type, life_skill, real_add_shuliandu);
	}

	// 制作出物品
	{
		ItemID item_id = 0;
		int cur_total_sld = LOGIC_CONFIG->GetLifeSkillConfg().GetTotalShuliandu(make_item_cfg->type, life_skill->shuliandu_lv) + life_skill->shuliandu;
		int total_sld = LOGIC_CONFIG->GetLifeSkillConfg().GetTotalShuliandu(make_item_cfg->type, LIFE_SKILL_SHULIANDU_LV_MAX);
		
		int is_make_precious = 0;
		LOGIC_CONFIG->GetLifeSkillConfg().GetMakeItemId(make_item_cfg, &item_id, m_role->GetProf(), cur_total_sld, total_sld, &is_make_precious);

		if (item_id <= 0)
		{
			this->SyncLifeSkillInfo();
			m_role->NoticeNum(errornum::EN_LIFESKILL_MAKE_FAIL);
			return;
		}

		ItemConfigData item_config_data(item_id, false, 1);

		int grid_index = -1;
		bool ret = m_role->GetKnapsack()->Put(item_config_data, PUT_REASON_LIFE_SKILL_MAKE, &grid_index);

		// 为装备类添加仙品和幸运属性
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(item_id);
			if (NULL != item_base && ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
			{
				if(ret && grid_index >= 0)
				{
					ItemGridData item_grid_data;

					bool succ = m_role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data);

					if (succ && EquipmentManager::GetRandomSpeicalAttr(&item_grid_data))												// 仙品
					{
						m_role->GetKnapsack()->SetItemDirty(grid_index);
					}

					if (is_make_precious && succ && RandomNum(100) <= make_item_cfg->lucky_rate && GLOBALCONFIG->GetEquipConfig().GetLucky(&item_grid_data)) // 幸运属性
					{
						m_role->GetKnapsack()->SetItemDirty(grid_index);
					}
				}
			}
		}
	}

	this->SyncLifeSkillInfo();
	m_role->NoticeNum(noticenum::NT_LIFESKILL_MAKE_SUCC);
}

void LifeSkillMgr::AddShuliandu(int skill_type, LifeSkill *life_skill, int add_shuliandu)
{
	if (NULL == life_skill || add_shuliandu <= 0) return;

	const LifeSkillShulianduItemCfg *item_cfg = LOGIC_CONFIG->GetLifeSkillConfg().GetShulianduCfg(skill_type);
	if (NULL == item_cfg)
	{
		return;
	}

	int round_count = 0; 
	while (add_shuliandu > 0 && round_count ++ < 100)
	{
		if (life_skill->shuliandu_lv >= LIFE_SKILL_SHULIANDU_LV_MAX)
		{
			break;
		}

		int need_shuliandu = item_cfg->need_shuliandu[life_skill->shuliandu_lv + 1];
		
		if (life_skill->shuliandu + add_shuliandu >= need_shuliandu)
		{
			add_shuliandu -= (need_shuliandu - life_skill->shuliandu);

			life_skill->shuliandu_lv += 1;
			life_skill->shuliandu = 0;
		}
		else
		{
			life_skill->shuliandu += add_shuliandu;
			break;
		}
	}
}

void LifeSkillMgr::SyncLifeSkillInfo()
{
	Protocol::SCLifeSkillInfo cmd;

	UNSTD_STATIC_CHECK(sizeof(cmd.skill_param) == sizeof(m_skill_p));
	memcpy(&cmd.skill_param, &m_skill_p, sizeof(cmd.skill_param));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}