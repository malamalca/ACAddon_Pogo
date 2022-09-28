#if !defined (POGO_ITEMS_LIST_H)
#define POGO_ITEMS_LIST_H

#pragma once

#include	"APIEnvir.h"
#include	"ACAPinc.h"	

#include	"PogoItem.hpp"
#include	"PogoSettings.hpp"

class PogoItemsList : public GS::Array<PogoItem>
{
	protected:
		PogoSettings pogoSettings;

		bool ParseXml(GS::UniString XML);

	public:
		PogoItemsList();
		~PogoItemsList();

		bool Fetch(GS::UniString SectionId);
};


#endif // POGO_ITEMS_LIST_H