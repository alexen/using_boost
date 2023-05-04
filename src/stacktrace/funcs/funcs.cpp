/// @file
/// @brief

#include <stacktrace/exception.h>
#include <stacktrace/funcs/funcs.h>
#include <stdexcept>


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
     funcA( std::string{} );
}

void funcA( std::string )
{
     funcB( 1.7 );
}


void funcB( float )
{
     funcC( 101 );
}


void funcC( int )
{
     impl::funcA();
}



} // namespace tools
} // namespace stacktrace
} // namespace using_boost
