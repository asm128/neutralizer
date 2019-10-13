#include "ntl_front.h"

#include "gpk_base64.h"

#include "gpk_json_expression.h"

::gpk::error_t										ntl::loadNTLArgs					(SNTLArgs & out_loaded, const ::gpk::view_array<const ::gpk::TKeyValConstString> & queryStringKeyVals)	{
	::gpk::find("s", queryStringKeyVals, out_loaded.Session);
	::gpk::find("l", queryStringKeyVals, out_loaded.Language);
	::gpk::find("m", queryStringKeyVals, out_loaded.Module);
	::gpk::find("w", queryStringKeyVals, out_loaded.Width);
	::gpk::find("h", queryStringKeyVals, out_loaded.Height);
	return 0;
}

::gpk::error_t										ntl::httpPath						(::gpk::view_const_char folder, ::gpk::view_const_string name, ::gpk::view_const_char extension, ::gpk::array_pod<char_t> & output) {
	output.append(folder);
	output.push_back('/');
	output.append(name);
	output.push_back('.');
	output.append(extension);
	return 0;
}

::gpk::error_t										ntl::htmlMenuIcon
	(	const ::gpk::view_const_char	& pathImages
	,	const ::gpk::view_const_char	& extension
	,	const ::gpk::view_const_char	& itemName
	,	const ::gpk::view_const_char	& txtIcon
	,	::gpk::array_pod<char_t>		& output
	, bool								iconLarge
	) {
	char													events		[4096]					= {};
	::gpk::array_pod<char_t>								iconId								= {};
	::gpk::array_pod<char_t>								idBase64							= {};
	{ // Generate id for this icon
		iconId												= ::gpk::view_const_string{"id_icon_"};
		iconId.append(txtIcon);
		::gpk::base64EncodeFS(iconId, idBase64);
	}
	output.append_string("<table style=\"width: 100%; height: 100%;text-align:center;\" ");
	output.append_string(" id=\"");
	output.append(idBase64);
	output.append_string("\" ");
	idBase64.push_back(0);
	sprintf_s(events, "onmouseout=\"cellColor('%s', null, 0);\" onmouseover=\"cellColor('%s', '#ec8106', 0);\" ", idBase64.begin(), idBase64.begin());
	output.append(::gpk::view_const_string{events});
	output.append_string(" >");

	output.append_string("<tr><td>");
	output.append_string("<image style=\"\"  src=\"");
	output.append(pathImages);
	if(iconLarge)
		output.append_string("/icon_large_");
	else
		output.append_string("/icon_small_");
	output.append(itemName);
	output.push_back('.');
	output.append(extension);
	output.append_string("\" />");
	output.append_string("</td></tr>");

	output.append_string("<tr >");
	output.append_string("<td style=\"max-height:1.5em;font-size:1.5em;\" >");
	output.append_string("<p style=\"color:black;max-height:1.5em;font-size:1.5em;\" >");
	output.append(txtIcon);
	output.append_string("</p>");
	output.append_string("</td>");
	output.append_string("</tr>");
	output.append_string("</table>");
	return 0;
}

::gpk::error_t									ntl::htmlMenuIconsHorizontal
	( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
	, const ::gpk::view_const_char						& pathImages
	, const ::gpk::view_const_char						& extensionImages
	, ::gpk::array_pod<char_t>							& output
	, bool												iconsLarge
	) {
	output.append_string("<table style=\"width:100%;height:100%;\" >");	// 181830
	output.append_string("<tr style=\"height:100%; \">");
	char													iconWidth	[32]		= {};
	const double											iconWidthPercent		= 100 / menuItems.size();
	sprintf_s(iconWidth, "%f%%", iconWidthPercent);
	for(uint32_t iItem = 0; iItem < menuItems.size(); ++iItem) {
		const ::ntl::SHTMLIcon									currentItem				= menuItems[iItem];
		output.append_string("<td style=\"height:100%;width:");
		output.append(::gpk::view_const_string{iconWidth});
		output.append_string("\" >");

		::ntl::htmlMenuIcon(pathImages, extensionImages, currentItem.Item, currentItem.Text, output, iconsLarge);	// ------ Unknown icon
		output.append_string("</td>");
	}
	output.append_string("</tr>");
	output.append_string("</table>");
	return 0;
}


::gpk::error_t										ntl::htmlControlMenuIcon
	(	const ::gpk::view_const_char	& pathImages
	,	const ::gpk::view_const_char	& extension
	,	const ::ntl::SHTMLIcon			& menuItem
	,	::gpk::array_pod<char_t>		& output
	, bool								iconLarge
	) {
	char													events		[4096]		= {};
	::gpk::array_pod<char_t>								iconId					= {};
	::gpk::array_pod<char_t>								idBase64				= {};
	{ // Generate id for this icon
		iconId											= ::gpk::view_const_string{ "id_icon_"};
		iconId.append(menuItem.Text);
		::gpk::base64EncodeFS(iconId, idBase64);
	}
	output.append_string("\n<table style=\"width: 100%; height: 100%;text-align:center;\" ");
	output.append_string(" id=\"");
	output.append(idBase64);
	output.append_string("\" onclick=\"reframe('tuMainFrame', '");
	output.append(menuItem.Item);
	output.append_string("', '");
	output.append(menuItem.Program);
	output.push_back('.');
	output.append_string("exe', document.getElementById('l').value, document.getElementById('s').value);\" ");
	idBase64.push_back(0);
	sprintf_s(events, "onmouseout=\"cellColor('%s', null, 0);\" onmouseover=\"cellColor('%s', '#ec8106', 0);\" ", idBase64.begin(), idBase64.begin());

	output.append_string(events);
	output.append_string(" >");

	output.append_string("\n<tr>\n<td>");
	output.append_string("\n<image style=\"\"  src=\"");
	output.append(pathImages);
	if(iconLarge)
		output.append_string("/icon_large_");
	else
		output.append_string("/icon_small_");
	output.append(menuItem.Item);
	output.push_back('.');
	output.append(extension);
	output.append_string("\" />");
	output.append_string("\n</td></tr>");

	output.append_string("\n<tr >");
	output.append_string("\n<td style=\font-size:1.5em;\" >");
	output.append_string("\n<p style=\"color:black;font-size:1.5em;\" id=\"");
	output.append(menuItem.Item);
	output.append_string("\" >");
	output.append(menuItem.Text);
	output.append_string("</p>");
	output.append_string("\n</td>");
	output.append_string("\n</tr>");

	output.append_string("\n</table>");
	return 0;
}

::gpk::error_t									ntl::htmlControlMenuIconsHorizontal
	( const ::gpk::view_array<const ::ntl::SHTMLIcon>	& menuItems
	, const ::gpk::view_const_char						& pathImages
	, const ::gpk::view_const_char						& extensionImages
	, ::gpk::array_pod<char_t>							& output
	, bool												iconsLarge
	) {
	output.append_string("\n<table style=\"width:100%;height:100%;\" >");	// 181830
	output.append_string("\n<tr style=\"height:100%; \">");
	char													iconWidth	[32]		= {};
	const double											iconWidthPercent		= 100 / menuItems.size();
	sprintf_s(iconWidth, "%f%%", iconWidthPercent);
	for(uint32_t iItem = 0; iItem < menuItems.size(); ++iItem) {
		const ::ntl::SHTMLIcon									currentItem				= menuItems[iItem];
		output.append_string("\n<td style=\"height:100%;width:");
		output.append(::gpk::view_const_string{iconWidth});
		output.append_string("\" >");

		::ntl::htmlControlMenuIcon(pathImages, extensionImages, currentItem, output, iconsLarge);	// ------ Unknown icon
		output.append_string("\n</td>");
	}
	output.append_string("\n</tr>");
	output.append_string("\n</table>");
	return 0;
}


::gpk::error_t									ntl::htmlTag
	( const ::gpk::view_const_string	& tagName
	, const ::gpk::view_const_char		& content
	, const ::gpk::view_const_char		& attributes
	, ::gpk::array_pod<char_t>			& output
	) {
	gpk_necall(output.push_back('<')	, "%s", "Out of memory?");
	gpk_necall(output.append(tagName)	, "%s", "Out of memory?");
	if(attributes.size()) {
		gpk_necall(output.push_back(' '), "%s", "Out of memory?");
		gpk_necall(output.append(attributes), "%s", "Out of memory?");
		gpk_necall(output.push_back(' '), "%s", "Out of memory?");
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
	, const ::gpk::view_const_char		& attributes
	, ::gpk::array_pod<char_t>			& output
	) {
	gpk_necall(output.push_back('<')	, "%s", "Out of memory?");
	gpk_necall(output.append(tagName)	, "%s", "Out of memory?");
	if(attributes.size()) {
		gpk_necall(output.push_back(' '), "%s", "Out of memory?");
		gpk_necall(output.append(attributes), "%s", "Out of memory?");
		gpk_necall(output.push_back(' '), "%s", "Out of memory?");
	}
	gpk_necall(output.push_back('/'), "%s", "Out of memory?");
	gpk_necall(output.push_back('>'), "%s", "Out of memory?");
	return 0;
}

static	::gpk::error_t							loadPath						(::gpk::view_const_char rootPart, const ::gpk::view_const_string & expression, ::gpk::SJSONReader & reader, int32_t indexRoot, ::gpk::array_pod<char> & path, ::gpk::view_const_char & output)	{
	::gpk::view_const_char							pathPart;
	path											= rootPart;
	::gpk::jsonExpressionResolve(expression, reader, indexRoot, pathPart);
	path.append(pathPart);
	output											= ::gpk::label(path.begin(), path.size());
	return 0;
}

::gpk::error_t									ntl::frontConfigLoad					(::ntl::SHTMLEndpoint & programState, int32_t indexRoot)	{
	if(-1 == indexRoot) {
		::gpk::view_const_char							rootNode;
		indexRoot									= ::gpk::jsonExpressionResolve("neutralizer", programState.Config.Reader, 0, rootNode);
	}
	{
		::gpk::view_const_char							rootPart;
		::gpk::jsonExpressionResolve("front.http.path.root", programState.Config.Reader, indexRoot, rootPart);

		::gpk::array_pod<char>								path;
		::loadPath(rootPart, "front.http.path.image"	, programState.Config.Reader, indexRoot, path, programState.Path.Image	);
		::loadPath(rootPart, "front.http.path.css"		, programState.Config.Reader, indexRoot, path, programState.Path.Style	);
		::loadPath(rootPart, "front.http.path.js"		, programState.Config.Reader, indexRoot, path, programState.Path.Script	);
		::loadPath(rootPart, "front.http.path.exe"		, programState.Config.Reader, indexRoot, path, programState.Path.Program);
	}
	::gpk::jsonExpressionResolve("front.title"				, programState.Config.Reader, indexRoot, programState.Page.Title		);
	::gpk::jsonExpressionResolve("front.extension.image"	, programState.Config.Reader, indexRoot, programState.Extension.Image	);
	::gpk::jsonExpressionResolve("front.extension.program"	, programState.Config.Reader, indexRoot, programState.Extension.Program	);
	return 0;
}
