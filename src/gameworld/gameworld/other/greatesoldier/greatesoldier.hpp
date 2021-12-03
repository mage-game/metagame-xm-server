#ifndef __GREATE_SOLDIER_HPP__
#define __GREATE_SOLDIER_HPP__

#include "servercommon/greatesoldierdef.hpp"
#include "obj/character/attribute.hpp"
#include "greatesoldier_skill.hpp"
#include "engineadapter.h"
#include "greatesoldier_goal.hpp"

#define GREATE_SOLDIER(role) role->GetGreateSoldier()

class Role;

class GreateSoldier
{
	friend class GreateSoldierSkill;
public:
	GreateSoldier() : m_role(nullptr), m_other_capability(0), m_combine_flag(0), m_bianshen_cd_reduce_s(0), m_fabao_skill_id(0) {}
	~GreateSoldier() {}

	void SetRole(Role *role) { m_role = role; }
	void InitParam(const GreateSoldierParam &param);
	void GetInitParam(GreateSoldierParam *param) { *param = m_param; }
	void OnRoleLogin(unsigned int last_logout_time);
	void OnRoleLogout();

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcAttrBigSmallGoalPer(CharIntAttrs &attr_add);			//��СĿ����ֱȼӳ�

	void Update(time_t now_second, unsigned long interval);
	void OnDayChange(UInt32 old_dayid, UInt32 now_dayid);
	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);


	GreateSoldierParam & GetParam() { return m_param; }
	GreateSoldierGoal & GetGoal() { return m_goal; }

	char GetHuanhuaID() const { return m_param.other_param.use_huanhua_id; }
	int	 OnUseHuanhuaCard(int id, bool is_notify = true);	// ��ʹ���˻û���,����û�������
	void OnUseHuanhuaID(int id);							// ʹ��ĳ���û�����0��ʾȡ��

	bool OnLevelUp(int seq);								// ��������
	void OnBianshen();										// ��������
	void OnBianshenTrial(int seq);							// ������������
	void OnPuton(int item_seq, int slot_seq);				// װ����λ
	void OnPutoff(int slot_seq);							// ж�½�λ
	bool OnSlotLevelUp(int slot_seq);						// ������λ
	void OnDraw(int draw_type, bool is_auto_buy);			// �齱
	void OnUpgradePotential(int seq);						// Ǳ������

	int PutOnEquip(ItemDataWrapper &item_wrapper, int seq);		// ����װ��
	bool TakeOffEquip(int seq, int equip_slot_index, bool is_puton);	// ж��װ��
	void UpgradeEquip(int seq, int equip_slot_index, short destroy_num, short destroy_backpack_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM]);  // ����װ��/����

	void SendAllInfo();

	bool IsBianshenStatus() { return m_param.other_param.cur_used_seq >= 0; }
	UInt32 GetBianshenRemainingTime();				// ��ȡ����ʣ��ʱ��
	void AddBianshenCDReduce(int reduce_s, UInt16 skill_id);	// ��������CD
	void ClearBianshenCDReduce(UInt16 skill_id);	// ȡ����������CD
	void ForceClearBianshenStatus();				// ǿ��ȡ������״̬

	const int GetSlotLevel(const int slot_seq);			// ��ȡ��λ�ȼ�
	const int GetPotentialLevelBySeq(const int seq);	// ��ȡ������Ǳ�ܵȼ�

	bool OnTiyanActive(int seq, int tiyan_time_sec);
	bool IsActive(int seq);							// �Ƿ񼤻�����
	void UnActive(int seq);

	void SetBianshenTrail(int is_on);				// �����Ƿ����������

	void GmClearBianshenCD();						// �������cd
	void GmClearAllInfo();

	int GetAchieveXinghunlevelNum(int level);		// ��ȡ�ǻ�ﵽĳ���ȼ�������
	int GetXinghunSumLevel();						// ��ȡ�ǻ��ܵȼ�
	int GetActiveCount(int color);					// ��ȡ������������
	int GetLevelCount(int level);					// ��ȡ����ȼ�����������

	void SyncFabaoSkillID(UInt16 skill_id) { m_fabao_skill_id = skill_id; }
	void OnFetchLimitDrawReward(int seq);			// ��ȡ����
	void OnExchangeSeq(int seq);

	void OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void SetGreateSoldierBaoji(bool is_baoji) { m_skill.SetBaoji(is_baoji); }	// �����������ܱ�������
	bool IsGreateSoldierBaoji() { return m_skill.IsBaoji(); }					// �Ƿ������������Լ���

private:
	void SendItemInfo(int seq = -1);				// ����������Ϣ��seqΪ-1��ȫ��
	void SendOtherInfo(bool is_bianshen_cd_end);	// ����������Ϣ��is_bianshen_cd_endΪ�Ƿ����cd����
	void SendSlotInfo();							// ���ͽ�λ��Ϣ

	int GetCombineMinLevel(int seq);				// ��ȡ�����͵ȼ�
	void CheckCombineFlag();						// �����ϱ��
	void CheckFetchLimitDrawReward();				// ���첹������
	void SendFetchRewardInfo();						// �����콱��Ϣ
	void CheckFuncOpen();

	
private:
	Role *m_role;
	GreateSoldierParam m_param;
	CharIntAttrs m_attrs_add;
	int m_other_capability;

	UInt32 m_combine_flag;							// �������ϱ��
	GreateSoldierSkill m_skill;						// ��������
	GreateSoldierGoal m_goal;						// ����Ŀ��

	int m_bianshen_cd_reduce_s;						// ����CD����ʱ��
	std::map<UInt16, int> m_bianshen_cd_reduce_skillid;	// ��ʹ����CD���̵�Skill ID -> reduce time

	UInt16 m_fabao_skill_id;						// ͬ�� fabao ����ID�������������CD����
};

//-------------------------------------------------------------------------------------------

inline UInt32 GreateSoldier::GetBianshenRemainingTime()
{
	if (m_param.other_param.bianshen_end_timestamp < (UInt32)(EngineAdapter::Instance().Time()))
	{
		return 0;
	}
	else
	{
		auto remaining_time = m_param.other_param.bianshen_end_timestamp - (UInt32)(EngineAdapter::Instance().Time());
		return remaining_time;
	}
}

//-------------------------------------------------------------------------------------------

#endif // __GREATE_SOLDIER_HPP__