#include "specialyizhandaodi.hpp"

#include "scene/activityshadow/activityshadow.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "config/logicconfigmanager.hpp"

#include "obj/otherobj/scenearea.h"
#include "monster/monsterpool.h"
#include "servercommon/errornum.h"

#include "skill/skillbase.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectbianshen.hpp"
#include "effect/skilleffect/effectdeadlyattack.hpp"
#include "ai/monsterai/monsteraibase.hpp"
#include "ai/monsterai/specialai/monsterailingshanxianqi.hpp"

#include "other/event/eventhandler.hpp"
#include "servercommon/noticenum.h"

#include "item/itempool.h"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "global/worldstatus/worldstatus.hpp"
#include "protocal/msgfight.h"
#include "config/activityconfig/yizhandaodiconfig.hpp"
#include "obj/otherobj/fallingitem.h"
#include "global/usercache/usercache.hpp"

#include "world.h"
#include "item/knapsack.h"
#include "other/roleactivity/roleactivity.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "gameworld/global/activity/activitymsgmanager.h"

SpecialYiZhanDaoDi::SpecialYiZhanDaoDi(Scene *scene)
	: SpecialLogic(scene), m_isopen(false), m_rankinfo_count(0), m_next_refresh_gahter_time(0), m_next_broast_total_num_time(0)
{
	this->Clear();
}

SpecialYiZhanDaoDi::~SpecialYiZhanDaoDi()
{

}

int SpecialYiZhanDaoDi::GetSpecialParam(Role *role)
{
	return role->GetRoleActivity()->GetYiZhanDaoDiJiShaCount(); 
}

void SpecialYiZhanDaoDi::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_isopen) return;

	if (m_yzdli.next_lucky_timestamp > 0 && now_second >= m_yzdli.next_lucky_timestamp)
	{
		m_yzdli.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetLuckyInterval();

		this->GaveLuckyItemToUser();
	}

	if (m_next_refresh_gahter_time >0 && static_cast<unsigned int>(now_second) > m_next_refresh_gahter_time)
	{
		this->RefreshBuff(static_cast<unsigned int>(now_second));
	}

	if (m_next_broast_total_num_time > 0 && static_cast<unsigned int>(now_second) > m_next_broast_total_num_time)
	{
		m_next_broast_total_num_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetBroactRoleNumInterval();

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_broast_cur_role_num,
			m_scene->RoleNum(), LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetBroactTitleID());
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
		}
	}

	this->CheckRankTopMsg();
}

void SpecialYiZhanDaoDi::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (!m_isopen || NULL == dead) return;
	Role *dead_role = dynamic_cast<Role*>(dead);
	Role *kill_role = m_scene->GetPrincipal(killer_id);
	if (NULL != dead_role && NULL != kill_role)
	{
		YiZhanDaoDiUser *yizhandaodi_deaduser = this->GetYiZhanDaoDiUser(dead_role->GetUserId());
		YiZhanDaoDiUser *yizhandaodi_killeruser = this->GetYiZhanDaoDiUser(kill_role->GetUserId());
		if (NULL != yizhandaodi_deaduser && NULL != yizhandaodi_killeruser)
		{
			// 处理输的 有可能踢他
			++yizhandaodi_deaduser->dead_count;
			dead_role->GetRoleActivity()->SetYiZhanDaoDiDeadTimes(yizhandaodi_deaduser->dead_count);

			this->CheckDeadRewardItem(dead_role);

			const int dead_count_max = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetDeadMaxCount();
			if (yizhandaodi_deaduser->dead_count >= dead_count_max)
			{
				// 弹窗 通知玩家
				static Protocol::SCYiZhanDaoDiKickout kbf;
				kbf.reason = Protocol::KICKOUT_BATTLE_FIELD_REASON_DEAD_ISOUT;
				EngineAdapter::Instance().NetSend(dead_role->GetNetId(), (const char*)&kbf, sizeof(kbf));

				{
					int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_kickout_notice,
						dead_role->GetUID(), dead_role->GetName(), dead_role->GetCamp());
					if (str_len > 0)
					{
						this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len);
					}
				}


				dead_role->ReAlive(100, 100);
				this->DelayKickOutRole(dead_role);
			}
			this->CheckUserKillRank(yizhandaodi_deaduser);
			this->SendInfoToUser(dead_role, yizhandaodi_deaduser, false);

			// 处理赢的 有可能更新称号
			yizhandaodi_killeruser->jisha_score += LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetKillHonor();
			++yizhandaodi_killeruser->jisha_count;
			int new_jisha_title_id = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetJiShaTitle(yizhandaodi_killeruser->jisha_count);
			if (new_jisha_title_id != yizhandaodi_killeruser->jisha_title_id)
			{
				yizhandaodi_killeruser->jisha_title_id = new_jisha_title_id;
				kill_role->GetRoleActivity()->SetYiZhanDaoDiJiShaCount(yizhandaodi_killeruser->jisha_count);

				static Protocol::SCYiZhanDaoDiTitleChange yzddtc;
				yzddtc.uid = UidToInt(kill_role->GetUserId());
				yzddtc.obj_id = kill_role->GetId();
				yzddtc.title_id = 0;
				yzddtc.jisha_count = yizhandaodi_killeruser->jisha_count;
				if (NULL != kill_role->GetScene()) kill_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(kill_role->GetObserHandle(), &yzddtc, sizeof(yzddtc));

				{
					int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandao_jisha_count_notice,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), yizhandaodi_killeruser->jisha_count);
					if (str_len > 0)
					{
						this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len);
					}
				}
			}
			this->CheckUserKillRank(yizhandaodi_killeruser, kill_role);
			this->CheckUserKillNumReward(kill_role, yizhandaodi_killeruser);
			this->SendInfoToUser(kill_role, yizhandaodi_killeruser, false);
			

			// 杀人传闻
			for (std::map<UserID, time_t>::iterator it = yizhandaodi_deaduser->hurt_map.begin(), end = yizhandaodi_deaduser->hurt_map.end(); it != end; ++it)
			{
				if (it->second + ASSIST_LIMIT_TIME >= EngineAdapter::Instance().Time() && it->first != kill_role->GetUserId())
				{
					YiZhanDaoDiUser *assist_user = this->GetYiZhanDaoDiUser(it->first);
					if (NULL != assist_user)
					{
						assist_user->assist++;
						assist_user->lianzhan++;

						Role *assist_role = m_scene->GetRoleByUID(assist_user->uid);
						if (NULL != assist_role)	//在这里加个传闻 先判断是否有助攻
						{
							this->CheckLianZhanNotice(assist_role, dead_role, true);
						}
					}
				}
			}

			yizhandaodi_killeruser->lianzhan++;

			this->CheckLianZhanNotice(kill_role, dead_role);
			yizhandaodi_deaduser->lianzhan = 0;
			yizhandaodi_deaduser->hurt_map.clear();
		}
		else
		{
			YiZhanDaoDiUser *yizhandaodi_deaduser = this->GetYiZhanDaoDiUser(dead_role->GetUserId());
			if (NULL != yizhandaodi_deaduser)
			{
				yizhandaodi_deaduser->lianzhan = 0;
				yizhandaodi_deaduser->hurt_map.clear();
			}
		}
	}
}

void SpecialYiZhanDaoDi::OnAddObj(Obj *obj)
{
	if (!m_isopen || NULL == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		YiZhanDaoDiUser *yizhandaodi_user = this->GetYiZhanDaoDiUser(role->GetUserId());
		if (NULL != yizhandaodi_user) 
		{
			role->GetRoleActivity()->SetYiZhanDaoDiJiShaCount(yizhandaodi_user->jisha_count);
			F_STRNCPY(yizhandaodi_user->user_name, role->GetName(), sizeof(GameName));

			EventHandler::Instance().OnEnterYiZhanDaoDi(role);

			this->SendInfoToUser(role, yizhandaodi_user, false);
			this->SendRankToUser(role);
			this->SendLuckyInfoToUser(role);
		}
	}
}

void SpecialYiZhanDaoDi::OnRemoveObj(Obj *obj)
{
	if (!m_isopen) return;
}

void SpecialYiZhanDaoDi::OnRoleEnterScene(Role *role)
{
	role->GetEffectList()->ClearFightEffect(EffectBase::CLEAR_REASON_ENTER_SCENE);
	this->RecalcGuwuBuff(role);
	this->RecalDeadRewardBuff(role);

	YiZhanDaoDiUser *user = this->GetYiZhanDaoDiUser(role->GetUserId());
	if (NULL != user && user->hp_when_leave > 0)
	{
		Attribute new_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		Attribute hp_add_value = user->hp_when_leave - new_hp;

		if (hp_add_value > 0)
		{
			role->ChangeHp(&hp_add_value, true);
		}
	}

	{ // 进入场景三秒无敌
		EffectSpecialState *wudi_effect = new EffectSpecialState(role->GetId(), 1, 3000, EBT_WUDI, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		wudi_effect->SetClientEffectType(EffectBase::ECT_YZDD_WUDI);
		role->AddEffect(wudi_effect);
	}
}

void SpecialYiZhanDaoDi::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (!m_isopen) return;

	YiZhanDaoDiUser *user = this->GetYiZhanDaoDiUser(role->GetUserId());
	if (NULL != user)
	{
		user->hp_when_leave = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	}

	{
		YiZhanDaoDiUser *yizhandaodi_user = this->GetYiZhanDaoDiUser(role->GetUserId());
		if (NULL != yizhandaodi_user)
		{
			++yizhandaodi_user->dead_count;
			role->GetRoleActivity()->SetYiZhanDaoDiDeadTimes(yizhandaodi_user->dead_count);
			const int dead_count_max = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetDeadMaxCount();
			this->SendInfoToUser(role, yizhandaodi_user, yizhandaodi_user->dead_count >= dead_count_max);
		}
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	role->GetRoleActivity()->SetYiZhanDaoDiLeaveInfo(now_second, m_scene->GetSceneKey() - 1);
}

bool SpecialYiZhanDaoDi::SpecialRelive(Role *role, int realive_type)
{
	if (!m_isopen || NULL == role) return false;

	Posi realive_pos(0, 0);
	if (LOGIC_CONFIG->GetYiZhanDaoDiConfig().RandomRealivePos(&realive_pos))
	{
		role->ReAlive(100, 100, realive_pos);
		this->RecalDeadRewardBuff(role);

		Attribute blood = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		Attribute delta_blood = blood - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		role->ChangeHp(&delta_blood, true);
	}

	{ // 复活三秒无敌
		EffectSpecialState *wudi_effect = new EffectSpecialState(role->GetId(), 1, 3000, EBT_WUDI, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		wudi_effect->SetClientEffectType(EffectBase::ECT_YZDD_WUDI);
		role->AddEffect(wudi_effect);
	}

	return true;
}

bool SpecialYiZhanDaoDi::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *target_role = (Role*)obj;

		bool is_friend = (role->GetUserId() == target_role->GetUserId());
		bool is_enemy = target_role->IsAlive() && !is_friend;

		return is_enemy;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

void SpecialYiZhanDaoDi::OnActivityStatusChange(int status)
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

void SpecialYiZhanDaoDi::YiZhanDaoDiGuWu(Role *role, Protocol::CSYiZhanDaoDiGuwu *yzddgw)
{
	if (role == nullptr || yzddgw == nullptr)
	{
		return;
	}

	YiZhanDaoDiUser *user = this->GetYiZhanDaoDiUser(role->GetUserId());
	if (NULL != user)
	{
		bool guwu_succ = false;

		if (Protocol::YIZHANDAODI_GUWU_TYPE_GONGJI == yzddgw->guwu_type)
		{
			int need_gold = 0, max_percent = 0, add_percent = 0;
			LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetGongjiGuWuInfo(&need_gold, &max_percent, &add_percent);

			if (user->gongji_guwu_per < max_percent)
			{
				if (!role->GetKnapsack()->GetMoney()->UseGold(need_gold, "YizhandaodiGuwu"))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}

				user->gongji_guwu_per += add_percent;
				if (user->gongji_guwu_per > max_percent) user->gongji_guwu_per = max_percent;

				guwu_succ = true;
			}
			else
			{
				role->NoticeNum(errornum::EN_YIZHANDAODI_GUWU_MAX);
			}
		}
		/*else if (Protocol::YIZHANDAODI_GUWU_TYPE_MAXHP == yzddgw->guwu_type)
		{
			int need_gold = 0, max_percent = 0, add_percent = 0;
			LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetMaxHPGuWuInfo(&need_gold, &max_percent, &add_percent);

			if (user->maxhp_guwu_per < max_percent)
			{
				if (!role->GetKnapsack()->GetMoney()->UseGold(need_gold, "YizhandaodiGuwu"))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}

				user->maxhp_guwu_per += add_percent;
				if (user->maxhp_guwu_per > max_percent) user->maxhp_guwu_per = max_percent;

				guwu_succ = true;
			}
			else
			{
				role->NoticeNum(errornum::EN_YIZHANDAODI_GUWU_MAX);
			}
		}*/

		if (guwu_succ)
		{
			role->NoticeNum(noticenum::NT_YIZHANDAODI_GUWU_SUCC);

			this->RecalcGuwuBuff(role);
			this->SendInfoToUser(role, user, false);
		}
	}
} 

void SpecialYiZhanDaoDi::RefreshBuff(unsigned int now_sec)
{
	m_next_refresh_gahter_time = now_sec + LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetGatherInterval();

	int gather_num = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetGatherNum();
	int gather_exit_time = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetGatherExitTime();
	std::vector<Posi> &pos_list = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetGatherPosiVec();
	int pos_list_size = static_cast<unsigned int>(pos_list.size());
	if (0 >= pos_list_size) return;

	int start_index = 0;
	if (gather_num < pos_list_size)
	{
		start_index = RandomNum(pos_list_size - gather_num + 1);
	}

	for (int i = 0; i < gather_num && start_index < pos_list_size; i++)
	{
		if (!m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos_list[start_index].x, pos_list[start_index].y))
		{
			continue;
		}

		FallingItem *fallingitem = new FallingItem();
		fallingitem->Init(0, 0, false, 0, INVALID_USER_ID, m_scene->GetSceneID(), gather_exit_time, 0, FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST, false);
		fallingitem->SetPos(pos_list[start_index]);
		fallingitem->SetBuffFalling(true);
		fallingitem->SetBuffAppearan(FallingItem::FALLING_APPEARAN_YZDD_BUFF);
		m_scene->AddObj(fallingitem);
		start_index++;
	}
}

void SpecialYiZhanDaoDi::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL == role || falling_item == NULL) return;

	int buff_total_pro = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetGatherBuffToatalPro();
	if (buff_total_pro <= 0) return;

	int rand_buff_pro = RandomNum(buff_total_pro);
	std::map<int, YiZhanDaoDiGatherBuffCfg> &buff_map = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetGatherBuffMap();
	std::map<int, YiZhanDaoDiGatherBuffCfg>::iterator buff_iter = buff_map.begin();
	for (; buff_iter != buff_map.end(); buff_iter++)
	{
		if (rand_buff_pro < buff_iter->second.buff_pro)
		{
			break;
		}

		rand_buff_pro -= buff_iter->second.buff_pro;
	}

	if (buff_iter == buff_map.end()) return;

	// 加buff前删除无敌buff，因为有无敌buff作用其他buff都加不上了
	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_SPECIAL_STATE, EBT_WUDI, role->GetId(), EffectBase::PRODUCT_METHOD_SCENE_SKILL);

	switch (buff_iter->second.buff_type)
	{
	case YIZHAN_BUFF_ADD_SPEED:
	{
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_JIASU, role->GetId(), EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, role->GetId(), EffectBase::PRODUCT_METHOD_SCENE_SKILL);

		EffectMoveSpeed *add_speed = new EffectMoveSpeed(role->GetId(), 1, buff_iter->second.param[0], EBT_JIASU, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		add_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		int value = static_cast<int>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (buff_iter->second.param[1] * ROLE_ATTR_PER));
		add_speed->SetMoveSpeed(value, buff_iter->second.param[1] / 100);
		add_speed->SetClientEffectType(EffectBase::ECT_YZDD_ADD_SPEED);
		role->AddEffect(add_speed);

		role->NoticeNum(noticenum::NT_YIZHANDAODI_ADD_SPEED);

		if (0 != buff_iter->second.is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
				role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}
	}
	break;

	case YIZHAN_BUFF_DEC_SPEED:
	{
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_JIASU, role->GetId(), EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, role->GetId(), EffectBase::PRODUCT_METHOD_SCENE_SKILL);

		EffectMoveSpeed *dec_speed = new EffectMoveSpeed(role->GetId(), 1, buff_iter->second.param[0], EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		dec_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		int value = static_cast<int>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (buff_iter->second.param[1] * ROLE_ATTR_PER));
		dec_speed->SetMoveSpeed(-value, -(buff_iter->second.param[1] / 100));
		dec_speed->SetClientEffectType(EffectBase::ECT_YZDD_DEC_SPEED);
		role->AddEffect(dec_speed);

		role->NoticeNum(noticenum::NT_YIZHANDAODI_DEC_SPEED);

		if (0 != buff_iter->second.is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
				role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}
	}
	break;

	case YIZHAN_BUFF_ADD_GONGJI:
	{
		EffectBuff *add_gongji = new EffectBuff(role->GetId(), 1, buff_iter->second.param[0], 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL, EBT_ADD_GONGJI);
		add_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (buff_iter->second.param[1] * ROLE_ATTR_PER));
		add_gongji->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, buff_iter->second.param[1]);
		add_gongji->SetClientEffectType(EffectBase::ECT_YZDD_ADD_GONGJI);
		role->AddEffect(add_gongji);

		role->NoticeNum(noticenum::NT_YIZHANDAODI_ADD_GONGJI);

		if (0 != buff_iter->second.is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
				role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}
	}
	break;

	case YIZHAN_BUFF_DEC_GONGJI:
	{
		EffectBuff *dec_gongji = new EffectBuff(role->GetId(), 1, buff_iter->second.param[0], 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL, EBT_REDUCE_GONGJI);
		dec_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (buff_iter->second.param[1] * ROLE_ATTR_PER));
		dec_gongji->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, -value, -buff_iter->second.param[1]);
		dec_gongji->SetClientEffectType(EffectBase::ECT_YZDD_DEC_GONGJI);
		role->AddEffect(dec_gongji);

		role->NoticeNum(noticenum::NT_YIZHANDAODI_DEC_GONGJI);

		if (0 != buff_iter->second.is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
				role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}
	}
	break;

	case YIZHAN_BUFF_ADD_FANGYU:
	{
		EffectBuff *add_fangyu = new EffectBuff(role->GetId(), 1, buff_iter->second.param[0], 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL, EBT_ADD_FANGYU);
		add_fangyu->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (buff_iter->second.param[1] * ROLE_ATTR_PER));
		add_fangyu->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, value, buff_iter->second.param[1]);
		add_fangyu->SetClientEffectType(EffectBase::ECT_YZDD_ADD_FANGYU);
		role->AddEffect(add_fangyu);

		role->NoticeNum(noticenum::NT_YIZHANDAODI_ADD_FANGYU);

		if (0 != buff_iter->second.is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
				role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}
	}
	break;

	case YIZHAN_BUFF_DEC_FANGYU:
	{
		EffectBuff *dec_fangyu = new EffectBuff(role->GetId(), 1, buff_iter->second.param[0], 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL, EBT_REDUCE_FANGYU);
		dec_fangyu->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (buff_iter->second.param[1] * ROLE_ATTR_PER));
		dec_fangyu->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, -value, -buff_iter->second.param[1]);
		dec_fangyu->SetClientEffectType(EffectBase::ECT_YZDD_DEC_FANGYU);
		role->AddEffect(dec_fangyu);

		role->NoticeNum(noticenum::NT_YIZHANDAODI_DEC_FANGYU);

		if (0 != buff_iter->second.is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
				role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}
	}
	break;

	case YIZHAN_BUFF_RECOVER:
	{
		Attribute recover_hp = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (buff_iter->second.param[1] * ROLE_ATTR_PER));
		if (recover_hp > 0)
		{
			EffectInterval *effect_interval = new EffectInterval(role->GetId(), 1, EBT_ADD_BLOOD, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
			effect_interval->SetParam(1000, buff_iter->second.param[0] / 1000, recover_hp);
			effect_interval->SetSave(false);
			effect_interval->SetClientEffectType(EffectBase::ECT_YZDD_RECOVER);
			role->AddEffect(effect_interval);

			role->NoticeNum(noticenum::NT_YIZHANDAODI_RECOVER);

			if (0 != buff_iter->second.is_notice)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
					role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
				if (length > 0)
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
				}
			}
		}
	}
	break;

	case YIZHAN_BUFF_WUDI:
	{
		EffectSpecialState *wudi_effect = new EffectSpecialState(role->GetId(), 1, buff_iter->second.param[0], EBT_WUDI, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		wudi_effect->SetClientEffectType(EffectBase::ECT_YZDD_WUDI);
		role->AddEffect(wudi_effect);

		role->NoticeNum(noticenum::NT_YIZHANDAODI_WUDI);

		if (0 != buff_iter->second.is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
				role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}
	}
	break;

	case YIZHAN_BUFF_BIANSHEN:
	{
		{
			EffectBianshen *effect_bianshen = new EffectBianshen(role->GetId(), 1, buff_iter->second.param[0], 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			effect_bianshen->SetImage(APPEARANCE_YIZHANDAODI);
			effect_bianshen->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
			effect_bianshen->SetClientEffectType(EffectBase::ECT_YZDD_BIANSHEN);
			role->AddEffect(effect_bianshen);

			role->NoticeNum(noticenum::NT_YIZHANDAODI_BIANSHEN);

			// 查找BUFF是否添加成功
			EffectBase *add_buff = role->GetEffectList()->FindEffect(EffectBase::EFFECT_TYPE_BIANSHEN, EBT_BIANXING_FOOL, role->GetId());
			if (nullptr != add_buff)
			{
				static Protocol::SCBianShenView cmd;
				cmd.obj_id = role->GetId();
				cmd.bianshen_image = APPEARANCE_YIZHANDAODI;
				role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
			}

			if (0 != buff_iter->second.is_notice)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
					role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
				if (length > 0)
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
				}
			}
		}

		{
			role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_JIASU, role->GetId(), EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, role->GetId(), EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			EffectMoveSpeed *dec_speed = new EffectMoveSpeed(role->GetId(), 1, buff_iter->second.param[2], EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			dec_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
			int value = static_cast<int>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (buff_iter->second.param[3] * ROLE_ATTR_PER));
			dec_speed->SetMoveSpeed(-value, -(buff_iter->second.param[3] / 100));
			role->AddEffect(dec_speed);
		}
	}
	break;

	case YIZHAN_BUFF_DEADLY_ATTACK:
	{
		role->NoticeNum(noticenum::NT_YIZHANDAODI_DEADLY_ATTACK);

		EffectDeadlyAttack *deadly_attack = new EffectDeadlyAttack(role->GetId(), 1, buff_iter->second.param[0], EBT_DEADLY_ATTACK, buff_iter->second.param[1], EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		deadly_attack->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		deadly_attack->SetClientEffectType(EffectBase::ECT_YZDD_DEADLY_ATTACK);
		role->AddEffect(deadly_attack);

		if (0 != buff_iter->second.is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_buff_broast,
				role->GetUID(), role->GetName(), static_cast<int>(role->GetCamp()));
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}
	}
	break;

	default:
		break;
	}
}

void SpecialYiZhanDaoDi::OnActivityStart()
{
	m_isopen = true;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	this->Clear();
	m_yzdli.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetLuckyInterval();
	m_yzdli.luck_user_count = 0;
	m_next_refresh_gahter_time = now_sec + LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetGatherFirstInterval();
	m_next_broast_total_num_time = now_sec + LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetBroactRoleNumInterval();
}

void SpecialYiZhanDaoDi::OnActivityClose()
{
	// 活动关闭，在场景里的人结算一下
	{
		for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (role == nullptr) continue;
			YiZhanDaoDiUser *user = this->GetYiZhanDaoDiUser(role->GetUserId());
			if (user == nullptr) continue;
			this->SendInfoToUser(role, user, true);
		}
	}

	m_isopen = false;

	this->CalcReward();

	this->DelayKickOutAllRole();

	this->Clear();
}

void SpecialYiZhanDaoDi::CheckUserKillRank(YiZhanDaoDiUser *yizhandaodi_user, Role * role)
{
	if (yizhandaodi_user->jisha_count <= 0)
	{
		return;
	}

	if (m_rankinfo_count >= YIZHANDAODI_RANK_NUM)
	{
		m_rankinfo_count = YIZHANDAODI_RANK_NUM;

		if (yizhandaodi_user->jisha_count <= m_rankinfo_list[m_rankinfo_count - 1].jisha_count)
		{
			return;
		}
	}

	int temp_index = 0;
	for (temp_index = 0; temp_index < m_rankinfo_count && temp_index < YIZHANDAODI_RANK_NUM; temp_index ++)
	{
		if (m_rankinfo_list[temp_index].user_id == yizhandaodi_user->user_id)
		{
			if (m_rankinfo_list[temp_index].jisha_count == yizhandaodi_user->jisha_count) 
			{
				m_rankinfo_list[temp_index].dead_count = yizhandaodi_user->dead_count;
				this->SendRankToUser();

				return;
			}
	
			break;
		}
	}

	if (temp_index >= m_rankinfo_count)
	{
		if (m_rankinfo_count < YIZHANDAODI_RANK_NUM) m_rankinfo_count ++;
		temp_index = m_rankinfo_count - 1;
	}

	UserID old_user_id = m_rankinfo_list[0].user_id;
	for (int i = 0; i < m_rankinfo_count && i < YIZHANDAODI_RANK_NUM; i ++)
	{
		if (yizhandaodi_user->jisha_count > m_rankinfo_list[i].jisha_count)
		{
			for (int j = temp_index; j >= i + 1; j --) 
			{
				m_rankinfo_list[j] = m_rankinfo_list[j - 1];
			}

			m_rankinfo_list[i].user_id = yizhandaodi_user->user_id;
			m_rankinfo_list[i].jisha_count = yizhandaodi_user->jisha_count;
			m_rankinfo_list[i].dead_count = yizhandaodi_user->dead_count;
			F_STRNCPY(m_rankinfo_list[i].user_name, yizhandaodi_user->user_name, sizeof(GameName));
			if (i == 0 && NULL != role && yizhandaodi_user->user_id != old_user_id) //第一名论替传闻
			{
				UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_rankinfo_list[1].user_id);
				if (NULL != user_node)
				{
					ActivityMsgManager::RankTopItem first;
					first.uid = role->GetUID();
					F_STRNCPY(first.user_name, role->GetName(), sizeof(first.user_name));
					first.camp = role->GetCamp();
					ActivityMsgManager::Instance().RankFirstMsg(m_scene, first);
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_first_replace_notice,
						role->GetUID(), role->GetName(), role->GetCamp(), user_node->uid, user_node->user_name, (int)user_node->camp);
					if (length > 0)
					{
						this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
					}
				}
			}
			this->SendRankToUser();
			break;
		}
	}
}

void SpecialYiZhanDaoDi::CheckUserKillNumReward(Role *role, YiZhanDaoDiUser *user)
{
	if (NULL == role || NULL == user ) return;

	int index = -1 ;
	const YiZhanDaoDiConfig::KillNumReward *reward_cfg = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetKillNumReward(user->jisha_count, index) ;
	if (index < 0 || reward_cfg == NULL || index > 31)
	{
		return;
	}
	if ( 0 != (user->kill_num_reward_flag &(1 << index)))
	{
		return;
	}
	if (role->GetUserId() != user->user_id)
	{
		return;
	}

	if (role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item, PUT_REASON_YIZHANDAODI_KILL_REWARD))
	{
		for (int i = 0; i < reward_cfg->reward_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			user->reward_item_vec.push_back(reward_cfg->reward_item[i]);
		}

		user->kill_num_reward_flag |= (1 << index);
	}
}


void SpecialYiZhanDaoDi::GaveLuckyItemToUser()
{
	int lucky_role_count = 0;
	int lucky_role_idx_list[YIZHANDAODI_LUACY_ROLE_MAX] = {0};

	{
		std::map<int, int> idx_map;

		int max_rolenum = m_scene->RoleNum();
		for (int i = 0; i < max_rolenum; i++)  // 通过map来打乱idx，用于随机
		{
			idx_map.insert(std::make_pair(RandomNum(1024), i));
		}

		int loop_count = 0;
		while (!idx_map.empty() && lucky_role_count < YIZHANDAODI_LUACY_ROLE_MAX && ++ loop_count < 128)
		{
			int rand_index = RandomNum(static_cast<int>(idx_map.size()));
			std::map<int, int>::iterator it = idx_map.begin();
			std::advance(it, rand_index);
			
			lucky_role_idx_list[lucky_role_count ++] = it->second;
			idx_map.erase(it);
		}
	}

	m_yzdli.luck_user_count = 0;

	char str_format[256] = {0};
	int max_length = sizeof(str_format) / sizeof(str_format[0]);
	int str_length = 0;

	const ItemConfigData &lucky_item = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetLuckyItem();

	for (int i = 0; i < lucky_role_count && i < YIZHANDAODI_LUACY_ROLE_MAX; i++)
	{
		int role_idx = lucky_role_idx_list[i];
		if (role_idx < (int)m_scene->RoleNum())
		{
			Role *lucky_role = m_scene->GetRoleByIndex(role_idx);
			if (NULL != lucky_role)
			{
				static MailContentParam contentparam; contentparam.Reset();

				const ItemBase *itembase = ITEMPOOL->GetItem(lucky_item.item_id);
				if (NULL != itembase)
				{
					contentparam.item_list[0].item_id = lucky_item.item_id;
					contentparam.item_list[0].is_bind = lucky_item.is_bind ? 1 : 0;
					contentparam.item_list[0].num = lucky_item.num;
					contentparam.item_list[0].invalid_time = itembase->CalInvalidTime();
				}
				
				MailRoute::MailToUser(lucky_role->GetUserId(), SYSTEM_MAIL_REASON_YIZHANDAODI_LUCKY_REWARD, contentparam);

				if (m_yzdli.luck_user_count < YIZHANDAODI_LUACY_ROLE_MAX)
				{
					lucky_role->GetName(m_yzdli.luck_user_namelist[m_yzdli.luck_user_count]);
					++ m_yzdli.luck_user_count;
				}

				YiZhanDaoDiUser *yizhandaodi_user = this->GetYiZhanDaoDiUser(lucky_role->GetUserId());
				if (NULL != yizhandaodi_user)
				{
					++ yizhandaodi_user->lucky_count;
				}

				if (i < 5)	// 最多打包5个名字 不用YIZHANDAODI_LUACY_ROLE_MAX 防止改动会导致字符串不够
				{
					int temp_length = SNPRINTF(str_format + str_length, max_length - str_length, "{r;%d;%s;%d} ", 
						UidToInt(lucky_role->GetUserId()), lucky_role->GetName(), lucky_role->GetCamp());
					str_length += temp_length;
				}
			}
		}
	}

	this->SendLuckyInfoToUser();

	{
		int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_lucky_notice, 
			str_format);
		if (str_len > 0) 
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len);
		}
	}
}

void SpecialYiZhanDaoDi::SendInfoToUser(Role *role, YiZhanDaoDiUser *user, bool jiesuan)
{
	if (NULL == role || NULL == user) return;
	Protocol::SCYiZhanDaoDiUserInfo yzddgi;
	yzddgi.jisha_count = user->jisha_count;
	yzddgi.jisha_title_id = user->jisha_title_id;
	yzddgi.dead_count = user->dead_count;
	yzddgi.jisha_score = user->jisha_score;
	yzddgi.gongji_guwu_per = user->gongji_guwu_per;
	yzddgi.maxhp_guwu_per = user->maxhp_guwu_per;
	yzddgi.kill_num_reward_flag = user->kill_num_reward_flag;
	yzddgi.show_jiesuan = jiesuan ? 1 : 0;
	yzddgi.reward_count = 0;
	if (jiesuan)
	{
		for (int i = 0; i < (int)user->reward_item_vec.size() && yzddgi.reward_count < Protocol::SCYiZhanDaoDiUserInfo::YZDD_REWARD_MAX_COUNT; ++i)
		{
			for (int j = 0; j < Protocol::SCYiZhanDaoDiUserInfo::YZDD_REWARD_MAX_COUNT; j++)
			{ 
				if ((int)user->reward_item_vec[i].item_id == yzddgi.kill_num_reward_list[j].item_id)
				{
					yzddgi.kill_num_reward_list[j].num += (short)user->reward_item_vec[i].num;
					break;
				}
				else if (yzddgi.kill_num_reward_list[j].item_id == 0)
				{
					yzddgi.kill_num_reward_list[j].item_id = (int)user->reward_item_vec[i].item_id;
					yzddgi.kill_num_reward_list[j].num = (short)user->reward_item_vec[i].num;
					yzddgi.kill_num_reward_list[j].is_bind = (short)user->reward_item_vec[i].is_bind;
					++yzddgi.reward_count;
					break;
				}
			}
		}
	}

	int send_len = sizeof(yzddgi) - (Protocol::SCYiZhanDaoDiUserInfo::YZDD_REWARD_MAX_COUNT - yzddgi.reward_count) * sizeof(yzddgi.kill_num_reward_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&yzddgi, send_len);
}

void SpecialYiZhanDaoDi::SendLuckyInfoToUser(Role *role)
{
	if (NULL == role)
	{
		int sendlen = sizeof(m_yzdli) - (YIZHANDAODI_LUACY_ROLE_MAX - m_yzdli.luck_user_count) * sizeof(m_yzdli.luck_user_namelist[0]);
		m_scene->SendToRole((const char*)&m_yzdli, sendlen);
	}
	else
	{
		int sendlen = sizeof(m_yzdli) - (YIZHANDAODI_LUACY_ROLE_MAX - m_yzdli.luck_user_count) * sizeof(m_yzdli.luck_user_namelist[0]);
		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(m_yzdli)))
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_yzdli, sendlen);
		}
	}
}

void SpecialYiZhanDaoDi::SendRankToUser(Role *role)
{
	if (NULL == role)
	{
		m_yzddri.count = 0;

		for (int i = 0; i < m_rankinfo_count && i < YIZHANDAODI_RANK_NUM; ++ i)
		{
			m_yzddri.rank_list[i].uid = UidToInt(m_rankinfo_list[i].user_id);
			m_yzddri.rank_list[i].jisha_count = m_rankinfo_list[i].jisha_count;
			m_yzddri.rank_list[i].dead_count = m_rankinfo_list[i].dead_count;
			F_STRNCPY(m_yzddri.rank_list[i].user_name, m_rankinfo_list[i].user_name, sizeof(GameName));

			++ m_yzddri.count;
		}

		int sendlen = sizeof(m_yzddri) - (YIZHANDAODI_RANK_NUM - m_yzddri.count) * sizeof(m_yzddri.rank_list[0]);
		for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
		{
			m_yzddri.self_jisha_count = 0; m_yzddri.self_rand_pos = 0;
			Role *t_role = m_scene->GetRoleByIndex(i);
			if (t_role != nullptr)
			{
				this->GetSelfRankInfo(t_role, m_yzddri.self_jisha_count, m_yzddri.self_rand_pos);
				EngineAdapter::Instance().NetSend(t_role->GetNetId(), (const char*)&m_yzddri, sendlen);
			}
		}
		//m_scene->SendToRole((const char*)&m_yzddri, sendlen);
	}
	else
	{
		int sendlen = sizeof(m_yzddri) - (YIZHANDAODI_RANK_NUM - m_yzddri.count) * sizeof(m_yzddri.rank_list[0]);
		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(m_yzddri)))
		{
			m_yzddri.self_jisha_count = 0; m_yzddri.self_rand_pos = 0;
			this->GetSelfRankInfo(role, m_yzddri.self_jisha_count, m_yzddri.self_rand_pos);
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_yzddri, sendlen);
		}
	}
}

void SpecialYiZhanDaoDi::GetSelfRankInfo(Role *role, short &jisha_count, short &rank_pos)
{
	if (role == nullptr) return;

	for (int i = 0; i < m_rankinfo_count && i < YIZHANDAODI_RANK_NUM; ++i)
	{
		if (m_rankinfo_list[i].user_id == role->GetUserId())
		{
			jisha_count = m_rankinfo_list[i].jisha_count;
			rank_pos = i + 1;
			return;
		}
	}
}

YiZhanDaoDiUser * SpecialYiZhanDaoDi::GetYiZhanDaoDiUser(const UserID &user_id)
{
	YiZhanDaoDiUserMapIt it = m_yizhandaodi_usermap.find(user_id);
	if (it == m_yizhandaodi_usermap.end()) // 不存在 插入新的用户
	{
		YiZhanDaoDiUser yizhandaodi_user;

		yizhandaodi_user.user_id = user_id;
		m_yizhandaodi_usermap[user_id] = yizhandaodi_user;
	}

	if (it == m_yizhandaodi_usermap.end()) it = m_yizhandaodi_usermap.find(user_id);

	if (it != m_yizhandaodi_usermap.end())
	{
		return &it->second;
	}

	return NULL;
}

void SpecialYiZhanDaoDi::Clear()
{
	m_rankinfo_count = 0;
	memset(m_rankinfo_list, 0, sizeof(m_rankinfo_list));

	m_yzddri.count = 0;

	m_yizhandaodi_usermap.clear();

	m_yzdli.next_lucky_timestamp = 0;

	m_yzdli.luck_user_count = 0;
	memset(m_yzdli.luck_user_namelist, 0, sizeof(m_yzdli.luck_user_namelist));
}

void SpecialYiZhanDaoDi::RecalDeadRewardBuff(Role *role)
{
	if (NULL == role) return;

	YiZhanDaoDiUser *user = this->GetYiZhanDaoDiUser(role->GetUserId());
	if (NULL != user)
	{
		const YiZhanDaoDiDeadConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetDeadCfg(user->dead_count);
		if (NULL != cfg_item && cfg_item->add_maxhp_percent > 0)
		{
			int skill_id = GetSkillIDBySceneTypeAndIndex(Scene::SCENE_TYPE_YIZHANDAODI, 2);
			role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, skill_id);

			int add_percent = cfg_item->add_maxhp_percent;
			int add_maxhp = (int)(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP) * (add_percent / 100.0f));

			EffectBuff *buff = new EffectBuff(INVALID_OBJ_ID, skill_id, 3600 * 1000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, add_maxhp, add_percent);
			buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			buff->SetClientEffectType(EffectBase::ECT_YZDD_DEAD_ADD);
			buff->SetClearOnDie(false);
			role->AddEffect(buff);
		}
	}
}

void SpecialYiZhanDaoDi::CheckDeadRewardItem(Role *role)
{
	if (NULL == role) return;

	YiZhanDaoDiUser *user = this->GetYiZhanDaoDiUser(role->GetUserId());
	if (NULL != user)
	{
		const YiZhanDaoDiDeadConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetDeadCfg(user->dead_count);
		if (NULL != cfg_item && cfg_item->has_item_reward > 0)
		{
			static MailContentParam contentparam; contentparam.Reset();
			const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = cfg_item->reward_item.is_bind;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();	
			}

			int length = 0;
			if (user->dead_count >= LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetDeadMaxCount())
			{
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_yizhandaodi_kickout_mail_content);
			}
			else
			{
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_yizhandaodi_dead_times_reward_content, user->dead_count);
			}
	
			if (length > 0)
			{
				MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}
}

void SpecialYiZhanDaoDi::CalcReward()
{
	std::set<UserID> rewarduser_set;

	for (int i = 0; i < m_rankinfo_count; i++)  // 榜上的人先发 
	{
		if (0 == i)
		{
			WorldStatus::Instance().SendYizhanDaoDiKingUserId(NULL);
		}
		if (i < 3)
		{
			UserID yizhandaodi_king_userid = m_rankinfo_list[i].user_id;
			WorldStatus::Instance().SetYiZhanDaoDiKingUserId(UidToInt(yizhandaodi_king_userid), i);
		}

		YiZhanDaoDiUserRankItem &rank_user = m_rankinfo_list[i];

		rewarduser_set.insert(rank_user.user_id);
		this->SendRewardToUser(rank_user.user_id, i);

	}
	if (1 == m_rankinfo_count)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_rankinfo_list[0].user_id);
		if (NULL != user_node)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_finish_rank_top1,
				user_node->uid, user_node->user_name, (int)user_node->camp);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
			}
		}
	}
	else if (2 == m_rankinfo_count)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_rankinfo_list[0].user_id);
		UserCacheNode *user_node2 = UserCacheManager::Instance().GetUserNode(m_rankinfo_list[1].user_id);

		if (NULL != user_node && NULL != user_node2)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_finish_rank_top2,
				user_node->uid, user_node->user_name, (int)user_node->camp,user_node2->uid,user_node2->user_name,(int)user_node2->camp);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
			}
		}
	}
	else if(m_rankinfo_count >=3 )
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_rankinfo_list[0].user_id);
		UserCacheNode *user_node2 = UserCacheManager::Instance().GetUserNode(m_rankinfo_list[1].user_id);
		UserCacheNode *user_node3 = UserCacheManager::Instance().GetUserNode(m_rankinfo_list[2].user_id);
		if (NULL != user_node && NULL != user_node2 && NULL != user_node3)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yizhandaodi_finish_rank_top3 , user_node->uid, user_node->user_name,
				(int)user_node->camp,user_node2->uid,user_node2->user_name,(int)user_node2->camp,user_node3->uid,user_node3->user_name,(int)user_node3->camp);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
			}
		}
	}

// 	if (m_rankinfo_count > 0)
// 	{
// 		World::GetInstWorld()->SendToAllGateway((const char*)&yzddrt, sizeof(Protocol::SCYiZhanDaoDiRankTopInfo));
// 	}

	for (YiZhanDaoDiUserMapIt it = m_yizhandaodi_usermap.begin(); it != m_yizhandaodi_usermap.end(); ++ it)
	{
		YiZhanDaoDiUser &yizhandaodi_user = it->second;

		if (rewarduser_set.find(yizhandaodi_user.user_id) == rewarduser_set.end()) // 发未上榜的
		{
			rewarduser_set.insert(yizhandaodi_user.user_id);
			this->SendRewardToUser(yizhandaodi_user.user_id, YIZHANDAODI_RANK_NUM);
		}
	}
}

void SpecialYiZhanDaoDi::RecalcGuwuBuff(Role *role)
{
	if (NULL == role) return;

	YiZhanDaoDiUser *user = this->GetYiZhanDaoDiUser(role->GetUserId());
	if (NULL != user && (user->gongji_guwu_per > 0 || user->maxhp_guwu_per > 0))
	{
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, 1);

		EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, 1, 60000 * 30, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect_buff->SetSave(false);
		effect_buff->SetClearOnDie(false);

		if (user->gongji_guwu_per > 0)
		{
			Attribute gongji_add_value = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * user->gongji_guwu_per / 100;
			if (gongji_add_value < 0) gongji_add_value = 0;
			effect_buff->AddGongJiEffect(gongji_add_value, user->gongji_guwu_per);
		}
		
		Attribute hp_add_value = 0;
		if (user->maxhp_guwu_per > 0)
		{
			hp_add_value = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * user->maxhp_guwu_per / 100;
			if (hp_add_value < 0) hp_add_value = 0;
			effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, hp_add_value, user->maxhp_guwu_per);
		}

		role->AddEffect(effect_buff);
		role->ChangeHp(&hp_add_value, true);
	}
}

void SpecialYiZhanDaoDi::SendRewardToUser(const UserID &user_id, int rank_idx)
{
	const ItemConfigData *cfg_item = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetJishaRankRewardItem(rank_idx);
	if (NULL == cfg_item)
	{
		return;
	}
	int reward_count = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetJishaRankRewardItemCount(rank_idx);
	
	static MailContentParam reward_mail; reward_mail.Reset();
	
	//策划要求改成奖励发item_list
	for (int i = 0; i < reward_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->item_id);
		if (NULL == item_base)
		{
			continue;
		}
		// 物品奖励
		reward_mail.item_list[i].item_id = cfg_item->item_id;
		reward_mail.item_list[i].num = cfg_item->num;
		reward_mail.item_list[i].is_bind = cfg_item->is_bind;
		reward_mail.item_list[i].invalid_time = item_base->CalInvalidTime();
	}

	int reward_shengwang = 0;
	{
		// 声望奖励
		YiZhanDaoDiUser *yizhandaodi_user = this->GetYiZhanDaoDiUser(user_id);
		if (NULL != yizhandaodi_user)
		{
			reward_shengwang = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetJishaShengwangReward(yizhandaodi_user->jisha_score);
			if (reward_shengwang > 0)
			{
				reward_mail.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG, reward_shengwang);
			}
		}
	}

	int l1 = SNPRINTF(reward_mail.subject, sizeof(reward_mail.subject), gamestring::g_yizhandaodi_reward_subject);
	int l2 = 0;
	
	if (rank_idx < YIZHANDAODI_RANK_NUM)
	{
		l2 = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), gamestring::g_yizhandaodi_reward_rank_content, rank_idx + 1, cfg_item->item_id, reward_shengwang);
	}
	else 
	{
		l2 = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), gamestring::g_yizhandaodi_reward_norank_content, cfg_item->item_id, reward_shengwang);
	}

	if (l1 > 0 && l2 > 0)
	{
		MailRoute::MailToUser(user_id, SYSTEM_MAIL_REASON_INVALID, reward_mail);
	}
	
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
		if (NULL != role && role->GetScene() == m_scene) // 同一个场景 才通知结算界面
		{
			YiZhanDaoDiUser *yizhandaodi_user = this->GetYiZhanDaoDiUser(role->GetUserId());
			if (NULL != yizhandaodi_user)
			{
				static Protocol::SCYiZhanDaoDiKickout yzddk;
				yzddk.reason = Protocol::KICKOUT_BATTLE_FIELD_REASON_TIMEOUT;
				yzddk.jisha_count = yizhandaodi_user->jisha_count;
				yzddk.dead_count = yizhandaodi_user->dead_count;
				yzddk.jisha_score = yizhandaodi_user->jisha_score;
				yzddk.lucky_count = yizhandaodi_user->lucky_count;
				yzddk.rank_pos = rank_idx + 1;
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&yzddk, sizeof(yzddk));
			}
		}
	}
}

void SpecialYiZhanDaoDi::NotifySystemMsg(char *str_buff, int str_len)
{
	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, SYS_MSG_ACTIVE_NOTICE))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void SpecialYiZhanDaoDi::CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist)
{
	return;
	if (NULL != kill_role && NULL != dead_role)
	{
		YiZhanDaoDiUser *killer = this->GetYiZhanDaoDiUser(kill_role->GetUserId());
		YiZhanDaoDiUser *dead = this->GetYiZhanDaoDiUser(dead_role->GetUserId());
		if (NULL != killer && NULL != dead)
		{
			if (!is_assist && dead->lianzhan >= 3)
			{
				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_stoplianzhan_notice,
					UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
					UidToInt(dead_role->GetUserId()), dead_role->GetName(), (int)dead_role->GetCamp(), dead->lianzhan);
				if (str_len > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len);
				}
			}

			if (killer->lianzhan > 0 && 0 == killer->lianzhan % 5)
			{
				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), killer->lianzhan, killer->lianzhan);

				if (str_len > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len);
				}
			}
		}
	}
}

void SpecialYiZhanDaoDi::CheckRankTopMsg()
{
	if (ActivityMsgManager::Instance().IsNeedRankTopMsg(m_scene))
	{
		ActivityMsgManager::RankTopItem rank[ACTIVITY_RANK_TOP_MSG_MAX];
		int i = 0;
		for (; i < ACTIVITY_RANK_TOP_MSG_MAX && i < m_rankinfo_count; ++i)
		{
			rank[i].uid = UidToInt(m_rankinfo_list[i].user_id);
			F_STRNCPY(rank[i].user_name, m_rankinfo_list[i].user_name, sizeof(rank[i].user_name));
		}
		ActivityMsgManager::Instance().RankTopMsg(m_scene, rank, i);
	}
}