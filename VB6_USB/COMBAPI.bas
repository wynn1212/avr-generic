Attribute VB_Name = "COMBAPI"

Public Declare Function OpenUsbDevice Lib "USBHIO.dll" (ByVal MyUsbVendorID As Integer, ByVal MyUsbProductID As Integer) As Boolean
Public Declare Sub OutDataCtrl Lib "USBHIO.dll" (ByVal OutData As Byte, ByVal OutControl As Byte)
Public Declare Sub OutDataEightByte Lib "USBHIO.dll" (ByVal OutData1 As Byte, ByVal OutData2 As Byte, ByVal OutData3 As Byte, _
                                                      ByVal OutData4 As Byte, ByVal OutData5 As Byte, ByVal OutData6 As Byte, _
                                                      ByVal OutData7 As Byte, ByVal OutData8 As Byte)
Public Declare Sub ReadData Lib "USBHIO.dll" (ByRef ReadBuffer() As Byte)
Public Declare Sub CloseUsbDevice Lib "USBHIO.dll" ()

Public ReadKeyData(8) As Byte

'



'



'
