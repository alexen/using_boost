/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <boost/date_time.hpp>
#include <boost/date_time/local_time/local_time_io.hpp>
#include <ctime>
#include <chrono>
#include <sstream>

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


#if 0
    using CharT = wchar_t;
    #define T(lit) L##lit
#else
    using CharT = char;
    #define T(lit) lit
#endif


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          using namespace std::chrono_literals;
          using C = std::chrono::system_clock;
          std::cout << std::boolalpha << std::unitbuf;

          C::time_point with_zone, without_zone;

          // all three equivalent:
          with_zone = DateUtils::Parse< CharT >(
               T( "2016-12-03T07:09:01 PST-05:00" ),
               T( "%Y-%m-%dT%H:%M:%S%ZP" ) );
          with_zone = DateUtils::Parse< CharT >(
               T( "2016-12-03T07:09:01 -05:00" ),
               T( "%Y-%m-%dT%H:%M:%S%ZP" ) );
          with_zone = DateUtils::Parse< CharT >(
               T( "2016-12-03T07:09:01-05:00" ),
               T( "%Y-%m-%dT%H:%M:%S%ZP" ) );
          without_zone = DateUtils::Parse< CharT >(
               T( "2016-12-03T07:09:01" ),
               T( "%Y-%m-%dT%H:%M:%S" ) );
          std::cout << "time_point equal? " << (with_zone == without_zone) << "\n";
          {
               std::time_t t_with_zone = C::to_time_t( with_zone );
               std::time_t t_without_zone = C::to_time_t( without_zone );

               std::cout << "time_t equal? " << (t_with_zone == t_without_zone) << "\n";
          }

          std::cout << (without_zone - with_zone) / 1h << " hours difference\n";
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
