/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <vector>
#include <stdexcept>
#include <iostream>
#include <set>

#include <boost/core/ignore_unused.hpp>

#include <boost/type_index.hpp>
#include <boost/utility/string_view.hpp>

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm_ext/is_sorted.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <boost/exception/diagnostic_information.hpp>


template< typename Cont >
std::size_t countUnique( const Cont& cont )
{
     return std::set< typename Cont::value_type >( std::cbegin( cont ), std::cend( cont ) ).size();
}


template< typename T >
std::string prettyViewOf()
{
     return boost::typeindex::type_id_with_cvr< T >().pretty_name();
}


template< typename Cont >
std::string prettyViewOf( const Cont& )
{
     return prettyViewOf< typename Cont::value_type >();
}


enum class PrintContainer{ False, True };


template< typename Cont >
void stat( std::string&& title, const Cont& cont, const PrintContainer printCont, std::ostream& os = std::cout )
{
     using ContValueType = typename Cont::value_type;
     os << title << '\n'
          << "- element type   : " << prettyViewOf( cont ) << '\n'
          << "- total elements : " << cont.size() << '\n'
          << "- unique elements: " << countUnique( cont ) << '\n'
          << "- is sorted      : " << std::boolalpha << boost::range::is_sorted( cont ) << '\n';
     if( printCont == PrintContainer::True )
     {
          os << "- content        : ";
          std::for_each(
               std::cbegin( cont )
               , std::cend( cont )
               , [ &os ]( const ContValueType& v )
               {
                    os << '[' << reinterpret_cast< const void* >( &v[0] ) << "]:" << v << ' ';
               }
               );
          os << '\n';
     }
}


template< typename Cont >
void stat( std::string&& title, const Cont& cont, std::ostream& os = std::cout )
{
     stat( std::move( title ), cont, PrintContainer::False, os );
}


template< template< typename > typename Cont >
std::vector< boost::string_view > sortedUnique( const Cont< std::string >& strings )
{
     std::vector< boost::string_view > result;
     boost::range::copy( strings, std::back_inserter( result ) );
     if( !boost::range::is_sorted( result ) )
     {
          boost::range::sort( result );
     }
     result.erase( boost::range::unique( result ).end(), result.end() );
     return result;
}


int main( int argc, char** argv )
{
     using namespace std::string_literals;
     boost::ignore_unused( argc, argv );
     try
     {
          static const auto uuids = {
               "7bbfd031-76fb-495f-98ac-a3d83fb6c399"s
               , "b9b75657-7e7d-4201-8910-be26f04df25e"s
               , "dc40b30b-d8db-464b-81f2-b35576cf904e"s
               , "a8a54eb0-9005-4ae4-98d9-d7e2aa20361c"s
               , "d9ba5500-4617-4d9e-9d17-7b3541b48ad4"s
               , "bcc2ff7d-6f0d-42b6-9187-0be9ab4e3595"s
               , "66696ca4-74a9-40e8-ac7d-a40e9a7d0573"s
               , "a8a54eb0-9005-4ae4-98d9-d7e2aa20361c"s
               , "4a7d3183-a1ac-463e-83da-7d6fa86a9cb0"s
               , "3b9d7d92-b3e1-49bc-8ab0-e9f068310097"s
               , "bcc2ff7d-6f0d-42b6-9187-0be9ab4e3595"s
               , "17bc6d33-49d6-4b7b-99f2-5c63d41f10a1"s
               , "eb8bed02-1468-4e3c-883b-bf0c6d712627"s
               , "8b85f3b3-f032-4000-82b5-52b912e08e35"s
               , "3b9d7d92-b3e1-49bc-8ab0-e9f068310097"s
               , "3a583aac-26f7-410e-939e-2929a5aab245"s
               , "c3dbbb0b-ae4e-4a1c-b8e7-2dbab233deb7"s
               , "4a7d3183-a1ac-463e-83da-7d6fa86a9cb0"s
               , "b9b75657-7e7d-4201-8910-be26f04df25e"s
               , "3a583aac-26f7-410e-939e-2929a5aab245"s
               , "17bc6d33-49d6-4b7b-99f2-5c63d41f10a1"s
               , "c3dbbb0b-ae4e-4a1c-b8e7-2dbab233deb7"s
               , "b8c0ce62-183c-4693-a98e-f18df1d653a5"s
               , "19ea1e91-7004-4d6e-8adb-2390131960b5"s
               , "d9ba5500-4617-4d9e-9d17-7b3541b48ad4"s
               , "c3a5fa44-69e3-457d-b166-197440cb0c06"s
               , "b8c0ce62-183c-4693-a98e-f18df1d653a5"s
               , "66696ca4-74a9-40e8-ac7d-a40e9a7d0573"s
               , "7bbfd031-76fb-495f-98ac-a3d83fb6c399"s
               , "c4c0bd3d-1d29-41b6-894b-cee2c14ebe8a"s
               , "dc40b30b-d8db-464b-81f2-b35576cf904e"s
               , "19ea1e91-7004-4d6e-8adb-2390131960b5"s
               , "ddc6979a-a3fb-4b54-b7b8-10b9ae517264"s
               , "ddc6979a-a3fb-4b54-b7b8-10b9ae517264"s
               , "e70cc26d-845a-4c5f-a5bf-6cea1ed4bc31"s
               , "c3a5fa44-69e3-457d-b166-197440cb0c06"s
               , "e70cc26d-845a-4c5f-a5bf-6cea1ed4bc31"s
               , "c4c0bd3d-1d29-41b6-894b-cee2c14ebe8a"s
               , "eb8bed02-1468-4e3c-883b-bf0c6d712627"s
               , "8b85f3b3-f032-4000-82b5-52b912e08e35"s
          };

          stat( "Source", uuids, PrintContainer::True );
          stat( "Result", sortedUnique( uuids ), PrintContainer::True );
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
