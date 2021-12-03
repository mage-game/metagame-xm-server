#include "rolebossmanager.h"
#include <algorithm>
#include "obj/character/monster.h"
#include "world.h"
#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"
#include "scene/scene.h"
#include "global/rank/personrank.hpp"
#include "global/rank/rankmanager.hpp"
#include "obj/character/roleshadow.hpp"
#include "global/challengefield/challengefield.hpp"
#include "global/challengefield/challengeuser.hpp"
#include "obj/character/role.h"
#include "monster/drop/droppool.hpp"
#include "ai/monsterai/monsteraibase.hpp"
#include "servercommon/string/gameworldstr.h"
#include "scene/worldshadow/worldshadow.hpp"

RoleBossManager & RoleBossManager::Instance()
{
	static RoleBossManager _instance;
	return _instance;
}

RoleBossManager::RoleBossManager()
{

}

RoleBossManager::~RoleBossManager()
{

}

void RoleBossManager::OnRoleLogin(Role *role)
{
// 	if (NULL != role)
// 	{
// 		this->SendInfo(role);
// 	}
}

void RoleBossManager::Update(unsigned long interval, time_t now_second)
{
// 	if (0 == m_next_refresh_timestamp)
// 	{
// 		m_next_refresh_timestamp = LOGIC_CONFIG->GetRoleBossCfg().CalNextRefreshTimestamp();
// 	}
// 
// 	if (0 != m_next_refresh_timestamp && now_second >= m_next_refresh_timestamp)
// 	{
// 		m_next_refresh_timestamp = LOGIC_CONFIG->GetRoleBossCfg().CalNextRefreshTimestamp();
// 
// 		PersonRank *person_rank = RankManager::Instance().GetPersonRank();
// 		if (NULL == person_rank)
// 		{
// 			return;
// 		}
// 
// 		PersonRankItem rank_item_list[ROLE_BOSS_MAX_COUNT];
// 		int rank_item_count = person_rank->GetRankTopItem(PERSON_RANK_TYPE_CAPABILITY_ALL, ROLE_BOSS_MAX_COUNT, rank_item_list);
// 
// 		int all_name_len = 0;
// 		char all_name_str[512] = {0};
// 
// 		for (int i = 0; i < ROLE_BOSS_MAX_COUNT; ++ i)
// 		{
// 			RoleShadowInfo &boss_info = m_role_shadow_info_list[i];
// 
// 			{
// 				// 如果还存活，干掉
// 				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_info.scene_id, 0);
// 				if (NULL != scene)
// 				{
// 					Obj *obj = scene->GetObj(boss_info.obj_id);
// 					if (NULL != obj && Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
// 					{
// 						RoleShadow *role_shadow = (RoleShadow *)obj;
// 
// 						if (UidToInt(role_shadow->GetUserId()) == boss_info.role_id)
// 						{
// 							scene->DeleteObj(obj->GetId());
// 						}
// 					}
// 				}
// 			}
// 
// 			boss_info.Reset();
// 
// 			{
// 				ChallengeUser *challenge_user = NULL;
// 				{
// 					if (i < rank_item_count)
// 					{
// 						challenge_user = ChallengeField::Instance().GetChallengeUser(rank_item_list[i].user_id);
// 					}
// 					
// 					if (NULL == challenge_user)
// 					{
// 						challenge_user = ChallengeField::Instance().GetChallengeUserByPos(i);
// 					}
// 				}
// 
// 				if (NULL != challenge_user)
// 				{
// 					const RoleBossRefreshCfg *cfg_item = LOGIC_CONFIG->GetRoleBossCfg().GetRefreshCfg(i);
// 					if (NULL != cfg_item)
// 					{
// 						Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(cfg_item->scene_id, 0);
// 						if (NULL != scene)
// 						{
// 							RoleShadow *role_shadow = new RoleShadow(ROLE_SHADOW_TYPE_ROLE_BOSS);
// 							ChallengeParam param = challenge_user->GetChallengeParam();
// 
// 							int attr_level = cfg_item->attr_level;
// 
// 							param.gongji = static_cast<int>(1965.3525 * attr_level - 107339.25) ;
// 							param.max_hp = static_cast<int>((5 * pow((double)attr_level, (double)4) + 10000 * attr_level + 100000) * (-0.00125 * pow((double)(attr_level - 80), (double)2) + 1.5));
// 							param.fangyu = static_cast<int>(1.2 * param.gongji + 3600);
// 							param.mingzhong = static_cast<int>(0.65 * param.gongji + 1950);
// 							param.shanbi = 1;
// 							param.baoji = static_cast<int>(param.gongji * 0.75 + 2250);
// 							param.jianren = static_cast<int>(param.gongji * 1.725 + 5175);
// 
// 							for (int i = 0; i < MAX_XIANJIAN_SOUL_SKILL_SLOT_COUNT; ++ i)
// 							{
// 								// param.xianjian_info.soul_skill_list[i].skill_id = -1;
// 							}
// 
// 							for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++ i)
// 							{
// 								// param.skill_list[i].level = 1;
// 							}
// 
// 							role_shadow->Init(param, 0, 0, UidToInt(INVALID_USER_ID));
// 							role_shadow->SetPos(cfg_item->pos);
// 							role_shadow->SetBirthPos(cfg_item->pos);
// 							scene->AddObj(role_shadow);
// 
// 							boss_info.scene_id = scene->GetSceneID();
// 							boss_info.pos_x = role_shadow->GetPos().x;
// 							boss_info.pos_y = role_shadow->GetPos().y;
// 							boss_info.obj_id = role_shadow->GetId();
// 							boss_info.role_id = UidToInt(role_shadow->GetUserId());
// 
// 							{
// 								// 名字拼接
// 								char member_name_str[64] = {0};
// 
// 								int namelen = SNPRINTF(member_name_str, sizeof(member_name_str), "{r;%d;%s;%d} ", UidToInt(challenge_user->GetUserID()), challenge_user->GetName(), (int)challenge_user->GetCamp());
// 								if (namelen > 0 && all_name_len + namelen < (int)sizeof(all_name_str))
// 								{
// 									STRNCPY(all_name_str + all_name_len, member_name_str, sizeof(all_name_str) - all_name_len);
// 									all_name_len += namelen;
// 								}
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 
// 		{
// 			// 传闻
// 			if (all_name_len > 0)
// 			{
// 				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_role_shadow_boss_refresh_notice, all_name_str);
// 				if (len > 0) 
// 				{
// 					// World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_AND_ROLL);
// 				}
// 			}
// 		}
// 
// 		this->SendInfo(NULL, Protocol::SCRoleShadowBossInfo::NOTIFY_REASON_REFRESH);
// 	}
}

bool SortFunc(const HurtRoleShadowInfo &item1, const HurtRoleShadowInfo &item2)
{
	if (item1.hurt != item2.hurt)
	{
		return item1.hurt > item2.hurt;
	}

	return item1.uid > item2.uid;
}

void RoleBossManager::OnKillRoleShadow(Role *role, RoleShadow *role_shadow)
{
// 	if (NULL == role || NULL == role_shadow) return;
// 
// 	Scene *scene = role_shadow->GetScene();
// 	if (NULL == scene)
// 	{
// 		return;
// 	}
// 
// 	std::set<int> filter_uid;
// 
// 	int role_boss_idx = -1;
// 	for (int i = 0; i < ROLE_BOSS_MAX_COUNT; ++ i)
// 	{
// 		if (INVALID_OBJ_ID != m_role_shadow_info_list[i].obj_id)
// 		{
// 			if (m_role_shadow_info_list[i].scene_id == scene->GetSceneID() &&
// 				role_shadow->GetId() == m_role_shadow_info_list[i].obj_id)
// 			{
// 				role_boss_idx = i;
// 				break;
// 			}
// 		}
// 	}
// 	if (role_boss_idx < 0 || role_boss_idx >= ROLE_BOSS_MAX_COUNT)
// 	{
// 		return;
// 	}
// 
// 	const RoleBossRefreshCfg *cfg_item = LOGIC_CONFIG->GetRoleBossCfg().GetRefreshCfg(role_boss_idx);
// 	if (NULL == cfg_item)
// 	{
// 		return;
// 	}
// 
// 	for (std::vector<UInt16>::const_iterator it = cfg_item->kill_dropid_set.begin(); it != cfg_item->kill_dropid_set.end(); ++ it)
// 	{
// 		DROPPOOL->DropHelper(*it, scene, role_shadow->GetPos(), role->GetUserId(), 0, true, false);
// 	}
// 	filter_uid.insert(role->GetUID());
// 
// 	int top_hurt_uid = 0;
// 	if (role_shadow->GetTopHurtUid(&top_hurt_uid, filter_uid))
// 	{
// 		for (std::vector<UInt16>::const_iterator it = cfg_item->top_hurt_dropid_set.begin(); it != cfg_item->top_hurt_dropid_set.end(); ++ it)
// 		{
// 			DROPPOOL->DropHelper(*it, scene, role_shadow->GetPos(), IntToUid(top_hurt_uid), 0, true, false);
// 		}
// 
// 		filter_uid.insert(top_hurt_uid);
// 	}
// 
// 	int rand_reward_uid_list[BOSS_RAND_REWARD_RAND_COUNT];
// 	int rand_reward_uid_count = role_shadow->RandGetHurtUid(BOSS_RAND_REWARD_RAND_COUNT, rand_reward_uid_list, filter_uid);
// 	
// 	for (int i = 0; i < rand_reward_uid_count; ++ i)
// 	{
// 		for (std::vector<UInt16>::const_iterator it = cfg_item->join_dropid_set.begin(); it != cfg_item->join_dropid_set.end(); ++ it)
// 		{
// 			DROPPOOL->DropHelper(*it, scene, role_shadow->GetPos(), IntToUid(rand_reward_uid_list[i]), 0, true, false);
// 		}
// 	}
// 
// 	{
// 		std::vector<HurtRoleShadowInfo> hurt_vector;
// 		role_shadow->GetHurtInfo(hurt_vector);
// 
// 		std::sort(hurt_vector.begin(), hurt_vector.end(), SortFunc);
// 
// 		int rank = 0;
// 		for (std::vector<HurtRoleShadowInfo>::iterator it = hurt_vector.begin(); it != hurt_vector.end(); ++ it)
// 		{
// 			Role *hurt_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(it->uid));
// 			if (NULL != hurt_role && hurt_role->GetScene()->GetSceneID() == role_shadow->GetScene()->GetSceneID())
// 			{
// 				double exp_factor = LOGIC_CONFIG->GetRoleBossCfg().GetExpFactor(rank);
// 				long long reward_exp = static_cast<long long>((918356.25 * cfg_item->exp_level- 53816050 + 666666) * exp_factor);
// 				if (reward_exp < 666666)
// 				{
// 					reward_exp = 666666;
// 				}
// 
// 				if (reward_exp > 0)
// 				{
// 					hurt_role->AddExp(reward_exp, "RoleShadowBoss");
// 				}
// 			}
// 
// 			++ rank;
// 		}
// 	}
// 
// 	m_role_shadow_info_list[role_boss_idx].obj_id = INVALID_OBJ_ID;
// 	this->SendInfo(NULL);
// 
// 	{
// 		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_role_shadow_boss_kill_notice, 
// 			role->GetUID(), role->GetName(), role->GetCamp());
// 		if (len > 0) 
// 		{
// 			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_AND_ROLL);
// 		}
// 	}
}

void RoleBossManager::SendInfo(Role *role, int notify_reason)
{
// 	static Protocol::SCRoleShadowBossInfo rsbi;
// 	rsbi.notify_reason = notify_reason;
// 	rsbi.next_refresh_timestamp = static_cast<unsigned int>(m_next_refresh_timestamp);
// 	
// 	for (int i = 0; i < ROLE_BOSS_MAX_COUNT; ++ i)
// 	{
// 		rsbi.item_list[i].scene_id = m_role_shadow_info_list[i].scene_id;
// 		rsbi.item_list[i].pos_x = m_role_shadow_info_list[i].pos_x;
// 		rsbi.item_list[i].pos_y = m_role_shadow_info_list[i].pos_y;
// 		rsbi.item_list[i].obj_id  = m_role_shadow_info_list[i].obj_id;
// 		rsbi.item_list[i].role_id = m_role_shadow_info_list[i].role_id;
// 
// 		ChallengeUser *top_user = ChallengeField::Instance().GetChallengeUser(IntToUid(m_role_shadow_info_list[i].role_id));
// 		if (NULL != top_user)
// 		{
// 			Protocol::SCChallengeFieldOpponentInfo::OpponentInfo &role_info = rsbi.item_list[i].role_info;
// 			const ChallengeParam &opponent_param = top_user->GetChallengeParam();
// 			role_info.user_id = UidToInt(top_user->GetUserID());
// 			top_user->GetName(role_info.name);
// 			role_info.prof = opponent_param.prof;
// 			role_info.sex = opponent_param.sex;
// 			role_info.camp = opponent_param.camp;
// 			role_info.wing_special_imgid = opponent_param.appearance_param.wing_special_imgid;
// 			role_info.wuqi_id = opponent_param.appearance_param.wuqi_id;
// 			role_info.wing_jinhua_img = opponent_param.appearance_param.wing_jinhua_img;
// 			role_info.capability = opponent_param.capability;
// 			role_info.shizhuang_guanghuan = opponent_param.appearance_param.shizhuang_guanghuan;;
// 			role_info.shizhuang_wuqi = opponent_param.appearance_param.shizhuang_wuqi;
// 			role_info.shizhuang_body = opponent_param.appearance_param.shizhuang_body;
// 			role_info.shizhuang_foot = opponent_param.appearance_param.shizhuang_foot;
// 			role_info.wing_use_grade = opponent_param.appearance_param.wing_use_grade;
// 		}
// 	}
// 
// 	if (NULL != role)
// 	{
// 		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rsbi, sizeof(rsbi));
// 	}
// 	else
// 	{
// 		World::GetInstWorld()->SendToAllGateway((const char *)&rsbi, sizeof(rsbi));
// 	}
}



