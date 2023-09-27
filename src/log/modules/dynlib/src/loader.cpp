/// @file loader.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <log/modules/dynlib/loader.h>

#include <dlfcn.h>

#include <boost/throw_exception.hpp>
#include <boost/exception/info.hpp> // Без этого хедера компилятор ругается: invalid use of incomplete type ‘boost::errinfo_file_name’
#include <boost/exception/errinfo_file_name.hpp>
#include <boost/exception/errinfo_api_function.hpp>


namespace using_boost {
namespace modules {
namespace dynlib {


namespace {
namespace aux {


void unload( native::Handler handler )
{
     dlclose( handler );
}


} // namespace aux
} // namespace {unnamed}


DynLibUptr load( const boost::filesystem::path& dynlib )
{
     const auto handler = dlopen( dynlib.c_str(), RTLD_LAZY );
     if( !handler )
     {
          BOOST_THROW_EXCEPTION( DynLibError{ "dynlib loading error" }
               << boost::errinfo_file_name{ dynlib.string() }
               << DynLibErrorDetails{ dlerror() } );
     }
     return DynLibUptr( handler, aux::unload );
}


void* exportFn( const DynLibUptr& handler, boost::string_view name )
{
     const auto fn = dlsym( handler.get(), name.data() );
     if( !fn )
     {
          BOOST_THROW_EXCEPTION( DynLibError{ "dynlib function export error" }
               << boost::errinfo_api_function{ name.data() }
               << DynLibErrorDetails{ dlerror() } );
     }
     return fn;
}


} // namespace dynlib
} // namespace modules
} // namespace using_boost
