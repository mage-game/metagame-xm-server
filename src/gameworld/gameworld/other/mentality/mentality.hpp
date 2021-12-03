#ifndef __MENTALITY_HPP__
#define __MENTALITY_HPP__

#include "config/mentalityconfig.hpp"

#include "protocal/msgmentality.hpp"

class Character;
class Role;

class Mentality
{
public:
	Mentality();
	~Mentality();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const MentalityParam &param);
	void GetOtherInitParam(MentalityParam *param);
	void GetMentalityParam(MentalityParam *param) { this->GetOtherInitParam(param); }

	void GetMentalityInfo(MentalityItem mentality_list[MENTALITY_TYPE_MAX]);

	void SendMentalityInfo();																			// 发送元神信息														// 发送元神合体信息

	void Update(unsigned int now_second);																// 更新元神状态

	void ClearTrainMentalityCD();
	void TrainMentality();																				// 训练元神
	void ClearMentalityCD();																			// 加速元神修炼
	void UpgradeMentalityGengu(int type, bool is_auto_buy, bool use_protect_item);						// 提升根骨
	void YijianTishengMentality();																		// 一键提升元神

	void OnMentalityUplevelWuxing(int wx_index, bool is_auto_buy);										// 提升五行

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);											// 重算元神加成
	
	int GetTotalGenguMaxLevel();																		// 根骨（境界）总等级

	int GetMinGenguLevel();																				// 获取最小根骨等级
	int GetMinMentalityBaseLevel();																		// 获取最小元神等级

	int GMUpgradeGenguTest(int type, int target_level, int *coin_cost, int *gold_cost);
	void GMResetGengu(int type, int level);
	void GMSetMentalityLevel(int level);

	bool OnUseShuxingdan(int slot_idx, int use_count);

private:

	Role *m_role;																						

	CharIntAttrs m_attr_temp;																			// 加成属性临时变量

	MentalityItem m_mentality_list[MENTALITY_TYPE_MAX];													// 元神和根骨等级
	
	unsigned int m_train_cd_next_time;																	// 元神锻炼完成时间 0表示完成
	int m_curr_trained_mentality_type;																	// 当前锻炼元神类型
	short m_flag;

	short m_wuxing_level_list[MENTALITY_WUXING_MAX_COUNT];												// 五行等级

	int mentality_total_level;																			// 经脉总等级

	int m_shuxingdan_list[MENTALITY_SHUXINGDAN_MAX_TYPE];												// 属性丹列表
};


#endif // __MENTALITY_HPP__

