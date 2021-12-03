
#pragma once

#ifndef IAPIMODULE_H
#define IAPIMODULE_H

#include "imodule.h"

static const char API_MODULE[] = "APIModule";

class IAPIModule : public IModule
{
public:
	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;
};

#endif
