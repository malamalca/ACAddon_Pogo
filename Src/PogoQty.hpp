#pragma once

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"PogoTypes.h"



class PogoQty : public GS::Object {
public:
	GS::UniString	id;
	GS::UniString	descript;
	int				sort_order;
	GS::UniString	aux_formula;
	double			aux_value;
	GS::UniString	qty_formula;
	double			qty_value;

	GS::UniString	guid;
	GS::UniString	formula;

	void SetValue(const GS::UniString name, const GS::UniString value);
	void SetValue(const GS::UniString name, const int value);
	void SetValue(const GS::UniString name, const double value);
};