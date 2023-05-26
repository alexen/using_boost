/// @file
/// @brief


#include <ostream>
#include <vector>
#include <tuple>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>


template< typename T >
auto makeSequence( std::initializer_list< T >&& il )
{
     return std::vector< T, std::allocator< T > >{ il.begin(), il.end() };
}


namespace std {
template< typename T >
ostream& boost_test_print_type( ostream& os, const vector< T, allocator< T > >& v )
{
     const char* sep = "";
     os << '{';
     for( auto&& el: v )
     {
          os << sep << el;
          sep = ",";
     }
     os << '}';
     return os;
}
} // namespace std


struct InfiniteSequence
{
     enum { arity = 1 };

     struct iterator
     {
          iterator() {}

          int operator*() const { return current_; }
          void operator++() { ++current_; }
     private:
          unsigned current_ = 0u;
     };

     InfiniteSequence() {}

     boost::unit_test::data::size_t size() const
     {
          return boost::unit_test::data::BOOST_TEST_DS_INFINITE_SIZE;
     }

     iterator begin() const
     {
          return iterator{};
     }
};


namespace boost {
namespace unit_test {
namespace data {
namespace monomorphic {

template<> struct is_dataset< InfiniteSequence > : mpl::true_ {};

} // namespace monomorphic
} // namespace data
} // namespace unit_test
} // namespace boost


BOOST_AUTO_TEST_SUITE(
     DatasetTestExamples,
     * boost::unit_test::label( "dataset" )
     * boost::unit_test::description( "Tests with datasets for example" )
     )
BOOST_DATA_TEST_CASE(
     TestInfiniteSequence
     , InfiniteSequence() ^ boost::unit_test::data::make({ 0,1,2,3,4,5,6,7,8,9,10,11,12 })
     , generatedValue
     , expectedValue
     )
{
     BOOST_TEST( expectedValue == generatedValue );
}


static constexpr int INTEGERS[] = {
     0,1,2,3,4,5,6,7,8,9
};


BOOST_DATA_TEST_CASE(
     TestInfiniteSequenceUsingArray
     , InfiniteSequence() ^ boost::unit_test::data::make( INTEGERS )
     , generatedValue
     , expectedValue
     )
{
     BOOST_TEST( expectedValue == generatedValue );
}


static constexpr int XRANGE_5 = 5;


BOOST_DATA_TEST_CASE(
     TestUsingXrange
     , boost::unit_test::data::xrange( XRANGE_5 )
     , value
     )
{
     BOOST_TEST( ((0 <= value) && (value < XRANGE_5)) );
}


static constexpr int XRANGE_3 = 3;
static const char* const CSTRS_3[] = { "zero", "one", "two" };


BOOST_DATA_TEST_CASE(
     TestUsingZip
     , boost::unit_test::data::xrange( XRANGE_3 ) ^ boost::unit_test::data::make( CSTRS_3 )
     , index
     , text
     )
{
     BOOST_TEST( (0 <= index && index < XRANGE_3 ) );
     BOOST_TEST( std::string{ CSTRS_3[ index ] } == text );
}


BOOST_DATA_TEST_CASE(
     TestUsingJoin
     , boost::unit_test::data::xrange( XRANGE_3 ) + boost::unit_test::data::make({ 66, 55, 44, 99, 12 })
     , value
     )
{
     BOOST_TEST((
          value == 0
          || value == 1
          || value == 2
          || value == 66
          || value == 55
          || value == 44
          || value == 99
          || value == 12
          ));
}


static const char* const CSTRS_4[] = { "cake", "owl", "radio", "top" };


BOOST_DATA_TEST_CASE(
     TestUsingCartesianProduct
     , boost::unit_test::data::xrange( 3 ) * boost::unit_test::data::make( CSTRS_4 )
     , first
     , second
     )
{
     static const auto isPresent =
          []( const std::string& str )
          {
               static const auto end = CSTRS_4 + 4;
               return end != std::find( CSTRS_4, end, str );
          };

     switch( first )
     {
          case 0:
               BOOST_TEST( isPresent( second ) );
               break;
          case 1:
               BOOST_TEST( isPresent( second ) );
               break;
          case 2:
               BOOST_TEST( isPresent( second ) );
               break;
          default:
               BOOST_TEST( false );
     }
}


BOOST_DATA_TEST_CASE(
     TestDatasetSingleton
     , boost::unit_test::data::make( 2 )
     , singleton
     )
{
     BOOST_TEST( (singleton == 2) );
}


BOOST_DATA_TEST_CASE(
     TestDatasetSingletonZip
     , boost::unit_test::data::xrange( 3 ) ^ boost::unit_test::data::make( 2 )
     , value
     , singleton
     )
{
     BOOST_TEST( ((0 <= value && value < 3) && (singleton == 2)) );
}


BOOST_DATA_TEST_CASE(
     TestDatasetOneToManyMapping
     , boost::unit_test::data::xrange( 3 )
          ^ boost::unit_test::data::make({
               makeSequence({ 1,2,3 })
               , makeSequence({ 9,7,3,5 })
               , makeSequence({ 4,0 })
          })
     , index
     , value
     )
{
     boost::ignore_unused( index, value );
     BOOST_TEST( true );
}


BOOST_DATA_TEST_CASE(
     TestDatasetRandomReal
     , boost::unit_test::data::xrange( 3 ) ^ boost::unit_test::data::random()
     , index
     , random
     )
{
     BOOST_TEST( (0 <= index && index < 3) );
     BOOST_TEST( (random != 0.538256) ); /// I feel lucky!
}


BOOST_DATA_TEST_CASE(
     TestDatasetRandomDiceRoll
     , boost::unit_test::data::xrange( 5 ) ^ boost::unit_test::data::random( 1, 6 )
     , step
     , dice
     )
{
     BOOST_TEST( (0 <= step && step < 5) );
     BOOST_TEST( (1 <= dice && dice <= 6) );
}

struct Point {
     int x, y, z;
};
std::ostream& boost_test_print_type( std::ostream& os, const Point& ) { return os; }


BOOST_DATA_TEST_CASE(
     TestDatasetStructSequence
     , boost::unit_test::data::make< Point >({
          { 1, 2, 3 },
          { 1, 2, 3 },
          { 1, 2, 3 }
     })
     , input
)
{
     boost::ignore_unused( input );
}

using Tuple = std::tuple< int, int, int >;
std::ostream& boost_test_print_type( std::ostream& os, const Tuple& ) { return os; }

BOOST_DATA_TEST_CASE(
     TestDatasetTupleSequence
     , boost::unit_test::data::make< Tuple >({
          { 1, 2, 3 },
          { 4, 5, 6 },
          { 7, 8, 9 }
     })
     ^ boost::unit_test::data::make({
          "First",
          "Second",
          "Third"
     })
     , a, b, c, d
)
{
     boost::ignore_unused( a, b, c, d );
}

BOOST_AUTO_TEST_SUITE_END()
