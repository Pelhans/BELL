#pragma once

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <vector>
#include <string>

namespace bell {

class XmlNode {
public:
    XmlNode();
    XmlNode(const xmlNodePtr node);

    bool isValid() const;
    std::string getText() const;
    const XmlNode operator[](const std::string& item_str) const;
    const XmlNode findChild(const std::string& item_str) const;
    const std::vector<XmlNode> findAllChildren(const std::string& item_str) const;

private:
    xmlNodePtr m_node;
};

class XmlDoc {
public:
    const XmlNode parser(const std::string& filename);

public:
    xmlDocPtr m_doc;
};

void GET_XML_CONF_INT(const XmlNode& root, const std::string field_name, int default_val, int* dst);
void GET_XML_CONF_DOUBLE(const XmlNode& root, const std::string field_name, double default_val, double* dst);
void GET_XML_CONF_STRING(const XmlNode& root, const std::string field_name, const std::string& default_val, std::string* dst);

const std::vector<XmlNode> GET_XML_CONF_LIST(const XmlNode& root, const std::string& field_name);

}