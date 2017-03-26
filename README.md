# Smart enum
Smart enum - short test library for smart enums in C++.
Implementation is based on materials for [SMETA](http://b.atch.se/posts/constexpr-meta-container/attachments/stateful_meta_container-poc.cpp) (stateful meta-programming library) by Filip Roséen  	filip.roseen@gmail.com.

Author doesn't know if these techniques are valid according to the C++ standard and this programs are not ill-formed.
So **it's not recommended to use this library in your production code**. **This library is just an experiment**.

## Design goals

C++ doesn't have reflection. It is a common technique to use macros to add compile time inforamtion about enumeration like the following:

```c++
enum class Animal {
  Dog = 1,
  Cat = 1,
  Lion = 1,
  Horse = 1,
};

DECLARE_ENUM(Animal, Animal::Dog, Animal::Cat, Animal::Lion, Animal::Horse);
```
This code violates **DRY** (don't repeat yourself) principle. The aim of **smart enum** library to give macros that can be use in enum
declaration and user will not need to repeat enum elements.


## Examples

Declaration of smart enum looks like this:
```c++
#include "smart_enum.h"

SMART_ENUM(Animal, int) {
    INITIALIZE_SMART_ENUM;

    SM_ENUM_ELEM(Dog,    1,  "dog"  , "dog_description");
    SM_ENUM_ELEM(Cat,    2,  "cat"  );
    SM_ENUM_ELEM(Lion,   5);
    SM_ENUM_ELEM(Horse,  10,  "hOrse");
};
```
Declaring variables:
```c++
Animal pet_cat   = Animal::Cat;
Animal pet_dog   = Animal::Dog;
Animal pet_lion  = Animal::Lion;
Animal pet_horse = Animal::Horse;
```

It is possible to convert smart enums to strings:
```c++
std::cout << "pet name: " << smart_enum::to_string(pet_cat) << std::endl;
//pet name: cat
std::cout << "pet name: " << smart_enum::to_string(pet_lion) << std::endl;
//pet name: Lion
```

It is possible get descriptions for smart enum values:
```c++
std::cout << "descr.: " << smart_enum::get_description(pet_dog) << std::endl;
//descr.: dog_description
std::cout << "descr.: " << smart_enum::get_description(pet_horse) << std::endl;
//descr.: hOrse
```

Smart enums can be use in switch statements like ordinal enums
```c++
switch (pet_cat) {
     case Animal::Cat:
         assert(true);
         break;
     case Animal::Horse:
         assert(false);
         break;
     default:
         assert(false);
         break;
 }
```
