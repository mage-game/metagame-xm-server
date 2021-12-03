
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
			ZHUANZHI_OPERA_TYPE_TIANMING = 0,  //点亮
			ZHUANZHI_OPERA_TYPE_CANGLONG,
			ZHUANZHI_OPERA_TYPE_SHENGXIAO,
			ZHUANZHI_OPERA_TYPE_YUANQI,
			ZHUANZHI_OPERA_TYPE_SHENQI,
			ZHUANZHI_OPERA_TYPE_XINGXIU,
			// 以上转职点亮 参数一  为是否使用经验转职
			ZHUANZHI_OPERA_TYPE_ONEKEY,         // 一键转职
			ZHUANZHI_OPERA_TYPE_PERFORM_SKILL,     // 释放技能 p1:vir_skill_seq p2:target_obj_id
		};

		short operate_type;
		short param_1;
		short param_2;
		short param_3;
	};

	class SCRoleZhuanZhiInfo    // 8813   转职信息
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

	class SCZhuanzhiSkillTrigger   // 8814 转职技能释放广播
	{
	public:
		SCZhuanzhiSkillTrigger();
		MessageHeader header;

		short skill_id;						 // 技能id
		short reserve_sh;
		ObjID target_obj_id;				 // 目标obj_id
		ObjID deliver_obj_id;				 // 释放obj_id
		unsigned int last_perform;			 // 上一次释放时间戳
		long long injure;					 // 伤害值
	};
}

#pragma pack(pop)

#endif

