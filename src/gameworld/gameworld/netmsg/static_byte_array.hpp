#pragma once

#include <string>

static const int MAX_MSG_LENGTH = 1024 * 256;

/*
 * �̶��ڴ泤�ȣ���ȡ��д�붼�Ǵ�ͷ��ʼ��������
 * 1 ��д�ٶȿ죬����Ҫ�ظ������ڴ�
 * 2 �Ѷ���д�Ļ���������ѭ�����ã��������ֻ��һ�ζ�д��Ȼ���Ҫclearһ�£��ָ����û�����
 */
class StaticByteArray
{
	static const int STRING_LENGTH_BYTE_SIZE = 2; // �ַ��������ֶ�ռ���ֽ���

public:
	StaticByteArray();
	virtual ~StaticByteArray();

	virtual unsigned int BytesAvailable() { return has_write_length_ - has_read_length_; }

	signed char ReadInt8();
	unsigned char ReadUint8();

	short ReadInt16();
	unsigned short ReadUint16();

	int ReadInt32();
	unsigned int ReadUint32();
	float ReadFloat();

	long long ReadInt64();
	unsigned long long ReadUint64();
	double ReadDouble();

	std::string ReadString();
	const char * ReadByteData(unsigned int size);

	void WriteInt8(const signed char &value);
	void WriteUint8(const unsigned char &value);

	void WriteInt16(const short &value);
	void WriteUint16(const unsigned short &value);

	void WriteInt32(const int &value);
	void WriteUint32(const unsigned int &value);
	void WriteFloat(const float &value);

	void WriteInt64(const long long &value);
	void WriteUint64(const unsigned long long &value);
	void WriteDouble(const double &value);

	void WriteString(const std::string &value);
	virtual void WriteWriteObject(const char *obj, int bytesLen);

	virtual void ReadFromByteArray(StaticByteArray *other, int bytesLen = 0);
	virtual void ShowAllBytes();

	virtual void Clear();

	StaticByteArray & operator+(StaticByteArray &other)
	{
		ReadFromByteArray(&other, other.BytesAvailable());
		return *this;
	}

	StaticByteArray & operator=(StaticByteArray &other)
	{
		if (this == &other)
			return *this;

		Clear();
		ReadFromByteArray(&other, other.BytesAvailable());
		return *this;
	}

	template <typename T>
	void WriteItemToBuffer(T val)
	{
		char *src = (char*)&val;
		char *dst = (buffer_cache_ + has_write_length_);
		for (size_t i = 0; i < sizeof(T); ++i)
		{
			dst[i] = src[i];
		}
		has_write_length_ += sizeof(T);
	}

	template <typename T>
	T ReadItemFromBuffer()
	{
		T val;
		char *src = (char*)(buffer_cache_ + has_read_length_);
		char *dst = (char*)(&val);
		for (size_t i = 0; i < sizeof(T); ++i)
		{
			dst[i] = src[i];
		}
		has_read_length_ += sizeof(T);
		return val;
	}

private:
	char buffer_cache_[MAX_MSG_LENGTH];
	int has_read_length_;
	int has_write_length_;
};
