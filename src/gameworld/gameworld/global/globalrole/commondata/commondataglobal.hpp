#ifndef __COMMONDATA_GLOBAL_HPP__
#define __COMMONDATA_GLOBAL_HPP__

#include "global/globalrole/globalrole.hpp"

#include <deque>

class Role;

class CommonDataGlobal : public IGlobalRoleItem
{
public:
	CommonDataGlobal(GlobalRole *g_role);
	~CommonDataGlobal();

	virtual void Init(const CommonDataGlobalParam &attr_data);
	virtual void GetAttr(CommonDataGlobalParam &attr_data);

	void OnRoleLogin(Role *role);
	void OnRoleLogout(Role *role);

	//���Ԫ������--------------------------------------------------------
	void AddUseGoldOnCross(int comsume_gold);

	//�Ϸ�BOSS����--------------------------------------------------------
	void AddCombineKillBoss(int count);
	void CheckAddCombineKillBossCount();

	// ����ս-------------------------------------------------------------
	void SetChengzhu();				// ���ó������
	void ClearChengzhu();			// ����������
	void SyncChengzhu();			// ͬ������ս��������
	void SyncChengzhuToCloss();		// ͬ������ս�������ܵ����

	// ��Ʒ����-------------------------------------------------------------
	void InitGiveItemRecord();
	void SaveGiveItemRecord();
	void AddGiveItemRecord(const ItemGiftInfo &info, bool is_give);		// ��������/�յ���¼
	void SendGiveItemRecord(Role *role, bool is_give);

	// ���������-------------------------------------------------------------
	void CheckReMoveInvalidId();							// ����Ƴ�����id
	void CalcNextClearTimes();						
	void AddCrossChallengefieldRankImageId(short rank, short mount_id, short title_id);

	short GetCrossChallengefieldRankMountId();
	void CheckSyncCrossChallengefieldRankMountId(bool is_add);

	short GetCrossChallengefieldRankTitleId();
	void CheckSyncCrossChallengefieldRankTitleId(bool is_add);

private:

	CommonDataGlobalParam m_param;

	std::deque<ItemGiftInfo> m_give_item_list;		// ���ͼ�¼
	std::deque<ItemGiftInfo> m_recieve_item_list;	// �յ���¼
};

#endif // __COMMONDATA_GLOBAL_HPP__
