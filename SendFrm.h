//---------------------------------------------------------------------------
#ifndef SendFrmH
#define SendFrmH
//---------------------------------------------------------------------------
#include <ActnList.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ImgList.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include "sButton.hpp"
#include "sMemo.hpp"
#include "sComboBoxes.hpp"
#include "acAlphaImageList.hpp"
#include "sComboBox.hpp"
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
	void __fastcall aExitExecute(TObject *Sender);
	void __fastcall aSelectMemoExecute(TObject *Sender);
	void __fastcall SendButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall StateComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
public:		// User declarations
	UnicodeString JID;
	int UserIdx;
	__fastcall TSendForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSendForm *SendForm;
//---------------------------------------------------------------------------
#endif
