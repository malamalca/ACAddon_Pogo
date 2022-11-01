#pragma once

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"PogoTypes.h"
#include	"PogoItem.hpp"
#include	"PogoSettings.hpp"

class PogoElementWithData {
public:
	API_Guid guid;
	PogoLinkedQties* qties;

	GS::UniString PogoElementWithData::RESTCreateQty(const PogoItem& item, const GS::UniString& formula, const GS::UniString& descript, const double& value);
	GS::UniString XMLExtractQtyId(const char* XML);

	bool   ParseFormula(const GS::UniString& formula, double& result);
	double ParseFormula(const GS::UniString& formula);
	bool   ElementExists();
	bool   UpdateQtiesToElement();
	bool   LoadQtiesFromElement();
	bool   HasQty(const GS::UniString& id);
	bool   AddQty(const PogoQtyData qtyData);
	bool   UpdateQty(const PogoQtyData qtyData);
};