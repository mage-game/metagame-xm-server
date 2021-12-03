#ifndef __MSG_XIANNVSHOUHU_H__
#define __MSG_XIANNVSHOUHU_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/xiannvshouhudef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCXiannvShouhuInfo				// 6825 ��Ů�ػ���Ϣ
	{
	public:
		SCXiannvShouhuInfo();
		MessageHeader header;

		short star_level;															// �Ǽ�
		short grade;																// ��
		short used_imageid;															// ʹ�õ�����
		short reserve;
		int active_image_flag;														// ����������б�  & (1 << imageid) != 0���򼤻�imageid
		int grade_bless_val;														// ����ף��ֵ
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CSUseXiannvShouhuImage			// 6801 ����ʹ������
	{
	public:
		CSUseXiannvShouhuImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSXiannvShouhuGetInfo				// 6802 ������Ů�ػ���Ϣ
	{
	public:
		CSXiannvShouhuGetInfo();	
		MessageHeader header;
	};

	class CSXiannvShouhuUpStarLevel			// 6800 ���Ǽ�����
	{
	public:
		CSXiannvShouhuUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;
	};

}

#pragma pack(pop)

#endif // 

