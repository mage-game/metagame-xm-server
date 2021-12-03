/******************************************************************************************
FileName: frienddef.hpp
Author: shell
Description: 好友、敌人、黑名单相关的定义
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
shell       2011/08/01/ 16:03:08     1.0           创建
*****************************************************************************************/

#ifndef __FRIENDDEF_H__
#define __FRIENDDEF_H__

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"

static const int MAX_FRIEND_NUM = 200;			// 最大好友数量
static const int MAX_ENEMY_NUM = 100;			// 最大敌人数量
static const int MAX_BLACK_NUM = 100;			// 最大黑名单数量

static const int EXPAND_SIZE = 4;				// 申请内存每次预留大小
static const int FRIENDNUM_FOR_LEVEL_COUNT = 4; // 好友最大数量不同区间段个数

static const int FRIENDNUM_HELI_COND_TYPE_MAX_COUNT = 30;	// 好友贺礼，祝贺条件类型最大值
static const int DEC_FRIEND_OFFLINE_TIME = 3;				// 连续离线x天并且亲密度少于x时删除好友(天）
static const int DEC_FRIEND_OFFLINE_INTIMACY = 99;			// 连续离线x天并且亲密度少于x时删除好友(亲密度）

#pragma pack(push, 4)

enum AddFriendStatus
{
	ADDFRIEND_REJECT = 0,						// 拒绝
	ADDFRIEND_AGREE = 1,						// 同意
	ADDFRIEND_OTHERFULL = 2,					// 对方已满
};

#pragma pack(pop)

#endif

