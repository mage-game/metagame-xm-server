
#ifndef MSGTITLE_HPP
#define MSGTITLE_HPP

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/titledef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	//	C --> S

	class CSGetTitleList
	{
	public:
		CSGetTitleList();
		MessageHeader		header;
	};

	class CSUseTitle
	{
	public:
		CSUseTitle();
		MessageHeader		header;

		unsigned short		use_title_list[MAX_USE_TITLE_COUNT];
		short				reserve_sh;
	};

	class CSUpgradeTitle					// 3652 ����ƺŽ���
	{
	public:
		CSUpgradeTitle();
		MessageHeader		header;

		unsigned short		title_id;		// �ƺ�id
		short				reserve_sh;
	};

	//	S --> C

	class SCTitleList						// 3600 �������гƺ��б�
	{
	public:
		SCTitleList();
		MessageHeader		header;

		struct TitleUpgrade
		{
			TitleUpgrade(): title_id(0), grade(0), reserve_ch(0)
			{}

			short title_id;		// �ƺ�id
			char grade;			// �ƺŵȼ�
			char reserve_ch;
		};

		struct TitleInfo
		{
			TitleInfo(): title_id(0), reserve(0), expired_time(0)
			{}
			unsigned short	title_id;
			short reserve;
			unsigned int expired_time;		// ����ʱ���
		};

		int					title_count;
		TitleUpgrade		title_upgrade_list[MAX_TITLE_COUNT_TO_SAVE];	// �ƺŽ�����Ϣ
		TitleInfo			title_id_list[MAX_OWN_TITLE_COUNT];				// �ƺ���Ϣ
	};

	class SCUsedTitleList
	{
	public:
		SCUsedTitleList();
		MessageHeader		header;

		int					use_jingling_title;
	
		short				use_title_count;
		unsigned short		use_title_list[MAX_USE_TITLE_COUNT];
	};

	class SCRoleUsedTitleChange
	{
	public:
		SCRoleUsedTitleChange();
		MessageHeader		header;

		ObjID				obj_id;
		unsigned short		use_jingling_title;
		short				use_title_count;
		unsigned short		used_title_list[MAX_USE_TITLE_COUNT];
	};
}

#pragma pack(pop)

#endif

