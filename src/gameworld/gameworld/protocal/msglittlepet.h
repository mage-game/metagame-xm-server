#ifndef __LITTLEPET_MSG_HPP__
#define __LITTLEPET_MSG_HPP__

#include "servercommon/littlepetdef.hpp"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/frienddef.hpp"

#pragma  pack(push, 4)

namespace Protocol
{
	enum LITTLE_PET_REQ_TYPE
	{
		LITTLE_PET_REQ_INTENSIFY_SELF,						// ǿ���Լ����� param1 ����������param2 ǿ����������param3�������� 
		LITTLE_PET_REQ_INTENSIFY_LOVER,						// ǿ�����˳��� param1 ����������param2 ǿ����������param3�������� 
		LITTLE_PET_REQ_CHOUJIANG,							// �齱	param1  1:10
		LITTLE_PET_REQ_RECYCLE,								// ����	param1 ��Ʒid ��param2 ��Ʒ������ param3 �Ƿ�� 1��0 Ĭ�ϰ� 
		LITTLE_PET_REQ_RELIVE,								// ����	param1 ��������												
		LITTLE_PET_REQ_FEED,								// ι���Լ�����	param1 �������� , param2 �Լ������� 1��0 param3:�Ƿ��Զ�����
		LITTLE_PET_REQ_PET_FRIEND_INFO,						// ������Ϣ			
		LITTLE_PET_REQ_INTERACT,							// ���� param1 �������� param2 Ŀ��role uid param3 �Լ������� 1��0	
		LITTLE_PET_REQ_EXCHANGE,							// �һ� param1 �һ���Ʒ���� param2 ������			
		LITTLE_PET_REQ_CHANGE_PET,                          // ���� param1 �������� param2 ʹ�õ���Ʒid							
		LITTLE_PET_REQ_USING_PET,							// ʹ������ param1 ����id��
		LITTLE_PET_REQ_FRIEND_PET_LIST,						// ����С�����б�param1 ����uid
		LITTLE_PET_REQ_INTERACT_LOG,						// ������¼��														
		LITTLE_PET_PUTON,									// װ��С���� param1:�����±� param2:����index					
		LITTLE_PET_TAKEOFF,									// ж��С���� param1:�����±�
		LITTLE_PET_REQ_EQUIPMENT_PUTON,						// С���ﴩ��װ�� param1:�����±� param2:����index
		LITTLE_PET_REQ_EQUIPMENT_TAKEOFF,					// С��������װ�� param1:�����±� param2:װ��index
		LITTLE_PET_REQ_EQUIPMENT_UPLEVEL_SELF,				// �Լ�С����װ������ param1 �����±� param2 װ���±� param3 �Ƿ��Զ�����
		LITTLE_PET_REQ_EQUIPMENT_UPLEVEL_LOVER,				// ����С����װ������ param1 �����±� param2 װ���±� param3 �Ƿ��Զ�����
		LITTEL_PET_REQ_WALK,								// ����� param1 ����Ƿ�idle 0���� 1��
	};

	class CSLittlePetREQ			// �������� 8001
	{
	public:
		CSLittlePetREQ();
		MessageHeader header;

		int opera_type;
		int param1;
		int param2;
		int param3;
	};

	struct LittlePetGridValue
	{
		short arrt_type;														//ǿ�����ӵ���ֵ����
		short grid_index;														//��������
		int attr_value;															//��ֵ
		
	};

	struct LittlePetPointValue
	{
		LittlePetGridValue gridvaluelist[LITTLEPET_QIANGHUAGRID_MAX_NUM];		//�����б�
	};

	struct LittlePetEquipInfo
	{
		unsigned short equipment_id;
		short level;
	};

	struct LittlePetSingleInfo
	{
		int index;																//��������
		short id;																//����id
		short info_type;														//�Լ��ģ����µ� 1: 0
		GameName pet_name;														//С��������

		int maxhp;																//����
		int gongji;
		int fangyu;
		int mingzhong;
		int shanbi;
		int baoji;
		int kangbao;

		unsigned int baoshi_active_time;										//�ϴα�ʳ������ʱ���
		short feed_degree;														//��ʳ��
		short interact_times;													//��������
		short feed_level;														//ι���ȼ�
		short reserve_sh;

		LittlePetPointValue point_list[LITTLEPET_QIANGHUAPOINT_CURRENT_NUM];			//ǿ�����б�
		LittlePetEquipInfo equipment_llist[LITTLEPET_EQUIP_INDEX_MAX_NUM];				// װ���б�
	};

	class SCLittlePetAllInfo		//8050
	{
	public:
		SCLittlePetAllInfo();
		MessageHeader header;
		int score;																//����
		unsigned int last_free_chou_timestamp;									//��ѳ齱ʱ���
		short interact_times;													//��һ�������	

		short pet_count;														//�������
		LittlePetSingleInfo pet_list[LITTLE_PET_COUPLE_MAX_SHARE_NUM];
	};

	class SCLittlePetSingleInfo		//8051
	{
	public:
		SCLittlePetSingleInfo();
		MessageHeader header;

		LittlePetSingleInfo pet_single;
	};

	class  SCLittlePetChangeInfo	//8052
	{
	public:
		SCLittlePetChangeInfo();
		MessageHeader header;

		char pet_index;															//��������
		char is_self;															//�Լ������� 1:0
		char point_type;														//ǿ����
		char grid_index;														//��������

		LittlePetGridValue gridvalue;											//���ӵ���ֵ
	};

	struct LittlePetReWarValue
	{
		int item_id;															//��Ʒid
		short item_num;															//��Ʒ����
		short is_bind;															//�Ƿ��
	};

	class SCLittlePetChouRewardList		//8053
	{
	public:
		SCLittlePetChouRewardList();
		MessageHeader header;

		int list_count;															//������Ʒ�б�����
		int final_reward_seq;													//���һ���齱���seq
		LittlePetReWarValue reward_list[LITTLE_PET_MAX_CHOU_COUNT];				//������Ʒ�б�
	};

	enum LITTLE_PET_NOTIFY_INFO_TYPE
	{
		LITTLE_PET_NOTIFY_INFO_SCORE,											//param1 ������Ϣ							
		LITTLE_PET_NOTIFY_INFO_FREE_CHOU_TIMESTAMP,								//param1 ��ѳ齱ʱ���
		LITTLE_PET_NOTIFY_INFO_INTERACT_TIMES,									//param1 ��һ�������
		LITTLE_PET_NOTIFY_INFO_FEED_DEGREE,										//param1 �������� �� param2 ��ʳ�ȣ�param3 �Լ������� �� 1��0
		LITTLE_PET_NOTIFY_INFO_PET_INTERACT_TIMES,								//param1 ���ﻥ������
	};


	class SCLittlePetNotifyInfo			//8054
	{
	public:
		SCLittlePetNotifyInfo();
		MessageHeader header;

		int param_type;
		unsigned int param1;
		int param2;
		int param3;
		unsigned int param4;
	};

	struct LittlePetFriendInfo
	{
		int friend_uid;															//����id
		short prof;																//ͷ��
		short sex;																//���ѵ�sex
		GameName owner_name;													//���ѵ�����
		int pet_num;															//���������
	};

	class SCLittlePetFriendInfo			//8055
	{
	public:
		SCLittlePetFriendInfo();
		MessageHeader header;

		int count;
		LittlePetFriendInfo pet_friend_list[MAX_FRIEND_NUM];
	};

	class SCLittlePetUsingImg			//8056
	{
	public:
		SCLittlePetUsingImg();
		MessageHeader header;

		int role_id;
		int using_pet_id;
		GameName pet_name;
	};

	struct LittlePetFriendPet
	{
		int index;
		int pet_id;
		int info_type;
		GameName pet_name;
		short interact_times;													//��������
		short reserve;				
	};

	class SCLittlePetFriendPetListInfo	//8057
	{
	public:
		SCLittlePetFriendPetListInfo();
		MessageHeader header;

		int count;
		LittlePetFriendPet pet_list[LITTLE_PET_COUPLE_MAX_SHARE_NUM];
	};

	struct LittlePetInteractLogStruct
	{
		GameName name;
		int pet_id;
		unsigned int timestamp;
		GameName pet_name;
	};

	class SCLittlePetInteractLog		//8058
	{
	public:
		SCLittlePetInteractLog();
		MessageHeader header;

		int count;
		LittlePetInteractLogStruct log_list[LITTLE_PET_SHARE_MAX_LOG_NUM];
	};

	class CSLittlePetRename				//8002
	{
	public:
		CSLittlePetRename();
		MessageHeader header;

		int index;
		GameName pet_name;
	};

	class SCLittlePetRename				//8059
	{
	public:
		SCLittlePetRename();
		MessageHeader header;

		short index;
		short info_type;
		GameName pet_name;
	};

	class SCLittlePetWalk				//8060
	{
	public:
		SCLittlePetWalk();
		MessageHeader header;

		ObjID obj_id;
		short reserve_sh;
		int pet_id;
		GameName pet_name;
	};
}

#pragma  pack(pop)
#endif
