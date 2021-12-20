/// @file
/// @brief

#include <stacktrace/tools/funcs.h>

#include <stdexcept>
#include <stacktrace/stacktrace_exception.h>


namespace using_boost {
namespace stacktrace {
namespace tools {


namespace {
namespace impl {


static void funcB()
{
     THROW_EXCEPTION( std::logic_error{ "logic error" } );
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
