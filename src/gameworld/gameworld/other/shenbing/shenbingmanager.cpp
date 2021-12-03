#include "shenbingmanager.hpp"
#include "servercommon/configcommon.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "config/shuxingdanconfig.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/serverdef.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/string/gameworldstr.h"
#include "serverlogic.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "gamelog.h"
#include "world.h"
#include "gstr.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"
#include "protocal/msgshenbing.hpp"

ShenBingManager::ShenBingManager() : m_role(NULL), m_last_add_gongji(0)
{
	memset(m_skill_level_list, 0, sizeof(m_skill_level_list));
}

ShenBingManager::~ShenBingManager()
{
}

void ShenBingManager::Init(Role *role, const ShenBingParam &shenbing_param)
{
	m_role = role;
	m_shenbing_param = shenbing_param;
	this->RefreshSkillLevel();
}

void ShenBingManager::GetInitParam(ShenBingParam *shenbing_param)
{
	*shenbing_param = m_shenbing_param;
}

void ShenBingManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		CharIntAttrs base_attrs;
		base_attrs.Reset();
		//等级加成
		{
			const ShenBingLevelCfg *level_cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingLevelCfg(m_shenbing_param.level);
			if (NULL != level_cfg && m_shenbing_param.level > 0)
			{
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += level_cfg->extra_zengshang;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += level_cfg->extra_mianshang;
				// 特殊属性
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += level_cfg->per_jingzhun;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += level_cfg->per_baoji;
// 				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += level_cfg->per_mianshang;
// 				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += level_cfg->per_pofang;
// 				base_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += level_cfg->pvp_zengshang_per;
// 				base_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += level_cfg->pve_zengshang_per;
// 				base_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += level_cfg->pvp_jianshang_per;
// 				base_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += level_cfg->pve_jianshang_per;

				// 下面特殊属性用乘法
				base_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, level_cfg->per_mianshang);
				base_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, level_cfg->per_pofang);
				base_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, level_cfg->pvp_zengshang_per);
				base_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, level_cfg->pve_zengshang_per);
				base_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, level_cfg->pvp_jianshang_per);
				base_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, level_cfg->pve_jianshang_per);

			}
		}

		m_attrs_add.Add(base_attrs.m_attrs);

		// 属性丹, 屏蔽属性丹
		/*{
			int shuxingdan_attr_per = 0;
			for (int slot_index = 0; slot_index < ShenBingParam::SHUXINGDAN_MAX_TYPE; slot_index++)
			{
				const int num = m_shenbing_param.shuxingdan_list[slot_index];
				if (num > 0)
				{
					const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHENBING, slot_index);
					if (nullptr != cfg_item)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren * num;
						shuxingdan_attr_per += cfg_item->attr_per * num;
					}
				}
			}

			for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
			{
				m_attrs_add.m_attrs[i] += static_cast<Attribute>(base_attrs.m_attrs[i] * (ROLE_ATTR_PER * shuxingdan_attr_per));
			}
		}*/
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENBING, m_attrs_add);
	m_shenbing_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENBING);
	base_add.Add(m_attrs_add.m_attrs, true);
}

void ShenBingManager::Update(time_t now_second)
{
	this->OnShenBingSkillEffect(now_second);
}

void ShenBingManager::OnShenBingSkillEffect(time_t now_sec)
{
	return; //屏蔽技能
	// 技能4：每损失a% 的气血， 额外增加自己的攻击力b% 
	static const int SKILL_ID3 = 3;
	UNSTD_STATIC_CHECK(ShenBingConfig::MAX_SHENBING_SKILL_COUNT > SKILL_ID3);

	const ShenBingSkillCfg *skill3_cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingSkillCfg(SKILL_ID3, m_skill_level_list[SKILL_ID3]);
	if (NULL != skill3_cfg)
	{
		Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		Attribute max_gongji = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI);
		Attribute delta_hp = max_hp - cur_hp;
		if (delta_hp > 0)
		{

			Attribute add_gongji_per_hp = (Attribute)(max_hp * (skill3_cfg->param_a * ROLE_ATTR_PER));
			if (add_gongji_per_hp == 0)
			{
				return;
			}
			Attribute add_gongji = (Attribute)(max_gongji * (skill3_cfg->param_b * ROLE_ATTR_PER) * (delta_hp / add_gongji_per_hp));

			if (m_last_add_gongji != add_gongji)
			{
				m_last_add_gongji = add_gongji;
				m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SKILL, SKILL_ID3);

				if (add_gongji > 0)
				{
					EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), 1, 99999000, (char)m_skill_level_list[SKILL_ID3]);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
					effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, add_gongji, 0);
					m_role->AddEffect(effect_buff);
				}
			}
		}
	}
}

void ShenBingManager::OnHurtRole(Character *target)
{
	return; //屏蔽技能
	if (Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		// 技能2：攻击的时候，a%概率触发狂暴效果，攻击力提升b %  ，持续c秒，冷却d秒  ,pvp才生效
		{
			const unsigned short SKILL_ID1 = 1;
			UNSTD_STATIC_CHECK(ShenBingConfig::MAX_SHENBING_SKILL_COUNT > SKILL_ID1);

			if (m_skill_level_list[SKILL_ID1] > 0)
			{
				const ShenBingSkillCfg *skill1_cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingSkillCfg(SKILL_ID1, m_skill_level_list[SKILL_ID1]);
				if (NULL != skill1_cfg)
				{
					unsigned long long now_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
					if (now_ms > m_shenbing_param.last_skill1_time + skill1_cfg->param_d)
					{
						if (RandomNum(10000) < skill1_cfg->param_a)
						{
							m_shenbing_param.last_skill1_time = now_ms;
							// 自己加攻
							EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), 1, skill1_cfg->param_c, (char)m_skill_level_list[SKILL_ID1]);
							effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
							int value = (int)(m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (skill1_cfg->param_b * ROLE_ATTR_PER));
							effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, skill1_cfg->param_b);
							effect_buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_GONGJI);					
							m_role->AddEffect(effect_buff);
						}
					}
				}
			}
		}

		// 技能3：攻击的时候，param_a%概率触发破甲效果，敌人防御下降param_b% ，持续param_c秒，冷却param_d秒
		{
			const unsigned short SKILL_ID2 = 2;
			UNSTD_STATIC_CHECK(ShenBingConfig::MAX_SHENBING_SKILL_COUNT > SKILL_ID2);

			if (m_skill_level_list[SKILL_ID2] > 0)
			{
				const ShenBingSkillCfg *skill2_cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingSkillCfg(SKILL_ID2, m_skill_level_list[SKILL_ID2]);
				if (NULL != skill2_cfg)
				{
					unsigned long long now_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
					if (now_ms > m_shenbing_param.last_skill2_time + skill2_cfg->param_d)
					{
						if (RandomNum(10000) < skill2_cfg->param_a)
						{
							m_shenbing_param.last_skill2_time = now_ms;

							// 目标降防
							EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), 1, skill2_cfg->param_c,  (char)m_skill_level_list[SKILL_ID2]);			
							effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
							int value = (int)(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (skill2_cfg->param_b * ROLE_ATTR_PER) * -1);
							effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, value, skill2_cfg->param_b);
							effect_buff->SetClientEffectType(EffectBase::ECT_SKILL_DEC_FANGYU);					
							target->AddEffect(effect_buff);
						}
					}
				}
			}
		}
	}
}

int ShenBingManager::OnHurtBoss()
{
	return 0; //屏蔽技能

	//技能1：玩家对BOSS伤害提升a %
	const unsigned short SKILL_ID0 = 0;
	UNSTD_STATIC_CHECK(ShenBingConfig::MAX_SHENBING_SKILL_COUNT > SKILL_ID0);

	const ShenBingSkillCfg *skill0_cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingSkillCfg(SKILL_ID0, m_skill_level_list[SKILL_ID0]);
	if (NULL != skill0_cfg)
	{
		return skill0_cfg->param_a;
	}

	return 0;
}


void ShenBingManager::SendAllShenBingInfo()
{
	static Protocol::SCAllShenBingInfo pro;
	pro.level = m_shenbing_param.level;
	pro.use_image = m_shenbing_param.use_image;
	pro.exp = m_shenbing_param.exp;

	ARRAYCPY(pro.shuxingdan_list, m_shenbing_param.shuxingdan_list);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void ShenBingManager::RefreshSkillLevel()
{
	return; //屏蔽技能

	memset(m_skill_level_list, 0, sizeof(m_skill_level_list));

	for (int i = 0; i < ShenBingConfig::MAX_SHENBING_SKILL_COUNT; i++)
	{
		for (int k = 1; k <= ShenBingConfig::MAX_SHENBING_SKILL_LEVEL; k++)
		{
			const ShenBingSkillCfg *cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingSkillCfg(i, k);
			if (NULL != cfg)
			{
				if (m_shenbing_param.level >= cfg->shenbing_level)
				{
					m_skill_level_list[i] = k;
				}
			}
		}
	}
}

bool ShenBingManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	return false; //屏蔽属性丹

	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const ShenBingLevelCfg *cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingLevelCfg(m_shenbing_param.level);
	if (NULL == cfg)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHENBING, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_shenbing_param.level < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_level_limit", cfg_item->order_limit, slot_idx);
		return false;
	}

	if (m_shenbing_param.shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int shuxingdan_limit = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? cfg->shuxingdan_limit : cfg->chengzhangdan_limit;

	// 策划需求，修正真正使用数量
	int can_use_count = shuxingdan_limit - m_shenbing_param.shuxingdan_list[slot_idx];
	if (can_use_count > 0)
	{
		if (can_use_count < use_count)
		{
			use_count = can_use_count;
		}
	}
	else
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	m_shenbing_param.shuxingdan_list[slot_idx] += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENBING);
	this->SendAllShenBingInfo();

	return true;
}

bool ShenBingManager::UpLevel(Role *role, short stuff_index)
{
	if (NULL == role) return false;

	if (stuff_index < 0 || stuff_index >= ShenBingConfig::MAX_UP_LEVEL_ITEM_COUNT)
		return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (role->GetSublock()->CheckHasLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	short old_level = m_shenbing_param.level;

	const ShenBingLevelCfg * curr_level_cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingLevelCfg(old_level);
	if (NULL == curr_level_cfg)
	{
		return false;
	}

	const ShenBingLevelCfg * next_level_cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingLevelCfg(old_level + 1);
	if (NULL == next_level_cfg)
	{
		return false;
	}

	const ShenBingUpLevelStuffCfg * up_level_stuff_cfg = LOGIC_CONFIG->GetShenBingConfig().GetUpLevelStuffCfg(stuff_index);
	if (NULL == up_level_stuff_cfg)
	{
		return false;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(up_level_stuff_cfg->up_level_item_id);
	if (NULL == item_base)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = up_level_stuff_cfg->up_level_item_id;
	stuff_list[0].num = 1;

	if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
	{
		return false;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(role, consume_list, "ShenBingManager::UpLevel"))
	{
		return false;
	}

	// 结算
	{
		int rand_value = RandomNum(10000);
		if (rand_value < up_level_stuff_cfg->baoji_per)
		{
			m_shenbing_param.exp += up_level_stuff_cfg->level_exp * up_level_stuff_cfg->baoji_mul;
		}
		else
		{
			m_shenbing_param.exp += up_level_stuff_cfg->level_exp;
		}

		int up_level_need_exp = curr_level_cfg->uplevel_exp;
		for (int loop_times = 0; loop_times < 100; ++loop_times)
		{
			if (up_level_need_exp <= 0)
			{
				m_shenbing_param.exp = 0;
				break;
			}

			if (m_shenbing_param.exp < up_level_need_exp)
			{
				break;
			}

			const ShenBingLevelCfg *level_cfg = LOGIC_CONFIG->GetShenBingConfig().GetShenBingLevelCfg(m_shenbing_param.level + 1);
			if (NULL == level_cfg)
			{
				break;
			}

			m_shenbing_param.exp -= up_level_need_exp;
			m_shenbing_param.level++;
			if (level_cfg->is_broadcast)
			{
				int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenbing_level_up,
					UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), m_shenbing_param.level);
				if (sendlen > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}

			up_level_need_exp = level_cfg->uplevel_exp;
			this->RefreshSkillLevel();
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENBING);	
	}

	role->GetShenBingManager()->SendAllShenBingInfo();

	role->GetShenBingManager()->PrintShenBing("ShenBingUplevel", old_level, m_shenbing_param.level);

	EventHandler::Instance().OnUplevelShenBing(role, m_shenbing_param.level);

	return true;
}

void ShenBingManager::PrintShenBing(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == ShenBingConfig::MAX_SHENBING_SKILL_COUNT);

	gamelog::g_log_shenbing.printf(LL_INFO, "ShenBing role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"level:%d, zizhidan_count:%d chengzhangdan_count:%d exp:%d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_shenbing_param.level,(int)m_shenbing_param.shuxingdan_list[0], (int)m_shenbing_param.shuxingdan_list[1],
		(int)m_shenbing_param.exp);
}