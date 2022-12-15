/// @file
/// @brief

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <multiindex/document.h>


struct Animal
{
     std::string name;
     std::uint16_t legs;

     struct Tag {
          struct Name {};
          struct Legs {};
     };
};


using AnimalIndexedContainer = boost::multi_index::multi_index_container<
     Animal
     , boost::multi_index::indexed_by<
          boost::multi_index::hashed_unique<
               boost::multi_index::tag< Animal::Tag::Name >
               , boost::multi_index::member< Animal, std::string, &Animal::name >
               >
          , boost::multi_index::ordered_non_unique<
               boost::multi_index::tag< Animal::Tag::Legs >
               , boost::multi_index::member< Animal, std::uint16_t, &Animal::legs >
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

          const AnimalIndexedContainer animals{
               {   "Ostrich",  2 }
             , {    "Spider",  8 }
             , {      "Deer",  4 }
             , {       "Bee",  6 }
             , {  "Kangaroo",  2 }
             , {    "Monkey",  2 }
             , {    "Beatle",  6 }
             , {     "Snake",  0 }
             , {       "Dog",  4 }
             , { "Centipede", 40 }
             , {       "Cat",  4 }
             , {      "Worm",  0 }
          };

          {
               const auto& icont = animals.get< Animal::Tag::Legs >();
               const auto found = icont.find( 2 );
               if( found != icont.end() )
               {
                    std::cout << "Yes, " << found->name << " has " << found->legs << " legs.\n";
               }
               else
               {
                    std::cout << "Not found!\n";
               }
          }
          {
               const auto& icont = animals.get< Animal::Tag::Name >();
               const auto found = icont.find( "Cat" );
               if( found != icont.end() )
               {
                    std::cout << "Yes, " << found->name << " has " << found->legs << " legs.\n";
               }
               else
               {
                    std::cout << "Not found!\n";
               }
          }
          {
               static constexpr auto from = 4;
               static constexpr auto upto = 8;

               const auto& icont = animals.get< Animal::Tag::Legs >();
               std::cout << "What animals has from " << from << " up to " << upto << " legs?\n";
               std::for_each(
                    icont.lower_bound( from )
                    , icont.upper_bound( upto )
                    , []( const auto& each )
                    {
                         std::cout << "- " << each.name << " has " << each.legs << " legs\n";
                    }
                    );
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
