#include "gpk_array.h"

namespace ntl
{
		::gpk::error_t								sessionFileLoad
	( const ::gpk::view_const_string						& cookie
	, ::gpk::array_pod<char_t>								& sessionFileContents
	, ::gpk::array_obj<::gpk::TKeyValConstString>			& cookieValues
	);
} // namespace
