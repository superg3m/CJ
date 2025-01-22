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

void cj_push(JSON* root, char* key, JSON* value) {
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

JSON* JSON_FLOAT(int value) {
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

JSON* JSON_NULL() {
    JSON* ret = cj_create();
    ret->type = CJ_TYPE_NULL;
    ret->cj_null.value = "null";

    return ret;
}

internal int depth = 0; 
#define INDENT "    "

// Date: January 22, 2025
// TODO(Jovanni): SWITCH THIS TO ALLOCATE WITH AN ARENA
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

            #define CJ_STR_FMT "STR|"
            char *to_append = "STR|\"STR|\": STR|"; // spaces, key, value
            char *fmt_specifier = "%s";

            int num_json = (depth - 1) * ckg_cstr_length(INDENT);
            int num_key = depth * ckg_cstr_length(INDENT);

            int count = ckg_vector_count(root->cj_json.key_value_pair_vector);
            int new_line_and_comma = 2;

            u64 allocation_size = (ckg_cstr_length(to_append) * count) + ((new_line_and_comma * count) - 1) + 1;
            char *create_format_string = ckg_alloc(allocation_size);

            for (int i = 0; i < count; i++) {
                ckg_cstr_append(create_format_string, allocation_size, to_append);
                if (i != (count - 1)) {
                    ckg_cstr_append_char(create_format_string, allocation_size, ',');
                    ckg_cstr_append_char(create_format_string, allocation_size, '\n');
                }
            }

            allocation_size = snprintf(NULLPTR, 0, "{\n%s\n%s}", create_format_string, generateSpaces(num_json)) + 1;
            char *buffer = ckg_alloc(allocation_size);
            snprintf(buffer, allocation_size, "{\n%s\n%s}", create_format_string, generateSpaces(num_json));

            char *ret = ckg_alloc(allocation_size);
            snprintf(ret, allocation_size, buffer, generateSpaces(num_key));
            ckg_free(buffer);

            for (int i = 0; i < ckg_vector_count(root->cj_json.key_value_pair_vector); i++) {
                for (int pass = 0; pass < 3; pass++) {
                    u32 start = ckg_cstr_index_of(ret, CJ_STR_FMT);
                    u32 end = start + ckg_cstr_length(CJ_STR_FMT);

                    for (u32 j = start; j < end; j++) {
                        ret[j] = ' ';
                    }

                    for (u32 j = start; j < start + 2; j++) {
                        ret[j] = fmt_specifier[j - start];
                        ckg_memory_delete_index(ret, allocation_size - 1, allocation_size - 1, start + 2);
                    }

                    ret[allocation_size - 3] = '\0';
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