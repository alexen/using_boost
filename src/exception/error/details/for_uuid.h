/// @file uuid_exception.h
/// @brief
/// @copyright Copyright 2022 InfoTeCS Internet Trust

#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/exception/info_tuple.hpp>

#include <exception/error/exception.h>


namespace alexen {
namespace error {
namespace details {


using ErrorUuid = boost::error_info< struct ErrorUuid_, boost::uuids::uuid >;
using ErrorDescribedUuid = boost::tuple< ErrorDescription, ErrorUuid >;


} // namespace details
} // namespace error
} // namespace alexen
