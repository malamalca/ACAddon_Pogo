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
	}; 

	DG::LeftText lblCategorySection;
	DG::LeftText lblItemDescription;
	DG::Button  btnOk;
	DG::Button  btnZoom;
	DG::SingleSelListBox lsQties;
	DG::RightText lblItemUnit;
	DG::RightText lblItemQty;
	DG::RightText lblItemPrice;
	DG::RightText lblItemTotal;

	PogoSettings pogoSettings;
	PogoItem item;
	PogoQtiesList qties;

	virtual void	PanelOpened(const DG::PanelOpenEvent& ev) override;
	virtual void    PanelResized(const DG::PanelResizeEvent& ev) override;
	virtual void	ButtonClicked(const DG::ButtonClickEvent& ev)	override;
public:
	PogoItemShowDialog(PogoItem AItem);
	~PogoItemShowDialog();
private:
	void QtiesListUpdateColumns();
	void UpdateQtiesList();
	void ZoomToElement();
}; 

#endif // POGO_ITEMSHOW_DIALOG_H