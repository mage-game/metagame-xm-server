
#ifndef RMI_VER_MANAGER
#define RMI_VER_MANAGER

#include <stdio.h>

#include "servercommon/servercommon.h"
#include "servercommon/struct/roleinitparam.h"

class TLVUnserializer;

/**********************************************************************************************
								
									角色属性版本管理
														dream  2011-05-07
		note:

		(1) 版本号请+1递增
		(2) 请不要删除之前版本数据
		(3) 请不要更改之前版本数据格式 比如 char -> int 
		(4) 如果没有涉及展示字段 那么仅仅更改角色属性时 无需更新该server
		(5) 请在后面追加字段 注意序列化时参数顺序


		todo:
		(1) 版本1升级到版本2如果出现增加字段，那么这些新增加的字段怎么复制？ 
		    目前是打算从Login同步一份过来，暂不实现。

**********************************************************************************************/


// Role新的Attr表的版本管理

class RMIRoleAttrVerManager
{
public:

	enum
	{
		RMI_ROLE_ATTR_VER_INVALID = 0,
		RMI_ROLE_ATTR_VER_1,

		RMI_ROLE_ATTR_VER_MAX,
	};

	static const int RMI_ROLE_ATTR_CURRENT_VERSION = RMI_ROLE_ATTR_VER_MAX - 1; // 当前内存版本
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
	
	// 版本递增后 需要调用的初始化函数
	typedef void (RMIRoleAttrVerManager::*VerFunc_Ver_Change_Init) (RoleInitParam & role_init_param);

	struct VerFuncVerChangeItem 
	{
		short ver_cur;			// 当前版本
		short ver_next;			// 下个版本
		VerFunc_Ver_Change_Init func;
	};
	
	// 更新到最新的版本
	void Update_TO_Max_Version(short ver_cur, RoleInitParam & role_init_param);

	// 第一个版本到第二个版本 需调用的初始化函数 相当是Diff信息 
	

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

