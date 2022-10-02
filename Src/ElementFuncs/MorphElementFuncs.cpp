#include "MorphElementFuncs.hpp"

void MorphElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
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
}
