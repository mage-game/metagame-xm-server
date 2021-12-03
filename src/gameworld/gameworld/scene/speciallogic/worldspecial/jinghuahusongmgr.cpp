#include "yaoshouguangchangmgr.hpp"

#include "config/logicconfigmanager.hpp"

#include "obj/character/role.h"

#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/errornum.h"
#include "servercommon/commondata.hpp"

#include "world.h"
#include "global/worldstatus//worldstatus.hpp"

#include "scene/speciallogic/worldspecial/jinghuahusongmgr.hpp"
#include "other/event/eventhandler.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "other/route/mailroute.hpp"

JingHuaHuSongMgr::JingHuaHuSongMgr() : last_dayid(0), m_next_try_refresh_time(0), m_isopen(false), m_last_refresh_gather_times(0), m_cur_remain_gather_times(0), 
										m_gather_objid(0)
{
	
}

JingHuaHuSongMgr::~JingHuaHuSongMgr()
{

}

JingHuaHuSongMgr & JingHuaHuSongMgr::Instance()
{
	static JingHuaHuSongMgr ysgcm;
	return ysgcm;
}

void JingHuaHuSongMgr::OnServerStart()
{
// 	//下次刷新时间
// 	m_next_try_refresh_time = static_cast<int>(EngineAdapter::Instance().Time()) + EngineAdapter::Instance().NextHourInterval(0, 0);
// 
// 	const JHHusongOtherCfg &other_cfg = LOGIC_CONFIG->GetJingHuaHusongCfg().GetOtherCfg();
// 
// 	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.gather_scene_id, 0);
// 	if (NULL != scene)
// 	{
// 		GatherObj *gather_obj = new GatherObj();
// 		gather_obj->Init(NULL, other_cfg.gather_id, other_cfg.gather_time * 1000, 0, false);
// 		gather_obj->SetPos(other_cfg.gather_pos);
// 		gather_obj->SetSpecialGatherType(SPECIAL_GATHER_TYPE_JINGHUA);
// 		scene->AddObj(gather_obj);
// 
// 		m_gather_objid = gather_obj->GetId();
// 	}
}

void JingHuaHuSongMgr::OnRoleLogin(Role *role)
{
//	this->CheckJingHuaHusongTime(role, static_cast<unsigned int>(EngineAdapter::Instance().Time()));
}

void JingHuaHuSongMgr::Update(unsigned long interval, time_t now_second)
{
// 	if (last_dayid != EngineAdapter::Instance().DayID())
// 	{
// 		m_last_refresh_gather_times = 0;
// 		m_cur_remain_gather_times = 0;
// 		last_dayid = EngineAdapter::Instance().DayID();
// 	}
// 
// 	if (static_cast<UInt32>(now_second) >= m_next_try_refresh_time)
// 	{
// 		m_next_try_refresh_time = static_cast<int>(now_second) + EngineAdapter::Instance().NextHourInterval(0, 0); // 下一个整点继续刷
// 		const JHHusongOtherCfg &other_cfg = LOGIC_CONFIG->GetJingHuaHusongCfg().GetOtherCfg();
// 
// 		int now_hour = EngineAdapter::Instance().LocalTime()->tm_hour;
// 		if (now_hour >= other_cfg.open_hour && now_hour < other_cfg.end_hour) //只在一段时间内的整点刷新
// 		{
// 			if (0 == m_last_refresh_gather_times)
// 			{
// 				m_cur_remain_gather_times = other_cfg.gather_min_times;
// 			}
// 			else if (0 == m_cur_remain_gather_times)
// 			{
// 				m_cur_remain_gather_times = m_last_refresh_gather_times + other_cfg.gather_change_times;
// 			}
// 			else
// 			{
// 				m_cur_remain_gather_times = m_last_refresh_gather_times - other_cfg.gather_change_times;
// 			}
// 
// 			{
// 				if (m_cur_remain_gather_times < other_cfg.gather_min_times)
// 				{
// 					m_cur_remain_gather_times = other_cfg.gather_min_times;
// 				}
// 
// 				if (m_cur_remain_gather_times > other_cfg.gather_max_times)
// 				{
// 					m_cur_remain_gather_times = other_cfg.gather_max_times;
// 				}
// 			}
// 
// 			m_last_refresh_gather_times = m_cur_remain_gather_times;
// 
// 			{
// 				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.gather_scene_id, 0);
// 				if (NULL != scene)
// 				{
// 					GatherObj *gather_obj = scene->GetGatherObj(m_gather_objid);
// 					if (NULL != gather_obj)
// 					{
// 						gather_obj->OnGatherChange();
// 					}
// 				}
// 			}
// 
// 			// 传闻
// 			{
// 				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_jinghua_refresh_notice, 
// 					other_cfg.gather_scene_id, other_cfg.gather_pos.x, other_cfg.gather_pos.y);
// 				if (len > 0) 
// 				{
// 					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_AND_ROLL);
// 				}
// 			}
// 		}
// 	}
}

void JingHuaHuSongMgr::CheckJingHuaHusongTime(Role *role, time_t now_second)
{
// 	if (NULL == role) return;
// 
// 	if (JH_HUSONG_STATUS_NONE != role->GetCommonDataParam()->jinghua_husong_status)
// 	{
// 		if (static_cast<UInt32>(now_second) >= role->GetCommonDataParam()->jinghua_husong_invalid_time)
// 		{
// 			this->OnHusongTimeout(role);
// 		}
// 	}
}

void JingHuaHuSongMgr::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
// 	if (NULL == role || NULL == role->GetScene())
// 	{
// 		return;
// 	}
// 
// 	if (role->GetHusongTask()->IsHusongStatus())
// 	{
// 		role->NoticeNum(errornum::EN_JINGHUA_HUSONG_TASK_LIMIT);
// 		return;
// 	}
// 
// 	if (Role::MOVE_MODE_FLY == role->GetMoveMode())
// 	{
// 		role->NoticeNum(errornum::EN_JINGHUA_GATHER_FLY_LIMIT);
// 		return;
// 	}
// 	
// 	if (JH_HUSONG_STATUS_NONE != role->GetCommonDataParam()->jinghua_husong_status)
// 	{
// 		role->NoticeNum(errornum::EN_JINGHUA_ALREADY_HAVE);
// 		return;
// 	}
// 
// 	Obj *obj = role->GetScene()->GetObj(obj_id);
// 	if (NULL == obj || Obj::OBJ_TYPE_GATHER != obj->GetObjType())
// 	{
// 		return;
// 	}
// 
// 	const JHHusongOtherCfg &other_cfg = LOGIC_CONFIG->GetJingHuaHusongCfg().GetOtherCfg();
// 
// 	GatherObj *gather_obj = (GatherObj*)obj;
// 	if (other_cfg.gather_id != gather_obj->GatherId() || role->GetScene()->GetSceneID() != other_cfg.gather_scene_id)
// 	{
// 		return;
// 	}
// 
// 	if (this->IsLevelLimit(role))
// 	{
// 		role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
// 		return;
// 	}
// 
// 	if (m_cur_remain_gather_times <= 0)
// 	{
// 		role->NoticeNum(errornum::EN_JINGHUA_GATHER_NONE);
// 		return;
// 	}
// 
// 	int day_count = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_JINGHUA_GATHER_COUNT);
// 	if (day_count >= other_cfg.gather_day_count)
// 	{
// 		role->NoticeNum(errornum::EN_JINGHUA_GATHER_DAYCOUNT_NOT_ENOUGH);
// 		return;
// 	}
// 	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JINGHUA_GATHER_COUNT);
// 	
// 	-- m_cur_remain_gather_times;
// 	gather_obj->OnGatherChange();
// 
// 	this->OnRoleHusongStatusChange(role, JH_HUSONG_STATUS_HS_FULL);
// 
// 	gamelog::g_log_jinhua_husong.printf(LL_INFO, "OnGather user %d %s, m_cur_remain_gather_times %d",
// 		role->GetUID(), role->GetName(), m_cur_remain_gather_times);
}

void JingHuaHuSongMgr::OnRoleDie(Role *die_role, Role *killer_role)
{
// 	if (NULL == die_role || NULL == killer_role || die_role == killer_role)
// 	{
// 		return;
// 	}
// 
// 	if (killer_role->GetHusongTask()->IsHusongStatus())
// 	{
// 		return;
// 	}
// 
// 	if (Role::MOVE_MODE_FLY == killer_role->GetMoveMode())
// 	{
// 		return;
// 	}
// 
// 	if (this->IsLevelLimit(killer_role))
// 	{
// 		return;
// 	}
// 
// 	// 看击杀规则
// 	if (JH_HUSONG_STATUS_HS_FULL == die_role->GetCommonDataParam()->jinghua_husong_status && 
// 		JH_HUSONG_STATUS_NONE == killer_role->GetCommonDataParam()->jinghua_husong_status)
// 	{
// 		this->OnRoleHusongStatusChange(die_role, JH_HUSONG_STATUS_HS_LOST);
// 		this->OnRoleHusongStatusChange(killer_role, JH_HUSONG_STATUS_HS_FULL);
// 	}
// 
// 	else if (JH_HUSONG_STATUS_HS_FULL == die_role->GetCommonDataParam()->jinghua_husong_status && 
// 		JH_HUSONG_STATUS_HS_LOST == killer_role->GetCommonDataParam()->jinghua_husong_status)
// 	{
// 		this->OnRoleHusongStatusChange(die_role, JH_HUSONG_STATUS_HS_LOST);
// 		this->OnRoleHusongStatusChange(killer_role, JH_HUSONG_STATUS_HS_FULL);
// 	}
}

void JingHuaHuSongMgr::OnCommit(Role *role)
{
// 	if (NULL == role) return;
// 
// 	const int husong_status = role->GetCommonDataParam()->jinghua_husong_status;
// 
// 	if (husong_status <= JH_HUSONG_STATUS_NONE || husong_status >= JH_HUSONG_STATUS_MAX)
// 	{
// 		role->NoticeNum(errornum::EN_JINGHUA_NOT_IN_ROLE);
// 		return;
// 	}
// 
// 	const JHHusongOtherCfg &other_cfg = LOGIC_CONFIG->GetJingHuaHusongCfg().GetOtherCfg();
// 
// 	NPC *npc = role->GetScene()->GetNPCList()->GetNpc(other_cfg.commit_npcid);
// 	if (NULL == npc)
// 	{
// 		return;
// 	}
// 
// 	if (!npc->CanTouch(role))
// 	{
// 		return;
// 	}
// 
// 	this->OnRoleHusongStatusChange(role, JH_HUSONG_STATUS_NONE);
// 	
// 	++ role->GetCommonDataParam()->jinghua_husong_commit_count;
// 
// 	{
// 		int get_jinghua = other_cfg.gather_get_jinghua;
// 		
// 		if (JH_HUSONG_STATUS_HS_LOST == husong_status)
// 		{
// 			get_jinghua = get_jinghua - static_cast<int>(get_jinghua * other_cfg.lost_jinghua_per * 0.01);
// 		}
// 
// 		int reward_per = LOGIC_CONFIG->GetJingHuaHusongCfg().GetJingHuaRewardPer(role->GetCommonDataParam()->jinghua_husong_commit_count);
// 		get_jinghua = static_cast<int>(get_jinghua * reward_per * 0.01);
// 
// 		if (get_jinghua > 0)
// 		{
// 			role->AddYuanli(get_jinghua, "JingHuaHuSong");
// 		}
// 
// 		this->SyncRoleHusongInfo(role);
// 
// 		role->NoticeNum(noticenum::NT_COMMIT_SUCC);
// 
// 		gamelog::g_log_jinhua_husong.printf(LL_INFO, "OnCommit user %d %s, reward_per %d get_jinghua %d, commit_count %d, husong_status %d",
// 			role->GetUID(), role->GetName(), reward_per, get_jinghua, role->GetCommonDataParam()->jinghua_husong_commit_count, husong_status);
// 	}
}

void JingHuaHuSongMgr::OnHusongTimeout(Role *role)
{
// 	if (NULL == role) return;
// 
// 	this->OnRoleHusongStatusChange(role, JH_HUSONG_STATUS_NONE);
// 
// 	++ role->GetCommonDataParam()->jinghua_husong_commit_count;
// 	this->SyncRoleHusongInfo(role);
// 
// 	{
// 		const JHHusongOtherCfg &other_cfg = LOGIC_CONFIG->GetJingHuaHusongCfg().GetOtherCfg();
// 		int get_jinghua = other_cfg.gather_get_jinghua - static_cast<int>(other_cfg.gather_get_jinghua * other_cfg.lost_jinghua_per * 0.01);
// 		int reward_per = LOGIC_CONFIG->GetJingHuaHusongCfg().GetJingHuaRewardPer(role->GetCommonDataParam()->jinghua_husong_commit_count);
// 		get_jinghua = static_cast<int>(get_jinghua * reward_per * 0.01);
// 
// 		static MailContentParam contentparam; contentparam.Reset();
// 
// 		if (get_jinghua > 0)
// 		{
// 			contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_YUANLI] = get_jinghua;
// 		}
// 
// 		int length_1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_jinghua_husong_timeout_subject);
// 		int length_2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_jinghua_husong_timeout_content);
// 		if (length_1 > 0 && length_2 > 0)
// 		{
// 			MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 		}
// 
// 		gamelog::g_log_jinhua_husong.printf(LL_INFO, "OnHusongTimeout user %d %s, reward_per %d get_jinghua %d, commit_count %d",
// 			role->GetUID(), role->GetName(), reward_per, get_jinghua, role->GetCommonDataParam()->jinghua_husong_commit_count);
// 	}
}

void JingHuaHuSongMgr::CheckBuffStatus(Role *role, bool is_add)
{
// 	if (NULL == role) return;
// 
// 	if (is_add)
// 	{
// 		if (this->IsHusongStatus(role))
// 		{
// 			const int HUSONG_BUFF_LAST_TIMES_MS = 7200 * 1000;
// 
// 			EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_JINGHUAHUSONG, HUSONG_BUFF_LAST_TIMES_MS, 1, EffectBase::PRODUCT_METHOD_JINGHUA_HUSONG);
// 			effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
// 			int dec_speed_percent = 40;
// 			int movespeed_dec_value = static_cast<int>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_speed_percent / 100.0f));
// 			effect_buff->AddMoveSpeedEffect(-movespeed_dec_value, -dec_speed_percent);
// 			role->AddEffect(effect_buff);
// 		}
// 	}
// 	else
// 	{
// 		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_JINGHUA_HUSONG, EffectBase::PRODUCT_ID_JINGHUAHUSONG);
// 	}
}

void JingHuaHuSongMgr::OnRoleHusongStatusChange(Role *role, int status)
{
// 	if (NULL == role) return;
// 
// 	const JHHusongOtherCfg &other_cfg = LOGIC_CONFIG->GetJingHuaHusongCfg().GetOtherCfg();
// 
// 	const int old_status = role->GetCommonDataParam()->jinghua_husong_status;
// 	role->GetCommonDataParam()->jinghua_husong_status = status;
// 
// 	if (JH_HUSONG_STATUS_NONE == old_status && JH_HUSONG_STATUS_NONE != status)
// 	{
// 		role->GetMountManager()->GoonMount(false, false);
// 		this->CheckBuffStatus(role, true);
// 		role->GetCommonDataParam()->jinghua_husong_invalid_time = static_cast<UInt32>(EngineAdapter::Instance().Time()) + other_cfg.husong_timeout;
// 	}
// 	else if (JH_HUSONG_STATUS_NONE != old_status && JH_HUSONG_STATUS_NONE == status)
// 	{
// 		role->GetMountManager()->GoonMount(true, false, true);
// 		this->CheckBuffStatus(role, false);
// 		role->GetCommonDataParam()->jinghua_husong_invalid_time = 0;
// 	}
// 
// 	this->BroadcastHusongInfo(role);
}

bool JingHuaHuSongMgr::IsLevelLimit(Role *role)
{
// 	if (NULL == role) return true;
// 
// 	JoinLimitUnit limit_unit;
// 	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_JH_HUSONG_JOIN, &limit_unit);
// 
// 	if (is_limit && role->GetLevel() < limit_unit.level)
// 	{
// 		return true;
// 	}

	return false;
}

bool JingHuaHuSongMgr::IsHusongStatus(Role *role)
{
	if (NULL == role) return false;

	return role->GetCommonDataParam()->jinghua_husong_status != JH_HUSONG_STATUS_NONE;
}

void JingHuaHuSongMgr::SyncRoleHusongInfo(Role *role)
{
// 	if (NULL == role) return;
// 	
// 	int commit_count = role->GetCommonDataParam()->jinghua_husong_commit_count;
// 	int husong_invalid_time = role->GetCommonDataParam()->jinghua_husong_invalid_time;
// 	role->SyncCommonInfo(Protocol::SCIT_JINGHUA_HUSONG_INFO, commit_count, husong_invalid_time);
}

void JingHuaHuSongMgr::BroadcastHusongInfo(Role *role)
{
// 	if (NULL == role || NULL == role->GetScene()) return;
// 
// 	static Protocol::SCJinghuaHusongViewChange cmd;
// 	cmd.obj_id = role->GetId();
// 	cmd.jinghua_husong_status = role->GetCommonDataParam()->jinghua_husong_status;
// 
// 	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

void JingHuaHuSongMgr::GmRefresh()
{
	m_next_try_refresh_time = static_cast<UInt32>(EngineAdapter::Instance().Time());
}
