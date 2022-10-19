#if !defined (QTIES_LIST_H)
#define QTIES_LIST_H

#pragma once

#include	"APIEnvir.h"
#include	"ACAPinc.h"	

#include	"PogoTypes.h"
#include	"PogoSettings.hpp"

class QtiesList : public GS::Array<PogoQtyData>
{
	public:
		QtiesList();
		~QtiesList();

		bool RESTSync();
		bool RESTDelete();
};


#endif // QTIES_LIST_H