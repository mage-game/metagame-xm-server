#include "runesystem.hpp"
#include <algorithm>

#include "servercommon/string/gameworldstr.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "obj/character/role.h"
#include "obj/otherobj/fallingitem.h"
#include "world.h"
#include "engineadapter.h"

#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"
#include "monster/drop/dropconfig.hpp"
#include "other/runesystem/runetowerconfig.hpp"
#include "other/runesystem/runesystemconfig.hpp"

#include "protocal/msgitem.h"
#include "item/knapsack.h"
#include "item/itempool.h"
#include "other/event/eventhandler.hpp"
#include "other/shop/roleshop.hpp"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "task/taskrecorder.h"
#include "gameworld/protocal/msgrole.h"
#include "servercommon/serverconfig/crossconfig.hpp"

RuneSystem::RuneSystem(): m_role(NULL), m_empty_grids(0), m_other_capability(0)
{
}

RuneSystem::~RuneSystem()
{
}

void RuneSystem::InitParam(Role *role, const RuneSystemParam &param)
{
	m_role = role;
	m_param = param;
	this->CheckOpenRuneSlotAndId();
	this->ReCalRuneEffct();
	this->SendVirtualBagAllInfo();
}

void RuneSystem::GetInitParam(RuneSystemParam *param) const
{
	*param = m_param;
}

void RuneSystem::OnRoleLogin(unsigned int last_save_time)
{
	this->CheckFuncOpen();
//	this->OnRuneTowerOfflineTime(last_save_time);
}

void RuneSystem::Update(unsigned int now_second)
{
	if (0 == m_param.next_free_xunbao_timestamp && 0 == m_param.free_xunbao_times)
	{
		m_param.free_xunbao_times = 1;
		this->SendOtherInfo();
	}

	if (0 == m_param.free_xunbao_times && now_second > m_param.next_free_xunbao_timestamp)
	{
		m_param.free_xunbao_times = 1;

		this->SendOtherInfo();
	}
}

void RuneSystem::OnDayChange(int old_dayid, int now_dayid)
{
	m_param.fb_fetch_time_count = 0;
	m_param.fb_today_layer = 0;
	m_param.fb_add_exp = 0;
	m_param.fb_add_jinghua = 0;
	m_param.fb_add_equip_blue = 0;
	m_param.fb_add_equip_purple = 0;
	m_param.fb_add_equip_orange = 0;
	m_param.rune_gold_awaken_times = 0;
	m_param.rune_common_awaken_times = 0;
	this->SendOtherInfo();
	this->SendRuneTowerInfo();
	this->CheckFuncOpen();
}

void RuneSystem::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		m_other_capability = 0;

		// 激活人物大目标
		this->ReCalcSpecialRoleBigGoalAttr();      

		// 符文基础属性加成
		for (int grid_index = 0; grid_index < RUNE_SYSTEM_SLOT_MAX_NUM && grid_index < LOGIC_CONFIG->GetRuneSystemCfg().GetRuneSlotNum(); ++grid_index)
		{
			RuneAwakenParm &rune_awaken = m_param.rune_grid_awaken[grid_index];
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += rune_awaken.gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += rune_awaken.fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += rune_awaken.maxhp;

			const RuneZhulingGradeConfig *grade_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetZhulingGradeCfg(grid_index, m_param.run_zhuling_list[grid_index].grade);
			if (!grade_cfg)
			{
				continue;
			}
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += grade_cfg->baoji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += grade_cfg->jianren;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_cfg->mingzhong;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_cfg->shanbi;

			RuneParam &rune = m_param.rune_grid[grid_index];
			if (rune.IsEmpty())
			{
				continue;
			}

			const RuneSystemAttrCfg *rune_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune.quality, rune.type, rune.level);
			if (NULL == rune_cfg)
			{
				continue;
			}

			int real_type_0 = 0;
			if (this->TransformAttrType(rune_cfg->attr_type_0, real_type_0) && CharIntAttrs::IsRoleBaseAttr(real_type_0))
			{
				Attribute add_value = static_cast<Attribute>(rune_cfg->add_attributes_0 * (1 + (rune_awaken.add_per * ROLE_ATTR_PER)));
				Attribute special_add_value = static_cast<Attribute>(rune_cfg->add_attributes_0 * (grade_cfg->special_add * ROLE_ATTR_PER));

				m_attrs_add.AddValue(real_type_0, add_value);
				m_attrs_add.AddValue(real_type_0, special_add_value);
			}

			int real_type_1 = 0;
			if (this->TransformAttrType(rune_cfg->attr_type_1, real_type_1) && CharIntAttrs::IsRoleBaseAttr(real_type_1))
			{
// 				m_attrs_add.m_attrs[real_type_1] += static_cast<Attribute>(rune_cfg->add_attributes_1 * (1 + (rune_awaken.add_per * ROLE_ATTR_PER)));
// 				m_attrs_add.m_attrs[real_type_1] += static_cast<Attribute>(rune_cfg->add_attributes_1 * (grade_cfg->special_add * ROLE_ATTR_PER));

				Attribute add_value = static_cast<Attribute>(rune_cfg->add_attributes_1 * (1 + (rune_awaken.add_per * ROLE_ATTR_PER)));
				Attribute special_add_value = static_cast<Attribute>(rune_cfg->add_attributes_1 * (grade_cfg->special_add * ROLE_ATTR_PER));

				m_attrs_add.AddValue(real_type_1, add_value);
				m_attrs_add.AddValue(real_type_1, special_add_value);
			}

			m_other_capability += rune_cfg->other_capability;
		}

		// 人物大目标固定属性
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_RUNE_SYSTEM, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_RUNE_SYSTEM, m_other_capability);
}

void RuneSystem::ReCalRuneEffct()
{
	m_special_buff_list.clear();

	// 符文基础效果加成
	for (int grid_index = 0; grid_index < RUNE_SYSTEM_SLOT_MAX_NUM && grid_index < LOGIC_CONFIG->GetRuneSystemCfg().GetRuneSlotNum(); ++grid_index)
	{
		RuneParam &rune = m_param.rune_grid[grid_index];
		RuneAwakenParm &rune_awaken = m_param.rune_grid_awaken[grid_index];
		if (rune.IsEmpty())
		{
			continue;
		}

		const RuneSystemAttrCfg *rune_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune.quality, rune.type, rune.level);
		if (NULL == rune_cfg)
		{
			continue;
		}

		this->ActiveSpecialEffectByRuneType(rune_cfg->attr_type_0, static_cast<double>(rune_cfg->add_attributes_0), rune_awaken.add_per, grid_index);
		this->ActiveSpecialEffectByRuneType(rune_cfg->attr_type_1, static_cast<double>(rune_cfg->add_attributes_1), rune_awaken.add_per, grid_index);
	}
}

void RuneSystem::ReCalcSpecialRoleBigGoalAttr()
{
	int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE);
	if (add_value <= 0)   // 未激活
	{
		return;
	}

	std::map<short, double>  tmp_special_buff_list;

	int real_type_0 = 0;
	int real_type_1 = 0;
	CharIntAttrs total_attrs; // 装备总属性
	for (int grid_index = 0; grid_index < RUNE_SYSTEM_SLOT_MAX_NUM && grid_index < LOGIC_CONFIG->GetRuneSystemCfg().GetRuneSlotNum(); ++grid_index)
	{
		RuneParam &rune = m_param.rune_grid[grid_index];
		if (rune.IsEmpty())
		{
			continue;
		}

		const RuneSystemAttrCfg *base_rune_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune.quality, rune.type, 1);   // 只算刚激活的，不算升级
		if (base_rune_cfg != nullptr)
		{
			real_type_0 = 0;
			if (this->TransformAttrType(base_rune_cfg->attr_type_0, real_type_0) && CharIntAttrs::IsRoleBaseAttr(real_type_0))
			{
				//total_attrs.m_attrs[real_type_0] += static_cast<Attribute>(base_rune_cfg->add_attributes_0);
				total_attrs.AddValue(real_type_0, base_rune_cfg->add_attributes_0);
			}
			else
			{
				short special_effect = this->GetSpecialEffectByRuneType(base_rune_cfg->attr_type_0);
				if (special_effect > RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_INVALID)
				{
					tmp_special_buff_list[special_effect] += base_rune_cfg->add_attributes_0;
				}
			}

			real_type_1 = 0;
			if (this->TransformAttrType(base_rune_cfg->attr_type_1, real_type_1) && CharIntAttrs::IsRoleBaseAttr(real_type_1))
			{
				//total_attrs.m_attrs[real_type_1] += static_cast<Attribute>(base_rune_cfg->add_attributes_1);
				total_attrs.AddValue(real_type_1, base_rune_cfg->add_attributes_1);
			}
			else
			{
				short special_effect = this->GetSpecialEffectByRuneType(base_rune_cfg->attr_type_1);
				if (special_effect > RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_INVALID)
				{
					tmp_special_buff_list[special_effect] += base_rune_cfg->add_attributes_1;
				}
			}
		}
	}

	for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
	{
		if (total_attrs.m_attrs[i] > 0)
		{
			total_attrs.m_attrs[i] = static_cast<Attribute>(total_attrs.m_attrs[i] * (add_value * ROLE_ATTR_PER));
		}
	}

	m_attrs_add.Add(total_attrs.m_attrs);

	for (auto &it : tmp_special_buff_list)
	{
		double value = it.second * (add_value * ROLE_ATTR_PER);
		this->AddSpecialEffect(it.first, value);
	}
}

void RuneSystem::OperaationReq(const Protocol::CSRuneSystemReq *req)
{
	switch (req->req_type)
	{
	case Protocol::RUNE_SYSTEM_REQ_TYPE_ALL_INFO:
		{
			this->SendAllInfo();
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_BAG_ALL_INFO:
		{
			this->SendVirtualBagAllInfo();
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_RUNE_GRID_ALL_INFO:
		{
			this->SendRuneGridAllInfo();
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_DISPOSE_ONE:
		{
			this->DisposeOne(req->param1);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_COMPOSE:
		{
			bool is_bag_index1 = (0 != req->param2) ? true : false;
			bool is_bag_index2 = (0 != req->param4) ? true : false;
			this->ReqComposeRune(req->param1, is_bag_index1, req->param3, is_bag_index2);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_SET_RUAN:
		{
			this->ReqSetRune(req->param1, req->param2);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_XUNBAO_ONE:
		{
			bool is_auto_buy = (0 != req->param1);
			this->ReqXunBaoOneTimes(is_auto_buy);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_XUNBAO_TEN:
		{
			bool is_auto_buy = (0 != req->param1);
			this->ReqXunBaoTenTimes(is_auto_buy);
		}
		break;


	case Protocol::RUNE_SYSTEM_REQ_TYPE_UPLEVEL:
		{
			this->ReqUpRuneLevel(req->param1);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_CONVERT:
		{
			this->ReqConvert(req->param1);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_OTHER_INFO:
		{
			this->SendOtherInfo();
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_AWAKEN:
		{
			this->ReqAwaken(req->param1, req->param2);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_AWAKEN_CALC_REQ:
		{
			this->ReqRecaleAwaken();
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_RAND_ZHILING_SLOT:
		{
			if (this->IsOpenLianhun())
			{
				this->RandZhulingSlot(req->param1);
				this->Zhuling(req->param1);
			}
			else
			{
				m_role->NoticeNum(errornum::EN_RUNE_TOWER_LAYER_NOT_ENOUGH);
			}
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_ZHULING:
		{
			this->Zhuling(req->param1);
		}
		break;

	default:
		break;
	}
}

double RuneSystem::GetSpecialEffectValuPercentByType(int effect_type)
{
	std::map<short, double>::iterator it_map = m_special_buff_list.find(effect_type);
	if (it_map != m_special_buff_list.end())
	{
		return it_map->second;
	}

	return 0;
}

void RuneSystem::ReqUpRuneLevel(int grid_index)
{
	if (this->IsInvalidGridIndex(grid_index))
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_UPLEVEL_RUNE, 0, 0);
		return;
	}

	RuneParam &rune = m_param.rune_grid[grid_index];
	if (!this->IsCanUpLevelRune(rune))
	{
		return;
	}

	m_role->SendOperateResult(Protocol::SCOperateResult::OP_UPLEVEL_RUNE, 1);
	this->SendOtherInfo();
	this->SendRuneGridAllInfo();
}

bool RuneSystem::IsCanUpLevelRune(RuneParam &rune)
{
	const RuneLevelOpenCfg *rune_level_open_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneLevelOpenCfg(rune.level);
	if (rune_level_open_cfg != nullptr)
	{
		if (m_param.fb_pass_layer < rune_level_open_cfg->need_rune_tower_layer)
		{
			//m_role->NoticeNum(errornum::EN_RUNE_TOWER_LAYER_NOT_ENOUGH);
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_UPLEVEL_RUNE, 0, 1, rune_level_open_cfg->need_rune_tower_layer);
			return false;
		}
	}

	const RuneSystemAttrCfg *level_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune.quality, rune.type, rune.level);
	if (NULL == level_cfg)
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_UPLEVEL_RUNE, 0, 2);
		return false;
	}
	const RuneSystemAttrCfg *next_level_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune.quality, rune.type, rune.level + 1);
	if (NULL == next_level_cfg)
	{
		//m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_MAX_LEVEL);
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_UPLEVEL_RUNE, 0, 2);
		return false;
	}

	//检查
	if (level_cfg->uplevel_need_jinghua <= 0 || level_cfg->uplevel_need_jinghua > m_param.rune_jinghua)
	{
		//m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_NOR_ENOUGH_SCORE);
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_UPLEVEL_RUNE, 0, 3);
		return false;
	}

	//消耗
	m_param.rune_jinghua -= level_cfg->uplevel_need_jinghua;

	//给予
	++rune.level;

	this->ReCalRuneEffct();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_RESON_RUNE_SYSTEM);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
	//m_role->NoticeNum(noticenum::NT_RUNE_SYSTEM_RUNE_UP_LEVEL_SUCC);

	//传闻
	if (next_level_cfg->broadcast)
	{
		ItemID rune_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune.quality, rune.type);
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_runesystem_up_level_send_to_world,
			m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), rune_id, rune.level);

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	EventHandler::Instance().OnRuneUplevel(m_role, rune.level);

	gamelog::g_log_rune_system.printf(LL_INFO, "RuneSystem::ReqUpRuneLevel user[%d %s],  rune[quality:%d, type:%d, level:%d], dec_jinghua[%d], now_jinghua[%d]",
		m_role->GetUID(), m_role->GetName(), rune.quality, rune.type, rune.level, level_cfg->uplevel_need_jinghua, m_param.rune_jinghua);

	return true;
}

void RuneSystem::ReqConvert(ItemID rune_id)
{
	if (!this->CheckHasSpace())
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_BAG_NO_ENOUGH_SPACE);
		return;
	}

	m_rune_id_set.clear();
	// 检查开放的符文id
	LOGIC_CONFIG->GetRuneSystemCfg().GetOpenRuneId(m_param.fb_pass_layer, m_rune_id_set);

	std::set<ItemID>::iterator it = m_rune_id_set.find(rune_id);
	if (it == m_rune_id_set.end())
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_RUNE_NOT_OPEN);
		return;
	}

	const RuneSystemRuneFetchCfg *convert_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdCfg(rune_id);
	if (NULL == convert_cfg)
	{
		return;
	}

	if (m_param.rune_suipian < convert_cfg->convert_consume_rune_suipian)
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_SUIPIAN_NOT_ENOUGH);
		return;
	}

	const RuneSystemRealIdCfg *id_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(rune_id);
	if (NULL == id_cfg)
	{
		return;
	}

	// 消耗
	m_param.rune_suipian -= convert_cfg->convert_consume_rune_suipian;

	// 给予
	RuneParam rune;
	rune.quality = id_cfg->quality;
	rune.type = id_cfg->type;
	rune.level = 1;

	this->SendOtherInfo();
	this->PutRuneToVirtualBag(rune, 0, Protocol::RUNE_SYSTEM_INFO_TYPE_CONVERT_INFO, "RuneSystem::ReqConvert");
	m_role->NoticeNum(noticenum::NT_RUNE_SYSTEM_CONVERT_SUCC);

	gamelog::g_log_rune_system.printf(LL_INFO, "RuneSystem::ReqConvert Succ user[%d %s], consume_suipian[%d], role_suipian[%d], rune_id[%d], rune[quality:%d, type:%d, level:%d]",
		m_role->GetUID(), m_role->GetName(), convert_cfg->convert_consume_rune_suipian, m_param.rune_suipian, rune_id, rune.quality, rune.type, rune.level);
}

void RuneSystem::ReqComposeRune(int index1, bool index1_is_bag, int index2, bool index2_is_bag)
{
	if (m_param.fb_pass_layer < LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().rune_compose_need_layer)
	{
		return;
	}

	// 两个索引都是符文格子索引
	if (!index1_is_bag && !index2_is_bag)
	{
		if (IsInvalidGridIndex(index1) || IsInvalidGridIndex(index2) || index1 == index2)
		{
			return;
		}

		RuneParam &rune1 = m_param.rune_grid[index1];
		RuneParam &rune2 = m_param.rune_grid[index2];
		if (rune1.IsEmpty() || rune2.IsEmpty())
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_RUNE_COMPOSE_NUM_LESS);
			return;
		}

		if (rune1.quality != rune2.quality)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_CAN_NOR_COMPOSE_DIFF_QUALITY);
			return;
		}

		ItemID rune1_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune1.quality, rune1.type);
		ItemID rune2_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune2.quality, rune2.type);
		if (rune1_id <= 0 || rune2_id <= 0)
		{
			return;
		}

		const RuneSystemComposeCfg *compose_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetComposeCfg(rune1_id, rune2_id);
		if (NULL == compose_cfg)
		{
			return;
		}

		if (m_param.magic_crystal < compose_cfg->magic_crystal_num)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_MAGIC_CRYSTAL_ENOUGH);
			return;
		}

		const RuneSystemRealIdCfg *id_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(compose_cfg->get_rune_id);
		if (NULL == id_cfg)
		{
			return;
		}

		const RuneSystemAttrCfg *attr_cfg1 = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune1.quality, rune1.type, rune1.level);
		const RuneSystemAttrCfg *attr_cfg2 = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune2.quality, rune2.type, rune2.level);
		if (NULL == attr_cfg1 || NULL == attr_cfg2)
		{
			return;
		}

		// 记录精华
		int rune_jinghua = attr_cfg1->dispose_fetch_jinghua + attr_cfg2->dispose_fetch_jinghua;

		// 消耗
		rune1.Reset();
		rune2.Reset();
		m_param.magic_crystal -= compose_cfg->magic_crystal_num;

		// 给予
		rune1.quality = id_cfg->quality;
		rune1.type = id_cfg->type;
		rune1.level = this->GetRuneLevelByJingHua(id_cfg->quality, id_cfg->type, rune_jinghua);//计算等级

		this->SendOtherInfo();
		this->SendRuneGridAllInfo();
		this->ReCalRuneEffct();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_RESON_RUNE_SYSTEM);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
		m_role->NoticeNum(noticenum::NT_RUNE_SYSTEM_COMPOSE_SUCC);
		this->SendComposeInfo();
	}

	// 两个索引都是背包里的索引
	else if (index1_is_bag && index2_is_bag)
	{
		if (IsInvalidVirtualIndex(index1) || IsInvalidVirtualIndex(index2) || index1 == index2)
		{
			return;
		}

		RuneParam &rune1 = m_param.virtual_bag_list[index1];
		RuneParam &rune2 = m_param.virtual_bag_list[index2];
		if (rune1.IsEmpty() || rune2.IsEmpty())
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_RUNE_COMPOSE_NUM_LESS);
			return;
		}

		if (rune1.quality != rune2.quality)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_CAN_NOR_COMPOSE_DIFF_QUALITY);
			return;
		}

		ItemID rune1_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune1.quality, rune1.type);
		ItemID rune2_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune2.quality, rune2.type);
		if (rune1_id <= 0 || rune2_id <= 0)
		{
			return;
		}

		const RuneSystemComposeCfg *compose_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetComposeCfg(rune1_id, rune2_id);
		if (NULL == compose_cfg)
		{
			return;
		}

		if (m_param.magic_crystal < compose_cfg->magic_crystal_num)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_MAGIC_CRYSTAL_ENOUGH);
			return;
		}

		const RuneSystemRealIdCfg *id_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(compose_cfg->get_rune_id);
		if (NULL == id_cfg)
		{
			return;
		}

		const RuneSystemAttrCfg *attr_cfg1 = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune1.quality, rune1.type, rune1.level);
		const RuneSystemAttrCfg *attr_cfg2 = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune2.quality, rune2.type, rune2.level);
		if (NULL == attr_cfg1 || NULL == attr_cfg2)
		{
			return;
		}

		// 记录精华
		int rune_jinghua = attr_cfg1->dispose_fetch_jinghua + attr_cfg2->dispose_fetch_jinghua;

		// 消耗
		rune1.Reset();
		rune2.Reset();
		m_param.magic_crystal -= compose_cfg->magic_crystal_num;

		// 给予
		rune1.quality = id_cfg->quality;
		rune1.type = id_cfg->type;
		rune1.level = this->GetRuneLevelByJingHua(id_cfg->quality, id_cfg->type, rune_jinghua);//计算等级

		this->SendOtherInfo();
		this->SendVirtualBagAllInfo();
		m_role->NoticeNum(noticenum::NT_RUNE_SYSTEM_COMPOSE_SUCC);
		this->SendComposeInfo();
	}

	// 1为符文格子，2为背包
	else if (!index1_is_bag && index2_is_bag)
	{
		if (IsInvalidGridIndex(index1) || IsInvalidVirtualIndex(index2))
		{
			return;
		}

		RuneParam &rune1 = m_param.rune_grid[index1];
		RuneParam &rune2 = m_param.virtual_bag_list[index2];
		if (rune1.IsEmpty() || rune2.IsEmpty())
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_RUNE_COMPOSE_NUM_LESS);
			return;
		}

		if (rune1.quality != rune2.quality)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_CAN_NOR_COMPOSE_DIFF_QUALITY);
			return;
		}

		ItemID rune1_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune1.quality, rune1.type);
		ItemID rune2_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune2.quality, rune2.type);
		if (rune1_id <= 0 || rune2_id <= 0)
		{
			return;
		}

		const RuneSystemComposeCfg *compose_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetComposeCfg(rune1_id, rune2_id);
		if (NULL == compose_cfg)
		{
			return;
		}

		if (m_param.magic_crystal < compose_cfg->magic_crystal_num)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_MAGIC_CRYSTAL_ENOUGH);
			return;
		}

		const RuneSystemRealIdCfg *id_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(compose_cfg->get_rune_id);
		if (NULL == id_cfg)
		{
			return;
		}

		const RuneSystemAttrCfg *attr_cfg1 = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune1.quality, rune1.type, rune1.level);
		const RuneSystemAttrCfg *attr_cfg2 = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune2.quality, rune2.type, rune2.level);
		if (NULL == attr_cfg1 || NULL == attr_cfg2)
		{
			return;
		}

		// 记录精华
		int rune_jinghua = attr_cfg1->dispose_fetch_jinghua + attr_cfg2->dispose_fetch_jinghua;

		// 消耗
		rune1.Reset();
		rune2.Reset();
		m_param.magic_crystal -= compose_cfg->magic_crystal_num;

		// 给予
		rune1.quality = id_cfg->quality;
		rune1.type = id_cfg->type;
		rune1.level = this->GetRuneLevelByJingHua(id_cfg->quality, id_cfg->type, rune_jinghua);//计算等级

		this->SendOtherInfo();
		this->SendRuneGridAllInfo();
		this->SendVirtualBagAllInfo();
		this->ReCalRuneEffct();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_RESON_RUNE_SYSTEM);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
		m_role->NoticeNum(noticenum::NT_RUNE_SYSTEM_COMPOSE_SUCC);
		this->SendComposeInfo();
	}

	// 1为背包，2为符文格子
	else if (index1_is_bag && !index2_is_bag)
	{
		if (IsInvalidGridIndex(index2) || IsInvalidVirtualIndex(index1))
		{
			return;
		}

		RuneParam &rune1 = m_param.virtual_bag_list[index1];
		RuneParam &rune2 = m_param.rune_grid[index2];
		if (rune1.IsEmpty() || rune2.IsEmpty())
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_RUNE_COMPOSE_NUM_LESS);
			return;
		}

		if (rune1.quality != rune2.quality)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_CAN_NOR_COMPOSE_DIFF_QUALITY);
			return;
		}

		ItemID rune1_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune1.quality, rune1.type);
		ItemID rune2_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune2.quality, rune2.type);
		if (rune1_id <= 0 || rune2_id <= 0)
		{
			return;
		}

		const RuneSystemComposeCfg *compose_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetComposeCfg(rune1_id, rune2_id);
		if (NULL == compose_cfg)
		{
			return;
		}

		if (m_param.magic_crystal < compose_cfg->magic_crystal_num)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_MAGIC_CRYSTAL_ENOUGH);
			return;
		}

		const RuneSystemRealIdCfg *id_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(compose_cfg->get_rune_id);
		if (NULL == id_cfg)
		{
			return;
		}

		const RuneSystemAttrCfg *attr_cfg1 = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune1.quality, rune1.type, rune1.level);
		const RuneSystemAttrCfg *attr_cfg2 = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune2.quality, rune2.type, rune2.level);
		if (NULL == attr_cfg1 || NULL == attr_cfg2)
		{
			return;
		}

		// 记录精华
		int rune_jinghua = attr_cfg1->dispose_fetch_jinghua + attr_cfg2->dispose_fetch_jinghua;

		// 消耗
		rune1.Reset();
		rune2.Reset();
		m_param.magic_crystal -= compose_cfg->magic_crystal_num;

		// 给予
		rune2.quality = id_cfg->quality;
		rune2.type = id_cfg->type;
		rune2.level = this->GetRuneLevelByJingHua(id_cfg->quality, id_cfg->type, rune_jinghua);//计算等级

		this->SendOtherInfo();
		this->SendVirtualBagAllInfo();
		this->SendRuneGridAllInfo();

		this->ReCalRuneEffct();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_RESON_RUNE_SYSTEM);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
		m_role->NoticeNum(noticenum::NT_RUNE_SYSTEM_COMPOSE_SUCC);
		this->SendComposeInfo();
	}
}

void RuneSystem::ReqAwaken(int grid_index, int awaken_type)
{
	if (grid_index < 0 || grid_index >= RUNE_SYSTEM_SLOT_MAX_NUM) return;

	if (awaken_type != RUEN_AWAKEN_TYPE_COMMON && awaken_type != RUEN_AWAKEN_TYPE_DIAMOND) return;

	if (m_role->GetKnapsack()->IsLock() || m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	// 检查符文槽是否开启
	if (0 == (m_param.rune_slot_open_flag & (1 << grid_index)))
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_SLOT_NOT_OPEN);
		return;
	}

	if (!this->IsOpenAwaken())
	{
		m_role->NoticeNum(errornum::EN_RUNE_TOWER_LAYER_NOT_ENOUGH);
		return;
	}

	const RuneSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg();

	RuneAwakenCost *cost_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAwakenCostByTimes(m_param.rune_common_awaken_times);
	if (NULL == cost_cfg) return;

	if (RUEN_AWAKEN_TYPE_COMMON == awaken_type)
	{
		int knaspack_num = m_role->GetKnapsack()->GetItemNum(cost_cfg->common_awaken_item.item_id);
		if (knaspack_num < cost_cfg->common_awaken_item.num)
		{
			Protocol::SCLackItem li;
			li.item_id = cost_cfg->common_awaken_item.item_id;
			li.item_count = cost_cfg->common_awaken_item.num - knaspack_num;
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&li, sizeof(li));
			return;
		}
	}
	else if (RUEN_AWAKEN_TYPE_DIAMOND == awaken_type)
	{
		cost_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAwakenCostByTimes(m_param.rune_gold_awaken_times);
		if (NULL == cost_cfg) return;

		if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cost_cfg->gold_cost))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	RuneSystemAwakenLimitCfg * limit_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAwakenLimitByLevel(m_param.fb_pass_layer);
	if (NULL == limit_cfg) return;

	std::map<int ,RuneSystemAwakenCfg> &awaken_map = LOGIC_CONFIG->GetRuneSystemCfg().GetAwakenCfgMap();
	if (static_cast<unsigned int>(awaken_map.size()) <= 0) return;

	RuneAwakenParm &rune_grid_awaken = m_param.rune_grid_awaken[grid_index];

	int awaken_common_total_pro = 0;
	int awaken_baodi_total_pro = 0;
	int awaken_diamond_total_pro = 0;
	std::map<int ,RuneSystemAwakenCfg> temp_awaken_map;
	std::map<int ,RuneSystemAwakenCfg>::iterator begin_iter = awaken_map.begin();
	for(; begin_iter != awaken_map.end(); begin_iter++)
	{
		switch(begin_iter->second.awaken_type)
		{
		case RUEN_AWAKEN_REWARD_TYPE_GONGJI:
			{
				if (rune_grid_awaken.gongji >= limit_cfg->gongji_limit)
					continue;
			}
			break;

		case RUEN_AWAKEN_REWARD_TYPE_FANGYU:
			{
				if (rune_grid_awaken.fangyu >= limit_cfg->fangyu_limit)
					continue;
			}
			break;

		case RUEN_AWAKEN_REWARD_TYPE_MAXHP:
			{
				if (rune_grid_awaken.maxhp >= limit_cfg->maxhp_limit)
					continue;
			}
			break;

		case RUEN_AWAKEN_REWARD_TYPE_ADD_PER:
			{
				if (rune_grid_awaken.add_per >= limit_cfg->addper_limit)
					continue;
			}
			break;
		}

		if (RUEN_AWAKEN_TYPE_COMMON == awaken_type)
		{
			if (begin_iter->second.common_pro <= 0)
			{
				continue;
			}

			awaken_common_total_pro += begin_iter->second.common_pro;

			if (RUEN_AWAKEN_REWARD_TYPE_GONGJI <= begin_iter->second.awaken_type && RUEN_AWAKEN_REWARD_TYPE_ADD_PER >= begin_iter->second.awaken_type)
			{
				awaken_baodi_total_pro += begin_iter->second.common_pro;
			}
		}
		else if (RUEN_AWAKEN_TYPE_DIAMOND == awaken_type)
		{
			if (begin_iter->second.diamond_pro <= 0)
			{
				continue;
			}

			awaken_diamond_total_pro += begin_iter->second.diamond_pro;
		}

		temp_awaken_map[begin_iter->second.awaken_seq] = begin_iter->second;
	}

	int awaken_total_pro = 0;
	bool is_baodi = false;
	if (RUEN_AWAKEN_TYPE_COMMON == awaken_type)
	{
		awaken_total_pro = awaken_common_total_pro;

		if (m_param.baodi >= other_cfg.awaken_baodi)
		{
			if (awaken_baodi_total_pro > 0)
			{
				is_baodi = true;
				awaken_total_pro = awaken_baodi_total_pro;
			}
		}
	}
	else if (RUEN_AWAKEN_TYPE_DIAMOND == awaken_type)
	{
		awaken_total_pro = awaken_diamond_total_pro;
	}

	if (awaken_total_pro <= 0 ||static_cast<unsigned int>(temp_awaken_map.size()) <= 0)
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_AWAKEN_LIMIT);
		return;
	}

	int awaken_rand_value = RandomNum(awaken_total_pro);
	int awaken_seq = RUEN_AWAKEN_REWARD_TYPE_INVALID;
	std::map<int ,RuneSystemAwakenCfg>::iterator temp_begin_iter = temp_awaken_map.begin();
	for(; temp_begin_iter != temp_awaken_map.end(); temp_begin_iter++)
	{
		int temp_pro = 0;
		if (RUEN_AWAKEN_TYPE_COMMON == awaken_type)
		{
			temp_pro = temp_begin_iter->second.common_pro;

			if (is_baodi && temp_begin_iter->second.awaken_type >= RUEN_AWAKEN_REWARD_TYPE_JINGHUA)
			{
				temp_pro = 0;
			}
		}
		else
		{
			temp_pro = temp_begin_iter->second.diamond_pro;
		}
		

		if (awaken_rand_value < temp_pro)
		{
			awaken_seq = temp_begin_iter->second.awaken_seq;
			break;
		}
		else
		{
			awaken_rand_value -= temp_pro;
		}
	}
		
	std::map<int ,RuneSystemAwakenCfg>::iterator awaken_item = awaken_map.find(awaken_seq);
	if (awaken_item == awaken_map.end()) return;

	short is_need_recal = 0;
	switch(awaken_item->second.awaken_type)
	{
	case RUEN_AWAKEN_REWARD_TYPE_GONGJI:
		{
			rune_grid_awaken.gongji += awaken_item->second.add_value;
			if (rune_grid_awaken.gongji > limit_cfg->gongji_limit)
			{
				rune_grid_awaken.gongji = limit_cfg->gongji_limit;
			}

			is_need_recal = 1;
			m_param.is_need_recalc_awaken = 1;

			if (RUEN_AWAKEN_TYPE_COMMON == awaken_type)
			{
				m_param.baodi = 0;
			}
		}
		break;

	case RUEN_AWAKEN_REWARD_TYPE_FANGYU:
		{
			rune_grid_awaken.fangyu += awaken_item->second.add_value;
			if (rune_grid_awaken.fangyu > limit_cfg->fangyu_limit)
			{
				rune_grid_awaken.fangyu = limit_cfg->fangyu_limit;
			}

			is_need_recal = 1;
			m_param.is_need_recalc_awaken = 1;
			
			if (RUEN_AWAKEN_TYPE_COMMON == awaken_type)
			{
				m_param.baodi = 0;
			}
		}
		break;

	case RUEN_AWAKEN_REWARD_TYPE_MAXHP:
		{
			rune_grid_awaken.maxhp += awaken_item->second.add_value;
			if (rune_grid_awaken.maxhp > limit_cfg->maxhp_limit)
			{
				rune_grid_awaken.maxhp = limit_cfg->maxhp_limit;
			}

			is_need_recal = 1;
			m_param.is_need_recalc_awaken = 1;
			
			if (RUEN_AWAKEN_TYPE_COMMON == awaken_type)
			{
				m_param.baodi = 0;
			}
		}
		break;

	case RUEN_AWAKEN_REWARD_TYPE_ADD_PER:
		{
			rune_grid_awaken.add_per += awaken_item->second.add_value;
			if (rune_grid_awaken.add_per > limit_cfg->addper_limit)
			{
				rune_grid_awaken.add_per = limit_cfg->addper_limit;
			}

			is_need_recal = 1;
			m_param.is_need_recalc_awaken = 1;
			
			if (RUEN_AWAKEN_TYPE_COMMON == awaken_type)
			{
				m_param.baodi = 0;
			}
		}
		break;

	case RUEN_AWAKEN_REWARD_TYPE_JINGHUA:
		{
			this->AddRuneJingHua(awaken_item->second.add_value);
			m_param.baodi++;
		}
		break;

	case RUEN_AWAKEN_REWARD_TYPE_MOJING:
		{
			m_role->GetRoleShop()->AddChestShopMojing(awaken_item->second.add_value, "RuneGrideAwaken");
			m_param.baodi++;
		}
		break;

	case RUEN_AWAKEN_REWARD_TYPE_EXP:
		{
			m_role->AddExp((m_role->GetLevel() + 50) * static_cast<long long>(awaken_item->second.add_value), "RuneGrideAwaken", Role::EXP_ADD_REASON_DEFAULT);
			m_param.baodi++;
		}
		break;

	default:
		return;
	}

	if (RUEN_AWAKEN_TYPE_COMMON == awaken_type)
	{
		m_param.rune_common_awaken_times++;
		m_role->GetKnapsack()->ConsumeItem(cost_cfg->common_awaken_item.item_id, cost_cfg->common_awaken_item.num, "RuneGrideAwaken");
	}
	else if (RUEN_AWAKEN_TYPE_DIAMOND == awaken_type)
	{
		m_param.rune_gold_awaken_times++;
		m_role->GetKnapsack()->GetMoney()->UseGold(cost_cfg->gold_cost, "RuneGrideAwaken");	
	}
	
	this->SendOtherInfo();
	this->SendAwakenInfo(awaken_seq, is_need_recal);
}

void RuneSystem::ReqRecaleAwaken()
{
	if (1 != m_param.is_need_recalc_awaken) return;

	this->ReCalRuneEffct();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_RESON_RUNE_SYSTEM);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
	this->SendRuneGridAllInfo();;
	m_param.is_need_recalc_awaken = 0;
}


int RuneSystem::GetRuneLevelByJingHua(int quality, int type, int jinghua)
{
	for (int i = RUNE_SYSTEM_RUNE_MAX_LEVEL; i > 0; --i)
	{
		if (jinghua >= LOGIC_CONFIG->GetRuneSystemCfg().GetLevelConsumeTotalJinghua(quality, type, i))
		{
			return i;
		}
	}

	return 1;
}

void RuneSystem::ReqXunBaoOneTimes(bool is_auto_buy)
{
	if (!this->CheckHasSpace())
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_BAG_NO_ENOUGH_SPACE);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ItemID item_id = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().xunbao_consume_itemid;
	int need_num = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().xunbao_one_consume_num;

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	// 优先使用免费次数
	bool is_free = false;
	if (m_param.free_xunbao_times > 0)
	{
		is_free = true;
	}
	else
	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;

		//材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = item_id;
		stuff_list[stuff_count].num = need_num;
		stuff_list[stuff_count].buyable = (0 == is_auto_buy) ? false : true;

		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return;
		}

		//元宝
		Money *money = knapsack->GetMoney();
		if (consume_list.need_gold_num > 0 && !money->GoldMoreThan(consume_list.need_gold_num))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	m_rune_fetch_weight_vec.clear();
	// 检查开放的符文id权重配置
	LOGIC_CONFIG->GetRuneSystemCfg().GetRuneFetchWeightCfg(m_param.fb_pass_layer, m_rune_fetch_weight_vec);

	int total_weight = 0;
	int tmp_total_times = m_param.total_xunbao_times + 1;
	for (std::vector<RuneSystemRuneFetchCfg>::const_iterator it = m_rune_fetch_weight_vec.begin(); it != m_rune_fetch_weight_vec.end(); ++it)
	{
		int real_weight = this->GetRuneRealWeight(*it, tmp_total_times, false);
		total_weight += real_weight;
	}

	if (total_weight <= 0) return;
	int rand_val = RandomNum(total_weight);
	ItemID get_rune_id = 0;

	for (std::vector<RuneSystemRuneFetchCfg>::const_iterator it = m_rune_fetch_weight_vec.begin(); it != m_rune_fetch_weight_vec.end(); ++it)
	{
		int real_weight = this->GetRuneRealWeight(*it, tmp_total_times, false);
		if (real_weight <= 0)
		{
			continue;
		}

		if (rand_val < real_weight)
		{
			get_rune_id = (*it).rune_id;
			break;
		}

		rand_val -= real_weight;
	}

	const RuneSystemRealIdCfg *id_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(get_rune_id);
	if (NULL == id_cfg)
	{
		return;
	}

	// 消耗
	if (is_free)
	{
		unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		unsigned int free_cd_s = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().free_cd;

		if (0 == m_param.next_free_xunbao_timestamp)
		{
			ItemID first_xunbao_rune = LOGIC_CONFIG->GetRuneSystemCfg().GetFirstXunbaoRuneId();
			if (first_xunbao_rune > 0)
			{
				id_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(first_xunbao_rune);
				if (NULL == id_cfg) return;
			}
		}

		m_param.free_xunbao_times = 0;
		m_param.next_free_xunbao_timestamp = now_second + free_cd_s;
	}
	else
	{
		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "RuneSystem::ReqXunBaoOneTimes"))
		{
			return;
		}
	}

	// 给予
	m_param.total_xunbao_times++;

	int suipian_low = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().rune_suipian_num_low;
	int suipian_high = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().rune_suipian_num_high;
	int role_get_suipian = suipian_low;
	if (suipian_high > suipian_low)
	{
		role_get_suipian = RandomNum(suipian_low, suipian_high + 1);
	}

	// 碎片
	m_param.rune_suipian += role_get_suipian;

	int magic_crystal = 0;
	// 魔晶
	if (0 != (m_param.rune_slot_open_flag & (1 << 15)))
	{
		m_param.magic_crystal += LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().xunbao_one_magic_crystal;
		magic_crystal = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().xunbao_one_magic_crystal;
	}

	RuneParam rune;
	rune.Reset();
	rune.quality = static_cast<char>(id_cfg->quality);
	rune.type = static_cast<char>(id_cfg->type);
	rune.level = 1;

	{
		// 其他信息
		Protocol::SCRuneSystemOtherInfo otio;
		memset(otio.suipian, 0, sizeof(otio.suipian));

		otio.pass_layer = m_param.fb_pass_layer;
		otio.rune_jinghua = m_param.rune_jinghua;
		otio.rune_suipian = m_param.rune_suipian;
		otio.magic_crystal = m_param.magic_crystal;
		otio.suipian[0] = static_cast<char>(role_get_suipian);
		otio.reserve_sh = 0;

		otio.next_free_xunbao_timestamp = m_param.next_free_xunbao_timestamp;
		otio.total_xunbao_times = m_param.total_xunbao_times;
		otio.rune_slot_open_flag = m_param.rune_slot_open_flag;
		otio.free_xunbao_times = m_param.free_xunbao_times;
		otio.reserve_ch = 0;
		otio.rune_awaken_times = m_param.rune_gold_awaken_times;

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&otio, sizeof(otio));
	}

	this->PutRuneToVirtualBag(rune, magic_crystal, Protocol::RUNE_SYSTEM_INFO_TYPE_RUNE_XUNBAO_INFO, "RuneSystem::ReqXunBaoOneTimes");

	// 传闻
	ItemID rune_id = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneIdByQualityAndTypeCfg(rune.quality, rune.type);
	if (id_cfg->is_notice && rune_id > 0)
	{
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_runesystem_one_xunbao_notice,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), rune_id);
		if (sendlen > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
		}
	}

	gamelog::g_log_rune_system.printf(LL_INFO, "RuneSystem::ReqXunBaoOneTimes user[%d %s], item_id[%d], item_num[%d], get_suipian[%d], role_suipian[%d], rune[quality:%d, type:%d, level:%d], magic_crystal[%d]",
		m_role->GetUID(), m_role->GetName(), item_id, need_num, role_get_suipian, m_param.rune_suipian, rune.quality, rune.type, rune.level, m_param.magic_crystal);
}

void RuneSystem::ReqXunBaoTenTimes(bool is_auto_buy)
{
	const int ten = 10;
	if (!this->CheckHasSpace(ten))
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_BAG_NO_ENOUGH_SPACE);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ItemID item_id = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().xunbao_consume_itemid;
	int need_num = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().xunbao_ten_consume_num;

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	{
		// 获取可消耗的背包材料列表

		short stuff_count = 0;

		//材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = item_id;
		stuff_list[stuff_count].num = need_num;
		stuff_list[stuff_count].buyable = (0 == is_auto_buy) ? false : true;

		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return;
		}

		//元宝
		Money *money = knapsack->GetMoney();

		if (consume_list.need_gold_num > 0 && !money->GoldMoreThan(consume_list.need_gold_num))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	m_rune_fetch_weight_vec.clear();
	// 检查开放的符文id权重配置
	LOGIC_CONFIG->GetRuneSystemCfg().GetRuneFetchWeightCfg(m_param.fb_pass_layer, m_rune_fetch_weight_vec);

	ItemID get_rune_id_list[ten]; memset(get_rune_id_list, 0, sizeof(get_rune_id_list));

	// 得到一个红色符文
// 	int red_rune_total_weight = 0;
// 	for (std::set<ItemID>::const_iterator it = m_rune_id_set.begin(); it != m_rune_id_set.end(); ++it)
// 	{
// 		const RuneSystemRuneFetchCfg *fetch_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneFetchCfg(*it);
// 		if (NULL != fetch_cfg && 1 == fetch_cfg->is_red_rune)
// 		{
// 			red_rune_total_weight += fetch_cfg->red_rune_weight;
// 		}
// 	}
// 
// 	if (red_rune_total_weight <= 0) return;
// 	int rand_red_val = RandomNum(red_rune_total_weight);
// 	for (std::set<ItemID>::const_iterator it = m_rune_id_set.begin(); it != m_rune_id_set.end(); ++it)
// 	{
// 		const RuneSystemRuneFetchCfg *fetch_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRuneFetchCfg(*it);
// 		if (NULL != fetch_cfg && 1 == fetch_cfg->is_red_rune)
// 		{
// 			if (rand_red_val < fetch_cfg->red_rune_weight)
// 			{
// 				get_rune_id_list[0] = *it;
// 				break;
// 			}
// 
// 			rand_red_val -= fetch_cfg->red_rune_weight;
// 		}
// 	}

	int tmp_total_times = m_param.total_xunbao_times + 1;
	for (int i = 0; i < ten; ++i)
	{
		// 计算总权重
		int total_weight = 0;
		for (std::vector<RuneSystemRuneFetchCfg>::const_iterator it = m_rune_fetch_weight_vec.begin(); it != m_rune_fetch_weight_vec.end(); ++it)
		{
			int real_weight = this->GetRuneRealWeight(*it, tmp_total_times, true);
			total_weight += real_weight;
		}

		if (total_weight <= 0) return;

		// 随机
		int rand_val = RandomNum(total_weight);

		for (std::vector<RuneSystemRuneFetchCfg>::const_iterator it = m_rune_fetch_weight_vec.begin(); it != m_rune_fetch_weight_vec.end(); ++it)
		{
			int real_weight = this->GetRuneRealWeight(*it, tmp_total_times, true);
			if (real_weight <= 0)
			{
				continue;
			}

			if (rand_val < real_weight)
			{
				get_rune_id_list[i] = (*it).rune_id;
				break;
			}

			rand_val -= real_weight;
		}

		tmp_total_times++;
	}

	RuneParam rune_list[ten];
	bool is_notice_list[ten]= {false};
	for (int i = 0; i < ten; ++i)
	{
		const RuneSystemRealIdCfg *id_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(get_rune_id_list[i]);
		if (NULL != id_cfg)
		{
			rune_list[i].quality = static_cast<char>(id_cfg->quality);
			rune_list[i].type = static_cast<char>(id_cfg->type);
			rune_list[i].level = 1;
			is_notice_list[i] = id_cfg->is_notice;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "RuneSystem::ReqXunBaoTenTimes"))
	{
		return;
	}

	// 给予
	m_param.total_xunbao_times += ten;

	int suipian_low = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().rune_suipian_num_low;
	int suipian_high = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().rune_suipian_num_high;

	int role_get_suipian = 0;
	char suipian[RUNE_SYSTEM_XUNBAO_RUNE_MAX_COUNT]; memset(suipian, 0, sizeof(suipian));
	for (int i = 0; i < ten; ++i)
	{
		suipian[i] = suipian_low;
		if (suipian_high > suipian_low)
		{
			suipian[i] = RandomNum(suipian_low, suipian_high + 1);
		}
		role_get_suipian += static_cast<int>(suipian[i]);
	}

	// 碎片
	m_param.rune_suipian += role_get_suipian;

	int magic_crystal = 0;
	// 魔晶
	if (0 != (m_param.rune_slot_open_flag & (1 << 15)))
	{
		m_param.magic_crystal += (ten * (LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().xunbao_one_magic_crystal));
		magic_crystal = (ten * (LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().xunbao_one_magic_crystal));
	}

	{
		Protocol::SCRuneSystemOtherInfo otio;

		UNSTD_STATIC_CHECK(sizeof(suipian) == sizeof(otio.suipian));
		memset(otio.suipian, 0, sizeof(otio.suipian));

		otio.pass_layer = m_param.fb_pass_layer;
		otio.rune_jinghua = m_param.rune_jinghua;
		otio.rune_suipian = m_param.rune_suipian;
		otio.magic_crystal = m_param.magic_crystal;
		memcpy(otio.suipian, suipian, sizeof(otio.suipian));
		otio.reserve_sh = 0;

		otio.next_free_xunbao_timestamp = m_param.next_free_xunbao_timestamp;
		otio.total_xunbao_times = m_param.total_xunbao_times;
		otio.rune_slot_open_flag = m_param.rune_slot_open_flag;
		otio.free_xunbao_times = m_param.free_xunbao_times;
		otio.reserve_ch = 0;
		otio.rune_awaken_times = m_param.rune_gold_awaken_times;

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&otio, sizeof(otio));
	}

	this->PutRuneListToVirtualBag(rune_list, ten, magic_crystal, Protocol::RUNE_SYSTEM_INFO_TYPE_RUNE_XUNBAO_INFO, "RuneSystem::ReqXunBaoTenTimes");

	// 传闻
	for (int i = 0; i < ten; ++i)
	{
		if (is_notice_list[i] && get_rune_id_list[i] > 0)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_runesystem_ten_xunbao_notice,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), get_rune_id_list[i]);
			if (sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
			}
		}
	}

	gamelog::g_log_rune_system.printf(LL_INFO, "RuneSystem::ReqXunBaoTenTimes user[%d %s], item_id[%d], item_num[%d], get_suipian[%d], role_suipian[%d], magic_crystal[%d]",
		m_role->GetUID(), m_role->GetName(), item_id, need_num, role_get_suipian, m_param.rune_suipian, m_param.magic_crystal);
}

int RuneSystem::GetRuneWeightType(int total_times)
{
	int rune_weight_type = RUNE_WEIGHT_TYPE_DEFAULT;
	
	const RuneSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg();

	if (other_cfg.total_x1_times > 0)
	{
		rune_weight_type = (total_times % other_cfg.total_x1_times) == 0 ? RUNE_WEIGHT_TYPE_X1 : RUNE_WEIGHT_TYPE_DEFAULT;
	}

	if (RUNE_WEIGHT_TYPE_DEFAULT == rune_weight_type && other_cfg.total_x_times > 0)
	{
		rune_weight_type = (total_times % other_cfg.total_x_times) == 0 ? RUNE_WEIGHT_TYPE_X : RUNE_WEIGHT_TYPE_DEFAULT;
	}

	return rune_weight_type;
}

int RuneSystem::GetRuneRealWeight(const RuneSystemRuneFetchCfg &rune_weight_cfg, int total_times, bool is_ten)
{
	int real_weight = 0;
	
	// 判断累计寻宝次数，获取实际权重
	int weight_type = this->GetRuneWeightType(total_times);
	switch (weight_type)
	{
	case RUNE_WEIGHT_TYPE_DEFAULT:
	{
		real_weight = is_ten ? rune_weight_cfg.ten_weight : rune_weight_cfg.one_weight;
	}
	break;

	case RUNE_WEIGHT_TYPE_X:
	{
		real_weight = rune_weight_cfg.total_x_times_weight;
	}
	break;

	case RUNE_WEIGHT_TYPE_X1:
	{
		real_weight = rune_weight_cfg.total_x1_times_weight;
	}
	break;
	}

	return real_weight;
}

void RuneSystem::DisposeOne(int virtual_index)
{
	if (this->IsInvalidVirtualIndex(virtual_index))
	{
		return;
	}

	RuneParam &rune = m_param.virtual_bag_list[virtual_index];
	if (rune.IsEmpty())
	{
		return;
	}

	const RuneSystemAttrCfg *attr_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune.quality, rune.type, rune.level);
	if (NULL == attr_cfg)
	{
		return;
	}

	int quality = rune.quality;
	int type = rune.type;
	int level = rune.level;

	// 消耗
	rune.Reset();

	// 给予
	++ m_empty_grids;
	m_param.rune_jinghua += attr_cfg->dispose_fetch_jinghua;

	m_role->NoticeNum(noticenum::NT_RUNE_SYSTEM_DECOMPOSE_SUCC);

	this->SendOtherInfo();
	this->SendVirtualBagAllInfo();

	gamelog::g_log_rune_system.printf(LL_INFO, "RuneSystem::DisposeOne user[%d %s], dismpose_rune[quality:%d, type:%d, level:%d], fetch_jinghua[%d], jinghua[%d]",
		m_role->GetUID(), m_role->GetName(), quality, type, level, attr_cfg->dispose_fetch_jinghua, m_param.rune_jinghua);
}

void RuneSystem::DisposeOneKey(Protocol::CSRuneSystemDisposeOneKey *req)
{
	bool is_need_notice_client = false;
	for(int i = 0; i < req->list_count && i < RUNE_SYSTEM_BAG_MAX_GRIDS; i++)
	{
		if (this->IsInvalidVirtualIndex(req->dispose_list[i]))
		{
			continue;
		}

		RuneParam &rune = m_param.virtual_bag_list[req->dispose_list[i]];
		if (rune.IsEmpty())
		{
			continue;
		}

		const RuneSystemAttrCfg *attr_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(rune.quality, rune.type, rune.level);
		if (NULL == attr_cfg)
		{
			continue;
		}

		int quality = rune.quality;
		int type = rune.type;
		int level = rune.level;

		// 消耗
		rune.Reset();

		// 给予
		++ m_empty_grids;
		m_param.rune_jinghua += attr_cfg->dispose_fetch_jinghua;
		is_need_notice_client = true;

		gamelog::g_log_rune_system.printf(LL_INFO, "RuneSystem::OneKeyDispose user[%d %s], dismpose_rune[quality:%d, type:%d, level:%d], fetch_jinghua[%d], jinghua[%d]",
			m_role->GetUID(), m_role->GetName(), quality, type, level, attr_cfg->dispose_fetch_jinghua, m_param.rune_jinghua);
	}

	if (is_need_notice_client)
	{
		this->SendOtherInfo();
		this->SendVirtualBagAllInfo();
		m_role->NoticeNum(noticenum::NT_RUNE_SYSTEM_DECOMPOSE_SUCC);
	}
}

bool RuneSystem::AddRuneByItemId(ItemID item_id, int num)
{
	if (!this->CheckHasSpace(num))
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_BAG_NO_ENOUGH_SPACE);
		return false;
	}

	const RuneSystemRealIdCfg *rune_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(item_id);
	if (NULL == rune_cfg)
	{
		return false;
	}

	RuneParam rune;
	rune.Reset();
	rune.quality = static_cast<char>(rune_cfg->quality);
	rune.type = static_cast<char>(rune_cfg->type);
	rune.level = 1;

	//this->PutRuneToVirtualBag(rune, 0, Protocol::RUNE_SYSTEM_INFO_TYPE_INVAILD, "RuneSystem::AddRuneByItemId");
	for (int i = 0; i < num && i < m_empty_grids && i < RUNE_SYSTEM_BAG_MAX_GRIDS; i++)
	{
		this->PutRuneToVirtualBag(rune, 0, Protocol::RUNE_SYSTEM_INFO_TYPE_INVAILD, "RuneSystem::AddRuneByItemId");
	}
	return true;
}

bool RuneSystem::UseRuneBox(int box_type, int level_limit)
{
	if (box_type <= RUNE_BOX_TYPE_INVALID || box_type > RUNE_BOX_TYPE_MAX || level_limit < 0)
	{
		return false;
	}

	const RuneSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg();
	switch (box_type)
	{
	case RUNE_BOX_TYPE_JINGHUA:
	{
		if (!this->CheckHasSpace(other_cfg.jinghua_box_max))
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_BAG_NO_ENOUGH_SPACE);
			return false;
		}

		int item_num = 0;
		RuneParam rune_list[RUNE_SYSTEM_BOX_REWAED_MAX_ITEM];
		if (other_cfg.jinghua_box_max > other_cfg.jinghua_box_min)
		{
			item_num = RandomNum(other_cfg.jinghua_box_min, other_cfg.jinghua_box_max + 1);
		}
		else
		{
			item_num = other_cfg.jinghua_box_min;
		}

		int count = 0;
		for (int i = 0; i < item_num && i < RUNE_SYSTEM_BOX_REWAED_MAX_ITEM; ++i)
		{
			ItemID item_id = LOGIC_CONFIG->GetRuneSystemCfg().GetOpenBoxGetItemId(box_type, level_limit);
			if (item_id <= 0)
			{
				continue;
			}

			const RuneSystemRealIdCfg *rune_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(item_id);
			if (NULL != rune_cfg)
			{
				rune_list[count].Reset();
				rune_list[count].quality = static_cast<char>(rune_cfg->quality);
				rune_list[count].type = static_cast<char>(rune_cfg->type);
				rune_list[count].level = 1;
				count++;
			}
		}

		if (count > 0)
		{
			int add_magic_crystal = 0;

			if (level_limit >= other_cfg.rune_compose_need_layer)
			{
				m_param.magic_crystal += other_cfg.jinghua_box_magic_crystal;
				add_magic_crystal = other_cfg.jinghua_box_magic_crystal;
				this->SendOtherInfo();
			}

			this->PutRuneListToVirtualBag(rune_list, count, add_magic_crystal, Protocol::RUNE_SYSTEM_INFO_TYPE_OPEN_BOX_INFO, "UseRuneBox::RuneBox");
		}
		else
		{
			return false;
		}
	}
	break;
	
	case RUNE_BOX_TYPE_RUNE_AND_JINGHUA:
	{
		if (!this->CheckHasSpace(other_cfg.rune_box_max))
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_BAG_NO_ENOUGH_SPACE);
			return false;
		}

		int item_num = 0;
		RuneParam rune_list[RUNE_SYSTEM_BOX_REWAED_MAX_ITEM];
		if (other_cfg.rune_box_max > other_cfg.rune_box_min)
		{
			item_num = RandomNum(other_cfg.rune_box_min, other_cfg.rune_box_min + 1);
		}
		else
		{
			item_num = other_cfg.rune_box_min;
		}

		int count = 0;
		for (int i = 0; i < item_num && i < RUNE_SYSTEM_BOX_REWAED_MAX_ITEM; ++i)
		{
			ItemID item_id = LOGIC_CONFIG->GetRuneSystemCfg().GetOpenBoxGetItemId(box_type, level_limit);
			if (item_id <= 0)
			{
				continue;
			}

			const RuneSystemRealIdCfg *rune_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(item_id);
			if (NULL != rune_cfg)
			{
				rune_list[count].Reset();
				rune_list[count].quality = static_cast<char>(rune_cfg->quality);
				rune_list[count].type = static_cast<char>(rune_cfg->type);
				rune_list[count].level = 1;
				count++;
			}
		}

		if (count > 0)
		{
			int add_magic_crystal = 0;

			if (level_limit >= other_cfg.rune_compose_need_layer)
			{
				m_param.magic_crystal += other_cfg.jinghua_box_magic_crystal;
				add_magic_crystal = other_cfg.jinghua_box_magic_crystal;
				this->SendOtherInfo();
			}

			this->PutRuneListToVirtualBag(rune_list, count, add_magic_crystal, Protocol::RUNE_SYSTEM_INFO_TYPE_OPEN_BOX_INFO, "UseRuneBox::JingHuaBox");
		}
		else
		{
			return false;
		}
	}
	break;

	default:
		break;
	}

	return true;
}

void RuneSystem::AddRuneJingHua(int jinghua)
{
	if (jinghua <= 0)
	{
		return;
	}

	m_param.rune_jinghua += jinghua;

	this->SendOtherInfo();
}

void RuneSystem::PutRuneTowerRewardByItemId(ItemID item_id)
{
	const RuneSystemRealIdCfg *rune_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(item_id);
	if (NULL == rune_cfg)
	{
		return;
	}

	RuneParam rune; rune.Reset();
	rune.quality = static_cast<char>(rune_cfg->quality);
	rune.type = static_cast<char>(rune_cfg->type);
	rune.level = 1;

	for (int index = 0; index < RUNE_SYSTEM_BAG_MAX_GRIDS; ++index)
	{
		RuneParam &bag_rune = m_param.virtual_bag_list[index];
		if (bag_rune.IsEmpty())
		{
			bag_rune = rune;
			--m_empty_grids;

			gamelog::g_log_rune_system.buff_printf("[quality:%d, type:%d, level:%d],", bag_rune.quality, bag_rune.type, bag_rune.level);
			break;
		}
	}

	gamelog::g_log_rune_system.buff_printf("),  RuneTowerReward[%d]", item_id);
	gamelog::g_log_rune_system.commit_buff(LL_INFO);
}

void RuneSystem::OnRuneTowerFetchTime()
{
	const RuneTowerOtherCfg &cfg = LOGIC_CONFIG->GetRuneTowerCfg().GetOtherCfg();
	if (m_param.fb_offline_time >= cfg.offline_time_max)
	{
		m_role->NoticeNum(errornum::EN_RUNE_TOWER_OFFLINE_TIME_MAX);
		return;
	}

	if (m_param.fb_fetch_time_count >= cfg.offline_time_free_count)
	{
		m_role->NoticeNum(errornum::EN_RUNE_TOWER_OFFLINE_TIME_FETCHED);
		return;
	}

	m_param.fb_fetch_time_count++;
	this->AddRuneTowerOfflineTime(cfg.offline_time_free, 1);
}

void RuneSystem::OnRuneTowerAutoFb()
{
	if (m_param.fb_today_layer >= m_param.fb_pass_layer)
	{
		return;
	}

	const RuneTowerLayerCfg *cfg = LOGIC_CONFIG->GetRuneTowerCfg().GetRuneTowerLayerCfg(m_param.fb_pass_layer);
	if (NULL == cfg)
	{
		return;
	}

	for (int i = m_param.fb_today_layer + 1; i <= m_param.fb_pass_layer; i++)
	{
		EventHandler::Instance().OnPassRuneTowerFB(m_role, i, true);
	}

	m_param.fb_today_layer = m_param.fb_pass_layer;

	this->AddRuneJingHua(cfg->normal_reward_rune_exp);
	m_role->GetKnapsack()->PutListOrMail(cfg->normal_reward_item_list, PUT_REASON_RUNE_TOWER);

	this->SendRuneTowerInfo();

	{
		Protocol::SCRuneTowerAutoReward pro;
		pro.reward_jinghua = cfg->normal_reward_rune_exp;
		pro.item_count = 0;

		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < Protocol::SCRuneTowerAutoReward::MAX_REWARD_COUNT; i++)
		{
			if (cfg->normal_reward_item_list[i].item_id > 0)
			{
				pro.item_list[i].item_id = cfg->normal_reward_item_list[i].item_id;
				pro.item_list[i].num = cfg->normal_reward_item_list[i].num;
				pro.item_count++;
			}	
		}

		int length = sizeof(pro) - (Protocol::SCRuneTowerAutoReward::MAX_REWARD_COUNT - pro.item_count) * sizeof(pro.item_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, length);
	}
}

void RuneSystem::OnRuneTowerPassLayer(int pass_layer)
{
	if (pass_layer > m_param.fb_pass_layer)
	{
		m_param.fb_pass_layer = pass_layer;

		this->CheckOpenRuneSlotAndId();
	}

	if (pass_layer > m_param.fb_today_layer)
	{
		m_param.fb_today_layer = pass_layer;
	}

	this->SendRuneTowerInfo();
	this->SendOtherInfo();
}
short RuneSystem::AddRuneTowerOfflineTime(int offline_time, short max_count)
{
	int offline_time_max = LOGIC_CONFIG->GetRuneTowerCfg().GetOtherCfg().offline_time_max;

	short add_count = 0;
	for (int i = 0; i < max_count; i++)
	{
		if (m_param.fb_offline_time >= offline_time_max)
		{
			break;
		}

		add_count++;
		m_param.fb_offline_time += offline_time;
	}

	if (add_count > 0)
	{
		this->SendRuneTowerInfo();
	}

	return add_count;
}

void RuneSystem::SendRuneTowerInfo()
{
	Protocol::SCRuneTowerInfo pro;
	pro.pass_layer = m_param.fb_pass_layer;
	pro.fb_today_layer = m_param.fb_today_layer;
	pro.offline_time = m_param.fb_offline_time;
	pro.add_exp = m_param.fb_add_exp;
	pro.add_jinghua = m_param.fb_add_jinghua;
	pro.add_equip_blue = m_param.fb_add_equip_blue;
	pro.add_equip_purple = m_param.fb_add_equip_purple;
	pro.add_equip_orange = m_param.fb_add_equip_orange;
	pro.fetch_time_count = m_param.fb_fetch_time_count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void RuneSystem::OnRuneTowerOfflineTime(unsigned int last_save_time)
{
	if (m_param.fb_offline_time <= 0 || last_save_time <= 0)
	{
		return;
	}

	int offline_time = (int)(EngineAdapter::Instance().Time() - last_save_time);
	if (offline_time < 120)
	{
		return;
	}

	if (offline_time > m_param.fb_offline_time)
	{
		offline_time = m_param.fb_offline_time;
	}

	const RuneTowerLayerCfg *cfg = LOGIC_CONFIG->GetRuneTowerCfg().GetRuneTowerLayerCfg(m_param.fb_pass_layer);
	if (NULL == cfg)
	{
		return;
	}

	int monster_num = (int)(offline_time * cfg->kill_monster_speed);
	if (monster_num <= 0)
	{
		return;
	}

	m_param.fb_offline_time -= offline_time;

	long long reward_exp = 0;
	int reward_jinghua = 0;
	int reward_equip_blue = 0;
	int reward_equip_purple = 0;
	int reward_equip_orange = 0;
	int reward_mojing = 0;

	int recycl_equip_blue = 0;
	int recycl_equip_purple = 0;
	int recycl_equip_orange = 0;

	int index_count = 0;
	static const int MAX_INDEX_COUNT = MAX_ATTACHMENT_ITEM_NUM * 20;
	ItemConfigData item_cfg_list[MAX_INDEX_COUNT];

	// 通关奖励
	if (cfg->monster_num > 0)
	{
		int layer_count = monster_num / cfg->monster_num;

		for (int i = 0; i < layer_count; i++)
		{
			reward_jinghua += cfg->normal_reward_rune_exp;

			for (int k = 0; k < MAX_ATTACHMENT_ITEM_NUM; k++)
			{
				const ItemConfigData &item_data = cfg->normal_reward_item_list[k];
				if (item_data.item_id > 0)
				{
					ItemExtern::PutInItemConfigList(MAX_INDEX_COUNT, item_cfg_list, &index_count, item_data.item_id, item_data.num, item_data.is_bind);
				}
			}
		}
	}

	// 杀怪奖励
	for (int i = 0; i < monster_num; i++)
	{
		// 符文精华
		if (RandomNum(10000) < cfg->monster_rune_exp_rate)
		{
			int add_rune_exp = cfg->monster_rune_exp_min;
			if (cfg->monster_rune_exp_max > 0 && cfg->monster_rune_exp_max >= cfg->monster_rune_exp_min)
			{
				add_rune_exp = RandomNum(cfg->monster_rune_exp_min, cfg->monster_rune_exp_max + 1);
			}
			reward_jinghua += add_rune_exp;
		}

		int temp_item_count = 0;
		static ItemConfigData temp_item_list[DropConfig::DROP_ITEM_PROB_MAX];

		long long temp_reward_exp = 0;
		MONSTERPOOL->GetMonsterDrop(cfg->monster_id, temp_item_list, &temp_item_count, &temp_reward_exp, NULL, NULL);

		reward_exp += temp_reward_exp;

		// 物品
		for (int k = 0; k < temp_item_count; k++)
		{
			ItemConfigData &item_data = temp_item_list[k];

			const ItemBase *item_base = ITEMPOOL->GetItem(item_data.item_id);
			if (NULL != item_base && ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
			{
				short item_color = item_base->GetColor();
				if (ItemBase::I_COLOR_BLUE == item_color) reward_equip_blue += item_data.num;
				else if (ItemBase::I_COLOR_PURPLE == item_color) reward_equip_purple += item_data.num;
				else if (ItemBase::I_COLOR_ORANGE == item_color) reward_equip_orange += item_data.num;

				// 回收
				if (item_base->GetColor() <= ItemBase::I_COLOR_BLUE 
					|| !m_role->IsProfSuitable((char)item_base->GetLimitProf())
					|| !m_role->GetKnapsack()->HaveSpace())
				{
					if (RECYCL_MOJING == item_base->GetRecyclType(NULL))
					{
						reward_mojing += item_base->GetRecyclGet(NULL) * item_data.num;

						if (ItemBase::I_COLOR_BLUE == item_color) recycl_equip_blue += item_data.num;
						else if (ItemBase::I_COLOR_PURPLE == item_color) recycl_equip_purple += item_data.num;
						else if (ItemBase::I_COLOR_ORANGE == item_color) recycl_equip_orange += item_data.num;
						continue;
					}
				}

				ItemDataWrapper item_wrapper;
				if (FallingItem::GetPickItem(cfg->monster_id, item_data.item_id, item_data.num, item_data.is_bind, item_wrapper, NULL))
				{
					m_role->GetKnapsack()->Put(item_wrapper, PUT_REASON_RUNE_TOWER);
				}
			}
			else
			{
				ItemExtern::PutInItemConfigList(MAX_INDEX_COUNT, item_cfg_list, &index_count, item_data.item_id, item_data.num, item_data.is_bind);
			}
		}
	}

	for (int i = 0; i < index_count && i <= MAX_INDEX_COUNT - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
	{
		m_role->GetKnapsack()->PutListOrMail(item_cfg_list + i, PUT_REASON_RUNE_TOWER);
	}

	Protocol::SCRuneTowerOfflineInfo pro;
	pro.old_level = m_role->GetLevel();

	m_role->AddExp(reward_exp, "RuneTower", Role::EXP_ADD_REASON_DEFAULT);
	this->AddRuneJingHua(reward_jinghua);
	m_role->GetRoleShop()->AddChestShopMojing(reward_mojing, "RuneTower");

	m_param.fb_add_exp += reward_exp;
	m_param.fb_add_jinghua += reward_jinghua;
	m_param.fb_add_equip_blue += reward_equip_blue;
	m_param.fb_add_equip_purple += reward_equip_purple;
	m_param.fb_add_equip_orange += reward_equip_orange;

	pro.fb_offline_time = m_param.fb_offline_time;
	pro.guaji_time = offline_time;
	pro.kill_monster_num = monster_num;
	pro.new_level = m_role->GetLevel();
	pro.add_exp = reward_exp;
	pro.add_jinghua = reward_jinghua;
	pro.add_equip_blue = reward_equip_blue;
	pro.add_equip_purple = reward_equip_purple;
	pro.add_equip_orange = reward_equip_orange;
	pro.add_mojing = reward_mojing;
	pro.recycl_equip_blue = recycl_equip_blue;
	pro.recycl_equip_purple = recycl_equip_purple;
	pro.recycl_equip_orange = recycl_equip_orange;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void RuneSystem::GMSetRuneTower(int pass_layer, int today_layer)
{
	m_param.fb_pass_layer = pass_layer;
	m_param.fb_today_layer = today_layer;
	this->OnRuneTowerPassLayer(pass_layer);
	this->SendOtherInfo();
}

void RuneSystem::SendComposeInfo()
{
	Protocol::SCRuneSystemComposeInfo cmd;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

int RuneSystem::GetRuneQualityByItemId(ItemID item_id)
{
	const RuneSystemRealIdCfg *rune_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetRealIdCfg(item_id);
	if (NULL == rune_cfg)
	{
		return 0;
	}

	return (int)rune_cfg->quality;
}

void RuneSystem::SendAllInfo()
{
	this->SendOtherInfo();
	this->CheckOpenRuneSlotAndId();
	this->SendVirtualBagAllInfo();
	this->SendRuneGridAllInfo();
	this->SendZhulingAllInfo();
}

void RuneSystem::GmOpera(int type, int p1, int p2, int p3, int p4)
{
	switch (type)
	{
	case Protocol::RUNE_SYSTEM_REQ_TYPE_ALL_INFO:
		{
			this->SendAllInfo();
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_DISPOSE_ONE:
		{
			this->DisposeOne(p1);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_COMPOSE:
		{
			bool is_bag_index1 = (0 != p2) ? true : false;
			bool is_bag_index2 = (0 != p4) ? true : false;
			this->ReqComposeRune(p1, is_bag_index1, p3, is_bag_index2);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_SET_RUAN:
		{
			this->ReqSetRune(p1, p2);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_XUNBAO_ONE:
		{
			this->ReqXunBaoOneTimes(true);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_XUNBAO_TEN:
		{
			this->ReqXunBaoTenTimes(true);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_UPLEVEL:
		{
			this->ReqUpRuneLevel(p1);
		}
		break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_RAND_ZHILING_SLOT:
		{
			this->RandZhulingSlot(p1);
		}
	break;

	case Protocol::RUNE_SYSTEM_REQ_TYPE_ZHULING:
		{
			this->Zhuling(p1);
		}
	break;

	case 11:
		{
			m_param.magic_crystal = p1;
			m_param.rune_jinghua = p2;
			m_param.rune_suipian = p3;
		}
		break;

	default:
		break;
	}
}

Attribute RuneSystem::RecalcFangJuAddFangyu(Attribute fangju_fangyu)
{
	return static_cast<Attribute>(fangju_fangyu * this->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_FANGYU));
}

Attribute RuneSystem::RecalcFangJuAddMaxHp(Attribute fangju_maxhp)
{
	return static_cast<Attribute>(fangju_maxhp * this->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_HP));
}

Attribute RuneSystem::RecalcFangJuAddShanbi(Attribute wuqi_pojia)
{
	return static_cast<Attribute>(wuqi_pojia * this->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_SHANBI));
}

Attribute RuneSystem::RecalcFangJuAddJianren(Attribute wuqi_pojia)
{
	return static_cast<Attribute>(wuqi_pojia * this->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_JIANREN));
}

Attribute RuneSystem::RecalcWuQiAddGongJi(Attribute wuqi_gongji)
{
	return static_cast<Attribute>(wuqi_gongji * this->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_GONGJI));
}

Attribute RuneSystem::RecalcEquipAddGongJi(Attribute equip_base_gongji)
{
	return static_cast<Attribute>(equip_base_gongji * this->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_EQUIP_PER_GONGJI));
}

Attribute RuneSystem::RecalcWuQiAddBaoji(Attribute wuqi_baoji)
{
	return static_cast<Attribute>(wuqi_baoji * this->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_BAOJI));
}

Attribute RuneSystem::RecalcShouShiAddGongji(Attribute shoushi_gongji)
{
	return static_cast<Attribute>(shoushi_gongji * this->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_GONGJI));
}

Attribute RuneSystem::RecalcShouShiAddBaoji(Attribute shoushi_baoji)
{
	return static_cast<Attribute>(shoushi_baoji * this->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_BAOJI));
}

void RuneSystem::ReqSetRune(int virtual_index, int grid_index)
{
	if (this->IsInvalidVirtualIndex(virtual_index) || this->IsInvalidGridIndex(grid_index))
	{
		return;
	}

	// 检查符文槽是否开启
	if (0 == (m_param.rune_slot_open_flag & (1 << grid_index)))
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_SLOT_NOT_OPEN);
		return;
	}

	RuneParam &bag_rune = m_param.virtual_bag_list[virtual_index];
	RuneParam &grid_rune = m_param.rune_grid[grid_index];

	if (bag_rune.IsEmpty())
	{
		return;
	}

	// 符文精华不能装备
	if (RUNE_TYPE_RUNEJINGHUA == bag_rune.type)
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_RUNE_JINGHUA_NOT_PUTON);
		return;
	}

	// “无视一击”符文只能装备在绝版槽（现在定义0号槽为绝版槽，默认开启）
	if (RUNE_TYPE_WUSHIYIJI == bag_rune.type)
	{
		if (grid_index!=0)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_RUNE_WUSHIYIJI_EQUIP_FAIL);
			return;
		}
	}

	//绝版槽位只能镶嵌无视一击符文
	if (grid_index == 0)
	{
		if (RUNE_TYPE_WUSHIYIJI != bag_rune.type)
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_SLOT_LIMIT);
			return;
		}
	}

	// 除了要装备的槽外，不能装备类型一样的符文
	std::set<int> types_set; types_set.clear();
	for (int i = 0; i < LOGIC_CONFIG->GetRuneSystemCfg().GetRuneSlotNum() && i < RUNE_SYSTEM_SLOT_MAX_NUM; ++i)
	{
		if (i == grid_index)
		{
			continue;
		}

		RuneParam &temp_grid_rune = m_param.rune_grid[i];
		if (temp_grid_rune.IsEmpty())
		{
			continue;
		}

		const RuneSystemAttrCfg *attr_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(temp_grid_rune.quality, temp_grid_rune.type, temp_grid_rune.level);
		if (NULL == attr_cfg)
		{
			continue;
		}

		//if (i == 0)	//无视一击符文不影响其他相同属性符文的镶嵌
		//{
		//	continue;
		//}

		if ((attr_cfg->add_attributes_0 > 0 || attr_cfg->add_attributes_1 > 0) && types_set.find(attr_cfg->types) == types_set.end())
		{
			types_set.insert(attr_cfg->types);
		}

	}

	const RuneSystemAttrCfg *attr_cfg1 = LOGIC_CONFIG->GetRuneSystemCfg().GetAttrCfg(bag_rune.quality, bag_rune.type, bag_rune.level);
	if (NULL == attr_cfg1)
	{
		return;
	}

	if (attr_cfg1->add_attributes_0 > 0 || attr_cfg1->add_attributes_1 > 0)
	{
		if (types_set.find(attr_cfg1->types) != types_set.end())
		{
			m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_TYPE_IS_SAME);
			return;
		}
	}

	RuneParam tmp_rune;
	tmp_rune.Reset();
	tmp_rune = grid_rune;
	grid_rune = bag_rune;
	bag_rune = tmp_rune;

	if (bag_rune.IsEmpty())
	{
		++m_empty_grids;
	}

	this->SendVirtualBagAllInfo();
	this->SendRuneGridAllInfo();
	this->ReCalRuneEffct();
	this->CheckGoalProgress(grid_rune.quality);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_RESON_RUNE_SYSTEM);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
	m_role->NoticeNum(noticenum::NT_RUNE_SYSTEM_SET_RUNE_SUCC);

	gamelog::g_log_rune_system.printf(LL_INFO, " RuneSystem::ReqSetRune user[%d %s],  bag_rune[quality:%d, type:%d, level:%d]  , grid_rune[quality:%d, type:%d, level:%d]",
	                                   m_role->GetUID(), m_role->GetName(), bag_rune.quality, bag_rune.type, bag_rune.level, grid_rune.quality, grid_rune.type, grid_rune.level);
}

void RuneSystem::PutRuneListToVirtualBag(const RuneParam rune[], int size, int magic_crystal, int info_type, const char *reason)
{
	if (NULL == rune)
	{
		return;
	}

	gamelog::g_log_rune_system.buff_printf("reason[%s] user[%d %s] rune_list(", reason, m_role->GetUID(), m_role->GetName());

	static Protocol::SCRuneSystemBagInfo rsbi;
	rsbi.info_type = info_type;
	rsbi.jinghua_box_magic_crystal = magic_crystal;
	rsbi.count = 0;

	int list_count = 0;
	for (int index = 0; index < RUNE_SYSTEM_BAG_MAX_GRIDS && list_count < size; ++ index)
	{
		RuneParam &bag_rune = m_param.virtual_bag_list[index];
		if (bag_rune.IsEmpty())
		{
			bag_rune = rune[list_count];
			++list_count;
			--m_empty_grids;

			rsbi.bag_list[rsbi.count].quanlity = bag_rune.quality;
			rsbi.bag_list[rsbi.count].type = bag_rune.type;
			rsbi.bag_list[rsbi.count].level = bag_rune.level;
			rsbi.bag_list[rsbi.count].index = index;
			++rsbi.count;

			gamelog::g_log_rune_system.buff_printf("[quality:%d, type:%d, level:%d],", bag_rune.quality, bag_rune.type, bag_rune.level);
		}
	}

	gamelog::g_log_rune_system.buff_printf("),  chou_count[%d]", list_count);
	gamelog::g_log_rune_system.commit_buff(LL_INFO);

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (RUNE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void RuneSystem::PutRuneToVirtualBag(const RuneParam &rune, int magic_crystal, int info_type, const char *reason)
{
	this->PutRuneListToVirtualBag(&rune, 1, magic_crystal, info_type, reason);
}

bool RuneSystem::IsInvalidGridIndex(int grid_index) 
{ return grid_index < 0 || grid_index >= RUNE_SYSTEM_SLOT_MAX_NUM || grid_index >= LOGIC_CONFIG->GetRuneSystemCfg().GetRuneSlotNum(); }

int CompareRuneStruc(const void *arg1, const void *arg2)
{
	RuneParam *s1 = (RuneParam*)arg1;
	RuneParam *s2 = (RuneParam*)arg2;

	if (s1->quality > s2->quality)
	{
		return -1;
	}
	else if (s1->quality < s2->quality)
	{
		return 1;
	}
	else if (s1->type < s2->type)
	{
		return -1;
	}
	else if (s1->type > s2->type)
	{
		return 1;
	}
	return s1->level > s2->level;
}

static bool JHSortFunc(const RuneParam &rune1, const RuneParam &rune2)
{
	return rune1.quality > rune2.quality; // 这里请不要用 >= 不然标准库会断言 
}

void RuneSystem::SortVirtrulBag()
{
	std::vector<RuneParam> rune_jinghua_vec; // 将符文精华找出来
	rune_jinghua_vec.clear();

	for (int i = 0; i < RUNE_SYSTEM_BAG_MAX_GRIDS; ++i)
	{
		RuneParam &bag_rune = m_param.virtual_bag_list[i];

		if (bag_rune.IsEmpty()) continue;

		if (RUNE_TYPE_RUNEJINGHUA == bag_rune.type)
		{
			rune_jinghua_vec.push_back(bag_rune);
			bag_rune.Reset();
		}
	}

	if (!rune_jinghua_vec.empty())
	{
		std::sort(rune_jinghua_vec.begin(), rune_jinghua_vec.end(), &JHSortFunc);
	}

	m_empty_grids = 0;
	qsort(m_param.virtual_bag_list, RUNE_SYSTEM_BAG_MAX_GRIDS, sizeof(RuneParam), CompareRuneStruc);

	for (int index = RUNE_SYSTEM_BAG_MAX_GRIDS - 1; index >= 0; --index)
	{
		RuneParam &rune = m_param.virtual_bag_list[index];
		if (!rune.IsEmpty())
		{
			break;
		}

		++m_empty_grids;
	}

	// 再将符文精华加到最后面
	int empty_index = RUNE_SYSTEM_BAG_MAX_GRIDS - m_empty_grids;
	for (int i = 0; i < (int)rune_jinghua_vec.size() && empty_index < RUNE_SYSTEM_BAG_MAX_GRIDS; ++i)
	{
		m_param.virtual_bag_list[empty_index] = rune_jinghua_vec[i];
		++ empty_index;
		-- m_empty_grids;

		if (empty_index >= RUNE_SYSTEM_BAG_MAX_GRIDS)
		{
			break;
		}
	}
}

bool RuneSystem::TransformAttrType(int type, int &real_type)
{
	switch (type)
	{
	case RUNE_ATTR_TYPE_GONGJI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
		}
		break;

	case RUNE_ATTR_TYPE_MAXHP:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
		}
		break;

	case RUNE_ATTR_TYPE_BAOJI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
		}
		break;

	case RUNE_ATTR_TYPE_SHANBI:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
		}
		break;

	case RUNE_ATTR_TYPE_MINGZHONG:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
		}
		break;

	case RUNE_ATTR_TYPE_JIANREN:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
		}
		break;

	case RUNE_ATTR_TYPE_FANGYU:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
		}
		break;

	case RUNE_ATTR_TYPE_PER_POFANG:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG;
		}
		break;

	case RUNE_ATTR_TYPE_PER_MIANSHANG:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG;
		}
		break;

	case RUNE_ATTR_TYPE_PER_POJIA:
		{
			real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN;
		}
	break;

	case RUNE_ATTR_TYPE_FUJIA_SHANGHAI:
	{
		real_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG;
	}
	break;

	default:
		return false;
	}

	return true;
}

bool RuneSystem::CheckIsActiveSpecialEffect(int effect_type)
{
	std::map<short, double>::iterator it_map = m_special_buff_list.find(effect_type);
	return it_map != m_special_buff_list.end();
}

void RuneSystem::ActiveSpecialEffectByRuneType(int type, double value, int add_per, int fuwen_index)
{
	switch (type)
	{
	case RUNE_ATTR_TYPE_WUQI_GONGJI:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_GONGJI, value, add_per, fuwen_index);
		}
		break;

	case RUNE_ATTR_TYPE_FANGJU_HP:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_HP, value, add_per, fuwen_index);
		}
		break;

	case RUNE_ATTR_TYPE_FANGJU_SHANBI:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_SHANBI, value, add_per, fuwen_index);
		}
		break;

	case RUNE_ATTR_TYPE_FANGJU_JIANREN:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_JIANREN, value, add_per, fuwen_index);
		}
		break;

	case RUNE_ATTR_TYPE_FANGJU_FANGYU:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_FANGYU, value, add_per, fuwen_index);
		}
		break;

	case RUNE_ATTR_TYPE_EXP_FETCH:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_KILL_MONSTER_EXP, value, add_per, fuwen_index);
		}
		break;

	case RUNE_ATTR_TYPE_WUQI_BAOJI:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_BAOJI, value, add_per, fuwen_index);
		}
		break;

	case RUNE_ATTR_TYPE_SHOUSHI_GONGJI:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_GONGJI, value, add_per, fuwen_index);
		}
		break;

	case RUNE_ATTR_TYPE_SHOUSHI_BAOJI:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_BAOJI, value, add_per, fuwen_index);
		}
		break;

	case RUNE_ATTR_TYPE_PER_EQUIP_GONGJI:
		{
			this->ActiveSpecialEffect(RUNE_SPECIAL_EFFECT_TYPE_ADD_EQUIP_PER_GONGJI, value, add_per, fuwen_index);
		}
		break;

	default:
		break;
	}
}

void RuneSystem::ActiveSpecialEffect(int effect_type, double value, int add_per, int fuwen_index)
{
	if (effect_type <= RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_INVALID || effect_type >= RUNE_SPECIAL_EFFECT_TYPE_COUNT || value <= 0)
	{
		return;
	}

	if (fuwen_index < 0 || fuwen_index >= RUNE_SYSTEM_SLOT_MAX_NUM) return;

	m_special_buff_list[static_cast<short>(effect_type)] += (value * ROLE_ATTR_PER) * (1 + add_per * ROLE_ATTR_PER);

	const RuneZhulingGradeConfig *cur_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetZhulingGradeCfg(fuwen_index, m_param.run_zhuling_list[fuwen_index].grade);
	if (NULL != cur_cfg)
	{
		m_special_buff_list[static_cast<short>(effect_type)] += ((value * ROLE_ATTR_PER) * (cur_cfg->special_add * ROLE_ATTR_PER));
	}
}

short RuneSystem::GetSpecialEffectByRuneType(int type)
{
	switch (type)
	{
	case RUNE_ATTR_TYPE_WUQI_GONGJI:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_GONGJI;
	}
	break;

	case RUNE_ATTR_TYPE_FANGJU_HP:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_HP;
	}
	break;

	case RUNE_ATTR_TYPE_FANGJU_SHANBI:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_SHANBI;
	}
	break;

	case RUNE_ATTR_TYPE_FANGJU_JIANREN:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_JIANREN;
	}
	break;

	case RUNE_ATTR_TYPE_FANGJU_FANGYU:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_FANGYU;
	}
	break;

	case RUNE_ATTR_TYPE_EXP_FETCH:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_KILL_MONSTER_EXP;
	}
	break;

	case RUNE_ATTR_TYPE_WUQI_BAOJI:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_BAOJI;
	}
	break;

	case RUNE_ATTR_TYPE_SHOUSHI_GONGJI:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_GONGJI;
	}
	break;

	case RUNE_ATTR_TYPE_SHOUSHI_BAOJI:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_BAOJI;
	}
	break;

	case RUNE_ATTR_TYPE_PER_EQUIP_GONGJI:
	{
		return RUNE_SPECIAL_EFFECT_TYPE_ADD_EQUIP_PER_GONGJI;
	}
	break;

	default:
		return 0;
		break;
	}
}

void RuneSystem::AddSpecialEffect(int effect_type, double value)
{
	if (effect_type <= RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_INVALID || effect_type >= RUNE_SPECIAL_EFFECT_TYPE_COUNT || value <= 0)
	{
		return;
	}

	m_special_buff_list[static_cast<short>(effect_type)] += (value * ROLE_ATTR_PER);
}

void RuneSystem::NotActiveSpecialEffect(int effect_type)
{
	std::map<short, double>::iterator it_map = m_special_buff_list.find(effect_type);
	if (it_map != m_special_buff_list.end())
	{
		m_special_buff_list.erase(it_map);
	}
}

void RuneSystem::SendVirtualBagAllInfo()
{
	this->SortVirtrulBag();

	static Protocol::SCRuneSystemBagInfo rsbi;
	rsbi.info_type = Protocol::RUNE_SYSTEM_INFO_TYPE_ALL_BAG_INFO;
	rsbi.jinghua_box_magic_crystal = 0;
	rsbi.count = 0;

	for (int index = 0; index < RUNE_SYSTEM_BAG_MAX_GRIDS; ++ index)
	{
		RuneParam &rune = m_param.virtual_bag_list[index];
		rsbi.bag_list[rsbi.count].quanlity = rune.quality;
		rsbi.bag_list[rsbi.count].type = rune.type;
		rsbi.bag_list[rsbi.count].level = rune.level;
		rsbi.bag_list[rsbi.count].index = index;
		++rsbi.count;
	}

	int length = sizeof(rsbi) - sizeof(rsbi.bag_list[0]) * (RUNE_SYSTEM_BAG_MAX_GRIDS - rsbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsbi, length);
}

void RuneSystem::SendRuneGridAllInfo()
{
	static Protocol::SCRuneSystemRuneGridInfo rsgi;

	UNSTD_STATIC_CHECK(sizeof(rsgi.rune_grid) == sizeof(m_param.rune_grid));
	memcpy(rsgi.rune_grid, m_param.rune_grid, sizeof(rsgi.rune_grid));

	UNSTD_STATIC_CHECK(sizeof(rsgi.rune_grid_awaken) == sizeof(m_param.rune_grid_awaken));
	memcpy(rsgi.rune_grid_awaken, m_param.rune_grid_awaken, sizeof(rsgi.rune_grid_awaken));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsgi, sizeof(rsgi));
}

void RuneSystem::SendOtherInfo()
{
	static Protocol::SCRuneSystemOtherInfo otio;
	memset(otio.suipian, 0, sizeof(otio.suipian));

	otio.pass_layer = m_param.fb_pass_layer;
	otio.rune_jinghua = m_param.rune_jinghua;
	otio.rune_suipian = m_param.rune_suipian;
	otio.magic_crystal = m_param.magic_crystal;

	otio.next_free_xunbao_timestamp = m_param.next_free_xunbao_timestamp;
	otio.total_xunbao_times = m_param.total_xunbao_times;
	otio.rune_slot_open_flag = m_param.rune_slot_open_flag;
	otio.free_xunbao_times = m_param.free_xunbao_times;
	otio.reserve_ch = 0;
	otio.rune_awaken_times = m_param.rune_gold_awaken_times;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&otio, sizeof(otio));
}

void RuneSystem::SendAwakenInfo(short seq, short is_need_recalc)
{
	static Protocol::SCRuneSystemRuneGridAwakenInfo otio;
	otio.awaken_seq = seq;
	otio.is_need_recalc = is_need_recalc;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&otio, sizeof(otio));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RuneSystem::CheckOpenRuneSlotAndId()
{
	int pass_layer = m_param.fb_pass_layer;
	if (pass_layer < 0)
	{
		return;
	}

	// 符文槽开启
	const RuneSystemSlotOpenCfg *open_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetSlotOpenCfg(pass_layer);
	if (NULL == open_cfg)
	{
		return;
	}

	// 检查符文槽是否开启
	int open_slot_index = open_cfg->open_rune_slot;
	for (int i = 0; i <= open_slot_index && i < RUNE_SYSTEM_SLOT_MAX_NUM; ++i)
	{
		if (0 == (m_param.rune_slot_open_flag & (1 << i)))
		{
			m_param.rune_slot_open_flag |= (1 << i);

			gamelog::g_log_rune_system.printf(LL_INFO, "RuneSystem::OpenRuneSlot Succ user[%d %s], open_slot_index[%d], pass_layer[%d], open_flag[%d]",
				m_role->GetUID(), m_role->GetName(), i, pass_layer, m_param.rune_slot_open_flag);
		}
	}

	// 检查符文合成是否开启
	int compose_open_need_layer = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().rune_compose_need_layer;
	if (pass_layer >= compose_open_need_layer && 0 == (m_param.rune_slot_open_flag & (1 << 15)))
	{
		m_param.rune_slot_open_flag |= (1 << 15);

		gamelog::g_log_rune_system.printf(LL_INFO, "RuneSystem::RuneComposeOpenSucc user[%d %s], pass_layer[%d], open_flag[%d]",
			m_role->GetUID(), m_role->GetName(), pass_layer, m_param.rune_slot_open_flag);
	}
}

void RuneSystem::RandZhulingSlot(int rune_index)
{
	const RuneSystemOtherCfg &other_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg();

	if (m_param.zhuling_slot_bless >= other_cfg.zhuling_slot_max_value)
	{
		return;
	}

	// 检查元宝
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.zhuling_cost))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 消耗元宝
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.zhuling_cost, "RuneSystem::RandZhulingSlot"))
	{
		return;
	}

	int total_weight = LOGIC_CONFIG->GetRuneSystemCfg().GetZhulingSlotCfgTotalWeight();
	int max_index = LOGIC_CONFIG->GetRuneSystemCfg().GetZhulingSlotCfgCount();
	if (total_weight <= 0)
	{
		return;
	}

	int rand_weight = RandomNum(total_weight);
	for (int index = 0; index < max_index; ++ index)
	{
		const RuneZhulingSlotConfig *cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetZhulingSlotCfg(index);
		if (NULL == cfg)
		{
			continue;
		}

		if (rand_weight < cfg->weight)
		{
			int value = 0;
			if (cfg->type == RuneZhulingSlotConfig::TYPE_FIX)
			{
				value = cfg->param1;
			}
			if (cfg->type == RuneZhulingSlotConfig::TYPE_UPSTAR)
			{
				if (rune_index >= 0 && rune_index < RUNE_SYSTEM_SLOT_MAX_NUM)
				{
					RuneZhuLingParam &zhuling_info = m_param.run_zhuling_list[rune_index];

					const RuneZhulingGradeConfig *cur_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetZhulingGradeCfg(rune_index, zhuling_info.grade);
					if (NULL == cur_cfg)
					{
						return;
					}

					value = cur_cfg->up_grade_need_bless - zhuling_info.zhuling_bless;
				}
			}
			else if (cfg->type == RuneZhulingSlotConfig::TYPE_PERCENT)
			{
				value = static_cast<int>(m_param.zhuling_slot_bless * (cfg->param1 / 100.0f));
			}

			if ((long long)m_param.zhuling_slot_bless + value > other_cfg.zhuling_slot_max_value)
			{
				m_param.zhuling_slot_bless = other_cfg.zhuling_slot_max_value;
			}
			else if (m_param.zhuling_slot_bless + value <= 0)
			{
				m_param.zhuling_slot_bless = 0;
			}
			else
			{
				m_param.zhuling_slot_bless += value;
			}

			this->SendZhulingResultInfo(index);
			break;
		}

		rand_weight -= cfg->weight;
	}
}

void RuneSystem::Zhuling(int rune_index)
{
	if (rune_index < 0 || rune_index >= RUNE_SYSTEM_SLOT_MAX_NUM)
	{
		return;
	}

	// 检查符文槽是否开启
	if (0 == (m_param.rune_slot_open_flag & (1 << rune_index)))
	{
		m_role->NoticeNum(errornum::EN_RUNE_SYSTEM_SLOT_NOT_OPEN);
		return;
	}

	RuneZhuLingParam &zhuling_info = m_param.run_zhuling_list[rune_index];

	const RuneZhulingGradeConfig *cur_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetZhulingGradeCfg(rune_index, zhuling_info.grade);
	const RuneZhulingGradeConfig *next_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetZhulingGradeCfg(rune_index, zhuling_info.grade + 1);
	if (NULL == cur_cfg || NULL == next_cfg)
	{
		return;
	}

	if (0 == m_param.zhuling_slot_bless)
	{
		return;
	}

	if (zhuling_info.zhuling_bless + m_param.zhuling_slot_bless > INT_MAX)
	{
		zhuling_info.zhuling_bless = INT_MAX;
	}
	else
	{
		zhuling_info.zhuling_bless += m_param.zhuling_slot_bless;
	}
	m_param.zhuling_slot_bless = 0;

	if (zhuling_info.zhuling_bless >= cur_cfg->up_grade_need_bless)
	{
		m_param.zhuling_slot_bless = zhuling_info.zhuling_bless - cur_cfg->up_grade_need_bless;
		zhuling_info.zhuling_bless = 0;
		++ zhuling_info.grade;
		m_role->ReCalcAttr(CharIntAttrs::RECALC_RESON_RUNE_SYSTEM);

		//自动升星（策划需求）
		while (m_param.zhuling_slot_bless > 0)
		{
			zhuling_info.zhuling_bless += m_param.zhuling_slot_bless;
			m_param.zhuling_slot_bless = 0;
			const RuneZhulingGradeConfig *cur1_cfg = LOGIC_CONFIG->GetRuneSystemCfg().GetZhulingGradeCfg(rune_index, zhuling_info.grade);
			if (NULL == cur_cfg)
			{
				return;
			}
			if (zhuling_info.zhuling_bless >= cur1_cfg->up_grade_need_bless)
			{
				m_param.zhuling_slot_bless = zhuling_info.zhuling_bless - cur_cfg->up_grade_need_bless;
				zhuling_info.zhuling_bless = 0;
				++zhuling_info.grade;
			}
		}
		

		//if (0 != next_cfg->is_broadcast)
		//{
		//	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_runesystem_zhuling,
		//		m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), rune_index, zhuling_info.grade);
		//	if (length > 0)
		//	{
		//		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		//	}
		//}
	}

	this->SendZhulingAllInfo();
}

void RuneSystem::SendZhulingResultInfo(int index)
{
	if (index < 0) return;

	static Protocol::SCRuneSystemZhulingRandResultInfo rszrri;
	rszrri.index = index;
	rszrri.zhuling_slot_bless = m_param.zhuling_slot_bless;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rszrri, sizeof(rszrri));
}

void RuneSystem::SendZhulingAllInfo()
{
	static Protocol::SCRuneSystemZhulingAllInfo rszai;

	rszai.zhuling_slot_bless = m_param.zhuling_slot_bless;

	for (int i = 0; i < RUNE_SYSTEM_SLOT_MAX_NUM; ++ i)
	{
		rszai.run_zhuling_list[i].grade = m_param.run_zhuling_list[i].grade;
		rszai.run_zhuling_list[i].zhuling_bless = m_param.run_zhuling_list[i].zhuling_bless;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rszai, sizeof(rszai));
}

bool RuneSystem::IsOpenAwaken()
{
	if (m_param.fb_pass_layer > 0 && m_param.fb_pass_layer >= LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().rune_awake_need_layer)
	{
		return true;
	}

	return false;
}

bool RuneSystem::IsOpenLianhun()
{
	if (m_param.fb_pass_layer > 0 && m_param.fb_pass_layer >= LOGIC_CONFIG->GetRuneSystemCfg().GetOterCfg().rune_lianhun_need_layer)
	{
		return true;
	}

	return false;
}

void RuneSystem::CheckGoalProgress(int quality)
{
	int same_quality_num = 0;

	for (int grid_index = 0; grid_index < RUNE_SYSTEM_SLOT_MAX_NUM; ++grid_index)
	{
		RuneParam &grid_rune = m_param.rune_grid[grid_index];
		if (quality == grid_rune.quality)
		{
			++same_quality_num;
		}
	}

	if (same_quality_num > 0)
	{
		m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_1, quality, same_quality_num);
	}
}

void RuneSystem::OnRoleLevelChange(int level)
{
	this->CheckFuncOpen();
}

void RuneSystem::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

void RuneSystem::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_RUNE);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE);
		}
	}
}