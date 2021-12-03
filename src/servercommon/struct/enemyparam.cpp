
#include "enemyparam.h"
#include "structcommon.h"
#include "servercommon/servercommon.h"


bool EnemyListParam::Serialize(TLVSerializer &outstream)const
{
	if (!outstream.Push(count))
	{
		return false;
	}

	bool ret = true;

	for (int i = 0; i < count && i < MAX_ENEMY_NUM; ++i)
	{
		ret = outstream.Push(enemy_list[i].change_state) && outstream.Push(enemy_list[i].index);

		if (!ret)
		{
			return false;
		}

		if (enemy_list[i].change_state == structcommon::CS_DELETE)
		{
			continue;
		}

		ret = outstream.Push(enemy_list[i].enemy_uid) &&
			  outstream.Push(enemy_list[i].enemy_plat_type) &&
			  outstream.Push(enemy_list[i].kill_count) &&
			  outstream.Push(enemy_list[i].last_kill_time) &&
			  outstream.Push(enemy_list[i].be_kill_count);


		if (!ret)
		{
			return false;
		}
	}

	return ret;
};

bool EnemyListParam::Unserialize(TLVUnserializer &instream)
{
	if (!instream.Pop(&count))
	{
		return false;
	}

	bool ret = true;

	for (int i = 0; i < count && i < MAX_ENEMY_NUM; ++i)
	{
		ret = instream.Pop(&enemy_list[i].change_state) && instream.Pop(&enemy_list[i].index);

		if (!ret)
		{
			return false;
		}

		if (enemy_list[i].change_state == structcommon::CS_DELETE)
		{
			continue;
		}

		ret = instream.Pop(&enemy_list[i].enemy_uid) &&
			  instream.Pop(&enemy_list[i].enemy_plat_type) &&
			  instream.Pop(&enemy_list[i].kill_count) &&
			  instream.Pop(&enemy_list[i].last_kill_time) &&
			  instream.Pop(&enemy_list[i].be_kill_count);

		if (!ret)
		{
			return false;
		}

	}

	return ret;
}

