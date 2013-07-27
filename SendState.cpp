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

TPluginAction PluginAction;
TPluginLink PluginLink;
TPluginInfo PluginInfo;
TPluginActionEdit PluginActionEdit;
TPluginStateChange PluginStateChange;

PPluginPopUp PopUp;
PPluginContact Contact;

UnicodeString PopUpName;
UnicodeString ContactJID;
int ContactUserIdx;

UnicodeString GetStatus(int UserIdx)
{
  UnicodeString Status;
  PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),UserIdx);
  Status = (wchar_t*)PluginStateChange.Status;
  return Status;
}
//---------------------------------------------------------------------------

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


int __stdcall SendStateService (WPARAM, LPARAM)
{
  Application->Handle = (HWND)SendForm;
  TSendForm * hSendForm = new TSendForm(Application);
  hSendForm->eJID = ContactJID;
  hSendForm->eUserIdx = ContactUserIdx;
  hSendForm->StatusMemo->Text = GetStatus(ContactUserIdx);
  hSendForm->StateLMDImageComboBox->ItemIndex = GetState(ContactUserIdx);
  hSendForm->ShowModal();
  delete hSendForm;

  return 0;
}
//---------------------------------------------------------------------------

//Wylaczanie skrotu
void SkrotOff()
{
  PluginActionEdit.cbSize = sizeof(TPluginActionEdit);
  PluginActionEdit.pszName = (wchar_t*)L"SendStateButton";
  PluginActionEdit.Caption = (wchar_t*) L"Wyœlij status/opis...";
  PluginActionEdit.Hint = (wchar_t*)L"";
  PluginActionEdit.Enabled = false;
  PluginActionEdit.Visible = false;
  PluginActionEdit.IconIndex = -1;
  PluginActionEdit.Checked = false;

  PluginLink.CallService(AQQ_CONTROLS_EDITPOPUPMENUITEM,0,(LPARAM)(&PluginActionEdit));
}
//---------------------------------------------------------------------------

//Wlaczanie skrotu
void SkrotOn()
{
  PluginActionEdit.cbSize = sizeof(TPluginActionEdit);
  PluginActionEdit.pszName = (wchar_t*)L"SendStateButton";
  PluginActionEdit.Caption = (wchar_t*) L"Wyœlij status/opis";
  PluginActionEdit.Hint = (wchar_t*)L"";
  PluginActionEdit.Enabled = true;
  PluginActionEdit.Visible = true;
  PluginActionEdit.IconIndex = -1;
  PluginActionEdit.Checked = false;

  PluginLink.CallService(AQQ_CONTROLS_EDITPOPUPMENUITEM,0,(LPARAM)(&PluginActionEdit));
}
//---------------------------------------------------------------------------

int __stdcall OnSystemPopUp (WPARAM wParam, LPARAM lParam)
{
  PopUp = (PPluginPopUp)lParam;
  PopUpName = PopUp->Name;
  if(PopUpName=="muItem")
  {
	Contact = (PPluginContact)wParam;
	ContactJID = Contact->JID;
	ContactUserIdx = Contact->UserIdx;

	if(AnsiPos("@plugin.gg",ContactJID)==0)
	{
	  if(Contact->Resource!="")
	   ContactJID = ContactJID + "/" + Contact->Resource;

	  SkrotOn();
	}
	else
	 SkrotOff();
  }

  return 0;
}
//---------------------------------------------------------------------------

void PrzypiszSkrot()
{
  PluginAction.cbSize = sizeof(TPluginAction);
  PluginAction.pszName = (wchar_t*)L"SendStateButton";
  PluginAction.pszCaption = (wchar_t*)L"Wyœlij status/opis";
  PluginAction.Position = 5;
  PluginAction.IconIndex = -1;
  PluginAction.pszService = (wchar_t*)L"serwis_SendStateService";
  PluginAction.pszPopupName = (wchar_t*)L"muItem";
  PluginAction.Checked = false;

  PluginLink.CallService(AQQ_CONTROLS_CREATEPOPUPMENUITEM,0,(LPARAM)(&PluginAction));
  PluginLink.CreateServiceFunction(L"serwis_SendStateService",SendStateService);
}
//---------------------------------------------------------------------------

void SendXML(UnicodeString JID, int State, UnicodeString Status, int UserIdx)
{
  if(Status!="")
   Status = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_CONVERTTOXML,0,(WPARAM)Status.w_str()));

  UnicodeString XML;
  UnicodeString ShowType;

  if(State==1)
   ShowType="chat";
  else if(State==2)
   ShowType="away";
  else if(State==3)
   ShowType="xa";
  else if(State==4)
   ShowType="dnd";
  else if(State==5)
   ShowType="unavailable";

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
	 XML = "<presence to=\"" + JID + "\" type=\"" + ShowType+ "\"></presence>";
  }
  else
  {
	if(Status!="")
	 XML = "<presence to=\"" + JID + "\"><status>" + Status + "</status></presence>";
	else
	 XML = "<presence to=\"" + JID + "\"></presence>";
  }

  PluginLink.CallService(AQQ_SYSTEM_SENDXML,(WPARAM)XML.w_str(),UserIdx);
}
//--------------------------------------------------------------------------

UnicodeString GetIconPath(int Icon)
{
  UnicodeString IconPath = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPNG_FILEPATH,Icon,0));
  IconPath = StringReplace(IconPath, "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
  return IconPath;
}
//--------------------------------------------------------------------------

extern "C" __declspec(dllexport) PPluginInfo __stdcall AQQPluginInfo(DWORD AQQVersion)
{
  PluginInfo.cbSize = sizeof(TPluginInfo);
  PluginInfo.ShortName = (wchar_t*)L"SendState";
  PluginInfo.Version = PLUGIN_MAKE_VERSION(1,0,0,2);
  PluginInfo.Description = (wchar_t *)L"Indywidualny status/opis dla kontaktów jabber";
  PluginInfo.Author = (wchar_t *)L"Krzysztof Grochocki (Beherit)";
  PluginInfo.AuthorMail = (wchar_t *)L"sirbeherit@gmail.com";
  PluginInfo.Copyright = (wchar_t *)L"Krzysztof Grochocki (Beherit)";
  PluginInfo.Homepage = (wchar_t *)L"http://beherit.pl/";

  return &PluginInfo;
}
//---------------------------------------------------------------------------

extern "C" int __declspec(dllexport) __stdcall Load(PPluginLink Link)
{
  PluginLink = *Link;

  PrzypiszSkrot();

  PluginLink.HookEvent(AQQ_SYSTEM_POPUP,OnSystemPopUp);

  return 0;
}
//---------------------------------------------------------------------------

extern "C" int __declspec(dllexport) __stdcall Unload()
{
  PluginLink.DestroyServiceFunction(SendStateService);
  PluginLink.CallService(AQQ_CONTROLS_DESTROYPOPUPMENUITEM,0,(LPARAM)(&PluginAction));
  PluginLink.UnhookEvent(OnSystemPopUp);

  return 0;
}
//--------------------------------------------------------------------------
