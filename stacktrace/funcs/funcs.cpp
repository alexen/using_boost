/// @file
/// @brief


#include <stacktrace/funcs/funcs.h>

#include <boost/throw_exception.hpp>
#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>


using Stacktrace = boost::error_info< struct Stacktrace_, boost::stacktrace::stacktrace >;


void funcC()
{
     BOOST_THROW_EXCEPTION( FuncError{ "some internal error" }
          << Stacktrace{ boost::stacktrace::stacktrace() } );
}


void funcB()
{
     funcC();
}


void funcA()
{
     funcB();
}
