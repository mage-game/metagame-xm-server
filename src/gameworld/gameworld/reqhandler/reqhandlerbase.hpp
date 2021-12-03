#ifndef __REQ_HANDLER_BASE_HPP__
#define __REQ_HANDLER_BASE_HPP__

// CS���������

// һ������������Ӧ���������Ĵ������̣����������� -> ����Ƿ���Դ������� -> ��������

#include "gamecommon.h"

class ReqHandlerBase
{
public:
	ReqHandlerBase() : m_role(0) {}
	virtual ~ReqHandlerBase() {}

	virtual bool OnReq(Role *role, void *cs);				// �ͻ��˵�һ������

protected:
	virtual bool ParamCheck(void *cs) { return false; }		// ��鲢�����������
	virtual bool PreHandleCheck() { return false; }			// ����Ƿ���Դ������󣨿ɷ����ġ��������������ȵȣ�
	virtual bool DoHandle() { return false; }				// �����������ġ����轱���������ȵȣ�ͬ��Э�顢��ӡ��־�ȵȣ�

	Role *m_role;
};

#endif // __REQ_HANDLER_BASE_HPP__