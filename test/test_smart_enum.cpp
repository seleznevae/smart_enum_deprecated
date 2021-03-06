
#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <assert.h>
#include <cstddef>
#include <utility>

#include "smart_enum.h"
#include "third_party/Catch/catch.hpp"


SMART_ENUM(Animal, int) {
    INITIALIZE_SMART_ENUM;

    SM_ENUM_ELEM(Dog,    1,  "dog"  , "dog_description");
    SM_ENUM_ELEM(Cat,    2,  "cat"  );
    SM_ENUM_ELEM(Lion,   5);
    SM_ENUM_ELEM(Horse,  10,  "hOrse");
};

TEST_CASE( "Base tests for smart enums", "[base]" )
{
    Animal pet_0 = Animal::Dog;
    Animal pet_1 = Animal::Horse;

    bool cond_0 = (pet_0 == Animal::Dog);
    bool cond_1 = (pet_1 == Animal::Horse);
    REQUIRE( cond_0 );
    REQUIRE( cond_1 );

    std::swap(pet_0, pet_1);
    cond_0 = (pet_0 == Animal::Horse);
    cond_1 = (pet_1 == Animal::Dog);
    REQUIRE( cond_0 );
    REQUIRE( cond_1 );
}


TEST_CASE( "Test descriptions of smart enums", "[description]" )
{
    REQUIRE( smart_enum::get_description(Animal(Animal::Dog))   == std::string("dog_description") );
    REQUIRE( smart_enum::get_description(Animal(Animal::Cat))   == std::string("cat") );
    REQUIRE( smart_enum::get_description(Animal(Animal::Lion))  == std::string("Lion") );
    REQUIRE( smart_enum::get_description(Animal(Animal::Horse)) == std::string("hOrse") );

    auto str_repr_0 = smart_enum::get_enum_description<0, Animal>();
    auto str_repr_1 = smart_enum::get_enum_description<1, Animal>();
    auto str_repr_2 = smart_enum::get_enum_description<2, Animal>();
    auto str_repr_3 = smart_enum::get_enum_description<3, Animal>();
    REQUIRE( str_repr_0 == std::string("dog_description") );
    REQUIRE( str_repr_1 == std::string("cat") );
    REQUIRE( str_repr_2 == std::string("Lion") );
    REQUIRE( str_repr_3 == std::string("hOrse") );
}

TEST_CASE( "Test string representation of smart enums", "[string]" )
{
    REQUIRE( smart_enum::to_string(Animal(Animal::Dog))   == std::string("dog") );
    REQUIRE( smart_enum::to_string(Animal(Animal::Cat))   == std::string("cat") );
    REQUIRE( smart_enum::to_string(Animal(Animal::Lion))  == std::string("Lion") );
    REQUIRE( smart_enum::to_string(Animal(Animal::Horse)) == std::string("hOrse") );

    //testing constexpr
    constexpr auto anim_0 = smart_enum::to_enum<Animal>("dog");
    constexpr auto anim_3 = smart_enum::to_enum<Animal>("hOrse");
    REQUIRE( Animal(Animal::Dog)   == anim_0 );
    REQUIRE( Animal(Animal::Horse) == anim_3 );

    constexpr auto anim_name_1 = smart_enum::to_string(Animal(Animal::Cat));
    constexpr auto anim_name_2 = smart_enum::to_string(Animal(Animal::Lion));
    REQUIRE( anim_name_1 == std::string("cat") );
    REQUIRE( anim_name_2 == std::string("Lion") );

    //    //won't compile as it should be
    //constexpr auto aa = smart_enum::to_string(smart_enum::int_to_enum_cast<Animal>(666));
    REQUIRE_THROWS_AS( smart_enum::to_string(smart_enum::int_to_enum_cast<Animal>(666)), std::bad_alloc );
}

TEST_CASE( "Test enum size evaluation", "[enum_size]" )
{
    REQUIRE( smart_enum::enum_size<Animal>()  == 4 );
}

TEST_CASE( "Test casts", "[cast]" )
{
    REQUIRE( static_cast<int>(Animal::Horse) == 10);
    Animal pet = Animal::Horse;
    REQUIRE( smart_enum::enum_to_int_cast<int>(pet) == 10);
    REQUIRE( smart_enum::int_to_enum_cast<Animal>(10) == pet);
}

TEST_CASE( "Checking data in enums", "[check]" )
{
    Animal valid(Animal::Horse);
    REQUIRE(smart_enum::check(valid) == true);

    Animal invalid;
    *(reinterpret_cast<int*>(&invalid)) = 6666;

    REQUIRE( smart_enum::check(invalid) == false);
    REQUIRE_THROWS_AS( smart_enum::to_string(invalid), std::bad_alloc);

    // Nonstrict conversion doesn't throw in invalid cases
    REQUIRE(smart_enum::to_string<smart_enum::NonStrictConvPolicy>(invalid) == std::string("Unknown"));
}


TEST_CASE( "Checking index of element in enums", "[index_of]" )
{
    REQUIRE(smart_enum::index_of<Animal>(Animal::Lion) == 2);
}
