#include "BeamElementFuncs.hpp"

void BeamElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
	GSErrCode		err;
	API_ElementMemo	memo;

	double projectionLength = sqrt(pow(element.beam.begC.x - element.beam.endC.x, 2) +
		pow(element.beam.begC.y - element.beam.endC.y, 2));

	MP.AddVariable("ProjectionLength", projectionLength);

	if (element.beam.isSlanted) {
		projectionLength = projectionLength / cos(element.beam.slantAngle);
	}
	MP.AddVariable("Length", projectionLength);

	err = ACAPI_Element_GetMemo(element.header.guid, &memo, APIMemoMask_BeamSegment);
	MP.AddVariable("Width", memo.beamSegments->assemblySegmentData.nominalWidth);
	MP.AddVariable("Height", memo.beamSegments->assemblySegmentData.nominalHeight);
	ACAPI_DisposeElemMemoHdls(&memo);

	GS::Array<API_CompositeQuantity> composites;
	API_ElementQuantity quantity;
	API_QuantitiesMask  mask;

	ACAPI_ELEMENT_QUANTITY_MASK_CLEAR(mask);
	ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, beam, area);
	ACAPI_ELEMENT_QUANTITY_MASK_SET(mask, beam, volume);
	ACAPI_ELEMENT_COMPOSITES_QUANTITY_MASK_SETFULL(mask);

	if (GetQuantities(element, mask, quantity, composites)) {
		MP.AddVariable("Area", quantity.beam.area);
		MP.AddVariable("Volume", quantity.beam.volume);
	}
}
