#include "map.h"

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

Map::Map(const ConfigMap &configmap)
	: m_x(configmap.width), m_y(configmap.height)
{
	assert((int)configmap.mask.length() >= configmap.width * configmap.height);

	m_map_info = (char **)::malloc(sizeof(char*) * m_y);
	::memset(m_map_info, 0, sizeof(char*) * m_y);

	for (Axis i = 0; i < m_y; ++i)
	{
		m_map_info[i] = (char *)malloc(sizeof(char) * m_x);
		::memset(m_map_info[i], 0, sizeof(char) * m_x);

		memcpy(m_map_info[i], configmap.mask.c_str() + m_x * i, m_x);
	}

}

Map::~Map()
{
	for (Axis i = 0; i < m_y; ++i)
	{
		::free(m_map_info[i]);
	}

	::free(m_map_info);
	m_map_info = 0;
}

