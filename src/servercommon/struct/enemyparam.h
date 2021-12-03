/******************************************************************************************
FileName: enemyparam.h
Author: 
Description: 敌人在数据库中的定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
shell       2011/08/01/ 16:04:02     1.0           维护
*****************************************************************************************/

#ifndef ENEMYLISTPARAM
#define ENEMYLISTPARAM

#include "servercommon/basedef.h"
#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/frienddef.hpp"

class EnemyListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct Enemy 
	{
		char			change_state;

		int				index;
		int				enemy_uid;
		int				enemy_plat_type;
		int				kill_count;			// 杀死该敌人的次数
		long long		last_kill_time;		// 最后杀/被杀死时间
		int				be_kill_count;		// 被该敌人杀死的次数
	};

	int count;
	Enemy enemy_list[MAX_ENEMY_NUM];
};


#endif

