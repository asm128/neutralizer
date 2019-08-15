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
				::gpk::view_const_string			Text	;
	};

	::gpk::error_t								htmlTag
		( const ::gpk::view_const_string	& tagName
		, const ::gpk::view_const_string	& content
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
		, const ::gpk::view_const_string	& itemName
		, const ::gpk::view_const_string	& txtIcon
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


	static inline ::gpk::error_t				htmlHeaderTitle		(const ::gpk::view_const_string	& content, ::gpk::array_pod<char_t> & output) { return ::ntl::htmlTag("title", content, {}, output); }
	static inline ::gpk::error_t				htmlHeaderScript	(const ::gpk::view_const_string	& content, ::gpk::array_pod<char_t> & output) { return ::ntl::htmlTag("script", content, {}, output); }
	static inline ::gpk::error_t				htmlHeaderStyle		(const ::gpk::view_const_string	& content, ::gpk::array_pod<char_t> & output) { return ::ntl::htmlTag("style", content, " type=\"text/css\" ", output); }

	::gpk::error_t								loadConfig			(::ntl::SHTMLEndpoint & programState, int32_t indexRoot);
} // namespace

#endif // NEUTRALIZER_H_2903874928374
