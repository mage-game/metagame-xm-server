#ifndef __NAME_FILTER_H__
#define __NAME_FILTER_H__

#include <map>
#include <string>

static const int MAX_SENSITIVEWORD_SIZE = 127;				// 敏感词最大字节数
static const int MAX_NUM_SIZE = 3;							// 名字最多出现数字个数

class NameNode
{
public:
	friend class NameFilter;

	NameNode();
	~NameNode();

private:
	typedef std::map<char, NameNode*> NodeMap;
	typedef std::map<char, NameNode*>::iterator NodeMapIt;

	NodeMap m_childs;
};

class NameFilter
{
public:
	NameFilter();
	~NameFilter();

	static NameFilter & Instance();

	bool Init(const char *filename, std::string *err);

	void AddSensitiveWord(int max_len, const char *word);
	bool HasSensitiveWord(int max_len, const char *input_word);

	bool HasNumberWord(int max_len, const char *name);

private:
	NameNode *m_root;
};

#endif

