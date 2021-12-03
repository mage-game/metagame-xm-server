/******************************************************************************************
Description:��ϵͳ��Ӧ�ͻ��˵�����ϵͳ,Ŀǰֻ�޸���ϵͳ��ʾ
******************************************************************************************/
#ifndef __PET_H__
#define __PET_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/petdef.hpp"
#include <set>
#include <stack>

class Pet
{
public:
	Pet();
	~Pet();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const PetParam &param);
	void GetInitParam(PetParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void ActivePet(int pet_id);												// ʹ�ó��ﵰ���������
	void SelectPet(int pet_id);												// ѡ���ս����
	void ChangeCurPetName(const GameName new_name);							// �޸ĵ�ǰ��ս��������
	void UplevelCurPet();													// ������ǰ��ս����
	bool UpgradeCurPet(int is_auto_buy);									// ���ף��ɳ�����ǰ��ս����
	void ChouPet(int type, short is_gold_first);							// �����
	void RecycleEgg(ItemID egg_id);											// ���ճ��ﵰ��ת����ָ�������ĳɳ�����
	void TransportRewardToKnapsack(int index, int is_fetch_all);			// ��ȡ����
	void LearnSkill(int pet_skill_slot, int skill_id);						// ѧϰ����
	void UpgradeSkill(int pet_skill_slot);									// ��������
	void ForgetSkill(int pet_skill_slot);									// ��������

	void GetPetInfo(int *use_pet_id, short *pet_level, short *pet_grade, GameName pet_name, short *user_pet_special_img);
	short GetCurPetGrade();

	void SendPetInfo();
	void SendPetBackpackList();
	void SendPetQinmiAllInfo();

	bool ActiveSpecialImage(int imageid);                                   //���������������
	bool UseImage(int imageid);                                             //ʹ�ó�������
	bool UplevelSpecialImage(const int imageid);							//�����û��ȼ�
	bool IsSpecialImageActive(int imageid)  { return (0 != (m_param.special_img_active_flag & (1 << imageid))); }

	bool PetQinmiPromote(int food_index, int use_pet_id);					// �������ܶ�
	void PetQinmiAutoPromote(int cur_pet_id);								// һ���������ܵȼ�
	void FoodMarketChouOnce(bool is_gold_first);							// �Ի��г����γ齱
	void FoodMarketChouTimes(int times, bool is_gold_first);				// �Ի��г���γ齱
	void Put(const ItemConfigData &item_info);								// ������Ʒ������ֿ�
	void PetPrint(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0);

	int GetCurPetQinmiLevel();
	Attribute GetPetIgnoreFangyuPercent() { return m_ignore_fangyu_percent; }

protected:
	void BroadcastPetInfo();
	void CheckIsActiveSuit(int cur_active_pet_id);
	void ChangePetSkill(int old_use_pet_id, int cur_on_use_pet_id);

	void SendPetQinmiSingleInfo(int pet_id);
	void ReCalcIgnoreFangyuPercent();										// �����Ʒ��ٷֱ�

private:
	Role *m_role;
	PetParam m_param;

	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ

	std::stack<int> m_empty_grid_index_list;								// ��ǰ�յĲֿ�����±�

	Attribute m_ignore_fangyu_percent;											// �Ʒ��ٷֱ�
	int m_ignore_fangyu_percent_level;										// �Ʒ�����
};

#endif

