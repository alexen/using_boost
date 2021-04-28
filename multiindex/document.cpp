/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <multiindex/document.h>


namespace using_boost {
namespace multiindex {


namespace {
namespace aux {


template< typename IndexedSequence, typename Key >
static const Document* findByKey( const IndexedSequence& iseq, const Key key )
{
     const auto found = iseq.find( key );
     if( found != std::cend( iseq ) )
     {
          return boost::addressof( *found );
     }
     return nullptr;
}


} // namespace aux
namespace consts {

static Documents documents {
     { "СЗВ-ТД",  "SZV-ETD",     Document::Type::PersonWorkActivity,     "Сведения о трудовой деятельности зарегистрированного лица" }
   , { "ЗПЭД",    "0ZPED",       Document::Type::ConnectingStatement,    "Заявление на подключение страхователя к электронному документообороту ПФР" }
   , { "ЗОЭД",    "0ZOED",       Document::Type::DisconnectingStatement, "Заявление на отключение страхователя от электронного документооборота ПФР" }
   , { "УПУП",    "0UPUP",       {},                                     "Уведомление о предоставлении полномочий представителю" }
   , { "УПРУП",   "UPRUP",       {},                                     "Уведомление о прекращении полномочий представителя" }
   , { "УОРР",    "0UORR",       {},                                     "Уведомление о результате рассмотрения" }
   , { "УОД",     "00UOD",       {},                                     "Уведомление о доставке" }
   , { "УОПП",    "0UOPP",       {},                                     "Уведомление об отказе в приеме пакета" }
   , { "УУОН-ПУ", "NTFC_TO_INS", {},                                     "Уведомление об устранении ошибок и (или) несоответствий между представленными страхователем сведениями и сведениями, имеющимися у ПФР" }
   , { "УППО",    "UPP",         {},                                     "Общий протокол проверок" }
   , { "ОСП",     {},            {},                                     "Опись содержания пакета ЭДО со страхователями" }
   , { "DER",     {},            {},                                     {} }
};


} // namespace consts
} // namespace {unnamed}


const Document* Document::findByText( boost::string_view text )
{
     return aux::findByKey( consts::documents.get< Document::Tag::Text >(), text );
}


const Document* Document::findByCode( boost::string_view code )
{
     return aux::findByKey( consts::documents.get< Document::Tag::Code >(), code );
}


const Document* Document::findByType( const Document::Type type )
{
     return aux::findByKey( consts::documents.get< Document::Tag::Type >(), type );
}


std::ostream& operator<<( std::ostream& os, const Document::Type type )
{
     switch( type )
     {
          case Document::Type::PersonWorkActivity:
               os << "[PersonWorkActivity]";
               break;
          case Document::Type::ConnectingStatement:
               os << "[ConnectingStatement]";
               break;
          case Document::Type::DisconnectingStatement:
               os << "[DisconnectingStatement]";
               break;
     }
     return os;
}


static std::ostream& operator<<( std::ostream& os, const boost::optional< Document::Type >& type )
{
     if( type )
     {
          os << *type;
     }
     else
     {
          os << "[x]";
     }
     return os;
}


static std::ostream& operator<<( std::ostream& os, boost::string_view sv )
{
     if( sv.empty() )
     {
          os << "~x~";
     }
     else
     {
          os.write( sv.data(), sv.size() );
     }
     return os;
}


std::ostream& operator<<( std::ostream& os, const Document& doc )
{
     return os << doc.text << " <=> " << doc.code << ' ' << doc.type << " (" << doc.description << ')';
}


std::ostream& operator<<( std::ostream& os, const Document* ptr )
{
     static constexpr boost::string_view nodata = "{nullptr}";
     if( ptr )
     {
          os << *ptr;
     }
     else
     {
          os << nodata;
     }
     return os;
}


} // namespace multiindex
} // namespace using_boost
