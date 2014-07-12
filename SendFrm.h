//---------------------------------------------------------------------------
// Copyright (C) 2010-2014 Krzysztof Grochocki
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
	void __fastcall sSkinManagerSysDlgInit(TacSysDlgData DlgData, bool &AllowSkinning);
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
