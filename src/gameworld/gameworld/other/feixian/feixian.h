#ifndef __FEIXIANMANAGER_H__
#define __FEIXIANMANAGER_H__

#include "item/itemgriddata.h"
#include "equipment/equipment.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "gamedef.h"
#include "equipment/suitbase.hpp"
#include "protocal/msgequipment.h"
#include "servercommon/feixianparam.hpp"
#include "reallyequipconfig.hpp"

class Role;

class FeixianManager
{
public:
	enum APPEREFRESHREASON
	{
		APRR_INIT = 1,
		APRR_EXPIRED,
		APRR_PUTON,
		APRR_TAKEOFF,
		APRR_CHANGE_ID,
		APRR_STRENGTH,
		APRR_NAIJIU,
		APRR_GEMSTONE,
		APPR_UPLEVEL,
		APPR_QUALITY,
		APPR_SHENZHU,
		APPR_FULING,
	};
	const static unsigned short STAR_COUNT_LIMIT = 3;

	FeixianManager();
	~FeixianManager();

	void SetRole(Role *role);

	void Init(Role *role, const FeiXianParam &param);
	void GetInitParam(FeiXianParam *param);

	void Update(time_t now_second);

	int GetEquipIndexByType(int equip_type);

	int PutOn(const ItemDataWrapper &item_wrapper);
	int TakeOff(short equip_index, bool is_from_puton = false);
	int ComposeEquipInKnapsack(short index1, short index2);                     // �ϳ�,װ������index1,index2���ڱ���
	int ComposeEquipInBody(short index1, short index2);                         // �ϳ�,װ������index1������
	int LevelUpInKnapsack(short index1, int& itemindex);                        // �ڱ�������װ��
	int LevelUpInBody(short index1);                                            // ����������װ��
	bool CheckKnapsack();                                                       // ��鱳���Ƿ���
	int CheckEquipInKnapsack(short index, ItemGridData* item_datas, bool levelup = true);             // ���װ���ڱ��������
	int CheckEquipInBody(short index,bool levelequip = true);                                         // ���װ�������ϵ����
	int LevelUpConsumeItem(ItemID id, const LevelupListConfig* pLevelUpListCfg);                      // ����������Ʒ

	void SyncEquipFeixianInfo();

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnRoleLevelChange();

	bool static GetRandomSpeicalAttr(ItemGridData *item_grid_data);
	bool static GetFixSpecialAttr(ItemGridData *item_grid_data, int getway, int getway_param1 = 0);

	void EquipXianPinFixValue(int attr_type, int &fix_value);

	void ReCalcXianpinTotalStar(int &total_star);

	bool FindFeixianEquipByIndex(short index);
private:
	void ReCalcXianpinAttr(CharIntAttrs &base_add, int puton_level);
	bool AttachAttr(const ItemGridData &item_data, CharIntAttrs &base_add, int puton_level, int &flush_purple_maxhp_count, int &flush_purple_gongji_count);
	void ReCalcXianpinParam(EquipmentParam* param1, EquipmentParam* param2);                // �ϳɳɹ�����ã�������װ��1��װ��2
	void ReCalcXianpinParamInBody(ItemDataWrapper& param1, EquipmentParam* param2);         // �ϳɳɹ�����ã�����װ��1�ͱ���װ��2
	static bool Compare(const EquipXianPinConfig* a, const EquipXianPinConfig* b);          // set����ȽϹ���
	const static int XIANPIN_SHUXING_NUM_MAX = 3;                                           // ���������Ŀ����,���ڳ�װ                

	Role			*m_role;

	FeiXianParam m_feixianparam;

	CharIntAttrs	m_attrs_add;							// �����Լӳ���ֵ

	Attribute		m_xianpin_value_list[Equipment::E_XIANPIN_MAX];	// ����Ʒ��ֵ�б�
};

#endif

