#pragma once

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"PogoSectionsList.hpp"

class PogoCategory {
public:
	GS::UniString id;
	GS::UniString title;

	PogoSectionsList sections;
};