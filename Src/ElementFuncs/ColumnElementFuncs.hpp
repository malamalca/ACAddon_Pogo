#if !defined (COLUMN_ELEMENT_FUNCS_H)
#define COLUMN_ELEMENT_FUNCS_H

#pragma once

#include "../APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "IElementFuncs.hpp"

class ColumnElementFuncs : public IElementFuncs {
public:
    GS::UniString               getName() override { return GS::UniString("Column"); };
    GS::Array<GS::UniString>    GetAvailebleVariables() { return { "CoreArea", "CoreVolume", "Height" }; };
    void                        SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif