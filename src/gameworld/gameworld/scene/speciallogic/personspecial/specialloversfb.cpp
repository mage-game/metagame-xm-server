#include "specialloversfb.hpp"
#include "servercommon/qingyuandef.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"
#include "other/event/eventhandler.hpp"
#include "world.h"
#include "gamelog.h"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/errornum.h"
#include "gameworld/effect/skilleffect/effectbuff.hpp"
#include "obj/otherobj/fallingitem.h"
#include "item/knapsack.h"
#include "friend/teaminfo.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "gameworld/config/monsterreplaceconfig.hpp"

SpecialLoversFB::SpecialLoversFB(Scene *scene) : SpecialLogic(scene), m_is_finish(false), m_is_pass(false), m_is_buff(false), m_curr_wave(0), m_wave_reward_mark(0), 
m_max_wave_count(0), m_buy_buff_times(0), m_buff_failure_timestamp(0), m_male_is_buy_double_reward(false), m_female_is_buy_double_reward(0), m_next_refresh_monster_time(0), 
m_kick_out_timestamp(0), m_per_wave_remain_time(0), m_total_get_uplevel_stuffs(0), m_total_get_exps(0),m_refresh_monster_role_level(0)
{
	memset(m_add_qingyuan_value_list, 0, sizeof(m_add_qingyuan_value_list));
	m_reward_item_record_vec_men.clear();
	m_reward_item_record_vec_woman.clear();

	for (int i = 0; i < QINGYUAN_FB_TEAM_MEMBER_COUNT; ++ i)
	{
		m_joiner_objid_list[i] = INVALID_OBJ_ID;
	}
}

SpecialLoversFB::~SpecialLoversFB()
{

}

void SpecialLoversFB::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (m_is_finish)
	{
		this->DelayKickOutRole(role);
		return;
	}

	bool is_exist = false;
	int free_slot = -1, locate_index = -1;

	if (role->GetTeamInfo()->IsLeader())
	{
		m_open_scene_userid = role->GetUserId();
	}

	for (int i = 0; i < QINGYUAN_FB_TEAM_MEMBER_COUNT; ++ i)
	{
		if (role->GetUserId() == m_joiner_userid_list[i])
		{
			is_exist = true;
			locate_index = i;

			break;
		}

		if (free_slot < 0 && INVALID_USER_ID == m_joiner_userid_list[i])
		{
			free_slot = i;
		}
	}

	if (!is_exist)
	{
		if (free_slot >= 0 && free_slot < QINGYUAN_FB_TEAM_MEMBER_COUNT)
		{
			m_joiner_userid_list[free_slot] = role->GetUserId();
			m_joiner_objid_list[free_slot] = role->GetId();
			m_add_qingyuan_value_list[free_slot] = 0;
		}
	}
	else
	{
		if (locate_index >= 0 && locate_index < QINGYUAN_FB_TEAM_MEMBER_COUNT)
		{
			m_joiner_objid_list[locate_index] = role->GetId();
		}
	}

	if (0 == m_max_wave_count)
	{
		UNSTD_STATIC_CHECK(2 == QINGYUAN_FB_TEAM_MEMBER_COUNT);

		Role *role_0 = this->GetOwnerRole(m_joiner_objid_list[0]);
		Role *role_1 = this->GetOwnerRole(m_joiner_objid_list[1]);

		if (NULL != role_0 && NULL != role_1)
		{
			const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
			m_max_wave_count = LOGIC_CONFIG->GetQingyuanCfg().GetMaxWaveCount();
			m_next_refresh_monster_time = EngineAdapter::Instance().Time() + other_cfg.next_wave_time_interval;
			m_per_wave_remain_time = m_next_refresh_monster_time + other_cfg.per_wave_max_time_s;
			m_refresh_monster_role_level = (role_0->GetLevel() + role_1->GetLevel()) / 2;
		}
	}

	this->SendInfo();

	EventHandler::Instance().OnEnterQingYuanFb(role);
}

void SpecialLoversFB::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_buff && m_buff_failure_timestamp < static_cast<int>(EngineAdapter::Instance().Time()))
	{
		m_is_buff = false;
		m_buff_failure_timestamp = 0;
		this->SendInfo();
	}

	if ((int)now_second >= m_per_wave_remain_time)
	{
		this->FBFinish(false);
	}

	if (!m_is_finish)
	{
		if (0 != m_next_refresh_monster_time && (int)now_second >= m_next_refresh_monster_time)
		{
			const QingyuanFBMonsterWavesConfig  &monster_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWaveCfg();
			const QingyuanFBMonsterWavesConfig::ConfigItem &cfg_item = monster_cfg.cfg_list[m_curr_wave];

			const QingyuanFBMonsterPosConfig &pos_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetPosCfg();

			for (int i = 0; i < pos_cfg.cfg_count && i < QingyuanFBMonsterPosConfig::CONFIG_ITEM_MAX_COUNT; ++ i)
			{
				if (0 != cfg_item.monster_id)
				{
					int monster_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByRoleLevel(REMONSTER_ENV_LOVERS_FB, m_refresh_monster_role_level, cfg_item.monster_id);
					MONSTERPOOL->CreateMonster(static_cast<UInt16>(monster_id), m_scene, pos_cfg.cfg_list[i]);
				}
			}

			if (0 != cfg_item.boss_id)
			{
				if (m_max_wave_count == m_curr_wave)
				{
					const QingyuanFBBossPos &boss_pos_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetFBBossPosCfg();
					for (int k = 0; k < QingyuanFBBossPos::MAX_ITEM_COUNT && k < cfg_item.boss_num; ++k)
					{
						int boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByRoleLevel(REMONSTER_ENV_LOVERS_FB, m_refresh_monster_role_level, cfg_item.boss_id);
						MONSTERPOOL->CreateMonster(static_cast<UInt16>(boss_id), m_scene, boss_pos_cfg.pos_list[k]);
					}
				}
				else
				{
					for (int n = 0; n < cfg_item.boss_num; ++n)
					{
						int boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByRoleLevel(REMONSTER_ENV_LOVERS_FB, m_refresh_monster_role_level, cfg_item.boss_id);
						MONSTERPOOL->CreateMonster(static_cast<UInt16>(boss_id), m_scene, pos_cfg.cfg_list[n]);
					}	
				}
			}

			++ m_curr_wave;
			m_next_refresh_monster_time = 0;

			this->SendInfo();
		}

		this->CheckSceneMonsterStatus(interval, now_second);
	}
	else
	{
		const QingyuanOtherConfig  &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
		Posi pos;
		pos.x = other_cfg.birth_pos_x;
		pos.y = other_cfg.birth_pos_y;

		Role *role_0 = this->GetOwnerRole(m_joiner_objid_list[0]);
		Role *role_1 = this->GetOwnerRole(m_joiner_objid_list[1]);

		if (NULL != role_0 && !role_0->IsAlive())
		{
			role_0->ReAlive(100, 100, pos);
		}

		if (NULL != role_1 && !role_1->IsAlive())
		{
			role_1->ReAlive(100, 100, pos);
		}

		if (0 != m_kick_out_timestamp && now_second >= m_kick_out_timestamp)
		{
			this->AllRoleReLive();
			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
	}
}

void SpecialLoversFB::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		for (int i = 0; i < QINGYUAN_FB_TEAM_MEMBER_COUNT; ++ i)
		{
			Role *role = this->GetOwnerRole(m_joiner_objid_list[i]);
			if (NULL == role)
			{
				continue;
			}

			m_add_qingyuan_value_list[i] += REWARD_QINGYUAN_VALUE_PER_MONSTER;
			role->GetQingyuan()->AddQingyuanValue(REWARD_QINGYUAN_VALUE_PER_MONSTER);
		}
	}
}

Role* SpecialLoversFB::GetOwnerRole(ObjID role_obj)
{
	Obj *obj = m_scene->GetObj(role_obj);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		return role;
	}

	return NULL;
}

bool SpecialLoversFB::OnTimeout()
{
	if (!m_is_finish)
	{
		this->FBFinish(false);
	}

	return m_is_pass;
}

void SpecialLoversFB::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL != role)
	{
		for (int i = 0; i < QINGYUAN_FB_TEAM_MEMBER_COUNT; ++ i)
		{
			if (m_joiner_userid_list[i] == role->GetUserId())
			{
				m_joiner_objid_list[i] = INVALID_OBJ_ID;
			}
		}
	}

	if (!m_is_finish)
	{
		this->SendFailRewardInfo(role);
	}
}

bool SpecialLoversFB::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (NULL == obj) return false;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return false;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

void SpecialLoversFB::OnBuyBuff(Role *role)
{
	if (m_buff_failure_timestamp > static_cast<int>(EngineAdapter::Instance().Time()))
	{
		return;
	}

	if (NULL == role)
	{
		return;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const QingyuanFBBuffConfig &buff_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetFBBuffCfg();

	int need_gold = buff_cfg.cost_gold;
	if (m_buy_buff_times > 0)
	{
		double need_gold_double = need_gold * (1 + m_buy_buff_times * buff_cfg.add_gold_per * ROLE_ATTR_PER);
		need_gold = (int)ceil(need_gold_double);
	}

	if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(need_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (role->GetKnapsack()->GetMoney()->UseGold(need_gold, "qingyuan_fb_buff"))
	{
		++ m_buy_buff_times;
		m_is_buff = true;
		m_buff_failure_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + buff_cfg.continue_time_s;

		//增加攻击A%，持续B秒
		int dur_ms = buff_cfg.continue_time_s * 1000;
		EffectBuff *effect_buff = new EffectBuff(role->GetId(), 1, dur_ms, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);

		effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		int delta_gongji = (int)(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * buff_cfg.buff_gongjing_per * ROLE_ATTR_PER);
		effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, delta_gongji, buff_cfg.buff_gongjing_per);
		effect_buff->SetClientEffectType(EffectBase::ECT_QY_FB_GONGJI);

		role->AddEffect(effect_buff);

		Role *marry_target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetLoverUid()));
		if (NULL != marry_target_role)
		{
			//增加伴侣攻击A%，持续B秒
			EffectBuff *effect_buff = new EffectBuff(marry_target_role->GetId(), 1, dur_ms, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);

			effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, delta_gongji, buff_cfg.buff_gongjing_per);
			effect_buff->SetClientEffectType(EffectBase::ECT_QY_FB_GONGJI);

			marry_target_role->AddEffect(effect_buff);
		}
	}

	this->SendInfo();
}

void SpecialLoversFB::OnBuyDoubleReward(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	bool is_already_buy = (FEMALE == role->GetSex() ? m_female_is_buy_double_reward : m_male_is_buy_double_reward);

	if (is_already_buy)
	{
		return;
	}

	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();

	if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.lover_fb_double_reward_need_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	bool is_succ = role->GetKnapsack()->GetMoney()->UseGold(other_cfg.lover_fb_double_reward_need_gold, "SpecialLoversFB::OnBuyDoubleReward");
	if (!is_succ)
	{
		return;
	}

	if (FEMALE == role->GetSex())
	{
		m_female_is_buy_double_reward = true;

		if (m_is_finish)
		{
			for (auto item : m_reward_item_record_vec_woman)
			{
				role->GetKnapsack()->PutOrMail(item, PUT_REASON_QINGYUAN_FB_REWARD);
			}
		}
	}
	else
	{
		m_male_is_buy_double_reward = true;

		if (m_is_finish)
		{
			for (auto item : m_reward_item_record_vec_men)
			{
				role->GetKnapsack()->PutOrMail(item, PUT_REASON_QINGYUAN_FB_REWARD);
			}
		}
	}

	this->SendInfo();
}

void SpecialLoversFB::OnAllMonsterDead()
{
	const QingyuanFBRewardConfig &fb_reward = LOGIC_CONFIG->GetQingyuanCfg().GetFBRewardCfg();

	bool is_get_reward = false;
	int reward_index = -1;

	if (m_curr_wave != m_wave_reward_mark)
	{
		for (int i = 0; i < fb_reward.cfg_count && i < QINGYUAN_FB_MAX_REWARD_COUNT; ++i)
		{
			if (fb_reward.cfg_list[i].wave_num == m_curr_wave)
			{
				is_get_reward = true;
				reward_index = i;
				m_wave_reward_mark = m_curr_wave;
				break;
			}
		}

		if (is_get_reward && reward_index >= 0 && reward_index < fb_reward.cfg_count)
		{
			ItemConfigData stuff;
			stuff.item_id = fb_reward.cfg_list[reward_index].stuff_id;
			stuff.num = fb_reward.cfg_list[reward_index].stuff_num;
			stuff.is_bind = true;

			for (int i = 0; i < QINGYUAN_FB_TEAM_MEMBER_COUNT; ++ i)
			{
				Role *role = this->GetOwnerRole(m_joiner_objid_list[i]);
				if (NULL == role)
				{
					continue;
				}

				bool is_already_buy = (FEMALE == role->GetSex() ? m_female_is_buy_double_reward : m_male_is_buy_double_reward);
				if (is_already_buy)
				{
					stuff.num = fb_reward.cfg_list[reward_index].stuff_num * 2;
				}
				else
				{
					stuff.num = fb_reward.cfg_list[reward_index].stuff_num;
				}

				// 奖励记录，结算面板用
				this->AddRewardRecord(&stuff, role->GetSex());

				m_total_get_exps = fb_reward.cfg_list[reward_index].exp_reward;
				m_total_get_uplevel_stuffs = fb_reward.cfg_list[reward_index].stuff_num;

				role->AddExp(fb_reward.cfg_list[reward_index].exp_reward, "qingyuan_fb", Role::EXP_ADD_REASON_DEFAULT);
				role->GetKnapsack()->PutOrMail(stuff, PUT_REASON_QINGYUAN_FB_REWARD);
			}
		}
	}

	if (m_curr_wave < m_max_wave_count)
	{
		if (0 == m_next_refresh_monster_time)
		{
			const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
			m_next_refresh_monster_time = EngineAdapter::Instance().Time() + other_cfg.next_wave_time_interval;
			m_per_wave_remain_time = m_next_refresh_monster_time + other_cfg.per_wave_max_time_s;
		}

		this->SendInfo();
	}
	else
	{
		m_is_pass = true;

		this->FBFinish(true);
	}
}

void SpecialLoversFB::AddRewardRecord(ItemConfigData *reward_item, char sex)
{
	if (NULL == reward_item)
	{
		return;
	}

	if (MALE == sex)
	{
		bool is_exist = false;
		for (int i = 0; i < static_cast<int>(m_reward_item_record_vec_men.size()) && i < Protocol::SCQingyuanFBRewardRecordInfo::MAX_ITEM_COUNT; ++i)
		{
			if (reward_item->item_id == m_reward_item_record_vec_men[i].item_id)
			{
				m_reward_item_record_vec_men[i].num += reward_item->num;
				is_exist = true;
				break;
			}
		}
		if (!is_exist)
		{
			m_reward_item_record_vec_men.push_back(*reward_item);
		}
	}
	else if (FEMALE == sex)
	{
		bool is_exist = false;
		for (int i = 0; i < static_cast<int>(m_reward_item_record_vec_woman.size()) && i < Protocol::SCQingyuanFBRewardRecordInfo::MAX_ITEM_COUNT; ++i)
		{
			if (reward_item->item_id == m_reward_item_record_vec_woman[i].item_id)
			{
				m_reward_item_record_vec_woman[i].num += reward_item->num;
				is_exist = true;
				break;
			}
		}
		if (!is_exist)
		{
			m_reward_item_record_vec_woman.push_back(*reward_item);
		}
	}
}

void SpecialLoversFB::SendRewardRecordInfo()
{	
	for (int i = 0; i < QINGYUAN_FB_TEAM_MEMBER_COUNT; ++ i)
	{
		Role *role = this->GetOwnerRole(m_joiner_objid_list[i]);
		if (NULL != role)
		{
			if (MALE == role->GetSex())
			{
				Protocol::SCQingyuanFBRewardRecordInfo pro;
				pro.item_count = 0;
				for (int i = 0; i < static_cast<int>(m_reward_item_record_vec_men.size()) && i < Protocol::SCQingyuanFBRewardRecordInfo::MAX_ITEM_COUNT; ++i)
				{
					Protocol::SCQingyuanFBRewardRecordInfo::ItemInfo &item_info = pro.item_list[i];
					item_info.reward_item_id = m_reward_item_record_vec_men[i].item_id;
					item_info.reward_item_num = m_reward_item_record_vec_men[i].num;
					pro.item_count++;
				}
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
			}
			else if (FEMALE == role->GetSex())
			{
				Protocol::SCQingyuanFBRewardRecordInfo pro;
				pro.item_count = 0;
				for (int i = 0; i < static_cast<int>(m_reward_item_record_vec_woman.size()) && i < Protocol::SCQingyuanFBRewardRecordInfo::MAX_ITEM_COUNT; ++i)
				{
					Protocol::SCQingyuanFBRewardRecordInfo::ItemInfo &item_info = pro.item_list[i];
					item_info.reward_item_id = m_reward_item_record_vec_woman[i].item_id;
					item_info.reward_item_num = m_reward_item_record_vec_woman[i].num;
					pro.item_count++;
				}
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
			}
		}
	}
}

void SpecialLoversFB::SendFailRewardInfo(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	Protocol::SCQingyuanFBRewardRecordInfo pro;
	pro.item_count = 0;
	memset(pro.item_list, 0, sizeof(pro.item_list));

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void SpecialLoversFB::SendInfo()
{
	Protocol::SCQingyuanFBInfo qyssi;
	qyssi.curr_wave = static_cast<char>(m_curr_wave);
	qyssi.max_wave_count = static_cast<char>(m_max_wave_count);
	qyssi.is_pass = m_is_pass;
	qyssi.is_finish = m_is_finish;
	qyssi.buy_buff_times = m_buy_buff_times;
	qyssi.buff_failure_timestamp = m_buff_failure_timestamp;
	qyssi.next_refresh_monster_time = (int)m_next_refresh_monster_time;
	qyssi.per_wave_remain_time = (int)m_per_wave_remain_time;
	qyssi.total_get_uplevel_stuffs = m_total_get_uplevel_stuffs;
	qyssi.total_get_exps = m_total_get_exps;
	qyssi.kick_out_timestamp = (int)m_kick_out_timestamp;
	qyssi.female_is_buy_double_reward = (m_female_is_buy_double_reward ? 1 : 0);
	qyssi.male_is_buy_double_reward = (m_male_is_buy_double_reward ? 1 : 0);

	for (int i = 0; i < QINGYUAN_FB_TEAM_MEMBER_COUNT; ++ i)
	{
		Role *role = this->GetOwnerRole(m_joiner_objid_list[i]);
		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&qyssi, sizeof(qyssi));
		}
	}
}

void SpecialLoversFB::FBFinish(bool is_pass)
{
	if (m_is_finish)
	{
		return;
	}

	m_is_finish = true;
	m_kick_out_timestamp = EngineAdapter::Instance().Time() + 10;

	this->SendInfo();
	this->SendRewardRecordInfo();

	{
		// 日志
		UNSTD_STATIC_CHECK(QINGYUAN_FB_TEAM_MEMBER_COUNT >= 2);

		int use_time = static_cast<int>(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());
		int kill_wave = m_curr_wave;

		gamelog::g_log_fb.printf(LL_INFO, "LoverFB::OnFinish uid[%d], other_uid[%d], use_time[%d], kill_wave[%d]", 
			UidToInt(m_joiner_userid_list[0]), UidToInt(m_joiner_userid_list[1]), use_time, kill_wave);
	}
}

void SpecialLoversFB::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL == role || NULL == falling_item || NULL == item_wrapper)
	{
		return;
	}

	ItemConfigData stuff;
	stuff.item_id = falling_item->GetItemId();
	stuff.num = 1;
	stuff.is_bind = true;

	this->AddRewardRecord(&stuff, role->GetSex());
}