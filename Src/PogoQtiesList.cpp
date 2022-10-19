#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

#include    "HTTP\Client\ClientConnection.hpp"
#include	"GSXMLDOMBuilder.hpp"
#include	"GSXMLDOMReader.hpp"
#include	"xercesc/dom/DOM.hpp"
#include	"xercesc/util/XMLString.hpp"

#include	"PogoQty.hpp"
#include	"PogoQtiesList.hpp"
#include	"PogoHttpClient.hpp"
#include	"ShowMessage.hpp"

bool PogoQtiesList::FetchByItem(const GS::UniString ItemId)
{
	return Fetch("/qties/index/" + ItemId + ".xml");
}

bool PogoQtiesList::Fetch(const GS::UniString url)
{
	GS::UniString XMLData;
	bool success = HttpRequest(HTTP::MessageHeader::Method::Get, url, "", XMLData);

	if (!success || XMLData.IsEmpty()) {
		ShowMessage("Cannot fetch Qties.");
		return false;
	}

	if (ParseXml(XMLData)) {
		return true;
	}
	else {
		ShowMessage("Cannot parse Qties XML.");
		return false;
	}
}

bool PogoQtiesList::ParseXml(GS::UniString XML)
{
	Clear();
	try {
		const char* xmlData = XML.ToCStr().Get();
		GSXML::DOMReader xmlReader(reinterpret_cast<const unsigned char*>(xmlData), (GS::USize)strlen(xmlData), "qties");

		xercesc::DOMElement* root = xmlReader.GetActualNode();

		GS::UniString qtyNodeName("qty");
		xercesc::DOMNodeList* qties = root->getElementsByTagName(qtyNodeName.ToUStr().Get());

		for (XMLSize_t i = 0; i < qties->getLength(); ++i)
		{
			xercesc::DOMElement* qtyNode = dynamic_cast<xercesc::DOMElement*>(qties->item(i));

			PogoQty qty;

			for (short j = 0; j < sizeof(fieldList); j++) {
				xercesc::DOMNodeList* aNode = qtyNode->getElementsByTagName(GS::UniString(fieldList[j].name).ToUStr().Get());
				if (aNode->getLength() > 0) {
					switch (fieldList[j].type) {
						case DbFT_String:
							qty.SetValue(fieldList[j].name, GS::UniString(aNode->item(0)->getTextContent()));
							break;
						case DbFT_Int:
							qty.SetValue(fieldList[j].name, atoi(xercesc::XMLString::transcode(aNode->item(0)->getTextContent())));
							break;
						case DbFT_Double:
							qty.SetValue(fieldList[j].name, atof(xercesc::XMLString::transcode(aNode->item(0)->getTextContent())));
							break;
					}
				}
			}

			Push(qty);

		}
	}
	catch (...) {
		return false;
	}

	return true;
}