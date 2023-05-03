/// @file
/// @brief


#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>
#include <boost/utility/identity_type.hpp>


using Size4Types = boost::mpl::list<
     int
     , unsigned
     , float
     , char[ 4 ]
     >;

BOOST_AUTO_TEST_SUITE(
     TemplateTestExamples,
     * boost::unit_test::label( "template" )
     )

BOOST_AUTO_TEST_CASE_TEMPLATE(
     TestTemplateMplList
     , EachT
     , Size4Types
     )
{
     BOOST_TEST( sizeof( EachT ) == 4u );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(
     TestTemplateMplList2
     , EachT
     , BOOST_IDENTITY_TYPE((boost::mpl::list<
          double
          , short[ 4 ]
          , int  [ 2 ]
          , float[ 2 ]
          , char [ 8 ]
     >))
     )
{
     BOOST_TEST( sizeof( EachT ) == 8u );
}

BOOST_AUTO_TEST_SUITE_END()
