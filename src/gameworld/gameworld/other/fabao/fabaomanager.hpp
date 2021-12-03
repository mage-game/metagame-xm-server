#ifndef __FABAOMANAGER_H__
#define __FABAOMANAGER_H__

class Role;
class Character;
struct RoleAppearance;

#include "obj/character/attribute.hpp"
#include "servercommon/struct/fabaoparam.hpp"

class FabaoManager
{
public:
	FabaoManager();
	~FabaoManager();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const FabaoParam &param);
	void GetInitParam(FabaoParam *param);
	FabaoParam* GetParam() { return  &m_param; }
	void OnLogin();                                                      // �ȷ��ţ��Ժ�����õõ�
	void Update(unsigned int now_second);                                   // ˢ��  
	void OnRoleUpLevel(int level);                                          // ��ɫ����ʱ���ã����ڼ����
	bool ActiveFabao(bool is_recal = true);                                 // �����

	void SendInfo();                                                        // ���ͷ�����Ϣ
	void GetRoleAppearance(RoleAppearance *appearance);                     // ��ȡ���
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);                // ����ӳ�

	bool UpOneGrade(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);				// ʹ�ý��׵�ֱ�ӽ���
	bool OnUpGrade(bool is_auto_buy, int repeat_times = 1);                                       // ����
	void UseImg(short img_id);                                              // ʹ������

	void UseSpecialImg(short special_img_id);                               // ʹ����������
	void UnUseSpecialImg(short special_img_id);                             // ��ʹ����������

	bool ActiveSpecialImg(short special_img_id, bool is_notify = true);		// ������������
	bool UnActiveSpecialImg(short special_img_id);                          // ȡ��������������
	bool FabaoSpecialImgUpgrade(short special_img_id);                      // ���������������
	int	GetHasActiveFabao();                                                // ��ȡ�Ѿ����������������Ŀ
	bool OnUseShuxingdan(int slot_idx, short &use_count);                      // ʹ�����Ե�(���ʵ�)
	bool UplevelSkill(int skill_idx, bool is_auto_buy);                     // ��������
	int GetHurtDecPercent();                                                // ������Ҵ��Լ�ʱ���ٵ��˺��ٷֱ�
	int GetFabaoEquipSkillLevel() { return m_param.equip_skill_level; }     // ��ȡ����װ�����ܵȼ�
	bool UplevelEquip(int equip_idx);                                       // װ������������Ϊװ������
	int RefreshFabaoMinEquipLevel();                                        // �ҵ�װ����͵ȼ�

	//void OnRoleBeenAttack(ObjID attacker);
	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure);

	void OnCompleteTask(TaskID task_id);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckActive();

	int GetGrade() { return m_param.grade; }								// ��ȡ��ǰ����

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetSpecialImageLevelById(int imageid);

	void CalcBaseAttrAddFromSkill(CharIntAttrs &base_add);					// �����ṩ�Ļ������Լӳ�

	int GetMedalSkillLevel();												// ��ȡѫ�¼��ܵȼ�

private:                           
	const static int NEXTDAY_CLEAR_BLESS_VAL_TIME = 5;                      //�ڶ������ף��ֵ����
	const static int CLEAR_BLESS_VAL_GRADE_LIMIT = 5;                       //���ڵ���4�׵ڶ������ף��ֵ����
	Role *m_role;                                                           // ��ɫ
	FabaoParam m_param;                                                     // ��������

	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ

	UInt64 m_skill0_last_colddown_time;										// ��һ�μ�����ȴ����ʱ��

	int m_other_capability;
};

#endif
