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

#include <iostreams/filters.h>


CELERO_MAIN


std::int64_t operator ""_KB( unsigned long long kb )
{
     return kb * 1024;
}


std::int64_t operator ""_MB( unsigned long long mb )
{
     return mb * 1_KB * 1_KB;
}


static constexpr auto N_SAMPLES = 3u;
static constexpr auto N_ITERATIONS = 300u;
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
               100_KB,
               300_KB,
               600_KB
          };
     }

     void setUp( const ExperimentValue& value ) override
     {
          prepareData( boost::numeric_cast< std::uint64_t >( value.Value ) );
     }

     void tearDown() override
     {
          clearData();
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
     static void clear( Buffer& out )
     {
          out.clear();
          BOOST_ASSERT( out.empty() );
     }
     static void generateData( Buffer& out, const std::uint64_t bytes )
     {
          BOOST_ASSERT( out.empty() );

          out.reserve( bytes );

          std::ifstream ifile{ "/dev/urandom", std::ios::binary };
          std::copy_n( std::istreambuf_iterator{ ifile }, bytes, std::back_inserter( out ) );

          BOOST_ASSERT( out.size() == bytes );
     }
     void prepareData( const std::uint64_t bytes )
     {
          clear( buffer_ );
          generateData( buffer_, bytes );
     }
     void clearData()
     {
          clear( buffer_ );
     }

     Buffer buffer_;
     boost::iostreams::stream< boost::iostreams::null_sink > null_;
};


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

     BOOST_ASSERT( boost::numeric_cast< std::size_t >( c.characters() ) == buffer().size() );
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

     BOOST_ASSERT( boost::numeric_cast< std::size_t >( c.characters() ) == buffer().size() );
}


BENCHMARK_F( IoFilter, CustomICounter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     using_boost::iostreams::filters::multichar::Counter c;
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

     using_boost::iostreams::filters::multichar::Counter c;
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

     using_boost::iostreams::filters::single_char::Transparent f;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( f ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );
}


BENCHMARK_F( IoFilter, SingleCharOFilter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     using_boost::iostreams::filters::single_char::Transparent f;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( f ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );
}


BENCHMARK_F( IoFilter, MulticharIFilter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     using_boost::iostreams::filters::multichar::Transparent f;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( f ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );
}


BENCHMARK_F( IoFilter, MulticharOFilter, TestFixture, N_SAMPLES, N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     std::ostream& os = null();

     using_boost::iostreams::filters::multichar::Transparent f;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( f ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );
}
