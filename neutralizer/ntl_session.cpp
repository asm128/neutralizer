#include "ntl_session.h"
#include "gpk_file.h"

::gpk::error_t			ntl::sessionFileLoad
	( const ::gpk::vcs							& cookie
	, ::gpk::apod<char>							& sessionFileContents
	, ::gpk::aobj<::gpk::TKeyValConstString>	& cookieValues
	) {
	::gpk::aobj<::gpk::vcc>		cookiePairs;
	gpk_necs(::gpk::split(cookie, ';', cookiePairs));
	gpk_necs(cookieValues.resize(cookiePairs.size()));
	for(uint32_t iPair = 0; iPair < cookiePairs.size(); ++iPair) {
		::gpk::vcc					& pair		= cookiePairs[iPair];
		::gpk::trim(pair, pair);
		::gpk::keyval_split(pair, cookieValues[iPair]);
	}
	::gpk::vcs					sessionFileName;
	gpk_necall(::gpk::find(::gpk::vcs{"tumama"}, cookieValues, sessionFileName), "%s", "Invalid cookie. 'tumama' not found.");
	gpk_necall(::gpk::fileToMemory(sessionFileName, sessionFileContents), "Invalid session name: '%s'. Already exists!", sessionFileName.begin());
	return 0;
}
