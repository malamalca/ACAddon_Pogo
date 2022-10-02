#if !defined (ELEMENT_FUNCS_H)
#define ELEMENT_FUNCS_H

#pragma once

#include    "ACAPinc.h"	
#include    "IElementFuncs.hpp"
#include	"../CMathParser.h"

// Factory for creating instances of ElementFuncs
class ElementFuncsFactory
{
private:
    IElementFuncs* GetClass(API_ElemTypeID typeId);
public:
    static ElementFuncsFactory* Get();
    
    GS::UniString   GetName(const API_Element element);
    bool            SetVariables(const API_Element element, CMathParser& MP);
};

#endif