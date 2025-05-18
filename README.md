# CJ - A Lightweight JSON Parser for C

### This project started out because I didn't like that cJSON doesn't let you choose a custom indent

**CJ** is a lightweight and intuitive JSON parser and builder for C. It supports nested objects, arrays, and various JSON value types, making it simple to work with JSON data in your C projects.

## Features
- Easy-to-use API for creating and manipulating JSON objects as well as pretty printing json
- Supports nested JSON structures.
- Converts JSON objects to formatted strings for easy readability.
- Parses JSON strings into structured data.
- Arena-based memory management for efficient and controlled allocations.

## Cons (I hacked this together in 2 days so forgive my sins)
- Not optimized at all
- Error handling is none-existant your program will likely crash on asserts if you give it malformed input. (This is being worked on)

## Getting Started

### Installation
`#define CJ_IMPL` and include the `cj.h` header and for the corresponding implementations.

cj.c
```c
#define CJ_IMPL
#include <cj.h>
```

### Usage
Here's an example program demonstrating how to use CJ to create, manipulate, and parse JSON data.

```c
#include <cj.h>

int main() {
    char* indent = "    ";
    cj_set_context_indent(indent);

    CJ_Arena* arena = cj_arena_create(0);
    JSON* root = cj_create(arena);
    cj_push(root, "name", "Example");
    cj_push(root, "age", 25);

    JSON* nested = cj_create(arena);
    cj_push(nested, "address1", "San Francisco");
    cj_push(nested, "address2", "California");

    JSON* nested_twice = cj_create(arena);
    cj_push(nested_twice, "KylE", (bool)true);
    cj_push(nested_twice, "ANdY", 84);
    cj_push(nested_twice, "Jovanni", 3.141598);
    cj_push(nested_twice, "Hykes", (bool)false);
    cj_push(nested_twice, "Conrad", JSON_NULL(arena));

    cj_push(nested, "Names", nested_twice);
    cj_push(root, "address", nested);

    JSON* hobbies = cj_array_create(arena);
    cj_array_push(hobbies, nested_twice);
    cj_array_push(hobbies, (bool)true);
    cj_array_push(hobbies, "Coding");
    cj_push(root, "hobbies", hobbies);

    char* str = cj_to_string(root);
    printf("%s\n", str);

    // u8* source_data = cj_os_read_entire_file(file_path, &file_size);

    char* test_json = "{\"BinaryOp\": {\"op\": \"+\", \"left\": {\"Grouping\": {\"BinaryOp\": {\"op\": \"*\", \"left\": [null, false, 7, 23.51, true, { \"test\": \"should_be\" }], \"right\": 123}}}, \"right\": {\"Grouping\": {\"BinaryOp\": {\"op\": \"/\", \"left\": 2, \"right\": {\"Grouping\": {\"BinaryOp\": {\"op\": \"+\", \"left\": 45.234001, \"right\": 5}}}}}}}}";

    printf("------------ PARSING ------------\n");
    JSON* json_test_parse = cj_parse(arena, test_json);
    char* str2 = cj_to_string(json_test_parse);
    printf("%s\n", str2);


    free(str);
    free(str2);
    cj_arena_free(arena);

    return 0;
}
```

### Output

```
{
    "name": "Example",
    "age": 25,
    "address": {
        "address1": "San Francisco",
        "address2": "California",
        "Names": {
            "KylE": true,
            "ANdY": 84,
            "Jovanni": 3.141598,
            "Hykes": false,
            "Conrad": null
        }
    },
    "hobbies": [
        {
            "KylE": true,
            "ANdY": 84,
            "Jovanni": 3.141598,
            "Hykes": false,
            "Conrad": null
        },
        true,
        "Coding"
    ]
}
------------ PARSING ------------
{
    "BinaryOp": {
        "op": "+",
        "left": {
            "Grouping": {
                "BinaryOp": {
                    "op": "*",
                    "left": [
                        null,
                        false,
                        7,
                        23.51,
                        true,
                        {
                            "test": null
                        }
                    ],
                    "right": 123
                }
            }
        },
        "right": {
            "Grouping": {
                "BinaryOp": {
                    "op": "/",
                    "left": 2,
                    "right": {
                        "Grouping": {
                            "BinaryOp": {
                                "op": "+",
                                "left": 45.234001,
                                "right": 5
                            }
                        }
                    }
                }
            }
        }
    }
}
```

## API Reference

### JSON Manipulation
- `JSON* cj_create(CJ_Arena* arena)` - Creates a new JSON object.
- `void cj_push(JSON* object, const char* key, ...)` - Adds a key-value pair to a JSON object.
- `JSON* cj_array_create(CJ_Arena* arena)` - Creates a new JSON array.
- `void cj_array_push(JSON* array, ...)` - Adds a value to a JSON array.

### JSON parsing/printing
- `char* cj_set_context_indent(char* indent)` - Sets the indent level you want
- `char* cj_to_string(JSON* json)` - Converts a JSON object to a formatted string.
- `JSON* cj_parse(CJ_Arena* arena, const char* json_str)` - Parses a JSON string into a JSON object.

### Memory Management
- `CJ_Arena* cj_arena_create(size_t size)` - Creates a memory arena.
- `void cj_arena_free(CJ_Arena* arena)` - Frees the memory used by the arena.

## License
CJ is open source and available under the [MIT License](LICENSE).

---

## --- Feel free to contribute, report issues, or suggest improvements! ---

#### Notes for me:
- Need to implement duplicate key checks should be trivial

- I have to implement scientific notation | 3.7e-5 = 3.7E-5 = 3.7×10-5 = 0.000037