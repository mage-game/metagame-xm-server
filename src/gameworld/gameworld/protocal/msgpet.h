#ifndef __MSG_PET_HPP__
#define __MSG_PET_HPP__

#include "servercommon/petdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum PET_REQ_TYPE
	{
		PET_REQ_TYPE_INFO = 0,									// ���������Ϣ����
		PET_REQ_TYPE_BACKPACK_INFO,								// ���ﱳ����Ϣ����
		PET_REQ_TYPE_SELECT_PET,								// �����ս����
		PET_REQ_TYPE_CHANGE_NAME,								// �����������	
		PET_REQ_TYPE_UP_LEVEL,									// ������������
		PET_REQ_TYPE_UP_GRADE,									// ������������(���ϣ��Ѿ�ʹ�ö���Э��)
		PET_REQ_TYPE_CHOU,										// �����ȡ����
		PET_REQ_TYPE_RECYCLE_EGG,								// ���ﵰ��������
		PET_REQ_TYPE_PUT_REWARD_TO_KNAPSACK,					// ������ȡ��������
		PET_REQ_TYPE_ACTIVE,									// ���Ｄ������
		PET_REQ_TYPE_LEARN_SKILL,								// ѧϰ����
		PET_REQ_TYPE_UPGRADE_SKILL,								// ��������
		PET_REQ_TYPE_FORGET_SKILL,								// ��������
		PET_REQ_TYPE_QINMI_PROMOTE,								// �������ܶ�
		PET_REQ_TYPE_QINMI_AUTO_PROMOTE,						// һ�������ܵȼ�
		PET_REQ_TYPE_FOOD_MARKET_CHOU_ONCE,						// �Ի��г����γ齱
		PET_REQ_TYPE_FOOD_MARKET_CHOU_TIMES,					// �Ի��г���γ齱
		PET_REQ_TYPE_UPLEVL_SPECIAL_IMG,						// �����û��ȼ�
	};

	class CSPetOperaReq											// �����������  6110
	{
	public:
		CSPetOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		GameName newname;
	};

	class CSPetUpgradeReq										// ������������  6111
	{
	public:
		CSPetUpgradeReq();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class SCPetBackpackInfo			 							// ���ﱳ����Ϣ 6106
	{
	public:
		SCPetBackpackInfo();
		MessageHeader header;

		unsigned int last_free_chou_timestamp;					// ��һ����ѳ�����ʱ���
		PetStoreItem backpack_item_list[PET_MAX_STORE_COUNT];
		int food_market_free_times;								// �Ի��г���Ѵ���
	};
	
	class CSPetUseSpecialImg									//ʹ�ó����������� 6112
	{
	public:
		CSPetUseSpecialImg();
		MessageHeader header;

		short special_img_id;
		short reserve_sh;
	};

	class SCPetInfo												// ���������Ϣ 6107
	{
	public:
		SCPetInfo();
		MessageHeader header;

		int cur_on_use_pet_id;
		PetParamItem own_pet_list[PET_MAX_COUNT_LIMIT];
		int special_img_active_flag;
		char special_img_level_list[MAX_SPECIAL_IMG_COUNT];
	};

	class SCFightOutPetInfo												// �����ս��Ϣ 6109
	{
	public:
		SCFightOutPetInfo();
		MessageHeader header;

		int cur_on_use_pet_id;											// ��ǰ��ս�ĳ���id
	};


	class SCPetViewChangeNotify									// �������Ըı�㲥 6105
	{
	public:
		SCPetViewChangeNotify();
		MessageHeader header;

		ObjID obj_id;											// ��Ҷ���ID
		short pet_id;											// ��ս����ID
		short pet_level;										// ��ս����ȼ�
		short pet_grade;										// ��ս�������
		GameName pet_name;										// ��ս������
		short use_img_id;                                       // ʹ�ûû�ID
		short reserve_sh;
	};


	static const int SC_CHOU_PET_MAX_TIMES = 10;
	class SCPetChouResult														// ����齱�����Ϣ 6108
	{
	public:
		SCPetChouResult();
		MessageHeader header;

		short reward_list_count;												// �齱��ȡ�Ľ�������
		char all_reward_index_list[SC_CHOU_PET_MAX_TIMES];						// �齱��ȡ�Ľ��������б�
	};

	class SCPetQinmiAllInfo										// 6117 ���г������ܶ���Ϣ
	{
	public:
		SCPetQinmiAllInfo();
		MessageHeader header;

		PetQinmiParamItem pet_qinmi_list[PET_MAX_COUNT_LIMIT];		// PET_MAX_COUNT_LIMIT = 12
		int ignore_fangyu_percent_level;
	};

	class SCPetQinmiSingleInfo									// 6118 �����������ܶ���Ϣ
	{
	public:
		SCPetQinmiSingleInfo();
		MessageHeader header;

		PetQinmiParamItem pet_qinmi_item;
		int cur_pet_id;
		int ignore_fangyu_percent_level;
	};

	static const int SC_CHOU_PET_FOOD_MARKET_MAX_TIMES = 50;
	class SCPetFoodMarcketChouResult							// 6119 �Ի��г��齱��Ϣ
	{
	public:
		SCPetFoodMarcketChouResult();
		MessageHeader header;

		short reward_list_count;
		char reward_seq_list[SC_CHOU_PET_FOOD_MARKET_MAX_TIMES];
	};
}

#pragma pack(pop)

#endif // __MSG_PET_HPP__


