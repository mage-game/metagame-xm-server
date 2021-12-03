#include "specialcrosspasture.hpp"
#include <algorithm>
#include "obj/character/role.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "internalcomm.h"
#include "gamelog.h"
#include "servercommon/serverconfig/crosspastureconfig.hpp"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "obj/character/attribute.hpp"
#include "effect/effectbase.hpp"
#include <limits>
#include "servercommon/string/crossstr.h"
#include "servercommon/servercommon.h"
#include "world.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "ai/monsterai/monsteraibase.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "skill/skillpool.hpp"
#include "skill/skillbase.hpp"
#include "servercommon/noticenum.h"
#include "other/cross/rolecross.h"
#include "item/knapsack.h"
#include "global/bossfamily/bossfamilymanager.hpp"


SpecialCrossPasture::SpecialCrossPasture(Scene *scene)
: SpecialLogic(scene), m_activity_open(false), m_activity_end_time(0), special_monster_pos(0,0)
{

}

SpecialCrossPasture::~SpecialCrossPasture()
{

}

void SpecialCrossPasture::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!CrossConfig::Instance().IsHiddenServer()) return;

	if (!m_activity_open) return;

	special_monster = CrossPastureConfig::Instance().GetSpecialMonsterId();

	this->FixSPMonsterNum(special_monster, now_second);

	int fence_cfg_count = CrossPastureConfig::Instance().GetFenceCfgCount();

	const CrossPastureMonsterInfoConfig *monster_info_cfg = nullptr;

	Role *role_on_scene = nullptr;
	int role_num = 0;

	if (now_second != m_last_check_animal_status_time) // 每秒遍历一下牧场所有动物
	{
		m_last_check_animal_status_time = (unsigned int)now_second;

		std::set<int> remove_monster_id_set;
		std::set<int> already_has_follow_animal_set;

		int monster_num = static_cast<int>(m_scene->MonsterNum());
		for (int i = 0; i < monster_num; i++)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (NULL == monster) continue;

			int get_score = m_monster_info_map[monster->GetMonsterId()].score;
			if (get_score <= 0) continue;

			// 检查是否被引进栅栏里
			const Posi &monster_pos = monster->GetPos();
			for (int index = 0; index < fence_cfg_count; ++index)
			{
				const CrossPastureFenceConfig *fence_cfg = CrossPastureConfig::Instance().GetFenceCfg(index);
				if (NULL != fence_cfg && monster_pos.x >= fence_cfg->x1 && monster_pos.x <= fence_cfg->x2 && monster_pos.y >= fence_cfg->y1 && monster_pos.y <= fence_cfg->y2)
				{
					bool get_reward = true;
					ObjID obj_id = monster->GetMonsterAI()->GetEnemy();
					CrossPasturePlayerInfo *player_info = this->GetPlayerInfo(obj_id);
					if (NULL != player_info)
					{
						monster_info_cfg = CrossPastureConfig::Instance().GetMonsterInfoCfgWithMonsterID(monster->GetMonsterId());
						if (monster_info_cfg != NULL)
						{
							//特殊monster不减次数、不限积分
							if (monster_info_cfg->is_control_refresh)
							{
								player_info->score += get_score;
								player_info->cur_cross_get_score += get_score;

								this->SendPlayerInfoToPlayer(obj_id);
							}
							//普通monster限制次数、限制积分
							else if (player_info->get_score_times < CrossPastureConfig::Instance().GetOtherCfg().day_max_get_score_times && monster_info_cfg->is_count)
							{
								player_info->score += get_score;
								player_info->cur_cross_get_score += get_score;

								++player_info->get_score_times;
								++player_info->cur_cross_get_score_times;

								this->SendPlayerInfoToPlayer(obj_id);
							}
							else
							{
								Role *player = this->GetRole(obj_id);
								if (NULL != player)
								{
									player->NoticeNum(errornum::EN_CROSS_PASTURE_GET_SCORE_TIMES_FULL);
									get_reward = false;
								}
							}
						}
					}

					Role *killer = NULL;
					if (NULL != player_info)
					{
						killer = this->GetRole(obj_id);
						if (killer != NULL)
						{
							monster->ForceSetDead(killer);
						}
					}

					const CrossPastureMonsterInfoConfig *monster_info_cfg_1 = CrossPastureConfig::Instance().GetMonsterInfoCfgWithMonsterID(monster->GetMonsterId());

					if (monster_info_cfg_1 != NULL)
					{
						if (!monster_info_cfg_1->is_control_refresh)
						{
							remove_monster_id_set.insert(monster->GetMonsterId());
						}

						else if (monster_info_cfg_1->is_control_refresh && m_pasture_next_refresh_specialmonster_timestamp == 0)	//控制怪物刷新，可配置
						{
							//m_pasture_next_refresh_specialmonster_timestamp = (unsigned int)(EngineAdapter::Instance().Time()) + CrossPastureConfig::Instance().GetOtherCfg().monster_create_interval_s;
							// 使用根据服务器人数进行动态刷新
							int interval_s = BossFamilyManager::Instance().GetBossSpecialFlushInterval(monster->GetMonsterId());
							if (interval_s <= 0)
							{
								// 如果动态刷新取不到配置，则取跨服牧场配置的刷新间隔
								interval_s = CrossPastureConfig::Instance().GetOtherCfg().monster_create_interval_s;
							}
							m_pasture_next_refresh_specialmonster_timestamp = EngineAdapter::Instance().Time() + interval_s;

							//special_monster = monster->GetMonsterId();

							// sp monster被捕获后通知场景内玩家刷新倒计时
							role_num = static_cast<int>(m_scene->RoleNum());
							for (int i = 0; i < role_num; i++)
							{
								Role *role_on_scene = m_scene->GetRoleByIndex(i);
								if (role_on_scene != NULL)
								{
									this->SendPlayerInfoToPlayer(role_on_scene);
								}
							}
						}
					}
					//特殊怪物捕获随机奖励
					monster_info_cfg = CrossPastureConfig::Instance().GetRandRewardWithMonsterID(monster->GetMonsterId());
					if (monster_info_cfg != NULL)
					{
						if (killer != NULL && get_reward)
						{
							killer->GetKnapsack()->PutOrMail(monster_info_cfg->reward_item, PUT_REASON_CROSS_PASTURE);
						}
					}
				}
				//	实时刷新特殊monster位置
				else if (monster->IsAlive())
				{
					monster_info_cfg = CrossPastureConfig::Instance().GetMonsterInfoCfgWithMonsterID(monster->GetMonsterId());

					if (monster_info_cfg != NULL)
					{
						if (monster_info_cfg->is_control_refresh)
						{
							special_monster_pos = monster->GetPos();

							role_num = static_cast<int>(m_scene->RoleNum());
							for (int i = 0; i < role_num; i++)
							{
								role_on_scene = m_scene->GetRoleByIndex(i);
								if (role_on_scene != NULL)
								{
									this->SendPlayerInfoToPlayer(role_on_scene);
								}
							}
						}
					}
				}
			}
			// 检查是否有多个怪物同时跟着一个玩家
			ObjID enemy_obj_id = monster->GetMonsterAI()->GetEnemy();
			if (already_has_follow_animal_set.find(enemy_obj_id) != already_has_follow_animal_set.end())
			{
				monster->GetMonsterAI()->ForceSetEnemy(INVALID_OBJ_ID);
			}

			already_has_follow_animal_set.insert(enemy_obj_id);
		}

		for (std::set<int>::iterator it = remove_monster_id_set.begin(); it != remove_monster_id_set.end(); ++it)
		{
			//this->FixMonsterNum(*it);
			// 使用根据服务器人数进行动态刷新
			CrossPastureMonster &info = m_monster_info_map[*it];
			int interval_s = BossFamilyManager::Instance().GetBossSpecialFlushInterval(*it);
			if (interval_s <= 0)
			{
				// 如果动态刷新取不到配置，则取跨服牧场配置的刷新间隔
				interval_s = CrossPastureConfig::Instance().GetOtherCfg().monster_create_interval_s;
			}
			info.next_refresh_time = (unsigned int)(EngineAdapter::Instance().Time()) + interval_s;
		}
		remove_monster_id_set.clear();

		// 遍历刷新
		for (auto & mon_info_it : m_monster_info_map)
		{
			const CrossPastureMonsterInfoConfig *monster_info_cfg_1 = CrossPastureConfig::Instance().GetMonsterInfoCfgWithMonsterID(mon_info_it.first);
			if (nullptr != monster_info_cfg_1 && !monster_info_cfg_1->is_control_refresh)
			{
				if (mon_info_it.second.next_refresh_time > 0 && (unsigned int)now_second >= mon_info_it.second.next_refresh_time)
				{
					this->FixMonsterNum(mon_info_it.first);
				}
			}
		}
	}

	if (now_second >= m_pasture_last_rank_timestamp + 3)
	{
		m_pasture_last_rank_timestamp = now_second;

		this->CheckPastureUserRank();
	}
}

void SpecialCrossPasture::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *add_role = (Role *)obj;
		UniqueUserID uuid = add_role->GetRoleCross()->GetCrossOriginalUniqueUserID();

		bool is_first_enter = false; // 第一次进入的玩家都免费送积分
		CrossPasturePlayerInfo *player_info = this->GetPlayerInfo(uuid);
		if (NULL == player_info) // 新来的玩家
		{
			is_first_enter = true;

			player_info = &m_user_info_cache[uuid];
			player_info->uuid = uuid;
			F_STRNCPY(player_info->name, add_role->GetName(), sizeof(player_info->name));

			const CrossPastureSkillConfig *skill_cfg_list = CrossPastureConfig::Instance().GetSkillCfgList();
			for (int index = 0; index < CROSS_PASTURE_SKILL_CFG_MAX_COUNT; ++ index)
			{
				player_info->skill_list[index].skill_id = skill_cfg_list[index].skill_id;
				player_info->skill_list[index].last_perform_time = 0;
			}
		}

		//在第一次进入场景时，清空上一次活动数据
		if (is_first_enter)
		{
			add_role->GetRoleCross()->GetRoleCrossParam().cross_pasture_score = 0;
			add_role->GetRoleCross()->GetRoleCrossParam().cross_pasture_today_get_score_times = 0;
		}

		// 积分是每次进来都要重新获取的
		player_info->score = add_role->GetRoleCross()->GetPastureScore();
		player_info->cur_cross_get_score = 0;
		if (is_first_enter)
		{
			player_info->score += PLAYER_FIRST_ENTER_ADD_SCORE;
			player_info->cur_cross_get_score += PLAYER_FIRST_ENTER_ADD_SCORE;
			add_role->NoticeNum(noticenum::NT_PASTURE_FIRST_ENTER_GIVE_SCORE);
		}

		player_info->get_score_times = add_role->GetRoleCross()->GetPastureGetScoreTimes();
		player_info->cur_cross_get_score_times = 0;

		Posi role_pos = CrossPastureConfig::Instance().GetOtherCfg().role_pos;
		int MAX_OFFSET = 5;
		int x_offset = RandomNum(-MAX_OFFSET, MAX_OFFSET);
		int y_offset = RandomNum(-MAX_OFFSET, MAX_OFFSET);
		role_pos.x += x_offset;
		role_pos.y += y_offset;
		add_role->SetPos(role_pos);

		this->SendPlayerInfoToPlayer(add_role);
	}
}

void SpecialCrossPasture::OnRemoveObj(Obj *obj)
{
	if (!m_activity_open)
	{
		return;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *die_role = (Role*)obj;
		UniqueUserID uuid = die_role->GetRoleCross()->GetCrossOriginalUniqueUserID();
		m_role_last_die_time_map[uuid] = (unsigned int)EngineAdapter::Instance().Time();
	}
	else if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *mon = (Monster *)obj;
		CrossPastureMonster &info = m_monster_info_map[mon->GetMonsterId()];
		if (info.cur_num > 0)
		{
			-- info.cur_num;
			const CrossPastureMonsterInfoConfig *monster_info_cfg_1 = CrossPastureConfig::Instance().GetMonsterInfoCfgWithMonsterID(mon->GetMonsterId());
			if (monster_info_cfg_1 != NULL)
			{
				if (!monster_info_cfg_1->is_control_refresh)
				{
					//this->FixMonsterNum(mon->GetMonsterId());
					// 使用根据服务器人数进行动态刷新
					if (info.next_refresh_time <= 0)
					{
						int interval_s = BossFamilyManager::Instance().GetBossSpecialFlushInterval(mon->GetMonsterId());
						if (interval_s <= 0)
						{
							// 如果动态刷新取不到配置，则取跨服牧场配置的刷新间隔
							interval_s = CrossPastureConfig::Instance().GetOtherCfg().monster_create_interval_s;
						}
						info.next_refresh_time = (unsigned int)(EngineAdapter::Instance().Time()) + interval_s;
					}
				}
			}
		}
	}
}

bool SpecialCrossPasture::SpecialRelive(Role *role, int realive_type)
{
	if (NULL == role) return true;

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return false;
	}

	UniqueUserID uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	unsigned int past_time = (unsigned int)EngineAdapter::Instance().Time() - m_role_last_die_time_map[uuid];
	if ((int)past_time < CrossPastureConfig::Instance().GetOtherCfg().player_relive_time_s)
	{
		return true;
	}

	const Posi enter_pos = CrossPastureConfig::Instance().GetOtherCfg().role_pos;
	return role->ReAlive(100, 100, enter_pos);
}

void SpecialCrossPasture::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	UniqueUserID uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	CrossPasturePlayerInfo *player_info = this->GetPlayerInfo(uuid);

	if (NULL != player_info)
	{
		this->SyncPlayerInfoToCross(role);
	}

	role->BackToOriginalServer();
}

bool SpecialCrossPasture::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (NULL == role || NULL == obj)
	{
		return false;
	}

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return false;
	}

	if (role->GetId() == obj->GetId())
	{
		return false;
	}

	return true;
}

long long SpecialCrossPasture::SpecialRecalcInjure(Character *attacker, Character *injurer, long long injure)
{
	if (NULL == attacker || NULL == injurer) return 0;

	// 人攻击怪物不掉血，只是吸引仇恨
	if (Obj::OBJ_TYPE_ROLE == attacker->GetObjType() && Obj::OBJ_TYPE_MONSTER == injurer->GetObjType())
	{
		return 1;
	}

	return injure;
}

bool SpecialCrossPasture::PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id)
{	
	UniqueUserID uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	CrossPasturePlayerInfo *player_info = this->GetPlayerInfo(uuid);
	if (NULL == player_info)
	{
		return false;
	}

	if (skill_index < 0 || skill_index >= CROSS_PASTURE_SKILL_CFG_MAX_COUNT)
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(player_info->skill_list[skill_index].skill_id);
	if (NULL == skill)
	{
		return false;
	}

	Character *target_character = NULL;
	Obj *target_obj = this->GetScene()->GetObj(target_id);
	if (NULL != target_obj)
	{
		if (Obj::OBJ_TYPE_ROLE != target_obj->GetObjType() && Obj::OBJ_TYPE_MONSTER != target_obj->GetObjType())
		{
			return false;
		}

		target_character = (Character *)target_obj;

		if ((target_obj->GetPos() - role->GetPos()).IsLongerThan(skill->GetAttackDis()))
		{
			role->NoticeNum(errornum::EN_CROSS_PASTURE_DISTANCE_NOT_ENOUGH);
			return false;
		}
	}

	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	if (now_time - player_info->skill_list[skill_index].last_perform_time < (unsigned int)skill->GetCoolDown(1))
	{
		role->NoticeNum(errornum::EN_CROSS_PASTURE_CD_NOT_ENOUGH);
		return false;
	}

	//skill->SendSkillTip(role);
	skill->Perform(role, 1, target_character, pos, 0);

	player_info->skill_list[skill_index].last_perform_time = now_time;

	return true;
}

int SpecialCrossPasture::GetSpecialParam(Role *role)
{
// 	if (NULL != role)
// 	{
// 		const CrossPasturePlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
// 		if (NULL != player_info)
// 		{
// 			return player_info->get_score_times;
// 		}
// 	}

	return 0;
}

void SpecialCrossPasture::OnActivityOpen()
{
	if (m_activity_open)
	{
		return;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}


	m_pasture_next_refresh_specialmonster_timestamp = 0;	//下一次刷新特殊monster时间戳

	{
		m_monster_info_map.clear();

		// 创建怪物
		int monster_count = CrossPastureConfig::Instance().GetMonsterInfoCfgCount();
		for (int index = monster_count - 1; index >= 0; -- index)
		{
			const CrossPastureMonsterInfoConfig *monster_info_cfg = CrossPastureConfig::Instance().GetMonsterInfoCfgWithIndex(index);
			if (NULL == monster_info_cfg) continue;

			CrossPastureMonster &info = m_monster_info_map[monster_info_cfg->monster_id];
			info.monster_id = monster_info_cfg->monster_id;
			info.score = monster_info_cfg->score;
			info.max_num = monster_info_cfg->max_num;
			info.cur_num = 0;
			info.next_refresh_time = 0;

			this->FixMonsterNum(monster_info_cfg->monster_id);
		}
	}

	m_pasture_last_rank_timestamp = (unsigned int)EngineAdapter::Instance().Time();
	m_activity_open = true;

	this->ClearRankTitle();		//清理上一次活动的称号
}

void SpecialCrossPasture::OnActivityClose()
{
	if (!m_activity_open)
	{
		return;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	{
		// 删除所有动物
		int monster_num = static_cast<int>(m_scene->MonsterNum());
		for (int i = 0; i < monster_num; i++)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (NULL != monster)
			{
				monster->ForceSetDead();
			}
		}

		m_monster_info_map.clear();
	}

	this->RankItemReward();

	{
		// 所有玩家信息同步回原服
		static crossgameprotocal::GameCrossPastureInfoSync pis;
		for (UserInfoMapIt it = m_user_info_cache.begin(); it != m_user_info_cache.end(); ++ it)
		{
			pis.uuid = UniqueUserIDToLongLong(it->first);

			pis.get_score = it->second.cur_cross_get_score;
			it->second.cur_cross_get_score = 0;

			pis.get_score_times = it->second.cur_cross_get_score_times;
			it->second.cur_cross_get_score_times = 0;

			//玩家排名
			int rank = 0;
			std::vector<CrossPastureRankItem>::iterator iter = m_pasture_rank_vec.begin();
			for (; iter != m_pasture_rank_vec.end(); ++iter)
			{
				++rank;
				if (it->first == iter->uuid)
				{
					pis.rank = rank;
				}
			}

			InternalComm::Instance().SendToCross((const char *)&pis, sizeof(pis));
		}

		m_user_info_cache.clear();
	}

	m_role_last_die_time_map.clear();

	this->DelayKickOutAllRole();

	m_activity_open = false;
}

void SpecialCrossPasture::SendPlayerInfoToPlayer(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	CrossPasturePlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL != player_info)
	{
		static Protocol::SCCrossPasturePlayerInfo ppi;
		ppi.score = player_info->score;
		ppi.left_get_score_times = CrossPastureConfig::Instance().GetOtherCfg().day_max_get_score_times - player_info->get_score_times;
		if (special_monster_pos.x > 0 && special_monster_pos.y > 0)
		{
			ppi.x = special_monster_pos.x;
			ppi.y = special_monster_pos.y;
		}
		else
		{
			ppi.x = 0;
			ppi.y = 0;
		}
		ppi.special_monster_refresh_time = static_cast<unsigned int>(m_pasture_next_refresh_specialmonster_timestamp);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&ppi, sizeof(ppi));
	}
}

void SpecialCrossPasture::SendPlayerInfoToPlayer(ObjID obj_id)
{
	Role *role = this->GetRole(obj_id);
	if (role)
	{
		this->SendPlayerInfoToPlayer(role);
	}
}

void SpecialCrossPasture::NotifySystemMsgToScene(char *str_buff, int str_len, int notice_type)
{
	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void SpecialCrossPasture::SyncPlayerInfoToCross(Role *role)
{
	if (NULL == role) return;

	UniqueUserID uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	CrossPasturePlayerInfo *p_info = this->GetPlayerInfo(uuid);
	if (NULL == p_info) return;

	static crossgameprotocal::GameCrossPastureInfoSync pis;

	pis.uuid = UniqueUserIDToLongLong(uuid);
	
	pis.get_score = p_info->cur_cross_get_score;
	p_info->cur_cross_get_score = 0;

	pis.get_score_times = p_info->cur_cross_get_score_times;
	p_info->cur_cross_get_score_times = 0;
	
	InternalComm::Instance().SendToCross((const char *)&pis, sizeof(pis));
}

Role *SpecialCrossPasture::GetRole(ObjID obj_id)
{
	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return  (Role *)obj;
	}

	return NULL;
}

CrossPasturePlayerInfo *SpecialCrossPasture::GetPlayerInfo(UniqueUserID uuid)
{
	UserInfoMapIt it = m_user_info_cache.find(uuid);
	if (it != m_user_info_cache.end())
	{
		return &it->second;
	}

	return NULL;
}

CrossPasturePlayerInfo *SpecialCrossPasture::GetPlayerInfo(ObjID obj_id)
{
	Role *role = this->GetRole(obj_id);
	if (NULL != role)
	{
		UniqueUserID uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
		return this->GetPlayerInfo(uuid);
	}

	return NULL;
}

void SpecialCrossPasture::FixMonsterNum(int monster_id)
{
	CrossPastureMonster &info = m_monster_info_map[monster_id];
	
	int create_num = info.cur_num;

	//判断是否特殊monster
	bool is_special_monster = false;
	const CrossPastureMonsterInfoConfig *monster_info_cfg_1 = CrossPastureConfig::Instance().GetMonsterInfoCfgWithMonsterID(monster_id);
	if (monster_info_cfg_1 != NULL)
	{
		if (monster_info_cfg_1->is_control_refresh)
		{
			is_special_monster = true;
		}	
	}

	while (create_num < info.max_num)
	{
		Posi monster_pos = CrossPastureConfig::Instance().GetRandomMonsterCreatePosByMonsterId(monster_id);
		
		// 坐标做个偏移，避免动物重叠
		int MAX_OFFSET = 2;
		int x_offset = RandomNum(-MAX_OFFSET, MAX_OFFSET);
		int y_offset = RandomNum(-MAX_OFFSET, MAX_OFFSET);
		monster_pos.x += x_offset;
		monster_pos.y += y_offset;

		Monster *monster = MONSTERPOOL->CreateMonster(info.monster_id, this->GetScene(), monster_pos);
		if (NULL != monster)
		{
			++info.cur_num;
		}
		++create_num;

		if (is_special_monster)
		{
			special_monster_pos = monster_pos;
		}
	}

	// 重置怪的刷新时间
	if (!is_special_monster && 0 != info.next_refresh_time)
	{
		info.next_refresh_time = 0;
	}
}

void SpecialCrossPasture::FixSPMonsterNum(int monster_id, time_t now_second)
{
	CrossPastureMonster &info = m_monster_info_map[monster_id];

	int create_num = info.cur_num;

	if (create_num < info.max_num)
	{
		Posi monster_pos = CrossPastureConfig::Instance().GetRandomMonsterCreatePosByMonsterId(monster_id);

		// 坐标做个偏移，避免动物重叠
		int MAX_OFFSET = 2;
		int x_offset = RandomNum(-MAX_OFFSET, MAX_OFFSET);
		int y_offset = RandomNum(-MAX_OFFSET, MAX_OFFSET);
		monster_pos.x += x_offset;
		monster_pos.y += y_offset;

		if (now_second > m_pasture_next_refresh_specialmonster_timestamp)
		{
			Monster *monster = MONSTERPOOL->CreateMonster(info.monster_id, this->GetScene(), monster_pos);
			if (NULL != monster)
			{
				++info.cur_num;
			}
			++create_num;

			special_monster_pos = monster_pos;

			if (create_num == info.max_num)
			{
				m_pasture_next_refresh_specialmonster_timestamp = 0;

				int role_num = static_cast<int>(m_scene->RoleNum());
				for (int i = 0; i < role_num; i++)
				{
					Role *role = m_scene->GetRoleByIndex(i);
					if (role != NULL)
					{
						this->SendPlayerInfoToPlayer(role);
					}
				}
			}
		}
	}

	// 特殊怪直接设0
	info.next_refresh_time = 0;
}


bool static SortPastureFunc(const CrossPastureRankItem &rank_item_1, const CrossPastureRankItem &rank_item_2)
{
	return rank_item_1.score > rank_item_2.score;
}

void SpecialCrossPasture::CheckPastureUserRank()
{
	m_pasture_rank_vec.clear();
	CrossPastureRankItem rank_item;
	for (UserInfoMapIt itr = m_user_info_cache.begin(); itr != m_user_info_cache.end(); ++itr)
	{
		rank_item.uuid = itr->second.uuid;
		rank_item.score = itr->second.score;
		F_STRNCPY(rank_item.user_name, itr->second.name, sizeof(GameName));
		m_pasture_rank_vec.push_back(rank_item);
	}
	std::sort(m_pasture_rank_vec.begin(), m_pasture_rank_vec.end(), &SortPastureFunc);

	this->SendPastureRankToUser();
}

void SpecialCrossPasture::SendPastureRankToUser(Role* role)
{
	static Protocol::SCCrossPastureRankInfo cpri;

	cpri.rank_count = 0;
	std::vector<CrossPastureRankItem>::iterator itr = m_pasture_rank_vec.begin();
	for (; itr!=m_pasture_rank_vec.end() && cpri.rank_count < CROSS_PASTURE_RANK_COUNT; ++itr)
	{
		cpri.rank_list[cpri.rank_count].uid = UidToInt(itr->uuid.user_id);
		cpri.rank_list[cpri.rank_count].score = itr->score;
		F_STRNCPY(cpri.rank_list[cpri.rank_count].name, itr->user_name, sizeof(GameName));

		++cpri.rank_count;
	}

	if (NULL == role)
	{
		int sendlen = sizeof(cpri) - (CROSS_PASTURE_RANK_COUNT - cpri.rank_count) * sizeof(cpri.rank_list[0]);
		m_scene->SendToRole((const char*)&cpri, sendlen);
	}
	else
	{
		int sendlen = sizeof(cpri) - (CROSS_PASTURE_RANK_COUNT - cpri.rank_count) * sizeof(cpri.rank_list[0]);
		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(cpri)))
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cpri, sendlen);
		}
	}
}

void SpecialCrossPasture::ClearRankTitle()
{
	// 清理上一次活动的排行榜及称号
	static crossgameprotocal::GameCrossPastureInfoSync pis;
	for (int i = 1; i <= CROSS_FISHING_TOP_RANK_COUNT_MAX; ++i)
	{
		pis.rank = i;
		pis.uuid = 0;
		pis.get_score = 0;
		pis.get_score_times = 0;
			
		InternalComm::Instance().SendToCross((const char *)&pis, sizeof(pis));
	}
}

void SpecialCrossPasture::RankItemReward()
{
	int rank = 0;
	std::vector<CrossPastureRankItem>::iterator iter = m_pasture_rank_vec.begin();
	for (; iter != m_pasture_rank_vec.end(); ++iter)
	{
		++rank;
		const CrossPastureRankReward * reward_item = CrossPastureConfig::Instance().GetRankRewardByRank(rank);
		if (NULL == reward_item)
		{
			break;
		}

		static MailContentParam mail_param; mail_param.Reset();
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			mail_param.AddItem(reward_item->reward_item[i]);
		}

		int length = gstr::MailContent(mail_param.contenttxt, sizeof(mail_param.contenttxt), "cross_pasture_rank_reward", rank);
		if (length > 0)
		{
			MailRoute::MailToCrossUser(iter->uuid, SYSTEM_MAIL_REASON_INVALID, mail_param);
		}
	}
}