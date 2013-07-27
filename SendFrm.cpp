//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SendFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "LMDCustomComboBox"
#pragma link "LMDCustomImageComboBox"
#pragma link "LMDImageComboBox"
#pragma link "LMDThemedComboBox"
#pragma link "LMDCustomImageList"
#pragma link "LMDGraphicList"
#pragma resource "*.dfm"
TSendForm *SendForm;
//---------------------------------------------------------------------------
__declspec(dllimport)void SendXML(UnicodeString JID, int State, UnicodeString Status, int UserIdx);
__declspec(dllimport)UnicodeString GetIconPath(int Icon);
//---------------------------------------------------------------------------
__fastcall TSendForm::TSendForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::aExitExecute(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::aChangeIconsExecute(TObject *Sender)
{
  LMDGraphicList->Graphics->Items[0]->Picture->LoadFromFile(GetIconPath(1));
  LMDGraphicList->Graphics->Items[1]->Picture->LoadFromFile(GetIconPath(7));
  LMDGraphicList->Graphics->Items[2]->Picture->LoadFromFile(GetIconPath(3));
  LMDGraphicList->Graphics->Items[3]->Picture->LoadFromFile(GetIconPath(4));
  LMDGraphicList->Graphics->Items[4]->Picture->LoadFromFile(GetIconPath(5));
  LMDGraphicList->Graphics->Items[5]->Picture->LoadFromFile(GetIconPath(2));
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::aSelectMemoExecute(TObject *Sender)
{
  StatusMemo->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::SendButtonClick(TObject *Sender)
{
  SendXML(eJID,StateLMDImageComboBox->ItemIndex,StatusMemo->Text.Trim(),eUserIdx);
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::FormShow(TObject *Sender)
{
  aChangeIcons->Execute();
}
//---------------------------------------------------------------------------

