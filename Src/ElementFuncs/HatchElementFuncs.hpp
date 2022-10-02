#if !defined (HATCH_ELEMENT_FUNCS_H)
#define HATCH_ELEMENT_FUNCS_H

#pragma once

#include "../APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "IElementFuncs.hpp"

class HatchElementFuncs : public IElementFuncs {
public:
    GS::UniString getName() override { return GS::UniString("Hatch"); };
    void SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif