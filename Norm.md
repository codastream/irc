# norm

## Commits

- format [prefix]:[msg]
- prefix one of
	- fix
	- feat
	- meta for anything like .gitignore, Readme, charts, ..
	- org when basically moving content to another file
	- wip if work in progress
	- bug if committed with a known bug
wip and bug should be avoided in commit history, can be used on local repository then squashed during interactive rebase.

## Header files
- naming : _file.hpp_
- template definitions should be made in a _tpp_
- self-contained : should compile on their own, without relying on transitive inclusions
- prefer includes over forward declarations
- include order : related > C system > C++ STL > other libs > project

## Namespace
- 🚫 `using` namespace `foo`
- internal only code goes in namespace named `internal`

## Scope

- `static` variables only if trivially destructible
- `static` functions should be put in appropriate namespace

## Variables and functions

- initialize at declaration (don't rely on default initialization)

## Naming

### files

- PascalCase for classes, lowercase_with_underscore for others
- `.cc` for implementation, `.h` for headers
- `_test.cc` for test files

### types

- PascalCase
- acronyms as words (HttpServer)
- pointer `*` of reference `&` symbol comes directly after type as in `Client* clients`

### variables

- lowercase_with_underscore for general variables
- UPPERCASE_WITH_UNDERSCORE for const class variables
- trailing underscore for class data members `count_`
- global const : UPPERCASE

### functions

- PascalCase
- accessors / mutators match variable name
- for boolean fonction, use names like `is_empty`, `has_value`, ...

### namespace

- all lowercase

### enumerators

- PascalCase

## Comments

- class : what it represents + key invariants
- function : what it does + input / output if not obvious

## Formatting

- indent : 4 spaces
- line length : max 100 chars
- space around operators

### braces

- Stroustrup style

```cpp
if (condition) {
	doSomething();
} else {
	doOther();
}
```

### class layout

- public > protected > private

## Sources

https://google.github.io/styleguide/cppguide.html