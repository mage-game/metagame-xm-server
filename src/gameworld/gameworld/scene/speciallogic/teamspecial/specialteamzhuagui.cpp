#include "specialteamzhuagui.hpp"

#include "engineadapter.h"
#include "obj/character/monster.h"
#include "obj/character/role.h"

#include "config/logicconfigmanager.hpp"
#include "item/itempool.h"
#include "config/activityconfig/zhuaguiconfig.hpp"
#include "monster/monsterpool.h"

#include "protocal/msgrole.h"
#include "servercommon/commondata.hpp"
#include "friend/teaminfo.hpp"
#include "global/team/team.hpp"
#include "monster/drop/droppool.hpp"
#include "obj/otherobj/fallingitem.h"

#include "scene/speciallogic/teamspecial/zhuaguimanager.hpp"

SpecialTeamZhuaGui::SpecialTeamZhuaGui(Scene *scene)
	: SpecialLogic(scene), m_first(true), m_boss_isflush(false), m_boss_monster_id(0), m_is_has_couple(false), m_team_member_count(0), m_team_average_level(0), m_kick_time(0)
{
	m_zgfi.Reset();
}

SpecialTeamZhuaGui::~SpecialTeamZhuaGui()
{

}

void SpecialTeamZhuaGui::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_first && 0 != m_team_member_count)
	{
		m_first = false;
		this->NotiryZhuaGuiAddPerInfo();

		{
			for (int i = 0; i < ZHUAGUI_MONSTER_FLUSH_COUNT; i++)
			{
				ZhuaGuiMonsterFlushCfg *flush_cfg = LOGIC_CONFIG->GetZhuaGuiConfig().GetFlushMonsterInfoByIdx(i);
				if (NULL != flush_cfg)
				{
					if (flush_cfg->IsBoss() && m_boss_isflush)
					{
						continue;
					}

					if (m_team_average_level < flush_cfg->team_level_min || m_team_average_level > flush_cfg->team_level_max)
					{
						continue;
					}

					int real_flush_prob = flush_cfg->flush_prob;

					if (flush_cfg->IsBoss())
					{
						if (m_is_has_couple)
						{
							real_flush_prob += LOGIC_CONFIG->GetZhuaGuiConfig().GetCoupleBossAddPer();
						}
						
						if (m_team_member_count > 0)
						{
							const ZhuaGuiTeamAdd * team_add_cfg = LOGIC_CONFIG->GetZhuaGuiConfig().GetTeamAddPer(m_team_member_count);
							real_flush_prob += team_add_cfg->add_boss_per;
						}
					}
					
					int rand_value = RandomNum(100);
					if (rand_value < real_flush_prob)
					{
						for (int j = 0; j < flush_cfg->flush_num; j++)
						{
							Posi rand_pos = gamecommon::GetDisRandPos(flush_cfg->flush_pos, flush_cfg->flush_range);
							if (!m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, rand_pos.x, rand_pos.y))
							{
								rand_pos = flush_cfg->flush_pos;
							}

							if (NULL != MONSTERPOOL->CreateMonster(flush_cfg->monster_id, m_scene, rand_pos, (long long)0, 0))
							{
								if (flush_cfg->IsBoss())
								{
									m_zgfi.ishave_boss = 1;
									m_zgfi.boss_isdead = 0;
									
									m_boss_isflush = true;
									m_boss_monster_id = flush_cfg->monster_id;

									break;
								}
								else 
								{
									++ m_zgfi.monster_count;
								}
							}
						}
					}
				}
			}
		}

		this->NotifyZhuaGuiInfo(Protocol::ZHUAGUI_NOTIFY_REASON_ENTER, NULL);
	}

	this->CheckSceneMonsterStatus(interval, now_second);

	if (m_kick_time != 0 && static_cast<unsigned int>(now_second) > m_kick_time)
	{
		this->DelayKickOutAllRole();
	}
}

void SpecialTeamZhuaGui::OnRoleEnterScene(Role *role)
{
	if (NULL == role) return;

	m_zgfi.AddHunli(role->GetUID(), 0, 0, role->GetCommonDataParam()->zhuagui_kill_boss_count);
	this->NotifyZhuaGuiInfo(Protocol::ZHUAGUI_NOTIFY_REASON_ENTER, role);
	this->AddMember(role);

	if (0 == m_team_member_count)
	{
		TeamInfo *team_info = role->GetTeamInfo();
		if (NULL != team_info)
		{
			Team *team = team_info->GetMyTeam();
			if (NULL != team)
			{
				if (team->IsLeader(role->GetUserId()))
				{
					m_team_member_count = team->GetMemberNum();
					m_team_average_level = team->GetTeamAverageLevel();
					
					if (team->IsHasCouple())
					{
						m_is_has_couple = true;
					}
				}
			}
		}
	}

	this->NotiryZhuaGuiAddPerInfo(role);
}

void SpecialTeamZhuaGui::OnRoleLeaveScene(Role *role)
{

}

void SpecialTeamZhuaGui::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster*)dead;
		
		int give_hunli = LOGIC_CONFIG->GetZhuaGuiConfig().GetHunliByMonsterID(monster->GetMonsterId());

		for (int i = 0; i < (int)m_scene->RoleNum(); i++)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				CommonDataParam *common_p = role->GetCommonDataParam();

				++ common_p->zhuagui_day_catch_count;

				if (common_p->zhuagui_kill_boss_count >= LOGIC_CONFIG->GetZhuaGuiConfig().GetMojingRewardTime())
				{
					if (LOGIC_CONFIG->GetZhuaGuiConfig().IsBoss(monster->GetMonsterId()))
					{
						common_p->zhuagui_kill_boss_count++;

						int mojing_reward = LOGIC_CONFIG->GetZhuaGuiConfig().GetMojingReward();
						role->AddHunli(mojing_reward, "ZhuaGuiReward");
						m_zgfi.AddHunli(role->GetUID(), 0, mojing_reward, 1);
					}
				}
				else
				{
					int role_give_hunli = LOGIC_CONFIG->GetZhuaGuiConfig().GetHunliFix(common_p->zhuagui_day_catch_count, give_hunli, m_team_member_count, m_is_has_couple);
					common_p->zhuagui_day_gethunli += role_give_hunli;

					role->AddHunli(role_give_hunli, "ZhuaGui");

					int boss_drop_limit = LOGIC_CONFIG->GetZhuaGuiConfig().GetBossDropLimit();
					if (LOGIC_CONFIG->GetZhuaGuiConfig().IsBoss(monster->GetMonsterId()))
					{
						common_p->zhuagui_kill_boss_count++;

						if (common_p->zhuagui_get_drop_time < boss_drop_limit)
						{
							ZhuaGuiMonsterFlushCfg * cfg = LOGIC_CONFIG->GetZhuaGuiConfig().GetFlushMonsterInfoByMonsterId(monster->GetMonsterId());
							if (NULL != cfg)
							{
								int drop_index = 0;
								for (std::vector<UInt16>::const_iterator it = cfg->drop_id_list.begin(); it != cfg->drop_id_list.end(); it++)
								{
									DROPPOOL->DropHelper(*it, role->GetScene(), monster->GetPos(), role->GetUserId(), role->GetProf(), monster->GetMonsterId(), false, false, &drop_index);
								}
							}

							++ common_p->zhuagui_get_drop_time;
						}


						// ·òÆÞ¶îÍâµôÂä
						if (m_is_has_couple)
						{
							TeamInfo *team_info = role->GetTeamInfo();
							if (NULL != team_info)
							{
								Team *team = team_info->GetMyTeam();
								if (NULL != team)
								{
									Team::TeamMember *lover_teammember = team->GetTeamMember(IntToUid(role->GetCommonDataParam()->lover_uid));
									if (NULL != lover_teammember)
									{
										int boss_couple_drop_limit = LOGIC_CONFIG->GetZhuaGuiConfig().GetBossCoupleDropLimit();
										if (common_p->zhuagui_get_couple_drop_time < boss_couple_drop_limit)
										{
											ZhuaGuiMonsterFlushCfg * cfg = LOGIC_CONFIG->GetZhuaGuiConfig().GetFlushMonsterInfoByMonsterId(monster->GetMonsterId());
											if (NULL != cfg)
											{
												int drop_index = 0;
												for (std::vector<UInt16>::const_iterator it = cfg->couple_drop_id_list.begin(); it != cfg->couple_drop_id_list.end(); it++)
												{
													DROPPOOL->DropHelper(*it, role->GetScene(), monster->GetPos(), role->GetUserId(), role->GetProf(), monster->GetMonsterId(), false, false, &drop_index);
												}
											}

											++ common_p->zhuagui_get_couple_drop_time;
										}
									}
								}
							}
						}
					}

					m_zgfi.AddHunli(role->GetUID(), role_give_hunli, 0, 1);
				}

				ZhuaGuiMgr::Instance().SyncZhuaGuiInfo(role);
			}
		}

		if (m_boss_monster_id == monster->GetMonsterId())
		{
			m_zgfi.boss_isdead = 1;
		}
		else 
		{
			-- m_zgfi.monster_count;
		}

		this->NotifyZhuaGuiInfo(Protocol::ZHUAGUI_NOTIFY_REASON_MONSTER_DEAD, NULL);
	}
}

bool SpecialTeamZhuaGui::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			return false;
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;
			return monster->IsAlive();
		}
		break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialTeamZhuaGui::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			Role *friend_role = (Role*)obj;
			return (!check_alive || friend_role->IsAlive());
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			return false;
		}
		break;
	}

	return SpecialLogic::SpecialIsFriend(role, obj, check_alive);
}

void SpecialTeamZhuaGui::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL == role || NULL == falling_item) return;

	if (NULL != ITEMPOOL->GetItem(falling_item->GetItemId()))
	{
		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; i ++)
		{
			if (m_personal_info_list[i].uid == role->GetUID())
			{
				for (int j = 0; j < Protocol::SCZhuaGuiFbInfo::NEW_FB_REWARD_ITEM_SC_MAX; j++)
				{
					if (0 == m_personal_info_list[i].drop_item_list[j].item_id)
					{
						m_personal_info_list[i].drop_item_list[j].item_id = falling_item->GetItemId();
						m_personal_info_list[i].drop_item_list[j].is_bind = (falling_item->GetItemIsBind() ? 1 : 0);
						m_personal_info_list[i].drop_item_list[j].num = falling_item->GetItemNum();
						++ m_personal_info_list[i].drop_item_num;
						break;
					}
					else if (falling_item->GetItemId() == m_personal_info_list[i].drop_item_list[j].item_id)
					{
						m_personal_info_list[i].drop_item_list[j].num += falling_item->GetItemNum();
						break;
					}
				}
				break;
			}
		}
	}
}

void SpecialTeamZhuaGui::GetPickFallingItem(Role *role, FBDropItemInfo *info_list, int &list_index, int list_max)
{
	if (NULL == role || NULL == info_list || list_max <= 0) return;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; i++ )
	{
		if (m_personal_info_list[i].uid == role->GetUID() && m_personal_info_list[i].drop_item_num > 0)
		{
			if (m_personal_info_list[i].drop_item_num > Protocol::SCZhuaGuiFbInfo::NEW_FB_REWARD_ITEM_SC_MAX)
			{
				m_personal_info_list[i].drop_item_num = Protocol::SCZhuaGuiFbInfo::NEW_FB_REWARD_ITEM_SC_MAX;
			}

			for (int j = 0; j < m_personal_info_list[i].drop_item_num && list_index < list_max; j++)
			{
				info_list[list_index] = m_personal_info_list[i].drop_item_list[j];
				++ list_index;
			}
		}
	}
}

void SpecialTeamZhuaGui::AddMember(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	TeamInfo *team_info = role->GetTeamInfo();
	if (NULL == team_info || !team_info->InTeam())
	{
		return;
	}

	Team *team = team_info->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	int join_user_list[MAX_TEAM_MEMBER_NUM]; memset(join_user_list, 0, sizeof(join_user_list));

	int join_user_count = team->GetMemberUidList(join_user_list, MAX_TEAM_MEMBER_NUM);

	for (int i = 0; i < join_user_count && i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		m_personal_info_list[i].uid = join_user_list[i];
	}

}

void SpecialTeamZhuaGui::OnAllMonsterDead()
{
	if (0 == m_kick_time)
	{
		m_kick_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetZhuaGuiConfig().GetKickDelayTime();
	}

	this->NotifyZhuaGuiInfo(Protocol::ZHUAGUI_NOTIFY_REASON_ENTER_FINISH, NULL);
}

void SpecialTeamZhuaGui::NotifyZhuaGuiInfo(int reason, Role *role)
{
	m_zgfi.reason = reason;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&m_zgfi, sizeof(m_zgfi));
	}
	else
	{
		if (NULL != m_scene)
		{
			m_scene->SendToRole((const char *)&m_zgfi, sizeof(m_zgfi));
		}
	}



	m_zgfi.reason = reason;
	m_zgfi.enter_role_num = m_scene->RoleNum();
	m_zgfi.item_count = 0;
	m_zgfi.kick_time = m_kick_time;

	if (NULL != role)
	{
		if (Protocol::ZHUAGUI_NOTIFY_REASON_ENTER_FINISH == reason)
		{
			for (int k = 0; k < MAX_TEAM_MEMBER_NUM; k ++)
			{
				if (m_personal_info_list[k].uid == role->GetUID())
				{
					m_zgfi.item_count = m_personal_info_list[k].drop_item_num;

					if (m_personal_info_list[k].drop_item_num > 0)
					{
						memcpy(m_zgfi.item_list, m_personal_info_list[k].drop_item_list, sizeof(FBDropItemInfo) * m_personal_info_list[k].drop_item_num);
					}

					int lenth = sizeof(m_zgfi) - (Protocol::SCZhuaGuiFbInfo::NEW_FB_REWARD_ITEM_SC_MAX - m_personal_info_list[k].drop_item_num) * sizeof(FBDropItemInfo);

					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&m_zgfi, lenth);
					break;
				}
			}
		}
		else
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&m_zgfi, sizeof(m_zgfi));
		}
	}
	else
	{
		if (NULL != m_scene)
		{
			if (Protocol::ZHUAGUI_NOTIFY_REASON_ENTER_FINISH == reason)
			{
				for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
				{
					Role *temp_role = m_scene->GetRoleByIndex(i);
					if (NULL != temp_role)
					{
						for (int k = 0; k < MAX_TEAM_MEMBER_NUM; k ++)
						{
							if (m_personal_info_list[k].uid == temp_role->GetUID())
							{
								m_zgfi.item_count = m_personal_info_list[k].drop_item_num;

								if (m_personal_info_list[k].drop_item_num > 0)
								{
									memcpy(m_zgfi.item_list, m_personal_info_list[k].drop_item_list, sizeof(FBDropItemInfo) * m_personal_info_list[k].drop_item_num);
								}

								int lenth = sizeof(m_zgfi) - (Protocol::SCZhuaGuiFbInfo::NEW_FB_REWARD_ITEM_SC_MAX - m_personal_info_list[k].drop_item_num) * sizeof(FBDropItemInfo);

								EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char *)&m_zgfi, lenth);
								break;
							}
						}
					}
				}
			}

			else
			{
				m_scene->SendToRole((const char *)&m_zgfi, sizeof(m_zgfi));
			}
		}
	}
}

void SpecialTeamZhuaGui::NotiryZhuaGuiAddPerInfo(Role *role)
{
	Protocol::SCZhuaguiAddPerInfo cmd;
	cmd.couple_hunli_add_per = 0;
	cmd.couple_boss_add_per = 0;
	cmd.team_hunli_add_per = 0;
	cmd.team_boss_add_per = 0;

	if (m_is_has_couple)
	{
		cmd.couple_hunli_add_per = LOGIC_CONFIG->GetZhuaGuiConfig().GetCoupleHunliAddPer();
		cmd.couple_boss_add_per = LOGIC_CONFIG->GetZhuaGuiConfig().GetCoupleBossAddPer();
	}

	const ZhuaGuiTeamAdd * team_add = LOGIC_CONFIG->GetZhuaGuiConfig().GetTeamAddPer(m_team_member_count);
	if (NULL != team_add)
	{
		cmd.team_hunli_add_per = team_add->hunli_add_per;
		cmd.team_boss_add_per = team_add->add_boss_per;
	}

	if (role != NULL)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, sizeof(cmd));
	}
	else
	{
		for(unsigned int i = 0; i < m_scene->RoleNum(); i++)
		{
			Role *temp_role = m_scene->GetRoleByIndex(i);
			if (NULL != temp_role)
			{
				EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
			}
		}
	}
}



