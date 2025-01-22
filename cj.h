#include <ckg.h> // This is temperary holy your horses

typedef struct JSON JSON;

typedef struct CJ_Bool {
    Boolean value;
} CJ_Bool;

typedef struct CJ_Int {
    int value;
} CJ_Int;

typedef struct CJ_Float {
    float value;
} CJ_Float;

typedef struct CJ_String{
    char* value;
} CJ_String;

typedef struct CJ_Array {
    JSON** jsonVector;
} CJ_Array;

typedef struct CJ_Null {
    char* value; // "null\0"
} CJ_Null;

typedef struct JSON_KEY_VALUE {
    char* key;
    JSON* value;
} JSON_KEY_VALUE;

typedef struct CJ_JSON {
    JSON_KEY_VALUE* key_value_pair_vector;
} CJ_JSON;

typedef enum CJ_Type {
    CJ_TYPE_BOOL,
    CJ_TYPE_INT,
    CJ_TYPE_FLOAT,
    CJ_TYPE_STRING,
    CJ_TYPE_ARRAY,
    CJ_TYPE_JSON,
    CJ_TYPE_NULL,
} CJ_Type;

typedef struct JSON {
    CJ_Type type;
    union {
        CJ_Bool cj_bool;
        CJ_String cj_string;
        CJ_Float cj_float;
        CJ_Int cj_int;
        CJ_Null cj_null;
        CJ_Array cj_array;
        CJ_JSON cj_json;
    };
} JSON;

JSON* cj_create();
void cj_push(JSON* root, char* key, JSON* value);

JSON* cj_array_create();
void cj_array_push(JSON* array, JSON* value);

JSON* JSON_INT(int value);
JSON* JSON_FLOAT(float value);
JSON* JSON_STRING(char* value);
JSON* JSON_BOOL(Boolean value);
JSON* JSON_NULL();

char* json_to_string(JSON* root);