#ifndef __CMDLIST_H__
#define __CMDLIST_H__

#include <string>

#include "common/tlvprotocol.h"

class CmdList
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct __cmd
	{
		int cmd_id;
		std::string creator;
		std::string cmd;
	};

	static const  int MAX_CMD_NUM = 32;

	int count;						
	__cmd cmd_list[MAX_CMD_NUM];
};

#endif // __CMDLIST_H__



