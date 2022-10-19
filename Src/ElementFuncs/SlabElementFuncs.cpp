#include "SlabElementFuncs.hpp"

void SlabElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
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

		double coreVolume = quantity.slab.volume;
		for (UInt32 i = 0; i < composites.GetSize(); i++) {
			if ((composites[i].flags & APISkin_Core) != 0) {
				coreVolume += composites[i].volumes;
			}
		}
		MP.AddVariable("CoreVolume", coreVolume);
	}
}
