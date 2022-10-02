#if !defined (ROOF_ELEMENT_FUNCS_H)
#define ROOF_ELEMENT_FUNCS_H

#pragma once

#include "../APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "IElementFuncs.hpp"

class RoofElementFuncs : public IElementFuncs {
public:
    GS::UniString getName() override { return GS::UniString("Roof"); };
    void SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif