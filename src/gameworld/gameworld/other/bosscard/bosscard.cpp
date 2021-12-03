#include "world.h"
#include "bosscard.hpp"
#include "bosscardconfig.hpp"
#include <vector>
#include "config/logicconfigmanager.hpp"
#include "servercommon/bitmapfunction.hpp"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "protocal/msgother.h"
#include "gamelog.h"
#include "servercommon/errornum.h"
#include "gameworld/gameworld/gstr.h"
#include "monster/monsterpool.h"
#include "servercommon/string/crossstr.h"
#include "servercommon/string/gameworldstr.h"
#include "other/capability/capability.hpp"
#include "item/knapsack.h"
#include "gamecommon.h"

void BossCard::Init(Role *role, const BossCardParam &param)
{
	m_role = role;
	m_param = param;
}

void BossCard::GetInitParam(BossCardParam *param) const
{
	*param = m_param;
}

void BossCard::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int card_type = 0; card_type < CARD_TYPE_MAX; card_type++)
		{
			for (int i = 0; i < BOSS_CARD_MAX_COUNT; i++)
			{
				bool is_active = BitmapFunction::IsBitSet(m_param.card_item_list[card_type].card_has_active_flag, sizeof(m_param.card_item_list[card_type].card_has_active_flag), i);
				if (is_active)
				{
					// ¼¤»îÊôÐÔ
					const BossCardActiveConfig *active_cfg = LOGIC_CONFIG->GetBossCardConfig().GetBossCardCfg(i);
					if (NULL == active_cfg)
					{
						continue;
					}

					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += active_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += active_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += active_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += active_cfg->per_pojia;
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_BOSS_CARD, m_attrs_add);
	}
	base_add.Add(m_attrs_add.m_attrs, true);
}

void BossCard::OnOperaReq(int opera_type, int param1, int param2)
{
	int open_level = LOGIC_CONFIG->GetBossCardConfig().GetOtherCfg().open_level;
	if (m_role->GetLevel() < open_level)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::BOSS_CARD_OPERA_TYPE_ALL_INFO:
	{
		this->SendAllInfo();
	}
	break;

	case Protocol::BOSS_CARD_OPERA_TYPE_ACTIVE:
	{
		this->OnActiveCard(param1);
	}
	break;

	case Protocol::BOSS_CARD_OPERA_TYPE_FETCH:
	{
		this->OnFetchCardGroupReward(param1);
	}

	default:
		break;
	}
}

void BossCard::OnActiveCard(int monster_seq)
{
	const BossCardActiveConfig *active_cfg = LOGIC_CONFIG->GetBossCardConfig().GetBossCardCfg(monster_seq);
	if (NULL == active_cfg)
	{
		return;
	}

	bool is_active = BitmapFunction::IsBitSet(m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag, sizeof(m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag), monster_seq);
	if (is_active)
	{
		gstr::SendError(m_role, "bosscard_has_active");
		return;
	}

	bool is_can_active = BitmapFunction::IsBitSet(m_param.card_item_list[CARD_TYPE_BOSS].card_can_active_flag, sizeof(m_param.card_item_list[CARD_TYPE_BOSS].card_can_active_flag), monster_seq);
	if (!is_can_active)
	{
		gstr::SendError(m_role, "bosscard_can_not_active");
		return;
	}

	BitmapFunction::SetBit(m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag, sizeof(m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag), monster_seq);

	if(this->IsActivityThisCardType(active_cfg->card_type))
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_boss_card_activity_this_type,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), active_cfg->scene_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BOSS_CARD);

	gstr::SendNotice(m_role, "bosscard_active_succ");

	this->SendAllInfo();
}

void BossCard::OnFetchCardGroupReward(int card_type)
{
	int max_card_type = LOGIC_CONFIG->GetBossCardConfig().GetBossCardMaxCardType();
	if (card_type < 0 || card_type > max_card_type || card_type > BOSS_CARD_MAX_TYPE)
	{
		return;
	}

	if (0 != (m_param.card_group_reward_fetch_flag & (LL_ONE_BIT << card_type)))
	{
		return;
	}

	if (this->IsActivityThisCardType(card_type))
	{
		const BossCardRewardConfig * reward_cfg = LOGIC_CONFIG->GetBossCardConfig().GetBossCardRewardCfg(card_type);
		if (NULL != reward_cfg && reward_cfg->reward_item.item_id > 0)
		{
			if (m_role->GetKnapsack()->Put(reward_cfg->reward_item, PUT_REASON_BOSSCARD_REWARD))
			{
				m_param.card_group_reward_fetch_flag |= (LL_ONE_BIT << card_type);
			}
		}
	}

	this->SendAllInfo();
}

void BossCard::CheckActiveCard(unsigned short monster_id)
{
	int monster_seq = LOGIC_CONFIG->GetBossCardConfig().GetBossCardCfgSeq(monster_id);
	if (monster_seq >= 0)
	{
		bool is_active = BitmapFunction::IsBitSet(m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag, sizeof(m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag), monster_seq);
		if (is_active)
		{
			return;
		}
		else
		{
			BitmapFunction::SetBit(m_param.card_item_list[CARD_TYPE_BOSS].card_can_active_flag, sizeof(m_param.card_item_list[CARD_TYPE_BOSS].card_can_active_flag), monster_seq);

			this->SendAllInfo();
		}
	}
}

void BossCard::SendAllInfo()
{
	static Protocol::SCBossCardAllInfo all_info;
	
	all_info.card_group_reward_fetch_flag = m_param.card_group_reward_fetch_flag;

	UNSTD_STATIC_CHECK(sizeof(all_info.card_can_active_flag) == sizeof(m_param.card_item_list[CARD_TYPE_BOSS].card_can_active_flag));
	memcpy(all_info.card_can_active_flag, m_param.card_item_list[CARD_TYPE_BOSS].card_can_active_flag, sizeof(all_info.card_can_active_flag));

	UNSTD_STATIC_CHECK(sizeof(all_info.card_has_active_flag) == sizeof(m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag));
	memcpy(all_info.card_has_active_flag, m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag, sizeof(all_info.card_has_active_flag));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&all_info, sizeof(all_info));
}

bool BossCard::IsActivityThisCardType(int card_type)
{
	for (int i = 0; i < BOSS_CARD_MAX_COUNT; i++)
	{
		const BossCardActiveConfig *active_cfg = LOGIC_CONFIG->GetBossCardConfig().GetBossCardCfg(i);
		if (NULL == active_cfg)
		{
			continue;
		}

		bool is_active = BitmapFunction::IsBitSet(m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag, sizeof(m_param.card_item_list[CARD_TYPE_BOSS].card_has_active_flag), i);

		if(active_cfg->card_type == card_type && !is_active)
		{
			return false;
		}
	}

	return true;
}