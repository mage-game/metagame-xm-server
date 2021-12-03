
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
		char reserve_1;							// ռλ

		ItemID wuqi_id;							// ����ID
		ItemID reserve_2;						// ռλ
		char shizhuang_wuqi;					// ʱװ����
		char shizhuang_wuqi_is_special;			// ʱװ��������������
		char shizhuang_body;					// ʱװ����
		char shizhuang_body_is_special;			// ʱװ��������������
		short wing_used_imageid;				// ��������
		short halo_used_imageid;				// �⻷����
		short yaoshi_used_imageid;				// ��������
		short toushi_used_imageid;				// ͷ������
		short qilinbi_used_imageid;				// ���������
		short mask_used_imageid;				// �������
		short upgrade_used_imageid_0;			// ����ϵͳ����id
		short upgrade_used_imageid_1;			// ����ϵͳ����id
		short upgrade_used_imageid_2;			// ����ϵͳ����id
		short upgrade_used_imageid_3;			// ����ϵͳ����id
		short upgrade_used_imageid_4;			// ����ϵͳ����id
		short upgrade_used_imageid_5;			// ����ϵͳ����id
		short upgrade_used_imageid_6;			// ����ϵͳ����id
		short upgrade_used_imageid_7;			// ����ϵͳ����id
		short upgrade_used_imageid_8;			// ����ϵͳ����id
		short shenbing_used_img_id;				// �������
		short shenbing_texiao_id;				// �����Ч
		short baojia_used_img_id;				// ��������
		short baojia_texiao_id;					// ������Ч
	};

	static const int MAX_ROLE_NUM = 6;

	int count;
	__roleinfo role_info_list[MAX_ROLE_NUM];
};

#endif
