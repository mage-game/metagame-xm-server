#ifndef __UPGRADE_SYS_HPP__
#define __UPGRADE_SYS_HPP__

#include "servercommon/struct/upgradeparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;
class UpgradeManager;
struct SkillConifg;

class Upgrade
{
public:
	Upgrade(UpgradeManager *upgrade_manager, int upgrade_type);
	virtual ~Upgrade();

	virtual void Init(Role *role, const UpgradeBaseData &upgrade_base_data);
	virtual void GetInitParam(UpgradeBaseData *upgrade_base_data);

	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void Update(time_t now_second);

	virtual void OnLogin();

	virtual void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	bool OnFightOut(bool is_fight_out, bool notify = true, bool is_force = false);
	bool IsFightOut();

	int GetUpgradeAppeID();													// ��ȡ����ID

	UpgradeBaseData * GetBaseData();										// ��ȡϵͳ����
	int GetUpgradeType() { return m_upgrade_type; }							// ��ȡϵͳ����
	int GetUpgradeCritType();												// ��ȡϵͳ��������

	bool CheckCanOperate();													// ����ܷ����

	bool OnConsumeUpgradeStuff(ItemID stuff_id, ItemID stuff2_id, int total_consume_count, bool is_auto_buy, std::string reason);   // ����ϵͳ�������Ľӿ�

	//�������---------------------------------------------------------------
	 bool OnUpgrade(bool is_auto_buy, int repeat_times = 1);										// ����

	 bool CheckCanUpOneGrade(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);	// ����ܷ�ֱ��һ��
	 bool OnUpOneGrade();													// ֱ��һ��

	//�������---------------------------------------------------------------
	bool ActiveImage(int imageid, bool recalc = true);						// ��������
	bool UnActiveImage(int imageid);										// ��ԭ����
	bool IsImageActive(int imageid);										// �ж��Ƿ񼤻�

	bool UseImage(int imageid);												// ʹ������
	bool UnUseImage(int imageid);											// ��ʹ������

	bool UseTemporaryImage(int imageid);									// ʹ����ʱ����

	bool CheckCanImgUpgrade(int img_id);									// ����ܷ��������
	bool OnImgUpgrade(int img_id);											// �������

	void OnUpgradeAppeChange();												// ��۸ı�

	//װ�����---------------------------------------------------------------
	bool CheckCanUplevelEquip(int equip_idx);								// �ܷ�����װ��
	bool OnUplevelEquip(int equip_idx);										// ����װ��

	//�������---------------------------------------------------------------
	bool CheckCanUpLevelSkill(int skill_idx, bool is_auto_buy);				// �ܷ���������
	bool OnUplevelSkill(int skill_idx);   									// ��������
	void OnAddSkillAttr(CharIntAttrs &attrs, const SkillConifg *skill_cfg);	// ��ȡ���ͼ�������

	//�������---------------------------------------------------------------
	void OnCompleteTask(TaskID task_id);									// �������
	void OnRoleUpLevel(int level);											// ��ɫ����

	void CheckActiveSys();													// ����ܷ񼤻�
	bool ActiveSys(bool is_recal = true);									// ����ϵͳ

	bool OnUseShuxingdan(int slot_idx, short &use_count);					// ʹ�����Ե�

	void SendUpgradeInfo();													// ������Ϣ

	void PrintUpgradeLog(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	//�ⲿ�������---------------------------------------------------------------
	void GetBaseAttr(CharIntAttrs &tmp_attr);								// ��ȡϵͳ��������

	int GetSkill0Level(){return m_base_data.skill_level_list[0];}			// ��ȡ����0�ȼ�
	int GetGrade() { return m_base_data.grade; }							// ��ȡ����

	bool IsSpecialImg(int img_id);											// �Ƿ�����������

	void SuperSpecialCapAdd(int image_id, int grade, CharIntAttrs &m_attrs_add);	//��ǿս���ӳ�

	bool GMResetGrade(Role *role);		// GM���ý���

	int GetImageLevel(int imageid);

	void CalcBaseAttrAddFromSkill(CharIntAttrs &base_add);					// �����ṩ�Ļ������Լӳ�

	int GetMedalSkillLevel();												// ��ȡѫ�¼��ܵȼ�

private:
	Role *m_role = nullptr;
	UpgradeBaseData m_base_data;
	CharIntAttrs m_attrs_add;								// �����Լӳ���ֵ

	UpgradeManager *m_upgrade_manager = nullptr;			// ����ϵͳ������

	int m_upgrade_type = UPGRADE_TYPE_INVALID;				// ��������

	int m_other_capability = 0;								// ��������ս��

};

#endif // 


