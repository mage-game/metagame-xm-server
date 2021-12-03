#ifndef __SHENZHOU_WEAPON_H__
#define __SHENZHOU_WEAPON_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/shenzhouweapondef.hpp"
#include "obj/character/character.h"
#include "protocal/msgshenzhouweapon.h"
#include "other/shenzhouweapon/shenzhouweaponconfig.hpp"

static const int SHENZHOU_WEAPON_GATHER_BROADCAST_INTERVAL = 30;				// 采集发传闻最小间隔
static const int MAX_IDENTIFY_BATCH_ITEM_COUNT = 999;							// 最大一键鉴定物品数量

class ShenzhouWeapon
{
public:
	ShenzhouWeapon();
	~ShenzhouWeapon();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const ShenzhouWeaponParam &param);
	void GetInitParam(ShenzhouWeaponParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	//////////////////////////////////////////////////////////////////////////
	
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);
	void OnKillOtherRole(Role * target_role);
	long long GetShenzhouSkillFixInjure(Character *target, long long base_injure, int skill_id);
	void CheckSkillAttr(CharIntAttrs &skill_attrs_add);
	
	//////////////////////////////////////////////////////////////////////////

	void SendBaseInfo();													// 发送基本信息
	void SendGatherInfo();													// 发送采集信息

	bool CanGather(int gather_id);											// 能否采集（只判断神州六器的采集物）
	void OnStartGather(int gather_id);										// 开始采集
	void OnGather(int gather_id);											// 玩家采集完毕
	void BuyGatherTimes();													// 购买采集次数
	void SkipGather();														// 一键消耗所有采集次数

	void ExchangeIdentifyExp();												// 兑换鉴定经验
	void AddIdentifyExp(int add_identify_exp, bool is_notice_client = true);// 增加鉴定经验

	void IdentifyBatchItem(int index, int num);								// 鉴定多件物品
	void OneKeyIdentifyItem();												// 一键鉴定

	void SlotUpLevel(int hunqi_id, int slot);								// 升级神州六器部件
	void RefreshWeaponLevel(int hunqi_id);									// 刷新神州六器等级

	void ElementUpLevel(int hunqi_id, int element_type);					// 炼魂元素升级

	void HelpOtherBox(int uid);												// 协助别人的宝箱
	void InviteHelpOtherBox();												// 邀请协助
	void OpenBox(int itmes);												// 开宝箱
	void PutBox();															// 放入宝箱
	void SendBoxInfo();														// 发送宝箱信息
	void RemoveBoxHelp();													// 清除宝箱协助

	void HunyinInlay(int hunqi_id, int hunyin_slot, int index_in_bag);		// 镶嵌魂印
	void RefreshHunyinSuitColor(int hunqi_id);								// 刷新魂印套装颜色
	void HunyinResolve(Protocol::CSShenzhouHunyinResolveReq *cmd);			// 分解魂印
	void LingshuUplevel(int hunqi_id, int hunyin_slot, bool is_notice_client = true);						// 灵枢升级
	void AutoLingshuUplevel(int hunqi_id);									// 灵枢升级,当前魂器的所有魂石直升1级

	void GMAddIdentifyExp(int exp);
	void GMAddShenzhouExp(int exp);

	ShenzhouWeaponParam & GetShenZhouParam(){ return m_param; }

	void OpenXilianSlot(int hunqi, int slot);
	void XilianReq(int hunqi, int lock_flag, int xilian_comsume_color, bool is_auto_buy, bool is_free);
	int TransformXilianAttrType(int type);
	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);

	int GetActiveCount();													// 激活数量
	int GetHunyinInlayCount(int hunyin_color);								// 镶嵌魂印数量						

	void OpenHunYinSlot(int hunqi_id, int hunyin_slot, bool is_notice = true, bool is_check_open = false);	//开启魂槽

	void ReCalcSuitCount();

	int GetHunyinQualityCount(int hunqi_id, int color);
	int GetHunyinSuitQualityCount(int hunqi_id, int color);					// 获取符合品质的套装数量,注:粉装也算红的

	void OnRoleLogin();	

	void GMAddHunqiLevel(int hunqi_id);

private:

	//////////////////////////////////////////////////////////////////////////
	// 触发技能
	void OnAttackSkill01(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnAttackSkill03(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnAttackSkill07(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnAttackSkill08(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnAttackSkill10(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);

	void OnHurtOtherSkill02(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill04(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill05(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill06(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill09(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill11(Character *target, int skill_id, long long base_injure, long long &injure);

	//////////////////////////////////////////////////////////////////////////

	void XilianOneReq(int hunqi, int slot, int xilian_comsume_color, bool is_recal = true);
	void CheckFuncOpen();

	Role *m_role;
	ShenzhouWeaponParam m_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值
	time_t m_last_gather_time;

	int m_hunyin_quality_count[SHENZHOU_WEAPON_COUNT][HUNYIN_COLOR_MAX];

	int m_capablity;
};

#endif

