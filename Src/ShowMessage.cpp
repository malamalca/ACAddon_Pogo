#include	"DG.h"
#include	"UC.h"
#include	"DGModule.hpp"
//#include	"UCModule.hpp"
#include	"APIEnvir.h"
#include	"ACAPinc.h"	
#include	"ShowMessage.hpp"
#include	"ResourceIDs.h"


// -----------------------------------------------------------------------------
// Show message dialog handler (callback)
// -----------------------------------------------------------------------------

static short DGCALLBACK ShowMessage_Handler(short				message,
											short				dialogID,
											short				item,
											DGUserData			userData,
											DGMessageData		msgData)
{
UNUSED_PARAMETER(msgData);

	// Dialog item IDs
	#define OkBtn		1
	#define ProcTextItm	2

	short	result = 0;

	GS::UniString* msg;

	msg = (GS::UniString*)userData;
	
	DGSetItemText(dialogID, ProcTextItm, *msg);

	switch (message) {
		case DG_MSG_INIT:
			//DGEnableMessage(dialogID, DG_ALL_ITEMS, DG_MSG_NULL);
			break;

		case DG_MSG_CLICK:
			switch (item) {
				case OkBtn:
				case DG_CLOSEBOX:
					result = item;
					break;
			}
			break;

		case DG_MSG_CLOSE:
			result = item;
			break;
	}

	return (result);

	#undef ProcTextItm
	#undef OkBtn
}		// Progress_Handler

void ShowMessage(const GS::UniString& msg)
{
	DGModalDialog(ACAPI_GetOwnResModule(), ID_POGO_SHOWMESSAGE, ACAPI_GetOwnResModule(), ShowMessage_Handler, (DGUserData) &msg);
}
