// ---------------------------------- Includes ---------------------------------  
  
#include   "PogoPalette.h" 
#include    "ResourceIDs.h"
  
//------------------------------ Class PogoPalette --------------------------  
 

const GS::Guid& PogoPalette::PogoPaletteGuid() {
    // We need a fix and unique GUID to construct the palette to make it dockable.
    static GS::Guid guid("B69AED35-3C1E-4246-8598-3C5A4013FD40");
    return guid;
}

Int32 PogoPalette::PogoPaletteRefId() {
    static Int32 refId(GS::CalculateHashValue(PogoPaletteGuid()));
    return refId;
}

GSErrCode __ACENV_CALL	PogoPalette::PaletteAPIControlCallBack(Int32 referenceID, API_PaletteMessageID messageID, GS::IntPtr param)
{
    GSErrCode	err = NoError;
    if (referenceID == PogoPalette::PogoPaletteRefId()) {
        PogoPalette& palette = PogoPalette::GetInstance();
        switch (messageID) {
        case APIPalMsg_OpenPalette:
            palette.Show();
            break;

        case APIPalMsg_ClosePalette:
            palette.SendCloseRequest();
            break;

        case APIPalMsg_HidePalette_Begin:
            palette.Hide();
            break;

        case APIPalMsg_HidePalette_End:
            palette.Show();
            break;

        case APIPalMsg_DisableItems_Begin:
            palette.DisableItems();
            break;

        case APIPalMsg_DisableItems_End:
            palette.EnableItems();
            break;

        case APIPalMsg_IsPaletteVisible:
            (*reinterpret_cast<bool*> (param)) = palette.IsVisible();
            break;

        default:
            break;
        }
    }

    return err;
}

//------------------------ Class PogoPalette -----------------------

PogoPalette::PogoPalette() :
    DG::Palette(ACAPI_GetOwnResModule(), ID_POGO_PALETTE, ACAPI_GetOwnResModule(), PogoPaletteGuid()),
    
    lblNumElements(GetReference (), LblNumElementsId)
   //createAtAngleButton   (GetReference (), CreateAtAngleButtonId),
   //createWith2P   (GetReference (), CreateLevelWith2PButtonId),
   //createWith3P   (GetReference (), CreateLevelWith3PButtonId)
{  
    // init observer
     this->Attach(*this);
     AttachToAllItems(*this);

     this->BeginEventProcessing();
}  
  
  
PogoPalette::~PogoPalette ()
{  
    this->EndEventProcessing();
    this->Detach(*this);
    DetachFromAllItems(*this);
}

PogoPalette& PogoPalette::GetInstance()
{
    static PogoPalette instance;
    return instance;
}

  
  
void PogoPalette::PanelOpened (const DG::PanelOpenEvent& /*ev*/)
{  
    SetClientSize(GetOriginalClientWidth(), GetOriginalClientHeight());
}  
  
  
void PogoPalette::PanelResized (const DG::PanelResizeEvent& ev)
{  
    short hGrow = ev.GetHorizontalChange();
    short vGrow = ev.GetVerticalChange();
    if (vGrow != 0 || hGrow != 0) {
        BeginMoveResizeItems();

        //okButton.Move(hGrow, vGrow);
        //switchImagesButton.Move(hGrow, vGrow);
        //getButton.Move(hGrow, vGrow);
        //listBox.Resize(hGrow, vGrow);

        EndMoveResizeItems();
    }
}  
  
  
void PogoPalette::PanelCloseRequested (const DG::PanelCloseRequestEvent& ev, bool* /*accept*/)
{  
    Hide();
}  
  
  
void PogoPalette::ButtonClicked (const DG::ButtonClickEvent& ev)
{  
	/*if (ev.GetSource() == &createLevelButton) {
		ToolboxSelectLand4Level ();
	} else if (ev.GetSource () == &createAtAngleButton) {
		Do_CreateLevelAtFixedAngle ();
	} else if (ev.GetSource () == &createWith2P) {
		Do_CreateLevelOnLine ();
	}  else if (ev.GetSource () == &createWith3P) {
		Do_CreateLevelOnPlane ();
	}*/
}  


void PogoPalette::SelectionChange(const API_Neig* selElemNeig)
{
    char            msgStr[256];
    GSErrCode		err;
    API_SelectionInfo    selectionInfo;
    GS::Array<API_Neig>  selNeigs;

    if (selElemNeig->neigID != APINeig_None) {
        err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);
        BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);

        if (err == APIERR_NOSEL)
            err = NoError;

        UInt32 nSel = 0;
        if (selectionInfo.typeID != API_SelEmpty) {
            nSel = selectionInfo.sel_nElem;
        }

        sprintf(msgStr, "Selected elements: %d", nSel);

        /*API_ElemTypeID elem_type = API_ZombieElemID;
        ACAPI_Goodies(APIAny_NeigIDToElemTypeID, (void*)&(selElemNeig->neigID), &elem_type);
        if (elem_type == API_WindowID || elem_type == API_DoorID) {

        }*/
    }
    else {
        sprintf(msgStr, "All elements deselected");
    }


    lblNumElements.SetText(msgStr);
}
