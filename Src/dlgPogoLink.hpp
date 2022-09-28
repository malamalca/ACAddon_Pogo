// *****************************************************************************
// Header file for the Pogo Data Dialog
// API Development Kit 19; Mac/Win
//
// Namespaces:        Contact person:
//     -None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

#if !defined (POGO_LINK_DIALOG_H)
#define POGO_LINK_DIALOG_H

#pragma once

#include	"DG.h"
#include	"DGModule.hpp"
#include	"PogoTypes.h"
#include	"PogoElementsList.hpp"

// --- Pogo Link Modal Dialog ------------------------------------------------------------ 

class PogoLinkDialog: public DG::ModalDialog,
	public DG::PanelObserver,
	public DG::ButtonItemObserver
{ 

protected: 

	enum Controls {
		btnOkId       = 5,
		btnCancelId   = 6,
		edtSearchId    = 2,
		memoItemId = 4
	}; 

	PogoElementsList _elements;

	DG::Button btnOk;
	DG::Button btnCancel;
	DG::TextEdit edtSearch;
	DG::MultiLineEdit edtFormula;

	virtual void	ButtonClicked(const DG::ButtonClickEvent& ev)	override;

public:

	PogoLinkDialog(PogoElementsList elements);
	~PogoLinkDialog();
}; 

#endif // POGO_LINK_DIALOG_H