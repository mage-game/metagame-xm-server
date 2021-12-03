#ifndef __BABY_H__
#define __BABY_H__

#include "servercommon/babydef.hpp"
#include "obj/character/attribute.hpp"
#include <set>

class Role;

class Baby
{
public:
	Baby();
	~Baby();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const BabyParam &param);
	void GetInitParam(BabyParam *param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	
	bool UpgradeBaby(short baby_index, int is_auto_buy);								// ����
	void UplevelBaby(short baby_index);													// ����
	void ChangeBabyName(short baby_index, const GameName new_name);						// ������
	void RequestCreateBaby(int qifu_type);												// ������
	void ReplyCreateBabyRequest(int type, bool is_accept);								// ��������
	void RequestChaosheng();															// ��������
	bool TrainBabySpirit(int baby_index, int spirit_id, bool is_auto_buy);				// ��������

	void OnRemoveBabyReq(int baby_index);												// �����������󣨵ȴ�ȷ�ϣ�
	void OnRemoveBabyRet(int req_baby_index, long long baby_unique_id, bool is_agree);	// ��Ӧ��������
	void RemoveBaby(int baby_index);													// ��������
	
	short GetBabyGrade(short baby_index);
	short GetSpiritLevel(short baby_index, short spirit_id);

	void LoverRequestCreateBaby(int type);
	void SendBabyInfo(short baby_index);
	void SendAllBabyInfo();
	void SendBabySpiritInfo(int baby_index);

	void CheckSyncCache();

	void OnBabyWalk(int is_idle_status);

private:
	bool CheckBabyNeedOtherBabyGrade();
	bool CheckBabyListIsFull();
	int GetUnusedBabyIndex();
	int GetChaoshengCount() { return m_param.chaosheng_count; }
	void AddChaoshengCount();

	void SyncBabyCacheInfo( long long unique_id );
	int GetBabyIndex(long long baby_unique_id);
	void GiveBaby(short baby_id, const GameName name, long long unique_id, int index);

	Role *m_role;
	BabyParam m_param;

	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ

	int m_lover_request_create_baby_type;

	std::set<int> m_dirty_baby_index_list;

};

#endif

