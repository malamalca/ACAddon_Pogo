#if !defined (BEAM_ELEMENT_FUNCS_H)
#define BEAM_ELEMENT_FUNCS_H

#pragma once

#include "../APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "IElementFuncs.hpp"

class BeamElementFuncs : public IElementFuncs {
public:
    GS::UniString getName() override { return GS::UniString("Beam"); };
    void SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif