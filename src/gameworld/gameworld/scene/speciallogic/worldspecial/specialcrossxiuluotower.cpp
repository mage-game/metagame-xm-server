#include "specialcrossxiuluotower.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "obj/otherobj/effectobj.h"

#include "engineadapter.h"

#include "internalcomm.h"

#include "world.h"

#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "servercommon/errornum.h"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "scene/activityshadow/activityshadow.hpp"
#include "scene/generater/gathergenerater.h"

#include "servercommon/crossdef.hpp"

#include "effect/effectbase.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"

#include "servercommon/serverconfig/crossxiuluotowercommonconfig.hpp"
#include "servercommon/userprotocal/msgcrosscommon.h"

#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "monster/drop/droppool.hpp"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/otherconfig.h"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/cross/rolecross.h"
#include "gameworld/other/route/mailroute.hpp"
#include "global/tempitemstore/tempitemstore.h"
#include "protocal/msgfb.h"
#include "other/event/eventhandler.hpp"

#include <map>
SpecialCrossXiuluoTower::SpecialCrossXiuluoTower(Scene *scene)
	: SpecialLogic(scene), m_is_init(false), m_need_send_gather_info(false), m_is_notice(false), m_send_rank_title(0), m_next_notice_gather_buff_info_time(0),
	m_next_check_boss_info_time(0)
{
	m_boss_info.monster_id = 0;
	m_boss_info.obj_id = INVALID_OBJ_ID;
	m_boss_info.max_hp = 0;
	m_boss_info.cur_hp = 0;
}

SpecialCrossXiuluoTower::~SpecialCrossXiuluoTower()
{

}

void SpecialCrossXiuluoTower::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_is_init)
	{
		m_is_init = true;
		this->RemoveAllGather();
	}

	if (m_need_send_gather_info)
	{
		m_need_send_gather_info = false;
		this->SendGatherInfo();
	}

	time_t boss_refresh_time = ActivityShadow::Instance().GetXiuLuoTowerBossRefreshTime();
	int refresh_notice_time = CrossXiuluoTowerCommonConfig::Instance().GetBossRefreshNoticeTime();

	if (m_is_notice && now_second > boss_refresh_time - refresh_notice_time)
	{
		if (m_scene->GetSceneID() == CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1))
		{
			this->MonsterNotice();
		}
	}

	if (0 != boss_refresh_time && now_second > boss_refresh_time)
	{
		if (m_scene->GetSceneID() == CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1))
		{
			this->FlushMonster();
		}
	}

	if (static_cast<unsigned int>(now_second) > m_next_notice_gather_buff_info_time)
	{
		m_next_notice_gather_buff_info_time = static_cast<unsigned int>(now_second) + 1;
		this->CheckNotifyGatherBuffInfo();
	}

	if (now_second > m_next_check_boss_info_time)
	{
 		m_next_check_boss_info_time = static_cast<unsigned int>(now_second) + 1;
		this->CheckSendBossInfo();//回血
	}
}

bool SpecialCrossXiuluoTower::CanDestroy()
{
	return false;
}

void SpecialCrossXiuluoTower::OnAddObj(Obj *obj)
{
	if (Obj::OBJ_TYPE_GATHER == obj->GetObjType())
	{
		m_need_send_gather_info = true;
	}
}

void SpecialCrossXiuluoTower::OnRemoveObj(Obj *obj)
{
	if (Obj::OBJ_TYPE_GATHER == obj->GetObjType())
	{
		m_need_send_gather_info = true;
	}
}

bool SpecialCrossXiuluoTower::SpecialCanReAlive(Role *role, int realive_type)
{	
	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER))
	{
		return false;
	}

	return true;
} 

bool SpecialCrossXiuluoTower::SpecialRelive(Role *role, int realive_type)
{
	CrossXiuluoTowerUser *user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == user)
	{
		return false;
	}

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
// 		int cross_free_relive_time = LOGIC_CONFIG->GetOtherCfg().GetOtherCfg().cross_free_relive_time;
// 		if (role->GetDeadTime() + cross_free_relive_time > EngineAdapter::Instance().Time() + 1)
// 		{
// 			role->NoticeNum(errornum::EN_CROSS_XIULUO_TOWER_REALIVE_TIME_LIMIT);
// 			return false;
// 		}

		
		// 一定概率掉层
		if (RandomNum(100) < CrossXiuluoTowerCommonConfig::Instance().GetDropWeightByLayerID(user->cur_layer))
		{
			// 其它层2次以后有概率掉层
			if (!role->ReAlive(SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER, 100)) return false;

			Posi enter_pos;
			int drop_scene_id = CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(user->cur_layer - 1, &enter_pos);
			if (drop_scene_id > 0)
			{
				-- user->cur_layer;
				user->cur_layer_kill_count = 0;
				user->not_drop_layer_count = 0;
				user->immediate_realive_count = 0;

				static Protocol::SCCrossXiuluoTowerChangeLayerNotice cxltdln;
				cxltdln.is_drop_layer = 1;
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cxltdln, sizeof(cxltdln));

				World::GetInstWorld()->GetSceneManager()->GoTo(role, drop_scene_id, CROSS_ACTIVITY_XIULUO_TOWER_SCENE_KEY, enter_pos);
			}
			return true;
		}
		else
		{
			Posi realive_pos = CrossXiuluoTowerCommonConfig::Instance().GetRealivePos(user->cur_layer);
			if (!role->ReAlive(SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER, 100, realive_pos)) return false;

			++ user->not_drop_layer_count;
			return true;
		}
	}
	else
	{

//	if (user->immediate_realive_count >= 2 + role->GetRoleCross()->GetCrossXiuluoTowerBuyRealiveCount())
// 		{
// 			int relive_need_gold = LOGIC_CONFIG->GetOtherCfg().GetOtherCfg().cross_relive_gold;
// 			if (!role->GetKnapsack()->GetMoney()->UseAllGold(relive_need_gold, "SpecialCrossXiuLuoTa::SpecialRelive"))
// 			{
// 				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 				return true;
// 			}
// 		}
// 		else
// 		{
// 			++ user->immediate_realive_count;
// 			this->SendXiuluoTowerSelfInfo(role);
// 		}

//		role->ReAlive(100, 100);
	}

	return false;
}

bool SpecialCrossXiuluoTower::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER))
	{
		return false;
	}

	bool is_enemy = false;

	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			Role *target_role = (Role*)obj;

			bool is_friend = (role->GetUserId() == target_role->GetUserId());
			is_enemy = target_role->IsAlive() && !is_friend;
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *target_monster = (Monster*)obj;
			is_enemy = target_monster->IsAlive();
		}
		break;
	}

	return is_enemy;
}

bool SpecialCrossXiuluoTower::SpecialStopGahterOnAttack(Role *role)
{
	if (nullptr == role)
	{
		return true;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (role->GetRoleCross()->GetCrossXiuluoTowerGatherBuffEndTimestamp() > now)
	{
		return false;
	}

	return true;
}

void SpecialCrossXiuluoTower::OnRoleEnterScene(Role *role)
{
	if (NULL == role) return;

	ActivityShadow::Instance().CheckCrossXiuluoTowerRank(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));

// 	{
// 		CrossXiuluoTowerUser *user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
// 		
// 		if (user->cur_layer > user->reward_layer)
// 		{
// 			// 获得首次进入奖励
// 			const CrossXiuluoTowerFirstRewardCfg *reward = CrossXiuluoTowerCommonConfig::Instance().GetCrossXiuluoTowerFirstRewardCfg(user->cur_layer);
// 			if (NULL != reward)
// 			{
// 				if (!role->GetKnapsack()->IsLock() && role->GetKnapsack()->HaveSpace())
// 				{
// 					role->AddCrossHonor(reward->honour, "XiuluoTowerFirst");
// 					role->GetKnapsack()->Put(reward->item, PUT_REASON_XIULUOTOWER);
// 				}
// 				else
// 				{
// 					// 这里应该是先发个数据给跨服，然后通过跨服发送回原服，再发邮件
// 					crossgameprotocal::GameCrossSyncXiuluoTowerInfo gcsxti;
// 					gcsxti.uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
// 					gcsxti.type = crossgameprotocal::XIULUOTOWER_INFO_TYPE_FIRST;
// 					gcsxti.param = user->cur_layer;
// 					InternalComm::Instance().SendToCross((const char *)&gcsxti, sizeof(gcsxti));
// 				}
// 			}
// 			user->reward_layer = static_cast<short>(user->cur_layer);
// 		}
// 	}

	// 采集buff

	unsigned int gather_buff_end_timestamp = role->GetRoleCross()->GetCrossXiuluoTowerGatherBuffEndTimestamp();
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (gather_buff_end_timestamp > now_sec)
	{
		int buff_duration_ms = (gather_buff_end_timestamp - now_sec) * 1000;

		EffectOtherBuff *effect_other = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_GATHER_NOT_STOP, buff_duration_ms, 1);
		effect_other->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		effect_other->SetClientEffectType(EffectBase::ECT_BUFF_GATHER_NO_STOP);
		effect_other->SetSave(false);
		role->AddEffect(effect_other);
	}

	int add_max_hp_per = role->GetRoleCross()->GetCrossXiuluoTowerAddHpPer(), add_gongji_per = role->GetRoleCross()->GetCrossXiuluoTowerAddGongjiPer();
	if (add_max_hp_per > 0 || add_gongji_per > 0)
	{
		EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, 1, 3600000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect_buff->SetClientEffectType(EffectBase::ECT_XIULUOTOWER_BUFF);
		effect_buff->SetClearOnDie(false);

		Attribute gongji_add_value = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * add_gongji_per / 100;
		if (gongji_add_value < 0) gongji_add_value = 0;
		effect_buff->AddGongJiEffect(gongji_add_value, add_gongji_per);

		Attribute hp_add_value = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * add_max_hp_per / 100;
		if (hp_add_value < 0) hp_add_value = 0;
		effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, hp_add_value, add_max_hp_per);

		role->AddEffect(effect_buff);
		role->ChangeHp(&hp_add_value, true);
	}

	this->RecalcBuff(role);
	this->SendXiuluoTowerSelfInfo(role);
	this->RefreshXiuluoTowerRankInfo(role);
	this->SendGatherInfo(role);
	this->SendRankTitleList(role);
	this->SendBossInfo(role);
	EventHandler::Instance().OnEnterCRossXiuluoTower(role);
}

void SpecialCrossXiuluoTower::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (is_logout)
	{
		role->Recover();
		role->BackToOriginalServer();
	}
}

void SpecialCrossXiuluoTower::OnLeaveFB(Role *role)
{
	this->OnSyncRewardLog(role);
}

void SpecialCrossXiuluoTower::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	const CrossXiuluoTowerGatherRewardCfg *cfg = CrossXiuluoTowerCommonConfig::Instance().GetCrossXiuluoTowerGatherRewardCfg(gather_id);
	if (NULL != cfg)
	{
		if (!role->GetKnapsack()->Put(cfg->reward_item, PUT_REASON_XIULUOTOWER))
		{
			return;
		}

		TempItemStore::Instance().PutItem(role->GetUserId(),cfg->reward_item);
		if (cfg->is_golden)
		{
			int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_xiuluo_tower_get_reward, 
				role->GetUID(), role->GetName(), (int)role->GetCamp());
			if (str_len > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
			}
		}
	}

	const int role_daily_gold_box_get_num = role->GetRoleCross()->GetCrossXiuluoTowerDailyGoldBoxNum();

	const int server_gold_box_title_num = ActivityShadow::Instance().GetCrossXiuluoTowerServerGoldBoxNum();
	const int server_drop_goldbox_num_limit = CrossXiuluoTowerCommonConfig::Instance().GetCrossXiuluoServerGoldBoxDropNumLimit();

	const CrossXiuluoTowerOtherCfg &other_cfg = CrossXiuluoTowerCommonConfig::Instance().GetOtherCfg();

	if (RandomNum(10000) < other_cfg.gold_box_item_rate)
	{
		if (server_gold_box_title_num < server_drop_goldbox_num_limit && role_daily_gold_box_get_num < other_cfg.role_gold_box_item_daily_num_limit)
		{
			ActivityShadow::Instance().IncCrossXiuluoTowerServerGoldBoxNum();
			role->GetRoleCross()->IncCrossXiuluoTowerDailyGoldBoxTitleNum();

			ItemConfigData title_item(other_cfg.gold_box_item_id, true, 1);

			if (!role->GetKnapsack()->Put(title_item, PUT_REASON_XIULUOTOWER))
			{
				static MailContentParam mail_param; mail_param.Reset();

				mail_param.AddItem(title_item);

				SNPRINTF(mail_param.contenttxt, sizeof(mail_param.contenttxt), gamestring::g_xiuluo_tower_gold_box_reward_mail_content, other_cfg.gold_box_item_id);

				MailRoute::MailToCrossUser(role->GetUniqueUserID(), SYSTEM_MAIL_REASON_INVALID, mail_param);
			}

			TempItemStore::Instance().PutItem(role->GetUserId(), title_item);
			this->OnSyncDropLog(role, CROSS_XIULUO_TOWER_DROP_LOG_TYPE_GOLD_BOX, other_cfg.gold_box_item_id, 1);
		}
	}
}

void SpecialCrossXiuluoTower::OnCharacterDie(Character *dead, ObjID killer_id)
{
	Role *kill_role = m_scene->GetPrincipal(killer_id);
	if (NULL != kill_role)
	{
		CrossXiuluoTowerUser *killer_user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(kill_role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
		if (NULL != killer_user)
		{
			++ killer_user->total_kill_count;
			++ killer_user->cur_layer_kill_count;

			if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
			{
				++ killer_user->kill_role_count; 
				++ killer_user->continually_kills;
				killer_user->continually_deads = 0;

				int kill_role_reward_honor = CrossXiuluoTowerCommonConfig::Instance().GetKillRoleRewardHonor();
				if (killer_user->kill_reward_cross_honor + kill_role_reward_honor > 2000)
				{
					kill_role_reward_honor = 2000 - killer_user->kill_reward_cross_honor;
				}
				killer_user->kill_reward_cross_honor += kill_role_reward_honor;
				kill_role->AddCrossHonor(kill_role_reward_honor, "XiuluoTower");
				EventHandler::Instance().OnXiuLuoTaKill(kill_role);

				Role *dead_role = (Role*)dead;
				CrossXiuluoTowerUser *dead_user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(dead_role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
				if (NULL != dead_user)
				{
					++ dead_user->continually_deads;
					dead_user->continually_kills = 0;
				}

				this->AddScore(kill_role, CrossXiuluoTowerCommonConfig::Instance().GetKillRoleScore());

				this->RecalcBuff(kill_role);
				this->RecalcBuff(dead_role);
			}
			else if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
			{
				Monster *dead_monster = (Monster*)dead;
				if (dead_monster->IsBossMonster())
				{
					m_boss_info.obj_id = INVALID_OBJ_ID;
					if (killer_user->cur_layer == CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1)
					{
						time_t boss_refresh_time = EngineAdapter::Instance().Time() + CrossXiuluoTowerCommonConfig::Instance().GetBossRefreshTime();
						ActivityShadow::Instance().SetXiuLuoTowerBossRefreshTime(boss_refresh_time);

						// 顶层boss掉落--称号掉落包、坐骑掉落包
						const CrossXiuluoTowerOtherCfg &other_cfg = CrossXiuluoTowerCommonConfig::Instance().GetOtherCfg();

						const int server_drop_title_num = ActivityShadow::Instance().GetCrossXiuluoTowerServerDropTitleNum();
						const int server_drop_title_num_limit = CrossXiuluoTowerCommonConfig::Instance().GetCrossXiuluoServerTitleDropNumLimit();

						const int server_drop_mount_num = ActivityShadow::Instance().GetCrossXiuluoTowerServerDropMountNum();
						const int server_drop_mount_num_limit = CrossXiuluoTowerCommonConfig::Instance().GetCrossXiuluoServerMountDropNumLimit();
						

						const int role_daily_drop_title_get_num = kill_role->GetRoleCross()->GetCrossXiuluoTowerDailyDropTitleNum();
						const int role_daily_drop_mount_get_num = kill_role->GetRoleCross()->GetCrossXiuluoTowerDailyDropMountNum();

						if (RandomNum(10000) < other_cfg.title_drop_rate)
						{
							if (server_drop_title_num < server_drop_title_num_limit && role_daily_drop_title_get_num < other_cfg.role_title_drop_daily_num_limit)
							{
								ActivityShadow::Instance().IncCrossXiuluoTowerServerDropTitleNum();
								kill_role->GetRoleCross()->IncCrossXiuluoTowerDailyDropTitleNum();

								// 称号
								DROPPOOL->DropHelper(other_cfg.title_drop_id, m_scene, dead->GetPos(), kill_role->GetUserId(), kill_role->GetProf(), dead_monster->GetMonsterId(),
									false, true, nullptr);
								// sync log
								int drop_item_count = 0;
								static ItemConfigData item_list[DropConfig::DROP_ITEM_PROB_MAX];
								drop_item_count = DROPPOOL->GetDropItemList(other_cfg.title_drop_id, item_list);

								for (int i = 0; i < drop_item_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
								{
									TempItemStore::Instance().PutItem(kill_role->GetUserId(), item_list[i]);
									this->OnSyncDropLog(kill_role, CROSS_XIULUO_TOWER_DROP_LOG_TYPE_MONSTER, item_list[i].item_id, item_list[i].num);
								}
							}
						}

						if (RandomNum(10000) < other_cfg.mount_drop_rate)
						{
							if (server_drop_mount_num < server_drop_mount_num_limit && role_daily_drop_mount_get_num < other_cfg.role_mount_drop_daily_num_limit)
							{
								ActivityShadow::Instance().IncCrossXiuluoTowerServerDropMountNum();
								kill_role->GetRoleCross()->IncCrossXiuluoTowerDailyDropMountNum();

								DROPPOOL->DropHelper(other_cfg.mount_drop_id, m_scene, dead->GetPos(), kill_role->GetUserId(), kill_role->GetProf(), dead_monster->GetMonsterId(),
									false, true, nullptr);

								// sync log
								int drop_item_count = 0;
								static ItemConfigData item_list[DropConfig::DROP_ITEM_PROB_MAX];
								drop_item_count = DROPPOOL->GetDropItemList(other_cfg.mount_drop_id, item_list);

								for (int i = 0; i < drop_item_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
								{
									TempItemStore::Instance().PutItem(kill_role->GetUserId(), item_list[i]);
									this->OnSyncDropLog(kill_role, CROSS_XIULUO_TOWER_DROP_LOG_TYPE_MONSTER, item_list[i].item_id, item_list[i].num);
								}
							}
						}
					}

					++ killer_user->kill_boss_count;
					ActivityShadow::Instance().SetXiuLuoTowerBossCount(0);
					ActivityShadow::Instance().SendXiuLuoTowerAllRoleInfo();

					ItemID kill_boss_reward_item_id = CrossXiuluoTowerCommonConfig::Instance().GetKillBossRewardItemId();
					ItemConfigData kill_boss_reward(kill_boss_reward_item_id, true, 1);
					kill_role->GetKnapsack()->PutOrMail(kill_boss_reward, PUT_REASON_XIULUOTOWER);

					TempItemStore::Instance().PutItem(kill_role->GetUserId(), kill_boss_reward);

					m_is_notice = true;

					int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_xiuluo_tower_kill_boss, 
						kill_role->GetUID(), kill_role->GetName(), (int)kill_role->GetCamp(), dead_monster->GetMonsterId());
					if (str_len > 0) 
					{
						if (CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1 == killer_user->cur_layer)
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
						}
						else
						{
							this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
						}
					}

					this->RefreshGather();
				}

				this->AddScore(kill_role, CrossXiuluoTowerCommonConfig::Instance().GetKillMonsterScore());
			}

			this->SendXiuluoTowerSelfInfo(kill_role);

			if (killer_user->cur_layer < CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1)
			{
				int uplayer_need_kills = CrossXiuluoTowerCommonConfig::Instance().GetUpLayerNeedKills(killer_user->cur_layer);
				if (killer_user->cur_layer_kill_count >= uplayer_need_kills)
				{
					Posi enter_pos;
					int up_scene_id = CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(killer_user->cur_layer + 1, &enter_pos);
					if (up_scene_id > 0)
					{
						++ killer_user->cur_layer;
						killer_user->cur_layer_kill_count = 0;
						killer_user->not_drop_layer_count = 0;
						killer_user->immediate_realive_count = 0;

						if (killer_user->cur_layer > killer_user->max_layer)
						{
							killer_user->max_layer = killer_user->cur_layer;
							this->AddScore(kill_role, CrossXiuluoTowerCommonConfig::Instance().GetPassLayerScore());

							if (killer_user->max_layer > ActivityShadow::Instance().GetCrossXiuluoTowerMaxLayer())
							{
								ActivityShadow::Instance().SetCrossXiuluoTowerMaxLayer(killer_user->max_layer);
							}

// 							if (CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1 == (killer_user->cur_layer + 1))
// 							{
// 								int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_xiuluo_tower_up_layer_notice, 
// 									UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), killer_user->cur_layer + 1);
// 								if (length > 0)
// 								{
// 									World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 								}
// 							}
						}

						if (killer_user->cur_layer >= CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1 && killer_user->finish_time <= 0)
						{
							unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
							unsigned int activity_begin_time = ActivityShadow::Instance().GetCrossActivityBeginTime(CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER);
							killer_user->finish_time = now_second - activity_begin_time;
							ActivityShadow::Instance().CheckCrossXiuluoTowerRank(killer_user->uuid);
							ActivityShadow::Instance().SetCrossXiuluoTowerRankTitle(killer_user->uuid);

							int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_xiuluo_tower_finish_notice, 
								UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), killer_user->cur_layer + 1);
							if (length > 0)
							{
								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
							}
						}

						static Protocol::SCCrossXiuluoTowerChangeLayerNotice cxltdln;
						cxltdln.is_drop_layer = 0;
						EngineAdapter::Instance().NetSend(kill_role->GetNetId(), (const char*)&cxltdln, sizeof(cxltdln));

						World::GetInstWorld()->GetSceneManager()->GoTo(kill_role, up_scene_id, CROSS_ACTIVITY_XIULUO_TOWER_SCENE_KEY, enter_pos);
					}
				}
			}
		}
	}
}

int SpecialCrossXiuluoTower::GetSpecialParam(Role *role)
{
	if (NULL == role) return 0;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	return role->GetRoleCross()->GetCrossXiuluoTowerGatherBuffEndTimestamp() > now_sec;
}

void SpecialCrossXiuluoTower::OnActivityStatusChange(int status)
{
	if (ACTIVITY_STATUS_OPEN == status)	
	{
		this->OnActivityOpen();
	}
	else if (ACTIVITY_STATUS_CLOSE == status)
	{
		this->OnActivityClose();
	}
}

void SpecialCrossXiuluoTower::SendXiuluoTowerSelfInfo(Role *role)
{
	if (NULL == role) return;

	CrossXiuluoTowerUser *user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == user) return;

	static Protocol::SCCrossXiuluoTowerSelfActivityInfo cxltsai;
	cxltsai.cur_layer = user->cur_layer;
	cxltsai.max_layer = user->max_layer;
	cxltsai.immediate_realive_count = user->immediate_realive_count;
	cxltsai.boss_num = (short)ActivityShadow::Instance().GetXiuLuoTowerBossCount();
	cxltsai.total_kill_count = user->total_kill_count;
	cxltsai.kill_role_count = user->kill_role_count;
	cxltsai.cur_layer_kill_count = user->cur_layer_kill_count;
	cxltsai.reward_cross_honor = user->kill_reward_cross_honor + user->score_reward_cross_honor;
	cxltsai.score = user->score;
	cxltsai.score_reward_flag = user->score_reward_flag;
	cxltsai.refresh_boss_time = (unsigned int)ActivityShadow::Instance().GetXiuLuoTowerBossRefreshTime();

	cxltsai.gather_buff_end_timestamp = role->GetRoleCross()->GetCrossXiuluoTowerGatherBuffEndTimestamp();

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cxltsai, sizeof(cxltsai));
}

void SpecialCrossXiuluoTower::RefreshXiuluoTowerRankInfo(Role *role)
{
	static Protocol::SCCrossXiuluoTowerRankInfo cxltri;
	cxltri.count = ActivityShadow::Instance().GetCrossXiuluoTowerRankList(cxltri.rank_list);

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cxltri, sizeof(cxltri));
	}
	else
	{
		if (NULL != m_scene) m_scene->SendToRole((const char*)&cxltri, sizeof(cxltri));
	}
}

void SpecialCrossXiuluoTower::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{

}

void SpecialCrossXiuluoTower::RecalcBuff(Role *role)
{
	if (NULL == role) return;

	CrossXiuluoTowerUser *user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == user) return;

	int add_max_hp_per = 0, add_gongji_per = 0;

	if (CrossXiuluoTowerCommonConfig::Instance().GetDurDeadBuff(user->continually_deads, &add_max_hp_per, &add_gongji_per))
	{
		EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, DUR_DEAD_SKILL_ID, 60000 * 30, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);

		Attribute gongji_add_value = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * add_gongji_per / 100;
		if (gongji_add_value < 0) gongji_add_value = 0;
		effect_buff->AddGongJiEffect(gongji_add_value, add_gongji_per);

		Attribute hp_add_value = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * add_max_hp_per / 100;
		if (hp_add_value < 0) hp_add_value = 0;
		effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, hp_add_value, add_max_hp_per);

		role->AddEffect(effect_buff);
		if (role->IsAlive()) role->ChangeHp(&hp_add_value, true);
	}
}

void SpecialCrossXiuluoTower::FlushMonster()
{
	// 场景上面有怪暂时不刷怪
	if (!IsAllMonsterDead()) return;

	const CrossXiuluoTowerBossCfg *boss_cfg = CrossXiuluoTowerCommonConfig::Instance().GetCrossXiuluoTowerBossCfg();
	if (NULL != boss_cfg   )
	{
		Monster *monster = MONSTERPOOL->CreateMonster(boss_cfg->monster_id, m_scene, gamecommon::GetDisRandPos(boss_cfg->pos, 1));
		if (NULL != monster)
		{
			int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_xiuluo_tower_boss_already_refresh, 
				monster->GetMonsterId());
			if (str_len > 0) 
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
			}

			ActivityShadow::Instance().SetXiuLuoTowerBossCount(1);
			m_boss_info.monster_id = monster->GetMonsterId();
			m_boss_info.obj_id = monster->GetId();
			m_boss_info.max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			m_boss_info.cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			this->SendBossInfo();
		}
	}

	ActivityShadow::Instance().SetXiuLuoTowerBossRefreshTime(0);
	ActivityShadow::Instance().SendXiuLuoTowerAllRoleInfo();
}

void SpecialCrossXiuluoTower::MonsterNotice()
{
	// 场景上面有怪暂时不刷怪
	if (!IsAllMonsterDead()) return;

	const CrossXiuluoTowerBossCfg *boss_cfg = CrossXiuluoTowerCommonConfig::Instance().GetCrossXiuluoTowerBossCfg();
	if (NULL != boss_cfg)
	{
		int refresh_notice_time = CrossXiuluoTowerCommonConfig::Instance().GetBossRefreshNoticeTime();
		int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_xiuluo_tower_boss_refresh, 
			refresh_notice_time, boss_cfg->monster_id);
		if (str_len > 0) 
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
		}
	}

	m_is_notice = false;
}

void SpecialCrossXiuluoTower::OnScoreRewardReq(Role *role, int index)
{
}

void SpecialCrossXiuluoTower::OnBuyGatherBuff(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	unsigned int buff_end_timestamp = role->GetRoleCross()->GetCrossXiuluoTowerGatherBuffEndTimestamp();
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (buff_end_timestamp > now_sec)
	{
		role->NoticeNum(errornum::EN_CROSS_XIULUOTOWER_HAS_BUY_BUFF);
		return;
	}

	const CrossXiuluoTowerOtherCfg &other_cfg = CrossXiuluoTowerCommonConfig::Instance().GetOtherCfg();

	if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(other_cfg.gather_buff_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.gather_buff_gold, "XiuluoTower::OnBuyGatherBuff"))
	{
		return;
	}

	int buff_duration_ms = other_cfg.gather_buff_duration * 1000;

	EffectOtherBuff *effect_other = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_GATHER_NOT_STOP, buff_duration_ms, 1);
	effect_other->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	effect_other->SetClientEffectType(EffectBase::ECT_BUFF_GATHER_NO_STOP);
	effect_other->SetSave(false);
	role->AddEffect(effect_other);

	EffectSpecialState *effect_special_xuanyun = new EffectSpecialState(INVALID_OBJ_ID, 0, buff_duration_ms, EBT_MIANYI_XUANYUN, 1);
	effect_special_xuanyun->SetSave(false);
	role->AddEffect(effect_special_xuanyun);

	EffectSpecialState *effect_special_dingshen = new EffectSpecialState(INVALID_OBJ_ID, 0, buff_duration_ms, EBT_MIANYI_DINGSHEN, 1);
	effect_special_dingshen->SetSave(false);
	role->AddEffect(effect_special_dingshen);

	role->GetRoleCross()->SetCrossXiuluoTowerGatherBuffEndTimestamp(now_sec + other_cfg.gather_buff_duration);

	CrossXiuluoTowerUser *user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL != user)
	{
		user->is_brocast_gather_buff = false;
	}

	this->SendXiuluoTowerSelfInfo(role);
	this->NotifyGatherBuffInfo(role, 1);
}

void SpecialCrossXiuluoTower::SendAllRoleInfo()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); i++)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			this->SendXiuluoTowerSelfInfo(role);
		}
	}
}

void SpecialCrossXiuluoTower::GMAddScore(Role *role, int add_score)
{
	this->AddScore(role, add_score);
	this->SendXiuluoTowerSelfInfo(role);
}

void SpecialCrossXiuluoTower::GMFlushBoss(Role *role)
{
	ActivityShadow::Instance().SetXiuLuoTowerBossRefreshTime(EngineAdapter::Instance().Time());
}

void SpecialCrossXiuluoTower::AddScore(Role *role, int add_score)
{
	if (NULL == role) return;

	CrossXiuluoTowerUser *user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == user) return;

	user->score += add_score;

	for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_MAX_SCORE_REWARD; i++)
	{
		if (user->score_reward_flag & (1 << i)) continue;

		const CrossXiuluoTowerScoreRewardCfg *cfg = CrossXiuluoTowerCommonConfig::Instance().GetCrossXiuluoTowerScoreRewardCfg(i);
		if (NULL == cfg) continue;

		if (user->score < cfg->score) break;

		user->score_reward_flag |= (1 << i);

		user->score_reward_cross_honor += cfg->reward_honor;
		role->AddCrossHonor(cfg->reward_honor, "XiuluoTower");
		role->GetKnapsack()->PutListOrMail(cfg->item_list, PUT_REASON_XIULUOTOWER);
		TempItemStore::Instance().PutItemList(role->GetUserId(), cfg->item_list);
	}
}

void SpecialCrossXiuluoTower::RemoveAllGather()
{
	SceneMonsterLogic::GatherGeneraterList &generater_list = m_scene->GetMonsterLogic()->GetGatherGeneraterList();
	for (SceneMonsterLogic::GatherGeneraterList::iterator it = generater_list.begin(); it != generater_list.end(); it++)
	{
		(*it)->SetIsOnce(true);
		(*it)->SetPause(true);
	}

	int gather_num = (int)m_scene->GatherObjNum();
	for (int i = 0; i < gather_num; i++)
	{
		GatherObj *gather = m_scene->GetGatherObjByIndex(i);
		if (NULL != gather)
		{
			if (NULL != gather->GetGatherGenerater()) gather->GetGatherGenerater()->Gather(NULL);
			m_scene->DeleteObj(gather->GetId());
		}
	}
}

void SpecialCrossXiuluoTower::RefreshGather()
{
// 	SceneMonsterLogic::GatherGeneraterList &generater_list = m_scene->GetMonsterLogic()->GetGatherGeneraterList();
// 	for (SceneMonsterLogic::GatherGeneraterList::iterator it = generater_list.begin(); it != generater_list.end(); it++)
// 	{
// 		(*it)->RefreshNow();
// 	}
	const CrossXiuluoTowerGatherCfg & gather_cfg = CrossXiuluoTowerCommonConfig::Instance().GetGatherCfg();
	for (int i = 0; i < gather_cfg.gather_count && gather_cfg.gather_list[i].gather_id > 0; ++i)
	{
		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, gather_cfg.gather_list[i].gather_id, gather_cfg.gather_list[i].gather_time * 1000, 0, true, gather_cfg.gather_list[i].disappear_s);
		gather_obj->SetPos(Posi(gather_cfg.gather_list[i].pos_x, gather_cfg.gather_list[i].pos_y));
		m_scene->AddObj(gather_obj);
	}
}

void SpecialCrossXiuluoTower::SendGatherInfo(Role *role)
{
	static Protocol::SCCrossXiuluoTowerGatherInfo pro;
	pro.count = 0;
	memset(pro.info_list, 0, sizeof(pro.info_list));
	std::map<int, int> gMap;
	gMap.clear();
	int gather_num = (int)m_scene->GatherObjNum();
	for (int i = 0; i < gather_num; i++)
	{
		GatherObj *gather = m_scene->GetGatherObjByIndex(i);
		if (NULL != gather)
		{
// 			for (int k = 0; k < Protocol::SCCrossXiuluoTowerGatherInfo::MAX_INFO_COUNT; k++)
// 			{
// 				if (0 == pro.info_list[k].gather_id)
// 				{
// 					pro.info_list[k].gather_id = gather->GatherId();
// 					pro.count++;
// 				}
// 
// 				if (gather->GatherId() == pro.info_list[k].gather_id)
// 				{
// 					pro.info_list[k].gather_count++;
// 					break;
// 				}
// 			}
			std::map<int, int>::iterator itr = gMap.find(gather->GatherId());
			if (itr != gMap.end())
			{
				itr->second += 1;
			}
			else
			{
				gMap.insert(std::make_pair(gather->GatherId(),1));
			}
		}
	}

	std::map<int, int>::iterator itr = gMap.begin();
	for (; itr != gMap.end() && pro.count < Protocol::SCCrossXiuluoTowerGatherInfo::MAX_INFO_COUNT; ++itr)
	{
		pro.info_list[pro.count].gather_id = itr->first;
		pro.info_list[pro.count].gather_count = itr->second;
		pro.count++;
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
	}
	else
	{
		m_scene->SendToRole((const char*)&pro, sizeof(pro));
	}
}

void SpecialCrossXiuluoTower::OnActivityOpen()
{
	if (m_scene->GetSceneID() == CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1))
	{
		ActivityShadow::Instance().SetXiuLuoTowerBossRefreshTime(EngineAdapter::Instance().Time());
	}

	if (m_scene->GetSceneID() == CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(0))
	{
		int role_num = m_scene->RoleNum();
		for (int i = 0; i < role_num; i++)
		{
			Role * role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				Posi posi = CrossXiuluoTowerCommonConfig::Instance().GetRealivePos(0);
				role->ResetPos(posi.x, posi.y);
			}
		}
	}
}

void SpecialCrossXiuluoTower::OnActivityClose()
{
	if (NULL != m_scene)
	{
		this->RemoveAllGather();

		for (int i = 0; i < (int)m_scene->RoleNum(); i++)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				CrossXiuluoTowerUser *user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
				if (NULL != user)
				{
					static Protocol::SCCrossXiuluoTowerUserResult cxltur;
					cxltur.max_layer = user->max_layer;
					cxltur.rank_pos = user->rank_pos;
					cxltur.kill_role_count = static_cast<short>(user->kill_role_count);
					cxltur.reward_cross_honor = user->kill_reward_cross_honor;

					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cxltur, sizeof(cxltur));

					if (1 == user->rank_pos)
					{
						static crossgameprotocal::GameCrossSyncCrossPersonRankInfo rank_info;
						rank_info.plat_type = LongLongToUniqueUserID(user->uuid).plat_type;
						rank_info.role_id = UidToInt(LongLongToUniqueUserID(user->uuid).user_id); 
						rank_info.level = user->level;
						rank_info.sex = user->sex;
						rank_info.prof = user->prof;
						rank_info.camp = user->camp;
						F_STRNCPY(rank_info.name, user->user_name, sizeof(user->user_name));

						rank_info.items[0].rank_type = CROSS_PERSON_RANK_TYPE_XIULUO_TOWER;
						rank_info.items[0].rank_value = user->max_layer;
						rank_info.items[0].record_index = 0;

						rank_info.item_count = 1;
						int sendlen = sizeof(rank_info) - (crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE - rank_info.item_count) * sizeof(rank_info.items[0]);
						InternalComm::Instance().SendToCross((const char *)&rank_info, sendlen);
					}
				}
				this->OnSyncRewardLog(role);
			}
		}

		Protocol::SCReturnOriginalServer ros;
		m_scene->SendToRole((const char*)&ros, sizeof(ros));

		{
			// 活动结束
			ActivityShadow::Instance().SetXiuLuoTowerBossRefreshTime(0);
		}
	}
}

void SpecialCrossXiuluoTower::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialCrossXiuluoTower::OnSyncDropLog(Role *role, int log_type, unsigned short item_id, short num)
{
	static crossgameprotocal::GameCrossSyncXiuluoTowerDropInfo gcsxltdi;

	gcsxltdi.uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	role->GetName(gcsxltdi.name);	
	gcsxltdi.log_type = log_type;
	gcsxltdi.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	gcsxltdi.item_id = item_id;
	gcsxltdi.item_num = num;

	InternalComm::Instance().SendToCross((const char *)&gcsxltdi, sizeof(gcsxltdi));
}


void SpecialCrossXiuluoTower::SendRankTitleList(Role * role)
{
	if (ActivityShadow::Instance().GetCrossXiuluoTowerMaxLayer() < CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1)
	{
		return;
	}

	static Protocol::SCCossXiuluoTowerRankTitleInfo scrti;
	scrti.rank_count = 0;
	memset(scrti.rank_title_list, 0, sizeof(scrti.rank_title_list));
	
	scrti.rank_count = ActivityShadow::Instance().GetCrossXiuluoTowerRankTitleList(scrti.rank_title_list);

	if (nullptr != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scrti, sizeof(scrti));
	}
	else
	{
		m_scene->SendToRole((const char*)&scrti, sizeof(scrti));
	}
}

void SpecialCrossXiuluoTower::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (role == nullptr || falling_item == nullptr || item_wrapper == nullptr)
	{
		return;
	}
	TempItemStore::Instance().PutItem(role->GetUserId(), *item_wrapper);
}

void SpecialCrossXiuluoTower::OnSyncRewardLog(Role *role)
{
	if (role == nullptr) return;

	const std::vector<ItemDataWrapper>& reward_vec = TempItemStore::Instance().GetItemList(role->GetUserId());
	Protocol::SCFBDropInfo fbdi;
	fbdi.get_coin = 0;
	fbdi.get_item_count = 0;

	memset(fbdi.drop_items, 0, sizeof(Protocol::SCFBDropInfo::DropItem) * Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM);
	for (std::vector<ItemDataWrapper>::const_iterator it = reward_vec.begin(); it != reward_vec.end(); it++)
	{
		if (fbdi.get_item_count >= Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM)
		{
			break;
		}
		fbdi.drop_items[fbdi.get_item_count].item_id = it->item_id;
		fbdi.drop_items[fbdi.get_item_count].num = it->num;
		fbdi.get_item_count++;
	}
	TempItemStore::Instance().Clean(role->GetUserId());
	int len = sizeof(fbdi) - sizeof(fbdi.drop_items[0]) * (Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM - fbdi.get_item_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbdi, len);
}

void SpecialCrossXiuluoTower::CheckNotifyGatherBuffInfo()
{
	if (m_scene->GetSceneID() != CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER - 1))
	{
		return;
	}

	int total_role_num = m_scene->RoleNum();
	for (int role_num = 0; role_num < total_role_num; role_num++)
	{
		Role *temp_role = m_scene->GetRoleByIndex(role_num);
		if (NULL != temp_role)
		{
			CrossXiuluoTowerUser *user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(temp_role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
			if (NULL != user)
			{
				unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				if (!user->is_brocast_gather_buff && temp_role->GetRoleCross()->GetCrossXiuluoTowerGatherBuffEndTimestamp() <now_sec)
				{
					user->is_brocast_gather_buff = true;
					this->NotifyGatherBuffInfo(temp_role, 0);
				}
			}
		}
	}
}

void SpecialCrossXiuluoTower::NotifyGatherBuffInfo(Role *role, short is_has_buff)
{
	if (NULL == role) return;

	Protocol::SCNoticeGatherBuffInfo cmd;
	cmd.obj_id = role->GetId();
	cmd.is_gather_wudi = is_has_buff;

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

void SpecialCrossXiuluoTower::SendBossInfo(Role * role)
{
	Protocol::SCCossXiuluoTowerBossInfo boss_info;

	boss_info.monster_id = m_boss_info.monster_id;
	boss_info.max_hp = m_boss_info.max_hp;
	boss_info.cur_hp = m_boss_info.cur_hp;

	if (role != nullptr)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&boss_info, sizeof(boss_info));
	}
	else
	{
		m_scene->SendToRole((const char*)&boss_info, sizeof(boss_info));
	}
}

void SpecialCrossXiuluoTower::CheckSendBossInfo()
{
	if (INVALID_OBJ_ID == m_boss_info.obj_id) return;
	Monster *monster = m_scene->GetMonster(m_boss_info.obj_id);

	if (NULL != monster)
	{
		long long cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		if (m_boss_info.cur_hp != cur_hp)
		{
			m_boss_info.cur_hp = cur_hp;
			this->SendBossInfo();
		}
	}
}