/// @file
/// @brief

#include <stacktrace/tools/funcs.h>

#include <boost/throw_exception.hpp>
#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>

#include <stacktrace/stacktrace_exception.h>


namespace using_boost {
namespace stacktrace {
namespace tools {


namespace {
namespace impl {


static void funcB()
{
     THROW_STACKTRACE_EXCEPTION( std::logic_error{ "logic error" } );
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
