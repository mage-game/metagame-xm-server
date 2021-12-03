#include "monstergenerater.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "engineadapter.h"
#include "internalcomm.h"

#include "scene/scene.h"
#include "world.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/guajiactivityconfig.hpp"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfb.h"
#include "config/activityconfig/babybossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialbabyboss.hpp"

MonsterGenerater::MonsterGenerater(Scene *scene)
	: m_is_create(true), m_cur_total_monster(0), m_max_total_monster(-1), m_max_monster_count(0), m_cur_monster_count(0), m_create_interval(0), 
	m_cur_time(0), m_scene(scene), m_pos(0, 0), m_monster_id(0), m_is_pause(false), m_add_maxhp(0), m_add_gongji(0), m_add_fangyu(0),
	m_had_update(false), m_need_adjust(false), m_is_baby_boss(false)
{

}

MonsterGenerater::~MonsterGenerater()
{

}

bool MonsterGenerater::Init(int max_total_monster, int max, unsigned long interval, const Posi &pos, UInt16 monster_id)
{
	m_max_total_monster = max_total_monster;
	m_max_monster_count = max;
	m_create_interval = interval;
	m_cur_time = interval;
	m_pos = pos;
	m_monster_id = monster_id;
	return true;
}

void MonsterGenerater::Update(unsigned long interval)
{
	if (!m_had_update)
	{
		if (nullptr != m_scene)
		{
			m_refresh_adjust_obj.refresh_id = m_monster_id;
			m_refresh_adjust_obj.scene_id = m_scene->GetSceneID();
			m_refresh_adjust_obj.refresh_type = RefreshAdjustKeyObj::REFRESH_OBJ_TYPE_COMMON_MONSTER;

			m_need_adjust = LOGIC_CONFIG->GetDynamicAdjustConfig().IsAdjustRefreshTime(m_refresh_adjust_obj);

			const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(m_monster_id);
			if (nullptr != monster_param && MonsterInitParam::MONSTER_TYPE_BOSS == monster_param->monster_type)
			{
				m_is_baby_boss = LOGIC_CONFIG->GetBabyBossConfig().IsBabyBoss(m_scene->GetSceneID(), m_scene->GetSceneType(), m_monster_id);
			}
			else
			{
				m_is_baby_boss = false;
			}
		}

		m_had_update = true;
	}

	if (!m_is_create || m_is_pause) return;
	
	if (m_cur_monster_count < m_max_monster_count)
	{
		if (m_cur_time < m_create_interval)
		{
			m_cur_time += interval;
		}
		else
		{
			int add_maxhp = 0, add_gongji = 0; int fangyu = 0;
			this->CalcMonsterAddAttrs(&add_maxhp, &add_gongji, &fangyu);

			Monster *monster = MONSTERPOOL->CreateMonster(m_monster_id, m_scene, m_pos, add_maxhp, add_gongji, fangyu);
			if (0 == monster)
			{
				m_is_create = false;
				return;
			}

			monster->SetGenerater(this);

			m_cur_time = 0;
			++m_cur_monster_count; ++m_cur_total_monster;

			if (m_max_total_monster != 0 && m_cur_total_monster >= m_max_total_monster)
			{
				m_is_create = false;
			}

			if (NULL != monster && monster->IsWorldBossMonster() && LOGIC_CONFIG->GetGuajiActivityConfig().IsGuaJiSceneID(m_scene->GetSceneID()))
			{

				Protocol::SCGuajiBossBornNotice gjbbn;
				gjbbn.boss_id = monster->GetMonsterId();
				gjbbn.scene_id = m_scene->GetSceneID();
				gjbbn.pos_x = monster->GetPos().x;
				gjbbn.pos_y = monster->GetPos().y;
				World::GetInstWorld()->SendToAllGateway((const char*)&gjbbn, sizeof(gjbbn));
			}

			if (m_is_baby_boss)
			{
				static Protocol::SCSingleBabyBossInfo pro;
				pro.boss_info.boss_id = monster->GetMonsterId();
				pro.boss_info.next_refresh_time = 0;
				pro.boss_info.scene_id = m_scene->GetSceneID();
				SpecialBabyBoss *special = (SpecialBabyBoss *)m_scene->GetSpecialLogic();
				memset(pro.boss_info.killer_info_list, 0, sizeof(pro.boss_info.killer_info_list));
				if (nullptr != special)
				{
					special->GetKillerInfo(m_monster_id, pro.boss_info.killer_info_list, Protocol::BabyBossInfo::KILLER_LIST_MAX_COUNT);
				}
				World::GetInstWorld()->SendToAllGateway((char *)&pro, sizeof(pro));
			}
		}
	}
}

void MonsterGenerater::MonsterDie(Monster *monster)
{
	--m_cur_monster_count;
	if (m_cur_monster_count < 0) m_cur_monster_count = 0;

	// 调整刷新时间
	if (m_need_adjust)
	{
		int adjust_interval = LOGIC_CONFIG->GetDynamicAdjustConfig().GetRefreshAdjustInterval(m_refresh_adjust_obj, m_scene->RoleNum());
		if (adjust_interval > 0)
		{
			m_create_interval = adjust_interval;
		}
	}
}

void MonsterGenerater::Reset()
{	
	m_cur_total_monster = 0, m_cur_monster_count = 0;
	m_is_create = true;
}

void MonsterGenerater::CalcMonsterAddAttrs(int *add_maxhp, int *add_gongji, int *add_fangyu)
{
	if (NULL == add_maxhp || NULL == add_gongji || NULL == add_fangyu || NULL == m_scene) return;

	*add_maxhp = m_add_maxhp; *add_gongji = m_add_gongji; *add_fangyu = m_add_fangyu;
}

void MonsterGenerater::CrossGuildBattleSetAddAttrs(int level)
{
	const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(m_monster_id);
	if (NULL != monster_param)
	{
		m_add_maxhp = (int)(monster_param->hp * (0.00009f * level * level - 0.06f * level + 9.975f));
	}
}

void MonsterGenerater::RefreshNow()
{
	m_cur_time = m_create_interval;
}

unsigned int MonsterGenerater::GetMonsterNextFlushTime()
{
	if ((m_cur_time <= 0 && m_cur_monster_count >= m_max_monster_count) || m_cur_time >= m_create_interval)
	{
		return 0;
	}

	int next_flush_interval = m_create_interval - m_cur_time;
	return static_cast<unsigned int>(EngineAdapter::Instance().Time() + next_flush_interval / 1000);
}

void MonsterGenerater::SetPause(bool is_pause)
{
	m_is_pause = is_pause;
}

