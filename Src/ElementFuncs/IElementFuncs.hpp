#if !defined (I_ELEMENT_FUNCS_H)
#define I_ELEMENT_FUNCS_H2

#pragma once

#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../CMathParser.h"

class IElementFuncs {
public:
	bool GetQuantities(const API_Element& element, API_QuantitiesMask& mask, API_ElementQuantity& quantity, GS::Array<API_CompositeQuantity>& composite);
	bool GetQuantities(const API_Element& element, API_QuantitiesMask& mask, API_ElementQuantity& quantity,
		GS::Array<API_CompositeQuantity>& composites,
		GS::Array<API_ElemPartQuantity>& elemPartQuantities,
		GS::Array <API_ElemPartCompositeQuantity>& elemPartComposites
	);

    virtual GS::UniString getName() { return GS::UniString(""); };
	virtual GS::Array<GS::UniString> GetAvailebleVariables() = 0;
    virtual void SetVariables(const API_Element element, CMathParser& MP) = 0;
};

#endif