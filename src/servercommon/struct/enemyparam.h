/******************************************************************************************
FileName: enemyparam.h
Author: 
Description: ���������ݿ��еĶ��� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
shell       2011/08/01/ 16:04:02     1.0           ά��
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
		int				kill_count;			// ɱ���õ��˵Ĵ���
		long long		last_kill_time;		// ���ɱ/��ɱ��ʱ��
		int				be_kill_count;		// ���õ���ɱ���Ĵ���
	};

	int count;
	Enemy enemy_list[MAX_ENEMY_NUM];
};


#endif

