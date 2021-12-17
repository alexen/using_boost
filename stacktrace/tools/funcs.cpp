/// @file
/// @brief

#include <stacktrace/tools/funcs.h>

#include <boost/throw_exception.hpp>
#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>


namespace using_boost {
namespace stacktrace {


using Stacktrace = boost::error_info< struct Stacktrace_, boost::stacktrace::stacktrace >;


namespace tools {


namespace {
namespace impl {


static void funcB()
{
     BOOST_THROW_EXCEPTION( FuncError{ "some internal error" }
          << Stacktrace{ boost::stacktrace::stacktrace() } );
}


static void funcA()
{
     funcB();
}


} // namespace impl
} // namespace {unnamed}


void main()
{
     funcA();
}

void funcA()
{
     funcB();
}


void funcB()
{
     funcC();
}


void funcC()
{
     impl::funcA();
}



} // namespace tools
} // namespace stacktrace
} // namespace using_boost
