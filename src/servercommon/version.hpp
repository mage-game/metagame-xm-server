/******************************************************************************************
FileName:revisiondef.hpp
Author: xlt
Description: server版本定义
	A1.A2.M.B1B2.
	A2 当某个版本全网升级时更新
	M 0是主干、1是分支
	B1 是主干版本号
	B2 该分支在主干上的版本号
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
xlt			 2011/9/27/14:43:00      1.0		  创建
*****************************************************************************************/

#ifndef __REVISION_HPP__
#define __REVISION_HPP__

const unsigned int CHAT_SVR_VERSION = 10020;	
const unsigned int DATAACCESS_SVR_VERSION = 10020;
const unsigned int GAMEWORLD_SVR_VERSION = 10020;	
const unsigned int GLOBAL_SVR_VERSION = 10020;
const unsigned int LOGIN_SVR_VERSION = 10020;
const unsigned int CROSS_SVR_VERSION = 10020;

#endif
