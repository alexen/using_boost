/// @file
/// @brief

#pragma once

#include <boost/throw_exception.hpp>

#ifdef __linux__

#include <boost/exception/error_info.hpp>
#include <boost/exception/enable_error_info.hpp>
#include <boost/stacktrace/stacktrace.hpp>


namespace using_boost {
namespace stacktrace {

using Stacktrace = ::boost::error_info< struct Stacktrace_, ::boost::stacktrace::stacktrace >;

} // namespace stacktrace
} // namespace using_boost


#define THROW_EXCEPTION( exc ) \
     BOOST_THROW_EXCEPTION( ::boost::enable_error_info( exc ) \
          << ::using_boost::stacktrace::Stacktrace( ::boost::stacktrace::stacktrace{} ) )

#else
#define THROW_EXCEPTION( exc ) BOOST_THROW_EXCEPTION( exc )
#endif
