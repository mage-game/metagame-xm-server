#include "specialcrosshotspring.hpp"
#include "obj/character/role.h"
#include "obj/otherobj/gatherobj.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "item/itempool.h"
#include "protocal/msgfight.h"
#include "engineadapter.h"
#include "internalcomm.h"

#include "world.h"
#include "gamelog.h"

#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectbianshen.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "globalconfig/globalconfig.h"
#include "config/activityconfig/hotspringconfig.hpp"

#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include <algorithm> 
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "servercommon/serverdef.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "config/activityconfig/questionconfig.hpp"

#include "item/knapsack.h"
#include "friend/friendmanager.h"
#include "other/vip/vip.hpp"
#include "other/sublock/sublock.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/cross/rolecross.h"
#include "other/itemlevelreward/itemlevelreward.hpp"

SpecialCrossHotspring::SpecialCrossHotspring(Scene *scene): SpecialLogic(scene), m_isopen(false), m_is_hidden_server(false), m_is_change(false),
	m_next_give_exp_timestamp(0), m_give_exp_interval(0), m_next_calc_rank_timestamp(0), m_popularity_rank_interval_s(0), m_next_give_item_timestamp(0), m_give_item_interval(0),
	m_is_finish_question_activity(false), m_is_can_answer(false), m_cur_question_begin_time(0), m_curr_question_end_time(0), m_next_question_begin_time(0),
	m_total_question_num(0), m_broadcast_question_total(0), m_curr_question_id(0), m_side(0), m_answer_continue_time(0), m_answer_prepare_time(0), m_last_first_broadcast_time(0),
	m_last_lover_broadcast_time(0), m_start_refresh_gather_time(0), m_next_refresh_gather_time(0)
{
		m_is_hidden_server = CrossConfig::Instance().IsHiddenServer();
}

SpecialCrossHotspring::~SpecialCrossHotspring()
{

}

void SpecialCrossHotspring::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case ACTIVITY_STATUS_STANDY:
		{
			this->OnActivityStandy();
		}
		break;

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

bool SpecialCrossHotspring::CanDestroy()
{
	return !m_isopen;
}

SpecialCrossHotspring::JoinerInfo* SpecialCrossHotspring::GetJoinerItem(long long uuid)
{
	std::map<long long, JoinerInfo*>::iterator iter = m_joiner_map.find(uuid);
	if (iter != m_joiner_map.end())
	{
		return iter->second;
	}

	return NULL;
}

void SpecialCrossHotspring::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_HOT_SPRING) || !m_is_hidden_server)
	{
		this->SyncRoleInfoToCross(role);
		return;
	}

	if (role->GetLevel() < LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg().open_level)
	{
		role->NoticeNum(errornum::EN_CROSS_HS_LEVEL_LIMIT);
		this->SyncRoleInfoToCross(role);
		return;
	}

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (joiner == NULL)
	{
		joiner = new JoinerInfo();
		joiner->uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
		joiner->unique_user_id = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
		joiner->uid = role->GetUID();
		joiner->obj_id = role->GetId();
		F_STRNCPY(joiner->name, role->GetName(), sizeof(GameName));
		joiner->camp = role->GetCamp();
		joiner->popularity = 0;
		joiner->popularity_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		joiner->partner_uuid = 0;
		joiner->partner_obj_id = INVALID_OBJ_ID;
		joiner->give_present_times = 0;
		joiner->add_exp_total = 0;

		joiner->question_score_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		joiner->question_total_score = 0;									
		joiner->question_shengwang = 0;
		joiner->question_honor = 0;
		joiner->question_gongxun = 0;
		joiner->question_weiwang = 0;
		joiner->question_right_count = 0;
		joiner->question_wrong_count = 0;
		joiner->question_curr_question_flag = 0;
		joiner->question_curr_issue_score = 0;
		joiner->question_curr_issue_answer_result = 0;

		m_joiner_map[joiner->uuid] = joiner;

		EventHandler::Instance().OnHotspringJoin(role);
	}
	else
	{
		joiner->partner_uuid = 0;
		joiner->partner_obj_id = INVALID_OBJ_ID;
		joiner->uid = role->GetUID();
		joiner->obj_id = role->GetId();
	}

	if (FEMALE == role->GetSex())
	{
		joiner->swinsuit_image = SWINSUIT_NORMAL_WOMAN;
		role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPERANCE_TYPE_CROSS_HOTSPRING, SWINSUIT_NORMAL_WOMAN, 0, true);
	}
	else
	{
		joiner->swinsuit_image = SWINSUIT_NORMAL_MAN;
		role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPERANCE_TYPE_CROSS_HOTSPRING, SWINSUIT_NORMAL_MAN, 0, true);
	}

	this->QASyncGlobalRankInfo(role);
	this->QASyncGlobalQuestionInfo(role, m_side);

	this->SendRoleInfo(role);
	this->SendRankInfo(role);

	EventHandler::Instance().OnEnterHotSpring(role);
}

void SpecialCrossHotspring::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL != joiner)
	{
		if (0 != joiner->partner_uuid)
		{
			JoinerInfo *partner_joiner = this->GetJoinerItem(joiner->partner_uuid);
			if (NULL != partner_joiner)
			{
				partner_joiner->partner_uuid = 0;
				partner_joiner->partner_obj_id = INVALID_OBJ_ID;

				Role *partner_role = m_scene->GetRoleByUUID(LongLongToUniqueUserID(joiner->partner_uuid));
				if (NULL != partner_role)
				{
					this->SendPartnerInfo(partner_role);
					partner_role->NoticeNum(errornum::EN_CROSS_HS_CANCEL_PARTNER);
					this->NotifyShuangxiuInfo(role, partner_role);
				}
			}

			joiner->partner_uuid = 0;
			joiner->partner_obj_id = INVALID_OBJ_ID;
			joiner->swinsuit_image = INVAILD_APPEARANCE;
		}

		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_JIASU, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_BIANSHEN, EBT_BIANXING_FOOL, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		joiner->question_curr_issue_answer_result = 0;
		joiner->question_curr_issue_score = 0;
	}

	role->GetSpecialAppearance()->SetFbAppearance(0, 0, 0, true);
	this->SyncRoleInfoToCross(role);	
}

int SpecialCrossHotspring::GetSpecialParam(Role *role)
{
	if (NULL == role) return INVALID_OBJ_ID;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner) return INVALID_OBJ_ID;

	return joiner->partner_obj_id;
}

void SpecialCrossHotspring::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_isopen || !m_is_hidden_server) return;

	if (m_give_exp_interval > 0 && m_next_give_exp_timestamp > 0 && now_second > m_next_give_exp_timestamp)
	{
		m_next_give_exp_timestamp = now_second + m_give_exp_interval;

		// 隔一段时间就群体加经验
		for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				this->AddExp(role);
			}
		}
	}

	if (m_next_give_item_timestamp > 0 && now_second > m_next_give_item_timestamp)
	{
		m_next_give_item_timestamp = now_second + m_give_item_interval;

		const HotspringOtherConfig &other_cfg = LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg();
		// 隔一段时间就群体发物品
		for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				if (other_cfg.item_count > 0)
				{
					role->GetKnapsack()->PutListOrMail(other_cfg.interval_reward_item, PUT_REASON_HOTSPRING_ACTIVITY);
				}
			}
		}
	}

	if (m_popularity_rank_interval_s > 0 && now_second >= m_next_calc_rank_timestamp && m_is_change)
	{
		m_is_change = false;
		m_next_calc_rank_timestamp = now_second + m_popularity_rank_interval_s;
		this->CalcPopularityRank();
	}

	if (!m_is_finish_question_activity)
	{
		if (!m_question_rank_vec.empty())
		{
			// 传闻
			static const unsigned int FIRST_INTERVAL = 60;
			if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_first_broadcast_time + FIRST_INTERVAL)
			{
				JoinerInfo *joiner = this->GetJoinerItem(m_question_rank_vec[0].uuid);
				if (NULL != joiner)
				{
					m_last_first_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hotspring_rank_first_player,
						joiner->uid, joiner->name, joiner->camp);
					if (length > 0) 
					{
						this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
					}
				}
			}
		}
	}

	if (!m_is_finish_question_activity && m_broadcast_question_total <= m_total_question_num)
	{
		if (m_next_question_begin_time > 0 && now_second >=  m_next_question_begin_time)
		{
			m_is_can_answer = true;
			++m_broadcast_question_total;
			m_cur_question_begin_time = static_cast<unsigned int>(now_second);
			m_curr_question_end_time = m_cur_question_begin_time + m_answer_continue_time;
			m_next_question_begin_time = m_curr_question_end_time + m_answer_prepare_time; 

			m_curr_question_id = LOGIC_CONFIG->GetQuestionConfig().RandQuestionID(NULL, QUESTION_TYPE_HOTSPRING);
			m_side = RandomNum(2);
			this->QASyncGlobalInfoToAll(QA_BROADCAST_QUSETION);
			this->ClearAllJoninerAnswerState();
		}
		else if (m_is_can_answer && m_cur_question_begin_time < m_curr_question_end_time && m_curr_question_end_time > 0 && now_second > m_curr_question_end_time)
		{
			m_is_can_answer = false;
			this->CalcAllJoinerAnswerResult();
			this->CalcScoreRank();
			m_curr_question_id = 0;

			if (m_broadcast_question_total == m_total_question_num)
			{
				++m_broadcast_question_total;
				m_cur_question_begin_time = 0;
				m_curr_question_end_time = 0;
				m_next_question_begin_time = 0;
				m_is_finish_question_activity = true;

				if (!m_question_rank_vec.empty())
				{
					// 传闻
					JoinerInfo *joiner = this->GetJoinerItem(m_question_rank_vec[0].uuid);
					if (NULL != joiner)
					{
						int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_question_rank_broadcast,
							joiner->uid, joiner->name, joiner->camp);
						if (length > 0)
						{
							//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
							this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
						}
					}
				}
			}	
		}
	}

	if (static_cast<unsigned int>(now_second) >= m_next_refresh_gather_time)
	{
		const HotspringOtherConfig &other_cfg = LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg();

		this->RefreshGather();

		m_next_refresh_gather_time += other_cfg.refresh_gather_interval;
	}
}

bool SpecialCrossHotspring::SpecialCanGather(Role *role, GatherObj *gather)
{
	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (nullptr != joiner)
	{
		return joiner->gather_times < LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg().max_gather_times;
	}

	return false;
}

void SpecialCrossHotspring::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	const HotspringGahterCfg *gather_cfg = LOGIC_CONFIG->GetHotspringCfg().GetGatherRewardCfg(gather_id);
	if (nullptr == gather_cfg)
	{
		return;
	}

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (nullptr != joiner)
	{
		if (gather_cfg->reward_item.item_id > 0)
		{
			if (!role->GetKnapsack()->HaveSpace())
			{
				role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
				return;
			}

			role->GetKnapsack()->Put(gather_cfg->reward_item, PUT_REASON_HOTSPRING_ACTIVITY);
		}

		//const HotspringExpRewardCfg *exp_rewad = LOGIC_CONFIG->GetHotspringCfg().GetExpCfg(role->GetLevel());
		//if (NULL != exp_rewad)
		//{
		//	long long add_exp = exp_rewad->base_exp * gather_cfg->add_times;
		//	role->AddExp(add_exp, __FUNCTION__);
		//}

		//策划需求，修改增加经验规则（2018-1-3）
		long long add_exp = static_cast<long long>(gather_cfg->exp_factor_gather * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));

		role->AddExp(add_exp, __FUNCTION__, Role::EXP_ADD_REASON_DEFAULT);

// 		if (gather_cfg->gather_exp > 0)
// 		{
// 			long long add_exp = role->RecalcExp(gather_cfg->gather_exp);
// 			if (add_exp > 0)
// 			{
// 				role->AddExp(add_exp, __FUNCTION__);
// 			}
// 		}

		++joiner->gather_times;
		this->SendRoleInfo(role);
	}
}

void SpecialCrossHotspring::OnAOIRoleEnter(Role *role, Role *target_role)
{
	if (NULL == role || NULL == target_role)
	{
		return;
	}

 	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
 	if (NULL != joiner && ((long long)0 != joiner->partner_uuid))
 	{
 		JoinerInfo *partner_joiner = this->GetJoinerItem(joiner->partner_uuid);
 		if (NULL != partner_joiner)
 		{
 			Role *partner_role = m_scene->GetRoleByUUID(LongLongToUniqueUserID(joiner->partner_uuid));
 			if (NULL != partner_role)
 			{
 				// 通知新进入视野角色正在双修
 				Protocol::SCHSShuangxiuInfo cmd;
 				cmd.role_1_obj_id = role->GetId();
 				cmd.role_1_partner_obj_id = joiner->partner_obj_id;
 				cmd.role_2_obj_id = partner_role->GetId();
 				cmd.role_2_partner_obj_id = partner_joiner->partner_obj_id;
 				EngineAdapter::Instance().NetSend(target_role->GetNetId(), (char*)&cmd, sizeof(cmd));
 
 				// 判断技能类型
 				//this->NotifySkillInfo(role, target_role, HOTSPRING_SKILL_MASSAGE);
 			}
 		}
 	}
}

void SpecialCrossHotspring::OnGivePresentReq(Role *role, Protocol::CSHotspringGivePresent *cmd)
{
	if (NULL == cmd || NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner) return;

	JoinerInfo *targe_joiner = NULL;
	Role *target_role = NULL;
	if (cmd->is_uid)
	{
		target_role = m_scene->GetRoleByUID((int)(cmd->target_uuid));
		if (NULL == target_role) return;
		
		targe_joiner = this->GetJoinerItem(UniqueUserIDToLongLong(target_role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	}
	else
	{	
		targe_joiner = this->GetJoinerItem(cmd->target_uuid);
		target_role = m_scene->GetRoleByUUID(LongLongToUniqueUserID(cmd->target_uuid));
	}

	if (NULL == targe_joiner)
	{
		role->NoticeNum(errornum::EN_CROSS_HS_NOT_IN_SCENES);
		return;
	}

	const HotspringPresentCfg * present_cfg = LOGIC_CONFIG->GetHotspringCfg().GetPresentCfg(cmd->present_id);
	if (NULL == present_cfg) return;

	const HotspringOtherConfig &other_cf = LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg();

	if(other_cf.free_present_id == cmd->present_id)
	{
		if (joiner->give_present_times >= other_cf.free_give_present_times)
		{
			if (!cmd->is_use_gold)
			{
				role->NoticeNum(errornum::EN_CROSS_HS_FREE_PRESETN_TIME_LIMIT);
				return;
			}
			else
			{
				if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(present_cfg->present_price))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}

				targe_joiner->popularity += present_cfg->add_popularity;
				targe_joiner->popularity_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_is_change = true;

				if (NULL != target_role)
				{
					this->SendRoleInfo(target_role);
				}
			
				role->GetKnapsack()->GetMoney()->UseGold(present_cfg->present_price, "HotSpring",false);
				role->NoticeNum(noticenum::NT_CROSS_HS_GIVE_PRESENT_SUCCESS);
				return;
			}
		}
		else
		{
			targe_joiner->popularity += present_cfg->add_popularity;
			targe_joiner->popularity_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			joiner->give_present_times++;
			m_is_change = true;
			role->NoticeNum(noticenum::NT_CROSS_HS_GIVE_PRESENT_SUCCESS);
			this->SendRoleInfo(role);
			
			if (NULL != target_role)
			{
				this->SendRoleInfo(target_role);
			}
			
			return;
		}
	}
	else
	{
		if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(present_cfg->present_price))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		targe_joiner->popularity += present_cfg->add_popularity;
		targe_joiner->popularity_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_is_change = true;

		if (NULL != target_role)
		{
			this->SendRoleInfo(target_role);
		}
		
		role->GetKnapsack()->GetMoney()->UseGold(present_cfg->present_price, "HotSpring",false);
		role->NoticeNum(noticenum::NT_CROSS_HS_GIVE_PRESENT_SUCCESS);
		return;
	}
}

void SpecialCrossHotspring::OnAddPartner(Role *role, Protocol::CSHSAddPartnerReq *cmd)
{
	if (nullptr == role || nullptr == cmd) return;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner) return;

	if (0 != joiner->partner_uuid)
	{
		role->NoticeNum(errornum::EN_CROSS_HS_HAS_PARTNER);
		return;
	}
	
	Obj* target_obj = m_scene->GetObj(cmd->obj_id);
	if (NULL == target_obj)
	{
		role->NoticeNum(errornum::EN_CROSS_HS_NOT_IN_SCENES);
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != target_obj->GetObjType()) return;
	Role *target_role = (Role*)target_obj;

	UniqueUserID target_user_id = target_role->GetRoleCross()->GetCrossOriginalUniqueUserID();
							
	if (target_user_id == role->GetUniqueUserID()) return;

	if (role->GetFriendManager()->IsBlack(target_user_id.user_id)) // 邀请者在自己的黑名单中
	{
		role->NoticeNum(errornum::EN_IN_YOUR_BLACKLISTS);
		return;
	}

	JoinerInfo *target_joiner = this->GetJoinerItem(UniqueUserIDToLongLong(target_role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == target_joiner)
	{
		role->NoticeNum(errornum::EN_CROSS_HS_NOT_IN_SCENES);
		return;
	}

	if (0 != target_joiner->partner_uuid) // 被邀请者正在幽会中
	{
		role->NoticeNum(errornum::EN_CROSS_HS_TARGET_HAS_PARTNER);
		return;
	}

	// 按摩中不能幽会
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (now_sec <= joiner->skill_1_can_perform_time)
	{
		role->NoticeNum(errornum::EN_ROLE_MASSAGE_CD_NOT_ENOUGH);
		return;
	}

	if (now_sec <= target_joiner->skill_1_can_perform_time)
	{
		role->NoticeNum(errornum::EN_TATGET_ROLE_MASSAGE_CD_NOT_ENOUGH);
		return;
	}

	if (cmd->is_yi_jian)
	{
		joiner->partner_uuid = target_joiner->uuid;
		joiner->partner_obj_id = target_role->GetId();
		role->NoticeNum(noticenum::NT_CROSS_HS_ADD_PARTNER_SUCCESS);
		this->SendPartnerInfo(role);

		target_joiner->partner_uuid = joiner->uuid;
		target_joiner->partner_obj_id = role->GetId();
		target_role->NoticeNum(noticenum::NT_CROSS_HS_ADD_PARTNER_SUCCESS);
		this->SendPartnerInfo(target_role);

		Protocol::SCHSAddExpInfo cmdx;

		int interaction_add =  LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg().interaction_add;
		// 通知发起者经验加成改变
		cmdx.add_exp_total = joiner->add_exp_total;
		cmdx.add_addition = (int)((interaction_add + LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FB_HOTSPRINT_EXTRA_REWARD)) * ROLE_ATTR_PER);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cmdx, sizeof(Protocol::SCHSAddExpInfo));

		// 通知被结伴者经验加成改变
		cmdx.add_exp_total = target_joiner->add_exp_total;
		cmdx.add_addition = (int)((interaction_add + LOGIC_CONFIG->GetVipConfig().GetAuthParam(target_role->GetVip()->GetVipLevel(), VAT_FB_HOTSPRINT_EXTRA_REWARD)) * ROLE_ATTR_PER);
		EngineAdapter::Instance().NetSend(target_role->GetNetId(), (char*)&cmdx, sizeof(Protocol::SCHSAddExpInfo));

		// 传闻
		static const unsigned int LOVER_INTERVAL = 10;
		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_lover_broadcast_time + LOVER_INTERVAL)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hotspring_find_lover,
				role->GetUID(), role->GetName(), (int)role->GetCamp(),
				target_role->GetUID(), target_role->GetName(), (int)target_role->GetCamp());
			if (length > 0) 
			{
				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}

			m_last_lover_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}
		
		this->NotifyShuangxiuInfo(role, target_role);
		return;
	}
	else
	{
		// 传闻
		static const unsigned int LOVER_INTERVAL = 10;
		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_lover_broadcast_time + LOVER_INTERVAL)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hotspring_find_lover,
				role->GetUID(), role->GetName(), (int)role->GetCamp(),
				target_role->GetUID(), target_role->GetName(), (int)target_role->GetCamp());
			if (length > 0) 
			{
				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}

			m_last_lover_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}

		Protocol::SCHSAddPartnerReqRoute af_route;
		role->GetName(af_route.req_gamename);
		af_route.req_user_id = UniqueUserIDToLongLong(role->GetUniqueUserID());
		af_route.req_avatar = role->GetAvatar();
		af_route.req_sex = role->GetSex();
		af_route.req_prof = role->GetProf();
		af_route.req_camp = role->GetCamp();
		af_route.req_level = role->GetLevel();
		af_route.req_avatar_timestamp = role->GetAvatarTimeStamp();
		EngineAdapter::Instance().NetSend(target_role->GetNetId(), (char*)&af_route, sizeof(Protocol::SCHSAddPartnerReqRoute));
	}
}

void SpecialCrossHotspring::OnAddPartnerRet(Role *role,Protocol::CSHSAddPartnerRet *cmd)
{
	if (NULL == cmd || NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (0 != joiner->partner_uuid)
	{
		role->NoticeNum(errornum::EN_CROSS_HS_HAS_PARTNER);
		return;
	}

	// 按摩中不能幽会
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (now_sec <= joiner->skill_1_can_perform_time)
	{
		role->NoticeNum(errornum::EN_ROLE_MASSAGE_CD_NOT_ENOUGH);
		return;
	}

	JoinerInfo *req_joiner = this->GetJoinerItem(cmd->req_user_id);
	if (NULL == req_joiner) return;

	Role *req_role = m_scene->GetRoleByUUID(LongLongToUniqueUserID(cmd->req_user_id));
	if (NULL == req_role) return;
	
	if (Protocol::ADDPARTNER_AGREE == cmd->is_accept)
	{
		if (0 != req_joiner->partner_uuid)
		{
			role->NoticeNum(errornum::EN_CROSS_HS_TARGET_HAS_PARTNER);
			return;
		}

		// 按摩中不能幽会
		if (now_sec <= req_joiner->skill_1_can_perform_time)
		{
			role->NoticeNum(errornum::EN_TATGET_ROLE_MASSAGE_CD_NOT_ENOUGH);
			return;
		}

		joiner->partner_uuid = req_joiner->uuid;
		joiner->partner_obj_id = req_role->GetId();
		role->NoticeNum(noticenum::NT_CROSS_HS_ADD_PARTNER_SUCCESS);
		this->SendPartnerInfo(role);

		req_joiner->partner_uuid = joiner->uuid;
		req_joiner->partner_obj_id = role->GetId();
		req_role->NoticeNum(noticenum::NT_CROSS_HS_ADD_PARTNER_SUCCESS);

		this->SendPartnerInfo(req_role);
		this->NotifyShuangxiuInfo(role, req_role);

		Protocol::SCHSAddExpInfo cmd;

		int interaction_add =  LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg().interaction_add;

		// 通知接受者经验加成改变信息
		cmd.add_exp_total = joiner->add_exp_total;
		cmd.add_addition = (int)((interaction_add + LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FB_HOTSPRINT_EXTRA_REWARD)) * ROLE_ATTR_PER);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cmd, sizeof(Protocol::SCHSAddExpInfo));

		// 通知发起者经验加成改变信息
		cmd.add_exp_total = req_joiner->add_exp_total;
		cmd.add_addition = (int)((interaction_add + LOGIC_CONFIG->GetVipConfig().GetAuthParam(req_role->GetVip()->GetVipLevel(), VAT_FB_HOTSPRINT_EXTRA_REWARD)) * ROLE_ATTR_PER);
		EngineAdapter::Instance().NetSend(req_role->GetNetId(), (char*)&cmd, sizeof(Protocol::SCHSAddExpInfo));
		return;
	}
	else
	{
		req_role->NoticeNum(errornum::EN_CROSS_HS_REJECT_PARTNER_INVITE);
		return;
	}
}
 
void SpecialCrossHotspring::OnDeletePartnerReq(Role *role)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner) return;

	if (0 != joiner->partner_uuid)
	{
		JoinerInfo *partner_joiner = this->GetJoinerItem(joiner->partner_uuid);
		if (NULL != partner_joiner)
		{
			Role *partner_role = m_scene->GetRoleByUUID(LongLongToUniqueUserID(joiner->partner_uuid));
			if (NULL != partner_role)
			{
				partner_joiner->partner_uuid = 0;
				partner_joiner->partner_obj_id = INVALID_OBJ_ID;
				this->SendPartnerInfo(partner_role);
				partner_role->NoticeNum(errornum::EN_CROSS_HS_CANCEL_PARTNER);

				Protocol::SCHSAddExpInfo cmd;
				cmd.add_exp_total = joiner->add_exp_total;
				cmd.add_addition = (int)(LOGIC_CONFIG->GetVipConfig().GetAuthParam(partner_role->GetVip()->GetVipLevel(), VAT_FB_HOTSPRINT_EXTRA_REWARD) * ROLE_ATTR_PER);
				EngineAdapter::Instance().NetSend(partner_role->GetNetId(), (char*)&cmd, sizeof(Protocol::SCHSAddExpInfo));

				joiner->partner_uuid = 0;
				joiner->partner_obj_id = INVALID_OBJ_ID;
				this->SendPartnerInfo(role);
				role->NoticeNum(noticenum::NT_CROSS_HS_CANCEL_PARTNER_SUCCESS);

				cmd.add_exp_total = joiner->add_exp_total;
				cmd.add_addition = (int)(LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FB_HOTSPRINT_EXTRA_REWARD) * ROLE_ATTR_PER);
				EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cmd, sizeof(Protocol::SCHSAddExpInfo));

				this->NotifyShuangxiuInfo(role, partner_role);
			}
		}
	}
}

void SpecialCrossHotspring::SendRoleInfo(Role *role)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL != joiner)
	{
		static Protocol::SCHotspringPlayerInfo cmd;
		cmd.popularity = joiner->popularity;
		cmd.partner_uuid = joiner->partner_uuid;
		cmd.give_free_present_time = joiner->give_present_times;
		cmd.swinsuit_image = joiner->swinsuit_image;
		cmd.self_uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
		cmd.partner_obj_id = joiner->partner_obj_id;

		cmd.question_right_count = joiner->question_right_count;
		cmd.question_wrong_count = joiner->question_wrong_count;
		cmd.curr_score = joiner->question_total_score;
		cmd.total_exp = joiner->add_exp_total;

		cmd.skill_use_times_1 = joiner->skill_use_times_1;
		cmd.skill_use_times_2 = joiner->skill_use_times_2;
		cmd.skill_1_can_perform_time = joiner->skill_1_can_perform_time;
		cmd.skill_2_can_perform_time = joiner->skill_2_can_perform_time;

		cmd.gather_times = joiner->gather_times;
		cmd.reserve_sh = 0;
		cmd.start_fresh_gather_time = m_start_refresh_gather_time;

		EngineAdapter::Instance().NetSend(role->GetNetId(),(char*)&cmd,sizeof(cmd));
	}
}

void SpecialCrossHotspring::SendRankInfo(Role *role)
{
	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner || NULL == role)
	{
		return;
	}

	static Protocol::SCHotspringRankInfo rank_info;
	rank_info.is_open = m_isopen? 1:0;
	rank_info.self_rank = 0;
	rank_info.self_popularity = joiner->popularity;
	rank_info.rank_count = 0;
	rank_info.reserve_1 = 0;
	rank_info.reserve_2 = 0;

	int joiner_size = static_cast<int>(m_popularity_rank_vec.size());
	for (int i = 0; i < Protocol::SCHotspringRankInfo::RANK_LIST && i < joiner_size; ++ i)
	{
		Protocol::SCHotspringRankInfo::RankList &rank_item = rank_info.rank_list[i];

		RankItem joiner_item = m_popularity_rank_vec[i];

		rank_item.uuid = joiner_item.uuid;
		rank_item.uid = joiner_item.uid;
		rank_item.popularity = joiner_item.popularity;
		F_STRNCPY(rank_item.name ,joiner_item.name,sizeof(GameName));

		++rank_info.rank_count;

		if (joiner_item.uuid == UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()))
		{
			rank_info.self_rank = rank_info.rank_count;
		}
	}

	unsigned int senlen = sizeof(rank_info) - sizeof(rank_info.rank_list[0]) * (Protocol::SCHotspringRankInfo::RANK_LIST - rank_info.rank_count);

	if (senlen > 0)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(),(const char*)&rank_info,senlen);
	}
}

void SpecialCrossHotspring::SyncRankInfoToAll()
{
	for(int i = 0; i < (int)m_scene->RoleNum(); i++)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);

		if (NULL != temp_role)
		{
			this->SendRankInfo(temp_role);
		}
	}
}

void SpecialCrossHotspring::SendPartnerInfo(Role *role)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner) return;
	
	Protocol::SCHSSendPartnerInfo afrj;
	JoinerInfo *partner_joiner = this->GetJoinerItem(joiner->partner_uuid);
	if (NULL != partner_joiner)
	{
		afrj.partner_uuid = joiner->partner_uuid;
		afrj.partner_obj_id = joiner->partner_obj_id;
	}
	else
	{
		joiner->partner_uuid = 0;
		joiner->partner_obj_id = INVALID_OBJ_ID;
		afrj.partner_uuid = joiner->partner_uuid;
		afrj.partner_obj_id = joiner->partner_obj_id;
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&afrj, sizeof(Protocol::SCHSSendPartnerInfo));

}

void SpecialCrossHotspring::NotifyShuangxiuInfo(Role *role_1, Role *role_2)
{
	if (NULL == role_1 || NULL == role_2) return;

	JoinerInfo *joiner_1 = this->GetJoinerItem(UniqueUserIDToLongLong(role_1->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner_1) return;

	JoinerInfo *joiner_2 = this->GetJoinerItem(UniqueUserIDToLongLong(role_2->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner_2) return;

	Protocol::SCHSShuangxiuInfo cmd;
	cmd.role_1_obj_id = role_1->GetId();
	cmd.role_1_partner_obj_id = joiner_1->partner_obj_id;
	cmd.role_2_obj_id = role_2->GetId();
	cmd.role_2_partner_obj_id = joiner_2->partner_obj_id;
	
	role_1->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role_1->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	role_2->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role_2->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

void SpecialCrossHotspring::QASyncGlobalInfoToAll(SpecialCrossHotspring::QASyncGlobalInfoType type)
{
	if (QA_BROADCAST_INVALID >= type || QA_BROADCAST_MAX <= type)
		return;

	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (NULL != temp_role)
		{
			switch(type)
			{
			case QA_BROADCAST_QUSETION:
				{
					this->QASyncGlobalQuestionInfo(temp_role,m_side);
				}
				break;

			case QA_BROADCAST_RANK:
				{
					this->QASyncGlobalRankInfo(temp_role);
				}
				break;

			default:
				break;
			}
		}
	}
}

void SpecialCrossHotspring::QASyncGlobalQuestionInfo(Role *role,int side)
{
	if (m_broadcast_question_total > LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg()->total_question_count)
	{
		return;
	}

	Protocol::SCHSQAQuestionBroadcast question_info;
	question_info.curr_question_begin_time = m_cur_question_begin_time;
	question_info.curr_question_end_time = m_curr_question_end_time;
	question_info.next_question_begin_time = m_next_question_begin_time;
	question_info.broadcast_question_total = m_broadcast_question_total;
	question_info.curr_question_id = 0;
	question_info.side = 0;

	if (m_curr_question_id > 0)
	{
		question_info.curr_question_id = m_curr_question_id;
		question_info.side = side;
		LOGIC_CONFIG->GetQuestionConfig().GetQuestionStr(m_curr_question_id, question_info.curr_question_str, QUESTION_TYPE_HOTSPRING);
		LOGIC_CONFIG->GetQuestionConfig().GetAnswer0DescStr(m_curr_question_id, question_info.curr_answer0_desc_str, QUESTION_TYPE_HOTSPRING);
		LOGIC_CONFIG->GetQuestionConfig().GetAnswer1DescStr(m_curr_question_id, question_info.curr_answer1_desc_str, QUESTION_TYPE_HOTSPRING);
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&question_info, sizeof(question_info));
}

void SpecialCrossHotspring::QASyncGlobalRankInfo(Role *role)
{
	if (NULL == role || m_broadcast_question_total <= 0) return;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner) return;
	
	static Protocol::SCHSQARankInfo rank_info;
	rank_info.self_score = joiner->question_total_score;
	rank_info.self_rank = 0;
	rank_info.is_finish = m_is_finish_question_activity ? 1 : 0;
	rank_info.reserve_1 = 0;
	rank_info.reserve_2 = 0;
	rank_info.rank_count = 0;

	int joiner_size = static_cast<int>(m_question_rank_vec.size());
	for (int i = 0; i < Protocol::SCHSQARankInfo::RANK_NUM && i < joiner_size; ++ i)
	{
		Protocol::SCHSQARankInfo::RankItem &rank_item = rank_info.rank_list[i];

		QuestiongRankItem  joiner_item = m_question_rank_vec[i];
		rank_item.uuid = joiner_item.uuid;
		rank_item.uid = joiner_item.uid;
		rank_item.score = joiner_item.score;
		F_STRNCPY(rank_item.name,joiner_item.name, sizeof(GameName));

		++rank_info.rank_count;

		if (joiner_item.uuid == UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()))
		{
			rank_info.self_rank = rank_info.rank_count;
		}
	}

	unsigned int sendlen = 0;
	if (rank_info.rank_count < 10)
	{
		sendlen = sizeof(rank_info) - sizeof(rank_info.rank_list[0]) * (Protocol::SCHSQARankInfo::RANK_NUM - rank_info.rank_count);
	}
	else
	{
		rank_info.rank_count = 10;
		sendlen = sizeof(rank_info) - sizeof(rank_info.rank_list[0]) * (Protocol::SCHSQARankInfo::RANK_NUM - 10);
	}

	if (sendlen > 0)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&rank_info, sendlen);
	}
}

void SpecialCrossHotspring::QARoleAnswerReq(Role *role,int choose, bool is_use_item)
{
	if (m_is_finish_question_activity || role == nullptr) return;

	if (!m_is_can_answer || m_curr_question_id <= 0)
	{
		if (is_use_item)
		{
			role->NoticeNum(errornum::EN_QUESTION_USE_DATI_CARD_LIMIT);
		}
		return;
	}

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner) return;

	if (joiner->question_curr_question_flag == 1)
	{
		if (is_use_item)
		{
			role->NoticeNum(errornum::EN_QUESTION_HAS_ANSER);
		}
		return;
	}

	if (joiner->question_right_count + joiner->question_wrong_count >= m_total_question_num)
	{
		return;
	}

	if (choose == 2)
	{
		joiner->question_curr_issue_score = 0;
		joiner->question_curr_issue_answer_result = 0;
	}
	else
	{
		//int add_exp = 0;
		int score = 0;

		if (!is_use_item)
		{
			int answer = LOGIC_CONFIG->GetQuestionConfig().GetAnswer(m_curr_question_id, QUESTION_TYPE_HOTSPRING);
			int use_time = static_cast<int>((unsigned int)EngineAdapter::Instance().Time() - m_cur_question_begin_time);
			score = LOGIC_CONFIG->GetQuestionConfig().HotSpringCalcScore(use_time);

			if (answer != choose)
			{
				joiner->question_curr_issue_score = 0;
				joiner->question_curr_issue_answer_result = 0;
			}
			else
			{
				joiner->question_curr_issue_score = score;
				joiner->question_curr_issue_answer_result = 1;
				EventHandler::Instance().OnHotspringAnswerRight(role);
			}
		}
		else
		{
			if (role->GetSublock()->CheckHasLock())
			{
				return;
			}

			if (role->GetKnapsack()->IsLock())
			{
				role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
				return;
			}

			static ItemExtern::ItemConsumeList consume_list; 
			consume_list.Reset();

			// 检查免答卡
			const QuestionOtherCfg *other_cf = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
			stuff_list[0].item_id = other_cf->mianda_card_id;
			stuff_list[0].num = 1;
			stuff_list[0].buyable = false;

			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
			{
				return ;
			}

			const HSQuestionRewardCfg *reward_cfg = LOGIC_CONFIG->GetQuestionConfig().GetHotSpringRewardCfg(role->GetLevel());
			if (NULL == reward_cfg) return;

			// 消耗
			if (!ItemExtern::ConsumeItem(role, consume_list, "UseMianDaCard"))
			{
				return;
			}

			score = 10;
			//add_exp = reward_cfg->exp;
			//策划需求，修改增加经验规则（2018-1-3）
			long long add_exp = 0;
			const QuestionOtherCfg *other_cfg = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
			if (other_cfg != NULL)
			{
				add_exp = static_cast<long long>(other_cfg->right_hotspring_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));
			}
			
			++joiner->question_right_count;
			joiner->question_total_score += score;
			joiner->question_score_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			joiner->question_curr_issue_answer_result = 1;
			//joiner->add_exp_total += static_cast<int>(add_exp);
			joiner->add_exp_total += add_exp;
			joiner->question_curr_question_flag = 1;

			role->AddExp(add_exp, "HotSpringQuestionActivity", Role::EXP_ADD_REASON_DEFAULT);
			role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item, PUT_REASON_HOTSPRING_ACTIVITY);
			role->NoticeNum(noticenum::NT_QUESTION_USE_MIANDA_CAR_SUCC);
			this->SendRoleInfo(role);
			EventHandler::Instance().OnHotspringAnswerRight(role);

			gamelog::g_log_question.printf(LL_INFO, "BatchAnswerQuestion user[%d, %s], level[%d], question_num[%d],add_exp[%lld]",
				role->GetUID(), role->GetName(), role->GetLevel(), joiner->question_right_count + joiner->question_wrong_count, add_exp);
		}
	}
}

void SpecialCrossHotspring::QAFllowRankFirst(Role *role)
{
	if (m_is_finish_question_activity) return;

	if (m_question_rank_vec.empty())
	{
		role->NoticeNum(errornum::EN_NOT_QUESTION_RANK);
		return;
	}

	Role *first_role = m_scene->GetRoleByUID(m_question_rank_vec[0].uid);
	if (NULL == first_role)
	{
		role->NoticeNum(errornum::EN_QUESTION_TARGE_NOT_IN_CUR_SCENE);
		return;
	}
	else
	{
		Protocol::SCHSQASendFirstPos rswi;
		rswi.obj_id = first_role->GetId();
		rswi.reserve = 0;
		rswi.pos_x = first_role->GetPos().x;
		rswi.pos_y = first_role->GetPos().y;

		Scene *scene = first_role->GetScene();
		if (NULL == scene) return;

		if (scene->GetSceneID() != LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg().sceneid)
		{
			role->NoticeNum(errornum::EN_QUESTION_TARGE_NOT_IN_CUR_SCENE);
			return;
		}

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rswi, sizeof(rswi));
	}
}

void SpecialCrossHotspring::OnUseBianshenCard(Role *user, Role *target)
{
	if (NULL == user || NULL == target || m_is_finish_question_activity)
	{
		return;
	}

	if (user->GetSublock()->CheckHasLock())
	{
		return;
	}

	if (user->GetKnapsack()->IsLock())
	{
		user->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	JoinerInfo *joiner_target = this->GetJoinerItem(UniqueUserIDToLongLong(target->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner_target) return;

	JoinerInfo *joiner_user = this->GetJoinerItem(UniqueUserIDToLongLong(user->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner_user) return;
	
	Role *player_user = m_scene->GetRoleByUID(user->GetUID());
	Role *player_target = m_scene->GetRoleByUID(target->GetUID());
	if (NULL == player_user || NULL == player_user)
	{
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	// 检查变身卡
	const QuestionOtherCfg *other_cf = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
	if (NULL == other_cf) return;

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = other_cf->bianshen_card_id;
	stuff_list[0].num = 1;
	stuff_list[0].buyable = false;

	if (!ItemExtern::GetConsumeListByOrder(user, false, 1, stuff_list, &consume_list, true))
	{
		return ;
	}	

	// 消耗
	if (!ItemExtern::ConsumeItem(user, consume_list, "OnUseBianshenCard"))
	{
		return;
	}

	int bianshen_probability = RandomNum(100);

	// 先驱除原有的变身效果
	target->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_JIASU, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
	target->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL);

	// 变身小兔，加速
	if (bianshen_probability < other_cf->rabbit_pro)
	{
		// 目标加速
		EffectMoveSpeed *effect_speed = new EffectMoveSpeed(user->GetId(), 1, other_cf->bianshen_continue_time, EBT_JIASU, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
		int value = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * (other_cf->rabbit_speed_up_per * ROLE_ATTR_PER));
		effect_speed->SetMoveSpeed(value, other_cf->rabbit_speed_up_per / 100);
		target->AddEffect(effect_speed);

		player_target->NoticeNum(errornum::EN_QUESTION_OTHER_USE_BIANSHEN_CARD_ON_YOU);

		//目标变身
		EffectBianshen *effect_bianshen = new EffectBianshen(user->GetId(), 1, other_cf->bianshen_continue_time, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		effect_bianshen->SetImage(APPEARANCE_DATI_RABBIT);
		effect_bianshen->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
		effect_bianshen->SetClientEffectType(EffectBase::ECT_SKILL_BIANSHEN);
		target->AddEffect(effect_bianshen);

		// 查找BUFF是否添加成功
		EffectBase *add_buff = target->GetEffectList()->FindEffect(EffectBase::EFFECT_TYPE_BIANSHEN, EBT_BIANXING_FOOL, user->GetId());
		if (nullptr != add_buff)
		{
			// 广播形象
			static Protocol::SCBianShenView cmd;
			cmd.obj_id = player_target->GetId();
			cmd.bianshen_image = APPEARANCE_DATI_RABBIT;
			player_target->GetScene()->GetZoneMatrix()->NotifyAreaMsg(player_target->GetObserHandle(), (void*)&cmd, sizeof(cmd));
		}
	}
	// 变身小猪，减速
	else
	{
		// 目标减速
		EffectMoveSpeed *effect_speed = new EffectMoveSpeed(user->GetId(), 2, other_cf->bianshen_continue_time, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
		int value = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * (other_cf->pig_slow_down_per * ROLE_ATTR_PER));
		effect_speed->SetMoveSpeed(-value, -other_cf->pig_slow_down_per / 100);
		target->AddEffect(effect_speed);

		player_target->NoticeNum(errornum::EN_QUESTION_OTHER_USE_BIANSHEN_CARD_ON_YOU);

		//目标变身
		EffectBianshen *effect_bianshen = new EffectBianshen(user->GetId(),1, other_cf->bianshen_continue_time, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		effect_bianshen->SetImage(APPEARANCE_DATI_PIG);
		effect_bianshen->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
		effect_bianshen->SetClientEffectType(EffectBase::ECT_SKILL_BIANSHEN);
		target->AddEffect(effect_bianshen);

		// 查找BUFF是否添加成功
		EffectBase *add_buff = target->GetEffectList()->FindEffect(EffectBase::EFFECT_TYPE_BIANSHEN, EBT_BIANXING_FOOL, user->GetId());
		if (nullptr != add_buff)
		{
			// 广播形象
			static Protocol::SCBianShenView cmd;
			cmd.obj_id = player_target->GetId();
			cmd.bianshen_image = APPEARANCE_DATI_PIG;
			player_target->GetScene()->GetZoneMatrix()->NotifyAreaMsg(player_target->GetObserHandle(), (void*)&cmd, sizeof(cmd));
		}
	}

	user->NoticeNum(noticenum::NT_QUESTION_USE_BIANSHEN_CAR_SUCC);
}

void SpecialCrossHotspring::OnUseSkillReq(Role *role, ObjID obj_id, short skill_type)
{
	if (NULL == role) return;

	if (skill_type <= HOTSPRING_SKILL_INVALID || skill_type >= HOTSPRING_SKILL_MAX) return;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner) return;

	Obj *target_obj = m_scene->GetObj(obj_id);
	if (NULL == target_obj || Obj::OBJ_TYPE_ROLE != target_obj->GetObjType())
	{
		role->NoticeNum(errornum::EN_CROSS_HS_NOT_IN_SCENES);
		return;
	}

	Role *target_role = (Role *)(target_obj);

	JoinerInfo *target_joiner = this->GetJoinerItem(UniqueUserIDToLongLong(target_role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == target_joiner) return;

	const HotspringSkillCfg *skill_cfg = LOGIC_CONFIG->GetHotspringCfg().GetSkillCfgBySkillType(skill_type);
	if (NULL == skill_cfg) return;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	switch(skill_type)
	{
	case HOTSPRING_SKILL_MASSAGE:
		{
			if (0 != joiner->partner_uuid)
			{
				role->NoticeNum(errornum::EN_CROSS_HS_SHUANGXIU_CANNOT_MASSAGE);
				return;
			}

			// 对方正在幽会，使用技能失败
			if (0 != target_joiner->partner_uuid)
			{
				role->NoticeNum(errornum::EN_CROSS_HS_TARGET_SHUANGXIU_CANNOT_MASSAGE);
				return;
			}

			if (joiner->skill_use_times_1 >= skill_cfg->use_times_limit)
			{
				role->NoticeNum(errornum::EN_CROSS_HS_SKILL_TIME_LIMIT);
				return;
			}

			if (now_sec <= joiner->skill_1_can_perform_time)
			{
				role->NoticeNum(errornum::EN_ROLE_MASSAGE_CD_NOT_ENOUGH);
				return;
			}

			// 对方正在按摩，冷却状态下不能再次被按摩
			if (now_sec <= target_joiner->skill_1_can_perform_time)
			{
				role->NoticeNum(errornum::EN_TATGET_ROLE_MASSAGE_CD_NOT_ENOUGH);
				return;
			}

			if ((target_obj->GetPos() - role->GetPos()).IsLongerThan(skill_cfg->skill_distance + 2))
			{
				role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
			}

			// 加经验
			const HotspringExpRewardCfg *exp_rewad = LOGIC_CONFIG->GetHotspringCfg().GetExpCfg(role->GetLevel());
			if (NULL != exp_rewad)
			{
				//long long use_add_exp = static_cast<long long>(exp_rewad->base_exp * skill_cfg->use_add_times);
				//策划需求，修改增加经验规则（2018-1-3）
				long long use_add_exp = static_cast<long long>(skill_cfg->exp_factor_use * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));
				role->AddExp(use_add_exp, "HotSpringSkill", Role::EXP_ADD_REASON_DEFAULT);

				//long long be_use_add_exp = static_cast<long long>(exp_rewad->base_exp * skill_cfg->be_use_add_times);
				//策划需求，修改增加经验规则（2018-1-3）
				long long be_use_add_exp = static_cast<long long>(skill_cfg->exp_factor_be_use * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(target_role->GetLevel(), 0)));
				target_role->AddExp(be_use_add_exp, "HotSpringSkill", Role::EXP_ADD_REASON_DEFAULT);
			}

			joiner->skill_use_times_1++;
			joiner->skill_1_can_perform_time = now_sec + skill_cfg->cold_down_time;
			target_joiner->skill_1_can_perform_time = now_sec + skill_cfg->cold_down_time;

			this->NotifySkillInfo(role, target_role, HOTSPRING_SKILL_MASSAGE);
			this->SendRoleInfo(role);

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hotspring_use_massage,
				role->GetUID(), role->GetName(), (int)role->GetCamp(),
				target_role->GetUID(), target_role->GetName(), (int)target_role->GetCamp());
			if (length > 0)
			{
				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}
		}
		break;

	case HOTSPRING_SKILL_THROW_SNOWBALL:
		{
			if (joiner->skill_use_times_2 >= skill_cfg->use_times_limit)
			{
				role->NoticeNum(errornum::EN_CROSS_HS_SKILL_TIME_LIMIT);
				return;
			}

			if (now_sec <= joiner->skill_2_can_perform_time)
			{
				role->NoticeNum(errornum::EN_ROLE_FIGHT_CD_NOT_ENOUGH);
				return;
			}

			if ((target_obj->GetPos() - role->GetPos()).IsLongerThan(skill_cfg->skill_distance + 2))
			{
				role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
			}

			// 加经验
			const HotspringExpRewardCfg *exp_rewad = LOGIC_CONFIG->GetHotspringCfg().GetExpCfg(role->GetLevel());
			if (NULL != exp_rewad)
			{
				//long long use_add_exp = static_cast<long long>(exp_rewad->base_exp * skill_cfg->use_add_times);
				//策划需求，修改增加经验规则（2018-1-3）
				long long use_add_exp = static_cast<long long>(skill_cfg->exp_factor_use * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));
				role->AddExp(use_add_exp, "HotSpringSkill", Role::EXP_ADD_REASON_DEFAULT);

				//long long be_use_add_exp = static_cast<long long>(exp_rewad->base_exp * skill_cfg->be_use_add_times);
				//策划需求，修改增加经验规则（2018-1-3）
				long long be_use_add_exp = static_cast<long long>(skill_cfg->exp_factor_be_use * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(target_role->GetLevel(), 0)));
				target_role->AddExp(be_use_add_exp, "HotSpringSkill", Role::EXP_ADD_REASON_DEFAULT);
			}

			joiner->skill_use_times_2++;
			joiner->skill_2_can_perform_time = now_sec + skill_cfg->cold_down_time;

			EffectSpecialState *effect_xuanyun = new EffectSpecialState(role->GetId(), skill_cfg->skill_type, skill_cfg->buff_effect_time, EBT_XUANYUN, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			effect_xuanyun->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
			target_role->AddEffect(effect_xuanyun);

			this->NotifySkillInfo(role, target_role, HOTSPRING_SKILL_THROW_SNOWBALL);
			this->SendRoleInfo(role);

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hotspring_use_snow,
				role->GetUID(), role->GetName(), (int)role->GetCamp(),
				target_role->GetUID(), target_role->GetName(), (int)target_role->GetCamp());
			if (length > 0)
			{
				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}
		}
		break;

	default:
		return;
	}
}

void SpecialCrossHotspring::NotifySkillInfo(Role* use_role, Role* be_use_role, short skill_type)
{
	if (NULL == use_role || NULL == be_use_role) return;

	Protocol::SCHSNoticeSkillInfo cmd;
	cmd.use_obj_id = use_role->GetId();
	cmd.be_use_obj_id = be_use_role->GetId();
	cmd.skill_type = skill_type;
	cmd.reserve = 0;

	use_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(use_role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	be_use_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(be_use_role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

void SpecialCrossHotspring::OnActivityStandy()
{
	m_joiner_map.clear();
	m_popularity_rank_vec.clear();
	m_question_rank_vec.clear();
}

void SpecialCrossHotspring::OnActivityStart()
{
	m_isopen = true;
	m_give_exp_interval = LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg().exp_interval_s;
	m_next_give_exp_timestamp = static_cast<time_t>(EngineAdapter::Instance().Time() + m_give_exp_interval);
	m_popularity_rank_interval_s = LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg().popularity_rank_interval_s;
	m_next_calc_rank_timestamp = static_cast<time_t>(EngineAdapter::Instance().Time());
	m_give_item_interval = LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg().reward_item_interval;
	m_next_give_item_timestamp = static_cast<time_t>(EngineAdapter::Instance().Time() + m_give_item_interval);

	m_is_finish_question_activity = false;
	m_is_can_answer = false;

	m_answer_continue_time = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg()->answer_continue_time;
	m_answer_prepare_time = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg()->answer_prepare_time;
	m_cur_question_begin_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg()->activity_prepare_time;			
	m_curr_question_end_time = m_cur_question_begin_time + m_answer_continue_time;
	m_next_question_begin_time = m_cur_question_begin_time;
	m_total_question_num = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg()->total_question_count;
	m_broadcast_question_total = 0;					
	m_curr_question_id = 0;
	
	// 采集物
	const HotspringOtherConfig &other_cfg = LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg();
	m_start_refresh_gather_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.refresh_gather_start_time_s;
	m_next_refresh_gather_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.refresh_gather_start_time_s;

	this->QASyncGlobalInfoToAll(QA_BROADCAST_QUSETION);
	this->QASyncGlobalInfoToAll(QA_BROADCAST_RANK);
}

void SpecialCrossHotspring::OnActivityClose()
{
	if (!m_isopen) return;

	m_isopen = false;

	// 活动结束进行最后一次答题积分排名
	m_is_finish_question_activity = true;
	this->CalcScoreRank();
	this->CalcRewardOnQAActClose();

	// 清理数据
	std::map<long long, JoinerInfo*>::iterator iter = m_joiner_map.begin();
	for (; m_joiner_map.end() != iter; ++ iter)
	{
		delete iter->second;
	}
	m_joiner_map.clear();

	m_popularity_rank_vec.clear();
	m_question_rank_vec.clear();

	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			this->SyncRoleInfoToCross(role);
		}
	}
}

void SpecialCrossHotspring::SyncRoleInfoToCross(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	SyncUserInfoOnBack *info = ActivityShadow::Instance().GetCrossUserInfoOnBack(uuid);

	if (NULL != info && info->cost_gold_in_cross > 0)
	{
		int use_gold = info->cost_gold_in_cross;
		info->cost_gold_in_cross = 0;

		static crossgameprotocal::GameCrossCommonInfoSync cis;
		cis.uuid = uuid;
		cis.has_use_gold = use_gold;
		InternalComm::Instance().SendToCross((const char*)&cis, sizeof(cis));
	}

	ActivityShadow::Instance().DeleteCrossUserInfoOnBack(uuid); // 用完即移除

	role->BackToOriginalServer();
}

void SpecialCrossHotspring::AddExp(Role *role)
{
	if(NULL == role)
		return;

	//int add_exp = 0;
	int exp_add_addition = 0;

	JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == joiner) return;
	
	//const HotspringExpRewardCfg *exp_rewad = LOGIC_CONFIG->GetHotspringCfg().GetExpCfg(role->GetLevel());
	//if (NULL == exp_rewad) return;

	//add_exp = exp_rewad->base_exp;

	//策划需求，修改增加经验规则（2018-1-3）
	long long add_exp = 0;
	const HotspringOtherConfig &other_cfg = LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg();
	add_exp = static_cast<long long>(other_cfg.exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));

	// 自身vip经验加成
	exp_add_addition = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FB_HOTSPRINT_EXTRA_REWARD);

	// 双休经验加成
	if (0 != joiner->partner_uuid)
	{
		exp_add_addition += LOGIC_CONFIG->GetHotspringCfg().GetOtherCfg().interaction_add;
	}


	if (0 < exp_add_addition)
	{
		//add_exp += (int)(add_exp * (exp_add_addition * ROLE_ATTR_PER));
		add_exp += static_cast<long long>(add_exp * (exp_add_addition * ROLE_ATTR_PER));
	}

	joiner->add_exp_total += add_exp;
	//joiner->add_exp_total += static_cast<int>(add_exp);

	Protocol::SCHSAddExpInfo cmd;
	cmd.add_exp_total = joiner->add_exp_total;
	cmd.add_addition = (int)(exp_add_addition * ROLE_ATTR_PER * 100);

	if (0 < add_exp)
	{
		role->AddExp(add_exp, "HotSpring", Role::EXP_ADD_REASON_DEFAULT, NULL, cmd.add_addition);
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cmd, sizeof(Protocol::SCHSAddExpInfo));

}

bool SortFuncByPopularity(const SpecialCrossHotspring::RankItem rankitem_1, const SpecialCrossHotspring::RankItem rankitem_2)
{
	if (rankitem_1.popularity != rankitem_2.popularity)
	{
		return rankitem_1.popularity > rankitem_2.popularity;
	}

	return rankitem_1.popularity_change_time < rankitem_2.popularity_change_time;
}

void SpecialCrossHotspring::CalcPopularityRank()
{
	m_popularity_rank_vec.clear();

	for (std::map<long long, JoinerInfo*>::iterator it = m_joiner_map.begin(); it != m_joiner_map.end(); it++)
	{
		JoinerInfo *info = it->second;
		if (info->popularity > 0)
		{
			m_popularity_rank_vec.push_back(RankItem(info->uuid,info->uid, info->name, info->popularity,info->popularity_change_time));
		}
	}

	if (!m_popularity_rank_vec.empty())
	{
		std::sort(m_popularity_rank_vec.begin(), m_popularity_rank_vec.end(), &SortFuncByPopularity);

		this->SyncRankInfoToAll();
	}
}

bool SortQuestionRankByScore(const SpecialCrossHotspring::QuestiongRankItem rankitem_1, const SpecialCrossHotspring::QuestiongRankItem rankitem_2)
{
	if (rankitem_1.score != rankitem_2.score)
	{
		return rankitem_1.score > rankitem_2.score;
	}

	return rankitem_1.score_change_time < rankitem_2.score_change_time;
}

void SpecialCrossHotspring::CalcScoreRank()
{
	m_question_rank_vec.clear();

	for (std::map<long long, JoinerInfo*>::iterator it = m_joiner_map.begin(); it != m_joiner_map.end(); it++)
	{
		JoinerInfo *info = it->second;
		if (info->question_total_score > 0)
		{
			m_question_rank_vec.push_back(QuestiongRankItem(info->uuid, info->uid, info->name, info->question_total_score, info->question_score_change_time));
		}
	}

	if (!m_question_rank_vec.empty())
	{
		std::sort(m_question_rank_vec.begin(), m_question_rank_vec.end(), &SortQuestionRankByScore);

		this->QASyncGlobalInfoToAll(QA_BROADCAST_RANK);
	}
}

void SpecialCrossHotspring::CalcRewardOnQAActClose()
{
	static MailContentParam contentparam;contentparam.Reset();
	std::map<long long, JoinerInfo*>::iterator iter = m_joiner_map.begin();
	for (; iter != m_joiner_map.end(); iter++)
	{
		int rank = this->FindQuestionRank(iter->second->uuid);
		const HSQuestionRankRewardCfg *rank_cfg = LOGIC_CONFIG->GetQuestionConfig().GetHotSpringRankRewardCfg(rank);
		if (NULL == rank_cfg)
		{
			// 在排名奖励配置中找不到配置，则发放通用奖励
			const QuestionOtherCfg *other_cfg = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
			if (NULL == other_cfg) continue;

			contentparam.Reset();
			int item_count = 0;

			for(; item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
			{
				if (other_cfg->common_item[item_count].item_id <= 0) break;

				contentparam.item_list[item_count].item_id = other_cfg->common_item[item_count].item_id;
				contentparam.item_list[item_count].num = other_cfg->common_item[item_count].num;
				contentparam.item_list[item_count].is_bind = (other_cfg->common_item[item_count].is_bind) ? 1 : 0; 
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_question_rank_reward);
			if (length > 0 && item_count > 0)
			{
				MailRoute::MailToCrossUser(iter->second->unique_user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_question.printf(LL_INFO, "ActivityQusestionReward::user[%d, %s], reward_item_1[%d],num_1[%d],reward_item_2[%d],num_2[%d],reward_item_3[%d],num_3[%d]",
					iter->second->uid, iter->second->name,contentparam.item_list[0].item_id,contentparam.item_list[0].num,
					contentparam.item_list[1].item_id,contentparam.item_list[1].num,contentparam.item_list[2].item_id,contentparam.item_list[2].num);
			}
		}
		else
		{
			//发送排名配置表的奖励
			contentparam.Reset();
			int item_count = 0;

			for(; item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
			{
				if (rank_cfg->rank_reward_item[item_count].item_id <= 0) break;

				contentparam.item_list[item_count].item_id = rank_cfg->rank_reward_item[item_count].item_id;
				contentparam.item_list[item_count].num = rank_cfg->rank_reward_item[item_count].num;
				contentparam.item_list[item_count].is_bind = (rank_cfg->rank_reward_item[item_count].is_bind) ? 1 : 0;
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_rank_reward_content, CROSS_ACTIVITY_TYPE_HOT_SPRING,rank);
			if (length > 0 && item_count > 0)
			{
				MailRoute::MailToCrossUser(iter->second->unique_user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_question.printf(LL_INFO, "ActivityQusestionReward::user[%d, %s], reward_item_1[%d],num_1[%d],reward_item_2[%d],num_2[%d],reward_item_3[%d],num_3[%d]",
					iter->second->uid, iter->second->name,contentparam.item_list[0].item_id,contentparam.item_list[0].num,
					contentparam.item_list[1].item_id,contentparam.item_list[1].num,contentparam.item_list[2].item_id,contentparam.item_list[2].num);
			}
		}
	}
}

void SpecialCrossHotspring::RefreshGather()
{
	const std::map<int, HotspringGahterCfg> &gather_cfg_map = LOGIC_CONFIG->GetHotspringCfg().GetGatherCfg();

	const std::vector<Posi> &gather_pos_vec = LOGIC_CONFIG->GetHotspringCfg().GetRandGatherPosVec();
	auto pos_it = gather_pos_vec.cbegin();

	for (auto it = gather_cfg_map.begin(); it != gather_cfg_map.end(); ++it)
	{
		const auto &cfg = it->second;

		for (int i = 0; i < cfg.gather_count && pos_it != gather_pos_vec.cend(); ++i, ++pos_it)
		{
			GatherObj *gather = new GatherObj();

			gather->Init(nullptr, cfg.gather_id, cfg.gather_time_ms, 0, true, cfg.gather_disappear_time);
			gather->SetPos(*pos_it);

			m_scene->AddObj(gather);
		}
	}
}

void SpecialCrossHotspring::ClearAllJoninerAnswerState()
{
	std::map<long long, JoinerInfo *>::iterator iter = m_joiner_map.begin();
	for (; iter != m_joiner_map.end(); iter++)
	{
		iter->second->question_curr_question_flag = 0;
		iter->second->question_curr_issue_score = 0;
		iter->second->question_curr_issue_answer_result = 0;
	}
}

void SpecialCrossHotspring::CalcAllJoinerAnswerResult()
{
	if (m_curr_question_id == 0) return;

	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (NULL == temp_role)
			continue;

		JoinerInfo *joiner = this->GetJoinerItem(UniqueUserIDToLongLong(temp_role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
		if (NULL == joiner) continue;

		if (joiner->question_curr_question_flag == 1)
		{
			int right_answer = LOGIC_CONFIG->GetQuestionConfig().GetAnswer(m_curr_question_id, QUESTION_TYPE_HOTSPRING);
			Protocol::SCHSQAnswerResult qr;
			qr.right_result = right_answer;
			qr.result = joiner->question_curr_issue_answer_result;
			EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char *)&qr, sizeof(qr));
			continue;
		}

		const HSQuestionRewardCfg *reward_cfg = LOGIC_CONFIG->GetQuestionConfig().GetHotSpringRewardCfg(temp_role->GetLevel());
		if (NULL == reward_cfg) continue;
	
		long long add_exp = 0;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];

		if (joiner->question_curr_issue_answer_result == 0)
		{
			//add_exp = static_cast<int>(reward_cfg->exp * (reward_cfg->wrong_answer_percentage * 0.01f));
			//策划需求，修改增加经验规则（2018-1-3）
			const QuestionOtherCfg *other_cfg = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
			if (other_cfg != NULL)
			{
				add_exp = static_cast<long long>(other_cfg->wrong_hotspring_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(temp_role->GetLevel(), 0)));
			}

			++joiner->question_wrong_count;

			int item_count = 0;
			for(int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
			{
				int item_num = static_cast<int>(reward_cfg->reward_item[i].num * reward_cfg->wrong_answer_percentage * 0.01f);
				if ( item_num > 0 || NULL != ITEMPOOL->GetItem(reward_cfg->reward_item[i].item_id))
				{
					reward_item[item_count].item_id = reward_cfg->reward_item[i].item_id;
					reward_item[item_count].num = reward_cfg->reward_item[i].num;
					reward_item[item_count].is_bind = reward_cfg->reward_item[i].is_bind;
					item_count++;
				}
			}
		}
		else if (joiner->question_curr_issue_answer_result == 1)
		{
			//add_exp = reward_cfg->exp;
			//策划需求，修改增加经验规则（2018-1-3）
			const QuestionOtherCfg *other_cfg = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
			if (other_cfg != NULL)
			{
				add_exp = static_cast<long long>(other_cfg->right_hotspring_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(temp_role->GetLevel(), 0)));
			}

			++ joiner->question_right_count;
			joiner->question_total_score += joiner->question_curr_issue_score;
			joiner->question_score_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			int item_count = 0;
			for(int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
			{
				if (reward_cfg->reward_item[i].num > 0 || NULL != ITEMPOOL->GetItem(reward_cfg->reward_item[i].item_id))
				{
					reward_item[item_count].item_id = reward_cfg->reward_item[i].item_id;
					reward_item[item_count].num = reward_cfg->reward_item[i].num;
					reward_item[item_count].is_bind = reward_cfg->reward_item[i].is_bind;
					item_count++;
				}
			}
		}

		// 变身卡&免答卡随机奖励
		{
			int mianda_pro = RandomNum(100);
			int bianshen_pro = RandomNum(100);
			const QuestionOtherCfg * other_cf = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
			if (mianda_pro < other_cf->mianda_pro)
			{
				const ItemBase *itembase = ITEMPOOL->GetItem(other_cf->mianda_card_id);
				if (NULL != itembase)
				{
					ItemConfigData item_config_data(other_cf->mianda_card_id, true, 1);
					temp_role->GetKnapsack()->Put(item_config_data, PUT_REASON_HOTSPRING_ACTIVITY);
				}
			}

			if (bianshen_pro < other_cf->bianshen_pro)
			{
				const ItemBase *itembase = ITEMPOOL->GetItem(other_cf->bianshen_card_id);
				if (NULL != itembase)
				{
					ItemConfigData item_config_data(other_cf->bianshen_card_id, true, 1);
					temp_role->GetKnapsack()->Put(item_config_data, PUT_REASON_HOTSPRING_ACTIVITY);
				}
			}
		}

		int right_answer = LOGIC_CONFIG->GetQuestionConfig().GetAnswer(m_curr_question_id, QUESTION_TYPE_HOTSPRING);
		Protocol::SCHSQAnswerResult qr;
		qr.right_result = right_answer;
		qr.result = joiner->question_curr_issue_answer_result;
		EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char *)&qr, sizeof(qr));

		//joiner->add_exp_total += static_cast<int>(add_exp);
		joiner->add_exp_total += add_exp;
		joiner->question_curr_question_flag = 1;
		temp_role->AddExp(add_exp, "QuestionActivity", Role::EXP_ADD_REASON_DEFAULT);
		temp_role->GetKnapsack()->PutListOrMail(reward_item, PUT_REASON_HOTSPRING_ACTIVITY);

		this->SendRoleInfo(temp_role);

		gamelog::g_log_question.printf(LL_INFO, "HotSpringBatchAnswerQuestion user[%d, %s], level[%d], question_num[%d],add_exp[%lld]",
			temp_role->GetUID(), temp_role->GetName(), temp_role->GetLevel(), joiner->question_right_count + joiner->question_wrong_count, add_exp);
	}
}

int SpecialCrossHotspring::FindQuestionRank(long long uuid)
{
	if (uuid <= 0) return -1;
	
	for(unsigned int i = 0;i < m_question_rank_vec.size();i++)
	{
		if (m_question_rank_vec[i].uuid == uuid)
		{
			return i+1;
		}
	}

	return -1;
}

void SpecialCrossHotspring::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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
















