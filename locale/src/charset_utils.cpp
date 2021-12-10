/// @file
/// @brief


#include <locale/charset_utils.h>

#include <boost/locale/encoding.hpp>


namespace edi {


namespace {
namespace consts {


static constexpr auto cp1251 = "cp1251";
static constexpr auto cp866 = "cp866";


} // namespace consts
} // namespace {unnamed}



std::wstring utf8ToUtf16( const std::string& utf8 )
{
     return boost::locale::conv::utf_to_utf< wchar_t >( utf8 );
}


std::string utf16ToUtf8( const std::wstring& utf16 )
{
     return boost::locale::conv::utf_to_utf< char >( utf16 );
}


std::string utf8ToCp1251( const std::string& utf8 )
{
     return boost::locale::conv::from_utf( utf8, consts::cp1251 );
}


std::string cp1251ToUtf8( const std::string& cp1251 )
{
     return boost::locale::conv::to_utf< char >( cp1251, consts::cp1251 );
}


std::string utf8ToOem( const std::string& utf8 )
{
     return boost::locale::conv::from_utf( utf8, consts::cp866 );
}


std::string oemToUtf8( const std::string& oem )
{
     return boost::locale::conv::to_utf< char >( oem, consts::cp866 );
}


} // namespace edi
