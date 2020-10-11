#include "gpk_array.h"
#include "gpk_json.h"

#ifndef NEUTRALIZER_H_2903874928374
#define NEUTRALIZER_H_2903874928374

namespace ntl
{
	struct SNTLArgs {
		::gpk::view_const_char						Language;
		::gpk::view_const_char						Module;
		::gpk::view_const_char						Session;
		::gpk::view_const_char						Username;
		::gpk::view_const_char						Width;
		::gpk::view_const_char						Height;
	};

	::gpk::error_t								loadNTLArgs					(SNTLArgs & out_loaded, const ::gpk::view_array<const ::gpk::TKeyValConstString> & queryStringKeyVals);

	struct SHTMLSiteExtension {
		::gpk::view_const_char						Program						= {};
		::gpk::view_const_char						Image						= {};
	};

	struct SHTMLSitePath {
		::gpk::view_const_char						Program						= {};
		::gpk::view_const_char						Image						= {};
		::gpk::view_const_char						Script						= {};
		::gpk::view_const_char						Style						= {};
	};

	struct SHTMLPage {
		::gpk::view_const_char						Title						= {};
		::gpk::array_obj<::gpk::view_const_char>	Scripts						= {};
		::gpk::array_obj<::gpk::view_const_char>	Styles						= {};
	};

	struct SHTMLEndpoint {
		::gpk::SJSONFile							Config						= {};
		::ntl::SHTMLPage							Page						= {};
		::ntl::SHTMLSitePath						Path						= {};
		::ntl::SHTMLSiteExtension					Extension					= {};
	};

	struct SHTMLIcon {
		::gpk::view_const_char						Item	;
		::gpk::view_const_char						Program	;
		::gpk::view_const_char						Text	;
	};

	::gpk::error_t								htmlTag
		( const ::gpk::view_const_string	& tagName
		, const ::gpk::view_const_char		& content
		, const ::gpk::view_const_char		& attributes
		, ::gpk::array_pod<char_t>			& output
		);

	::gpk::error_t								htmlVoidTag
		( const ::gpk::view_const_string	& tagName
		, const ::gpk::view_const_char		& attributes
		, ::gpk::array_pod<char_t>			& output
		);

	::gpk::error_t								htmlMenuIcon
		( const ::gpk::view_const_char		& pathImages
		, const ::gpk::view_const_char		& extension
		, const ::gpk::view_const_char		& itemName
		, const ::gpk::view_const_char		& txtIcon
		, ::gpk::array_pod<char_t>			& output
		, bool								iconsLarge					= true
		);

	::gpk::error_t								htmlMenuIconsHorizontal
		( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
		, const ::gpk::view_const_char						& pathImages
		, const ::gpk::view_const_char						& extensionImages
		, ::gpk::array_pod<char_t>							& output
		, bool								iconsLarge					= true
		);

	::gpk::error_t								htmlControlMenuIcon
		( const ::gpk::view_const_char		& pathImages
		, const ::gpk::view_const_char		& extension
		,	const ::ntl::SHTMLIcon			& menuItem
		, ::gpk::array_pod<char_t>			& output
		, bool								iconsLarge					= true
		);

	::gpk::error_t								htmlControlMenuIconsHorizontal
		( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
		, const ::gpk::view_const_char						& pathImages
		, const ::gpk::view_const_char						& extensionImages
		, ::gpk::array_pod<char_t>							& output
		, bool								iconsLarge					= true
		);


	static inline ::gpk::error_t				htmlHeaderTitle		(const ::gpk::view_const_char	& content	, ::gpk::array_pod<char_t> & output)	{ return ::ntl::htmlTag("title", content, {}, output); }
	static inline ::gpk::error_t				htmlHeaderScript	(const ::gpk::view_const_char	& content	, ::gpk::array_pod<char_t> & output)	{ return ::ntl::htmlTag("script", content, {}, output); }
	static inline ::gpk::error_t				htmlHeaderStyle		(const ::gpk::view_const_char	& content	, ::gpk::array_pod<char_t> & output)	{ return ::ntl::htmlTag("style", content, " type=\"text/css\" ", output); }
	static inline ::gpk::error_t				htmlHeaderScriptFile(const ::gpk::view_const_char	& file		, ::gpk::array_pod<char_t> & output)	{
		::gpk::array_pod<char_t>						src					= ::gpk::view_const_string{"type=\"text/javascript\" src=\""};
		src.append(file);
		src.push_back('"');
		return ::ntl::htmlTag("script", {}, {src.begin(), src.size()}, output);
	}

	static inline ::gpk::error_t				htmlHeaderStyleLink	(const ::gpk::view_const_char	& file		, ::gpk::array_pod<char_t> & output)	{
		::gpk::array_pod<char_t>						src					= ::gpk::view_const_string{" rel=\"stylesheet\" type=\"text/css\" href=\""};
		src.append(file);
		src.push_back('"');
		return ::ntl::htmlVoidTag("link", {src.begin(), src.size()}, output);
	}

	static inline ::gpk::error_t				htmlHRefLink		(const ::gpk::view_const_char	& content
		, const ::gpk::view_const_char	& url
		, const ::gpk::view_const_char	& attributes
		, ::gpk::array_pod<char_t>			& output
		) {
		::gpk::array_pod<char_t>						src					= attributes;
		src.append(::gpk::view_const_string{" href=\""});
		src.append(url);
		src.push_back('"');
		return ::ntl::htmlTag("a", content, {src.begin(), src.size()}, output);
	}

	::gpk::error_t								httpPath			(::gpk::view_const_string folder, ::gpk::view_const_string name, ::gpk::view_const_string extension, ::gpk::array_pod<char_t> & output);

	::gpk::error_t								frontConfigLoad		(::ntl::SHTMLEndpoint & programState, int32_t indexRoot);
} // namespace

#endif // NEUTRALIZER_H_2903874928374
