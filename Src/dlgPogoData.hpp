// *****************************************************************************
// Header file for the Pogo Data Dialog
// API Development Kit 19; Mac/Win
//
// Namespaces:        Contact person:
//     -None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

#if !defined (POGO_DATA_DIALOG_H)
#define POGO_DATA_DIALOG_H

#pragma once

#include	"DG.h"
#include	"DGModule.hpp"
#include	"PogoTypes.h"
#include	"PogoElementWithData.hpp"
#include	"PogoElementsList.hpp"

// --- Pogo Data Modal Dialog ------------------------------------------------------------ 

class PogoDataDialog: public DG::ModalDialog,
	public DG::PanelObserver,
	public DG::ButtonItemObserver,
	public DG::ListBoxObserver,
	public DG::PopUpObserver,
	public DG::CompoundItemObserver
{ 
protected: 

	enum Controls {
		btnCloseId       = 5,
		cbElementsId     = 2,
		lsQtiesId        = 4,
		btnSyncQtyId     = 6,
		btnDeleteQtyId   = 7,
		btnEditQtyId     = 8,
		btnInfoQtyId     = 9,
	}; 

	PogoElementWithData _element;

	DG::Button btnClose;
	DG::PopUp cbElements;
	DG::MultiSelListBox lsQties;

	DG::Button btnSyncQty;
	DG::Button btnDeleteQty;
	DG::Button btnEditQty;
	DG::Button btnInfoQty;

	virtual void	PanelOpened(const DG::PanelOpenEvent& ev) override;
	virtual void	ButtonClicked(const DG::ButtonClickEvent& ev)	override;
	virtual void	ListBoxSelectionChanged(const DG::ListBoxSelectionEvent& ev) override;
	virtual void	PanelHotkeyPressed(const DG::PanelHotKeyEvent& ev, bool* processed) override;
	virtual void	PopUpChanged(const DG::PopUpChangeEvent& ev) override;

public:
	PogoDataDialog(PogoElementsList selectedElements);
	~PogoDataDialog();

private:
	short KeyID_ESC;
	PogoElementsList selectedElements;

	void UpdateQtiesList();
	void UpdateInterface();
	void UpdateQtyListItem(short listItemIndex, PogoQtyData& qty);

	void OnSyncQtyClick();
	void OnDeleteQtyClick();
	void OnEditQtyClick();
	void OnInfoQtyClick();
}; 

#endif // POGO_DATA_DIALOG_H