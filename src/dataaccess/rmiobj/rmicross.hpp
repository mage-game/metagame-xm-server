/******************************************************************************************
FileName: rmicross.hpp
Author: dreamwang
Description: CROSS库的处理  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __RMI_CROSS_HPP__
#define __RMI_CROSS_HPP__

#include "rmi/rmibase.h"

#include "servercommon/crossdef.hpp"
#include "servercommon/struct/cross/crossstatusparam.hpp"
#include "servercommon/struct/global/rankparam.hpp"

class IStatement;
class DataNode;

class RMICrossObject : public rmi::RMIObject
{
public:
	RMICrossObject();
	virtual ~RMICrossObject();

	virtual void __free();
	virtual const char * __get_obj_id();
	
	static int __InitCrossStatus(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitCrossStatus(long long id_from, CrossStatusParam *crossstatus_param);

	static int __SaveCrossStatus(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveCrossStatus(const CrossStatusParam &crossstatus_param);

	static int __InitCrossPersonRank(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitCrossPersonRank(CrossPersonRankParam *cross_person_rank_param);

	static int __SaveCrossPersonRank(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveCrossPersonRank(const CrossPersonRankParam &cross_person_rank_param);
};

#endif  // __RMI_CROSS_HPP__

