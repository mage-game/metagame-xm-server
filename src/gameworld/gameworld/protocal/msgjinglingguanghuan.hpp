#ifndef __MSG_JINGLINGGUANGHUAN_H__
#define __MSG_JINGLINGGUANGHUAN_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/jinglingguanghuandef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCJinglingGuanghuanInfo				// 6875 ����⻷��Ϣ
	{
	public:
		SCJinglingGuanghuanInfo();
		MessageHeader header;

		short reserve;															
		short grade;																// ��
		short used_imageid;															// ʹ�õ�����
		short reserve_2;
		int active_image_flag;														// ����������б�  & (1 << imageid) != 0���򼤻�imageid
		int grade_bless_val;														// ����ף��ֵ
		int active_special_image_flag;												// ��������������б�

		char special_img_grade_list[JinglingGuanghuanParam::MAX_SPECIAL_IMAGE_ID + 1];	// �����������
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CSUseJinglingGuanghuanImage			// 6851 ����ʹ������
	{
	public:
		CSUseJinglingGuanghuanImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSJinglingGuanghuanGetInfo				// 6852 ������⻷��Ϣ
	{
	public:
		CSJinglingGuanghuanGetInfo();	
		MessageHeader header;
	};

	class CSJinglingGuanghuanUpStarLevel			// 6850 ���Ǽ�����
	{
	public:
		CSJinglingGuanghuanUpStarLevel();
		MessageHeader header;

		int is_auto_buy;
	};

	class CSJinglingGuanghuanSpecialImgUpgrade			// 6853 ����⻷�����������
	{
	public:
		CSJinglingGuanghuanSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

}

#pragma pack(pop)

#endif // 

