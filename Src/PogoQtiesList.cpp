#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

#include    "HTTP\Client\ClientConnection.hpp"
#include	"GSXMLDOMBuilder.hpp"
#include	"GSXMLDOMReader.hpp"
#include	"xercesc/dom/DOM.hpp"
#include	"xercesc/util/XMLString.hpp"
#include	"xercesc/sax/SAXParseException.hpp"
#include	"xercesc/parsers/XercesDOMParser.hpp"
#include	"xercesc/framework/MemBufInputSource.hpp"

#include	"PogoQty.hpp"
#include	"PogoQtiesList.hpp"
#include	"PogoHttpClient.hpp"
#include	"ShowMessage.hpp"

bool PogoQtiesList::FetchByItem(const GS::UniString ItemId)
{
	return Fetch("/qties/index/" + ItemId + ".xml");
}

bool PogoQtiesList::FetchById(const GS::UniString qtyId)
{
	return Fetch("/qties/view/" + qtyId + ".xml");
}

bool PogoQtiesList::Fetch(const GS::UniString url)
{
	char* XMLData = nullptr;
	bool success = HttpRequest(HTTP::MessageHeader::Method::Get, url, "", XMLData);

	if (!success || strlen(XMLData) == 0) {
		//ShowMessage("Cannot fetch Qties.");
		return false;
	}

	if (ParseXml(XMLData)) {
		XMLData = "";
		return true;
	}
	else {
		//ShowMessage("Cannot parse Qties XML.");
		XMLData = "";
		return false;
	}
}

bool PogoQtiesList::ParseXml(const char* XML)
{
	Clear();
	try {
		GSXML::DOMReader xmlReader(reinterpret_cast<const unsigned char*>(XML), (GS::USize)strlen(XML), "qties");
		xercesc::DOMElement* root = xmlReader.GetActualNode();

		//xercesc::XercesDOMParser domParser;
		//xercesc::MemBufInputSource myxml_buf((const XMLByte*)(XML), strlen(XML), "myxml (in memory)");	
		//domParser.parse(myxml_buf);
		//xercesc::DOMElement* root = domParser.getDocument()->getDocumentElement();		

		if (root) {

			GS::UniString qtyNodeName("qty");
			xercesc::DOMNodeList* qties = root->getElementsByTagName(qtyNodeName.ToUStr().Get());

			for (XMLSize_t i = 0; i < qties->getLength(); ++i)
			{
				xercesc::DOMElement* qtyNode = dynamic_cast<xercesc::DOMElement*>(qties->item(i));

				PogoQty qty;

				for (short j = 0; j < fieldCount; j++) {
					GS::UniString fieldName = GS::UniString(fieldList[j].name);
					xercesc::DOMNodeList* aNode = qtyNode->getElementsByTagName(fieldName.ToUStr().Get());
					if (aNode->getLength() > 0) {
						switch (fieldList[j].type) {
						case DbFT_String:
							qty.SetValue(fieldName, GS::UniString(aNode->item(0)->getTextContent()));
							break;
						case DbFT_Int:
							qty.SetValue(fieldName, atoi(xercesc::XMLString::transcode(aNode->item(0)->getTextContent())));
							break;
						case DbFT_Double:
							qty.SetValue(fieldName, atof(xercesc::XMLString::transcode(aNode->item(0)->getTextContent())));
							break;
						}
					}
				}

				Push(qty);

			}
		}
	}
	catch (xercesc_3_0::SAXParseException ex) {
		DBPrintf(ex.getMessage());
		return false;
	}
	catch (GSXML::DOMReaderException ex2) {
		DBPrintf(ex2.GetMessage().ToCStr().Get());
		return false;
	}
	catch (...) {
		return false;
	}

	return true;
}