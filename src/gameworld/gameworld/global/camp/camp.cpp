#include "camp.hpp"

#include "servercommon/servercommon.h"
#include "engineadapter.h"
#include "servercommon/activitydef.hpp"

Camp::Camp()
	: m_camp_manager(NULL), m_camp_type(CAMP_TYPE_INVALID), m_role_count(0), m_change_flag(false), m_total_capability(0)
{

}

Camp::~Camp()
{

}

void Camp::Init(const CampAttr &camp_attr)
{
	m_role_count = camp_attr.base_info.role_count;
	m_total_capability = camp_attr.base_info.total_capability;
}

void Camp::GetChangeAttr(CampAttr *camp_attr, bool *change_flag)
{
	memset(camp_attr, 0, sizeof(CampAttr));

	camp_attr->base_info.camp_type = m_camp_type;
	camp_attr->base_info.role_count = m_role_count;
	camp_attr->base_info.guild_count = 0;
	camp_attr->base_info.total_capability = m_total_capability;

	if (NULL != change_flag) *change_flag = m_change_flag;
}

void Camp::ClearDirtyMark()
{
	m_change_flag = false;
}

void Camp::AddRoleCount(int add_val)
{ 
	m_role_count += add_val;
	if (m_role_count < 0) m_role_count = 0;
	
	m_change_flag = true; 
}

