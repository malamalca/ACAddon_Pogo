#if !defined (POGO_QTIES_LIST_H)
#define POGO_QTIES_LIST_H

#pragma once

#include	"APIEnvir.h"
#include	"ACAPinc.h"	

#include	"PogoTypes.h"
#include	"PogoQty.hpp"

class PogoQtiesList : public GS::Array<PogoQty>
{
	public:
		bool FetchById(const GS::UniString qtyId);
		bool Fetch(const GS::UniString url);
		bool FetchByItem(const GS::UniString QtyId);

		bool Delete();
	private:
		short fieldCount = 9;
		DbField	fieldList[9] = {
			{"id", DbFT_String},
			{"descript", DbFT_String},
			{"sort_order", DbFT_Int},
			{"aux_formula", DbFT_String},
			{"aux_value", DbFT_Double},
			{"qty_formula", DbFT_String},
			{"qty_value", DbFT_Double},
			{"guid", DbFT_String},
			{"formula", DbFT_String},
		};

		bool ParseXml(const char* XML);
};


#endif // POGO_QTIES_LIST_H