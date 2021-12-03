#include "servercommon/mempool/mempoolmanager.h"

#include "rmibackobjdef.h"
REGISTER_MEMPOOLMGR(gamemempool, RMIGetProfBackObjectImpl, 64, "RMIGetProfBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIUserLoginBackObjectImpl, 64, "RMIUserLoginBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIUserLoginBackObjectImplCreate, 64, "RMIUserLoginBackObjectImplCreate")
REGISTER_MEMPOOLMGR(gamemempool, RMIAddRoleBackObjectImpl, 64, "RMIAddRoleBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIRemoveRoleBackObjectImpl, 64, "RMIRemoveRoleBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIPassAntiWallowBackObjectImpl, 64, "RMIPassAntiWallowBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIAddNameInfoBackObjectImpl, 64, "RMIAddNameInfoBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIUpdateNameInfoBackObjectImpl, 64, "RMIUpdateNameInfoBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIDeleteNameInfoBackObjectImpl, 64, "RMIDeleteNameInfoBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIGetRoleInfoBackObjectImpl, 64, "RMIGetRoleInfoBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIGetRoleInfoBackObjectImplRoleReq, 64, "RMIGetRoleInfoBackObjectImplRoleReq")
REGISTER_MEMPOOLMGR(gamemempool, RMICreateRoleBackObjectImpl, 64, "RMICreateRoleBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIDestroyRoleBackObjectImpl, 64, "RMIDestroyRoleBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIUserLoginBackObjectImplRole, 64, "RMIUserLoginBackObjectImplRole")
REGISTER_MEMPOOLMGR(gamemempool, RMIForbidBackObjectImpl, 64, "RMIForbidBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIForbidOneRoleBackObjectImpl, 64, "RMIForbidOneRoleBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMILoginGetCmdBackObjectImpl, 4, "RMILoginGetCmdBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMILoginInitWorldStatusBackObjectImpl, 4, "RMILoginInitWorldStatusBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMILoginSaveWorldStatusBackObjectImpl, 4, "RMILoginSaveWorldStatusBackObjectImpl")

#include "mergeloginmanager.h"
REGISTER_MEMPOOLMGR(gamemempool, MergeLoginItem, 64, "MergeLoginItem")

OLD_USE_INTERNAL_NETWORK();
