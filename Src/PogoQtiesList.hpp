#if !defined (POGO_QTIES_LIST_H)
#define POGO_QTIES_LIST_H

#pragma once

#include	"APIEnvir.h"
#include	"ACAPinc.h"	

#include	"PogoTypes.h"
#include	"PogoSettings.hpp"

class PogoQtiesList : public GS::Array<PogoQtyData>
{
	protected:
		PogoSettings pogoSettings;
	public:
		PogoQtiesList();
		~PogoQtiesList();

		bool RESTSync();
		bool RESTDelete();
};


#endif // POGO_QTIES_LIST_H