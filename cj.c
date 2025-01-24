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

void MACRO_cj_array_push(JSON* root, JSON* value) {
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

internal char* global_intend = "    ";
void cj_set_context_indent(char* new_indent) {
    global_intend = new_indent;
}

char* cj_sprint(CKG_Arena* arena, u64* allocation_size, char* fmt, ...) {
    u64 ret_alloc = 0; 
    va_list args;

    va_start(args, fmt);
    ret_alloc = vsnprintf(NULLPTR, 0, fmt, args) + 1;
    char* ret = NULLPTR;
    if (arena != NULLPTR) {
        ret = MACRO_ckg_arena_push(arena, ret_alloc);
    } else {
        ret = ckg_alloc(ret_alloc);
    }

    vsnprintf(ret, ret_alloc, fmt, args);
    va_end(args);

    if (allocation_size != NULLPTR) {
        *allocation_size = ret_alloc;
    }

    return ret;
}

internal char* json_to_string_helper(CKG_Arena* arena, JSON* root, int depth) {
    switch (root->type) {
        case CJ_TYPE_BOOL: {
            char* bool_string = root->cj_bool.value ? "true" : "false"; 
            return cj_sprint(arena, NULLPTR, "%s", bool_string);
        } break;

        case CJ_TYPE_INT: {
            return cj_sprint(arena, NULLPTR, "%d",  root->cj_int.value);
        } break;

        case CJ_TYPE_FLOAT: {
            return cj_sprint(arena, NULLPTR, "%f",  root->cj_float.value);
        } break;

        case CJ_TYPE_STRING: {
            return cj_sprint(arena, NULLPTR, "\"%s\"",  root->cj_string.value);    
        } break;
        
        case CJ_TYPE_NULL: {
            return cj_sprint(arena, NULLPTR, "%s",  root->cj_null.value);
        } break;

        case CJ_TYPE_ARRAY: {
            depth += 1;
            int count = ckg_vector_count(root->cj_json.key_value_pair_vector);
            char** buffers = ckg_arena_push_array(arena, char*, count);
            u64 total_allocation_size = sizeof("[]") - 1;
            for (int i = 0; i < count; i++) {
                u64 allocation_size = 0;
                char* value = json_to_string_helper(arena, root->cj_array.jsonVector[i], depth);
                if (i == (count - 1)) {
                    buffers[i] = cj_sprint(arena, &allocation_size, "%s", value);
                } else {
                    buffers[i] = cj_sprint(arena, &allocation_size, "%s, ", value);
                }

                total_allocation_size += allocation_size;
            }

            char* ret = MACRO_ckg_arena_push(arena, total_allocation_size);
            ret[total_allocation_size - 1] = '\0';
            ckg_cstr_append(ret, total_allocation_size, "[");
            for (int i = 0; i < count; i++) {
                ckg_cstr_append(ret, total_allocation_size, buffers[i]);
            }
            ckg_cstr_append(ret, total_allocation_size, "]");

            return ret;
        } break;

        // Date: January 23, 2025
        // TODO(Jovanni): FIX THE MEMORY LEAKS with buffers and stuff
        case CJ_TYPE_JSON: {
            depth += 1;
            int num_json = (depth - 1) * ckg_cstr_length(global_intend);
            int num_key = depth * ckg_cstr_length(global_intend);
            int count = ckg_vector_count(root->cj_json.key_value_pair_vector);

            char** buffers = ckg_arena_push_array(arena, char*, count);
            u64 total_allocation_size = sizeof("{\n%s}") - 1;
            for (int i = 0; i < count; i++) {
                u64 allocation_size = 0;
                char *key = root->cj_json.key_value_pair_vector[i].key;
                char *value = json_to_string_helper(arena, root->cj_json.key_value_pair_vector[i].value, depth);

                if (i == (count - 1)) {
                    buffers[i] = cj_sprint(arena, &allocation_size, "%s\"%s\": %s", generateSpaces(num_key), key, value); 
                } else {
                    buffers[i] = cj_sprint(arena, &allocation_size, "%s\"%s\": %s,\n", generateSpaces(num_key), key, value);  
                }
                total_allocation_size += allocation_size;
            }

            char* ret = MACRO_ckg_arena_push(arena, total_allocation_size);
            ret[total_allocation_size - 1] = '\0';
            ckg_cstr_append(ret, total_allocation_size, "{\n");
            for (int i = 0; i < count; i++) {
                ckg_cstr_append(ret, total_allocation_size, buffers[i]);
            }
            ckg_cstr_append(ret, total_allocation_size, "\n%s}");
            ret = cj_sprint(NULLPTR, NULLPTR, ret, generateSpaces(num_json));

            return ret;
        } break;

    }

    return NULLPTR;
}

char* json_to_string(JSON* root) {
    CKG_Arena* temp_arena = ckg_arena_create(KiloBytes(1));
    char* ret = json_to_string_helper(temp_arena, root, 0);
    ckg_arena_free(temp_arena);

    return ret;
}