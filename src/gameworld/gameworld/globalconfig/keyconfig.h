#ifndef __KEYCONFIG_H__
#define __KEYCONFIG_H__

#include <string>

#include "gamedef.h"

namespace globalconfig
{
	// 关键全局配置
	/*
	zone_matrix_x：zone matrix 单元区域大小x
	zone_matrix_y：zone matrix 单元区域大小y

	role_aoi_x：role的aoi区域大小x
	role_aoi_y：role的aoi区域大小y
	role_aoi_leave_more_then_enter：Role的AOI离开大于进入范围的值

	role_aoi_sensitive_distance：角色aoi敏感区域的距离
	pet_aoi_sensitive_distance：宠物aoi敏感区域的距离

	role_save_time:角色存储时间间隔，秒
	*/
	class KeyConfig
	{
	public:
		KeyConfig();
		~KeyConfig() {};

		bool Init(const std::string& path, std::string* err);

		const unsigned int GetZoneMatrixX() const;
		const unsigned int GetZoneMatrixY() const;

		const unsigned int GetRoleAoiX() const;
		const unsigned int GetRoleAoiY() const;
		const unsigned int GetRoleAoiLeaveMoreThenEnter() const;

		const unsigned int GetRoleAoiSensitiveDistance() const;
		const unsigned int GetPetAoiSensitiveDistance() const;

		const unsigned int GetRoleSaveTime() const;

	private:
		unsigned int zone_matrix_x;
		unsigned int zone_matrix_y;

		unsigned int role_aoi_x;
		unsigned int role_aoi_y;
		unsigned int role_aoi_leave_more_then_enter;

		unsigned int role_aoi_sensitive_distance;
		unsigned int pet_aoi_sensitive_distance;

		unsigned int role_save_time;
	};

};

#endif
