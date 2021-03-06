#include "ntl_session.h"
#include "gpk_storage.h"

::gpk::error_t										ntl::sessionFileLoad
	( const ::gpk::view_const_string					& cookie
	, ::gpk::array_pod<char_t>							& sessionFileContents
	, ::gpk::array_obj<::gpk::TKeyValConstString>		& cookieValues
	) {
	::gpk::array_obj<::gpk::view_const_char>				cookiePairs;
	gpk_necall(::gpk::split(cookie, ';', cookiePairs), "%s", "Out of memory?");
	gpk_necall(cookieValues.resize(cookiePairs.size()), "%s", "Out of memory?");
	for(uint32_t iPair = 0; iPair < cookiePairs.size(); ++iPair) {
		::gpk::view_const_char								& pair					= cookiePairs[iPair];
		::gpk::trim(pair, pair);
		::gpk::keyval_split(pair, cookieValues[iPair]);
	}
	::gpk::view_const_string								sessionFileName;
	gpk_necall(::gpk::find(::gpk::vcs{"tumama"}, cookieValues, sessionFileName), "%s", "Invalid cookie. 'tumama' not found.");
	gpk_necall(::gpk::fileToMemory(sessionFileName, sessionFileContents), "Invalid session name: '%s'. Already exists!", sessionFileName.begin());
	return 0;
}
