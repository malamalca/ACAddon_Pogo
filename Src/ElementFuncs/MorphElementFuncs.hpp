#if !defined (MORPH_ELEMENT_FUNCS_H)
#define MORPH_ELEMENT_FUNCS_H

#pragma once

#include "../APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "IElementFuncs.hpp"

class MorphElementFuncs : public IElementFuncs {
public:
    GS::UniString               getName() override { return GS::UniString("Morph"); };
    GS::Array<GS::UniString>    GetAvailebleVariables() { return { "Volume", "Area" }; };
    void                        SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif