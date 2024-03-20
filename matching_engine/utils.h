#include <string>
#include <type_traits>

namespace utils {

template <typename Container>
const std::string toString(const Container& container)
{
    std::string str = "[";
    for (const auto& element : container) {
        str += format("{} ", element->toString());
    }

    str += "]";
    return str;
}
} // namespace utils
