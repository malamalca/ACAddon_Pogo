#include "RoofElementFuncs.hpp"

void RoofElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
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
		MP.AddVariable("ProjectionArea", quantity.roof.contourArea);
		MP.AddVariable("Area", quantity.roof.topSurface);
		MP.AddVariable("Volume", quantity.roof.volume);
	}
}
