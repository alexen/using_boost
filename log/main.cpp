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


namespace show_higher_than_info {

void init()
{
     boost::log::core::get()->set_filter( boost::log::trivial::severity >= boost::log::trivial::info );
}

} // namespace show_higher_than_info



int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          show_higher_than_info::init();

          BOOST_LOG_TRIVIAL( debug ) << "This is debug";
          BOOST_LOG_TRIVIAL( info ) << "This is info";
          BOOST_THROW_EXCEPTION( std::runtime_error{ "some kind of failure" } );
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
