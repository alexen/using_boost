/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>


struct UserDefinedType
{
     UserDefinedType( int id ) : id_{ id } {}

     int id() const
     {
          return id_;
     }
private:
     int id_;
};


bool operator==( const UserDefinedType& lhs, const UserDefinedType& rhs )
{
     return lhs.id() == rhs.id();
}


std::ostream& boost_test_print_type( std::ostream& os, const UserDefinedType& udt )
{
     os << "{user defined type (" << udt.id() << ")}";
     return os;
}


BOOST_AUTO_TEST_CASE( CustomTypesOutputTest,
     * boost::unit_test::label( "output" )
     )
{
     UserDefinedType a{ 7 }, b{ 3 };

     BOOST_TEST( a == b );
}
