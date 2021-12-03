#ifndef MSG_SHENGWANG_H
#define MSG_SHENGWANG_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shengwangdef.hpp"

#pragma pack(push, 4) 

namespace Protocol
{
	enum SHENGWNAG_OPERA_TYPE
	{
		SHENGWANG_OPERA_REQ_INFO = 0,			// �������������Ϣ
		SHENGWANG_OPERA_XIANJIE_UPLEVEL,		// �ɽ�����
		SHENGWANG_OPERA_XIANDAN_UPLEVEL,		// �ɵ�����
	};

	class CSShengWangOpera
	{
	public:
		CSShengWangOpera();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
	};

	class SCShengWangInfo
	{
	public:
		SCShengWangInfo();
		MessageHeader header;

		short xianjie_level;
		short xiandan_level;

		short process_times;
		short reserve_sh;

		int need_gold_on_uplevel_next_xiandan;

		int shuxingdan_list[ShengWangParam::SHENGWANG_SHUXINGDAN_MAX_TYPE];
	};

	class SCXianJieViewChange
	{
	public:
		SCXianJieViewChange();
		MessageHeader header;

		ObjID obj_id;
		short xianjie_level;
	};
}

#pragma pack(pop)

#endif

