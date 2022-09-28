// *****************************************************************************
// Header file for the Pogo Qty Formula Dialog
// API Development Kit 19; Mac/Win
//
// Namespaces:        Contact person:
//     -None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

#if !defined (POGO_SETTINGS_DIALOG_H)
#define POGO_SETTINGS_DIALOG_H

#pragma once

#include	"DG.h"
#include	"DGModule.hpp"
#include	"DGPanel.hpp"


// --- Pogo Settings Modal Dialog ------------------------------------------------------------ 
class PogoSettingsDialog : public DG::ModalDialog,
	public DG::PanelObserver,
	public DG::ButtonItemObserver,
	public DG::CompoundItemObserver
{ 

protected: 

	enum Controls {
		edtProjectIdId		= 2,
		edtHostId			= 4,
		edtUsernameId		= 6,
		edtPasswordId		= 8,
		btnOkId				= 9,
		btnCancelId			= 10,
	}; 

	DG::TextEdit edtProjectId;
	DG::TextEdit edtHost;
	DG::TextEdit edtUsername;
	DG::TextEdit edtPassword;
	DG::Button  btnOk;
	DG::Button  btnCancel;	

	PogoSettings	pogoSettings;

	virtual void	PanelOpened(const DG::PanelOpenEvent& ev) override;
	virtual void	PanelCloseRequested(const DG::PanelCloseRequestEvent& ev, bool* accepted) override;
	virtual void	PanelClosed(const DG::PanelCloseEvent& ev);

	virtual void    PanelResized(const DG::PanelResizeEvent& ev) override;
	virtual void	ButtonClicked(const DG::ButtonClickEvent& ev)	override;

public:

	PogoSettingsDialog();
	~PogoSettingsDialog();

	const PogoSettings& GetPogoSettings() const;
}; 

#endif // POGO_SETTINGS_DIALOG_H