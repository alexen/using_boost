/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <chrono>

#include <boost/make_shared.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/local_time/local_date_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/exception/diagnostic_information.hpp>


struct DateUtils
{
     using ptime = boost::posix_time::ptime;
     using time_zone_ptr = boost::local_time::time_zone_ptr;
     using local_date_time = boost::local_time::local_date_time;

     template< typename CharT >
     static std::chrono::system_clock::time_point Parse(
          const std::basic_string< CharT >& dateText,
          const CharT* const format )
     {
          static time_zone_ptr s_GMT( new boost::local_time::posix_time_zone( "GMT" ) );

          local_date_time value( boost::local_time::special_values::not_a_date_time, s_GMT );

          std::basic_istringstream< CharT > buffer( dateText );
          buffer.imbue(
               std::locale(
                    std::locale::classic(),
                    new boost::date_time::time_input_facet< ptime, CharT >( format ) ) );

          std::basic_string< CharT > dummy;
          if( buffer >> value && (buffer >> dummy).eof() )
          {
               //std::cout << "DEBUG: " << value.utc_time() << " EOF:" << buffer.eof() << "\n";
               auto timeInfo = boost::posix_time::to_tm( value.utc_time() );
               return std::chrono::system_clock::from_time_t( std::mktime( &timeInfo ) );
          }
          else
          {
               return std::chrono::system_clock::time_point::min();
          }
     }
};


template< typename CharT >
boost::posix_time::ptime toPtime_(
     const std::basic_string< CharT >& dateTime
     , const std::basic_string< CharT >& format
)
{
     static const auto tzGmt = boost::make_shared< boost::local_time::posix_time_zone >( "GMT" );
     boost::local_time::local_date_time value{
          boost::local_time::special_values::not_a_date_time
          , tzGmt
     };
     std::basic_istringstream< CharT > buffer{ dateTime };
     buffer.imbue(
          std::locale{
               std::locale::classic()
               , new boost::date_time::time_input_facet< boost::posix_time::ptime, CharT >{ format }
          }
     );
     buffer >> value;
     auto timeInfo = boost::posix_time::to_tm( value.utc_time() );
     return boost::posix_time::from_time_t( std::mktime( &timeInfo ) );
}


boost::posix_time::ptime toPtime( const std::string& timestampz, const std::string& format )
{
     return toPtime_< typename std::string::value_type >( timestampz, format );
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     static const auto lenLess =
          []( const char* const lhs, const char* const rhs )
          {
               return std::strlen( lhs ) < std::strlen( rhs );
          };

     try
     {
          const auto timestampzs = {
               "2020-10-27T22:33:12 PST+04:00"
               , "2020-10-27T22:33:12 +04:00"
               , "2020-10-27T22:33:12+04:00"
               , "2020-10-27T22:33:12"
          };
          constexpr auto fmt = "%Y-%m-%dT%H:%M:%S%ZP";

          const auto width = std::strlen( std::max( timestampzs, lenLess ) );
          std::cout << "===== My function:\n";
          for( auto&& tsz: timestampzs )
          {
               std::cout << std::setw( width ) << std::left
                    << tsz << ": " << toPtime( tsz, fmt ) << '\n';
          }
          std::cout << "===== Example function:\n";
          for( auto&& tsz: timestampzs )
          {
               std::cout << std::setw( width ) << std::left
                    << tsz << ": " << boost::posix_time::from_time_t( std::chrono::system_clock::to_time_t( DateUtils::Parse< char >( tsz, fmt ) ) ) << '\n';
          }

          boost::gregorian::date date{ 1978, boost::gregorian::Aug, 2 };
          std::cout
               << "===== Misc:\n"
               << "My birth date: " << date << '\n'
               << "As ISO: " << boost::gregorian::to_iso_string( date ) << '\n';
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
