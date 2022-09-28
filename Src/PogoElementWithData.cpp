#include    "HTTP\Client\ClientConnection.hpp"

#include	"GSXMLDOMBuilder.hpp"
#include	"GSXMLDOMReader.hpp"
#include	"xercesc/dom/DOM.hpp"
#include	"CMathParser.h"
#include "PogoElementWithData.hpp"
#include "PogoHttpClient.hpp"

GS::UniString PogoElementWithData::RESTCreateQty(const PogoItem& item, const GS::UniString& descript, const double& value)
{
	using namespace HTTP::MessageHeader;

	PogoSettings pogoSettings;
	PogoSettings::LoadPogoSettingsFromPreferences(pogoSettings);

	GS::UniString host = pogoSettings.Host;
	GS::UniString url = "/qties/add.xml";
	GS::UniString username = pogoSettings.Username;
	GS::UniString password = pogoSettings.Password;

	GS::UniString postData = "";
	postData.Append(GS::UniString::Printf("id=%s", ""));
	postData.Append("&");
	postData.Append(GS::UniString::Printf("item_id=%s", item.id.ToCStr().Get()));
	postData.Append("&");
	postData.Append(GS::UniString::Printf("guid=%s", APIGuid2GSGuid(this->guid).ToUniString().ToCStr().Get()));
	postData.Append("&");
	postData.Append(GS::UniString::Printf("descript=%s", descript.ToCStr().Get()));
	postData.Append("&");
	postData.Append(GS::UniString::Printf("qty_value_native=%.2f", value));

	// do request
	GS::UniString XMLData;
	bool success = HttpRequest(Method::Post, host, url, username, password, postData, XMLData);
	if (!success) {
		return "";
	}

	return XMLExtractQtyId(XMLData);
}

GS::UniString PogoElementWithData::XMLExtractQtyId(GS::UniString& XML)
{
	try {
		const char* xmlData = XML.ToCStr(CC_UTF8).Get();
		GSXML::DOMReader xmlReader(reinterpret_cast<const unsigned char*>(xmlData), (GS::USize)strlen(xmlData), "response");

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
		const std::string ff(formula.ToCStr(0, MaxUSize, GSCharCode::CC_Legacy));

		CMathParser MP;
		double res(0);

		MP.DebugMode(false);
		MP.SetVariableSetCallback(nullptr);

		switch (element.header.typeID) {
			case API_ObjectID: {
				MP.AddVariable("A", element.object.xRatio);
				MP.AddVariable("B", element.object.yRatio);
				break;
			}
			case API_SlabID: {
				MP.AddVariable("Thickness", element.slab.thickness);

				GS::Array<API_CompositeQuantity> composites;		
				API_ElementQuantity quantity;
				API_QuantitiesMask  mask;

				ACAPI_ELEMENT_QUANTITY_MASK_CLEAR(mask);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, slab, perimeter);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, slab, topSurface);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, slab, volume);
				ACAPI_ELEMENT_COMPOSITES_QUANTITY_MASK_SETFULL(mask);

				
				if (GetQuantities(element, mask, quantity, composites)) {
					MP.AddVariable("Perimeter", quantity.slab.perimeter);
					MP.AddVariable("Area", quantity.slab.topSurface);
					MP.AddVariable("Volume", quantity.slab.volume);

					double coreVolume = 0;
					for (UInt32 i = 0; i < composites.GetSize(); i++) {
						if ((composites[i].flags & APISkin_Core) != 0) {
							coreVolume += composites[i].volumes;
						}
					}
					MP.AddVariable("CoreVolume", coreVolume);
				}

				break;
			}
			case API_ColumnID: {
				GS::Array<API_CompositeQuantity> composites;
				API_ElementQuantity quantity;
				API_QuantitiesMask  mask;

				ACAPI_ELEMENT_QUANTITY_MASK_CLEAR(mask);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, column, coreBotSurf);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, column, coreVolume);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, column, maxHeight);

				if (GetQuantities(element, mask, quantity, composites)) {
					MP.AddVariable("CoreArea", quantity.column.coreBotSurf);
					MP.AddVariable("CoreVolume", quantity.column.coreVolume);
					MP.AddVariable("Height", quantity.column.maxHeight);
				}

				break;
			}
			case API_MorphID: {
				GS::Array<API_CompositeQuantity> composites;
				API_ElementQuantity quantity;
				API_QuantitiesMask  mask;

				ACAPI_ELEMENT_QUANTITY_MASK_CLEAR(mask);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, morph, volume);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, morph, surface);

				if (GetQuantities(element, mask, quantity, composites)) {
					MP.AddVariable("Volume", quantity.morph.volume);
					MP.AddVariable("Area", quantity.morph.surface);
				}

				break;
			}
			case API_HatchID: {
				GS::Array<API_CompositeQuantity> composites;
				API_ElementQuantity quantity;
				API_QuantitiesMask  mask;

				ACAPI_ELEMENT_QUANTITY_MASK_CLEAR(mask);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, hatch, perimeter);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, hatch, surface);

				if (GetQuantities(element, mask, quantity, composites)) {
					MP.AddVariable("Perimeter", quantity.hatch.perimeter);
					MP.AddVariable("Area", quantity.hatch.surface);
				}

				break;
			}
			case API_RoofID: {
				GS::Array<API_CompositeQuantity> composites;
				API_ElementQuantity quantity;
				API_QuantitiesMask  mask;

				ACAPI_ELEMENT_QUANTITY_MASK_CLEAR(mask);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, roof, topSurface);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, roof, volume);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, roof, perimeter);
				ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, roof, contourArea);

				if (GetQuantities(element, mask, quantity, composites)) {
					MP.AddVariable("Perimeter", quantity.roof.perimeter);
					MP.AddVariable("Area", quantity.roof.contourArea);
					MP.AddVariable("TopArea", quantity.roof.topSurface);
					MP.AddVariable("Volume", quantity.roof.volume);
				}

				break;
			}
		}

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

/// <summary>
/// Store qties list back to ARCHICAD Element
/// </summary>
/// <returns></returns>
bool PogoElementWithData::UpdateQties()
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

bool PogoElementWithData::GetQuantities(const API_Element& element, const API_QuantitiesMask& mask, API_ElementQuantity& quantity,
	GS::Array<API_CompositeQuantity>& composites
)
{
	GSErrCode           err;
	API_Quantities      quantities;

	quantities.elements = &quantity;
	quantities.composites = &composites;

	BNZeroMemory(&quantity, sizeof(API_ElementQuantity));

	err = ACAPI_Element_GetQuantities(element.header.guid, nullptr, &quantities, &mask);

	return err == NoError;
}


bool PogoElementWithData::GetQuantities(const API_Element& element, const API_QuantitiesMask& mask, API_ElementQuantity& quantity,
		GS::Array<API_CompositeQuantity>& composites,
		GS::Array<API_ElemPartQuantity>& elemPartQuantities,
		GS::Array <API_ElemPartCompositeQuantity>&	elemPartComposites
)
{
	GSErrCode           err;
	API_Quantities      quantities;

	quantities.elements = &quantity;
	quantities.composites = &composites;
	quantities.elemPartQuantities = &elemPartQuantities;
	quantities.elemPartComposites = &elemPartComposites;

	BNZeroMemory(&quantity, sizeof(API_ElementQuantity));

	err = ACAPI_Element_GetQuantities(element.header.guid, nullptr, &quantities, &mask);

	return err == NoError;
}
