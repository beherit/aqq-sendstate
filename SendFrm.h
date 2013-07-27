//---------------------------------------------------------------------------
#ifndef SendFrmH
#define SendFrmH
//---------------------------------------------------------------------------
#include "LMDCustomComboBox.hpp"
#include "LMDCustomImageComboBox.hpp"
#include "LMDImageComboBox.hpp"
#include "LMDThemedComboBox.hpp"
#include <ActnList.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ImgList.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include "LMDCustomImageList.hpp"
#include "LMDGraphicList.hpp"
//---------------------------------------------------------------------------
class TSendForm : public TForm
{
__published:	// IDE-managed Components
	TLMDImageComboBox *StateLMDImageComboBox;
	TMemo *StatusMemo;
	TButton *SendButton;
	TActionList *ActionList;
	TAction *aExit;
	TAction *aChangeIcons;
	TAction *aSelectMemo;
	TLMDGraphicList *LMDGraphicList;
	void __fastcall aExitExecute(TObject *Sender);
	void __fastcall aChangeIconsExecute(TObject *Sender);
	void __fastcall aSelectMemoExecute(TObject *Sender);
	void __fastcall SendButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
public:		// User declarations
	UnicodeString eJID;
	int eUserIdx;
	__fastcall TSendForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSendForm *SendForm;
//---------------------------------------------------------------------------
#endif
