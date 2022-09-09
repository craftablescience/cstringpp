#pragma once

namespace cstringpp::detail {

[[nodiscard]] constexpr char toLower(char in) {
    if (in >= 'A' && in <= 'Z') {
        return static_cast<char>(in + 32);
    }
    return in;
}

[[nodiscard]] constexpr char toUpper(char in) {
    if (in >= 'a' && in <= 'z') {
        return static_cast<char>(in - 32);
    }
    return in;
}

template<size_t Length>
constexpr size_t getPositiveIndex(int index) {
    while (index < 0) {
        index += Length;
    }
    while (index >= Length) {
        index -= Length;
    }
    return index;
}

} // namespace cstringpp
