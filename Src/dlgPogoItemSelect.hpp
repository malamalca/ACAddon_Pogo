// *****************************************************************************
// Header file for the Pogo Item Select Dialog
// API Development Kit 19; Mac/Win
//
// Namespaces:        Contact person:
//     -None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

#if !defined (POGO_ITEMSELECT_DIALOG_H)
#define POGO_ITEMSELECT_DIALOG_H

#pragma once

#include	"DG.h"
#include	"DGModule.hpp"
#include	"PogoSettings.hpp"
#include	"PogoElementsList.hpp"
#include	"PogoCategoriesList.hpp"
#include	"PogoItemsList.hpp"
#include	"PogoItem.hpp"

// --- Pogo Item Select Modal Dialog ------------------------------------------------------------ 

class PogoItemSelectDialog: public DG::ModalDialog,
	public DG::PanelObserver,
	public DG::ButtonItemObserver,
	public DG::PopUpObserver,
	public DG::ListBoxObserver,
	public DG::CompoundItemObserver
{ 

protected: 

	enum Controls {
		cbCategoryId	= 2,
		cbSectionId		= 4,
		lsItemsId		= 6,
		btnOkId			= 7,
		btnCancelId		= 8,
	}; 

	DG::Button  btnOk;
	DG::Button  btnCancel;
	DG::PopUp   cbCategory;
	DG::PopUp   cbSection;
	DG::SingleSelListBox lsItems;

	PogoSettings pogoSettings;

	PogoCategoriesList categories;
	PogoItemsList items;

	virtual void	PanelOpened(const DG::PanelOpenEvent& ev) override;
	virtual void	PopUpChanged(const DG::PopUpChangeEvent& ev) override;
	virtual void    PanelResized(const DG::PanelResizeEvent& ev) override;
	virtual void	ListBoxTabFieldUpdate(const DG::ListBoxTabItemUpdateEvent& ev) override;
	virtual void	ListBoxDoubleClicked(const DG::ListBoxDoubleClickEvent& ev) override;
	virtual void	ButtonClicked(const DG::ButtonClickEvent& ev)	override;

	void UpdateCategories();
	void UpdateSections();
	void UpdateItems();

public:
	PogoItemSelectDialog();
	~PogoItemSelectDialog();

	bool GetSelectedPogoItem(PogoItem& item);
private:
	void ItemListUpdateColumns();
}; 

#endif // POGO_ITEMSELECT_DIALOG_H