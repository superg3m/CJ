#include <cj.h>

int main() {
    // char* indent = "    "
    // json_set_context_format(indent);

    // CJ_START()
    JSON* root = cj_create();
    cj_push(root, "name", JSON_STRING("Example"));
    cj_push(root, "age", JSON_INT(25));

    JSON* nested = cj_create();
    cj_push(nested, "address1", JSON_STRING("San Francisco"));
    cj_push(nested, "address2", JSON_STRING("California"));

    cj_push(root, "address", nested);

    JSON* hobbies = cj_array_create();
    cj_array_push(hobbies, JSON_STRING("Reading"));
    cj_array_push(hobbies, JSON_STRING("Coding"));
    cj_array_push(hobbies, JSON_STRING("Hiking"));
    cj_push(root, "hobbies", hobbies);

    char* str = json_to_string(root);
    printf("%s\n", str);

    //char *json_string = json_print(root);
    //printf("%s\n", json_string);

    //free(json_string);
    // CJ_END()

    return 0;
}