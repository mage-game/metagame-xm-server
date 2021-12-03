#include "tuhaojin.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/tuhaojin/tuhaojinconfig.hpp"
#include "item/itemextern.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "servercommon/string/crossstr.h"
#include "protocal/msgtuhaojin.h"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/world.h"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"

TuHaoJin::TuHaoJin() : m_role(NULL)
{
}

TuHaoJin::~TuHaoJin()
{

}

void TuHaoJin::Init(Role *role, const TuHaoJinParam &param)
{
	m_role = role;
	m_param = param;
}

void TuHaoJin::GetInitParam(TuHaoJinParam *param)
{
	*param = m_param;
}

void TuHaoJin::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		const TuhaojinLevelConfig *jinghua_cfg = LOGIC_CONFIG->GetTuHaoJin().GetTuhaojinLevelCfg(m_param.tuhaojin_level);
		if (NULL != jinghua_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += jinghua_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += jinghua_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += jinghua_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += jinghua_cfg->mingzhong;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += jinghua_cfg->shanbi;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += jinghua_cfg->baoji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += jinghua_cfg->jianren;
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_TUHAOJIN, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void TuHaoJin::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

}

bool TuHaoJin::OnTuhaojinUpLevel()
{
	int max_level_cfg = LOGIC_CONFIG->GetTuHaoJin().GetTuhaojinMaxLevel();
	if (m_param.tuhaojin_level < 0 || m_param.tuhaojin_level >= max_level_cfg || m_param.tuhaojin_level >= TUHAOJIN_MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_TUHAOJIN_LEVEL_LIMIT);
		return false;
	}

	const TuhaojinLevelConfig * cur_leve_cfg = LOGIC_CONFIG->GetTuHaoJin().GetTuhaojinLevelCfg(m_param.tuhaojin_level);
	if (NULL == cur_leve_cfg)
	{
		m_role->NoticeNum(errornum::EN_TUHAOJIN_LEVEL_LIMIT);
		return false;
	}

	const TuhaojinLevelConfig * next_leve_cfg = LOGIC_CONFIG->GetTuHaoJin().GetTuhaojinLevelCfg(m_param.tuhaojin_level + 1);
	if (NULL == next_leve_cfg)
	{
		m_role->NoticeNum(errornum::EN_TUHAOJIN_LEVEL_LIMIT);
		return false;
	}

	int old_level = m_param.tuhaojin_level;

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		int item_count = 0;

		if (cur_leve_cfg->common_item.item_id > 0)
		{
			stuff_list[item_count].item_id = cur_leve_cfg->common_item.item_id;
			stuff_list[item_count].num = cur_leve_cfg->common_item.num;
			stuff_list[item_count].buyable = false;
			item_count++;
		}


		if (PROF_TYPE_PROF_1 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_one_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_one_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_one_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else if (PROF_TYPE_PROF_2 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_two_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_two_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_two_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else if (PROF_TYPE_PROF_3 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_three_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_three_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_three_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else if (PROF_TYPE_PROF_4 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_four_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_four_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_four_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else
		{
			return false;
		}

		if (item_count <= 0)
		{
			return false;
		}

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, item_count, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "TuHaoJin::OnTuhaojinUpLevel"))
	{
		return false;
	}

	// 升级
	++ m_param.tuhaojin_level;

	if (m_param.max_tuhaojin_color < next_leve_cfg->tuhaojin_color_id)
	{
		m_param.max_tuhaojin_color = static_cast<char>(next_leve_cfg->tuhaojin_color_id);
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TUHAOJIN);
	this->SendJinguaInfo();

	if (0 != next_leve_cfg->is_notice)
	{
		int length = 0;

		length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tuhaojin_activate_notic, 
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());

		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			m_param.reserve = 1;
		}
	}

	//日志
	gamelog::g_log_tuhaojin.printf(LL_INFO, "TuHaoJin::UpLevel user[%d %s] old_level %d, cur_level %d",
		m_role->GetUID(), m_role->GetName(), old_level, m_param.tuhaojin_level);
	
	return true;
}

void TuHaoJin::SendJinguaInfo()
{
	Protocol::SCTuHaoJinInfo scthji;
	scthji.tuhaojin_level = m_param.tuhaojin_level;
	scthji.reserve = 0;
	scthji.cur_tuhaojin_color = m_param.cur_tuhaojin_color;
	scthji.max_tuhaojin_color = m_param.max_tuhaojin_color;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&scthji, sizeof(scthji));
}

char TuHaoJin::GetTuhaojinColor()
{
	return m_param.cur_tuhaojin_color;
}

void TuHaoJin::OnUseTuhaoJin(char tuhaojin_color_id)
{
	if (m_param.max_tuhaojin_color < tuhaojin_color_id)
	{
		m_role->NoticeNum(errornum::EN_TUHAOJIN_COLOR_NOT_ACTIVE);
		return;
	}

	if (0 != tuhaojin_color_id)
	{
		m_param.cur_tuhaojin_color = tuhaojin_color_id;
		m_role->NoticeNum(noticenum::NT_USE_TUHAOJIN_SCUCC);
	}
	else
	{
		m_param.cur_tuhaojin_color = tuhaojin_color_id;
		m_role->NoticeNum(noticenum::NT_NOT_USE_TUHAOJIN_SCUCC);
	}

	this->SendJinguaInfo();
}


