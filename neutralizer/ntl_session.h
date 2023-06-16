#include "gpk_array.h"

namespace ntl
{
	::gpk::error_t			sessionFileLoad
		( const ::gpk::vcs							& cookie
		, ::gpk::apod<char>							& sessionFileContents
		, ::gpk::aobj<::gpk::TKeyValConstString>	& cookieValues
		);
} // namespace
