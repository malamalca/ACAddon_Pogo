// *****************************************************************************
// Source code for the Item Select Modal Dialog
// API Development Kit 25; Mac/Win
//
// Namespaces:			Contact person:
//		-None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h

#include	"dlgPogoItemSelect.hpp" 
#include	"PogoTypes.h"
#include	"ResourceIDs.h"
#include    "PogoItemsList.hpp"

#include	"DGModule.hpp"
#include	"DGNativeContexts.hpp"
#include	"Font.hpp"

//---------------------------- Class PogoDataDialog -----------------------

PogoItemSelectDialog::PogoItemSelectDialog () :
	DG::ModalDialog		(ACAPI_GetOwnResModule (), ID_POGO_ITEMSELECT_MODAL, ACAPI_GetOwnResModule ()),

	btnOk				(GetReference (), btnOkId),
	btnCancel			(GetReference (), btnCancelId),
	cbCategory			(GetReference (), cbCategoryId),
	cbSection			(GetReference (), cbSectionId),
	lsItems				(GetReference (), lsItemsId)
{
	AttachToAllItems(*this);
	Attach (*this);

	ItemListUpdateColumns();

	// to catch ListBoxTabFieldUpdate event:
	lsItems.SetTabFieldOwnerDrawFlag(2, true);

	// set texts:
	lsItems.SetHeaderItemText(1, "No");
	lsItems.SetHeaderItemText(2, "Description");
}

PogoItemSelectDialog::~PogoItemSelectDialog()
{
	this->Detach(*this);
	this->DetachFromAllItems(*this);
}

bool PogoItemSelectDialog::GetSelectedPogoItem(PogoItem& item)
{
	short selectedItemIndex = this->lsItems.GetSelectedItem();

	if (selectedItemIndex > 0) {
		item = this->items.Get(selectedItemIndex - 1);
		return true;
	}
	else {
		return false;
	}
}

void PogoItemSelectDialog::ItemListUpdateColumns()
{
	// initialize the listbox
	const short width = lsItems.GetItemWidth();
	const short	NoCol_SIZE = 50;
	const short	DescriptCol_SIZE = width - NoCol_SIZE;

	lsItems.SetTabFieldCount(2);
	lsItems.SetHeaderSynchronState(false);

	lsItems.SetHeaderItemSize(1, NoCol_SIZE);
	lsItems.SetHeaderItemSize(2, lsItems.GetItemWidth() - NoCol_SIZE);

	short pos = 0;
	lsItems.SetTabFieldProperties(1, pos, pos + NoCol_SIZE, DG::ListBox::Center, DG::ListBox::NoTruncate, false);
	pos += NoCol_SIZE;
	lsItems.SetTabFieldProperties(2, pos, pos + DescriptCol_SIZE, DG::ListBox::Left, DG::ListBox::NoTruncate, false);
	pos += DescriptCol_SIZE;
}

void PogoItemSelectDialog::PanelOpened(const DG::PanelOpenEvent& ev)
{
	this->SetClientSize(this->GetOriginalClientWidth(), this->GetOriginalClientHeight());

	PogoSettings::LoadPogoSettingsFromPreferences(this->pogoSettings);

	if (this->categories.Fetch()) {
		this->UpdateCategories();
	}
	else {
		this->Abort();
	}
}

void PogoItemSelectDialog::PopUpChanged(const DG::PopUpChangeEvent& ev)
{
	if (ev.GetSource() == &cbCategory) {
		this->UpdateSections();
	}
	if (ev.GetSource() == &cbSection) {
		this->UpdateItems();
	}
}

void PogoItemSelectDialog::ButtonClicked (const DG::ButtonClickEvent& ev)
{
	if (ev.GetSource () == &btnCancel)
		this->PostCloseRequest(Cancel);

	if (ev.GetSource () == &btnOk) {
		this->PostCloseRequest(Accept);
	}
}

void PogoItemSelectDialog::UpdateCategories()
{
	// clear interface
	for (short i = this->cbCategory.GetItemCount(); i > 0; i--) {
		this->cbCategory.DeleteItem(i);
	}

	this->cbCategory.DisableDraw();

	for (short i = 0; i < (short)this->categories.GetSize(); i++) {
		this->cbCategory.InsertItem(i + 1);
		this->cbCategory.SetItemText(i + 1, this->categories.Get(i).title);
	}

	//cbCategory.SelectItem(0);
	this->cbCategory.EnableDraw();
	this->cbCategory.Redraw();

	this->UpdateSections();
}

void PogoItemSelectDialog::UpdateSections()
{
	for (short i = cbSection.GetItemCount(); i > 0; i--) {
		cbSection.DeleteItem(i);
	}

	short selectedCategory = cbCategory.GetSelectedItem();

	if (selectedCategory > 0) {
		cbSection.DisableDraw();
		for (short i = 0; i < (short)categories.Get(selectedCategory - 1).sections.GetSize(); i++) {
			PogoSection section = categories.Get(selectedCategory - 1).sections.Get(i);

			cbSection.InsertItem(i + 1);
			cbSection.SetItemText(i + 1, section.title);
		}
		cbSection.EnableDraw();
		cbSection.Redraw();
	}

	UpdateItems();
}

void PogoItemSelectDialog::UpdateItems()
{
	for (short i = lsItems.GetItemCount(); i > 0; i--) {
		lsItems.DeleteItem(i);
	}
	items.Clear();

	short selectedCategory = cbCategory.GetSelectedItem();
	short selectedSection = cbSection.GetSelectedItem();

	if (selectedSection > 0) {
		cbCategory.Disable();
		cbSection.Disable();
		btnOk.Disable();
		btnCancel.Disable();

		if (items.FetchBySection(categories.Get(selectedCategory - 1).sections.Get(selectedSection - 1).id)) {
			for (short i = 0; i < (short)items.GetSize(); i++) {
				lsItems.InsertItem(i + 1);
				lsItems.SetTabItemText(i + 1, 1, "");
				lsItems.SetTabItemText(i + 1, 2, items.Get(i).descript);
			}
		}

		btnCancel.Enable();
		btnOk.Enable();
		cbSection.Enable();
		cbCategory.Enable();
	}
}

void PogoItemSelectDialog::PanelResized(const DG::PanelResizeEvent& ev)
{
	short vGrow = ev.GetVerticalChange();
	short hGrow = ev.GetHorizontalChange();
	if (vGrow != 0 || hGrow != 0) {
		BeginMoveResizeItems();

		btnOk.Move(0, vGrow);
		btnCancel.Move(0, vGrow);
		lsItems.Resize(hGrow, vGrow);

		cbCategory.Resize(hGrow, 0);
		cbSection.Resize(hGrow, 0);

		EndMoveResizeItems();

		ItemListUpdateColumns();
		lsItems.Redraw();
	}
}

void PogoItemSelectDialog::ListBoxTabFieldUpdate(const DG::ListBoxTabItemUpdateEvent& ev)
{
	short item = ev.GetListItem();
	if (item < 1)
		return;

	if (ev.GetSource() == &lsItems) {
		NewDisplay::ListBoxUpdateEventContext context(ev);

		short width = ev.GetWidth();
		short height = ev.GetHeight();

		TE::Font fontPlain;
		DG::GetFont(DG::Font::Size::Large, DG::Font::Plain, &fontPlain);

		switch (ev.GetTabFieldIndex()) {
			case 2:
				if (item % 2 && lsItems.GetSelectedItem() != item) {
					context.FillRect(0, 0, (float)width - 1, (float)height - 1, 250, 250, 250);
				}

				context.SetForeColor(Gfx::Color::Black);
				//context.FrameRect(0, 0, (float)width - 1, (float)height - 1);
				context.DrawPlainMLTextSafe(items.Get(item-1).descript, fontPlain, 0, 0, 0, width - 1, height - 1, 0);
				break;
			default:
				break;
		}
	}
}
