
#ifndef RMIBASE_H
#define RMIBASE_H

#include <map>
#include <string>
#include "common/tlvprotocol.h"
#include "irmimodule.h"

namespace rmi
{
	class RMISession;

	enum ERMIDispatchStatus
	{
		DispatchOK,
		DispatchObjectNotExist,
		DispatchMethodNotExist,
		DispatchParamError,
		DispatchOutParamBuffTooShort,
		SessionDisconnect,
	};

	static const unsigned int MAX_IDENTITY_LEN = 32;

	// 服务端调用对象
	class RMIObject
	{
	public:
		typedef int (*RMIMethod)(TLVUnserializer &in_param, TLVSerializer *out_param); 
		RMIObject(){};
		virtual ~RMIObject(){};
		int __dispatch(const char *method, TLVUnserializer &in_param, TLVSerializer *out_param);
		virtual const char * __get_obj_id()=0;
		virtual void __free()=0;
	protected:
		typedef std::map<std::string, RMIMethod> MethodList;
		MethodList	m_method_list;
	};

	// 客户端调用对象
	class RMIProxyObject
	{
	public:
		RMIProxyObject(){};
		virtual ~RMIProxyObject(){};
		void __bind_session(const Session &session) { m_session = session; }
	protected:
		Session m_session;
	};

	// 返回调用对象
	class RMIBackObject
	{
	public:
		RMIBackObject(){};
		virtual ~RMIBackObject(){};
		virtual bool __response(TLVUnserializer &out_param){ return true; }
		virtual void __exception(int error){};
		virtual void __timeout(){};
		void __set_id(unsigned long mid) { m_message_id = mid; }
		unsigned long __get_id() { return m_message_id; }
		virtual void __free()=0;
	protected:
		unsigned long m_message_id;
	};
}

#endif
