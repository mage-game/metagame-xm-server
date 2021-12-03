#ifndef __JINGLINGFAZHEN_MANAGER_HPP__
#define __JINGLINGFAZHEN_MANAGER_HPP__

#include "servercommon/jinglingfazhendef.hpp"
#include "obj/character/attribute.hpp"
#include "jinglingfazhenconfig.hpp"

class Character;
class Role;

class JinglingFazhenManager
{
public:
	JinglingFazhenManager();
	~JinglingFazhenManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const JinglingFazhenParam &xiannvshouhu_param);
	void GetOtherInitParam(JinglingFazhenParam *param);
	void OnLogin();
	void GetRoleAppearance(RoleAppearance *appearance);

	int GetJinglingFazhenAppeID();	

	bool ActiveImage(int imageid, bool recalc = true);
	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	JinglingFazhenParam * GetJinglingFazhenParam();

	void SendJinglingFazhenInfo();

	void PrintJinglingFazhen(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckActive();

private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;

	JinglingFazhenParam m_jinglingfazhen_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

};

#endif // __SHENYI_MANAGER_HPP__






