/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>

#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/algorithm/hex.hpp>

#include <iostreams/iostream_filters.h>


namespace {
namespace aux {


using UpdCallback = std::function< void( const char*, std::size_t ) >;
using FinCallback = std::function< void( const std::string& ) >;


struct BlockProcessingImpl
{
     BlockProcessingImpl( UpdCallback&& updateCb, FinCallback&& finalizeCb )
          : updateCb_{ std::move( updateCb ) }, finalizeCb_{ std::move( finalizeCb ) }
     {}

     void update( const char* data, std::size_t len )
     {
          updateCb_( data, len );
     }

     void finalize( std::string param )
     {
          finalizeCb_( param );
     }
private:
     UpdCallback updateCb_;
     FinCallback finalizeCb_;
};


} // namespace aux
} // namespace {unnamed}


BOOST_AUTO_TEST_SUITE( IostreamFiltersTest )
BOOST_AUTO_TEST_CASE( GenericBlockOutputFilter )
{
     const std::string expectedData = "Some data";
     const std::string expectedFinalizeData = "Finalized";

     std::string updateData;
     std::size_t updateLen = 0u;
     std::string finalizeParam;

     BlockOutputFilter< aux::BlockProcessingImpl > filter {
          [ & ]( const char* s, std::size_t l )
          {
               updateData = s;
               updateLen = l;
          },
          [ & ]( const std::string& s )
          {
               finalizeParam = s;
          }
     };
     boost::iostreams::null_sink null;
     boost::iostreams::filtering_ostream fos;
     fos.push( filter );
     fos.push( null );
     fos << expectedData << std::flush;
     filter.finalize( expectedFinalizeData );

     BOOST_TEST( expectedData == updateData );
     BOOST_TEST( expectedData.size() == updateLen );
     BOOST_TEST( expectedFinalizeData == finalizeParam );
}


using BinaryBuffer = std::vector< std::uint8_t >;


BOOST_AUTO_TEST_CASE( Md5DigestFilterTest )
{
     const std::string sourceData = "My Bonny is over the ocean...";
     const BinaryBuffer expectedDigest {
          0x7f, 0xe2, 0x2d, 0xaa, 0xed, 0xb4, 0x79, 0xe5, 0xeb, 0x41, 0x7a, 0x0b,
          0x51, 0x77, 0x66, 0x63
     };
     Md5DigestFilter md5filt;
     boost::iostreams::null_sink null;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( md5filt ) );
     fos.push( boost::ref( null ) );
     fos << sourceData << std::flush;

     BinaryBuffer actualDigest( md5filt.impl().digestLength(), 0 );
     md5filt.finalize( &actualDigest[ 0 ] );

     BOOST_TEST( expectedDigest == actualDigest, boost::test_tools::per_element{} );
}


BOOST_AUTO_TEST_CASE( Sha1DigestFilterTest )
{
     const std::string sourceData = "My Bonny is over the ocean...";
     const BinaryBuffer expectedDigest {
          0xab, 0x38, 0x46, 0x41, 0x73, 0xcb, 0xef, 0xf0, 0xf4, 0xc7, 0xeb, 0x75,
          0x07, 0xf7, 0x24, 0x12, 0x19, 0x40, 0xf9, 0xe6
     };

     Sha1DigestFilter sha1filt;
     boost::iostreams::null_sink null;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( sha1filt ) );
     fos.push( boost::ref( null ) );
     fos << sourceData << std::flush;

     BinaryBuffer actualDigest( sha1filt.impl().digestLength(), 0 );
     sha1filt.finalize( &actualDigest[ 0 ] );

     BOOST_TEST( expectedDigest == actualDigest, boost::test_tools::per_element{} );
}


BOOST_AUTO_TEST_CASE( Sha256DigestFilterTest )
{
     const std::string sourceData = "My Bonny is over the ocean...";
     const BinaryBuffer expectedDigest {
          0xc7, 0xdc, 0x36, 0x3c, 0x39, 0x52, 0x01, 0xdc, 0x52, 0xef, 0x45, 0x33,
          0x1e, 0xfa, 0xab, 0x30, 0x49, 0xcc, 0x0f, 0x78, 0x34, 0x9a, 0x76, 0x21,
          0x3b, 0xd8, 0x4c, 0xe2, 0x05, 0xc5, 0xa2, 0xb4
     };

     Sha256DigestFilter sha256filt;
     boost::iostreams::null_sink null;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( sha256filt ) );
     fos.push( boost::ref( null ) );
     fos << sourceData << std::flush;

     BinaryBuffer actualDigest( sha256filt.impl().digestLength(), 0 );
     sha256filt.finalize( &actualDigest[ 0 ] );

     BOOST_TEST( expectedDigest == actualDigest, boost::test_tools::per_element{} );
}


BOOST_AUTO_TEST_CASE( AllDigestFilterTest )
{
     const std::string sourceData = "My Bonny is over the ocean...";
     const BinaryBuffer expectedMd5Digest {
          0x7f, 0xe2, 0x2d, 0xaa, 0xed, 0xb4, 0x79, 0xe5, 0xeb, 0x41, 0x7a, 0x0b,
          0x51, 0x77, 0x66, 0x63
     };
     const BinaryBuffer expectedSha1Digest {
          0xab, 0x38, 0x46, 0x41, 0x73, 0xcb, 0xef, 0xf0, 0xf4, 0xc7, 0xeb, 0x75,
          0x07, 0xf7, 0x24, 0x12, 0x19, 0x40, 0xf9, 0xe6
     };
     const BinaryBuffer expectedSha256Digest {
          0xc7, 0xdc, 0x36, 0x3c, 0x39, 0x52, 0x01, 0xdc, 0x52, 0xef, 0x45, 0x33,
          0x1e, 0xfa, 0xab, 0x30, 0x49, 0xcc, 0x0f, 0x78, 0x34, 0x9a, 0x76, 0x21,
          0x3b, 0xd8, 0x4c, 0xe2, 0x05, 0xc5, 0xa2, 0xb4
     };

     Md5DigestFilter md5filt;
     Sha1DigestFilter sha1filt;
     Sha256DigestFilter sha256filt;
     boost::iostreams::null_sink null;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( md5filt ) );
     fos.push( boost::ref( sha1filt ) );
     fos.push( boost::ref( sha256filt ) );
     fos.push( boost::ref( null ) );
     fos << sourceData << std::flush;

     BinaryBuffer actualMd5Digest( md5filt.impl().digestLength(), 0 );
     md5filt.finalize( &actualMd5Digest[ 0 ] );

     BinaryBuffer actualSha1Digest( sha1filt.impl().digestLength(), 0 );
     sha1filt.finalize( &actualSha1Digest[ 0 ] );

     BinaryBuffer actualSha256Digest( sha256filt.impl().digestLength(), 0 );
     sha256filt.finalize( &actualSha256Digest[ 0 ] );

     BOOST_TEST( expectedMd5Digest == actualMd5Digest, boost::test_tools::per_element{} );
     BOOST_TEST( expectedSha1Digest == actualSha1Digest, boost::test_tools::per_element{} );
     BOOST_TEST( expectedSha256Digest == actualSha256Digest, boost::test_tools::per_element{} );
}
BOOST_AUTO_TEST_SUITE_END()
