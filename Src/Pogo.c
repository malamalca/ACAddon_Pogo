// *****************************************************************************
// Source code for the Pogo ARCHICAD Add-On
// API Development Kit; Mac/Win
//
// Namespaces:			Contact person:
//		-None-
//
// [SG compatible] - Yes
// *****************************************************************************

// ---------------------------------- Includes ---------------------------------

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

#ifdef POGO_USE_CURL
#include	<curl/curl.h>
#endif

#include	"ResourceIDs.h"
#include	"PogoPalette.h"
#include	"PogoTypes.h"
#include	"PogoElementsList.hpp"
#include	"dlgPogoData.hpp"
#include	"dlgPogoItemSelect.hpp"
#include	"dlgPogoQtyFormula.hpp"
#include	"dlgPogoSettings.hpp"
#include	"dlgPogoItemShow.hpp"
#include	"ShowMessage.hpp"


// -----------------------------------------------------------------------------
// Display Pogo Dialog and prompt user for input
// -----------------------------------------------------------------------------
void PogoShowDataDialog ()
{
	PogoElementsList	data;

	data.GetSelectedElements(true);

	if (data.IsEmpty()) {
		ShowMessage("No Elements Selected or No Data Attached");

		return;
	}

	PogoDataDialog		dialog(data);
	if (DBERROR(dialog.GetId() == 0)) {
		return;
	}

	dialog.Invoke ();

	return;
} // Show_PogoDataDialog

// -----------------------------------------------------------------------------
// Display Pogo Items Select Dialog and prompt user for input
// -----------------------------------------------------------------------------
void PogoShowItemsSelectDialog()
{
	PogoElementsList selectedElements;
	selectedElements.GetSelectedElements(false);
	if (selectedElements.IsEmpty()) {
		ShowMessage("No Elements Selected!");
		return;
	}

	// show dialog
	PogoItemSelectDialog		itemSelectDialog;
	if (DBERROR(itemSelectDialog.GetId() == 0)) {
		return;
	}

	PogoItem selectedItem;
	if (itemSelectDialog.Invoke() && itemSelectDialog.GetSelectedPogoItem(selectedItem)) {
		PogoQtyFormulaDialog qtyFormulaDialog;
		if (DBERROR(qtyFormulaDialog.GetId() == 0)) {
			return;
		}

		// store qty
		GSErrCode			err;
		PogoQtyData			qty;
		bool				dialogSuccess = false;

		qtyFormulaDialog.item = selectedItem;
		qtyFormulaDialog.SetPogoElement(selectedElements.GetFirst());

		while (!dialogSuccess) {
			if (qtyFormulaDialog.Invoke()) {
				qtyFormulaDialog.GetPogoQty(qty);

				err = ACAPI_CallUndoableCommand("Attach Qties", [&selectedElements, &selectedItem, &qty]() -> GSErrCode {
					bool res = selectedElements.AttachQty(selectedItem, qty.descript, qty.formula);
					return res == true ? NoError : -1;
				});

				if (err != NoError) {
					ShowMessage("Failed to attach Qty!");
				}
				else {
					ShowMessage("Qty Attached!");
					dialogSuccess = true;
				}
			}
			else {
				dialogSuccess = true;
			}
		}
	}

	return;
} // PogoShowItemsSelectDialog

// -----------------------------------------------------------------------------
// Select item, show quantities, sync pogo qties to elements
// -----------------------------------------------------------------------------
void PogoItemDetails()
{
	// show dialog
	PogoItemSelectDialog		itemSelectDialog;
	if (DBERROR(itemSelectDialog.GetId() == 0)) {
		return;
	}

	PogoItem selectedItem;
	if (itemSelectDialog.Invoke() && itemSelectDialog.GetSelectedPogoItem(selectedItem)) {
		PogoItemShowDialog itemShowDialog(selectedItem);
		if (DBERROR(itemShowDialog.GetId() == 0)) {
			return;
		}

		itemShowDialog.Invoke();
	}

	return;
}

// -----------------------------------------------------------------------------
// Send all updates to Pogo server
// -----------------------------------------------------------------------------
void PogoSendQties()
{
	PogoElementsList data;
	data.GetSelectedElements (true);

	if (data.IsEmpty ()) {
		ShowMessage("No Elements With Data Selected");

		return;
	}

	GSErrCode err;
	err = ACAPI_CallUndoableCommand("Delete user data to an elem", [&data]() -> GSErrCode {
		data.UpdateQtyValues();

		return NoError;
	});

	data.SendUpdate();
} // PogoSendQties

// -----------------------------------------------------------------------------
// Delete All Associated Qties from Elelents
// -----------------------------------------------------------------------------
void PogoDeleteData()
{
	PogoElementsList data;
	data.GetSelectedElements();

	if (data.IsEmpty()) {
		ShowMessage("No Elements Selected or No Data Attached");

		return;
	}

	GSErrCode err;
	err = ACAPI_CallUndoableCommand("Delete user data to an elem", [&data]() -> GSErrCode {
		data.DeleteData();

		return NoError;
	});
} // PogoDeleteData

// -----------------------------------------------------------------------------
// Delete All Qties from selected elements That Does not have PogoQty
// -----------------------------------------------------------------------------
void PogoDeleteDetached()
{
	PogoElementsList data;
	data.GetSelectedElements();

	if (data.IsEmpty()) {
		ShowMessage("No Elements Selected or No Data Attached");

		return;
	}

	GSErrCode err;
	err = ACAPI_CallUndoableCommand("Delete detached qties", [&data]() -> GSErrCode {
		data.DeleteDetachedQties();

		return NoError;
	});
} // PogoDeleteData


// -----------------------------------------------------------------------------
// Display Pogo Settings Dialog
// -----------------------------------------------------------------------------
void PogoShowSettingsDialog()
{
	// show dialog
	PogoSettingsDialog		dialog;
	if (DBERROR(dialog.GetId() == 0)) {
		return;
	}

	dialog.Invoke();
} // PogoShowSettingsDialog



// -----------------------------------------------------------------------------
// Handles menu commands
// -----------------------------------------------------------------------------
GSErrCode __ACENV_CALL MenuCommandHandler (const API_MenuParams *menuParams)
{
	switch (menuParams->menuItemRef.menuResID) {
	case ID_MENU_STRINGS:
		switch (menuParams->menuItemRef.itemIndex) {
			case 1:
				//PogoPalette::GetInstance().Show();
				PogoShowItemsSelectDialog();
				break;
			case 2:
				PogoShowDataDialog();
				break;
			case 3:
				PogoItemDetails();
				break;
			case 5:
				PogoSendQties();
				break;
			case 6:
				PogoDeleteData();
				break;
			case 7:
				PogoDeleteDetached();
				break;
			case 9:
				PogoShowSettingsDialog();
				break;
		}

	}

	return NoError;
}		// MenuCommandHandler

// -----------------------------------------------------------------------------
// Selection change handler function
// -----------------------------------------------------------------------------
static GSErrCode __ACENV_CALL    SelectionChangeHandler(const API_Neig* selElemNeig)
{
	PogoPalette::GetInstance().SelectionChange(selElemNeig);

	return NoError;
}   // SelectionChangeHandler


// =============================================================================
//
// Required functions
//
// =============================================================================


// -----------------------------------------------------------------------------
// Dependency definitions
// -----------------------------------------------------------------------------

API_AddonType	__ACENV_CALL	CheckEnvironment (API_EnvirParams* envir)
{
	RSGetIndString (&envir->addOnInfo.name, ID_ADDON_INFO, 1, ACAPI_GetOwnResModule());
	RSGetIndString (&envir->addOnInfo.description, ID_ADDON_INFO, 2, ACAPI_GetOwnResModule());

	return APIAddon_Normal;
}		// CheckEnvironment


// -----------------------------------------------------------------------------
// Interface definitions
// -----------------------------------------------------------------------------

GSErrCode	__ACENV_CALL	RegisterInterface (void)
{
	GSErrCode err = ACAPI_Register_Menu (ID_MENU_STRINGS, ID_MENU_PROMPT_STRINGS, /*MenuCode_Tools*/ MenuCode_UserDef, MenuFlag_Default);

	return err;
}		// RegisterInterface


// -----------------------------------------------------------------------------
// Called when the Add-On has been loaded into memory
// to perform an operation
// -----------------------------------------------------------------------------

GSErrCode	__ACENV_CALL Initialize	(void)
{
#ifdef POGO_USE_CURL
	curl_global_init(CURL_GLOBAL_ALL);
#endif

	GSErrCode err = ACAPI_Install_MenuHandler (ID_MENU_STRINGS, MenuCommandHandler);

	// catch changes in the selection
	// err = ACAPI_Notify_CatchSelectionChange(SelectionChangeHandler);

	// If the palette is constructed and registered, the add-on will be not unloaded from the memory while ARCHICAD runs.
	/*ACAPI_RegisterModelessWindow(PogoPalette::PogoPaletteRefId(),
		PogoPalette::PaletteAPIControlCallBack,
		API_PalEnabled_FloorPlan + API_PalEnabled_Section + API_PalEnabled_Elevation +
		API_PalEnabled_InteriorElevation + API_PalEnabled_3D +
		API_PalEnabled_Detail + API_PalEnabled_Worksheet + API_PalEnabled_Layout +
		API_PalEnabled_DocumentFrom3D, GSGuid2APIGuid(PogoPalette::PogoPaletteGuid()));
		*/

	return err;
}		// Initialize


// -----------------------------------------------------------------------------
// FreeData
//		called when the Add-On is going to be unloaded
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	FreeData (void)
{
#ifdef POGO_USE_CURL
	curl_global_cleanup();
#endif

	//ACAPI_UnregisterModelessWindow(PogoPalette::PogoPaletteRefId());


	return NoError;
}		// FreeData
