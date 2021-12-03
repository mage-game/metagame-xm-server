#ifndef __CAMP_HPP__
#define __CAMP_HPP__

#include "servercommon/struct/global/campparam.hpp"

class CampManager;

class Camp
{
public:
	Camp();
	~Camp();

	void SetCampManager(CampManager *camp_manager) { m_camp_manager = camp_manager; }
	CampManager * GetCampManager() { return m_camp_manager; }

	void Init(const CampAttr &camp_attr);
	void GetChangeAttr(CampAttr *camp_attr, bool *change_flag);
	void ClearDirtyMark();

	void SetCampType(int camp_type) { m_camp_type = camp_type; }
	int GetCampType() { return m_camp_type; }

	int GetRoleCount() { return m_role_count; }
	void AddRoleCount(int add_val);

	long long GetTotalCapability() { return m_total_capability; }
	void SetTotalCapability(long long capability) { m_total_capability = capability; m_change_flag = true; }

private:
	CampManager *m_camp_manager;

	int m_camp_type;                            // 阵营类型
	int m_role_count;                           // 角色数量
	bool m_change_flag;

	long long m_total_capability;				// 阵营战力（统计排行榜前100）
};

#endif // __CAMP_HPP__

