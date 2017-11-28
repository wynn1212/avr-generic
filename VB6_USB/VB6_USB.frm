VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5565
   ClientLeft      =   60
   ClientTop       =   405
   ClientWidth     =   9885
   LinkTopic       =   "Form1"
   ScaleHeight     =   5565
   ScaleWidth      =   9885
   StartUpPosition =   3  '系統預設值
   Begin VB.Timer Timer2 
      Interval        =   500
      Left            =   120
      Top             =   600
   End
   Begin VB.Timer Timer1 
      Interval        =   300
      Left            =   120
      Top             =   120
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Beep 1000Hz"
      Height          =   495
      Index           =   0
      Left            =   1560
      TabIndex        =   3
      Top             =   1680
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "EXIT"
      Height          =   495
      Left            =   7800
      TabIndex        =   0
      Top             =   4800
      Width           =   1815
   End
   Begin VB.Shape online 
      FillStyle       =   0  '實心
      Height          =   375
      Left            =   1080
      Shape           =   3  '圓形
      Top             =   480
      Width           =   375
   End
   Begin VB.Label Label2 
      BackColor       =   &H8000000E&
      BorderStyle     =   1  '單線固定
      Caption         =   "Label2"
      BeginProperty Font 
         Name            =   "新細明體"
         Size            =   20.25
         Charset         =   136
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   240
      TabIndex        =   2
      Top             =   4800
      Width           =   5415
   End
   Begin VB.Label Label1 
      Alignment       =   2  '置中對齊
      BackColor       =   &H8000000E&
      BorderStyle     =   1  '單線固定
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "新細明體"
         Size            =   48
         Charset         =   136
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1095
      Left            =   1560
      TabIndex        =   1
      Top             =   480
      Width           =   8055
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Dim VBKEY As Integer
Dim READY As Boolean
Dim PowerON As Boolean
Dim datain(10) As Byte
Dim Station_no As Byte
'Dim VID, PID As Integer
Const VID = &H1234
Const PID = &H2468

Private Sub form_initialize()
    Station_no = 255
    READY = True
    Call USBFunc("online")
End Sub

Private Sub Command1_Click()
    Unload Forms(0)
End Sub

Private Sub Command2_Click(Index As Integer)
    VBKEY = Index
    If VBKEY = 0 Then Call USBFunc("buzz")
End Sub


Sub USBFunc(ByVal mode As String)
    If (OpenUsbDevice(VID, PID)) Then
        online.FillColor = vbGreen
        Label1.Caption = "DEVICE ONLINE!"
        If (mode = "offline") Then
            OutDataEightByte 0, &H0, 0, 0, 0, 0, 0, 0
        ElseIf (mode = "online") Then
            OutDataEightByte 0, &H1, Station_no, 0, 0, 0, 0, 0
            PowerON = True
        ElseIf (mode = "buzz") Then
            OutDataEightByte 0, &H2, 0, 0, 0, 0, 0, 0
        End If
        Label2.Caption = ""
        ReadData datain
        For i = 0 To 8: Label2.Caption = Label2.Caption & datain(i) & "-": Next i
        CloseUsbDevice
    Else
        online.FillColor = vbRed
        Label1.Caption = "DEVICE OFFLINE!"
        PowerON = False
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    USBFunc ("offline")
End Sub

Private Sub Timer1_Timer()
    If (PowerON = False) Then
        Call USBFunc("online")
    Else
        Call USBFunc(other)
    End If
End Sub

Private Sub Timer2_Timer()
    'Call USBFunc("online")
End Sub
