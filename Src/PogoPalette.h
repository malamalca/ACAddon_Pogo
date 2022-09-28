// ---------------------------------- Includes ---------------------------------  
  
#include   "APIEnvir.h"  
#include   "ACAPinc.h"  
#include   "DG.h"  
#include   "DGModule.hpp"  
  
// --- PogoPalette ----------------------------------------------------------  
  
class PogoPalette:   public   DG::Palette,
            public   DG::PanelObserver,  
            public   DG::ButtonItemObserver,  
            public   DG::CompoundItemObserver
{  
private: 
   enum {  
	   LblNumElementsId = 1,
	   CreateAtAngleButtonId = 2,
	   CreateLevelWith2PButtonId = 3,
	   CreateLevelWith3PButtonId = 4
   };  
  
   DG::LeftText      lblNumElements;  
   //DG::Button	createLevelButton;
   //DG::Button	createAtAngleButton;
   //DG::Button	createWith2P;
   //DG::Button	createWith3P;
  
   PogoPalette   ();
  
protected:  
   virtual void   PanelOpened      (const DG::PanelOpenEvent& ev) override;  
   virtual void   PanelResized      (const DG::PanelResizeEvent& ev) override;  
   virtual void   PanelCloseRequested   (const DG::PanelCloseRequestEvent& ev, bool* accept) override;  
  
   virtual void   ButtonClicked      (const DG::ButtonClickEvent& ev) override;  
  
public:  

	virtual ~PogoPalette();

	virtual void SelectionChange(const API_Neig* selElemNeig);

	static GSErrCode PaletteAPIControlCallBack(Int32 referenceID, API_PaletteMessageID messageID, GS::IntPtr param);
	static Int32			PogoPaletteRefId();
	static const GS::Guid& PogoPaletteGuid();

	static PogoPalette& GetInstance(); 
};  