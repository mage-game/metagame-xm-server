
#pragma once

#ifndef RMIMAIN_H
#define RMIMAIN_H

#ifdef _MSC_VER
	#ifdef RMI_EXPORTS 
		#define RMI_API __declspec( dllexport )
	#else
		#define RMI_API __declspec( dllimport )
	#endif
#else
	#define RMI_API
#endif



class IInterfaceMgr;

namespace RMIDLL
{
	typedef void (*RegisterInterfaceFunc)(IInterfaceMgr* interface_mgr);
	static const char INTERFACE_NAME[] = "RegisterInterface";

	RMI_API void Register(IInterfaceMgr* interface_mgr, const char *network_module=0);
}

#endif
