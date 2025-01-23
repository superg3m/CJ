#include <cj.h>

internal char* generateSpaces(int number_of_spaces) {
    char* ret = ckg_alloc((sizeof(char) * number_of_spaces) + 1);
    for (int i = 0; i < number_of_spaces; i++) {
        ret[i] = ' ';
    }
    
    return ret;
}

JSON* cj_create() {
    JSON* ret = (JSON*)ckg_alloc(sizeof(JSON));
    ret->type = CJ_TYPE_JSON;

    return ret;
}

JSON* cj_array_create() {
    JSON* ret = (JSON*)ckg_alloc(sizeof(JSON));
    ret->type = CJ_TYPE_ARRAY;

    return ret;
}

void MACRO_cj_push(JSON* root, char* key, JSON* value) {
    if (root->type != CJ_TYPE_JSON) {
        // I would normally assert, but Andy wouldn't like that so I guess error codes?
        // If its not of type JSON then you shouldn't be pushing to it
    }

    JSON_KEY_VALUE pair;
    pair.key = key;
    pair.value = value;

    ckg_vector_push(root->cj_json.key_value_pair_vector, pair);
}

void cj_array_push(JSON* root, JSON* value) {
    if (root->type != CJ_TYPE_ARRAY) {
        // I would normally assert, but Andy wouldn't like that so I guess error codes?
        // If its not of type JSON then you shouldn't be pushing to it
    }

    ckg_vector_push(root->cj_array.jsonVector, value);
}

JSON* JSON_INT(int value) {
    JSON* ret = cj_create();
    ret->type = CJ_TYPE_INT;
    ret->cj_int.value = value;

    return ret;
}

JSON* JSON_FLOAT(float value) {
    JSON* ret = cj_create();
    ret->type = CJ_TYPE_FLOAT;
    ret->cj_float.value = value;

    return ret;
}

JSON* JSON_STRING(char* value) {
    JSON* ret = cj_create();
    ret->type = CJ_TYPE_STRING;
    ret->cj_string.value = value;

    return ret;
}

JSON* JSON_BOOL(Boolean value) {
    JSON* ret = cj_create();
    ret->type = CJ_TYPE_BOOL;
    ret->cj_bool.value = value;

    return ret;
}

JSON* JSON_JSON(JSON* json) {
    return json;
}

JSON* JSON_NULL() {
    JSON* ret = cj_create();
    ret->type = CJ_TYPE_NULL;
    ret->cj_null.value = "null";

    return ret;
}

internal int depth = 0; 
#define INDENT "    "

void buffer_replace_with_specifer(char* buffer, u32 buffer_cap, char* string_to_remove) {
    char *fmt_specifier = "%s";
    u32 start = ckg_cstr_index_of(buffer, string_to_remove);
    u32 end = start + ckg_cstr_length(string_to_remove);

    for (u32 j = start; j < end; j++) {
        buffer[j] = ' ';
    }

    for (u32 j = start; j < start + 2; j++) {
        buffer[j] = fmt_specifier[j - start];
        ckg_memory_delete_index(buffer, buffer_cap, buffer_cap, start + 2);
        buffer[(buffer_cap - 1) - (start - j)] = '\0';
    }
}

char* cj_sprint(u64* allocation_size, char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    *allocation_size = vsnprintf(NULLPTR, 0, fmt, args) + 1;
    char* ret = ckg_alloc(*allocation_size);
    vsnprintf(ret, *allocation_size, fmt, args);
    va_end(args);

    return ret;
}

#define CJ_STR_FMT "<ST>"
char* generateSpecifer(int count, int num_json) {
    char* to_append = "<ST>\"<ST>\": <ST>"; // spaces, key, value

    int new_line_and_comma = 2;
    u64 allocation_size = (ckg_cstr_length(to_append) * count) + ((new_line_and_comma * count) - 1) + 1;
    char* buffer = ckg_alloc(allocation_size);

    for (int i = 0; i < count; i++) {
        ckg_cstr_append(buffer, allocation_size, to_append);
        if (i != (count - 1)) {
            ckg_cstr_append_char(buffer, allocation_size, ',');
            ckg_cstr_append_char(buffer, allocation_size, '\n');
        }
    }

    char* ret = cj_sprint(&allocation_size, "{\n%s\n%s}", buffer, generateSpaces(num_json));
    ckg_free(buffer);

    return ret;
}

// Date: January 22, 2025
// TODO(Jovanni): SWITCH THIS TO ALLOCATE WITH AN ARENA

// Date: January 22, 2025
// TODO(Jovanni): I need ckg_cstr_remove(buffer, buffer_size, to_remove)
char* json_to_string(JSON* root) {
    switch (root->type) {
        case CJ_TYPE_BOOL: {
            char* bool_string = root->cj_bool.value ? "true" : "false"; 
            return ckg_cstr_sprint("%s", bool_string);
        } break;

        case CJ_TYPE_INT: {
            return ckg_cstr_sprint("%d",  root->cj_int.value);
        } break;

        case CJ_TYPE_FLOAT: {
            return ckg_cstr_sprint("%f",  root->cj_float.value);
        } break;

        case CJ_TYPE_STRING: {
            return ckg_cstr_sprint("\"%s\"",  root->cj_string.value);    
        } break;
        
        case CJ_TYPE_NULL: {
            return ckg_cstr_sprint("%s",  root->cj_null.value);
        } break;

        case CJ_TYPE_ARRAY: {
            // I need some conditional logic here
            // return ckit_str_sprint("[%s]", root->cj_array.jsonVector);
        } break;

        case CJ_TYPE_JSON: {
            depth += 1;
            int num_json = (depth - 1) * ckg_cstr_length(INDENT);
            int num_key = depth * ckg_cstr_length(INDENT);

            int count = ckg_vector_count(root->cj_json.key_value_pair_vector);
            int new_line_and_comma = 2;

            char* create_format_string = generateSpecifer(count, num_json);
            u64 allocation_size = 0; 
        
            char* ret = cj_sprint(&allocation_size, create_format_string, generateSpaces(num_key));
            ckg_free(create_format_string);

            for (int i = 0; i < ckg_vector_count(root->cj_json.key_value_pair_vector); i++) {
                for (int pass = 0; pass < 3; pass++) {
                    buffer_replace_with_specifer(ret, allocation_size, CJ_STR_FMT);
                }

                char *key = root->cj_json.key_value_pair_vector[i].key;
                char *value = json_to_string(root->cj_json.key_value_pair_vector[i].value);

                allocation_size = snprintf(NULLPTR, 0, ret, generateSpaces(num_key), key, value) + 1;
                char* buffer = ckg_alloc(allocation_size);
                snprintf(buffer, allocation_size, ret, generateSpaces(num_key), key, value);
                ckg_free(ret);
                ret = buffer;
            }

            return ret;
        } break;

    }

    return NULLPTR;
}