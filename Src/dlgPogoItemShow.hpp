// *****************************************************************************
// Header file for the Pogo Item Show Dialog
// API Development Kit 19; Mac/Win
//
// Namespaces:        Contact person:
//     -None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

#if !defined (POGO_ITEMSHOW_DIALOG_H)
#define POGO_ITEMSHOW_DIALOG_H

#pragma once

#include	"DG.h"
#include	"DGModule.hpp"
#include	"PogoItemsList.hpp"
#include	"PogoItem.hpp"
#include	"PogoQtiesList.hpp"

// --- Pogo Item Show Modal Dialog ------------------------------------------------------------ 

class PogoItemShowDialog: public DG::ModalDialog,
	public DG::PanelObserver,
	public DG::ButtonItemObserver,
	public DG::ListBoxObserver,
	public DG::CompoundItemObserver
{ 

protected: 

	enum Controls {
		lblCategorySectionId	= 2,
		lblItemDescriptionId	= 3,
		btnOkId					= 1,
		lblItemUnitId			= 4,
		lblItemQtyId			= 5,
		lblItemPriceId			= 6,
		lblItemTotalId			= 7,
		lsQtiesId				= 8,
		btnZoomId				= 9,
		btnCheckId				= 10,
		btnSyncId				= 11,
		btnDeleteId				= 12,
	}; 

	DG::LeftText lblCategorySection;
	DG::LeftText lblItemDescription;
	DG::Button  btnOk;
	DG::Button  btnZoom;
	DG::Button	btnCheck;
	DG::Button	btnSync;
	DG::Button	btnDelete;
	DG::MultiSelListBox lsQties;
	DG::RightText lblItemUnit;
	DG::RightText lblItemQty;
	DG::RightText lblItemPrice;
	DG::RightText lblItemTotal;

	PogoSettings pogoSettings;
	PogoItem item;
	PogoQtiesList qties;

	virtual void	PanelOpened(const DG::PanelOpenEvent& ev) override;
	virtual void    PanelResized(const DG::PanelResizeEvent& ev) override;
	virtual void	PanelHotkeyPressed(const DG::PanelHotKeyEvent& ev, bool* processed) override;
	virtual void	ButtonClicked(const DG::ButtonClickEvent& ev)	override;
public:
	PogoItemShowDialog(PogoItem AItem);
	~PogoItemShowDialog();

	void SetSelectedQty(PogoQtyData qty);
private:
	short KeyID_ESC;

	void QtiesListUpdateColumns();
	void UpdateQtiesList();
	void ZoomToElement();
	void CheckElements();
	void SyncElements();
	void DeleteQties();
}; 

#endif // POGO_ITEMSHOW_DIALOG_H