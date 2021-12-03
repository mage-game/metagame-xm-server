#include "zone.h"

Zone::Zone() : m_ld_x(0), m_ld_y(0), m_ru_x(0), m_ru_y(0), m_aoi_list(0), m_aoi_size(0), m_max_aoi_size(0)
{
	ResizeAOIList(32);
}

Zone::~Zone()
{
	free(m_aoi_list);
}

void Zone::AddAOI(UInt32 aoi)
{
	if (m_aoi_size == m_max_aoi_size)
	{
		ResizeAOIList(m_max_aoi_size * 2);
	}

	m_aoi_list[m_aoi_size++] = aoi;
}

void Zone::EraseAOI(UInt32 aoi)
{
	for (int i = 0; i < m_aoi_size; ++i)
	{
		if (m_aoi_list[i] == aoi)
		{
			if (i != m_aoi_size - 1)
			{
				m_aoi_list[i] = m_aoi_list[m_aoi_size - 1];
			}
			--m_aoi_size;
			break;
		}
	}
}

void Zone::ResizeAOIList(int size)
{
	UInt32 *aoi_list_new = (UInt32*)malloc(sizeof(UInt32) * size);
	memset(aoi_list_new, 0, sizeof(UInt32) * size);

	if (m_aoi_list != 0)
	{
		memcpy(aoi_list_new, m_aoi_list, sizeof(UInt32) * m_aoi_size);
		free(m_aoi_list);
	}

	m_aoi_list = aoi_list_new;
	m_max_aoi_size = size;
}
