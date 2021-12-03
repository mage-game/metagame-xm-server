/******************************************************************************************
FileName: checkresource.hpp
Author: dreamwang
Description: 资源检查
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/01/31/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __CHECK_RESOURCE_CENTER_HPP__
#define __CHECK_RESOURCE_CENTER_HPP__

#include <vector>

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"


#include "gamedef.h"

class CheckResourceCenter
{
public:
	static CheckResourceCenter & Instance();

	bool Check(std::string *err);

private:
	CheckResourceCenter();
	~CheckResourceCenter();

	bool CheckHelper(std::string *err);
	void Clear();

	// 套装关联检查
	struct SuitCheckStruct
	{
		void Add(ItemID equip_id, UInt16 suit_id);
		bool Check(std::string *err);

		std::vector<std::pair<ItemID, UInt16> > equip_suit_vec;
	};

	SuitCheckStruct m_suit_check;

	// 宠物蛋关联检查
	struct PetEggCheckStruct
	{
		void Add(ItemID item_id, UInt16 pet_config_id);
		bool Check(std::string *err);

		std::vector<std::pair<ItemID, UInt16> > pet_vec;
	};

	PetEggCheckStruct m_pet_egg_check;

	// 任务检查
	struct TaskCheckStruct
	{
		struct CheckItem
		{
			CheckItem(TaskID _task_id, bool _is_must_common_task) : task_id(_task_id), is_must_common_task(_is_must_common_task) {}

			TaskID task_id;
			bool is_must_common_task;
		};

		void Add(TaskID task_id, bool is_must_common_task);
		bool Check(std::string *err);

		std::vector<CheckItem> task_vec;
	};
	TaskCheckStruct m_task_check;

	// 物品是否有效
	struct ItemValidCheckStruct
	{
		struct CheckItem
		{
			CheckItem(ItemID _item_owner, ItemID _item_id, int _num) : item_owner(_item_owner), item_id(_item_id), num(_num) {}

			ItemID item_owner;
			ItemID item_id;
			int num;
		};

		void Add(ItemID item_owner, ItemID item_id, int num);
		bool Check(std::string *err);

		std::vector<CheckItem> item_vec;
	};
	
	ItemValidCheckStruct m_item_valid_check;

	// 血包、魔包值和间隔是否相同
	struct PackageCheckStruct
	{
		PackageCheckStruct() : is_init(false), value(0), interval(0) {}

		bool is_init;
		int value;
		int interval;
	};

	PackageCheckStruct m_hp_package_check;
	PackageCheckStruct m_mp_package_check;

	// 物品上的任务是否存在
	struct ItemTaskCheckStruct
	{
		void Add(ItemID item_id, TaskID task_id);
		bool Check(std::string *err);

		std::vector<std::pair<ItemID, TaskID> > item_task_vec;
	};

	ItemTaskCheckStruct m_item_task_check;

	// NPC关联任务是否存在
	struct NPCTaskCheckStruct
	{
		void Add(UInt16 npc_id, TaskID task_id);
		bool Check(std::string *err);

		std::vector<std::pair<UInt16, TaskID> > npc_task_vec;
	};

	NPCTaskCheckStruct m_npc_task_check;

	// 技能召唤的怪物是否存在
	struct SkillCallMonsterCheckStruct
	{
		void Add(UInt16 skill_id, int monster_id);
		bool Check(std::string *err);

		std::vector<std::pair<UInt16, int> > skil_monster_vec;
	};

	SkillCallMonsterCheckStruct m_skill_call_monster_check;

	// 是否商城有卖的物品
	struct ItemSellCheckStruct
	{
		void Add(ItemID item_id);
		bool Check(std::string *err);

		std::vector<ItemID>	item_vec;
	};

	ItemSellCheckStruct m_item_sell_check;

	// 另一性别物品检查
	struct OtherSexItemCheckStruct
	{
		void Add(ItemID item_id, ItemID other_sex_itemid);
		bool Check(std::string *err);

		std::vector<std::pair<ItemID, ItemID> >	item_vec;
	};

	OtherSexItemCheckStruct m_other_sex_item_check;

	// 怪物召唤的怪是否存在
	struct MonsterCallMonsterCheckStruct
	{
		void Add(UInt16 monster_id, UInt16 call_monster_id);
		bool Check(std::string *err);

		std::vector<std::pair<UInt16, UInt16> > monster_call_monster_vec;
	};

	MonsterCallMonsterCheckStruct m_monster_call_monster_check;

	// 场景是否存在
	struct SceneCheckStruct
	{
		void Add(int scene_id);
		bool Check(std::string *err);

		std::vector<int> scene_vec;
	};

	SceneCheckStruct m_scene_check;

	// 宠物资质检查
	struct PetZizhiCheckStruct
	{
		void Add(short pet_id);
		bool Check(std::string *err);

		std::vector<short> pet_vec;
	};

	PetZizhiCheckStruct m_pet_zizhi_check;

	// 主动使用类道具检查
	struct NorexItemCheckStruct
	{
		void Add(ItemID item_id);
		bool Check(std::string *err);

		std::vector<ItemID> item_vec;
	};

	NorexItemCheckStruct m_norex_check;

	// 世界事件检查
	struct WorldEventCheckStruct
	{
		void Add(unsigned short event_id);
		bool Check(std::string *err);

		std::vector<unsigned short> worldevent_vec;
	};

	WorldEventCheckStruct m_worldevent_check;

	// boss cond skill 检查
	struct BossCondSkillCheckStruct
	{
		void Add(UInt16 monster_id, unsigned short cond_id);
		bool Check(std::string *err);

		std::vector<std::pair<UInt16, unsigned short> > cond_vec;
	};

	BossCondSkillCheckStruct m_condskill_check;

public:
	SuitCheckStruct * GetSuitCheck() { return &m_suit_check; }
	PetEggCheckStruct * GetPetEggCheck() { return &m_pet_egg_check; }
	TaskCheckStruct * GetTaskCheck() { return &m_task_check; }
	ItemValidCheckStruct * GetItemValidCheck() { return &m_item_valid_check; }
	bool CheckPackage(bool is_hp_package, int value, int interval);
	ItemTaskCheckStruct * GetItemTaskCheck() { return &m_item_task_check; }
	NPCTaskCheckStruct * GetNPCTaskCheck() { return &m_npc_task_check; }
	SkillCallMonsterCheckStruct * GetSkillCallMonsterCheck() { return &m_skill_call_monster_check; }
	void AddSellItem(ItemID item_id) { m_item_sell_check.Add(item_id); }
	OtherSexItemCheckStruct * GetOtherSexItemCheck() { return &m_other_sex_item_check; }
	MonsterCallMonsterCheckStruct * GetMonsterCallMonsterCheck() { return &m_monster_call_monster_check; }
	SceneCheckStruct * GetSceneCheck() { return &m_scene_check; }
	PetZizhiCheckStruct * GetPetzizhiCheck() { return &m_pet_zizhi_check; }

	NorexItemCheckStruct * GetNorexCheck() { return &m_norex_check; }
	WorldEventCheckStruct * GetWorldEventCheck() { return &m_worldevent_check; }
	BossCondSkillCheckStruct * GetBossCondSkillCheck() { return &m_condskill_check; }
};

#endif // __CHECK_RESOURCE_CENTER_HPP__
