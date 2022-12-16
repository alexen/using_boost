/// @file
/// @brief

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/multi_index/composite_key.hpp>

#include <multiindex/document.h>


#define THROW_ASSERT( cond ) \
     do { if( not (cond) ){ \
          BOOST_THROW_EXCEPTION( std::runtime_error{ "condition failed: " #cond } ); } \
     } while( false )


struct TagPrinter
{
     explicit TagPrinter( const char* tag ) : tag_{ tag }
     {
          std::cout << "-----[ BEGIN: " << tag_ << " ]-------------------------\n\n";
     }
     ~TagPrinter()
     {
          std::cout << "\n-----[   END: " << tag_ << " ]-------------------------\n";
     }

     const char* const tag_ = nullptr;
};


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
          boost::multi_index::ordered_unique<
               boost::multi_index::composite_key<
                    Animal
                    , boost::multi_index::member< Animal, std::string, &Animal::name >
                    , boost::multi_index::member< Animal, std::uint16_t, &Animal::legs >
                    >
               >
          , boost::multi_index::hashed_unique<
               boost::multi_index::tag< Animal::Tag::Name >
               , boost::multi_index::member< Animal, std::string, &Animal::name >
               >
          , boost::multi_index::ordered_non_unique<
               boost::multi_index::tag< Animal::Tag::Legs >
               , boost::multi_index::member< Animal, std::uint16_t, &Animal::legs >
               >
          >
     >;

using AnimalCompositeContainer = boost::multi_index::multi_index_container<
     Animal
     , boost::multi_index::indexed_by<
          boost::multi_index::ordered_unique<
               boost::multi_index::composite_key<
                    Animal
                    , boost::multi_index::member< Animal, std::string, &Animal::name >
                    , boost::multi_index::member< Animal, std::uint16_t, &Animal::legs >
                    >
               >
          >
     >;


void usage1()
{
     TagPrinter _{ __FUNCTION__ };

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


void usage2()
{
     TagPrinter _{ __FUNCTION__ };

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
     {
          static constexpr auto name = "Beatle";
          static constexpr auto legs = 6;

          const auto found = animals.find( std::make_tuple( name, legs ) );
          if( found != animals.end() )
          {
               std::cout << "Yes, " << found->name << " has " << found->legs << " legs.\n";
          }
          else
          {
               std::cout << "Not found!\n";
          }
     }
}


void usage3()
{
     TagPrinter _{ __FUNCTION__ };

     const AnimalCompositeContainer animals{
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

     static constexpr auto name = "Beatle";
     static constexpr auto legs = 6;

     const auto found = animals.find( std::make_tuple( name, legs ) );
     if( found != animals.end() )
     {
          std::cout << "Yes, " << found->name << " has " << found->legs << " legs.\n";
     }
     else
     {
          std::cout << "Not found!\n";
     }
}


struct Card
{
     const std::uint32_t uniqueInt;
     const std::string uniqueText;
     const std::string nonUniqueText;
};


std::ostream& operator<<( std::ostream& os, const Card& c )
{
     return os
          << "ui:" << c.uniqueInt
          << ", utext:" << c.uniqueText
          << ", nutext:" << c.nonUniqueText;
}


void usage4()
{
     TagPrinter _{ __FUNCTION__ };

     using WrongCards = boost::multi_index::multi_index_container<
          Card,
          boost::multi_index::indexed_by<
               boost::multi_index::ordered_unique<
                    boost::multi_index::member< Card, const std::uint32_t, &Card::uniqueInt >
                    >,
               boost::multi_index::ordered_unique<
                    boost::multi_index::member< Card, const std::string, &Card::uniqueText >
                    >,
               boost::multi_index::ordered_unique<
                    boost::multi_index::member< Card, const std::string, &Card::nonUniqueText >
                    >
               >
          >;

     using GoodCards = boost::multi_index::multi_index_container<
          Card,
          boost::multi_index::indexed_by<
               boost::multi_index::ordered_unique<
                    boost::multi_index::member< Card, const std::uint32_t, &Card::uniqueInt >
                    >,
               boost::multi_index::ordered_unique<
                    boost::multi_index::member< Card, const std::string, &Card::uniqueText >
                    >,
               boost::multi_index::ordered_non_unique<
                    boost::multi_index::member< Card, const std::string, &Card::nonUniqueText >
                    >
               >
          >;

     try
     {
          const WrongCards cards {
               { 1,   "One", "Number" },
               { 2,   "Two", "Number" },
               { 3, "Three", "Number" }
          };

          THROW_ASSERT( cards.get<0>().find( 1 ) != cards.get<0>().end() );
          THROW_ASSERT( cards.get<0>().find( 2 ) != cards.get<0>().end() );
          THROW_ASSERT( cards.get<0>().find( 3 ) != cards.get<0>().end() );
          THROW_ASSERT( cards.get<0>().find( 0 ) == cards.get<0>().end() );

          std::cout
               << *cards.get<0>().find( 1 ) << '\n'
               << *cards.get<0>().find( 2 ) << '\n'
               << *cards.get<0>().find( 3 ) << '\n'
               ;

          THROW_ASSERT( cards.get<1>().find(   "One" ) != cards.get<1>().end() );
          THROW_ASSERT( cards.get<1>().find(   "Two" ) != cards.get<1>().end() );
          THROW_ASSERT( cards.get<1>().find( "Three" ) != cards.get<1>().end() );
          THROW_ASSERT( cards.get<1>().find( "~~~~~" ) == cards.get<1>().end() );

          std::cout
               << *cards.get<1>().find( "Three" ) << '\n'
               << *cards.get<1>().find(   "Two" ) << '\n'
               << *cards.get<1>().find(   "One" ) << '\n'
               ;
     }
     catch( const std::exception& e )
     {
          std::cout << "exception: " << boost::diagnostic_information( e ) << '\n';
     }
     try
     {
          const GoodCards cards {
               { 1,   "One", "Number" },
               { 2,   "Two", "Number" },
               { 3, "Three", "Number" }
          };

          THROW_ASSERT( cards.get<0>().find( 1 ) != cards.get<0>().end() );
          THROW_ASSERT( cards.get<0>().find( 2 ) != cards.get<0>().end() );
          THROW_ASSERT( cards.get<0>().find( 3 ) != cards.get<0>().end() );
          THROW_ASSERT( cards.get<0>().find( 0 ) == cards.get<0>().end() );

          std::cout
               << *cards.get<0>().find( 1 ) << '\n'
               << *cards.get<0>().find( 2 ) << '\n'
               << *cards.get<0>().find( 3 ) << '\n'
               ;

          THROW_ASSERT( cards.get<1>().find(   "One" ) != cards.get<1>().end() );
          THROW_ASSERT( cards.get<1>().find(   "Two" ) != cards.get<1>().end() );
          THROW_ASSERT( cards.get<1>().find( "Three" ) != cards.get<1>().end() );
          THROW_ASSERT( cards.get<1>().find( "~~~~~" ) == cards.get<1>().end() );

          std::cout
               << *cards.get<1>().find( "Three" ) << '\n'
               << *cards.get<1>().find(   "Two" ) << '\n'
               << *cards.get<1>().find(   "One" ) << '\n'
               ;
     }
     catch( const std::exception& e )
     {
          std::cout << "exception: " << boost::diagnostic_information( e ) << '\n';
     }
}


int main( int argc, char** argv )
{
     using namespace std::string_literals;

     boost::ignore_unused( argc, argv );
     try
     {
          usage4();
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
