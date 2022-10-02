#include "LineElementFuncs.hpp"

void LineElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
	MP.AddVariable("Length", sqrt(pow(element.line.begC.x - element.line.endC.x, 2) + pow(element.line.begC.y - element.line.endC.y, 2)));
}
