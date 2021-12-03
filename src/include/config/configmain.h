
#pragma once

#ifndef CONFIGMAIN_H
#define CONFIGMAIN_H

#ifdef _MSC_VER
	#ifdef CONFIG_EXPORTS 
	#define CONFIG_API __declspec( dllexport )
	#else
	#define CONFIG_API __declspec( dllimport )
	#endif

	#define EXPORT __declspec( dllexport )
#else
	#define CONFIG_API
	#define EXPORT
#endif


class IInterfaceMgr;

namespace ConfigDLL
{
	typedef void (*RegisterInterfaceFunc)(IInterfaceMgr* interface_mgr, const char* path, bool create_file_when_error);
	static const char INTERFACE_NAME[] = "RegisterInterface";

	CONFIG_API void Register(IInterfaceMgr* interface_mgr, const char* path="config.xml", bool create_file_when_error=true);
}

#endif
