// *****************************************************************************
// Source code for the BuildingMaterial Dialog in DG Test Add-On
// API Development Kit 19; Mac/Win
//
// Namespaces:			Contact person:
//		-None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"ResourceIDs.h"

#include	"dlgPogoData.hpp" 
#include	"PogoTypes.h"
#include	"PogoQtiesList.hpp"
#include	"dlgPogoQtyFormula.hpp"
#include	"ShowMessage.hpp"

//---------------------------- Class PogoDataDialog -----------------------

PogoDataDialog::PogoDataDialog (PogoElementsList selectedElements) :
	DG::ModalDialog		(ACAPI_GetOwnResModule (), ID_POGO_DATA_MODAL, ACAPI_GetOwnResModule ()),

	btnClose			(GetReference (), btnCloseId),
	cbElements			(GetReference (), cbElementsId),
	lsQties				(GetReference (), lsQtiesId),
	btnSyncQty			(GetReference (), btnSyncQtyId),
	btnDeleteQty        (GetReference (), btnDeleteQtyId),
	btnEditQty          (GetReference (), btnEditQtyId),

	selectedElements	(selectedElements)
{
	AttachToAllItems(*this);
	Attach (*this);

	// initialize the listbox
	const short width = lsQties.GetItemWidth();
	const short	IdCol_SIZE = 100;
	const short	Val1Col_SIZE = 50;
	const short	Val2Col_SIZE = 50;
	const short	DescriptCol_SIZE = width - IdCol_SIZE - Val1Col_SIZE - Val2Col_SIZE;

	lsQties.SetTabFieldCount(4);
	lsQties.SetHeaderSynchronState(false);

	lsQties.SetHeaderItemSize(1, IdCol_SIZE);
	lsQties.SetHeaderItemSize(2, DescriptCol_SIZE);
	lsQties.SetHeaderItemSize(3, Val1Col_SIZE);
	lsQties.SetHeaderItemSize(4, Val2Col_SIZE);

	lsQties.SetHeaderItemText(1, "Qty Id");
	lsQties.SetHeaderItemText(2, "Formula");
	lsQties.SetHeaderItemText(3, "Last Value");
	lsQties.SetHeaderItemText(4, "Current Value");

	short pos = 0;
	lsQties.SetTabFieldProperties(1, pos, pos + IdCol_SIZE, DG::ListBox::Left, DG::ListBox::NoTruncate, false);
	pos += IdCol_SIZE;
	lsQties.SetTabFieldProperties(2, pos, pos + DescriptCol_SIZE, DG::ListBox::Left, DG::ListBox::NoTruncate, false);
	pos += DescriptCol_SIZE;
	lsQties.SetTabFieldProperties(3, pos, pos + Val1Col_SIZE, DG::ListBox::Right, DG::ListBox::NoTruncate, false);
	pos += Val1Col_SIZE;
	lsQties.SetTabFieldProperties(4, pos, pos + Val2Col_SIZE, DG::ListBox::Right, DG::ListBox::NoTruncate, false);

	UpdateInterface();
}

PogoDataDialog::~PogoDataDialog ()
{
	Detach(*this);
	DetachFromAllItems(*this);
}

void PogoDataDialog::PanelOpened(const DG::PanelOpenEvent& ev)
{
	SetClientSize(GetOriginalClientWidth(), GetOriginalClientHeight());

	// fill elemetns combobox
	cbElements.DisableDraw();
	for (short i = 0; i < (short)selectedElements.GetSize(); i++) {
		API_Element element;
		BNZeroMemory(&element, sizeof(API_Element));
		element.header.guid = selectedElements.Get(i).guid;

		if (ACAPI_Element_Get(&element) == NoError) {
			cbElements.InsertItem(cbElements.GetItemCount() + 1);

			GS::UniString elementCaption;
			switch (element.header.typeID) {
				case API_ObjectID:
					elementCaption = "Object";
					break;
				default:
					elementCaption = "Unknown";
			}

			cbElements.SetItemText(cbElements.GetItemCount(), elementCaption);
		}
	}
	cbElements.EnableDraw();
	cbElements.Redraw();

	UpdateQtiesList();
}

void PogoDataDialog::ButtonClicked (const DG::ButtonClickEvent& ev)
{
	if (ev.GetSource () == &btnClose) {
		PostCloseRequest (Accept);
	}
	if (ev.GetSource() == &btnSyncQty) {
		OnSyncQtyClick();
	}
	if (ev.GetSource() == &btnDeleteQty) {
		OnDeleteQtyClick();
	}
	if (ev.GetSource() == &btnEditQty) {
		OnEditQtyClick();
	}
}

void PogoDataDialog::ListBoxSelectionChanged(const DG::ListBoxSelectionEvent& ev)
{
	UpdateInterface();
}

void PogoDataDialog::LoadPogoData()
{
	/*edtQtyId.SetText(_element.qties->qtyData[0].qty_id);
	edtFormula.SetText(_element.qties->qtyData[0].formula);*/
}

void PogoDataDialog::SetPogoData ()
{
	/*GSErrCode			err;

	PogoLinkedQties data;
	strcpy (data.qtyData[0].formula, edtFormula.GetText ().ToCStr());
	strcpy (data.qtyData[0].qty_id, edtQtyId.GetText ().ToCStr ());
	data.qtyData[0].last_value = 1.2;

	API_ElementUserData userData = {};
	userData.dataVersion = 1;
	userData.platformSign = GS::Act_Platform_Sign;
	userData.flags = APIUserDataFlag_FillWith | APIUserDataFlag_Pickup;
	userData.dataHdl = BMAllocateHandle(sizeof(data), ALLOCATE_CLEAR, 0);
	*reinterpret_cast<PogoLinkedQties*> (*userData.dataHdl) = data;

	API_Elem_Head element = {};
	element.guid = _element.guid;

	err = ACAPI_CallUndoableCommand("Set user data to an elem",
		[&userData, &element]() -> GSErrCode {
			return ACAPI_Element_SetUserData(&element, &userData);
		}
	);*/
}

void PogoDataDialog::UpdateQtiesList()
{
	for (short i = lsQties.GetItemCount(); i > 0; i--) {
		lsQties.DeleteItem(i);
	}

	short selectedIndex = cbElements.GetSelectedItem();
	if (selectedIndex > 0) {
		PogoElementWithData selectedElement = selectedElements.Get(selectedIndex - 1);
		if (selectedElement.qties != nullptr) {

			for (short i = 0; i < (short)selectedElement.qties->count; i++) {
				lsQties.InsertItem(i + 1);
				UpdateQtyListItem(i + 1, selectedElement.qties->qtyData[i]);
				//lsQties.SetTabItemText(i + 1, 1, selectedElement.qties->qtyData[i].qty_id);
				//lsQties.SetTabItemText(i + 1, 2, selectedElement.qties->qtyData[i].formula);
				//lsQties.SetTabItemText(i + 1, 3, GS::UniString::Printf("%.2f", selectedElement.qties->qtyData[i].last_value));
				//lsQties.SetTabItemText(i + 1, 4, GS::UniString::Printf("%.2f", selectedElement.ParseFormula(selectedElement.qties->qtyData[i].formula)));
			}
		}
	}
}


void PogoDataDialog::UpdateQtyListItem(short listItemIndex, PogoQtyData& qty)
{
	lsQties.SetTabItemText(listItemIndex, 1, qty.qty_id);
	lsQties.SetTabItemText(listItemIndex, 2, qty.formula);
	lsQties.SetTabItemText(listItemIndex, 3, GS::UniString::Printf("%.2f", qty.last_value));
	lsQties.SetTabItemText(listItemIndex, 4, GS::UniString::Printf("%.2f", qty.formula));
}

void PogoDataDialog::UpdateInterface()
{
	if (lsQties.GetSelectedCount() > 0) {
		btnSyncQty.Enable();
		btnDeleteQty.Enable();
		if (lsQties.GetSelectedCount() == 1) {
			btnEditQty.Enable();
		}
	}
	else {
		btnSyncQty.Disable();
		btnDeleteQty.Disable();
		btnEditQty.Disable();
	}
}

void PogoDataDialog::OnSyncQtyClick()
{
	PogoElementWithData selectedElement = selectedElements.Get(cbElements.GetSelectedItem() - 1);
	PogoQtiesList selectedQties;
	GS::Array<short> selectedQtyItems = lsQties.GetSelectedItems();

	double formulaResult;
	for (short i = 0; i < (short)selectedQtyItems.GetSize(); i++) {
		if (selectedElement.ParseFormula(selectedElement.qties->qtyData[selectedQtyItems[i] - 1].formula, formulaResult)) {
			selectedElement.qties->qtyData[selectedQtyItems[i] - 1].last_value = formulaResult;
			selectedQties.Insert(selectedQties.GetSize(), selectedElement.qties->qtyData[selectedQtyItems[i] - 1]);
		}
	}

	if (selectedQties.RESTSync()) {
		// modify data on element
		GSErrCode			err;
		err = ACAPI_CallUndoableCommand("Update Qties", [&selectedElement]() -> GSErrCode {
			if (selectedElement.UpdateQties()) {
				return NoError;
			}
			return false;
		});

		if (err == NoError) {
			ShowMessage("Qties Synced");

			// update listbox
			for (short i = 0; i < (short)selectedQtyItems.GetSize(); i++) {
				lsQties.SetTabItemText(selectedQtyItems[i], 3, GS::UniString::Printf("%.2f", selectedElement.qties->qtyData[selectedQtyItems[i] - 1].last_value));
				//UpdateQtyListItem(selectedQtyItems[i], selectedElement.qties->qtyData[selectedQtyItems[i] - 1]);
			}

			return;
		}
	}
	ShowMessage("Error Syncing Qties");
}

void PogoDataDialog::OnDeleteQtyClick()
{
	PogoElementWithData selectedElement = selectedElements.Get(cbElements.GetSelectedItem() - 1);
	PogoQtiesList selectedQties;
	GS::Array<short> selectedListItems = lsQties.GetSelectedItems();

	for (short i = (short)selectedListItems.GetSize(); i > 0; i--) {
		short qtyToDelete = selectedListItems[i - 1] - 1;

		selectedQties.Insert(selectedQties.GetSize(), selectedElement.qties->qtyData[qtyToDelete]);
		if (selectedQties.RESTDelete()) {
			if (qtyToDelete == selectedElement.qties->count - 1) {
				// last item
			}
			else {
				memmove(
					&selectedElement.qties->qtyData[qtyToDelete-1],
					&selectedElement.qties->qtyData[qtyToDelete],
					sizeof(PogoQtyData)*(10 - qtyToDelete)
				);
			}
			selectedElement.qties->count--;

			selectedElement.UpdateQties();
			lsQties.DeleteItem(selectedListItems[i - 1]);
		}
		selectedQties.Clear();
	}
}

void PogoDataDialog::OnEditQtyClick()
{
	PogoElementWithData selectedElement = selectedElements.Get(cbElements.GetSelectedItem() - 1);
	PogoQtyData selectedQty = selectedElement.qties->qtyData[lsQties.GetSelectedItem() - 1];

	PogoQtyFormulaDialog qtyFormulaDialog;
	if (DBERROR(qtyFormulaDialog.GetId() == 0)) {
		return;
	}
	qtyFormulaDialog.SetPogoQty(selectedQty);
	qtyFormulaDialog.SetPogoElement(selectedElement);

	
	if (qtyFormulaDialog.Invoke()) {
		qtyFormulaDialog.GetPogoQty(selectedElement.qties->qtyData[lsQties.GetSelectedItem() - 1]);

		// modify data on element
		GSErrCode			err;
		err = ACAPI_CallUndoableCommand("Update Qties", [&selectedElement]() -> GSErrCode {
			if (selectedElement.UpdateQties()) {
				return NoError;
			}
			return false;
		});

		if (err == NoError) {
			UpdateQtyListItem(lsQties.GetSelectedItem(), selectedElement.qties->qtyData[lsQties.GetSelectedItem() - 1]);

			ShowMessage("Qty Edited");
		}
	}
}