/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include <boost/optional/optional.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/greg_serialize.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/exception/diagnostic_information.hpp>


class Person {
public:
     Person(
          const std::string& lastName,
          const std::string& firstName,
          const boost::optional< std::string >& middleName = boost::none,
          const boost::optional< boost::gregorian::date >& birthDate = boost::none
          )
          : lastName_{ lastName }
          , firstName_{ firstName }
          , middleName_{ middleName }
          , birthDate_{ birthDate }
     {}

     Person(
          const std::string& lastName,
          const std::string& firstName,
          const boost::gregorian::date& birthDate
          )
          : Person{ lastName, firstName, boost::none, birthDate }
     {}

     const std::string& lastName() const noexcept
     {
          return lastName_;
     }

     const std::string& firstName() const noexcept
     {
          return firstName_;
     }

     const boost::optional< std::string >& middleName() const noexcept
     {
          return middleName_;
     }

     const boost::optional< boost::gregorian::date >& birthDate() const noexcept
     {
          return birthDate_;
     }

     template< typename Archive >
     void serialize( Archive& ar, const unsigned /*version*/ )
     {
          ar & BOOST_SERIALIZATION_NVP( lastName_ );
          ar & BOOST_SERIALIZATION_NVP( firstName_ );
          ar & BOOST_SERIALIZATION_NVP( middleName_ );
          ar & BOOST_SERIALIZATION_NVP( birthDate_ );
     }

private:
     Person() = default;

     std::string lastName_;
     std::string firstName_;
     boost::optional< std::string > middleName_;
     boost::optional< boost::gregorian::date > birthDate_;

     friend class boost::serialization::access;
};


class PersonsList {
public:
     using ListType = std::vector< Person >;
     using ListIterator = ListType::const_iterator;

     PersonsList() = default;
     PersonsList( std::initializer_list< Person > persons )
          : persons_{ persons.begin(), persons.end() }
     {}

     template< typename Archive >
     void serialize( Archive& ar, const unsigned /*version*/ )
     {
          ar & persons_;
     }

     ListIterator begin() const noexcept
     {
          return std::begin( persons_ );
     }

     ListIterator end() const noexcept
     {
          return std::end( persons_ );
     }
private:
     std::vector< Person > persons_;
};


std::ostream& operator<<( std::ostream& os, const Person& p )
{
     os << p.lastName() << ' ' << p.firstName();
     if( const auto& middleName = p.middleName() )
     {
          os << ' ' << *middleName;
     }
     if( const auto& birthDate = p.birthDate() )
     {
          os << ", " << *birthDate;
     }
     return os;
}


std::ostream& operator<<( std::ostream& os, const PersonsList& pl )
{
     for( auto&& p: pl )
     {
          os << p << '\n';
     }
     return os;
}


boost::gregorian::date operator ""_date( const char* s, std::size_t n )
{
     return boost::gregorian::from_string( std::string{ s, n } );
}


boost::gregorian::date operator ""_date( unsigned long long d )
{
     const auto dd = d % 100;
     d /= 100;
     return boost::gregorian::date{
          static_cast< boost::gregorian::date::year_type >( d / 100 ),
          static_cast< boost::gregorian::date::month_type >( d % 100 ),
          static_cast< boost::gregorian::date::day_type >( dd )
          };
}


template< typename Archive, typename Stream, typename Entity >
void save( Stream& s, const Entity& e )
{
     Archive{ s } << e;
}


template< typename Archive, typename Stream, typename Entity >
void load( Stream& s, Entity& e )
{
     Archive{ s } >> e;
}


void save( const PersonsList& persons, const boost::filesystem::path& file, bool compress, bool binary )
{
     std::ofstream ofile{ file.string(), std::ios::binary };
     boost::iostreams::filtering_stream< boost::iostreams::output > ofilters;
     if( compress )
     {
          ofilters.push( boost::iostreams::zlib_compressor() );
     }
     ofilters.push( ofile );
     if( binary )
     {
          save< boost::archive::binary_oarchive >( ofilters, persons );
     }
     else
     {
          save< boost::archive::text_oarchive >( ofilters, persons );
     }
}


PersonsList load( const boost::filesystem::path& file, bool compressed, bool binary )
{
     std::ifstream ifile{ file.string(), std::ios::binary };
     boost::iostreams::filtering_stream< boost::iostreams::input > ifilters;
     if( compressed )
     {
          ifilters.push( boost::iostreams::zlib_decompressor() );
     }
     ifilters.push( ifile );
     PersonsList persons;
     if( binary )
     {
          load< boost::archive::binary_iarchive >( ifilters, persons );
     }
     else
     {
          load< boost::archive::text_iarchive >( ifilters, persons );
     }
     return persons;
}


int main( int argc, char** argv )
{
     using namespace std::string_literals;
     try
     {
          const auto begin = argv;
          const auto end = argv + argc;

          const auto useBinary   = std::find( begin, end, "-b"s ) != end;
          const auto useCompress = std::find( begin, end, "-z"s ) != end;

          const auto filepath = "trash/persons"
               + std::string{ useBinary ? ".bin" : ".text" }
               + std::string{ useCompress ? ".zlib" : "" };

          std::cout << "File: " << filepath << '\n'
                    << "Binary mode: " << (useBinary ? "ON" : "OFF") << '\n'
                    << "Compressing: " << (useCompress ? "ON" : "OFF") << '\n';

          {
               const PersonsList persons{
                    { "Пиминов", "Сергей" },
                    { "Кулибин", "Федул", "1896-03-12"_date },
                    { "Загибов", "Александр", "Иванович"s, 19631107_date },
                    { "Кибитка", "Семен", "Килиманджарович"s, "1966-09-21"_date },
                    { "Капустник", "Михаил", "Федорович"s, "1978-06-24"_date },
                    { "Пижон", "Владилен", "Феоктистович"s, "1937-08-19"_date },
                    { "Сучка", "Казимир", "Червонцев"s, 19140927_date }
               };

               std::cout << "Source:\n" << persons << '\n';
               save( persons, filepath, useCompress, useBinary );
          }

          {
               const auto persons = load( filepath, useCompress, useBinary );
               std::cout << "Result:\n" << persons << '\n';
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
