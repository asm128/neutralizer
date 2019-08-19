#include "gpk_array.h"
#include "gpk_json.h"

#ifndef NEUTRALIZER_H_2903874928374
#define NEUTRALIZER_H_2903874928374

namespace ntl
{
	struct SHTMLSiteExtension {
		::gpk::view_const_string					Program						= {};
		::gpk::view_const_string					Image						= {};
	};

	struct SHTMLSitePath {
		::gpk::view_const_string					Program						= {};
		::gpk::view_const_string					Image						= {};
		::gpk::view_const_string					Script						= {};
		::gpk::view_const_string					Style						= {};
	};

	struct SHTMLPage {
		::gpk::view_const_string					Title						= {};
		::gpk::array_obj<::gpk::view_const_string>	Scripts						= {};
		::gpk::array_obj<::gpk::view_const_string>	Styles						= {};
	};

	struct SHTMLEndpoint {
		::gpk::SJSONFile							Config						= {};
		::ntl::SHTMLPage							Page						= {};
		::ntl::SHTMLSitePath						Path						= {};
		::ntl::SHTMLSiteExtension					Extension					= {};
	};

	struct SHTMLIcon {
				::gpk::view_const_string			Item	;
				::gpk::view_const_string			Program	;
				::gpk::view_const_string			Text	;
	};

	::gpk::error_t								htmlTag
		( const ::gpk::view_const_string	& tagName
		, const ::gpk::view_const_string	& content
		, const ::gpk::view_const_string	& attributes
		, ::gpk::array_pod<char_t>			& output
		);

	::gpk::error_t								htmlVoidTag
		( const ::gpk::view_const_string	& tagName
		, const ::gpk::view_const_string	& attributes
		, ::gpk::array_pod<char_t>			& output
		);

	::gpk::error_t								htmlMenuIcon
		( const ::gpk::view_const_string	& pathImages
		, const ::gpk::view_const_string	& extension
		, const ::gpk::view_const_string	& itemName
		, const ::gpk::view_const_string	& txtIcon
		, ::gpk::array_pod<char_t>			& output
		, bool								iconsLarge					= true
		);

	::gpk::error_t								htmlMenuIconsHorizontal
		( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
		, const ::gpk::view_const_string					& pathImages
		, const ::gpk::view_const_string					& extensionImages
		, ::gpk::array_pod<char_t>							& output
		, bool								iconsLarge					= true
		);

	::gpk::error_t								htmlControlMenuIcon
		( const ::gpk::view_const_string	& pathImages
		, const ::gpk::view_const_string	& extension
		,	const ::ntl::SHTMLIcon			& menuItem
		, ::gpk::array_pod<char_t>			& output
		, bool								iconsLarge					= true
		);

	::gpk::error_t								htmlControlMenuIconsHorizontal
		( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
		, const ::gpk::view_const_string					& pathImages
		, const ::gpk::view_const_string					& extensionImages
		, ::gpk::array_pod<char_t>							& output
		, bool								iconsLarge					= true
		);


	static inline ::gpk::error_t				htmlHeaderTitle		(const ::gpk::view_const_string	& content	, ::gpk::array_pod<char_t> & output) { return ::ntl::htmlTag("title", content, {}, output); }
	static inline ::gpk::error_t				htmlHeaderScript	(const ::gpk::view_const_string	& content	, ::gpk::array_pod<char_t> & output) { return ::ntl::htmlTag("script", content, {}, output); }
	static inline ::gpk::error_t				htmlHeaderStyle		(const ::gpk::view_const_string	& content	, ::gpk::array_pod<char_t> & output) { return ::ntl::htmlTag("style", content, " type=\"text/css\" ", output); }
	static inline ::gpk::error_t				htmlHeaderScriptFile(const ::gpk::view_const_string	& file		, ::gpk::array_pod<char_t> & output) {
		::gpk::array_pod<char_t>						src					= ::gpk::view_const_string{"type=\"text/javascript\" src=\""};
		src.append(file);
		src.push_back('"');
		return ::ntl::htmlTag("script", {}, {src.begin(), src.size()}, output);
	}

	static inline ::gpk::error_t				htmlHeaderStyleLink	(const ::gpk::view_const_string	& file		, ::gpk::array_pod<char_t> & output) {
		::gpk::array_pod<char_t>						src					= ::gpk::view_const_string{" rel=\"stylesheet\" type=\"text/css\" href=\""};
		src.append(file);
		src.push_back('"');
		return ::ntl::htmlVoidTag("link", {src.begin(), src.size()}, output);
	}

	static inline ::gpk::error_t				htmlHRefLink		(const ::gpk::view_const_string	& content
		, const ::gpk::view_const_string	& url
		, const ::gpk::view_const_string	& attributes
		, ::gpk::array_pod<char_t>			& output
		) {
		::gpk::array_pod<char_t>						src					= attributes;
		src.append(::gpk::view_const_string{" href=\""});
		src.append(url);
		src.push_back('"');
		return ::ntl::htmlTag("a", content, {src.begin(), src.size()}, output);
	}

	::gpk::error_t								httpPath			(::gpk::view_const_string folder, ::gpk::view_const_string name, ::gpk::view_const_string extension, ::gpk::array_pod<char_t> & output);

	::gpk::error_t								loadConfig			(::ntl::SHTMLEndpoint & programState, int32_t indexRoot);
} // namespace

#endif // NEUTRALIZER_H_2903874928374
