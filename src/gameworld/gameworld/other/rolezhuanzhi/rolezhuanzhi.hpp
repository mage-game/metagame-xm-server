#ifndef __ROLE_ZHUAN_ZHI_HPP__
#define __ROLE_ZHUAN_ZHI_HPP__

#include "obj/character/attribute.hpp"
#include "obj/character/character.h"
#include "config/zhuanzhiconfig.hpp"
#include "protocal/msgzhuanzhi.h"
#include "rolezhuanzhiskill.hpp"

class Role;

class RoleZhuanZhi
{
	friend class RoleZhuanZhiSkill;
public:
	RoleZhuanZhi();
	~RoleZhuanZhi();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role * role, char zhuanzhi_tianming_level, char zhuanzhi_canglong_level, char zhuanzhi_shengxiao_level, char zhuanzhi_yuanqi_level, char zhuanzhi_shenqi_level, char zhuanzhi_xingxiu_level);
	void OnInitFinish();
	void GetInitParam(char *zhuanzhi_tianming_level, char *zhuanzhi_canglong_level, char *zhuanzhi_shengxiao_level, char *zhuanzhi_yuanqi_level, char *zhuanzhi_shenqi_level, char *zhuanzhi_xingxiu_level);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void Update(unsigned long interval, unsigned int now_second);

	void OnKillOtherRole(Role * target_role);

	void OnOneKeyFinishLevel3TaskReq();

	char GetTianMingLevel() { return m_zhuanzhi_info.zhuanzhi_level_tianming; }
	void OnTianMingUpLevel(bool is_use_exp, bool is_auto_buy = false);

	char GetCangLongLevel() { return m_zhuanzhi_info.zhuanzhi_level_canglong; }
	void OnCangLongUpLevel(bool is_use_exp, bool is_auto_buy = false);

	char GetShengXiaoLevel() { return m_zhuanzhi_info.zhuanzhi_level_shengxiao; }
	void OnShengXiaoUpLevel(bool is_use_exp, bool is_auto_buy = false);

	char GetYuanQiLevel() { return m_zhuanzhi_info.zhuanzhi_level_yuanqi; }
	void OnYuanQiUpLevel(bool is_use_exp, bool is_auto_buy = false);

	char GetShenQiLevel() { return m_zhuanzhi_info.zhuanzhi_level_shenqi; }
	void OnShenQiUpLevel(bool is_use_exp, bool is_auto_buy = false);

	char GetXingXiuLevel() { return m_zhuanzhi_info.zhuanzhi_level_xingxiu; }
	void OnXingXiuUpLevel(bool is_use_exp, bool is_auto_buy = false);

	bool UpLevel(const ZhuanzhiStruct *next_level_cfg, bool is_use_exp, const char *reason, bool is_auto_buy = false);

	void OnRoleZhuanZhiReq(short opera_type, short param1, short param2, short param3);

	void SendRoleZhuanZhiInfo();

	void GMFinishZhuanzhiTask();

	RoleZhuanZhiSkill & GetRoleZhuanZhiSkill() { return m_skill; }

private:
	Role *m_role;
	CharIntAttrs m_attrs_add;

	RoleZhuanZhiSkill m_skill;

	Protocol::SCRoleZhuanZhiInfo m_zhuanzhi_info;
};

#endif