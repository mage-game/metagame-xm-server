#include <assert.h>
#include <memory.h>
#include "gamecommon.h"
#include "zonematrix.h"
#include "zone.h"
#include "obj/obj.h"
#include "obj/objmanager.h"
#include "obj/character/role.h"

#include "obj/character/character.h"

#include "engineadapter.h"
#include "globalconfig/globalconfig.h"

void ZoneMatrix::CreateZoneMatrix(ZoneMatrix *z, Axis CX, Axis CY, Axis per_x, Axis per_y, ObjManager *obj_manager)
{
	assert(CX != 0);
	assert(CY != 0);
	assert(per_x != 0);
	assert(per_y != 0);

	z->m_matrix_x_count = (CX - 1) / per_x + 1;
	z->m_matrix_y_count = (CY - 1) / per_y + 1;
	z->m_zone_per_x = per_x;
	z->m_zone_per_y = per_y;
	z->m_cx = CX;
	z->m_cy = CY;
	z->m_obj_manager = obj_manager;

	z->m_zone_matrix = (Zone**)malloc(sizeof(Zone*) * z->m_matrix_x_count);
	memset(z->m_zone_matrix, 0, sizeof(Zone*) * z->m_matrix_x_count);

	Axis cur_ld_x = 0;
	for (unsigned int i = 0; i < z->m_matrix_x_count; ++i, cur_ld_x += per_x)
	{
		z->m_zone_matrix[i] = new Zone[z->m_matrix_y_count];

		Axis cur_ld_y = 0;
		for (unsigned int j = 0; j < z->m_matrix_y_count; ++j, cur_ld_y += per_y)
		{
			// 此处不处理最后一个半块的情况
			z->m_zone_matrix[i][j].Init(cur_ld_x, cur_ld_y, per_x, per_y);
		}
	}
}

void ZoneMatrix::DestriyZoneMatrix()
{
	for (unsigned int i = 0; i < m_matrix_x_count; ++i)
	{
		delete [](m_zone_matrix[i]);
	}

	if (m_zone_matrix != 0)
	{
		free(m_zone_matrix);
		m_zone_matrix = 0;
	}

	for (ObserPool::Iterator i = m_obser_pool.Beg(); i != m_obser_pool.End(); ++i)
	{
		delete (*i);
	}
	m_obser_pool.Clear();

	m_zone_per_x = 0;
	m_zone_per_y = 0;
	m_matrix_x_count = 0;
	m_matrix_y_count = 0;
	m_cx = 0;
	m_cy = 0;
	m_obj_manager = 0;
}

UInt32 ZoneMatrix::CreateObser(ObjID obj_id)
{
	ObserItem *obser = new ObserItem();
	obser->obj_id = obj_id;
	UInt32 obser_handle = m_obser_pool.Insert(obser);

	// 注册到对应zone的被观察者列表中
	Obj *obj = m_obj_manager->GetObj(obj_id);
	if (obj == 0) return -1;

	obser->pos = obj->GetPos();

	Posu zone_local;
	if (!CheckZone(obser->pos, &zone_local))
	{
		obj->SetPos(Posi(0, 0));
		zone_local.Init(0, 0);
		obser->pos.Init(0, 0);
	}
	obser->be_obs_local = zone_local;
	obser->be_obs_index = m_zone_matrix[zone_local.x][zone_local.y].AddObser(obj_id);

	// 触发AOIEnter消息
	Zone *zone = &m_zone_matrix[obser->be_obs_local.x][obser->be_obs_local.y];
	int aoi_size = zone->AOISize();
	for (int i = 0; i < aoi_size; ++i)
	{
		AOI *aoi = &m_aoi_pool[zone->GetAOI(i)];

		if (aoi->IsIn(obser->pos))
		{
			Obj *aoi_obj_tmp = m_obj_manager->GetObj(aoi->obj_id);
			if (aoi_obj_tmp != 0)
			{
				aoi_obj_tmp->OnAOIEnter(obser->obj_id);
			}
		}
	}

	return obser_handle;
}

void ZoneMatrix::DestroyObser(UInt32 obser_handle)
{
	if (!m_obser_pool.Exist(obser_handle))
	{
		return;
	}
	ObserItem *obser = m_obser_pool[obser_handle];
	
	// 触发AOILeave消息
	Zone *zone = &m_zone_matrix[obser->be_obs_local.x][obser->be_obs_local.y];
	int aoi_size = zone->AOISize();
	for (int i = 0; i < aoi_size; ++i)
	{
		AOI *aoi = &m_aoi_pool[zone->GetAOI(i)];
		if (!aoi->IsOut(obser->pos))
		{
			Obj *aoi_obj_tmp = m_obj_manager->GetObj(aoi->obj_id);
			if (aoi_obj_tmp != 0)
			{
				aoi_obj_tmp->OnAOILeave(obser->obj_id);
			}
		}
	}

	// 从对应zone的被观察者列表中移除
	m_zone_matrix[obser->be_obs_local.x][obser->be_obs_local.y].EraseObser(obser->be_obs_index);

	delete obser;
	m_obser_pool.Erase(obser_handle);
}

void ZoneMatrix::MoveObser(UInt32 obser_handle)
{
	if (!m_obser_pool.Exist(obser_handle))
	{
		return;
	}

	ObserItem *obser = m_obser_pool[obser_handle];
	ObjID obj_id = obser->obj_id;
	Posu zone_local_old = obser->be_obs_local;
	Posi pos_old = obser->pos;

	Obj *obj = m_obj_manager->GetObj(obj_id);
	if (obj == 0) return ;

	obser->pos = obj->GetPos();

	if (pos_old == obser->pos) return;

	Posu zone_local;
	// 可以先检测Zone::IsInZone后再CheckZone
	if (CheckZone(obser->pos, &zone_local))
	{
		if (zone_local != obser->be_obs_local)
		{
			m_zone_matrix[obser->be_obs_local.x][obser->be_obs_local.y].EraseObser(obser->be_obs_index);

			obser->be_obs_local = zone_local;
			obser->be_obs_index = m_zone_matrix[zone_local.x][zone_local.y].AddObser(obj_id);
		}
	}


	// 触发AOILeave消息
	Zone *zone = &m_zone_matrix[zone_local_old.x][zone_local_old.y];
	int aoi_size = zone->AOISize();
	for (int i = 0; i < aoi_size; ++i)
	{
		AOI *aoi = &m_aoi_pool[zone->GetAOI(i)];

		if (aoi->obj_id == obj_id)
		{
			continue;
		}

		if (!aoi->IsOut(pos_old) && aoi->IsOut(obser->pos))
		{
			Obj *aoi_obj_temp = m_obj_manager->GetObj(aoi->obj_id);
			if (aoi_obj_temp != 0)
			{
				aoi_obj_temp->OnAOILeave(obser->obj_id);
			}
		}
	}

	// 触发AOIEnter消息
	zone = &m_zone_matrix[obser->be_obs_local.x][obser->be_obs_local.y];
	aoi_size = zone->AOISize();
	for (int i = 0; i < aoi_size; ++i)
	{
		AOI *aoi = &m_aoi_pool[zone->GetAOI(i)];
		if (aoi->obj_id == obj_id)
		{
			continue;
		}
		if (!aoi->IsIn(pos_old) && aoi->IsIn(obser->pos))
		{
			Obj *aoi_obj_temp = m_obj_manager->GetObj(aoi->obj_id);
			if (aoi_obj_temp != 0)
			{
				aoi_obj_temp->OnAOIEnter(obser->obj_id);
			}
		}
	}
}

UInt32 ZoneMatrix::CreateCircleAOI(ObjID obj_id, const Posi &centre, Axis enter_radius, Axis leave_radius)
{
	return CreateAOI(obj_id, true, centre, Posi(enter_radius, enter_radius), Posi(leave_radius, leave_radius));
}

UInt32 ZoneMatrix::CreateRectAOI(ObjID obj_id, const Posi &centre, const Posi &enter_r, const Posi &leave_r)
{
	return CreateAOI(obj_id, false, centre, enter_r, leave_r);
}

UInt32 ZoneMatrix::CreateAOI(ObjID obj_id, bool circle, const Posi &centre, const Posi &enter_radius, const Posi &leave_radius)
{
	Posi offset_centre(centre.x < leave_radius.x ? leave_radius.x : centre.x, centre.y < leave_radius.y ? leave_radius.y : centre.y);
	offset_centre.x = ((offset_centre.x > m_cx - leave_radius.x) ? m_cx - leave_radius.x : offset_centre.x);
	offset_centre.y = ((offset_centre.y > m_cy - leave_radius.y) ? m_cy - leave_radius.y : offset_centre.y);

	AOI aoi(obj_id, circle, offset_centre, enter_radius, leave_radius);
	UInt32 aoi_handler = m_aoi_pool.Insert(aoi);

	Posu zone_ld;
	CalcZone(offset_centre.x - leave_radius.x, offset_centre.y - leave_radius.y, &zone_ld);
	Posu zone_ru;
	CalcZone(offset_centre.x + leave_radius.x - 1, offset_centre.y + leave_radius.y - 1, &zone_ru);

	Obj *aoi_obj = m_obj_manager->GetObj(obj_id);
	if (aoi_obj == 0) return -1;
	
	for (unsigned int x = zone_ld.x; x <= zone_ru.x; ++x)
	{
		for (unsigned int y = zone_ld.y; y <= zone_ru.y; ++y)
		{
			Zone *zone_iter = &m_zone_matrix[x][y];
			zone_iter->AddAOI(aoi_handler);
			for (Zone::ObjInterator i = zone_iter->FirstObser(); i != zone_iter->LastObser(); ++i)
			{
				Obj *obj = m_obj_manager->GetObj(*i);
				if (obj != 0 && aoi.IsIn(obj->GetPos()) && obj_id != obj->GetId())
				{
					aoi_obj->OnAOIEnter(obj->GetId());
				}
			}
		}
	}

	return aoi_handler;
}

void ZoneMatrix::DestroyAOI(UInt32 aoi_handler)
{
	if (!m_aoi_pool.Exist(aoi_handler))
	{
		return ;
	}

	Posi centre = m_aoi_pool[aoi_handler].centre;
	Posi leave_radius = m_aoi_pool[aoi_handler].leave_radius;

	Posu zone_ld;
	CalcZone(centre.x - leave_radius.x, centre.y - leave_radius.y, &zone_ld);
	Posu zone_ru;
	CalcZone(centre.x + leave_radius.x - 1, centre.y + leave_radius.y - 1, &zone_ru);

	for (unsigned int x = zone_ld.x; x <= zone_ru.x; ++x)
	{
		for (unsigned int y = zone_ld.y; y <= zone_ru.y; ++y)
		{
			m_zone_matrix[x][y].EraseAOI(aoi_handler);
		}
	}

	m_aoi_pool.Erase(aoi_handler);
}

void ZoneMatrix::MoveAOI(UInt32 aoi_handler)
{
	if (!m_aoi_pool.Exist(aoi_handler))
	{
		return ;
	}

	AOI *aoi = &m_aoi_pool[aoi_handler];
	ObjID obj_id = aoi->obj_id;
	Obj *obj = m_obj_manager->GetObj(obj_id);
	if (obj == 0) return ;

	Posi new_centre = obj->GetPos();
	Posi leave_radius = aoi->leave_radius;

	(new_centre.x < leave_radius.x) ? (new_centre.x = leave_radius.x) : 0;
	(new_centre.y < leave_radius.y) ? (new_centre.y = leave_radius.y) : 0;
	(new_centre.x > m_cx - leave_radius.x) ? (new_centre.x = m_cx - leave_radius.x) : 0;
	(new_centre.y > m_cy - leave_radius.y) ? (new_centre.y = m_cy - leave_radius.y) : 0;

	static const int AOI_SENSITIVE_DISTANCE = (int)GLOBALCONFIG->GetKeyConfig_RoleAoiSensitiveDistance();	// AOI模块中对两次MoveAOI小于这个时不加处理
	if (!(new_centre - aoi->centre).IsLongerThan(AOI_SENSITIVE_DISTANCE))
	{
		return;
	}

	AOI aoi_old = *aoi;
	Posi centre = aoi->centre;

	Posu zone_ld_old, zone_ru_old;
	CalcZone(centre.x - leave_radius.x, centre.y - leave_radius.y, &zone_ld_old);
	CalcZone(centre.x + leave_radius.x - 1, centre.y + leave_radius.y - 1, &zone_ru_old);

	aoi->centre = new_centre;

	Posu zone_ld_new, zone_ru_new;
	CalcZone(new_centre.x - leave_radius.x, new_centre.y - leave_radius.y, &zone_ld_new);
	CalcZone(new_centre.x + leave_radius.x - 1, new_centre.y + leave_radius.y - 1, &zone_ru_new);

	bool zone_is_the_same = (zone_ld_old == zone_ld_new && zone_ru_old == zone_ru_new);

	for (unsigned int x = zone_ld_old.x; x <= zone_ru_old.x; ++x)
	{
		for (unsigned int y = zone_ld_old.y; y <= zone_ru_old.y; ++y)
		{
			bool is_in = false;
			if (zone_is_the_same)
			{
				is_in = true;
			}
			else
			{
				for (unsigned int x_new = zone_ld_new.x; x_new <= zone_ru_new.x; ++x_new)
				{
					for (unsigned int y_new = zone_ld_new.y; y_new <= zone_ru_new.y; ++y_new)
					{
						if (x == x_new && y == y_new)
						{
							is_in = true;
							break;
						}
					}
					if (is_in)
					{
						break;
					}
				}
			}

			Zone *zone = &m_zone_matrix[x][y];
			for (Zone::ObjInterator i = zone->FirstObser(); i != zone->LastObser(); ++i)
			{
				ObjID oid = *i;
				if (obj_id == oid)
				{
					continue;
				}
				Obj *obser = m_obj_manager->GetObj(oid);
				if (obser != 0 && aoi->IsOut(obser->GetPos()) && !aoi_old.IsOut(obser->GetPos()))
				{
					obj->OnAOILeave(oid);
				}
			}

			if (!is_in)
			{
				zone->EraseAOI(aoi_handler);
			}
		}
	}

	for (unsigned int x = zone_ld_new.x; x <= zone_ru_new.x; ++x)
	{
		for (unsigned int y = zone_ld_new.y; y <= zone_ru_new.y; ++y)
		{
			bool is_in = false;
			if (zone_is_the_same)
			{
				is_in = true;
			}
			else
			{
				for (unsigned int x_old = zone_ld_old.x; x_old <= zone_ru_old.x; ++x_old)
				{
					for (unsigned int y_old = zone_ld_old.y; y_old <= zone_ru_old.y; ++y_old)
					{
						if (x == x_old && y == y_old)
						{
							is_in = true;
							break;
						}
					}
					if (is_in)
					{
						break;
					}
				}
			}

			Zone *zone = &m_zone_matrix[x][y];
			for (Zone::ObjInterator i = zone->FirstObser(); i != zone->LastObser(); ++i)
			{
				ObjID oid = *i;
				if (obj_id == oid)
				{
					continue;
				}
				Obj *obser = m_obj_manager->GetObj(oid);
				if (obser != 0 && !aoi_old.IsIn(obser->GetPos()) && aoi->IsIn(obser->GetPos()))
				{
					obj->OnAOIEnter(oid);
				}
			}

			if (!is_in)
			{
				zone->AddAOI(aoi_handler);
			}
		}
	}
}

void ZoneMatrix::NotifyAreaMsg(UInt32 obser_handle, void *msg, unsigned int length)
{
	if (!m_obser_pool.Exist(obser_handle))
	{
		return ;
	}

	ObserItem *obser = m_obser_pool[obser_handle];

	Zone *zone = &m_zone_matrix[obser->be_obs_local.x][obser->be_obs_local.y];
	int aoi_size = zone->AOISize();

	static GSNetID gsnetid[1024];
	int rolenum = 0;

	for (int i = 0; i < aoi_size; ++i)
	{
		AOI *aoi = &m_aoi_pool[zone->GetAOI(i)];
		Obj *obj = m_obj_manager->GetObj(aoi->obj_id);
		if (obj != 0 && obj->GetObjType() == Obj::OBJ_TYPE_ROLE && aoi->IsNotifyMsg(obser->pos))
		{
			gsnetid[rolenum++] = ((Role*)obj)->GetNetId();
			if (rolenum == 1024)
			{
				break;
			}
		}
	}

	if (rolenum != 0)
	{
		EngineAdapter::Instance().NetMulSend(gsnetid, rolenum, (const char *)msg, length);
	}
}

int ZoneMatrix::GetObjByArea(Posi pos, Axis x_range, Axis y_range, ObjID *obj_list, int max_num)
{
	Posi begin_pos(pos.x - x_range, pos.y - y_range);	// 前后边界都包括
	Posi end_pos(pos.x + x_range, pos.y + y_range);
	Posu begin_zone, end_zone;
	CalcZone(begin_pos.x, begin_pos.y, &begin_zone);
	CalcZone(end_pos.x, end_pos.y, &end_zone);

	int count = 0;
	for (unsigned int x = begin_zone.x; x <= end_zone.x; ++x)
	{
		for (unsigned int y = begin_zone.y; y <= end_zone.y; ++y)
		{
			Zone *zone = &m_zone_matrix[x][y];
			for (Zone::ObjInterator i = zone->FirstObser(); count < max_num && i != zone->LastObser(); ++i)
			{
				Obj *obj = m_obj_manager->GetObj(*i);
				if (obj != 0)
				{
					Posi obj_pos = obj->GetPos();
					if (obj_pos.x >= begin_pos.x && obj_pos.x <= end_pos.x 
						&& obj_pos.y >= begin_pos.y && obj_pos.y <= end_pos.y)
					{
						obj_list[count++] = obj->GetId();
						if (count >= max_num)
						{
							return count;
						}
					}
				}
			}
		}
	}

	return count;
}

int ZoneMatrix::GetObjByCircleArea(const Posi& pos, int rang, ObjID * obj_list, int max_num)
{
	Posi begin_pos(pos.x - rang, pos.y - rang);	// 前后边界都包括
	Posi end_pos(pos.x + rang, pos.y + rang);
	Posu begin_zone, end_zone;
	CalcZone(begin_pos.x, begin_pos.y, &begin_zone);
	CalcZone(end_pos.x, end_pos.y, &end_zone);

	int count = 0;
	for (unsigned int x = begin_zone.x; x <= end_zone.x; ++x)
	{
		for (unsigned int y = begin_zone.y; y <= end_zone.y; ++y)
		{
			Zone *zone = &m_zone_matrix[x][y];
			for (Zone::ObjInterator i = zone->FirstObser(); count < max_num && i != zone->LastObser(); ++i)
			{
				Obj *obj = m_obj_manager->GetObj(*i);
				if (obj != 0)
				{
					Posi obj_pos = obj->GetPos();
					//if (obj_pos.x >= begin_pos.x && obj_pos.x <= end_pos.x
					//	&& obj_pos.y >= begin_pos.y && obj_pos.y <= end_pos.y)
					if(!(obj_pos - pos).IsLongerThan(rang))
					{
						obj_list[count++] = obj->GetId();
						if (count >= max_num)
						{
							return count;
						}
					}
				}
			}
		}
	}

	return count;
}


