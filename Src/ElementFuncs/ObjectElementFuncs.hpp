#if !defined (OBJECT_ELEMENT_FUNCS_H)
#define OBJECT_ELEMENT_FUNCS_H

#pragma once

#include "ACAPinc.h"	
#include "IElementFuncs.hpp"

class ObjectElementFuncs : public IElementFuncs {
public:
    GS::UniString               getName() override { return GS::UniString("Object"); };
    GS::Array<GS::UniString>    GetAvailebleVariables() { return { "A", "B" }; };
    void                        SetVariables(const API_Element element, CMathParser& MP) override;
};

#endif