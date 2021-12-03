#include "servercommon/mempool/mempoolmanager.h"

#include "crossuser/crossuser.h"
REGISTER_MEMPOOLMGR(gamemempool, CrossUser, 64, "CrossUser")

#include "rmibackobjdef.h"
REGISTER_MEMPOOLMGR(gamemempool, RMIInitCrossStatusBackObjectImpl, 64, "RMIInitCrossStatusBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveCrossStatusBackObjectImpl, 64, "RMISaveCrossStatusBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIInitCrossPersonRankBackObjectImpl, 64, "RMIInitCrossPersonRankBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveCrossPersonRankBackObjectImpl, 64, "RMISaveCrossPersonRankBackObjectImpl")



