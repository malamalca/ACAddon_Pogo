#include "HatchElementFuncs.hpp"

void HatchElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
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
}
