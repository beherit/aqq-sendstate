//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SendFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "acAlphaImageList"
#pragma link "sButton"
#pragma link "sComboBox"
#pragma link "sMemo"
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TSendForm *SendForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetThemeSkinDir();
__declspec(dllimport)bool ChkSkinEnabled();
__declspec(dllimport)bool ChkThemeAnimateWindows();
__declspec(dllimport)bool ChkThemeGlowing();
__declspec(dllimport)void SendXML(UnicodeString JID, int UserIdx, UnicodeString Status, int State);
__declspec(dllimport)UnicodeString GetIconPath(int Icon);
__declspec(dllimport)UnicodeString GetStatus(int UserIdx);
__declspec(dllimport)int GetState(int UserIdx);
//---------------------------------------------------------------------------
__fastcall TSendForm::TSendForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::WMTransparency(TMessage &Message)
{
  Application->ProcessMessages();
  sSkinProvider->BorderForm->UpdateExBordersPos(true,(int)Message.LParam);
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::aExitExecute(TObject *Sender)
{
  //Zamkniecie formy
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::FormCreate(TObject *Sender)
{
  //Wlaczona zaawansowana stylizacja okien
  if(ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	//Plik zaawansowanej stylizacji okien istnieje
	if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  if(ChkThemeAnimateWindows()) sSkinManager->AnimEffects->FormShow->Time = 200;
	  else sSkinManager->AnimEffects->FormShow->Time = 0;
	  sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
	  sSkinManager->Active = true;
	}
	//Brak pliku zaawansowanej stylizacji okien
	else sSkinManager->Active = false;
  }
  //Zaawansowana stylizacja okien wylaczona
  else sSkinManager->Active = false;
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::FormShow(TObject *Sender)
{
  //Wczytanie ikon z interfesju AQQ
  sAlphaImageList->AcBeginUpdate();
  sAlphaImageList->Clear();
  sAlphaImageList->LoadFromFile(GetIconPath(1));
  sAlphaImageList->LoadFromFile(GetIconPath(7));
  sAlphaImageList->LoadFromFile(GetIconPath(3));
  sAlphaImageList->LoadFromFile(GetIconPath(4));
  sAlphaImageList->LoadFromFile(GetIconPath(5));
  sAlphaImageList->LoadFromFile(GetIconPath(2));
  sAlphaImageList->LoadFromFile(GetIconPath(49));
  //Ustawienie ikonki na przycisku przywracania statusu
  ResetButton->Images = sAlphaImageList;
  ResetButton->ImageIndex = 6;
  //Ustawianie fokusa na polu wpisywania opisu
  StatusMemo->SetFocus();
  StatusMemo->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::aSelectMemoExecute(TObject *Sender)
{
  //Zaznaczenie calego tekstu
  StatusMemo->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::SendButtonClick(TObject *Sender)
{
  //Wyslanie nowego statusu
  SendXML(JID,UserIdx,StatusMemo->Text.Trim(),StateComboBox->ItemIndex);
  //Zamkniecie formy
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::StateComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
		  TOwnerDrawState State)
{
  //Dodanie ikonek do ComboBox
  if(State.Contains(odSelected)) StateComboBox->Canvas->DrawFocusRect(Rect);
  StateComboBox->Canvas->Brush->Style = bsClear;
  sAlphaImageList->Draw(StateComboBox->Canvas,Rect.left+2,Rect.top+2,Index);
  StateComboBox->Canvas->TextOutW(Rect.left+22,Rect.top+3,StateComboBox->Items->Strings[Index]);
}
//---------------------------------------------------------------------------

void __fastcall TSendForm::ResetButtonClick(TObject *Sender)
{
  //Przywracanie aktualnego opisu
  StatusMemo->Text = GetStatus(UserIdx);
  //Przywracanie aktualnego statusu
  StateComboBox->ItemIndex = GetState(UserIdx);
}
//---------------------------------------------------------------------------
