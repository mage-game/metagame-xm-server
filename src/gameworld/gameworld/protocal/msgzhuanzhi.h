
#ifndef MSG_ZHUANZHI_H
#define MSG_ZHUANZHI_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/chatserverdef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class CSRoleZhuanZhiReq          // 8812
	{
	public:
		CSRoleZhuanZhiReq();
		MessageHeader		header;

		enum ZHUANZHI_OPERA_TYPE
		{
			ZHUANZHI_OPERA_TYPE_TIANMING = 0,  //����
			ZHUANZHI_OPERA_TYPE_CANGLONG,
			ZHUANZHI_OPERA_TYPE_SHENGXIAO,
			ZHUANZHI_OPERA_TYPE_YUANQI,
			ZHUANZHI_OPERA_TYPE_SHENQI,
			ZHUANZHI_OPERA_TYPE_XINGXIU,
			// ����תְ���� ����һ  Ϊ�Ƿ�ʹ�þ���תְ
			ZHUANZHI_OPERA_TYPE_ONEKEY,         // һ��תְ
			ZHUANZHI_OPERA_TYPE_PERFORM_SKILL,     // �ͷż��� p1:vir_skill_seq p2:target_obj_id
		};

		short operate_type;
		short param_1;
		short param_2;
		short param_3;
	};

	class SCRoleZhuanZhiInfo    // 8813   תְ��Ϣ
	{
	public:
		SCRoleZhuanZhiInfo();
		MessageHeader		header;

		void Reset() 
		{
			zhuanzhi_level_tianming = 0;
			zhuanzhi_level_canglong = 0;
			zhuanzhi_level_shengxiao = 0;
			zhuanzhi_level_yuanqi = 0;
			zhuanzhi_level_shenqi = 0;
			zhuanzhi_level_xingxiu = 0;
			res_sh = 0;
		}

		char zhuanzhi_level_tianming;
		char zhuanzhi_level_canglong;
		char zhuanzhi_level_shengxiao;
		char zhuanzhi_level_yuanqi;

		char zhuanzhi_level_shenqi;
		char zhuanzhi_level_xingxiu;
		short res_sh;
	};

	class SCZhuanzhiSkillTrigger   // 8814 תְ�����ͷŹ㲥
	{
	public:
		SCZhuanzhiSkillTrigger();
		MessageHeader header;

		short skill_id;						 // ����id
		short reserve_sh;
		ObjID target_obj_id;				 // Ŀ��obj_id
		ObjID deliver_obj_id;				 // �ͷ�obj_id
		unsigned int last_perform;			 // ��һ���ͷ�ʱ���
		long long injure;					 // �˺�ֵ
	};
}

#pragma pack(pop)

#endif

