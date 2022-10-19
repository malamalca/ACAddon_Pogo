#include	"PogoItemsList.hpp"
#include	"PogoTypes.h"
#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

#include    "HTTP\Client\ClientConnection.hpp"
#include    "HTTP\Client\Authentication\BasicAuthentication.hpp"
#include	"IBinaryChannelUtilities.hpp"
#include	"IOBinProtocolXs.hpp"
#include	"IChannelX.hpp"
#include	"URIFormat.hpp"

#include	"GSXMLDOMBuilder.hpp"
#include	"GSXMLDOMReader.hpp"
#include	"xercesc/dom/DOM.hpp"
#include	"xercesc/util/XMLString.hpp"

#include	"PogoHttpClient.hpp"
#include	"ShowMessage.hpp"

PogoItemsList::PogoItemsList()
{
}

PogoItemsList::~PogoItemsList()
{
}

bool PogoItemsList::FetchBySection(const GS::UniString SectionId)
{
	return Fetch("/items/index.xml?section=" + SectionId);
}

bool PogoItemsList::FetchByQty(const GS::UniString QtyId)
{
	return Fetch("/items/index.xml?qty=" + QtyId);
}

bool PogoItemsList::Fetch(const GS::UniString url)
{
	GS::UniString XMLData;
	bool success = HttpRequest(HTTP::MessageHeader::Method::Get, url, "", XMLData);

	if (!success || XMLData.IsEmpty()) {
		ShowMessage("Cannot fetch Items.");
		return false;
	}

	if (ParseXml(XMLData)) {
		return true;
	}
	else {
		ShowMessage("Cannot parse Items XML.");
		return false;
	}
}

bool PogoItemsList::ParseXml(GS::UniString XML)
{
	Clear();
	try {
		const char* xmlData = XML.ToCStr(CC_UTF8).Get();
		GSXML::DOMReader xmlReader(reinterpret_cast<const unsigned char*>(xmlData), (GS::USize)strlen(xmlData), "items");

		xercesc::DOMElement* root = xmlReader.GetActualNode();

		GS::UniString itemNodeName("item");
		GS::UniString idNodeName("id");
		GS::UniString descriptNodeName("descript");
		GS::UniString unitNodeName("unit");
		GS::UniString qtyNodeName("qty");
		GS::UniString priceNodeName("price");

		xercesc::DOMNodeList* items = root->getElementsByTagName(itemNodeName.ToUStr().Get());
		for (XMLSize_t i = 0; i < items->getLength(); ++i)
		{
			xercesc::DOMElement* itemNode = dynamic_cast<xercesc::DOMElement*>(items->item(i));

			PogoItem item;

			xercesc::DOMNodeList* idNode = itemNode->getElementsByTagName((const XMLCh*)idNodeName.ToUStr().Get());
			if (idNode->getLength() > 0) {
				item.id = idNode->item(0)->getTextContent();
			}

			xercesc::DOMNodeList* descriptNode = itemNode->getElementsByTagName((const XMLCh*)descriptNodeName.ToUStr().Get());
			if (idNode->getLength() > 0) {
				item.descript = descriptNode->item(0)->getTextContent();
			}

			xercesc::DOMNodeList* unitNode = itemNode->getElementsByTagName((const XMLCh*)unitNodeName.ToUStr().Get());
			if (unitNode->getLength() > 0) {
				item.unit = unitNode->item(0)->getTextContent();
			}

			xercesc::DOMNodeList* qtyNode = itemNode->getElementsByTagName((const XMLCh*)qtyNodeName.ToUStr().Get());
			if (qtyNode->getLength() > 0) {
				item.qty = atof(xercesc::XMLString::transcode(qtyNode->item(0)->getTextContent()));
			}

			xercesc::DOMNodeList* priceNode = itemNode->getElementsByTagName((const XMLCh*)priceNodeName.ToUStr().Get());
			if (priceNode->getLength() > 0) {
				item.price = atof(xercesc::XMLString::transcode(priceNode->item(0)->getTextContent()));
			}

			Push(item);
		}
	}
	catch (...) {
		return false;
	}

	return true;
}