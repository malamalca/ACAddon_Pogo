#include    "HTTP\Client\ClientConnection.hpp"

#include	"GSXMLDOMBuilder.hpp"
#include	"GSXMLDOMReader.hpp"
#include	"xercesc/dom/DOM.hpp"
#include	"CMathParser.h"
#include	"PogoElementWithData.hpp"
#include	"PogoHttpClient.hpp"
#include	"ElementFuncs/ElementFuncsFactory.hpp"

GS::UniString PogoElementWithData::RESTCreateQty(const PogoItem& item, const GS::UniString& formula, const GS::UniString& descript, const double& value)
{
	using namespace HTTP::MessageHeader;

	GS::UniString url = "/qties/add.xml";
	GS::UniString postData = "";

	postData.Append(GS::UniString::Printf("id=%s", ""));
	postData.Append("&");
	postData.Append(GS::UniString::Printf("item_id=%s", item.id.ToCStr().Get()));
	postData.Append("&");
	postData.Append(GS::UniString::Printf("guid=%s", APIGuid2GSGuid(this->guid).ToUniString().ToCStr().Get()));
	postData.Append("&");
	postData.Append(GS::UniString::Printf("formula=%s", formula.ToCStr().Get()));
	postData.Append("&");
	postData.Append(GS::UniString::Printf("descript=%s", descript.ToCStr().Get()));
	postData.Append("&");
	postData.Append(GS::UniString::Printf("qty_value_native=%.2f", value));

	// do request
	char* XMLData;
	bool success = HttpRequest(Method::Post, url, postData, XMLData);
	if (!success) {
		return "";
	}

	return XMLExtractQtyId(XMLData);
}

GS::UniString PogoElementWithData::XMLExtractQtyId(const char* XML)
{
	try {
		GSXML::DOMReader xmlReader(reinterpret_cast<const unsigned char*>(XML), (GS::USize)strlen(XML), "response");

		xercesc::DOMElement* root = xmlReader.GetActualNode();

		GS::UniString qtyNodeName("qty");
		GS::UniString errorsNodeName("errors");
		GS::UniString idNodeName("id");

		xercesc::DOMNodeList* errorsNode = root->getElementsByTagName(errorsNodeName.ToUStr().Get());
		if (errorsNode->getLength() > 0) {
			xercesc::DOMElement* errorsElement = dynamic_cast<xercesc::DOMElement*>(errorsNode->item(0));
			GS::UniString errors = errorsElement->getTextContent();
			if (errors == "false") {
				xercesc::DOMNodeList* qtyNode = root->getElementsByTagName(qtyNodeName.ToUStr().Get());
				if (qtyNode->getLength() > 0) {
					xercesc::DOMElement* qtyElement = dynamic_cast<xercesc::DOMElement*>(qtyNode->item(0));
					xercesc::DOMNodeList* qtyIdNode = qtyElement->getElementsByTagName(idNodeName.ToUStr().Get());
					if (qtyIdNode->getLength() > 0) {
						return qtyIdNode->item(0)->getTextContent();
					}
				}
			}
		}
	}
	catch (...) {
		return "";
	}

	return "";
}

bool PogoElementWithData::ParseFormula(const GS::UniString& formula, double& result)
{
	// get element
	API_Element element;
	BNZeroMemory(&element, sizeof(API_Element));

	element.header.guid = this->guid;

	if (ACAPI_Element_Get(&element) == NoError) {
		CMathParser MP;
		double res(0);

		MP.DebugMode(false);
		MP.SetVariableSetCallback(nullptr);

		(ElementFuncsFactory::Get())->SetVariables(element, MP);

		if (MP.Calculate(formula.ToCStr(0, MaxUSize, GSCharCode::CC_Legacy).Get(), &result) == CMathParser::ResultOk) {
			return true;
		}
	}

	return false;
}

double PogoElementWithData::ParseFormula(const GS::UniString& formula)
{
	double formulaResult;
	if (this->ParseFormula(formula, formulaResult)) {
		return formulaResult;
	}
	return 0.0;
}

bool PogoElementWithData::ElementExists()
{
	// get element
	API_Elem_Head head;
	BNZeroMemory(&head, sizeof(API_Elem_Head));
	head.guid = guid;

	return ACAPI_Element_GetHeader(&head) == NoError;
}

/// <summary>
/// Store qties list back to ARCHICAD Element
/// </summary>
/// <returns></returns>
bool PogoElementWithData::UpdateQtiesToElement()
{
	// get element
	API_Elem_Head head;
	BNZeroMemory(&head, sizeof(API_Elem_Head));
	head.guid = guid;

	GSErr err;

	err = ACAPI_Element_DeleteUserData(&head);
	if (err == NoError) {
		if (qties->count > 0) {
			PogoLinkedQties linkedQties;
			BNZeroMemory(&linkedQties, sizeof(PogoLinkedQties));
			BNCopyMemory(&linkedQties, qties, sizeof(PogoLinkedQties));

			API_ElementUserData userData;
			BNZeroMemory(&userData, sizeof(API_ElementUserData));

			userData.dataVersion = 1;
			userData.platformSign = GS::Act_Platform_Sign;
			userData.flags = APIUserDataFlag_FillWith | APIUserDataFlag_Pickup;
			userData.dataHdl = BMAllocateHandle(sizeof(linkedQties), ALLOCATE_CLEAR, 0);
			*reinterpret_cast<PogoLinkedQties*> (*userData.dataHdl) = linkedQties;

			ACAPI_Element_SetUserData(&head, &userData);
		}

		return true;
	}

	return false;
}

bool PogoElementWithData::LoadQtiesFromElement()
{
	qties = nullptr;

	API_ElementUserData userData;
	BNZeroMemory(&userData, sizeof(API_ElementUserData));

	API_Elem_Head head;
	BNZeroMemory(&head, sizeof(API_Elem_Head));
	head.guid = guid;

	bool hasData = false;
	if (ACAPI_Element_GetUserData(&head, &userData) == NoError) {
		if (userData.dataHdl != nullptr && userData.platformSign == GS::Act_Platform_Sign) {
			qties = new PogoLinkedQties();
			BNZeroMemory(qties, sizeof(PogoLinkedQties));
			BNCopyMemory(qties, *(userData.dataHdl), sizeof(PogoLinkedQties));
			BMKillHandle(&userData.dataHdl);

			hasData = true;
		}
	}

	return hasData;
}

bool PogoElementWithData::HasQty(const GS::UniString& id)
{
	if (!qties) {
		return false;
	}
	for (short i = 0; i < qties->count; i++) {
		if (qties->qtyData[i].qty_id == id) {
			return true;
		}
	}
	return false;
}

bool   PogoElementWithData::AddQty(const PogoQtyData qtyData)
{
	if (!qties) {
		qties = new PogoLinkedQties();
		qties->count = 0;
	}

	if (qties->count < 10) {
		qties->count++;
		qties->qtyData[qties->count - 1] = qtyData;

		return true;
	}
	else {
		return false;
	}
}

bool PogoElementWithData::UpdateQty(const PogoQtyData qtyData)
{
	if (!qties) {
		return false;
	}
	for (short i = 0; i < qties->count; i++) {
		if (strcmp(qties->qtyData[i].qty_id, qtyData.qty_id)==0) {
			qties->qtyData[i] = qtyData;
			return true;
		}
	}

	return false;
}

