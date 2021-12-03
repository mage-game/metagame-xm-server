#ifndef __RUNE_SYSTEM_HPP__
#define __RUNE_SYSTEM_HPP__

#include <map>
#include <set>

#include "protocal/msgrunesystem.h"
#include "obj/character/attribute.hpp"
#include "other/runesystem/runesystemconfig.hpp"

class Role;

enum RUNE_SYSTEM_AWAKEN_TYPE
{
	RUEN_AWAKEN_TYPE_COMMON = 0,
	RUEN_AWAKEN_TYPE_DIAMOND,
};

enum RUNE_SYSTEM_ERROR_TYPE
{
	RUNE_SYSTEM_ERROR_TYPE_GRID_INVALID = 0,					//������Ч
	RUNE_SYSTEM_ERROR_TYPE_TOWER_LAYER_NOT_ENOUGH = 1,			//������δ�ﵽָ������
	RUNE_SYSTEM_ERROR_TYPE_MAX_LEVEL = 2,						//�����Ѵﵽ���ȼ�
	RUNE_SYSTEM_ERROR_TYPE_NOT_ENOUGH_SCORE = 3,				//ս�꾫������
};

class RuneSystem
{

public:
	RuneSystem();
	~RuneSystem();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const RuneSystemParam &param);
	void GetInitParam(RuneSystemParam *param) const;

	void OnRoleLogin(unsigned int last_save_time);
	void Update(unsigned int now_second);
	void OnDayChange(int old_dayid, int now_dayid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalRuneEffct();
	void ReCalcSpecialRoleBigGoalAttr();        // ���������Ŀ�����Լӳ�   ֻ��װ���ķ��ģ�������ĵȼ�

	void OperaationReq(const Protocol::CSRuneSystemReq *req);							// ��������

	//////////////////////////////////////////////////////////////////////////////////////
	
	void ReqSetRune(int virtual_index, int grid_index);		// װ������
	void ReqUpRuneLevel(int grid_index);					// ��������
	void ReqConvert(ItemID rune_id);						// ���Ķһ�
	void ReqComposeRune(int index1, bool index1_is_bag, int index2, bool index2_is_bag);// ���ĺϳ�
	void ReqAwaken(int grid_index, int awaken_type);		// ���ľ���
	void ReqRecaleAwaken();									// ������ľ�������

	int GetRuneLevelByJingHua(int quality, int type, int jinghua);						// ����������ĵȼ�

	void ReqXunBaoOneTimes(bool is_auto_buy);				// Ѱ��һ��
	void ReqXunBaoTenTimes(bool is_auto_buy);				// Ѱ��ʮ��

	int GetRuneWeightType(int total_times);																	// ��ȡѰ��Ȩ������
	int GetRuneRealWeight(const RuneSystemRuneFetchCfg &rune_weight_cfg, int total_times, bool is_ten);		// ��ȡȨ��ֵ

	void DisposeOne(int virtual_index);						// �ֽ�һ��
	void DisposeOneKey(Protocol::CSRuneSystemDisposeOneKey *req);	// һ���ֽ�

	bool IsCanUpLevelRune(RuneParam &rune);					// �Ƿ������������
	void CheckOpenRuneSlotAndId();							// �����Ĳۼ�����id����
	bool IsOpenAwaken();									// ����Ƿ�������
	bool IsOpenLianhun();									// ����Ƿ�������

	double GetSpecialEffectValuPercentByType(int effect_type);

	Attribute RecalcFangJuAddMaxHp(Attribute fangju_maxhp);
	Attribute RecalcFangJuAddFangyu(Attribute fangju_fangyu);
	Attribute RecalcFangJuAddShanbi(Attribute wuqi_pojia);
	Attribute RecalcFangJuAddJianren(Attribute wuqi_pojia);
	Attribute RecalcWuQiAddGongJi(Attribute wuqi_gongji);
	Attribute RecalcEquipAddGongJi(Attribute equip_base_gongji);
	Attribute RecalcWuQiAddBaoji(Attribute wuqi_baoji);
	Attribute RecalcShouShiAddGongji(Attribute shoushi_gongji);
	Attribute RecalcShouShiAddBaoji(Attribute shoushi_baoji);
	
	void SendAllInfo();

	void GmOpera(int type, int p1, int p2, int p3, int p4);

	bool AddRuneByItemId(ItemID item_id, int num);			// ���ݷ���id�ӷ���
	bool UseRuneBox(int box_type, int level_limit);			// ʹ�÷��ı���
	int GetBagSpaceNum() { return m_empty_grids; }
	int GetRuneQualityByItemId(ItemID item_id);

	void AddRuneJingHua(int jinghua);						// �ӷ��ľ���

	void PutRuneTowerRewardByItemId(ItemID item_id);		// ������ͨ�ؽ�������

	void OnRuneTowerFetchTime();
	void OnRuneTowerAutoFb();
	int GetRuneTowerPassLayer() { return m_param.fb_pass_layer; }
	void OnRuneTowerPassLayer(int pass_layer);
	int GetRuneTowerTodayLayer() { return m_param.fb_today_layer; }
	short AddRuneTowerOfflineTime(int offline_time, short max_count);
	void OnRuneTowerOfflineTime(unsigned int last_save_time);
	void SendRuneTowerInfo();

	void GMSetRuneTower(int pass_layer, int today_layer);
	void SendComposeInfo();

	void RandZhulingSlot(int rune_index);
	void Zhuling(int rune_index);
	void SendZhulingResultInfo(int index);
	void SendZhulingAllInfo();
	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);
	void SendVirtualBagAllInfo();
private:

	void PutRuneListToVirtualBag(const RuneParam rune[], int size, int magic_crystal, int info_type, const char *reason);	// �����Ʒ�����ⱳ��
	void PutRuneToVirtualBag(const RuneParam &rune, int magic_crystal, int info_type, const char *reason);					// �����Ʒ�����ⱳ��

	static bool IsInvalidVirtualIndex(int virtual_index) { return virtual_index < 0 || virtual_index >= RUNE_SYSTEM_BAG_MAX_GRIDS; }// ������ⱳ�������Ƿ�Ƿ�
	static bool IsInvalidGridIndex(int grid_index);// ���������������Ƿ�Ƿ�

	bool CheckHasSpace(int count = 1) const { return m_empty_grids >= count; }				// ���ⱳ���Ƿ����㹻�ո���
	void SortVirtrulBag();																	// ����

	static bool TransformAttrType(int type, int &real_type);								// ��ȡ������������
	bool CheckIsActiveSpecialEffect(int effect_type);										// ����Ƿ񼤻�ĳ������Ч��
	void ActiveSpecialEffectByRuneType(int type, double value, int add_per, int fuwen_index);
	void ActiveSpecialEffect(int effect_type, double value, int add_per, int fuwen_index);
	short GetSpecialEffectByRuneType(int type);												// ��ȡЧ������
	void AddSpecialEffect(int effect_type, double value);									// ��������Ч����ֵ							
	void NotActiveSpecialEffect(int effect_type);

	void SendRuneGridAllInfo();
	void SendOtherInfo();
	void SendAwakenInfo(short seq, short is_need_recalc);
	void CheckFuncOpen();
	void CheckGoalProgress(int quality);           // ��СĿ�����

	Role *m_role;
	RuneSystemParam m_param;
	CharIntAttrs m_attrs_add;
	int m_empty_grids;														// �����ո�������
	std::map<short, double>  m_special_buff_list;

	std::set<ItemID> m_rune_id_set;											// ��¼�ѿ����ķ���id

	std::vector<RuneSystemRuneFetchCfg> m_rune_fetch_weight_vec;			// ���Ļ�ȡȨ������

	int m_other_capability;
};

#endif