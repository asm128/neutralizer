#include "neutralizer_ad_category.h"

#include "neutralizer.h"

#include "gpk_json_expression.h"

::gpk::error_t								ntl::pageCatalog					(const ::gpk::view_const_string & contentFileName, const ::gpk::view_const_string & pathStyles, const AD_SHOP_CATEGORY category, const ::gpk::view_const_string & title, const ::gpk::view_const_string & lang, ::gpk::array_pod<char_t> & output) {

	output.append(::gpk::view_const_string{ "\n<html>"});
	output.append(::gpk::view_const_string{ "\n<head>"});
	output.append(::gpk::view_const_string{ "\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />"});

	::gpk::array_pod<char_t>								fileStyle			;
	::ntl::httpPath(pathStyles, "blankstyle", "css"	, fileStyle);
	::ntl::htmlHeaderTitle		(title, output);
	::ntl::htmlHeaderStyleLink	({fileStyle.begin(), fileStyle.size()}, output);
	output.append(::gpk::view_const_string{"\n</head>"});

	output.append(::gpk::view_const_string{"\n<body style=\"width:100%;height:100%;background-color:#E0E0E0;font-family:Arial;\">"	}); // 202050
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"	});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"font-size:16px; font-weight:bold; vertical-align:top;\">"});

	::ntl::htmlBoardGenerate(contentFileName, category, title, lang, output);

	output.append(::gpk::view_const_string{"\n</td>"			});
	output.append(::gpk::view_const_string{"\n</tr>"			});
	output.append(::gpk::view_const_string{"\n</table>"			});
	output.append(::gpk::view_const_string{"\n</body>"			});
	output.append(::gpk::view_const_string{"\n</html>"			});
	return 0;
}


::gpk::error_t								ntl::htmlBoardGenerate				(const ::gpk::view_const_string & contentFileName, const ::ntl::AD_SHOP_CATEGORY category, const ::gpk::view_const_string & title, const ::gpk::view_const_string & lang, ::gpk::array_pod<char_t> & output)	{
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"});

	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">"});
	output.append(::gpk::view_const_string{ "\n<h1>"});
	output.append(title);
	output.append(::gpk::view_const_string{ "</h1>"});
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">"});


	//---------------------
	::gpk::SJSONFile										config							= {};
	gpk_necall(::gpk::jsonFileRead(config, contentFileName), "Failed to load configuration file: %s.", contentFileName);
	const ::gpk::error_t									countItems						= ::gpk::jsonArraySize(*config.Reader[0]);
	for(int32_t iItem = 0; iItem < countItems; ++iItem) {
		const ::gpk::error_t									jsonIndexCurrentItem			= ::gpk::jsonArrayValueGet(*config.Reader[0], iItem);
		::gpk::view_const_string								viewWikiCategory				= {};
		const ::gpk::error_t									jsonIndexArrayCategory			= ::gpk::jsonExpressionResolve("category"	, config.Reader, jsonIndexCurrentItem, viewWikiCategory);
		bool													isCategory						= false;
		for(uint32_t iCat = 0, countCat = (uint32_t)::gpk::jsonArraySize(*config.Reader[jsonIndexArrayCategory]); iCat < countCat; ++iCat) {
			const ::gpk::error_t									jsonIndexNodeCategory			= ::gpk::jsonArrayValueGet(*config.Reader[jsonIndexArrayCategory], iCat);
			const ::gpk::SJSONToken									& jsonToken						= config.Reader.Token[jsonIndexNodeCategory];
			if(category == jsonToken.Value) {
				isCategory											= true;
				break;
			}
		}
		if(false == isCategory)
			continue;

		::gpk::view_const_string								viewWikiURL						= {};
		::gpk::view_const_string								viewWikiTitle					= {};
		::gpk::view_const_string								viewWikiText					= {};
		::gpk::view_const_string								viewWikiImageHRef				= {};
		::gpk::view_const_string								viewWikiImageSrc				= {};
		::gpk::view_const_string								viewWikiImageTitle				= {};
		::gpk::view_const_string								viewWikiImageAlt				= {};
		::gpk::view_const_string								viewMapURL						= {};
		const ::gpk::error_t									jsonIndexArrayPhone				= ::gpk::jsonExpressionResolve("phone"		, config.Reader, jsonIndexCurrentItem, viewMapURL);
		const ::gpk::error_t									jsonIndexArrayWP				= ::gpk::jsonExpressionResolve("whatsapp"	, config.Reader, jsonIndexCurrentItem, viewMapURL);
		const ::gpk::error_t									jsonIndexMap					= ::gpk::jsonExpressionResolve("location"	, config.Reader, jsonIndexCurrentItem, viewMapURL);
		if(lang == ::gpk::view_const_string{"es"}) {
			const ::gpk::error_t									jsonIndexWiki					= ::gpk::jsonExpressionResolve("es.wiki"		, config.Reader, jsonIndexCurrentItem, viewWikiURL			);
			const ::gpk::error_t									jsonIndexName					= ::gpk::jsonExpressionResolve("es.title"		, config.Reader, jsonIndexCurrentItem, viewWikiTitle		);
			const ::gpk::error_t									jsonIndexText					= ::gpk::jsonExpressionResolve("es.text"		, config.Reader, jsonIndexCurrentItem, viewWikiText			);
			const ::gpk::error_t									jsonIndexImageHRef				= ::gpk::jsonExpressionResolve("es.image.href"	, config.Reader, jsonIndexCurrentItem, viewWikiImageHRef	);
			const ::gpk::error_t									jsonIndexImageSrc				= ::gpk::jsonExpressionResolve("es.image.src"	, config.Reader, jsonIndexCurrentItem, viewWikiImageSrc		);
			const ::gpk::error_t									jsonIndexImageTitle				= ::gpk::jsonExpressionResolve("es.image.title"	, config.Reader, jsonIndexCurrentItem, viewWikiImageTitle	);
			const ::gpk::error_t									jsonIndexImageAlt				= ::gpk::jsonExpressionResolve("es.image.alt"	, config.Reader, jsonIndexCurrentItem, viewWikiImageAlt		);
		}
		else {
			const ::gpk::error_t									jsonIndexWiki					= ::gpk::jsonExpressionResolve("en.wiki"		, config.Reader, jsonIndexCurrentItem, viewWikiURL			);
			const ::gpk::error_t									jsonIndexName					= ::gpk::jsonExpressionResolve("en.title"		, config.Reader, jsonIndexCurrentItem, viewWikiTitle		);
			const ::gpk::error_t									jsonIndexText					= ::gpk::jsonExpressionResolve("en.text"		, config.Reader, jsonIndexCurrentItem, viewWikiText			);
			const ::gpk::error_t									jsonIndexImageHRef				= ::gpk::jsonExpressionResolve("en.image.href"	, config.Reader, jsonIndexCurrentItem, viewWikiImageHRef	);
			const ::gpk::error_t									jsonIndexImageSrc				= ::gpk::jsonExpressionResolve("en.image.src"	, config.Reader, jsonIndexCurrentItem, viewWikiImageSrc		);
			const ::gpk::error_t									jsonIndexImageTitle				= ::gpk::jsonExpressionResolve("en.image.title"	, config.Reader, jsonIndexCurrentItem, viewWikiImageTitle	);
			const ::gpk::error_t									jsonIndexImageAlt				= ::gpk::jsonExpressionResolve("en.image.alt"	, config.Reader, jsonIndexCurrentItem, viewWikiImageAlt		);
		}
		output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;text-align:center;border-style:solid;border-width:2px;\" >"});
		output.append(::gpk::view_const_string{ "\n<tr>"});
		output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;text-align:left;font-size:32px;vertical-align:top;\">"});
		//
			output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:16px;\" >"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:left;font-size:32px;vertical-align:top;\">"});
			//
			::ntl::htmlHRefLink(viewWikiTitle, viewWikiURL, "style=\"text-align:left;text-decoration:none;font-weight:bold;\"", output);
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:white;height:100%;text-align:left;font-size:32px;vertical-align:top;\">"});
			::ntl::htmlTag("p", viewWikiText, "style=\" font-weight:normal;text-align:left;\"", output);
			//
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{"\n</table>"});
		//
		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;height:100%;text-align:left;font-size:32px;vertical-align:top;\">"});

			output.append(::gpk::view_const_string{ "\n<table style=\"background-color:lightgrey;\" >"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:left;font-size:32px;vertical-align:top;width:100%;\">"});
			output.append(::gpk::view_const_string{ "<a target=\"blank\" title=\""});
			output.append(viewWikiImageTitle);
			output.append(::gpk::view_const_string{ "\" href=\""});
			output.append(viewWikiImageHRef);
			output.append(::gpk::view_const_string{ "\" class=\"image\" ><img style=\"width:320px;\" decoding=\"async\" elementtiming=\"thumbnail\" alt=\""});
			output.append(viewWikiImageAlt);
			output.append(::gpk::view_const_string{ "\" src=\""});
			output.append(viewWikiImageSrc);
			output.append(::gpk::view_const_string{ "\" ></a>"});
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;font-size:32px;vertical-align:top;\">"});

			// ----- Image info table
				output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:16px;\" >"});
				output.append(::gpk::view_const_string{ "\n<tr>"});
				if(viewMapURL.size()) {
					output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:center;font-size:32px;vertical-align:top;\">"});
					output.append(::gpk::view_const_string{"\n<a target=\"blank\" style=\"margin:4px;font-size:24px;\" href=\""});
					output.append(viewMapURL);
					output.append(::gpk::view_const_string{"\">"});
					if(lang == ::gpk::view_const_string{"es"})
						output.append(::gpk::view_const_string{"Localización"});
					else
						output.append(::gpk::view_const_string{"Location"});
					output.append(::gpk::view_const_string{"\n</a>"});
					output.append(::gpk::view_const_string{"\n</td>"});
					output.append(::gpk::view_const_string{"\n</tr>"});
				}
				output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%;\">"});
				output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:center;font-size:24px;vertical-align:top;border-style:solid;border-top-width:1px;\">"});
				// ----- Contact info
				const ::gpk::error_t									countPhones						= ::gpk::jsonArraySize(*config.Reader[jsonIndexArrayPhone	]);
				const ::gpk::error_t									countWPs						= ::gpk::jsonArraySize(*config.Reader[jsonIndexArrayWP		]);
				for(int32_t iPhone = 0; iPhone < countPhones	; ++iPhone) { ::gpk::view_const_string textPhone = config.Reader.View[::gpk::jsonArrayValueGet(*config.Reader[jsonIndexArrayPhone	], iPhone)]; output.append(textPhone); output.append(::gpk::view_const_string{"\n<br />"}); }

				for(int32_t iPhone = 0; iPhone < countWPs		; ++iPhone) {
					::gpk::view_const_string textPhone = config.Reader.View[::gpk::jsonArrayValueGet(*config.Reader[jsonIndexArrayWP], iPhone)];
					output.append(::gpk::view_const_string{"\n<a target=\"blank\" href=\"https://wa.me/"});
					output.append(textPhone);
					output.append(::gpk::view_const_string{"\" >"});
					output.append(textPhone);
					output.append(::gpk::view_const_string{"</a>"});
					output.append(::gpk::view_const_string{"<br />"});
					// https://wa.me/15551234567
				}

				//
				output.append(::gpk::view_const_string{"\n</td>"});
				output.append(::gpk::view_const_string{"\n</tr>"});
				output.append(::gpk::view_const_string{"\n</table>"});

			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{"\n</table>"});


		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});
		output.append(::gpk::view_const_string{"\n</table>"});
	}
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});

	output.append(::gpk::view_const_string{"\n</table>"});
	return 0;
}
