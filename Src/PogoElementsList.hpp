#if !defined (POGO_ELEMENTS_LIST_H)
#define POGO_ELEMENTS_LIST_H

#pragma once

#include	"PogoTypes.h"
#include	"APIEnvir.h"
#include	"ACAPinc.h"	
#include	"PogoElementWithData.hpp"
#include	"PogoItem.hpp"

class PogoElementsList : public GS::Array<PogoElementWithData>
{
	public:
		~PogoElementsList();

		void GetSelectedElements(bool onlyWithData = false);
		void DeleteData();
		bool AttachQty(const PogoItem&, const GS::UniString& descript, const GS::UniString& formula);
		bool SendUpdate(const GS::UniString& host, const GS::UniString& username = "", const GS::UniString& password = "");
};


#endif // POGO_ELEMENTS_LIST_H