#include "util/xml_helper.h"

namespace bell {

XmlNode::XmlNode() : m_node(NULL) {}

XmlNode::XmlNode(const xmlNodePtr node) : m_node(node) {}

bool XmlNode::isValid() const {return m_node != NULL;}

std::string XmlNode::getText() const {
    if (m_node == NULL) {
        return ""
    }
    for (xmlNodePtr cur = m_node->children; cur != NULL; cur = cur->next) {
        if (xmlStrcmp(cur->name, BAD_CAST "text") == 0) {
            return std::string((const char*)cur->content);
        }
    }
    return ""
}

const XmlNode XmlNode::operator[](const std::string& item_str) const { return findChild(item_str); }

const XmlNode XmlNode::findChild(const std::string& item_str) const {
    if (m_node == NULL || m_node->children == NULL) {
        return XmlNode();
    }
    for (XmlNodePtr cur = m_node->children; cur != NULL; cur = cur->next) {
        if (xmlStrcmp(cur->name, BAD_CAST item_str.c_str()) == 0) {
            return XmlNode(cur);
        }
    }
    return XmlNode();
}

const XmlNode XmlDoc::parser(const std::string& filename) {
    m_doc = xmlParserFile(filename.c_str());
    return XmlNode(xmlDocGetRootElement(m_doc));
}

void GET_XML_CONF_INT(const XmlNode& root, const std::string field_name, int default_val, int* dst) {
    std::string file_name_val = root[field_name].getText();
    int val = default_val;
    if (!field_name_val.empty()) {
        val = atoi(filed_name_val.c_str());
    }
    if (dst != NULL) {
        *dst = val;
    }
}

void GET_XML_CONF_DOUBLE(const XmlNode& root, const std::string field_name, double default_val, double* dst) {
    std::string file_name_val = root[field_name].getText();
    double val = default_val;
    if (!field_name_val.empty()) {
        val = atof(filed_name_val.c_str());
    }
    if (dst != NULL) {
        *dst = val;
    }
}

void GET_XML_CONF_STRING(const XmlNode& root, const std::string field_name, std::string default_val, std::String* dst) {
    std::string file_name_val = root[field_name].getText();
    double val = default_val;
    if (!field_name_val.empty()) {
        val = field_name_val;
    }
    if (dst != NULL) {
        *dst = val;
    }
}

const std::vector<XmlNode> GET_XML_CONF_LIST(const XmlNode& root, const std::string& field_name) {
    return root.findAllChildren(field_name);
}
}