#pragma once

#include "base_signal_output.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

using std::string;

namespace bell {
namespace types {

typedef struct _EntityNode__isset {
    _EntityNode__isset() : type_name(false), score(false), entity_id(false), attr(false) {}
    bool type_name:1;
    bool score:1;
    bool entity_id:1;
    bool attr:1;
} _EntityNode__isset;

class EntityNode {
public:
    EntityNode(const EntityNode&);
    EntityNode& operator=(const EntityNode&);
    EntityNode() : type(0), type_name(0), score(0), entity_id() {}

    virtual ~EntityNode() throw();
    int32_t type;
    std::string type_name;
    double score;
    std::string entity_id;
    std::map<std::string, std::string> attr;

    _EntityNode__isset __isset;

    bool operator == (const EntityNode& rhs) const {
        if (!(type == rhs.type)) {
            return false;
        }
        if (__isset.type_name != rhs.__isset.type_name) {
            return false;
        } else if (__isset.type_name && !(type_name == rhs.type_name)) {
            return false;
        }
        if (__isset.score != rhs.__isset.score) {
            return false;
        } else if (__isset.score && !(score == rhs.score)) {
            return false;
        }
        if (__isset.entity_id != rhs.__isset.entity_id) {
            return false;
        } else if (__isset.entity_id && !(entity_id == rhs.entity_id)) {
            return false;
        }
        if (__isset.attr != rhs.__isset.attr) {
            return false;
        } else if (__isset.attr && !(attr == rhs.attr)) {
            return false;
        }
        return true;
    }

    bool operator != (const EntityNode& rhs) const {
        return !(*this == rhs);
    }
};

typedef struct _Entity__isset {
    _Entity__isset() : ex_data(false) {}
    bool ex_data:1;
} _Entity__isset;

class Entity {
public:
    Entity(const Entity&);
    Entity& operator=(const Entity&);
    Entity() : text(0), term_begin(0), term_end() {}

    virtual ~Entity() throw();
    std::string text;
    int32_t term_begin;
    int32_t term_end;
    std::vector<EntityNode> entity_list;
    std::map<std::string, std::string> ex_data;

    _Entity__isset __isset;

    bool operator == (const Entity& rhs) const {
        if (!(text == rhs.text)) {
            return false;
        }
        if (!(term_begin == rhs.term_begin)) {
            return false;
        }
        if (!(term_begin == rhs.term_begin)) {
            return false;
        }
        if (!(entity_list == rhs.entity_list)) {
            return false;
        }
        if (__isset.ex_data != rhs.__isset.ex_data) {
            return false;
        } else if (__isset.ex_data && !(ex_data == rhs.ex_data)) {
            return false;
        }
        return true;
    }

    bool operator != (const Entity& rhs) const {
        return !(*this == rhs);
    }
};

typedef struct _NerPath__isset {
    _NerPath__isset() : score(false), ex_data(false), status(false) {}
    bool score:1;
    bool ex_data:1;
    bool status:1;
} NerPath__isset;

class NerPath {
public:
    NerPath(const NerPath&);
    NerPath& operator=(const NerPath&);
    NerPath() : score(0), status(0) {}

    virtual ~NerPath() throw();
    std::vector<Entity> ner_list;
    double score;
    std::map<std::string, std::string> ex_data;
    int32_t status;

    _NerPath__isset __isset;

    bool operator == (const NerPath& rhs) const {
        if (!(ner_list == rhs.ner_list)) {
            return false;
        }
        if (__isset.score != rhs.__isset.score) {
            return false;
        } else if (__isset.score && !(score == rhs.score)) {
            return false;
        }
        if (__isset.ex_data != rhs.__isset.ex_data) {
            return false;
        } else if (__isset.ex_data && !(ex_data == rhs.ex_data)) {
            return false;
        }
        if (__isset.status != rhs.__isset.status) {
            return false;
        } else if (__isset.status && !(status == rhs.status)) {
            return false;
        }
        return true;
    }

    bool operator != (const NerPath& rhs) const {
        return !(*this == rhs);
    }
};
}
}