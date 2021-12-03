
#include "keyconfig.h"

#include "servercommon/configcommon.h"
#include "item/itempool.h"

namespace globalconfig
{

	KeyConfig::KeyConfig()
		: zone_matrix_x(0), zone_matrix_y(0), role_aoi_x(0), role_aoi_y(0),
		role_aoi_leave_more_then_enter(0), role_aoi_sensitive_distance(0), pet_aoi_sensitive_distance(0), role_save_time(60)
	{
	}

	bool KeyConfig::Init(const std::string& configname, std::string* err)
	{
		TiXmlDocument m_document;

		if ( configname == "" || !m_document.LoadFile(configname) )
		{
			*err = configname + " Load fail: " +m_document.ErrorDesc();
			return false;
		}

		TiXmlElement *RootElement = m_document.RootElement( );
		std::string configpath;

		if (!RootElement)
		{
			*err = configname + ": xml node error in root";
			return false;
		}

		// ¿ªÊ¼¼ÓÔØ
		if (!GetSubNodeValue(RootElement, "zone_matrix_x", zone_matrix_x))
		{
			*err = configname + ": xml node error in [config->zone_matrix_x]";
			return false;
		}
		if (!GetSubNodeValue(RootElement, "zone_matrix_y", zone_matrix_y))
		{
			*err = configname + ": xml node error in [config->zone_matrix_y]";
			return false;
		}

		if (!GetSubNodeValue(RootElement, "role_aoi_x", role_aoi_x))
		{
			*err = configname + ": xml node error in [config->role_aoi_x]";
			return false;
		}
		if (!GetSubNodeValue(RootElement, "role_aoi_y", role_aoi_y))
		{
			*err = configname + ": xml node error in [config->role_aoi_y]";
			return false;
		}
		if (!GetSubNodeValue(RootElement, "role_aoi_leave_more_then_enter", role_aoi_leave_more_then_enter))
		{
			*err = configname + ": xml node error in [config->role_aoi_leave_more_then_enter]";
			return false;
		}

		if (!GetSubNodeValue(RootElement, "role_aoi_sensitive_distance", role_aoi_sensitive_distance))
		{
			*err = configname + ": xml node error in [config->role_aoi_sensitive_distance]";
			return false;
		}
		if (!GetSubNodeValue(RootElement, "pet_aoi_sensitive_distance", pet_aoi_sensitive_distance))
		{
			*err = configname + ": xml node error in [config->pet_aoi_sensitive_distance]";
			return false;
		}

		if (!GetSubNodeValue(RootElement, "role_save_time", role_save_time))
		{
			*err = configname + ": xml node error in [config->role_save_time]";
			return false;
		}

		return true;
	}

	const unsigned int KeyConfig::GetZoneMatrixX() const
	{
		return zone_matrix_x;
	}
	const unsigned int KeyConfig::GetZoneMatrixY() const
	{
		return zone_matrix_y;
	}

	const unsigned int KeyConfig::GetRoleAoiX() const
	{
		return role_aoi_x;
	}
	const unsigned int KeyConfig::GetRoleAoiY() const
	{
		return role_aoi_y;
	}
	const unsigned int KeyConfig::GetRoleAoiLeaveMoreThenEnter() const
	{
		return role_aoi_leave_more_then_enter;
	}

	const unsigned int KeyConfig::GetRoleAoiSensitiveDistance() const
	{
		return role_aoi_sensitive_distance;
	}

	const unsigned int KeyConfig::GetPetAoiSensitiveDistance() const
	{
		return pet_aoi_sensitive_distance;
	}

	const unsigned int KeyConfig::GetRoleSaveTime() const
	{
		return role_save_time;
	}

}
