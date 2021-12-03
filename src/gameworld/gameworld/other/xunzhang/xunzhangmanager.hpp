#ifndef __XUNZHANG_MANAGER_HPP__
#define __XUNZHANG_MANAGER_HPP__

#include "servercommon/struct/xunzhangparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class XunZhangManager
{
public:
	XunZhangManager();
	~XunZhangManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const XunZhangParam &xunzhang_param);
	void GetInitParam(XunZhangParam *xunzhang_param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	int GetMountAddPer();
	int GetWingAddPer();
	int GetHaloAddPer();
	int GetShengongAddPer();
	int GetShenyiAddPer();
	int GetFootprintAddPer();
	int GetXunZhangTotalLevel() { return m_total_level; }

	void SendAllXunZhangInfo();
	void OnUplevelReq(int xunzhang_id,int is_only_bind);

	int XunZhangGetAttributeValue(int attr_type);
private:
	void ReCalcTotalLevel();

	Role *m_role;

	XunZhangParam m_xunzhang_param;
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_total_level;
	int m_mount_attr_add;
	int m_wing_attr_add;
	int m_halo_attr_add;
	int m_shengong_attr_add;
	int m_shenyi_attr_add;
	int m_footprint_attr_add;
};

#endif // __XUNZHANG_MANAGER_HPP__
