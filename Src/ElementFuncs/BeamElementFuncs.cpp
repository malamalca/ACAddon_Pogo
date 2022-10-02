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
}
