/// @file loader.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/utility/string_view.hpp>

#include <log/modules/dynlib/types.h>


namespace using_boost {
namespace modules {
namespace dynlib {


DynLibUptr load( const boost::filesystem::path& dynlib );

void* exportFn( const DynLibUptr& handler, boost::string_view name );


template< typename Res >
inline Res get( const DynLibUptr& handler, boost::string_view name )
{
     return (Res) exportFn( handler, name );
}


template< typename Fn >
inline auto call( const DynLibUptr& handler, boost::string_view name )
{
     return get< Fn >( handler, name )();
}


} // namespace dynlib
} // namespace modules
} // namespace using_boost
