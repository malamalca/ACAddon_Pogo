#if !defined (WALL_ELEMENT_FUNCS_H)
#define WALL_ELEMENT_FUNCS_H

#pragma once

#include "../APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "IElementFuncs.hpp"

class WallElementFuncs : public IElementFuncs {
public:
    GS::UniString               getName() override { return GS::UniString("Wall"); };
    GS::Array<GS::UniString>    GetAvailebleVariables() { return { "Volume", "CoreVolume", "Area", "Thickness", "Perimeter", "Length", "RefLength", "MaxHeight", "MinHeight" }; };
    void                        SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif