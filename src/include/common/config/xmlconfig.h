#ifndef XMLCONFIG_H
#define XMLCONFIG_H

#include "config.h"

/*
	XML Config类，继承自Config，基于tinyxml。提供XML的文件访问和操作。
*/

class TiXmlNode;
class TiXmlDocument;

class XMLConfig : public IConfig
{
public:
	XMLConfig();
	virtual ~XMLConfig();
	bool read(const char* file);
	bool write(const char* file);
protected:
	virtual void clean();
	virtual bool up();
	virtual bool down();
	virtual bool attribute();
	virtual bool value(const std::string& v, bool read_only);
	virtual bool list(std::vector<std::string>* element_list, std::vector<std::string>* attribute_list);
	virtual bool get(std::string* str)const;
	virtual bool set(const std::string& str);

private:

	int m_op;
	std::string m_current_attribute_name;
	TiXmlNode* m_current_node;
	TiXmlDocument *m_document;
};


#endif // xmlconfig_h__
