
#pragma once

#ifndef LOGMAIN_H
#define LOGMAIN_H

#ifdef _MSC_VER

	#ifdef LOG_EXPORTS 
		#define LOG_API __declspec( dllexport )
	#else
		#define LOG_API __declspec( dllimport )
	#endif

	#define EXPORT __declspec( dllexport )
#else
	#define LOG_API
	#define EXPORT
#endif


class IInterfaceMgr;

namespace LogDLL
{
	typedef void (*RegisterInterfaceFunc)(IInterfaceMgr* interface_mgr);
	static const char INTERFACE_NAME[] = "RegisterInterface";

	LOG_API void Register(IInterfaceMgr* interface_mgr);
}

#endif
