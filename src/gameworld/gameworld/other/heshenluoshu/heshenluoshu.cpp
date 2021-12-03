//#include "stdafx.h"

#include "heshenluoshu.hpp"
#include "heshenluoshuconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "protocal/msgheshenluoshu.h"
#include "engineadapter.h"
#include "gamelog.h"
#include "item/knapsack.h"
#include "item/itempool.h"
#include "other/capability/capability.hpp"
#include "other/event/eventhandler.hpp"

HeShengLuoShu::HeShengLuoShu()
{

};
HeShengLuoShu::~HeShengLuoShu()
{

};

void HeShengLuoShu::Init(Role * role, const HeShenLuoShuParam & heshen_luoshu_param)
{
	m_role = role;
	m_heshen_luoshu_param = heshen_luoshu_param;
}

void HeShengLuoShu::GetInitParam(HeShenLuoShuParam * heshen_luoshu_param) const
{
	*heshen_luoshu_param = m_heshen_luoshu_param;
}

void HeShengLuoShu::UseItemActivation(ItemID item_id)
{
	const HeShenLuoShuItemStar::ItemPos * item_attr = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuItemPos(item_id);

	if (NULL == item_attr || item_attr->type < 0 || item_attr->type >= HESHENLUOSHU_MAX_TYPE ||
		item_attr->seq < 0 || item_attr->seq >= HESHENLUOSHU_MAX_SEQ ||
		item_attr->index < 0 || item_attr->index >= HESHENLUOSHU_MAX_INDEX)
	{
		return;
	}

	//if (m_heshen_luoshu_param.heshenluotudata[item_attr->type][item_attr->seq][item_attr->index] >= 0)
	//{
	//	m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_ALREADY_ACTIVATED);
	//	return;
	//}

	//if (!m_role->GetKnapsack()->ConsumeItem(item_id, 1, "HeShengLuoShu::UseItemActivation"))
	//{
	//	m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_NOT_ITEM);
	//	return;
	//}

	if (m_heshen_luoshu_param.heshenluotudata[item_attr->type][item_attr->seq][item_attr->index] >= 0)
	{
		m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_ALREADY_ACTIVATED);
		return;
	}
	
	int stuff_id = 0;
	const HeShenLuoShuItemStar::ItemStarCfg * itemstar_cfg = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuItemStarCfg(item_id, 0);
	if (NULL == itemstar_cfg)
	{
		return;
	}

	//消耗方式修改为按职业消耗
	switch (m_role->GetBaseProf())
	{
		case PROF_TYPE_PROF_1:
		{
			stuff_id = itemstar_cfg->stuff_1;
		}
		break;

		case PROF_TYPE_PROF_2:
		{
			stuff_id = itemstar_cfg->stuff_2;
		}
		break;

		case PROF_TYPE_PROF_3:
		{
			stuff_id = itemstar_cfg->stuff_3;
		}
		break;

		case PROF_TYPE_PROF_4:
		{
			stuff_id = itemstar_cfg->stuff_4;
		}
		break;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(stuff_id, 1, "HeShengLuoShu::UseItemActivation"))
	{
		m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_NOT_ITEM);
		return;
	}

	m_heshen_luoshu_param.heshenluotudata[item_attr->type][item_attr->seq][item_attr->index] = 0;

	this->SendHeShenLuoShuChangeInfo(item_attr->type, item_attr->seq, item_attr->index);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HESHEN_LUOSHU);

	EventHandler::Instance().OnActiveHeshenluoshu(m_role);

	this->PrintOperationLog();
}

void HeShengLuoShu::UseItemUpgradeLevel(ItemID item_id)
{
	const HeShenLuoShuItemStar::ItemPos * item_attr = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuItemPos(item_id);

	if (NULL == item_attr || item_attr->type < 0 || item_attr->type >= HESHENLUOSHU_MAX_TYPE ||
		item_attr->seq < 0 || item_attr->seq >= HESHENLUOSHU_MAX_SEQ ||
		item_attr->index < 0 || item_attr->index >= HESHENLUOSHU_MAX_INDEX)
	{
		return;
	}

	if (m_heshen_luoshu_param.heshenluotudata[item_attr->type][item_attr->seq][item_attr->index] < 0)
	{
		m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_NOT_ACTIVATED);
		return;
	}

	int star_level = m_heshen_luoshu_param.heshenluotudata[item_attr->type][item_attr->seq][item_attr->index];
	int max_level = LOGIC_CONFIG->GetHeShenLuoShuConfig().HeShenLuoShuOtherMaxStarLevel();
	if (star_level + 1 <= max_level)
	{
		const HeShenLuoShuItemStar::ItemStarCfg * itemstar_cfg = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuItemStarCfg(item_id, star_level + 1);
		if (NULL == itemstar_cfg)
		{
			return;
		}

		int stuff_id = 0;

		switch (m_role->GetBaseProf())
		{
			case PROF_TYPE_PROF_1:
			{
				stuff_id = itemstar_cfg->stuff_1;
			}
			break;

			case PROF_TYPE_PROF_2:
			{
				stuff_id = itemstar_cfg->stuff_2;
			}
			break;

			case PROF_TYPE_PROF_3:
			{
				stuff_id = itemstar_cfg->stuff_3;
			}
			break;

			case PROF_TYPE_PROF_4:
			{
				stuff_id = itemstar_cfg->stuff_4;
			}
			break;
		}

		//if (!m_role->GetKnapsack()->ConsumeItem(item_id, itemstar_cfg->consume_jinghua, "HeShengLuoShu::UseItemUpgradeLevel"))
		if (!m_role->GetKnapsack()->ConsumeItem(stuff_id, itemstar_cfg->consume_jinghua, "HeShengLuoShu::UseItemUpgradeLevel"))
		{
			m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_NOT_ITEM);
			return;
		}

		m_heshen_luoshu_param.heshenluotudata[item_attr->type][item_attr->seq][item_attr->index] = star_level + 1;
		this->SendHeShenLuoShuChangeInfo(item_attr->type, item_attr->seq, item_attr->index);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HESHEN_LUOSHU);
	}
	else
	{
		m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_UPGRADE_MAX);
	}

	this->PrintOperationLog();
}

void HeShengLuoShu::ItemDecompose(ItemID item_id)
{
	const HeShenLuoShuItemStar::ItemPos * item_attr = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuItemPos(item_id);

	if (NULL == item_attr || item_attr->type < 0 || item_attr->type >= HESHENLUOSHU_MAX_TYPE ||
		item_attr->seq < 0 || item_attr->seq >= HESHENLUOSHU_MAX_SEQ ||
		item_attr->index < 0 || item_attr->index >= HESHENLUOSHU_MAX_INDEX)
	{
		return;
	}

	int max_level = LOGIC_CONFIG->GetHeShenLuoShuConfig().HeShenLuoShuOtherMaxStarLevel();
	if (m_heshen_luoshu_param.heshenluotudata[item_attr->type][item_attr->seq][item_attr->index] < max_level)
	{
		m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_NOT_MAX_LEVEL);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(item_id);
	if (NULL == item_base)
	{
		return;
	}

	int add_exp = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuItemDecompose(item_base->GetColor());
	if (add_exp > 0)
	{
		int level = m_heshen_luoshu_param.heshenluotushushengdata[item_attr->type][item_attr->seq].level;

		if (level + 1 > HESHENLUOSHU_HUASHEN_MAX_LEVEL)
		{
			m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_MAX_LEVEL);
			return;
		}
		const HeShenLuoShuShenHuaAdd * shenhuanadd = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuShenHuaAdd(item_attr->type, item_attr->seq, level + 1);
		if (NULL == shenhuanadd) return;

		if (!m_role->GetKnapsack()->ConsumeItem(item_id, 1, "HeShengLuoShu::ItemDecompose"))
		{
			m_role->NoticeNum(errornum::EN_HESHENG_LUOSHU_YIJING_NOT_ITEM);
			return;
		}

		m_heshen_luoshu_param.heshenluotushushengdata[item_attr->type][item_attr->seq].exp += add_exp;

		short w_num = 0;
		while (m_heshen_luoshu_param.heshenluotushushengdata[item_attr->type][item_attr->seq].exp >= shenhuanadd->exp)
		{
			w_num++;
			if (w_num > 50)
			{
				break;
			}
			m_heshen_luoshu_param.heshenluotushushengdata[item_attr->type][item_attr->seq].exp -= shenhuanadd->exp;
			m_heshen_luoshu_param.heshenluotushushengdata[item_attr->type][item_attr->seq].level += 1;

			level = m_heshen_luoshu_param.heshenluotushushengdata[item_attr->type][item_attr->seq].level;
			if (level + 1 > HESHENLUOSHU_HUASHEN_MAX_LEVEL)
			{
				break;
			}
			shenhuanadd = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuShenHuaAdd(item_attr->type, item_attr->seq, level + 1);
			if (NULL == shenhuanadd)
			{
				break;
			}
		}

		this->SendHeShenLuoShuChangeInfo(item_attr->type, item_attr->seq, 0);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HESHEN_LUOSHU);
	}

	this->PrintOperationLog();
}

void HeShengLuoShu::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		int suit_num = 0;
		{
			for (int type = 0; type < HESHENLUOSHU_MAX_TYPE; ++type)
			{
				for (int seq = 0; seq < HESHENLUOSHU_MAX_SEQ; ++seq)
				{
					suit_num = 0;
					for (int index = 0; index < HESHENLUOSHU_MAX_INDEX; ++index)
					{
						if (m_heshen_luoshu_param.heshenluotudata[type][seq][index] >= 0)
						{
							suit_num++;
						}

						// 星级属性
						const HeShenLuoShuItemStar::ItemStarCfg *star_attr = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuItemStarCfg(type, seq, index, m_heshen_luoshu_param.heshenluotudata[type][seq][index]);
						if (NULL != star_attr)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += star_attr->maxhp;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += star_attr->gongji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += star_attr->fangyu;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += star_attr->per_baoji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += star_attr->baoji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] += star_attr->per_kangbao;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += star_attr->fujiashanghai;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI] += star_attr->dikangshanghai;
							//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_POJIA] += star_attr->pojia;
						}
					}

					// 套装属性
					if (suit_num > 0)
					{
						const HeShenLuoShuSuiteAttr::SuiteAttr *suit_attr = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuSuiteAttr(type, seq, suit_num);
						if (NULL != suit_attr)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_attr->maxhp;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_attr->gongji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_attr->fangyu;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += suit_attr->per_baoji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += suit_attr->baoji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] += suit_attr->per_kangbao;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += suit_attr->fujiashanghai;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI] += suit_attr->dikangshanghai;
							//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_POJIA] += suit_attr->pojia;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] += suit_attr->skill_jianshang_per;
// 							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += suit_attr->per_pofang;
// 							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += suit_attr->per_mianshang;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER] += suit_attr->skill_zengshang_per;

							// 下面特殊属性用乘法
							m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, suit_attr->per_mianshang);
							m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, suit_attr->per_pofang);
						}
					}
					if (m_heshen_luoshu_param.heshenluotushushengdata[type][seq].level > 0)
					{
						const HeShenLuoShuShenHuaAdd * shenhuanadd = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuShenHuaAdd(type, seq, m_heshen_luoshu_param.heshenluotushushengdata[type][seq].level);
						if (shenhuanadd == NULL)
						{
							continue;
						}

						double per_add = shenhuanadd->per_add * ROLE_ATTR_PER;

						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * per_add);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * per_add);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * per_add);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] * per_add);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * per_add);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] * per_add);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] * per_add);
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI] * per_add);
						//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_POJIA] += static_cast<Attribute>(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_POJIA] * per_add);
					}
				}
			}
		}
		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_HESHENLUOSHU, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void HeShengLuoShu::OnHandlerOpera(short opera_type, short param_1)
{
	if (opera_type < Protocol::HESHENLUOSHU_REQ_TYPE_ACTIVATION || opera_type > Protocol::HESHENLUOSHU_REQ_TYPE_DECOMPOSE)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::HESHENLUOSHU_REQ_TYPE_ACTIVATION:
	{
		this->UseItemActivation(param_1);
	}
	break;

	case Protocol::HESHENLUOSHU_REQ_TYPE_UPGRADELEVEL:
	{
		this->UseItemUpgradeLevel(param_1);
	}
	break;

	case Protocol::HESHENLUOSHU_REQ_TYPE_DECOMPOSE:
	{
		this->ItemDecompose(param_1);
	}
	break;

	default:
		break;
	}
}

void HeShengLuoShu::SendHeShenLuoShuAllInfo() const
{
	static Protocol::SCHeShenLuoShuAllInfo hslsai;

	memcpy(&hslsai.all_info, &m_heshen_luoshu_param, sizeof(hslsai.all_info));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&hslsai, sizeof(hslsai));
}

void HeShengLuoShu::SendHeShenLuoShuChangeInfo(short type, short seq, short index) const
{
	if (type < 0 || type >= HESHENLUOSHU_MAX_TYPE ||
		seq < 0 || seq >= HESHENLUOSHU_MAX_SEQ ||
		index < 0 || index >= HESHENLUOSHU_MAX_INDEX)
	{
		return;
	}


	static Protocol::SCHeShenLuoShuChangeInfo hslsai;

	//.....
	//memcpy(&hslsai.param1, &type, sizeof(hslsai.param1));
	//memcpy(&hslsai.param2, &seq, sizeof(hslsai.param2));
	//memcpy(&hslsai.param3, &index, sizeof(hslsai.param3));
	//memcpy(&hslsai.param4, &m_heshen_luoshu_param.heshenluotudata[type][seq][index], sizeof(hslsai.param4));
	//memcpy(&hslsai.param5, &m_heshen_luoshu_param.heshenluotushushengdata[type][seq].level, sizeof(hslsai.param5));
	//memcpy(&hslsai.param6, &m_heshen_luoshu_param.heshenluotushushengdata[type][seq].exp, sizeof(hslsai.param6));

	hslsai.param1 = type;
	hslsai.param2 = seq;
	hslsai.param3 = index;
	hslsai.param4 = m_heshen_luoshu_param.heshenluotudata[type][seq][index];
	hslsai.param5 = m_heshen_luoshu_param.heshenluotushushengdata[type][seq].level;
	hslsai.param6 = m_heshen_luoshu_param.heshenluotushushengdata[type][seq].exp;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&hslsai, sizeof(hslsai));
}

int HeShengLuoShu::GetActiveLuoShuNum(int type, int color)
{
	int num = 0;

	for (int seq = 0; seq < HESHENLUOSHU_MAX_SEQ; ++seq)
	{
		for (int index = 0; index < HESHENLUOSHU_MAX_INDEX; ++index)
		{
			if (m_heshen_luoshu_param.heshenluotudata[type][seq][index] >= 0)
			{
				const HeShenLuoShuItemStar::ItemStarCfg *star_attr = LOGIC_CONFIG->GetHeShenLuoShuConfig().GetHeShenLuoShuItemStarCfg(type, seq, index, m_heshen_luoshu_param.heshenluotudata[type][seq][index]);
				if (NULL != star_attr && star_attr->color >= color)
				{
					num++;
				}
			}
		}
	}

	return num;
}

void HeShengLuoShu::PrintOperationLog()
{
	long long capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_HESHENLUOSHU);

	char log_quick_str[LOG_BASE_LENGTH] = { 0 };
	SNPRINTF(log_quick_str, LOG_BASE_LENGTH, "[%lld]", capability);
	ROLE_LOG_QUICK6(LOG_TYPE_HESHENLUOSHU, m_role, 0, 0, log_quick_str, NULL);
}
