#include <cj.h>

JSON* cj_create() {
    JSON* ret = (JSON*)malloc(sizeof(JSON));
    ret->type = CJ_TYPE_JSON;

    return ret;
}

JSON* cj_array_create() {
    JSON* ret = (JSON*)malloc(sizeof(JSON));
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
            // I need some conditional logic here
            // return ckit_str_sprint("[%s]", root->cj_array.jsonVector);
        } break;

    }

    return NULLPTR;
}

char* cj_str_alloc(JSON* root) {
    return NULLPTR;
}