#include "shenzhuang.h"

#include "servercommon/servercommon.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"

#include "config/logicconfigmanager.hpp"
#include "shenzhuangconfig.hpp"

#include "engineadapter.h"
#include "scene/scene.h"
#include "world.h"
#include "item/knapsack.h"
#include "item/itemextern.hpp"
#include "obj/character/role.h"
#include "obj/character/attribute.hpp"

ShenZhuang::ShenZhuang() : m_role(NULL)
{
}

ShenZhuang::~ShenZhuang() 
{
}

void ShenZhuang::SetRole(Role *role)
{
	m_role = role;
}

void ShenZhuang::Init(Role *role, const ShenZhuangParam &szparam)
{
	m_role = role;
	m_szparam = szparam;
}

void ShenZhuang::GetInitParam(ShenZhuangParam *szparam)
{
	*szparam = m_szparam;
}

void ShenZhuang::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int i = 0; i < SHENZHUANG_MAX_PART; ++i)
		{
			const SZUplevelItemCfg *item_cfg = LOGIC_CONFIG->GetShenZhuangCfg().GetUplevelItemCfg(i, m_szparam.szlevel_list[i]);
			if (NULL != item_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += item_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += item_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += item_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += item_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += item_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += item_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += item_cfg->jianren;
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENZHUANG, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void ShenZhuang::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_szparam.day_drop_level_times = 0;
	m_szparam.day_gain_times = 0;
}

void ShenZhuang::OnUplevel(int part_index)
{
	if (part_index < 0 || part_index >= SHENZHUANG_MAX_PART)
	{
		return;
	}

	const int old_level = m_szparam.szlevel_list[part_index];
	
	if (old_level >= SHENZHUANG_MAX_LEVEL || old_level >= LOGIC_CONFIG->GetShenZhuangCfg().GetMaxLevel())
	{
		m_role->NoticeNum(errornum::EN_SHENZHUANG_MAX_LEVEL);
		return;
	}

	const SZUplevelItemCfg *uplevel_cfg = LOGIC_CONFIG->GetShenZhuangCfg().GetUplevelItemCfg(part_index, old_level + 1);
	if (NULL == uplevel_cfg)
	{
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		stuff_list[stuff_count].item_id = uplevel_cfg->stuff_id;
		stuff_list[stuff_count].num = uplevel_cfg->stuff_num;
		stuff_list[stuff_count].buyable = false;
		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
	}

	// ÏûºÄ
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ShenZhuang::OnUplevel"))
	{
		return;
	}

	++ m_szparam.szlevel_list[part_index];

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_ZHUANG);
	this->SyncShenZhuangInfo();
}

void ShenZhuang::OnDie(Role *killer)
{
	if (NULL == killer || m_role == killer)
	{
		return;
	}

	if (!LOGIC_CONFIG->GetShenZhuangCfg().IsDownScene(m_role->GetScene()->GetSceneID()))
	{
		return;
	}

	if (killer->GetCapability()->GetTotalCapability() > m_role->GetCapability()->GetTotalCapability() * 2)
	{
		return;
	}

	UNSTD_STATIC_CHECK(SHENZHUANG_MAX_PART > 0);
	int max_level = 0;
	for (int i = 0; i < SHENZHUANG_MAX_PART; i++)
	{
		if (max_level < m_szparam.szlevel_list[i])
		{
			max_level = m_szparam.szlevel_list[i];
		}
	}

	if (max_level <= 0)
	{
		return;
	}

	static int rand_list[SHENZHUANG_MAX_PART];
	int rand_count = 0;
	for (int j = 0; j < SHENZHUANG_MAX_PART; j++)
	{
		if (max_level == m_szparam.szlevel_list[j])
		{
			rand_list[rand_count++] = j;
		}
	}

	if (rand_count <= 0)
	{
		return;
	}

	int reward_percent = LOGIC_CONFIG->GetShenZhuangCfg().GetRewardPercent(max_level);
	if (reward_percent <= 0)
	{
		return;
	}

	const int rand_index = RandomNum(rand_count);
	const int part_index = rand_list[rand_index];
	if (part_index < 0 || part_index >= SHENZHUANG_MAX_PART)
	{
		return;
	}

	int level = m_szparam.szlevel_list[part_index];
	if (level <= 0)
	{
		return;
	}
	
	const SZUplevelItemCfg *item_cfg = LOGIC_CONFIG->GetShenZhuangCfg().GetUplevelItemCfg(part_index, level);
	if (NULL == item_cfg)
	{
		return;
	}

	if (RandomNum(10000) <= item_cfg->downlevel_rate)
	{
		const SZOtherCfg &other_cfg = LOGIC_CONFIG->GetShenZhuangCfg().GetOtherCfg();
		if (m_szparam.day_drop_level_times < other_cfg.drop_level_max_times)
		{
			++m_szparam.day_drop_level_times;
			--m_szparam.szlevel_list[part_index];
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_ZHUANG);
			this->SyncShenZhuangInfo();

			if (killer->GetShenZhuang()->GetDayGainTimes() < other_cfg.gain_max_times)
			{
				killer->GetShenZhuang()->AddDayGainTiems(1);
				static ItemConfigData config_data;
				config_data.item_id = item_cfg->stuff_id;
				config_data.num = static_cast<int>(item_cfg->stuff_num * reward_percent / 100);
				config_data.is_bind = false;

				killer->GetKnapsack()->PutOrMail(config_data, PUT_REASON_SHENZHUANG_KILLROLE);

				// ´«ÎÅ
				{
					if (m_szparam.szlevel_list[part_index] >= 5)
					{
						int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenzhuang_downlevel,
							m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), killer->GetUID(), killer->GetName(), killer->GetCamp(),
							config_data.item_id, config_data.num);
						if (len > 0)
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}
			}
		}
	}
}

int ShenZhuang::GetRedRatio(int part_index, int red_star)
{
	if (part_index < 0 || part_index >= SHENZHUANG_MAX_PART)
	{
		return 0;
	}

	const SZUplevelItemCfg *item_cfg = LOGIC_CONFIG->GetShenZhuangCfg().GetUplevelItemCfg(part_index, m_szparam.szlevel_list[part_index]);
	if (NULL == item_cfg)
	{
		return 0;
	}

	if (1 == red_star) return item_cfg->red_ratio_1;
	else if (2 == red_star) return item_cfg->red_ratio_2;
	else if (3 == red_star) return item_cfg->red_ratio_3;

	return 0;
}

int ShenZhuang::GetPinkRatio(int part_index)
{
	if (part_index < 0 || part_index >= SHENZHUANG_MAX_PART)
	{
		return 0;
	}

	const SZUplevelItemCfg *item_cfg = LOGIC_CONFIG->GetShenZhuangCfg().GetUplevelItemCfg(part_index, m_szparam.szlevel_list[part_index]);
	if (NULL == item_cfg)
	{
		return 0;
	}

	return item_cfg->pink_ratio;
}

void ShenZhuang::AddDayGainTiems(int times)
{
	if (times > 0)
	{
		m_szparam.day_gain_times += times;
	}
}

void ShenZhuang::SyncShenZhuangInfo()
{
	static Protocol::SCShenzhaungInfo pro;
	UNSTD_STATIC_CHECK(sizeof(pro.level_list) == sizeof(m_szparam.szlevel_list));
	memcpy(pro.level_list, m_szparam.szlevel_list, sizeof(pro.level_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}
