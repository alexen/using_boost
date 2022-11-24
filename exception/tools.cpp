/// @file tools.cpp
/// @brief
/// @copyright Copyright 2022 InfoTeCS Internet Trust

#include "tools.h"

#include <ostream>
#include <boost/throw_exception.hpp>

#include "user_exception.h"


namespace alexen {
namespace tools {


std::ostream& operator<<( std::ostream& os, const Enum e )
{
     const auto code = static_cast< std::int64_t >( e );
     switch( e )
     {
          case Enum::Field1:
               os << "FIELD#1 (code: " << code << ')';
               break;
          case Enum::Field2:
               os << "FIELD#2 (code: " << code << ')';
               break;
          case Enum::Field3:
               os << "FIELD#3 (code: " << code << ')';
               break;
          default:
               os.setstate( std::ios_base::failbit );
     }
     return os;
}



void doSmth( const Enum e )
{
     BOOST_THROW_EXCEPTION( error::Exception{}
          << error::details::ErrorDescription{ "unbelievable situation" }
          << error::details::ErrorEnum{ e }
          );
}


} // namespace tools
} // namespace alexen
