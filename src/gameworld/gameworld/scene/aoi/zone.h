#ifndef __ZONE_H__
#define __ZONE_H__

#include "common/raobjlist.h"
#include "gamedef.h"

class Zone
{
	typedef RAObjList<ObjID> ZoneObjList;

public:
	typedef ZoneObjList::Iterator ObjInterator;

	Zone();
	~Zone();

	void Init(Axis ld_x, Axis ld_y, Axis CX, Axis CY)
	{
		m_ld_x = ld_x;
		m_ld_y = ld_y;
		m_ru_x = ld_x + CX;
		m_ru_y = ld_y + CY;
	}

	void EraseObser(unsigned int index)
	{
		m_be_obs_list.Erase(index);
	}

	unsigned int AddObser(ObjID obj_id)
	{
		return m_be_obs_list.Insert(obj_id);
	}

	bool IsInZone(Axis x, Axis y)
	{
		return x >= m_ld_x && x < m_ru_x && y >= m_ld_y && y < m_ru_y;
	}

	ObjInterator FirstObser()
	{
		return m_be_obs_list.Beg();
	}

	ObjInterator LastObser()
	{
		return m_be_obs_list.End();
	}

	void AddAOI(UInt32 aoi);
	void EraseAOI(UInt32 aoi);

	int AOISize() { return m_aoi_size; }
	UInt32 GetAOI(int i) { return m_aoi_list[i];}

private:
	//			    	(m_ld_x,m_ru_y)	(m_ru_x,m_ru_y)
	//						    -------------
	//							|			|
	//							|			|
	//                          |Y			|
	//        					|			|
	//       					|	  X		|
	//                          -------------
	//					(m_ld_x,m_ld_y)	(m_ru_x,m_ld_y)

	Axis m_ld_x;
	Axis m_ld_y;
	Axis m_ru_x;
	Axis m_ru_y;

	ZoneObjList	m_be_obs_list;		// 被观察者列表

	// aoi列表
	UInt32 *m_aoi_list;
	int m_aoi_size;
	int m_max_aoi_size;

	void ResizeAOIList(int size);
};

#endif

