#include <concepts>
#include <type_traits>

namespace Constraints {

template <typename T>
concept ConstType = std::is_const_v<std::remove_reference<T>>;

template <typename T>
concept MutableType = !ConstType<T>;

} // namespace Constrains
