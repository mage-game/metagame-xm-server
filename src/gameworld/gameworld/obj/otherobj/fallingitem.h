
#ifndef FALLINGITEM_H
#define FALLINGITEM_H

#include "obj/obj.h"
#include "config/fallingmsgconfig.hpp"

class Role;
class EquipmentParam;
struct ItemDataWrapper;
struct MailContentParam;

class FallingItem : public Obj
{
public:
	FallingItem();
	virtual ~FallingItem();

	bool IsCreate() const { return m_is_create; }
	UInt16 GetItemId() const { return m_item_id; }
	int GetCoinOrGold() const { return m_coin_or_gold; }
	const UserID & GetOwner() const { return m_owner_user_id; }
	UInt16 GetMonsterId() const { return m_monster_id; }
	int GetItemNum() const { return m_item_num; }
	unsigned int GetDropTime() { return static_cast<unsigned int>(m_drop_time); }
	bool IsPicked() { return m_has_pick; }
	bool GetItemIsBind() { return m_item_is_bind; }
	int GetSceneId() { return m_scene_id; }
	short isBroadcast() { return m_broadcast; }

	void SetBuffFalling(bool buff_falling) { m_is_buff_falling = buff_falling; }
	bool IsBuffFalling() { return m_is_buff_falling; }

	void SetBuffAppearan(int param_1) { m_buff_appearan = param_1; }
	int GetBuffAppearan() { return m_buff_appearan; }

	void Init(UInt16 item_id, int item_num, bool item_is_bind, int coin_or_gold, const UserID &owner_user_id, int scene_id, unsigned int disappear_time, 
			UInt16 monster_id, short is_broadcast, bool is_from_body);

	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	enum PICK_RESULT
	{
		PR_SUC=0,
		PR_EXCEPTION=-1,
		PR_NOT_YOURS=-2,
		PR_TOO_FAR=-3,
		PR_KNAPSACK_NO_SPACE=-4,
		PR_ROLE_DEAD=-5,
		PR_HAS_PICK=-6,
		PR_SCENE_SKILL_NO_SPACE = -7,
		PR_SCENE_SPECLAI_CAN_NOT_PICK = -8,
	};

	enum BUFF_FALLING_APPEARAN_TYPE
	{
		FALLING_APPEARAN_INVALID = 0,

		FALLING_APPEARAN_NSTF_BUFF_1,
		FALLING_APPEARAN_NSTF_BUFF_2,
		FALLING_APPEARAN_NSTF_BUFF_3,
		FALLING_APPEARAN_NSTF_BUFF_4,

		FALLING_APPEARAN_YZDD_BUFF,

		FALLING_APPEARAN_MAX,
	};

	int Pick(Role *role, bool is_auto_pick = false, MailContentParam *contentparam = NULL);
	bool GetPickItem(ItemDataWrapper &item_wrapper, EquipmentParam *equip_param);
	static bool GetPickItem(int monster_id, ItemID item_id, int item_num, bool is_bind, ItemDataWrapper &item_wrapper, EquipmentParam *equip_param, int scene_id = 0);

	void OnPickGuildRareItemInHidden(Role *role);

	void *			operator new(size_t c);
	void			operator delete(void *m);

private:
	bool				m_is_create;										// 是否刚创建，第一次update时设会false
	UInt16				m_item_id;											// 物品ID
	int					m_item_num;											// 物品数量
	bool				m_item_is_bind;										// 物品是否绑定
	UserID				m_owner_user_id;									// 拥有者ID
	int					m_scene_id;											// 掉落场景ID

	time_t				m_drop_time;										// 掉落时间
	time_t				m_disappear_time;									// 消失时间
	int					m_coin_or_gold;										// 掉落铜币或者绑定元宝
	bool				m_has_pick;											// 是否被拾取
	UInt16				m_monster_id;										// 关联的怪物ID
	short				m_broadcast;										// 拾取时传闻类型
	bool				m_is_from_body;										// 是否来自尸体
	bool				m_is_buff_falling;									// 是否是buff掉落物
	int					m_buff_appearan;									// buff掉落物参数

	bool IsOwner(Role *role);
};

#endif
