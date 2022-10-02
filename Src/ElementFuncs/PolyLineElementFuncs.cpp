#include "../APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "PolyLineElementFuncs.hpp"

void PolyLineElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
	API_ElementMemo	memo;
	GSErrCode		err;
	double			totalLength = 0;
	short			arcIndex;
	double			segmentLength;

	err = ACAPI_Element_GetMemo(element.header.guid, &memo, APIMemoMask_Polygon);

	if (err == NoError) {
		// first point of memo.coords is dummy zeroes
		for (short r = 2; r <= element.polyLine.poly.nCoords; r++) {
			segmentLength = sqrt(pow((*memo.coords)[r].x - (*memo.coords)[r - 1].x, 2) + pow((*memo.coords)[r].y - (*memo.coords)[r - 1].y, 2));
			arcIndex = FindArc(*memo.parcs, element.polyLine.poly.nArcs, r - 1);
			if (arcIndex >= 0) {
				// points with indexes r-1 and r are beginning and ending of an arc
				segmentLength = abs((*memo.parcs)[arcIndex].arcAngle) * segmentLength / 2 / sin(abs((*memo.parcs)[arcIndex].arcAngle) / 2);
			}
			totalLength += segmentLength;
		}
	}
	ACAPI_DisposeElemMemoHdls(&memo);

	MP.AddVariable("Length", totalLength);
}

// -----------------------------------------------------------------------------
// Tell whether an arc starts from the given node in the polygon
//	Return:
//		-1		no arc starts from the given node
//		(long)	the index into the polygon arcs array
// -----------------------------------------------------------------------------

Int32 PolyLineElementFuncs::FindArc(const API_PolyArc* parcs, Int32 nArcs, Int32 node)
{
	Int32		i;

	if (parcs == nullptr)
		return (-1);
	for (i = 0; i < nArcs; i++)
		if (parcs[i].begIndex == node)
			return (i);
	return (-1);
}		// FindArc