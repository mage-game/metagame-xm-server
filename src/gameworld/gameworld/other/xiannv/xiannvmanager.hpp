#ifndef __XIANNV_MANAGER_HPP__
#define __XIANNV_MANAGER_HPP__

#include "servercommon/struct/xiannvparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class XiannvManager
{
	static const int MAX_EXP_CHOU_TIME = 10;		// Ů��ʥ�ﾭ����Ĵ���
	static const int CHOU_MAX_LING_COUNT = 2;		// һ�γ�ȡ�����ֵģ�0�������
	static const int SYSTEM_CHOU_MAX_LING_COUNT = 1;// ϵͳ��ȡ�����ֵģ�0�������

	struct ShengwuSkill
	{
		const static int TRIGGER_RATE_NUM = 10000;	// ��ݱ�����

		ShengwuSkill(): cd_end_perform_time(0), trigger_rate(0), cool_down_ms(0), param_1(0), param_2(0), capability(0), capability_per(0) {}

		unsigned long cd_end_perform_time;			// ����cd����ʱ�䣨��Ϸʱ�䣩
		int trigger_rate;							// ���ܴ�����ݱ�
		int cool_down_ms;
		int param_1;
		int param_2;
		int capability;								// ����ս���ӳ�
		int capability_per;							// ����ս���ӳ���ֱ�
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
	int GetActiveXiannvCount();									    // ��ȡ�Ѽ������
	int GetActiveXiannvCountFast();									// ��ȡ�Ѽ���������㷨�����Ż�
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

	int GetGongMingTotalLevel();									// �����ܵȼ�
	int GetShengwuTotalLevel();										// �����ܵȼ�
	int GetShengwuBaseCap() { return m_shengwu_base_capapblity; }	// ��������ս��
	int GetActiveXiannvZizhiNum(int zizhi_level);					// ��Ů����ĳ���ʵȼ����ϸ���
	bool AddShengwuEssence(int add_number, const char * type);		// ������������
	bool UseShengwuEssence(int consume_number, const char * type, bool is_consume_sys = true);	// ʹ����������
	bool AddShengwuExp(int shengwu_id);
	bool IsShengwuEssenceEnough(int number);						// ���������Ƿ��㹻
	void ShengwuUpgrade(int shengwu_id);							// ʥ�����,������ת���������׾���
	int GetSpecialImageLevelById(int special_image_id);

	void GMSetShengwuLevel(int shengwu_id, int level);

private:
	void PrintXiannv(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	bool ShengwuChouExpOnce(int chou_type, bool is_auto_buy);
	void ShengwuChouExp(int chou_type, bool is_auto_buy, bool is_auto_fetch, bool is_multi_chou);
	void ShengwuFetchExp();
	void ShengwuFetchUpgradeExp();											// ʥ����ȡ���׾���
	void ShengwuUpgradeExp(int shengwu_id);									// ʥ�����
	void ShengwuActive(int shengwu_id);										// ����ʥ��
	void ShengwuUpgradeGrid(short grid_id);
	void ShengwuSystemChouLing();
	void ShengwuChouLing();
	bool ShengwuFetchLing(bool is_double_fetch);
	void ShengwuSendChangeInfo(short notify_type, short param1, short param2 = 0, short param3 = 0, int param4 = 0) const;
	void SendMilingList() const;
	void SendShengwuChouExpList(char chou_type) const;
	void RecalcSkillList();
	void RecalcSkillCapability(int &capability);							// ���㼼������ս��
	void CheckFuncOpen();
	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	Role *m_role;

	XiannvParam m_xiannv_param;
	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ
	int m_shengwu_skill_capablity;											// ʥ�＼�ܵ�ս�����������ȥ
	int m_shengwu_base_capapblity;											// ʥ���������ս��

	unsigned long m_last_skill_time_list[MAX_XIANNV_COUNT];

	ShengwuSkill m_shengwu_skill_list[XIANNV_SHENGWU_SKILL_TYPE_COUNT];		// ��ǰ����ļ��ܲ����������̣����ڴ�����
};

#endif // __XIANNV_MANAGER_HPP__
