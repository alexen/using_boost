/// @file
/// @brief


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


namespace {


[[maybe_unused]] std::int64_t operator ""_KB( unsigned long long kb )
{
     return kb * 1024;
}


[[maybe_unused]] std::int64_t operator ""_MB( unsigned long long mb )
{
     return mb * 1_KB * 1_KB;
}


namespace bm_env {
namespace consts {


static constexpr auto N_SAMPLES = 3u;
static constexpr auto N_ITERATIONS = 300u;
static constexpr auto N_THREADS = 8u;


} // namespace consts


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
};


} // namespace bm_env
} // namespace {unnamed}


BASELINE_F( CopyStream, BoostIosCopy, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };
     celero::DoNotOptimizeAway( boost::iostreams::copy( is, os ) );
}


BENCHMARK_F( CopyStream, StreamIter, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };
     celero::DoNotOptimizeAway(
          std::copy(
               std::istream_iterator< char >{ is }
               , std::istream_iterator< char >{}
               , std::ostream_iterator< char >{ os }
               )
          );
}


BENCHMARK_F( CopyStream, StreambufIter, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };
     celero::DoNotOptimizeAway(
          std::copy(
               std::istreambuf_iterator< char >{ is }
               , std::istreambuf_iterator< char >{}
               , std::ostreambuf_iterator< char >{ os }
               )
          );
}


namespace {
namespace bm_env {
namespace fastest {


void copy( std::istream& is, std::ostream& os )
{
     celero::DoNotOptimizeAway(
          std::copy(
               std::istreambuf_iterator< char >{ is }
               , std::istreambuf_iterator< char >{}
               , std::ostreambuf_iterator< char >{ os }
               )
          );
}


void copyToNull( std::istream& is )
{
     static boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };
     copy( is, os );
}


} // namespace fastest
} // namespace bm_env
} // namespace {unnamed}



BASELINE_F( IoFilter, NoFilters, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     bm_env::fastest::copyToNull( is );
}


BENCHMARK_F( IoFilter, BoostInCounter, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };

     boost::iostreams::counter c;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( is );

     bm_env::fastest::copyToNull( is );

     BOOST_ASSERT( boost::numeric_cast< std::size_t >( c.characters() ) == buffer().size() );
}


BENCHMARK_F( IoFilter, BoostOutCounter, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     boost::iostreams::counter c;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     bm_env::fastest::copy( is, fos );

     BOOST_ASSERT( boost::numeric_cast< std::size_t >( c.characters() ) == buffer().size() );
}


BENCHMARK_F( IoFilter, CustomInCounter, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };

     using_boost::iostreams::filters::multichar::Counter c;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( is );

     bm_env::fastest::copyToNull( is );

     BOOST_ASSERT( c.chars() == buffer().size() );
}


BENCHMARK_F( IoFilter, CustomOutCounter, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     using_boost::iostreams::filters::multichar::Counter c;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     bm_env::fastest::copy( is, fos );

     BOOST_ASSERT( c.chars() == buffer().size() );
}


BENCHMARK_F( IoFilter, CharInFilt, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };

     using_boost::iostreams::filters::single_char::Transparent t;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( t ) );
     fis.push( is );

     bm_env::fastest::copyToNull( is );
}


BENCHMARK_F( IoFilter, CharOutFilt, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     using_boost::iostreams::filters::single_char::Transparent t;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( t ) );
     fos.push( os );

     bm_env::fastest::copy( is, fos );
}


BENCHMARK_F( IoFilter, BlockInFilt, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };

     using_boost::iostreams::filters::multichar::Transparent t;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( t ) );
     fis.push( is );

     bm_env::fastest::copyToNull( is );
}


BENCHMARK_F( IoFilter, BlockOutFilt, bm_env::TestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( buffer() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     using_boost::iostreams::filters::multichar::Transparent t;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( t ) );
     fos.push( os );

     bm_env::fastest::copy( is, fos );
}
