#ifndef __ZONEMATRIX_H__
#define __ZONEMATRIX_H__

#include "gamedef.h"
#include "zone.h"
#include "obseritem.h"
#include "aoi.h"

/*

	zone_matrix 下标索引方式：

	 (0, matrix_y_count)         (matrix_x_count, matrix_y_count)
			-------------------------
			|     |     |     |     |
			| 0,3 |	1,3	| 2,3 | 3,3 |
			-------------------------
			|     |     |     |     |
			| 0,2 |	1,2	| 2,2 | 3,2 |
			-------------------------
			|     |     |     |     |
			| 0,1 |	1,1	| 2,1 | 3,1 |
			-------------------------
			|     |     |     |     |
			| 0,0 |	1,0	| 2,0 | 3,0 |
	(0,0)	-------------------------  (matrix_x_count, 0)
		          

*/

class ObjManager;

class ZoneMatrix 
{
public:
	static void CreateZoneMatrix(ZoneMatrix *z, Axis CX, Axis CY, Axis perX, Axis perY, ObjManager *obj_manager);

	ZoneMatrix() : m_zone_matrix(0), m_matrix_x_count(0), m_matrix_y_count(0), m_zone_per_x(0), m_zone_per_y(0), m_cx(0), m_cy(0), m_obj_manager(0) {}

	void DestriyZoneMatrix();

	// 计算zone位置并进行验证
	bool CheckZone(const Posi &pos, Posu *zone_local)
	{
		if (pos.x < 0 || pos.y < 0 || m_zone_per_x <= 0 || m_zone_per_y <= 0)
		{
			return false;
		}
		zone_local->x = pos.x / m_zone_per_x;
		zone_local->y = pos.y / m_zone_per_y;
		return zone_local->x < m_matrix_x_count && zone_local->y < m_matrix_y_count;
	}

	// 计算zone位置 非法位置则设为边界值
	void CalcZone(Axis x, Axis y, Posu *zone_local)
	{
		(x < 0) ? (x = 0) : 0;
		(y < 0) ? (y = 0) : 0;
		zone_local->x = (m_zone_per_x > 0 ? x / m_zone_per_x : 0);
		zone_local->y = (m_zone_per_y > 0 ? y / m_zone_per_y : 0);
		(zone_local->x < m_matrix_x_count) ? 0 : (zone_local->x = m_matrix_x_count - 1);
		(zone_local->y < m_matrix_y_count) ? 0 : (zone_local->y = m_matrix_y_count - 1);
	}

	// 此处不检查越界 由外界保证
	Zone::ObjInterator FirstObser(unsigned int x, unsigned int y) { return m_zone_matrix[x][y].FirstObser(); }
	Zone::ObjInterator LastObser(unsigned int x, unsigned int y) { return m_zone_matrix[x][y].LastObser(); }

	UInt32 CreateObser(ObjID obj_id);
	void DestroyObser(UInt32 obser_handle);
	void MoveObser(UInt32 obser_handle);

	UInt32 CreateCircleAOI(ObjID obj_id, const Posi &centre, Axis enter_radius, Axis leave_radius);
	UInt32 CreateRectAOI(ObjID obj_id, const Posi &centre, const Posi &enter_r, const Posi &leave_r);
	void DestroyAOI(UInt32 aoi_handler);
	void MoveAOI(UInt32 aoi_handler);

	void NotifyAreaMsg(UInt32 obser_handle, void *msg, unsigned int length);

	int GetObjByArea(Posi pos, Axis x_range, Axis y_range, ObjID *obj_list, int max_num);	//方形

	int GetObjByCircleArea(const Posi& pos, int rang, ObjID * obj_list, int max_num);	//圆形
protected:
	UInt32 CreateAOI(ObjID obj_id, bool circle, const Posi &centre, const Posi &enter_radius, const Posi &leave_radius);

protected:
	Zone **m_zone_matrix;
	unsigned int m_matrix_x_count;
	unsigned int m_matrix_y_count;
	Axis m_zone_per_x;
	Axis m_zone_per_y;
	Axis m_cx;
	Axis m_cy;

	typedef RAObjList<ObserItem*> ObserPool;
	ObserPool m_obser_pool;

	typedef RAObjList<AOI> AOIPool;
	AOIPool m_aoi_pool;

	ObjManager *m_obj_manager;
};

#endif
