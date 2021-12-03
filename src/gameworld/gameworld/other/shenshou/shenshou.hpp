#ifndef __SHENSHOU_HPP__
#define __SHENSHOU_HPP__

#include "servercommon/shenshoudef.hpp"
#include "obj/character/attribute.hpp"
#include <vector>

class Role;
class Character;

class Shenshou
{
	static const int MAX_SKILL_ADDTION_TIMES = 10;	// �����ͬ���ܵ��Ӵ��������ڴ���ʹ�ã��Ժ���Ը�����Ҫ�Ӵ�

	struct SkillItem
	{
		int skill_type;
		int level;
		unsigned int last_perform_time;				// ��һ�δ������ܵ�ʱ��
	};

	enum SKILL_PARAM
	{
		PARAM1 = 0, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7,
		PARAM_COUNT,
	};

public: 
	Shenshou();
	~Shenshou();

	//void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const ShenshouParam &Shenshou_param); 
	void GetInitParam(ShenshouParam *shenshou_param) const;

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcSpecialRoleBigGoalAttr();                    // ���������Ŀ�����Լӳ�   ֻ�����޼�װ��������װ���ȼ�
	void OnRoleBeenAttack(ObjID attacker);
	void OnHurtOther(Character *target, long long injure);

	bool IsMianYiXuanYun();
	bool IsMianYiChenMo();

	long long RecalcExtraInjureById(Character *target, long long injure, int skill_id);
	void RecalcCD(int skill_id, int cold_down_times, int &reduce_cd) const;

	//bool TransformFromKnapsackItem(UInt16 item_base_id, int item_base_num, short item_id, short strength_level, bool is_rand_start, int start_num , const char *reason);
	int PutOnEquip(ItemDataWrapper &item_wrapper, int shou_id);
	bool TakeOffEquip(int shenshou_id,int slot_index,bool is_puton);
	void OnHandlerOpera(short opera_type, short param_1, short param_2, short param_3, int param_4);
	void OnGMReq(int type, int param1, int param2, int param3);
	void UpgradeEquip(short shenshou_id, short equip_index, short destroy_num, short destroy_backpack_index_list[SHENSHOU_MAX_BACKPACK_COUNT], bool use_double_shuliandu = false);

	int GetSkill10HurtPercent();
	int GetSkill12HurtPercent();
	void GetSkill12HurtNum(int &num);
	void GetSkill10DecSpeedInterval(int &Interval);

	void CheckFirstHuanlingRefresh();
	void AddHunalingScore(int add_score);
	void OnDayChange(int old_dayid, int now_dayid);
	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);

	int GetActiveCount();  // ��ȡ�������
	int GetEquipCount(int quality);  // ��ȡ��������

	bool IsActiveShenshou(int shenshou_id) const;

protected:
	//bool AddItem(ShenshouBackpackItem &tmp_item, const char *reason, int num = 1, bool is_send = true, char is_full_backpack = 0);											// ������Ʒ
	void RecalcSkillAttrForSingleShou(const CharIntAttrs & char_int_attrs, CharIntAttrs & char_extra_attrs, int skill_id, const int param_list[]);			// ���ڵ���ս������������������
	void RecalcSkillAttrForAllZhuzhanShou(const CharIntAttrs & char_int_attrs, CharIntAttrs & char_extra_attrs, int skill_id, const int param_list[]);		// ����ȫ����ս������������������
	static void RecalcSkillAttrForRoleBaseAttr(CharIntAttrs & role_attrs, int skill_id, const int param_list[]);			// �������ǻ�����������������
	void RecalcSkillAttrForRoleAttr(CharIntAttrs & role_attrs, int skill_id, const int param_list[]);  // ���ڽ�ɫȫ����������
	static void CalcRandomAttr(const CharIntAttrs & char_int_attrs, CharIntAttrs & char_extra_attrs, CharIntAttrs & role_attrs, int attr_type, int attr_value);			// �������޻�������������������Լӳɣ�����װ��������ԣ�

	
	//bool EmptyBackpackNumMoreThan(int check_num) const;
	static int GetAttrType(int Shenshou_attr_type);
	//void PutOnEquip(int shou_id, int backpack_index, int slot_index);
	//void TakeOffEquip(int shou_id, int slot_index);
	void Zhuzhan(int shou_id);
	void AddZhuzhan();
	void Compose(int v_item_id, int index_1, int index_2, int index_3);
	int GetHurtDecPer();
	int GetHurtIncPercent();

	void HuanlingRefresh(bool is_first_refresh = false);
	void HuanlingDraw();

	bool CheckOpengameDay() const;
	void CheckFuncOpen();
#ifdef _DEBUG
	bool AutoTest();
#endif
	//void SendBackpackAdditionInfo(int backpack_index);
	//void SendBackpackInfo();
	void SendShenshouInfo(int shenshou_id = 0);
	void SendShenshouBaseInfo();
	void SendShenshouHuanlingInfo();
	void SendShenshouHuanlingDrawResult(int seq);
	void SpecialSkillDeal();
	void RecalcSkillList(int shou_id = 0, bool is_new_active = true);					// ���ݵ�ǰ����������Ϣ������ս����

private:
	Role *m_role;
	ShenshouParam m_param;
	CharIntAttrs m_attrs_add;															// �����Լӳ���ֵ
	
	//bool m_backpack_durty_list[SHENSHOU_MAX_BACKPACK_COUNT];

	std::map<int, std::vector<SkillItem> > m_shenshou_active_skill_list;				// �������޵�ǰ����ļ���
	std::map<int, unsigned int> m_skill_last_perform_time_map;							// ����CD�ļ��ܣ��ͷ�ʱ��
	std::map<int, int> m_stength_skill_map;

	int m_cur_zhuzhan_count;															// ��ǰ��ս���޸�������̬ά���������̣�
	int m_cd_reduce_percent;
};

#endif // __SHENSHOU_HPP__



