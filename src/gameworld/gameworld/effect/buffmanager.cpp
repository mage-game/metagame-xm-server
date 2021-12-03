#include "buffmanager.hpp"

//#include "protocal/msgfight.h"
//#include "obj/character/character.h"
//#include "scene/scene.h"
//
//#include "gamelog.h"
//
//#define LL_ONE_BIT ((long long)0x0000000000000001)
//
//BuffManager::BuffManager()
//	: m_character(NULL), m_buff_mark(0)
//{
//	memset(m_buff_count, 0, sizeof(m_buff_count));
//}
//
//BuffManager::~BuffManager()
//{
//}
//
//void BuffManager::Init(Character *chara, unsigned long long buff_mark, const char buff_count[RoleOtherInitParam::MAX_BUFF_MARK_BIT])
//{
//	UNSTD_STATIC_CHECK(BT_COUNT < RoleOtherInitParam::MAX_BUFF_MARK_BIT);
//
//	m_character = chara;
//
//	if (0 != buff_mark && NULL != m_buff_count)
//	{
//		m_buff_mark = buff_mark;
//		memcpy(m_buff_count, buff_count, sizeof(m_buff_count));
//
//		for (int i = 0; i < BT_COUNT; ++ i)
//		{
//			if ((this->HasBuff(i) && 0 == m_buff_count[i]) || (!this->HasBuff(i) && 0 != m_buff_count[i]))
//			{
//				m_buff_mark &= ~(LL_ONE_BIT << i);
//				m_buff_count[i] = 0;
//
//				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[BuffManager::Init] [buff_mark bit %d mismatch with buff_count]", i);
//			}
//		}
//	}
//}
//
//void BuffManager::GetInitParam(char buff_count[RoleOtherInitParam::MAX_BUFF_MARK_BIT])
//{
//	if (NULL == buff_count) return;
//	
//	memcpy(buff_count, m_buff_count, sizeof(m_buff_count));
//}
//
//bool BuffManager::HasBuff(int buff_type)
//{
//	if (buff_type <= BT_INVALID || buff_type >= RoleOtherInitParam::MAX_BUFF_MARK_BIT) return false;
//	return ((m_buff_mark >> buff_type) & LL_ONE_BIT);
//}
//
//bool BuffManager::AddBuff(int buff_type)
//{
//	if (buff_type <= BT_INVALID || buff_type >= RoleOtherInitParam::MAX_BUFF_MARK_BIT) return false;
//
//	if (0 == m_buff_count[buff_type])
//	{
//		m_buff_mark |= (LL_ONE_BIT << buff_type);
//
//		Protocol::SCBuffAdd ba;
//		ba.buff_type = (short)buff_type;
//		ba.obj_id = m_character->GetId();
//		m_character->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_character->GetObserHandle(), (void *)&ba, sizeof(Protocol::SCBuffAdd));
//	}
//
//	++ m_buff_count[buff_type];
//
//	return true;
//}
//
//bool BuffManager::RemoveBuff(int buff_type)
//{
//	if (buff_type <= BT_INVALID || buff_type >= RoleOtherInitParam::MAX_BUFF_MARK_BIT) return false;
//
//	-- m_buff_count[buff_type];
//
//	if (m_buff_count[buff_type] <= 0)
//	{
//		m_buff_mark &= ~(LL_ONE_BIT << buff_type);
//
//		Protocol::SCBuffRemove br;
//		br.buff_type = (short)buff_type;
//		br.obj_id = m_character->GetId();
//		m_character->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_character->GetObserHandle(), (void *)&br, sizeof(Protocol::SCBuffRemove));
//	}
//
//	return true;
//}
//
//void BuffManager::ClearBuff()
//{
//	m_buff_mark = 0;
//	memset(m_buff_count, 0, sizeof(m_buff_count));
//}
//
//void BuffManager::ClearFightBuff(bool clear_skill_buff, bool clear_scene_buff)
//{
//	for (int buff_type = BT_INVALID + 1; buff_type < RoleOtherInitParam::MAX_BUFF_MARK_BIT; ++buff_type)
//	{
//		if (0 != m_buff_count[buff_type] && ((clear_skill_buff && BuffManager::IsSkillBuff(buff_type)) || (clear_scene_buff && BuffManager::IsSceneBuff(buff_type))))
//		{
//			m_buff_mark &= ~(LL_ONE_BIT << buff_type);
//			m_buff_count[buff_type] = 0;
//		}
//	}
//}

