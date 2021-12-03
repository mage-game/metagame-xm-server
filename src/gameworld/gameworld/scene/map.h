#ifndef __MAP_H__
#define __MAP_H__

#include "gamedef.h"
#include "config/mapconfig.h"
#include "obj/obj.h"

class Map
{
public:
	Map(const ConfigMap &configmap);
	~Map();

	/*
	0 阻挡
	1 可走
	2 安全区
	3 跳跃区，不可停留
	*/

	bool Validate(int obj_type, Axis x, Axis y)
	{
		// 如有多种状态则集中定义元素意义
		//return x >= 0 && x < m_x && y >= 0 && y < m_y && (obj_type != Obj::OBJ_TYPE_ROLE || m_map_info[y][x] == '0');
		bool ret = x >= 0 && x < m_x && y >= 0 && y < m_y;
		if (!ret)
		{
			return false;
		}
		switch (obj_type)
		{
		case Obj::OBJ_TYPE_ROLE:
			return m_map_info[y][x] > '0';
		case Obj::OBJ_TYPE_FALLINGITEM:
			return m_map_info[y][x] > '0' && m_map_info[y][x] != '9';
		default:
			return true;
		}
	}

	// 如果周围八格有一个是阻挡就认为是阻挡
	bool IsValidatePos(int obj_type, int x, int y)
	{
		static const int POS_OFFSET_COUNT = 9;
		static const Posi offset_pos_list[POS_OFFSET_COUNT] = {Posi(0, 0), Posi(1, 0), Posi(1, -1), Posi(0, -1), Posi(-1, -1), Posi(-1, 0), Posi(-1, 1), Posi(0, 1), Posi(1, 1)};

		for (int i = 0; i < POS_OFFSET_COUNT; i++)
		{
			if (!this->Validate(obj_type, x + offset_pos_list[i].x, y + offset_pos_list[i].y))
			{
				return false;
			}
		}

		return true;
	}

	void GetArea(Axis *ru_x, Axis *ru_y)
	{
		*ru_x = m_x;
		*ru_y = m_y;
	}

	void GetArea(Posi *ru)
	{
		ru->x = m_x;
		ru->y = m_y;
	}

	char GetType(Axis x, Axis y)
	{
		if (x < 0 || x >= m_x || y < 0 || y >= m_y) return '0';

		return m_map_info[y][x];
	}
	//行走区
	bool WalkArea(Axis x, Axis y)
	{
		if (x < 0 || x >= m_x || y < 0 || y >= m_y) return false;

		return m_map_info[y][x] == '1';
	}

	// 安全区
	bool SaveArea(Axis x, Axis y)
	{
		if (x < 0 || x >= m_x || y < 0 || y >= m_y) return false;

		return m_map_info[y][x] == '2';
	}

	// 跳跃区
	bool JumpArea(Axis x, Axis y)
	{
		if (x < 0 || x >= m_x || y < 0 || y >= m_y) return false;

		return m_map_info[y][x] == '3';
	}

protected:
	Map(const Map &_m);
	Map &operator=(const Map &_m);
private:

		//			    		(0,m_y)			(m_x,m_y)
		//						    -------------
		//							|			|
		//							|			|
		//                          |Y			|
		//        					|			|
		//       					|	  X		|
		//                          -------------
		//						(0,0)			(m_x,0)

	Axis m_x;
	Axis m_y;

	char **m_map_info;
};

#endif

