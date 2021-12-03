#ifndef __ROLE_CONVERT_SHOP_HPP__
#define __ROLE_CONVERT_SHOP_HPP__

class Role;

#include "servercommon/struct/converrecordparam.hpp"

class RoleConvertRecord
{
public:
	RoleConvertRecord();
	~RoleConvertRecord();

	void SetRole(Role *role);
	void Init(Role *role, const ConvertRecordParam &record_param);
	void GetParam(ConvertRecordParam *record_param);

	void OnRoleLogin();
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	int GetRecordCount(int type, short seq);
	int GetLifetimeRecordCount(int type, short seq);
	bool AddRecordCount(int convert_type, short seq, short convert_count, bool is_lifetime_limit_item);

	void OnRoleLevelChange(int level);
	unsigned int GetRecordLimitBeginTimeStamp(int convert_type, int seq);
	bool AddRecordLimitBeginTimeStamp(int convert_type, int seq);

	void SendInfo();

private:
	Role *m_role;
	ConvertRecordParam m_record_param;
};

#endif // __ROLE_CONVERT_SHOP_HPP__


