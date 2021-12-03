
#ifndef BUSINESSPARAM_H
#define BUSINESSPARAM_H


#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"

class BusinessListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct __businessgood 
	{
		char			change_state;

		int				index;
		BusinessGoodID	business_good_id;
		int				bury_prince;
	};


	int count;
	static const  int MAX_BUSINESS_GOOD_NUMS = 64;
	__businessgood business_list[MAX_BUSINESS_GOOD_NUMS];
};



#endif


