#include <json_typed.h>

struct JsonNode * json_typed_object(const char *name, struct JsonNode *object) {
    struct JsonNode *obj = json_mkobject();
    json_append_member(obj, "type", json_mkstring(name));
    json_append_member(obj, "value", object);
    return obj;
}

struct JsonNode * json_typed_number(const char *name, double value) {
    struct JsonNode *obj = json_mkobject();
    json_append_member(obj, "type", json_mkstring(name));
    json_append_member(obj, "value", json_mknumber(value));
    return obj;
}

struct JsonNode * json_typed_string(const char *name, const char *value) {
    struct JsonNode *obj = json_mkobject();
    json_append_member(obj, "type", json_mkstring(name));
    json_append_member(obj, "value", json_mkstring(value));
    return obj;
}

struct JsonNode * json_typed_boolean(const char *name, bool value) {
    struct JsonNode *obj = json_mkobject();
    json_append_member(obj, "type", json_mkstring(name));
    json_append_member(obj, "value", json_mkbool(value));
    return obj;
}