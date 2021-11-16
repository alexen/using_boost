/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/err.h>

#include <boost/iostreams/concepts.hpp>
#include <boost/utility/string_view.hpp>


template< typename Impl >
struct BlockOutputFilter : boost::iostreams::multichar_output_filter
{
     template< typename ...Args >
     BlockOutputFilter( Args&& ...args ) : impl_{ std::forward< Args >( args )... } {}

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
     void finalize( Args&& ...args )
     {
          impl_.finalize( std::forward< Args >( args )... );
     }

     const Impl& impl() const
     {
          return impl_;
     }

     Impl& mutableImpl()
     {
          return impl_;
     }
private:
     Impl impl_;
};


inline void throwOnError( const int retcode, std::string&& msg )
{
     static constexpr auto osslSuccessCode = 1;
     if( retcode != osslSuccessCode )
     {
          char errstr[ 256 ] = { 0 };
          ERR_error_string_n( ERR_peek_last_error(), errstr, sizeof( errstr ) );
          throw std::runtime_error{ msg + ": " + std::string{ errstr } };
     }
}


struct Md5DigestImpl
{
     Md5DigestImpl()
     {
          throwOnError( MD5_Init( &ctx_ ), "MD5 context init failed" );
     }
     void update( const char* data, std::size_t len )
     {
          throwOnError( MD5_Update( &ctx_, data, len ), "MD5 update failed" );
     }
     void finalize( std::uint8_t* out )
     {
          throwOnError( MD5_Final( out, &ctx_ ), "MD5 finalization failed" );
     }
     constexpr std::size_t digestLength() const
     {
          return MD5_DIGEST_LENGTH;
     }
private:
     MD5_CTX ctx_;
};


struct Sha1DigestImpl
{
     Sha1DigestImpl()
     {
          throwOnError( SHA1_Init( &ctx_ ), "SHA1 context init failed" );
     }
     void update( const char* data, std::size_t len )
     {
          throwOnError( SHA1_Update( &ctx_, data, len ), "SHA1 update failed" );
     }
     void finalize( std::uint8_t* out )
     {
          throwOnError( SHA1_Final( out, &ctx_ ), "SHA1 finalization failed" );
     }
     constexpr std::size_t digestLength() const
     {
          return SHA_DIGEST_LENGTH;
     }
private:
     SHA_CTX ctx_;
};


struct Sha256DigestImpl
{
     Sha256DigestImpl()
     {
          throwOnError( SHA256_Init( &ctx_ ), "SHA256 context init failed" );
     }
     void update( const char* data, std::size_t len )
     {
          throwOnError( SHA256_Update( &ctx_, data, len ), "SHA256 update failed" );
     }
     void finalize( std::uint8_t* out )
     {
          throwOnError( SHA256_Final( out, &ctx_ ), "SHA256 finalization failed" );
     }
     constexpr std::size_t digestLength() const
     {
          return SHA256_DIGEST_LENGTH;
     }
private:
     SHA256_CTX ctx_;
};


using Md5DigestFilter = BlockOutputFilter< Md5DigestImpl >;
using Sha1DigestFilter = BlockOutputFilter< Sha1DigestImpl >;
using Sha256DigestFilter = BlockOutputFilter< Sha256DigestImpl >;
