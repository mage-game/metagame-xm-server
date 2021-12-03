#ifndef _MSG_MULTI_MOUNT_H_
#define _MSG_MULTI_MOUNT_H_

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/multimountdef.hpp"
#include "servercommon/newmultimountdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum MULTI_MOUNT_REQ_TYPE
	{
		MULTI_MOUNT_REQ_TYPE_SELECT_MOUNT = 0,								// ѡ��ʹ�����param1 ����id
		MULTI_MOUNT_REQ_TYPE_UPGRADE,										// ������ף�param1 ����id, param2 �ظ�������param3 �Ƿ��Զ�����
		MULTI_MOUNT_REQ_TYPE_RIDE,											// ������
		MULTI_MOUNT_REQ_TYPE_UNRIDE,										// ������
		MULTI_MOUNT_REQ_TYPE_INVITE_RIDE,									// ������ˣ�param1 ���id
		MULTI_MOUNT_REQ_TYPE_INVITE_RIDE_ACK,								// ��Ӧ������ˣ�param1 ���id��param2 �Ƿ�ͬ��
		MULTI_MOUNT_REQ_TYPE_USE_SPECIAL_IMG,								// ����ʹ�ûû�����param1 ��������ID
		MULTI_MOUNT_REQ_TYPE_UPLEVEL_SPECIAL_IMG,							// ����������������param1 ��������ID
	};

	class CSMultiMountOperaReq												// ˫������������� 5686
	{
	public:
		CSMultiMountOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCMultiMountAllInfo												// ����������Ϣ 6123
	{
	public:
		struct MountItem
		{
			char is_mount_active;											// �����Ƿ��Ѽ���
			char is_img_active;												// ���������Ƿ��Ѽ���
			short reserve_sh;

			short grade_bless;												// ����ף��ֵ
			short star_level;												// �Ǽ�
		};

		SCMultiMountAllInfo();
		MessageHeader header;

		short cur_use_mount_id;
		short cur_use_special_image_id;										// ��ǰʹ�õ���������id,0��ʹ���κ���������

		MountItem mount_list[COUPLEMOUNT_MAX_ID + 1];

		unsigned int special_img_active_flag;								// �������Ｄ��λ���, 0δ���1����

		char special_img_level_list[COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT];
		char reserve_ch;
	};

	enum MULTI_MOUNT_CHANGE_NOTIFY_TYPE
	{
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_SELECT_MOUNT = 0,					// ��ǰʹ���е�����ı�, param1 ����id
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPGRADE,								// �������ݸı�, param1 ����id��param2 ������param3 ף��ֵ
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_INVITE_RIDE,							// �յ�������������, param1 ���ID��param2 ����ID
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_ACTIVE_SPECIAL_IMG,					// ����˫�������������� param1 �������󼤻��� param2 ����id param3 ����ȼ�
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_USE_SPECIAL_IMG,						// ʹ����������	param1 ��������id��param2 ��������ȼ�
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPGRADE_EQUIP,						// ����װ��	param1 װ�����ͣ� param2 װ���ȼ�
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPLEVEL_SPECIAL_IMG,					// ������������	param1 ��������id�� param2 ��������ȼ�
	};

	class SCMultiMountChangeNotify											// ������Ϣ�������� 6124
	{
	public:
		SCMultiMountChangeNotify();
		MessageHeader header;

		short notify_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCMultiMountNotifyArea											// ������Ϣ����㲥 6125
	{
	public:
		SCMultiMountNotifyArea();
		MessageHeader header;

		int owner_uid;														// �������id
		int follower_uid;													// ���������id
		ObjID owner_obj_id;													// ����id
		ObjID folower_obj_id;												// ������id
		int owner_multimount_res_id;										// ����������Դid
		int folower_multimount_res_id;										// ������������Դid
	};
}

#pragma pack(pop)

#endif

