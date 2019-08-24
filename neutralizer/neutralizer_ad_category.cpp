#include "neutralizer_ad_category.h"

#include "neutralizer.h"

#include "gpk_json_expression.h"
#include "gpk_base64.h"
#include "gpk_find.h"
#include "gpk_stdstring.h"

static	const ::gpk::view_const_string				jsSearch							=
	"\nfunction obeSearch(textToFind) {"
	"\n var lowerToFind		= textToFind.toLowerCase();"
	"\n	var wordsToFind		= lowerToFind.split(' ');"
	"\n	var pos				= 0;"
	"\n	for(pos = 0; pos < idList.length; pos = pos +1)"
	"\n		document.getElementById(idList[pos]).style.visibility = 'collapse';"
	"\n	for(pos = 0; pos < names.length; pos = pos +1) {"
	"\n		for(toFind of wordsToFind) {"
	"\n			if(' ' === toFind || '' === toFind)	"
	"\n				continue;		"
	"\n			if(names[pos].indexOf(toFind) > -1) {"
	"\n				var idc = 0;"
	"\n				for(idc = 0; idc < indices[pos].length; idc++)"
	"\n					document.getElementById(indices[pos][idc]).style.visibility = 'visible';"
	"\n			}"
	"\n		}"
	"\n	}"
	"\n}"
	"\nfunction clearSearch() {"
	"\n	for(id of idList)"
	"\n		document.getElementById(id).style.visibility = 'visible';"
	"\n}"
	;

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

static	::gpk::error_t								getWordList							(const ::SItemViews& item, ::gpk::array_obj<::gpk::array_pod<char_t>> & itemWords) {
	::gpk::array_obj<::gpk::view_const_string>				upper								= {};
	gpk_necall(::gpk::split(item.Text	, ' ', upper), "%s", "Out of memory?");
	gpk_necall(::gpk::split(item.Title	, ' ', upper), "%s", "Out of memory?");
	gpk_necall(::gpk::split(item.URL	, ' ', upper), "%s", "Out of memory?");
	gpk_necall(::gpk::split(item.Text	, ' ', upper), "%s", "Out of memory?");
	for(uint32_t iAddr = 0; iAddr < item.Addresses	.size(); ++iAddr) gpk_necall(::gpk::split(item.Addresses[iAddr], ' ', upper), "%s", "Out of memory?");
	for(uint32_t iAddr = 0; iAddr < item.Phones		.size(); ++iAddr) gpk_necall(::gpk::split(item.Phones	[iAddr], ' ', upper), "%s", "Out of memory?");
	for(uint32_t iAddr = 0; iAddr < item.WPs		.size(); ++iAddr) gpk_necall(::gpk::split(item.WPs		[iAddr], ' ', upper), "%s", "Out of memory?");

	gpk_necall(itemWords.resize(upper.size()), "%s", "Out of memory?");
	const ::gpk::TKeyValConstString							replaceDict	[]						=
		{ ::gpk::TKeyValConstString{"áéíóúàèìòùäëïöüãõñçºª", "aeiouaeiouaeiouaoncoa"}
		};
	::gpk::array_pod<char_t>								fixedWord;
	for(uint32_t iWord = 0, countWords = upper.size(); iWord < countWords; ++iWord) {
		::gpk::array_pod<char_t> & finalWord = itemWords[iWord];
		finalWord.resize(upper[iWord].size());
		memcpy(finalWord.begin(), upper[iWord].begin(), upper[iWord].size());
		::gpk::tolower(finalWord);
		fixedWord											= finalWord;
		bool													replaced		= false;
		for(uint32_t iChar = 0; iChar < fixedWord.size(); ++iChar) {
			char													& wordChar		= fixedWord[iChar];
			for(uint32_t iTable = 0; iTable < ::gpk::size(replaceDict); ++iTable) {
				const ::gpk::TKeyValConstString &	table = replaceDict[iTable];
				for(uint32_t iItem = 0; iItem < table.Key.size(); ++iItem)
					if(wordChar == table.Key[iItem]) {
						wordChar											= table.Val[iItem];
						replaced											= true;
					}
			}
		}
		if(replaced) {
			itemWords.push_back(fixedWord);
			info_printf("fixedWord: %s.", fixedWord.begin());
		}
		info_printf("finalWord: %s.", finalWord.begin());
	}
	return 0;
}

struct SWordIndices {
	::gpk::array_pod<char_t>							Text;
	::gpk::array_pod<uint32_t>							Indices;
};

static	::gpk::error_t								getWordDict							(const ::SItemViews& item, uint32_t iItem, ::gpk::array_obj<::SWordIndices> & wordSet, ::gpk::array_obj<::gpk::array_pod<char_t>> & cacheItemWords) {
	::getWordList(item, cacheItemWords);
	for(uint32_t iItemWord = 0; iItemWord < cacheItemWords.size(); ++iItemWord) {
		::gpk::view_const_string								itemWord						= {cacheItemWords[iItemWord].begin(), cacheItemWords[iItemWord].size()};
		info_printf("itemWord: %s.", itemWord.begin());
		if(1 >= itemWord.size())
			continue;
		bool													bFound							= false;
		if(',' == itemWord[itemWord.size() - 1] || '.' == itemWord[itemWord.size() - 1]) {
			itemWord											= {itemWord.begin(), itemWord.size() - 1};
			if(0 >= itemWord.size())
				continue;
		}
		for(uint32_t iWordInSet = 0; iWordInSet < wordSet.size(); ++iWordInSet) {
			SWordIndices											& dictWord						= wordSet[iWordInSet];
			if(itemWord != dictWord.Text)
				continue;
			bFound												= true;
			if(0 > ::gpk::find(iItem, {dictWord.Indices.begin(), dictWord.Indices.size()}))
				dictWord.Indices.push_back(iItem);
		}
		if(false == bFound)
			wordSet[wordSet.push_back({itemWord})].Indices.push_back(iItem);
	}
	return 0;
}

static	::gpk::error_t								outputSearchScript					(const ::gpk::view_array<const ::SItemViews> & indicesToDisplay, ::gpk::array_pod<char_t> & output) {
	::gpk::array_obj<::SWordIndices>						wordSet;
	::gpk::array_obj<::gpk::array_pod<char_t>>				cacheItemWords;

	for(uint32_t iItem = 0, countItems = indicesToDisplay.size(); iItem < countItems; ++iItem)
		::getWordDict(indicesToDisplay[iItem], iItem, wordSet, cacheItemWords);

	output.append(::gpk::view_const_string{"\nvar names=["});
	for(uint32_t iWord = 0, countWords = wordSet.size(); iWord < countWords; ++iWord) {
		const SWordIndices										& element							= wordSet[iWord];
		info_printf("Word: %s.", element.Text.begin());
		if(0 <= ::gpk::find('\'', ::gpk::view_array{element.Text.begin(), element.Text.size()})) {
			if(0 <= ::gpk::find('"', ::gpk::view_array{element.Text.begin(), element.Text.size()}))
				continue;
			output.push_back('"');
			output.append(element.Text);
			output.push_back('"');
		}
		else {
			output.push_back('\'');
			output.append(element.Text);
			output.push_back('\'');
		}
		if(iWord < (wordSet.size() - 1))
			output.push_back(',');
	}
	output.append(::gpk::view_const_string{"];"});

	output.append(::gpk::view_const_string{"\nvar indices=["});
	::gpk::array_pod<char_t>								base64Id;
	char													tempIntStr[256]				= {};
	for(uint32_t iWord = 0; iWord < wordSet.size(); ++iWord) {
		const SWordIndices		& element = wordSet[iWord];
		output.push_back('[');
		for(uint32_t iArticle = 0; iArticle < element.Indices.size(); ++iArticle) {
			base64Id.clear();
			sprintf_s(tempIntStr, "%u", element.Indices[iArticle]);
			::gpk::base64EncodeFS(::gpk::view_const_string{tempIntStr}, base64Id);

			output.push_back('\'');
			output.append(::gpk::view_const_string{base64Id.begin(), base64Id.size()});
			output.push_back('\'');
			if(iArticle < (element.Indices.size() - 1))
				output.push_back(',');
		}
		output.push_back(']');
		if(iWord < (wordSet.size() - 1))
			output.push_back(',');
	}
	output.append(::gpk::view_const_string{"];"});

	output.append(::gpk::view_const_string{"\nvar idList=["});
	for(uint32_t iArticle = 0; iArticle < indicesToDisplay.size(); ++iArticle) {
		base64Id.clear();
		sprintf_s(tempIntStr, "%u", iArticle);
		::gpk::base64EncodeFS(::gpk::view_const_string{tempIntStr}, base64Id);
		output.push_back('\'');
		output.append(::gpk::view_const_string{base64Id.begin(), base64Id.size()});
		output.push_back('\'');
		if(iArticle < (indicesToDisplay.size() - 1))
			output.push_back(',');
	}
	output.append(::gpk::view_const_string{"];"});
	output.append(jsSearch);
	return 0;
}

static	::gpk::error_t								getItemViews						(::gpk::SJSONReader & content, const ::ntl::AD_SHOP_CATEGORY category, const ::gpk::view_const_string & lang, ::gpk::array_obj<::SItemViews> & indicesToDisplay)	{
	for(int32_t iItem = 0, countItems = ::gpk::jsonArraySize(*content[0]); iItem < countItems; ++iItem) {
		SItemViews												views								= {(uint32_t)iItem};
		const ::gpk::error_t									jsonIndexCurrentItem				= ::gpk::jsonArrayValueGet(*content[0], iItem);
		::gpk::view_const_string								viewWikiCategory					= {};
		const ::gpk::error_t									jsonIndexArrayCategory				= ::gpk::jsonExpressionResolve("category", content, jsonIndexCurrentItem, viewWikiCategory);
		bool													isCategory							= false;
		for(uint32_t iCat = 0, countCat = (uint32_t)::gpk::jsonArraySize(*content[jsonIndexArrayCategory]); iCat < countCat; ++iCat) {
			const ::gpk::error_t									jsonIndexNodeCategory				= ::gpk::jsonArrayValueGet(*content[jsonIndexArrayCategory], iCat);
			const ::gpk::SJSONToken									& jsonToken							= content.Token[jsonIndexNodeCategory];
			if(category == jsonToken.Value) {
				isCategory											= true;
				break;
			}
		}
		if(false == isCategory)
			continue;

		// ---- Root arrays.
		const ::gpk::error_t									jsonIndexArrayPhone					= ::gpk::jsonExpressionResolve("phone"		, content, jsonIndexCurrentItem, views.MapURL		);
		const ::gpk::error_t									jsonIndexArrayWP					= ::gpk::jsonExpressionResolve("whatsapp"	, content, jsonIndexCurrentItem, views.MapURL		);
		const ::gpk::error_t									jsonIndexArrayAddr					= ::gpk::jsonExpressionResolve("address"	, content, jsonIndexCurrentItem, views.MapURL		);
		// ---- Root properties.
		const ::gpk::error_t									jsonIndexName						= ::gpk::jsonExpressionResolve("name"		, content, jsonIndexCurrentItem, views.Name		);
		const ::gpk::error_t									jsonIndexMap						= ::gpk::jsonExpressionResolve("location"	, content, jsonIndexCurrentItem, views.MapURL		);
		// ---- Language-based properties.
		const ::gpk::error_t									jsonIndexLang						= ::gpk::jsonExpressionResolve(lang			, content, jsonIndexCurrentItem, views.URL		);
		const ::gpk::error_t									jsonIndexWiki						= ::gpk::jsonExpressionResolve("wiki"		, content, jsonIndexLang, views.URL				);
		const ::gpk::error_t									jsonIndexTitle						= ::gpk::jsonExpressionResolve("title"		, content, jsonIndexLang, views.Title				);
		const ::gpk::error_t									jsonIndexText						= ::gpk::jsonExpressionResolve("text"		, content, jsonIndexLang, views.Text				);
		const ::gpk::error_t									jsonIndexImageHRef					= ::gpk::jsonExpressionResolve("image.href"	, content, jsonIndexLang, views.ImageHRef			);
		const ::gpk::error_t									jsonIndexImageSrc					= ::gpk::jsonExpressionResolve("image.src"	, content, jsonIndexLang, views.ImageSrc			);
		const ::gpk::error_t									jsonIndexImageTitle					= ::gpk::jsonExpressionResolve("image.title", content, jsonIndexLang, views.ImageTitle		);
		const ::gpk::error_t									jsonIndexImageAlt					= ::gpk::jsonExpressionResolve("image.alt"	, content, jsonIndexLang, views.ImageAlt			);

		for(int32_t iPhone = 0, countAddr	= ::gpk::jsonArraySize(*content[jsonIndexArrayAddr	]); iPhone < countAddr	; ++iPhone)	views.Addresses	.push_back(content.View[::gpk::jsonArrayValueGet(*content[jsonIndexArrayAddr	], iPhone)]);
		for(int32_t iPhone = 0, countPhones	= ::gpk::jsonArraySize(*content[jsonIndexArrayPhone	]); iPhone < countPhones; ++iPhone)	views.Phones	.push_back(content.View[::gpk::jsonArrayValueGet(*content[jsonIndexArrayPhone	], iPhone)]);
		for(int32_t iPhone = 0, countWPs	= ::gpk::jsonArraySize(*content[jsonIndexArrayWP	]); iPhone < countWPs	; ++iPhone)	views.WPs		.push_back(content.View[::gpk::jsonArrayValueGet(*content[jsonIndexArrayWP		], iPhone)]);

		gpk_necall(indicesToDisplay.push_back(views), "%s", "Out of memory?");
	}
	return 0;
}

static ::gpk::error_t								htmlBoardGenerate					(const ::gpk::view_array<const ::SItemViews> indicesToDisplay, const ::gpk::SCoord2<uint32_t> screenSize, const ::gpk::view_const_string & title, const ::gpk::view_const_string & lang, ::gpk::array_pod<char_t> & output)	{
	char													fontSize	[32]					= {};
	if(screenSize.x > screenSize.y)
		sprintf_s(fontSize, "%u", screenSize.x ? screenSize.x / 55 : 24);
	else
		sprintf_s(fontSize, "%u", screenSize.y ? screenSize.y / 44 : 24);

	output.append(::gpk::view_const_string{
		"\n<div style=\"background-color:#ffffff;position:sticky;left:0;top:0;\">"
		"\n<table style=\"width:100%;height:100%;text-align:center;\">"
		"\n<tr style=\"\" >"
		"\n<td style=\"font-weight:bold;vertical-align:center;font-size:"
		});
	output.append(::gpk::view_const_string{fontSize});
	output.append(::gpk::view_const_string{ "px;\" >"});

	const ::gpk::view_const_string							textSearch							= (lang == ::gpk::view_const_string{"es"}) ? "Buscar" : "Search";
	::ntl::htmlTag("h5", textSearch, {}, output);

	output.append(::gpk::view_const_string{
		"\n</td>"
		"\n<td style=\"position:sticky;left:0px;top:0px;sticky;font-size:24px;font-weight:bold;width:20px;text-align:left;vertical-align:top;\">"
	//"\n<img src=\"/obelisco/image/blank.png\"/>"
		"\n</td>"
		"\n<td style=\"position:sticky;left:0px;top:0px;width:100%;text-align:left;vertical-align:top;\">"
		"<input oninput=\"if(this.value.length > 0) obeSearch(this.value); else clearSearch();\" style=\"position:sticky;left:0px;top:0px;font-size:24px;border-width:1px;width:50%;text-align:left;border-style:solid;border-radius:8px;\" type=\"text\"></input>"
		"\n</td>"
		});

	//output.append(::gpk::view_const_string{"\n<td style=\"font-weight:bold;font-size:"});
	//output.append(::gpk::view_const_string{fontSize});
	//output.append(::gpk::view_const_string{ "px;\" >"});
	//output.append(::gpk::view_const_string{"<h4>Barrio:<h4>"});
	//output.append(::gpk::view_const_string{"</td>"});
	//output.append(::gpk::view_const_string{
	//	"\n<td>"
	//	"<select id=\"testSelect1\" style=\"font-size:"
	//	});
	//output.append(::gpk::view_const_string{fontSize});
	//output.append(::gpk::view_const_string{ "px;\" >"});
	//
	//::gpk::SJSONFile			barrios;
	//
	//output.append(::gpk::view_const_string{
	//	"  <option value=\"1\"  style=\"\">Montserrat</option>"
	//	});
	//output.append(::gpk::view_const_string{
	//	"</select>"
	//	"\n</td>"
	//	});
	output.append(::gpk::view_const_string{
		"\n</tr>"
		"\n</table>"
		"\n</div>"
		});

	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;font-size:"});
	output.append(::gpk::view_const_string{fontSize});
	output.append(::gpk::view_const_string{ "px;\" >"});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-weight:bold; vertical-align:top;\">"});
	::ntl::htmlTag("h2", title, {}, output);
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-weight:bold; vertical-align:top;\">"});

		output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;text-align:center;border-style:solid;border-width:2px;font-size:"});
		output.append(::gpk::view_const_string{fontSize});
		output.append(::gpk::view_const_string{"px;\" >"});
		::gpk::array_pod<char_t>								base64Id;
		char													tempIntStr[256]				= {};
		for(int32_t iItem = 0, countItems = indicesToDisplay.size(); iItem < countItems; ++iItem) {
			const SItemViews										& views						= indicesToDisplay[iItem];
			base64Id.clear();
			sprintf_s(tempIntStr, "%u", iItem);
			::gpk::base64EncodeFS(::gpk::view_const_string{tempIntStr}, base64Id);

			output.append(::gpk::view_const_string{"\n<tr id=\""});
			output.append(::gpk::view_const_string{base64Id.begin(), base64Id.size()});
			output.append(::gpk::view_const_string{"\">"});
			output.append(::gpk::view_const_string{"\n<td style=\"width:100%;text-align:left;vertical-align:top;\">"});
			//
				output.append(::gpk::view_const_string{"\n<table style=\"width:100%;height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:16px;font-size:"});
				output.append(::gpk::view_const_string{fontSize});
				output.append(::gpk::view_const_string{"px;\" >"});
				output.append(::gpk::view_const_string{"\n<tr>"});
				output.append(::gpk::view_const_string{"\n<td style=\"background-color:lightgrey;text-align:left;vertical-align:top;\">"});
				//
				::gpk::array_pod<char_t> pTitle;
				::ntl::htmlTag("h3", views.Title, "style=\"text-align:left;\"", pTitle);
				::ntl::htmlHRefLink({pTitle.begin(), pTitle.size()}, views.URL, "style=\"text-decoration:none;font-weight:bold;\"", output);
				output.append(::gpk::view_const_string{"\n</td>"});
				output.append(::gpk::view_const_string{"\n</tr>"});
				output.append(::gpk::view_const_string{"\n<tr>"});
				output.append(::gpk::view_const_string{"\n<td style=\"vertical-align:top;background-color:white;height:100%;text-align:left;font-size:"});
				output.append(::gpk::view_const_string{fontSize});
				output.append(::gpk::view_const_string{ "px;\">"});
				::ntl::htmlTag("h3", views.Text, "style=\" font-weight:normal;text-align:left;\"", output);
				//
				output.append(::gpk::view_const_string{"\n</td>"});
				output.append(::gpk::view_const_string{"\n</tr>"});
				output.append(::gpk::view_const_string{"\n</table>"});
			//
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;height:100%;text-align:left;vertical-align:top;\">"});

				output.append(::gpk::view_const_string{ "\n<table style=\"background-color:lightgrey;\" >"});
				output.append(::gpk::view_const_string{ "\n<tr>"});
				output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:left;vertical-align:top;width:100%;\">"});
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
				output.append(::gpk::view_const_string{"\n</tr><tr>"});
				output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;vertical-align:top;\">"});

				// ----- Image info table
					output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:16px;\" font-size:"});
					output.append(::gpk::view_const_string{fontSize});
					output.append(::gpk::view_const_string{ "px;\">"});
					if(views.MapURL.size()) {
						output.append(::gpk::view_const_string{ "\n<tr>"});
						output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:center;vertical-align:top;font-size:"});
						output.append(::gpk::view_const_string{fontSize});
						output.append(::gpk::view_const_string{ "px;\">"});
						output.append(::gpk::view_const_string{"\n<a target=\"blank\" style=\"margin:4px;\" href=\""});
						output.append(views.MapURL);
						output.append(::gpk::view_const_string{"\" >"});
						if(lang == ::gpk::view_const_string{"es"})
							output.append(::gpk::view_const_string{"Localización"});
						else
							output.append(::gpk::view_const_string{"Location"});
						output.append(::gpk::view_const_string{"\n</a>"});
						output.append(::gpk::view_const_string{"\n</td>"});
						output.append(::gpk::view_const_string{"\n</tr>"});
					}
					// ----- Addresses
					if(views.Addresses.size()) {
						output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%;\">"});
						output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:center;vertical-align:top;border-style:solid;border-top-width:1px;font-size:"});
						output.append(::gpk::view_const_string{fontSize});
						output.append(::gpk::view_const_string{"px;\" >"});
						for(uint32_t iPhone = 0, countAddr = views.Addresses.size(); iPhone < countAddr; ++iPhone) {
							output.append(views.Addresses[iPhone]);
							output.append(::gpk::view_const_string{"\n<br />"});
						}
						output.append(::gpk::view_const_string{"\n</td>"});
						output.append(::gpk::view_const_string{"\n</tr>"});
					}

					// ----- Phones
					if(views.Phones.size() || views.WPs.size()) {
						output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%;\">"});
						output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:center;vertical-align:top;border-style:solid;border-top-width:1px;font-size:"});
						output.append(::gpk::view_const_string{fontSize});
						output.append(::gpk::view_const_string{"px;\" >"});
						for(uint32_t iPhone = 0, countPhones = views.Phones.size(); iPhone < countPhones; ++iPhone) {
							output.append(views.Phones[iPhone]);
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
						}

						//
						output.append(::gpk::view_const_string{"\n</td>"});
						output.append(::gpk::view_const_string{"\n</tr>"});
					}
					output.append(::gpk::view_const_string{"\n</table>"});

				output.append(::gpk::view_const_string{"\n</td>"});
				output.append(::gpk::view_const_string{"\n</tr>"});
				output.append(::gpk::view_const_string{"\n</table>"});


			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
		}
		output.append(::gpk::view_const_string{"\n</table>"});

	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{"\n</table>"});

	output.append(::gpk::view_const_string{"\n<script charset=\"iso-8859-1\">"});
	::outputSearchScript(indicesToDisplay, output);
	output.append(::gpk::view_const_string{"\n</script>"});
	return 0;
}

::gpk::error_t								ntl::pageCatalog					(const ::gpk::view_const_string & contentFileName, const ::gpk::SCoord2<uint32_t> screenSize, const ::gpk::view_const_string & pathStyles, const AD_SHOP_CATEGORY category, const ::gpk::view_const_string & title, const ::gpk::view_const_string & lang, ::gpk::array_pod<char_t> & output) {

	output.append(::gpk::view_const_string{ "\n<html>"});
	// --- Head
	output.append(::gpk::view_const_string{ "\n<head>"});
	output.append(::gpk::view_const_string{ "\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />"});
	::gpk::array_pod<char_t>								fileStyle			;
	::ntl::httpPath(pathStyles, "blankstyle", "css"	, fileStyle);
	::ntl::htmlHeaderTitle		(title, output);
	::ntl::htmlHeaderStyleLink	({fileStyle.begin(), fileStyle.size()}, output);
	output.append(::gpk::view_const_string{"\n</head>"});

	// --- Body
	output.append(::gpk::view_const_string{
		"\n<body style=\"width:100%;height:100%;background-color:#E0E0E0;font-family:Arial;\">"
		"\n<table style=\"width:100%;height:100%;text-align:center;\">"
		"\n<tr style=\"\" >"
		"\n<td style=\"font-size:16px; font-weight:bold; vertical-align:top;\">"
		});

	//---------------------
	::gpk::SJSONFile										config								= {};
	gpk_necall(::gpk::jsonFileRead(config, contentFileName), "Failed to load configuration file: %s.", contentFileName);
	::gpk::array_obj<::SItemViews>							indicesToDisplay					= {};
	::getItemViews(config.Reader, category, lang, indicesToDisplay);

	::htmlBoardGenerate(indicesToDisplay, screenSize, title, lang, output);

	output.append(::gpk::view_const_string{
		"\n</td>"
		"\n</tr>"
		"\n</table>"
		"\n</body>"
		});

	output.append(::gpk::view_const_string{"\n</html>"	});
	return 0;
}
