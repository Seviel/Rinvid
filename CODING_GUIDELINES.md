# Coding Style Guidelines

## 1. General

1.1 Formatting shall be done by clang-format  
All formatting (line breaks, spaces, etc) is done by clang-format using the `_clang-format` file in the root of the Rinvid repository. Use a version that supports this configuration, and use the same version across contributors and CI to keep formatting consistent. A utility bash script apply_clang_format.sh is made available in the root of the repository to facilitate formatting.

1.2 All source files shall start with license notice comment block (see any of the existing source files for an example)

1.3 End every file with a newline.

1.4 Includes shall have the following order:
1. Standard lib includes first, sorted alphabetically
2. SFML and other extern lib includes, sorted alphabetically
3. Rinvid headers, sorted alphabetically

1.5 Prefer uniform initialization. Parentheses are allowed when they express the intended behavior, such as selecting a size constructor instead of an initializer-list constructor.

Example:

```cpp
// Bad
float f = 5.0F;

// Good 
float f{5.0F};
```

For example, these vector constructors are both allowed because they have different meanings:

```cpp
std::vector<std::int32_t> values(10); // Ten zero-initialized elements.
std::vector<std::int32_t> values{10}; // One element with the value 10.
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

1.7 Use integer types with specified width, except that `std::size_t` (from `<cstddef>`) is allowed for sizes, counts, and indexing.

Example:

```cpp
// Bad
int i;

// Good
#include <cstdint>

...

std::int32_t i;
```

1.8 Braces shall be used even when if/else/while/for statements have only one instruction in body

Example:

```cpp
// Bad
if(counter == max_count)
    function();

// Good
if(counter == max_count)
{
    function();
}
```

## 2. Naming

2.1 The following naming conventions are used:

| Type        | Convention  |
| ----------- | ----------- |
| file name   | snake_case.cpp|
| type (class, struct, union, enum, typedef)   | PascalCase|
| function  | snake_case|
| benchmark function | snake_case or PascalCase (capital camel case)|
| variable   | snake_case|
| constexpr variable | snake_case or MACRO_CASE|
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

Example for file include/rinvid/core/circle_shape.h:

```cpp
#ifndef INCLUDE_RINVID_CORE_CIRCLE_SHAPE_H
#define INCLUDE_RINVID_CORE_CIRCLE_SHAPE_H

// contents of the header file

#endif // INCLUDE_RINVID_CORE_CIRCLE_SHAPE_H
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
