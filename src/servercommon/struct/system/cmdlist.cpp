
#include "cmdlist.h"


bool CmdList::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < CmdList::MAX_CMD_NUM; ++i)
	{
		ret = outstream.Push(cmd_list[i].cmd_id) && outstream.Push(cmd_list[i].creator.c_str()) && outstream.Push(cmd_list[i].cmd.c_str());
		if (!ret) return false;
	}

	return true;
}

bool CmdList::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret || count >  CmdList::MAX_CMD_NUM) return false;

	for (int i = 0; i < count; ++i)
	{
		const char *creator_tmp = 0;
		const char *cmd_tmp = 0;

		ret = instream.Pop(&cmd_list[i].cmd_id) && instream.Pop(&creator_tmp) && instream.Pop(&cmd_tmp);
		if (!ret) return false;

		if (creator_tmp != 0) cmd_list[i].creator = creator_tmp;
		if (cmd_tmp != 0) cmd_list[i].cmd = cmd_tmp;
	}

	return true;
}


