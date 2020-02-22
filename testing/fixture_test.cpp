/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>


struct LocalFixture
{
     static int objectsCount()
     {
          return objectsCount_;
     }
     static unsigned ctorCallCount()
     {
          return ctorCallCount_;
     }
     static unsigned dtorCallCount()
     {
          return dtorCallCount_;
     }

     LocalFixture()
     {
          ++objectsCount_;
          ++ctorCallCount_;
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__
               << ": c-tor: objects created: " << objectsCount_
               << ", c-tor called: " << ctorCallCount_
               << ", d-tor called: " << dtorCallCount_
               );
     }
     ~LocalFixture()
     {
          --objectsCount_;
          ++dtorCallCount_;
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__
               << ": d-tor: objects left: " << objectsCount_
               << ", c-tor called: " << ctorCallCount_
               << ", d-tor called: " << dtorCallCount_
               );
     }
private:
     static int objectsCount_;
     static unsigned ctorCallCount_;
     static unsigned dtorCallCount_;
};

int LocalFixture::objectsCount_ = 0;
unsigned LocalFixture::ctorCallCount_ = 0u;
unsigned LocalFixture::dtorCallCount_ = 0u;


struct GlobalFixture
{
     static int objectsCount()
     {
          return objectsCount_;
     }
     static unsigned ctorCallCount()
     {
          return ctorCallCount_;
     }
     static unsigned dtorCallCount()
     {
          return dtorCallCount_;
     }

     GlobalFixture()
     {
          ++objectsCount_;
          ++ctorCallCount_;
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__
               << ": c-tor: objects created: " << objectsCount_
               << ", c-tor called: " << ctorCallCount_
               << ", d-tor called: " << dtorCallCount_
               );
     }
     ~GlobalFixture()
     {
          --objectsCount_;
          ++dtorCallCount_;
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__
               << ": d-tor: objects left: " << objectsCount_
               << ", c-tor called: " << ctorCallCount_
               << ", d-tor called: " << dtorCallCount_
               );
     }
private:
     static int objectsCount_;
     static unsigned ctorCallCount_;
     static unsigned dtorCallCount_;
};

int GlobalFixture::objectsCount_ = 0;
unsigned GlobalFixture::ctorCallCount_ = 0;
unsigned GlobalFixture::dtorCallCount_ = 0;


struct SpecificFixture
{
     static int objectsCount()
     {
          return objectsCount_;
     }
     static unsigned ctorCallCount()
     {
          return ctorCallCount_;
     }
     static unsigned dtorCallCount()
     {
          return dtorCallCount_;
     }

     SpecificFixture()
     {
          ++objectsCount_;
          ++ctorCallCount_;
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__
               << ": c-tor: objects created: " << objectsCount_
               << ", c-tor called: " << ctorCallCount_
               << ", d-tor called: " << dtorCallCount_
               );
     }
     ~SpecificFixture()
     {
          --objectsCount_;
          ++dtorCallCount_;
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__
               << ": d-tor: objects left: " << objectsCount_
               << ", c-tor called: " << ctorCallCount_
               << ", d-tor called: " << dtorCallCount_
               );
     }
private:
     static int objectsCount_;
     static unsigned ctorCallCount_;
     static unsigned dtorCallCount_;
};

int SpecificFixture::objectsCount_ = 0;
unsigned SpecificFixture::ctorCallCount_ = 0;
unsigned SpecificFixture::dtorCallCount_ = 0;


BOOST_TEST_DECORATOR(
     * boost::unit_test::label( "fixture" )
     * boost::unit_test::description( "Test fixtures (local and global) usage examples" )
     )
BOOST_TEST_GLOBAL_FIXTURE( GlobalFixture );
BOOST_AUTO_TEST_SUITE( FixtureTestExamples )
     /// Локальная фикстура создается и разрушается (c-tor/d-tor) для каждого тест-кейса,
     /// поэтому в каждом тесте:
     /// - кол-во объектов всегда будет равно единице;
     /// - кол-во вызовов конструкторов и деструкторов будет увеличиваться.
     BOOST_AUTO_TEST_SUITE( LocalFixtureTest )
          BOOST_FIXTURE_TEST_CASE( TestCaseA1, LocalFixture )
          {
               BOOST_CHECK_EQUAL( 1, objectsCount() );
               BOOST_CHECK_EQUAL( 1, ctorCallCount() );
               BOOST_CHECK_EQUAL( 0, dtorCallCount() );
          }
          BOOST_FIXTURE_TEST_CASE( TestCaseA2, LocalFixture )
          {
               BOOST_CHECK_EQUAL( 1, objectsCount() );
               BOOST_CHECK_EQUAL( 2, ctorCallCount() );
               BOOST_CHECK_EQUAL( 1, dtorCallCount() );
          }
     BOOST_AUTO_TEST_SUITE_END()

     /// Глобальная фикстура создается при старте программы, а затем, для каждого тест кейса
     /// все равно создается и уничтожается отдельная копия оъекта. Поэтому для глобальной
     /// фикстуры счетчик объектов будет равен двойке.
     BOOST_AUTO_TEST_SUITE( GlobalFixtureTest )
          BOOST_FIXTURE_TEST_CASE( TestCaseB1, GlobalFixture )
          {
               BOOST_CHECK_EQUAL( 2, objectsCount() );
               BOOST_CHECK_EQUAL( 2, ctorCallCount() );
               BOOST_CHECK_EQUAL( 0, dtorCallCount() );
          }
          BOOST_FIXTURE_TEST_CASE( TestCaseB2, GlobalFixture )
          {
               BOOST_CHECK_EQUAL( 2, objectsCount() );
               BOOST_CHECK_EQUAL( 3, ctorCallCount() );
               BOOST_CHECK_EQUAL( 1, dtorCallCount() );
          }
          BOOST_FIXTURE_TEST_CASE( TestCaseB3, GlobalFixture )
          {
               BOOST_CHECK_EQUAL( 2, objectsCount() );
               BOOST_CHECK_EQUAL( 4, ctorCallCount() );
               BOOST_CHECK_EQUAL( 2, dtorCallCount() );
          }
     BOOST_AUTO_TEST_SUITE_END()

     /// Для "внешней" фикстуры констрктор и деструктор (как и setup() и teardown()) вызываются
     /// только один раз - конструктор перед первым тестом, деструктор - после последнего.
     BOOST_AUTO_TEST_SUITE(
          LocalFixtureTestWithCommonInitialization,
          * boost::unit_test::fixture< SpecificFixture >()
          )
          BOOST_AUTO_TEST_CASE( TestCaseC1 )
          {
               BOOST_CHECK_EQUAL( 1, SpecificFixture::objectsCount() );
               BOOST_CHECK_EQUAL( 1, SpecificFixture::ctorCallCount() );
               BOOST_CHECK_EQUAL( 0, SpecificFixture::dtorCallCount() );
          }
          BOOST_AUTO_TEST_CASE( TestCaseC2 )
          {
               BOOST_CHECK_EQUAL( 1, SpecificFixture::objectsCount() );
               BOOST_CHECK_EQUAL( 1, SpecificFixture::ctorCallCount() );
               BOOST_CHECK_EQUAL( 0, SpecificFixture::dtorCallCount() );
          }
          BOOST_AUTO_TEST_CASE( TestCaseC3 )
          {
               BOOST_CHECK_EQUAL( 1, SpecificFixture::objectsCount() );
               BOOST_CHECK_EQUAL( 1, SpecificFixture::ctorCallCount() );
               BOOST_CHECK_EQUAL( 0, SpecificFixture::dtorCallCount() );
          }
     BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
