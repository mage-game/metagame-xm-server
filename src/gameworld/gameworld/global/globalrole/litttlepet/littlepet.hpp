#ifndef __LITTLEPET_HPP__
#define __LITTLEPET_HPP__

#include "littlepetconfig.hpp"
#include "obj/character/attribute.hpp"
#include "global/globalrole/globalrole.hpp"
#include "protocal/msglittlepet.h"

#include <vector>

class Role;

static const int LITTLE_PET_CHONGJIANG_TYPE_ONE = 1;										//小宠物1抽
static const int LITTLE_PET_CHONGJIANG_TYPE_TEN = 10;										//小宠物10抽	
UNSTD_STATIC_CHECK(LITTLE_PET_CHONGJIANG_TYPE_TEN <= LITTLE_PET_MAX_CHOU_COUNT);

//强化点类型
enum QIANG_HUA_POINT_TYPE
{
	QIANG_HUA_POINT_TYPE_MIN = 0,
	QIANG_HUA_POINT_TYPE_GONGJI= QIANG_HUA_POINT_TYPE_MIN,									//攻击
	QIANG_HUA_POINT_TYPE_HP,																//气血
	QIANG_HUA_POINT_TYPE_MINGZHONG,															//防御
	QIANG_HUA_POINT_TYPE_FANGYU,															//命中
	QIANG_HUA_POINT_TYPE_SHANBI,															//闪避
	QIANG_HUA_POINT_TYPE_BAOJI,																//暴击
	QIANG_HUA_POINT_TYPE_JIANREN,															//坚韧
	QIANG_HUA_POINT_TYPE_SPECIAL,															//特殊
	QIANG_HUA_POINT_TYPE_MAX = QIANG_HUA_POINT_TYPE_SPECIAL,
};
UNSTD_STATIC_CHECK(QIANG_HUA_POINT_TYPE_MAX < LITTLEPET_QIANGHUAPOINT_CURRENT_NUM);

//格子属性值类型
enum  LITTLEPET_VALUE_TYPE
{
	LITTLEPET_VALUE_TYPE_MIN = 0,

	LITTLEPET_VALUE_TYPE_HP = LITTLEPET_VALUE_TYPE_MIN,										//气血
	LITTLEPET_VALUE_TYPE_GONGJI,															//攻击
	LITTLEPET_VALUE_TYPE_FANGYU,															//防御
	LITTLEPET_VALUE_TYPE_MINGZHONG,															//命中
	LITTLEPET_VALUE_TYPE_SHANBI,															//闪避
	LITTLEPET_VALUE_TYPE_BAOJI,																//暴击
	LITTLEPET_VALUE_TYPE_KANGBAO,															//抗暴

	LITTLEPET_VALUE_TYPE_MAX = LITTLEPET_VALUE_TYPE_KANGBAO,
};
UNSTD_STATIC_CHECK(LITTLEPET_VALUE_TYPE_MAX < LITTLEPET_VALUE_TYPE_MAX_NUM);

enum class LITTLEPET_ROLE_IDLE_STATUS
{
	NOT_IN_IDLE_STATUS,
	IN_IDLE_STATUS,
};

class LittlePet : public IGlobalRoleItem
{
public:
	LittlePet(GlobalRole *g_role);
	~LittlePet();

	virtual void Init(const LittlePetData &little_pet_attr);
	virtual void GetAttr(LittlePetData &little_pet_attr);

	void Update(unsigned int now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc, Role *role);
	void OnRoleLogin(Role *role);

	bool ActiveLittlePet(ItemID id);
	void Remove(int index);
	void RecyclePetItem(int item_id, int num, bool is_bind = true, bool is_relive = false);
	void RecycleStuffItem(int item_id, int index, bool is_bind, bool is_relive);
	void Replace(int index, ItemID item_id);
	void ChangeImg(int index);
	void ReName(int index, GameName new_name);
	void Strengthen(Role *role, int index, int point_type, int grid_index);
	
	// 宠物喂养(废弃)
	void FeedLittlePet(Role *role, int index);
	// 宠物喂养
	void FeedLittlePetNew(Role *role, int index, bool is_auto_buy);

	// 互动
	void Interact(Role *role, int index);

	void Choujiang(int type);
	void AddScore(int score, const char * reason);
	void ExchangeItem(int seq, int num);

	//宠物穿戴、卸下
	bool PutOn(int pet_index, int knapsack_index);
	bool TakeOff(int pet_index);
	//宠物装备穿戴、卸下、升级
	bool EquipmentPutOn(Role *role, int pet_index, int knapsack_index);
	bool EquipmentTakeOff(Role *role, int pet_index, int equip_index);
	bool OnEquipmentUplevel(Role *role, int pet_index, int equip_index, bool is_auto_buy);

	// 回收返还喂养材料
	void RecycleReturnStuffList(int cur_feed_level, ItemID recycle_itemid, std::vector<ItemConfigData> &item_list);

	int GetLoverId();
	void GetTotalAttr(CharIntAttrs &tmp_attr);
	int GetUsingPetId();
	void GetCurPetName(GameName &name);
	int	GetScore();
	void GetInteractLog(Protocol::SCLittlePetInteractLog &log_info);
	void GetPetBriefInfo(Protocol::SCLittlePetFriendPetListInfo &pet_info, int info_type = 1);
	void GetAllPetInfo(Protocol::SCLittlePetAllInfo &all_info, int info_type);

	void SendRoleInteractTimes();
	void SendPetFriendList();
	void SendToFrienPetInfo(Role *role);
	void SendInteractLog();
	void SendAllInfo();
	void BroadcastTaskStatus(Role *role);

	void SetLoverId(int lover_id, bool is_calc = true);

	void OnLittlePetWalk(Role *role, int is_idle_status);
	int GetPetId(int index);

private:
	Role * GetLoverRole();
	int GetRealType(int vutual_type);
	void SetInteractLog(Role *target_role, LittlePetParam &pet_info);
	void GetPetBaseAttr(LittlePetParam &tem_attrs, const LittlePetBaseCfg * base_cfg);
	bool GetSingleInfo(Protocol::LittlePetSingleInfo &tmp_pet, int info_type, int index);
	void SendSingleInfo(Role *role, int index, int info_type = 1);
	void SendChangeInfo(int index, int point_type, int grid);
	void SendFeedDegree(int index, int value);
	void SendPetNewName(int index);
	void SendFreeChouTimestamp();
	void SendScoreInfo(Role * m_role);
	void SendPetInteractTimes(Role *role, int index);
	void RecalNextFeedDegreeTime();
	unsigned int GetNextCheckFeedDegreeTime();

	unsigned int m_next_check_feed_degree_time;												// 下一次有宠物饱食度过期时间
	CharIntAttrs m_attrs_add;
	LittlePetData m_little_pet_param;
};


#endif // __LITTLEPET_HPP__
