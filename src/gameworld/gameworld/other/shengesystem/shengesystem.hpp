#ifndef __SHENGE_SYSTEM_HPP__
#define __SHENGE_SYSTEM_HPP__

#include "obj/character/attribute.hpp"
#include "protocal/msgshengesystem.h"
#include "other/shengesystem/shengesystemconfig.hpp"

#include <map>
#include "obj/character/character.h"

class Role;

static const int SHENGE_SYSTEM_CHOUJIANG_ONE_TIMES = 1;												// һ��
static const int SHENGE_SYSTEM_CHOUJIANG_TEN_TIMES = 10;											// ʮ��
static const int SHENGE_SYSTEM_ADD_ATTR_NUM = 2;													// ���Լӳ�����

class ShengeSystem
{
	static const int SPECIAL_EFFECT_ADD_PERCENT = 10000;											// ����Ч���ӳ���ֱ�

public:
	ShengeSystem();
	~ShengeSystem();

	void SetRole(Role* role) { m_role = role; }
	void InitParam(Role *role, const ShengeSystemParam &param);
	void GetInitParam(ShengeSystemParam *param) const;

	void OnDayChange(int old_dayid, int now_dayid);
	void Update(unsigned long interval, unsigned int now_second);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalShengeEffct(bool is_init = false, bool is_broad_cast_combine = false);

	void OperaationReq(const Protocol::CSShengeSystemReq *req);	// ��������

	UInt16 GetDropId();
	double GetSpecialEffectValuPercentByType(int effect_type);
	void ReqUpShengeLevelInVirtualBag(int virtual_index);
	void ReqUpShengeLevelInShengePage(int page_index, int grid_index, bool is_auto);
	bool AddShengeByItemId(ItemID item_id);
	bool AddShengeByItemIdByColor(int color, ItemID item_id, int num);

	void SendAllInfo();

	void GmOpera(int type, int p1, int p2, int p3);
	int GetMarrowScore() const { return m_param.marrow_score; }

	void RecalcShanbi(Character *target, int &shanbi);
	int GetDecHurtPercent();
	int GetAddExpPercent();
	int GetAddItemDropPercent();
	void OnHurtOther(Character *target, long long base_injure, long long &injure);
	bool AddMarrowScore(int score, const char *reason);																					// ���ӻ���

	void OnStartFight();
	void OnStopFight();
	bool CheckHasSpace(int count = 1) const { return m_empty_grids >= count; }															// ���ⱳ���Ƿ����㹻�ո���

	void ReqUpLevelZhangkong(int type);
	void UpZhangkongLevelRet();
	int GetZhangkongRandGrid();
	void SendZhangkongInfo();
	void SendZhangkongSignInfo(int grid, int level, int exp, int add_exp);

	void Strengthen(int shenqu_id, int point_type, bool is_auto, unsigned int select_slot);
	void ReqUpLevelGrid(int index, short is_auto = 0);

	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);

	int GetSatisfyMagicalPreciousCount(int order);  // ��ȡ��ӦƷ�ʸ���

	void ReCalcSuitCount();
	void SendSuitInfo();

	void OnRoleLogin();

private:
	void CheckShengesystemRecover();
	void ReqDecomposeShenge(const Protocol::CSShengeSystemReq *req);
	void ReqComposeShenge(int virtual_index_1, int virtual_index_2, int virtual_index_3);												// ����ϳ�
	void ReqSetShenge(int virtual_index, int page_index, int grid_index);																// װ�����
	bool ReqUpShengeLevel(ShengeParamData &shenge);																						// �������
	void ReqChangeShengePage(int page_index);																							// �л����ҳ
	void ReqUnloadShenge(int page_index, int grid_index);																				// ��ж���
	void ReqClearShengePage(int page_index);																							// ������ҳ

	void PutShengeListToVirtualBag(const ShengeParamData shenge[], int size, const char *reason);										// �����Ʒ�����ⱳ��
	void PutShengeToVirtualBag(const ShengeParamData &shenge, const char *reason);														// �����Ʒ�����ⱳ��
	void ReqChoujiang(int times);																										// �齱
	void GiveFisrTenCHou(ShengeParamData reward_list[], int size) const;																// �����״�ʮ��

	static bool IsInvalidVirtualIndex(int virtual_index) { return virtual_index < 0 || virtual_index >= SHENGE_SYSTEM_BAG_MAX_GRIDS; }	// ������ⱳ�������Ƿ�Ƿ�
	static bool IsInvalidPageIndex(int page_index) { return page_index < 0 || page_index >= SHENGE_SYSTEM_CUR_SHENGE_PAGE; }			// ����������ҳ�����Ƿ�Ƿ�
	static bool IsInvalidGridIndex(int grid_index) { return grid_index < 0 || grid_index >= SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID; }		// ���������������Ƿ�Ƿ�

	void SortVirtrulBag();																												// ����
	bool CheckCanActiveCombinationBySeq(int seq);																						// ����ܷ񼤻�����
	static bool TransformAttrType(int type, int &real_type);																			// ��ȡ������������
	bool CheckIsMaxTyptCount(int type, int page_index);
	bool CheckIsActiveSpecialEffect(int effect_type);																					// ����Ƿ񼤻�ĳ������Ч��
	void ActiveSpecialEffectByShengeType(int type, double value);
	void ActiveSpecialEffect(int effect_type, double value);
	void NotActiveSpecialEffect(int effect_type);

	void SendVirtualBagSingleInfo(int virtual_index);
	void SendShengePageGridSingleInfo(int page_index, int grid_index);
	void SendVirtualBagAllInfo();
	void SendShengePageInfo(int page_index);
	void SendShengePageGridAllInfo();
	void SendMarrowScoreInfo() const;
	void SendUsingIndex() const;
	void SendComposeInfo(ShengeParamData &shenge, int is_succ) const;
	void SendActiveCombinationInfo() const;
	void SendChoujiangInfo() const;
	void SendSingleXilianInfo(int shenqu_id);
	void SendXilianAllInfo();

	void CheckChangeTotalLevel();

	int GetRealType(int vutual_type);
	int GetXilianStuffNum(int shenqu_id, int select_slot_count);
	int GetShenquPerfectAttrNum(int shenqu_id);
	void CheckPerfectAttr(int shenqu_id, int point_type);
	void CheckFuncOpen();

	Role *m_role;
	ShengeSystemParam m_param;
	CharIntAttrs m_attrs_add;
	int m_empty_grids;																		// �����ո���������
	std::map<short, double>  m_special_buff_list;

	unsigned int m_drop_cd_tiemstamp;
	int m_combine_seq_list[SHENGE_SYSTEM_COMBINATION_MAX_CFG_COUNTS];

	bool m_can_recover;
	unsigned long m_time_interval;
	int m_other_capbility;
	int m_up_zhangkong_level_notice;

	int m_quality_equip_count[SHENGE_QUALITY_COUNT];
};


#endif