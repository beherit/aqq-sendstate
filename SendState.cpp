//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#pragma argsused
#include "Aqq.h"
#include "SendFrm.h"
//---------------------------------------------------------------------------

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  return 1;
}
//--------------------------------------------------------------------------

//Struktury glowne
TPluginLink PluginLink;
TPluginInfo PluginInfo;
TPluginAction SendStateButton;
TPluginActionEdit PluginActionEdit;
TPluginStateChange PluginStateChange;
PPluginPopUp PopUp;
PPluginContact Contact;
//Zmienne globalne
UnicodeString ContactJID;
int ContactUserIdx;
TCustomIniFile* ChangedStateList = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));

//Pobieranie sciezki do skorki kompozycji
UnicodeString GetThemeSkinDir()
{
  return StringReplace((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETTHEMEDIR,0,0)), "\\", "\\\\", TReplaceFlags() << rfReplaceAll) + "\\\\Skin";
}
//---------------------------------------------------------------------------

//Sprawdzanie czy wlaczona jest obsluga stylow obramowania okien
bool ChkSkinEnabled()
{
  TStrings* IniList = new TStringList();
  IniList->SetText((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP,0,0)));
  TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
  Settings->SetStrings(IniList);
  delete IniList;
  UnicodeString AlphaSkinsEnabled = Settings->ReadString("Settings","UseSkin","1");
  delete Settings;
  return StrToBool(AlphaSkinsEnabled);
}
//---------------------------------------------------------------------------

//Sprawdzanie czy wlaczony jest natywny styl Windows
bool ChkNativeEnabled()
{
  TStrings* IniList = new TStringList();
  IniList->SetText((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP,0,0)));
  TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
  Settings->SetStrings(IniList);
  delete IniList;
  UnicodeString NativeEnabled = Settings->ReadString("Settings","Native","0");
  delete Settings;
  return StrToBool(NativeEnabled);
}
//---------------------------------------------------------------------------

//Pobieranie opisu
UnicodeString GetStatus(int UserIdx)
{
  PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),UserIdx);
  return (wchar_t*)PluginStateChange.Status;
}
//---------------------------------------------------------------------------

//Pobieranie stanu
int GetState(int UserIdx)
{
  int State;
  PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),UserIdx);
  State = PluginStateChange.NewState;

  if((State==0)||(State==6))
   State=5;
  else
   State=State-1;

  return State;
}
//---------------------------------------------------------------------------

//Pobieranie wybranego wczeœniej opisu
UnicodeString GetChangedStatus(UnicodeString JID, int UserIdx)
{
  return ChangedStateList->ReadString(JID, "Status", GetStatus(UserIdx));
}
//---------------------------------------------------------------------------

//Pobieranie wybranego wczeœniej stanu
int GetChangedState(UnicodeString JID, int UserIdx)
{
  return ChangedStateList->ReadInteger(JID, "State", GetState(UserIdx));
}
//---------------------------------------------------------------------------

//Pobieranie sciezki ikony z interfejsu AQQ
UnicodeString GetIconPath(int Icon)
{
  return StringReplace((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPNG_FILEPATH,Icon,0)), "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
}
//--------------------------------------------------------------------------

void SendXML(UnicodeString JID, int State, UnicodeString Status, int UserIdx)
{
  //Konwersja specjalnych znakow
  if(!Status.IsEmpty())
   Status = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_CONVERTTOXML,0,(WPARAM)Status.w_str()));
  //Podostawowe zmienne
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
	if(Status!="")
	 XML = "<presence to=\"" + JID + "\"><status>" + Status + "</status><show>" + ShowType + "</show></presence>";
	else
	 XML = "<presence to=\"" + JID + "\"><status/><show>" + ShowType + "</show></presence>";
  }
  else if(State==5)
  {
	if(Status!="")
	 XML = "<presence to=\"" + JID + "\" type=\"" + ShowType + "\"><status>" + Status + "</status></presence>";
	else
	 XML = "<presence to=\"" + JID + "\" type=\"" + ShowType+ "\"/>";
  }
  else
  {
	if(Status!="")
	 XML = "<presence to=\"" + JID + "\"><status>" + Status + "</status></presence>";
	else
	 XML = "<presence to=\"" + JID + "\"/>";
  }
  //Wysylanie pakietu XML
  PluginLink.CallService(AQQ_SYSTEM_SENDXML,(WPARAM)XML.w_str(),UserIdx);
  //Zapisanie wyslanych danych
  ChangedStateList->WriteString(JID, "Status", Status);
  ChangedStateList->WriteInteger(JID, "State", State);
}
//--------------------------------------------------------------------------

//Glowny serwis wtyczki
int __stdcall SendStateService (WPARAM, LPARAM)
{
  Application->Handle = (HWND)SendForm;
  TSendForm * hSendForm = new TSendForm(Application);
  hSendForm->JID = ContactJID;
  hSendForm->UserIdx = ContactUserIdx;
  hSendForm->StatusMemo->Text = GetChangedStatus(ContactJID, ContactUserIdx);
  hSendForm->StateComboBox->ItemIndex = GetChangedState(ContactJID, ContactUserIdx);
  hSendForm->ShowModal();
  delete hSendForm;

  return 0;
}
//---------------------------------------------------------------------------

//Zmiana statusu buttona
void ChangeButtonState(bool Enabled)
{
  PluginActionEdit.cbSize = sizeof(TPluginActionEdit);
  PluginActionEdit.pszName = (wchar_t*)L"SendStateButton";
  PluginActionEdit.Caption = (wchar_t*) L"Wyœlij status";
  PluginActionEdit.Hint = (wchar_t*)L"";
  PluginActionEdit.Enabled = Enabled;
  PluginActionEdit.Visible = Enabled;
  PluginActionEdit.IconIndex = -1;
  PluginActionEdit.Checked = false;

  PluginLink.CallService(AQQ_CONTROLS_EDITPOPUPMENUITEM,0,(LPARAM)(&PluginActionEdit));
}
//---------------------------------------------------------------------------

//Tworzenie przycisku
void BuildSendStateButton()
{
  SendStateButton.cbSize = sizeof(TPluginAction);
  SendStateButton.pszName = (wchar_t*)L"SendStateButton";
  SendStateButton.pszCaption = (wchar_t*)L"Wyœlij status";
  SendStateButton.Position = 5;
  SendStateButton.IconIndex = -1;
  SendStateButton.pszService = (wchar_t*)L"aSendStateService";
  SendStateButton.pszPopupName = (wchar_t*)L"muItem";
  SendStateButton.Checked = false;

  PluginLink.CallService(AQQ_CONTROLS_CREATEPOPUPMENUITEM,0,(LPARAM)(&SendStateButton));
  PluginLink.CreateServiceFunction(L"aSendStateService",SendStateService);

  ChangeButtonState(false);
}
//---------------------------------------------------------------------------

//Hook na pokazywanie popupmenu
int __stdcall OnSystemPopUp (WPARAM wParam, LPARAM lParam)
{
  PopUp = (PPluginPopUp)lParam;
  UnicodeString PopUpName = (wchar_t*)PopUp->Name;
  if(PopUpName=="muItem")
  {
	Contact = (PPluginContact)wParam;
	ContactJID = (wchar_t*)Contact->JID;
	if((!Contact->FromPlugin)&&
	 (!ContactJID.Pos("@gg."))&&
	 (!ContactJID.Pos("@gadu-gadu."))&&
	 (!ContactJID.Pos("@icq."))&&
	 (!ContactJID.Pos("@aim."))&&
	 (!ContactJID.Pos("@msn."))&&
	 (!ContactJID.Pos("@irc."))&&
	 (!ContactJID.Pos("@yahoo."))&&
	 (!ContactJID.Pos("@skype.")))
	{
	  if(Contact->Resource!="") ContactJID = ContactJID + "/" + (wchar_t*)Contact->Resource;
	  ContactUserIdx = Contact->UserIdx;
	  ChangeButtonState(true);
	}
	else
	 ChangeButtonState(false);
  }

  return 0;
}
//---------------------------------------------------------------------------

extern "C" int __declspec(dllexport) __stdcall Load(PPluginLink Link)
{
  PluginLink = *Link;
  //Tworzenie przycisku
  BuildSendStateButton();
  //Hook na pokazywanie popupmenu
  PluginLink.HookEvent(AQQ_SYSTEM_POPUP,OnSystemPopUp);

  return 0;
}
//---------------------------------------------------------------------------

extern "C" int __declspec(dllexport) __stdcall Unload()
{
  //Usuniecie przycisku + serwisu
  PluginLink.DestroyServiceFunction(SendStateService);
  PluginLink.CallService(AQQ_CONTROLS_DESTROYPOPUPMENUITEM,0,(LPARAM)(&SendStateButton));
  //Wyladowanie hookow
  PluginLink.UnhookEvent(OnSystemPopUp);
  //Usuniecie zmiennych globalnych
  delete ChangedStateList;

  return 0;
}
//--------------------------------------------------------------------------

extern "C" __declspec(dllexport) PPluginInfo __stdcall AQQPluginInfo(DWORD AQQVersion)
{
  PluginInfo.cbSize = sizeof(TPluginInfo);
  PluginInfo.ShortName = L"SendState";
  PluginInfo.Version = PLUGIN_MAKE_VERSION(1,1,2,0);
  PluginInfo.Description = L"Indywidualny status dla kontaktów Jabber";
  PluginInfo.Author = L"Krzysztof Grochocki (Beherit)";
  PluginInfo.AuthorMail = L"kontakt@beherit.pl";
  PluginInfo.Copyright = L"Krzysztof Grochocki (Beherit)";
  PluginInfo.Homepage = L"http://beherit.pl/";

  return &PluginInfo;
}
//---------------------------------------------------------------------------
