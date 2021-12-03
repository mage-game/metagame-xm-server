#include "specialnationalboss.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "scene/activityshadow/activityshadow.hpp"
#include "item/itempool.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "engineadapter.h"
#include "gamelog.h"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"
#include "config/activityconfig/zhuxieconfig.hpp"
#include "config/activityconfig/nationalbossconfig.hpp"

#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/scenemanager.h"
#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include <algorithm> 

#include "global/activity/activitymanager.hpp"
#include "item/knapsack.h"

bool static SortFunc(const NationalBossRankItem &rank_item_1, const NationalBossRankItem &rank_item_2)
{
	return rank_item_1.hurt > rank_item_2.hurt;
}

SpecialNationalBoss::SpecialNationalBoss(Scene *scene) : 
	SpecialLogic(scene), m_is_finish(false), m_kickout_user_timestamp(0), m_next_flush_time(0), m_flush_times(0), m_total_bosshp(0), m_world_level(0),
	m_next_refresh_dps_rank_time(0)
{
	
}

SpecialNationalBoss::~SpecialNationalBoss()
{

}

void SpecialNationalBoss::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish) 
	{
		if (0 != m_kickout_user_timestamp && now_second >= m_kickout_user_timestamp)
		{
			m_kickout_user_timestamp = 0;
			
			this->DelayKickOutAllRole();
		}

		return;
	}

	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_NATIONAL_BOSS)) return;

	if (m_next_flush_time > 0 && now_second >= m_next_flush_time)
	{
		m_next_flush_time = 0;

		this->RefreshAll();
	}

	if (m_next_refresh_dps_rank_time > 0 && now_second >= m_next_refresh_dps_rank_time)
	{
		m_next_refresh_dps_rank_time = EngineAdapter::Instance().Time() + 3;

		this->RefreshDPSRank();
	}

	this->CheckSceneMonsterStatus(interval, now_second);
}

void SpecialNationalBoss::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (m_is_finish) return;

	if (NULL == dead) return;

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;

		Obj *atk_obj = m_scene->GetObj(killer_id);
		if (NULL != atk_obj && Obj::OBJ_TYPE_ROLE == atk_obj->GetObjType())
		{
			Role *attacker_role = (Role *)atk_obj;

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_kill_national_boss,
							attacker_role->GetUID(), attacker_role->GetName(), attacker_role->GetCamp(), monster->GetMonsterId());

			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
		}
	}
}

void SpecialNationalBoss::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (m_is_finish) return;

	if (NULL == cha) return;

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Obj *atk_obj = m_scene->GetObj(enemy_id);
		if (NULL != atk_obj && Obj::OBJ_TYPE_ROLE == atk_obj->GetObjType())
		{
			Role *attacker_role = (Role *)atk_obj;

			NBUser *user_info = this->GetUser(attacker_role->GetUID());
			if (NULL != user_info)
			{
				user_info->hurt += (-delta_hp);
			}
		}
	}
}

void SpecialNationalBoss::OnRoleEnterScene(Role *role)
{
	if (m_is_finish) return;

	if (NULL == role)
	{
		return;
	}

	int add_gongji_percent = 0;

	NBUser *user_info = this->GetUser(role->GetUID());
	if (NULL != user_info)
	{
		user_info->obj_id = role->GetId();
		user_info->level = role->GetLevel();
		add_gongji_percent = user_info->add_gongji_percent;
	}
	else
	{
		NBUser user;
		user.obj_id = role->GetId();
		user.level = role->GetLevel();
		user.uid = role->GetUID();
		user.camp = role->GetCamp();
		role->GetName(user.user_name);
		user.hurt = 0;
		user.add_gongji_percent = 0;
		user.rank_pos = 0;
		user.buy_times = 0;

		m_user_map[user.uid] = user;
	}

	if (add_gongji_percent > 0)
	{
		this->RecalcBuff(role, add_gongji_percent);
	}

	this->SendUserInfo(role);
	this->SendRankInfo(role);

	EventHandler::Instance().OnEnterNationalBoss(role);
}

void SpecialNationalBoss::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (m_is_finish) return;

	if (NULL == role)
	{
		return;
	}

	NBUser *user_info = this->GetUser(role->GetUID());
	if (NULL == user_info)
	{
		return;
	}

	user_info->obj_id = INVALID_OBJ_ID;
}

bool SpecialNationalBoss::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			return false;
		}
		break;
	}

	return true;
}

bool SpecialNationalBoss::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			return true;
		}
		break;
	}

	return false;
}

void SpecialNationalBoss::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case ACTIVITY_STATUS_OPEN:
		{
			this->OnActivityStart();
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			this->OnActivityClose();
		}
		break;
	}
}

void SpecialNationalBoss::SendUserInfo(Role *role)
{
	if (NULL == role) return;

	NBUser *nb_user = this->GetUser(role->GetUID());
	if (NULL != nb_user)
	{
		Protocol::SCNationalBossUserInfo nbui;
		nbui.total_hurt = nb_user->hurt;
		nbui.buy_times = nb_user->buy_times;
		nbui.add_gongji_percent = nb_user->add_gongji_percent;
		nbui.rank_pos = nb_user->rank_pos;
		nbui.total_bosshp = m_total_bosshp;
		nbui.boss_flush_times = static_cast<char>(m_flush_times);
		nbui.is_finish = m_is_finish ? 1 : 0;
		nbui.next_flush_timestamp = static_cast<unsigned int>(m_next_flush_time);
		nbui.world_level = m_world_level;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&nbui, sizeof(nbui));
	}
}

void SpecialNationalBoss::SendRankInfo(Role *role)
{
	if (NULL == role)
	{
		m_nbri.rank_count = 0;

		for (int i = 0; i < (int)m_dps_rank_vec.size() && i < Protocol::SCNationalBossRankInfo::RANK_ITEM_MAX_COUNT; i ++)
		{
			Protocol::SCNationalBossRankInfo::RankItem &rank_item = m_nbri.rank_list[i];

			rank_item.uid = m_dps_rank_vec[i].uid;
			rank_item.total_hurt= m_dps_rank_vec[i].hurt;
			rank_item.camp = m_dps_rank_vec[i].camp;
			F_STRNCPY(rank_item.name, m_dps_rank_vec[i].user_name, sizeof(GameName));
			++ m_nbri.rank_count;
		}

		int sendlen = sizeof(m_nbri) - (Protocol::SCNationalBossRankInfo::RANK_ITEM_MAX_COUNT - m_nbri.rank_count) * sizeof(m_nbri.rank_list[0]);
		m_scene->SendToRole((const char*)&m_nbri, sendlen);
	}
	else
	{
		int sendlen = sizeof(m_nbri) - (Protocol::SCNationalBossRankInfo::RANK_ITEM_MAX_COUNT - m_nbri.rank_count) * sizeof(m_nbri.rank_list[0]);
		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(m_nbri)))
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_nbri, sendlen);
		}
	}
}

void SpecialNationalBoss::OnActivityStart()
{
	m_is_finish = false;
	m_kickout_user_timestamp = 0;
	m_next_flush_time = EngineAdapter::Instance().Time();
	m_flush_times = 0;
	m_total_bosshp = 0;
	m_next_refresh_dps_rank_time = EngineAdapter::Instance().Time();
	m_dps_rank_vec.clear();
	m_user_map.clear();
	m_nbri.total_bosshp = 0;
	m_nbri.rank_count = 0;	
	m_world_level = WorldShadow::Instance().GetWorldLevel();
}

void SpecialNationalBoss::OnActivityClose()
{
	m_next_flush_time = 0;

	this->OnFinish();
}

void SpecialNationalBoss::NotifySystemMsg(char *str_buff, int str_len, int msg_type)
{
	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, msg_type))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void SpecialNationalBoss::RefreshAll()
{
	if (m_is_finish) return;

	++ m_flush_times;

	const NBRefreshItem * refresh_cfg = LOGIC_CONFIG->GetNationalBossConfig().GetRefreshList(m_world_level);
	if (NULL == refresh_cfg)
	{
		return;
	}

	for (int i = 0; i < NATIONALBOSS_FRESH_BOSS_COUNT; ++i)
	{
		MONSTERPOOL->CreateMonster(refresh_cfg->boss_id_list[i], m_scene, refresh_cfg->born_pos_list[i]);
	}

	if (m_total_bosshp <= 0)
	{
		UInt32 monster_num = m_scene->MonsterNum();
		for (int i = 0; i < (int)monster_num; ++ i)
		{
			Monster * monster = m_scene->GetMonsterByIndex(i);
			if (NULL != monster)
			{
				m_total_bosshp += (long long)monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			}
		}

		m_total_bosshp *= MONSTER_FLUSH_MAX_TIMES;
	}
}

void SpecialNationalBoss::OnAllMonsterDead()
{
	if (m_flush_times < MONSTER_FLUSH_MAX_TIMES)
	{
		if (0 == m_next_flush_time)
		{
			m_next_flush_time = EngineAdapter::Instance().Time() + 10;

			this->RefreshDPSRank();
		}
	}
	else
	{
		this->OnFinish();
	}
}

void SpecialNationalBoss::RefreshDPSRank()
{
	m_dps_rank_vec.clear();
	m_dps_rank_vec.reserve(m_user_map.size());

	for (std::map<int, NBUser>::iterator it = m_user_map.begin(); it != m_user_map.end(); ++it)
	{
		NationalBossRankItem rank_item;
		rank_item.uid = it->first;
		rank_item.hurt = it->second.hurt;
		rank_item.camp = it->second.camp;
		F_STRNCPY(rank_item.user_name, it->second.user_name, sizeof(GameName));

		m_dps_rank_vec.push_back(rank_item);
	}

	std::sort(m_dps_rank_vec.begin(), m_dps_rank_vec.end(), &SortFunc);

	for (std::vector<NationalBossRankItem>::iterator it = m_dps_rank_vec.begin(); it != m_dps_rank_vec.end(); ++it)
	{
		NBUser *dps_user = this->GetUser(it->uid);
		if (NULL != dps_user)
		{
			dps_user->rank_pos = (int)(std::distance(m_dps_rank_vec.begin(), it)) + 1;
		}
	}

	this->SendRankInfo(); 

	// 同时更新用户信息
	if (NULL != m_scene)
	{
		for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				this->SendUserInfo(role);
			}
		}
	}
}

void SpecialNationalBoss::OnFinish()
{
	if (m_is_finish)
	{
		return;
	}

	m_is_finish = true;
	m_kickout_user_timestamp = EngineAdapter::Instance().Time() + 25;

	this->RefreshDPSRank();

	// 排行奖励
	int rank = 0;
	for (std::vector<NationalBossRankItem>::iterator it = m_dps_rank_vec.begin(); it != m_dps_rank_vec.end(); ++it)
	{
		rank += 1;

		NationalBossRankItem &rank_item = *it;

		if (rank <= 10)
		{
			gamelog::g_log_activity.printf(LL_INFO, "NationalBossFinish rank:%d user:%d %s hurt:%lld",
				rank, rank_item.uid, rank_item.user_name, rank_item.hurt);
		}

		ItemConfigData *reward_item = LOGIC_CONFIG->GetNationalBossConfig().GetRankReward(rank);
		if (NULL != reward_item)
		{
			static MailContentParam contentparam; contentparam.Reset();
			const ItemBase *itembase = ITEMPOOL->GetItem(reward_item->item_id);
			if (NULL != itembase) 
			{
				contentparam.item_list[0].item_id = reward_item->item_id;
				contentparam.item_list[0].is_bind = reward_item->is_bind ? 1 : 0;
				contentparam.item_list[0].num = reward_item->num;
				contentparam.item_list[0].invalid_time = itembase->CalInvalidTime();

				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_nationalboss_reward_subject);
				int length2 = 0;
					
				if (rank <= 10)
				{
					length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_nationalboss_reward_content_top10, rank);
				}
				else
				{
					length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_nationalboss_reward_content_other);
				}

				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(IntToUid(rank_item.uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}

	// 任务奖励
	if (m_total_bosshp > 0)
	{
		for (std::map<int, NBUser>::iterator it = m_user_map.begin(); it != m_user_map.end(); ++it)
		{
			NBUser &nb_user = it->second;
			int hurt_percent = static_cast<int>(nb_user.hurt * 10000.f / m_total_bosshp);

			const NBTaskRewardConfig::ConfigItem *task_reward = LOGIC_CONFIG->GetNationalBossConfig().GetTaskReward(nb_user.level, hurt_percent);
			if (NULL != task_reward)
			{
				static MailContentParam contentparam; contentparam.Reset();

				long long reward_exp = task_reward->reward_exp;
				reward_exp += WorldShadow::Instance().GetWorLeveldExtraExp(nb_user.level, reward_exp);

				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_EXP, reward_exp);
				contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_BIND_COIN, task_reward->reward_coin);
				
				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_nationalboss_task_reward_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_nationalboss_task_reward_content, nb_user.hurt, task_reward->reward_level);

				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(IntToUid(nb_user.uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			{
				// 日志
				gamelog::g_log_roleactivity.printf(LL_INFO, "SpecialNationalBoss::JoinRecord user[%d, %s], level[%d], boss_hurt_percent[%d], hurt[%lld]",
					nb_user.uid, nb_user.user_name, nb_user.level, hurt_percent, nb_user.hurt);
			}
		}
	}

	// 清掉所有怪物
	UInt32 monster_num = m_scene->MonsterNum();
	for (UInt32 i = 0; i < monster_num; i++)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster && Obj::OBJ_TYPE_MONSTER == monster->GetObjType())
		{
			monster->ForceSetDead();
		}
	}

	{
		int rank_count = 0;
		int all_name_len = 0;
		char all_name_str[256] = {0};

		for (std::vector<NationalBossRankItem>::iterator it = m_dps_rank_vec.begin(); it != m_dps_rank_vec.end() && rank_count < 3; ++ it)
		{
			NationalBossRankItem &rank_item = *it;
			char member_name_str[64] = {0};

			int namelen = SNPRINTF(member_name_str, sizeof(member_name_str), "{r;%d;%s;%d} ", rank_item.uid, rank_item.user_name, rank_item.camp);
			if (namelen > 0 && all_name_len + namelen < (int)sizeof(all_name_str))
			{
				STRNCPY(all_name_str + all_name_len, member_name_str, sizeof(all_name_str) - all_name_len);
				all_name_len += namelen;
			}

			++ rank_count;
		}

		all_name_str[sizeof(all_name_str) - 1] = '\0';

		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_activity_national_boss_end, rank_count, all_name_str);
		if (len > 0) 
		{
			//this->NotifySystemMsg(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL_2);
		}
	}

	if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_NATIONAL_BOSS))
	{
		ActivityManager::Instance().OnForceToNextState(ACTIVITY_TYPE_NATIONAL_BOSS);
	}
}

void SpecialNationalBoss::OnBuyBuff(Role *role)
{
	if (m_is_finish) return;

	if (NULL == role) return;

	NBUser *user_info = this->GetUser(role->GetUID());
	if (NULL == user_info)
	{
		return;
	}

	if (user_info->buy_times >= LOGIC_CONFIG->GetNationalBossConfig().GetMaxBuyTimes())
	{
		role->NoticeNum(errornum::EN_NATIONALBOSS_BUY_TIMES_UP);
		return;
	}

	int need_gold = LOGIC_CONFIG->GetNationalBossConfig().GetBuffGold();
	int buff_percent = LOGIC_CONFIG->GetNationalBossConfig().GetBuyGongjiPercent();

	if (!role->GetKnapsack()->GetMoney()->UseGold(need_gold, "NationalBossBuyGongji"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	user_info->buy_times += 1;
	user_info->add_gongji_percent += buff_percent;

	this->RecalcBuff(role, user_info->add_gongji_percent);

	this->SendUserInfo(role);

	gamelog::g_log_roleactivity.printf(LL_INFO, "SpecialNationalBoss BuyBuff, user[%d, %s], use_gold[%d], buy_times[%d], add_percent[%d]", 
		role->GetUID(), role->GetName(), need_gold, (int)user_info->buy_times, user_info->add_gongji_percent);
}

SpecialNationalBoss::NBUser * SpecialNationalBoss::GetUser(int uid)
{
	std::map<int, NBUser>::iterator iter = m_user_map.find(uid);
	if (iter != m_user_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

Role * SpecialNationalBoss::GetRole(NBUser *user)
{
	if (NULL != user && INVALID_OBJ_ID != user->obj_id)
	{
		Obj *obj = m_scene->GetObj(user->obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role *)obj;
			if (role->GetUID() == user->uid)
			{
				return role;
			}
		}
	}

	return NULL;
}

void SpecialNationalBoss::RecalcBuff(Role *role, int add_percent)
{
	if (NULL == role || add_percent <= 0)
	{
		return;
	}

	int skill_id = GetSkillIDBySceneTypeAndIndex(Scene::SCENE_TYPE_NATIONAL_BOSS, 0);

	int add_gongji = (int)(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * add_percent / 100.0f);

	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, skill_id);

	EffectBuff *buff = new EffectBuff(INVALID_OBJ_ID, skill_id, 3600 * 1000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
	buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, add_gongji, add_percent);
	buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	buff->SetSave(false);

	role->AddEffect(buff);
}

