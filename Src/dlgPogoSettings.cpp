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

#include	"PogoSettings.hpp"
#include	"dlgPogoSettings.hpp" 


//---------------------------- Class PogoDataDialog -----------------------

PogoSettingsDialog::PogoSettingsDialog() :
	DG::ModalDialog		(ACAPI_GetOwnResModule (), ID_POGO_SETTINGS_MODAL, ACAPI_GetOwnResModule ()),

	btnOk				(GetReference (), btnOkId),
	btnCancel			(GetReference (), btnCancelId),
	edtProjectId		(GetReference (), edtProjectIdId),
	edtHost				(GetReference (), edtHostId),
	edtUsername			(GetReference (), edtUsernameId),
	edtPassword			(GetReference (), edtPasswordId)
{
	AttachToAllItems(*this);
	Attach (*this);
}

PogoSettingsDialog::~PogoSettingsDialog()
{
	Detach(*this);
	DetachFromAllItems(*this);

	
}

const PogoSettings& PogoSettingsDialog::GetPogoSettings() const
{
	return pogoSettings;
}

void PogoSettingsDialog::PanelOpened(const DG::PanelOpenEvent& ev)
{
	SetClientSize(GetOriginalClientWidth(), GetOriginalClientHeight());

	PogoSettings::LoadPogoSettingsFromPreferences(pogoSettings);

	edtProjectId.SetText(pogoSettings.ProjectId);
	edtHost.SetText(pogoSettings.Host);
	edtUsername.SetText(pogoSettings.Username);
	edtPassword.SetText(pogoSettings.Password);
}

void PogoSettingsDialog::PanelCloseRequested(const DG::PanelCloseRequestEvent& ev, bool* accepted)
{
}

void PogoSettingsDialog::PanelClosed(const DG::PanelCloseEvent& ev)
{
	pogoSettings.ProjectId = edtProjectId.GetText();
	pogoSettings.Host = edtHost.GetText();
	pogoSettings.Username = edtUsername.GetText();
	pogoSettings.Password = edtPassword.GetText();

	PogoSettings::WritePogoSettingsToPreferences(pogoSettings);
}

void PogoSettingsDialog::PanelResized(const DG::PanelResizeEvent& ev)
{
	short vGrow = ev.GetVerticalChange();
	if (vGrow != 0) {
		BeginMoveResizeItems();
		btnOk.Move(0, vGrow);
		btnCancel.Move(0, vGrow);
		EndMoveResizeItems();
	}
}

void PogoSettingsDialog::ButtonClicked(const DG::ButtonClickEvent& ev)
{	
	if (ev.GetSource() == &btnCancel)
		PostCloseRequest(DG::ModalDialog::Cancel);

	if (ev.GetSource() == &btnOk) {
		PostCloseRequest(Accept);
	}
}
