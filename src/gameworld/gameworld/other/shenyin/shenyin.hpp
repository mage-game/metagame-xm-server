#ifndef __SHEN_YIN_HPP__
#define __SHEN_YIN_HPP__

#include "global/virtualbag/virtualbag.hpp"
#include "servercommon/shenyindef.hpp"
#include <map>
#include <set>

class Role;
class CharIntAttrs;

namespace Protocol
{
	class CSShenYinOneKeyRecyleReq;
}

enum SHEN_YIN_SCORE_NOTIFY_TYPE
{
	SHEN_YIN_SCORE_NOTIFY_TYPE_INVALID = -1,

	SHEN_YIN_SCORE_NOTIFY_TYPE_EXCHANGE,	// add_score = 兑换积分增加值
	SHEN_YIN_SCORE_NOTIFY_TYPE_LIEHUN,		// add_score = 猎魂积分增加值
};

class ShenYinVirtualBag : public VirtualBag<ShenYinPastureSpiritGridParam>
{
public:
	ShenYinVirtualBag(ShenYinPastureSpiritGridParam *bag_list, int size)
		: VirtualBag<ShenYinPastureSpiritGridParam>(bag_list, size)
	{
	}

	void SendBagInfoByIndex(int index) const;
	void SendBagAllInfo(INDEX_LIST *index_list = NULL) const;
private:
	void OnBagChange(INDEX_LIST *index_list = NULL) const;
	void PrintLogPut(int virtual_item_id, int num, bool is_bind, const char *reason) const;
	void PrintLogConsume(int virtual_item_id, int num, const char *reason) const;
	int GetStackLimitByVid(short virtual_item_id) const;
};

class ShenYin
{
	struct BeadPosi
	{
		BeadPosi() : x(-1), y(-1) {}
		BeadPosi(int _x, int _y): x(_x), y(_y) {}

		void Reset()
		{
			x = -1;
			y = -1;
		}

		void Set(int _x ,int _y)
		{
			x = _x;
			y = _y;
		}

		bool operator<(const BeadPosi &right)const
		{
			if (x < right.x)
			{
				return true;
			}
			if (x > right.x)
			{
				return false;
			}

			return y <= right.y;
		}

		short x;
		short y;
	};

public:
	ShenYin();
	~ShenYin();

	void InitParam(Role *role, const ShenYinParam &param);
	void GetInitParam(ShenYinParam *param) const;
	void SetRole(Role *role) { m_role = role; }
	void Update(unsigned int now_second);
	void OnRoleLogin();

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void ReqOpera(int info_type, int param1, int param2, int param3, int param4);			// 操作请求
	void OneKeyRecycleOpera(const Protocol::CSShenYinOneKeyRecyleReq *cmd);					// 批量回收操作

	void ShenYinUpLevel(int slot_type, bool is_use_protect, bool is_auto_buy);				// 神印升星
	void ShenYinUpGrade();
	//void ShenYinEquip(int v_bag_index, int slot_type);										// 装备神印
	//void ShenYinTakeOff(int slot_type);														// 卸下神印
	void ShenYinAddAttrCount(int slot_type);												// 洗炼（增加条数）
	void ShenYinFlushAttrType(int slot_type, int lock_flag);								// 洗炼（洗炼属性）
	void ShenYinFlushAttrValue(int slot_type, int lock_flag);								// 洗炼（洗炼数值）
	void ShenYinAplyFlushResult(int slot_type);												// 保存洗炼属性
	//void ShenYinRecycle(int bag_index, int num = 1);										// 神印回收
	void ShenYinExchange(int index);														// 神印兑换
	//void SortVirtualBag() { m_virtual_bag.SortBag(); }										// 整理背包
	int ShenYinChouhun(bool notify_client, int chouhun_type, bool is_use_score);			// 神印猎魂
	void ShenYinSuperChouhun();
	void ShenYinBatchChouhun(bool is_use_score);											// 一键猎魂
	void ShenYinPutInBagByChouHunPool(int pool_idx, bool is_notice = true);					// 猎魂池放入神印背包
	void ShenYinOnekeyPutInBagByChouHunPool();												// 一键放入神印背包
	void ShenYinChouhunPoolConverToScore();													// 猎魂池一键回收
	void ShenyinChouhunPoolSingleItemConverToScore(int pool_idx);							// 猎魂池回收

	//bool TranstItemToVitem(ItemID item_id, int item_num, bool is_bind);
	bool PutOnItem(ItemID item_id, int item_num, bool is_bind);
	int  PutOnItem(ItemGridData &item_data);
	bool TakeOffToKnapsack( int slot_type,bool is_puton);
	bool TrantsItem();
	void AddShenYinChouHunScore(int score);
	void SendAllInfo();

	void GmAddImpritScore(int num);
	void OnDayChange(int old_dayid, int now_dayid);
	void OnCompleteTask(TaskID task_id);
	void OnRoleLevelChange(int level);
	void ReCalcSuitCount();
	int GetEquipCount(int quality);
	int GetSuitEquipCount(int quality);														//根据品质获取套装数量，注：粉装也算红装

	void NewShenyinRecycle(short index, short discard_num);									// 神印分解（新）

protected:
	bool Activebead(int x, int y);															// 激活神印天象珠子
	void ChangeGrid(int src_x, int src_y, int target_x, int target_y);						// 珠子移动位置
	void ChangeBeadType(int x, int y, int target_type);										// 改变珠子颜色

	void CheckCombineAll();
	bool CheckCombine(int x, int y);
	bool IsActiveCombine(int seq);
	bool CanActiveCombine(int seq, int src_type, const BeadPosi &src_posi);
	void ResetCombine(const ShenYinTableGridParam &grid);

	static bool isValidPosi(const int x, const int y);																						// 检查是否非法下标

	void SendShenYinTianXiangAllInfo() const;
	void SendSingleInfo(int x, int y) const;
	void SendCombineInfo() const;
	void SendShenYinScoreInfo(int type = (int)SHEN_YIN_SCORE_NOTIFY_TYPE_INVALID, int add_score = 0) const;
	void SendShenYinSlotInfo(int *slot_type = NULL) const;
	void SendShopInfo(int *index = NULL) const;
	void SendLieHunPoolInfo() const;

	void GetRandAttr(int slot_type);
	static bool IsInvalidSlotType(int slot_type) { return slot_type < 0 || slot_type >= SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT; };
	bool IsHaveEnoughImprintScore(int score) const { return m_param.imprint_score >= score; }
	void AddShenYinScore(int score);
	void CheckFuncOpen();
private:
	Role *m_role;
	
	ShenYinParam m_param;
	CharIntAttrs m_attrs_add;
	std::map<int, BeadPosi> m_combine_map;
	std::set<BeadPosi> m_tmp_posi_set;
	int m_bead_count;

	//ShenYinVirtualBag m_virtual_bag;

	int m_liehun_pool_hunshou_count;

	int m_quality_equip_count[SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK + 1];
};

#endif
