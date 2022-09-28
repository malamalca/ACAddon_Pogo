#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

#include    "HTTP\Client\ClientConnection.hpp"
#include	"GSXMLDOMBuilder.hpp"
#include	"GSXMLDOMReader.hpp"
#include	"xercesc/dom/DOM.hpp"

#include	"PogoQtiesList.hpp"
#include	"PogoTypes.h"
#include	"PogoHttpClient.hpp"
#include	"PogoSettings.hpp"
#include	"ShowMessage.hpp"

PogoQtiesList::PogoQtiesList()
{
	PogoSettings::LoadPogoSettingsFromPreferences(pogoSettings);
}

PogoQtiesList::~PogoQtiesList()
{
}


bool PogoQtiesList::RESTSync()
{
	using namespace HTTP::MessageHeader;

	GS::UniString host = pogoSettings.Host;
	GS::UniString url = "/qties/update.xml";
	GS::UniString username = pogoSettings.Username;
	GS::UniString password = pogoSettings.Password;

	GS::UniString postData;

	for (unsigned short i = 0; i < GetSize(); i++) {
		if (!postData.IsEmpty()) {
			postData.Append("&");
		}

		postData.Append(GS::UniString::Printf("%d[id]=%s", i, Get(i).qty_id));
		postData.Append("&");
		postData.Append(GS::UniString::Printf("%d[descript]=%s", i, Get(i).descript));
		postData.Append("&");
		postData.Append(GS::UniString::Printf("%d[qty_value_native]=%.2f", i, Get(i).last_value));
	}

	bool success = HttpRequest(Method::Post, host, url, username, password, postData);

	return success;
}

bool PogoQtiesList::RESTDelete()
{
	using namespace HTTP::MessageHeader;

	GS::UniString host = pogoSettings.Host;
	GS::UniString url = "/qties/delete.xml";
	GS::UniString username = pogoSettings.Username;
	GS::UniString password = pogoSettings.Password;

	GS::UniString postData;

	for (unsigned short i = 0; i < GetSize(); i++) {
		if (!postData.IsEmpty()) {
			postData.Append("&");
		}

		postData.Append(GS::UniString::Printf("%d[id]=%s", i, Get(i).qty_id));
	}

	bool success = HttpRequest(Method::Post, host, url, username, password, postData);

	return success;
}
