#include <PTTK/file/xml/xmlnode.h>
#include <algorithm>

XmlNode::XmlNode() {}

std::vector<std::string> XmlNode::attributeKeys() const 
{
    std::vector<std::string> keys; 
    std::for_each(m_attributes.cbegin(), m_attributes.cend(), [&](const auto& pair) {
        keys.push_back(pair.first);
    });
    return keys;
}

std::string XmlNode::attributeByKey(const std::string& key) const 
{
    auto value { m_attributes.find(key) };
    if (value == m_attributes.cend())
        return std::string{};
    else
        return value->second;
}

void XmlNode::addAttribute(const std::string& key, const std::string& value) 
{
    m_attributes.insert_or_assign(key, value);
}

void XmlNode::addChild(XmlNode node) 
{
    m_children.push_back(node);
}