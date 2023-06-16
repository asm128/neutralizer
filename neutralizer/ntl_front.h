#include "gpk_array.h"
#include "gpk_json.h"

#ifndef NEUTRALIZER_H_2903874928374
#define NEUTRALIZER_H_2903874928374

namespace ntl
{
	struct SNTLArgs {
		::gpk::vcc						Language;
		::gpk::vcc						Module;
		::gpk::vcc						Session;
		::gpk::vcc						Username;
		::gpk::vcc						Width;
		::gpk::vcc						Height;
	};

	::gpk::error_t					loadNTLArgs					(SNTLArgs & out_loaded, const ::gpk::view_array<const ::gpk::TKeyValConstString> & queryStringKeyVals);

	struct SHTMLSiteExtension {
		::gpk::vcc						Program						= {};
		::gpk::vcc						Image						= {};
	};

	struct SHTMLSitePath {
		::gpk::vcc						Program						= {};
		::gpk::vcc						Image						= {};
		::gpk::vcc						Script						= {};
		::gpk::vcc						Style						= {};
	};

	struct SHTMLPage {
		::gpk::vcc						Title						= {};
		::gpk::array_obj<::gpk::vcc>	Scripts						= {};
		::gpk::array_obj<::gpk::vcc>	Styles						= {};
	};

	struct SHTMLEndpoint {
		::gpk::SJSONFile				Config						= {};
		::ntl::SHTMLPage				Page						= {};
		::ntl::SHTMLSitePath			Path						= {};
		::ntl::SHTMLSiteExtension		Extension					= {};
	};

	struct SHTMLIcon {
		::gpk::vcc						Item	;
		::gpk::vcc						Program	;
		::gpk::vcc						Text	;
	};

	::gpk::error_t					htmlTag
		( const ::gpk::view_const_string	& tagName
		, const ::gpk::vcc		& content
		, const ::gpk::vcc		& attributes
		, ::gpk::apod<char>			& output
		);

	::gpk::error_t					htmlVoidTag
		( const ::gpk::view_const_string	& tagName
		, const ::gpk::vcc					& attributes
		, ::gpk::apod<char>			& output
		);

	::gpk::error_t					htmlMenuIcon
		( const ::gpk::vcc		& pathImages
		, const ::gpk::vcc		& extension
		, const ::gpk::vcc		& itemName
		, const ::gpk::vcc		& txtIcon
		, ::gpk::apod<char>			& output
		, bool								iconsLarge					= true
		);

	::gpk::error_t					htmlMenuIconsHorizontal
		( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
		, const ::gpk::vcc						& pathImages
		, const ::gpk::vcc						& extensionImages
		, ::gpk::apod<char>							& output
		, bool								iconsLarge					= true
		);

	::gpk::error_t								htmlControlMenuIcon
		( const ::gpk::vcc		& pathImages
		, const ::gpk::vcc		& extension
		,	const ::ntl::SHTMLIcon			& menuItem
		, ::gpk::apod<char>			& output
		, bool								iconsLarge					= true
		);

	::gpk::error_t								htmlControlMenuIconsHorizontal
		( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
		, const ::gpk::vcc						& pathImages
		, const ::gpk::vcc						& extensionImages
		, ::gpk::apod<char>							& output
		, bool								iconsLarge					= true
		);


	static inline ::gpk::error_t	htmlHeaderTitle		(const ::gpk::vcc	& content	, ::gpk::apod<char> & output)	{ return ::ntl::htmlTag("title", content, {}, output); }
	static inline ::gpk::error_t	htmlHeaderScript	(const ::gpk::vcc	& content	, ::gpk::apod<char> & output)	{ return ::ntl::htmlTag("script", content, {}, output); }
	static inline ::gpk::error_t	htmlHeaderStyle		(const ::gpk::vcc	& content	, ::gpk::apod<char> & output)	{ return ::ntl::htmlTag("style", content, " type=\"text/css\" ", output); }
	static inline ::gpk::error_t	htmlHeaderScriptFile(const ::gpk::vcc	& file		, ::gpk::apod<char> & output)	{
		::gpk::apod<char>					src					= ::gpk::vcs{"type=\"text/javascript\" src=\""};
		src.append(file);
		src.push_back('"');
		return ::ntl::htmlTag("script", {}, {src.begin(), src.size()}, output);
	}

	static inline ::gpk::error_t	htmlHeaderStyleLink	(const ::gpk::vcc	& file		, ::gpk::apod<char> & output)	{
		::gpk::apod<char>					src					= ::gpk::vcs{" rel=\"stylesheet\" type=\"text/css\" href=\""};
		src.append(file);
		src.push_back('"');
		return ::ntl::htmlVoidTag("link", {src.begin(), src.size()}, output);
	}

	static inline ::gpk::error_t				htmlHRefLink		(const ::gpk::vcc	& content
		, const ::gpk::vcc	& url
		, const ::gpk::vcc	& attributes
		, ::gpk::apod<char>			& output
		) {
		::gpk::apod<char>						src					= attributes;
		src.append(::gpk::view_const_string{" href=\""});
		src.append(url);
		src.push_back('"');
		return ::ntl::htmlTag("a", content, {src.begin(), src.size()}, output);
	}

	::gpk::error_t								httpPath			(::gpk::view_const_string folder, ::gpk::view_const_string name, ::gpk::view_const_string extension, ::gpk::apod<char> & output);

	::gpk::error_t								frontConfigLoad		(::ntl::SHTMLEndpoint & programState, int32_t indexRoot);
} // namespace

#endif // NEUTRALIZER_H_2903874928374
