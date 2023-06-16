#include "ntl_front.h"

#include "gpk_base64.h"

#include "gpk_json_expression.h"
#include "gpk_label.h"

::gpk::error_t			ntl::loadNTLArgs					(SNTLArgs & out_loaded, const ::gpk::view_array<const ::gpk::TKeyValConstString> & queryStringKeyVals)	{
	::gpk::find(::gpk::vcs{"s"}, queryStringKeyVals, out_loaded.Session);
	::gpk::find(::gpk::vcs{"l"}, queryStringKeyVals, out_loaded.Language);
	::gpk::find(::gpk::vcs{"m"}, queryStringKeyVals, out_loaded.Module);
	::gpk::find(::gpk::vcs{"w"}, queryStringKeyVals, out_loaded.Width);
	::gpk::find(::gpk::vcs{"h"}, queryStringKeyVals, out_loaded.Height);
	return 0;
}

::gpk::error_t			ntl::httpPath						(::gpk::vcs folder, ::gpk::vcs name, ::gpk::vcs extension, ::gpk::apod<char> & output) {
	output.append(folder);
	output.push_back('/');
	output.append(name);
	output.push_back('.');
	output.append(extension);
	return 0;
}

::gpk::error_t			ntl::htmlMenuIcon
	(	const ::gpk::vcc	& pathImages
	,	const ::gpk::vcc	& extension
	,	const ::gpk::vcc	& itemName
	,	const ::gpk::vcc	& txtIcon
	,	::gpk::apod<char>	& output
	, bool					iconLarge
	) {
	char						events		[4096]	= {};
	::gpk::apod<char>			iconId				= {};
	::gpk::apod<char>			idBase64			= {};
	{ // Generate id for this icon
		iconId					= ::gpk::vcs{"id_icon_"};
		iconId.append(txtIcon);
		::gpk::base64EncodeFS(::gpk::vcc{iconId}, idBase64);
	}
	output.append_string("<table style=\"width: 100%; height: 100%;text-align:center;\" ");
	output.append_string(" id=\"");
	output.append(idBase64);
	output.append_string("\" ");
	idBase64.push_back(0);
	sprintf_s(events, "onmouseout=\"cellColor('%s', null, 0);\" onmouseover=\"cellColor('%s', '#ec8106', 0);\" ", idBase64.begin(), idBase64.begin());
	output.append(::gpk::vcs{events});
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
	, const ::gpk::vcc						& pathImages
	, const ::gpk::vcc						& extensionImages
	, ::gpk::apod<char>							& output
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
		output.append(::gpk::vcs{iconWidth});
		output.append_string("\" >");

		::ntl::htmlMenuIcon(pathImages, extensionImages, currentItem.Item, currentItem.Text, output, iconsLarge);	// ------ Unknown icon
		output.append_string("</td>");
	}
	output.append_string("</tr>");
	output.append_string("</table>");
	return 0;
}


::gpk::error_t										ntl::htmlControlMenuIcon
	(	const ::gpk::vcc	& pathImages
	,	const ::gpk::vcc	& extension
	,	const ::ntl::SHTMLIcon			& menuItem
	,	::gpk::apod<char>		& output
	, bool								iconLarge
	) {
	char													events		[4096]		= {};
	::gpk::apod<char>								iconId					= {};
	::gpk::apod<char>								idBase64				= {};
	{ // Generate id for this icon
		iconId											= ::gpk::vcs{ "id_icon_"};
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
	, const ::gpk::vcc						& pathImages
	, const ::gpk::vcc						& extensionImages
	, ::gpk::apod<char>							& output
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
		output.append(::gpk::vcs{iconWidth});
		output.append_string("\" >");

		::ntl::htmlControlMenuIcon(pathImages, extensionImages, currentItem, output, iconsLarge);	// ------ Unknown icon
		output.append_string("\n</td>");
	}
	output.append_string("\n</tr>");
	output.append_string("\n</table>");
	return 0;
}


::gpk::error_t									ntl::htmlTag
	( const ::gpk::vcs	& tagName
	, const ::gpk::vcc		& content
	, const ::gpk::vcc		& attributes
	, ::gpk::apod<char>			& output
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
	( const ::gpk::vcs	& tagName
	, const ::gpk::vcc		& attributes
	, ::gpk::apod<char>			& output
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

static	::gpk::error_t							loadPath						(::gpk::vcc rootPart, const ::gpk::vcs & expression, ::gpk::SJSONReader & reader, int32_t indexRoot, ::gpk::array_pod<char> & path, ::gpk::vcc & output)	{
	::gpk::vcc							pathPart;
	path											= rootPart;
	::gpk::jsonExpressionResolve(expression, reader, indexRoot, pathPart);
	path.append(pathPart);
	const ::gpk::label safePath = {path.begin(), path.size()};
	output											= safePath;
	return 0;
}

::gpk::error_t									ntl::frontConfigLoad					(::ntl::SHTMLEndpoint & programState, int32_t indexRoot)	{
	if(-1 == indexRoot) {
		::gpk::vcc							rootNode;
		indexRoot									= ::gpk::jsonExpressionResolve(::gpk::vcs{"neutralizer"}, programState.Config.Reader, 0, rootNode);
	}
	{
		::gpk::vcc							rootPart;
		::gpk::jsonExpressionResolve(::gpk::vcs{"front.http.path.root"}, programState.Config.Reader, indexRoot, rootPart);

		::gpk::array_pod<char>								path;
		::loadPath(rootPart, ::gpk::vcs{"front.http.path.image"	}, programState.Config.Reader, indexRoot, path, programState.Path.Image		);
		::loadPath(rootPart, ::gpk::vcs{"front.http.path.css"	}, programState.Config.Reader, indexRoot, path, programState.Path.Style		);
		::loadPath(rootPart, ::gpk::vcs{"front.http.path.js"	}, programState.Config.Reader, indexRoot, path, programState.Path.Script	);
		::loadPath(rootPart, ::gpk::vcs{"front.http.path.exe"	}, programState.Config.Reader, indexRoot, path, programState.Path.Program	);
	}
	::gpk::jsonExpressionResolve(::gpk::vcs{"front.title"				}, programState.Config.Reader, indexRoot, programState.Page.Title		);
	::gpk::jsonExpressionResolve(::gpk::vcs{"front.extension.image"		}, programState.Config.Reader, indexRoot, programState.Extension.Image	);
	::gpk::jsonExpressionResolve(::gpk::vcs{"front.extension.program"	}, programState.Config.Reader, indexRoot, programState.Extension.Program	);
	return 0;
}
