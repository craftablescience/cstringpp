#include <gtest/gtest.h>

#include <cstringpp.hpp>

using namespace cstringpp;

TEST(String, ctor) {
    constexpr char s1[] = "hello world";
    constexpr String str1{s1};
    EXPECT_STREQ(str1.toCString(), "hello world");

    constexpr String str2{"hello world"};
    EXPECT_STREQ(str2.toCString(), "hello world");
}

TEST(String, strcat) {
    constexpr String str1 = strcat(String{"hello "}, String{"world"});
    EXPECT_STREQ(str1.toCString(), "hello world");

    constexpr String str2 = strcat(String{"this"}, String{"is"}, String{"a"}, String{"test"});
    EXPECT_STREQ(str2.toCString(), "thisisatest");
}

TEST(String, strcmp) {
    constexpr String str1{"hello"}, str2{"world"};
    EXPECT_EQ(strcmp(str1, str2), -15);
    EXPECT_EQ(strcmp(str2, str1),  15);
    EXPECT_EQ(strcmp(str1, str1),   0);
}

TEST(String, strlwr) {
    constexpr String str = strlwr(String{"HElLo"});
    EXPECT_STREQ(str.toCString(), "hello");
}

TEST(String, strupr) {
    constexpr String str = strupr(String{"HElLo"});
    EXPECT_STREQ(str.toCString(), "HELLO");
}

TEST(String, strrev) {
    constexpr String str = strrev(String{"palindrome"});
    EXPECT_STREQ(str.toCString(), "emordnilap");
}

TEST(String, slice) {
    constexpr String str1 = String{"this is a string"}.slice<5>();
    EXPECT_STREQ(str1.toCString(), "is a string");

    constexpr String str2 = String{"this is a string"}.slice<5,7>();
    EXPECT_STREQ(str2.toCString(), "is");

    constexpr String str3 = String{"this is a string"}.slice<5,-10>();
    EXPECT_STREQ(str3.toCString(), "is");

    constexpr String str4 = String{"this is a string"}.slice<192, -4>();
    EXPECT_STREQ(str4.toCString(), "is a str");
}
