#include <cj.h>

int main() {
    char* indent = "    ";
    cj_set_context_indent(indent);

    JSON* root = cj_create();
    cj_push(root, "name", "Example");
    cj_push(root, "age", 25);

    JSON* nested = cj_create();
    cj_push(nested, "address1", "San Francisco");
    cj_push(nested, "address2", "California");

    JSON* nested_twice = cj_create();
    cj_push(nested_twice, "KylE", (Boolean)TRUE);
    cj_push(nested_twice, "ANdY", 84);
    cj_push(nested_twice, "Jovanni", 3.141598f);
    cj_push(nested_twice, "Hykes", (Boolean)FALSE);

    cj_push(nested, "Names", nested_twice);
    cj_push(root, "address", nested);

    JSON* hobbies = cj_array_create();
    cj_array_push(hobbies, (Boolean)TRUE);
    cj_array_push(hobbies, "Coding");
    cj_array_push(hobbies, JSON_NULL());
    cj_push(root, "hobbies", hobbies);

    char* str = json_to_string(root);
    printf("%s\n", str);
    free(str);

    // u8* source_data = cj_os_read_entire_file(file_path, &file_size);

    char* test_json = "{ \"test\": -5 }";
    Lexer lexer = lexerCreate();
    SPL_Token* token_stream = lexerGenerateTokenStream(&lexer, test_json, cj_cstr_length(test_json));

    printf("------------ LEXICAL ANALYSIS ------------\n");
    for (u64 i = 0; i < cj_vector_count(token_stream); i++ ) {
        SPL_Token token = token_stream[i];
        printf("%s(%s) | Line: %llu\n", tokenTypeToString(token.type), token.lexeme, token.line);
    }

    return 0;
}