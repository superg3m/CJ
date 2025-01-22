#pragma once

#ifdef __cplusplus
    #define CKG_API extern "C"
#else 
    #define CKG_API
#endif 

#if defined(CKG_IMPL)
    #define CKG_IMPL_TYPES
    #define CKG_IMPL_ARENA
    #define CKG_IMPL_CSTRING
    #define CKG_IMPL_CHAR
#endif

#define CKG_INCLUDE_TYPES
#define CKG_INCLUDE_ARENA
#define CKG_INCLUDE_CSTRING
#define CKG_INCLUDE_CHAR

#if defined(CKG_INCLUDE_TYPES)
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
#endif

#if defined(CKG_INCLUDE_ARENA)
    #define CKG_ARENA_FLAG_FIXED 0
    #define CKG_ARENA_FLAG_CIRCULAR 1
    #define CKG_ARENA_FLAG_EXTENDABLE_PAGES 2
    #define CKG_ARENA_FLAG_COUNT 3
    typedef u8 CKG_ArenaFlag;
    typedef struct CKG_Arena CKG_Arena;


    CKG_API CKG_Arena* MACRO_ckg_arena_create(size_t allocation_size, const char* name, CKG_ArenaFlag flag, u8 alignment);
    CKG_API void* MACRO_ckg_arena_push(CKG_Arena* arena, size_t element_size);	
    CKG_API CKG_Arena* MACRO_ckg_arena_free(CKG_Arena* arena);
    CKG_API void ckg_arena_clear(CKG_Arena* arena);

    #define ckg_arena_create(allocation_size, name) MACRO_ckg_arena_create(allocation_size, name, CKG_ARENA_FLAG_EXTENDABLE_PAGES, 0)
    #define ckg_arena_create_custom(allocation_size, name, flags, alignment) MACRO_ckg_arena_create(allocation_size, name, flags, alignment)
    #define ckg_arena_free(arena) arena = MACRO_ckg_arena_free(arena)
    #define ckg_arena_push(arena, type) ((type*)MACRO_ckg_arena_push(arena, sizeof(type)))
    #define ckg_arena_push_array(arena, type, element_count) ((type*)MACRO_ckg_arena_push(arena, sizeof(type) * element_count))
#endif

#if defined(CKG_INCLUDE_CSTRING)
    // types
	/**
	 * @brief returns a string buffer with nullterm
	 * must free with ckg_free()
	 * @param s1 
	 * @return char* 
	 */
	CKG_API char* ckg_cstr_alloc(const char* s1);

	/**
	 * @brief Requires the string buffer to be cleared to zero terminated
	 * 
	 * @param string_buffer 
	 * @param string_buffer_size 
	 * @param source 
	 */
	CKG_API void ckg_cstr_append(char* string_buffer, size_t string_buffer_capacity, const char* to_append);
	CKG_API void ckg_cstr_append_char(char* string_buffer, size_t string_buffer_capacity, const char to_append);

	/**
	 * @brief Requires the string buffer to be cleared to zero terminated
	 * 
	 * @param string_buffer 
	 * @param string_buffer_size 
	 * @param index 
	 */
	CKG_API void ckg_cstr_insert(char* string_buffer, size_t string_buffer_capacity, const char* to_insert, const u32 index);
	CKG_API void ckg_cstr_insert_char(char* string_buffer, size_t string_buffer_capacity, const char to_insert, const u32 index);
	
	/**
	 * @brief Requires the string buffer to be cleared to zero, modifies string_buffer
	 * 
	 * 
	 * @param string_buffer 
	 * @param string_buffer_size 
	 */
	CKG_API void ckg_cstr_copy(char* string_buffer, size_t string_buffer_capacity, const char* to_copy);

	/**
	 * @brief generate a random string and copy it to the dest pointer
	 * 
	 * @param dest 
	 * @param length 
	 */
	CKG_API void ckg_cstr_random(char* dest, size_t length);
	
	/**
	 * @brief Tests each charater in the string for equaility
	 * returns TRUE(1) if equal and FALSE(0) if not equal
	 * => if (ckg_cstr_equal("hi", "hi"))
	 * 
	 * 
	 * @param s1 
	 * @param s2 
	 * @return Boolean 
	 */
	CKG_API Boolean ckg_cstr_equal(const char* s1, const char* s2);
	CKG_API u32 ckg_cstr_length(const char* c_string);
	CKG_API void ckg_cstr_clear(char* string_buffer);
	// Inclusive start and end STR: SHOW | 0, 0 is: S | 0, 1 is: SH
	CKG_API void ckg_substring(const char* string_buffer, char* returned_buffer, u32 start_range, u32 end_range);
	CKG_API Boolean ckg_cstr_contains(const char* string_buffer, const char* contains);
	CKG_API u32 ckg_cstr_index_of(const char* string_buffer, const char* sub_string);
	CKG_API u32 ckg_cstr_last_index_of(const char* string_buffer, const char* sub_string);
	CKG_API Boolean ckg_cstr_starts_with(const char* string_buffer, const char* starts_with);
	CKG_API Boolean ckg_cstr_ends_with(const char* string_buffer, const char* ends_with);
	CKG_API void ckg_cstr_reverse(const char* str, char* returned_reversed_string_buffer, size_t reversed_buffer_capacity);
	CKG_API void ckg_cstr_int_to_cstr(char* string_buffer, size_t string_buffer_capacity, int number);
#endif

#if defined(CKG_INCLUDE_CHAR)
    CKG_API Boolean ckg_char_is_digit(char c);
    CKG_API Boolean ckg_char_is_alpha(char c);
    CKG_API Boolean ckg_char_is_alpha_numeric(char c);
    CKG_API Boolean ckg_char_is_upper(char c);
    CKG_API Boolean ckg_char_is_lower(char c);
#endif

#if defined(CKG_INCLUDE_MATH)
    #include <math.h>
    // more here later
    // round intrinsic
    // trig fucntion intrinsics or approximations with taylor series
#endif

#if defined(CKG_INCLUDE_COLLECTIONS)
    //
    // ========== START CKG_VECTOR ==========
    //
    typedef struct CKG_VectorHeader {
        u32 count;
        u32 capacity;
    } CKG_VectorHeader;

    CKG_API void* ckg_vector_grow(void* vector, size_t element_size);

    #define VECTOR_DEFAULT_CAPACITY 1
    #define ckg_vector_header_base(vector) ((CKG_VectorHeader*)(((u8*)vector) - sizeof(CKG_VectorHeader)))
    #define ckg_vector_count(vector) (*ckg_vector_header_base(vector)).count
    #define ckg_vector_capacity(vector) (*ckg_vector_header_base(vector)).capacity

    #ifdef __cplusplus
        #define ckg_vector_push(vector, element) vector = (decltype(vector))ckg_vector_grow(vector, sizeof(element)); vector[ckg_vector_header_base(vector)->count++] = element
    #else 
        #define ckg_vector_push(vector, element) vector = ckg_vector_grow(vector, sizeof(element)); vector[ckg_vector_header_base(vector)->count++] = element
    #endif
    
    #define ckg_vector_free(vector) ckg_free(ckg_vector_header_base(vector))
    //
    // ========== END CKG_VECTOR ==========
    //


    //
    // ========== START CKG_LinkedList ==========
    //
    typedef struct CKG_Node {
        struct CKG_Node* next;
        struct CKG_Node* prev;
        size_t element_size_in_bytes;
        void* data;
    } CKG_Node;

    typedef struct CKG_LinkedList {
        CKG_Node* head;
        CKG_Node* tail;
        size_t element_size_in_bytes;
        u32 count;
        Boolean is_pointer_type;
    } CKG_LinkedList;

    CKG_API CKG_LinkedList* MACRO_ckg_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type);
    CKG_API CKG_Node* ckg_linked_list_insert(CKG_LinkedList* linked_list, u32 index, void* data);
    CKG_API CKG_Node* ckg_linked_list_get_node(CKG_LinkedList* linked_list, u32 index);
    CKG_API void* ckg_linked_list_get(CKG_LinkedList* linked_list, u32 index);
    CKG_API void* ckg_linked_list_peek_head(CKG_LinkedList* linked_list);
    CKG_API void* ckg_linked_list_peek_tail(CKG_LinkedList* linked_list);
    CKG_API CKG_Node* ckg_linked_list_push(CKG_LinkedList* linked_list, void* data);
    CKG_API CKG_Node ckg_linked_list_pop(CKG_LinkedList* linked_list);
    CKG_API CKG_Node ckg_linked_list_remove(CKG_LinkedList* linked_list, u32 index);
    CKG_API void* MACRO_ckg_linked_list_free(CKG_LinkedList* linked_list);
    CKG_API u32 ckg_linked_list_node_to_index(CKG_LinkedList* linked_list, CKG_Node* address);

    #define ckg_linked_list_create(type, is_pointer_type) MACRO_ckg_linked_list_create(sizeof(type), is_pointer_type)

    #ifdef __cplusplus
        #define ckg_linked_list_free(linked_list) linked_list = (decltype(linked_list))MACRO_ckg_linked_list_free(linked_list)
    #else 
        #define ckg_linked_list_free(linked_list) linked_list = MACRO_ckg_linked_list_free(linked_list)
    #endif
    
    //
    // ========== END CKG_VECTOR ==========
    //
#endif 

//
// ===================================================== CKG_IMPL =====================================================
//

#if defined(CKG_IMPL_ARENA)
    #define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

    typedef struct CKG_ArenaPage {
        u8* base_address;
        u64 capacity;
        u64 used;
    } CKG_ArenaPage;

    typedef struct CKG_Arena {
        const char* name;
        CKG_LinkedList* pages;
        CKG_ArenaFlag flag;
        u8 alignment;
    } CKG_Arena;

    Boolean ckg_CKG_ARENA_FLAG_is_set(CKG_Arena* arena, CKG_ArenaFlag flag) {
        return arena->flag == flag;
    }

    internal CKG_ArenaPage* ckg_arena_page_create(size_t allocation_size) {
        CKG_ArenaPage* ret = (CKG_ArenaPage*)ckg_alloc(sizeof(CKG_ArenaPage));
        ret->used = 0;
        ret->capacity = allocation_size;
        ret->base_address = (u8*)ckg_alloc(allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE);

        return ret;
    }

    CKG_Arena* MACRO_ckg_arena_create(size_t allocation_size, const char* name, CKG_ArenaFlag flag, u8 alignment) {
        CKG_Arena* arena = (CKG_Arena*)ckg_alloc(sizeof(CKG_Arena));
        arena->alignment = alignment == 0 ? 8 : alignment;
        arena->name = name;
        arena->flag = flag;
        arena->pages = ckg_linked_list_create(CKG_ArenaPage*, TRUE);
        CKG_ArenaPage* inital_page = ckg_arena_page_create(allocation_size);
        ckg_linked_list_push(arena->pages, inital_page);

        return arena;
    }

    CKG_Arena* MACRO_ckg_arena_free(CKG_Arena* arena) {
        ckg_assert(arena);

        for (u32 i = 0; i < arena->pages->count; i++) {
            CKG_ArenaPage* page = (CKG_ArenaPage*)ckg_linked_list_remove(arena->pages, 0).data;
            ckg_assert(page->base_address);
            ckg_free(page->base_address);
            ckg_free(page);
        }
        ckg_linked_list_free(arena->pages);
        ckg_free(arena);

        return arena;
    }

    void ckg_arena_clear(CKG_Arena* arena) {
        ckg_assert(arena);

        for (u32 i = 0; i < arena->pages->count; i++) {
            CKG_ArenaPage* page = (CKG_ArenaPage*)ckg_linked_list_get(arena->pages, i);
            ckg_assert(page->base_address);
            ckg_memory_zero(page->base_address, page->used);
            page->used = 0;
        }
    }

    void* MACRO_ckg_arena_push(CKG_Arena* arena, size_t element_size) {
        ckg_assert(arena);

        CKG_ArenaPage* last_page = (CKG_ArenaPage*)ckg_linked_list_peek_tail(arena->pages);
        if (ckg_CKG_ARENA_FLAG_is_set(arena, CKG_ARENA_FLAG_FIXED)) { // single page assert if you run out of memory
            ckg_assert((last_page->used + element_size <= last_page->capacity));
        } else if (ckg_CKG_ARENA_FLAG_is_set(arena, CKG_ARENA_FLAG_CIRCULAR)) { // single page circle around if you run out of memory
            if ((last_page->used + element_size > last_page->capacity)) {
                last_page->used = 0;
                ckg_assert((last_page->used + element_size <= last_page->capacity));
            }
        } else if (ckg_CKG_ARENA_FLAG_is_set(arena, CKG_ARENA_FLAG_EXTENDABLE_PAGES)) {
            ckg_assert(last_page->base_address);
            if ((last_page->used + element_size > last_page->capacity)) {
                CKG_ArenaPage* next_page = ckg_arena_page_create((last_page->capacity + element_size) * 2);
                ckg_assert(next_page->base_address);
                ckg_linked_list_push(arena->pages, next_page);
            }
        } else {
            ckg_assert(FALSE);
        }

        last_page = (CKG_ArenaPage*)ckg_linked_list_peek_tail(arena->pages); // tail might change

        u8* ret = last_page->base_address + last_page->used;
        last_page->used += element_size;
        if ((last_page->used & (arena->alignment - 1)) != 0) { // if first bit is set then its not aligned
            last_page->used += (arena->alignment - (last_page->used & (arena->alignment - 1)));
        }

        return ret;
    }
#endif

#if defined(CKG_IMPL_CSTRING)
    u32 ckg_cstr_length(const char* cstring) {
        ckg_assert(cstring);

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

    Boolean ckg_cstr_equal(const char* s1, const char* s2) {
        ckg_assert(s1);
        ckg_assert(s2);

        u32 s1_length = ckg_cstr_length(s1);
        u32 s2_length = ckg_cstr_length(s2);

        return ckg_memory_compare(s1, s2, s1_length, s2_length);
    }

    void ckg_cstr_insert(char* str, size_t str_capacity, const char* to_insert, const u32 index) {
        ckg_assert(str);
        ckg_assert(to_insert);

        const u32 str_length = ckg_cstr_length(str);
        const u32 to_insert_length = ckg_cstr_length(to_insert);

        const u32 new_length = str_length + to_insert_length;

        ckg_assert(index >= 0 && index <= str_length);
        ckg_assert_msg(new_length < str_capacity, "string_insert: str_capacity is %lld but new valid cstring length is %d + %d + 1(null_term)= %d\n", str_capacity, str_length, to_insert_length, new_length + 1);
        u8* move_source_ptr = (u8*)(str + index);
        u8* move_dest_ptr = (u8*)(move_source_ptr + to_insert_length);

        ckg_memory_copy(move_source_ptr, move_dest_ptr, str_length - index, str_capacity - (index + to_insert_length));
        
        u8* copy_dest_ptr = (u8*)(str + index);
        ckg_memory_copy(to_insert, copy_dest_ptr, to_insert_length, str_capacity);
    }

    void ckg_cstr_insert_char(char* str, size_t str_capacity, const char to_insert, const u32 index) {
        ckg_assert(str);
        ckg_assert(to_insert);

        u32 str_length = ckg_cstr_length(str);
        u32 source_length = 1;

        ckg_assert(index >= 0 && index <= str_length);
        Boolean expression = str_length + source_length < str_capacity;
        ckg_assert_msg(expression, "ckg_cstr_insert_char: str overflow new_capacity_required: %d >= current_capacity: %lld\n",  str_length + source_length, str_capacity);

        char* source_ptr = str + index;

        ckg_memory_copy(source_ptr, source_ptr + 1, str_length - index, str_capacity - (index + 1));
        str[index] = to_insert;
    }

    void ckg_cstr_append(char* str, size_t str_capacity, const char* to_append) {
        u32 str_length = ckg_cstr_length(str);
        ckg_cstr_insert(str, str_capacity, to_append, str_length);
    }

    void ckg_cstr_append_char(char* str, size_t str_capacity, const char to_append) {
        u32 str_length = ckg_cstr_length(str);
        ckg_cstr_insert_char(str, str_capacity, to_append, str_length);
    }

    void ckg_cstr_clear(char* str) {
        ckg_assert(str);

        size_t str_length = ckg_cstr_length(str); 
        ckg_memory_zero(str, str_length);
    }

    void ckg_cstr_copy(char* str, size_t str_capacity, const char* to_copy) {
        ckg_assert(to_copy);
        ckg_assert(str);

        u32 source_length = ckg_cstr_length(to_copy);
        ckg_cstr_clear(str);

        ckg_memory_copy(to_copy, str, source_length + 1, str_capacity);
    }

    void ckg_cstr_random(char *dest, size_t length) {
        char charset[] = "0123456789"
                        "abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        while (length-- > 0) {
            u64 index = rand() % (sizeof(charset) - 1);
            *dest++ = charset[index];
        }
        *dest = '\0';
    }

    void ckg_substring(const char* str, char* returned_buffer, u32 start, u32 end) {
        ckg_assert(str);
        ckg_assert(returned_buffer);
        size_t str_length = ckg_cstr_length(str); 

        Boolean start_check = (start >= 0) && (start <= str_length - 1);
        Boolean end_check = (end >= 0) && (end <= str_length - 1);

        ckg_assert_msg(start_check, "ckg_substring: Start range is outside expected range: [%d - %lld] got: %d\n", 0, str_length - 1, start);
        ckg_assert_msg(end_check, "ckg_substring: End range is outside expected range: [%d - %lld] got: %d\n", 0, str_length - 1, end);
        ckg_assert_msg(start <= end, "ckg_substring: Start range is greater than end range[start: %d > end: %d]\n", start, end);

        //char* str = "hello"
        //0 - 4 = hello\0 = 6
        //0 - 0 = h\0 = 2
        //0 - 1 = he\0 = 3
        //1 - 4 = ello\0 = 5

        u32 counter = 0;
        for (u32 i = start; i <= end; i++) {
            returned_buffer[counter++] = str[i];
        }
        returned_buffer[counter] = '\0'; 
    }

    Boolean ckg_cstr_contains(const char* str, const char* contains) {
        ckg_assert(str);
        ckg_assert(contains);

        size_t str_length = ckg_cstr_length(str); 
        size_t contains_length = ckg_cstr_length(contains);

        if (str_length == 0 && contains_length == 0) {
            return TRUE;
        } else if ((contains_length == 0) || (str_length == 0)) {
            return FALSE;
        }

        if (contains_length > str_length) {
            return FALSE;
        }

        // "\0" = 0
        // "a\0" = 0
        // "fss\0" = 2
        
        Boolean contains_substring = FALSE;
        for (u32 i = 0; !contains_substring && (i <= str_length - contains_length); i++) {
            if (str[i] != contains[0]) {
                continue;
            }

            u32 end_index = (u32)(i + (contains_length - 1));
            if (end_index > str_length) {
                break;
            }

            char* temp_string = (char*)ckg_alloc(contains_length + 1);
            ckg_substring(str, temp_string, i, end_index);
            if (ckg_cstr_equal(temp_string, contains)) {
                contains_substring = TRUE;
            }
            ckg_free(temp_string);
        }

        return contains_substring;
    }

    u32 ckg_cstr_index_of(const char* str, const char* sub_string) {
        ckg_assert(str);
        ckg_assert(sub_string);
        
        size_t str_length = ckg_cstr_length(str); 
        size_t contains_length = ckg_cstr_length(sub_string);

        if (str_length == 0 && contains_length == 0) {
            return 0;
        } else if (contains_length == 0) {
		    ckg_assert_msg(FALSE, "Substring is empty\n");		
            return 0; // Never gets here
        } else if (str_length == 0) {
			ckg_assert_msg(FALSE, "String is empty\n");		
            return 0; // Never gets here
        }

        if (contains_length > str_length) {
        	ckg_assert_msg(FALSE, "Can't find substring %s in string %s\n", sub_string, str);		
            return 0; // Never gets here
        }
        
        s32 ret_index = -1;
        for (u32 i = 0; i <= str_length - contains_length; i++) {
            if (ret_index != -1) {
                break;
            }
            
            if (str[i] != sub_string[0]) {
                continue;
            }

            s32 end_index = (u32)(i + (contains_length - 1));
            if (end_index > str_length) {
                break;
            }

            char* temp_string = (char*)ckg_alloc((end_index - i) + 2);
            ckg_substring(str, temp_string, i, end_index);
            if (ckg_cstr_equal(temp_string, sub_string)) {
                ret_index = i;
            }
            ckg_free(temp_string);
        }

        if (ret_index < 0) {
            ckg_assert_msg(FALSE, "Can't find substring %s in string %s\n", sub_string, str);		
            return 0; // Never gets here
        }

        return ret_index;
    }

    u32 ckg_cstr_last_index_of(const char* str, const char* sub_string) {
        ckg_assert(str);
        ckg_assert(sub_string);
        
        size_t str_length = ckg_cstr_length(str); 
        size_t contains_length = ckg_cstr_length(sub_string);

        if (str_length == 0 && contains_length == 0) {
            return 0;
        } else if (contains_length == 0) {
			ckg_assert_msg(FALSE, "Substring is empty\n");		
            return 0; // Never gets here
        } else if (str_length == 0) {
			ckg_assert_msg(FALSE, "String is empty\n");		
            return 0; // Never gets here
        }

        if (contains_length > str_length) {
            ckg_assert_msg(FALSE, "Can't find substring %s in string %s\n", sub_string, str);		
            return 0; // Never gets here
        }
        
        s32 ret_index = -1;
        for (u32 i = 0; i <= str_length - contains_length; i++) {
            if (str[i] != sub_string[0]) {
                continue;
            }

            s32 end_index = (u32)(i + (contains_length - 1));
            if (end_index > str_length) {
                break;
            }

            char* temp_string = (char*)ckg_alloc((end_index - i) + 1);
            ckg_substring(str, temp_string, i, end_index);
            if (ckg_cstr_equal(temp_string, sub_string)) {
                ret_index = i;
            }
            ckg_free(temp_string);
        }

        if (ret_index < 0) {
            ckg_assert_msg(FALSE, "Can't find substring %s in string %s\n", sub_string, str);		
            return 0; // Never gets here
        }

        return ret_index;
    }

    Boolean ckg_cstr_starts_with(const char* str, const char* starts_with) {
        ckg_assert(str);
        ckg_assert(starts_with);
        
        u32 str_length = ckg_cstr_length(str); 
        u32 starts_with_length = ckg_cstr_length(starts_with);

        if (str_length == 0 && starts_with_length == 0) {
            return TRUE;
        } else if (str[0] != starts_with[0] || str_length < starts_with_length) {
            return FALSE;
        }

        Boolean starts_with_substring = FALSE;
        char* temp_string = (char*)ckg_alloc(starts_with_length + 1);
        ckg_substring(str, temp_string, (u32)0, starts_with_length - 1);
        if (ckg_cstr_equal(temp_string, starts_with)) {
            starts_with_substring = TRUE;
        }
        ckg_free(temp_string);

        return starts_with_substring;
    }

    Boolean ckg_cstr_ends_with(const char* str, const char* ends_with) {
        ckg_assert(str);
        ckg_assert(ends_with);

        u32 str_length = ckg_cstr_length(str); 
        u32 ends_with_length = ckg_cstr_length(ends_with);

        if ((str_length == 0) || (ends_with_length == 0)) {
            return TRUE;
        } else if (str_length < ends_with_length) {
            return FALSE;
        }

        u32 start_index = MAX((s32)str_length - 1, 0) - MAX((s32)ends_with_length - 1, 0);
        if (str[start_index] != ends_with[0]) {
            return FALSE;
        }

        Boolean starts_with_substring = FALSE;
        char* temp_string = (char*)ckg_alloc(str_length - start_index + 1);
        ckg_substring(str, temp_string, start_index, str_length - 1);
        if (ckg_cstr_equal(temp_string, ends_with)) {
            starts_with_substring = TRUE;
        }
        ckg_free(temp_string);

        return starts_with_substring;
    }

    void ckg_cstr_reverse(const char* str, char* returned_reversed_string_buffer, size_t reversed_buffer_capacity) {
        ckg_assert(str);

        u32 str_length = ckg_cstr_length(str);
        ckg_assert(reversed_buffer_capacity > str_length);
        u32 str_guarenteed_capacity = str_length + 1;

        for (int i = str_length - 1; i >= 0; i--) {
            ckg_cstr_append_char(returned_reversed_string_buffer, str_guarenteed_capacity, str[i]);
        }
    }

    void ckg_cstr_int_to_cstr(char* string_buffer, size_t string_buffer_capacity, int number) {
        while (number != 0) {
            char c = '0' + (number % 10);
            ckg_cstr_insert_char(string_buffer, string_buffer_capacity, c, 0);
            number /= (int)10;
        }
    }
#endif

#if defined(CKG_IMPL_CHAR)
    Boolean ckg_char_is_digit(char c) {
        return (c >= '0' && c <= '9');
    }

    Boolean ckg_char_is_upper(char c) {
        return (c >= 'A' && c <= 'Z');
    }

    Boolean ckg_char_is_lower(char c) {
        return (c >= 'a' && c <= 'z');
    }

    Boolean ckg_char_is_alpha(char c) {
        return ckg_char_is_lower(c) || ckg_char_is_upper(c) ;
    }

    Boolean ckg_char_is_alpha_numeric(char c) {
        return ckg_char_is_alpha(c) || ckg_char_is_digit(c);
    }
#endif

#if defined(CKG_IMPL_COLLECTIONS)
    //
    // ========== START CKG_VECTOR ==========
    //
    void* ckg_vector_grow(void* vector, size_t element_size) {
        if (vector == NULLPTR) {
            vector = ckg_alloc(sizeof(CKG_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size));
            vector = (u8*)vector + sizeof(CKG_VectorHeader);
            ckg_vector_capacity(vector) = VECTOR_DEFAULT_CAPACITY;
        }

        u32 count = ckg_vector_count(vector);
        u32 capactiy = ckg_vector_capacity(vector);

        if (capactiy < count + 1) {
            size_t old_allocation_size = sizeof(CKG_VectorHeader) + (capactiy * element_size);
            u32 new_capactiy = capactiy * 2;
            size_t new_allocation_size = sizeof(CKG_VectorHeader) + (new_capactiy * element_size);

            vector = ckg_realloc(ckg_vector_header_base(vector), old_allocation_size, new_allocation_size);
            vector = (u8*)vector + sizeof(CKG_VectorHeader);

            ckg_vector_header_base(vector)->count = count;
            ckg_vector_header_base(vector)->capacity = new_capactiy;
        }

        return vector;
    }
    //
    // ========== END CKG_VECTOR ==========
    //

    //
    // ========== START CKG_LinkedList ==========
    //
    CKG_LinkedList* MACRO_ckg_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type) {
        CKG_LinkedList* ret = (CKG_LinkedList*)ckg_alloc(sizeof(CKG_LinkedList));
        ret->count = 0;
        ret->element_size_in_bytes = element_size_in_bytes;
        ret->head = NULLPTR;
        ret->tail = NULLPTR;
        ret->is_pointer_type = is_pointer_type;
        return ret;
    }

    CKG_Node* MACRO_ckg_node_create(CKG_LinkedList* linked_list, void* data) {
        CKG_Node* ret = (CKG_Node*)ckg_alloc(sizeof(CKG_Node));
        if (linked_list->is_pointer_type) {
            ret->data = data;
        } else {
            ret->data = ckg_alloc(linked_list->element_size_in_bytes); // user has to free
            ckg_memory_copy(data, ret->data, linked_list->element_size_in_bytes, linked_list->element_size_in_bytes); 
        }

        ret->element_size_in_bytes = linked_list->element_size_in_bytes;
        ret->next = NULLPTR;
        ret->prev = NULLPTR;
        return ret;
    }
    #define ckg_node_create(linked_list, data) MACRO_ckg_node_create(linked_list, data)

    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CKG_Node* 
     */
    internal CKG_Node* MACRO_ckg_node_free(CKG_LinkedList* linked_list, CKG_Node* node) {
        ckg_assert(linked_list);
        ckg_assert(node);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        ckg_free(node);

        return node;
    }
    #define ckg_node_free(linked_list, node) node = MACRO_ckg_node_free(linked_list, node)


    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CKG_Node* 
     */
    internal CKG_Node* MACRO_ckg_node_data_free(CKG_LinkedList* linked_list, CKG_Node* node) {
        ckg_assert(linked_list);
        ckg_assert(node);
        ckg_assert(node->data);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        if (!linked_list->is_pointer_type) {
            ckg_free(node->data);
        }
        ckg_free(node);

        return node;
    }
    #define ckg_node_data_free(linked_list, node) node = MACRO_ckg_node_data_free(linked_list, node)

    CKG_Node* ckg_linked_list_insert(CKG_LinkedList* linked_list, u32 index, void* data) {
        ckg_assert(linked_list);
        ckg_assert(data);
        ckg_assert(index >= 0);

        u32 old_count = linked_list->count++;
        if (linked_list->head == NULLPTR) { // there is not head and by definition no tail
            CKG_Node* new_node_to_insert = ckg_node_create(linked_list, data);
            linked_list->head = new_node_to_insert;
            linked_list->tail = new_node_to_insert;

            return linked_list->head;
        }

        ckg_assert(index <= old_count);
        CKG_Node* new_node_to_insert = ckg_node_create(linked_list, data);

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
        CKG_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        new_node_to_insert->prev = current_node;
        new_node_to_insert->next = current_node->next;

        current_node->next->prev = new_node_to_insert;
        current_node->next = new_node_to_insert;

        return new_node_to_insert;
    }

    CKG_Node* ckg_linked_list_get_node(CKG_LinkedList* linked_list, u32 index) {
        ckg_assert(linked_list);
        CKG_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        return current_node;
    }

    void* ckg_linked_list_get(CKG_LinkedList* linked_list, u32 index) {
        return ckg_linked_list_get_node(linked_list, index)->data;
    }

    void* ckg_linked_list_peek_head(CKG_LinkedList* linked_list) {
        return ckg_linked_list_get_node(linked_list, 0)->data;
    }

    void* ckg_linked_list_peek_tail(CKG_LinkedList* linked_list) {
        return ckg_linked_list_get_node(linked_list, linked_list->count - 1)->data;
    }

    CKG_Node* ckg_linked_list_push(CKG_LinkedList* linked_list, void* data) {
        return ckg_linked_list_insert(linked_list, linked_list->count, data);
    }

    u32 ckg_linked_list_node_to_index(CKG_LinkedList* linked_list, CKG_Node* address) {
        CKG_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < linked_list->count + 1; i++) {
            if (current_node == address) {
                return i;
            }
            current_node = current_node->next;
        }

        ckg_assert(FALSE); // couldn't match a node to an address
        return 0; // should never get here
    }

    CKG_Node ckg_linked_list_pop(CKG_LinkedList* linked_list) {
        return ckg_linked_list_remove(linked_list, linked_list->count - 1);
    }

    CKG_Node ckg_linked_list_remove(CKG_LinkedList* linked_list, u32 index) {
        ckg_assert(linked_list); 
        ckg_assert(linked_list->count > 0); 
        ckg_assert(index >= 0);

        u32 old_count = linked_list->count--;
        if (index == 0 && old_count == 1) { // removing the head fully
            CKG_Node ret = *linked_list->head;
            ckg_node_free(linked_list, linked_list->head);
            linked_list->head = NULLPTR;

            return ret;
        }

        if (index == 0) { // remove head
            CKG_Node* cached_head = linked_list->head;
            linked_list->head = linked_list->head->next;
            CKG_Node ret = *cached_head; 
            ckg_node_free(linked_list, cached_head);

            return ret;
        }

        ckg_assert(index < old_count);
        if (index == (old_count - 1)) { // remove tail
            CKG_Node* cached_tail = linked_list->tail;
            linked_list->tail = linked_list->tail->prev;
            CKG_Node ret = *cached_tail; 
            ckg_node_free(linked_list, cached_tail);

            return ret;
        }

        CKG_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        current_node->next->prev = current_node->prev;
        current_node->prev->next = current_node->next;
        CKG_Node ret = *current_node; 
        ckg_node_free(linked_list, current_node);

        return ret;
    }

    void* MACRO_ckg_linked_list_free(CKG_LinkedList* linked_list) {
        ckg_assert(linked_list); 
        CKG_Node* current_node = linked_list->head; 
        CKG_Node* next_node = NULLPTR; 
        for (u32 i = 0; i < linked_list->count; i++) {
            next_node = current_node->next;
            ckg_node_data_free(linked_list, current_node);
            current_node = next_node;
        }
        ckg_free(linked_list);

        return linked_list;
    }
    //
    // ========== END CKG_LinkedList ==========
    //
#endif

#if defined(CKG_IMPL_MATH)
    // Nothing
#endif

// ------------------------------------------------------------------------

#define CJ_INCLUDE_TYPES
#define CJ_INCLUDE_BUILD
#define CJ_INCLUDE_PRINT