#include "marryobj.hpp"

#include "engineadapter.h"
#include "internalcomm.h"
#include "gamecommon.h"
#include "world.h"

#include "scene/scene.h"
#include "scene/marryshadow/marryshadow.h"
#include "scene/speciallogic/worldspecial/specialhunyan.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "config/marryconfig.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"

#include "obj/character/role.h"

#include "config/logicconfigmanager.hpp"

MarryObj::MarryObj()
	: Character(OBJ_TYPE_MARRY), m_marry_seq(-1), m_last_sync_pos_time(0), m_cur_xunyou_point_index(0), 
	m_last_dir(0.0f), m_last_dir_ditance(0.0f)
{

}

MarryObj::~MarryObj()
{

}

void MarryObj::Init(int marry_seq)
{
	m_marry_seq = marry_seq;

	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();

	CharIntAttrs int_attribute;
	int_attribute.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] = 1;
	int_attribute.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP] = 1;
	int_attribute.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED] = hunyan_cfg.xunyou_marryobj_speed;

	SkillListParam skill_param; skill_param.count = 0;

	Character::Init(int_attribute, skill_param); 
}

void MarryObj::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	Character::Update(interval, now_second, now_dayid);

	if (this->IsFirstMarryobj())
	{
// 		if (MARRY_STATUS_BAI_TIAN_DI_ZHUN_BEI <= MarryShadow::Instance().GetMarryStatus() ||
// 			MarryShadow::Instance().GetMarryStatus() <= MARRY_STATUS_XUN_YOU)
// 		{
// 			if (now_second >= m_last_sync_pos_time + 5)
// 			{
// 				m_last_sync_pos_time = now_second;
// 
// 				this->SyncPosToGlobal();
// 			}
// 		}
	}
	else
	{
	
		int hunyan_status = HUNYAN_STATE_TYPE_INVALID;
		Posi pre_obj_pos(0, 0);

		ObjID obj_id = INVALID_OBJ_ID;
		SpecialHunyan *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, false);
		if (NULL != hunyan_scene)
		{
			hunyan_status = hunyan_scene->GetHunyanState();
			pre_obj_pos = hunyan_scene->GetXunyouObjCurPos(m_marry_seq - 1, obj_id);
		}

		if (HUNYAN_STATE_TYPE_XUNYOU == hunyan_status)
		{
			if (this->IsStatic() && Dir_S == m_last_dir)
			{
				if (m_cur_xunyou_point_index < LOGIC_CONFIG->GetQingyuanCfg().GetXunyouPosCount())
				{
					Posi next_xunyou_pos = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouPos(m_cur_xunyou_point_index);
					Posi pre_deltaP = next_xunyou_pos - pre_obj_pos;
					Scalar pre_distance = 0;
					Dir pre_dir = gamecommon::GetMoveDir(pre_deltaP, &pre_distance);

					int dis = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouObjDis(m_marry_seq);

					int pos_x = (int)(pre_obj_pos.x - (dis * cos(pre_dir)));
					int pos_y = (int)(pre_obj_pos.y - (dis * sin(pre_dir)));

					// 因每次update会产生时间差，故需要修正obj间的间距
					this->SetPos(Posi(pos_x, pos_y));

					Posi deltaP = next_xunyou_pos - this->GetPos();
					Scalar distance = 0;
					Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
					this->SetDir(dir, distance);

					++ m_cur_xunyou_point_index;
				}
			}
		}
	}
}

void MarryObj::OnFirstMarryobjMove()
{
	if (this->IsFirstMarryobj()) return;
	
	if (Dir_S != m_last_dir)
	{
		this->SetDir(m_last_dir, m_last_dir_ditance);
		m_last_dir = Dir_S;
		m_last_dir_ditance = 0.0f;
	}
}

void MarryObj::OnFirstMarryobjMoveStop()
{
	if (this->IsFirstMarryobj()) return;

	m_last_dir = m_dir;
	m_last_dir_ditance = m_dir_distance;

	if (m_cur_xunyou_point_index == LOGIC_CONFIG->GetQingyuanCfg().GetXunyouPosCount())
	{
		this->ActionStop();
	}
}

void MarryObj::SyncPosToGlobal()
{
	/*static gglobalprotocal::MarryobjSyncPosToGlobal msptg;
	msptg.pos_x = m_posi.x;
	msptg.pos_y = m_posi.y;
	InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char *)&msptg, sizeof(msptg));*/
}
