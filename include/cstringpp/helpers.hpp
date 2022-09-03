#pragma once

namespace cstringpp {

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

} // namespace cstringpp
