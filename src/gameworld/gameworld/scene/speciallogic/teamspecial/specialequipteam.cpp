#include "specialequipteam.hpp"
#include "engineadapter.h"
#include "gamecommon.h"
#include "servercommon/errornum.h"
#include "obj/character/role.h"
#include "protocal/msgfight.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/fbequipconfig.hpp"
#include "internalcomm.h"
#include "world.h"
#include "item/itempool.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "other/event/eventhandler.hpp"
#include "other/fb/rolefbequip.hpp"
#include "scene/teamequipfbmanager/teamequipfbmanager.hpp"
#include "friend/teaminfo.hpp"
#include "global/team/teammanager.hpp"
#include "global/team/team.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "protocal/msgfb.h"
#include "obj/character/monster.h"
#include "globalconfig/globalconfig.h"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "globalconfig/otherconfig.h"

SpecialEquipTeam::SpecialEquipTeam(Scene *scene)
	: SpecialLogic(scene), m_is_passed(false), m_is_over(false), m_is_first_enter(true), m_can_jump(false), m_total_monster_num(0),
	m_totall_boss_num(0), m_kill_boss_num(0), m_kill_monster_num(0), m_jumplayer_limit_time(0), m_monster_hp_percent(0), m_team_index(0), 
	m_add_monster_maxhp_per(0), m_add_monster_gongji_per(0), m_add_monster_fangyu_per(0)
{
}

SpecialEquipTeam::~SpecialEquipTeam()
{

}

void SpecialEquipTeam::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	this->CheckSceneMonsterStatus(interval, now_second);
}

void SpecialEquipTeam::OnAddObj(Obj *obj)
{
	if (nullptr != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		++m_total_monster_num;

		Monster *monster = static_cast<Monster *>(obj);
		if (monster->IsBossMonster())
		{
			++m_totall_boss_num;
		}

		this->SyncSceneLogicInfo();

		Attribute maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		Attribute gongji = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
		Attribute fangyu = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU);
		Attribute add_maxhp = static_cast<Attribute>(m_add_monster_maxhp_per * ROLE_ATTR_PER * maxhp);
		Attribute add_gongji = static_cast<Attribute>(m_add_monster_gongji_per * ROLE_ATTR_PER * gongji);
		Attribute add_fangyu = static_cast<Attribute>(m_add_monster_fangyu_per * ROLE_ATTR_PER * fangyu);
//		long long total_maxhp = maxhp + add_maxhp;

		monster->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, add_maxhp);
		monster->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, add_gongji);
		monster->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, add_fangyu);
		monster->ChangeHp(&add_maxhp, true);
	}
}

void SpecialEquipTeam::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (nullptr == dead)
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = static_cast<Monster *>(dead);

		if (monster->IsBossMonster())
		{
			++m_kill_boss_num;
		}

		++m_kill_monster_num;

		this->SyncSceneLogicInfo();
	}
}

void SpecialEquipTeam::OnRoleEnterScene(Role *role)
{
	TeamEquipFBManager::Instance().OnEnterScene(role, m_scene->GetSceneKey(), m_scene->GetSceneID());

	if (m_is_first_enter)
	{
		m_is_first_enter = false;

		const FBEquipJumpLayerCfg *jump_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamJumpCfgBySceneID(m_scene->GetSceneID());
		if (NULL != jump_layer_cfg && jump_layer_cfg->CanJump())
		{
			m_jumplayer_limit_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + jump_layer_cfg->limit_time);
		}

		m_team_index = role->GetTeamInfo()->GetTeamIndex();

		Team *team = TeamManager::Instance().GetTeam(m_team_index);
		int m_member_num = (nullptr != team) ? team->GetMemberNum() : MAX_TEAM_MEMBER_NUM;
		const OtherTeamfbMonsterAttrConfig* monster_upattr_cfg = LOGIC_CONFIG->GetOtherCfg().GetTeamFbMonsterAttrCfg(m_member_num);
		if (nullptr != monster_upattr_cfg)
		{
			m_add_monster_maxhp_per = monster_upattr_cfg->monster_hp_percent;
			m_add_monster_gongji_per = monster_upattr_cfg->monster_gongji_percent;
			m_add_monster_fangyu_per = monster_upattr_cfg->monster_fangyu_percent;
		}
	}

	if (m_is_passed)
	{
		bool is_allover = false;

		const FBEquipLayerCfg *cur_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(m_scene->GetSceneID());
		if (nullptr != cur_layer_cfg)
		{
			const int max_layer = LOGIC_CONFIG->GetEquipFBConfig().GetTeamMaxLayer();

			if (cur_layer_cfg->layer >= max_layer)
			{
				is_allover = true;
			}
		}

		role->GetFBEquip()->OnFBFinish(m_is_over, m_is_passed, m_can_jump, is_allover);

		if (m_rewardexp_user_id_set.find(role->GetUserId()) == m_rewardexp_user_id_set.end())
		{
			const FBEquipLayerCfg *equip_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(m_scene->GetSceneID());
			if (NULL != equip_layer_cfg)
			{
				role->AddExp(equip_layer_cfg->pass_exp, "SpecialEquipTeam pass", Role::EXP_ADD_REASON_DEFAULT);
				TeamEquipFBManager::Instance().OnAddPassExp(role, m_scene->GetSceneKey(), equip_layer_cfg->pass_exp);

				//role->GetFBEquip()->OnTeamPass(equip_layer_cfg->show_layer);

				m_rewardexp_user_id_set.insert(role->GetUserId());
			}
		}
	}
	else
	{
		role->GetFBEquip()->OnFBFinish(m_is_over,m_is_passed, m_can_jump, false);
	}
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_TEAMEQUIP_FB_LEVEL); //嘉年华
	this->SyncSceneLogicInfo(role);
}

void SpecialEquipTeam::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (role == nullptr) return;

	//bool can_give_prelayer_reward = true;
	bool is_change_scene = false;

	if (!is_logout)
	{
		bool is_allover = false;

		if (m_is_passed)
		{
			if (nullptr != role->GetTeamInfo()->GetMyTeam())     // 先看一下我的队伍是否全通
			{
				int scene_key = role->GetTeamInfo()->GetMyTeam()->GetTeamSeq();
				is_allover = TeamEquipFBManager::Instance().GetIsTeamAllOver(scene_key);
			}

			const FBEquipLayerCfg *cur_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(m_scene->GetSceneID());
			if (!is_allover && nullptr != cur_layer_cfg)         // 我的队伍还没全通的情况下看看我是不是第一个通关的
			{
				const int max_layer = LOGIC_CONFIG->GetEquipFBConfig().GetTeamMaxLayer();

				if (cur_layer_cfg->layer >= max_layer)
				{
					is_allover = true;
				}
			}
		}

		if (is_allover || !m_is_passed)			//主动退出也给历史通关层奖励
		{
			short min_layer = role->GetFBEquip()->GetMemberMinLayer();
			role->GetFBEquip()->GiveEquipTeamFbLayerReward(min_layer);
		}

		role->GetFBEquip()->OnFBFinish(true, m_is_passed, m_can_jump, is_allover, true);
		is_change_scene = true;					//正常切场景标识
	}

	//掉线也给历史通关层奖励
	if (!is_change_scene)
	{
		short min_layer = role->GetFBEquip()->GetMemberMinLayer();
		role->GetFBEquip()->GiveEquipTeamFbLayerReward(min_layer);
	}

	TeamEquipFBManager::Instance().OnLeaveScene(role, m_scene->GetSceneKey());
	this->AutoPickFallingItem(role);
}

bool SpecialEquipTeam::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return false;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}


bool SpecialEquipTeam::SpecialCanTransport(Role *role)
{
	if (m_is_passed) return true;

	return false;
}

bool SpecialEquipTeam::OnTimeout()
{
	if (m_is_passed)
	{
		return true;
	}
	else
	{
		this->OnOver();
		this->DelayKickOutAllRole(2000);

		return true;
	}
}

void SpecialEquipTeam::OnJumpLayerReq(Role *role)
{
	if (NULL == role) return;

	if (NULL != role && m_is_passed && m_can_jump && NULL != m_scene)
	{
		const FBEquipJumpLayerCfg *jump_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamJumpCfgBySceneID(m_scene->GetSceneID());
		if (NULL != jump_layer_cfg)
		{
			int target_scene_id = LOGIC_CONFIG->GetEquipFBConfig().GetTeamSceneIDByLayer(jump_layer_cfg->target_layer);
			if (target_scene_id > 0)
			{
				{	// 跳层通关经验
					long long total_pass_exp = 0;
					const FBEquipLayerCfg *equip_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(NULL != m_scene ? m_scene->GetSceneID() : 0);
					if (NULL != equip_layer_cfg)
					{
						for (int i = equip_layer_cfg->layer + 1; i < jump_layer_cfg->target_layer; ++i)
						{
							const FBEquipLayerCfg *temp_equip_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgByLayer(i);
							if (NULL != temp_equip_layer_cfg)
							{
								total_pass_exp += temp_equip_layer_cfg->pass_exp;
							}
						}

						if (total_pass_exp > 0)
						{
							role->AddExp(total_pass_exp, "SpecialEquipTeam JumpLayer", Role::EXP_ADD_REASON_DEFAULT);
							TeamEquipFBManager::Instance().OnAddPassExp(role, m_scene->GetSceneKey(), total_pass_exp);
						}
					}
				}

				{	// 发送跳层奖励
					const ItemBase *itembase = ITEMPOOL->GetItem(jump_layer_cfg->gift_item.item_id);
					if (NULL != itembase)
					{
						static MailContentParam contentparam; contentparam.Reset();

						contentparam.item_list[0].item_id = jump_layer_cfg->gift_item.item_id;
						contentparam.item_list[0].is_bind = jump_layer_cfg->gift_item.is_bind ? 1 : 0;
						contentparam.item_list[0].num = jump_layer_cfg->gift_item.num;
						contentparam.item_list[0].invalid_time = itembase->CalInvalidTime();

						int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_equipfb_team_jump_subject);
						int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_equipfb_team_jump_content,
							LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerBySceneID(m_scene->GetSceneID()), jump_layer_cfg->target_layer, jump_layer_cfg->gift_item.item_id);

						if (length1 > 0 && length2 > 0)
						{
							MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}
					}
				}

				{	// 传送
					if (nullptr != role->GetTeamInfo()->GetMyTeam())
					{
						int scene_key = role->GetTeamInfo()->GetMyTeam()->GetTeamSeq();
						World::GetInstWorld()->GetSceneManager()->GoTo(role, target_scene_id, scene_key, jump_layer_cfg->enter_pos);
					}
				}
			}
		}
	}
}

void SpecialEquipTeam::GMJumpLayerReq(Role *role, int to_layer)
{
	int target_scene_id = LOGIC_CONFIG->GetEquipFBConfig().GetTeamSceneIDByLayer(to_layer);
	if (role == nullptr || target_scene_id <= 0 || m_scene == nullptr)
	{
		return;
	}

	const FBEquipJumpLayerCfg *target_layer = LOGIC_CONFIG->GetEquipFBConfig().GetTeamJumpCfgBySceneID(target_scene_id);

	if (nullptr != role->GetTeamInfo()->GetMyTeam() && target_layer != nullptr)
	{
		this->KillAllMonster();
		m_is_passed = true;
		m_can_jump = true;
		this->OnOver();

		int scene_key = role->GetTeamInfo()->GetMyTeam()->GetTeamSeq();
		World::GetInstWorld()->GetSceneManager()->GoTo(role, target_scene_id, scene_key, target_layer->enter_pos);
	}
}

void SpecialEquipTeam::OnAllMonsterDead()
{
	if (!m_is_passed)
	{
		m_is_passed = true;
		m_can_jump = (m_jumplayer_limit_time > 0 && EngineAdapter::Instance().Time() <= m_jumplayer_limit_time);
		this->OnOver();
	}
}

void SpecialEquipTeam::SendSceneLogicInfo(Role *role)
{
	this->SyncSceneLogicInfo(role);
}

void SpecialEquipTeam::SyncSceneLogicInfo(Role *role)
{
	Protocol::SCFBSceneLogicInfo sli;

	sli.scene_type = m_scene->GetSceneType();
	sli.is_finish = m_is_over ? 1 : 0;
	sli.is_pass = m_is_passed ? 1 : 0;
	sli.time_out_stamp = m_scene->GetSceneTimeOutStamp();
	sli.total_allmonster_num = m_total_monster_num;
	sli.total_boss_num = m_totall_boss_num;
	sli.kill_allmonster_num = m_kill_monster_num;
	sli.kill_boss_num = m_kill_boss_num;
	sli.pass_time_s = this->GetPassTime();
	sli.kick_timestamp = 0;

	if (role != nullptr)
	{
		if(nullptr != role->GetTeamInfo()->GetMyTeam())
		{
			int scene_key = role->GetTeamInfo()->GetMyTeam()->GetTeamSeq();
			sli.kick_timestamp = TeamEquipFBManager::Instance().GetTeamKickOutTimestamp(scene_key);
		}
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&sli, sizeof(sli));
	}
	else
	{
		m_scene->SendToRole((const char *)&sli, sizeof(sli));
	}
}

void SpecialEquipTeam::OnOver()
{
	if (!m_is_over)
	{
		m_is_over = true;

		{
			int finish_time = static_cast<int>(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());
			if (m_scene->GetTimeOut() > 0 && finish_time > (int)m_scene->GetTimeOut())
			{
				finish_time = m_scene->GetTimeOut();
			}

			this->OnPassNoticeWord();
			TeamEquipFBManager::Instance().OnLayerFinish(m_scene->GetSceneKey(), m_scene->GetSceneID(), m_is_passed, m_can_jump, finish_time);
		}

		this->SyncSceneLogicInfo();
	}
}

void SpecialEquipTeam::OnPassNoticeWord()
{
	if (!m_is_over || !m_is_passed) return;

	const FBEquipLayerCfg *cur_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(m_scene->GetSceneID());
	if (cur_layer_cfg == nullptr || !cur_layer_cfg->is_pass_notice) return;

	Team *team = TeamManager::Instance().GetTeam(m_team_index);
	if (team == nullptr) return;
	int member_list[MAX_TEAM_MEMBER_NUM]; memset(member_list, 0, sizeof(member_list));
	int member_count = team->GetMemberUidList(member_list, MAX_TEAM_MEMBER_NUM);
	
	int all_name_len = 0; static char all_name_str[256] = { 0 };
	char member_name_str[64] = { 0 };
	for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; i++)
	{
		if (member_list[i] > 0)
		{
			Role *role = m_scene->GetRoleByUID(member_list[i]);
			if (role == nullptr) continue;

			int namelen = SNPRINTF(member_name_str, sizeof(member_name_str), "{r;%d;%s;%d} ",
				(int)UidToInt(role->GetUserId()), role->GetName(), role->GetCamp());
			if (namelen > 0 && all_name_len < (int)sizeof(all_name_str) - 1)
			{
				STRNCPY(all_name_str + all_name_len, member_name_str, sizeof(all_name_str) - all_name_len);
				all_name_len += namelen;
			}
		}
	}

	all_name_str[sizeof(all_name_str) - 1] = 0;

	int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_equip_fb_pass_notice, all_name_str, m_scene->GetSceneID());
	if (sendlen > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
	}
}

bool SpecialEquipTeam::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(10, 10, realive_pos)) return true;
		}
	}

	return false;
}

void SpecialEquipTeam::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (role == nullptr) return;
	Team* team = role->GetTeamInfo()->GetMyTeam();
	if (!team)
	{
		return;
	}
	int scene_key = team->GetTeamSeq();
	TeamEquipFBManager::Instance().OnPickFallingItem(role, falling_item, item_wrapper, scene_key);

	{
		//用于客户端掉落统计
		Protocol::SCEquipFBResult::FallItemInfo fall_item_list[NEQFB_PICK_FALL_ITEM_RECORD_COUNT];
		memset(fall_item_list, 0, sizeof(fall_item_list));

		int m_get_item_count = TeamEquipFBManager::Instance().GetTotalFallItem(role, scene_key, fall_item_list, NEQFB_PICK_FALL_ITEM_RECORD_COUNT);		//掉落物品数

		Protocol::SCFBDropInfo fbdi;
		fbdi.get_coin = 0;
		fbdi.get_item_count = 0;

		memset(fbdi.drop_items, 0, sizeof(Protocol::SCFBDropInfo::DropItem) * Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM);
		for (int i = 0; i < m_get_item_count && i < NEQFB_PICK_FALL_ITEM_RECORD_COUNT; ++i)
		{
			fbdi.drop_items[i].item_id = fall_item_list[i].item_id;
			fbdi.drop_items[i].num = static_cast<short>(fall_item_list[i].item_num);

			++fbdi.get_item_count;
		}

		int len = sizeof(fbdi) - sizeof(fbdi.drop_items[0]) * (Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM - fbdi.get_item_count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbdi, len);
	}
}
