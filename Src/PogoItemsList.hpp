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
	public:
		PogoItemsList();
		~PogoItemsList();

		bool Fetch(const GS::UniString url);
		bool FetchBySection(const GS::UniString SectionId);
		bool FetchByQty(const GS::UniString QtyId);
	private:
		bool ParseXml(GS::UniString XML);
};


#endif // POGO_ITEMS_LIST_H