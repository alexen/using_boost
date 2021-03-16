/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <strings.h>

#include <stdexcept>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/range/iterator_range_core.hpp>


static const std::string data = R"ini(
[general]
main_path = /path/to/somewhere
number_of_something = 96

[specific]
inner_interval = 15s
bad_suffix_interval = 21xoxoxo
optional_inner_interval = 25m
big_interval = 119h
gender = male

)ini";


template< typename T >
struct ValueTranslator
{
     using internal_type = std::string;
     using external_type = T;

     boost::optional< external_type > get_value( const internal_type& it )
     {
          return boost::lexical_cast< external_type >( it );
     }

     boost::optional< internal_type > put_value( const external_type& ) = delete;
};


struct IniConfigError : std::runtime_error
{
     explicit IniConfigError( std::string&& what ) : std::runtime_error{ what } {}
};

struct ParameterNotFound : IniConfigError
{
     ParameterNotFound( const std::string& section, const std::string& key )
          : IniConfigError{ "no key \"" + key + "\" found at section \"" + section + "\"" }
     {}
};


struct ValueParsingError : IniConfigError
{
     explicit ValueParsingError( const std::string& value )
          : IniConfigError( "cannot parse value \"" + value + "\"" )
     {}
};


class IniConfigParser
{
public:
     explicit IniConfigParser( const boost::filesystem::path& config )
     {
          boost::property_tree::ini_parser::read_ini( config.string(), propertyTree_ );
     }

     explicit IniConfigParser( std::istream& config )
     {
          boost::property_tree::ini_parser::read_ini( config, propertyTree_ );
     }

     template< class T >
     inline boost::optional< T > getOptionalConfigParameter( const std::string& section, const std::string& key ) const
     {
          return getValue< T >( section, key );
     }

     template< typename T >
     inline T getConfigParameter( const std::string& section, const std::string& key ) const
     {
          if( auto&& value = getOptionalConfigParameter< T >( section, key ) )
          {
               return *value;
          }
          BOOST_THROW_EXCEPTION( ParameterNotFound( section, key ) );
          return {};
     }

     const boost::property_tree::ptree& ptree() const
     {
          return propertyTree_;
     }

private:
     template< typename T >
     inline boost::optional< T > getValue( const std::string& section, const std::string& key ) const
     {
          return propertyTree_.get_optional< T >(
               section + '.' + key
               , ValueTranslator< T >{}
               );
     }

     boost::property_tree::ptree propertyTree_;
};


template<>
struct ValueTranslator< boost::posix_time::time_duration >
{
     boost::optional< boost::posix_time::time_duration > get_value( const std::string& value )
     {
          std::size_t suffix = std::string::npos;
          const auto num = std::stoll( value, &suffix );
          if( suffix != std::string::npos )
          {
               switch( value.at( suffix ) )
               {
                    case 'm': return boost::posix_time::minutes{ num };
                    case 'h': return boost::posix_time::hours{ num };
                    case 's': break;
                    default:
                         BOOST_THROW_EXCEPTION( ValueParsingError{ value } );
               }
          }
          return boost::posix_time::seconds{ num };
     }
};


std::ostream& operator<<( std::ostream& os, const IniConfigParser& icp )
{
     boost::property_tree::write_ini( os, icp.ptree() );
     return os;
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          boost::iostreams::filtering_istream istr{ boost::make_iterator_range( data ) };

          IniConfigParser parser{ istr };

          std::cout
               << "SOURCE INI:\n"
               << "===========\n"
               << parser << '\n';

          std::cout
               << "integer value          : " << parser.getConfigParameter< int >( "general", "number_of_something" ) << '\n'
               << "string value           : " << parser.getConfigParameter< std::string >( "general", "main_path" ) << '\n'
               << "time duration          : " << parser.getConfigParameter< boost::posix_time::time_duration >( "specific", "inner_interval" ) << '\n'
               << "optional time duration : " << parser.getOptionalConfigParameter< boost::posix_time::time_duration >( "specific", "optional_inner_interval" ) << '\n'
               << "big interval           : " << parser.getConfigParameter< boost::posix_time::time_duration >( "specific", "big_interval" ) << '\n'
               ;

          std::cout
               << "RESULT INI:\n"
               << "===========\n"
               << parser << '\n';
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
