

#include "reqhandlerbase.hpp"

bool ReqHandlerBase::OnReq(Role *role, void *cs)
{
	m_role = role;

	auto ret = ParamCheck(cs);
	if (ret)
	{
		ret = PreHandleCheck();
	}

	if (ret)
	{
		ret = DoHandle();
	}

	if (ret)
	{
		//交由派生类去提示
		//m_role->NoticeNum(noticenum::NT_REQ_HANDLE_SUCC);
		return true;
	}

	return false;
}