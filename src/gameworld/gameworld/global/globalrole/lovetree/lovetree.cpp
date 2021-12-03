#include "lovetree.hpp"
#include "config/logicconfigmanager.hpp"
#include "lovetreeconfig.hpp"
#include "servercommon/errornum.h"
#include "servercommon/commondata.hpp"
#include "obj/character/role.h"
#include "item/itempool.h"
#include "scene/scene.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/usercache/usercache.hpp"
#include "world.h"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"

LoveTree::LoveTree(GlobalRole *g_role) : IGlobalRoleItem(g_role)
{
}

LoveTree::~LoveTree()
{
}

void LoveTree::Init(const LoveTreeData &love_tree_attr)
{
	m_love_tree_param = love_tree_attr;
}

void LoveTree::GetAttr(LoveTreeData &love_tree_attr)
{
	love_tree_attr = m_love_tree_param;
}

void LoveTree::Update(unsigned int now_second)
{

}

void LoveTree::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{

}

void LoveTree::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc, Role *role)
{
	// 屏蔽爱情树
// 	if (is_recalc)
// 	{
// 		m_attrs_add.Reset();
// 
// 		this->GetTotalAttr(m_attrs_add);
// 
// 		LoveTree * lover_pet_info = GLOBAL_ROLE_MGR.GetLoveTree(this->GetLoverId());
// 		if (NULL != lover_pet_info)
// 		{
// 			lover_pet_info->GetTotalAttr(m_attrs_add, false);
// 		}
// 
// 		role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_LOVE_TREE, m_attrs_add);
// 	}
// 
// 	base_add.Add(m_attrs_add.m_attrs);
}

void LoveTree::OnRoleLogin(Role *role)
{
	this->SetLoverId(role->GetLoverUid(), false);
}

void LoveTree::ReName(GameName new_name)
{
	Role *role = this->GetRole();
	if (NULL == role) return;
	
	const LoveTreeOtherConfig &other_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeOtherCfg();

	if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.rename_cost))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	GameName tmp_new_name; memset(tmp_new_name, 0, sizeof(tmp_new_name));
	F_STRNCPY(tmp_new_name, new_name, sizeof(tmp_new_name));

	InvalidCharFilte(tmp_new_name);

	if (!role->GetKnapsack()->GetMoney()->UseGold(other_cfg.rename_cost, "lovetree_rename"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	F_STRNCPY(m_love_tree_param.tree_name, tmp_new_name, sizeof(GameName));

	this->OnDataChange();

	role->NoticeNum(noticenum::NT_LOVE_TREE_RENAME_SUCC);

	this->SendLoveTreeInfo(role, 0);
}

int LoveTree::GetLoverId()
{
	return m_love_tree_param.lover_id;
}

void LoveTree::GetTotalAttr(CharIntAttrs &tmp_attr, bool is_self)
{
	// 自己树等级属性加成
	if (is_self)
	{
		const LoveTreeLevelConfig * leve_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level);
		if (NULL != leve_cfg)
		{
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += leve_cfg->maxhp;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += leve_cfg->gongji;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += leve_cfg->fangyu;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += leve_cfg->mingzhong;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += leve_cfg->shanbi;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += leve_cfg->baoji;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += leve_cfg->jianren;
		}
	}
	// 伴侣树等级加成
	else
	{
		const LoveTreeLevelConfig * leve_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level);
		if (NULL != leve_cfg)
		{
			const LoveTreeOtherConfig &other_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeOtherCfg();

			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(leve_cfg->maxhp * other_cfg.marry_add_per * 0.01);
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(leve_cfg->gongji * other_cfg.marry_add_per * 0.01);
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(leve_cfg->fangyu * other_cfg.marry_add_per * 0.01);
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(leve_cfg->mingzhong * other_cfg.marry_add_per * 0.01);
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(leve_cfg->shanbi * other_cfg.marry_add_per * 0.01);
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(leve_cfg->baoji * other_cfg.marry_add_per * 0.01);
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(leve_cfg->jianren * other_cfg.marry_add_per * 0.01);
		}
	}
}

void LoveTree::SetLoverId(int lover_id, bool is_calc)
{
	Role * role = this->GetRole();
	if (NULL == role) return;

	if (m_love_tree_param.lover_id != lover_id)
	{
		m_love_tree_param.lover_id = lover_id;
		this->OnDataChange();
	}
	
	if (is_calc)
	{
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);
	}
}

bool LoveTree::AddLoveTreeExp(Role *role, bool is_auto_buy, bool is_water_other)
{
	if (NULL == role) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	const LoveTreeLevelConfig * cur_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level);
	if (NULL == cur_level_cfg)
	{
		return false;
	}

	const LoveTreeLevelConfig * next_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level + 1);
	if (NULL == next_level_cfg)
	{
		role->NoticeNum(errornum::EN_QINGYUAN_TREE_LEVEL_MAX);
		return false;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();
	if (NULL == common_data) return false;

	const LoveTreeOtherConfig &other_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeOtherCfg();

	// 给自己的树浇水
	if (!is_water_other)
	{
		// 给自己浇水免费次数消耗完，才能消耗道具浇水
		if (common_data->water_self_love_tree_time < other_cfg.self_free_water_time)
		{
			m_love_tree_param.love_tree_cur_exp += cur_level_cfg->add_exp;

			int rand_value = RandomNum(10000);
			if (rand_value < cur_level_cfg->baoji_add_exp_rate)
			{
				m_love_tree_param.love_tree_cur_exp += static_cast<int>(cur_level_cfg->add_exp * cur_level_cfg->extra_add_exp_per * 0.01);
			}

			common_data->water_self_love_tree_time++;

			for (int i = 0; m_love_tree_param.love_tree_cur_exp >= cur_level_cfg->need_exp && i < 100; i++)
			{
				m_love_tree_param.love_tree_star_level++;
				m_love_tree_param.love_tree_cur_exp -= cur_level_cfg->need_exp;

				cur_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level);
				if (NULL == cur_level_cfg)
				{
					break;
				}

				next_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level + 1);
				if (NULL == next_level_cfg)
				{
					break;
				}
			}

			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);

			// 伴侣在线，伴侣战斗力也要加
			Role *marry_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetCommonDataParam()->lover_uid));
			if (NULL != marry_role)
			{
				marry_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);
			}
			this->SendLoveTreeInfo(role, true);
			return true;
		}
		else
		{
			static ItemExtern::ItemConsumeList consume_list; 
			consume_list.Reset();

			// 进阶符
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

			if (MALE == role->GetSex())
			{
				stuff_list[0].item_id = cur_level_cfg->male_up_star_item.item_id;
				stuff_list[0].num = cur_level_cfg->male_up_star_item.num;
				stuff_list[0].buyable = is_auto_buy ? true : false;
			}
			else
			{
				stuff_list[0].item_id = cur_level_cfg->female_up_star_item.item_id;
				stuff_list[0].num = cur_level_cfg->female_up_star_item.num;
				stuff_list[0].buyable = is_auto_buy ? true : false;
			}

			// 检查
			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
			{
				return false;
			}

			// 消耗
			if (!ItemExtern::ConsumeItem(role, consume_list, "LoveTree::AddLoveTreeExp"))
			{
				return false;
			}

			{
				// 生效
				m_love_tree_param.love_tree_cur_exp += cur_level_cfg->add_exp;

				int rand_value = RandomNum(10000);
				if (rand_value < cur_level_cfg->baoji_add_exp_rate)
				{
					m_love_tree_param.love_tree_cur_exp += static_cast<int>(cur_level_cfg->add_exp * cur_level_cfg->extra_add_exp_per * 0.01);
				}

				for (int i = 0; m_love_tree_param.love_tree_cur_exp >= cur_level_cfg->need_exp && i < 100; i++)
				{
					m_love_tree_param.love_tree_star_level++;
					m_love_tree_param.love_tree_cur_exp -= cur_level_cfg->need_exp;

					cur_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level);
					if (NULL == cur_level_cfg)
					{
						break;
					}

					next_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level + 1);
					if (NULL == next_level_cfg)
					{
						break;
					}
				}
			}
			
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);

			// 伴侣在线，伴侣战斗力也要加
			Role *marry_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetCommonDataParam()->lover_uid));
			if (NULL != marry_role)
			{
				marry_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);
			}
			this->SendLoveTreeInfo(role, true);
			return true;
		}
	}
	// 给伴侣的树浇水
	else
	{
		// 给伴侣免费浇水次数用完，才能消耗道具浇水
		if (common_data->water_other_love_tree_time < other_cfg.assist_free_water_time)
		{
			m_love_tree_param.love_tree_cur_exp += cur_level_cfg->add_exp;

			int rand_value = RandomNum(10000);
			if (rand_value < cur_level_cfg->baoji_add_exp_rate)
			{
				m_love_tree_param.love_tree_cur_exp += static_cast<int>(cur_level_cfg->add_exp * cur_level_cfg->extra_add_exp_per * 0.01);
			}

			common_data->water_other_love_tree_time++;

			for (int i = 0; m_love_tree_param.love_tree_cur_exp >= cur_level_cfg->need_exp && i < 100; i++)
			{
				m_love_tree_param.love_tree_star_level++;
				m_love_tree_param.love_tree_cur_exp -= cur_level_cfg->need_exp;

				cur_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level);
				if (NULL == cur_level_cfg)
				{
					break;
				}

				next_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level + 1);
				if (NULL == next_level_cfg)
				{
					break;
				}
			}

			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);

			// 伴侣在线，伴侣战斗力也要加
			Role *marry_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetCommonDataParam()->lover_uid));
			if (NULL != marry_role)
			{
				marry_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);
			}
			this->SendLoveTreeInfo(role, false);
			return true;
		}
		else
		{
			static ItemExtern::ItemConsumeList consume_list; 
			consume_list.Reset();

			// 进阶符
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

			if (MALE == role->GetSex())
			{
				stuff_list[0].item_id = cur_level_cfg->female_up_star_item.item_id;
				stuff_list[0].num = cur_level_cfg->female_up_star_item.num;
				stuff_list[0].buyable = is_auto_buy ? true : false;
			}
			else
			{
				stuff_list[0].item_id = cur_level_cfg->male_up_star_item.item_id;
				stuff_list[0].num = cur_level_cfg->male_up_star_item.num;
				stuff_list[0].buyable = is_auto_buy ? true : false;
			}

			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
			{
				return false;
			}

			// 消耗
			if (!ItemExtern::ConsumeItem(role, consume_list, "LoveTree::AddLoveTreeExp"))
			{
				return false;
			}

			{
				// 生效
				m_love_tree_param.love_tree_cur_exp += cur_level_cfg->add_exp;

				int rand_value = RandomNum(10000);
				if (rand_value < cur_level_cfg->baoji_add_exp_rate)
				{
					m_love_tree_param.love_tree_cur_exp += static_cast<int>(cur_level_cfg->add_exp * cur_level_cfg->extra_add_exp_per * 0.01);
				}

				for (int i = 0; m_love_tree_param.love_tree_cur_exp >= cur_level_cfg->need_exp && i < 100; i++)
				{
					m_love_tree_param.love_tree_star_level++;
					m_love_tree_param.love_tree_cur_exp -= cur_level_cfg->need_exp;

					cur_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level);
					if (NULL == cur_level_cfg)
					{
						break;
					}

					next_level_cfg = LOGIC_CONFIG->GetLoveTreeCfg().GetLoveTreeLevelCfg(m_love_tree_param.love_tree_star_level + 1);
					if (NULL == next_level_cfg)
					{
						break;
					}
				}
			}

			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);

			// 伴侣在线，伴侣战斗力也要加
			Role *marry_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetCommonDataParam()->lover_uid));
			if (NULL != marry_role)
			{
				marry_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LOVE_TREE);
			}
			
			this->SendLoveTreeInfo(role, false);
			return true;
		}
	}

	return true;
}

void LoveTree::SendLoveTreeInfo(Role *role, bool is_self)
{
// 	if (NULL == role) return;
// 
// 	Protocol::SCLoveTreeInfo cmd;
// 
// 	cmd.is_self = is_self ? 1 : 0;
// 	cmd.reserve_1 = 0;
// 	cmd.reserve_2 = 0;
// 	cmd.love_tree_star_level = m_love_tree_param.love_tree_star_level;
// 	cmd.love_tree_cur_exp = m_love_tree_param.love_tree_cur_exp;
// 	cmd.other_love_tree_star_level = 0;
// 
// 	if (is_self)
// 	{
// 		LoveTree * lover_love_tree = GLOBAL_ROLE_MGR.GetLoveTree(role->GetLoverUid());
// 		if (NULL != lover_love_tree)
// 		{
// 			cmd.other_love_tree_star_level = lover_love_tree->m_love_tree_param.love_tree_star_level;
// 		}
// 	}
// 	else
// 	{
// 		LoveTree * self_love_tree = GLOBAL_ROLE_MGR.GetLoveTree(role->GetUID());
// 		if (NULL != self_love_tree)
// 		{
// 			cmd.other_love_tree_star_level = self_love_tree->m_love_tree_param.love_tree_star_level;
// 		}
// 	}
// 
// 	CommonDataParam *common = role->GetCommonDataParam();
// 	cmd.free_water_self = common->water_self_love_tree_time;
// 	cmd.free_water_other = common->water_other_love_tree_time;
// 
// 	F_STRNCPY(cmd.tree_name, m_love_tree_param.tree_name, sizeof(GameName));
// 
// 	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cmd, sizeof(cmd));
}


