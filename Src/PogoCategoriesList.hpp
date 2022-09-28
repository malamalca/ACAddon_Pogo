#if !defined (POGO_CATEGORIES_LIST_H)
#define POGO_CATEGORIES_LIST_H

#pragma once

#include	"APIEnvir.h"
#include	"ACAPinc.h"	

#include	"PogoCategory.hpp"
#include	"PogoSettings.hpp"

class PogoCategoriesList : public GS::Array<PogoCategory>
{
	protected:
		PogoSettings pogoSettings;

		bool ParseXml(GS::UniString XML);

	public:
		PogoCategoriesList();
		~PogoCategoriesList();

		bool Fetch();
};


#endif // POGO_CATEGORIES_LIST_H