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

	void OnInitFinish();												   // ��Ҫ��ʼ������Ĳ���������

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void Update(time_t now_second);

	void ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all);

	bool IsCanOpera(int upgrade_type, int opera_type);						// ����ܷ����
	void OnRoleOpera(Protocol::CSUpgradeOperaReq *upr);						// �������

	int GetOperaModuleType(int opera_type);									// �ɲ������ͻ�ȡģ������

	bool IsValidUpgradeType(int upgrade_type);								// �жϽ���ϵͳ�Ƿ���Ч

	//�������---------------------------------------------------------------
	void OnLogin();
	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	//�ⲿ�������---------------------------------------------------------------
	Upgrade * GetUpgrade(int upgrade_type);									// ��ȡ����ϵͳ
	UpgradeSkill & GetUpgradeSkill() { return m_upgrade_skill; }			// ��ȡ���׼���

	void GetBaseAttr(int upgrade_type, CharIntAttrs &tmp_attr);				// ��ȡ����ϵͳ��������
	void GetRoleAppearance(RoleAppearance *appearance);						// ��ȡ�����Ϣ

	bool OnUseShuxingdan(int upgrade_type, int slot_idx, short &use_count);	 // ʹ�����Ե�

	bool UpOneGrade(int upgrade_type, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num); // ֱ��һ��

	bool IsImageActive(int upgrade_type, int image_id);						// �����Ƿ񼤻�
	bool ActiveImage(int upgrade_type, int image_id);						// ��������
	bool OnImgUpgrade(int upgrade_type, int image_id);						// �������
	short GetUseImgID(int upgrade_type);									// ��ȡʹ���е�����id
	bool IsSpecialImg(int upgrade_type, int img_id);						// �ж��Ƿ�����������

	int GetGrade(int upgrade_type);											// ��ȡ����
	int GetSkill0Level(int upgrade_type);									// ��ȡ����0�ȼ�
	
	bool GMOpera(Role *role);		// GM

	int GetSpecialImageLevelById(int upgrade_type, int imageid);
private:

	Role *m_role;
	UpgradeSkill m_upgrade_skill;				// ����ϵͳ����

	Upgrade *m_upgrade_list[UPGRADE_TYPE_MAX];  // ����ϵͳ�б�

};

#endif // 

