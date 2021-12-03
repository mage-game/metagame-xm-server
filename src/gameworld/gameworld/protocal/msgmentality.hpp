#ifndef __MSG_MENTALITY_HPP__
#define __MSG_MENTALITY_HPP__

#include "servercommon/userprotocal/msgheader.h"

#include "servercommon/mentalitydef.hpp"

#pragma pack(push)
#pragma pack(4)

namespace Protocol
{
	class CSTrainMentality											// ѵ��Ԫ��
	{
	public:
		CSTrainMentality();
		MessageHeader header;
	};

	class CSMentalityClearTrainCD										// ���Ԫ��CD
	{
	public:
		CSMentalityClearTrainCD();
		MessageHeader header;
	};

	class CSUpgradeMentalityGengu									// ��������
	{
	public:
		CSUpgradeMentalityGengu();
		MessageHeader header;

		short type;													// Ԫ������
		short reserved;		
		int use_protect_item;										// ʹ�ñ�����
		int is_auto_buy;											// �����Ƿ��Զ����� 1����
	};

	class CSSpeedUpMentalityTrain									// ����Ԫ������
	{
	public:
		CSSpeedUpMentalityTrain();
		MessageHeader header;
	};

	class SCMentalityList											// ����Ԫ����Ϣ
	{
	public:
		SCMentalityList();
		MessageHeader header;

		unsigned int cd_next_time;									// Ԫ��ѵ�����ʱ���
		int curr_train_type;										// ��ǰѵ������
		short flag;													// ���
		short reserve_sh;											// ����

		MentalityItem mentality_list[MENTALITY_TYPE_MAX];			// Ԫ���б�

		UNSTD_STATIC_CHECK(35 == MENTALITY_WUXING_MAX_COUNT);

		short reserve_sh_list[5];	
		short wuxing_level_list[MENTALITY_WUXING_MAX_COUNT];		// ���еȼ�

		int shuxingdan_list[MENTALITY_SHUXINGDAN_MAX_TYPE];			// ���Ե�
	};

	class CSMentalityUplevelWuxing									// ��������
	{
	public:
		CSMentalityUplevelWuxing();
		MessageHeader header;

		short wuxing_index;
		short is_auto_buy;
	};

	class CSMentalityYijianTisheng									// һ������
	{
	public:
		CSMentalityYijianTisheng();
		MessageHeader header;
	};

	class SCMentalityYijianTishengAck								// һ���������֪ͨ
	{
	public:
		SCMentalityYijianTishengAck();
		MessageHeader header;

		int tisheng_count;											// Ԫ����������
	};
}

#pragma pack(pop)

#endif // __MSG_MENTALITY_HPP__

