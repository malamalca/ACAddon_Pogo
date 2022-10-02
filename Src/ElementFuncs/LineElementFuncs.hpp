#if !defined (LINE_ELEMENT_FUNCS_H)
#define LINE_ELEMENT_FUNCS_H

#pragma once

#include "ACAPinc.h"	
#include "IElementFuncs.hpp"

class LineElementFuncs : public IElementFuncs {
public:
    GS::UniString getName() override { return GS::UniString("Line"); };
    void SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif