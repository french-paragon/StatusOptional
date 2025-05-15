#include <gtest/gtest.h>

#include "./status_optional.h"

struct Foo {
    int fizz;
    std::string buzz;
};

struct Bar {
    std::string fizz;
    int buzz;
};

// Basic .
TEST(StatusOptional, BasicFunctionality) {
    using TestStatusOptional = StatusOptional<Foo,Bar>;

    StatusOptional<Foo,Bar> invalid;
    StatusOptional<Foo,Bar> base = Foo{69, "everything is nice"};
    StatusOptional<Foo,Bar> warning = StatusOptional<Foo,Bar>::warning(Foo{33, "everything should be nice"}, Bar{"Something may have gone wrong", 42});
    StatusOptional<Foo,Bar> error = StatusOptional<Foo,Bar>::error(Bar{"Something has definitly gone wrong", 27});

    static_assert(std::is_same_v<std::decay_t<decltype(base.value())>,Foo>, "Unexpected function return type");
    static_assert(std::is_same_v<std::decay_t<decltype(base.message())>,Bar>, "Unexpected function return type");

    ASSERT_FALSE(invalid.is_valid());
    ASSERT_TRUE(base.is_valid());
    ASSERT_TRUE(warning.is_valid());
    ASSERT_TRUE(error.is_valid());

    ASSERT_TRUE(base.has_value());
    ASSERT_TRUE(warning.has_value());
    ASSERT_FALSE(error.has_value());

    ASSERT_FALSE(base.has_message());
    ASSERT_TRUE(warning.has_message());
    ASSERT_TRUE(error.has_message());

    ASSERT_NO_THROW(base.value());
    ASSERT_NO_THROW(warning.value());
    ASSERT_ANY_THROW(error.value());

    ASSERT_ANY_THROW(base.message());
    ASSERT_NO_THROW(warning.message());
    ASSERT_NO_THROW(error.message());

    ASSERT_EQ(base.value().fizz, 69);
    ASSERT_EQ(warning.value().fizz, 33);
    ASSERT_EQ(base.value().buzz, std::string("everything is nice"));
    ASSERT_EQ(warning.value().buzz, std::string("everything should be nice"));

    ASSERT_EQ(warning.message().buzz, 42);
    ASSERT_EQ(error.message().buzz, 27);
    ASSERT_EQ(warning.message().fizz, std::string("Something may have gone wrong"));
    ASSERT_EQ(error.message().fizz, std::string("Something has definitly gone wrong"));


    ASSERT_EQ(base->fizz, base.value().fizz);
    ASSERT_EQ(warning->fizz, warning.value().fizz);
    ASSERT_EQ(base->buzz, base.value().buzz);
    ASSERT_EQ(warning->buzz, warning.value().buzz);
}
