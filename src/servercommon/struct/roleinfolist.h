
#ifndef ROLEINFOLIST_H
#define ROLEINFOLIST_H

#include "servercommon/serverdef.h"
#include "servercommon/struct/upgradeparam.hpp"

class TLVSerializer;
class TLVUnserializer;

class RoleInfoList
{
public:
	bool Serialize(TLVSerializer *outstream) const;
	bool Unserialize(TLVUnserializer *instream);

	struct __roleinfo
	{
		int role_id;
		int scene_id;
		int last_scene_id;
		int level;
		int professional;
		unsigned int create_time;
		unsigned int last_login_time;
		GameName role_name;
		char avatar;
		char sex;
		char country;
		char reserve_1;							// 占位

		ItemID wuqi_id;							// 武器ID
		ItemID reserve_2;						// 占位
		char shizhuang_wuqi;					// 时装武器
		char shizhuang_wuqi_is_special;			// 时装武器是特殊形象
		char shizhuang_body;					// 时装身体
		char shizhuang_body_is_special;			// 时装身体是特殊形象
		short wing_used_imageid;				// 羽翼形象
		short halo_used_imageid;				// 光环形象
		short yaoshi_used_imageid;				// 腰饰形象
		short toushi_used_imageid;				// 头饰形象
		short qilinbi_used_imageid;				// 麒麟臂形象
		short mask_used_imageid;				// 面具形象
		short upgrade_used_imageid_0;			// 进阶系统形象id
		short upgrade_used_imageid_1;			// 进阶系统形象id
		short upgrade_used_imageid_2;			// 进阶系统形象id
		short upgrade_used_imageid_3;			// 进阶系统形象id
		short upgrade_used_imageid_4;			// 进阶系统形象id
		short upgrade_used_imageid_5;			// 进阶系统形象id
		short upgrade_used_imageid_6;			// 进阶系统形象id
		short upgrade_used_imageid_7;			// 进阶系统形象id
		short upgrade_used_imageid_8;			// 进阶系统形象id
		short shenbing_used_img_id;				// 神兵形象
		short shenbing_texiao_id;				// 神兵特效
		short baojia_used_img_id;				// 宝甲形象
		short baojia_texiao_id;					// 宝甲特效
	};

	static const int MAX_ROLE_NUM = 6;

	int count;
	__roleinfo role_info_list[MAX_ROLE_NUM];
};

#endif
