#ifndef __DRESSING_ROONM_MGR_HPP__
#define __DRESSING_ROONM_MGR_HPP__

#include "servercommon/dressingroomdef.hpp"
#include "obj/character/attribute.hpp"

class Role;

class DressingRoomMgr
{
public:
	DressingRoomMgr();
	~DressingRoomMgr() = default;

	void InitParam(const DressingRoomParam &param);
	void GetInitParam(DressingRoomParam *param);

	void SetRole(Role *role) { m_role = role; }
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnDressingRoomOpera(int opera_type);
	void SendDressingRoomInfo();
	void CheckSuitChange(int system_type, int img_id, bool active);
	void SendDressingRoomSingleInfo(int suit_index, int img_index, bool active);
	void OnRoleLogin() { this->FirstInit(); }

	void OnDressingRoomExchange(int suit_index, int sub_index);

private:
	void FirstInit();
	bool CheckActiveSpecialImg(int system_type, int img_id);
	int CalcImgCount(int suit_index);

	Role *m_role;
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	DressingRoomParam m_param;
};

#endif // __DRESSING_ROONM_MGR_HPP__
