#include "PogoQty.hpp"

void PogoQty::SetValue(const GS::UniString name, const GS::UniString value)
{
	if (name == "id") id = value;
	if (name == "descript") descript = value;
	if (name == "aux_formula") aux_formula = value;
	if (name == "qty_formula") qty_formula = value;
	if (name == "guid") guid = value;
	if (name == "formula") guid = value;
}

void PogoQty::SetValue(const GS::UniString name, const int value)
{
	if (name == "sort_order") sort_order = value;
}

void PogoQty::SetValue(const GS::UniString name, const double value)
{
	if (name == "aux_value") aux_value = value;
	if (name == "qty_value") qty_value = value;
}
