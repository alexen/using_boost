/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <fstream>
#include <iterator>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/null.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <celero/Celero.h>

#include <iostreams/base64.h>


CELERO_MAIN


using CeleroExperimentValue = celero::TestFixture::ExperimentValue;
using CeleroExperimentValues = std::vector< CeleroExperimentValue >;
using Buffer = std::vector< char >;


std::int64_t operator ""_KB( unsigned long long kb )
{
     return kb * 1024;
}


std::int64_t operator ""_MB( unsigned long long mb )
{
     return mb * 1_KB * 1_KB;
}


class TestFixture : public celero::TestFixture
{
public:
     CeleroExperimentValues getExperimentValues() const override
     {
          return { 300_KB, 800_KB, 1_MB };
     }

     void setUp( const CeleroExperimentValue& value ) override
     {
          prepareData( boost::numeric_cast< std::uint64_t >( value.Value ) );
     }

     const Buffer& data() const noexcept
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


BASELINE_F( Base64Encoding, BoostOnIterators, TestFixture, 3, 300 )
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
     std::ostream& os = null();

     std::copy(
          Base64EncodingIterator{ std::istreambuf_iterator< char >{ is } }
          , Base64EncodingIterator{ std::istreambuf_iterator< char >{} }
          , std::ostreambuf_iterator< char >{ os }
     );
}


BENCHMARK_F( Base64Encoding, BoostOnPtrs, TestFixture, 3, 300 )
{
     using Base64EncodingIterator =
          boost::archive::iterators::base64_from_binary<
               boost::archive::iterators::transform_width<
                    const char*
                    , 6
                    , 8
               >
          >;
     std::ostream& os = null();

     std::copy(
          Base64EncodingIterator{ data().data() }
          , Base64EncodingIterator{ data().data() + data().size() }
          , std::ostreambuf_iterator< char >{ os }
     );
}


BENCHMARK_F( Base64Encoding, Custom, TestFixture, 3, 300 )
{
     boost::iostreams::filtering_istream is{ boost::make_iterator_range( data() ) };
     std::ostream& os = null();

     base64::encode( is, os );
}
