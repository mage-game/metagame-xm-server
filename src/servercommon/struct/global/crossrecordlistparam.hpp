#ifndef __CROSS_RECORD_LIST_PARAM_HPP__
#define __CROSS_RECORD_LIST_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"

#include "servercommon/crossdef.hpp"

class CrossRecordListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct CrossRecordAttr
	{
		long long id_cross_record;				// 仅在初始化时有用
		char change_state;
		int role_id;
		
		GameName role_name;
		short level;
		char avatar;
		char sex;
		char prof;
		char camp;
		char order_view_match_index;
		UserCrossRecordData user_record_data;
		RoleAppearance user_appearance_data;
	};

	static const int CROSS_RECORD_MAX_COUNT = 512;

	int count;
	CrossRecordAttr cross_record_list[CROSS_RECORD_MAX_COUNT];
};

#endif  // __CROSS_RECORD_LIST_PARAM_HPP__

