#pragma once

#include <cstddef>
#include <array>
#include <compare>
#include <utility>

#include "detail.hpp"

namespace cstringpp {

template<size_t Length>
class String {
public:
    /// Strings are assumed to be null-terminated
    constexpr String(const char(&str)[Length]) { // NOLINT(google-explicit-constructor)
        for (size_t i = 0; i < Length; i++) {
            m_str[i] = str[i];
        }
    }

    /// Strings are assumed to be null-terminated
    constexpr explicit String(const std::array<char, Length>& ary) {
        std::copy(ary.begin(), ary.end(), m_str.begin());
    }

    constexpr String(const String<Length>& other) {
        std::copy(other.m_str.begin(), other.m_str.end(), m_str.begin());
    }

    constexpr String(String<Length>&& other) noexcept {
        std::move(other.m_str.begin(), other.m_str.end(), m_str.begin());
    }

    constexpr String& operator=(const String<Length>& other) {
        std::copy(other.m_str.begin(), other.m_str.end(), m_str.begin());
        return *this;
    }

    constexpr String& operator=(String<Length>&& other) noexcept {
        std::move(other.m_str.begin(), other.m_str.end(), m_str.begin());
        return *this;
    }

    ~String() = default;

    [[nodiscard]] constexpr const char* toCString() const {
        return m_str.data();
    }

    [[nodiscard]] constexpr char operator[](size_t index) const {
        return m_str.at(detail::getPositiveIndex<Length>(index));
    }

    [[nodiscard]] constexpr int find(char needle, int occurence = 0) const {
        int seen = 0;
        for (int i = 0; i < Length; i++) {
            if (m_str[i] == needle && (seen++) == occurence) {
                return i;
            }
        }
        return -1;
    }

    [[nodiscard]] constexpr int rfind(char needle, int occurence = 0) const {
        int seen = 0;
        for (int i = Length - 1; i >= 0; i--) {
            if (m_str[i] == needle && (seen++) == occurence) {
                return i;
            }
        }
        return -1;
    }

    /*
    template<size_t Length2>
    [[nodiscard]] constexpr int find(const String<Length2>& search) {
        if ((Length < Length2) || (Length == Length2 && m_str != search))
            return -1;
        //todo
    }

    template<size_t Length1, size_t Length2>
    [[nodiscard]] constexpr int rfind(const String<Length1>& str, const String<Length2>& find) {
        if ((Length1 < Length2) || (Length1 == Length2 && str != find))
            return -1;
        //todo
    }
    */

    template<int Start, int End = -1>
    [[nodiscard]] constexpr auto slice() const {
        constexpr int startIndex = detail::getPositiveIndex<Length>(Start);
        constexpr int endIndex = detail::getPositiveIndex<Length>(End);
        constexpr int newLength = endIndex - startIndex;

        if constexpr (endIndex <= startIndex) {
            return String<1>{""};
        } else {
            std::array<char, newLength + 1> result;
            std::copy(m_str.begin() + startIndex, m_str.begin() + endIndex, result.begin());
            result[newLength] = 0;
            return String<newLength + 1>{result};
        }
    }

    [[nodiscard]] constexpr String<Length> map(char(*mapFunc)(char)) const {
        std::array<char, Length> result;
        for (int i = 0; i < Length; i++) {
            result[i] = (*mapFunc)(m_str[i]);
        }
        return String<Length>{result};
    }

    [[nodiscard]] constexpr String<Length> toLower() const {
        return map(&cstringpp::detail::toLower);
    }

    [[nodiscard]] constexpr String<Length> toUpper() const {
        return map(&cstringpp::detail::toUpper);
    }

    [[nodiscard]] constexpr String<Length> reverse() const {
        std::array<char, Length> result;
        for (int i = 1; i < Length; i++) {
            result[i - 1] = m_str[Length - i - 1];
        }
        result[Length - 1] = 0;
        return String<Length>{result};
    }

    [[nodiscard]] constexpr auto operator<=>(const String& other) const = default;

    [[nodiscard]] constexpr bool operator==(const String& other) const = default;

    [[nodiscard]] constexpr size_t size() const noexcept {
        return Length;
    }

    [[nodiscard]] constexpr size_t length() const noexcept {
        return Length;
    }

    [[nodiscard]] constexpr auto begin() const noexcept {
        return m_str.begin();
    }

    [[nodiscard]] constexpr auto end() const noexcept {
        return m_str.end();
    }

protected:
    std::array<char, Length> m_str;
};

template<size_t... Lengths>
[[nodiscard]] constexpr auto strcat(const String<Lengths>&... strings) {
    // We need to strip null terminators from every string except the last
    // Also resize the result array so it doesn't have extra chars at the end
    constexpr int CombinedLength = (Lengths + ...) + 1 - sizeof...(strings);
    std::array<char, CombinedLength> result;
    size_t index = 0;
    ((std::copy_n(strings.begin(), Lengths, result.begin() + index), index += Lengths - 1), ...);
    return String<CombinedLength>{result};
}

template<size_t Length1, size_t Length2>
[[nodiscard]] constexpr int strcmp(const String<Length1>& str1, const String<Length2>& str2) {
    for (int i = 0; i < Length1 && i < Length2; i++) {
        char c1 = str1[i], c2 = str2[i];
        if (c1 != c2) {
            return c1 - c2;
        }
    }
    return 0;
}

template<size_t Length1, size_t Length2>
[[nodiscard]] constexpr int stricmp(const String<Length1>& str1, const String<Length2>& str2) {
    for (int i = 0; i < Length1 && i < Length2; i++) {
        char c1 = toLower(str1[i]), c2 = toLower(str2[i]);
        if (c1 != c2) {
            return c1 - c2;
        }
    }
    return 0;
}

template<size_t Length>
[[nodiscard]] constexpr inline auto strdup(const String<Length>& str) {
    return String<Length>{str};
}

// need strset and strnset

// also a tokenizer would be nice

} // namespace cstringpp
