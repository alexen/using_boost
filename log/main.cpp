/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/utility/string_view.hpp>


static std::size_t operator ""_KB( long long unsigned kb )
{
     return kb * 1024;
}


namespace show_higher_than_info {


void init()
{
     boost::log::core::get()->set_filter( boost::log::trivial::severity >= boost::log::trivial::info );
}


} // namespace show_higher_than_info
namespace log_to_file_with_rotation {


void init( boost::string_view logMask, std::size_t maxBytes = 10_KB )
{
     boost::log::add_file_log(
          boost::log::keywords::file_name = logMask.to_string(),
          boost::log::keywords::rotation_size = maxBytes,
          boost::log::keywords::format = "[%TimeStamp%]: %Message%"
          );
     /// Эта запись нужна, чтобы работал плейсхолдер %TimeStamp%
     boost::log::add_common_attributes();
}


} // namespace log_to_file_with_rotation




int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          log_to_file_with_rotation::init( "boost_log_trivial.%N", 1_KB );

          for( auto i = 0u; i < 1_KB; ++i )
          {
               BOOST_LOG_TRIVIAL( debug ) << "This is debug";
               BOOST_LOG_TRIVIAL( info ) << "This is info";
          }
          BOOST_THROW_EXCEPTION( std::runtime_error{ "some kind of failure" } );
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
