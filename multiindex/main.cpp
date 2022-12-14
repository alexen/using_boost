/// @file
/// @brief

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <multiindex/document.h>


struct DataPack
{
     std::uint16_t number;
     boost::optional< std::string > text;

     struct Tag {
          struct Number {};
          struct Text {};
     };
};


using DataPackIndexContainer = boost::multi_index::multi_index_container<
     DataPack
     , boost::multi_index::indexed_by<
          boost::multi_index::ordered_unique<
               boost::multi_index::tag< DataPack::Tag::Number >
               , boost::multi_index::member< DataPack, std::uint16_t, &DataPack::number >
               >
          , boost::multi_index::ordered_unique<
               boost::multi_index::tag< DataPack::Tag::Text >
               , boost::multi_index::member< DataPack, boost::optional< std::string >, &DataPack::text >
               >
          >
     >;


void usage1()
{
     using using_boost::multiindex::Document;

     std::cout
          << Document::findBy( {} ) << '\n'
          << Document::findBy( {} ) << '\n'
          << Document::findBy( "ОСП" ) << '\n'
          << Document::findBy( "DER" ) << '\n'
          << Document::findBy( "УПРУП" ) << '\n'
          << Document::findBy( "???" ) << '\n'
          << Document::findBy( "ЗПЭД" ) << '\n'
          << Document::findBy( "00UOD" ) << '\n'
          << Document::findBy( "---" ) << '\n'
          << Document::findBy( "DER" ) << '\n'
          << Document::findBy( "SZV-ETD" ) << '\n'
          << Document::findBy( Document::Type::DisconnectingStatement ) << '\n'
          ;
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          using namespace std::string_literals;

          const DataPackIndexContainer cont{
                 {  1,   "One"s }
               , {  2,   "Two"s }
               , {  3, "Three"s }
               , { 10,       {} }
               , { 11,       {} }
               , { 12,       {} }
          };

          {
               const auto& icont = cont.get< DataPack::Tag::Number >();
               const auto found = icont.find( 2 );
               if( found != icont.end() )
               {
                    std::cout << "Value: " << found->number
                         << " <=> " << found->text.value_or( "--" ) << '\n';
               }
               else
               {
                    std::cout << "Not found!\n";
               }
          }
          {
               const auto& icont = cont.get< DataPack::Tag::Text >();
               const auto found = icont.find( "Three"s );
               if( found != icont.end() )
               {
                    std::cout << "Value: " << found->number
                         << " <=> " << found->text.value_or( "--" ) << '\n';
               }
               else
               {
                    std::cout << "Not found!\n";
               }
          }
          {
               const auto& icont = cont.get< DataPack::Tag::Text >();
               const auto found = icont.find( boost::none );
               if( found != icont.end() )
               {
                    std::cout << "Value: " << found->number
                         << " <=> " << found->text.value_or( "--" ) << '\n';
               }
               else
               {
                    std::cout << "Not found!\n";
               }
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
