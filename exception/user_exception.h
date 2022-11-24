/// @file user_exception.h
/// @brief
/// @copyright Copyright 2022 InfoTeCS Internet Trust

#pragma once

#include <exception>
#include <boost/exception/exception.hpp>
#include <boost/exception/info_tuple.hpp>


namespace alexen {
namespace error {


/// @note Заметьте, что не нужно больше создавать наследование от std::runtime_error
/// чтобы передавать строковое значение для `const char* what() const`. Для этого
/// используется инъекция `details::ErrorDescription`.
///
/// @note Также заметьте, что используется **виртуальное** наследование. Это важно,
/// подробности смотри [здесь](https://www.boost.org/doc/libs/1_80_0/libs/exception/doc/using_virtual_inheritance_in_exception_types.html).
struct Exception : virtual std::exception, virtual boost::exception {};


namespace details {


using ErrorDescription = boost::error_info< struct ErrorDescription_, std::string >;
using ErrorCode        = boost::error_info< struct ErrorCode_, int >;


} // namespace details
} // namespace error
} // namespace alexen
