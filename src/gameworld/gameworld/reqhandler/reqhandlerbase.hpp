#ifndef __REQ_HANDLER_BASE_HPP__
#define __REQ_HANDLER_BASE_HPP__

// CS请求处理基类

// 一个正常的请求都应该有这样的处理流程：检查请求参数 -> 检查是否可以处理请求 -> 处理请求

#include "gamecommon.h"

class ReqHandlerBase
{
public:
	ReqHandlerBase() : m_role(0) {}
	virtual ~ReqHandlerBase() {}

	virtual bool OnReq(Role *role, void *cs);				// 客户端的一个请求

protected:
	virtual bool ParamCheck(void *cs) { return false; }		// 检查并设置请求参数
	virtual bool PreHandleCheck() { return false; }			// 检查是否可以处理请求（可否消耗、给奖励、升级等等）
	virtual bool DoHandle() { return false; }				// 处理请求（消耗、给予奖励、升级等等，同步协议、打印日志等等）

	Role *m_role;
};

#endif // __REQ_HANDLER_BASE_HPP__