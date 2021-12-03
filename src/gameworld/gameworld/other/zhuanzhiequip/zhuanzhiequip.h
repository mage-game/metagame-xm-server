#ifndef __ZHUANZHI_EQUIP_H__
#define __ZHUANZHI_EQUIP_H__

#include "item/itemgriddata.h"
#include "equipment/equipment.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "gamedef.h"
#include "equipment/suitbase.hpp"
#include "protocal/msgequipment.h"
#include "servercommon/zhuanzhiequipdef.hpp"
#include "zhuanzhiequipconfig.hpp"
#include "protocal/msgzhuanzhiequip.hpp"

#include <set>

class Role;

class ZhuanzhiEquip
{
public:
	ZhuanzhiEquip();
	~ZhuanzhiEquip();

	void SetRole(Role *role);

	void Init(Role *role, const ZhuanzhiEquipParam &param);
	void GetInitParam(ZhuanzhiEquipParam *param);
	void OnInitFinish();

	void Update(time_t now_second);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcAttrConditionAttr(CharIntAttrs &base_add);

	int GetEquipIndexByType(int equip_type);												// ��ȡװ������

	void OnOperaReq(int opera_type, int param1, int param2, int param3, int param4, int param5);// ��������

	int PutOn(const ItemDataWrapper &item_wrapper);											// ����
	int TakeOff(short equip_index, bool is_from_puton = false);								// ����

	bool CheckKnapsack();																	// ��鱳���Ƿ���
	bool IsCanPuton(int equip_part, int equip_order, ItemID item_id);

	void ReCalZhuanZhiEquipTotalLevel();
	void RefreshEquipInfo();																// ˢ��װ����Ϣ

	void GetEquipSpecialAttr(CharIntAttrs &tmp_attr, const CharIntAttrs &base_equip_attr);	// ��ȡװ����������
	const Equipment *GetZhuanzhiEquip(ItemID item_id);										// ��ȡװ��

	void OnRoleLevelChange();

	const ItemDataWrapper *GetEquipmentGridData(short equip_index);

	//���Ǹ������---------------------------------------------------------------------------------------------
	bool EquipUpStar(short equip_index);													// ��������
	bool OnFuling(int equip_index);															// ����

	int GetEquipTotalStarLevel();															// ��ȡ�����ǵȼ�
	int GetEquipStarLevel(int equip_index);													// ��ȡ���ǵȼ�
	void AddEquipStarLevel(int equip_index);												// �������ǵȼ�
	int GetEquipStarExp(int equip_index);													// ��ȡ���Ǿ���
	void SetEquipStarExp(int equip_index, int exp);											// �������Ǿ���

	void SyncZhuanzhiEquipInfo();

	//��ʯ���---------------------------------------------------------------------------------------------
	void OnInlayStone(int part_index, int slot_index, int bag_index);						// ��Ƕ��ʯ
	bool OnUnInlayStone(int part_index, int slot_index, bool is_from_inlay = false);		// ж����ʯ
	void OnStoneLevelUp(int part_index, int slot_index);									// ������ʯ
	bool UpRefineLevel(int part_index, int seq, int is_autobuy);							// ������ʯ
	void OnStoneResolve(int bag_index);														// ��ʯ�ֽ�
	void OnStoneConvert(int seq);															// ��ʯ�һ�
	bool OnComsumeStoneScore(int consume_score);											// ��ʯ��������

	void CheckAllPartChangeSlot(int open_type, int part_index = 0);							// ��鿪��/�رղ�
	void CheckPutBackStone(int part_index, int slot_index);									// ��鷵����ʯ

	int GetTotalStoneLevel();																// ��ȡ�ܵȼ�
	int GetZhuanzhiEquipOrder(int part_index);												// ��ȡװ������
	int GetStoneScore() { return m_param.stone_score; }										// ��ȡ��ʯ����

	bool IsCanInlayStone(int part_index, int slot_index, int stone_id);						// �ж��ܷ���Ƕ

	void SyncStoneInfo();

	//��װ���---------------------------------------------------------------------------------------------
	void OnForgeZhuanzhiSuit(int suit_index, int equip_index);								// ����תְ��װ

	void CheckPutBackStuff(int equip_index, int equip_order);								// ��鷵���������
	int ReCalcSameOrderCount(int suit_index, int group_type, int order = 0);				// ������ͬ��������

	void GetSuitTotalAttr(CharIntAttrs &tmp_attr);

	void SyncZhuanzhiSuitInfo();

	//����װ�����---------------------------------------------------------------------------------------------
	void OnZhiZunEquipCompose(ItemID compose_id, int best_attr_num, int equip_index, int bag_index1, int bag_index2);	// ����װ���ϳ�

	bool IsCanCompose(int equip_type, int index);											// ��鱳��װ���ܷ�ϳ�
	int GetZhizunEquipIndexList(int *index_list);											// ��ȡ����װ������

	int GetZhiZunEquipOtherCap();															// ��������װ������ս��

	//Ч������---------------------------------------------------------------------------------------------
	void CheckActiveEquipEffect();															// ���Ч������

	void OnHurtOther(Character *target, long long base_injure, long long &injure);			// ����

	void OnHurtOtherZhiZunEffect(Character *target, long long &injure);						// ����װ��Ч��
	void OnHurtOtherUpGongji(Character * target);											// ���ӹ�����
	void OnHurtOtherPVPUpShanghai(Character *target, long long base_injure);				// ���Ӷ�����˺�
	void OnHurtOtherPVEUpShanghai(Character *target, long long base_injure);				// ���ӶԹ����˺�

	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);	// ������

	void OnBeendAttackDownShanghai(ObjID attacker, Attribute &delta_hp);					// ����
	void OnBeendAttackPVPDownShanghai(ObjID attacker, Attribute &delta_hp);					// ������Ҷ��Լ��˺�
	void OnBeendAttackPVEDownShanghai(ObjID attacker, Attribute &delta_hp);					// ���ٹ�����Լ��˺�

	//��Ʒ�������-------------------------------------------------------------------------------------------
	Attribute EquipXianPinFixValue(int attr_type);
	void ReCalcXianpinTotalStar(int &total_star);

	//----------------------------------------------------------------------------------------------------------
	int GetSatisfyPutonCount(int order, int color);											// ��ȡ���������Ĵ�������
	int GetSuitTypeCount(int suit_index);													// ��ȡ��װ��������																		
	bool HasSuit(int part_index);															// �ò�λ�Ƿ��Ѵ���																		

	void PrintLog(const char *reason, int part_index, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);

	int GetEquipNoticeColor(int equip_index);

	//�������-------------------------------------------------------------------------------------------
	void ReCalcAttrAwakening(CharIntAttrs &base_add);										// ����������Լӳ�
	void Wake(int part_index, int is_auto_buy);												// ����
	void CoverAwakening(int part_index);													// ���Ǿ���(�滻)
	void SyncAwakeningAllInfo();															// ���;���������Ϣ
	void SyncAwakeningInfo(int part_index);													// ���;��ѵ�����Ϣ
	bool LockAwakening(int part_index, int lock_index);										// ����
	bool UnLockAwakening(int part_index, int lock_index);									// ����
	bool IsLockAwakening(int part_index, int lock_index);									// �Ƿ�����
	unsigned int GetLockAwakeningNum(int part_index);										// ��ȡ��������
	wake_level_t GetAwakeningLevel(int part_index, int lock_index);							// ��ȡ���ѵȼ�
	wake_level_t GetAwakeningEquipLevel(int part_index, int lock_index);					// ��ȡ����װ���ȼ�
	wake_type_t GetAwakeningType(int part_index, int lock_index);							// ��ȡ��������
	wake_type_t GetAwakeningEquipType(int part_index, int lock_index);						// ��ȡ����װ������
	bool IsAwakeningMaxLevel(int part_index, int lock_index);								// �Ƿ��������
	void WakeRumor(int part_index);															// ���Ѵ���
	//---------------------------------------------------------------------------------------------------

	void ComposeZhuanzhiEquip(const Protocol::CSZhuanzhiEquipCompose *req);
	void SyncComposeSucceed(bool is_succeed);
	bool IsCanComposeZhuanzhi(const Equipment *compose_equip, int index);					// ��鱳��װ���ܷ�ϳ�
	bool IsZhuanzhiEquipType(int equip_type);												// �Ƿ�תְװ������
	int GetXianpinCountByBag(short bag_index);												// ��ȡ������װ������Ʒ���������ݱ�����������
	int GetXianpinCount(int part_index);													// ��ȡ���ϵ���Ʒ����

	int GetForgeZhuanzhiNum() const;														//��ȡ����װ���ļ���
private:
	int GetEquipColor(int equip_index);
	void ReCalcXianpinAttr(CharIntAttrs &base_add, int puton_level);
	bool AttachAttr(const ItemGridData &item_data, CharIntAttrs &base_add, int puton_level, int &flush_purple_maxhp_count, int &flush_purple_gongji_count);               

	//�������-------------------------------------------------------------------------------------------
	bool WakeConsume(int part_index, int is_auto_buy);										// ��������
	bool WakeBase(int part_index, int lock_index);											// ���ѵ��߼�
	bool ChangeAwakeningLevel(int part_index, int lock_index);								// �ı���ѵȼ����߼�����
	bool ChangeAwakeningType(int part_index, int lock_index);								// �ı�������͵��߼�����
	bool AddAwakeningLevelBase(int part_index, int lock_index, wake_level_t level);			// ʵ�����ӵȼ�
	bool DecAwakeningLevelBase(int part_index, int lock_index, wake_level_t level);			// ʵ�ʼ��ٵȼ�
	bool SetAwakeningTypeBase(int part_index, int lock_index, wake_type_t type);			// ���þ�������
	//---------------------------------------------------------------------------------------------------


	Role			*m_role = nullptr;
	ZhuanzhiEquipParam m_param;
	CharIntAttrs	m_attrs_add;								// �����Լӳ���ֵ

	int m_total_up_star_level = 0;								// ȫ�������ܵȼ�
	int m_max_up_star_level = 0;								// ������ǵȼ�
	int m_total_stone_level = 0;								// ȫ����ʯ�ܵȼ�

	int m_special_attr_pvp_jianshang = 0;						// װ����������pvp���� 
	int m_special_attr_pvp_zengshang = 0;						// װ����������pvp����
	int m_special_attr_pve_jianshang = 0;						// װ����������pve����
	int m_special_attr_pve_zengshang = 0;						// װ����������pve����

	int m_other_capability = 0;									// ��������ս��

	Attribute	m_xianpin_value_list[Equipment::E_XIANPIN_MAX];			// ����Ʒ��ֵ�б�

	std::set<int> m_active_effect_type_set;						// �Ѽ���Ч���б�
};

//-------------------------------------------------------------------------------------------
inline int ZhuanzhiEquip::GetEquipStarLevel(int equip_index)
{
	if (equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX) return 0;

	return m_param.zhuanzhi_equip_list[equip_index].star_level;
}

inline void ZhuanzhiEquip::AddEquipStarLevel(int equip_index)
{
	if (equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX) return;

	m_param.zhuanzhi_equip_list[equip_index].star_level += 1;
}

inline int ZhuanzhiEquip::GetEquipStarExp(int equip_index)
{
	if (equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX) return 0;

	return m_param.zhuanzhi_equip_list[equip_index].star_exp;
}

inline void ZhuanzhiEquip::SetEquipStarExp(int equip_index, int exp)
{
	if (equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || exp < 0) return;

	m_param.zhuanzhi_equip_list[equip_index].star_exp = exp;
}
//-------------------------------------------------------------------------------------------

#endif

