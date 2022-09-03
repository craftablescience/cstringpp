#include <gtest/gtest.h>

#include <cstringpp.hpp>

using namespace cstringpp;

TEST(String, ctor) {
    constexpr char s1[] = "hello world";
    constexpr String str1{s1};
    EXPECT_STREQ(str1.toCString(), "hello world");

    constexpr String str2{"hello world"};
    EXPECT_STREQ(str2.toCString(), "hello world");

    constexpr String str3 = "hello world";
    EXPECT_STREQ(str3.toCString(), "hello world");
}

TEST(String, index) {
    constexpr String str = "hello world";
    EXPECT_EQ(str[1], 'e');
    EXPECT_EQ(str[100], 'o');
    EXPECT_EQ(str[-2], 'd'); // null terminator is -1
}

TEST(String, find) {
    constexpr String str = "hello world";
    EXPECT_EQ(str.find('o'), 4);
    EXPECT_EQ(str.find('o', 1), 7);
    EXPECT_EQ(str.find('o', 2), -1);
    EXPECT_EQ(str.find('a'), -1);
}

TEST(String, rfind) {
    constexpr String str = "hello world";
    EXPECT_EQ(str.rfind('o'), 7);
    EXPECT_EQ(str.rfind('o', 1), 4);
    EXPECT_EQ(str.rfind('o', 2), -1);
    EXPECT_EQ(str.rfind('a'), -1);
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

TEST(String, map) {
    constexpr String str1 = String{"HElLo"}.map([](char c) { return cstringpp::toUpper(c); });
    EXPECT_STREQ(str1.toCString(), "HELLO");

    constexpr String str2 = String{"HElLo"}.map(&cstringpp::toLower);
    EXPECT_STREQ(str2.toCString(), "hello");
}

TEST(String, toLower) {
    constexpr String str = String{"HElLo"}.toLower();
    EXPECT_STREQ(str.toCString(), "hello");
}

TEST(String, toUpper) {
    constexpr String str = String{"HElLo"}.toUpper();
    EXPECT_STREQ(str.toCString(), "HELLO");
}

TEST(String, reverse) {
    constexpr String str = String{"palindrome"}.reverse();
    EXPECT_STREQ(str.toCString(), "emordnilap");
}

TEST(String, strcat) {
    constexpr String str1 = strcat(String{"hello "}, String{"world"});
    EXPECT_STREQ(str1.toCString(), "hello world");

    constexpr String str2 = strcat(String{"this"}, String{"is"}, String{"a"}, String{"test"});
    EXPECT_STREQ(str2.toCString(), "thisisatest");
}

TEST(String, strcmp) {
    constexpr String str1 = "hello", str2 = "world";
    EXPECT_EQ(strcmp(str1, str2), -15);
    EXPECT_EQ(strcmp(str2, str1),  15);
    EXPECT_EQ(strcmp(str1, str1),   0);
}
