#ifndef __CHINESE_ZODIAC_H__
#define __CHINESE_ZODIAC_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/chinesezodiacdef.hpp"
#include "protocal/msgchinesezodiac.h"
#include <set>

class ChineseZodiac
{
public:
	ChineseZodiac();
	~ChineseZodiac();

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

	void SetRole(Role *role) { m_role = role; }

	void InitParam(Role *role, const ChineseZodiacParam &param);
	void GetInitParam(ChineseZodiacParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcAttrBigSmallGoalAddPer(CharIntAttrs &attr_add);							////大小目标万分比加成
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool PromoteZodiac(int zodiac_type, short is_auto_buy);
	void SendChineseZodiacAllInfo(int upgrade_zodiac = -1);
	void SendChineseZodiacEquipInfo(int zodiac_type, int equip_type);

	int PutOnEquip(ItemDataWrapper &item_wrapper,int zodiac_type);
	int TakeOffEquip(int zodiac_type, int equip_type,bool is_put_on);
	void Recycle(Protocol::CSChineseZodiacRecycle * rcy_msg);
	bool IsChineseZodiacActivity(int zodiac_type);
	int GetZoiacTotalLevel();

	void ReqOpera(int info_type, int param1, int param2, int param3, int param4, int param5);																	// 操作请求
	void GmOpera(int type, int p1, int p2, int p3, int p4);
	bool TrantsItem(int type);
	void UnloadBead(int x, int y, int chapter);

	void GunGunLeChou(int type);

	void ZoiacUnlock();
	void XinhunUnlock();

	void PutMiji(int zodiac_type, int miji_index);
	void PutMijiRet();
	void MijiCompound(int index1);

	int GetHuiXuePercent();
	int GetSkillCoolDownShuoduan();
	int GetSkillXishuUp();

	void UpgradeXingling(int xingling_id, int is_auto_buy);

	void OnRoleLogin();

	int GetTotalLevel();						  // 获取总等级
	int GetUpLevelCount(int level);               // 获取达到等级的生肖个数
	int GetActiveCount(int color);				  // 获取激活个数

	void PromoteXingHun(int zodiac_type, bool is_auto_buy, bool is_use_promote_item);   //提升生肖星魂等级
	void GetXingHunTotalAddPer(int *mount_per, int *wing_per, int *halo_per, int *shengong_per, int *shenyi_per, int *fightmount_per, int *footprint_per);

	void ReCalcSuitCount();

	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);
	void CheckFuncOpen();
	
private:

	bool Activebead(int x, int y, short type, int chapter);
	void ReqActivebead(short type, int chapter);
	void ChangeGrid(int src_x, int src_y, int target_x, int target_y, int chapter);

	void CheckCombineAll(int chapter, bool is_init = false);
	void UpdateChapter(int chapter);
	bool CanActiveCombine(int seq, short src_type, const BeadPosi &src_posi, int chapter);

	static bool IsValidPosi(const int x, const int y);																						// 检查是否非法下标

	void SendAllInfo() const;
	void SendSingleInfo(int x, int y, int chapter) const;
	void SendCombineInfo(int chapter) const;
	
	bool TransformAttrType(int type, int &real_type);
	void ActiveSpecialEffect(int effect_type, int value);
	void SendSingleMiji(int zodiac_type, int kong_index, int miji_index);
	void SendXinglingInfo();
	int GetSpecialEffectValuPercentByType(int effect_type);

	Role *m_role;
	ChineseZodiacParam m_param;

	CharIntAttrs m_attrs_add;						// 各属性加成数值

	std::set<BeadPosi> m_tmp_posi_set;
	
	int m_curr_seq;
	char m_active_list[TIAN_XIANG_CURR_CHAPTER_NUM][TIAN_XIANG_ALL_CHAPTER_COMBINE];

	std::map<short, int>  m_special_buff_list;
	int m_other_capbility;
	int m_miji_put_notice;							//放置秘籍后的提示语，要等动画播完才显示

	int m_equip_quality_count[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT][ItemBase::I_COLOR_MAX];
};

#endif

