/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <openssl/md5.h>

#include <boost/iostreams/concepts.hpp>


template< typename Impl >
struct BlockOutputFilter : boost::iostreams::multichar_output_filter
{
     template< typename ...Args >
     BlockOutputFilter( Args... args ) : impl_{ std::forward< Args >( args )... } {}

     template< typename Sink >
     std::streamsize write( Sink& sink, const char* s, std::streamsize size )
     {
          if( size > 0 )
          {
               impl_.update( s, static_cast< std::size_t >( size ) );
          }
          return boost::iostreams::write( sink, s, size );
     }

     template< typename ...Args >
     void finalize( Args... args )
     {
          impl_.finalize( std::forward< Args >( args )... );
     }
private:
     Impl impl_;
};
