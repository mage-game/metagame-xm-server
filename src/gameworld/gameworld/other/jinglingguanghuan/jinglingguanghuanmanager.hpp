#ifndef __JINGLINGGUANGHUAN_MANAGER_HPP__
#define __JINGLINGGUANGHUAN_MANAGER_HPP__

#include "servercommon/jinglingguanghuandef.hpp"
#include "obj/character/attribute.hpp"
#include "jinglingguanghuanconfig.hpp"

class Character;
class Role;

class JinglingGuanghuanManager
{
public:
	JinglingGuanghuanManager();
	~JinglingGuanghuanManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const JinglingGuanghuanParam &jinglingguanghuan_param);
	void GetOtherInitParam(JinglingGuanghuanParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	int GetJinglingGuanghuanAppeID();	

	bool ActiveImage(int imageid, bool recalc = true);
	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	JinglingGuanghuanParam * GetJinglingGuanghuanParam();

	void SendJinglingGuanghuanInfo();

	void PrintJinglingGuanghuan(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckActive();

	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;

	JinglingGuanghuanParam m_jinglingguanghuan_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

};

#endif // __SHENYI_MANAGER_HPP__






