/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <fstream>
#include <sstream>
#include <iostream>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/filter/counter.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <celero/Celero.h>
#include <celero/Console.h>


CELERO_MAIN


#define KB( n ) (n * 1024)
#define MB( n ) (KB( n ) * 1024)


class TestFixture : public celero::TestFixture
{
public:
     using ExperimentalValue = celero::TestFixture::ExperimentValue;
     using ExperimentalValues = std::vector< ExperimentValue >;
     using Buffer = std::vector< char >;

     ExperimentalValues getExperimentValues() const override
     {
          return {
               KB( 300 ),
               KB( 500 ),
               MB(   1 ),
          };
     }

     void setUp( const ExperimentValue& value ) override
     {
          prepareData( boost::numeric_cast< std::uint64_t >( value.Value ) );
     }

     const Buffer& buffer() const noexcept
     {
          return buffer_;
     }

     std::ostream& null()
     {
          return null_;
     }

private:
     void prepareData( const std::uint64_t bytes )
     {
          buffer_.clear();
          buffer_.resize( bytes );

          BOOST_ASSERT( buffer_.empty() );

          std::ifstream{ "/dev/urandom", std::ios::binary }
               .read( buffer_.data(), buffer_.size() );

          BOOST_ASSERT( buffer_.size() == bytes );
     }

     Buffer buffer_;
     boost::iostreams::stream< boost::iostreams::null_sink > null_;
};


BASELINE_F( CopyStream, BoostCopy, TestFixture, 3, 300 )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     boost::iostreams::copy( is, os );
}


BENCHMARK_F( CopyStream, StdStreamIt, TestFixture, 3, 300 )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     std::copy(
          std::istream_iterator< char >{ is }
          , std::istream_iterator< char >{}
          , std::ostream_iterator< char >{ os }
          );
}


BENCHMARK_F( CopyStream, StdStreambufIt, TestFixture, 3, 300 )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     std::copy(
          std::istreambuf_iterator< char >{ is }
          , std::istreambuf_iterator< char >{}
          , std::ostreambuf_iterator< char >{ os }
          );
}


BASELINE_F( IoFilter, NoFilters, TestFixture, 1, 500 )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();
     boost::iostreams::copy( is, os );
}


BENCHMARK_F( IoFilter, BoostICounter, TestFixture, 1, 500 )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     boost::iostreams::counter c;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_ASSERT( c.characters() == buffer().size() );
}


BENCHMARK_F( IoFilter, BoostOCounter, TestFixture, 1, 500 )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     boost::iostreams::counter c;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_ASSERT( c.characters() == buffer().size() );
}


namespace {
namespace custom {


struct Counter : boost::iostreams::multichar_dual_use_filter
{
     explicit Counter( unsigned n = 0 ) : chars_{ n } {}

     template< typename Source >
     std::streamsize read( Source& src, char* s, std::streamsize n )
     {
          const auto result = boost::iostreams::read( src, s, n );
          chars_ += result > 0 ? result : 0;
          return result;
     }

     template< typename Sink >
     std::streamsize write( Sink& snk, const char* s, std::streamsize n )
     {
          const auto result = boost::iostreams::write( snk, s, n );
          chars_ += result > 0 ? result : 0;
          return result;
     }

     unsigned chars() const noexcept
     {
          return chars_;
     }

private:
     unsigned chars_ = 0;
};


} // namespace custom
} // namespace {unnamed}


BENCHMARK_F( IoFilter, CustomOCounter, TestFixture, 1, 500 )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::Counter c;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_ASSERT( c.chars() == buffer().size() );
}


BENCHMARK_F( IoFilter, CustomICounter, TestFixture, 1, 500 )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::Counter c;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_ASSERT( c.chars() == buffer().size() );
}
