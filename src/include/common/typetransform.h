
#ifndef TYPETRANSFORM_H
#define TYPETRANSFORM_H
#include <string>


class TypeTransform  
{
	static const int MAX_TRANSFORM_BUFF_LEN = 24;

public:
	TypeTransform(int v);
	TypeTransform(float v);
	TypeTransform(double v);
	TypeTransform(unsigned int v);
	TypeTransform(short v);
	TypeTransform(bool v);
	TypeTransform(long long v);
	//	CTypeTransform(char * szChar);

	virtual ~TypeTransform();
	const char* ToString();

private:
	char buf[MAX_TRANSFORM_BUFF_LEN];
};

#define ToStr(X) TypeTransform(X).ToString()

#endif



