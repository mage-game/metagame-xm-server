#include "mojiemanager.hpp"
#include "servercommon/configcommon.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/mojie/mojieconfig.hpp"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "gameworld/scene/scenemanager.h"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/world.h"
#include "protocal/msgother.h"
#include "serverlogic.h"
#include "effect/effectbase.hpp"
#include "servercommon/struct/mojieparam.hpp"
#include "protocal/msgmojie.hpp"
#include "skill/skillpool.hpp"
#include "skill/skillbase.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"

MojieManager::MojieManager() : m_role(NULL)
{
	m_mojie_param.Reset();
}

MojieManager::~MojieManager()
{
}

void MojieManager::Init(Role *role, const MojieParam &mojie_param)
{
	m_role = role;
	m_mojie_param = mojie_param;
}

void MojieManager::GetInitParam(MojieParam *mojie_param)
{
	*mojie_param = m_mojie_param;
}

void MojieManager::Update(time_t now_second)
{
	if (!m_role->IsAlive())
	{
// 		unsigned short mojie_skill_id = m_mojie_param.mojie_list[MOJIE_TYPE_4].mojie_skill_id;
// 
// 		const SkillBase *mojie_skill_4 = SKILLPOOL->GetSkill(mojie_skill_id);
// 		if (NULL == mojie_skill_4) return;
// 
// 		unsigned short mojie_skill_level = m_mojie_param.mojie_list[MOJIE_TYPE_4].mojie_skill_level;
// 		unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 		if (now_sec > m_mojie_param.last_colddown_time)
// 		{
// 			mojie_skill_4->Perform(m_role, mojie_skill_level, m_role, m_role->GetPos());
// 			m_mojie_param.last_colddown_time =  now_sec + static_cast<unsigned int>(mojie_skill_4->GetCoolDown(mojie_skill_level));
// 		}
	}
	else
	{
		// 技能72 自身生命值低于param_a%时，无敌param_b秒，冷却120秒。
		unsigned short mojie_skill_id = m_mojie_param.mojie_list[MOJIE_TYPE_3].mojie_skill_id;

		const SkillBase *mojie_skill_3 = SKILLPOOL->GetSkill(mojie_skill_id);
		if (NULL == mojie_skill_3) return;

		unsigned short mojie_skill_level = m_mojie_param.mojie_list[MOJIE_TYPE_3].mojie_skill_level;
		unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		// 检查是否达到触发血量
		long long curr_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		long long max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		int hp_per = static_cast<int>(max_hp > 0 ? curr_hp * 10000 / max_hp : 0);	//血量万分比
		if (now_sec > m_mojie_param.mojie_list[MOJIE_TYPE_3].last_colddown_time && hp_per < mojie_skill_3->GetParamA(mojie_skill_level))
		{
			mojie_skill_3->Perform(m_role, mojie_skill_level, m_role, m_role->GetPos());
			m_mojie_param.mojie_list[MOJIE_TYPE_3].last_colddown_time = now_sec + static_cast<unsigned int>(mojie_skill_3->GetCoolDown(mojie_skill_level));
		}
	}
	
}

void MojieManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int mojie_type = 0; mojie_type < MojieParam::MOJIE_MAX_TYPE; mojie_type++)
		{
			const MojieUplevelCfg *level_cfg = LOGIC_CONFIG->GetMoJieConfig().GetMojieLevelCfg(mojie_type, m_mojie_param.mojie_list[mojie_type].mojie_level);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += level_cfg->per_pofang;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI] += level_cfg->per_baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += level_cfg->per_baoji_hurt;
//				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += level_cfg->pvp_jianshang_per;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += level_cfg->per_mianshang;

				// 下面特殊属性用乘法
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, level_cfg->per_mianshang);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, level_cfg->per_pofang);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, level_cfg->pvp_jianshang_per);
			}
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_MOJIE, m_attrs_add);
	base_add.Add(m_attrs_add.m_attrs, true);
}

void MojieManager::SendMojieInfo()
{
	static Protocol::SCMojieInfo pro;

	for(int i = 0; i < MojieParam::MOJIE_MAX_TYPE; i++)
	{
		pro.mojie_list[i].mojie_skill_type = m_mojie_param.mojie_list[i].mojie_skill_type;
		pro.mojie_list[i].mojie_level = m_mojie_param.mojie_list[i].mojie_level;
		pro.mojie_list[i].mojie_skill_id = m_mojie_param.mojie_list[i].mojie_skill_id;
		pro.mojie_list[i].mojie_skill_level = m_mojie_param.mojie_list[i].mojie_skill_level;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

bool MojieManager::OnUpMojielevel(Role *role, int mojie_type, bool is_auto_buy)
{
	if (NULL == role) return false;

	if (mojie_type <= INVALID_MOJIE_TYPE || mojie_type >= MOJIE_TYPE_MAX) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	int next_level = m_mojie_param.mojie_list[mojie_type].mojie_level + 1; 
	const MojieUplevelCfg *next_level_cfg = LOGIC_CONFIG->GetMoJieConfig().GetMojieLevelCfg(mojie_type, next_level);
	if (NULL == next_level_cfg)
	{
		role->NoticeNum(errornum::EN_MOJIE_LEVEL_LIMIT);
		return false;
	}

	int curr_level = m_mojie_param.mojie_list[mojie_type].mojie_level; 
	const MojieUplevelCfg *curr_level_cfg = LOGIC_CONFIG->GetMoJieConfig().GetMojieLevelCfg(mojie_type, curr_level);
	if (NULL == curr_level_cfg)
	{
		role->NoticeNum(errornum::EN_MOJIE_LEVEL_LIMIT);
		return false;
	}

	if (role->GetLevel() < curr_level_cfg->up_level_limit)
	{
		role->NoticeNum(errornum::EN_MOJIE_ROLE_LEVEL_LIMIT);
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		// 进阶符
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = curr_level_cfg->up_level_stuff_id;
		stuff_list[0].num = curr_level_cfg->up_level_stuff_num;
		stuff_list[0].buyable = is_auto_buy;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "MojieManager::OnUpMojielevel"))
		{
			return false;
		}
	}

	{
		// 生效

		m_mojie_param.mojie_list[mojie_type].mojie_level++;

		if (0 != next_level_cfg->has_skill)
		{
			m_mojie_param.mojie_list[mojie_type].mojie_skill_id = next_level_cfg->skill_id;
			m_mojie_param.mojie_list[mojie_type].mojie_skill_level = next_level_cfg->skill_level;

			if (0 == m_mojie_param.is_first_active_mojie && MOJIE_TYPE_1 == mojie_type && NULL != SKILLPOOL->GetSkill(next_level_cfg->skill_id) && 0 < next_level_cfg->skill_level)
			{
				m_mojie_param.is_first_active_mojie = 1;
				m_role->GetSkillManager()->AddMojieSkill(next_level_cfg->skill_id, next_level_cfg->skill_level, mojie_type);
			}
		}
		else
		{
			m_mojie_param.mojie_list[mojie_type].mojie_skill_id = 0;
			m_mojie_param.mojie_list[mojie_type].mojie_skill_level = 0;
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOJIE);

		if (1 == m_mojie_param.mojie_list[mojie_type].mojie_level)
		{
			role->NoticeNum(noticenum::NT_MOJIE_ACTIVE_SUCC);
		}
		else
		{
			role->NoticeNum(noticenum::NT_MOJIE_UP_LEVEL_SUCC);
		}

		if (0 != next_level_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mojie_uplevel, 
				UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), mojie_type, curr_level, next_level);
			if (sendlen > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	this->SendMojieInfo();

	EventHandler::Instance().OnActiveMoJie(role, curr_level_cfg->up_level_stuff_id);

	return true;
}

void MojieManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag)
{
	// 技能71 攻击时有param_a概率把主目标变成怪物（要有内置CD）
	if (NULL == target || base_injure <= 0)
	{
		return;
	}

	unsigned short mojie_skill_id = m_mojie_param.mojie_list[MOJIE_TYPE_2].mojie_skill_id;

	const SkillBase *mojie_skill_2 = SKILLPOOL->GetSkill(mojie_skill_id);
	if (NULL == mojie_skill_2) return;

	unsigned short mojie_skill_level = m_mojie_param.mojie_list[MOJIE_TYPE_2].mojie_skill_level;
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (0 == target_index && now_sec > m_mojie_param.mojie_list[MOJIE_TYPE_2].last_colddown_time)
	{
		if (RandomNum(10000) < mojie_skill_2->GetParamA(mojie_skill_level))
		{
			mojie_skill_2->Perform(m_role, mojie_skill_level, target, target->GetPos());
			m_mojie_param.mojie_list[MOJIE_TYPE_2].last_colddown_time = now_sec + static_cast<unsigned int>(mojie_skill_2->GetCoolDown(mojie_skill_level));
		}
	}
}

bool MojieManager::IsMianyiDeadlyAttack()
{
	// 技能73 受到致死一击时，拦截死亡，并且恢复param_a%最大生命值。

	unsigned short mojie_skill_id = m_mojie_param.mojie_list[MOJIE_TYPE_4].mojie_skill_id;

	const SkillBase *mojie_skill_4 = SKILLPOOL->GetSkill(mojie_skill_id);
	if (NULL == mojie_skill_4) return false;

	unsigned short mojie_skill_level = m_mojie_param.mojie_list[MOJIE_TYPE_4].mojie_skill_level;
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec > m_mojie_param.mojie_list[MOJIE_TYPE_4].last_colddown_time)
	{
		//if (RandomNum(10000) < mojie_skill_4->GetParamA(mojie_skill_level))
		{
			mojie_skill_4->Perform(m_role, mojie_skill_level, m_role, m_role->GetPos());
			m_mojie_param.mojie_list[MOJIE_TYPE_4].last_colddown_time = now_sec + static_cast<unsigned int>(mojie_skill_4->GetCoolDown(mojie_skill_level));

			// 返回true，拦截这次致命的伤害

			return true;
		}
	}

	return false;
}
