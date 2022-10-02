#if !defined (POLYLINE_ELEMENT_FUNCS_H)
#define POLYLINE_ELEMENT_FUNCS_H

#pragma once

#include "ACAPinc.h"	
#include "IElementFuncs.hpp"

class PolyLineElementFuncs : public IElementFuncs {
private:
    Int32 FindArc(const API_PolyArc* parcs, Int32 nArcs, Int32 node);
public:
    GS::UniString getName() override { return GS::UniString("PolyLine"); };
    void SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif