#include "namecreateconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"

NameCreateConfig::NameCreateConfig()
{

}

NameCreateConfig::~NameCreateConfig()
{

}

bool NameCreateConfig::Init(const std::string &configname, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "NameCreateConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("random_name");
		if (root_element.empty())
		{
			*err = configname + ": xml node error in root->degree";
			return false;
		}

		iRet = this->InitNameCreateConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s:InitNameCreateConfig fail %d\n", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int NameCreateConfig::InitNameCreateConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}
	
	// Å®
	std::string female_first;
	if (!PugiGetSubNodeValue(data_element, "female_first", female_first))
	{
		return -1;
	}
	F_STRSPLIT(m_female_first_vec, female_first, ",");
	if (m_female_first_vec.size() <= 0) return -2;
	
	std::string female_last;
	if (!PugiGetSubNodeValue(data_element, "female_last", female_last))
	{
		return -3;
	}
	F_STRSPLIT(m_female_last_vec, female_last, ",");
	if (m_female_last_vec.size() <= 0) return -4;

	// ÄÐ
	std::string male_first;
	if (!PugiGetSubNodeValue(data_element, "male_first", male_first))
	{
		return -5;
	}
	F_STRSPLIT(m_male_first_vec, male_first, ",");
	if (m_male_first_vec.size() <= 0) return -6;

	std::string male_last;
	if (!PugiGetSubNodeValue(data_element, "male_last", male_last))
	{
		return -7;
	}
	F_STRSPLIT(m_male_last_vec, male_last, ",");
	if (m_male_last_vec.size() <= 0) return -8;

	return 0;
}

bool NameCreateConfig::GetRandName(GameName name)
{
	if (m_female_first_vec.size() <= 0 || m_female_last_vec.size() <= 0 || m_male_first_vec.size() <= 0 || m_male_last_vec.size() <= 0)
	{
		return false;
	}

	if (RandomNum(2) > 0)
	{
		std::string first = m_female_first_vec[RandomNum(m_female_first_vec.size())];
		std::string last = m_female_last_vec[RandomNum(m_female_last_vec.size())];
		SNPRINTF(name, sizeof(GameName), "%s%s", first.c_str(), last.c_str());
	}
	else
	{
		std::string first = m_male_first_vec[RandomNum(m_male_first_vec.size())];
		std::string last = m_male_last_vec[RandomNum(m_male_last_vec.size())];
		SNPRINTF(name, sizeof(GameName), "%s%s", first.c_str(), last.c_str());
	}
	return true;
}

bool NameCreateConfig::GetRandName(GameName name, int rand_index, bool is_female)
{
	if (m_female_first_vec.size() <= 0 || m_female_last_vec.size() <= 0 || m_male_first_vec.size() <= 0 || m_male_last_vec.size() <= 0)
	{
		return false;
	}

	if (is_female)
	{
		std::string first = m_female_first_vec[rand_index % m_female_first_vec.size()];
		std::string last = m_female_last_vec[rand_index % m_female_last_vec.size()];
		SNPRINTF(name, sizeof(GameName), "%s%s", first.c_str(), last.c_str());
	}
	else
	{
		std::string first = m_male_first_vec[rand_index % m_male_first_vec.size()];
		std::string last = m_male_last_vec[rand_index % m_male_last_vec.size()];
		SNPRINTF(name, sizeof(GameName), "%s%s", first.c_str(), last.c_str());
	}
	return true;
}