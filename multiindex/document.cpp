/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <multiindex/document.h>


namespace using_boost {
namespace multiindex {


namespace {
namespace aux {


template< typename IndexedSequence >
static const Document* findByKey( const IndexedSequence& iseq, boost::string_view key )
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

static constexpr boost::string_view _;
static Documents documents {
     { "СЗВ-ТД",  "SZV-ETD",     "Сведения о трудовой деятельности зарегистрированного лица" }
   , { "ЗПЭД",    "0ZPED",       "Заявление на подключение страхователя к электронному документообороту ПФР" }
   , { "ЗОЭД",    "0ZOED",       "Заявление на отключение страхователя от электронного документооборота ПФР" }
   , { "УПУП",    "0UPUP",       "Уведомление о предоставлении полномочий представителю" }
   , { "УПРУП",   "UPRUP",       "Уведомление о прекращении полномочий представителя" }
   , { "УОРР",    "0UORR",       "Уведомление о результате рассмотрения" }
   , { "УОД",     "00UOD",       "Уведомление о доставке" }
   , { "УОПП",    "0UOPP",       "Уведомление об отказе в приеме пакета" }
   , { "УУОН-ПУ", "NTFC_TO_INS", "Уведомление об устранении ошибок и (или) несоответствий между представленными страхователем сведениями и сведениями, имеющимися у ПФР" }
   , { "УППО",    "UPP",         "Общий протокол проверок" }
   , { "ОСП",     _,             "Опись содержания пакета ЭДО со страхователями" }
   , { "DER",     _,             _ }
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


std::ostream& operator<<( std::ostream& os, const Document& doc )
{
     return os << doc.text << " <=> " << doc.code << " (" << doc.description << ')';
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
