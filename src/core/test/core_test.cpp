/// @file
/// @brief


#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/core/addressof.hpp>
#include <boost/core/is_same.hpp>
#include <boost/core/exchange.hpp>
#include <boost/core/null_deleter.hpp>


BOOST_AUTO_TEST_SUITE( TestBoostCore )
BOOST_AUTO_TEST_CASE( TestAddressOf )
{
     class AddressOpOverloaded {
     public:
          void operator&() const
          {
               throw std::runtime_error{ "oops!" };
          }
     };

     AddressOpOverloaded var;

     BOOST_CHECK_THROW( &var, std::runtime_error );
     BOOST_CHECK_NO_THROW( boost::addressof( var ) );

     const auto ptr = static_cast< AddressOpOverloaded* >( nullptr );
     const auto addr = boost::addressof( var );

     constexpr auto variablesHaveSameTypes = boost::core::is_same< decltype( ptr ), decltype( addr ) >::value;

     BOOST_TEST( variablesHaveSameTypes );
}


BOOST_AUTO_TEST_CASE( TestExchange )
{
     int x = 10;
     int y = -5;

     BOOST_TEST( x == 10 );
     BOOST_TEST( y == -5 );

     y = boost::exchange( x, 15 );

     BOOST_TEST( x == 15 );
     BOOST_TEST( y == 10 );
}


BOOST_AUTO_TEST_CASE( TestNullDeleter )
{
     class Destructable {
     public:
          Destructable( bool& f ) : ref_( f ) {}
          ~Destructable() { ref_ = true; }
     private:
          bool& ref_;
     };

     /// "cleared" must be true after all
     {
          bool cleared = false;
          BOOST_TEST( not cleared );
          {
               boost::shared_ptr< Destructable >{ new Destructable{ cleared } };
          }
          BOOST_TEST( cleared );
     }

     /// "cleared" must be left unchangeable
     {
          bool cleared = false;
          Destructable var{ cleared };
          BOOST_TEST( not cleared );
          {
               boost::shared_ptr< Destructable >( &var, boost::null_deleter{} );
          }
          BOOST_TEST( not cleared );
     }
}
BOOST_AUTO_TEST_SUITE_END()
