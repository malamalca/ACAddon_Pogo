#pragma once

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

class PogoItem : public GS::Object {
public:
	GS::UniString id;
	GS::UniString descript;
	GS::UniString unit;
	double price;
	double qty;
};