
#ifndef JSON_TYPED_H
#define JSON_TYPED_H

#include <ccan/json/json.h>

struct JsonNode * json_typed_object(const char *name, struct JsonNode *object);
struct JsonNode * json_typed_number(const char *name, double value);
struct JsonNode * json_typed_string(const char *name, const char *value);
struct JsonNode * json_typed_boolean(const char *name, bool value);

#endif