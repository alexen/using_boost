/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <gtest/gtest.h>
#include <fstream>
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


TEST( IostreamFilters, GenericBlockOutputFilter )
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
     std::ofstream null{ "/dev/null" };
     boost::iostreams::filtering_ostream fos;
     fos.push( filter );
     fos.push( null );
     fos << expectedData << std::flush;
     filter.finalize( expectedFinalizeData );

     ASSERT_EQ( expectedData, updateData );
     ASSERT_EQ( expectedData.size(), updateLen );
     ASSERT_EQ( expectedFinalizeData, finalizeParam );
}
