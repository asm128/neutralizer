#include "neutralizer_ad_category.h"

#include "neutralizer.h"

#include "gpk_json_expression.h"
#include "gpk_base64.h"

::gpk::error_t								ntl::pageCatalog					(const ::gpk::view_const_string & contentFileName, const ::gpk::SCoord2<uint32_t> screenSize, const ::gpk::view_const_string & pathStyles, const AD_SHOP_CATEGORY category, const ::gpk::view_const_string & title, const ::gpk::view_const_string & lang, ::gpk::array_pod<char_t> & output) {

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

	::ntl::htmlBoardGenerate(contentFileName, screenSize, category, title, lang, output);

	output.append(::gpk::view_const_string{"\n</td>"	});
	output.append(::gpk::view_const_string{"\n</tr>"	});
	output.append(::gpk::view_const_string{"\n</table>"	});
	output.append(::gpk::view_const_string{"\n</body>"	});
	output.append(::gpk::view_const_string{"\n</html>"	});
	return 0;
}

struct SItemViews {
	uint32_t											ItemIndex					= 0;
	::gpk::view_const_string							URL							= {};
	::gpk::view_const_string							Name						= {};
	::gpk::view_const_string							Title						= {};
	::gpk::view_const_string							Text						= {};
	::gpk::view_const_string							ImageHRef					= {};
	::gpk::view_const_string							ImageSrc					= {};
	::gpk::view_const_string							ImageTitle					= {};
	::gpk::view_const_string							ImageAlt					= {};
	::gpk::view_const_string							MapURL						= {};
	::gpk::array_obj<::gpk::view_const_string>			Phones						= {};
	::gpk::array_obj<::gpk::view_const_string>			Addresses					= {};
	::gpk::array_obj<::gpk::view_const_string>			WPs							= {};
};

::gpk::error_t										ntl::htmlBoardGenerate				(const ::gpk::view_const_string & contentFileName, const ::gpk::SCoord2<uint32_t> screenSize, const ::ntl::AD_SHOP_CATEGORY category, const ::gpk::view_const_string & title, const ::gpk::view_const_string & lang, ::gpk::array_pod<char_t> & output)	{
	//---------------------
	::gpk::SJSONFile										config								= {};
	gpk_necall(::gpk::jsonFileRead(config, contentFileName), "Failed to load configuration file: %s.", contentFileName);
	::gpk::array_pod<SItemViews>							indicesToDisplay					= {};
	for(int32_t iItem = 0, countItems = ::gpk::jsonArraySize(*config.Reader[0]); iItem < countItems; ++iItem) {
		SItemViews												views								= {(uint32_t)iItem};
		const ::gpk::error_t									jsonIndexCurrentItem				= ::gpk::jsonArrayValueGet(*config.Reader[0], iItem);
		::gpk::view_const_string								viewWikiCategory					= {};
		const ::gpk::error_t									jsonIndexArrayCategory				= ::gpk::jsonExpressionResolve("category", config.Reader, jsonIndexCurrentItem, viewWikiCategory);
		bool													isCategory							= false;
		for(uint32_t iCat = 0, countCat = (uint32_t)::gpk::jsonArraySize(*config.Reader[jsonIndexArrayCategory]); iCat < countCat; ++iCat) {
			const ::gpk::error_t									jsonIndexNodeCategory				= ::gpk::jsonArrayValueGet(*config.Reader[jsonIndexArrayCategory], iCat);
			const ::gpk::SJSONToken									& jsonToken							= config.Reader.Token[jsonIndexNodeCategory];
			if(category == jsonToken.Value) {
				isCategory											= true;
				break;
			}
		}
		if(false == isCategory)
			continue;

		// ---- Root arrays.
		const ::gpk::error_t									jsonIndexArrayPhone				= ::gpk::jsonExpressionResolve("phone"		, config.Reader, jsonIndexCurrentItem, views.MapURL		);
		const ::gpk::error_t									jsonIndexArrayWP				= ::gpk::jsonExpressionResolve("whatsapp"	, config.Reader, jsonIndexCurrentItem, views.MapURL		);
		const ::gpk::error_t									jsonIndexArrayAddr				= ::gpk::jsonExpressionResolve("address"	, config.Reader, jsonIndexCurrentItem, views.MapURL		);
		// ---- Root properties.
		const ::gpk::error_t									jsonIndexName					= ::gpk::jsonExpressionResolve("name"		, config.Reader, jsonIndexCurrentItem, views.Name		);
		const ::gpk::error_t									jsonIndexMap					= ::gpk::jsonExpressionResolve("location"	, config.Reader, jsonIndexCurrentItem, views.MapURL		);
		// ---- Language-based properties.
		const ::gpk::error_t									jsonIndexLang					= ::gpk::jsonExpressionResolve(lang			, config.Reader, jsonIndexCurrentItem, views.URL		);
		const ::gpk::error_t									jsonIndexWiki					= ::gpk::jsonExpressionResolve("wiki"		, config.Reader, jsonIndexLang, views.URL				);
		const ::gpk::error_t									jsonIndexTitle					= ::gpk::jsonExpressionResolve("title"		, config.Reader, jsonIndexLang, views.Title				);
		const ::gpk::error_t									jsonIndexText					= ::gpk::jsonExpressionResolve("text"		, config.Reader, jsonIndexLang, views.Text				);
		const ::gpk::error_t									jsonIndexImageHRef				= ::gpk::jsonExpressionResolve("image.href"	, config.Reader, jsonIndexLang, views.ImageHRef			);
		const ::gpk::error_t									jsonIndexImageSrc				= ::gpk::jsonExpressionResolve("image.src"	, config.Reader, jsonIndexLang, views.ImageSrc			);
		const ::gpk::error_t									jsonIndexImageTitle				= ::gpk::jsonExpressionResolve("image.title", config.Reader, jsonIndexLang, views.ImageTitle		);
		const ::gpk::error_t									jsonIndexImageAlt				= ::gpk::jsonExpressionResolve("image.alt"	, config.Reader, jsonIndexLang, views.ImageAlt			);

		const ::gpk::error_t									countPhones						= ::gpk::jsonArraySize(*config.Reader[jsonIndexArrayPhone	]);
		const ::gpk::error_t									countWPs						= ::gpk::jsonArraySize(*config.Reader[jsonIndexArrayWP		]);
		const ::gpk::error_t									countAddr						= ::gpk::jsonArraySize(*config.Reader[jsonIndexArrayAddr]);

		for(int32_t iPhone = 0; iPhone < countAddr; ++iPhone) {
			::gpk::view_const_string								textPhone						= config.Reader.View[::gpk::jsonArrayValueGet(*config.Reader[jsonIndexArrayAddr], iPhone)];
			views.Addresses.push_back(textPhone);
		}

		for(int32_t iPhone = 0; iPhone < countPhones	; ++iPhone) {
			::gpk::view_const_string								textPhone						= config.Reader.View[::gpk::jsonArrayValueGet(*config.Reader[jsonIndexArrayPhone], iPhone)];
			views.Phones.push_back(textPhone);
		}

		for(int32_t iPhone = 0; iPhone < countWPs		; ++iPhone) {
			::gpk::view_const_string								textPhone						= config.Reader.View[::gpk::jsonArrayValueGet(*config.Reader[jsonIndexArrayWP], iPhone)];
			views.WPs.push_back(textPhone);
		}

		gpk_necall(indicesToDisplay.push_back(views), "%s", "Out of memory?");
	}
	char													fontSize	[32]					= {};
	if(screenSize.x > screenSize.y)
		sprintf_s(fontSize, "%u", screenSize.x ? screenSize.x / 45 : 24);
	else
		sprintf_s(fontSize, "%u", screenSize.y ? screenSize.y / 34 : 24);

	struct SWordIndices {
		::gpk::view_const_string		Text;
		::gpk::array_pod<uint32_t>		Indices;
	};

	//::gpk::array_obj<SWordIndices>	wordSet;
	//for(int32_t iItem = 0, countItems = indicesToDisplay.size(); iItem < countItems; ++iItem) {
	//	bool bFound = false;
	//	::gpk::array_obj<::gpk::view_const_string>					itemWords;
	//	::gpk::split(, ' ', itemWords);
	//	for(uint32_t iWordInSet = 0; iWordInSet < wordSet.size(); ++iWordInSet) {
	//		if()
	//	}
	//
	//	if(false == bFound)
	//		wordSet.push_back();
	//}

	output.append(::gpk::view_const_string{"\n<div style=\"background-color:#ffffff;position:sticky;left:0;top:0;\">"});
	output.append(::gpk::view_const_string{ "\n<table style=\"left:0px;top:0px;position:sticky;width:100%;height:100%;text-align:center;\">"});

	output.append(::gpk::view_const_string{ "\n<tr style=\"position:sticky;left:0px;top:0px;\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"position:sticky;left:0px;top:0px;font-size:24px;font-weight:bold;vertical-align:top;\">"});
	if(lang == ::gpk::view_const_string{"es"})
		output.append(::gpk::view_const_string{"Buscar"});
	else
		output.append(::gpk::view_const_string{"Search"});

	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n<td style=\"position:sticky;left:0px;top:0px;sticky;font-size:24px;font-weight:bold;width:20px;text-align:left;vertical-align:top;\">"});
	output.append(::gpk::view_const_string{"\n<img src=\"/obelisco/image/blank.png\"/>"});
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{ "\n<td style=\"position:sticky;left:0px;top:0px;width:100%;text-align:left;vertical-align:top;\">"});
	output.append(::gpk::view_const_string{"<input onclick=\"return false\" style=\"position:sticky;left:0px;top:0px;font-size:24px;border-width:1px;width:50%;text-align:left;border-style:solid;border-radius:8px;\" type=\"text\"></input>"});
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{"\n</table>"});
	output.append(::gpk::view_const_string{"\n</div>"});

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

	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;text-align:center;border-style:solid;border-width:2px;\" >"});
	::gpk::array_pod<char_t>								base64Id;
	for(int32_t iItem = 0, countItems = indicesToDisplay.size(); iItem < countItems; ++iItem) {
		const SItemViews										& views						= indicesToDisplay[iItem];
		::gpk::base64EncodeFS(views.Name, base64Id);

		output.append(::gpk::view_const_string{"\n<tr id=\""});
		output.append(::gpk::view_const_string{base64Id.begin(), base64Id.size()});
		output.append(::gpk::view_const_string{"\">"});
		output.append(::gpk::view_const_string{"\n<td style=\"width:100%;text-align:left;font-size:32px;vertical-align:top;\">"});
		//
			output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:16px;\" >"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:left;font-size:32px;vertical-align:top;\">"});
			//
			::ntl::htmlHRefLink(views.Title, views.URL, "style=\"text-align:left;text-decoration:none;font-weight:bold;\"", output);
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:white;height:100%;text-align:left;font-size:"});
			output.append(::gpk::view_const_string{fontSize});
			output.append(::gpk::view_const_string{ "px;vertical-align:top;\">"});
			::ntl::htmlTag("p", views.Text, "style=\" font-weight:normal;text-align:left;\"", output);
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
			output.append(views.ImageTitle);
			output.append(::gpk::view_const_string{ "\" href=\""});
			output.append(views.ImageHRef);
			output.append(::gpk::view_const_string{ "\" class=\"image\" ><img style=\"width:320px;\" decoding=\"async\" elementtiming=\"thumbnail\" alt=\""});
			output.append(views.ImageAlt);
			output.append(::gpk::view_const_string{ "\" src=\""});
			output.append(views.ImageSrc);
			output.append(::gpk::view_const_string{ "\" ></a>"});
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;font-size:32px;vertical-align:top;\">"});

			// ----- Image info table
				output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:16px;\" >"});
				output.append(::gpk::view_const_string{ "\n<tr>"});
				if(views.MapURL.size()) {
					output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:center;font-size:32px;vertical-align:top;\">"});
					output.append(::gpk::view_const_string{"\n<a target=\"blank\" style=\"margin:4px;font-size:24px;\" href=\""});
					output.append(views.MapURL);
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
				// ----- Addresses
				for(uint32_t iPhone = 0, countAddr = views.Addresses.size(); iPhone < countAddr; ++iPhone) {
					output.append(views.Addresses[iPhone]);
					output.append(::gpk::view_const_string{"\n<br />"});
				}

				output.append(::gpk::view_const_string{"\n</td>"});
				output.append(::gpk::view_const_string{"\n</tr>"});

				// ----- Phones
				output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%;\">"});
				output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:center;font-size:24px;vertical-align:top;border-style:solid;border-top-width:1px;\">"});

				for(uint32_t iPhone = 0, countPhones = views.Phones.size(); iPhone < countPhones; ++iPhone) {
					::gpk::view_const_string								textPhone						= views.Phones[iPhone];
					output.append(textPhone);
					output.append(::gpk::view_const_string{"\n<br />"});

				}
				for(uint32_t iPhone = 0, countWPs	 = views.WPs	.size(); iPhone < countWPs		; ++iPhone) {
					::gpk::view_const_string								textPhone						= views.WPs[iPhone];
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
	}
	output.append(::gpk::view_const_string{"\n</table>"});

	output.append(::gpk::view_const_string{"\n<script>"});
	output.append(::gpk::view_const_string{""});
	output.append(::gpk::view_const_string{"\n</script>"});

	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});

	output.append(::gpk::view_const_string{"\n</table>"});
	return 0;
}
