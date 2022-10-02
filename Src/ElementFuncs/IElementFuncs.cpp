#include "../APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "IElementFuncs.hpp"

bool IElementFuncs::GetQuantities(const API_Element& element, API_QuantitiesMask& mask, API_ElementQuantity& quantity,
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


bool IElementFuncs::GetQuantities(const API_Element& element, API_QuantitiesMask& mask, API_ElementQuantity& quantity,
	GS::Array<API_CompositeQuantity>& composites,
	GS::Array<API_ElemPartQuantity>& elemPartQuantities,
	GS::Array <API_ElemPartCompositeQuantity>& elemPartComposites
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
