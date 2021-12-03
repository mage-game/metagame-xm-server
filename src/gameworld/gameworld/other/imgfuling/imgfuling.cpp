#include "imgfuling.hpp"
#include "other/imgfuling/imgfulingconfig.hpp"
#include "protocal/msgimgfuling.hpp"

#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "engineadapter.h"
#include "other/capability/capability.hpp"
#include "item/knapsack.h"
#include "item/itemextern.hpp"
#include "item/itempool.h"
#include "servercommon/errornum.h"
#include "other/event/eventhandler.hpp"
#include "other/sublock/sublock.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"

#include "other/mount/mountmanager.hpp"
#include "other/mount/mountopconfig.hpp"

#include "other/wing/wingmanager.hpp"
#include "other/wing/wingconfig.hpp"

#include "other/halo/halomanager.hpp"

#include "other/fightmount/fightmountmanager.hpp"
#include "other/fightmount/fightmountconfig.hpp"

#include "other/shengong/shengongmanager.hpp"
#include "other/shengong/shengongconfig.hpp"

#include "other/shenyi/shenyimanager.hpp"
#include "other/shenyi/shenyiconfig.hpp"

#include "other/footprint/footprint.hpp"
#include "other/footprint/footprintconfig.hpp"

#include "effect/skilleffect/effecthpstore.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectabsorbhp.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/fabao/fabaoconfig.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/shizhuang/shizhuangconfig.h"

ImgFuling::ImgFuling() : m_role(NULL)
{
	memset(m_skill_next_perform_time, 0, sizeof(m_skill_next_perform_time));
}

ImgFuling::~ImgFuling()
{

}

void ImgFuling::Init(Role *role, const ImgFulingParam &param)
{
	m_role = role;
	m_param = param;
}

void ImgFuling::GetInitParam(ImgFulingParam *param)
{
	*param = m_param;
}

void ImgFuling::OnImgFulingOperate(short operate_type, short param_1, short param_2, short param_3)
{
	switch (operate_type)
	{
	case Protocol::IMG_FULING_OPERATE_TYPE_INFO_REQ:
	{
		this->SendAllImgFulingInfo();
	}
	break;

	case Protocol::IMG_FULING_OPERATE_TYPE_LEVEL_UP:
	{
		this->OnLevelUp(param_1, param_2);
	}
	break;

	default:
		break;
	}
}

bool ImgFuling::OnLevelUp(int jingjie_type, int stuff_index)
{
	if (jingjie_type < JINGJIE_TYPE_MOUNT || jingjie_type >= JINGJIE_TYPE_COUNT)
	{
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	short old_level = m_param.fuling_list[jingjie_type].level;

	const ImgFulingLevelCfg * curr_exp_cfg = LOGIC_CONFIG->GetImgFulingConfig().GetLevelCfg(jingjie_type, old_level);
	if (NULL == curr_exp_cfg)
	{
		return false;
	}

	const ImgFulingLevelCfg * next_exp_cfg = LOGIC_CONFIG->GetImgFulingConfig().GetLevelCfg(jingjie_type, old_level + 1);
	if (NULL == next_exp_cfg)
	{
		return false;
	}

	const ImgFulingStuffCfg * up_level_stuff_cfg = LOGIC_CONFIG->GetImgFulingConfig().GetStuffCfg(jingjie_type, stuff_index);
	if (NULL == up_level_stuff_cfg)
	{
		return false;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(up_level_stuff_cfg->stuff_id);
	if (NULL == item_base)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = up_level_stuff_cfg->stuff_id;
	stuff_list[0].num = 1;
	stuff_list[0].buyable = false;

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
	{
		return false;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ImgFuling::OnLevelUp"))
	{
		return false;
	}

	// 结算
	{
		m_param.fuling_list[jingjie_type].cur_exp += up_level_stuff_cfg->add_exp;
		unsigned int up_level_need_exp = curr_exp_cfg->exp;
		for (int loop_times = 0; loop_times < 100; ++loop_times)
		{
			if (m_param.fuling_list[jingjie_type].cur_exp >= up_level_need_exp)
			{
				m_param.fuling_list[jingjie_type].cur_exp -= up_level_need_exp;
				++m_param.fuling_list[jingjie_type].level;
				if (next_exp_cfg->broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_img_fuling_level_up,
						m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), jingjie_type, m_param.fuling_list[jingjie_type].level);
					if (sendlen > 0)
					{
						//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
					}
				}

				curr_exp_cfg = LOGIC_CONFIG->GetImgFulingConfig().GetLevelCfg(jingjie_type, m_param.fuling_list[jingjie_type].level);
				if (NULL == curr_exp_cfg)
				{
					break;
				}

				next_exp_cfg = LOGIC_CONFIG->GetImgFulingConfig().GetLevelCfg(jingjie_type, m_param.fuling_list[jingjie_type].level + 1);
				if (NULL == next_exp_cfg)
				{
					break;
				}

				up_level_need_exp = curr_exp_cfg->exp;
			}
			else
			{
				break;
			}
		}
	}

	if (m_param.fuling_list[jingjie_type].level > old_level)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);
	}

	this->SendAllImgFulingInfo();
	return true;
}

void ImgFuling::OnPutOnImg(int jingjie_type, int img_id)
{
	if (jingjie_type < 0 || jingjie_type >= JINGJIE_TYPE_COUNT)
	{
		return;
	}

	if (m_param.fuling_list[jingjie_type].img_count < IMG_FULING_SLOT_COUNT)
	{
		m_param.fuling_list[jingjie_type].img_id_list[m_param.fuling_list[jingjie_type].img_count] = static_cast<unsigned char>(img_id);
		++m_param.fuling_list[jingjie_type].img_count;
	}

	int next_skill_level = m_param.fuling_list[jingjie_type].skill_level + 1;
	int index = LOGIC_CONFIG->GetImgFulingConfig().GetSkillIndexByJingJieType(jingjie_type);
	const ImgFulingSkillCfg *cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(index, next_skill_level);
	if (NULL != cfg && m_param.fuling_list[jingjie_type].img_count >= cfg->img_count_limit)
	{
		++m_param.fuling_list[jingjie_type].skill_level;
		if (cfg->broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_img_fuling_skill_level_up,
				m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), m_param.fuling_list[jingjie_type].img_count, jingjie_type, index);
			if (sendlen > 0)
			{
				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
			}
		}
	}

	this->SendAllImgFulingInfo();
}

void ImgFuling::CheckFirstLoadImg()
{
	if (1 == m_param.first_check)
	{
		return;
	}

	this->FirstSetImgID(JINGJIE_TYPE_MOUNT, m_role->GetMountManager()->GetMountParam()->active_special_image_flag, sizeof(m_role->GetMountManager()->GetMountParam()->active_special_image_flag));
	this->FirstSetImgID(JINGJIE_TYPE_WING, m_role->GetWing()->GetWingParam()->active_special_image_flag, sizeof(m_role->GetWing()->GetWingParam()->active_special_image_flag));
	this->FirstSetImgID(JINGJIE_TYPE_HALO, m_role->GetHalo()->GetHaloParam()->active_special_image_flag, sizeof(m_role->GetHalo()->GetHaloParam()->active_special_image_flag));
	this->FirstSetImgID(JINGJIE_TYPE_FIGHT_MOUNT, m_role->GetFightMountManager()->GetFightMountParam()->active_special_image_flag, sizeof(m_role->GetFightMountManager()->GetFightMountParam()->active_special_image_flag));
	this->FirstSetImgID(JINGJIE_TYPE_SHIZHUANG, m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_BODY].special_img_active_flag, sizeof(m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_BODY].special_img_active_flag));
	this->FirstSetImgID(JINGJIE_TYPE_SHIZHUANG_WEAPON, m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_WUQI].special_img_active_flag, sizeof(m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_WUQI].special_img_active_flag));
	this->FirstSetImgID(JINGJIE_TYPE_FOOT_PRINT, m_role->GetFootPrint()->GetFootprintParam()->active_special_image_flag, sizeof(m_role->GetFootPrint()->GetFootprintParam()->active_special_image_flag));
	this->FirstSetImgID(JINGJIE_TYPE_FABAO, m_role->GetFabaoManager()->GetParam()->active_special_image_flag, sizeof(m_role->GetFabaoManager()->GetParam()->active_special_image_flag));

	m_param.first_check = 1;
	this->SendAllImgFulingInfo();
}

void ImgFuling::SendAllImgFulingInfo()
{
	static Protocol::SCImgFulingInfo pro;
	memcpy(pro.fuling_list, m_param.fuling_list, sizeof(pro.fuling_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void ImgFuling::OnAttack(Attribute delta_hp)
{
	int jingjie_type = -1;
	//  受到攻击后，有[param_a] % 概率触发一个护盾，护盾为自身血量的[param_b] % ，持续[param_c]秒，触发间隔最少[param_d]秒
	if (this->CanPerformSkill(IMG_FULING_SKILL_0))
	{
		jingjie_type = LOGIC_CONFIG->GetImgFulingConfig().GetJingJieTypeBySkillIndex(IMG_FULING_SKILL_0);		
		if (jingjie_type >= 0 && jingjie_type < JINGJIE_TYPE_COUNT)
		{
			const ImgFulingSkillCfg *cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(IMG_FULING_SKILL_0, m_param.fuling_list[jingjie_type].skill_level);
			if (NULL != cfg)
			{
				Attribute store_value = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (cfg->param_b * 0.01));
				EffectHpStore *effect_hp_store = new EffectHpStore(m_role->GetId(), IMG_FULING_SKILL_0, cfg->param_c, store_value,
					static_cast<char>(m_param.fuling_list[jingjie_type].skill_level), EffectBase::PRODUCT_METHOD_IMG_FULING);
				effect_hp_store->SetReplacePercent(100);
				effect_hp_store->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
				m_role->AddEffect(effect_hp_store);
				m_skill_next_perform_time[IMG_FULING_SKILL_0] = static_cast<unsigned int>(EngineAdapter::Instance().Time() + (cfg->param_d * 0.001));
			}
		}
	}

	//	受到攻击后，有[param_a] % 概率触发无敌状态，受到伤害降为1点，并且增加移动速度[param_b] % ，持续[param_c]秒，触发间隔最少[param_d]秒
	if (this->CanPerformSkill(IMG_FULING_SKILL_3))
	{
		jingjie_type = LOGIC_CONFIG->GetImgFulingConfig().GetJingJieTypeBySkillIndex(IMG_FULING_SKILL_3);
		if (jingjie_type >= 0 && jingjie_type < JINGJIE_TYPE_COUNT)
		{
			const ImgFulingSkillCfg *cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(IMG_FULING_SKILL_3, m_param.fuling_list[jingjie_type].skill_level);
			if (NULL != cfg)
			{
				EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_role->GetId(), IMG_FULING_SKILL_3, cfg->param_c, EBT_JIASU,
					static_cast<char>(m_param.fuling_list[jingjie_type].skill_level), EffectBase::PRODUCT_METHOD_IMG_FULING);
				effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
				int value = static_cast<int>(m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (cfg->param_b * 0.01));
				effect_speed->SetMoveSpeed(value, cfg->param_b);
				m_role->AddEffect(effect_speed);

				EffectSpecialState *effect_state = new EffectSpecialState(m_role->GetId(), IMG_FULING_SKILL_3, cfg->param_c, EBT_WUDI,
					static_cast<char>(m_param.fuling_list[jingjie_type].skill_level), EffectBase::PRODUCT_METHOD_IMG_FULING);
				effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
				effect_state->SetClientEffectType(EffectBase::ECT_IMG_FULING_WUDI);
				m_role->AddEffect(effect_state);
				m_skill_next_perform_time[IMG_FULING_SKILL_3] = static_cast<unsigned int>(EngineAdapter::Instance().Time() + (cfg->param_d * 0.001));
			}
		}
	}

	//血量低于[param_e]%的时候必定触发，身体变大30%，受到的伤害结果直接减少[param_b]%，持续[param_c]秒，冷却[param_d]秒
	if (this->CanPerformSkill(IMG_FULING_SKILL_5))
	{
		jingjie_type = LOGIC_CONFIG->GetImgFulingConfig().GetJingJieTypeBySkillIndex(IMG_FULING_SKILL_5);
		if (jingjie_type >= 0 && jingjie_type < JINGJIE_TYPE_COUNT)
		{
			const ImgFulingSkillCfg *cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(IMG_FULING_SKILL_5, m_param.fuling_list[jingjie_type].skill_level);
			if (NULL != cfg)
			{
				Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				if (cur_hp - delta_hp > static_cast<Attribute>(max_hp * (cfg->param_e * 0.01)) && cur_hp < static_cast<Attribute>(max_hp * (cfg->param_e * 0.01)))
				{
					EffectDecHurt *dec_hurt_effect = new EffectDecHurt(m_role->GetId(), IMG_FULING_SKILL_5, cfg->param_c,
						static_cast<char>(m_param.fuling_list[jingjie_type].skill_level), EffectBase::PRODUCT_METHOD_IMG_FULING, EBT_MOHUA);
					dec_hurt_effect->SetDecPercent(cfg->param_b * 100);
					dec_hurt_effect->SetClientEffectType(EffectBase::ECT_IMG_FULING_MOHUA);
					m_role->AddEffect(dec_hurt_effect);
					m_skill_next_perform_time[IMG_FULING_SKILL_5] = static_cast<unsigned int>(EngineAdapter::Instance().Time() + (cfg->param_d * 0.001));
				}
			}
		}
	}

	//受到使生命值低于param_a%的伤害的时候触发，触发后抵挡一次伤害，伤害为1（现在先做成持续一帧(一秒三帧)的效果，如果不行，后面再改）,在接下来的param_b秒内降低受到的伤害的param_c%，param_d秒冷却
	if (this->CanPerformSkill(IMG_FULING_SKILL_7))
	{
		jingjie_type = LOGIC_CONFIG->GetImgFulingConfig().GetJingJieTypeBySkillIndex(IMG_FULING_SKILL_7);
		if (jingjie_type >= 0 && jingjie_type < JINGJIE_TYPE_COUNT)
		{
			const ImgFulingSkillCfg* cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(IMG_FULING_SKILL_7, m_param.fuling_list[jingjie_type].skill_level);
			Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			if (cur_hp + delta_hp <= static_cast<Attribute>(max_hp * (cfg->param_a * 0.01)))
			{
				EffectSpecialState *effect_state = new EffectSpecialState(m_role->GetId(), IMG_FULING_SKILL_7, (int)(1000/3) + 1, EBT_WUDI,
					                                                      static_cast<char>(m_param.fuling_list[jingjie_type].skill_level),
					                                                      EffectBase::PRODUCT_METHOD_IMG_FULING);
				effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
				effect_state->SetClientEffectType(EffectBase::ECT_IMG_FULING_WUDI);
				m_role->AddEffect(effect_state);

				EffectDecHurt* dec_hurt_effect = new EffectDecHurt(m_role->GetId(), IMG_FULING_SKILL_7, cfg->param_b,
					                                               static_cast<char>(m_param.fuling_list[jingjie_type].skill_level),
					                                               EffectBase::PRODUCT_METHOD_IMG_FULING, EBT_DEC_HURT);
				dec_hurt_effect->SetDecPercent(cfg->param_c * 100);
				dec_hurt_effect->SetClientEffectType(EffectBase::ECT_SKILL_DEC_ON_HURT);
				m_role->AddEffect(dec_hurt_effect);
			}
			m_skill_next_perform_time[IMG_FULING_SKILL_7] = static_cast<unsigned int>(EngineAdapter::Instance().Time() + (cfg->param_d * 0.001));
		}
	}

}

void ImgFuling::OnHurtOther(Character *target, long long base_injure, long long &injure, int target_index)
{
	int jingjie_type = -1;
	//	攻击敌方，有[param_a] % 概率眩晕对手[param_c]秒，触发间隔最少[param_d]秒
	if (this->CanPerformSkill(IMG_FULING_SKILL_1))
	{
		jingjie_type = LOGIC_CONFIG->GetImgFulingConfig().GetJingJieTypeBySkillIndex(IMG_FULING_SKILL_1);
		if (jingjie_type >= 0 && jingjie_type < JINGJIE_TYPE_COUNT)
		{
			const ImgFulingSkillCfg *cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(IMG_FULING_SKILL_1, m_param.fuling_list[jingjie_type].skill_level);
			if (NULL != cfg)
			{
				EffectSpecialState *effect_yun = new EffectSpecialState(m_role->GetId(), IMG_FULING_SKILL_1, cfg->param_c, EBT_XUANYUN,
					static_cast<char>(m_param.fuling_list[jingjie_type].skill_level), EffectBase::PRODUCT_METHOD_IMG_FULING);
				effect_yun->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
				effect_yun->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
				target->AddEffect(effect_yun);
				m_skill_next_perform_time[IMG_FULING_SKILL_1] = static_cast<unsigned int>(EngineAdapter::Instance().Time() + (cfg->param_d * 0.001));
			}
		}
	}

	//	攻击敌方，有[param_a] % 概率触发狂暴状态，当前攻击增加[param_b] % ，持续[param_c]秒，触发间隔最少[param_d]秒
	if (this->CanPerformSkill(IMG_FULING_SKILL_2))
	{
		jingjie_type = LOGIC_CONFIG->GetImgFulingConfig().GetJingJieTypeBySkillIndex(IMG_FULING_SKILL_2);
		if (jingjie_type >= 0 && jingjie_type < JINGJIE_TYPE_COUNT)
		{
			const ImgFulingSkillCfg *cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(IMG_FULING_SKILL_2, m_param.fuling_list[jingjie_type].skill_level);
			if (NULL != cfg)
			{
				EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), IMG_FULING_SKILL_2, cfg->param_c,
					static_cast<char>(m_param.fuling_list[jingjie_type].skill_level), EffectBase::PRODUCT_METHOD_IMG_FULING);
				effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				int value = (int)(m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (cfg->param_b * 0.01));
				effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, cfg->param_b * 100);
				effect_buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_GONGJI);
				m_role->AddEffect(effect_buff);
				m_skill_next_perform_time[IMG_FULING_SKILL_2] = static_cast<unsigned int>(EngineAdapter::Instance().Time() + (cfg->param_d * 0.001));
			}
		}
	}

	//	攻击敌方，有[param_a] % 概率触发[param_b] % 吸血状态，持续有[param_c]秒，触发间隔最少[param_d]秒
	if (this->CanPerformSkill(IMG_FULING_SKILL_4))
	{
		jingjie_type = LOGIC_CONFIG->GetImgFulingConfig().GetJingJieTypeBySkillIndex(IMG_FULING_SKILL_4);
		if (jingjie_type >= 0 && jingjie_type < JINGJIE_TYPE_COUNT)
		{
			const ImgFulingSkillCfg *cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(IMG_FULING_SKILL_4, m_param.fuling_list[jingjie_type].skill_level);
			if (NULL != cfg)
			{
				EffectAbsorbHP *effect = new EffectAbsorbHP(m_role->GetId(), IMG_FULING_SKILL_4, cfg->param_c, cfg->param_b,
					static_cast<char>(m_param.fuling_list[jingjie_type].skill_level), EffectBase::PRODUCT_METHOD_IMG_FULING);
				effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
				effect->SetClientEffectType(EffectBase::ECT_IMG_FULING_ABSORB);
				m_role->AddEffect(effect);
				m_skill_next_perform_time[IMG_FULING_SKILL_4] = static_cast<unsigned int>(EngineAdapter::Instance().Time() + (cfg->param_d * 0.001));
			}
		}
	}

	//对血量低于[param_b]%的玩家，有[param_a]%的概率直接斩杀，隔[param_d]秒才会触发一次，每次触发只能触发一个人（主目标）
	if (0 == target_index && this->CanPerformSkill(IMG_FULING_SKILL_6))
	{
		jingjie_type = LOGIC_CONFIG->GetImgFulingConfig().GetJingJieTypeBySkillIndex(IMG_FULING_SKILL_6);
		if (jingjie_type >= 0 && jingjie_type < JINGJIE_TYPE_COUNT)
		{
			const ImgFulingSkillCfg *cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(IMG_FULING_SKILL_6, m_param.fuling_list[jingjie_type].skill_level);
			if (NULL != cfg && Obj::OBJ_TYPE_ROLE == target->GetObjType())
			{
				if (target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP) <
					static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (cfg->param_b * 0.01)))
				{
					injure = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
					m_skill_next_perform_time[IMG_FULING_SKILL_6] = static_cast<unsigned int>(EngineAdapter::Instance().Time() + (cfg->param_d * 0.001));
				}
			}
		}
	}
}

void ImgFuling::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int i = 0; i < JINGJIE_TYPE_COUNT; ++i)
		{
			const ImgFulingLevelCfg *level_cfg = LOGIC_CONFIG->GetImgFulingConfig().GetAttrCfg(i, m_param.fuling_list[i].level);
			if (NULL == level_cfg)
			{
				continue;
			}

			int per_add = level_cfg->per_add;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;

			CharIntAttrs extra_attr;
			switch (i)
			{
			case JINGJIE_TYPE_MOUNT:
			{
				int level = m_role->GetMountManager()->GetMountParam()->star_level;
				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetMountOpConfig().GetUpStarExpCfg(level));
			}
			break;

			case JINGJIE_TYPE_WING:
			{
				int level = m_role->GetWing()->GetWingParam()->star_level;
				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(level));
			}
			break;

			case JINGJIE_TYPE_HALO:
			{
				int level = m_role->GetHalo()->GetHaloParam()->star_level;
				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(level));
			}
			break;

			case JINGJIE_TYPE_FIGHT_MOUNT:
			{
				int level = m_role->GetFightMountManager()->GetFightMountParam()->star_level;
				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(level));
			}
			break;

			case JINGJIE_TYPE_SHIZHUANG:
			{
//				int level = m_role->GetShengong()->GetShengongParam()->star_level;
//				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetShengongConfig().GetUpStarExpCfg(level));

				int grade = m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_BODY].grade;
				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(grade));
			}
			break;

			case JINGJIE_TYPE_SHIZHUANG_WEAPON:
			{
//				int level = m_role->GetShenyi()->GetShenyiParam()->star_level;
//				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(level));

				int grade = m_role->GetShizhuang()->GetParam()->item_list[SHIZHUANG_TYPE_WUQI].grade;
				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(grade));
			}
			break;

			case JINGJIE_TYPE_FOOT_PRINT:
			{
				int level = m_role->GetFootPrint()->GetFootprintParam()->star_level;
				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(level));
			}
			break;

			case JINGJIE_TYPE_FABAO:
			{
				int grade = m_role->GetFabaoManager()->GetParam()->grade;
				this->AddJingJiePerAttr(extra_attr, per_add, LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(grade));
			}
			break;

			default:
				break;
			}

			m_attrs_add.Add(extra_attr.m_attrs);
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_IMG_FULING, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

bool ImgFuling::CanPerformSkill(int index)
{
	if (index < 0 || index >= IMG_FULING_SKILL_MAX)
	{
		return false;
	}

	int jingjie_type = LOGIC_CONFIG->GetImgFulingConfig().GetJingJieTypeBySkillIndex(index);
	if (jingjie_type < 0 || jingjie_type >= JINGJIE_TYPE_COUNT)
	{
		return false;
	}

	const ImgFulingSkillCfg *cfg = LOGIC_CONFIG->GetImgFulingConfig().GetSkillCfg(index, m_param.fuling_list[jingjie_type].skill_level);
	if (NULL == cfg)
	{
		return false;
	}

	if (static_cast<int>(m_param.fuling_list[jingjie_type].img_count) < cfg->img_count_limit)
	{
		return false;
	}

	if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) < m_skill_next_perform_time[index])
	{
		return false;
	}

	if(index != IMG_FULING_SKILL_7)
	{
		if (RandomNum(100) >= cfg->param_a)
		{
			return false;
		}
	}

	return true;
}

void ImgFuling::FirstSetImgID(int jingjie_type, unsigned char *active_flag, int active_flag_byte)
{
	if (jingjie_type < 0 || jingjie_type >= JINGJIE_TYPE_COUNT || nullptr == active_flag || active_flag_byte <= 0)
	{
		return;
	}
	int flag_count = active_flag_byte * BIT_COUNT_OF_BYTE;
	if (flag_count > UCHAR_MAX + 1)
	{
		flag_count = UCHAR_MAX + 1;
	}
	for (int i = 0; i < flag_count; ++i)
	{
		if (static_cast<unsigned int>(m_param.fuling_list[jingjie_type].img_count) >= IMG_FULING_SLOT_COUNT)
		{
			break;
		}

		if (0 == m_param.fuling_list[jingjie_type].img_id_list[m_param.fuling_list[jingjie_type].img_count]
			&& BitmapFunction::IsBitSet(active_flag, active_flag_byte, i))
		{
			m_param.fuling_list[jingjie_type].img_id_list[m_param.fuling_list[jingjie_type].img_count] = static_cast<unsigned char>(i);
			++m_param.fuling_list[jingjie_type].img_count;
		}
	}

	m_param.fuling_list[jingjie_type].skill_level = LOGIC_CONFIG->GetImgFulingConfig().GetSkillLevelByCount(jingjie_type, m_param.fuling_list[jingjie_type].img_count);
}