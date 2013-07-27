object SendForm: TSendForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Wy'#347'lij status'
  ClientHeight = 167
  ClientWidth = 201
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ResetButton: TsSpeedButton
    Left = 10
    Top = 136
    Width = 23
    Height = 22
    Hint = 'Przywr'#243#263' aktualny opis i status'
    Flat = True
    ParentShowHint = False
    ShowHint = True
    Visible = False
    OnClick = ResetButtonClick
    SkinData.SkinSection = 'SPEEDBUTTON'
    ImageIndex = 6
    Images = sAlphaImageList
  end
  object StatusMemo: TsMemo
    Left = 8
    Top = 41
    Width = 185
    Height = 89
    ScrollBars = ssVertical
    TabOrder = 1
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'EDIT'
  end
  object SendButton: TsButton
    Left = 118
    Top = 136
    Width = 75
    Height = 25
    Caption = 'Wy'#347'lij'
    TabOrder = 2
    OnClick = SendButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object StateComboBox: TsComboBox
    Left = 8
    Top = 8
    Width = 185
    Height = 27
    Alignment = taLeftJustify
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Style = csOwnerDrawFixed
    ItemHeight = 21
    ItemIndex = 0
    TabOrder = 0
    Text = 'Po'#322#261'czony'
    OnDrawItem = StateComboBoxDrawItem
    Items.Strings = (
      'Po'#322#261'czony'
      'Wolny'
      'Oddalony'
      'Nieobecny'
      'Nie przeszkadza'#263
      'Roz'#322#261'czony')
  end
  object ActionList: TActionList
    Left = 32
    object aExit: TAction
      Caption = 'aExit'
      ShortCut = 27
      OnExecute = aExitExecute
    end
    object aSelectMemo: TAction
      Caption = 'aSelectMemo'
      ShortCut = 16449
      OnExecute = aSelectMemoExecute
    end
  end
  object sSkinManager: TsSkinManager
    ExtendedBorders = True
    Active = False
    InternalSkins = <>
    MenuSupport.IcoLineSkin = 'ICOLINE'
    MenuSupport.ExtraLineFont.Charset = DEFAULT_CHARSET
    MenuSupport.ExtraLineFont.Color = clWindowText
    MenuSupport.ExtraLineFont.Height = -11
    MenuSupport.ExtraLineFont.Name = 'Tahoma'
    MenuSupport.ExtraLineFont.Style = []
    SkinDirectory = 'c:\Skins'
    SkinInfo = 'N/A'
    ThirdParty.ThirdEdits = ' '
    ThirdParty.ThirdButtons = 'TButton'
    ThirdParty.ThirdBitBtns = ' '
    ThirdParty.ThirdCheckBoxes = ' '
    ThirdParty.ThirdGroupBoxes = ' '
    ThirdParty.ThirdListViews = ' '
    ThirdParty.ThirdPanels = ' '
    ThirdParty.ThirdGrids = ' '
    ThirdParty.ThirdTreeViews = ' '
    ThirdParty.ThirdComboBoxes = ' '
    ThirdParty.ThirdWWEdits = ' '
    ThirdParty.ThirdVirtualTrees = ' '
    ThirdParty.ThirdGridEh = ' '
    ThirdParty.ThirdPageControl = ' '
    ThirdParty.ThirdTabControl = ' '
    ThirdParty.ThirdToolBar = ' '
    ThirdParty.ThirdStatusBar = ' '
    ThirdParty.ThirdSpeedButton = ' '
    ThirdParty.ThirdScrollControl = ' '
    ThirdParty.ThirdUpDown = ' '
    ThirdParty.ThirdScrollBar = ' '
    Left = 64
  end
  object sSkinProvider: TsSkinProvider
    AddedTitle.Font.Charset = DEFAULT_CHARSET
    AddedTitle.Font.Color = clNone
    AddedTitle.Font.Height = -11
    AddedTitle.Font.Name = 'Tahoma'
    AddedTitle.Font.Style = []
    SkinData.SkinSection = 'FORM'
    TitleButtons = <>
    Left = 96
  end
  object sAlphaImageList: TsAlphaImageList
    DrawingStyle = dsTransparent
    Items = <>
  end
end
