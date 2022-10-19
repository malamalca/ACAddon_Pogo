#include "ElementFuncsFactory.hpp"
#include "ObjectElementFuncs.hpp"
#include "SlabElementFuncs.hpp"
#include "WallElementFuncs.hpp"
#include "ColumnElementFuncs.hpp"
#include "BeamElementFuncs.hpp"
#include "RoofElementFuncs.hpp"
#include "MorphElementFuncs.hpp"
#include "HatchElementFuncs.hpp"
#include "LineElementFuncs.hpp"
#include "PolyLineElementFuncs.hpp"

ElementFuncsFactory* ElementFuncsFactory::Get()
{
    static ElementFuncsFactory instance;
    return &instance;
}

IElementFuncs* ElementFuncsFactory::GetClass(API_ElemTypeID typeId)
{
    switch (typeId) {
        case API_ObjectID:
            return new ObjectElementFuncs();
            break;
        case API_SlabID:
            return new SlabElementFuncs();
            break;
        case API_WallID:
            return new WallElementFuncs();
            break;
        case API_ColumnID:
            return new ColumnElementFuncs();
            break;
        case API_BeamID:
            return new BeamElementFuncs();
            break;
        case API_RoofID:
            return new RoofElementFuncs();
            break;
        case API_MorphID:
            return new MorphElementFuncs();
            break;
        case API_HatchID:
            return new HatchElementFuncs();
            break;
        case API_LineID:
            return new LineElementFuncs();
            break;
        case API_PolyLineID:
            return new PolyLineElementFuncs();
            break;
    }

    return nullptr;
}

GS::UniString ElementFuncsFactory::GetName(const API_Element element)
{
    IElementFuncs* elFuncs = GetClass(element.header.typeID);
    GS::UniString result = "Unknown";
        
    if (elFuncs) {
        result = elFuncs->getName();
        delete elFuncs;
    }
    elFuncs = NULL;

    return result;
}

GS::Array<GS::UniString> ElementFuncsFactory::GetAvailebleVariables(const API_Element element)
{
    IElementFuncs* elFuncs = GetClass(element.header.typeID);
    GS::Array<GS::UniString> result = {};

    if (elFuncs) {
        result = elFuncs->GetAvailebleVariables();
        delete elFuncs;
    }
    elFuncs = NULL;

    return result;
}

bool ElementFuncsFactory::SetVariables(const API_Element element, CMathParser& MP)
{
    IElementFuncs* elFuncs = GetClass(element.header.typeID);
    bool result = true;

    if (elFuncs) {
        elFuncs->SetVariables(element, MP);
        delete elFuncs;
    }
    elFuncs = NULL;

    return true;
}
