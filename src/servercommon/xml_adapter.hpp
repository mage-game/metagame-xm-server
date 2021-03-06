#ifndef __XML_ADAPTER_HPP__
#define __XML_ADAPTER_HPP__

/*
// 使用这一段宏，启用TinyXml作为读取库
#include "common/tinyxml/tinyxml.h"
#define YY_XML_NODE TiXmlElement
#define YY_XML_NODE_PTR YY_XML_NODE *
#define YY_XML_NODE_IS_EMPTY(NODE) (NULL == NODE)
#define YY_XML_FUNCTION_NODE_TYPE YY_XML_NODE_PTR

#define YY_XML_FIRST_CHILD_ELEMENT(NODE, NODE_NAME) NODE->FirstChildElement(NODE_NAME);
#define YY_XML_GET_SUB_NODE_VALUE(NODE, ELEMENT_NAME, VALUE) GetSubNodeValue(NODE, ELEMENT_NAME, VALUE)
#define YY_XML_GET_NODE_VALUE(NODE, VALUE) GetNodeValue(NODE, VALUE)
#define YY_XML_NEXT_SIBLING(NODE) NODE->NextSiblingElement()

#define YY_XML_CONFIG_PRE_LOAD PRE_LOAD_CONFIG
#define YY_XML_LOAD_CONFIG(ELEMENT_NAME, INIT_FUNC) LOAD_CONFIG(ELEMENT_NAME, INIT_FUNC)
//*/

//*
// 使用这一段宏，启用PugiXml作为读取库
#include "pugixml/pugixml_adapter.hpp"
#define YY_XML_NODE PUGI_XML_NODE
#define YY_XML_NODE_CONST_REF const YY_XML_NODE &
#define YY_XML_NODE_IS_EMPTY(NODE) PUGI_XML_NODE_IS_EMPTY(NODE)
#define YY_XML_FUNCTION_NODE_TYPE YY_XML_NODE_CONST_REF

#define YY_XML_FIRST_CHILD_ELEMENT(NODE, NODE_NAME) PUGI_XML_FIRST_CHILD_ELEMENT(NODE, NODE_NAME)
#define YY_XML_GET_SUB_NODE_VALUE(NODE, ELEMENT_NAME, VALUE) PugiGetSubNodeValue(NODE, ELEMENT_NAME, VALUE) 
#define YY_XML_GET_NODE_VALUE(NODE, VALUE) PugiGetNodeValue(NODE, VALUE) 
#define YY_XML_NEXT_SIBLING(NODE) PUGI_XML_NEXT_SIBLING(NODE)

#define YY_XML_CONFIG_PRE_LOAD PUGI_XML_CONFIG_PRE_LOAD
#define YY_XML_LOAD_CONFIG(element_name, init_func) PUGI_XML_LOAD_CONFIG(element_name, init_func)
//*/

#endif // __XML_ADAPTER_HPP__