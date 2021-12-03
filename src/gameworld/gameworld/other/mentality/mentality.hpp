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

	void SendMentalityInfo();																			// ����Ԫ����Ϣ														// ����Ԫ�������Ϣ

	void Update(unsigned int now_second);																// ����Ԫ��״̬

	void ClearTrainMentalityCD();
	void TrainMentality();																				// ѵ��Ԫ��
	void ClearMentalityCD();																			// ����Ԫ������
	void UpgradeMentalityGengu(int type, bool is_auto_buy, bool use_protect_item);						// ��������
	void YijianTishengMentality();																		// һ������Ԫ��

	void OnMentalityUplevelWuxing(int wx_index, bool is_auto_buy);										// ��������

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);											// ����Ԫ��ӳ�
	
	int GetTotalGenguMaxLevel();																		// ���ǣ����磩�ܵȼ�

	int GetMinGenguLevel();																				// ��ȡ��С���ǵȼ�
	int GetMinMentalityBaseLevel();																		// ��ȡ��СԪ��ȼ�

	int GMUpgradeGenguTest(int type, int target_level, int *coin_cost, int *gold_cost);
	void GMResetGengu(int type, int level);
	void GMSetMentalityLevel(int level);

	bool OnUseShuxingdan(int slot_idx, int use_count);

private:

	Role *m_role;																						

	CharIntAttrs m_attr_temp;																			// �ӳ�������ʱ����

	MentalityItem m_mentality_list[MENTALITY_TYPE_MAX];													// Ԫ��͸��ǵȼ�
	
	unsigned int m_train_cd_next_time;																	// Ԫ��������ʱ�� 0��ʾ���
	int m_curr_trained_mentality_type;																	// ��ǰ����Ԫ������
	short m_flag;

	short m_wuxing_level_list[MENTALITY_WUXING_MAX_COUNT];												// ���еȼ�

	int mentality_total_level;																			// �����ܵȼ�

	int m_shuxingdan_list[MENTALITY_SHUXINGDAN_MAX_TYPE];												// ���Ե��б�
};


#endif // __MENTALITY_HPP__

