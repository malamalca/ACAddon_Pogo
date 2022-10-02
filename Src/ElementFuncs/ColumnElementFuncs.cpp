#include "ColumnElementFuncs.hpp"

void ColumnElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
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
}
