/******************************************************************************************
FileName: campparam.hpp
Author: dreamwang
Description: 国家在数据库中的定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __CAMP_PARAM_HPP__
#define __CAMP_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/campdef.hpp"
#include "servercommon/serverdef.h"

class CampBaseParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	int camp_type;					// 阵营类型
	int role_count;					// 角色个数
	int guild_count;				// 军团个数
	int rich_level;					// 矿藏富裕度
	unsigned int earnings_end_time1;// 盗取珍宝收益结束时间
	int earnings1;					// 盗取珍宝收益
	unsigned int earnings_end_time2;// 盗取珍宝收益结束时间
	int earnings2;					// 盗取珍宝收益

	long long total_capability;		// 总战力
};

struct CampAttr
{
	CampBaseParam base_info;
};

class CampParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	int count;
	CampAttr camp_list[CAMP_TYPE_MAX]; 
};

#endif // __CAMP_PARAM_HPP__

