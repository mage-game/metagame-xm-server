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
	0 �赲
	1 ����
	2 ��ȫ��
	3 ��Ծ��������ͣ��
	*/

	bool Validate(int obj_type, Axis x, Axis y)
	{
		// ���ж���״̬���ж���Ԫ������
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

	// �����Χ�˸���һ�����赲����Ϊ���赲
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
	//������
	bool WalkArea(Axis x, Axis y)
	{
		if (x < 0 || x >= m_x || y < 0 || y >= m_y) return false;

		return m_map_info[y][x] == '1';
	}

	// ��ȫ��
	bool SaveArea(Axis x, Axis y)
	{
		if (x < 0 || x >= m_x || y < 0 || y >= m_y) return false;

		return m_map_info[y][x] == '2';
	}

	// ��Ծ��
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

