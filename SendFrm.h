//---------------------------------------------------------------------------
#ifndef SendFrmH
#define SendFrmH
#define WM_ALPHAWINDOWS (WM_USER + 666)
//---------------------------------------------------------------------------
#include "acAlphaImageList.hpp"
#include "sButton.hpp"
#include "sComboBox.hpp"
#include "sMemo.hpp"
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include "sSpeedButton.hpp"
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
class TSendForm : public TForm
{
__published:	// IDE-managed Components
	TsMemo *StatusMemo;
	TsButton *SendButton;
	TActionList *ActionList;
	TAction *aExit;
	TAction *aSelectMemo;
	TsSkinManager *sSkinManager;
	TsSkinProvider *sSkinProvider;
	TsAlphaImageList *sAlphaImageList;
	TsComboBox *StateComboBox;
	TsSpeedButton *ResetButton;
	void __fastcall aExitExecute(TObject *Sender);
	void __fastcall aSelectMemoExecute(TObject *Sender);
	void __fastcall SendButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall StateComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall ResetButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	UnicodeString JID;
	int UserIdx;
	__fastcall TSendForm(TComponent* Owner);
	void __fastcall WMTransparency(TMessage &Message);
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_ALPHAWINDOWS,TMessage,WMTransparency);
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TSendForm *SendForm;
//---------------------------------------------------------------------------
#endif
