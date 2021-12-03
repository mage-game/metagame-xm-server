#include "crossequip.hpp"
#include "item/itempool.h"
#include "protocal/msgcrossequip.h"
#include "other/capability/capability.hpp"
#include "crossequipconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "other/shop/roleshop.hpp"
#include "item/knapsack.h"
#include "equipment/equipment.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "gstr.h"
#include "servercommon/noticenum.h"
#include "config/joinlimitconfig.hpp"

static const int ONE_CHOU_LOOP_TIMES = 1;
static const int TEN_CHOU_LOOP_TIMES = 10;
static const int THIRTY_CHOU_LOOP_TIMES = 30;

std::map<int, int> CrossEquip::m_equip_type2inde_map =
{
	{ Equipment::E_TYPE_CHUANSHI_WUQI,			CROSS_EQUIP_INDEX_WUQI },
	{ Equipment::E_TYPE_CHUANSHI_TOUKUI,		CROSS_EQUIP_INDEX_TOUKUI },
	{ Equipment::E_TYPE_CHUANSHI_YIFU,			CROSS_EQUIP_INDEX_YIFU },
	{ Equipment::E_TYPE_CHUANSHI_HUSHOU,		CROSS_EQUIP_INDEX_HUSHOU },
	{ Equipment::E_TYPE_CHUANSHI_YAODAI,		CROSS_EQUIP_INDEX_YAODAI },
	{ Equipment::E_TYPE_CHUANSHI_HUTUI,			CROSS_EQUIP_INDEX_KUZI } ,
	{ Equipment::E_TYPE_CHUANSHI_XIEZI,			CROSS_EQUIP_INDEX_XIEZI } ,
	{ Equipment::E_TYPE_CHUANSHI_ZHUOZI,		CROSS_EQUIP_INDEX_ZHUOZI } ,
	{ Equipment::E_TYPE_CHUANSHI_XIANGLIAN,		CROSS_EQUIP_INDEX_XIANGLIAN },
	{ Equipment::E_TYPE_CHUANSHI_JIEZHI,		CROSS_EQUIP_INDEX_JIEZHI },
};

CrossEquip::CrossEquip() : m_role(nullptr)
{
	memset(m_suit_num_list, 0, sizeof(m_suit_num_list));
}

CrossEquip::~CrossEquip()
{
}

void CrossEquip::ReCalcAttr(CharIntAttrs & base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		{
			// 斗气阶级
			const CrossEquipDouqiCfg * cfg = CROSS_EQUIP_CFG().GetCrossEquipDouqi(m_param.douqi_grade);
			if (nullptr != cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->jianren;
			}
		}

		{
			// 斗气装备
			CharIntAttrs equip_attrs;
			for (int equip_index = CROSS_EQUIP_INDEX_MIN; equip_index < CROSS_EQUIP_INDEX_MAX; ++equip_index)
			{
				ItemDataWrapper &itemdata = m_param.equip_list[equip_index];
				if (itemdata.Invalid())
				{
					continue;
				}

				const Equipment *equip_item = static_cast<const Equipment *>(ITEMPOOL->GetItem(itemdata.item_id));
				if (nullptr != equip_item && ItemBase::I_TYPE_EQUIPMENT == equip_item->GetItemType() 
					&& equip_item->GetEquipType() > Equipment::E_TYPE_CHUANSHI_MIN && equip_item->GetEquipType() < Equipment::E_TYPE_CHUANSHI_MAX)
				{
					equip_attrs.Reset();
					equip_item->GetBaseAttr(m_attrs_add);
					base_add.AddAttrs(&equip_attrs);
				}
			}
		}

		// 套装
		this->GetSuitTotalAttr(m_attrs_add);

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_CROSSEQUIP, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void CrossEquip::Update(time_t now_second, unsigned long interval)
{
}

void CrossEquip::OnDayChange(UInt32 old_dayid, UInt32 now_dayid)
{
	m_param.today_xiulian_times = 0;
	memset(m_param.douqi_dan_used_count, 0, sizeof(m_param.douqi_dan_used_count));

	this->SendAllInfo();
}

void CrossEquip::HandleMsg(short req_type, short param_1, short param_2, short param_3)
{
	switch (req_type)
	{
		case Protocol::CROSS_EQUIP_REQ_TYPE_INFO:
			{
				this->SendAllInfo();
			}
			break;

		case Protocol::CROSS_EQUIP_REQ_TYPE_DOUQI_GRADE_UP:
			{
				this->DouqiUpgrade();
			}
			break;

		case Protocol::CROSS_EQUIP_REQ_TYPE_DOUQI_XIULIAN:
			{
				this->DouqiXiulian();
			}
			break;

		case Protocol::CROSS_EQUIP_REQ_TYPE_ROLL:
			{
				this->Roll(param_1, param_2);
			}
			break;

		case Protocol::CROSS_EQUIP_REQ_TAKEOFF:
			{
				this->TakeOff(param_1);
			}
			break;

		case Protocol::CROSS_EQUIP_REQ_LIANZHI:
			{
				this->Lianzhi(param_1, param_2);
			}
			break;

		case Protocol::CROSS_EQUIP_REQ_ONE_EQUIP_INFO:
			{
				this->SendOneEquipInfo(param_1);
			}
			break;

		case Protocol::CROSS_EQUIP_REQ_ALL_EQUIP_INFO:
			{
				this->SendAllEquipInfo();
			}
			break;

		default:
			break;
	}
}

void CrossEquip::DouqiUpgrade()
{
	if (!this->IsFuncOpen())
	{
		return;
	}

	const CrossEquipDouqiCfg * grade_cfg = CROSS_EQUIP_CFG().GetCrossEquipDouqi(m_param.douqi_grade);
	if (nullptr == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}
	const CrossEquipDouqiCfg * next_grade_cfg = CROSS_EQUIP_CFG().GetCrossEquipDouqi(m_param.douqi_grade + 1);
	if (nullptr == next_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	if (m_param.douqi_exp < grade_cfg->upgrade_need_exp)
	{
		gstr::SendError(m_role, "en_cross_equip_douqi_exp_not_enough");
		return;
	}

	m_param.douqi_exp -= grade_cfg->upgrade_need_exp;

	{
		++m_param.douqi_grade;
		this->SendAllInfo();
	}

	if (next_grade_cfg->is_broadcast)
	{
		int len = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "douqi_upgrade",
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), this->GetDouqiGrade());
		if (len > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_NOT_CHAT);
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CROSSEQUIP);
}

void CrossEquip::DouqiXiulian()
{
	if (!this->IsFuncOpen())
	{
		return;
	}

	const CrossEquipDouqiXiulianCfg * xiulian_cfg = CROSS_EQUIP_CFG().GetCrossEquipDouqiXiulianCfg(m_param.douqi_grade, m_param.today_xiulian_times + 1);
	if (nullptr == xiulian_cfg)
	{
		gstr::SendError(m_role, "en_cross_equip_douqi_xiulian_times_limit");
		return;
	}

	if (m_role->GetRoleShop()->GetChestShopMojing() < xiulian_cfg->need_mojing)
	{
		gstr::SendError(m_role, "en_cross_equip_douqi_xiulian_need_mojing_not_enough");
		return;
	}

	if (m_role->GetRoleShop()->AddChestShopMojing(-(xiulian_cfg->need_mojing), "douqi_xiulian"))
	{
		m_param.douqi_exp += xiulian_cfg->reward_exp;
		++m_param.today_xiulian_times;

		gstr::SendNotice(m_role, "nt_cross_equip_douqi_xiulian_succ");

		this->SendDouqiExpChange(xiulian_cfg->reward_exp);
		this->SendAllInfo();
	}
}

void CrossEquip::Roll(short roll_type, short roll_times_type)
{
	if (!this->IsFuncOpen())
	{
		return;
	}

	const CrossEquipRollCfg * roll_cfg = CROSS_EQUIP_CFG().GetCrossEquipRoll(roll_type);
	if (nullptr == roll_cfg)
	{
		return;
	}

	if (roll_cfg->need_grade > m_param.douqi_grade)
	{
		gstr::SendError(m_role, "en_cross_equip_roll_need_grade_limit");
		return;
	}

	if (roll_times_type <= CROSS_EQUIP_ROLL_COUNTS_TYPE_BEGIN || roll_times_type >= CROSS_EQUIP_ROLL_COUNTS_TYPE_MAX)
	{
		return;
	}
	int loop_times = roll_cfg->roll_times[roll_times_type];

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	//if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(loop_times))
	//{
	//	m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
	//	return;
	//}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(roll_cfg->consume[roll_times_type]))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(roll_cfg->consume[roll_times_type], "cross_equip::roll"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	static Protocol::SCCrossEquipRollResult msg;
	msg.reward_count = 0;

	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	int item_count = 0;

	for (int i = 0; i < loop_times && msg.reward_count < Protocol::CROSS_EQUIP_ROLL_TIMES_MAX && item_count < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		if (CROSS_EQUIP_CFG().GetCrossEquipRollReward(roll_type, &reward_item[item_count]))
		{
			msg.reward_list[msg.reward_count].item_id = reward_item[item_count].item_id;
			msg.reward_list[msg.reward_count].reward_num = reward_item[item_count].num;
			++item_count;
			++msg.reward_count;
		}

		if (item_count >= MAX_ATTACHMENT_ITEM_NUM || (i + 1) >= loop_times)
		{
			m_role->GetKnapsack()->PutListOrMail(reward_item, PUT_REASON_CROSS_EQUIP_ROLL);
			item_count = 0;
			memset(reward_item, 0, sizeof(reward_item));
		}
	}

	m_role->GetRoleShop()->AddCrossEquipChuanshiScore(roll_cfg->reward_chuanshi_score[roll_times_type], "cross_equip::roll");

	SEND_TO_ROLE(m_role, msg);
}

bool CrossEquip::UseDouqidan(short douqidan_type, short &use_num)
{
	if (!this->IsFuncOpen())
	{
		return false;
	}

	if (!CROSS_EQUIP_CFG().IsValidCrossEquipDouqidanType(douqidan_type))
	{
		return false;
	}

	const CrossEquipDouqiDanCfg * douqidan_cfg = CROSS_EQUIP_CFG().GetCrossEquipDouqiDan(douqidan_type);
	if (nullptr == douqidan_cfg)
	{
		return false;
	}

	if (m_param.douqi_dan_used_count[douqidan_type] >= douqidan_cfg->used_times_per_day)
	{
		gstr::SendError(m_role, "en_cross_equip_douqidan_used_times_limit");
		return false;
	}

	int can_use_num = douqidan_cfg->used_times_per_day - m_param.douqi_dan_used_count[douqidan_type];
	if (use_num > can_use_num)
	{
		use_num = can_use_num;
	}

	{
		m_param.douqi_exp += douqidan_cfg->reward_exp * use_num;
		m_param.douqi_dan_used_count[douqidan_type] += use_num;
	}

	this->SendDouqiExpChange(douqidan_cfg->reward_exp);

	this->SendAllInfo();

	return true;
}

void CrossEquip::Lianzhi(int equip_index, int douqi_grade)
{
	if (!this->IsFuncOpen())
	{
		return;
	}

	if (douqi_grade > this->GetDouqiGrade())
	{
		gstr::SendError(m_role, "en_cross_equip_douqi_grade_not_enough");
		return;
	}

	if (!IsValidEquipIndex(equip_index))
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const CrossEquipLianzhiCfg *lianzhi_cfg = CROSS_EQUIP_CFG().GetLianzhiCfg(m_role->GetBaseProf(), douqi_grade, equip_index);
	if (nullptr == lianzhi_cfg)
	{
		lianzhi_cfg = CROSS_EQUIP_CFG().GetLianzhiCfg(PROF_TYPE_PROF_NO_LIMIT, douqi_grade, equip_index);
	}
	if (nullptr == lianzhi_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}
	if (lianzhi_cfg->need_fragment <= 0)
	{
		return;
	}

	if (m_param.chuanshi_fragment < lianzhi_cfg->need_fragment)
	{
		gstr::SendError(m_role, "en_cross_equip_chuanshi_frag_not_enough");
		return;
	}

	{
		m_param.chuanshi_fragment -= lianzhi_cfg->need_fragment;
		this->SendChuanshiFragChange(Protocol::SCCrossEquipChuanshiFragmentChange::CHUANSHI_FRAG_CHANGE_TYPE_DEC, lianzhi_cfg->need_fragment);

		ItemConfigData reward_item(lianzhi_cfg->equip_id, true, 1);
		m_role->GetKnapsack()->Put(reward_item, PUT_REASON_CROSS_EQUIP_LIANZHI);

		int len = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "cross_equip_lianzhi",
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), lianzhi_cfg->equip_id);
		if (len > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_NOT_CHAT);
		}

		gstr::SendNotice(m_role, "nt_cross_equip_lianzhi_succ");

		this->SendAllInfo();
	}
}

bool CrossEquip::AddChuanshiFragment(int score)
{
	if (score <= 0)
	{
		return false;
	}
	m_param.chuanshi_fragment += score;
	this->SendChuanshiFragChange(Protocol::SCCrossEquipChuanshiFragmentChange::CHUANSHI_FRAG_CHANGE_TYPE_ADD, score);
	this->SendAllInfo();
	return true;
}

void CrossEquip::ReCalcSuitCount()
{
	memset(m_suit_num_list, 0, sizeof(m_suit_num_list));
	memset(m_equip_info_list, 0, sizeof(m_equip_info_list));

	for (int equip_index = CROSS_EQUIP_INDEX_MIN; equip_index < CROSS_EQUIP_INDEX_MAX; ++equip_index)
	{
		ItemDataWrapper &itemdata = m_param.equip_list[equip_index];
		if (itemdata.Invalid())
		{
			continue;
		}

		const Equipment *equip_item = static_cast<const Equipment *>(ITEMPOOL->GetItem(itemdata.item_id));
		if (nullptr != equip_item && ItemBase::I_TYPE_EQUIPMENT == equip_item->GetItemType()
			&& equip_item->GetEquipType() > Equipment::E_TYPE_CHUANSHI_MIN && equip_item->GetEquipType() < Equipment::E_TYPE_CHUANSHI_MAX)
		{
			CrossEquipInfo &info = m_equip_info_list[equip_index];
			info.color = equip_item->GetColor();
			info.order = equip_item->GetOrder();
		}
	}

	std::sort(std::begin(m_equip_info_list), std::end(m_equip_info_list));

	int equip_num = 0;
	int last_color = 0;
	for (int i = 0; i < CROSS_EQUIP_INDEX_MAX; i ++)
	{
		CrossEquipInfo &info = m_equip_info_list[i];
		if (last_color != info.color)
		{
			equip_num = 0;
			last_color = info.color;
		}

		equip_num++;

		const CrossEquipSuitAttrCfg *suit_cfg = LOGIC_CONFIG->GetCrossEquipConfig().GetSuitAttrCfg(info.order, info.color, equip_num);
		if (NULL != suit_cfg)
		{
			m_suit_num_list[info.order][info.color] = equip_num;
			equip_num = 0;
		}
	}
}

void CrossEquip::GetSuitTotalAttr(CharIntAttrs &tmp_attr)
{
	this->ReCalcSuitCount();

	for (int order = CROSS_EQUIP_MAX_ORDER; order > 0; order--)
	{
		for (int color = Equipment::I_COLOR_MAX - 1; color > 0; color--)
		{
			int equip_num = m_suit_num_list[order][color];
			const CrossEquipSuitAttrCfg *suit_cfg = LOGIC_CONFIG->GetCrossEquipConfig().GetSuitAttrCfg(order, color, equip_num);
			if (NULL != suit_cfg)
			{
				tmp_attr.AddAttrFromConfig(suit_cfg->attr_cfg);

				tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER, suit_cfg->per_mingzhong);
				tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER, suit_cfg->per_shanbi);
				tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER, suit_cfg->skill_zengshang_per);
				tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER, suit_cfg->skill_jianshang_per);
			}
		}
	}
}

void CrossEquip::SendAllInfo()
{
	static Protocol::SCCrossEquipAllInfo msg;

	msg.douqi_grade = m_param.douqi_grade;
	msg.xiulian_times = m_param.today_xiulian_times;
	msg.douqi_exp = m_param.douqi_exp;
	msg.chuanshi_fragment = m_param.chuanshi_fragment;
	memcpy(msg.douqidan_used_count, m_param.douqi_dan_used_count, sizeof(msg.douqidan_used_count));

	SEND_TO_ROLE(m_role, msg);
}

void CrossEquip::SendDouqiExpChange(int add_exp)
{
	static Protocol::SCCrossEquipDouqiExpChange msg;

	msg.add_exp = add_exp;

	SEND_TO_ROLE(m_role, msg);
}

void CrossEquip::SendAllEquipInfo()
{
	static Protocol::SCCrossEquipAllEquip msg;

	memcpy(msg.equipment_list, m_param.equip_list, sizeof(msg.equipment_list));

	SEND_TO_ROLE(m_role, msg);
}

void CrossEquip::SendOneEquipInfo(int equip_index)
{
	if (IsValidEquipIndex(equip_index))
	{
		static Protocol::SCCrossEquipOneEquip msg;

		msg.equip_index = equip_index;
		msg.item_wrapper = m_param.equip_list[equip_index];

		SEND_TO_ROLE(m_role, msg);
	}
}

void CrossEquip::SendChuanshiFragChange(int change_type, int change_frag)
{
	static Protocol::SCCrossEquipChuanshiFragmentChange msg;

	msg.change_type = change_type;
	msg.change_fragment = change_frag;

	SEND_TO_ROLE(m_role, msg);
}

int CrossEquip::PutOn(const ItemDataWrapper &item_wrapper)
{
	if (!this->IsFuncOpen())
	{
		return -10;
	}

	const Equipment* equip = (const Equipment*)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (nullptr == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		return -1;
	}

	if (0 == item_wrapper.has_param)
	{
		return -2;
	}

	// 斗气阶级是否足够
	short equip_grade = CROSS_EQUIP_CFG().GetEquipGrade(item_wrapper.item_id);
	if (equip_grade < 0)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return -4;
	}
	if (this->GetDouqiGrade() < equip_grade)
	{
		gstr::SendError(m_role, "en_cross_equip_grade_not_enough");
		return -5;
	}

	if (!IsValidEquipType(equip->GetEquipType()))
	{
		return -6;
	}

	const int equip_index = this->GetEquipIndexByType(equip->GetEquipType());
	if (!IsValidEquipIndex(equip_index))
	{
		return -7;
	}

	ItemDataWrapper &itemdata = m_param.equip_list[equip_index];
	if (!itemdata.Invalid())
	{
		if (this->TakeOff(equip_index, true) < 0)
		{
			return -8;
		}
	}

	// 加强判断
	if (!itemdata.Invalid() || item_wrapper.Invalid())
	{
		return -9;
	}

	itemdata.Reset(item_wrapper);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CROSSEQUIP);

	this->SendAllEquipInfo();

	return 0;
}

int CrossEquip::TakeOff(short equip_index, bool is_from_puton)
{
	// 判断传入的下标以及用其取出的装备
	if (!IsValidEquipIndex(equip_index))
	{
		return -1;
	}

	const ItemDataWrapper &item_data = m_param.equip_list[equip_index];
	if (item_data.Invalid())
	{
		return -2;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_data.item_id);
	if (nullptr == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT 
		|| !IsValidEquipType(equip->GetEquipType()))
	{
		return -3;
	}

	// 判断背包
	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return -4;
	}

	if (!knapsack->Put(item_data, PUT_REASON_NO_NOTICE))
	{
		return -5;
	}

	m_param.equip_list[equip_index].Reset();

	if (!is_from_puton)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CROSSEQUIP);
		this->SendAllEquipInfo();
	}

	return 0;
}

int CrossEquip::GetEquipIndexByType(int equip_type)
{
	if (!IsValidEquipType(equip_type))
	{
		return -1;
	}

	// 戒指特殊处理 如果第一个已经装备且第二个为空 返回第二个
// 	if (Equipment::E_TYPE_CHUANSHI_JIEZHI == equip_type 
// 		&& !m_param.equip_list[CROSS_EQUIP_INDEX_JIEZHI].Invalid() 
// 		&& m_param.equip_list[CROSS_EQUIP_INDEX_JIEZHI2].Invalid())
// 	{
// 		return CROSS_EQUIP_INDEX_JIEZHI2;
// 	}

	std::map<int, int>::iterator it = m_equip_type2inde_map.find(equip_type);
	if (m_equip_type2inde_map.end() != it)
	{
		return it->second;
	}

	return -1;
}

ItemID CrossEquip::GetCrossEquipID(int equip_index)
{
	if (!IsValidEquipIndex(equip_index))
	{
		return 0;
	}

	return m_param.equip_list[equip_index].item_id;
}

bool CrossEquip::IsFuncOpen()
{
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_CROSS_EQUIP);
	if (fun_open_cfg != nullptr && m_role->GetLevel() >= fun_open_cfg->role_level)
	{
		return true;
	}

	gstr::SendError(m_role, "en_cross_equip_func_not_open");
	return false;
}
