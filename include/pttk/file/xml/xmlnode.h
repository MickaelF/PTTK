#pragma once
#include <map>
#include <vector>
#include <string>

class XmlNode 
{
public: 
    explicit XmlNode(); 
    std::vector<std::string> attributeKeys() const;
    std::string attributeByKey(const std::string& key) const; 

    void addAttribute(const std::string& key, const std::string& value);
    void addChild(XmlNode node);

private: 
    std::map<std::string, std::string> m_attributes; 
    std::vector<XmlNode> m_children; 
};