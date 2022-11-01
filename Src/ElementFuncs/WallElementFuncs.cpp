#include "WallElementFuncs.hpp"

void WallElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
	MP.AddVariable("Thickness", element.wall.thickness);

	GS::Array<API_CompositeQuantity> composites;
	API_ElementQuantity quantity;
	API_QuantitiesMask  mask;

	ACAPI_ELEMENT_QUANTITY_MASK_CLEAR(mask);
	ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, wall, perimeter);
	ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, wall, length);
	ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, wall, refLineLength);
	ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, wall, maxHeight);
	ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, wall, minHeight);
	ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, wall, area);
	ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, wall, volume);
	ACAPI_ELEMENT_COMPOSITES_QUANTITY_MASK_SETFULL(mask);

	if (GetQuantities(element, mask, quantity, composites)) {
		MP.AddVariable("Perimeter", quantity.wall.perimeter);
		MP.AddVariable("Length", quantity.wall.length);
		MP.AddVariable("RefLength", quantity.wall.refLineLength);
		MP.AddVariable("MaxHeight", quantity.wall.maxHeight);
		MP.AddVariable("MinHeight", quantity.wall.minHeight);
		MP.AddVariable("Area", quantity.wall.area);
		MP.AddVariable("Volume", quantity.wall.volume);

		double coreVolume = quantity.wall.volume;
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
