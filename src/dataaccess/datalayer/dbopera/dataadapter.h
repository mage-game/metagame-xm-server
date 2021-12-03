
#ifndef DATAADAPTER_H
#define DATAADAPTER_H

#include <string>
#include <vector>

/*
	ʹ��ע�⣺
	1��MataTableDef����ȷ����ʹ���߱�֤��
	2��MataTableDef::m_id_index ����Ϊlong long �����������ݿ���Ҳ����Ϊ��Ӧ����
	3��DataArea ����� һ�� MataTableDef ��Ӧ
*/

enum 
{
	DATYPE_CHAR,
	DATYPE_INT,
	DATYPE_LONG64,
	DATYPE_DOUBLE,
	DATYPE_STRING,
	DATYPE_TIME,
};



#pragma pack(push) 
#pragma pack(4)

struct DataArea
{
	unsigned int	length;
	union {
		char		vchar;
		int			vint;
		long long	vint64;
		double		vdouble;
		char		*vcharp;
		long long	vtime;
	};
};


#pragma pack(pop)


class MataTableDef;

class DataAdapter
{
	friend class MataTableDef;
protected:
	DataAdapter();
	void Init(const MataTableDef *table);
public:
	DataAdapter(const DataAdapter &_r);
	~DataAdapter();

	// Ϊ��Ҫ�����ռ���ֶδ����ռ䣬��Ҫ���ڴ����ݿ��ж�ȡ
	void Malloc();
	void Free();

	typedef std::vector<DataArea> DataAreaList;
	DataAreaList		m_data_area;
	const MataTableDef	*m_table;
};
typedef std::vector<DataAdapter> DataAdapterList;

struct TableInitArg;

class MataTableDef
{
public:
	struct MataData
	{
		int				type;
		std::string		name;
		unsigned int	length;
	};

	MataTableDef();
	~MataTableDef();

	bool Init(const TableInitArg &arg);
	const DataAdapter& GetPrototype()const { return m_data_prototype; }

	typedef std::vector<MataData> MataDataList;
	int				m_table_id;
	std::string		m_table_name;
	MataDataList	m_mata_data;
	int				m_key_id_index;
	DataAdapter		m_data_prototype;

	typedef std::vector<int> MallocAreaList;
	MallocAreaList	m_malloc_area_list;	// ��¼��Ҫ��������ռ���ֶ�
};

struct TableInitArg
{
	int				table_id;
	std::string		table_name;
	MataTableDef::MataDataList	mata_data;
	int				key_id_index;
};

#endif







