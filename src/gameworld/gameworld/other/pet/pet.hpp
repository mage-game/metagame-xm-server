/******************************************************************************************
Description:本系统对应客户端的灵器系统,目前只修改了系统提示
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

	void ActivePet(int pet_id);												// 使用宠物蛋，激活宠物
	void SelectPet(int pet_id);												// 选择出战宠物
	void ChangeCurPetName(const GameName new_name);							// 修改当前出战宠物名字
	void UplevelCurPet();													// 升级当前出战宠物
	bool UpgradeCurPet(int is_auto_buy);									// 升阶（成长）当前出战宠物
	void ChouPet(int type, short is_gold_first);							// 抽宠物
	void RecycleEgg(ItemID egg_id);											// 回收宠物蛋（转化成指定数量的成长丹）
	void TransportRewardToKnapsack(int index, int is_fetch_all);			// 领取奖励
	void LearnSkill(int pet_skill_slot, int skill_id);						// 学习技能
	void UpgradeSkill(int pet_skill_slot);									// 升级技能
	void ForgetSkill(int pet_skill_slot);									// 遗忘技能

	void GetPetInfo(int *use_pet_id, short *pet_level, short *pet_grade, GameName pet_name, short *user_pet_special_img);
	short GetCurPetGrade();

	void SendPetInfo();
	void SendPetBackpackList();
	void SendPetQinmiAllInfo();

	bool ActiveSpecialImage(int imageid);                                   //激活宠物特殊形象
	bool UseImage(int imageid);                                             //使用宠物形象
	bool UplevelSpecialImage(const int imageid);							//升级幻化等级
	bool IsSpecialImageActive(int imageid)  { return (0 != (m_param.special_img_active_flag & (1 << imageid))); }

	bool PetQinmiPromote(int food_index, int use_pet_id);					// 提升亲密度
	void PetQinmiAutoPromote(int cur_pet_id);								// 一键升级亲密等级
	void FoodMarketChouOnce(bool is_gold_first);							// 吃货市场单次抽奖
	void FoodMarketChouTimes(int times, bool is_gold_first);				// 吃货市场多次抽奖
	void Put(const ItemConfigData &item_info);								// 放置物品到宠物仓库
	void PetPrint(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0);

	int GetCurPetQinmiLevel();
	Attribute GetPetIgnoreFangyuPercent() { return m_ignore_fangyu_percent; }

protected:
	void BroadcastPetInfo();
	void CheckIsActiveSuit(int cur_active_pet_id);
	void ChangePetSkill(int old_use_pet_id, int cur_on_use_pet_id);

	void SendPetQinmiSingleInfo(int pet_id);
	void ReCalcIgnoreFangyuPercent();										// 重算破防百分比

private:
	Role *m_role;
	PetParam m_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

	std::stack<int> m_empty_grid_index_list;								// 当前空的仓库格子下标

	Attribute m_ignore_fangyu_percent;											// 破防百分比
	int m_ignore_fangyu_percent_level;										// 破防级别
};

#endif

