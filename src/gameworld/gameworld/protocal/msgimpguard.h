#ifndef __MSG_IMP_GUARD_H_
#define __MSG_IMP_GUARD_H_

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/cardzudef.hpp"
#include "other/impguard/impguard.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum IMP_GUARD_REQ_TYPE
	{
		IMP_GUARD_REQ_TYPE_RENEW_PUTON = 0,			// ���Ѵ������ϵ�С�� param1:����С�����index  param2:�Ƿ�ʹ�ð�Ԫ
		IMP_GUARD_REQ_TYPE_RENEW_KNAPSACK,			// ���ѱ����е�С�� param1:����index param2:�Ƿ�ʹ�ð�Ԫ
		IMP_GUARD_REQ_TYPE_TAKEOFF,					// ����С�� param1:����С�����index  0 1
		IMP_GUARD_REQ_TYPE_ALL_INFO,

		IMP_GUARD_REQ_TYPE_MAX,
	}; 

	class CSImpGuardOperaReq						// 8805
	{
	public:
		CSImpGuardOperaReq();
		MessageHeader header;

		int opera_type;
		short param1;
		short param2;
	};

	class SCImpGuardInfo							// 8806
	{
	public:
		SCImpGuardInfo();
		MessageHeader header;

		struct ImpGuardData
		{
			char grid_index;
			char used_imp_type;
			char is_expire;			// �Ƿ���ڡ� 1���ڣ�0û��
			char res_ch;
			ItemDataWrapper item_wrapper;
		};

		ImpGuardData imp_guard_data[ImpGuard::IMP_GUARD_GRID_INDEX_MAX];   // 2
	};

	class SCRoleImpAppeChange         //8807    С���ػ���۱任֪ͨ��Χ�ڵ����
	{
	public:
		SCRoleImpAppeChange();
		MessageHeader header;

		ObjID objid;                    // �����任С������˵�objid
		short appe_index;               // �����仯��С��index   0��1
		short appe_id;
	};

	class SCRoleImpExpireTime         // 8811    ֪ͨС����   ���ֲ����ˣ��Ȳ�ɾ�ˣ������⣬�Ժ�������ã�
	{
	public:
		SCRoleImpExpireTime();
		MessageHeader header;

		short grid_index;               // �����仯��С��index   0��1
		short res_sh;
	};
}

#pragma pack(pop)

#endif

