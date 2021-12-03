#include "gamecommon.h"
#include "monster.h"

#include "scene/generater/monstergenerater.h"
#include "scene/scene.h"
#include "scene/map.h"

#include "globalconfig/globalconfig.h"

#include "obj/character/role.h"

#include "servercommon/string/gameworldstr.h"
#include "scene/scenemanager.h"

#include "gamelog.h"
#include "engineadapter.h"
#include "world.h"
#include "internalcomm.h"

#include "monster/monsterpool.h"
#include "monster/monsterinitparam.h"
#include "monster/createmonsterdelayer.h"

#include "ai/monsterai/monsteraifool.hpp"
#include "ai/monsterai/monsteraistatic.hpp"
#include "ai/monsterai/monsteraipassive.hpp"
#include "ai/monsterai/monsteraiinitiative.hpp"
#include "ai/monsterai/monsteraistaticinitiative.hpp"
#include "ai/monsterai/monsteraijustrun.hpp"
#include "ai/monsterai/monsteraijustflee.hpp"
#include "ai/monsterai/monsteraiaddmianshangbuff.hpp"
#include "ai/monsterai/monsteraiaddrecoverbuff.hpp"

#include "ai/monsterai/specialai/monsterailingshanxianqi.hpp"
#include "ai/monsterai/specialai/monsteraibattlefield.hpp"
#include "ai/monsterai/specialai/monsteraitowerdefendteam.hpp"

#include "ai/monsterai/specialai/monsteraixianmengzhanareaboss.hpp"
#include "ai/monsterai/specialai/monsteraixianmengzhanguardboss.hpp"
#include "ai/monsterai/specialai/monsteraixianmengzhandanyaoboss.hpp"
#include "ai/monsterai/specialai/monsteraiguildtowerdefendattack.hpp"
#include "ai/monsterai/specialai/monsteraiguildtowerdefendmt.hpp"
#include "ai/monsterai/specialai/monsteraiguildtowerdefendshouhu.hpp"
#include "ai/monsterai/specialai/monsteraigongchengzhan.hpp"
#include "ai/monsterai/specialai/monsteraicishacamp1.hpp"
#include "ai/monsterai/specialai/monsteraicishacamp2.hpp"
#include "ai/monsterai/specialai/monsteraicishacamp3.hpp"
#include "ai/monsterai/specialai/monsteraigterritorywartower.hpp"

#include "ai/monsterai/specialai/monsteraiguildfbguard.hpp"
#include "ai/monsterai/specialai/monsteraiguildfbenemy.hpp"

#include "ai/monsterai/specialai/monsteraicoin.hpp"
#include "ai/monsterai/specialai/monsteraicrosspastureanimal.hpp"

#include "ai/monsterai/specialai/monsterairunningmonster.hpp"
#include "ai/monsterai/specialai/monsteraibuildtower.hpp"
#include "ai/monsterai/specialai/monsteraigiftharvest.hpp"
#include "ai/monsterai/monsteraitowerdefendshouhu.hpp"

#include "other/event/eventhandler.hpp"

#include "npc/transmit/fbmanager.hpp"

#include "monster/drop/droppool.hpp"
#include "obj/otherobj/gatherobj.h"

#include "skill/bossskillconditionconfig.hpp"
#include "skill/bossskillconditionpool.hpp"

#include "config/monsterlogicconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/guildconfig.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/dropextendconfig.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "other/worldboss/worldbossshadow.hpp"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "protocal/msgscene.h"

#include "scene/speciallogic/speciallogic.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/guildshadow/guildshadow.hpp"

#include "scene/activityshadow/activityshadow.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/worldevent/worldeventmanager.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "scene/speciallogic/personspecial/specialloversfb.hpp"
#include "scene/speciallogic/worldspecial/huangchenghuizhanmgr.hpp"

#include "friend/teaminfo.hpp"
#include "friend/friendmanager.h"
#include "other/roleactivity/roleactivity.hpp"
#include "other/shengesystem/shengesystem.hpp"

#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "config/activityconfig/babybossconfig.hpp"
#include "global/bossshanggu/bossshanggumanager.hpp"
#include "scene/speciallogic/worldspecial/specialshangguboss.hpp"
#include "other/bosscard/bosscard.hpp"
#include "task/newtaskmanager.h"
#include "scene/speciallogic/worldspecial/specialbabyboss.hpp"
#include "global/team/team.hpp"
#include "global/team/teammanager.hpp"
#include "scene/speciallogic/worldspecial/specialcrossboss.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/fb/fbequipconfig.hpp"
#include "other/jinglingadvantage/jinglingadvantageconfig.hpp"
#include "servercommon/errornum.h"
#include "other/fb/towerdefendteamconfig.hpp"
#include "gameworld/global/activity/activitymsgmanager.h"
#include "scene/speciallogic/worldspecial/specialcrossmizangboss.hpp"
#include "scene/speciallogic/worldspecial/specialcrossyoumingboss.hpp"
#include "obj/otherobj/fallingitem.h"
#include "scene/speciallogic/worldspecial/specialgodmagicboss.hpp"
#include "other/touzijihua/touzijihua.hpp"
#include "config/monsterextradropconfig.hpp"

Monster::Monster(const GameName _name)
: Character(OBJ_TYPE_MONSTER, _name), m_monster_type(MonsterInitParam::MONSTER_TYPE_NORMAL), m_boss_type(0), m_monster_id(0), m_monster_level(0), m_monster_capability(0), m_generater(NULL), m_owner_user_id(INVALID_USER_ID),
m_owner_monster_objid(INVALID_OBJ_ID), m_owner_monster_key(0), m_drop_type(0), m_drop_index(0), m_drop_money_count(0), m_drop_money_prob(0), m_exp_reward(0), m_yuanli_reward(0), m_create_time(0), m_disappear_time(0),
m_die_notice(false), m_forbid_capture_by_skill(false), m_is_fix_hurt_on_attack(false), m_is_forbid_recover(false), m_is_forbid_recover_backup(false), m_auto_reover_per(0), m_auto_reover_time_gap(0),
m_ai(NULL), m_monster_key(0), m_special_param(0), m_need_notice_to_hurt_role_when_die(false), OWNER_MAX_DISTANCE(0), m_drop_owner_uid(0), m_drop_owner_team_index(INVALID_TEAM_INDEX), m_drop_owner_team_next_check_time(0), m_drop_owner_team_last_force_check_time(0), m_drop_owner_hurt(0)
{
}

Monster::~Monster()
{
	if (NULL != m_ai)
	{
		delete m_ai; m_ai = NULL;
	}
}

void Monster::Init(const MonsterInitParam &param, int monster_key)
{
	m_monster_type = param.monster_type;
	m_boss_type = param.boss_type;
	m_monster_id = param.monster_id;
	m_monster_level = param.level;
	m_monster_capability = param.capability;
	m_dropid_list = param.dropid_list;
	m_firsthit_dropid_list = param.firsthit_dropid_list;
	m_dps_dropid_list = param.dps_dropid_list;
	m_rand_dropid_list = param.rand_dropid_list;
	m_task_dropid_list = param.task_dropid_list;

	m_drop_money_count = param.drop_money_count;
	m_drop_money_prob = param.drop_money_prob;

	m_exp_reward = param.exp_reward;
	m_yuanli_reward = param.yuanli_reward;
	m_drop_type = param.drop_type;

	OWNER_MAX_DISTANCE = param.run_range + param.aoi_range;

	CharIntAttrs int_attribute;

	{
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GLOBAL_COOLDOWN, param.cold_down);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP, param.hp);

		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, param.hp);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, param.gongji);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, param.fangyu);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG, param.mingzhong);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI, param.shanbi);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI, param.baoji);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN, param.jianren);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED, param.move_speed);

		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN, param.per_jingzhun);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT, param.per_baoji);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO, param.per_kangbao);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG, param.per_pofang);
		int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG, param.per_mianshang);

		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, param.hp);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, param.gongji);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, param.fangyu);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, param.mingzhong);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, param.shanbi);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, param.baoji);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, param.jianren);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED, param.move_speed);

		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN, param.per_jingzhun);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT, param.per_baoji);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO, param.per_kangbao);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, param.per_pofang);
		int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, param.per_mianshang);
	}

	SkillListParam skill_param; skill_param.count = 1;
	skill_param.skill_list[0].index = 0;
	skill_param.skill_list[0].skill_id = param.skillid;
	skill_param.skill_list[0].level = 1;
	skill_param.skill_list[0].last_perform_time = 0;
	skill_param.skill_list[0].exp = 0;

	{
		const BossSkillConditionConfig *boss_skill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(param.boss_skill_cond_id);
		if (NULL != boss_skill_cond_cfg)
		{
			for (int i = 0; i < boss_skill_cond_cfg->m_skill_count; i++)
			{
				skill_param.skill_list[skill_param.count].index = i + 1;	// 人为约束顺序 提高效率
				skill_param.skill_list[skill_param.count].skill_id = boss_skill_cond_cfg->m_skill_list[i];
				skill_param.skill_list[skill_param.count].level = 1;
				skill_param.skill_list[skill_param.count].last_perform_time = 0;
				skill_param.skill_list[skill_param.count].exp = 0;

				++ skill_param.count;
				if (skill_param.count >= SkillListParam::MAX_SKILL_NUM)
				{
					break;
				}
			}
		}
	}

	Character::Init(int_attribute, skill_param); 

	switch (param.ai_type)
	{
	case MonsterInitParam::AITYPE_FOOL:
		m_ai = new MonsterAIFool(this);
		break;
	case MonsterInitParam::AITYPE_STATIC:
		m_ai = new MonsterAIStatic(this);
		break;
	case MonsterInitParam::AITYPE_PASSIVE:
		m_ai = new MonsterAIPassive(this);
		break;
	case MonsterInitParam::AITYPE_INITIATIVE:
		m_ai = new MonsterAIInitiative(this);
		break;
	case MonsterInitParam::AITYPE_STAT_INITIAT:
		m_ai = new MonsterAIStaticInitiative(this);
		break;
	case MonsterInitParam::AITYPE_JUST_RUN:
		m_ai = new MonsterAIJustRun(this);
		break;
	case MonsterInitParam::AITYPE_JUST_FLEE:
		m_ai = new MonsterAIJustFlee(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_LINGSHANXIANQI:
		m_ai = new MonsterAILingShanXianQi(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_BATTLEFIELD:
		m_ai = new MonsterAIBattleField(this);
		break;
	case MonsterInitParam::AITYPE_CISHA_CAMP1:
		m_ai = new MonsterAiCishaCamp1(this);
		break;
	case MonsterInitParam::AITYPE_CISHA_CAMP2:
		m_ai = new MonsterAiCishaCamp2(this);
		break;
	case MonsterInitParam::AITYPE_CISHA_CAMP3:
		m_ai = new MonsterAiCishaCamp3(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_GONGCHENGZHAN:
		m_ai = new MonsterAIGongchengzhan(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK:
		m_ai = new MonsterAITowerDefendTeam(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_SHOUHU:
		m_ai = new MonsterAITowerDefendShouhu(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_XIANMENGZHAN_AREABOSS:
		m_ai = new MonsterAIXianMengZhanAreaBoss(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_XIANMENGZHAN_GUARDBOSS:
		m_ai = new MonsterAIXianMengZhanGuardBoss(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_XIANMENGZHAN_DANYAOBOSS:
		m_ai = new MonsterAIXianMengZhanDanYaoBoss(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_GUILD_TOWERDEFEND_ATTACK:
		m_ai = new MonsterAIGuildTowerDefendAttack(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_GUILD_TOWERDEFEND_SHOUHU:
		m_ai = new MonsterAIGuildTowerDefendShouHu(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_GUILD_TOWERDEFEND_MT:
		m_ai = new MonsterAIGuildTowerDefendMT(this);
		break;
	case MonsterInitParam::AITYPE_GUILDFB_GUARD:
		m_ai = new MonsterAIGuildFbGuard(this);
		break;
	case MonsterInitParam::AITYPE_GUILDFB_ENEMY:
		m_ai = new MonsterAIGuildFbEnemy(this);
		break;
	case MonsterInitParam::AITYPE_COIN:
		m_ai = new MonsterAICoin(this);
		break;
	case MonsterInitParam::AITYPE_CROSS_PASTURE_ANIMAL:
		m_ai = new MonsterAICrossPastureAnimal(this);
		break;
	case MonsterInitParam::AITYPE_BUILD_TOWER_RUNNING:
		m_ai = new MonsterAIRunningMonster(this);
		break;
	case MonsterInitParam::AITYPE_BUILD_TOWER_TOWER:
		m_ai = new MonsterAIBuildTower(this);
		break;
	case MonsterInitParam::AITYPE_SPECIALAI_TERRITORYWAR_TOWER:
		m_ai = new MonsterAITerritoryWarTower(this);
		break;
	case MonsterInitParam::AITYPE_STATIC_ADD_MIANSHANG_BUFF:
		m_ai = new MonsterAIAddMianshanBuff(this);
		break;
	case MonsterInitParam::AITYPE_STATCI_ADD_RECOVER_BUFF:
		m_ai = new MonsterAIAddRecoverBuff(this);
		break;

	case MonsterInitParam::AITYPE_SPECIALAI_GIFT_HARVEST:
		m_ai = new MonsterAIGiftHarvest(this);
		break;

	default:
		m_ai = new MonsterAIStatic(this);
		break;
	}

	m_ai->Init(MonsterAIParam(param.run_range, param.aoi_range, param.boss_skill_cond_id, param.walk_point_num, param.walk_range));
	if (!m_dps_dropid_list.empty())
	{
		m_ai->SetHasDpsDrop(true);
	}

	if (!m_firsthit_dropid_list.empty())
	{
		m_ai->SetHasFirstDrop(true);
	}

	int dir = (Dir_Max > 1 ? int(RandomNum(static_cast<int>(2 * Dir_Max * 100)) - Dir_Max * 100) : 0);
	m_dir = static_cast<float>(dir / 100.0f);
	m_create_time = EngineAdapter::Instance().Time();

	if (0 != param.disappear_time)
	{
		m_disappear_time = m_create_time + param.disappear_time;
	}

	m_die_notice = param.die_notice;
	m_need_notice_to_hurt_role_when_die = param.dead_notice;
	m_forbid_capture_by_skill = param.forbid_capture_by_skill;
	m_is_fix_hurt_on_attack = param.is_fix_hurt_on_attack;
	m_is_forbid_recover = param.is_forbid_recover;
	m_is_forbid_recover_backup = param.is_forbid_recover;
	m_auto_reover_per = param.auto_reover_per;
	m_auto_reover_time_gap = param.auto_reover_time_gap;

	m_monster_key = monster_key;

	MonsterSpecialParam *special_param = LOGIC_CONFIG->GetMonsterLogicConfig().GetMonsterSpecialParam(m_monster_id);
	if (NULL != special_param)
	{
	}
}

// 获取AI类型
int Monster::GetAIType()
{
	if (NULL != m_ai) return m_ai->GetAIType();
	return MonsterInitParam::AITYPE_INVALID;
}

// 进入场景
void Monster::OnEnterScene()
{
	Character::OnEnterScene();
	if (NULL != m_ai) m_ai->OnEnterScene();

	this->SetBossMonsterBuff();
}

// 离开场景
void Monster::OnLeaveScene()
{
	if (NULL != m_ai) m_ai->OnLeaveScene();

	Character::OnLeaveScene();
}

// 进入视野
void Monster::OnAOIEnter(ObjID obj_id)
{
	if (NULL != m_ai) m_ai->OnAOIEnter(obj_id);
}

// 离开视野
void Monster::OnAOILeave(ObjID obj_id)
{
	if (NULL != m_ai) m_ai->OnAOILeave(obj_id);
}

// 附近角色复活
void Monster::OnNearRoleRealive(ObjID obj_id, const Posi &pos)
{
	const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(m_monster_id);
	if (NULL != monster_param)
	{
		if (!(pos - this->GetBirthPosi()).IsLongerThan(monster_param->aoi_range - 2))
		{
			if (NULL != m_ai)
			{
				m_ai->OnAOILeave(obj_id); m_ai->OnAOIEnter(obj_id);
			}
		}
	}
}

// 附近角色死亡
void Monster::OnNearRoleDie(ObjID obj_id, const Posi &pos)
{
	const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(m_monster_id);
	if (NULL != monster_param)
	{
		if (!(pos - this->GetBirthPosi()).IsLongerThan(monster_param->aoi_range - 2))
		{
			if (NULL != m_ai)
			{
				m_ai->OnRoleDie(obj_id);
			}
		}
	}
}

void Monster::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	if (NULL == m_scene) return;

	Character::Update(interval, now_second, now_dayid);

	this->CheckOwnerAlive();

	if (IsAlive())
	{
		if (NULL != m_ai) 
		{
			if (!m_ai->IsSatic())
			{
				m_ai->AI(interval);
			}
			
			m_ai->Update(interval, static_cast<unsigned int>(now_second));
		}

		if (0 != m_disappear_time && m_disappear_time < now_second)
		{
			EventHandler::Instance().OnMonsterDisapper(this);

			m_scene->DeleteObj(m_obj_id);
			if (NULL != m_generater)
			{
				m_generater->MonsterDie(this);
			}

			if (!m_scene->InStaticScene()) m_scene->OnTrigger(TRIGGER_COND_TYPE_MONSTER_DIE, NULL, m_monster_id);

			this->SyncDeadInfo();

			// 消失的boss也要通知事件
			if (SceneRegister::Instance().IsWorldBossScene(m_scene->GetSceneID()))
			{
				int killer_uid = 0;
				GameName killer_name; memset(killer_name, 0, sizeof(killer_name));

				WorldBossShadow::Instance().OnMonsterDie(m_monster_id, killer_uid, killer_name);
			}
		}

		// 伤害排名归属的计算放这里，定时计算
		if (now_second > m_drop_owner_team_next_check_time)
		{
			m_drop_owner_team_next_check_time = now_second + RECALC_DROP_OWNER_INTERVAL_S;

			if (!this->RecalcDropOwner(false) && NULL != m_ai)
			{
				m_ai->OnEnemyLost();
			}
		}
	}
	else
	{
		const int dead_delay = (int)GLOBALCONFIG->GetOtherConfig_Monster_Dead_Delay();
		if (m_dead_time > dead_delay)
		{
			m_scene->DeleteObj(m_obj_id);

			this->SyncDeadInfo();
		}
	}
}

bool Monster::SetOwnerUserId(const UserID &user_id, bool chk_boss)
{	
	if (NULL == m_scene || !m_scene->InStaticScene()) return false;		// 只有野外普通怪 才能设置owner
	if (chk_boss && this->IsBossMonster()) return false;

	m_owner_user_id = user_id;

	return true;
}

// 强制设置为敌方
void Monster::ForceSetEnemy(ObjID obj_id)
{
	if (obj_id != m_obj_id)
	{
		if (NULL != m_ai) m_ai->ForceSetEnemy(obj_id);
	}
}

bool Monster::IsEnemy(Obj *obj)
{
	if (NULL != m_ai) return m_ai->IsEnemy(obj);
	return false;
}

bool Monster::IsFriend(Obj *obj, bool check_alive)
{
	if (NULL != m_ai) return m_ai->IsFriend(obj, check_alive);
	return false;
}

// 固定伤害
int Monster::GetFixHurtOnAttack()
{
	return 1000;
}

void Monster::OnAttackCalcHurt(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill, int fight_type)
{
	Character::OnAttackCalcHurt(attacker, skill_id, delta_hp, from_skill, fight_type);

	Role *attacker_role = m_scene->GetPrincipal(attacker);
	if (NULL != attacker_role)
	{
		EventHandler::Instance().OnRoleHurtMonster(attacker_role, this, delta_hp);

		if (delta_hp < 0)
		{
			m_monster_hurter_map[attacker_role->GetId()] = attacker_role->GetUserId();
		}
	}
}

// 受击
void Monster::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill)
{
	Character::OnAttack(attacker, skill_id, delta_hp, from_skill);

	if (RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GOLDEN_PIG))
	{
		int boss_id = m_monster_id;
		int summon_type = LOGIC_CONFIG->GetRandActivityCfg().IsGoldenPigBoss(boss_id);
		if (summon_type >= 0)
		{
			Role *attacker_role = m_scene->GetPrincipal(attacker);
			if (NULL != attacker_role && delta_hp < 0 && this->GetFixHurtOnAttack() == -delta_hp)
			{
				m_monster_hurter_map[attacker_role->GetId()] = attacker_role->GetUserId();
			}

			Attribute current_hp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			Attribute max_hp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);

			int joiner_reward_flag = LOGIC_CONFIG->GetRandActivityCfg().GetGoldenPigJoinerRewardFlag(boss_id);
			joiner_reward_flag++;

			const RandActivityGoldenPigOtherCfg *golden_pig_other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAGoldenPigOtherCfg();

			if (NULL != golden_pig_other_cfg && joiner_reward_flag > 0 && joiner_reward_flag <= (10000 / golden_pig_other_cfg->per_hp_reward) &&
				(max_hp - current_hp) * (10000 / golden_pig_other_cfg->per_hp_reward) >= max_hp * joiner_reward_flag)
			{
				for (MonsterHurterMap::iterator iter = m_monster_hurter_map.begin(); iter != m_monster_hurter_map.end(); ++iter)
				{
					Role *role = m_scene->GetRoleByUID(UidToInt(iter->second));
					if (NULL != role)
					{
						ItemConfigData *item = LOGIC_CONFIG->GetRandActivityCfg().GetRAGoldenPigRewardCfg(summon_type, RandActivityGoldenPigRewardCfg::GOLDEN_PIG_REWARD_TYPE_JOINER, boss_id);
						if (NULL != item)
						{
							role->GetKnapsack()->PutOrMail(*item, PUT_REASON_GOLDEN_PIG_RANDOM_REWARD);
						}
					}
				}

				//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_hurt_golden_pig,
				//	m_monster_id, 100 - golden_pig_other_cfg->per_hp_reward * joiner_reward_flag / 100);

				//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);

				LOGIC_CONFIG->GetRandActivityCfg().SetGoldenPigJoinerRewardFlag(joiner_reward_flag, boss_id);
			}
		}
	}

	Role *attacker_role = m_scene->GetPrincipal(attacker);
	if (NULL != attacker_role)
	{
		if (delta_hp < 0)
		{
			bool is_first_attack_before_owner = m_role_hurt_info_map.empty();

			RoleHurtInfo &info = m_role_hurt_info_map[attacker_role->GetUID()];
			info.uid = attacker_role->GetUID();
			info.objid = attacker_role->GetId();
			info.hurt -= delta_hp;
			// 个人伤害掉落的计算放这里
			if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_ROLE == m_drop_type)
			{
				if (info.hurt > m_drop_owner_hurt)
				{
					m_drop_owner_hurt = info.hurt;
					if (m_drop_owner_uid != attacker_role->GetUID())
					{
						Role *old_dps_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_drop_owner_uid));
						if (NULL != old_dps_role)
						{
							old_dps_role->SetTopDpsFlag(false);
						}

						m_drop_owner_uid = attacker_role->GetUID();
						attacker_role->SetTopDpsFlag(true);
						this->NotifyDropOwner();
					}
				}
			}
			// 队伍掉落的情况，在允许的时间间隔范围内，可以一攻击马上就检查一遍归属，尽量做到及时刷新归属，防止几刀秒的情况没有归属判定
			else if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_TEAM == m_drop_type)
			{
				static const int CHECK_INTERVAL_S = 5;
				time_t now_second = EngineAdapter::Instance().Time();
				if ((now_second > m_drop_owner_team_last_force_check_time + CHECK_INTERVAL_S) || is_first_attack_before_owner)
				{
					m_drop_owner_team_last_force_check_time = now_second;
					this->RecalcDropOwner(false);

					this->NotifyDropOwner();
				}
			}
			// 没有归属的时候，被攻击的第一刀马上下发归属信息
			else if (is_first_attack_before_owner)
			{
				this->NotifyDropOwner();
			}
		}

		// 仙盟镖车 
		static const int biaoche_mosnterid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheMonsterID();
		if (m_monster_id == biaoche_mosnterid && NULL != attacker_role)
		{
			GuildShadow::Instance().OnGuildYunBiaoBeHurt(m_special_param, attacker_role->GetGuildID(), attacker_role->GetUserId());
			EventHandler::Instance().OnEnterGuildYunBiao(attacker_role);
		}

		// 世界BOSS
		if (this->IsWorldBossMonster())
		{
			WorldBossShadow::Instance().OnBeenAttack(attacker_role, this, skill_id, delta_hp, from_skill);
		}
	}

	if (LOGIC_CONFIG->GetJinglingAdvantageCfg().IsJingLingAdvantageBoss(m_monster_id))
	{
		Obj *attack_obj = m_scene->GetObj(attacker);
		if (nullptr != attack_obj && Obj::OBJ_TYPE_ROLE == attack_obj->GetObjType())
		{
			Role *attack_role = static_cast<Role *>(attack_obj);
			const JinglingAdvantageOtherCfg *cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();
			if (nullptr != cfg)
			{
				if (attack_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_JINGLING_ADVANTAGE_BOSS_KILL_COUNT) >= cfg->boss_max_drop_times)
				{
					attack_role->NoticeNum(errornum::EN_JINGLING_ADVANTAGE_BOSS_LIMIT);
				}
			}
		}
	}

	if (NULL != m_ai) m_ai->OnAttack(attacker, skill_id, delta_hp);

	ActivityMsgManager::Instance().MonsterOnAttack(m_scene, this, attacker, delta_hp);
}

// 死亡
void Monster::OnDie(ObjID killer)
{
	Character::OnDie(killer);

	if (NULL != m_ai) m_ai->OnDie(killer);

	if (NULL != m_generater) m_generater->MonsterDie(this);

	this->RecalcDropOwner(true);  // 死亡立即计算一次归属，防止归属者不在线

	this->OnBossDie();  // boss死亡,检查设置刷新等...

	if (INVALID_TEAM_INDEX != m_drop_owner_team_index || 0 != m_drop_owner_uid || INVALID_UNIQUE_USER_ID != m_ai->GetFirstHitUser())
	{
		if (0 != m_drop_owner_uid)
		{
			Role *dps_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_drop_owner_uid));
			if (NULL != dps_role)
			{
				dps_role->SetTopDpsFlag(false);
			}
		}
		this->NotifyDropOwner(NULL, true);
	}

	Role *killer_role = m_scene->GetPrincipal(killer);
	Role *real_killer_role = m_scene->GetPrincipal(killer);      // 最后一刀
	if (this->HasOwner() && m_drop_type != MonsterInitParam::BOSS_DROP_OWN_TYPE_KILLER && m_drop_type != MonsterInitParam::BOSS_DROP_OWN_TYPE_ALL_KILL_TEAM)
	{
		killer_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_owner_user_id);
	}
	else if (this->IsBossMonster())
	{
		if (NULL == killer_role && NULL != m_ai)
		{
			killer_role = m_ai->RandGetHurtRole();
		}
	}

	if (m_need_notice_to_hurt_role_when_die)
	{
		this->NoticeHurtRoleWhenDie(killer_role);
	}

	if (NULL != killer_role)
	{
		killer_role->GetRoleActivity()->OnKillAnyMonster(m_monster_id);

		int member_count = 0;
		Role *member_list[MAX_TEAM_MEMBER_NUM] = {NULL};

		if (!this->HasOwner())
		{
			TeamInfo *team_info = killer_role->GetTeamInfo();
			if (team_info->InTeam())
			{
				if (m_scene->InStaticScene() || Scene::SCENE_TYPE_TEAM_EQUIPFB == m_scene->GetSceneType()) // 组队装备本全队掉落
				{
					member_count = team_info->GetOtherMemberList(member_list, sizeof(member_list) / sizeof(member_list[0]));
				}
				else 
				{
					if (Scene::SCENE_TYPE_WUXINGGUAJI != m_scene->GetSceneType()) // 五行挂机的掉落规则, 只掉落给自己，队伍成员不给
					{
						member_count = team_info->GetOtherMemberInTheSameScene(member_list, sizeof(member_list) / sizeof(member_list[0]));
					}
				}
			}
		}

		{
			int temp_member_count = 0;
			Role *temp_member_list[MAX_TEAM_MEMBER_NUM] = {NULL};
			TeamInfo *team_info = killer_role->GetTeamInfo();
			if (team_info->InTeam())
			{
				temp_member_count = team_info->GetOtherMemberInTheSameScene(temp_member_list, sizeof(temp_member_list) / sizeof(temp_member_list[0]));
			}
			this->RewardExpOnDead(killer_role, temp_member_count, temp_member_list);
		}

		{
			Role *temp_member_list[MAX_TEAM_MEMBER_NUM] = {NULL}; memcpy(temp_member_list, member_list, sizeof(temp_member_list));
			this->RewardYuanliOnDead(killer_role, member_count, temp_member_list);
		}

		if (member_count > 0)
		{
			Role *temp_member_list[MAX_TEAM_MEMBER_NUM] = {NULL}; memcpy(temp_member_list, member_list, sizeof(temp_member_list));
			this->RewardIntimacyOnDead(killer_role, member_count, temp_member_list);
		}

		{
			Role *temp_member_list[MAX_TEAM_MEMBER_NUM] = {NULL}; memcpy(temp_member_list, member_list, sizeof(temp_member_list));
			this->DropOnDead(killer_role, member_count, temp_member_list);
		}


		{
			Role *temp_member_list[MAX_TEAM_MEMBER_NUM] = {NULL}; memcpy(temp_member_list, member_list, sizeof(temp_member_list));
			this->DropMoneyOnDead(killer_role, member_count, temp_member_list);
		}

		{
			Role *temp_member_list[MAX_TEAM_MEMBER_NUM] = {NULL}; memcpy(temp_member_list, member_list, sizeof(temp_member_list));
			this->EventOnDead(killer_role, member_count, temp_member_list);
		}

		if (m_die_notice && this->IsBossMonster())
		{
			Role *temp_member_list[MAX_TEAM_MEMBER_NUM] = {NULL}; memcpy(temp_member_list, member_list, sizeof(temp_member_list));
			this->NoticeOnDead(killer_role, member_count, temp_member_list);
		}

		if (RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GOLDEN_PIG))
		{
			int summon_type = LOGIC_CONFIG->GetRandActivityCfg().IsGoldenPigBoss(m_monster_id);
			if (summon_type >= 0)
			{
				killer_role->GetRoleActivity()->OnKillGoldenPigBoss(summon_type);

				LOGIC_CONFIG->GetRandActivityCfg().SetGoldenPigJoinerRewardFlag(0, m_monster_id);

				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_kill_golden_pig,
					killer_role->GetUID(), killer_role->GetName(), killer_role->GetCamp(), m_monster_id);

				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
				}
			}
		}

		{
			// 仙盟镖车 
			static const int biaoche_mosnterid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheMonsterID();
			if (m_monster_id == biaoche_mosnterid && NULL != killer_role)
			{
				GuildShadow::Instance().OnGuildYunBiaoDead(m_special_param, killer_role->GetGuildID(), killer_role->GetUserId());
			}
		}
	}

	if (this->IsWorldBossMonster())
	{
		if (SceneRegister::Instance().IsWorldBossScene(m_scene->GetSceneID()))
		{
			int killer_uid = 0;
			GameName killer_name; memset(killer_name, 0, sizeof(killer_name));
			if (NULL != killer_role)
			{
				killer_uid = UidToInt(killer_role->GetUserId());
				killer_role->GetName(killer_name);
			}

			WorldBossShadow::Instance().OnMonsterDie(m_monster_id, killer_uid, killer_name);

			if (NULL != killer_role)
			{
				EventHandler::Instance().OnKillWorldBoss(killer_role);

				// 传闻
				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_kill_world_boss, 
					(int)UidToInt(killer_role->GetUserId()), killer_role->GetName(), (int)killer_role->GetCamp(), m_monster_id);
				if (len > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
				}

				// 日志
				gamelog::g_log_worldboss.printf(LL_INFO, "WorldBossDead::Killer monster_id[%d], scene_id[%d], pos[%d, %d], killer_role[%d, %s]",
					m_monster_id, m_scene->GetSceneID(), m_posi.x, m_posi.y, killer_role->GetUID(), killer_role->GetName());

				ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, killer_role, killer_role->GetLevel(), m_monster_id, "World", NULL);
			}

			Role *top_hurt_role = m_ai->GetTopHurtRole();
			if (NULL != top_hurt_role)
			{
				gamelog::g_log_worldboss.printf(LL_INFO, "WorldBossDead::TopHurt monster_id[%d], scene_id[%d], pos[%d, %d], top_hurt[%d, %s]",
					m_monster_id, m_scene->GetSceneID(), m_posi.x, m_posi.y, top_hurt_role->GetUID(), top_hurt_role->GetName());

				//激活图鉴   世界boss 最后一刀 dps最高
				if (real_killer_role != nullptr)
				{
					real_killer_role->GetBossCard()->CheckActiveCard(m_monster_id);
					top_hurt_role->GetBossCard()->CheckActiveCard(m_monster_id);
				}
			}
		}
	}

	HuangchenghuizhanMgr::Instance().OnMonsterDie(this, killer_role);

	if (!m_scene->InStaticScene()) m_scene->OnTrigger(TRIGGER_COND_TYPE_MONSTER_DIE, killer_role, m_monster_id);

	ActivityMsgManager::Instance().OnMonsterDie(m_scene, this, killer);
}

void Monster::OnBossDie()
{
	// 只处理BOSS
	if (!this->IsBossMonster())
	{
		return;
	}

	if (Scene::IsBossFamilyScene(m_scene->GetSceneID()))
	{
		BossFamilyManager::Instance().BossDie(this);
	}

	if (Scene::IsPerciousBossScene(m_scene->GetSceneID()))
	{
		BossFamilyManager::Instance().OnPreciousBossDie(this);
	}

	if (LOGIC_CONFIG->GetBossFaimlyConfig().IsYunyouBoss(m_monster_id))
	{
		BossFamilyManager::Instance().OnYunyouBossDie(this, m_scene->GetSceneID());
	}

	if (Scene::IsBossDongKuScene(m_scene->GetSceneID()))    //精英(洞窟)boss加疲劳
	{
		BossFamilyManager::Instance().OnMikuBossDie(this);
	}

	if (m_scene->IsDabaoBossScene(m_scene->GetSceneID()))    // 套装（打宝）boss加愤怒值
	{
		BossFamilyManager::Instance().OnDabaoBossDie(this);
	}

	if (m_scene->IsActiveBossScene(m_scene->GetSceneID()))         // 活跃boss加疲劳
	{
		BossFamilyManager::Instance().OnActiveBossDie(this);
	}
}

bool Monster::IsWorldBossMonster()
{
	if (this->IsBossMonster() && MonsterInitParam::BOSS_TYPE_WORLD == m_boss_type)
	{
		return true;
	}

	return false;
}

bool Monster::IsCrossBossMonster()
{
	if (this->IsBossMonster() && MonsterInitParam::BOSS_TYPE_CROSS_BOSS == m_boss_type)
	{
		return true;
	}

	return false;
}

// 强制死亡
void Monster::ForceSetDead(Role *role)
{
	if (this->IsAlive())
	{
		this->ActionStop();

		m_int_attrs.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP, 0);

		m_dead_time = 0;

		if (NULL != role)
		{
			GetMonsterAI()->SetFirstHitUser(role->GetUniqueUserID());
			this->OnDie(role->GetId());
		}

		if (NULL != m_generater) m_generater->MonsterDie(this);

		if (!m_scene->InStaticScene()) m_scene->OnTrigger(TRIGGER_COND_TYPE_MONSTER_DIE, NULL, m_monster_id);
	}
}

// 复活
void Monster::ReAlive()
{
	if (!this->IsAlive())
	{
		Attribute blood = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		ChangeHp(&blood, true);
	}
}

bool Monster::ResetPos(Axis x, Axis y)
{
	if (this->MoveBySkill(x, y, m_dir, 0, false))
	{
		return true;
	}

	return false;
}

bool Monster::MoveBySkill(Axis _x, Axis _y, Dir _dir, Scalar distance, bool check_special_state)
{
	if (!this->CanBeCaptureBySkill())
	{
		return false;
	}

	return Character::MoveBySkill(_x, _y, _dir, distance, check_special_state);
}

// 设置怪物Buff
void Monster::SetBossMonsterBuff()
{
	if (this->IsWorldBossMonster())
	{
		m_effect_list.AddSpecialBuff(EBT_MIANYI_XUANYUN);
		m_effect_list.AddSpecialBuff(EBT_MIANYI_DINGSHEN);
		m_effect_list.AddSpecialBuff(EBT_MIANYI_CHENMO);
		m_effect_list.AddSpecialBuff(EBT_MIANYI_CHIHUAN);
	}
}

// 设置怪物初始方向相关参数
void Monster::SetMonsterInitDir(Dir dir, Scalar dis)
{
	m_dir = dir;
	m_dir_distance = dis;
	m_action_status = Character::ACTION_RUN;
}

// 出生点
const Posi & Monster::GetBirthPosi()
{
	if (NULL != m_ai) return m_ai->GetBirthPosi();
	return m_posi;
}

void Monster::RewardExpOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM])
{
	// 获取的member数是同场景的队友数,不包括自己
	if (0 != m_exp_reward)
	{
		long long add_exp = m_exp_reward;
		int add_percent = 0;
		if (1 == member_count)
		{
			add_percent = 15;
		}
		else if (2 == member_count)
		{
			add_percent = 30;
		}

		//float level_factor = GLOBALCONFIG->GetMonsterRewardLevelFactor(killer_role->GetLevel() - m_monster_level);

		if (NULL != m_scene->GetSpecialLogic())
		{
			add_exp = m_scene->GetSpecialLogic()->SpecialRecalcMonsterExp(killer_role, this, add_exp);
		}

		//根据人物等级修正打怪获得经验，衰减分为人物等级比怪物等级高/低两个情况
		{
			int delta_level = killer_role->GetLevel() - m_monster_level;
			bool is_monster_high = delta_level > 0 ? false : true;
			delta_level = delta_level >= 0 ? delta_level : -delta_level;
			double level_factor = GLOBALCONFIG->GetMonsterRewardLevelFactor(killer_role->GetLevel(), delta_level, is_monster_high);
			//reward_exp = (long long)(ceil(reward_exp * level_factor));

			add_exp = (long long)(ceil(add_exp * level_factor));
		}

		killer_role->AddExp(add_exp, "KillMonster", Role::EXP_ADD_REASON_KILL_MONSTER, NULL, add_percent);
	}
}

void Monster::RewardYuanliOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM])
{
	if (m_yuanli_reward > 0)
	{
		if (m_scene->InStaticScene())
		{
			if (member_count >= 0 && member_count < MAX_TEAM_MEMBER_NUM)
			{
				member_list[member_count ++] = killer_role; 

				for (int i = 0; i < member_count; i++)
				{
					if (NULL != member_list[i]) 
					{
						member_list[i]->AddYuanli(m_yuanli_reward, "KillMonster");
					}
				}
			}
		}
		else
		{
			if (member_count >= 0 && member_count < MAX_TEAM_MEMBER_NUM)
			{
				member_list[member_count ++] = killer_role; 

				for (int i = 0; i < member_count; i++)
				{
					if (NULL != member_list[i]) 
					{
						int tmp_yuanli_rwd = m_yuanli_reward;

						if (NULL != m_scene->GetSpecialLogic())
						{
							tmp_yuanli_rwd = m_scene->GetSpecialLogic()->SpecialRecalcMonsterYuanli(member_list[i], this, tmp_yuanli_rwd);
						}

						member_list[i]->AddYuanli(tmp_yuanli_rwd, "KillMonster");
					}
				}
			}
		}
	}
}

void Monster::RewardIntimacyOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM])
{
	const int INTIMACY_REWARD_MONSTER_LEVEL = GLOBALCONFIG->GetIntimacyRewardMonsterLevel();
	const int TEAM_INTIMACY_REWARD = GLOBALCONFIG->GetTeamIntimacyReward();
	int intimacy_reward = this->IsBossMonster() ? TEAM_INTIMACY_REWARD * 2 : TEAM_INTIMACY_REWARD;

	if (m_monster_level >= INTIMACY_REWARD_MONSTER_LEVEL && member_count > 0 && member_count < MAX_TEAM_MEMBER_NUM)
	{
		member_list[member_count ++] = killer_role;
		for (int i = 0; i < member_count - 1; i++)
		{
			for (int j = i + 1; j < member_count; j++)
			{
				if (NULL != member_list[i] && NULL != member_list[j])
				{
					FriendManager::IntimacyIncrease(member_list[i], member_list[j], intimacy_reward, "Team Kill Monster Add", true);
				}
			}
		}
	}
}

// 死亡掉落
void Monster::DropOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM])
{
	if (NULL == killer_role)
	{
		return;
	}

	// 额外掉落，有时间间隔，且一定是谁杀给谁
	{
		if (m_scene->InStaticScene())
		{
			CommonDataParam *common_data = killer_role->GetCommonDataParam();
			if (NULL == common_data)
			{
				return;
			}

			if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) >= common_data->next_extra_drop_timestamp)
			{
				UInt16 drop_id = LOGIC_CONFIG->GetDropExtendCfg().GetRandomExtraDropId(this->GetLevel());
				DROPPOOL->DropHelper(drop_id, m_scene, m_posi, killer_role->GetUserId(), killer_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);

				// 集字活动掉落
				if (RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ITEM_COLLECTION))
				{
					int drop_per = LOGIC_CONFIG->GetDropExtendCfg().GetRandomCollectCutPer(common_data->collect_drop_num);
					if (RandomNum(10000) < drop_per)
					{
						drop_id = LOGIC_CONFIG->GetDropExtendCfg().GetRandomCollectExtraDropId(this->GetLevel());
						DROPPOOL->DropHelper(drop_id, m_scene, m_posi, killer_role->GetUserId(), killer_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
					}
				}

				common_data->next_extra_drop_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetDropExtendCfg().GetOtherCfg()->extradrop_time_gap;
			}
		}
	}

	//首刀掉落
	this->DropToFirstRole(killer_role->GetUniqueUserID());
	//首刀全队掉落
	this->DropToFirstAllTeam(killer_role->GetUniqueUserID());

	// 任务定向掉落
	{
		if (!m_task_dropid_list.empty())
		{
			std::set<UInt16> drop_id_list;
			killer_role->GetTaskManager()->GetTaskDropIdList(&drop_id_list);
			for (std::set<UInt16>::iterator it = drop_id_list.begin(); it != drop_id_list.end(); ++it)
			{
				for (std::vector<UInt16>::iterator iter = m_task_dropid_list.begin(); iter != m_task_dropid_list.end(); ++iter)
				{
					if (*it == *iter)
					{
						DROPPOOL->DropHelper(*iter, m_scene, m_posi, killer_role->GetUserId(), killer_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
					}
				}
			}
		}
	}

	{
		if (!m_dropid_list.empty())
		{
			if (MonsterInitParam::BOSS_DROP_OWN_TYPE_NONE == m_drop_type)
			{
				this->DropNoKiller();
			}
			else if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_ROLE == m_drop_type)
			{
				if (0 != m_drop_owner_uid)
				{
					Role *drop_owner_role = m_scene->GetRoleByUID(m_drop_owner_uid);
					if (NULL != drop_owner_role)
					{
						this->DropToRole(drop_owner_role->GetUserId());
						this->DropExtra(drop_owner_role);
					}
					m_drop_owner_uid = 0;
				}
			}
			else if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_TEAM == m_drop_type)
			{
				if (INVALID_TEAM_INDEX != m_drop_owner_team_index)
				{
					this->DropToTeam(m_drop_owner_team_index);

					m_drop_owner_team_index = INVALID_TEAM_INDEX;
				}
				else if (0 != m_drop_owner_uid) // 组队排行里面，如果有非组队个人的伤害是第一的，发给他
				{
					Role *drop_owner_role = m_scene->GetRoleByUID(m_drop_owner_uid);
					if (NULL != drop_owner_role)
					{
						this->DropToRole(drop_owner_role->GetUserId());
						this->DropExtra(drop_owner_role);
					}

					m_drop_owner_uid = 0;
				}
			}
			else if (MonsterInitParam::BOSS_DROP_OWN_TYPE_KILLER == m_drop_type)
			{
				this->DropToRole(killer_role->GetUserId());
				this->DropExtra(killer_role);
			}
			else if (MonsterInitParam::BOSS_DROP_OWN_TYPE_ALL_KILL_TEAM == m_drop_type)
			{
				this->DropToTeam(killer_role, member_count, member_list);
			}
		}

		if (!this->IsBossMonster())
		{
			// 神格增加掉落概率
			{
				int team_assign_mode = killer_role->GetTeamInfo()->GetMyTeamAssignMode();
				if (RandomNum(100) <= killer_role->GetShengeSystem()->GetAddItemDropPercent())
				{
					if (m_dropid_list.size() > 0)
					{
						UInt16 drop_id = m_dropid_list[RandomNum(m_dropid_list.size())];

						if (TEAM_ASSIGN_MODE_RANDOM == team_assign_mode || Scene::SCENE_TYPE_LOVERS_FB == m_scene->GetSceneType())
						{
							this->DropToRandRole(drop_id, member_count, member_list);
						}
						else
						{
							DROPPOOL->DropHelper(drop_id, m_scene, m_posi, killer_role->GetUserId(), killer_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
						}
					}
				}
			}

			// 挂机活动多倍掉落 只给最后的击杀者 [因为里面只有小怪] [不能放底层函数调用 DropToRandRole]
			{
				this->GuaJiActivityExtraDrop(killer_role);
			}
		}
	}

	if (this->IsBossMonster())
	{
		std::set<int> drop_role_set;
		drop_role_set.clear();

		// 伤害列表，按队伍算，从每个掉落ID里掉落给随机某个人，可能有些人随机不到就得不到
		if (!m_dps_dropid_list.empty() || !m_rand_dropid_list.empty())
		{
			RewarderUnit top_unit;
			RewarderUnit rand_unit_list[BOSS_RAND_REWARD_RAND_COUNT];

			UNSTD_STATIC_CHECK(MAX_TEAM_MEMBER_NUM == static_cast<int>(sizeof(top_unit.objid_list) / sizeof(top_unit.objid_list[0])));

			m_ai->GetTopHurtRewarder(&top_unit, rand_unit_list);

			{
				// dps第一名
// 				if (!m_dps_dropid_list.empty())
// 				{
// 					int top_member_count = 0;
// 					Role *top_member_list[MAX_TEAM_MEMBER_NUM] = {NULL};
// 					Role *top_role = NULL;
// 
// 					int temp_top_member_count = 0;
// 					Role *temp_top_member_list[MAX_TEAM_MEMBER_NUM] = { NULL };
// 
// 					Obj *top_obj = m_scene->GetObj(top_unit.objid_list[0]);
// 					if (NULL != top_obj && Obj::OBJ_TYPE_ROLE == top_obj->GetObjType())
// 					{
// 						top_role = (Role *)top_obj;
// 
// 						TeamInfo *top_team_info = top_role->GetTeamInfo();
// 						if (top_team_info->InTeam())
// 						{
// 							temp_top_member_count = top_team_info->GetOtherMemberInTeam(temp_top_member_list, sizeof(temp_top_member_list) / sizeof(temp_top_member_list[0]));
// 						}
// 
// 						if (temp_top_member_count >= 0 && temp_top_member_count < MAX_TEAM_MEMBER_NUM)
// 						{
// 							temp_top_member_list[temp_top_member_count++] = top_role;
// 						}
// 
// 						for (int i = 0; i < temp_top_member_count && i < MAX_TEAM_MEMBER_NUM; i++)
// 						{
// 							Role *temp_role = temp_top_member_list[i];
// 							if (NULL != temp_role)
// 							{
// 								// 奇遇boss限制次数
// 								if (LOGIC_CONFIG->GetJinglingAdvantageCfg().IsJingLingAdvantageBoss(m_monster_id))
// 								{
// 									const JinglingAdvantageOtherCfg *cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();
// 									if (nullptr != cfg)
// 									{
// 										if (temp_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_JINGLING_ADVANTAGE_BOSS_KILL_COUNT) >= cfg->boss_max_drop_times)
// 										{
// 											temp_role->NoticeNum(errornum::EN_JINGLING_ADVANTAGE_BOSS_LIMIT);
// 											continue;
// 										}
// 										else
// 										{
// 											temp_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JINGLING_ADVANTAGE_BOSS_KILL_COUNT);
// 										}
// 									}
// 								}
// 
// 								if (top_member_count >= 0 && top_member_count < MAX_TEAM_MEMBER_NUM)
// 								{
// 									top_member_list[top_member_count++] = temp_role;
// 								}
// 							}
// 						}
// 					}
// 
// 					if (NULL != top_role)
// 					{
// 						int team_assign_mode = top_role->GetTeamInfo()->GetMyTeamAssignMode();
// 						for (std::vector<UInt16>::iterator it = m_dps_dropid_list.begin(); it != m_dps_dropid_list.end(); ++it)
// 						{
// 							if (TEAM_ASSIGN_MODE_RANDOM == team_assign_mode)
// 							{
// 								int drop_role_uid = this->DropToRandRole(*it, top_member_count, top_member_list);
// 								if (0 != drop_role_uid)
// 								{
// 									drop_role_set.insert(drop_role_uid);
// 								}
// 							}
// 							else
// 							{
// 								if (top_member_count > 0)
// 								{
// 									DROPPOOL->DropHelper(*it, m_scene, m_posi, top_role->GetUserId(), top_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
// 									drop_role_set.insert(top_role->GetUID());
// 								}
// 							}
// 						}
// 					}
// 				}
			}

			{
				// 随机名次
				if (!m_rand_dropid_list.empty())
				{
					for (int rand_unit_idx = 0; rand_unit_idx < BOSS_RAND_REWARD_RAND_COUNT; ++ rand_unit_idx)
					{
						int rand_member_count = 0;
						Role *rand_member_list[MAX_TEAM_MEMBER_NUM] = {NULL};

						Obj *tmp_obj = m_scene->GetObj(rand_unit_list[rand_unit_idx].objid_list[0]);
						if (NULL != tmp_obj && Obj::OBJ_TYPE_ROLE == tmp_obj->GetObjType())
						{
							Role *tmp_role = (Role *)tmp_obj;

							TeamInfo *rand_team_info = tmp_role->GetTeamInfo();
							if (rand_team_info->InTeam())
							{
								rand_member_count = rand_team_info->GetOtherMemberInTeam(rand_member_list, sizeof(rand_member_list) / sizeof(rand_member_list[0]));
							}

							if (rand_member_count >= 0 && rand_member_count < MAX_TEAM_MEMBER_NUM)								
							{
								rand_member_list[rand_member_count ++] = tmp_role; 
							}

							int team_assign_mode = tmp_role->GetTeamInfo()->GetMyTeamAssignMode();

							for (std::vector<UInt16>::iterator it = m_rand_dropid_list.begin(); it != m_rand_dropid_list.end(); ++it)
							{
								if (TEAM_ASSIGN_MODE_RANDOM == team_assign_mode)
								{
									this->DropToRandRole(*it, rand_member_count, rand_member_list);
								}
								else
								{
									DROPPOOL->DropHelper(*it, m_scene, m_posi, tmp_role->GetUserId(), tmp_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
								}
							}
						}
					}
				}						
			}
		}
	}
}

// 死亡铜币掉落
void Monster::DropMoneyOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM])
{
	if (m_drop_money_count <= 0) 
	{
		return;
	}

	int rand_val = RandomNum(100);
	if (rand_val >= m_drop_money_prob)
	{
		return;
	}

	if (m_drop_money_count >= 10000)
	{
		if (member_count >= 0 && member_count < MAX_TEAM_MEMBER_NUM)								
		{
			member_list[member_count ++] = killer_role; 
		}

		int piles = RandomNum(4, 6);
		int base_money = m_drop_money_count / piles;

		for (int i = 0; i < piles; ++i)
		{
			int min_money = (int)(base_money * 0.8);
			int max_money = (int)(base_money * 1.2);

			int money = base_money;

			if (max_money - min_money + 1 > 0)
			{
				money = RandomNum(min_money, max_money + 1);
			}

			if (member_count > 0 && member_count <= MAX_TEAM_MEMBER_NUM && money > 0)
			{
				int rand_val = RandomNum(member_count);

				Role *role = member_list[rand_val];
				if (NULL != role)
				{
					DROPPOOL->DropToHelper(m_scene, COIN_ITEM_ID, 1, true, money, m_posi, role->GetUserId(), m_monster_id, this->IsLogDrop(), FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST, false, &m_drop_index);
				}
			}
		}
	}
	else
	{
		if (NULL != killer_role)
		{
			int min_money = (int)(m_drop_money_count * 0.8);
			int max_money = (int)(m_drop_money_count * 1.2);

			int money = m_drop_money_count;

			if (max_money - min_money + 1 > 0)
			{
				money = RandomNum(min_money, max_money + 1);
			}

			DROPPOOL->DropToHelper(m_scene, COIN_ITEM_ID, 1, true, money, m_posi, killer_role->GetUserId(), m_monster_id, this->IsLogDrop(), FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST, false, &m_drop_index);
		}
	}
}

// 死亡事件
void Monster::EventOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM])
{
	EventHandler::Instance().OnKillerRoleKillMonster(killer_role, this);
	EventHandler::Instance().OnKillerTeamKillMonster(killer_role, this);

	{
		for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; i++) 
		{
			if (NULL != member_list[i])
			{
				EventHandler::Instance().OnKillMonster(member_list[i], m_scene->GetSceneID(), m_posi, m_monster_id, m_monster_level);
				EventHandler::Instance().OnKillerTeamKillMonster(killer_role, this);

				m_monster_hurter_map.erase(member_list[i]->GetId());
			}
		}
	}

	{
		for (MonsterHurterMapIt it = m_monster_hurter_map.begin(), end = m_monster_hurter_map.end(); it != end; ++ it)
		{
			Obj *attacker_obj = m_scene->GetObj(it->first);
			if (NULL != attacker_obj && Obj::OBJ_TYPE_ROLE == attacker_obj->GetObjType())
			{
				Role *attacker_role = (Role*)attacker_obj;
				if (attacker_role->GetUserId() == it->second)
				{
					EventHandler::Instance().OnKillMonster(attacker_role, m_scene->GetSceneID(), m_posi, m_monster_id, m_monster_level);
				}
			}
		}
	}

	m_monster_hurter_map.clear();
}

// 死亡通知
void Monster::NoticeOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM])
{
	if (m_die_notice && this->IsBossMonster())
	{
		if (m_scene->InStaticScene())
		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_kill_common_scene_boss, 
				(int)UidToInt(killer_role->GetUserId()), killer_role->GetName(), (int)killer_role->GetCamp(), m_name);
			if (len > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
			}
		}
		else 
		{
			int all_name_len = 0; static char all_name_str[256] = {0};
			{
				char member_name_str[64] = {0};

				if (member_count >= 0 && member_count < MAX_TEAM_MEMBER_NUM) member_list[member_count++] = killer_role;
				for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; i++)
				{
					if (NULL != member_list[i])
					{
						int namelen = SNPRINTF(member_name_str, sizeof(member_name_str), "{r;%d;%s;%d} ", 
							(int)UidToInt(member_list[i]->GetUserId()), member_list[i]->GetName(), (int)member_list[i]->GetCamp());
						if (namelen > 0 && all_name_len < (int)sizeof(all_name_str) - 1)
						{
							STRNCPY(all_name_str + all_name_len, member_name_str, sizeof(all_name_str) - all_name_len);
							all_name_len += namelen;
						}
					}
				}
			}
			all_name_str[sizeof(all_name_str) - 1] = 0;

			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_kill_fb_boss, all_name_str, m_name);
			if (sendlen > 0) 
			{
				if (!CrossConfig::Instance().IsHiddenServer())
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
				else
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}
}

void Monster::NotifyDropOwner(Role *role, bool dead_notify)
{
	if (!this->IsBossMonster())
	{
		return;
	}

	static Protocol::SCBossDpsFlag rs;
	rs.monster_obj_id = m_obj_id;
	rs.role_obj_id = INVALID_OBJ_ID;
	rs.owner_user_name[0] = '\0';
	rs.guild_name[0] = '\0';

	if (!dead_notify) // boss死亡时候的通知，后面的objid不用赋值
	{
		if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_ROLE == m_drop_type)
		{
			Role *owner_role = m_scene->GetRoleByUID(m_drop_owner_uid);
			if (NULL != owner_role)
			{
				rs.role_obj_id = owner_role->GetId();
				owner_role->GetName(rs.owner_user_name);
				owner_role->GetGuildName(rs.guild_name);
			}
		}
		else if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_TEAM == m_drop_type)
		{
			if (INVALID_TEAM_INDEX != m_drop_owner_team_index)
			{
				Team *team = TeamManager::Instance().GetTeam(m_drop_owner_team_index);
				if (NULL != team)
				{
					long long max_hurt_owner_hurt = 0;
					int member_id_list[MAX_TEAM_MEMBER_NUM];
					int member_count = team->GetMemberUidList(member_id_list, MAX_TEAM_MEMBER_NUM);
					for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
					{
						Role *member_role = m_scene->GetRoleByUID(member_id_list[i]);
						if (NULL != member_role)
						{
							RoleHurtInfoMap::iterator it = m_role_hurt_info_map.find(member_role->GetUID());
							if (m_role_hurt_info_map.end() != it && it->second.hurt > max_hurt_owner_hurt)
							{
								rs.role_obj_id = member_role->GetId();
								member_role->GetName(rs.owner_user_name);
								member_role->GetGuildName(rs.guild_name);
								max_hurt_owner_hurt = it->second.hurt;
							}
						}
					}
				}
			}
			else if (INVALID_OBJ_ID != m_drop_owner_uid)
			{
				Role *owner_role = m_scene->GetRoleByUID(m_drop_owner_uid);
				if (NULL != owner_role)
				{
					rs.role_obj_id = owner_role->GetId();
					owner_role->GetName(rs.owner_user_name);
					owner_role->GetGuildName(rs.guild_name);
				}
			}
		}
	}

	// 强制攻击伤害最高的归属者
	ObjID max_hurt_owner_obj_id = rs.role_obj_id;

	if (INVALID_OBJ_ID != max_hurt_owner_obj_id)
	{
		m_ai->ForceSetEnemy(max_hurt_owner_obj_id);
	}

	// 首刀归属
	if (!dead_notify)
	{
		if (MonsterInitParam::BOSS_DROP_OWN_TYPE_FIRST_ATTACK == m_drop_type)
		{
			UniqueUserID first_hit_userid = m_ai->GetFirstHitUser();
			Role* first_hit_role = m_scene->GetRoleByUUID(first_hit_userid);
			if (first_hit_role != NULL)
			{
				rs.role_obj_id = first_hit_role->GetId();
				first_hit_role->GetName(rs.owner_user_name);
				first_hit_role->GetGuildName(rs.guild_name);
			}
		}
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char *)&rs, sizeof(rs));
	}
	else
	{
		m_scene->SendToRole((char*)&rs, sizeof(rs));
	}
}

void Monster::BroadcastFirstHitInfo()
{
	static Protocol::SCMonsterFirstHitInfo mfhi;

	mfhi.obj_id = m_obj_id;

	UniqueUserID first_hit_user_id = m_ai->GetFirstHitUser();

	Role *role = m_scene->GetRoleByUUID(first_hit_user_id);
	if (nullptr == role)
	{
		mfhi.is_show = 0;
		mfhi.first_hit_user_name[0] = '\0';
		mfhi.guild_name[0] = '\0';
		mfhi.first_hit_user_obj_id = INVALID_OBJ_ID;
		mfhi.reserve_sh = 0;
	}
	else
	{
		mfhi.is_show = 1;
		role->GetName(mfhi.first_hit_user_name);
		role->GetGuildName(mfhi.guild_name);
		mfhi.first_hit_user_obj_id = role->GetId();
		mfhi.reserve_sh = 0;
	}

	m_scene->SendToRole((char*)&mfhi, sizeof(mfhi));
}

// 是否特殊掉落
bool Monster::IsDropSpecial()
{
	bool is_drop = false;

	return is_drop;
}

// 同步死亡信息
void Monster::SyncDeadInfo()
{
	UNSTD_STATIC_CHECK(88 == Scene::SCENE_TYPE_COUNT);

	if (Scene::IsPersonalScene(m_scene->GetSceneType(), m_scene->GetSceneID()) || 
		Scene::IsTeamScene(m_scene->GetSceneType(), m_scene->GetSceneID()) || 
		Scene::IsGuildScene(m_scene->GetSceneType(), m_scene->GetSceneID()))
	{
		static Protocol::SCSceneMonsterDie smd;
		smd.obj_id = m_obj_id;
		smd.monster_id = m_monster_id;
		smd.monster_key = m_monster_key;
		m_scene->SendToRole((const char*)&smd, sizeof(smd));
	}
}

// 掉落到随机玩家上
int Monster::DropToRandRole(UInt16 dropid, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM])
{
	if (member_count > 0 && member_count <= MAX_TEAM_MEMBER_NUM)
	{
		int rand_val = RandomNum(member_count);

		Role *role = member_list[rand_val];
		if (NULL != role)
		{
			DROPPOOL->DropHelper(dropid, m_scene, m_posi, role->GetUserId(), role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);

			// 无论是击杀、dps、随机 只要是获得掉落，那么在有仙盟幸运的情况下，都检查是否再掉落
			// 仅仅针对仙盟副本 防止出错
			if (Scene::SCENE_TYPE_GUILD_FB == m_scene->GetSceneType())
			{
				this->GuildFbLuckyExtraDrop(role, dropid);
			}

			return role->GetUID();
		}
	}

	return 0;
}

// 非杀死的掉落
void Monster::DropNoKiller()
{
	for (std::vector<UInt16>::iterator it = m_dropid_list.begin(); it != m_dropid_list.end(); ++it)
	{
		DROPPOOL->DropHelper(*it, m_scene, m_posi, INVALID_USER_ID, 0, m_monster_id, this->IsLogDrop(), false, &m_drop_index);
	}
}

void Monster::DropExtra(Role *role)
{
	if (NULL == role)
	{
		return;
	}
	unsigned short kill_count = 0;
	int index = 0;
	if (!role->GetBossFamilyKillCount(m_monster_id, index, kill_count))
	{
		return;
	}
	const MonsterExtraDropCfg *cfg = LOGIC_CONFIG->GetMonsterExtraDropConfig().GetMonsterDropCfg(m_monster_id, kill_count);	
	if (NULL == cfg)
	{
		return;
	}
	size_t size = cfg->drop_id_list.size();
	if (size == 0)
	{
		return;
	}
	role->AddBossFamilyKillCount(m_monster_id, index, 1);
	for (size_t i = 0; i < size; ++i)
	{
		DROPPOOL->DropHelper(cfg->drop_id_list[i], m_scene, m_posi, role->GetUserId(), 0, m_monster_id, this->IsLogDrop(), false, &m_drop_index);
	}
}


// 检查拥有者是否还活着
void Monster::CheckOwnerAlive()
{
	if (NULL != m_scene && INVALID_OBJ_ID != m_owner_monster_objid)
	{
		Obj *owner_obj = m_scene->GetObj(m_owner_monster_objid);
		if (NULL != owner_obj && Obj::OBJ_TYPE_MONSTER == owner_obj->GetObjType())
		{
			Monster *owner_monster = (Monster *)owner_obj;
			if (owner_monster->GetMonsterKey() == m_owner_monster_key && owner_monster->IsAlive() &&
				NULL != owner_monster->GetMonsterAI() && owner_monster->GetMonsterAI()->IsFighting())
			{
				return;
			}
		}

		this->ForceSetDead();
	}
}

// 挂机活动的额外掉落
void Monster::GuaJiActivityExtraDrop(Role *drop_role)
{
	if (NULL == drop_role)
	{
		return;
	}

	int reward_extra_times = 0;
	if (ActivityShadow::Instance().IsGuaJiActivityExtraReward(drop_role, &reward_extra_times))
	{
		for (int i = 0; i < reward_extra_times && i < 5; i++) // 容错 担心策划配置异常 最多给它5次
		{
			for (std::vector<UInt16>::iterator it = m_dropid_list.begin(); it != m_dropid_list.end(); ++it)
			{
				DROPPOOL->DropHelper(*it, m_scene, m_posi, drop_role->GetUserId(), drop_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
			}
		}
	}
}

// 公会Boss额外掉落
void Monster::GuildBossLuckyExtraDrop(Role *drop_role)
{
	if (NULL == drop_role)
	{
		return;
	}

	for (std::vector<UInt16>::iterator it = m_dropid_list.begin(); it != m_dropid_list.end(); ++it)
	{
		DROPPOOL->DropHelper(*it, m_scene, m_posi, drop_role->GetUserId(), drop_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
	}
}

// 公会副本额外掉落
void Monster::GuildFbLuckyExtraDrop(Role *drop_role, int dropid)
{
	if (NULL == drop_role || dropid <= 0)
	{
		return;
	}

	int guildfb_lucky_dropfactor = LOGIC_CONFIG->GetGuildConfig().GetGuildLuckyFactor(drop_role->GetGuildLuckyColor(), GUILD_LUCKY_FACTOR_TYPE_FBDROP);
	if (RandomNum(100) < guildfb_lucky_dropfactor)
	{
		DROPPOOL->DropHelper(dropid, m_scene, m_posi, drop_role->GetUserId(), drop_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
	}
}

bool Monster::RecalcDropOwner(bool boss_die)
{
	int old_drop_owner_uid = m_drop_owner_uid;
	int old_team_index = m_drop_owner_team_index;
	//long long old_owner_hurt = m_drop_owner_hurt;

	// 清空所有归属者信息
	m_drop_owner_uid = 0;
	m_drop_owner_team_index = INVALID_TEAM_INDEX;
	m_drop_owner_hurt = 0;

	bool owner_team_some_one_leave = false;

	for (RoleHurtInfoMap::iterator it = m_role_hurt_info_map.begin(); it != m_role_hurt_info_map.end();)
	{
		Role *role = m_scene->GetRoleByUID(it->second.uid);
		if (NULL == role)
		{
			if (boss_die) // 如果是boss死亡时的计算，只要玩家不在就立马清掉他
			{
				m_ai->ClearHate(it->second.objid);
				m_role_hurt_info_map.erase(it++);

				continue;
			}

			// 玩家不在当前场景，则确认一下是不是在其他场景，都不在，表明是下线了，给他保留信息，否则直接删除（怪物没有玩家logout接口，只能这样判断）
			role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(it->second.uid));
			if (NULL == role)
			{
				time_t now_second = EngineAdapter::Instance().Time();
				if (it->second.logout_time <= 0)
				{
					it->second.logout_time = now_second;
					++it;
				}
				else if (now_second >= it->second.logout_time + OWNER_KEEP_TIME_S /*|| this->IsCrossBossMonster()*/) // 超过归属保留时间了，删除
				{
					m_ai->ClearHate(it->second.objid);
					m_role_hurt_info_map.erase(it++);
				}
				else // 处于保留归属状态
				{
					++it;
				}
			}
			else
			{
				m_ai->ClearHate(it->second.objid);
				m_role_hurt_info_map.erase(it++);
			}
			continue;
		}

		if ((role->GetPos() - m_posi).IsLongerThan(OWNER_MAX_DISTANCE)) // 超过归属范围，从伤害列表里去除
		{
			if (role->GetTeamIndex() == m_drop_owner_team_index)
			{
				owner_team_some_one_leave = true;
			}

			m_ai->ClearHate(it->second.objid);
			m_role_hurt_info_map.erase(it++);
			continue;
		}

		++it;
	}

	// 个人伤害排行计算
	if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_ROLE == m_drop_type)
	{
		for (RoleHurtInfoMap::iterator it = m_role_hurt_info_map.begin(); it != m_role_hurt_info_map.end(); ++it)
		{
			if (it->second.hurt > m_drop_owner_hurt)
			{
				m_drop_owner_hurt = it->second.hurt;
				m_drop_owner_uid = it->second.uid;
			}
		}
	}
	// 组队伤害排行计算
	else if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_TEAM == m_drop_type)
	{
		std::map<int, long long> team_hurt_map;           // 组队伤害列表
		int max_hurt_team_index = INVALID_TEAM_INDEX;	  // 组队最大伤害的队伍下标
		long long max_team_hurt = 0;			          // 组队最大伤害

		int max_hurt_person_uid = 0;			         // 个人最大伤害玩家uid
		long long max_person_hurt = 0;			         // 个人最大伤害

		for (RoleHurtInfoMap::iterator it = m_role_hurt_info_map.begin(); it != m_role_hurt_info_map.end(); ++it)
		{
			Role *role = m_scene->GetRoleByUID(it->second.uid);
			//if (role == NULL)  // 玩家不在场景有可能是归属保留状态，所以要继续执行
			//{
			//	continue;
			//}

			if (NULL != role && role->GetTeamInfo()->InTeam())
			{
				long long &team_hurt = team_hurt_map[role->GetTeamIndex()];
				team_hurt += it->second.hurt;
				if (team_hurt > max_team_hurt)
				{
					max_team_hurt = team_hurt;
					max_hurt_team_index = role->GetTeamIndex();
				}
			}
			else
			{
				if (it->second.hurt > max_person_hurt)
				{
					max_person_hurt = it->second.hurt;
					max_hurt_person_uid = it->second.uid;
				}
			}
		}

		if (max_team_hurt > max_person_hurt) // 组队总战力比个人最大伤害要大，说明归属者属于某个队伍
		{
			m_drop_owner_hurt = max_team_hurt;
			m_drop_owner_team_index = max_hurt_team_index;
		}
		else
		{
			m_drop_owner_hurt = max_person_hurt;
			m_drop_owner_uid = max_hurt_person_uid;
		}
	}

	if (old_drop_owner_uid != m_drop_owner_uid || old_team_index != m_drop_owner_team_index || owner_team_some_one_leave)
	{
		if (old_drop_owner_uid != m_drop_owner_uid)
		{
			Role *old_dps_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_drop_owner_uid));
			if (NULL != old_dps_role)
			{
				old_dps_role->SetTopDpsFlag(false);
			}

			Role *new_dps_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_drop_owner_uid));
			if (NULL != new_dps_role)
			{
				new_dps_role->SetTopDpsFlag(true);
			}
		}

		this->NotifyDropOwner();

		if (0 == m_drop_owner_uid && INVALID_TEAM_INDEX == m_drop_owner_team_index)
		{
			if (m_role_hurt_info_map.empty())
			{
				m_is_forbid_recover = m_is_forbid_recover_backup;
			}

			return false;
		}
	}

	if (0 != m_drop_owner_uid || INVALID_TEAM_INDEX != m_drop_owner_team_index)
	{
		m_is_forbid_recover = true;
	}

	return true;
}

void Monster::DropToFirstRole(UniqueUserID killer_role_uuid)
{
	if (!m_firsthit_dropid_list.empty() && m_drop_type == MonsterInitParam::BOSS_DROP_OWN_TYPE_FIRST_ATTACK)
	{
		UniqueUserID first_hit_userid = m_ai->GetFirstHitUser();
		Role* first_hit_role = m_scene->GetRoleByUUID(first_hit_userid);
		if (nullptr == first_hit_role)
		{
			return;
		}

		if (TeamManager::Instance().GetTeam(first_hit_role->GetTeamIndex()) != nullptr)
		{
			this->DropToFirstRoleTeam(first_hit_role);
		}
		else
		{
			//if (killer_role_uuid == first_hit_userid)
			{
				//激活图鉴  Boss归属
				first_hit_role->GetBossCard()->CheckActiveCard(m_monster_id);
				this->AddAngryOrTireToRole(first_hit_role);
				//BossFamilyManager::Instance().OnEventDropOwnerWithMonsterDie(first_hit_role, this);
			}

			if (this->CanGetDropLevel(first_hit_role->GetLevel()))
			{
				for (std::vector<UInt16>::iterator it = m_firsthit_dropid_list.begin(); it != m_firsthit_dropid_list.end(); ++it)
				{
					DROPPOOL->DropHelper(*it, m_scene, m_posi, first_hit_role->GetUserId(), first_hit_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
				}
				this->DropExtra(first_hit_role);
			}
		}
	}
}

void Monster::DropToFirstRoleTeam(Role *first_hit_role)
{
	if (nullptr == first_hit_role)
	{
		return;
	}
	int team_index = first_hit_role->GetTeamIndex();
	Team *team = TeamManager::Instance().GetTeam(team_index);
	if (team == nullptr) return;

	Role *member_list[MAX_TEAM_MEMBER_NUM] = { NULL };
	int get_drop_count = 0;

	int member_uid_list[MAX_TEAM_MEMBER_NUM] = { 0 };
	int member_count = team->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);

	for (int j = 0; j < member_count && j < MAX_TEAM_MEMBER_NUM && get_drop_count < MAX_TEAM_MEMBER_NUM; ++j)
	{
		Role *temp_role = m_scene->GetRoleByUID(member_uid_list[j]);
		if (temp_role == nullptr) continue;
		
		if (m_role_hurt_info_map.find(member_uid_list[j]) != m_role_hurt_info_map.end() && this->CanGetDropLevel(temp_role->GetLevel()))     // 有造成伤害的玩家
		{
			if (first_hit_role->GetUniqueUserID() == temp_role->GetUniqueUserID())
			{
				//激活图鉴  Boss归属
				temp_role->GetBossCard()->CheckActiveCard(m_monster_id);
			}
			member_list[get_drop_count] = temp_role;
			get_drop_count++;
		}
	}

	std::set<int> drop_role_set;
	drop_role_set.clear();

	for (std::vector<UInt16>::iterator it = m_firsthit_dropid_list.begin(); it != m_firsthit_dropid_list.end(); ++it)
	{
		int drop_role_uid = this->DropToRandRole(*it, get_drop_count, member_list);
		if (0 != drop_role_uid)
		{
			drop_role_set.insert(drop_role_uid);
		}
	}

	for (int i = 0; i < get_drop_count && i < MAX_TEAM_MEMBER_NUM; i++)
	{
		Role *temp_role = member_list[i];
		if (NULL != temp_role)
		{
			if (drop_role_set.find(temp_role->GetUID()) == drop_role_set.end()) continue;
			this->AddAngryOrTireToRole(temp_role);
			//BossFamilyManager::Instance().OnEventDropOwnerWithMonsterDie(temp_role, this);
			this->DropExtra(temp_role);
		}
	}
}

void Monster::DropToFirstAllTeam(UniqueUserID killer_role_uuid)
{
	if (!m_firsthit_dropid_list.empty() && m_drop_type == MonsterInitParam::BOSS_DROP_OWN_TYPE_FIRST_ALL_TEAM)
	{
		UniqueUserID first_hit_userid = m_ai->GetFirstHitUser();
		Role* first_hit_role = m_scene->GetRoleByUUID(first_hit_userid);
		if (nullptr == first_hit_role)
		{
			return;
		}

		int team_index = first_hit_role->GetTeamIndex();
		Team *team = TeamManager::Instance().GetTeam(team_index);
		if (nullptr != team)
		{
			int member_uid_list[MAX_TEAM_MEMBER_NUM] = { 0 };
			int member_count = team->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);

			for (int j = 0; j < member_count && j < MAX_TEAM_MEMBER_NUM; ++j)
			{
				Role *temp_role = m_scene->GetRoleByUID(member_uid_list[j]);
				if (temp_role == nullptr) continue;

				if (m_role_hurt_info_map.find(member_uid_list[j]) != m_role_hurt_info_map.end() && this->CanGetDropLevel(temp_role->GetLevel()))     // 有造成伤害的玩家
				{
					{
						//激活图鉴  Boss归属
						temp_role->GetBossCard()->CheckActiveCard(m_monster_id);
					}

					// 每人一份
					for (std::vector<UInt16>::iterator it = m_firsthit_dropid_list.begin(); it != m_firsthit_dropid_list.end(); ++it)
					{
						DROPPOOL->DropHelper(*it, m_scene, m_posi, temp_role->GetUserId(), temp_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
					}
					this->DropExtra(temp_role);

					this->AddAngryOrTireToRole(temp_role);
				}
			}
		}
		else
		{
			// 如果没有队伍的话就算自己的

			{
				//激活图鉴  Boss归属
				first_hit_role->GetBossCard()->CheckActiveCard(m_monster_id);
				this->AddAngryOrTireToRole(first_hit_role);
			}

			if (this->CanGetDropLevel(first_hit_role->GetLevel()))
			{
				for (std::vector<UInt16>::iterator it = m_firsthit_dropid_list.begin(); it != m_firsthit_dropid_list.end(); ++it)
				{
					DROPPOOL->DropHelper(*it, m_scene, m_posi, first_hit_role->GetUserId(), first_hit_role->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
				}
				this->DropExtra(first_hit_role);
			}
		}
	}
}

void Monster::DropToRole(UserID user_id)
{
	Role *role = m_scene->GetRoleByUID(UidToInt(user_id));
	if (role == nullptr) return;

	//激活图鉴  Boss归属
	role->GetBossCard()->CheckActiveCard(m_monster_id);
	if (!this->CanGetDropLevel(role->GetLevel()))
	{
		return;
	}

	if (!this->CanDropToRole(*role))
	{
		return;
	}

	for (std::vector<UInt16>::iterator it = m_dropid_list.begin(); it != m_dropid_list.end(); ++it)
	{
		DROPPOOL->DropHelper(*it, m_scene, m_posi, user_id, role->GetProf(), m_monster_id, true, false, &m_drop_index);
	}

	this->AddAngryOrTireToRole(role);

	//// 归属相关
	//if (this->IsBossMonster())
	//{
	//	if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_ROLE == m_drop_type || MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_TEAM == m_drop_type)
	//	{
	//BossFamilyManager::Instance().OnEventDropOwnerWithMonsterDie(role, this);
	//	}
	//}
}

bool Monster::CanDropToRole(Role &role)
{
	// 奇遇boss限制次数
	if (LOGIC_CONFIG->GetJinglingAdvantageCfg().IsJingLingAdvantageBoss(m_monster_id))
	{
		const JinglingAdvantageOtherCfg *cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();
		if (!cfg)
		{
			return false;
		}

		if (role.GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_JINGLING_ADVANTAGE_BOSS_KILL_COUNT) >= cfg->boss_max_drop_times)
		{
			role.NoticeNum(errornum::EN_JINGLING_ADVANTAGE_BOSS_LIMIT);
			return false;
		}
	}

	return true;
}

void Monster::DropToTeam(int team_index)
{
	Team *team = TeamManager::Instance().GetTeam(team_index);
	if (team == nullptr) return;

	Role *member_list[MAX_TEAM_MEMBER_NUM] = { NULL };
	int get_drop_count = 0;

	int member_uid_list[MAX_TEAM_MEMBER_NUM] = { 0 };
	int member_count = team->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);

	for (int j = 0; j < member_count && j < MAX_TEAM_MEMBER_NUM && get_drop_count < MAX_TEAM_MEMBER_NUM; ++j)
	{
		Role *temp_role = m_scene->GetRoleByUID(member_uid_list[j]);
		if (temp_role == nullptr) continue;
		//激活图鉴  Boss归属
		temp_role->GetBossCard()->CheckActiveCard(m_monster_id);

		if (m_role_hurt_info_map.find(member_uid_list[j])!= m_role_hurt_info_map.end() && this->CanGetDropLevel(temp_role->GetLevel()) && this->CanDropToRole(*temp_role))     // 有造成伤害的玩家
		{
			member_list[get_drop_count] = temp_role;
			get_drop_count++;
		}
	}

	std::set<int> drop_role_set;
	drop_role_set.clear();

	for (std::vector<UInt16>::iterator it = m_dropid_list.begin(); it != m_dropid_list.end(); ++it)
	{
		int drop_role_uid = this->DropToRandRole(*it, get_drop_count, member_list);
		if (0 != drop_role_uid)
		{
			drop_role_set.insert(drop_role_uid);
		}
	}

	for (int i = 0; i < get_drop_count && i < MAX_TEAM_MEMBER_NUM; i++)
	{
		Role *temp_role = member_list[i];
		if (NULL != temp_role)
		{
			if (drop_role_set.find(temp_role->GetUID()) == drop_role_set.end()) continue;
			this->AddAngryOrTireToRole(temp_role);
			this->DropExtra(temp_role);

			//// 归属相关
			//if (this->IsBossMonster())
			//{
			//	if (MonsterInitParam::BOSS_DROP_OWN_TYPE_MAX_HURT_TEAM == m_drop_type)
			//	{
			//BossFamilyManager::Instance().OnEventDropOwnerWithMonsterDie(temp_role, this);
			//	}
			//}
		}
	}
}

void Monster::DropToTeam(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM])
{
	int daycount_id = 0;
	int max_times = 0;

	if (Scene::SCENE_TYPE_TEAM_EQUIPFB == m_scene->GetSceneType())
	{
		daycount_id = DayCounter::DAYCOUNT_ID_TEAM_EQUIP_FB_JOIN_TIMES;
		max_times = LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().join_times;
	}
	else if (Scene::SCENE_TYPE_TEAM_TOWERDEFEND == m_scene->GetSceneType())
	{
		daycount_id = DayCounter::DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES;
		max_times = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().free_join_times;
	}

	if (member_count >= 0 && member_count < MAX_TEAM_MEMBER_NUM)	// 组队副本里面只按最后一刀掉落 每人一份
	{
		member_list[member_count++] = killer_role;                  // 外面传进来的是除自己外的队友

		for (int i = 0; i < member_count; i++)
		{
			if (NULL != member_list[i])
			{
				// 判断要用大于，因为一进去就扣除次数
				if (daycount_id > 0 && member_list[i]->GetDayCounter()->GetDayCount(daycount_id) > max_times) continue;

				for (std::vector<UInt16>::iterator it = m_dropid_list.begin(); it != m_dropid_list.end(); ++it)
				{
					DROPPOOL->DropHelper(*it, m_scene, m_posi, member_list[i]->GetUserId(), member_list[i]->GetProf(), m_monster_id, this->IsLogDrop(), false, &m_drop_index);
				}
				this->DropExtra(member_list[i]);
			}
		}
	}
}

bool Monster::CanGetDropLevel(int level)
{
	int min_lv = 0;
	int max_lv = 0;

	if (IsWorldBossMonster())     // 世界boss
	{
		const WorldBossInfoCfg * world_boss_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossInfo(m_monster_id);		if (world_boss_cfg != nullptr)		{			min_lv = world_boss_cfg->min_lv;			max_lv = world_boss_cfg->max_lv;		}
	}
	else if (Scene::IsBossDongKuScene(m_scene->GetSceneID()))
	{
		const BossFamilyMikuBossInfoCfg * miku_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetMikuBossInfo(m_monster_id);
		if (miku_boss_info_cfg != nullptr)
		{
			min_lv = miku_boss_info_cfg->min_lv;			max_lv = miku_boss_info_cfg->max_lv;
		}
	}
	else if (Scene::IsActiveBossScene(m_scene->GetSceneID()))
	{
		const ActiveBossInfoCfg * active_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetActiveBossInfo(m_monster_id);
		if (active_boss_info_cfg != nullptr)
		{
			min_lv = active_boss_info_cfg->min_lv;			max_lv = active_boss_info_cfg->max_lv;
		}

	}
	else if (Scene::IsBossFamilyScene(m_scene->GetSceneID()))
	{
		const BossFamilyBossInfoCfg * family_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyBossInfo(m_monster_id);
		if (family_boss_info_cfg != nullptr)
		{
			min_lv = family_boss_info_cfg->min_lv;			max_lv = family_boss_info_cfg->max_lv;
		}
	}

	if (max_lv != 0 && level > max_lv)
	{
		return false;
	}	if (min_lv != 0 && level < min_lv)
	{
		return false;
	}

	return true;
}

void Monster::AddAngryOrTireToRole(Role *role)
{
	if (role == nullptr) return;
	CommonDataParam *common_p = role->GetCommonDataParam();
	if (common_p == NULL) return;

	{
		//魔龙秘宝(极限挑战)
		role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_KILL_MONSTER_NUM, 1);
	}

	{
		if (Scene::IsBossFamilyScene(m_scene->GetSceneID()))
		{
			BossFamilyManager::Instance().MonsterBeKill(role, this);
			ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, role, role->GetLevel(), m_monster_id, "Family", NULL);
		}

		if (Scene::IsPerciousBossScene(m_scene->GetSceneID()))
		{
			BossFamilyManager::Instance().OnPreciousMonsterBeKill(this, role);
			ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, role, role->GetLevel(), m_monster_id, "Percious", NULL);
		}

		if (LOGIC_CONFIG->GetBossFaimlyConfig().IsYunyouBoss(m_monster_id))
		{
			BossFamilyManager::Instance().OnYunyouMonsterBeKill(this, role, m_scene->GetSceneID());
			ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, role, role->GetLevel(), m_monster_id, "Yunyou", NULL);
		}

		if (Scene::IsBossDongKuScene(m_scene->GetSceneID()))    //精英(洞窟)boss加疲劳
		{
			BossFamilyManager::Instance().MonsterBeKill(role, this);
			BossFamilyManager::Instance().OnMikuMonsterBeKill(this, role);
			ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, role, role->GetLevel(), m_monster_id, "DongKu", NULL);
		}

		if (m_scene->IsDabaoBossScene(m_scene->GetSceneID()))    // 套装（打宝）boss加愤怒值
		{
			const DabaoBossInfoCfg * cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetDabaoBossInfo(m_monster_id);
			if (nullptr != cfg)
			{
				common_p->dabao_angry_value += cfg->kill_boss_value;
				const BossFamilyOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
				if (common_p->dabao_angry_value >= other_cfg.max_value)
				{
					common_p->dabao_kick_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.dabao_kick_delay_time;
				}

				BossFamilyManager::Instance().SendDabaoBossInfo(role);
			}
			BossFamilyManager::Instance().OnDabaoMonsterBeKill(this, role);
			ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, role, role->GetLevel(), m_monster_id, "Dabao", NULL);
		}

		if (m_scene->IsActiveBossScene(m_scene->GetSceneID()))         // 活跃boss加疲劳
		{
			BossFamilyManager::Instance().OnActiveMonsterBeKill(this, role);
			ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, role, role->GetLevel(), m_monster_id, "Active", NULL);
		}
	}

	if (m_monster_type == MonsterInitParam::MONSTER_TYPE_BOSS)
	{
		if (Scene::IsBossDongKuScene(m_scene->GetSceneID()))    //精英(洞窟)boss加疲劳
		{
			//BossFamilyManager::Instance().MonsterDie(role, this);
			//BossFamilyManager::Instance().OnMikuMonsterDie(this, role);
			//ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, role, role->GetLevel(), m_monster_id, "DongKu", NULL);

			role->AddKillBossNum(MonsterInitParam::BOSS_TYPE_TASK_MIKU, 1);

			const BossFamilyMikuBossInfoCfg * miku_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetMikuBossInfo(m_monster_id);
			if ((nullptr != miku_boss_info_cfg) && (1 != miku_boss_info_cfg->is_peace))  // 不是和平层才能增加疲劳
			{
				role->GetTouzijihua()->OnKillBoss();
				const WearyCfg *weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_MIKU_WEARY);
				if (weary_cfg == NULL) return;
				role->AddMikuBossWeary(weary_cfg->kill_add_weary);				//增加疲劳
			}
		}
		else if (Scene::SCENE_TYPE_BABY_BOSS == m_scene->GetSceneType())      // 宝宝boss加愤怒值
		{
			SpecialBabyBoss *baby_boss = dynamic_cast<SpecialBabyBoss*>(m_scene->GetSpecialLogic());
			if (baby_boss != nullptr)
			{
				baby_boss->AddAngryValueToDropOwner(role, m_monster_id);
			}
		}
		else if (m_scene->GetSceneType() == Scene::SCENE_TYPE_SHANGGU_BOSS)    // 上古boss加愤怒值
		{
			SpecialShangGuBoss *shanggu_boss = dynamic_cast<SpecialShangGuBoss*>(m_scene->GetSpecialLogic());
			if (shanggu_boss != nullptr)
			{
				shanggu_boss->AddAngryValueToDropOwner(role, m_monster_id);
			}
		}
		else if (m_scene->IsDabaoBossScene(m_scene->GetSceneID()))    // 套装（打宝）boss加愤怒值
		{
			//BossFamilyManager::Instance().OnDabaoMonsterDie(this, role);
			//ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, role, role->GetLevel(), m_monster_id, "Dabao", NULL);
			//const BossFamilyOtherCfg cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
		}
 		else if (m_scene->IsActiveBossScene(m_scene->GetSceneID()))         // 活跃boss加疲劳
 		{
			//BossFamilyManager::Instance().OnActiveMonsterDie(this, role);
			//ROLE_LOG_QUICK6(LOG_TYPE_KILL_BOSS, role, role->GetLevel(), m_monster_id, "Active", NULL);

			role->AddKillBossNum(MonsterInitParam::BOSS_TYPE_TASK_ACTIVE, 1);

 			const WearyCfg *weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY);
 			if (weary_cfg == NULL) return;
			role->AddActiveBossWeary(weary_cfg->kill_add_weary);
 		}
		else if (m_scene->GetSceneType() == Scene::SCENE_TYPE_CROSS_BOSS)   // 跨服boss加疲劳
		{
			SpecialCrossBoss *cross_boss = dynamic_cast<SpecialCrossBoss*>(m_scene->GetSpecialLogic());
			if (cross_boss != nullptr)
			{
				cross_boss->AddCrossBossTireToDropOwn(role);
				role->GetTouzijihua()->OnKillShenyuBoss();
			}
		}
		else if (m_scene->GetSceneType() == Scene::SCENE_TYPE_CROSS_MIZANG_BOSS)
		{
			SpecialCrossMiZangBoss *cross_boss = dynamic_cast<SpecialCrossMiZangBoss*>(m_scene->GetSpecialLogic());
			if (cross_boss != nullptr)
			{
				cross_boss->AddCrossBossTireToDropOwn(role);
			}
		}
		else if (m_scene->GetSceneType() == Scene::SCENE_TYPE_CROSS_YOUMING_BOSS)
		{
			SpecialCrossYouMingBoss *cross_boss = dynamic_cast<SpecialCrossYouMingBoss*>(m_scene->GetSpecialLogic());
			if (cross_boss != nullptr)
			{
				cross_boss->AddCrossBossTireToDropOwn(role);
			}
		}
		else if (m_scene->GetSceneType() == Scene::SCENE_TYPE_GODMAGIC_BOSS)
		{
			SpecialGodmagicBoss * godmagic_boss = dynamic_cast<SpecialGodmagicBoss*>(m_scene->GetSpecialLogic());
			if (godmagic_boss != nullptr)
			{
				godmagic_boss->AddGodmagicBossTireToDropOwn(role);
			}
		}

		// 仙盟工资
		WorldStatus::Instance().AddGuildGongziByMonster(role, m_monster_id);

		// 奇遇boss限制次数
		if (LOGIC_CONFIG->GetJinglingAdvantageCfg().IsJingLingAdvantageBoss(m_monster_id))
		{
			const JinglingAdvantageOtherCfg *cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();
			if (nullptr != cfg)
			{
				if (role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_JINGLING_ADVANTAGE_BOSS_KILL_COUNT) >= cfg->boss_max_drop_times)
				{
					role->NoticeNum(errornum::EN_JINGLING_ADVANTAGE_BOSS_LIMIT);
				}
				else
				{
					role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JINGLING_ADVANTAGE_BOSS_KILL_COUNT);
				}
			}
		}
	}

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_KILL_BOSS_TYPE);
}

void Monster::NoticeHurtRoleWhenDie(Role * killer)
{
	if (killer == nullptr) return;
	static Protocol::SCNoticeBossDead nbd;
	nbd.boss_id = m_monster_id;
	nbd.killer_uid = killer->GetUID();
	nbd.killer_avatar_timestamp = killer->GetAvatarTimeStamp();

	for (RoleHurtInfoMapIt it = m_role_hurt_info_map.begin(); it != m_role_hurt_info_map.end(); ++it)
	{
		Obj *obj = m_scene->GetObj(it->second.objid);
		if (obj != nullptr && obj->GetObjType() == Obj::OBJ_TYPE_ROLE)
		{
			Role *role = (Role *)obj;
			EngineAdapter::Instance().NetSend(role->GetNetId(), (char *)&nbd, sizeof(nbd));
		}
	}
}
