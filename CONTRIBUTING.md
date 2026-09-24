# CONTRIBUTINIG

## Commenting conventions:
- Begin single line comments with a space.
```cpp
//bad!
// good!
```
- Avoid long comments after a line of code.
```cpp
my_function(); // Dont put a long comment here.

// Put it here instead!
my_function();

// If your comment is short, Then it's fine.
float speed; // m/s
```

## Declarations conventions:
- Be specific with the type. Prefer fixed-width integers when possible.
```cpp
int my_pin; // BAD!
unsigned char my_pin; // BAD!
uint8_t my_pin; // GOOD!
size_t array_length; // GOOD!
```
- Use `const` AGGRESSIVELY! Things should not be `const` for good reasons. (/!\ `const` does not mean __constant__ in c/c++. It means __immutable__.)
```cpp
const function(const uint32_t arg1, const char* arg2) {
  const float pi = 3.1415;
  // this is expected to change during the function's run-time,
  // therefore it's fine to keep it non-`const`.
  float running_sum = 0;
  // ...
}
```

## Naming conventions:
- `snake_case` for function names and variables.
- `PascalCase` for struct names.
- `SCREAMING_CASE` for constants.
- Suffix the variable names with their purpose when it cannot be determined by the type alone.
```cpp
uint8_t led_red_pin = 3; 
const int32_t motor_speed;
```

## Code style conventions:
- Avoid macros (`#def`).

## Pointers:
// T.B.D
