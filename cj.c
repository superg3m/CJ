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

internal int depth = 1; 
#define INDENT "  "

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
            return ckg_cstr_sprint("%s",  root->cj_string.value);    
        } break;
        
        case CJ_TYPE_NULL: {
            return ckg_cstr_sprint("%s",  root->cj_null.value);
        } break;

        case CJ_TYPE_ARRAY: {
            // I need some conditional logic here
            // return ckit_str_sprint("[%s]", root->cj_array.jsonVector);
        } break;


        case CJ_TYPE_JSON: {
            char* to_append = "%s%s: %s"; // spaces, key, value
            
            //int count = ckg_vector_count(root->cj_json.key_value_pair_vector);
            int count = 1;
            int new_line_and_comma = 2;
            u64 allocation_size = (ckg_cstr_length(to_append) * count) + ((new_line_and_comma * count) - 1) + 1;
            char* create_formate_string = ckg_alloc(allocation_size);

            for (int i = 0; i < count; i++) {
                if (i != (count - 1)) {
                    ckg_cstr_append(create_formate_string, allocation_size, to_append);
                    ckg_cstr_append_char(create_formate_string, allocation_size, '\n');
                    ckg_cstr_append_char(create_formate_string, allocation_size, ',');
                } else {
                    ckg_cstr_append(create_formate_string, allocation_size, to_append);
                }
            }

            char* ret = ckg_cstr_sprint("{\n%s%s\n}", create_formate_string, generateSpaces((depth - 1) * ckg_cstr_length(INDENT)));

            for (int i = 0; i < ckg_vector_count(root->cj_json.key_value_pair_vector); i++) {
                char* key = root->cj_json.key_value_pair_vector->key;
                char* value = json_to_string(root->cj_json.key_value_pair_vector->value);
                ret = ckg_cstr_sprint(ret, generateSpaces(depth * ckg_cstr_length(INDENT)), key, value);
                break;
            }

            depth += 1;
            return ret;
        } break;

    }

    return NULLPTR;
}