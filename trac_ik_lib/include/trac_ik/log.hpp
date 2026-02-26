#ifndef __FMT_LOG_MACROS_HPP__
#define __FMT_LOG_MACROS_HPP__
#include <fmt/core.h>
#include <chrono>

#include <Eigen/Core>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>

#include <sstream>

/*
// Specialization for Eigen types
template <typename Derived>
struct fmt::formatter<Eigen::MatrixBase<Derived>> {
    // Parses format specs like {: .2f}
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Eigen::MatrixBase<Derived>& matrix, FormatContext& ctx) const {
        // Eigen's default IO stream operator is very robust
        std::stringstream ss;
        ss << matrix.transpose(); // Transpose vectors to print horizontally
        return fmt::format_to(ctx.out(), "{}", ss.str());
    }
};
*/

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of_v<Eigen::DenseBase<T>, T>, char>> : ostream_formatter {};

// Make specialization for Eigen::VectorXd to prevent compilation error regarding ambiguous type.
template <>
struct fmt::formatter<Eigen::VectorXd, std::enable_if_t<true, char>> : ostream_formatter {};


#define LOG_NAMED(name, format_str, ...) \
{ \
        fmt::print("[{}]: " format_str "\n", name, ##__VA_ARGS__); \
} \

#define LOG_THROTTLED(dt, format_str, ...) \
{ \
    static auto last_hit = std::chrono::steady_clock::time_point::min(); \
    auto now = std::chrono::steady_clock::now(); \
    if (now - last_hit >= std::chrono::duration<float>(dt)) { \
        last_hit = now; \
        fmt::print(format_str "\n", ##__VA_ARGS__); \
    } \
} \

#endif // __FMT_LOG_MACROS_HPP__
