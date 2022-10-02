#include "ObjectElementFuncs.hpp"

void ObjectElementFuncs::SetVariables(const API_Element element, CMathParser& MP)
{
	MP.AddVariable("A", element.object.xRatio);
	MP.AddVariable("B", element.object.yRatio);
}
