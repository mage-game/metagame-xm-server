#include "roleattrvermanager.h"

#include "common/tlvprotocol.h"
#include "servercommon/servercommon.h"
#include "servercommon/struct/roleinitparam.h"

bool RMIRoleAttrVerManager::HandlerVerUnSerializer(short ver, RoleInitParam & role_init_param, TLVUnserializer &instream)
{
	if (ver < 0) return false;

	VerHandlerItem *list = GetVerHandlerItemList();
	if (NULL == list) return false;
		
	if (ver < RMI_ROLE_ATTR_VER_MAX)
	{
		if (list[ver].ver != ver) // 后面可加个循环 在这里容错下 TODO 但是现在先不管它 
		{
			return false;
		}
		else 
		{	
			if (NULL != list[ver].func)
			{
				bool ret = (role_init_param.*list[ver].func)(instream);
				if (ret) this->Update_TO_Max_Version(ver, role_init_param);
				return ret;
			}
		}
	}
	else
	{
		return (role_init_param.*list[RMI_ROLE_ATTR_VER_MAX - 1].func)(instream);
	}

	return false;
}

void RMIRoleAttrVerManager::Update_TO_Max_Version(short ver_cur, RoleInitParam & role_init_param)
{
	if (ver_cur < 0 || ver_cur >= RMI_ROLE_ATTR_CURRENT_VERSION) return;

	int idx = 0;

	VerFuncVerChangeItem *list = GetVerFuncChangeInitList();

	if (NULL != list)
	{
		while (list[idx].ver_cur != ver_cur) idx++;
		
		for (; NULL != list[idx].func; idx++)
		{
			(this->*list[idx].func)(role_init_param);
		}
	}
}
