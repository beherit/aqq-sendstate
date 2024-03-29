object SendStateForm: TSendStateForm
  Tag = 1
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
    Tag = 4
    Left = 8
    Top = 136
    Width = 28
    Height = 25
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
    Top = 42
    Width = 185
    Height = 88
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
    Tag = 3
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
    Tag = 2
    Left = 8
    Top = 8
    Width = 185
    Height = 28
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
    VerticalAlignment = taAlignTop
    Style = csOwnerDrawFixed
    ItemHeight = 22
    ItemIndex = -1
    TabOrder = 0
    OnDrawItem = StateComboBoxDrawItem
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
    Options.OptimizingPriority = opMemory
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
    ThirdParty.ThirdStaticText = ' '
    ThirdParty.ThirdNativePaint = ' '
    OnSysDlgInit = sSkinManagerSysDlgInit
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
