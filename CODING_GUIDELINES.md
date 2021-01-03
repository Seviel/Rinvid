# Coding Style Guidelines

## 1. General

1.1 Formatting shall be done by clang-format  
All formatting (line breaks, spaces, etc) is done by clang-format-6.0 (install on ubuntu: sudo apt install clang-format-6.0). Formatting style is defined in _clang-format file in the root of Rinvid repository. A utility bash script apply_clang_format.sh is made available in the root of the repository to facilitate formatting.

1.2 All source files shall start with license notice comment block (see any of the existing source files for an example)

1.3 An extra empty line shall end every file.

1.4 Includes shall have the following order:
1. Standard lib includes first, sorted alphabetically
2. SFML and other extern lib includes, sorted alphabetically
3. Rinvid headers, sorted alphabetically

1.5 Uniform initialization shall always be used instead of other means of initialization

Example:

```cpp
// Bad
float f = 5.0F;

// Good 
float f{5.0F};
```

1.6 Capital F literal suffix shall always be appended to float constants

Example:

```cpp
// Bad
float fl;
fl = 0.1;

// Bad
float fl;
fl = 0.1f;

// Good
float fl;
fl = 0.1F;
```

1.5 Only integer types with specified width shall be used

Example:

```cpp
// Bad
int i;

// Good
#include <cstdint>

...

std::int32_t i;
```

## 2. Naming

2.1 The following naming conventions are used:

| Type        | Convention  |
| ----------- | ----------- |
| file name   | snake_case.cpp|
| type (class, struct, union, enum, typedef)   | PascalCase|
| function  | snake_case|
| variable   | snake_case|
| class member variable  | snake_case_|
| macro | MACRO_CASE|    
  

2.2 Names shall be as descriptive as possible
Avoid using abbreviations unless it's an extremely widely known and used abbreviation. 

```cpp
// Bad
std::int32_t msg_cnt;

// Good 
std::int32_t message_count;
```

## 3. Headers

3.1 Header files shall have the extension .h

3.2 Every header shall be enclosed in a unique include guard  
Include guards shall be in all caps. Include guards shall consist of full path to file, excluding name of root repo directory and including file name, separated by underscores. Dots in file names shall be replaced by underscores.

Example for file rinvid/core/include/circle_shape.h:

```cpp
#ifndef CORE_INCLUDE_CIRCLE_SHAPE_H
#define CORE_INCLUDE_CIRCLE_SHAPE_H

// contents of the header file

#endif // CORE_INCLUDE_CIRCLE_SHAPE_H
```

## 4. Classes

4.1 In a class declaration, public interface comes first followed by protected and private respectively

## 5. Comments

5.1 Everything in public API shall be documented with doxygen comments

5.2 Doxygen annotated comments shall be used in following style:

```cpp
    /**************************************************************************************************
     * @brief A brief description.
     * Optional more detailed explanation.
     *
     * @param param1 A brief description of each parameter
     * @param param2 A brief description of each parameter
     * 
     * @return brief descriptrion of return value
     *
     *************************************************************************************************/
```
