/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/type_index.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/core/addressof.hpp>
#include <boost/core/is_same.hpp>
#include <boost/core/exchange.hpp>
#include <boost/core/null_deleter.hpp>

#include <gtest/gtest.h>


TEST( BoostCore, AddressOf )
{
     class AddressOpOverloaded {
     public:
          void operator&() const
          {
               throw std::runtime_error{ "oops!" };
          }
     };

     AddressOpOverloaded var;

     ASSERT_THROW( &var, std::runtime_error );
     ASSERT_NO_THROW( boost::addressof( var ) );

     const auto ptr = static_cast< AddressOpOverloaded* >( nullptr );
     const auto addr = boost::addressof( var );

     constexpr auto variablesHaveSameTypes = boost::core::is_same< decltype( ptr ), decltype( addr ) >::value;

     ASSERT_TRUE( variablesHaveSameTypes )
          << "Types mismatch:\n"
          << "  " << boost::typeindex::type_id_with_cvr< decltype( ptr ) >().pretty_name() << '\n'
          << "  " << boost::typeindex::type_id_with_cvr< decltype( addr ) >().pretty_name() << '\n';
}


TEST( BoostCore, Exchange )
{
     int x = 10;
     int y = -5;

     ASSERT_EQ( x, 10 );
     ASSERT_EQ( y, -5 );

     y = boost::exchange( x, 15 );

     ASSERT_EQ( x, 15 );
     ASSERT_EQ( y, 10 );
}


TEST( BoostCore, NullDeleter )
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
          ASSERT_FALSE( cleared );
          {
               boost::shared_ptr< Destructable >{ new Destructable{ cleared } };
          }
          ASSERT_TRUE( cleared );
     }

     /// "cleared" must be left unchangeable
     {
          bool cleared = false;
          Destructable var{ cleared };
          ASSERT_FALSE( cleared );
          {
               boost::shared_ptr< Destructable >( &var, boost::null_deleter{} );
          }
          ASSERT_FALSE( cleared );
     }
}
