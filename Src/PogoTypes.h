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

