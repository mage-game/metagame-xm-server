#ifndef __KEYCONFIG_H__
#define __KEYCONFIG_H__

#include <string>

#include "gamedef.h"

namespace globalconfig
{
	// �ؼ�ȫ������
	/*
	zone_matrix_x��zone matrix ��Ԫ�����Сx
	zone_matrix_y��zone matrix ��Ԫ�����Сy

	role_aoi_x��role��aoi�����Сx
	role_aoi_y��role��aoi�����Сy
	role_aoi_leave_more_then_enter��Role��AOI�뿪���ڽ��뷶Χ��ֵ

	role_aoi_sensitive_distance����ɫaoi��������ľ���
	pet_aoi_sensitive_distance������aoi��������ľ���

	role_save_time:��ɫ�洢ʱ��������
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
