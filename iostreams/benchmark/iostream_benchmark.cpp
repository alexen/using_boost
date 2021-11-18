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


static constexpr auto N_SAMPLES = 4u;
static constexpr auto N_ITERATIONS = 500u;
static constexpr auto N_THREADS = 8u;


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


namespace {
namespace custom {
namespace filters {
namespace single_char {


struct Transparent : boost::iostreams::dual_use_filter
{
     template< typename Source >
     int get( Source& src )
     {
          return boost::iostreams::get( src );
     }

     template< typename Sink >
     bool put( Sink& snk, int c )
     {
          return boost::iostreams::put( snk, c );
     }
};


} // namespace single_char
namespace multichar {
namespace by_char {


struct Transparent : boost::iostreams::multichar_dual_use_filter
{
     template< typename Source >
     std::streamsize read( Source& src, char* s, const std::streamsize n )
     {
          for( std::streamsize i = 0; i < n; ++i )
          {
               const auto c = boost::iostreams::get( src );
               if( c < 0 )
               {
                    return c;
               }
               s[ i ] = c;
          }
          return n;
     }

     template< typename Sink >
     std::streamsize write( Sink& snk, const char* s, const std::streamsize n )
     {
          for( std::streamsize i = 0; i < n; ++i )
          {
               const int c = s[ i ];
               if( not boost::iostreams::put( snk, c ) )
               {
                    return i;
               }
          }
          return n;
     }
};


} // namespace by_char
namespace by_block {


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


struct Transparent : boost::iostreams::multichar_dual_use_filter
{
     template< typename Source >
     std::streamsize read( Source& src, char* s, const std::streamsize n )
     {
          return boost::iostreams::read( src, s, n );
     }

     template< typename Sink >
     std::streamsize write( Sink& snk, const char* s, const std::streamsize n )
     {
          return boost::iostreams::write( snk, s, n );
     }
};


} // namespace by_block
} // namespace multichar
} // namespace filters
} // namespace custom
} // namespace {unnamed}


BASELINE_F( CopyStream, BoostCopy, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     boost::iostreams::copy( is, os );
}


BENCHMARK_F( CopyStream, StdStreamIt, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     std::copy(
          std::istream_iterator< char >{ is }
          , std::istream_iterator< char >{}
          , std::ostream_iterator< char >{ os }
          );
}


BENCHMARK_F( CopyStream, StdStreambufIt, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     std::copy(
          std::istreambuf_iterator< char >{ is }
          , std::istreambuf_iterator< char >{}
          , std::ostreambuf_iterator< char >{ os }
          );
}


BASELINE_F( IoFilter, NoFilters, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();
     boost::iostreams::copy( is, os );
}


BENCHMARK_F( IoFilter, BoostICounter, TestFixture, N_SAMPLES, N_ITERATIONS )
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


BENCHMARK_F( IoFilter, BoostOCounter, TestFixture, N_SAMPLES, N_ITERATIONS )
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


BENCHMARK_F( IoFilter, CustomICounter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::filters::multichar::by_block::Counter c;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_ASSERT( c.chars() == buffer().size() );
}


BENCHMARK_F( IoFilter, CustomOCounter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::filters::multichar::by_block::Counter c;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_ASSERT( c.chars() == buffer().size() );
}


BENCHMARK_F( IoFilter, SingleCharIFilter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::filters::single_char::Transparent f;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( f ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );
}


BENCHMARK_F( IoFilter, SingleCharOFilter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::filters::single_char::Transparent f;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( f ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );
}


BENCHMARK_F( IoFilter, MulticharCharIFilter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::filters::multichar::by_char::Transparent f;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( f ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );
}


BENCHMARK_F( IoFilter, MulticharCharOFilter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::filters::multichar::by_char::Transparent f;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( f ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );
}


BENCHMARK_F( IoFilter, MulticharBlockIFilter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::filters::multichar::by_block::Transparent f;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( f ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );
}


BENCHMARK_F( IoFilter, MulticharBlockOFilter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     custom::filters::multichar::by_block::Transparent f;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( f ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );
}
