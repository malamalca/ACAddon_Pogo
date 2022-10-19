#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

#include    "HTTP\Client\ClientConnection.hpp"
#include	"GSXMLDOMBuilder.hpp"
#include	"GSXMLDOMReader.hpp"
#include	"xercesc/dom/DOM.hpp"

#include	"PogoCategoriesList.hpp"
#include	"PogoTypes.h"
#include	"PogoHttpClient.hpp"
#include	"ShowMessage.hpp"
#include	"PogoSettings.hpp"

PogoCategoriesList::PogoCategoriesList()
{
	PogoSettings::LoadPogoSettingsFromPreferences(pogoSettings);
}

PogoCategoriesList::~PogoCategoriesList()
{
}


bool PogoCategoriesList::Fetch()
{
	using namespace HTTP::MessageHeader;
	GS::UniString url = "/categories/index/" + pogoSettings.ProjectId + ".xml";

	/*GS::UniString host = "http://192.168.88.20/";
	GS::UniString url = "/pogo/categories/index/83e37347-85e3-488b-a028-1f79d7006457.xml";
	GS::UniString username = "miha.nahtigal";
	GS::UniString password = "miha3869";*/

	GS::UniString XMLData;
	bool success = HttpRequest(Method::Get, url, "", XMLData);

	if (!success || XMLData.IsEmpty()) {
		ShowMessage("Cannot fetch Categories.");
		return false;
	}

	if (ParseXml(XMLData)) {
		return true;
	}
	else {
		ShowMessage("Cannot parse Categories XML.");
		return false;
	}
}

bool PogoCategoriesList::ParseXml(GS::UniString XML)
{
	Clear();
	try {
		const char* xmlData = XML.ToCStr(CC_UTF8).Get();
		GSXML::DOMReader xmlReader(reinterpret_cast<const unsigned char*>(xmlData), (GS::USize)strlen(xmlData), "categories");

		xercesc::DOMElement* root = xmlReader.GetActualNode();

		GS::UniString categoryNodeName("category");
		GS::UniString idNodeName("id");
		GS::UniString titleNodeName("title");
		GS::UniString sectionsNodeName("sections");
		GS::UniString sectionNodeName("section");

		xercesc::DOMNodeList* categories = root->getElementsByTagName(categoryNodeName.ToUStr().Get());
		for (XMLSize_t i = 0; i < categories->getLength(); ++i)
		{
			xercesc::DOMElement* categoryNode = dynamic_cast<xercesc::DOMElement*>(categories->item(i));

			PogoCategory category;

			xercesc::DOMNodeList* idNode = categoryNode->getElementsByTagName((const XMLCh*)idNodeName.ToUStr().Get());
			if (idNode->getLength() > 0) {
				category.id = idNode->item(0)->getTextContent();
			}

			xercesc::DOMNodeList* titleNode = categoryNode->getElementsByTagName((const XMLCh*)titleNodeName.ToUStr().Get());
			if (titleNode->getLength() > 0) {
				category.title = titleNode->item(0)->getTextContent();
			}

			// Parse sections for category
			
			xercesc::DOMNodeList* sectionsNode = categoryNode->getElementsByTagName(sectionsNodeName.ToUStr().Get());
			if (sectionsNode->getLength() > 0) {

				xercesc::DOMNodeList* sections = dynamic_cast<xercesc::DOMElement*>(sectionsNode->item(0))->getElementsByTagName(sectionNodeName.ToUStr().Get());

				for (XMLSize_t j = 0; j < sections->getLength(); ++j)
				{
					xercesc::DOMElement* sectionNode = dynamic_cast<xercesc::DOMElement*>(sections->item(j));

					PogoSection section;

					xercesc::DOMNodeList* idNode = sectionNode->getElementsByTagName((const XMLCh*)idNodeName.ToUStr().Get());
					if (idNode->getLength() > 0) {
						section.id = idNode->item(0)->getTextContent();
					}

					xercesc::DOMNodeList* titleNode = sectionNode->getElementsByTagName((const XMLCh*)titleNodeName.ToUStr().Get());
					if (titleNode->getLength() > 0) {
						section.title = titleNode->item(0)->getTextContent();
					}

					category.sections.Push(section);
				}
			}


			Push(category);
		}
	}
	catch (...) {
		return false;
	}

	return true;
}