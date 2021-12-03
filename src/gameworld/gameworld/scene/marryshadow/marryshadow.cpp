#include "marryshadow.h"

#include "obj/character/marryobj.hpp"
#include "config/marryconfig.hpp"
#include "scene/speciallogic/worldspecial/specialhunyan.hpp"

#include "world.h"
#include "gamecommon.h"

#include "config/logicconfigmanager.hpp"

MarryShadow::MarryShadow()
	: m_cur_xunyou_point_index(0), m_marryobj_stop_time(0)
{
	for (int i = 0; i < MARRY_MARRYOBJ_MAX_NUM; i++)
	{
		m_marryobj_id_list[i] = INVALID_OBJ_ID;
	}
}

MarryShadow::~MarryShadow()
{
}

MarryShadow & MarryShadow::Instance()
{
	static MarryShadow shadow;
	return shadow;
}

void MarryShadow::OnSyncMarryInfo(const SyncMarryInfo &marry_info)
{
	int old_status = m_marry_info.marry_status;

	m_marry_info = marry_info;

	if (old_status != m_marry_info.marry_status)
	{
		this->OnChangeToStatus(m_marry_info.marry_status);
	}
}

void MarryShadow::Update(unsigned long interval)
{
// 	if (MARRY_STATUS_XUN_YOU == m_marry_info.marry_status && m_cur_xunyou_point_index < LOGIC_CONFIG->GetMarryConfig().GetMarryXunyouPointCount())
// 	{
// 		Scene *scene = this->GetMarryScene();
// 		if (NULL != scene)
// 		{
// 			Obj *obj = scene->GetObj(m_marryobj_id_list[0]);
// 			if (NULL != obj && Obj::OBJ_TYPE_MARRY == obj->GetObjType())
// 			{
// 				// 巡游时候 需要定期移动
// 				MarryObj *marryobj = (MarryObj*)obj;
// 				if (marryobj->IsStatic())
// 				{
// 					m_marryobj_stop_time += interval;
// 					unsigned long point_stop_time = LOGIC_CONFIG->GetMarryConfig().GetMarryXunyouPointStopTimeS(m_cur_xunyou_point_index) * 1000;
// 					if (m_marryobj_stop_time >= point_stop_time)
// 					{
// 						Posi deltaP = LOGIC_CONFIG->GetMarryConfig().GetMarryXunyouPointPos(m_cur_xunyou_point_index) - marryobj->GetPos();
// 						Scalar distance = 0;
// 						Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
// 						marryobj->SetDir(dir, distance);
// 
// 						for (int i = 1; i < MARRY_MARRYOBJ_MAX_NUM; i++)
// 						{
// 							Obj *obj = scene->GetObj(m_marryobj_id_list[i]);
// 							if (NULL != obj && Obj::OBJ_TYPE_MARRY == obj->GetObjType())
// 							{
// 								MarryObj *other_marryobj = (MarryObj*)obj;
// 								other_marryobj->OnFirstMarryobjMove();
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
}

void MarryShadow::OnCharacterMoveStop(Character *cha)
{
	if (NULL == cha) return;

	if (Obj::OBJ_TYPE_MARRY == cha->GetObjType())
	{
		MarryObj *marryobj = (MarryObj*)cha;
		if (marryobj->IsFirstMarryobj())
		{
			++ m_cur_xunyou_point_index;
			m_marryobj_stop_time = 0;

			Scene *scene = this->GetMarryScene();
			if (NULL != scene)
			{
				for (int i = 1; i < MARRY_MARRYOBJ_MAX_NUM; i++)
				{
					Obj *obj = scene->GetObj(m_marryobj_id_list[i]);
					if (NULL != obj && Obj::OBJ_TYPE_MARRY == obj->GetObjType())
					{
						MarryObj *other_marryobj = (MarryObj*)obj;
						other_marryobj->OnFirstMarryobjMoveStop();
					}
				}
			}
		}
	}
}

bool MarryShadow::CanMarryXunyouOpera(const UserID &user_id)
{
	if (MARRY_STATUS_XUN_YOU != m_marry_info.marry_status) return false;

	for (int i = 0; i < MARRY_COUPLE_USER_LIMIT_NUM; i++)
	{
		if (m_marry_info.uid_list[i] == UidToInt(user_id))
		{
			return true;
		}
	}

	return false;
}

bool MarryShadow::IsMarryUser(const UserID &user_id)
{
	if (MARRY_STATUS_INVALID == m_marry_info.marry_status || MARRY_STATUS_FINISH == m_marry_info.marry_status) 
	{
		return false;
	}

	for (int i = 0; i < MARRY_COUPLE_USER_LIMIT_NUM; i++)
	{
		if (m_marry_info.uid_list[i] == UidToInt(user_id))
		{
			return true;
		}
	}

	return false;
}

bool MarryShadow::GetMarryobjPos(int *pos_x, int *pos_y)
{
	if (MarryShadow::Instance().GetMarryStatus() < MARRY_STATUS_BAI_TIAN_DI_ZHUN_BEI || MarryShadow::Instance().GetMarryStatus() > MARRY_STATUS_XUN_YOU)
	{
		return false;
	}

	if (NULL != pos_x) *pos_x = m_marry_info.marryobjpos_x;
	if (NULL != pos_y) *pos_y = m_marry_info.marryobjpos_y;

	return true;
}

bool MarryShadow::GetMarrySceneId(int *scene_id)
{
	if (MarryShadow::Instance().GetMarryStatus() < MARRY_STATUS_BAI_TIAN_DI_ZHUN_BEI || MarryShadow::Instance().GetMarryStatus() > MARRY_STATUS_XUN_YOU)
	{
		return false;
	}

	if (NULL != scene_id) *scene_id = LOGIC_CONFIG->GetMarryConfig().GetMarrySceneId();

	return true;
}

Scene * MarryShadow::GetMarryScene()
{
	return World::GetInstWorld()->GetSceneManager()->GetSceneById(LOGIC_CONFIG->GetMarryConfig().GetMarrySceneId(), 0);
}

void MarryShadow::OnChangeToStatus(int to_status)
{
	switch (to_status)
	{
	case MARRY_STATUS_BAI_TIAN_DI_ZHUN_BEI:
		{
			this->OnMarryStatusToBaiTianDiZhunBei();
		}
		break; 

	case MARRY_STATUS_XUN_YOU:
		{
			this->OnMarryStatusToXunYou();
		}
		break; 

	case MARRY_STATUS_HUN_YAN_ZHUN_BEI:
		{
			this->OnMarryStatusToHunYanZhunBei();
		}
		break;

	case MARRY_STATUS_HUN_YAN:
		{
			this->OnMarryStatusToHunYan();
		}
		break;

	case MARRY_STATUS_FINISH:
		{
			this->OnMarryStatusToFinish();
		}
		break;
	}

	SpecialHunyan *special_hunyan = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(MARRY_HUNYAN_SCENE_KEY, true);
	if (NULL != special_hunyan)
	{

	}
}

void MarryShadow::OnMarryStatusToBaiTianDiZhunBei()
{
	for (int i = 0; i < MARRY_MARRYOBJ_MAX_NUM; i++)
	{
		m_marryobj_id_list[i] = INVALID_OBJ_ID;
	}

	// 1: 创建一个结婚对象
	Scene *scene = this->GetMarryScene();
	if (NULL != scene)
	{
		MarryObj *marry_obj = new MarryObj();

		marry_obj->SetPos(LOGIC_CONFIG->GetMarryConfig().GetMarryBaiTianDiPos());
		marry_obj->Init(0);
		scene->AddObj(marry_obj);

		m_marryobj_id_list[0] = marry_obj->GetId();
	}
	
	// 2: 客户端隐藏掉对应的玩家
}

void MarryShadow::OnMarryStatusToXunYou()
{
	Posi bai_tiandi_pos = LOGIC_CONFIG->GetMarryConfig().GetMarryBaiTianDiPos();
	Posi deltaP = bai_tiandi_pos - LOGIC_CONFIG->GetMarryConfig().GetMarryXunyouPointPos(0);
	Scalar distance = 0;
	Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
	
	Scene *scene = this->GetMarryScene();
	if (NULL != scene)
	{
		int MARRYOBJ_COUNT = LOGIC_CONFIG->GetMarryConfig().GetMarryXunyouMarryobjCount();
		for (int i = 1; i < MARRYOBJ_COUNT && i < MARRY_MARRYOBJ_MAX_NUM; i++)
		{
			if (INVALID_OBJ_ID == m_marryobj_id_list[i])
			{
				MarryObj *marry_obj = new MarryObj();

				int marry_xunyou_marryobj_dis = LOGIC_CONFIG->GetMarryConfig().GetMarryXunyouMarryobjDis(i);

				Posi pos = bai_tiandi_pos;
				pos.x = (int)(pos.x + (marry_xunyou_marryobj_dis * i * cos(dir)));
				pos.y = (int)(pos.y + (marry_xunyou_marryobj_dis * i * sin(dir)));

				marry_obj->SetPos(pos);
				marry_obj->Init(i);
				scene->AddObj(marry_obj);

				m_marryobj_id_list[i] = marry_obj->GetId();
			}
		}
	}
}

void MarryShadow::OnMarryStatusToHunYanZhunBei()
{
	Scene *scene = this->GetMarryScene();
	if (NULL != scene)
	{
		for (int i = 0; i < MARRY_MARRYOBJ_MAX_NUM; i++)
		{
			Obj *obj = scene->GetObj(m_marryobj_id_list[i]);
			if (NULL != obj && Obj::OBJ_TYPE_MARRY == obj->GetObjType())
			{
				scene->DeleteObj(m_marryobj_id_list[i]);
				m_marryobj_id_list[i] = INVALID_OBJ_ID;
			}
		}
	}
}

void MarryShadow::OnMarryStatusToHunYan()
{

}

void MarryShadow::OnMarryStatusToFinish()
{
	m_cur_xunyou_point_index = 0;
	m_marryobj_stop_time = 0;
	m_marry_info.Reset();
}

