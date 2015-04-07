//---------------------------------------------------------------------------
// Copyright (C) 2010-2015 Krzysztof Grochocki
//
// This file is part of SendState
//
// SendState is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// SendState is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Radio. If not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#include <LangAPI.hpp>
#pragma hdrstop
#include "SendStateFrm.h"
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
TSendStateForm *SendStateForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetThemeSkinDir();
__declspec(dllimport)bool ChkSkinEnabled();
__declspec(dllimport)bool ChkThemeAnimateWindows();
__declspec(dllimport)bool ChkThemeGlowing();
__declspec(dllimport)int GetHUE();
__declspec(dllimport)int GetSaturation();
__declspec(dllimport)int GetBrightness();
__declspec(dllimport)void SendXML(UnicodeString JID, int UserIdx, UnicodeString Status, int State);
__declspec(dllimport)UnicodeString GetIconPath(int Icon);
__declspec(dllimport)UnicodeString GetStatus(int UserIdx);
__declspec(dllimport)int GetState(int UserIdx);
//---------------------------------------------------------------------------
__fastcall TSendStateForm::TSendStateForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TSendStateForm::WMTransparency(TMessage &Message)
{
	Application->ProcessMessages();
	if(sSkinManager->Active) sSkinProvider->BorderForm->UpdateExBordersPos(true, (int)Message.LParam);
}
//---------------------------------------------------------------------------

void __fastcall TSendStateForm::aExitExecute(TObject *Sender)
{
	//Zamkniecie formy
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TSendStateForm::FormCreate(TObject *Sender)
{
	//Lokalizowanie formy
	LangForm(this);
	//Wlaczona zaawansowana stylizacja okien
	if(ChkSkinEnabled())
	{
		UnicodeString ThemeSkinDir = GetThemeSkinDir();
		//Plik zaawansowanej stylizacji okien istnieje
		if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
		{
			//Dane pliku zaawansowanej stylizacji okien
			ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
			sSkinManager->SkinDirectory = ThemeSkinDir;
			sSkinManager->SkinName = "Skin.asz";
			//Ustawianie animacji AlphaControls
			if(ChkThemeAnimateWindows()) sSkinManager->AnimEffects->FormShow->Time = 200;
			else sSkinManager->AnimEffects->FormShow->Time = 0;
			sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
			//Zmiana kolorystyki AlphaControls
			sSkinManager->HueOffset = GetHUE();
			sSkinManager->Saturation = GetSaturation();
			sSkinManager->Brightness = GetBrightness();
			//Aktywacja skorkowania AlphaControls
			sSkinManager->Active = true;
		}
		//Brak pliku zaawansowanej stylizacji okien
		else sSkinManager->Active = false;
	}
	//Zaawansowana stylizacja okien wylaczona
	else sSkinManager->Active = false;
}
//---------------------------------------------------------------------------

void __fastcall TSendStateForm::FormShow(TObject *Sender)
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

void __fastcall TSendStateForm::aSelectMemoExecute(TObject *Sender)
{
	//Zaznaczenie calego tekstu
	StatusMemo->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TSendStateForm::SendButtonClick(TObject *Sender)
{
	//Wyslanie nowego statusu
	SendXML(JID, UserIdx, StatusMemo->Text.Trim(), StateComboBox->ItemIndex);
	//Zamkniecie formy
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TSendStateForm::StateComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
			TOwnerDrawState State)
{
	StateComboBox->Canvas->Brush->Style = bsClear;
	sAlphaImageList->Draw(StateComboBox->Canvas, Rect.Left+3, Rect.Top+3, Index);
	StateComboBox->Canvas->TextOutW(Rect.Left+22, Rect.Top+4, StateComboBox->Items->Strings[Index]);
}
//---------------------------------------------------------------------------

void __fastcall TSendStateForm::ResetButtonClick(TObject *Sender)
{
	//Przywracanie aktualnego opisu
	StatusMemo->Text = GetStatus(UserIdx);
	//Przywracanie aktualnego statusu
	StateComboBox->ItemIndex = GetState(UserIdx);
}
//---------------------------------------------------------------------------

void __fastcall TSendStateForm::sSkinManagerSysDlgInit(TacSysDlgData DlgData, bool &AllowSkinning)
{
	AllowSkinning = false;
}
//---------------------------------------------------------------------------
