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
	ACAPI_ELEMENT_COMPOSITES_QUANTITY_MASK_SETFULL(mask);

	if (GetQuantities(element, mask, quantity, composites)) {
		MP.AddVariable("Perimeter", quantity.roof.perimeter);
		MP.AddVariable("ProjectionArea", quantity.roof.contourArea);
		MP.AddVariable("Area", quantity.roof.topSurface);
		MP.AddVariable("Volume", quantity.roof.volume);

		double coreVolume = quantity.roof.volume;
		if (composites.GetSize() > 0) {
			coreVolume = 0;
		}
		for (UInt32 i = 0; i < composites.GetSize(); i++) {
			if ((composites[i].flags & APISkin_Core) != 0) {
				coreVolume += composites[i].volumes;
			}
		}
		MP.AddVariable("CoreVolume", coreVolume);
	}

}
