/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <fstream>
#include <iterator>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/null.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <celero/Celero.h>

#include <iostreams/base64.h>


namespace {


using CeleroExperimentValue = celero::TestFixture::ExperimentValue;
using CeleroExperimentValues = std::vector< CeleroExperimentValue >;
using Buffer = std::vector< char >;


namespace bm_env {


[[maybe_unused]] std::int64_t operator ""_KB( unsigned long long kb )
{
     return kb * 1024;
}


[[maybe_unused]] std::int64_t operator ""_MB( unsigned long long mb )
{
     return mb * 1_KB * 1_KB;
}

namespace consts {


static constexpr auto N_SAMPLES = 3u;
static constexpr auto N_ITERATIONS = 400u;
static const CeleroExperimentValues BUFFER_SIZES{ 100_KB, 300_KB, 800_KB };


} // namespace consts


class EncodingTestFixture : public celero::TestFixture
{
public:
     CeleroExperimentValues getExperimentValues() const override
     {
          return consts::BUFFER_SIZES;
     }

     void setUp( const CeleroExperimentValue& value ) override
     {
          prepareData( boost::numeric_cast< std::uint64_t >( value.Value ) );
     }

     void tearDown() override
     {
          clearData();
     }

     const Buffer& data() const noexcept
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


class DecodingTestFixture : public celero::TestFixture
{
public:
     CeleroExperimentValues getExperimentValues() const override
     {
          return consts::BUFFER_SIZES;
     }

     void setUp( const CeleroExperimentValue& value ) override
     {
          prepareData( boost::numeric_cast< std::uint64_t >( value.Value ) );
     }

     void tearDown() override
     {
          clearData();
     }

     const Buffer& data() const noexcept
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
     static void encode( const Buffer& in, Buffer& out )
     {
          boost::iostreams::filtering_istream is{ boost::make_iterator_range( in ) };
          boost::iostreams::filtering_ostream os{ boost::iostreams::back_inserter( out ) };
          base64::encode( is, os );
          BOOST_ASSERT( out.size() > in.size() );
     }
     void prepareData( const std::uint64_t bytes )
     {
          Buffer binary;
          generateData( binary, bytes );
          encode( binary, buffer_ );
     }
     void clearData()
     {
          clear( buffer_ );
     }

     Buffer buffer_;
};


} // namespace bm_env
} // namespace {unnamed}


BASELINE_F( Base64Encoding, BoostOnIterators, bm_env::EncodingTestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     using Base64EncodingIterator =
          boost::archive::iterators::base64_from_binary<
               boost::archive::iterators::transform_width<
                    std::istreambuf_iterator< char >
                    , 6
                    , 8
               >
          >;
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( data() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     std::copy(
          Base64EncodingIterator{ std::istreambuf_iterator< char >{ is } }
          , Base64EncodingIterator{ std::istreambuf_iterator< char >{} }
          , std::ostreambuf_iterator< char >{ os }
     );
}


BENCHMARK_F( Base64Encoding, BoostOnPtrs, bm_env::EncodingTestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     using Base64EncodingIterator =
          boost::archive::iterators::base64_from_binary<
               boost::archive::iterators::transform_width<
                    const char*
                    , 6
                    , 8
               >
          >;
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     std::copy(
          Base64EncodingIterator{ data().data() }
          , Base64EncodingIterator{ data().data() + data().size() }
          , std::ostreambuf_iterator< char >{ os }
     );
}


BENCHMARK_F( Base64Encoding, Custom, bm_env::EncodingTestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( data() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     base64::encode( is, os );
}


BASELINE_F( Base64Decoding, BoostOnIterators, bm_env::DecodingTestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     using Base64DecodingIterator =
          boost::archive::iterators::transform_width<
               boost::archive::iterators::binary_from_base64<
                    std::istreambuf_iterator< char >
               >
               , 8
               , 6
               >;

     boost::iostreams::filtering_istream is{ boost::make_iterator_range( data() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     std::copy(
          Base64DecodingIterator{ std::istreambuf_iterator< char >{ is } }
          , Base64DecodingIterator{ std::istreambuf_iterator< char >{} }
          , std::ostreambuf_iterator< char >{ os }
          );
}


BENCHMARK_F( Base64Decoding, BoostOnPtrs, bm_env::DecodingTestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     using Base64DecodingIterator =
     boost::archive::iterators::transform_width<
          boost::archive::iterators::binary_from_base64<
               const char*
          >
          , 8
          , 6
          >;

     boost::iostreams::filtering_istream is{ boost::make_iterator_range( data() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     std::copy(
          Base64DecodingIterator{ data().data() }
          , Base64DecodingIterator{ data().data() + data().size() }
          , std::ostreambuf_iterator< char >{ os }
          );
}


BENCHMARK_F( Base64Decoding, Custom, bm_env::DecodingTestFixture, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( data() ) };
     boost::iostreams::stream< boost::iostreams::null_sink > os{
          boost::iostreams::null_sink{}
     };

     base64::decode( is, os );
}

