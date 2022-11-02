#ifndef DUTILITY_H_
#define DUTILITY_H_

#include <string>

namespace ding
{

// template function to format string like sprintf.
inline void string_replace(std::string &str, const std::string &from, const std::string &to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

inline void replaceArgs(std::string &str, size_t index, const std::string &arg)
{
    std::string tmp = "%" + std::to_string(index);
    string_replace(str, tmp, arg);
}

inline void replaceArgs(std::string &str, size_t index, const char *arg)
{
    std::string tmp = "%" + std::to_string(index);
    string_replace(str, tmp, std::string(arg));
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic<std::remove_reference_t<T>>::value>>
void replaceArgs(std::string &str, size_t index, T &&arg)
{
    string_replace(str, "%" + std::to_string(index), std::to_string(std::forward<T>(arg)));
}

inline std::string string_format_imp(const std::string &fmt, size_t index)
{
	(void)index;
    return fmt;
}

template <typename First, typename... Rest>
std::string string_format_imp(const std::string &fmt, size_t index, First &&first, Rest &&... rest)
{
    std::string ret(fmt);
    replaceArgs(ret, index, std::forward<First>(first));
    return string_format_imp(ret, index + 1, std::forward<Rest>(rest)...);
}

template <typename... Args>
std::string string_format(const std::string &fmt, Args &&... args)
{
    return string_format_imp(fmt, 1, std::forward<Args>(args)...);
}

//recruit calling function F on args.
template <typename F>
void recruitArgs(F f) {(void)f;}

template<typename F, typename First, typename... Rest>
void recruitArgs(F f, First&& first, Rest&&... args)
{
    f(std::forward<First>(first));
    recruitArgs(f,std::forward<Rest>(args)...);
}

}  // namespace ding
#endif /* DUTILITY_H_ */
