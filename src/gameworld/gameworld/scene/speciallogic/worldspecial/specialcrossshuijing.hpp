#ifndef __SPECIAL_CROSS_SHUIJING_HPP__
#define __SPECIAL_CROSS_SHUIJING_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <list>
#include <map>
#include "servercommon/serverconfig/crossshuijingconfig.hpp"

struct CrossShuijingPlayerInfo
{
	CrossShuijingPlayerInfo() : uuid(INVALID_UNIQUE_USER_ID), total_bind_gold(0), total_mojing(0), total_cross_honor(0),
		cur_limit_gather_times(0), gather_buff_time(0)
	{
		memset(name, 0, sizeof(name));
	}

	UniqueUserID uuid;
	GameName name;															// ��ɫ��

	int total_bind_gold;													// �ܹ�-��Ԫ��
	int total_mojing;														// �ܹ�-ħ��
	int total_cross_honor;													// �ܹ�-�������

	int cur_limit_gather_times;												// ��ǰ��-ʣ��Ŀɲɼ�����
	unsigned int gather_buff_time;											// �ɼ��������buffʱ��
};

class SpecialCrossShuiJing : public SpecialLogic
{
	struct FlushItem // ��Ҫˢ�µĲɼ���
	{
		FlushItem() : gather_id(0), flush_time(0) {}

		int gather_id;
		unsigned int flush_time;
	};

public:
	SpecialCrossShuiJing(Scene *scene);
	virtual ~SpecialCrossShuiJing();

	virtual void OnAddObj(Obj *obj);
	virtual void OnRemoveObj(Obj *obj);
	virtual bool CanDestroy() { return false; }
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialStopGahterOnAttack(Role *role);

	void OnActivityOpen();
	void OnActivityClose();
	void OnBuyGatherBuff(Role *role);

protected:
	Role * GetRole(ObjID obj_id);
	CrossShuijingPlayerInfo * GetPlayerInfo(UniqueUserID uuid);

	void SendPlayerInfoToPlayer(Role *role);
	void UpdateBigGatherNum();

	typedef std::map<UniqueUserID, CrossShuijingPlayerInfo> UserInfoMap;
	UserInfoMap m_user_info_cache;											// �����Ϣ���棬�����������

	int m_big_shuijing_num;													// ����ˮ������
};

#endif // __SPECIAL_CROSS_SHUIJING_HPP__
