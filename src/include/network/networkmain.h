
#pragma once

#ifndef NETWORKMAIN_H
#define NETWORKMAIN_H

#ifdef _MSC_VER
	#ifdef NETWORK_EXPORTS 
		#define NETWORK_API __declspec( dllexport )
	#else
		#define NETWORK_API __declspec( dllimport )
	#endif
#else
	#define NETWORK_API
#endif



class IInterfaceMgr;

namespace NetworkDLL
{
	typedef void (*RegisterInterfaceFunc)(IInterfaceMgr* interface_mgr);
	static const char INTERFACE_NAME[] = "RegisterInterface";

	NETWORK_API void Register(IInterfaceMgr* interface_mgr, const char *module_name=0);
}

#endif
