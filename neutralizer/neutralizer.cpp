#include "neutralizer.h"

#include "gpk_base64.h"

#include "gpk_json_expression.h"

::gpk::error_t										ntl::httpPath						(::gpk::view_const_string folder, ::gpk::view_const_string name, ::gpk::view_const_string extension, ::gpk::array_pod<char_t> & output) {
	output.append(folder);
	output.push_back('/');
	output.append(name);
	output.push_back('.');
	output.append(extension);
	return 0;
}

::gpk::error_t										ntl::htmlMenuIcon
	(	const ::gpk::view_const_string	& pathImages
	,	const ::gpk::view_const_string	& extension
	,	const ::gpk::view_const_string	& itemName
	,	const ::gpk::view_const_string	& txtIcon
	,	::gpk::array_pod<char_t>		& output
	, bool								iconLarge
	) {

	char													events		[4096]		= {};
	::gpk::array_pod<char_t>								iconId					= {};
	::gpk::array_pod<char_t>								idBase64				= {};
	{ // Generate id for this icon
		iconId											= ::gpk::view_const_string{ "id_icon_"};
		iconId.append(txtIcon);
		::gpk::base64EncodeFS(iconId, idBase64);
	}
	output.append(::gpk::view_const_string{ "<a style=\"height: 100%;\" href=\"" });
	output.append(itemName);
	output.push_back('.');
	output.append(::gpk::view_const_string{ "exe\" >" });
	output.append(::gpk::view_const_string{ "<table style=\"width: 100%; height: 100%;text-align:center;\" " });
	output.append(::gpk::view_const_string{" id=\""});
	output.append(idBase64);
	output.append(::gpk::view_const_string{"\" "});
	idBase64.push_back(0);
	sprintf_s(events, "onclick=\"cellClick('%s');\" onmouseout=\"cellColor('%s', null, 0);\" onmouseover=\"cellColor('%s', '#ec8106', 0);\" ", idBase64.begin(), idBase64.begin(), idBase64.begin());
	output.append(::gpk::view_const_string{events});
	output.append(::gpk::view_const_string{" >"});

	output.append(::gpk::view_const_string{ "<tr><td>"	});
	output.append(::gpk::view_const_string{ "<image style=\"\"  src=\""	});
	output.append(pathImages);
	if(iconLarge)
		output.append(::gpk::view_const_string{ "/icon_large_"								});
	else
		output.append(::gpk::view_const_string{ "/icon_small_"								});
	output.append(itemName);
	output.push_back('.');
	output.append(extension);
	output.append(::gpk::view_const_string{ "\" />"										});
	output.append(::gpk::view_const_string{"</td></tr>"										});

	output.append(::gpk::view_const_string{ "<tr >"										});
	output.append(::gpk::view_const_string{ "<td style=\"max-height:1.5em;font-size:1.5em;\" >"			});
	output.append(::gpk::view_const_string{ "<p style=\"color:black;max-height:1.5em;font-size:1.5em;\" >"			});
	output.append(txtIcon);
	output.append(::gpk::view_const_string{"</p>"										});
	output.append(::gpk::view_const_string{"</td>"										});
	output.append(::gpk::view_const_string{ "</tr>"										});
	output.append(::gpk::view_const_string{ "</table>"									});
	output.append(::gpk::view_const_string{ "</a>"									});
	return 0;
}

::gpk::error_t									ntl::htmlMenuIconsHorizontal
	( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
	, const ::gpk::view_const_string					& pathImages
	, const ::gpk::view_const_string					& extensionImages
	, ::gpk::array_pod<char_t>							& output
	, bool												iconsLarge
	) {
	output.append(::gpk::view_const_string{ "<table style=\"width:100%;height:100%;\" >"		});	// 181830
	output.append(::gpk::view_const_string{ "<tr style=\"height:100%; \">"			});
	char													iconWidth	[32]		= {};
	const double											iconWidthPercent		= 100 / menuItems.size();
	sprintf_s(iconWidth, "%f%%", iconWidthPercent);
	for(uint32_t iItem = 0; iItem < menuItems.size(); ++iItem) {
		const ::ntl::SHTMLIcon									currentItem				= menuItems[iItem];
		output.append(::gpk::view_const_string{"<td style=\"height:100%;width:"});
		output.append(::gpk::view_const_string{iconWidth});
		output.append(::gpk::view_const_string{"\" >"});

		::ntl::htmlMenuIcon(pathImages, extensionImages, currentItem.Item, currentItem.Text, output, iconsLarge);	// ------ Unknown icon
		output.append(::gpk::view_const_string{"</td>"});
	}
	output.append(::gpk::view_const_string{"</tr>"			});
	output.append(::gpk::view_const_string{"</table>"		});
	return 0;
}


::gpk::error_t										ntl::htmlControlMenuIcon
	(	const ::gpk::view_const_string	& pathImages
	,	const ::gpk::view_const_string	& extension
	,	const ::gpk::view_const_string	& itemName
	,	const ::gpk::view_const_string	& txtIcon
	,	::gpk::array_pod<char_t>		& output
	, bool								iconLarge
	) {

	char													events		[4096]		= {};
	::gpk::array_pod<char_t>								iconId					= {};
	::gpk::array_pod<char_t>								idBase64				= {};
	{ // Generate id for this icon
		iconId											= ::gpk::view_const_string{ "id_icon_"};
		iconId.append(txtIcon);
		::gpk::base64EncodeFS(iconId, idBase64);
	}
	output.append(::gpk::view_const_string{ "\n<table style=\"width: 100%; height: 100%;text-align:center;\" " });
	output.append(::gpk::view_const_string{" id=\""});
	output.append(idBase64);
	output.append(::gpk::view_const_string{"\" onclick=\"reframe('dumMainFrame', '"});
	output.append(itemName);
	output.append(::gpk::view_const_string{"', '"});
	output.append(itemName);
	output.push_back('.');
	output.append(::gpk::view_const_string{"exe');\" "});
	idBase64.push_back(0);
	sprintf_s(events, "onmouseout=\"cellColor('%s', null, 0);\" onmouseover=\"cellColor('%s', '#ec8106', 0);\" ", idBase64.begin(), idBase64.begin());

	output.append(::gpk::view_const_string{events});
	output.append(::gpk::view_const_string{" >"});

	output.append(::gpk::view_const_string{ "\n<tr>\n<td>"	});
	output.append(::gpk::view_const_string{ "\n<image style=\"\"  src=\""	});
	output.append(pathImages);
	if(iconLarge)
		output.append(::gpk::view_const_string{ "/icon_large_"								});
	else
		output.append(::gpk::view_const_string{ "/icon_small_"								});
	output.append(itemName);
	output.push_back('.');
	output.append(extension);
	output.append(::gpk::view_const_string{ "\" />"										});
	output.append(::gpk::view_const_string{"\n</td></tr>"										});

	output.append(::gpk::view_const_string{ "\n<tr >"										});
	output.append(::gpk::view_const_string{ "\n<td style=\font-size:1.5em;\" >"			});
	output.append(::gpk::view_const_string{ "\n<p style=\"color:black;font-size:1.5em;\" id=\""			});
	output.append(itemName);
	output.append(::gpk::view_const_string{ "\" >"										});
	output.append(txtIcon);
	output.append(::gpk::view_const_string{"</p>"										});
	output.append(::gpk::view_const_string{"\n</td>"									});
	output.append(::gpk::view_const_string{"\n</tr>"									});
	output.append(::gpk::view_const_string{"\n</table>"									});
	//output.append(::gpk::view_const_string{ "</a>"									});
	return 0;
}

::gpk::error_t									ntl::htmlControlMenuIconsHorizontal
	( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
	, const ::gpk::view_const_string					& pathImages
	, const ::gpk::view_const_string					& extensionImages
	, ::gpk::array_pod<char_t>							& output
	, bool												iconsLarge
	) {
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;\" >"		});	// 181830
	output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%; \">"			});
	char													iconWidth	[32]		= {};
	const double											iconWidthPercent		= 100 / menuItems.size();
	sprintf_s(iconWidth, "%f%%", iconWidthPercent);
	for(uint32_t iItem = 0; iItem < menuItems.size(); ++iItem) {
		const ::ntl::SHTMLIcon									currentItem				= menuItems[iItem];
		output.append(::gpk::view_const_string{"\n<td style=\"height:100%;width:"});
		output.append(::gpk::view_const_string{iconWidth});
		output.append(::gpk::view_const_string{"\" >"});

		::ntl::htmlControlMenuIcon(pathImages, extensionImages, currentItem.Item, currentItem.Text, output, iconsLarge);	// ------ Unknown icon
		output.append(::gpk::view_const_string{"\n</td>"});
	}
	output.append(::gpk::view_const_string{"\n</tr>"			});
	output.append(::gpk::view_const_string{"\n</table>"		});
	return 0;
}


::gpk::error_t									ntl::htmlTag
	( const ::gpk::view_const_string	& tagName
	, const ::gpk::view_const_string	& content
	, const ::gpk::view_const_string	& attributes
	, ::gpk::array_pod<char_t>			& output
	) {
	gpk_necall(output.push_back('<')	, "%s", "Out of memory?");
	gpk_necall(output.append(tagName)	, "%s", "Out of memory?");
	if(attributes.size()) {
		gpk_necall(output.push_back(' ')	, "%s", "Out of memory?");
		gpk_necall(output.append(attributes), "%s", "Out of memory?");
		gpk_necall(output.push_back(' ')	, "%s", "Out of memory?");
	}
	gpk_necall(output.push_back('>')	, "%s", "Out of memory?");
	gpk_necall(output.append(content)	, "%s", "Out of memory?");
	gpk_necall(output.push_back('<')	, "%s", "Out of memory?");
	gpk_necall(output.push_back('/')	, "%s", "Out of memory?");
	gpk_necall(output.append(tagName)	, "%s", "Out of memory?");
	gpk_necall(output.push_back('>')	, "%s", "Out of memory?");
	return 0;
}

::gpk::error_t									ntl::htmlVoidTag
	( const ::gpk::view_const_string	& tagName
	, const ::gpk::view_const_string	& attributes
	, ::gpk::array_pod<char_t>			& output
	) {
	gpk_necall(output.push_back('<')	, "%s", "Out of memory?");
	gpk_necall(output.append(tagName)	, "%s", "Out of memory?");
	if(attributes.size()) {
		gpk_necall(output.push_back(' ')	, "%s", "Out of memory?");
		gpk_necall(output.append(attributes), "%s", "Out of memory?");
		gpk_necall(output.push_back(' ')	, "%s", "Out of memory?");
	}
	gpk_necall(output.push_back('/')	, "%s", "Out of memory?");
	gpk_necall(output.push_back('>')	, "%s", "Out of memory?");
	return 0;
}

::gpk::error_t									ntl::loadConfig					(::ntl::SHTMLEndpoint & programState, int32_t indexRoot)	{
	if(-1 == indexRoot) {
		::gpk::view_const_string						rootNode;
		indexRoot									= ::gpk::jsonExpressionResolve("neutralizer", programState.Config.Reader, 0, rootNode);
	}
	{
		::gpk::view_const_string							rootPart;
		::gpk::view_const_string							pathPart;
		::gpk::array_pod<char>								path;
		::gpk::jsonExpressionResolve("front.http.path.root", programState.Config.Reader, indexRoot, rootPart);
		path											= rootPart;
		::gpk::jsonExpressionResolve("front.http.path.image", programState.Config.Reader, indexRoot, pathPart);
		path.append(pathPart);
		programState.Path.Image							= ::gpk::label(path.begin(), (uint32_t)-1);

		path											= rootPart;
		::gpk::jsonExpressionResolve("front.http.path.css", programState.Config.Reader, indexRoot, pathPart);
		path.append(pathPart);
		programState.Path.Style							= ::gpk::label(path.begin(), (uint32_t)-1);

		path											= rootPart;
		::gpk::jsonExpressionResolve("front.http.path.js", programState.Config.Reader, indexRoot, pathPart);
		path.append(pathPart);
		programState.Path.Script						= ::gpk::label(path.begin(), (uint32_t)-1);

		path											= rootPart;
		::gpk::jsonExpressionResolve("front.http.path.exe", programState.Config.Reader, indexRoot, pathPart);
		path.append(pathPart);
		programState.Path.Program						= ::gpk::label(path.begin(), (uint32_t)-1);
	}

	::gpk::jsonExpressionResolve("front.extension.image"	, programState.Config.Reader, indexRoot, programState.Extension.Image	);
	::gpk::jsonExpressionResolve("front.extension.program"	, programState.Config.Reader, indexRoot, programState.Extension.Program	);
	return 0;
}
