#include <cj.h>

int main() {
    // char* indent = "    "
    // json_set_context_format(indent);

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

    // JSON* hobbies = cj_array_create();
    // cj_array_push(hobbies, JSON_STRING("Reading"));
    // cj_array_push(hobbies, JSON_STRING("Coding"));
    // cj_array_push(hobbies, JSON_STRING("Hiking"));
    // cj_push(root, "hobbies", hobbies);

    char* str = json_to_string(root);
    printf("%s\n", str);

    //char *json_string = json_print(root);
    //printf("%s\n", json_string);

    //free(json_string);
    // CJ_END()

    return 0;
}