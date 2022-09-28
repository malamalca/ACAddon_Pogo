#include	"PogoElementsList.hpp"
#include	"PogoTypes.h"
#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

#include    "HTTP\Client\ClientConnection.hpp"
#include	"PogoHttpClient.hpp"
#include	"PogoQtiesList.hpp"
#include	"ShowMessage.hpp"

PogoElementsList::~PogoElementsList()
{
	for (PogoElementWithData& El : *this) {
		if (El.qties != nullptr) {
			//BMKillHandle(reinterpret_cast<GSHandle*> (El.qties));
			//delete El.qties;
			//El.qties = nullptr;
		}
	}
}

void PogoElementsList::GetSelectedElements(bool onlyWithData)
{
	GSErrCode						err;
	API_SelectionInfo				selectionInfo;
	GS::Array<API_Neig>				selNeigs;

	err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);
	BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);

	if (err == APIERR_NOSEL) {
		err = NoError;

		return;
	}

	if (selectionInfo.typeID != API_SelEmpty) {
		bool hasData = false;
		for (const API_Neig& neig : selNeigs) {
			API_Elem_Head head;
			BNZeroMemory(&head, sizeof(API_Elem_Head));
			head.guid = neig.guid;
			
			PogoElementWithData el;
			BNZeroMemory(&el, sizeof(PogoElementWithData));
			el.guid = head.guid;
			el.qties = nullptr;

			API_ElementUserData userData;
			BNZeroMemory(&userData, sizeof(API_ElementUserData));		
				
			hasData = false;
			if (ACAPI_Element_GetUserData(&head, &userData) == NoError) {
				if (userData.dataHdl != nullptr && userData.platformSign == GS::Act_Platform_Sign) {
					el.qties = new PogoLinkedQties();
					BNZeroMemory(el.qties, sizeof(PogoLinkedQties));
					BNCopyMemory(el.qties, *(userData.dataHdl), sizeof(PogoLinkedQties));
					BMKillHandle(&userData.dataHdl);

					hasData = true;
				}
			}

			if (!onlyWithData || hasData) {
				this->Push(el);
			}
		}
	}

	return;
}

//
// Must Be Called Inside ACAPI_CallUndoableCommand
// 
void PogoElementsList::DeleteData()
{
	API_Elem_Head head;
	PogoQtiesList elementsQties;

	for (const PogoElementWithData& El : *this) {
		for (short i = 0; i < (short)El.qties->count; i++) {
			elementsQties.Insert(elementsQties.GetSize(), El.qties->qtyData[i]);
		}

		BNZeroMemory(&head, sizeof(API_Elem_Head));
		head.guid = El.guid;

		ACAPI_Element_DeleteUserData(&head);
	}

	elementsQties.RESTDelete();
}

bool PogoElementsList::AttachQty(const PogoItem& item, const GS::UniString& descript, const GS::UniString& formula)
{
	// 1. foreach AC_Element in list do
	// 2. www.pogo.si :: POST :: create new qty for specified item and extract qty_id
	// 3. add qty to PogoLinkedQties structure
	// 4. deleet existing data from AC_Element
	// 5. add PogoLinkedQties structure to AC_Element data

	for (PogoElementWithData& El : *this) {
		// add qty to list
		if (El.qties == nullptr) {
			El.qties = new PogoLinkedQties();
		}

		// 2. create qty
		double qtyValue;
		if (El.ParseFormula(formula, qtyValue)) {
			GS::UniString qtyId = El.RESTCreateQty(item, descript, qtyValue);

			if (qtyId.IsEmpty()) {
				return false;
			}

			El.qties->count++;
			El.qties->qtyData[El.qties->count - 1].last_value = qtyValue;
			strncpy(El.qties->qtyData[El.qties->count - 1].descript, descript.ToCStr().Get(), 100);
			strncpy(El.qties->qtyData[El.qties->count - 1].formula, formula.ToCStr().Get(), 100);
			strncpy(El.qties->qtyData[El.qties->count - 1].qty_id, qtyId.ToCStr().Get(), 40);


			// get element
			API_Elem_Head head;
			BNZeroMemory(&head, sizeof(API_Elem_Head));
			head.guid = El.guid;


			if (ACAPI_Element_DeleteUserData(&head) == NoError) {
				PogoLinkedQties linkedQties;
				BNZeroMemory(&linkedQties, sizeof(PogoLinkedQties));
				BNCopyMemory(&linkedQties, El.qties, sizeof(PogoLinkedQties));

				API_ElementUserData userData;
				BNZeroMemory(&userData, sizeof(API_ElementUserData));

				userData.dataVersion = 1;
				userData.platformSign = GS::Act_Platform_Sign;
				userData.flags = APIUserDataFlag_FillWith | APIUserDataFlag_Pickup;
				userData.dataHdl = BMAllocateHandle(sizeof(linkedQties), ALLOCATE_CLEAR, 0);
				*reinterpret_cast<PogoLinkedQties*> (*userData.dataHdl) = linkedQties;

				if (ACAPI_Element_SetUserData(&head, &userData) != NoError) {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}

	return true;
}

bool PogoElementsList::SendUpdate(const GS::UniString& host, const GS::UniString& username, const GS::UniString& password)
{
	// prepare POST request
	using namespace HTTP::MessageHeader;

	GS::UniString url = "/qties/update.xml";
	GS::UniString postData = "";

	int i = 0;
	for (const PogoElementWithData& El : *this) {	
		if (!postData.IsEmpty()) {
			postData.Append("&");
		}

		for (unsigned short j = 0; j < El.qties->count; j++) {
			postData.Append(GS::UniString::Printf("%d[id]=%s", i, El.qties->qtyData[j].qty_id));
			postData.Append("&");
			postData.Append(GS::UniString::Printf("%d[descript]=%s", i, El.qties->qtyData[j].descript));
			postData.Append("&");
			postData.Append(GS::UniString::Printf("%d[qty_value_native]=%.2f", i, El.qties->qtyData[j].last_value));

			++i;
		}
	}

	// do request
	return HttpRequest(HTTP::MessageHeader::Method::Post, host, url, username, password, postData);
}
