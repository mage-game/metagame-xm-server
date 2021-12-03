#ifndef __UPGRADE_MANAGER_HPP__
#define __UPGRADE_MANAGER_HPP__

#include "servercommon/struct/upgradeparam.hpp"
#include "upgrade.hpp"
#include "upgradeskill.hpp"
#include "obj/character/attribute.hpp"
#include "protocal/msgupgrade.hpp"

class Character;
class Role;

class UpgradeManager
{
	friend class UpgradeSkill;
public:
	UpgradeManager();
	~UpgradeManager();

	void Init(Role *role, const UpgradeManagerParam &param);
	void GetInitParam(UpgradeManagerParam *param);

	void OnInitFinish();												   // 需要初始化计算的操作放这里

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void Update(time_t now_second);

	void ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all);

	bool IsCanOpera(int upgrade_type, int opera_type);						// 检查能否操作
	void OnRoleOpera(Protocol::CSUpgradeOperaReq *upr);						// 请求操作

	int GetOperaModuleType(int opera_type);									// 由操作类型获取模块类型

	bool IsValidUpgradeType(int upgrade_type);								// 判断进阶系统是否有效

	//触发相关---------------------------------------------------------------
	void OnLogin();
	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	//外部调用相关---------------------------------------------------------------
	Upgrade * GetUpgrade(int upgrade_type);									// 获取进阶系统
	UpgradeSkill & GetUpgradeSkill() { return m_upgrade_skill; }			// 获取进阶技能

	void GetBaseAttr(int upgrade_type, CharIntAttrs &tmp_attr);				// 获取进阶系统基础属性
	void GetRoleAppearance(RoleAppearance *appearance);						// 获取外观信息

	bool OnUseShuxingdan(int upgrade_type, int slot_idx, short &use_count);	 // 使用属性丹

	bool UpOneGrade(int upgrade_type, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num); // 直升一阶

	bool IsImageActive(int upgrade_type, int image_id);						// 形象是否激活
	bool ActiveImage(int upgrade_type, int image_id);						// 激活形象
	bool OnImgUpgrade(int upgrade_type, int image_id);						// 形象进阶
	short GetUseImgID(int upgrade_type);									// 获取使用中的形象id
	bool IsSpecialImg(int upgrade_type, int img_id);						// 判断是否是特殊形象

	int GetGrade(int upgrade_type);											// 获取阶数
	int GetSkill0Level(int upgrade_type);									// 获取技能0等级
	
	bool GMOpera(Role *role);		// GM

	int GetSpecialImageLevelById(int upgrade_type, int imageid);
private:

	Role *m_role;
	UpgradeSkill m_upgrade_skill;				// 进阶系统技能

	Upgrade *m_upgrade_list[UPGRADE_TYPE_MAX];  // 进阶系统列表

};

#endif // 

