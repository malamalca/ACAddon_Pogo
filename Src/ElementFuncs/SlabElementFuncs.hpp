#if !defined (SLAB_ELEMENT_FUNCS_H)
#define SLAB_ELEMENT_FUNCS_H

#pragma once

#include "../APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "IElementFuncs.hpp"

class SlabElementFuncs : public IElementFuncs {
public:
    GS::UniString               getName() override { return GS::UniString("Slab"); };
    GS::Array<GS::UniString>    GetAvailebleVariables() { return { "Volume", "Area", "Perimeter" }; };
    void                        SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif