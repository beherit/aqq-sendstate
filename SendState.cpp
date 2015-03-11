//---------------------------------------------------------------------------
// Copyright (C) 2010-2015 Krzysztof Grochocki
//
// This file is part of SendState
//
// SendState is free software; you can redistribute it and/or modify
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
// along with GNU Radio; see the file COPYING. If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street,
// Boston, MA 02110-1301, USA.
//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <IdHashMessageDigest.hpp>
#include <PluginAPI.h>
#include <LangAPI.hpp>
#pragma hdrstop
#include "SendStateFrm.h"

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
//---------------------------------------------------------------------------

//Struktury-glowne-----------------------------------------------------------
TPluginLink PluginLink;
TPluginInfo PluginInfo;
//Dane-na-temat-kontaktow----------------------------------------------------
UnicodeString ContactJID;
int ContactUserIdx;
int AccountUserIdx;
TCustomIniFile* ChangedStateList = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
//Uchwyt-do-okna-timera------------------------------------------------------
HWND hTimerFrm;
//FORWARD-AQQ-HOOKS----------------------------------------------------------
INT_PTR __stdcall OnContactsUpdate(WPARAM wParam, LPARAM lParam);
INT_PTR __stdcall OnLangCodeChanged(WPARAM wParam, LPARAM lParam);
INT_PTR __stdcall OnStateChange(WPARAM wParam, LPARAM lParam);
INT_PTR __stdcall OnSystemPopUp(WPARAM wParam, LPARAM lParam);
//---------------------------------------------------------------------------

//Pobieranie sciezki katalogu prywatnego wtyczek
UnicodeString GetPluginUserDir()
{
	return StringReplace((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR,0,0), "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
}
//---------------------------------------------------------------------------

//Pobieranie sciezki do skorki kompozycji
UnicodeString GetThemeSkinDir()
{
	return StringReplace((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETTHEMEDIR, 0, 0), "\\", "\\\\", TReplaceFlags() << rfReplaceAll) + "\\\\Skin";
}
//---------------------------------------------------------------------------

//Sprawdzanie czy wlaczona jest zaawansowana stylizacja okien
bool ChkSkinEnabled()
{
	TStrings* IniList = new TStringList();
	IniList->SetText((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP, 0, 0));
	TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
	Settings->SetStrings(IniList);
	delete IniList;
	UnicodeString SkinsEnabled = Settings->ReadString("Settings", "UseSkin", "1");
	delete Settings;
	return StrToBool(SkinsEnabled);
}
//---------------------------------------------------------------------------

//Sprawdzanie ustawien animacji AlphaControls
bool ChkThemeAnimateWindows()
{
	TStrings* IniList = new TStringList();
	IniList->SetText((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP, 0, 0));
	TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
	Settings->SetStrings(IniList);
	delete IniList;
	UnicodeString AnimateWindowsEnabled = Settings->ReadString("Theme", "ThemeAnimateWindows", "1");
	delete Settings;
	return StrToBool(AnimateWindowsEnabled);
}
//---------------------------------------------------------------------------
bool ChkThemeGlowing()
{
	TStrings* IniList = new TStringList();
	IniList->SetText((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP, 0, 0));
	TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
	Settings->SetStrings(IniList);
	delete IniList;
	UnicodeString GlowingEnabled = Settings->ReadString("Theme", "ThemeGlowing", "1");
	delete Settings;
	return StrToBool(GlowingEnabled);
}
//---------------------------------------------------------------------------

//Pobieranie ustawien koloru AlphaControls
int GetHUE()
{
	return (int)PluginLink.CallService(AQQ_SYSTEM_COLORGETHUE, 0, 0);
}
//---------------------------------------------------------------------------
int GetSaturation()
{
	return (int)PluginLink.CallService(AQQ_SYSTEM_COLORGETSATURATION, 0, 0);
}
//---------------------------------------------------------------------------
int GetBrightness()
{
	return (int)PluginLink.CallService(AQQ_SYSTEM_COLORGETBRIGHTNESS, 0, 0);
}
//---------------------------------------------------------------------------

//Kodowanie ciagu znakow do Base64
UnicodeString EncodeBase64(UnicodeString Str)
{
	return (wchar_t*)PluginLink.CallService(AQQ_FUNCTION_BASE64, (WPARAM)Str.w_str(), 3);
}
//---------------------------------------------------------------------------

//Dekodowanie ciagu znakow z Base64
UnicodeString DecodeBase64(UnicodeString Str)
{
	return (wchar_t*)PluginLink.CallService(AQQ_FUNCTION_BASE64, (WPARAM)Str.w_str(), 2);
}
//---------------------------------------------------------------------------

//Pobieranie aktualnego opisu wskazanego konta
UnicodeString GetStatus(int UserIdx)
{
	TPluginStateChange PluginStateChange;
	PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE, (WPARAM)(&PluginStateChange), UserIdx);
	return (wchar_t*)PluginStateChange.Status;
}
//---------------------------------------------------------------------------

//Pobieranie aktualnego stanu wskazanego konta
int GetState(int UserIdx)
{
	//Pobranie aktualnego stanu konta
	TPluginStateChange PluginStateChange;
	PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE, (WPARAM)(&PluginStateChange), UserIdx);
	int State = PluginStateChange.NewState;
	//Zwrocenie odpowiedniego indeksu
	if((State==0)||(State==6))
		return 5;
	else
		return State-1;
}
//---------------------------------------------------------------------------

//Pobieranie wybranego wczesniej opisu
UnicodeString GetChangedStatus(UnicodeString JID, int UserIdx)
{
	return DecodeBase64(ChangedStateList->ReadString("Status:"+IntToStr(UserIdx), JID, EncodeBase64(GetStatus(UserIdx))));
}
//---------------------------------------------------------------------------

//Pobieranie wybranego wczesniej stanu
int GetChangedState(UnicodeString JID, int UserIdx)
{
	return ChangedStateList->ReadInteger("State:"+IntToStr(UserIdx), JID, GetState(UserIdx));
}
//---------------------------------------------------------------------------

//Pobieranie sciezki ikony z interfejsu AQQ
UnicodeString GetIconPath(int Icon)
{
	return StringReplace((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETPNG_FILEPATH, Icon, 0), "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
}
//--------------------------------------------------------------------------

void SendXML(UnicodeString JID, int UserIdx, UnicodeString Status, int State)
{
	//Konwersja specjalnych znakow
	UnicodeString StatusXML = Status;
	StatusXML = (wchar_t*)PluginLink.CallService(AQQ_FUNCTION_CONVERTTOXML, 0, (WPARAM)StatusXML.w_str());
	//Podstawowe zmienne
	UnicodeString XML;
	UnicodeString ShowType;
	//Odkodowanie stanow
	if(State==1)
		ShowType = "chat";
	else if(State==2)
		ShowType = "away";
	else if(State==3)
		ShowType = "xa";
	else if(State==4)
		ShowType = "dnd";
	else if(State==5)
		ShowType = "unavailable";
	//Tworzenie pakietu XML
	if((State!=5)&&(State!=0))
	{
		if(!StatusXML.IsEmpty())
			XML = "<presence to=\"" + JID + "\"><status>" + StatusXML + "</status><show>" + ShowType + "</show></presence>";
		else
			XML = "<presence to=\"" + JID + "\"><status/><show>" + ShowType + "</show></presence>";
	}
	else if(State==5)
	{
		if(!StatusXML.IsEmpty())
			XML = "<presence to=\"" + JID + "\" type=\"" + ShowType + "\"><status>" + StatusXML + "</status></presence>";
		else
			XML = "<presence to=\"" + JID + "\" type=\"" + ShowType+ "\"/>";
	}
	else
	{
		if(!StatusXML.IsEmpty())
			XML = "<presence to=\"" + JID + "\"><status>" + StatusXML + "</status></presence>";
		else
			XML = "<presence to=\"" + JID + "\"/>";
	}
	//Wysylanie pakietu XML
	PluginLink.CallService(AQQ_SYSTEM_SENDXML, (WPARAM)XML.w_str(), UserIdx);
	//Usuniecie zapisanych wczesniej danych
	ChangedStateList->DeleteKey("Idx:"+IntToStr(UserIdx), JID);
	ChangedStateList->DeleteKey("Status:"+IntToStr(UserIdx), JID);
	ChangedStateList->DeleteKey("State:"+IntToStr(UserIdx), JID);
	//Zapisanie wyslanych danych
	if((Status!=GetStatus(UserIdx))||(State!=GetState(UserIdx))) ChangedStateList->WriteBool("Idx:"+IntToStr(UserIdx), JID, true);
	if(Status!=GetStatus(UserIdx)) ChangedStateList->WriteString("Status:"+IntToStr(UserIdx), JID, EncodeBase64(Status));
	if(State!=GetState(UserIdx)) ChangedStateList->WriteInteger("State:"+IntToStr(UserIdx), JID, State);
}
//--------------------------------------------------------------------------

//Procka okna timera
LRESULT CALLBACK TimerFrmProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg==WM_TIMER)
	{
		//Zatrzymanie timera
		KillTimer(hTimerFrm, wParam);
		//Pobranie indeksu konta z ID timera
		int UserIdx = wParam;
		//Sprawdzanie czy zostaly wyslane jakies statusy kontaktom
		TStringList *NewStatus = new TStringList;
		ChangedStateList->ReadSection("Idx:"+IntToStr(UserIdx), NewStatus);
		if(NewStatus->Count>0)
		{
			//Sprawdzanie zapisanych identyfikatorow kontaktow
			for(int Count=0;Count<NewStatus->Count;Count++)
			{
				//Pobranie identyfikatora kontatku
				UnicodeString JID = NewStatus->Strings[Count];
				//Wyslanie nowego statusu
				if(!JID.IsEmpty())
					SendXML(JID, UserIdx, DecodeBase64(ChangedStateList->ReadString("Status:"+IntToStr(UserIdx), JID, EncodeBase64(GetStatus(UserIdx)))), ChangedStateList->ReadInteger("State:"+IntToStr(UserIdx), JID, GetState(UserIdx)));
			}
		}
		delete NewStatus;

		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//---------------------------------------------------------------------------

//Glowny serwis wtyczki
INT_PTR __stdcall SendStateService(WPARAM wParam, LPARAM lParam)
{
	//Tworzenie uchwytu do formy
	Application->Handle = (HWND)SendStateForm;
	TSendStateForm * hSendStateForm = new TSendStateForm(Application);
	//Przekazywanie zmiennych do formy
	hSendStateForm->JID = ContactJID;
	hSendStateForm->UserIdx = ContactUserIdx;
	//Wstawianie danych do komponentow
	hSendStateForm->StatusMemo->Text = GetChangedStatus(ContactJID, ContactUserIdx);
	hSendStateForm->StateComboBox->ItemIndex = GetChangedState(ContactJID, ContactUserIdx);
	//Pokazywanie przycisku do przywracania opisu/statusu
	if((hSendStateForm->StatusMemo->Text!=GetStatus(ContactUserIdx))||(hSendStateForm->StateComboBox->ItemIndex!=GetState(ContactUserIdx)))
		hSendStateForm->ResetButton->Visible = true;
	else
		hSendStateForm->ResetButton->Visible = false;
	//Pokazywanie formy
	hSendStateForm->ShowModal();
	//Usuwanie uchwytu do formy
	delete hSendStateForm;

	return 0;
}
//---------------------------------------------------------------------------

//Zmiana statusu przycisku
void ChangeItemState(bool Enabled, bool Checked)
{
	TPluginActionEdit PluginActionEdit;
	PluginActionEdit.cbSize = sizeof(TPluginActionEdit);
	PluginActionEdit.pszName = L"SendStateItem";
	PluginActionEdit.Caption = GetLangStr("SendStateItem").w_str();
	PluginActionEdit.Hint = L"";
	PluginActionEdit.Enabled = Enabled;
	PluginActionEdit.Visible = Enabled;
	PluginActionEdit.IconIndex = -1;
	PluginActionEdit.Checked = Checked;
	PluginLink.CallService(AQQ_CONTROLS_EDITPOPUPMENUITEM, 0, (LPARAM)(&PluginActionEdit));
}
//---------------------------------------------------------------------------

//Usuniecie przycisku z interfejsu
void DestroySendStateItem()
{
	TPluginAction SendStateItem;
	ZeroMemory(&SendStateItem, sizeof(TPluginAction));
	SendStateItem.cbSize = sizeof(TPluginAction);
	SendStateItem.pszName = L"SendStateItem";
	PluginLink.CallService(AQQ_CONTROLS_DESTROYPOPUPMENUITEM, 0, (LPARAM)(&SendStateItem));
}
//---------------------------------------------------------------------------

//Tworzenie przycisku w interfejsie
void BuildSendStateItem()
{
	TPluginAction SendStateItem;
	ZeroMemory(&SendStateItem, sizeof(TPluginAction));
	SendStateItem.cbSize = sizeof(TPluginAction);
	SendStateItem.pszName = L"SendStateItem";
	SendStateItem.pszCaption = GetLangStr("SendStateItem").w_str();
	SendStateItem.Position = 5;
	SendStateItem.IconIndex = -1;
	SendStateItem.pszService = L"sSendStateItem";
	SendStateItem.pszPopupName = L"muItem";
	SendStateItem.PopupPosition = 0;
	PluginLink.CallService(AQQ_CONTROLS_CREATEPOPUPMENUITEM, 0, (LPARAM)(&SendStateItem));
}
//---------------------------------------------------------------------------

//Hook na zmianê stanu kontaktu
INT_PTR __stdcall OnContactsUpdate(WPARAM wParam, LPARAM lParam)
{
	//Jezeli kontakt dopiero sie polaczyl
	if(lParam==CONTACT_UPDATE_ONLINE)
	{
		//Pobieranie danych kontatku
		TPluginContact ContactsUpdateContact = *(PPluginContact)wParam;
		//Pobieranie identyfikatora kontatku
		UnicodeString JID = (wchar_t*)ContactsUpdateContact.JID;
		//Pobranie indeksu konta
		int UserIdx = ContactsUpdateContact.UserIdx;
		//Sprawdzanie czy dla kontatku zostal wyslany inny status
		if((ChangedStateList->ValueExists("Status:"+IntToStr(UserIdx), JID))||(ChangedStateList->ValueExists("State:"+IntToStr(UserIdx), JID)))
			SendXML(JID, UserIdx, GetChangedStatus(JID, UserIdx), GetChangedState(JID, UserIdx));
	}

	return 0;
}
//---------------------------------------------------------------------------

//Hook na zmiane lokalizacji
INT_PTR __stdcall OnLangCodeChanged(WPARAM wParam, LPARAM lParam)
{
	//Czyszczenie cache lokalizacji
	ClearLngCache();
	//Pobranie sciezki do katalogu prywatnego uzytkownika
	UnicodeString PluginUserDir = GetPluginUserDir();
	//Ustawienie sciezki lokalizacji wtyczki
	UnicodeString LangCode = (wchar_t*)lParam;
	LangPath = PluginUserDir + "\\\\Languages\\\\SendState\\\\" + LangCode + "\\\\";
	if(!DirectoryExists(LangPath))
	{
		LangCode = (wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETDEFLANGCODE,0,0);
		LangPath = PluginUserDir + "\\\\Languages\\\\SendState\\\\" + LangCode + "\\\\";
	}

	return 0;
}
//---------------------------------------------------------------------------

//Notyfikacja zmiany stanu
INT_PTR __stdcall OnStateChange(WPARAM wParam, LPARAM lParam)
{
	//Pobranie danych dotyczacych konta
	TPluginStateChange StateChange = *(PPluginStateChange)lParam;
	//Pobranie indeksu konta
	int UserIdx = StateChange.UserIdx;
	//Sprawdzanie czy zostaly wyslane jakies statusy kontaktom z danego konta
	TStringList *NewStatus = new TStringList;
	ChangedStateList->ReadSection("Idx:"+IntToStr(UserIdx), NewStatus);
	if(NewStatus->Count>0)
		//Wlaczenie timera wysylania ustawionych statusow dla kontaktow
		SetTimer(hTimerFrm, UserIdx, 10, (TIMERPROC)TimerFrmProc);
	delete NewStatus;

	return 0;
}
//---------------------------------------------------------------------------

//Hook na pokazywanie popupmenu
INT_PTR __stdcall OnSystemPopUp(WPARAM wParam, LPARAM lParam)
{
	//Pobranie danych dotyczacych popupmenu
	TPluginPopUp PopUp = *(PPluginPopUp)lParam;
	//Pobieranie nazwy popupmenu
	UnicodeString PopUpName = (wchar_t*)PopUp.Name;
	//Popupmenu dostepne spod PPM na kontakcie w oknie kontaktow
	if(PopUpName=="muItem")
	{
		//Pobieranie danych kontatku
		TPluginContact SystemPopUContact = *(PPluginContact)wParam;
		//Pobieranie identyfikatora kontatku
		UnicodeString JID = (wchar_t*)SystemPopUContact.JID;
		//Kontakt nie pochodzi od wtyczki i czatu FB oraz nie jest z transportu
		if((!SystemPopUContact.FromPlugin)
		&&(!JID.Pos("@chat.facebook.com"))
		&&(!JID.Pos("@gg."))
		&&(!JID.Pos("@gadu-gadu."))
		&&(!JID.Pos("@icq."))
		&&(!JID.Pos("@aim."))
		&&(!JID.Pos("@msn."))
		&&(!JID.Pos("@irc."))
		&&(!JID.Pos("@yahoo."))
		&&(!JID.Pos("@skype.")))
		{
			//Zapisanie identyfikatora kontatku w zmiennej globalnej
			ContactJID = JID;
			//Zapisanie indeksu konta w zmiennej globalnej
			ContactUserIdx = SystemPopUContact.UserIdx;
			//Pokazanie przycisku w interfejsie
			ChangeItemState(true, ChangedStateList->ValueExists("Idx:"+IntToStr(ContactUserIdx), ContactJID));
		}
		//Ukrycie przycisku z interfejsu
		else
			ChangeItemState(false, false);
	}

	return 0;
}
//---------------------------------------------------------------------------

//Zapisywanie zasobów
void ExtractRes(wchar_t* FileName, wchar_t* ResName, wchar_t* ResType)
{
	TPluginTwoFlagParams PluginTwoFlagParams;
	PluginTwoFlagParams.cbSize = sizeof(TPluginTwoFlagParams);
	PluginTwoFlagParams.Param1 = ResName;
	PluginTwoFlagParams.Param2 = ResType;
	PluginTwoFlagParams.Flag1 = (int)HInstance;
	PluginLink.CallService(AQQ_FUNCTION_SAVERESOURCE,(WPARAM)&PluginTwoFlagParams,(LPARAM)FileName);
}
//---------------------------------------------------------------------------

//Obliczanie sumy kontrolnej pliku
UnicodeString MD5File(UnicodeString FileName)
{
	if(FileExists(FileName))
	{
		UnicodeString Result;
		TFileStream *fs;
		fs = new TFileStream(FileName, fmOpenRead | fmShareDenyWrite);
		try
		{
			TIdHashMessageDigest5 *idmd5= new TIdHashMessageDigest5();
			try
			{
				Result = idmd5->HashStreamAsHex(fs);
			}
			__finally
			{
				delete idmd5;
			}
		}
		__finally
		{
			delete fs;
		}
		return Result;
	}
	else return 0;
}
//---------------------------------------------------------------------------

extern "C" INT_PTR __declspec(dllexport) __stdcall Load(PPluginLink Link)
{
	//Linkowanie wtyczki z komunikatorem
	PluginLink = *Link;
  //Pobranie sciezki do prywatnego folderu wtyczek
	UnicodeString PluginUserDir = GetPluginUserDir();
  //Tworzenie katalogow lokalizacji
	if(!DirectoryExists(PluginUserDir+"\\\\Languages"))
		CreateDir(PluginUserDir+"\\\\Languages");
	if(!DirectoryExists(PluginUserDir+"\\\\Languages\\\\SendState"))
		CreateDir(PluginUserDir+"\\\\Languages\\\\SendState");
	if(!DirectoryExists(PluginUserDir+"\\\\Languages\\\\SendState\\\\EN"))
		CreateDir(PluginUserDir+"\\\\Languages\\\\SendState\\\\EN");
	if(!DirectoryExists(PluginUserDir+"\\\\Languages\\\\SendState\\\\PL"))
		CreateDir(PluginUserDir+"\\\\Languages\\\\SendState\\\\PL");
	//Wypakowanie plikow lokalizacji
	//9B2EC6CCC372B9805AD484B9BAD69098
	if(!FileExists(PluginUserDir+"\\\\Languages\\\\SendState\\\\EN\\\\Const.lng"))
		ExtractRes((PluginUserDir+"\\\\Languages\\\\SendState\\\\EN\\\\Const.lng").w_str(),L"EN_CONST",L"DATA");
	else if(MD5File(PluginUserDir+"\\\\Languages\\\\SendState\\\\EN\\\\Const.lng")!="9B2EC6CCC372B9805AD484B9BAD69098")
		ExtractRes((PluginUserDir+"\\\\Languages\\\\SendState\\\\EN\\\\Const.lng").w_str(),L"EN_CONST",L"DATA");
	//EB5EE165E3E14B4E72668083A335F628
	if(!FileExists(PluginUserDir+"\\\\Languages\\\\SendState\\\\EN\\\\TSendStateForm.lng"))
		ExtractRes((PluginUserDir+"\\\\Languages\\\\SendState\\\\EN\\\\TSendStateForm.lng").w_str(),L"EN_SENDSTATEFRM",L"DATA");
	else if(MD5File(PluginUserDir+"\\\\Languages\\\\SendState\\\\EN\\\\TSendStateForm.lng")!="EB5EE165E3E14B4E72668083A335F628")
		ExtractRes((PluginUserDir+"\\\\Languages\\\\SendState\\\\EN\\\\TSendStateForm.lng").w_str(),L"EN_SENDSTATEFRM",L"DATA");
	//66263DCB82136562906AE9FF28DF174D
	if(!FileExists(PluginUserDir+"\\\\Languages\\\\SendState\\\\PL\\\\Const.lng"))
		ExtractRes((PluginUserDir+"\\\\Languages\\\\SendState\\\\PL\\\\Const.lng").w_str(),L"PL_CONST",L"DATA");
	else if(MD5File(PluginUserDir+"\\\\Languages\\\\SendState\\\\PL\\\\Const.lng")!="66263DCB82136562906AE9FF28DF174D")
		ExtractRes((PluginUserDir+"\\\\Languages\\\\SendState\\\\PL\\\\Const.lng").w_str(),L"PL_CONST",L"DATA");
	//AD71E71FCFEE35C3214B05DACE5206AB
	if(!FileExists(PluginUserDir+"\\\\Languages\\\\SendState\\\\PL\\\\TSendStateForm.lng"))
		ExtractRes((PluginUserDir+"\\\\Languages\\\\SendState\\\\PL\\\\TSendStateForm.lng").w_str(),L"PL_SENDSTATEFRM",L"DATA");
	else if(MD5File(PluginUserDir+"\\\\Languages\\\\SendState\\\\PL\\\\TSendStateForm.lng")!="AD71E71FCFEE35C3214B05DACE5206AB")
		ExtractRes((PluginUserDir+"\\\\Languages\\\\SendState\\\\PL\\\\TSendStateForm.lng").w_str(),L"PL_SENDSTATEFRM",L"DATA");
	//Ustawienie sciezki lokalizacji wtyczki
	UnicodeString LangCode = (wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETLANGCODE,0,0);
	LangPath = PluginUserDir + "\\\\Languages\\\\SendState\\\\" + LangCode + "\\\\";
	if(!DirectoryExists(LangPath))
	{
		LangCode = (wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETDEFLANGCODE,0,0);
		LangPath = PluginUserDir + "\\\\Languages\\\\SendState\\\\" + LangCode + "\\\\";
	}
	//Tworzenie serwisu dla przycisku
	PluginLink.CreateServiceFunction(L"sSendStateItem", SendStateService);
	//Tworzenie przycisku w interfejsie
	BuildSendStateItem();
	//Hook na zmianê stanu kontaktu
	PluginLink.HookEvent(AQQ_CONTACTS_UPDATE, OnContactsUpdate);
	//Hook na zmiane lokalizacji
	PluginLink.HookEvent(AQQ_SYSTEM_LANGCODE_CHANGED,OnLangCodeChanged);
	//Hook dla zmiany stanu
	PluginLink.HookEvent(AQQ_SYSTEM_STATECHANGE, OnStateChange);
	//Hook na pokazywanie popupmenu
	PluginLink.HookEvent(AQQ_SYSTEM_POPUP, OnSystemPopUp);
	//Rejestowanie klasy okna timera
	WNDCLASSEX wincl;
	wincl.cbSize = sizeof (WNDCLASSEX);
	wincl.style = 0;
	wincl.lpfnWndProc = TimerFrmProc;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hInstance = HInstance;
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wincl.lpszMenuName = NULL;
	wincl.lpszClassName = L"TSendStateTimer";
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wincl);
	//Tworzenie okna timera
	hTimerFrm = CreateWindowEx(0, L"TSendStateTimer", L"", 0, 0, 0, 0, 0, NULL, NULL, HInstance, NULL);

	return 0;
}
//---------------------------------------------------------------------------

extern "C" INT_PTR __declspec(dllexport) __stdcall Unload()
{
	//Usuwanie okna timera
	DestroyWindow(hTimerFrm);
	//Wyrejestowanie klasy okna timera
	UnregisterClass(L"TSendStateTimer", HInstance);
	//Usuniecie przycisku z interfejsu
	DestroySendStateItem();
	//Usuniecie serwisu dla przyciku
	PluginLink.DestroyServiceFunction(SendStateService);
	//Wyladowanie hookow
	PluginLink.UnhookEvent(OnContactsUpdate);
	PluginLink.UnhookEvent(OnLangCodeChanged);
	PluginLink.UnhookEvent(OnStateChange);
	PluginLink.UnhookEvent(OnSystemPopUp);

	return 0;
}
//--------------------------------------------------------------------------

//Informacje o wtyczce
extern "C" PPluginInfo __declspec(dllexport) __stdcall AQQPluginInfo(DWORD AQQVersion)
{
	PluginInfo.cbSize = sizeof(TPluginInfo);
	PluginInfo.ShortName = L"SendState";
	PluginInfo.Version = PLUGIN_MAKE_VERSION(1,3,3,0);
	PluginInfo.Description = L"Wtyczka s³u¿y do wysy³ania indywidualnego statusu kontaktom z sieci Jabber. Wystarczy wybraæ kontakt, klikn¹æ w pozycjê \"Wyœlij status\", wybraæ odpowiedni stan oraz zmieniæ opis i klikn¹æ w przycisk \"Wyœlij\".";
	PluginInfo.Author = L"Krzysztof Grochocki";
	PluginInfo.AuthorMail = L"kontakt@beherit.pl";
	PluginInfo.Copyright = L"Krzysztof Grochocki";
	PluginInfo.Homepage = L"http://beherit.pl";
	PluginInfo.Flag = 0;
	PluginInfo.ReplaceDefaultModule = 0;

	return &PluginInfo;
}
//---------------------------------------------------------------------------
