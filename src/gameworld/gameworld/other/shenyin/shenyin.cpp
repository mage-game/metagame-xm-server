#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "shenyin.hpp"
#include "netmsg/protocol/protocol_110.hpp"
#include "shenyinconfig.hpp"

#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "scene/scene.h"
#include "other/capability/capability.hpp"
#include "item/knapsack.h"
#include "protocal/msgother.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "protocal/msgrole.h"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "task/taskrecorder.h"
#include "item/itempool.h"
#include "config/rolebigsmallgoalconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

#include <algorithm>

void ShenYinVirtualBag::SendBagInfoByIndex(int index) const
{
	INDEX_LIST grid_index_set;
	grid_index_set.push_back(index);
	this->OnBagChange(&grid_index_set);
}

void ShenYinVirtualBag::SendBagAllInfo(INDEX_LIST *index_list) const
{
	static Protocol::SCShenYinBagInfo psibi;
	psibi.type = (char)Protocol::SCShenYinBagInfo::BAG;
	psibi.count = 0;
	psibi.imprint_grade = 0;

	if (NULL == index_list)
	{
		for (int index = 0; index < this->GetBagSize(); ++index)
		{
			const ShenYinPastureSpiritGridParam *grid = this->GetGridPrt(index);
			if (NULL == grid)
			{
				continue;
			}

			Protocol::ShenYinGridParamMsg &msg_grid = psibi.grid_list[(int)psibi.count];
			msg_grid.param1 = index;
			msg_grid.v_item_id = grid->v_item_id;
			msg_grid.item_num = grid->num;
			msg_grid.is_bind = grid->is_bind;
			msg_grid.level = 0;
			msg_grid.grade = 0;
			msg_grid.attr_param = grid->attr_param;
			msg_grid.new_attr_param = grid->attr_param;

			++psibi.count;
		}
	}
	else
	{
		for (int i = 0; i < this->GetBagSize(); ++i)
		{
			const ShenYinPastureSpiritGridParam *grid = this->GetGridPrt(i);
			if (NULL == grid)
			{
				continue;
			}

			Protocol::ShenYinGridParamMsg &msg_grid = psibi.grid_list[(int)psibi.count];
			msg_grid.param1 = i;
			msg_grid.v_item_id = grid->v_item_id;
			msg_grid.item_num = grid->num;
			msg_grid.is_bind = grid->is_bind;
			msg_grid.level = 0;
			msg_grid.grade = 0;
			msg_grid.attr_param = grid->attr_param;
			msg_grid.new_attr_param = grid->attr_param;
			++psibi.count;
		}
	}

	int length = sizeof(psibi) - sizeof(psibi.grid_list[0]) * (SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT - psibi.count);
	EngineAdapter::Instance().NetSend(this->GetRole()->GetNetId(), (const char*)&psibi, length);
}

void ShenYinVirtualBag::OnBagChange(INDEX_LIST *index_list) const
{
	this->SendBagAllInfo(index_list);
}

void ShenYinVirtualBag::PrintLogPut(int virtual_item_id, int num, bool is_bind, const char *reason) const
{
	VIRTUAL_BAG_LOG_PUT(g_log_tianxiang)
}

void ShenYinVirtualBag::PrintLogConsume(int virtual_item_id, int num, const char *reason) const
{
	VIRTUAL_BAG_LOG_CONSUME(g_log_tianxiang)
}

int ShenYinVirtualBag::GetStackLimitByVid(short virtual_item_id) const
{
	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(virtual_item_id);
	if (NULL == item_cfg)
	{
		return 0;
	}

	return item_cfg->stack_num;
}

ShenYin::ShenYin() : m_role(NULL), m_bead_count(0), m_liehun_pool_hunshou_count(0) 
{
	memset(m_quality_equip_count, 0, sizeof(m_quality_equip_count));
}

ShenYin::~ShenYin()
{
}

void ShenYin::InitParam(Role *role, const ShenYinParam &param)
{
	m_role = role;
	m_param = param;
	//m_virtual_bag.SetRole(role);
}

void ShenYin::GetInitParam(ShenYinParam *param) const
{
	*param = m_param;
}

void ShenYin::Update(unsigned int now_second)
{
	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT; ++ i)
	{
		char &buy_count = m_param.shop_buy_count_list[i];
		unsigned int &timestamp = m_param.shop_buy_count_flush_timestamp_list[i];
		const ShenYinPastureSpiritImprintExchangeCfg *exchange_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintExchange(i);
		if (timestamp > 0 && now_second >= timestamp && NULL != exchange_cfg)
		{
			buy_count = 0;
			if (0 == exchange_cfg->is_week_refresh)
			{
				timestamp = static_cast<unsigned int>(GetZeroTime(now_second + exchange_cfg->day * SECOND_PER_DAY));
			}
			else
			{
				timestamp = static_cast<unsigned int>(now_second + EngineAdapter::Instance().NextWeekInterval(exchange_cfg->day, 0, 0, 0));
			}

			this->SendShopInfo(&i);
		}
	}
}

void ShenYin::OnRoleLogin()
{
	// 读取组合
	BeadPosi posi;
	for (int y = 0; y < SHEN_YIN_TABEL_ROW_COUNT; ++y)
	{
		for (int x = 0; x < SHEN_YIN_TABEL_MIDDLE_GRIDS; ++x)
		{
			this->CheckCombine(x, y);

			if (!m_param.table_lsit[x][y].IsEmpty())
			{
				++m_bead_count;
			}
		}
	}

	for (int i = 0; i < SHEN_YIN_LIEHUN_POOL_MAX_COUNT; ++i)
	{
		if (0 == m_param.chouhun_pool[i])
		{
			break;
		}

		++m_liehun_pool_hunshou_count;
	}

	//for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT; ++i)
	//{
	//	this->GetRandAttr(i);
	//}

	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT; ++i)
	{
		unsigned int &timestamp = m_param.shop_buy_count_flush_timestamp_list[i];
		const ShenYinPastureSpiritImprintExchangeCfg *exchange_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintExchange(i);
		if (NULL != exchange_cfg && timestamp <= 0)
		{
			if (0 == exchange_cfg->is_week_refresh)
			{
				timestamp = static_cast<unsigned int>(GetZeroTime(EngineAdapter::Instance().Time() + exchange_cfg->day * SECOND_PER_DAY));
			}
			else
			{
				timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time() + EngineAdapter::Instance().NextWeekInterval(exchange_cfg->day, 0, 0, 0));
			}
		}
	}

	this->CheckFuncOpen();
	this->ReCalcSuitCount();
}

void ShenYin::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		this->ReCalcSuitCount();	//重算套装

		m_attrs_add.Reset();
	
		CharIntAttrs base_total_tabel_attr;			// 珠子基础总属性
		CharIntAttrs base_total_imprint_attr;		// 印记基础总属性

		// 基础属性
		for (int y = 0; y < SHEN_YIN_TABEL_ROW_COUNT; ++ y)
		{
			for (int x = 0; x < SHEN_YIN_TABEL_MIDDLE_GRIDS; ++ x)
			{
				const ShenYinTableGridParam &tmp_bead = m_param.table_lsit[x][y];
				if (tmp_bead.IsEmpty() || !this->isValidPosi(x, y))
				{
					continue;
				}

				const ShenYinBeadCfg *bead_cfg = LOGIC_CONFIG->GetShenYinConfig().GetBeadCfg(tmp_bead.type);
				if (NULL != bead_cfg)
				{
					base_total_tabel_attr.AddAttrFromConfig(bead_cfg->attr);
				}
			}
		}

		m_attrs_add.Add(base_total_tabel_attr.m_attrs);

		// 组合
		std::map<int, BeadPosi>::iterator it_map = m_combine_map.begin();
		for (;it_map != m_combine_map.end(); ++ it_map)
		{
			const ShenYinCombineAttrCfg *combine_attr_cfg = LOGIC_CONFIG->GetShenYinConfig().GetCombineAttrCfg(it_map->first);
			if (NULL != combine_attr_cfg)
			{
				m_attrs_add.AddAttrFromConfig(combine_attr_cfg->attr);
			}
		}

		std::map<int, int> suit_map;
		for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT; ++ i)
		{
			ShenYinPastureSpiritGridParam &slot = m_param.imprint_slot_list[i];
			if (slot.IsEmpty())
			{
				continue;
			}

			const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(slot.v_item_id);
			if (NULL == item_cfg)
			{
				continue;
			}

			//base
			static  AttrConfig tmp_attr; tmp_attr.Reset();
			const ShenYinPastureSpiritImprintBaseAttrCfg::SlotParam *base_attr = LOGIC_CONFIG->GetShenYinConfig().GetImprintBaseAttr(item_cfg->quanlity, item_cfg->imprint_slot);
			if (NULL != base_attr)
			{
				tmp_attr.Add(base_attr->attr, 1);
				base_total_imprint_attr.AddAttrFromConfig(tmp_attr);
			}

			//附加
			for (int attr_type = 0; attr_type < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT - 1; ++ attr_type)
			{
				int &type = slot.attr_param.type_list[attr_type];
				int &value = slot.attr_param.value_list[attr_type];
				if (type < 0)
				{
					break;
				}
				tmp_attr.AddTranstAttr(type, value);
			}

			//升级百分比
			const ShenYinPastureSpiritImprintUpStartCfg *level_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintUpLevel(i, slot.grade, slot.param1);
			if (NULL != level_cfg)
			{
				m_attrs_add.AddAttrFromConfig(level_cfg->attr_cfg);
				tmp_attr.Add(tmp_attr, level_cfg->basics_addition * ROLE_ATTR_PER);
			}

			const int combine_count = LOGIC_CONFIG->GetShenYinConfig().GetShenYinCombineAttrAddCount();
			for (int i = 0; i < combine_count; ++ i)
			{
				const ShenYinCombineAttrAddCfg *combine_cfg = LOGIC_CONFIG->GetShenYinConfig().GetShenYinCombineAttrAddCfg(i);
				if (NULL != combine_cfg)
				{
					std::map<int, BeadPosi>::iterator combine_first = m_combine_map.find(combine_cfg->combine_seq_1);
					std::map<int, BeadPosi>::iterator combine_second = m_combine_map.find(combine_cfg->combine_seq_2);
					if (combine_first != m_combine_map.end() && combine_second != m_combine_map.end())
					{
						tmp_attr.Add(tmp_attr, combine_cfg->attr_add * ROLE_ATTR_PER);
					}
				}
			}

			m_attrs_add.AddAttrFromConfig(tmp_attr);

			//屏蔽旧的铭纹套装
			//suit
			//++suit_map[item_cfg->suit_id];
			//const ShenYinPastureSpiritImprintSuitCfg::SuitAttr *suit_cfg = LOGIC_CONFIG->GetShenYinConfig().GetSuitAttr(item_cfg->suit_id, suit_map[item_cfg->suit_id]);
			//if (NULL != suit_cfg)
			//{
			//	m_attrs_add.AddAttrFromConfig(suit_cfg->attr);
			//}
		}

		// 激活大目标
		{
			int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN);
			if (add_value > 0)
			{
				for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
				{
					if (base_total_imprint_attr.m_attrs[i] > 0)
					{
						//m_attrs_add.m_attrs[i] += static_cast<Attribute>(round(base_total_imprint_attr.m_attrs[i] * (add_value * ROLE_ATTR_PER)));
						m_attrs_add.AddValue(i, static_cast<Attribute>(round(base_total_imprint_attr.m_attrs[i] * (add_value * ROLE_ATTR_PER))));
					}
				}
			}
		}

		// (新)铭纹套装属性
		{
			for (int i = 0; i < SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK + 1; i++)
			{
				int quality_equip_count = this->GetSuitEquipCount(i);
				const ShenYinNewSuitAttrCfg *suit_cfg = LOGIC_CONFIG->GetShenYinConfig().GetSuitCfg(i, quality_equip_count);
				if (suit_cfg != NULL)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += suit_cfg->jianren;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_cfg->shanbi;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] += suit_cfg->skill_jianshang_per;

					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(suit_cfg->per_gongji * ROLE_ATTR_PER);	//套装全身攻击加成
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(suit_cfg->per_maxhp * ROLE_ATTR_PER);		//套装全身气血加成
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(suit_cfg->per_fangyu * ROLE_ATTR_PER);	//套装全身防御加成
				}
			}
		}

		// 人物大目标固定属性
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN, m_attrs_add);
		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHEN_YIN, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void ShenYin::ReqOpera(int info_type, int param1, int param2, int param3, int param4)
{
	const ShenYinOtherCfg &other_cfg = LOGIC_CONFIG->GetShenYinConfig().GetOtherCfg();
	if (m_role->GetLevel() < other_cfg.limit_level)
	{
		return;
	}

	switch (info_type)
	{
	case Protocol::CS_TIAN_XIANG_REQ_TYPE_ALL_INFO:
		{
			this->ReCalcSuitCount();
			this->SendAllInfo();
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_CHANGE_BEAD_TYPE:
		{
			this->ChangeBeadType(param1, param2, param3);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_CHANGE_BEAD:
		{
			this->ChangeGrid(param1, param2, param3, param4);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_UP_START:
		{
			bool is_use_protect = 0 != param2;
			bool is_auto_buy = 0 != param3;
			this->ShenYinUpLevel(param1, is_use_protect, is_auto_buy);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_UP_LEVEL:
		{
			/*this->ImprintUpGrade();*/
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_EQUIT:
		{
			//this->ShenYinEquip(param1, param2);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_TAKE_OFF:
		{
			//this->ShenYinTakeOff(param1);
			this->TakeOffToKnapsack(param1,false);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_ADD_ATTR_COUNT:
		{
			this->ShenYinAddAttrCount(param1);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_FLUSH_ATTR_TYPE:
		{
			this->ShenYinFlushAttrType(param1, param2);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_FLUSH_ATTR_VALUE:
		{
			this->ShenYinFlushAttrValue(param1, param2);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_APLY_FLUSH:
		{
			this->ShenYinAplyFlushResult(param1);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_RECYCLE:
		{
			//this->ShenYinRecycle(param1, param2);
			this->NewShenyinRecycle(param1, param2);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_IMPRINT_EXCHANGE:
		{
			//this->ShenYinExchange(param1);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_SORT:
		{
			//this->SortVirtualBag();
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_CHOUHUN:
		{
			bool is_use_score = 0 != param1;
			this->ShenYinChouhun(true, Protocol::CS_TIAN_XIANG_REQ_TYPE_CHOUHUN, is_use_score);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_SUPER_CHOUHUN:
		{
			this->ShenYinSuperChouhun();
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_BATCH_HUNSHOU:
		{
			bool is_use_score = 0 != param1;
			this->ShenYinBatchChouhun(is_use_score);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_PUT_BAG:
		{
			this->ShenYinPutInBagByChouHunPool(param1);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_CONVERT_TO_EXP:
		{
			this->ShenYinChouhunPoolConverToScore();
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_SINGLE_CONVERT_TO_EXP:
		{
			this->ShenyinChouhunPoolSingleItemConverToScore(param1);
		}
		break;

	case Protocol::CS_TIAN_XIANG_REQ_TYPE_PUT_BAG_ONE_KEY:
		{
			this->ShenYinOnekeyPutInBagByChouHunPool();
		}
		break;

	default:
		break;
	}
}

// void ShenYin::OneKeyRecycleOpera(const Protocol::CSShenYinOneKeyRecyleReq *cmd)
// {
// 	if (NULL == cmd) return;
// 
// 	int total_add_score = 0;
// 	for (int i = 0; i < cmd->count && i < SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT; ++i)
// 	{
// 		ShenYinPastureSpiritGridParam *grid = m_virtual_bag.GetGridPrt(cmd->virtual_bag_list[i].virtual_bag_index);
// 		if (NULL == grid || grid->IsEmpty())
// 		{
// 			break;
// 		}
// 
// 		const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(grid->v_item_id);
// 		if (NULL == item_cfg || item_cfg->item_type == (int)ShenYinPastureSpiritItemCfg::ITEM)
// 		{
// 			break;
// 		}
// 
// 		if (grid->num < cmd->virtual_bag_list[i].num)
// 		{
// 			break;
// 		}
// 
// 		const ShenYinPastureSpiritImprintRecycleCfg *recycle_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintRecycle(item_cfg->quanlity, item_cfg->suit_id > 0);
// 		if (NULL == recycle_cfg)
// 		{
// 			break;
// 		}
// 
// 		grid->num -= cmd->virtual_bag_list[i].num;
// 		if (grid->num <= 0)
// 		{
// 			grid->Reset();
// 		}
// 
// 		m_param.imprint_score += (recycle_cfg->add_imprint_score * cmd->virtual_bag_list[i].num);
// 		total_add_score += (recycle_cfg->add_imprint_score * cmd->virtual_bag_list[i].num);
// 	}
// 
// 	this->SendShenYinScoreInfo((int)SHEN_YIN_SCORE_NOTIFY_TYPE_EXCHANGE, total_add_score);
// 	m_virtual_bag.SendBagAllInfo();
// }

void ShenYin::OneKeyRecycleOpera(const Protocol::CSShenYinOneKeyRecyleReq *cmd)
{
	if (NULL == cmd) return;

	Knapsack* knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	std::vector<ItemConfigData> item_vec;

	for (unsigned int i = 0; i < (unsigned int)cmd->count && i < static_array_size(cmd->bag_index_list); i++)
	{
		int backpack_index = cmd->bag_index_list[i];
		if (backpack_index < 0 || backpack_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
		{
			continue;
		}

		ItemGridData *item_data = knapsack->GetItemGridData(backpack_index);
		if (item_data == NULL) continue;

		const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByid(item_data->item_id);
		if (NULL == item_cfg || item_cfg->item_type != (int)ShenYinPastureSpiritItemCfg::EQUIPMENT)		//只有印记能分解
		{
			continue;
		}

		if (item_cfg->return_item_id > 0)
		{
			ItemConfigData return_item;
			return_item.item_id = item_cfg->return_item_id;
			return_item.num = item_cfg->return_item_num;
			return_item.is_bind = item_data->is_bind;

			if (!knapsack->Discard(backpack_index, item_data->num, DISCARD_METHOD_RECYCL, false))
			{
				continue;
			}

			// 记录放入背包的物品
			const ItemBase *itembase = ITEMPOOL->GetItem(item_cfg->return_item_id);
			if (itembase == NULL) continue;

			auto it = std::find_if(item_vec.begin(), item_vec.end(), [return_item](const ItemConfigData &tmp_item) {
				return return_item.item_id == tmp_item.item_id && return_item.is_bind == tmp_item.is_bind;
			});
			if (item_vec.end() != it)
			{
				(*it).num += return_item.num;
			}
			else
			{
				item_vec.push_back(return_item);
			}
		}
	}

	// 放物品进背包
	{
		for (auto &it : item_vec)
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(it.item_id);
			if (itembase == NULL) continue;

			int pile_max_num = itembase->GetPileLimit();
			int put_times = it.num / pile_max_num;	// 堆叠数量的倍数
			int remain_num = it.num % pile_max_num;	// 剩余数量
			if (put_times > 0)
			{
				for (int times = 0; times < put_times; ++times)
				{
					ItemConfigData item_data(it.item_id, it.is_bind, pile_max_num);
					m_role->GetKnapsack()->PutOrMail(item_data, PUT_REASON_SHENYIN_DECOMPOSE);
				}
			}

			if (remain_num > 0)
			{
				ItemConfigData item_data(it.item_id, it.is_bind, remain_num);
				m_role->GetKnapsack()->PutOrMail(item_data, PUT_REASON_SHENYIN_DECOMPOSE);
			}
		}

		m_role->GetKnapsack()->SendKnapsackInfoToRole();
	}
}

bool ShenYin::TrantsItem()
{
	const ShenYinLevelLimitCfg *limit_cfg = LOGIC_CONFIG->GetShenYinConfig().GetLevelLimitCfg(m_role->GetLevel());
	if (NULL == limit_cfg || m_bead_count >= limit_cfg->bead_num)
	{
		m_role->NoticeNum(errornum::EN_SHEN_YIN_BEAN_NUM_LIMIT);
		return false;
	}

	for (int y = 0; y < SHEN_YIN_TABEL_ROW_COUNT; ++y)
	{
		for (int x = 0; x < SHEN_YIN_TABEL_MIDDLE_GRIDS; ++x)
		{
			if (!this->isValidPosi(x, y))
			{
				continue;
			}
			
			if (m_param.table_lsit[x][y].IsEmpty())
			{
				return this->Activebead(x, y);
			}
		}
	}

	return false;
}

void ShenYin::SendAllInfo()
{
	this->SendShenYinTianXiangAllInfo();
	this->SendShenYinScoreInfo();
	this->SendShenYinSlotInfo();
	//m_virtual_bag.SendBagAllInfo();
	this->SendShopInfo();
	this->SendLieHunPoolInfo();
}

bool ShenYin::Activebead(int x, int y)
{
	if (!this->isValidPosi(x, y))
	{
		return false;
	}
	
	ShenYinTableGridParam &grid = m_param.table_lsit[x][y];
	if (!grid.IsEmpty())
	{
		return false;
	}

	grid.type = RandomNum(SHEN_YIN_BEAD_TYPE_FIRE, SHEN_YIN_BEAD_TYPE_MAX);

	this->CheckCombineAll();
	this->SendSingleInfo(x, y);
	this->SendCombineInfo();

	m_role->NoticeNum(noticenum::NT_SHEN_YIN_ACTIVE_BEAD_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
	++ m_bead_count;
	return true;
}

void ShenYin::ChangeGrid(int src_x, int src_y, int target_x, int target_y)
{
	if (!this->isValidPosi(src_x, src_y) || !this->isValidPosi(target_x, target_y))
	{
		return;
	}

	ShenYinTableGridParam &src_grid = m_param.table_lsit[src_x][src_y];
	ShenYinTableGridParam &target_grid = m_param.table_lsit[target_x][target_y];
	if (target_grid.type == src_grid.type)
	{
		return;
	}

	//重置包含替换珠子的组合；
	this->ResetCombine(src_grid);
	this->ResetCombine(target_grid);
	src_grid.ResetCobine();
	target_grid.ResetCobine();

	//替换
	ShenYinTableGridParam tmp_grid = src_grid;
	src_grid = target_grid;
	target_grid = tmp_grid;

	this->CheckCombineAll();
	this->SendSingleInfo(src_x, src_y);
	this->SendSingleInfo(target_x, target_y);
	this->SendCombineInfo();

	m_role->NoticeNum(noticenum::NT_SHEN_YIN_MOVE_BEAD_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
}

void ShenYin::ChangeBeadType(int x, int y, int target_type)
{
	if (!this->isValidPosi(x, y))
	{
		return;
	}

	if (target_type <= SHEN_YIN_BEAD_TYPE_INVALID || target_type >= SHEN_YIN_BEAD_TYPE_MAX)
	{
		return;
	}

	const ShenYinOtherCfg &other_cfg = LOGIC_CONFIG->GetShenYinConfig().GetOtherCfg();
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.change_bead_type_need_gold, __FUNCTION__))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	ShenYinTableGridParam tmp_bead = m_param.table_lsit[x][y];

	// 改变类型
	m_param.table_lsit[x][y].type = target_type;

	// 重置组合
	this->ResetCombine(tmp_bead);
	tmp_bead.ResetCobine();
	this->CheckCombineAll();
	this->SendSingleInfo(x, y);
	this->SendCombineInfo();

	m_role->NoticeNum(noticenum::NT_SHEN_YIN_CHANGE_BEAD_TYPE_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
}

void ShenYin::CheckCombineAll()
{
	m_combine_map.clear();

	for (int y = 0; y < SHEN_YIN_TABEL_ROW_COUNT; ++y)
	{
		for (int x = 0; x < SHEN_YIN_TABEL_MIDDLE_GRIDS; ++x)
		{
			this->CheckCombine(x, y);
		}
	}
}

bool ShenYin::CheckCombine(int x, int y)
{
	if (!this->isValidPosi(x, y))
	{
		return false;
	}

	ShenYinTableGridParam &grid_bead = m_param.table_lsit[x][y];
	if (grid_bead.IsEmpty())
	{
 		return false;
	}

	bool is_can_active = false;
	BeadPosi combine_posi(x, y);
	int combine_count = LOGIC_CONFIG->GetShenYinConfig().GetCombineSeqCfgCount();
	for (int seq = 0; seq < combine_count; ++ seq)
	{
		// 检查是否已激活, 不能重复激活；
		if (this->IsActiveCombine(seq))
		{
			continue;
		}

		// 是否可以激活
		if (!this->CanActiveCombine(seq, grid_bead.type, combine_posi))
		{
			continue;
		}
		
		// 激活组合
		grid_bead.seq = seq;
		m_combine_map[seq] = combine_posi;
		is_can_active = true;
	}

	return is_can_active;
}

bool ShenYin::IsActiveCombine(int seq)
{
	//已有组合抛弃
	std::map<int, BeadPosi>::iterator combine_item = m_combine_map.find(seq);
	if (combine_item != m_combine_map.end())
	{
		return true;
	}

	return false;
}

bool ShenYin::CanActiveCombine(int seq, int src_type, const BeadPosi &src_posi)
{
	// 到这里不存在第一个珠子为无效类型的组合配置
	const ShenYinCombineSeqCfg *Combine_cfg = LOGIC_CONFIG->GetShenYinConfig().GetCombineSeqCfg(seq);
	if (NULL == Combine_cfg)
	{
		return false;
	}

	// 对比第一个格子 第一个格子就不符合的，直接跳过；
	const ShenYinCombineItem &firt_bead = Combine_cfg->bead_list[0];
	if (firt_bead.type != SHEN_YIN_BEAD_TYPE_SAME && firt_bead.type != src_type)
	{
		return false;
	}

	// 先插入第一个格子坐标
	m_tmp_posi_set.clear();
	m_tmp_posi_set.insert(src_posi);
	BeadPosi posi;
	// 对比后面的格子
	for (int index = 1; index < SHEN_YIN_COMBINE_MAX_BEADS; ++index)
	{
		const ShenYinCombineItem &bead_cfg = Combine_cfg->bead_list[index];

		//只能是组合的必要珠子都合格了，才会到这里, 配置正确的话；
		if (bead_cfg.type == SHEN_YIN_BEAD_TYPE_INVALID)
		{
			break;
		}

		// 寻找格子
		posi.Set(src_posi.x + bead_cfg.x, src_posi.y + bead_cfg.y);
		if (!this->isValidPosi(posi.x, posi.y))
		{
			return false;
		}

		const ShenYinTableGridParam &tmp_bead = m_param.table_lsit[posi.x][posi.y];

		// 检查类型
		if (bead_cfg.type == SHEN_YIN_BEAD_TYPE_SAME && tmp_bead.type != src_type)
		{
			return false;
		}
		
		if (bead_cfg.type != SHEN_YIN_BEAD_TYPE_SAME && bead_cfg.type != tmp_bead.type)
		{
			return false;
		}

		m_tmp_posi_set.insert(posi);
	}


	// 这里的坐标在循环里面已经检查过了
// 	for (std::set<BeadPosi>::iterator it_set = m_tmp_posi_set.begin(); it_set != m_tmp_posi_set.end(); ++ it_set)
// 	{
// 		m_param.table_lsit[it_set->x][it_set->y].src_x = (char)src_posi.x;
// 		m_param.table_lsit[it_set->x][it_set->y].src_y = (char)src_posi.y;
// 	}
// 	m_tmp_posi_set.clear();

	return true;
}

void ShenYin::ResetCombine(const ShenYinTableGridParam &grid)
{
	if (!grid.IsUse())
	{
		return;
	}

	ShenYinTableGridParam combine_src_grid = m_param.table_lsit[static_cast<int>(grid.src_x)][static_cast<int>(grid.src_y)];

	const ShenYinCombineSeqCfg *Combine_cfg = LOGIC_CONFIG->GetShenYinConfig().GetCombineSeqCfg(combine_src_grid.seq);
	if (NULL == Combine_cfg)
	{
		return;
	}

	for (int index = 0; index < SHEN_YIN_COMBINE_MAX_BEADS; ++index)
	{
		const ShenYinCombineItem &bead_cfg = Combine_cfg->bead_list[index];
		int x = combine_src_grid.src_x + bead_cfg.x;
		int y = combine_src_grid.src_y + bead_cfg.y;
		if (!this->isValidPosi(x, y))
		{
			return;
		}

		// 只有坐标合法就重置，出错也只是配置错误，不妨碍改正配置之后重新正确激活；
		m_param.table_lsit[x][y].ResetCobine();
	}

	m_combine_map.erase(combine_src_grid.seq);
}

bool ShenYin::isValidPosi(const int x, const int y)
{
	if (x < 0 || x >= SHEN_YIN_TABEL_MIDDLE_GRIDS || y < 0 || y >= SHEN_YIN_TABEL_ROW_COUNT)
	{
		return false;
	}

	if (y < SHEN_YIN_TABEL_MIDDLE_ROW)
	{
		int difference = SHEN_YIN_TABEL_MIDDLE_ROW - y;
		if (x >= SHEN_YIN_TABEL_MIDDLE_GRIDS - difference)
		{
			return false;
		}
	}
	else
	{
		int difference = y - SHEN_YIN_TABEL_MIDDLE_ROW;
		if (x < difference)
		{
			return false;
		}
	}

	return true;
}

void ShenYin::SendShenYinTianXiangAllInfo() const
{
	static Protocol::SCSendTianXiangAllInfo txai;
	txai.grid_list.clear();
	txai.combine_list.clear();

	for (int y = 0; y < SHEN_YIN_TABEL_ROW_COUNT; ++y)
	{
		for (int x = 0; x < SHEN_YIN_TABEL_MIDDLE_GRIDS; ++x)
		{
			const ShenYinTableGridParam &tmp_grid = m_param.table_lsit[x][y];
			if (tmp_grid.IsEmpty())
			{
				continue;
			}
			
			// 珠子信息
			Protocol::TianXiangBeadParam tmp_bead;
			tmp_bead.x = x;
			tmp_bead.y = y;
			tmp_bead.type = tmp_grid.type;
			txai.grid_list.push_back(tmp_bead);

			// 组合信息
// 			if (tmp_grid.seq >= 0)
// 			{
// 				Protocol::TianXiangCombineParam tmp_combine;
// 				tmp_combine.x = (char)x;
// 				tmp_combine.y = (char)y;
// 				tmp_combine.seq = tmp_grid.seq;
// 
// 				txai.combine_list.push_back(tmp_combine);
// 			}
		}
	}

	Protocol::TianXiangCombineParam tmp_combine;
	std::map<int, BeadPosi>::const_iterator it_map = m_combine_map.begin();
	for (; it_map != m_combine_map.end(); ++it_map)
	{
		tmp_combine.x = static_cast<char>(it_map->second.x);
		tmp_combine.y = static_cast<char>(it_map->second.y);
		tmp_combine.seq = it_map->first;

		txai.combine_list.push_back(tmp_combine);
	}

	SerializeMgr.NetSend(m_role->GetNetId(), txai);
}

void ShenYin::SendSingleInfo(int x, int y) const
{
	if (!this->isValidPosi(x, y))
	{
		return;
	}
	const ShenYinTableGridParam &tmp_grid = m_param.table_lsit[x][y];

	static Protocol::SCSendTianXiangAllInfo txai;
	txai.grid_list.clear();
	txai.combine_list.clear();

	// 珠子信息
	Protocol::TianXiangBeadParam tmp_bead;
	tmp_bead.x = (char)x;
	tmp_bead.y = (char)y;
	tmp_bead.type = tmp_grid.type;
	txai.grid_list.push_back(tmp_bead);

	MsgSerializeManager::GetInstance().NetSend(m_role->GetNetId(), txai);
}

void ShenYin::SendCombineInfo() const
{
	static Protocol::SCSendTianXiangAllInfo txai;
	txai.grid_list.clear();
	txai.combine_list.clear();

	Protocol::TianXiangCombineParam tmp_combine;

	std::map<int, BeadPosi>::const_iterator it_map = m_combine_map.begin();
	for (; it_map != m_combine_map.end(); ++it_map)
	{
		tmp_combine.x = static_cast<char>(it_map->second.x);
		tmp_combine.y = static_cast<char>(it_map->second.y);
		tmp_combine.seq = it_map->first;

		txai.combine_list.push_back(tmp_combine);
	}

	if (txai.combine_list.size() > 0 )
	{
		MsgSerializeManager::GetInstance().NetSend(m_role->GetNetId(), txai);
	}
}

void ShenYin::ShenYinUpLevel(int slot_type, bool is_use_protect, bool is_auto_buy)
{
	if (this->IsInvalidSlotType(slot_type))
	{
		return;
	}

	ShenYinPastureSpiritGridParam &slot = m_param.imprint_slot_list[slot_type];
	if (slot.IsEmpty())
	{
		return;
	}

	const ShenYinPastureSpiritImprintUpStartCfg *cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintUpLevel(slot_type, slot.grade, slot.param1);
	const ShenYinPastureSpiritImprintUpStartCfg *next_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintUpLevel(slot_type, slot.grade, slot.param1 + 1);
	const ShenYinPastureSpiritImprintUpStartCfg *next_grade_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintUpLevel(slot_type, slot.grade + 1, 0);
	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByid(cfg->consume_v_item_id);
	if (NULL == cfg || NULL == item_cfg)
	{
		return;
	}

	int tmp_level = slot.param1;
	int tmp_grade = slot.grade;

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = cfg->consume_v_item_id;
		stuff_list[stuff_count].num = cfg->need_num;
		stuff_list[stuff_count].buyable = is_auto_buy;
		++stuff_count;

		/*
		//18-9-10  梁高聪需求 失败不掉级
		if (is_use_protect)
		{
			stuff_list[stuff_count].item_id = cfg->protect_v_item_id;
			stuff_list[stuff_count].num = cfg->need_protect_num;
			stuff_list[stuff_count].buyable = false;
			++stuff_count;
		}*/

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	if (NULL != next_cfg && tmp_level < ShenYinPastureSpiritImprintUpStartCfg::MAX_LEVEL)
	{
		++ tmp_level;
	}
	else if (NULL == next_cfg && tmp_level >= ShenYinPastureSpiritImprintUpStartCfg::MAX_LEVEL && NULL != next_grade_cfg)
	{
		tmp_level = 0;
		++ tmp_grade;
	}
	else
	{
		m_role->NoticeNum(errornum::EN_SHEN_YIN_MAX_LEVEL);
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShenYin::ShenYinUpLevel"))
	{
		return;
	}

	int rand_value = RandomNum(PROBABILITY_HUNDRED);
	if (rand_value >= cfg->rate)
	{
		m_role->NoticeNum(errornum::EN_SHEN_YIN_UPLEVEL_UNSUCCESS);
		/*
		//18-9-10  梁高聪需求 失败不掉级
		if (!is_use_protect)
		{
			--slot.param1;
			if (slot.param1 < 0)
			{
				slot.param1 = 0;
			}
			this->SendShenYinSlotInfo(&slot_type);
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
		}*/

		return;
	}

	slot.param1 = tmp_level;
	slot.grade = tmp_grade;

	this->SendShenYinSlotInfo(&slot_type);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);

}

void ShenYin::ShenYinUpGrade()
{
// 	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT; ++i)
// 	{
// 		ShenYinPastureSpiritGridParam &slot = m_param.imprint_slot_list[i];
// 		if (slot.IsEmpty() || slot.param1 < ShenYinPastureSpiritImprintUpStartCfg::MAX_LEVEL)
// 		{
// 			return;
// 		}
// 	}
// 
// 	for (int j = 0; j < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT; ++j)
// 	{
// 		ShenYinPastureSpiritGridParam &psgp = m_param.imprint_slot_list[j];
// 		psgp.param1 = 0;
// 	}
// 
// 	this->SendImprintSlotInfo();
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TIAN_XIANG);
}

// void ShenYin::ShenYinEquip(int v_bag_index, int slot_type)
// {
// 	if (this->IsInvalidSlotType(slot_type))
// 	{
// 		return;
// 	}
// 
// 	ShenYinPastureSpiritGridParam &slot = m_param.imprint_slot_list[slot_type];
// 	ShenYinPastureSpiritGridParam *grid = m_virtual_bag.GetGridPrt(v_bag_index);
// 	if (NULL == grid || grid->IsEmpty())
// 	{
// 		return;
// 	}
// 
// 	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(grid->v_item_id);
// 	if (NULL == item_cfg || item_cfg->imprint_slot != slot_type || item_cfg->item_type != (int)ShenYinPastureSpiritItemCfg::EQUIPMENT)
// 	{
// 		return;
// 	}
// 
// 	ShenYinPastureSpiritGridParam::AttrItem tmp_new_attr_list;
// 	ShenYinPastureSpiritGridParam::AttrItem tmp_attr_list;
// 	tmp_new_attr_list.Reset();
// 	tmp_attr_list.Reset();
// 
// 	int level = slot.param1;
// 	int grade = slot.grade;
// 	int qianghua_count = slot.qianghua_count;
// 	tmp_attr_list = slot.attr_param;
// 	tmp_new_attr_list = slot.new_attr_param;
// 	ShenYinPastureSpiritGridParam tmp; tmp.Reset();
// 	tmp = slot;
// 	slot = *grid;
// 	*grid = tmp;
// 	slot.param1 = level;
// 	slot.grade = grade;
// 	grid->param1 = 0;
// 	grid->grade = 0;
// 	slot.qianghua_count = qianghua_count;
// 	slot.new_attr_param = tmp_new_attr_list;
// 	slot.attr_param = tmp_attr_list;
// 
// 	if (grid->IsEmpty())
// 	{
// 		m_virtual_bag.ResetGrid(*grid);
// 	}
// 
// 	m_virtual_bag.SendBagInfoByIndex(v_bag_index);
// 	this->SendShenYinSlotInfo(&slot_type);
// 
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
// }

// void ShenYin::ShenYinTakeOff(int slot_type)
// {
// 	if (this->IsInvalidSlotType(slot_type))
// 	{
// 		return;
// 	}
// 
// 	ShenYinPastureSpiritGridParam &slot = m_param.imprint_slot_list[slot_type];
// 	if (slot.IsEmpty())
// 	{
// 		return;
// 	}
// 
// 	if (!m_virtual_bag.HasEnoughEmptyGrid())
// 	{
// 		return;
// 	}
// 
// 	ShenYinPastureSpiritGridParam::AttrItem tmp_new_attr_list;
// 	ShenYinPastureSpiritGridParam::AttrItem tmp_attr_list;
// 	tmp_new_attr_list.Reset();
// 	tmp_attr_list.Reset();
// 
// 	int level = slot.param1;
// 	int grade = slot.grade;
// 	int qianghua_count = slot.qianghua_count;
// 	tmp_attr_list = slot.attr_param;
// 	tmp_new_attr_list = slot.new_attr_param;
// 	slot.param1 = 0;
// 	m_virtual_bag.PutVirtualItem(slot, __FUNCTION__, 1);
// 	slot.Reset();
// 	slot.param1 = level;
// 	slot.grade = grade;
// 	slot.qianghua_count = qianghua_count;
// 	slot.new_attr_param = tmp_new_attr_list;
// 	slot.attr_param = tmp_attr_list;
// 	this->SendShenYinSlotInfo(&slot_type);
// 
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
// }

void ShenYin::ShenYinAddAttrCount(int slot_type)
{
	if (this->IsInvalidSlotType(slot_type))
	{
		return;
	}
	ShenYinPastureSpiritGridParam &tmp = m_param.imprint_slot_list[slot_type];
	if (tmp.IsEmpty())
	{
		return;
	}

	int index = -1;
	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT - 1; ++ i)
	{
		int &type = tmp.attr_param.type_list[i];
		if (type <= (int)AttrConfig::ATTR_TYPE_INVALID)
		{
			index = i;
			break;
		}
	}

	if (index < 0)
	{
		return;
	}

	const ShenYinWashAddCount *add_count_cfg = LOGIC_CONFIG->GetShenYinConfig().GetShenYinWashAddCountCfg(index + 1);
	if (NULL == add_count_cfg)
	{
		return;
	}

	int attr_index = RandomNum(static_cast<int>(AttrConfig::ATTR_TYPE_MAX_COUNT));
	const ShenYinPastureSpiritImprintExtraAttrCfg *result = LOGIC_CONFIG->GetShenYinConfig().GetRandImprintExtraAttrByType(slot_type, attr_index);
	if (result == NULL)
	{
		return;
	}

	const ShenYinOtherCfg &other_cfg = LOGIC_CONFIG->GetShenYinConfig().GetOtherCfg();
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = other_cfg.add_attr_type_need_v_item_id;
		stuff_list[stuff_count].num = add_count_cfg->consume_num;
		stuff_list[stuff_count].buyable = false;
		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShenYin::ShenYinAddAttrCount"))
	{
		return;
	}

	if (RandomNum(PROBABILITY_HUNDRED) >= add_count_cfg->rate)
	{
		m_role->NoticeNum(errornum::EN_SHEN_YIN_XILIAN_UNSUCCESS);
		return;
	}

	int qianghua_count = tmp.qianghua_count;
	int baodi_index = -1;
	if (slot_type >= 0 && slot_type < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT)
	{
		if (qianghua_count > 0 && qianghua_count > result->baodi_count_cfg[SHEN_YIN_XILIAN_BAODI_MAX_COUNT - 1].baodi_count)
		{
			baodi_index = SHEN_YIN_XILIAN_BAODI_MAX_COUNT - 1;
		}
		else
		{
			for (int i = 0; i < SHEN_YIN_XILIAN_BAODI_MAX_COUNT; ++i)
			{
				if (qianghua_count <= result->baodi_count_cfg[i].baodi_count)
				{
					baodi_index = i;
					break;
				}
			}
		}

		if (baodi_index < 0 || baodi_index >= SHEN_YIN_XILIAN_BAODI_MAX_COUNT)
		{
			return;
		}
		int add_val = static_cast<int>((result->max_value - result->min_value) * result->baodi_count_cfg[baodi_index].criti_per / 100.0f);
		int value_max = add_val + result->min_value;
		tmp.attr_param.type_list[index] = result->attr_type;
		tmp.attr_param.value_list[index] = RandomNum(result->min_value, value_max);
	}

	this->SendShenYinSlotInfo(&slot_type);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
	m_role->NoticeNum(noticenum::NT_SHEN_YIN_XILIAN_SUCC);
}

void ShenYin::ShenYinFlushAttrType(int slot_type, int lock_flag)
{
	if (this->IsInvalidSlotType(slot_type))
	{
		return;
	}

	ShenYinPastureSpiritGridParam &tmp = m_param.imprint_slot_list[slot_type];
	if (tmp.IsEmpty())
	{
		return;
	}

	if (tmp.attr_param.type_list[0] < 0)
	{
		return;
	}

	int lock_num = 0;
	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT - 1; i++)
	{
		if (0 != (lock_flag & (1 << i)))
		{
			++lock_num;
		}
	}

	const ShenYinOtherCfg &other_cfg = LOGIC_CONFIG->GetShenYinConfig().GetOtherCfg();
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = other_cfg.flush_attr_type_need_v_item_id;
		stuff_list[stuff_count].num = other_cfg.flush_attr_type_need_v_item_num;
		stuff_list[stuff_count].buyable = false;

		if (lock_num > 0)
		{
			stuff_list[stuff_count].num = stuff_list[stuff_count].num + (lock_num * other_cfg.lock_consume);
		}
		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShenYin::ShenYinFlushAttrType"))
	{
		return;
	}

	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT - 1; ++ i)
	{
		if (tmp.attr_param.type_list[i] < 0)
		{
			break;
		}

		if (0 != (lock_flag & (1 << i)))
		{
			tmp.new_attr_param.type_list[i] = tmp.attr_param.type_list[i];
			tmp.new_attr_param.value_list[i] = tmp.attr_param.value_list[i];
			continue;
		}

		AttrConfig::ATTR_TYPE new_type = LOGIC_CONFIG->GetShenYinConfig().GetRandImprintExtraAttrType();
		const ShenYinPastureSpiritImprintExtraAttrCfg *old_result = LOGIC_CONFIG->GetShenYinConfig().GetRandImprintExtraAttrByType(slot_type, tmp.attr_param.type_list[i]);
		const ShenYinPastureSpiritImprintExtraAttrCfg *new_result = LOGIC_CONFIG->GetShenYinConfig().GetRandImprintExtraAttrByType(slot_type, (int)new_type);
		if (old_result == NULL || new_result == NULL)
		{
			continue;
		}

		tmp.new_attr_param.type_list[i] = (int)new_type;
		double percnet = tmp.attr_param.value_list[i] * 1.0 / old_result->max_value;
		tmp.new_attr_param.value_list[i] = (int)(new_result->max_value * percnet);
	}

	this->SendShenYinSlotInfo(&slot_type);
	m_role->NoticeNum(noticenum::NT_SHEN_YIN_XILIAN_SUCC);
}

void ShenYin::ShenYinFlushAttrValue(int slot_type, int lock_flag)
{
	if (this->IsInvalidSlotType(slot_type))
	{
		return;
	}

	ShenYinPastureSpiritGridParam &tmp = m_param.imprint_slot_list[slot_type];
	if (tmp.IsEmpty())
	{
		return;
	}

	if (tmp.attr_param.type_list[0] < 0)
	{
		return;
	}

	int lock_num = 0;
	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT - 1; i++)
	{
		if (0 != (lock_flag & (1 << i)))
		{
			++lock_num;
		}
	}

	const ShenYinOtherCfg &other_cfg = LOGIC_CONFIG->GetShenYinConfig().GetOtherCfg();
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = other_cfg.flush_attr_value_need_v_item_id;
		stuff_list[stuff_count].num = other_cfg.flush_attr_value_need_v_item_num;
		stuff_list[stuff_count].buyable = false;

		if (lock_num > 0)
		{
			stuff_list[stuff_count].num = other_cfg.flush_attr_value_need_v_item_num + (lock_num * other_cfg.lock_consume);
		}
		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShenYin::ShenYinFlushAttrValue"))
	{
		return;
	}

	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT - 1; ++i)
	{
		if (tmp.attr_param.type_list[i] < 0)
		{
			break;
		}

		if (0 != (lock_flag & (1 << i)))
		{
			tmp.new_attr_param.type_list[i] = tmp.attr_param.type_list[i];
			tmp.new_attr_param.value_list[i] = tmp.attr_param.value_list[i];
			continue;
		}

		const ShenYinPastureSpiritImprintExtraAttrCfg *result = LOGIC_CONFIG->GetShenYinConfig().GetRandImprintExtraAttrByType(slot_type, tmp.attr_param.type_list[i]);
		if (result == NULL)
		{
			continue;
		}

		int qianghua_count = tmp.qianghua_count;
		int baodi_index = -1;
		if (qianghua_count > 0 && qianghua_count > result->baodi_count_cfg[SHEN_YIN_XILIAN_BAODI_MAX_COUNT - 1].baodi_count)
		{
			baodi_index = SHEN_YIN_XILIAN_BAODI_MAX_COUNT - 1;
		}
		else
		{
			for (int i = 0; i < SHEN_YIN_XILIAN_BAODI_MAX_COUNT; ++i)
			{
				if (qianghua_count <= result->baodi_count_cfg[i].baodi_count)
				{
					baodi_index = i;
					break;
				}
			}
		}

		if (baodi_index < 0 || baodi_index >= SHEN_YIN_XILIAN_BAODI_MAX_COUNT)
		{
			return;
		}
		int add_val = static_cast<int>((result->max_value - result->min_value) * result->baodi_count_cfg[baodi_index].criti_per / 100.0f);
		int value_max = add_val + result->min_value;
		tmp.new_attr_param.type_list[i] = tmp.attr_param.type_list[i];
		tmp.new_attr_param.value_list[i] = RandomNum(result->min_value, value_max);
	}

	++ tmp.qianghua_count;

	this->SendShenYinSlotInfo(&slot_type);
	m_role->NoticeNum(noticenum::NT_SHEN_YIN_XILIAN_SUCC);
}

void ShenYin::ShenYinAplyFlushResult(int slot_type)
{
	if (this->IsInvalidSlotType(slot_type))
	{
		return;
	}

	ShenYinPastureSpiritGridParam &tmp = m_param.imprint_slot_list[slot_type];
	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT - 1; ++ i)
	{
		if (tmp.attr_param.type_list[i] >= 0 && tmp.new_attr_param.type_list[i] < 0)
		{
			return;
		}
	}

	tmp.attr_param = tmp.new_attr_param;
	tmp.new_attr_param.Reset();

	this->SendShenYinSlotInfo(&slot_type);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
}

// void ShenYin::ShenYinRecycle(int v_bag_index, int num)
// {
// 	ShenYinPastureSpiritGridParam *grid = m_virtual_bag.GetGridPrt(v_bag_index);
// 	if (NULL == grid || grid->IsEmpty())
// 	{
// 		return;
// 	}
// 
// 	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(grid->v_item_id);
// 	if (NULL == item_cfg || item_cfg->item_type == (int)ShenYinPastureSpiritItemCfg::ITEM)
// 	{
// 		return;
// 	}
// 
// 	if (grid->num < num)
// 	{
// 		return;
// 	}
// 
// 	const ShenYinPastureSpiritImprintRecycleCfg *recycle_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintRecycle(item_cfg->quanlity, item_cfg->suit_id > 0);
// 	if (NULL == recycle_cfg)
// 	{
// 		return;
// 	}
// 
// 	grid->num -= num;
// 	if (grid->num <= 0)
// 	{
// 		grid->Reset();
// 	}
// 
// 	m_param.imprint_score += (recycle_cfg->add_imprint_score * num);
// 
// 	this->SendShenYinScoreInfo((int)SHEN_YIN_SCORE_NOTIFY_TYPE_EXCHANGE, recycle_cfg->add_imprint_score * num);
// 	m_virtual_bag.SendBagInfoByIndex(v_bag_index);
// }

void ShenYin::ShenYinExchange(int index)
{
	return;
// 	if (index < 0 || index >= SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT)
// 	{
// 		return;
// 	}
// 
// 	const ShenYinPastureSpiritImprintExchangeCfg *exchange_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintExchange(index);
// 	if (NULL == exchange_cfg)
// 	{
// 		return;
// 	}
// 
// 	char &buy_count = m_param.shop_buy_count_list[index];
// 	if (buy_count >= exchange_cfg->buy_count)
// 	{
// 		return;
// 	}
// 
// 	if (!this->IsHaveEnoughImprintScore(exchange_cfg->imprint_score))
// 	{
// 		return;
// 	}
// 	//检查背包空间
// 	if (!m_virtual_bag.HasEnoughEmptyGrid() || !m_role->GetKnapsack()->HaveSpace())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return;
// 	}
// 
// 	++ buy_count;
// 
// 	this->AddShenYinScore(-exchange_cfg->imprint_score);
// 
// 	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(exchange_cfg->v_item_id);
// 	if (exchange_cfg->is_virtual_item && NULL != item_cfg && item_cfg->item_type != (int)ShenYinPastureSpiritItemCfg::ITEM)
// 	{
// 		ShenYinPastureSpiritGridParam grid;
// 		grid.v_item_id = item_cfg->v_item_id;
// 		grid.num = exchange_cfg->item_num;
// 		grid.is_bind = exchange_cfg->is_bind;
// 		m_virtual_bag.PutVirtualItem(grid, __FUNCTION__, item_cfg->stack_num);
// 	}
// 	else
// 	{
// 		ItemConfigData item;
// 		item.item_id = exchange_cfg->v_item_id;
// 		item.num = exchange_cfg->item_num;
// 		item.is_bind = exchange_cfg->is_bind;
// 		m_role->GetKnapsack()->PutOrMail(item, PUT_REASON_SHEN_YIN);
// 	}
// 
// 	this->SendShopInfo(&index);
}

// bool ShenYin::TranstItemToVitem(ItemID item_id, int item_num, bool is_bind)
// {
// 	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByid(item_id);
// 	if (NULL == item_cfg)
// 	{
// 		return false;
// 	}
// 
// 	if (item_cfg->item_type == (int)ShenYinPastureSpiritItemCfg::ITEM)
// 	{
// 		m_role->NoticeNum(errornum::EN_ITEM_NOT_PUT_IN_BAG);
// 		return false;
// 	}
// 
// 	ShenYinPastureSpiritGridParam grid;
// 	grid.v_item_id = item_cfg->v_item_id;
// 	grid.num = item_num;
// 	grid.is_bind = is_bind ? 1 : 0;
// 
// 	if (!m_virtual_bag.HasEnoughSpace(grid.v_item_id, grid.num, is_bind, item_cfg->stack_num))
// 	{
// 		return false;
// 	}
// 
// 	m_virtual_bag.PutVirtualItem(grid, __FUNCTION__, item_cfg->stack_num);
// 	return true;
// }

bool ShenYin::PutOnItem(ItemID item_id, int item_num, bool is_bind)
{
	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByid(item_id);
	if (NULL == item_cfg || item_cfg->item_type != (int)ShenYinPastureSpiritItemCfg::EQUIPMENT)
	{
		return false;
	}

	ShenYinPastureSpiritGridParam &slot = m_param.imprint_slot_list[item_cfg->imprint_slot];

	if (!slot.IsEmpty() && !this->TakeOffToKnapsack(item_cfg->imprint_slot, true))
	{
		return false;
	}

	slot.v_item_id = item_cfg->v_item_id;
	slot.num = item_num;
	slot.is_bind = is_bind ? 1 : 0;

	int slot_type = item_cfg->imprint_slot;
	this->SendShenYinSlotInfo(&slot_type);

	this->ReCalcSuitCount();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
	return true;
}

int ShenYin::PutOnItem(ItemGridData &item_data)
{
	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByid(item_data.item_id);
	if (NULL == item_cfg || item_cfg->item_type != (int)ShenYinPastureSpiritItemCfg::EQUIPMENT)
	{
		return -1;
	}

	ShenYinPastureSpiritGridParam &slot = m_param.imprint_slot_list[item_cfg->imprint_slot];
	int slot_type = item_cfg->imprint_slot;

	bool is_replace = false;

	if (!slot.IsEmpty())
	{
		// 产生替换
		if (this->IsInvalidSlotType(slot_type))
		{
			return -2;
		}

		int tmp_item_id = LOGIC_CONFIG->GetShenYinConfig().GetItemIdByVid(slot.v_item_id);
		int tmp_item_is_bind = slot.is_bind;
		int tmp_item_is_num = 1;

		// 交换，更新背包格子
		ItemDataWrapper item_wrapper(tmp_item_id, tmp_item_is_num, tmp_item_is_bind);
		item_data.Set(item_wrapper);
		
		is_replace = true;
	}

	slot.v_item_id = item_cfg->v_item_id;
	slot.num = item_data.num;
	//slot.is_bind = item_data.is_bind ? (char)1 : (char)0;
	slot.is_bind = 1;								//装备后变为绑定物品

	this->SendShenYinSlotInfo(&slot_type);

	this->ReCalcSuitCount();
	for (int i = 0; i < ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX; ++i)	//检查是否完成大小目标
	{
		const RoleBSGoalItemConfig *cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalItemConfig(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN, i);
		if (cfg != nullptr)
		{
			int need_quality = cfg->param_1;						//品质
			int count = this->GetEquipCount(need_quality);						//满足品质的数量
			m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_1, item_cfg->quanlity, count);
		}
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);

	return is_replace ? 1 : 0;
}

bool ShenYin::TakeOffToKnapsack(int slot_type,  bool is_puton)
{
	if (this->IsInvalidSlotType(slot_type))
	{
		return false;
	}

	ShenYinPastureSpiritGridParam & slot = m_param.imprint_slot_list[slot_type];

	if (slot.IsEmpty())
	{
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	ItemConfigData item_data;
	item_data.item_id = LOGIC_CONFIG->GetShenYinConfig().GetItemIdByVid(slot.v_item_id);
	item_data.is_bind = slot.is_bind ? true : false;
	item_data.num = 1;

	if (!knapsack->Put(item_data, PUT_REASON_NO_NOTICE))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_EQUIP_TAKEOFF_ERROR);
		return false;
	}

	m_param.imprint_slot_list[slot_type].v_item_id = -1;
	m_param.imprint_slot_list[slot_type].num = 0;
	m_param.imprint_slot_list[slot_type].is_bind = 0;
	m_param.imprint_slot_list[slot_type].param1 = 0;
	m_param.imprint_slot_list[slot_type].param2 = 0;

	if (!is_puton)
	{
		this->ReCalcSuitCount();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
		this->SendShenYinSlotInfo(&slot_type);
	}
	return true;
}

void ShenYin::SendShenYinScoreInfo(int type, int add_score) const
{
	static Protocol::SCShenYinScoreInfo psisi;
	psisi.notify_type = type;
	psisi.add_score = add_score;
	psisi.score = m_param.imprint_score;
	psisi.chouhun_score = m_param.shen_yin_chouhun_score;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&psisi, sizeof(psisi));
}

void ShenYin::SendShenYinSlotInfo(int *slot_type) const
{
	static Protocol::SCShenYinBagInfo psibi;
	psibi.type = (char)Protocol::SCShenYinBagInfo::IMPRINT_SLOT;
	psibi.count = 0;

	if (slot_type != NULL)
	{
		if (this->IsInvalidSlotType(*slot_type))
		{
			return;
		}

		const ShenYinPastureSpiritGridParam &grid = m_param.imprint_slot_list[*slot_type];
		Protocol::ShenYinGridParamMsg &msg_grid = psibi.grid_list[(int)psibi.count];
		msg_grid.param1 = *slot_type;
		msg_grid.v_item_id = grid.v_item_id;
		msg_grid.item_num = grid.num;
		msg_grid.is_bind = grid.is_bind;
		msg_grid.level = grid.param1;
		msg_grid.grade = grid.grade;
		msg_grid.attr_param = grid.attr_param;
		msg_grid.new_attr_param = grid.new_attr_param;
		++psibi.count;
	}
	else
	{
		for (int type = 0; type < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT; ++type)
		{
			const ShenYinPastureSpiritGridParam &grid = m_param.imprint_slot_list[type];
			Protocol::ShenYinGridParamMsg &msg_grid = psibi.grid_list[(int)psibi.count];
			msg_grid.param1 = type;
			msg_grid.v_item_id = grid.v_item_id;
			msg_grid.item_num = grid.num;
			msg_grid.is_bind = grid.is_bind;
			msg_grid.level = grid.param1;
			msg_grid.grade = grid.grade;
			msg_grid.attr_param = grid.attr_param;
			msg_grid.new_attr_param = grid.new_attr_param;
			++psibi.count;
		}
	}

	psibi.imprint_grade = m_param.reserve_sh;
	int length = sizeof(psibi) - sizeof(psibi.grid_list[0]) * (SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT - psibi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&psibi, length);
}

void ShenYin::SendShopInfo(int *index) const
{
	static Protocol::SCShenYinShopInfo psisi;
	psisi.count = 0;
	if (index != NULL)
	{
		if (*index < 0 || *index >= SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT)
		{
			return;
		}

		Protocol::SCShenYinShopInfo::Shop &tmp = psisi.shop_list[(int)psisi.count];
		tmp.index = *index;
		tmp.buy_count = m_param.shop_buy_count_list[*index];
		tmp.timestamp = m_param.shop_buy_count_flush_timestamp_list[*index];
		++psisi.count;
	}
	else
	{
		for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT; ++i)
		{
			Protocol::SCShenYinShopInfo::Shop &tmp = psisi.shop_list[(int)psisi.count];
			tmp.index = psisi.count;
			tmp.buy_count = m_param.shop_buy_count_list[i];
			tmp.timestamp = m_param.shop_buy_count_flush_timestamp_list[i];
			++psisi.count;
		}
	}

	int length = sizeof(psisi) - sizeof(psisi.shop_list[0]) * (SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT - psisi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&psisi, length);
}

void ShenYin::SendLieHunPoolInfo() const
{
	static Protocol::SCShenYinLieMingBagInfo sylmbi;

	sylmbi.liehun_color = m_param.chouhun_color;
	memcpy(sylmbi.liehun_pool, m_param.chouhun_pool, sizeof(sylmbi.liehun_pool));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sylmbi, sizeof(sylmbi));
}

void ShenYin::GetRandAttr(int slot_type)
{
	if (slot_type < 0 || slot_type >= SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT) return;

	ShenYinPastureSpiritGridParam &tmp = m_param.imprint_slot_list[slot_type];
	int attr_slot_index = 0;
	if (tmp.attr_param.type_list[0] >= 0)
	{
		return;
	}

	int attr_index = RandomNum(static_cast<int>(AttrConfig::ATTR_TYPE_MAX_COUNT));
	const ShenYinPastureSpiritImprintExtraAttrCfg *result = LOGIC_CONFIG->GetShenYinConfig().GetRandImprintExtraAttrByType(slot_type, attr_index);
	if (result == NULL)
	{
		return;
	}

	int qianghua_count = tmp.qianghua_count;
	int baodi_index = -1;
	if (slot_type >= 0 && slot_type < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT)
	{
		if (qianghua_count > 0 && qianghua_count > result->baodi_count_cfg[SHEN_YIN_XILIAN_BAODI_MAX_COUNT - 1].baodi_count)
		{
			baodi_index = SHEN_YIN_XILIAN_BAODI_MAX_COUNT - 1;
		}
		else
		{
			for (int i = 0; i < SHEN_YIN_XILIAN_BAODI_MAX_COUNT; ++i)
			{
				if (qianghua_count <= result->baodi_count_cfg[i].baodi_count)
				{
					baodi_index = i;
					break;
				}
			}
		}

		if (baodi_index < 0 || baodi_index >= SHEN_YIN_XILIAN_BAODI_MAX_COUNT)
		{
			return;
		}

		int add_val = static_cast<int>((result->max_value - result->min_value) * result->baodi_count_cfg[baodi_index].criti_per / 100.0f);
		int value_max = add_val + result->min_value;
		tmp.attr_param.type_list[attr_slot_index] = result->attr_type;
		tmp.attr_param.value_list[attr_slot_index] = RandomNum(result->min_value, value_max);
	}
}

void ShenYin::AddShenYinScore(int score)
{
	if (INT_MAX - m_param.imprint_score < score)
	{
		m_param.imprint_score = INT_MAX;
	}
	else
	{
		m_param.imprint_score += score;
	}

	this->SendShenYinScoreInfo();
}

void ShenYin::GmAddImpritScore(int num)
{
	m_param.imprint_score += num;
	this->SendShenYinScoreInfo((int)SHEN_YIN_SCORE_NOTIFY_TYPE_EXCHANGE, num);
}

int ShenYin::ShenYinChouhun(bool notify_client, int chouhun_type, bool is_use_score)
{
	const char *chouhun_type_str = "";
	bool notify_bag_change = false;

	if (Protocol::CS_TIAN_XIANG_REQ_TYPE_CHOUHUN == chouhun_type)
	{
		notify_bag_change = true;
		chouhun_type_str = "Chouhun";
	}
	else if (Protocol::CS_TIAN_XIANG_REQ_TYPE_BATCH_HUNSHOU == chouhun_type)
	{
		chouhun_type_str = "BatchChouhun";
	}
	else
	{
		return -1;
	}

	if (m_liehun_pool_hunshou_count >= SHEN_YIN_LIEHUN_POOL_MAX_COUNT)
	{
		if (notify_client) m_role->NoticeNum(errornum::EN_SHEN_YIN_LIEMING_LIEHUN_POOL_FULL);
		return -2;
	}

	const ShenYinLieMingChouhunConfig *chouhun_cfg = LOGIC_CONFIG->GetShenYinConfig().GetChouhunCfg(m_param.chouhun_color);
	if (NULL == chouhun_cfg)
	{
		return -3;
	}

	if (!is_use_score && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(chouhun_cfg->cost_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return -4;
	}

	if (is_use_score && ((int)m_param.shen_yin_chouhun_score < chouhun_cfg->chouhun_score))
	{
		m_role->NoticeNum(errornum::EN_SCORE_NOT_ENOUGH);
		return -5;
	}

	int ret_color = -1;
	int color_rand_val = RandomNum(100);

	for (int i = 0; i < SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX; ++i)
	{
		if (color_rand_val < chouhun_cfg->rate_list[i])
		{
			ret_color = i;
			break;
		}

		color_rand_val -= chouhun_cfg->rate_list[i];
	}

	int reward_index = 0;

	if (-1 != ret_color)
	{
		const ShenYinLieMingChouhunItemWeightCfg *hunshou_weight_cfg = LOGIC_CONFIG->GetShenYinConfig().GetChouhunItemWeightCfg(ret_color);
		if (NULL == hunshou_weight_cfg || hunshou_weight_cfg->cfg_count <= 0 || hunshou_weight_cfg->total_weight <= 0)
		{
			return -5;
		}

		if (hunshou_weight_cfg->total_weight <= 0)
		{
			return -6;
		}

		int hunshou_rand_val = RandomNum(hunshou_weight_cfg->total_weight);

		for (int i = 0; i < hunshou_weight_cfg->cfg_count; ++ i)
		{
			if (hunshou_rand_val < hunshou_weight_cfg->cfg_list[i].weight)
			{
				reward_index = hunshou_weight_cfg->cfg_list[i].index;
				break;
			}

			hunshou_rand_val -= hunshou_weight_cfg->cfg_list[i].weight;
		}
	}

	if (0 == reward_index && NULL == LOGIC_CONFIG->GetShenYinConfig().GetHunshouCfg(reward_index))
	{
		return -7;
	}

	if (!is_use_score && !m_role->GetKnapsack()->GetMoney()->UseGold(chouhun_cfg->cost_gold, "ShenYin::Chouhun")) return -8;

	if (is_use_score)
	{
		m_param.shen_yin_chouhun_score -= chouhun_cfg->chouhun_score;
	}

	int put_index = m_liehun_pool_hunshou_count;
	m_param.chouhun_pool[put_index] = reward_index;
	++m_liehun_pool_hunshou_count;

	int old_chouhun_color = m_param.chouhun_color;
	if (RandomNum(100) < chouhun_cfg->advance_rate)
	{
		m_param.chouhun_color = (m_param.chouhun_color + 1) % SHEN_YIN_LIEMING_CHOUHUN_COLOR_XILIAN_FIRST;
	}
	else
	{
		m_param.chouhun_color = 0;
	}

	if (notify_bag_change)
	{
		this->SendLieHunPoolInfo();
	}

	//传闻
	{
		const ShenYinLieMingItemConfig *hunshou_cfg = LOGIC_CONFIG->GetShenYinConfig().GetHunshouCfg(reward_index);
		if (NULL != hunshou_cfg && (0 != hunshou_cfg->is_broadcast))
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_shenyin,
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), reward_index);
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}
	}

	// 日志
	{
		gamelog::g_log_tianxiang.printf(LL_INFO, "Chouhun type %s, user %d %s, reward_hunshou_id %d, put_idx %d, m_pool_count %d, "
			"old_chouhun_color %d, new_chouhun_color %d, cost_hunli %d",
			chouhun_type_str, m_role->GetUID(), m_role->GetName(), reward_index, put_index, m_liehun_pool_hunshou_count,
			old_chouhun_color, m_param.chouhun_color, chouhun_cfg->cost_gold);
	}

	this->SendShenYinScoreInfo();
	return put_index;
}

void ShenYin::ShenYinSuperChouhun()
{
	if (m_param.chouhun_color >= SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK)
	{
		m_role->NoticeNum(errornum::EN_LIEMING_ALREADY_MAX_CHOUHUN_COLOR);
		return;
	}

	int gold_price = LOGIC_CONFIG->GetShenYinConfig().GetOtherCfg().super_chouhun_price;

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(gold_price, "SuperChouhun"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int old_chouhun_color = m_param.chouhun_color;

	m_param.chouhun_color = SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK;
	this->SendLieHunPoolInfo();
	m_role->NoticeNum(noticenum::NT_OPERATE_SUCC);

	gamelog::g_log_tianxiang.printf(LL_INFO, "SuperChouhun user %d %s, use_all_gold %d, old_chouhun_color %d, new_chouhun_color %d",
		m_role->GetUID(), m_role->GetName(), gold_price, old_chouhun_color, m_param.chouhun_color);
}

void ShenYin::ShenYinBatchChouhun(bool is_use_score)
{
	int chouhun_count = 0;
	for (int i = 0; i < SHEN_YIN_LIEHUN_POOL_MAX_COUNT; ++ i)
	{
		int put_idx = this->ShenYinChouhun(0 == i, Protocol::CS_TIAN_XIANG_REQ_TYPE_BATCH_HUNSHOU, is_use_score);
		if (-4 == put_idx)
		{
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_SHEN_YIN_LIEHUN, 0);
			break;
		}

		if (put_idx < 0 || put_idx >= SHEN_YIN_LIEHUN_POOL_MAX_COUNT)
		{
			break;
		}

		chouhun_count++;
	}

	if (chouhun_count > 0)
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_SHEN_YIN_LIEHUN, 1);
	}

	this->SendLieHunPoolInfo();
}

void ShenYin::ShenYinPutInBagByChouHunPool(int pool_idx, bool is_notice)
{
	if (pool_idx < 0 || pool_idx >= m_liehun_pool_hunshou_count)
	{
		return;
	}

// 	if (!m_virtual_bag.HasEnoughEmptyGrid())
// 	{
// 		return;
// 	}

	short hunshou_id = m_param.chouhun_pool[pool_idx];
	const ShenYinLieMingItemConfig *hunshou_cfg = LOGIC_CONFIG->GetShenYinConfig().GetHunshouCfg(hunshou_id);
	if (hunshou_id <= 0 || NULL == hunshou_cfg)
	{
		return;
	}

 	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(hunshou_cfg->virtual_item_id);
	if (NULL == item_cfg)
	{
		return;
	}
// 
// 	if (item_cfg->item_type != (int)ShenYinPastureSpiritItemCfg::ITEM && NULL != item_cfg)
// 	{
// 		ShenYinPastureSpiritGridParam grid;
// 		grid.v_item_id = item_cfg->v_item_id;
// 		grid.num = 1;
// 		grid.is_bind = 1;
// 		m_virtual_bag.PutVirtualItem(grid, __FUNCTION__, item_cfg->stack_num);
// 	}
// 	else
// 	{
		ItemConfigData item;
		item.item_id = item_cfg->item_id;
		item.num = 1;
		item.is_bind = 1;
		m_role->GetKnapsack()->PutOrMail(item, PUT_REASON_SHEN_YIN);


	m_param.chouhun_pool[pool_idx] = m_param.chouhun_pool[m_liehun_pool_hunshou_count - 1];
	m_param.chouhun_pool[m_liehun_pool_hunshou_count - 1] = 0;
	-- m_liehun_pool_hunshou_count;

	if (is_notice)
	{
		this->SendLieHunPoolInfo();
	}
}

void ShenYin::ShenYinOnekeyPutInBagByChouHunPool()
{
	if (m_liehun_pool_hunshou_count <= 0 || m_liehun_pool_hunshou_count > SHEN_YIN_LIEHUN_POOL_MAX_COUNT) return;

	for (int i = m_liehun_pool_hunshou_count - 1; i >= 0; -- i)
	{
// 		if (!m_virtual_bag.HasEnoughEmptyGrid())
// 		{
// 			continue;
// 		}

		short hunshou_id = m_param.chouhun_pool[i];
		const ShenYinLieMingItemConfig *hunshou_cfg = LOGIC_CONFIG->GetShenYinConfig().GetHunshouCfg(hunshou_id);
		if (hunshou_id <= 0 || NULL == hunshou_cfg)
		{
			continue;
		}

		const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(hunshou_cfg->virtual_item_id);
		if (NULL == item_cfg)
		{
			continue;
		}

// 		if (item_cfg->item_type != (int)ShenYinPastureSpiritItemCfg::ITEM)
// 		{
// 			ShenYinPastureSpiritGridParam grid;
// 			grid.v_item_id = item_cfg->v_item_id;
// 			grid.num = 1;
// 			grid.is_bind = 1;
// 			m_virtual_bag.PutVirtualItem(grid, __FUNCTION__, item_cfg->stack_num, false);
// 		}
// 		else
// 		{
			ItemConfigData item;
			item.item_id = item_cfg->item_id;
			item.num = 1;
			item.is_bind = 1;
			m_role->GetKnapsack()->PutOrMail(item, PUT_REASON_SHEN_YIN);
		

		m_param.chouhun_pool[i] = m_param.chouhun_pool[m_liehun_pool_hunshou_count - 1];
		m_param.chouhun_pool[m_liehun_pool_hunshou_count - 1] = 0;
		--m_liehun_pool_hunshou_count;
	}

	this->SendLieHunPoolInfo();
	//m_virtual_bag.SendBagAllInfo();
}

void ShenYin::ShenYinChouhunPoolConverToScore()
{
	if (m_liehun_pool_hunshou_count <= 0 || m_liehun_pool_hunshou_count > SHEN_YIN_LIEHUN_POOL_MAX_COUNT)
	{
		return;
	}

	int total_add_score = 0;
	for (int i = m_liehun_pool_hunshou_count - 1; i >= 0; -- i)
	{
		if (m_param.chouhun_pool[i] < 0)
		{
			continue;
		}

		short hunshou_id = m_param.chouhun_pool[i];
		const ShenYinLieMingItemConfig *hunshou_cfg = LOGIC_CONFIG->GetShenYinConfig().GetHunshouCfg(hunshou_id);
		if (hunshou_id <= 0 || NULL == hunshou_cfg)
		{
			continue;
		}

		const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(hunshou_cfg->virtual_item_id);
		if (NULL == item_cfg)
		{
			continue;
		}

		const ShenYinPastureSpiritImprintRecycleCfg *recycle_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintRecycle(item_cfg->quanlity, item_cfg->suit_id > 0);
		if (NULL == recycle_cfg)
		{
			continue;
		}

		m_param.imprint_score += (recycle_cfg->add_imprint_score);
		total_add_score += recycle_cfg->add_imprint_score;

		m_param.chouhun_pool[i] = m_param.chouhun_pool[m_liehun_pool_hunshou_count - 1];
		m_param.chouhun_pool[m_liehun_pool_hunshou_count - 1] = 0;
		--m_liehun_pool_hunshou_count;
	}

	this->SendShenYinScoreInfo((int)SHEN_YIN_SCORE_NOTIFY_TYPE_EXCHANGE, total_add_score);
	this->SendLieHunPoolInfo();
}

void ShenYin::ShenyinChouhunPoolSingleItemConverToScore(int pool_idx)
{
	if (m_liehun_pool_hunshou_count <= 0 || pool_idx >= m_liehun_pool_hunshou_count || pool_idx < 0)
	{
		return;
	}

	short hunshou_id = m_param.chouhun_pool[pool_idx];
	const ShenYinLieMingItemConfig *hunshou_cfg = LOGIC_CONFIG->GetShenYinConfig().GetHunshouCfg(hunshou_id);
	if (hunshou_id <= 0 || NULL == hunshou_cfg)
	{
		return;
	}

	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(hunshou_cfg->virtual_item_id);
	if (NULL == item_cfg)
	{
		return;
	}

	const ShenYinPastureSpiritImprintRecycleCfg *recycle_cfg = LOGIC_CONFIG->GetShenYinConfig().GetImprintRecycle(item_cfg->quanlity, item_cfg->suit_id > 0);
	if (NULL == recycle_cfg)
	{
		return;
	}

	m_param.chouhun_pool[pool_idx] = m_param.chouhun_pool[m_liehun_pool_hunshou_count - 1];
	m_param.chouhun_pool[m_liehun_pool_hunshou_count - 1] = 0;
	--m_liehun_pool_hunshou_count;

	m_param.imprint_score += (recycle_cfg->add_imprint_score);

	this->SendShenYinScoreInfo((int)SHEN_YIN_SCORE_NOTIFY_TYPE_EXCHANGE, recycle_cfg->add_imprint_score);
	this->SendLieHunPoolInfo();
}

void ShenYin::AddShenYinChouHunScore(int score)
{
	if (score < 0) return;

	if (INT_MAX - m_param.shen_yin_chouhun_score < score)
	{
		m_param.shen_yin_chouhun_score = INT_MAX;
	}
	else
	{
		m_param.shen_yin_chouhun_score += score;
	}

	this->SendShenYinScoreInfo((int)SHEN_YIN_SCORE_NOTIFY_TYPE_LIEHUN, score);
}

void ShenYin::OnDayChange(int old_dayid, int now_dayid)
{
	this->CheckFuncOpen();
}

void ShenYin::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

void ShenYin::OnRoleLevelChange(int level)
{
	this->CheckFuncOpen();
}

void ShenYin::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENYIN);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN);
		}
	}
}

void ShenYin::ReCalcSuitCount()
{
	memset(m_quality_equip_count, 0, sizeof(m_quality_equip_count));

	for (int i = 0; i < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT; ++i)
	{
		ShenYinPastureSpiritGridParam &slot = m_param.imprint_slot_list[i];
		if (slot.IsEmpty())
		{
			continue;
		}
		
		const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByVid(slot.v_item_id);
		if (NULL == item_cfg)
		{
			continue;
		}

		if (item_cfg->quanlity >= 0 && item_cfg->quanlity <= SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK)
		{
			++m_quality_equip_count[item_cfg->quanlity];
		}
	}
}

int ShenYin::GetEquipCount(int quality)
{
	if (quality >= 0 && quality <= SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK)
	{
		return m_quality_equip_count[quality];
	}
	else
	{
		return 0;
	}
}

int ShenYin::GetSuitEquipCount(int quality)
{
	if (quality >= 0 && quality <= SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK)
	{
		if (quality == SHEN_YIN_LIEMING_CHOUHUN_COLOR_RED)
		{
			return m_quality_equip_count[quality] + m_quality_equip_count[SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK];
		}
		else
		{
			return m_quality_equip_count[quality];
		}
	}
	else
	{
		return 0;
	}
}

void ShenYin::NewShenyinRecycle(short index, short discard_num)
{
	Knapsack* knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ItemGridData *item_data = knapsack->GetItemGridData(index);
	if (item_data == NULL) return;

	const ShenYinPastureSpiritItemCfg *item_cfg = LOGIC_CONFIG->GetShenYinConfig().GetItemByid(item_data->item_id);
	if (NULL == item_cfg || item_cfg->item_type != (int)ShenYinPastureSpiritItemCfg::EQUIPMENT)		//只有印记能分解
	{
		return;
	}

	if (item_cfg->return_item_id > 0)
	{
		if (!knapsack->Discard(index, discard_num, DISCARD_METHOD_RECYCL))
		{
			return;
		}

		// 检查背包空间是否足够
		ItemConfigData return_item;
		return_item.item_id = item_cfg->return_item_id;
		return_item.num = item_cfg->return_item_num;

		const ItemBase *itembase = ITEMPOOL->GetItem(item_cfg->return_item_id);
		if (itembase == NULL) return;

		return_item.is_bind = item_data->is_bind;

		m_role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_SHENYIN_DECOMPOSE);
	}
}