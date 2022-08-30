#pragma once

#include <cstddef>
#include <cstring>
#include <array>
#include <compare>
#include <utility>

namespace cstringpp {

template<size_t Length>
class String {
public:
    /// Strings are assumed to be null-terminated
    constexpr explicit String(const char(&str)[Length]) {
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

    constexpr String& operator=(const String<Length> other) {
        std::copy(other.m_str.begin(), other.m_str.end(), m_str.begin());
        return *this;
    }

    constexpr String(String<Length>&& other) noexcept {
        std::move(other.m_str.begin(), other.m_str.end(), m_str.begin());
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
        return m_str.at(index);
    }

    template<int Start, int End = -1>
    [[nodiscard]] constexpr auto slice() const {
        // A necessity to wrap this in a function sadly
        constexpr auto getPositiveIndex = [](int index) -> int {
            while (index < 0) {
                index += Length;
            }
            while (index >= Length) {
                index -= Length;
            }
            return index;
        };
        constexpr int startIndex = getPositiveIndex(Start);
        constexpr int endIndex = getPositiveIndex(End);
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
    std::array<char, Length> m_str{};
};

[[nodiscard]] constexpr int toLower(char in) {
    if (in >= 'A' && in <= 'Z') {
        return in + 32;
    }
    return in;
}

[[nodiscard]] constexpr int toUpper(char in) {
    if (in >= 'a' && in <= 'z') {
        return in - 32;
    }
    return in;
}

template<size_t... Lengths>
[[nodiscard]] constexpr auto strcat(const String<Lengths>&... strings) {
    // We need to strip null terminators from every string except the last
    // Also resize the result array so it doesn't have extra chars at the end
    constexpr int NumArgsMinus1 = (Lengths + ...) + 1 - [] {
        int numArgs = 0;
        ((Lengths, numArgs++), ...);
        return numArgs;
    }();
    std::array<char, NumArgsMinus1> result;
    size_t index = 0;
    ((std::copy_n(strings.begin(), Lengths, result.begin() + index), index += Lengths - 1), ...);
    return String<NumArgsMinus1>{result};
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
[[nodiscard]] constexpr int strchr(const String<Length>& str, char needle, int occurence = 0) {
    int seen = 0;
    for (int i = 0; i < Length; i++) {
        if (str[i] == needle && seen++ == occurence ) {
            return i;
        }
    }
    return -1;
}

template<size_t Length>
[[nodiscard]] constexpr int strrchr(const String<Length>& str, char needle, int occurence = 0) {
    int seen = 0;
    for (int i = Length - 1; i >= 0; i--) {
        if (str[i] == needle && seen++ == occurence ) {
            return i;
        }
    }
    return -1;
}

template<size_t Length1, size_t Length2>
[[nodiscard]] constexpr int strstr(const String<Length1>& str, const String<Length2>& find) {
    if ((Length1 < Length2) || (Length1 == Length2 && str != find))
        return -1;
    //todo
}

template<size_t Length1, size_t Length2>
[[nodiscard]] constexpr int strrstr(const String<Length1>& str, const String<Length2>& find) {
    if ((Length1 < Length2) || (Length1 == Length2 && str != find))
        return -1;
    //todo
}

template<size_t Length>
[[nodiscard]] constexpr inline auto strdup(const String<Length>& str) {
    return String{str};
}

template<size_t Length>
[[nodiscard]] constexpr auto strlwr(const String<Length>& str) {
    std::array<char, Length> result;
    for (int i = 0; i < Length; i++) {
        result[i] = toLower(str[i]);
    }
    return String<Length>{result};
}

template<size_t Length>
[[nodiscard]] constexpr auto strupr(const String<Length>& str) {
    std::array<char, Length> result;
    for (int i = 0; i < Length; i++) {
        result[i] = toUpper(str[i]);
    }
    return String<Length>{result};
}

template<size_t Length>
[[nodiscard]] constexpr auto strrev(const String<Length>& str) {
    std::array<char, Length> result;
    for (int i = 1; i < Length; i++) {
        result[i - 1] = str[Length - i - 1];
    }
    result[Length - 1] = 0;
    return String<Length>{result};
}

// need strset and strnset

// also a tokenizer would be nice

} // namespace cstringpp
