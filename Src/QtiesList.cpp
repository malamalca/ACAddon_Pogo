#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

#include    "HTTP\Client\ClientConnection.hpp"
#include	"GSXMLDOMBuilder.hpp"
#include	"GSXMLDOMReader.hpp"
#include	"xercesc/dom/DOM.hpp"

#include	"QtiesList.hpp"
#include	"PogoQty.hpp"
#include	"PogoTypes.h"
#include	"PogoHttpClient.hpp"
#include	"PogoSettings.hpp"
#include	"ShowMessage.hpp"

QtiesList::QtiesList()
{
}

QtiesList::~QtiesList()
{
}


bool QtiesList::RESTSync()
{
	GS::UniString url = "/qties/update.xml";

	GS::UniString postData;

	for (unsigned short i = 0; i < GetSize(); i++) {
		if (!postData.IsEmpty()) {
			postData.Append("&");
		}

		postData.Append(GS::UniString::Printf("%d[id]=%s", i, Get(i).qty_id));
		postData.Append("&");
		postData.Append(GS::UniString::Printf("%d[descript]=%s", i, Get(i).descript));
		postData.Append("&");
		postData.Append(GS::UniString::Printf("%d[formula]=%s", i, Get(i).formula));
		postData.Append("&");
		postData.Append(GS::UniString::Printf("%d[qty_value_native]=%.2f", i, Get(i).last_value));
	}

	bool success = HttpRequest(HTTP::MessageHeader::Method::Post, url, postData);

	return success;
}

bool QtiesList::RESTDelete()
{
	GS::UniString url = "/qties/delete.xml";
	GS::UniString postData;

	for (unsigned short i = 0; i < GetSize(); i++) {
		if (!postData.IsEmpty()) {
			postData.Append("&");
		}

		postData.Append(GS::UniString::Printf("%d[id]=%s", i, Get(i).qty_id));
	}

	bool success = HttpRequest(HTTP::MessageHeader::Method::Post, url, postData);

	return success;
}