// *****************************************************************************
// Source code for the Item Show Modal Dialog
// API Development Kit 25; Mac/Win
//
// Namespaces:			Contact person:
//		-None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

#include	"dlgPogoItemShow.hpp" 
#include	"PogoTypes.h"
#include	"ResourceIDs.h"

//---------------------------- Class PogoItemShowDialog -----------------------

PogoItemShowDialog::PogoItemShowDialog (PogoItem AItem) :
	DG::ModalDialog		(ACAPI_GetOwnResModule (), ID_POGO_ITEMSHOW_MODAL, ACAPI_GetOwnResModule ()),

	btnOk				(GetReference(), btnOkId),
	btnZoom				(GetReference(), btnZoomId),
	lblCategorySection  (GetReference(), lblCategorySectionId),
	lblItemDescription  (GetReference(), lblItemDescriptionId),
	lblItemQty			(GetReference(), lblItemQtyId),
	lblItemPrice		(GetReference(), lblItemPriceId),
	lblItemUnit			(GetReference(), lblItemUnitId),
	lblItemTotal		(GetReference(), lblItemTotalId),
	lsQties				(GetReference(), lsQtiesId),
	item				(AItem)
{
	AttachToAllItems(*this);
	Attach (*this);

	QtiesListUpdateColumns();

	lblItemDescription.SetText(item.descript);
	lblItemUnit.SetText(item.unit);
	lblItemQty.SetText(GS::UniString::Printf("%.2f", item.qty));
	lblItemPrice.SetText(GS::UniString::Printf("%.2f", item.price));
	lblItemTotal.SetText(GS::UniString::Printf("%.2f", item.price * item.qty));
}

PogoItemShowDialog::~PogoItemShowDialog()
{
	this->Detach(*this);
	this->DetachFromAllItems(*this);
}

void PogoItemShowDialog::PanelOpened(const DG::PanelOpenEvent& ev)
{
	this->SetClientSize(this->GetOriginalClientWidth(), this->GetOriginalClientHeight());

	QtiesListUpdateColumns();

	qties.FetchByItem(item.id);
	UpdateQtiesList();
}

void PogoItemShowDialog::ButtonClicked (const DG::ButtonClickEvent& ev)
{
	if (ev.GetSource () == &btnOk) {
		this->PostCloseRequest(Cancel);
	}
	if (ev.GetSource() == &btnZoom) {
		this->ZoomToElement();
	}
}

void PogoItemShowDialog::PanelResized(const DG::PanelResizeEvent& ev)
{
	short vGrow = ev.GetVerticalChange();
	short hGrow = ev.GetHorizontalChange();
	if (vGrow != 0 || hGrow != 0) {
		BeginMoveResizeItems();

		lblItemPrice.Move(hGrow, 0);
		lblItemUnit.Move(hGrow, 0);
		lblItemQty.Move(hGrow, 0);
		lblItemTotal.Move(hGrow, 0);

		btnOk.Move(0, vGrow);
		lsQties.Resize(hGrow, vGrow);
		QtiesListUpdateColumns();

		EndMoveResizeItems();

		lsQties.Redraw();
	}
}

void PogoItemShowDialog::QtiesListUpdateColumns()
{
	// initialize the listbox
	const short width = lsQties.GetWidth();
	const short	NoCol_SIZE = 50;
	const short	UnitCol_SIZE = 50;
	const short	QtyCol_SIZE = 50;
	const short	PriceCol_SIZE = 50;
	const short	TotalCol_SIZE = 50;

	const short	DescriptCol_SIZE = width - NoCol_SIZE - UnitCol_SIZE - QtyCol_SIZE - PriceCol_SIZE - TotalCol_SIZE;

	lsQties.SetTabFieldCount(6);
	lsQties.SetHeaderSynchronState(false);

	lsQties.SetHeaderItemSize(1, NoCol_SIZE);
	lsQties.SetHeaderItemSize(2, DescriptCol_SIZE);
	lsQties.SetHeaderItemSize(3, UnitCol_SIZE);
	lsQties.SetHeaderItemSize(4, QtyCol_SIZE);
	lsQties.SetHeaderItemSize(5, PriceCol_SIZE);
	lsQties.SetHeaderItemSize(6, TotalCol_SIZE);

	short pos = 0;
	lsQties.SetTabFieldProperties(1, pos, pos + NoCol_SIZE, DG::ListBox::Center, DG::ListBox::NoTruncate, false);
	pos += NoCol_SIZE;
	lsQties.SetTabFieldProperties(2, pos, pos + DescriptCol_SIZE, DG::ListBox::Left, DG::ListBox::NoTruncate, false);
	pos += DescriptCol_SIZE;
	lsQties.SetTabFieldProperties(3, pos, pos + UnitCol_SIZE, DG::ListBox::Left, DG::ListBox::NoTruncate, false);
	pos += UnitCol_SIZE;
	lsQties.SetTabFieldProperties(4, pos, pos + QtyCol_SIZE, DG::ListBox::Left, DG::ListBox::NoTruncate, false);
	pos += QtyCol_SIZE;
	lsQties.SetTabFieldProperties(5, pos, pos + PriceCol_SIZE, DG::ListBox::Left, DG::ListBox::NoTruncate, false);
	pos += PriceCol_SIZE;
	lsQties.SetTabFieldProperties(6, pos, pos + TotalCol_SIZE, DG::ListBox::Left, DG::ListBox::NoTruncate, false);
	pos += TotalCol_SIZE;

	// set texts:
	lsQties.SetHeaderItemText(1, "No");
	lsQties.SetHeaderItemText(2, "Description");
	lsQties.SetHeaderItemText(3, "Unit");
	lsQties.SetHeaderItemText(4, "Qty");
	lsQties.SetHeaderItemText(5, "Price");
	lsQties.SetHeaderItemText(6, "Total");
}

void PogoItemShowDialog::UpdateQtiesList()
{
	// clear interface
	for (short i = lsQties.GetItemCount(); i > 0; i--) {
		lsQties.DeleteItem(i);
	}

	for (const PogoQty& qty : qties) {
		lsQties.InsertItem(lsQties.GetItemCount() + 1);
		lsQties.SetTabItemText(lsQties.GetItemCount(), 1, GS::UniString::Printf("%u", qty.sort_order));
		lsQties.SetTabItemText(lsQties.GetItemCount(), 2, qty.descript);

		lsQties.SetTabItemText(lsQties.GetItemCount(), 4, GS::UniString::Printf("%.2f", qty.qty_value));
		
	}
}

void PogoItemShowDialog::ZoomToElement()
{
	short selectedQty = lsQties.GetSelectedItem();

	if (selectedQty >= 0) {
		PogoQty qty = qties.Get(selectedQty - 1);
		if (!qty.guid.IsEmpty()) {
			API_Guid tmpGuid = APIGuidFromString(qty.guid.ToCStr().Get());
			GS::Array<API_Guid> guids = {tmpGuid};
			GSErrCode err;

			err = ACAPI_Automate(
				APIDo_ZoomToElementsID,
				&guids
			);

			GS::Array<API_Neig> neigs;
			neigs.PushNew(tmpGuid);
			err = ACAPI_Element_Select(neigs, true);

			if (err == NoError) {
				this->PostCloseRequest(Accept);
			}
		}
	}
}