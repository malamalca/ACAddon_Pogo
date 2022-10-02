// *****************************************************************************
// Header file for the Pogo Qty Formula Dialog
// API Development Kit 19; Mac/Win
//
// Namespaces:        Contact person:
//     -None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

#if !defined (POGO_QTYFORMULA_DIALOG_H)
#define POGO_QTYFORMULA_DIALOG_H

#pragma once

#include	"DG.h"
#include	"DGModule.hpp"
#include	"DGPanel.hpp"
#include	"PogoElementsList.hpp"
#include	"PogoItem.hpp"

// --- Pogo Qty Formula Modal Dialog ------------------------------------------------------------ 
class PogoQtyFormulaDialog : public DG::ModalDialog,
	public DG::PanelObserver,
	public DG::ButtonItemObserver,
	public DG::CompoundItemObserver
{ 

protected: 

	enum Controls {
		btnOkId				= 1,
		btnCancelId			= 2,
		edtQtyDescriptId	= 4,
		edtFormulaId		= 6,
		edtCalculatedValueId= 8,
		btnRecalcId			= 9,
	}; 

	DG::TextEdit edtQtyDescript;
	DG::MultiLineEdit edtFormula;
	DG::TextEdit edtCalculatedValue;
	DG::Button  btnOk;
	DG::Button  btnCancel;	
	DG::Button	btnRecalc;

	virtual void	PanelOpened(const DG::PanelOpenEvent& ev) override;
	virtual void    PanelResized(const DG::PanelResizeEvent& ev) override;
	virtual void	PanelCloseRequested(const DG::PanelCloseRequestEvent& ev, bool* accepted) override;
	
	virtual void	ButtonClicked(const DG::ButtonClickEvent& ev)	override;

public:
	PogoItem item;
	PogoElementWithData element;
	GS::UniString qty_id;

	PogoQtyFormulaDialog();
	~PogoQtyFormulaDialog();

	bool GetPogoQty(PogoQtyData& qty);
	void SetPogoQty(PogoQtyData& qty);
	void SetPogoElement(PogoElementWithData& element);

private:
	void RecalculateQtyValue();
		
}; 

#endif // POGO_QTYFORMULA_DIALOG_H