
#ifndef RMI_VER_MANAGER
#define RMI_VER_MANAGER

#include <stdio.h>

#include "servercommon/servercommon.h"
#include "servercommon/struct/roleinitparam.h"

class TLVUnserializer;

/**********************************************************************************************
								
									��ɫ���԰汾����
														dream  2011-05-07
		note:

		(1) �汾����+1����
		(2) �벻Ҫɾ��֮ǰ�汾����
		(3) �벻Ҫ����֮ǰ�汾���ݸ�ʽ ���� char -> int 
		(4) ���û���漰չʾ�ֶ� ��ô�������Ľ�ɫ����ʱ ������¸�server
		(5) ���ں���׷���ֶ� ע�����л�ʱ����˳��


		todo:
		(1) �汾1�������汾2������������ֶΣ���ô��Щ�����ӵ��ֶ���ô���ƣ� 
		    Ŀǰ�Ǵ����Loginͬ��һ�ݹ������ݲ�ʵ�֡�

**********************************************************************************************/


// Role�µ�Attr��İ汾����

class RMIRoleAttrVerManager
{
public:

	enum
	{
		RMI_ROLE_ATTR_VER_INVALID = 0,
		RMI_ROLE_ATTR_VER_1,

		RMI_ROLE_ATTR_VER_MAX,
	};

	static const int RMI_ROLE_ATTR_CURRENT_VERSION = RMI_ROLE_ATTR_VER_MAX - 1; // ��ǰ�ڴ�汾
	static const int ROLE_ATTR_MAX_BUFFER_LENGTH = 2048;							

	static RMIRoleAttrVerManager & Instance()
	{
		static RMIRoleAttrVerManager inst;
		return inst;
	}

	bool HandlerVerUnSerializer(short ver, RoleInitParam & role_init_param, TLVUnserializer &instream);

private:

	typedef bool (RoleInitParam::*VerHandlerFunc) (TLVUnserializer &instream);

	struct VerHandlerItem 
	{
		short ver;
		VerHandlerFunc func;
	};

	VerHandlerItem* GetVerHandlerItemList()
	{
		static VerHandlerItem ver_hander_item_list[] = 
		{
			{RMI_ROLE_ATTR_VER_INVALID, NULL},
			{RMI_ROLE_ATTR_VER_1, &RoleInitParam::Unserialize},
		};

		UNSTD_STATIC_CHECK(sizeof(ver_hander_item_list) / sizeof(ver_hander_item_list[0]) == RMI_ROLE_ATTR_VER_MAX);

		return &ver_hander_item_list[0];
	}

private:
	
	// �汾������ ��Ҫ���õĳ�ʼ������
	typedef void (RMIRoleAttrVerManager::*VerFunc_Ver_Change_Init) (RoleInitParam & role_init_param);

	struct VerFuncVerChangeItem 
	{
		short ver_cur;			// ��ǰ�汾
		short ver_next;			// �¸��汾
		VerFunc_Ver_Change_Init func;
	};
	
	// ���µ����µİ汾
	void Update_TO_Max_Version(short ver_cur, RoleInitParam & role_init_param);

	// ��һ���汾���ڶ����汾 ����õĳ�ʼ������ �൱��Diff��Ϣ 
	

	VerFuncVerChangeItem* GetVerFuncChangeInitList()
	{
		static VerFuncVerChangeItem ver_func_vec_change_item_list[] = 
		{
			{RMI_ROLE_ATTR_VER_INVALID, RMI_ROLE_ATTR_VER_INVALID, NULL},
		};

		UNSTD_STATIC_CHECK(sizeof(ver_func_vec_change_item_list) / sizeof(ver_func_vec_change_item_list[0]) == (RMI_ROLE_ATTR_VER_MAX - 1));

		return &ver_func_vec_change_item_list[0];
	}

private:
	RMIRoleAttrVerManager() {}
	RMIRoleAttrVerManager(const RMIRoleAttrVerManager &rhs);
};


#endif  // RMI_VER_MANAGER

