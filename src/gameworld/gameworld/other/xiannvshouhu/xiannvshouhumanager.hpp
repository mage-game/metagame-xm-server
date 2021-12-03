#ifndef __XIANNVSHOUHU_MANAGER_HPP__
#define __XIANNVSHOUHU_MANAGER_HPP__

#include "servercommon/xiannvshouhudef.hpp"
#include "obj/character/attribute.hpp"
#include "xiannvshouhuconfig.hpp"

class Character;
class Role;

class XiannvShouhuManager
{
public:
	XiannvShouhuManager();
	~XiannvShouhuManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const XiannvShouhuParam &xiannvshouhu_param);
	void GetOtherInitParam(XiannvShouhuParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	int GetXiannvShouhuAppeID();	

	bool ActiveImage(int imageid, bool recalc = true);
	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	XiannvShouhuParam * GetXiannvShouhuParam();

	void SendXiannvShouhuInfo();

	void PrintXiannvShouhu(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckActive();

private:

	Role *m_role;

	XiannvShouhuParam m_xiannvshouhu_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

};

#endif // __SHENYI_MANAGER_HPP__






