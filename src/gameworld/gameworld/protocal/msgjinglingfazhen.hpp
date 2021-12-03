#ifndef __MSG_JINGLINGFAZHEN_H__
#define __MSG_JINGLINGFAZHEN_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/jinglingfazhendef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCJinglingFazhenInfo				// 6925 ���鷨����Ϣ
	{
	public:
		SCJinglingFazhenInfo();
		MessageHeader header;

		short reserve;															
		short grade;																// ��
		short used_imageid;															// ʹ�õ�����
		short reserve_2;
		int active_image_flag;														// ����������б�  & (1 << imageid) != 0���򼤻�imageid
		int grade_bless_val;														// ����ף��ֵ
		int active_special_image_flag;												// ��������������б�

		char special_img_grade_list[JinglingFazhenParam::MAX_SPECIAL_IMAGE_ID + 1];	// �����������
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CSUseJinglingFazhenImage			// 6901 ����ʹ������
	{
	public:
		CSUseJinglingFazhenImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSJinglingFazhenGetInfo				// 6902 �����鷨����Ϣ
	{
	public:
		CSJinglingFazhenGetInfo();	
		MessageHeader header;
	};

	class CSJinglingFazhenUpStarLevel			// 6900 ���Ǽ�����
	{
	public:
		CSJinglingFazhenUpStarLevel();
		MessageHeader header;

		int is_auto_buy;
	};

	class CSJinglingFazhenSpecialImgUpgrade			// 6903 ���鷨�������������
	{
	public:
		CSJinglingFazhenSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

}

#pragma pack(pop)

#endif // 

