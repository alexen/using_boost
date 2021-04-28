/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <boost/optional/optional.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index_container.hpp>


namespace using_boost {
namespace multiindex {


struct Document
{
     enum class Type
     {
          PersonWorkActivity,
          ConnectingStatement,
          DisconnectingStatement
     };

     boost::string_view text;
     boost::string_view code;
     boost::optional< Type > type;
     boost::string_view description;

     struct Tag {
          struct Text {};
          struct Code {};
          struct Type {};
     };

     static const Document* findByText( boost::string_view text );
     static const Document* findByCode( boost::string_view code );
     static const Document* findByType( Type type );
};


using Documents = boost::multi_index::multi_index_container<
     Document
     , boost::multi_index::indexed_by<
          boost::multi_index::hashed_unique<
               boost::multi_index::tag< Document::Tag::Text >
               , boost::multi_index::member< Document, boost::string_view, &Document::text >
               >
          , boost::multi_index::hashed_non_unique<
               boost::multi_index::tag< Document::Tag::Code >
               , boost::multi_index::member< Document, boost::string_view, &Document::code >
               >
          , boost::multi_index::ordered_non_unique<
               boost::multi_index::tag< Document::Tag::Type >
               , boost::multi_index::member< Document, boost::optional< Document::Type >, &Document::type >
               >
          >
     >;


std::ostream& operator<<( std::ostream&, const Document& );
std::ostream& operator<<( std::ostream&, Document::Type );
std::ostream& operator<<( std::ostream&, const Document* );


} // namespace multiindex
} // namespace using_boost
