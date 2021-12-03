#include <iostream>
#include <cstring>

#include "static_byte_array.hpp"

StaticByteArray::StaticByteArray() : has_read_length_(0), has_write_length_(0)
{
	memset(buffer_cache_, 0, sizeof(buffer_cache_));
}

StaticByteArray::~StaticByteArray()
{
}

#define READNUM(T) \
	if (has_read_length_ + (int)sizeof(T) > has_write_length_)\
	{\
		return 0;\
	}\
	\
	T read_val = 0;\
	read_val = this->ReadItemFromBuffer<T>();\
	return read_val;

signed char StaticByteArray::ReadInt8()
{
	READNUM(signed char);
}

unsigned char StaticByteArray::ReadUint8()
{
	READNUM(unsigned char);
}

short StaticByteArray::ReadInt16()
{
	READNUM(short);
}

unsigned short StaticByteArray::ReadUint16()
{
	READNUM(unsigned short);
}

int StaticByteArray::ReadInt32()
{
	READNUM(int);
}

unsigned int StaticByteArray::ReadUint32()
{
	READNUM(unsigned int);
}

float StaticByteArray::ReadFloat()
{
	READNUM(float);
}

long long StaticByteArray::ReadInt64()
{
	READNUM(long long);
}

unsigned long long StaticByteArray::ReadUint64()
{
	READNUM(unsigned long long);
}

double StaticByteArray::ReadDouble()
{
	READNUM(double);
}

std::string StaticByteArray::ReadString()
{
	int valid_buffer_len_ = has_write_length_ - has_read_length_;
	if (valid_buffer_len_ < STRING_LENGTH_BYTE_SIZE) //  // not enough length bytes
		return "";

	unsigned short str_len = this->ReadUint16();
	valid_buffer_len_ -= STRING_LENGTH_BYTE_SIZE;

	static char read_str[MAX_MSG_LENGTH + 1] = {0};

	if (str_len < 0 || str_len > valid_buffer_len_)
	{
		read_str[0] = 0;
		return read_str;
	}

	memcpy(read_str, buffer_cache_ + has_read_length_, str_len);
	read_str[str_len] = 0;

	has_read_length_ += str_len;

	return std::string(read_str);
}

const char * StaticByteArray::ReadByteData(unsigned int size)
{
	unsigned int valid_buffer_len_ = has_write_length_ - has_read_length_;
	if (size <= 0 || size > valid_buffer_len_)
        {
          return NULL;
        }

	//char *data = new char[size];
	//memcpy(data, buffer_cache_ + has_read_length_, size);
	char *return_data = static_cast<char *>(buffer_cache_ + has_read_length_);
	has_read_length_ += size;

	return return_data;
}

#define WRITENUM(VALUE) \
	if (MAX_MSG_LENGTH < has_write_length_ + (int)sizeof(VALUE))\
	{\
		return;\
	}\
	this->WriteItemToBuffer(VALUE);

void StaticByteArray::WriteInt8(const signed char &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteUint8(const unsigned char &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteInt16(const short &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteUint16(const unsigned short &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteInt32(const int &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteUint32(const unsigned int &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteFloat(const float &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteInt64(const long long &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteUint64(const unsigned long long &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteDouble(const double &value)
{
	WRITENUM(value);
}

void StaticByteArray::WriteString(const std::string &str)
{
	int str_len = (int)str.size();
	if (MAX_MSG_LENGTH < has_write_length_ + str_len + STRING_LENGTH_BYTE_SIZE)
	{
		return;
	}

	this->WriteUint16(str_len); // –¥»Î≥§∂»

	char *v_p_str = buffer_cache_ + has_write_length_;
	memcpy(v_p_str, str.c_str(), str_len);
	has_write_length_ += str_len;
}

void StaticByteArray::WriteWriteObject(const char *obj, int bytesLen)
{
	if (bytesLen <= 0)
	{
		return;
	}

	if (has_write_length_ + bytesLen > MAX_MSG_LENGTH)
	{
		return;
	}

	char *v_p_str = buffer_cache_ + has_write_length_;
	memcpy(v_p_str, obj, bytesLen);
	has_write_length_ += bytesLen;
}

void StaticByteArray::ReadFromByteArray(StaticByteArray *other, int bytesLen)
{
	unsigned int read_size = bytesLen;
	if (read_size <= 0 || read_size > other->BytesAvailable())
		return;

	this->WriteWriteObject(other->ReadByteData(read_size), read_size);
}

void StaticByteArray::ShowAllBytes()
{
	if (has_read_length_ > has_write_length_)
		return;

	for (int i = has_read_length_; i < has_write_length_; i++)
		std::cout << "bytes[" << i << "] = " << char(buffer_cache_[i]) << "(" << int(buffer_cache_[i]) << ")" << std::endl;
}

void StaticByteArray::Clear()
{
	memset(buffer_cache_, 0, sizeof(buffer_cache_));
	has_read_length_ = 0;
	has_write_length_ = 0;
}

