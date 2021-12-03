#ifndef __XIANNV_MANAGER_HPP__
#define __XIANNV_MANAGER_HPP__

#include "servercommon/struct/xiannvparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class XiannvManager
{
	static const int MAX_EXP_CHOU_TIME = 10;		// 女神圣物经验多抽的次数
	static const int CHOU_MAX_LING_COUNT = 2;		// 一次抽取最多出现的（0）灵个数
	static const int SYSTEM_CHOU_MAX_LING_COUNT = 1;// 系统抽取最多出现的（0）灵个数

	struct ShengwuSkill
	{
		const static int TRIGGER_RATE_NUM = 10000;	// 万份比数量

		ShengwuSkill(): cd_end_perform_time(0), trigger_rate(0), cool_down_ms(0), param_1(0), param_2(0), capability(0), capability_per(0) {}

		unsigned long cd_end_perform_time;			// 技能cd结束时间（游戏时间）
		int trigger_rate;							// 技能触发万份比
		int cool_down_ms;
		int param_1;
		int param_2;
		int capability;								// 技能战力加成
		int capability_per;							// 技能战力加成万分比
	};
	
public:
	XiannvManager();
	~XiannvManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const XiannvParam &xiannv_param);
	void GetInitParam(XiannvParam *xiannv_param);
	void OnLogin();
	XiannvParam * GetXiannvParam() { return &m_xiannv_param; }

	void Update(time_t now_second);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnAttack(ObjID attacker, Attribute delta_hp);
	void OnHurtOther(Character *target, long long base_injure, long long &injure);
	void OnDayChange(int old_dayid, int now_dayid);

	bool ActiveXiannv(int xiannv_id);
	void Rename(short xiannv_id, const GameName new_name);

	void SendAllXiannvInfo();
	void SendXiannvInfo(int xiannv_id, int notify_reason = 0);
	void BroadcastXiannvInfo();

	void FightOut(const char pos_list[MAX_XIANNV_POS]);
	int GetFightOutXiannv(){return m_xiannv_param.pos_list[0];}
	bool OnActiveReq(int xiannv_id, short item_index);
	void OnRoleLevelChange(int level);
	bool OnAddZizhiReq(int xiannv_id, bool is_auto_buy);

	const char * GetName(short xiannv_id);
	void GetXiannvInfo(GameName &xiannv_name);
	int GetActiveXiannvCount();									    // 获取已激活个数
	int GetActiveXiannvCountFast();									// 获取已激活个数，算法做了优化
	int GetMaxXiannvZizhi();

	bool ActiveHuanHua(int huanhua_id);
	bool OnActiveHuanHuaReq(int huanhua_id, short item_index);
	bool OnHuanHuaReq(int huanhua_id);
	bool OnHuanHuaUplevelReq(int huanhua_id, bool is_auto_buy);
	short GetXiannvHuanhuaId(){return m_xiannv_param.huanhua_id;}

	void GetRankXnData(int *max_xn_id, long long *xn_zizhi);
	bool ShengwuAddLingye(int total_lingye, const char * reason_str);

	void OnShengwuReq(short req_type, short param1, int param2, int param3);
	void OnShengwuGm(int type, int param1, int param2, int param3);

	void OnCompleteTask(TaskID task_id);

	int GetGongMingTotalLevel();									// 共鸣总等级
	int GetShengwuTotalLevel();										// 仙器总等级
	int GetShengwuBaseCap() { return m_shengwu_base_capapblity; }	// 仙器基础战力
	int GetActiveXiannvZizhiNum(int zizhi_level);					// 仙女激活某资质等级以上个数
	bool AddShengwuEssence(int add_number, const char * type);		// 增加仙器精华
	bool UseShengwuEssence(int consume_number, const char * type, bool is_consume_sys = true);	// 使用仙器精华
	bool AddShengwuExp(int shengwu_id);
	bool IsShengwuEssenceEnough(int number);						// 仙器精华是否足够
	void ShengwuUpgrade(int shengwu_id);							// 圣物进阶,用于跳转到激活或进阶经验
	int GetSpecialImageLevelById(int special_image_id);

	void GMSetShengwuLevel(int shengwu_id, int level);

private:
	void PrintXiannv(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	bool ShengwuChouExpOnce(int chou_type, bool is_auto_buy);
	void ShengwuChouExp(int chou_type, bool is_auto_buy, bool is_auto_fetch, bool is_multi_chou);
	void ShengwuFetchExp();
	void ShengwuFetchUpgradeExp();											// 圣物领取进阶经验
	void ShengwuUpgradeExp(int shengwu_id);									// 圣物进阶
	void ShengwuActive(int shengwu_id);										// 激活圣物
	void ShengwuUpgradeGrid(short grid_id);
	void ShengwuSystemChouLing();
	void ShengwuChouLing();
	bool ShengwuFetchLing(bool is_double_fetch);
	void ShengwuSendChangeInfo(short notify_type, short param1, short param2 = 0, short param3 = 0, int param4 = 0) const;
	void SendMilingList() const;
	void SendShengwuChouExpList(char chou_type) const;
	void RecalcSkillList();
	void RecalcSkillCapability(int &capability);							// 计算技能虚拟战力
	void CheckFuncOpen();
	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	Role *m_role;

	XiannvParam m_xiannv_param;
	CharIntAttrs m_attrs_add;												// 各属性加成数值
	int m_shengwu_skill_capablity;											// 圣物技能的战力，额外加上去
	int m_shengwu_base_capapblity;											// 圣物基础属性战力

	unsigned long m_last_skill_time_list[MAX_XIANNV_COUNT];

	ShengwuSkill m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_COUNT];		// 当前激活的技能参数（不存盘，用于触发）
};

#endif // __XIANNV_MANAGER_HPP__
