
#include "darmiinit.h"
#include "rmiobj/rmilogin.h"
#include "rmiobj/rmirole.h"
#include "rmiobj/rmisystem.h"
#include "rmiobj/rmiglobal.hpp"
#include "rmiobj/rmicross.hpp"

void DARMIInit::Init(IRMIModule *rmimodule)
{
	rmimodule->Register(new RMILoginObject());
	rmimodule->Register(new RMIRoleObject());
	rmimodule->Register(new RMIGlobalObject());
	rmimodule->Register(new RMISystemObject());
	rmimodule->Register(new RMICrossObject());
}
