#include "magiccard.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/magiccard/magiccardconfig.hpp"
#include "item/itemextern.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "other/event/eventhandler.hpp"
#include "protocal/msgmagiccard.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "protocal/msgmagiccard.h"
#include "servercommon/string/crossstr.h"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"

MagicCard::MagicCard() : m_role(NULL)
{
}

MagicCard::~MagicCard()
{

}

void MagicCard::InitParam(Role *role, const MagicCardParam &param)
{
	m_role = role;
	m_param = param;
}

void MagicCard::GetInitParam(MagicCardParam *param)
{
	*param = m_param;
}

void MagicCard::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		// 计算魔卡套装属性
		for (int color = MAGIC_CARD_COLOR_TYPE_BLUE; color < MAGIC_CARD_COLOR_TYPE_COLOR_COUNT; ++ color)
		{
			bool color_suit_full = true; // 套装卡牌是否凑齐
			for (int card_type = 0; card_type < MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT; ++ card_type)
			{
				int strength_level = m_param.card_slot_list[color][card_type].strength_level;
				int slot_card_id = m_param.card_slot_list[color][card_type].card_id;

				if (slot_card_id > 0)
				{
					const MagicCardInfoConfig *card_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetCardCfg(slot_card_id);
					if (NULL != card_cfg && strength_level > 0)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += card_cfg->strength_attr_list[strength_level].maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += card_cfg->strength_attr_list[strength_level].gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += card_cfg->strength_attr_list[strength_level].fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += card_cfg->strength_attr_list[strength_level].mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += card_cfg->strength_attr_list[strength_level].shanbi;
					}
					//计算套装等级是否满足
					const MagicCardSuitConfig * suit_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetCardSuitCfg(color);
					if (NULL != suit_cfg)
					{
						if(strength_level < suit_cfg->attr_need_level)
						{
							color_suit_full = false;
						}
					}
				}
				else
				{
					color_suit_full = false;
				}
			}

			if (color_suit_full) // 激活套装信息
			{
				const MagicCardSuitConfig *suit_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetCardSuitCfg(color);
				if (NULL != suit_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += suit_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += suit_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += suit_cfg->shanbi;
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_MAGIC_CARD, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void MagicCard::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	m_param.today_purple_free_chou_card_times = 0;
}


bool MagicCard::ActiveCard(ItemID item_id, int num)
{
	int card_id = LOGIC_CONFIG->GetMagicCardCfg().GetCardIdWithItemID(item_id);
	if (card_id <= 0 || card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
	{
		return false;
	}

	m_param.all_card_num_list[card_id] += num;
	this->SendAllInfo();

	return true;
}

void MagicCard::UseCard(int card_id)
{
	if (card_id <= 0 || card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
	{
		return;
	}

	if (m_param.all_card_num_list[card_id] <= 0)
	{
		return;
	}

	const MagicCardInfoConfig *card_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetCardCfg(card_id);
	if (NULL == card_cfg)
	{
		return;
	}

	MagicCardSlotItem &slot = m_param.card_slot_list[card_cfg->color][card_cfg->slot_index];
	if (slot.card_id > 0)
	{
		m_role->NoticeNum(errornum::EN_MAGIC_CARD_SLOT_ALEADY_HAS_CARD);
		return;
	}

	int old_card_num = m_param.all_card_num_list[card_id];
	-- m_param.all_card_num_list[card_id];

	slot.card_id = card_id;
	slot.strength_level = 1;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MAGIC_CARD);
	m_role->NoticeNum(noticenum::NT_MAGICCARD_CARD_USE_SUCC);
	this->SendAllInfo();

	// 装备魔卡日志
	gamelog::g_log_magic_card.printf(LL_INFO, "MagicCard::UseCard user[%d, %s] card_id[%d] color[%d] slot_index[%d] old_card_num[%d] new_card_num[%d]", 
		m_role->GetUID(), m_role->GetName(), card_id, card_cfg->color, card_cfg->slot_index, old_card_num, m_param.all_card_num_list[card_id]);
}

void MagicCard::UpgradeCard(int color, int slot_index, int consume_card_id, int consume_card_num)
{
	if (color < MAGIC_CARD_COLOR_TYPE_BLUE || color >= MAGIC_CARD_COLOR_TYPE_COLOR_COUNT)
	{
		return;
	}

	if (slot_index < 0 || slot_index >= MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT)
	{
		return;
	}

	if (consume_card_id <= 0 || consume_card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
	{
		return;
	}

	if (m_param.all_card_num_list[consume_card_id] < consume_card_num)
	{
		return;
	}

	MagicCardSlotItem &slot = m_param.card_slot_list[color][slot_index];

	if (slot.strength_level >= MAGIC_CARD_LIMIT_STRENGTH_LEVEL_MAX)
	{
		m_role->NoticeNum(errornum::EN_MAGIC_CARD_STRENGTH_LEVEL_FULL);
		return;
	}

	const MagicCardInfoConfig *consume_card_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetCardCfg(consume_card_id);
	if (NULL == consume_card_cfg)
	{
		return;
	}

	if (consume_card_cfg->strength_max_level <= 0)
	{
		return;
	}

	const MagicCardInfoConfig *slot_card_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetCardCfg(slot.card_id);
	if (NULL == slot_card_cfg)
	{
		return;
	}

	if (slot_card_cfg->strength_max_level < slot.strength_level)
	{
		return;
	}

	int old_level = slot.strength_level;
	int target_level = old_level;
	int target_exp = slot.exp + consume_card_cfg->strength_attr_list[1].contain_exp * consume_card_num;

	const MagicCardInfoConfig::StrengthAttr *card_upgrade_cfg = slot_card_cfg->GetStrenthAttrCfg(slot.strength_level);
	if(NULL == card_upgrade_cfg)
	{
		return;
	}

	while(target_exp >= card_upgrade_cfg->up_level_need_exp)
	{
		if (target_level >= MAGIC_CARD_LIMIT_STRENGTH_LEVEL_MAX)
		{
			target_exp = card_upgrade_cfg->up_level_need_exp;
			break;
		}

		target_exp -= card_upgrade_cfg->up_level_need_exp;
		++ target_level;

		card_upgrade_cfg = slot_card_cfg->GetStrenthAttrCfg(target_level);
		if(NULL == card_upgrade_cfg)
		{
			break;
		}
	}

	if (target_level < old_level)
	{
		return;
	}

	if (target_level == slot.strength_level && target_exp <= slot.exp)
	{
		return;
	}

	m_param.all_card_num_list[consume_card_id] -= consume_card_num;

	slot.exp = target_exp;
	slot.strength_level = target_level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MAGIC_CARD);
	this->SendAllInfo();

	// 升级魔卡日志
	gamelog::g_log_magic_card.printf(LL_INFO, "MagicCard::UpgradeCard user[%d, %s] color[%d] slot_index[%d] consume_card_id[%d] old_level[%d] target_level[%d] target_exp[%d]", 
		m_role->GetUID(), m_role->GetName(), color, slot_index, consume_card_id, old_level, target_level, target_exp);
}

void MagicCard::ChouCard(int type, int times)
{
	if (times < 0 || times > Protocol::SC_MAGIC_CARD_CHOU_CARD_MAX_TIMES)
	{
		return;
	}

	int cfg_consume_gold = 0;
	ItemID consume_item_id = ItemBase::INVALID_ITEM_ID;

	if (MAGIC_CARD_CHOU_CARD_TYPE_PURPLE == type)
	{
		cfg_consume_gold = LOGIC_CONFIG->GetMagicCardCfg().GetOtherCfg().purple_chou_card_consume_gold;
	}
	else if (MAGIC_CARD_CHOU_CARD_TYPE_ORAGEN == type)
	{
		cfg_consume_gold = LOGIC_CONFIG->GetMagicCardCfg().GetOtherCfg().orange_chou_card_consume_gold;
		consume_item_id = LOGIC_CONFIG->GetMagicCardCfg().GetOtherCfg().orange_chou_card_consume_item_id;
	}
	else if (MAGIC_CARD_CHOU_CARD_TYPE_RED == type)
	{
		cfg_consume_gold = LOGIC_CONFIG->GetMagicCardCfg().GetOtherCfg().red_chou_card_consume_gold;
		consume_item_id = LOGIC_CONFIG->GetMagicCardCfg().GetOtherCfg().red_chou_card_consume_item_id;
	}
	else
	{
		return;
	}

	static Protocol::SCMagicCardChouCardResult m_mcccr;
	m_mcccr.reward_list_count = 0;

	bool is_use_item = false;
	if(Protocol::SC_MAGIC_CARD_CHOU_CARD_MAX_TIMES == times && MAGIC_CARD_CHOU_CARD_TYPE_ORAGEN == type)
	{
		if (m_role->GetKnapsack()->ConsumeItem(consume_item_id, 1, "MagicCard::ChouCard"))
		{
			is_use_item = true;
		}
	}

	if (Protocol::SC_MAGIC_CARD_CHOU_CARD_MAX_TIMES == times && MAGIC_CARD_CHOU_CARD_TYPE_RED == type)
	{
		if (m_role->GetKnapsack()->ConsumeItem(consume_item_id, 1, "MagicCard::ChouCard"))
		{
			is_use_item = true;
		}
	}

	for (int i = 0; i < times; ++ i)
	{
		const MagicCardChouCardConfig *reward_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetRandomRewardCfg(type);
		if (NULL == reward_cfg)
		{
			continue;
		}

		if (m_param.all_card_num_list[reward_cfg->reward_card_id] + reward_cfg->reward_count >= USHRT_MAX)
		{
			continue;
		}

		int consume_gold = cfg_consume_gold;
		if(true == is_use_item)
		{
			consume_gold = 0;
		}

		{
			const MagicCardOtherConfig &other_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetOtherCfg();
			if(consume_gold > 0 && 1 == times && // 单次抽奖，低级抽奖，还有免费抽奖次数，才给消耗免费次数
				MAGIC_CARD_CHOU_CARD_TYPE_PURPLE == type && 
				m_param.today_purple_free_chou_card_times < other_cfg.day_free_times)
			{
				// 消耗免费次数
				++ m_param.today_purple_free_chou_card_times;
				consume_gold = 0;
			}

			if (consume_gold > 0)
			{
				// 消耗元宝
				if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					break;
				}
				m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "magic_card_choucard");
			}

			// 给予魔卡
			m_param.all_card_num_list[reward_cfg->reward_card_id] += reward_cfg->reward_count;
		}

		if (m_mcccr.reward_list_count < Protocol::SC_MAGIC_CARD_CHOU_CARD_MAX_TIMES)
		{
			m_mcccr.all_reward_index_list[m_mcccr.reward_list_count ++] = reward_cfg->reward_seq;
		}

		//传闻
		if (reward_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_magic_card_reward_content,
				m_role->GetUID(),m_role->GetName(),(int)m_role->GetCamp(),reward_cfg->reward_card_id);
			if(sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_ROLL);
			}
		}


		// 抽魔卡日志
		ROLE_LOG_QUICK10(LOG_TYPE_MAGIC_CARD, m_role, i, times, NULL, NULL, reward_cfg->reward_card_id, reward_cfg->reward_count, consume_gold, 0);

		gamelog::g_log_magic_card.printf(LL_INFO, "MagicCard::ChouCard user[%d, %s] times[%d] card_id[%d] count[%d]", 
			m_role->GetUID(), m_role->GetName(), times, reward_cfg->reward_card_id, reward_cfg->reward_count);
	}

	if (m_mcccr.reward_list_count > 0) // 有抽到奖励才发
	{
		this->SendAllInfo();

		int len = sizeof(m_mcccr) - (Protocol::SC_MAGIC_CARD_CHOU_CARD_MAX_TIMES - m_mcccr.reward_list_count) * sizeof(m_mcccr.all_reward_index_list[0]); 
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&m_mcccr, len);
	}
}

void MagicCard::ExchangeCard(int card_id)
{
	const MagicCardInfoConfig *card_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetCardCfg(card_id);
	if (NULL == card_cfg)
	{
		return;
	}

	if (m_param.all_card_num_list[card_cfg->card_id] >= USHRT_MAX)
	{
		return;
	}

	int sprite_card_id = LOGIC_CONFIG->GetMagicCardCfg().GetSpriteCardId(card_cfg->color);
	if (sprite_card_id <= 0 || sprite_card_id > MAGIC_CARD_MAX_LIMIT_COUNT)
	{
		return;
	}

	if (m_param.all_card_num_list[sprite_card_id] < card_cfg->need_sprit_num)
	{
		m_role->NoticeNum(errornum::EN_MAGIC_CARD_SPRITE_NOT_ENOUGH);
		return;
	}

	int old_card_num = m_param.all_card_num_list[card_cfg->card_id];
	int ole_sprite_num = m_param.all_card_num_list[sprite_card_id];

	m_param.all_card_num_list[sprite_card_id] -= card_cfg->need_sprit_num;
	m_param.all_card_has_exchange_flag |= 1 << card_cfg->card_id;

	++ m_param.all_card_num_list[card_cfg->card_id];

	m_role->NoticeNum(noticenum::NT_MAGICCARD_CARD_EXCHANGE_SUCC);
	this->SendAllInfo();

	// 兑换魔卡日志
	gamelog::g_log_magic_card.printf(LL_INFO, "MagicCard::ExchangeCard user[%d, %s], card_id[%d], old_card_num[%d], new_card_num[%d], old_sprite_num[%d], new_sprite_num[%d]", 
		m_role->GetUID(), m_role->GetName(), card_id, old_card_num, m_param.all_card_num_list[card_cfg->card_id], ole_sprite_num, m_param.all_card_num_list[sprite_card_id]);
}

void MagicCard::SendAllInfo()
{
	static Protocol::SCMagicCardAllInfo mcai;
	mcai.today_purple_free_chou_card_times = m_param.today_purple_free_chou_card_times;
	mcai.all_card_has_exchange_flag = m_param.all_card_has_exchange_flag;
	memcpy(mcai.all_card_num_list, m_param.all_card_num_list, sizeof(mcai.all_card_num_list));
	memcpy(mcai.card_slot_list, m_param.card_slot_list, sizeof(mcai.card_slot_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&mcai, sizeof(mcai));
}

void MagicCard::ActiveSkill(int color)
{
	return;
	//if (color < MAGIC_CARD_COLOR_TYPE_BLUE || color >= MAGIC_CARD_COLOR_TYPE_COLOR_COUNT)
	//{
	//	return;
	//}

	//const MagicCardSuitConfig *suit_use_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetCardSuitCfg(color);
	//if (NULL == suit_use_cfg)
	//{
	//	return;
	//}

	//int need_level = suit_use_cfg->skill_need_level;
	//for (int slot_index = 0; slot_index < MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT; ++ slot_index)
	//{
	//	if (m_param.card_slot_list[color][slot_index].strength_level < need_level)
	//	{
	//		m_role->NoticeNum(errornum::EN_MAIGC_CARD_STRENGTH_LEVEL_NO_ENOUGH);
	//		return;
	//	}
	//}

	//for (int card_color = 0; card_color < MAGIC_CARD_COLOR_TYPE_COLOR_COUNT; ++ card_color)
	//{
	//	const MagicCardSuitConfig *suit_cfg = LOGIC_CONFIG->GetMagicCardCfg().GetCardSuitCfg(card_color);
	//	if (NULL == suit_cfg)
	//	{
	//		return;
	//	}

	//	m_role->GetSkillManager()->RemoveSkill(suit_cfg->skill_id);
	//}

	//m_role->GetSkillManager()->AddMagicCardSkill(suit_use_cfg->skill_id);
	//m_role->NoticeNum(noticenum::NT_MAGICCARD_SKILL_SUCC);
}


void MagicCard::OnGMRequest(int type, int param1, int param2, int param3)
{
	static const int SET_CARD_NUM = 1;
	static const int SET_SLOT_CARD_STRENGTH = 2;
	static const int CLEAR_ALL_DATA = 3;

	switch (type)
	{
	case SET_CARD_NUM:
		{
			if (param1 < 0 || param1 >= MAGIC_CARD_MAX_LIMIT_COUNT)
			{
				return;
			}
			if (param2 >= USHRT_MAX)
			{
				return;
			}

			m_param.all_card_num_list[param1] = param2;
		}
		break;

	case SET_SLOT_CARD_STRENGTH:
		{
			if (param1 < 0 || param1 >= MAGIC_CARD_COLOR_TYPE_COLOR_COUNT)
			{
				return;
			}
			if (param2 < 0 || param2 >= MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT)
			{
				return;
			}

			if (param3 < 0 || param3 > MAGIC_CARD_LIMIT_STRENGTH_LEVEL_MAX)
			{
				return;
			}

			m_param.card_slot_list[param1][param2].strength_level = param3;
		}
		break;

	case CLEAR_ALL_DATA:
		{
			memset(m_param.card_slot_list, 0, sizeof(m_param.card_slot_list));
			memset(m_param.all_card_num_list, 0, sizeof(m_param.all_card_num_list));
		}
		break;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MAGIC_CARD);
	this->SendAllInfo();
}
