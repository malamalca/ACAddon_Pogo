#pragma once

struct PogoQtyData {
	char qty_id[40];
	char formula[100];
	char descript[200];
	double last_value;
};

struct PogoLinkedQties {
	PogoQtyData qtyData[10];
	unsigned short count = 0;
};

typedef enum {
	DbFT_String = 0,
	DbFT_Int = 1,
	DbFT_Double = 2
} DbFieldTypes;

struct DbField {
	char name[40];
	DbFieldTypes type;
};