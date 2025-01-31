#include <cj.h>

#if 0
    int main() {
        char* indent = "    ";
        cj_set_context_indent(indent);

        CJ_Arena* arena = cj_arena_create(0);
        JSON* root = cj_create(arena);
        cj_push(root, "name", "Example");
        cj_push(root, "age", 25.0123);

        JSON* nested = cj_create(arena);
        cj_push(nested, "address1", "San Francisco");
        cj_push(nested, "address2", "California");

        JSON* nested_twice = cj_create(arena);
        cj_push(nested_twice, "KylE", (Boolean)TRUE);
        cj_push(nested_twice, "ANdY", 84);
        cj_push(nested_twice, "Jovanni", 3.0141598);
        cj_push(nested_twice, "Hykes", (Boolean)FALSE);
        cj_push(nested_twice, "Conrad", JSON_NULL(arena));

        cj_push(nested, "Names", nested_twice);
        cj_push(root, "address", nested);

        JSON* hobbies = cj_array_create(arena);
        cj_array_push(hobbies, nested_twice);
        cj_array_push(hobbies, (Boolean)TRUE);
        cj_array_push(hobbies, "Coding");
        cj_push(root, "hobbies", hobbies);

        char* str = cj_to_string(root);
        printf("%s\n", str);

        // u8* source_data = cj_os_read_entire_file(file_path, &file_size);

        char* test_json = "{\"BinaryOp\": {\"op\": \"+\", \"\": {\"Grouping\": {\"BinaryOp\": {\"op\": \"*\", \"left\": [null, false, 7, 23.51, true, { \"test\": \"should_be\" }], \"right\": 123}}}, \"right\": {\"Grouping\": {\"BinaryOp\": {\"op\": \"/\", \"left\": 2, \"right\": {\"Grouping\": {\"BinaryOp\": {\"op\": \"+\", \"left\": 45.234001, \"right\": 5}}}}}}}}";

        printf("------------ PARSING ------------\n");
        JSON* json_test_parse = cj_parse(arena, test_json);
        char* str2 = cj_to_string(json_test_parse);
        printf("%s\n", str2);


        free(str);
        free(str2);
        cj_arena_free(arena);

        return 0;
    }
#else
    int main() {
        char src[1<<14] = {0};
        memset(src, '[', sizeof(src)-1);
        
        CJ_Arena *a = cj_arena_create(0);
        
        JSON* parsed = cj_parse(a, src);
        char* formatted = cj_to_string(parsed);
        printf("%s\n", formatted);

        cj_arena_free(a);
    }
#endif