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
#include	"ResourceIDs.h"

#include	"PogoTypes.h"
#include    "PogoItemsList.hpp"

#include	"ShowMessage.hpp"
#include	"dlgPogoQtyFormula.hpp" 


//---------------------------- Class PogoDataDialog -----------------------

PogoQtyFormulaDialog::PogoQtyFormulaDialog() :
	DG::ModalDialog		(ACAPI_GetOwnResModule (), ID_POGO_QTYFORMULA_MODAL, ACAPI_GetOwnResModule ()),

	btnOk				(GetReference (), btnOkId),
	btnCancel			(GetReference (), btnCancelId),
	edtFormula			(GetReference (), edtFormulaId),
	edtQtyDescript		(GetReference (), edtQtyDescriptId),
	edtCalculatedValue  (GetReference (), edtCalculatedValueId),
	btnRecalc			(GetReference (), btnRecalcId),
	qty_id				("")
{
	AttachToAllItems(*this);
	Attach (*this);
}

PogoQtyFormulaDialog::~PogoQtyFormulaDialog()
{
	Detach(*this);
	DetachFromAllItems(*this);
}

bool PogoQtyFormulaDialog::GetPogoQty(PogoQtyData& qty)
{
	strncpy(qty.descript, edtQtyDescript.GetText().ToCStr().Get(), 100);
	strncpy(qty.formula, edtFormula.GetText().ToCStr().Get(), 100);

	return true;
}

void PogoQtyFormulaDialog::SetPogoQty(PogoQtyData& qty)
{
	edtQtyDescript.SetText(qty.descript);
	edtFormula.SetText(qty.formula);
	qty_id = qty.qty_id;
}

void PogoQtyFormulaDialog::SetPogoElement(PogoElementWithData& element)
{
	this->element = element;
	this->edtCalculatedValue.SetText(GS::UniString::Printf("%.2f", this->element.ParseFormula(edtFormula.GetText())));
}

void PogoQtyFormulaDialog::RecalculateQtyValue()
{
	double value;
	if (element.ParseFormula(edtFormula.GetText(), value)) {
		edtCalculatedValue.SetText(GS::UniString::Printf("%.2f", value));
	}
	else {
		edtCalculatedValue.SetText("Invalid Formula");
	}
}

void PogoQtyFormulaDialog::PanelOpened(const DG::PanelOpenEvent& ev)
{
	SetClientSize(GetOriginalClientWidth(), GetOriginalClientHeight());
	edtCalculatedValue.Disable();
}

void PogoQtyFormulaDialog::PanelResized(const DG::PanelResizeEvent& ev)
{
	short vGrow = ev.GetVerticalChange();
	if (vGrow != 0) {
		BeginMoveResizeItems();
		btnOk.Move(0, vGrow);
		btnCancel.Move(0, vGrow);
		EndMoveResizeItems();
	}
}

void PogoQtyFormulaDialog::PanelCloseRequested(const DG::PanelCloseRequestEvent& ev, bool* accepted)
{
	if (ev.IsAccepted()) {
		if (edtFormula.GetText() == "") {
			*accepted = false;
			ShowMessage("Please enter formula!");

			return;
		}

		double value;
		if (!element.ParseFormula(edtFormula.GetText(), value)) {
			*accepted = false;
			ShowMessage("Invalid formula!");

			return;
		}
	}
}
void PogoQtyFormulaDialog::ButtonClicked(const DG::ButtonClickEvent& ev)
{	
	if (ev.GetSource() == &btnCancel)
		PostCloseRequest(DG::ModalDialog::Cancel);

	if (ev.GetSource() == &btnOk) {
		PostCloseRequest(Accept);
	}

	if (ev.GetSource() == &btnRecalc) {
		RecalculateQtyValue();
	}
}
