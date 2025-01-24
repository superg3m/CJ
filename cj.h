#pragma once

#ifdef __cplusplus
    #define CJ_API extern "C"
#else 
    #define CJ_API
#endif 

#if defined(CJ_IMPL)
    #define CJ_IMPL_MEMORY
    #define CJ_IMPL_OS
    #define CJ_IMPL_COLLECTIONS
    #define CJ_IMPL_CSTRING
    #define CJ_IMPL_ARENA

    #define CJ_IMPL_CREATION
    #define CJ_IMPL_FORMATTED_BUFFER

    #define CJ_IMPL_SPL_TOKEN
    #define CJ_IMPL_LEXER
    #define CJ_IMPL_PARSING
#endif

#define CJ_INCLUDE_TYPES
#define CJ_INCLUDE_MEMORY
#define CJ_INCLUDE_OS
#define CJ_INCLUDE_COLLECTIONS
#define CJ_INCLUDE_CSTRING
#define CJ_INCLUDE_ARENA

#define CJ_INCLUDE_CREATION
#define CJ_INCLUDE_FORMATTED_BUFFER

#define CJ_INCLUDE_SPL_TOKEN
#define CJ_INCLUDE_LEXER
#define CJ_INCLUDE_PARSING

#if defined(CJ_INCLUDE_TYPES)
    #include <stdint.h>
    #include <stdio.h>
    #include <stdarg.h>
    #include <stdlib.h>

    typedef int8_t  s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;

    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef u8 Boolean;

    #define FALSE 0
    #define TRUE 1
    #define NULLPTR 0
    #define PI 3.14159265359

    #define stringify(entry) #entry
    #define glue(a, b) a##b

    #define KiloBytes(value) ((u64)(value) * 1024L)
    #define MegaBytes(value) ((u64)KiloBytes(value) * 1024L)
    #define GigaBytes(value) ((u64)MegaBytes(value) * 1024L)

    #define MIN(a, b) (((a) < (b)) ? (a) : (b))
    #define MAX(a, b) (((a) > (b)) ? (a) : (b))
    #define CLAMP(value, min_value, max_value) (MIN(MAX(value, min_value), max_value))

    #define local_persist static
    #define internal static

    // Date: July 12, 2024
    // TODO(Jovanni): Test this to make sure its actually works but it makes sense to me
    #define OFFSET_OF(type, member) sizeof((u64)(&(((type*)0)->member)))
    #define FIRST_DIGIT(number) ((int)number % 10);

    #define GET_BIT(number, bit_to_check) ((number & (1 << bit_to_check)) >> bit_to_check)
    #define SET_BIT(number, bit_to_set) number |= (1 << bit_to_set);
    #define UNSET_BIT(number, bit_to_unset) number &= (~(1 << bit_to_unset));

    #define ArrayCount(array) (sizeof(array) / sizeof(array[0]))
    #define PLATFORM_MAX_PATH 256

    #if defined(_WIN32)
        #include <windows.h>
        #define PLATFORM_WINDOWS
        #define OS_DELIMITER '\\'
        #define CRASH __debugbreak()
    #elif defined(__APPLE__)
        #define PLATFORM_APPLE
        #define OS_DELIMITER '/'
        #define CRASH __builtin_trap()
    #elif defined(__linux__) || defined(__unix__) || defined(__POSIX__)
        #define PLATFORM_LINUX
        #define OS_DELIMITER '/'
        #define CRASH __builtin_trap()
    #else
        #error "Unknown Platform???"
    #endif

    #if defined(__clang__)
        #define UNUSED_FUNCTION __attribute__((used))
    #elif defined(__GNUC__) || defined(__GNUG__)
        #define UNUSED_FUNCTION __attribute__((used))
    #elif defined(_MSC_VER)
        #define UNUSED_FUNCTION
    #endif

    #define cj_assert(expression)                     \
    do {                                               \
        if (!(expression)) {                           \
            char msg[] = "Func: %s, File: %s:%d\n";    \
            printf(msg, __func__, __FILE__, __LINE__); \
            CRASH;                                     \
        }                                              \
    } while (FALSE)                                    \

    #define cj_assert_msg(expression, message, ...)	          \
    do {                                                          \
        if (!(expression)) {                                      \
            char msg[] = "Func: %s, File: %s:%d\n";               \
            printf(msg, __func__, __FILE__, __LINE__);            \
            printf(message, ##__VA_ARGS__);                       \
            CRASH;                                                \
        }                                                         \
    } while (FALSE)                                               \

#endif

#if defined(CJ_INCLUDE_MEMORY)
    void* cj_alloc(u64 allocation_size);
    void* MACRO_cj_free(void* data);
    void* cj_realloc(void* data, u64 old_allocation_size, u64 new_allocation_size);

    CJ_API void cj_memory_copy(const void* source, void* destination, size_t source_size_in_bytes, size_t destination_size_in_bytes);
    CJ_API void cj_memory_zero(void* data, size_t data_size_in_bytes);

    Boolean cj_memory_compare(const void* buffer_one, const void* buffer_two, u32 buffer_one_size, u32 buffer_two_size);

    #define cj_free(data) data = MACRO_cj_free(data)
#endif

#if defined (CJ_INCLUDE_OS)
    Boolean cj_os_path_exists(const char* path);
    u8* cj_os_read_entire_file(const char* path, u64* returned_file_size);
#endif

#if defined(CJ_INCLUDE_COLLECTIONS)
    //
    // ========== START CJ_VECTOR ==========
    //
    typedef struct CJ_VectorHeader {
        u32 count;
        u32 capacity;
    } CJ_VectorHeader;

    CJ_API void* cj_vector_grow(void* vector, size_t element_size);

    #define VECTOR_DEFAULT_CAPACITY 1
    #define cj_vector_header_base(vector) ((CJ_VectorHeader*)(((u8*)vector) - sizeof(CJ_VectorHeader)))
    #define cj_vector_count(vector) (*cj_vector_header_base(vector)).count
    #define cj_vector_capacity(vector) (*cj_vector_header_base(vector)).capacity

    #ifdef __cplusplus
        #define cj_vector_push(vector, element) vector = (decltype(vector))cj_vector_grow(vector, sizeof(element)); vector[cj_vector_header_base(vector)->count++] = element
    #else 
        #define cj_vector_push(vector, element) vector = cj_vector_grow(vector, sizeof(element)); vector[cj_vector_header_base(vector)->count++] = element
    #endif
    
    #define cj_vector_free(vector) cj_free(cj_vector_header_base(vector)); vector = NULLPTR
    //
    // ========== END CJ_VECTOR ==========
    //

    //
    // ========== START CJ_LinkedList ==========
    //
    typedef struct CJ_Node {
        struct CJ_Node* next;
        struct CJ_Node* prev;
        size_t element_size_in_bytes;
        void* data;
    } CJ_Node;

    typedef struct CJ_LinkedList {
        CJ_Node* head;
        CJ_Node* tail;
        size_t element_size_in_bytes;
        u32 count;
        Boolean is_pointer_type;
    } CJ_LinkedList;

    CJ_API CJ_LinkedList* MACRO_cj_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type);
    CJ_API CJ_Node* cj_linked_list_insert(CJ_LinkedList* linked_list, u32 index, void* data);
    CJ_API CJ_Node* cj_linked_list_get_node(CJ_LinkedList* linked_list, u32 index);
    CJ_API void* cj_linked_list_get(CJ_LinkedList* linked_list, u32 index);
    CJ_API void* cj_linked_list_peek_head(CJ_LinkedList* linked_list);
    CJ_API void* cj_linked_list_peek_tail(CJ_LinkedList* linked_list);
    CJ_API CJ_Node* cj_linked_list_push(CJ_LinkedList* linked_list, void* data);
    CJ_API CJ_Node cj_linked_list_pop(CJ_LinkedList* linked_list);
    CJ_API CJ_Node cj_linked_list_remove(CJ_LinkedList* linked_list, u32 index);
    CJ_API void* MACRO_cj_linked_list_free(CJ_LinkedList* linked_list);
    CJ_API u32 cj_linked_list_node_to_index(CJ_LinkedList* linked_list, CJ_Node* address);

    #define cj_linked_list_create(type, is_pointer_type) MACRO_cj_linked_list_create(sizeof(type), is_pointer_type)

    #ifdef __cplusplus
        #define cj_linked_list_free(linked_list) linked_list = (decltype(linked_list))MACRO_cj_linked_list_free(linked_list)
    #else 
        #define cj_linked_list_free(linked_list) linked_list = MACRO_cj_linked_list_free(linked_list)
    #endif
    
    //
    // ========== END CJ_VECTOR ==========
    //
#endif

#if defined(CJ_INCLUDE_CSTRING)
    CJ_API u32 cj_cstr_length(const char* cstring);
    CJ_API void cj_cstr_append(char* string_buffer, size_t string_buffer_capacity, const char* to_append);
	CJ_API void cj_cstr_append_char(char* string_buffer, size_t string_buffer_capacity, const char to_append);
    char* cj_substring(const char* str, u64 start, u64 end);
    Boolean cj_cstr_equal(const char* s1, const char* s2);
#endif

#if defined(CJ_INCLUDE_ARENA)
    #define CJ_ARENA_FLAG_FIXED 0
    #define CJ_ARENA_FLAG_CIRCULAR 1
    #define CJ_ARENA_FLAG_EXTENDABLE_PAGES 2
    #define CJ_ARENA_FLAG_COUNT 3
    typedef u8 CJ_ArenaFlag;
    typedef struct CJ_Arena CJ_Arena;

    CJ_API CJ_Arena* MACRO_cj_arena_create(size_t allocation_size, CJ_ArenaFlag flag, u8 alignment);
    CJ_API void* MACRO_cj_arena_push(CJ_Arena* arena, size_t element_size);	
    CJ_API CJ_Arena* MACRO_cj_arena_free(CJ_Arena* arena);
    CJ_API void cj_arena_clear(CJ_Arena* arena);

    #define cj_arena_create(allocation_size) MACRO_cj_arena_create(allocation_size, CJ_ARENA_FLAG_EXTENDABLE_PAGES, 0)
    #define cj_arena_create_custom(allocation_size, flags, alignment) MACRO_cj_arena_create(allocation_size, flags, alignment)
    #define cj_arena_free(arena) arena = MACRO_cj_arena_free(arena)
    #define cj_arena_push(arena, type) ((type*)MACRO_cj_arena_push(arena, sizeof(type)))
    #define cj_arena_push_array(arena, type, element_count) ((type*)MACRO_cj_arena_push(arena, sizeof(type) * element_count))
#endif

#if defined(CJ_INCLUDE_CREATION)
    typedef struct JSON JSON;

    typedef struct CJ_Array {
        JSON** jsonVector;
    } CJ_Array;

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
            Boolean cj_bool;
            char* cj_string;
            float cj_float;
            int cj_int; 
            char* cj_null; // "null\0"
            CJ_Array cj_array;
            CJ_JSON cj_json;
        };
        CJ_Arena* arena;
    } JSON;

    JSON* cj_create(CJ_Arena* arena);
    void MACRO_cj_push(JSON* root, char* key, JSON* value);

    JSON* cj_array_create(CJ_Arena* arena);
    void MACRO_cj_array_push(JSON* array, JSON* value);

    JSON* JSON_INT(CJ_Arena* arena, int value);
    JSON* JSON_FLOAT(CJ_Arena* arena, float value);
    JSON* JSON_STRING(CJ_Arena* arena, char* value);
    JSON* JSON_BOOL(CJ_Arena* arena, Boolean value);
    JSON* JSON_JSON(CJ_Arena* arena, JSON* json);
    JSON* JSON_NULL(CJ_Arena* arena);

    #define cj_push(root, key, value) MACRO_cj_push(root, key, _Generic((value),  \
        Boolean: JSON_BOOL,                \
        char[sizeof(value)]: JSON_STRING,              \
        const char[sizeof(value)]: JSON_STRING,              \
        char*: JSON_STRING,               \
        const char*: JSON_STRING,         \
        float: JSON_FLOAT,                \
        int: JSON_INT,                    \
        JSON*: JSON_JSON                 \
    )(root->arena, value))


    #define cj_array_push(root, value) MACRO_cj_array_push(root, _Generic((value),  \
        Boolean: JSON_BOOL,                \
        char[sizeof(value)]: JSON_STRING,              \
        const char[sizeof(value)]: JSON_STRING,              \
        char*: JSON_STRING,               \
        const char*: JSON_STRING,         \
        float: JSON_FLOAT,                \
        int: JSON_INT,                    \
        JSON*: JSON_JSON                 \
    )(root->arena, value))
#endif

#if defined(CJ_INCLUDE_FORMATTED_BUFFER)
    CJ_API char* json_to_string(JSON* root);
    CJ_API void cj_set_context_indent(char* new_indent);
#endif

#if defined(CJ_INCLUDE_SPL_TOKEN)
    typedef enum SPL_TokenType {
        SPL_TOKEN_ILLEGAL_TOKEN,
        SPL_TOKEN_EOF,
        SPL_TOKEN_LEFT_PAREN,                 // "("
        SPL_TOKEN_RIGHT_PAREN,                // ")"
        SPL_TOKEN_COMMA,                      // ","
        SPL_TOKEN_MINUS,                      // "-"
        SPL_TOKEN_LEFT_BRACKET,               // "["
        SPL_TOKEN_RIGHT_BRACKET,              // "]"
        SPL_TOKEN_LEFT_CURLY,                 // "{"
        SPL_TOKEN_RIGHT_CURLY,                // "}"
        SPL_TOKEN_COLON,                      // ":"
        SPL_TOKEN_STRING_LITERAL,             // "TESTING
        SPL_TOKEN_INTEGER_LITERAL,            // 6
        SPL_TOKEN_FLOAT_LITERAL,              // 2.523
        SPL_TOKEN_CHARACTER_LITERAL,          // 'a'
        SPL_TOKEN_TRUE,                       // true
        SPL_TOKEN_FALSE,                      // false
        SPL_TOKEN_NULL,                       // null
        SPL_TOKEN_COUNT
    } SPL_TokenType;

    typedef struct SPL_Token {
        char *lexeme;
        SPL_TokenType type;
        u64 line;
    } SPL_Token;

    const char* tokenTypeToString(SPL_TokenType type);
#endif

#if defined(CJ_INCLUDE_LEXER)
    typedef struct Lexer {
        u64 left_pos;
        u64 right_pos;
        u64 line;

        u8 c;
        u8* source;
        u64 source_size;

        SPL_Token* tokens;
    } Lexer;

    Lexer lexerCreate();
    SPL_Token* lexerGenerateTokenStream(Lexer* lexer, char* file_data, u64 file_size);
#endif

#if defined(CJ_INCLUDE_PARSING)
    JSON* parse_json_buffer(char* json_buffer);

    typedef struct Parser {
        SPL_Token* tokens;
        SPL_Token tok;
        int current;
        CJ_Arena* arena_allocator;
    } Parser;

    Parser parserCreate();
#endif

//
// ===================================================== CJ_IMPL =====================================================
//

#if defined(CJ_IMPL_MEMORY)
    void* cj_alloc(u64 allocation_size) {
        cj_assert(allocation_size != 0);

        void* ret = malloc(allocation_size);
        cj_memory_zero(ret, allocation_size);
        return ret;
    }

    void* cj_realloc(void* data, u64 old_allocation_size, u64 new_allocation_size) {
        cj_assert(old_allocation_size != 0);
        cj_assert(new_allocation_size != 0);

        void* ret = cj_alloc(new_allocation_size);
        cj_memory_copy(data, ret, old_allocation_size, new_allocation_size);
        cj_free(data);
        return ret;
    }

    void* MACRO_cj_free(void* data) {
        free(data);
        data = NULLPTR;
        return data;
    }

    void cj_memory_copy(const void* source, void* destination, size_t source_size_in_bytes, size_t destination_size_in_bytes) {
        cj_assert(source);
        cj_assert(destination);
        cj_assert(source_size_in_bytes <= destination_size_in_bytes);
        if (source_size_in_bytes == 0) {
            return;
        }

        u8* temp_data_copy = (u8*)cj_alloc(source_size_in_bytes);
        for (u32 i = 0; i < source_size_in_bytes; i++) {
            temp_data_copy[i] = ((u8*)source)[i];
        }

        for (u32 i = 0; i < source_size_in_bytes; i++) {
            ((u8*)destination)[i] = temp_data_copy[i];
        }

        cj_free(temp_data_copy);
    }

    void cj_memory_zero(void* data, size_t data_size_in_bytes) {
        for (u32 i = 0; i < data_size_in_bytes; i++) {
            ((u8*)data)[i] = 0;
        }
    }

    Boolean cj_memory_compare(const void* buffer_one, const void* buffer_two, u32 buffer_one_size, u32 buffer_two_size) {
        cj_assert(buffer_one);
        cj_assert(buffer_two);

        if (buffer_one_size != buffer_two_size) {
            return FALSE;
        }

        u8* buffer_one_data = (u8*)buffer_one;
        u8* buffer_two_data = (u8*)buffer_two;
        for (u32 i = 0; i < buffer_one_size; i++) {
            if (buffer_one_data[i] != buffer_two_data[i]) {
                return FALSE;
            }
        }

        return TRUE;
    }
#endif

#if defined(CJ_IMPL_OS)
    Boolean cj_os_path_exists(const char* path) {
        FILE *fptr = fopen(path, "r");

        if (fptr == NULL) {
            return FALSE;
        }

        fclose(fptr);

        return TRUE;
    }

    u8* cj_os_read_entire_file(const char* path, u64* returned_file_size) {
        cj_assert(cj_os_path_exists(path));

        FILE* file_handle = fopen(path, "r");

        if (file_handle == NULL) {
            return FALSE;
        }

        fseek(file_handle, 0L, SEEK_END);
        size_t file_size = ftell(file_handle);
        rewind(file_handle);

        u8* file_data = cj_alloc(file_size);
        cj_assert(fread(file_data, file_size, 1 , file_handle) != file_size); // Error reading file
        rewind(file_handle);

        fclose(file_handle);
        *returned_file_size = file_size;

        return file_data;
    }
#endif

#if defined(CJ_IMPL_COLLECTIONS)
    //
    // ========== START CJ_VECTOR ==========
    //
    void* cj_vector_grow(void* vector, size_t element_size) {
        if (vector == NULLPTR) {
            vector = cj_alloc(sizeof(CJ_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size));
            cj_memory_zero(vector, sizeof(CJ_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size));
            vector = (u8*)vector + sizeof(CJ_VectorHeader);
            cj_vector_capacity(vector) = VECTOR_DEFAULT_CAPACITY;
        }

        
        u32 count = cj_vector_count(vector);
        u32 capactiy = cj_vector_capacity(vector);

        if (capactiy < count + 1) {
            size_t old_allocation_size = sizeof(CJ_VectorHeader) + (capactiy * element_size);
            u32 new_capactiy = capactiy * 2;
            size_t new_allocation_size = sizeof(CJ_VectorHeader) + (new_capactiy * element_size);

            vector = cj_realloc(cj_vector_header_base(vector), old_allocation_size, new_allocation_size);
            vector = (u8*)vector + sizeof(CJ_VectorHeader);

            cj_vector_header_base(vector)->count = count;
            cj_vector_header_base(vector)->capacity = new_capactiy;
        }

        return vector;
    }
    //
    // ========== END CJ_VECTOR ==========
    //

    //
    // ========== START CJ_LinkedList ==========
    //
    CJ_LinkedList* MACRO_cj_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type) {
        CJ_LinkedList* ret = (CJ_LinkedList*)cj_alloc(sizeof(CJ_LinkedList));
        ret->count = 0;
        ret->element_size_in_bytes = element_size_in_bytes;
        ret->head = NULLPTR;
        ret->tail = NULLPTR;
        ret->is_pointer_type = is_pointer_type;
        return ret;
    }

    CJ_Node* MACRO_cj_node_create(CJ_LinkedList* linked_list, void* data) {
        CJ_Node* ret = (CJ_Node*)cj_alloc(sizeof(CJ_Node));
        if (linked_list->is_pointer_type) {
            ret->data = data;
        } else {
            ret->data = cj_alloc(linked_list->element_size_in_bytes); // user has to cj_free
            cj_memory_copy(data, ret->data, linked_list->element_size_in_bytes, linked_list->element_size_in_bytes); 
        }

        ret->element_size_in_bytes = linked_list->element_size_in_bytes;
        ret->next = NULLPTR;
        ret->prev = NULLPTR;
        return ret;
    }
    #define cj_node_create(linked_list, data) MACRO_cj_node_create(linked_list, data)

    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CJ_Node* 
     */
    internal CJ_Node* MACRO_cj_node_free(CJ_LinkedList* linked_list, CJ_Node* node) {
        cj_assert(linked_list);
        cj_assert(node);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        cj_free(node);

        return node;
    }
    #define cj_node_free(linked_list, node) node = MACRO_cj_node_free(linked_list, node)


    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CJ_Node* 
     */
    internal CJ_Node* MACRO_cj_node_data_free(CJ_LinkedList* linked_list, CJ_Node* node) {
        cj_assert(linked_list);
        cj_assert(node);
        cj_assert(node->data);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        if (!linked_list->is_pointer_type) {
            cj_free(node->data);
        }
        cj_free(node);

        return node;
    }
    #define cj_node_data_free(linked_list, node) node = MACRO_cj_node_data_free(linked_list, node)

    CJ_Node* cj_linked_list_insert(CJ_LinkedList* linked_list, u32 index, void* data) {
        cj_assert(linked_list);
        cj_assert(data);
        cj_assert(index >= 0);

        u32 old_count = linked_list->count++;
        if (linked_list->head == NULLPTR) { // there is not head and by definition no tail
            CJ_Node* new_node_to_insert = cj_node_create(linked_list, data);
            linked_list->head = new_node_to_insert;
            linked_list->tail = new_node_to_insert;

            return linked_list->head;
        }

        cj_assert(index <= old_count);
        CJ_Node* new_node_to_insert = cj_node_create(linked_list, data);

        if (index == 0) { // insert at head
            linked_list->head->prev = new_node_to_insert;
            new_node_to_insert->next = linked_list->head;
            linked_list->head = new_node_to_insert;
            
            return new_node_to_insert;
        }

        if (index == old_count) { // insert at tail
            linked_list->tail->next = new_node_to_insert;
            new_node_to_insert->prev = linked_list->tail;
            linked_list->tail = new_node_to_insert;

            return new_node_to_insert;
        }

        // Date: July 19, 2024
        // TODO(Jovanni): check if index is closer to count or not then reverse the loop if approaching from the tail end.
        // as opposed to the head end.
        CJ_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        new_node_to_insert->prev = current_node;
        new_node_to_insert->next = current_node->next;

        current_node->next->prev = new_node_to_insert;
        current_node->next = new_node_to_insert;

        return new_node_to_insert;
    }

    CJ_Node* cj_linked_list_get_node(CJ_LinkedList* linked_list, u32 index) {
        cj_assert(linked_list);
        CJ_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        return current_node;
    }

    void* cj_linked_list_get(CJ_LinkedList* linked_list, u32 index) {
        return cj_linked_list_get_node(linked_list, index)->data;
    }

    void* cj_linked_list_peek_head(CJ_LinkedList* linked_list) {
        return cj_linked_list_get_node(linked_list, 0)->data;
    }

    void* cj_linked_list_peek_tail(CJ_LinkedList* linked_list) {
        return cj_linked_list_get_node(linked_list, linked_list->count - 1)->data;
    }

    CJ_Node* cj_linked_list_push(CJ_LinkedList* linked_list, void* data) {
        return cj_linked_list_insert(linked_list, linked_list->count, data);
    }

    u32 cj_linked_list_node_to_index(CJ_LinkedList* linked_list, CJ_Node* address) {
        CJ_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < linked_list->count + 1; i++) {
            if (current_node == address) {
                return i;
            }
            current_node = current_node->next;
        }

        cj_assert(FALSE); // couldn't match a node to an address
        return 0; // should never get here
    }

    CJ_Node cj_linked_list_pop(CJ_LinkedList* linked_list) {
        return cj_linked_list_remove(linked_list, linked_list->count - 1);
    }

    CJ_Node cj_linked_list_remove(CJ_LinkedList* linked_list, u32 index) {
        cj_assert(linked_list); 
        cj_assert(linked_list->count > 0); 
        cj_assert(index >= 0);

        u32 old_count = linked_list->count--;
        if (index == 0 && old_count == 1) { // removing the head fully
            CJ_Node ret = *linked_list->head;
            cj_node_free(linked_list, linked_list->head);
            linked_list->head = NULLPTR;

            return ret;
        }

        if (index == 0) { // remove head
            CJ_Node* cached_head = linked_list->head;
            linked_list->head = linked_list->head->next;
            CJ_Node ret = *cached_head; 
            cj_node_free(linked_list, cached_head);

            return ret;
        }

        cj_assert(index < old_count);
        if (index == (old_count - 1)) { // remove tail
            CJ_Node* cached_tail = linked_list->tail;
            linked_list->tail = linked_list->tail->prev;
            CJ_Node ret = *cached_tail; 
            cj_node_free(linked_list, cached_tail);

            return ret;
        }

        CJ_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        current_node->next->prev = current_node->prev;
        current_node->prev->next = current_node->next;
        CJ_Node ret = *current_node; 
        cj_node_free(linked_list, current_node);

        return ret;
    }

    void* MACRO_cj_linked_list_free(CJ_LinkedList* linked_list) {
        cj_assert(linked_list); 
        CJ_Node* current_node = linked_list->head; 
        CJ_Node* next_node = NULLPTR; 
        for (u32 i = 0; i < linked_list->count; i++) {
            next_node = current_node->next;
            cj_node_data_free(linked_list, current_node);
            current_node = next_node;
        }
        cj_free(linked_list);

        return linked_list;
    }
    //
    // ========== END CJ_LinkedList ==========
    //
#endif

#if defined(CJ_IMPL_CSTRING)
    u32 cj_cstr_length(const char* cstring) {
        cj_assert(cstring);

        if (!cstring) {
            return 0; // This should never get here but the compiler want this
        }

        u32 length = 0;
        char* cursor = (char*)cstring;
        while(*cursor++ != '\0') {
            length++;
        }

        return length;
    }

    void cj_cstr_append(char* str, size_t str_capacity, const char* to_append) {
        u32 str_length = cj_cstr_length(str);
        u32 to_append_length = cj_cstr_length(to_append);

        cj_assert((str_length + to_append_length) < str_capacity);

        for (u32 i = 0; i < to_append_length; i++) {
            str[str_length + i] = to_append[i];
        }
    }

    void cj_cstr_append_char(char* str, size_t str_capacity, const char to_append) {
        u32 str_length = cj_cstr_length(str);
        cj_assert((str_length + 1) < str_capacity);
        str[str_length + 1] = to_append;
    }

    char* cj_substring(const char* str, u64 start, u64 end) {
        cj_assert(str);
        u64 str_length = cj_cstr_length(str); 
        char* ret = cj_alloc((end - start) + 1);

        Boolean start_check = (start >= 0) && (start <= str_length - 1);
        Boolean end_check = (end >= 0) && (end <= str_length - 1);

        cj_assert_msg(start_check, "ckg_substring: Start range is outside expected range: [%d - %llu] got: %llu\n", 0, str_length - 1, start);
        cj_assert_msg(end_check, "ckg_substring: End range is outside expected range: [%d - %llu] got: %lld\n", 0, str_length - 1, end);
        cj_assert_msg(start <= end, "ckg_substring: Start range is greater than end range[start: %llu > end: %llu]\n", start, end);

        //char* str = "hello"
        //0 - 4 = hello\0 = 6
        //0 - 0 = h\0 = 2
        //0 - 1 = he\0 = 3
        //1 - 4 = ello\0 = 5

        u64 counter = 0;
        for (u64 i = start; i <= end; i++) {
            ret[counter++] = str[i];
        }
        ret[counter] = '\0'; 

        return ret;
    }

    Boolean cj_cstr_equal(const char* s1, const char* s2) {
        cj_assert(s1);
        cj_assert(s2);

        u32 s1_length = cj_cstr_length(s1);
        u32 s2_length = cj_cstr_length(s2);

        return cj_memory_compare(s1, s2, s1_length, s2_length);
    }
#endif

#if defined(CJ_IMPL_ARENA)
    #define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

    typedef struct CJ_ArenaPage {
        u8* base_address;
        u64 capacity;
        u64 used;
    } CJ_ArenaPage;

    typedef struct CJ_Arena {
        const char* name;
        CJ_LinkedList* pages;
        CJ_ArenaFlag flag;
        u8 alignment;
    } CJ_Arena;

    Boolean cj_is_set(CJ_Arena* arena, CJ_ArenaFlag flag) {
        return arena->flag == flag;
    }

    internal CJ_ArenaPage* cj_arena_page_create(size_t allocation_size) {
        CJ_ArenaPage* ret = (CJ_ArenaPage*)cj_alloc(sizeof(CJ_ArenaPage));
        ret->used = 0;
        ret->capacity = allocation_size;
        ret->base_address = (u8*)cj_alloc(allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE);

        return ret;
    }

    CJ_Arena* MACRO_cj_arena_create(size_t allocation_size, CJ_ArenaFlag flag, u8 alignment) {
        CJ_Arena* arena = (CJ_Arena*)cj_alloc(sizeof(CJ_Arena));
        arena->alignment = alignment == 0 ? 8 : alignment;
        arena->flag = flag;
        arena->pages = cj_linked_list_create(CJ_ArenaPage*, TRUE);
        CJ_ArenaPage* inital_page = cj_arena_page_create(allocation_size);
        cj_linked_list_push(arena->pages, inital_page);

        return arena;
    }

    CJ_Arena* MACRO_cj_arena_free(CJ_Arena* arena) {
        cj_assert(arena);

        for (u32 i = 0; i < arena->pages->count; i++) {
            CJ_ArenaPage* page = (CJ_ArenaPage*)cj_linked_list_remove(arena->pages, 0).data;
            cj_assert(page->base_address);
            cj_free(page->base_address);
            cj_free(page);
        }
        cj_linked_list_free(arena->pages);
        cj_free(arena);

        return arena;
    }

    void cj_arena_clear(CJ_Arena* arena) {
        cj_assert(arena);

        for (u32 i = 0; i < arena->pages->count; i++) {
            CJ_ArenaPage* page = (CJ_ArenaPage*)cj_linked_list_get(arena->pages, i);
            cj_assert(page->base_address);
            cj_memory_zero(page->base_address, page->used);
            page->used = 0;
        }
    }

    void* MACRO_cj_arena_push(CJ_Arena* arena, size_t element_size) {
        CJ_ArenaPage* last_page = (CJ_ArenaPage*)cj_linked_list_peek_tail(arena->pages);
        if (cj_is_set(arena, CJ_ARENA_FLAG_FIXED)) { // single page assert if you run out of memory
            cj_assert((last_page->used + element_size <= last_page->capacity));
        } else if (cj_is_set(arena, CJ_ARENA_FLAG_CIRCULAR)) { // single page circle around if you run out of memory
            if ((last_page->used + element_size > last_page->capacity)) {
                last_page->used = 0;
                cj_assert((last_page->used + element_size <= last_page->capacity));
            }
        } else if (cj_is_set(arena, CJ_ARENA_FLAG_EXTENDABLE_PAGES)) {
            cj_assert(last_page->base_address);
            if ((last_page->used + element_size > last_page->capacity)) {
                CJ_ArenaPage* next_page = cj_arena_page_create((last_page->capacity + element_size) * 2);
                cj_assert(next_page->base_address);
                cj_linked_list_push(arena->pages, next_page);
            }
        } else {
            cj_assert(FALSE);
        }

        last_page = (CJ_ArenaPage*)cj_linked_list_peek_tail(arena->pages); // tail might change

        u8* ret = last_page->base_address + last_page->used;
        last_page->used += element_size;
        if ((last_page->used & (arena->alignment - 1)) != 0) { // if first bit is set then its not aligned
            last_page->used += (arena->alignment - (last_page->used & (arena->alignment - 1)));
        }

        return ret;
    }
#endif

#if defined(CJ_IMPL_CREATION) 
    JSON* cj_create(CJ_Arena* arena) {
        JSON* ret = (JSON*)cj_arena_push(arena, JSON);
        ret->arena = arena;
        ret->type = CJ_TYPE_JSON;

        return ret;
    }

    JSON* cj_array_create(CJ_Arena* arena) {
        JSON* ret = (JSON*)cj_arena_push(arena, JSON);
        ret->arena = arena;
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

        cj_vector_push(root->cj_json.key_value_pair_vector, pair);
    }

    void MACRO_cj_array_push(JSON* root, JSON* value) {
        if (root->type != CJ_TYPE_ARRAY) {
            // I would normally assert, but Andy wouldn't like that so I guess error codes?
            // If its not of type JSON then you shouldn't be pushing to it
        }

        cj_vector_push(root->cj_array.jsonVector, value);
    }

    JSON* JSON_INT(CJ_Arena* arena,int value) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_INT;
        ret->cj_int = value;

        return ret;
    }

    JSON* JSON_FLOAT(CJ_Arena* arena, float value) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_FLOAT;
        ret->cj_float = value;

        return ret;
    }

    JSON* JSON_STRING(CJ_Arena* arena,char* value) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_STRING;
        ret->cj_string = value;

        return ret;
    }

    JSON* JSON_BOOL(CJ_Arena* arena,Boolean value) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_BOOL;
        ret->cj_bool = value;

        return ret;
    }

    JSON* JSON_JSON(JSON* json) {
        return json;
    }

    JSON* JSON_NULL(CJ_Arena* arena) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_NULL;
        ret->cj_null = "null";

        return ret;
    }
#endif

#if defined (CJ_IMPL_FORMATTED_BUFFER)
    internal char* global_intend = "    ";
    void cj_set_context_indent(char* new_indent) {
        global_intend = new_indent;
    }

    internal char* generateSpaces(int number_of_spaces) {
        char* ret = cj_alloc((sizeof(char) * number_of_spaces) + 1);
        for (int i = 0; i < number_of_spaces; i++) {
            ret[i] = ' ';
        }
        
        return ret;
    }

    char* cj_sprint(CJ_Arena* arena, u64* allocation_size, char* fmt, ...) {
        u64 ret_alloc = 0; 
        va_list args;

        va_start(args, fmt);
        ret_alloc = vsnprintf(NULLPTR, 0, fmt, args) + 1;
        char* ret = NULLPTR;
        if (arena != NULLPTR) {
            ret = MACRO_cj_arena_push(arena, ret_alloc);
        } else {
            ret = cj_alloc(ret_alloc);
        }

        vsnprintf(ret, ret_alloc, fmt, args);
        va_end(args);

        if (allocation_size != NULLPTR) {
            *allocation_size = ret_alloc;
        }

        return ret;
    }

    internal char* json_to_string_helper(CJ_Arena* arena, JSON* root, int depth) {
        switch (root->type) {
            case CJ_TYPE_BOOL: {
                char* bool_string = root->cj_bool ? "true" : "false"; 
                return cj_sprint(arena, NULLPTR, "%s", bool_string);
            } break;

            case CJ_TYPE_INT: {
                return cj_sprint(arena, NULLPTR, "%d",  root->cj_int);
            } break;

            case CJ_TYPE_FLOAT: {
                return cj_sprint(arena, NULLPTR, "%f",  root->cj_float);
            } break;

            case CJ_TYPE_STRING: {
                return cj_sprint(arena, NULLPTR, "\"%s\"",  root->cj_string);    
            } break;
            
            case CJ_TYPE_NULL: {
                return cj_sprint(arena, NULLPTR, "%s",  root->cj_null);
            } break;

            case CJ_TYPE_ARRAY: {
                depth += 1;
                int count = cj_vector_count(root->cj_json.key_value_pair_vector);
                char** buffers = cj_arena_push_array(arena, char*, count);
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

                char* ret = MACRO_cj_arena_push(arena, total_allocation_size);
                ret[total_allocation_size - 1] = '\0';
                cj_cstr_append(ret, total_allocation_size, "[");
                for (int i = 0; i < count; i++) {
                    cj_cstr_append(ret, total_allocation_size, buffers[i]);
                }
                cj_cstr_append(ret, total_allocation_size, "]");

                return ret;
            } break;

            // Date: January 23, 2025
            // TODO(Jovanni): FIX THE MEMORY LEAKS with buffers and stuff
            case CJ_TYPE_JSON: {
                depth += 1;
                int num_json = (depth - 1) * cj_cstr_length(global_intend);
                int num_key = depth * cj_cstr_length(global_intend);
                int count = cj_vector_count(root->cj_json.key_value_pair_vector);

                char** buffers = cj_arena_push_array(arena, char*, count);
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

                char* ret = MACRO_cj_arena_push(arena, total_allocation_size);
                ret[total_allocation_size - 1] = '\0';
                cj_cstr_append(ret, total_allocation_size, "{\n");
                for (int i = 0; i < count; i++) {
                    cj_cstr_append(ret, total_allocation_size, buffers[i]);
                }
                cj_cstr_append(ret, total_allocation_size, "\n%s}");
                ret = cj_sprint(NULLPTR, NULLPTR, ret, generateSpaces(num_json));

                return ret;
            } break;

        }

        return NULLPTR;
    }

    char* json_to_string(JSON* root) {
        CJ_Arena* temp_arena = cj_arena_create(KiloBytes(1));
        char* ret = json_to_string_helper(temp_arena, root, 0);
        cj_arena_free(temp_arena);

        return ret;
    }
#endif

#if defined(CJ_IMPL_SPL_TOKEN)
    internal const char* lookup_table[SPL_TOKEN_COUNT] = {
        stringify(SPL_TOKEN_ILLEGAL_TOKEN),
        stringify(SPL_TOKEN_EOF),
        stringify(SPL_TOKEN_LEFT_PAREN),
        stringify(SPL_TOKEN_RIGHT_PAREN),
        stringify(SPL_TOKEN_COMMA),
        stringify(SPL_TOKEN_MINUS),
        stringify(SPL_TOKEN_LEFT_BRACKET),
        stringify(SPL_TOKEN_RIGHT_BRACKET),
        stringify(SPL_TOKEN_LEFT_CURLY),
        stringify(SPL_TOKEN_RIGHT_CURLY),
        stringify(SPL_TOKEN_COLON),
        stringify(SPL_TOKEN_STRING_LITERAL),
        stringify(SPL_TOKEN_INTEGER_LITERAL),
        stringify(SPL_TOKEN_FLOAT_LITERAL),
        stringify(SPL_TOKEN_CHARACTER_LITERAL),
        stringify(SPL_TOKEN_TRUE),
        stringify(SPL_TOKEN_FALSE),
        stringify(SPL_TOKEN_NULL),
    };

    const char* tokenTypeToString(SPL_TokenType type) {
        return lookup_table[type];
    }

    const SPL_Token tokenCreate(SPL_TokenType type, char* lexeme, u64 line) {
        SPL_Token ret = {0};
        ret.lexeme = lexeme;
        ret.type = type;
        ret.line = line;

        return ret;
    }
#endif

#if defined (CJ_IMPL_LEXER)
    internal Boolean isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    Lexer lexerCreate() {
        Lexer lexer = {0};

        lexer.left_pos  = 0;
        lexer.right_pos = 0;
        lexer.line      = 1;
        lexer.c = '\0';
        lexer.source = NULLPTR;
        lexer.source_size = 0;

        return lexer;
    }

    internal void lexerReset(Lexer* lexer) {
        lexer->left_pos  = 0;
        lexer->right_pos = 0;
        lexer->line      = 1;
        lexer->c = '\0';
        lexer->source = NULLPTR;
        lexer->source_size = 0;
    }

    internal char* getScratchBuffer(Lexer* lexer) {
        return cj_substring(lexer->source, lexer->left_pos, lexer->right_pos - 1);
    }

    internal SPL_TokenType getAcceptedSyntax(Lexer* lexer) {
        internal char syntaxLookup[] = { 
            '(', ')', ',', '-',
            '[', ']', '{', '}', ':'
        };

        internal SPL_TokenType syntaxTokenTable[] = {
            SPL_TOKEN_LEFT_PAREN, SPL_TOKEN_RIGHT_PAREN,
            SPL_TOKEN_COMMA, SPL_TOKEN_MINUS,
            SPL_TOKEN_LEFT_BRACKET, SPL_TOKEN_RIGHT_BRACKET, 
            SPL_TOKEN_LEFT_CURLY, SPL_TOKEN_RIGHT_CURLY, SPL_TOKEN_COLON
        };

        for (int i = 0; i < ArrayCount(syntaxTokenTable); i++) {
            char keyword = syntaxLookup[i];
            if (keyword == getScratchBuffer(lexer)[0]) {
                return syntaxTokenTable[i];
            }
        }

        return SPL_TOKEN_ILLEGAL_TOKEN;
    }

    internal SPL_TokenType getAcceptedKeyword(Lexer* lexer) {
        internal SPL_TokenType keywordTokenTable[] = {
            SPL_TOKEN_TRUE, SPL_TOKEN_FALSE, SPL_TOKEN_NULL,
        };

        internal char* keywords[] = {
            "true", "false", "null"
        };

        for (int i = 0; i < ArrayCount(keywords); i++) {
            char* keyword = keywords[i];
            if (cj_cstr_equal(keyword, getScratchBuffer(lexer))) {
                return keywordTokenTable[i];
            }
        }

        return SPL_TOKEN_ILLEGAL_TOKEN;
    }

    internal Boolean isEOF(Lexer* lexer) {
        return lexer->right_pos >= lexer->source_size;
    }

    internal char peekNthChar(Lexer* lexer, u8 n) {
        if ((lexer->right_pos + n) >= lexer->source_size) {
            return '\0';
        }

        return lexer->source[lexer->right_pos + n];
    }

    internal consumeNextChar(Lexer* lexer) {
        lexer->c = lexer->source[lexer->right_pos];
        lexer->right_pos += 1;
    }

    internal Boolean consumeOnMatch(Lexer* lexer, char expected) {
        if (peekNthChar(lexer, 0) != expected) {
            return FALSE;
        }

        consumeNextChar(lexer);
        return TRUE;
    }

    internal void consumeUntilNewLine(Lexer* lexer) {
        while (!isEOF(lexer) && peekNthChar(lexer, 0) != '\r') {
            consumeNextChar(lexer);
        }
    }

    internal void addToken(Lexer* lexer, SPL_TokenType type) {
        cj_vector_push(lexer->tokens, tokenCreate(type, getScratchBuffer(lexer), lexer->line));
    }

    internal Boolean consumeWhitespace(Lexer* lexer) {
        if (!isWhitespace(lexer->c)) {
            return FALSE;
        }

        if (lexer->c == '\n') {
            lexer->line += 1;
        }

        return TRUE;
    }

    internal void reportError(Lexer* lexer, char* msg) {
        printf("Lexical Error: %s | Line: %llu\n", getScratchBuffer(lexer), lexer->line);
        printf("Msg: %s\n", msg);
        cj_assert(FALSE);
    }

    internal Boolean tryConsumeWord(Lexer* lexer) {
        if (!isalpha(lexer->c)) {
        return FALSE;
        }
    

        while (isalnum(peekNthChar(lexer, 0)) || peekNthChar(lexer, 0) == '_') {
            if (isEOF(lexer)) {
                break;
            }

            consumeNextChar(lexer);
        }

        return TRUE;
    }

    internal void tryConsumeStringLiteral(Lexer* lexer) {
        while (peekNthChar(lexer, 0) != '\"') {
            if (isEOF(lexer)) {
                reportError(lexer, "String literal doesn't have a closing double quote!");
            }

            consumeNextChar(lexer);
        }

        consumeNextChar(lexer);
        addToken(lexer, SPL_TOKEN_STRING_LITERAL);
    }


    internal void tryConsumeCharacterLiteral(Lexer* lexer) {
        if (consumeOnMatch(lexer, '\'')) {
            reportError(lexer, "character literal doesn't have any ascii data in between");
        }


        while (peekNthChar(lexer, 0) != '\'') {
            if (isEOF(lexer)) {
                reportError(lexer, "character literal doesn't have a closing quote!");
            }

            consumeNextChar(lexer);
        }

        consumeNextChar(lexer);
        addToken(lexer, SPL_TOKEN_CHARACTER_LITERAL);
    }


    internal void tryConsumeDigitLiteral(Lexer* lexer) {
        SPL_TokenType kind = SPL_TOKEN_INTEGER_LITERAL;

        while (isdigit(peekNthChar(lexer, 0)) || peekNthChar(lexer, 0) == '.') {
            if (lexer->c == '.') {
                kind = SPL_TOKEN_FLOAT_LITERAL;
            }

            consumeNextChar(lexer);
        }

        addToken(lexer, kind);
    }


    internal Boolean consumeLiteral(Lexer* lexer) {
        if (isdigit(lexer->c)) {
            tryConsumeDigitLiteral(lexer);
            return TRUE;
        } else if (lexer->c == '\"') {
            tryConsumeStringLiteral(lexer);
            return TRUE;
        } else if (lexer->c == '\'') {
            tryConsumeCharacterLiteral(lexer);
            return TRUE;
        } else {
            return FALSE;
        }
    }

    internal Boolean consumeKeyword(Lexer* lexer) {
        if (!tryConsumeWord(lexer)) {
            return FALSE;
        }

        SPL_TokenType token_type = getAcceptedKeyword(lexer);
        if (token_type != SPL_TOKEN_ILLEGAL_TOKEN) {
            addToken(lexer, token_type);
            return TRUE;
        }

        return FALSE;
    }

    internal Boolean consumeSyntax(Lexer* lexer) {
        SPL_TokenType type = getAcceptedSyntax(lexer);
        if (type != SPL_TOKEN_ILLEGAL_TOKEN) {
            addToken(lexer, type);
            return TRUE;
        }

        return FALSE;
    }

    internal void consumeNextToken(Lexer* lexer) {
        lexer->left_pos = lexer->right_pos;
        consumeNextChar(lexer);

        if (consumeWhitespace(lexer)) {}
        else if (consumeLiteral(lexer)) {}
        else if (consumeKeyword(lexer)) {}
        else if (consumeSyntax(lexer)) {}
        else {
            reportError(lexer, "Illegal token found");
        }
    }

    SPL_Token* lexerGenerateTokenStream(Lexer* lexer, char* file_data, u64 file_size) {
        lexer->source = file_data;
        lexer->source_size = file_size;

        while (!isEOF(lexer)) {
            consumeNextToken(lexer);
        }

        cj_vector_push(lexer->tokens, tokenCreate(SPL_TOKEN_EOF, "", lexer->line));
        return lexer->tokens;
    }
#endif

#if defined(CJ_IMPL_PARSING)
    // if you have a { token append JSON

    // if you have "": $value, | append key and make sure there is another one
    // if you have "": $value  | append key

    // if you have 5 append primary
    // if you have true append bool
    // if you have null append JSON_NULL()
    // if you have a [] append JSON_Array
    // if you have ""

    JSON* parseExpression(Parser* parser);

    Parser parserCreate() {
        Parser ret;
        ret.tokens = NULLPTR;
        ret.current = 0;
        ret.tok;
        ret.arena_allocator = ckit_arena_create(KiloBytes(2), "Parser Allocator");

        return ret;
    }

    void parserFree(Parser* parser) {
        ckit_arena_free(parser->arena_allocator);
    }

    internal void consumeNextToken(Parser* parser) {
        parser->tok = parser->tokens[parser->current];
        parser->current += 1;
    }

    internal SPL_Token peekNthToken(Parser* parser, int n) {
        return parser->tokens[parser->current + n];
    }

    internal void reportError(Parser* parser, char* msg) {
        LOG_ERROR("Parser Error: %s | Line: %d\n", peekNthToken(parser, 0).lexeme, peekNthToken(parser, 0).line);
        LOG_ERROR("Msg: %s\n", msg);
        ckit_assert(FALSE);
    }

    internal void expect(Parser* parser, SPL_TokenType expected_type) {
        if (peekNthToken(parser, 0).type != expected_type) {
            LOG_ERROR("Expected: %s | Got: %s", tokenTypeToString(expected_type), peekNthToken(parser, 0).lexeme);
            reportError(parser, "\n");
        }
    }

    internal Boolean consumeOnMatch(Parser* parser, SPL_TokenType expected_type) {
        if (peekNthToken(parser, 0).type == expected_type) {
            consumeNextToken(parser);
            return TRUE;
        }

        return FALSE;
    }

    internal SPL_Token previousToken(Parser* parser) {
        return parser->tokens[parser->current - 1];
    }

    internal JSON* parseJSON(Parser* parser) {
        return parseTerm(parser);
    }

    JSON* parse_json_buffer(Parser* parser, SPL_Token* tokens) {
        parser->current = 0;
        parser->tokens = tokens;
        parser->tok = parser->tokens[parser->current];

        JSON* json = parseJSON(parser);

        return json;
    }
#endif