/******************************************************************************************
FileName: campparam.hpp
Author: dreamwang
Description: ���������ݿ��еĶ��� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           ����
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

	int camp_type;					// ��Ӫ����
	int role_count;					// ��ɫ����
	int guild_count;				// ���Ÿ���
	int rich_level;					// ��ظ�ԣ��
	unsigned int earnings_end_time1;// ��ȡ�䱦�������ʱ��
	int earnings1;					// ��ȡ�䱦����
	unsigned int earnings_end_time2;// ��ȡ�䱦�������ʱ��
	int earnings2;					// ��ȡ�䱦����

	long long total_capability;		// ��ս��
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

