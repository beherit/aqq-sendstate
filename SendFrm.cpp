//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SendFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma link "sButton"
#pragma link "sMemo"
#pragma link "sComboBoxes"
#pragma link "acAlphaImageList"
#pragma link "sComboBox"
#pragma resource "*.dfm"
TSendForm *SendForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetThemeSkinDir();
__declspec(dllimport)bool ChkSkinEnabled();
__declspec(dllimport)bool ChkNativeEnabled();
__declspec(dllimport)void SendXML(UnicodeString JID, int State, UnicodeString Status, int UserIdx);
__declspec(dllimport)UnicodeString GetIconPath(int Icon);
bool IconsChange = false;
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

void __fastcall TSendForm::aSelectMemoExecute(TObject *Sender)
{
  StatusMemo->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::SendButtonClick(TObject *Sender)
{
  SendXML(JID,StateComboBox->ItemIndex,StatusMemo->Text.Trim(),UserIdx);
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::FormShow(TObject *Sender)
{
  //AplhaSkins
  if(!ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  sSkinProvider->DrawNonClientArea = false;
	  sSkinManager->Active = true;
	}
	else
	 sSkinManager->Active = false;
  }
  StateComboBox->SetFocus();
  StatusMemo->SetFocus();
  StatusMemo->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::FormCreate(TObject *Sender)
{
  //AlphaSkins
  if(ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  sSkinProvider->DrawNonClientArea = true;
	  sSkinManager->Active = true;
	}
	else
	 sSkinManager->Active = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::StateComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
		  TOwnerDrawState State)
{
  if(!IconsChange)
  {
    sAlphaImageList->AcBeginUpdate();
	sAlphaImageList->Clear();
	sAlphaImageList->LoadFromFile(GetIconPath(1));
	sAlphaImageList->LoadFromFile(GetIconPath(7));
	sAlphaImageList->LoadFromFile(GetIconPath(3));
	sAlphaImageList->LoadFromFile(GetIconPath(4));
	sAlphaImageList->LoadFromFile(GetIconPath(5));
	sAlphaImageList->LoadFromFile(GetIconPath(2));
	IconsChange = true;
  }
  if(State.Contains(odSelected)) StateComboBox->Canvas->DrawFocusRect(Rect);
  StateComboBox->Canvas->Brush->Style = bsClear;
  sAlphaImageList->Draw(StateComboBox->Canvas,Rect.left+2,Rect.top+2,Index);
  StateComboBox->Canvas->TextOutA(Rect.left+22,Rect.top+3,StateComboBox->Items->Strings[Index]);
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::FormPaint(TObject *Sender)
{
  IconsChange = false;	
}
//---------------------------------------------------------------------------

