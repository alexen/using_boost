/// @file
/// @brief

#pragma once

#include <boost/stacktrace/stacktrace.hpp>
#include <boost/exception/enable_error_info.hpp>
#include <boost/exception/error_info.hpp>


using Stacktrace = boost::error_info< struct Stacktrace_, boost::stacktrace::stacktrace >;


#define THROW_STACKTRACE_EXCEPTION( exc ) \
     BOOST_THROW_EXCEPTION( boost::enable_error_info( exc ) \
          << Stacktrace{ boost::stacktrace::stacktrace() } )
