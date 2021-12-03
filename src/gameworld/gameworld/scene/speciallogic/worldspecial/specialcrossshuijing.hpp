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
	GameName name;															// 角色名

	int total_bind_gold;													// 总共-绑定元宝
	int total_mojing;														// 总共-魔晶
	int total_cross_honor;													// 总共-跨服荣誉

	int cur_limit_gather_times;												// 当前次-剩余的可采集次数
	unsigned int gather_buff_time;											// 采集不被打断buff时间
};

class SpecialCrossShuiJing : public SpecialLogic
{
	struct FlushItem // 将要刷新的采集物
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
	UserInfoMap m_user_info_cache;											// 玩家信息缓存，包括离线玩家

	int m_big_shuijing_num;													// 至尊水晶数量
};

#endif // __SPECIAL_CROSS_SHUIJING_HPP__
