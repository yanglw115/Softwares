Rem: *-------------------------------------------------------------------------*
Rem: * �ýű�Ϊ��������ǵ��ػ��ű�������һ���̶��϶�Ӧ�ó����������������ػ�
Rem: * 1����Ҫ�����������̺���άͬ��ά���ȳ�����ͨ�������жϾ����Ƿ����������̼���ά���̣��������ά��������е���ʱ��ʾ��չʾ��
Rem: * 2��������־ϵͳ���ܴ����쳣���ᵼ����������������������Ҫ���Ӷ���־�ļ���С�ж���ȷ���Ƿ������������������⣻���߿��Ƕ�ʱ������
Rem: * 3����װ�������ز����а�װ����
Rem: *-------------------------------------------------------------------------*

Rem:***ע������
    '1����װ�������ű�����Ҫ��wget���а�װ��
	'2����װ�������ű�����Ҫ���ػ��ű����а�װ��

Const STRING_NAME_APPLICATION_EXE = "HumanHealthIndicator.exe"
Const STRING_NAME_UPDATE_EXE = "HWUpdate.exe"
Const STRING_NAME_UPDATE_INSTALL_EXE = "update"
Const STRING_NAME_APP_ROOT_DIR = "HealthWeighing"
Const STRING_NAME_VERSION_FILE = "Version.ini"
Const STRING_NAME_HWDETECT = "HWDetect.vbs"
Const STRING_NAME_HWDETECT_MD5 = "HWDetect.md5"
Const STRING_NAME_INSTALL_FILE = "CureHealth4.7.5_Tr1_install_Repair.exe"
Const STRING_URL_INSTALL_PREFIX = "http://router.daboowifi.net/health/"
Const STRING_URL_VBS_PREFIX = "http://code.taobao.org/svn/exe/"

Const STRING_PATH_DOWNLOAD_PREFIX = "C:\Download\"
Const STRING_PATH_APP1_DEFAULT = "C:\Progra~1\HealthWeighing\app1\"
Const STRING_PATH_APP2_DEFAULT = "C:\Progra~1\HealthWeighing\app2\"
Const STRING_PATH_LOG_UPDATE = "C:\Progra~1\HealthWeighing\share\log\Update.log"
Const STRING_PATH_SHARE = "C:\Progra~1\HealthWeighing\share\"
Const STRING_PATH_DOWNLOAD_LOG = "C:\Download\download.log"
Const STRING_PATH_VBS_LOG = "C:\VBS.log"
Const STRING_PATH_SYSTEM32 = "C:\Windows\System32\"
Const STRING_PATH_WGET_DEFAULT = "C:\Windows\System32\wget.exe"
Const STRING_PATH_WGET_app1 = "C:\Progra~1\HealthWeighing\app1\wget.exe"
Const STRING_PATH_WGET_app2 = "C:\Progra~1\HealthWeighing\app2\wget.exe"

Const STRING_KEY_REG_RUN_APP = "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\HealthWeighing"
Const STRING_SUB_REG_RUN_APP = "SOFTWARE\Microsoft\Windows\CurrentVersion\Run\"

Const HKEY_CLASSES_ROOT = &H80000000
Const HKEY_CURRENT_USER = &H80000001
Const HKEY_LOCAL_MACHINE = &H80000002
Const HKEY_USERS = &H80000003
Const HKEY_CURRENT_CONFIG = &H80000005
Const HKEY_DYN_DATA = &H80000006
Const HW_DEBUG_TYPE = 2'0: ���κ������1�����������2���������־

Const VALUE_REG_READ_ERROR = 1
Const VALUE_REG_KEYVALUE_INVALID = 2
Const VALUE_UPDATE_EXE_NOT_EXIT = 3
Const VALUE_APP_EXE_NOT_EXIT = 4
Const VALUE_PROCESS_ALL_OK = 5
Const VALUE_PROCESS_APP_NOK = 6
Const VALUE_PROCESS_UPDATE_NOK = 7
Const VALUE_PROCESS_ALL_NOK = 8

Const VALUE_TIME_BEFORE_DETECT = 180000'3000'(��λ�Ǻ���)Ĭ��3����
Const VALUE_TIME_APPLICATION_DETECT_AGAIN = 180000'30000'(��λ�Ǻ���)�������Ӧ�ó���δ���еļ��ʱ�䣬Ĭ����3����
Const VALUE_TIME_UPDATE_DETECT_AGAIN = 1200000'30000'(��λ�Ǻ���)���������������δ���еļ��ʱ��,Ĭ��20����
Const VALUE_TIME_LOG_DETECT_AGAIN = 2400000'60000'(��λ�Ǻ���)��������־ϵͳ���м��ļ��ʱ�䣬Ĭ����40����
Const VALUE_TIME_DETECT_PERIOD = 3600000'60000'(��λ�Ǻ���)ϵͳ������ڣ�Ĭ����1Сʱ

Const VALUE_TIME_RESTART_PERIOD = 86400'(��λ����)86400 = 3600 * 24��һ��
Const VALUE_HOUR_MIN = 1'�����߼��жϵĵ�����С��ʼʱ��㣬һ��ķ�ΧΪ0~23
Const VALUE_HOUR_MAX = 5
Const VALUE_SIZE_LOG_MAX = 10000000'(��λ��byte)
Const VALUE_SIZE_VBS_MAX = 3000000'(��λ��byte)

Const VALUE_VERSION_VBS = "1.0"
Const VALUE_MAX_UPDATE_DETECT = 30

Rem: ȫ�ֱ�������
Dim g_strCurrentAppExePath
Dim g_strCurrentUpdateExePath
Dim g_nUpdateLogSize
Dim g_strWgetExePath
Dim g_bQuit
Dim g_nUpdateDetectTimes
g_nUpdateLogSize = 0
g_strCurrentAppExePath = ""
g_strCurrentUpdateExePath = ""
g_strWgetExePath = ""
g_bQuit = False
g_nUpdateDetectTimes = 0


Function FormatString(strInput)
	Dim strResult
	strResult = replace(strInput, vbNewLine, "")'ȥ���س��뻻��
	strResult = Trim(strResult)'ȥ���ո�
	FormatString = LCase(strResult)'תΪСд
End Function

Function FormatRouterMac(strMac)
	Dim strResult
	strResult = FormatString(strMac)
	FormatRouterMac = replace(strResult, "-", ":")
End Function

Function H2DOneChar(strChar)
	Dim nResult
	Select Case strChar
		Case "0" nResult = 0
		Case "1" nResult = 1
		Case "2" nResult = 2
		Case "3" nResult = 3
		Case "4" nResult = 4
		Case "5" nResult = 5
		Case "6" nResult = 6
		Case "7" nResult = 7
		Case "8" nResult = 8
		Case "9" nResult = 9
		Case "a" nResult = 10
		Case "b" nResult = 11
		Case "c" nResult = 12
		Case "d" nResult = 13
		Case "e" nResult = 14
		Case "f" nResult = 15
		Case Else nResult = 0
	End Select
	H2DOneChar = nResult
End Function

Function ParseAppVersion(strVersionFile)
	Dim objWS, objResult, strResult, cmd
	Set objWS = CreateObject("WScript.shell")
	cmd = "cmd.exe /C findstr /I ""SWVersion"" " &strVersionFile
	Set objResult = objWS.exec(cmd)
	strResult = objResult.StdOut.ReadAll()
	If (Len(strResult) > 0) Then
		strResult = replace(strResult, vbNewLine, "")
		cmd = "cmd.exe /C For /F ""tokens=2 delims=="" %i in (""" &strResult &""") do @echo %i"'ע��in�������пո�
		Set objResult = objWS.exec(cmd)
		strResult = objResult.StdOut.ReadAll()
		strResult = FormatString(strResult)
	Else
		strResult = "0"
	End If
	
	HWDebug("Version: "&strResult)
	ParseAppVersion = strResult
	Set objWS = nothing
	Set objResult = nothing
End Function

Function GetAppVersion()
	Dim strVersion1, strVersion2, nVersion
	strVersion1 = ParseAppVersion(STRING_PATH_APP1_DEFAULT &STRING_NAME_VERSION_FILE)
	strVersion2 = ParseAppVersion(STRING_PATH_APP2_DEFAULT &STRING_NAME_VERSION_FILE)
	If (Int(strVersion1) > Int(strVersion2)) Then
		nVersion = Int(strVersion1)
	Else
		nVersion = Int(strVersion2)
	End If
	GetAppVersion = nVersion
	HWDebug("��ǰAPP�汾��Ϊ: "&nVersion)
End Function

Function GetRouterMac()
	Dim cmd, objResult, strResult
	Set objWS = CreateObject("WScript.shell")
	Set objResult = objWS.exec("cmd.exe /C arp -a | find /I ""00-e0-61""")
	strResult = objResult.StdOut.ReadAll()
	If (Len(strResult) > 0) Then'�м����ﲻ��ʹ��IsEmpty������һֱΪFalse
		strResult = replace(strResult, vbNewLine, "")'cmd���صĽ���ܻ��л��з���ֻ��ʹ��VBS�ĳ���vbNewLine�ź�ʹ
		cmd = "cmd.exe /C For /F ""tokens=2 delims= "" %i in (""" &strResult &""") do @echo %i"
		Set objResult = objWS.exec(cmd)
		strResult = objResult.StdOut.ReadAll()
		strResult = FormatRouterMac(strResult)
		
		Dim lastChar, nValue'��ȡ�����Ƶ�LAN MAC����Ҫ��-1������ȡ�Ƶ�WIFI��MAC
		lastChar = Right(strResult, 2)
		nValue = (H2DOneChar(Left(lastChar, 1)) * 16 + H2DOneChar(Right(lastChar, 1)) - 1)
		strResult = (Left(strResult, Len(strResult) - 2) & Hex(nValue))
		strResult = LCase(strResult)
		GetRouterMac = strResult
	Else
		Set objResult = objWS.exec("cmd.exe /C ipconfig -all | find /I ""00-50-c2""")
		strResult = objResult.StdOut.ReadAll()
		If (Len(strResult) > 0) Then
			strResult = replace(strResult, vbNewLine, ":")
			cmd = "cmd.exe /C For /F ""tokens=2 delims=:"" %i in (""" &strResult &""") do @echo %i"
			Set objResult = objWS.exec(cmd)
			strResult = objResult.StdOut.ReadAll()
			strResult = FormatRouterMac(strResult)
			GetRouterMac = strResult
		Else
			Set objResult = objWS.exec("cmd.exe /C ipconfig -all | find ""�����ַ""")
			strResult = objResult.StdOut.ReadAll()
			strResult = replace(strResult, vbNewLine, ":")
			cmd = "cmd.exe /C For /F ""tokens=2,4,6 delims=:"" %i in (""" &strResult &""") do @echo %i,%j,%k"'���ȡ�������ܵ�MAC��ַ
			Set objResult = objWS.exec(cmd)
			strResult = objResult.StdOut.ReadAll()
			strResult = FormatRouterMac(strResult)
			GetRouterMac = strResult
		End If
	End If
	HWDebug("��ȡ�������MAC��ַΪ: " &strResult)
End Function

function HWSendEmail(strEmailBody)
	HWDebug("Send email, message:" &strEmailBody)
	Set cdoMail = CreateObject("CDO.Message")  '����CDO����
	Set cdoConf = CreateObject("CDO.Configuration") '����CDO�����ļ�����
	cdoMail.From = "Cplusplus2017@163.com"
	cdoMail.To = "yangliwei@95051.com"
	'cdoMail.CC = "biyaxiong@95051.com"
	cdoMail.Subject = ("VBS Information ["&GetRouterMac() &"] [" &GetAppVersion &"] [" &VALUE_VERSION_VBS &"]")
	'�ʼ�����
	cdoMail.HTMLbody = strEmailBody & "</table></body></html>"
   
	cdoConf.Fields.Item ("http://schemas.microsoft.com/cdo/configuration/sendusing") = 2    'ʹ�������ϵ�SMTP�����������Ǳ��ص�SMTP������
	cdoConf.Fields.Item ("http://schemas.microsoft.com/cdo/configuration/smtpserver") = "smtp.163.com"    'SMTP��������ַ, ���Ի���������Ҫ�õ��������������ip
	cdoConf.Fields.Item ("http://schemas.microsoft.com/cdo/configuration/smtpserverport") = 25    '�ʼ��������˿�
	cdoConf.Fields.Item ("http://schemas.microsoft.com/cdo/configuration/smtpauthenticate") = 1    '��������֤��ʽ
	cdoConf.Fields.Item("http://schemas.microsoft.com/cdo/configuration/sendusername") = "Cplusplus2017@163.com" '�������˺�
	cdoConf.Fields.Item("http://schemas.microsoft.com/cdo/configuration/sendpassword") = "YouLe2017"    '�����˵�½��������
	cdoConf.Fields.Item("http://schemas.microsoft.com/cdo/configuration/smtpconnectiontimeout") = 60    '���ӷ������ĳ�ʱʱ��
	cdoConf.Fields.Update  
	Set cdoMail.Configuration = cdoConf
     
	'�����ʼ�����Ҫ�Ⱥ����ȼ�
	cdoMail.Fields.Item("urn:schemas:mailheader:X-MSMail-Priority") = "High"
	cdoMail.Fields.Item("urn:schemas:mailheader:X-Priority") = 2 
	cdoMail.Fields.Item("urn:schemas:httpmail:importance") = 2 
	cdoMail.Fields.Update
     
	'�����ʼ�
	dim sleepSeconds     
	sleepSeconds = 5
	cdoMail.Send
	WScript.Sleep(1000 * sleepSeconds)
     
	Set cdoMail = nothing
	Set cdoConf = nothing
End function

Function GetFileHash(fileName)'��ֵ������ﲻ����·����ֻ���õ�һ���ļ���
	Dim objWI, objFileHash
	Dim file_Size, fileAttributes, fileVersion, hashValue
	Dim i
	Set objWI = CreateObject("WindowsInstaller.Installer")
	file_Size = objWI.FileSize(STRING_NAME_HWDETECT)
	fileAttributes = objWI.FileAttributes(STRING_NAME_HWDETECT)
	fileVersion = objWI.FileVersion(STRING_NAME_HWDETECT)
	
	Set objFileHash = objWI.FileHash(STRING_NAME_HWDETECT, 0)
	hashValue = ""
	For i = 1 To objFileHash.FieldCount
		hashValue = hashValue & BigEndianHex(objFileHash.IntegerData(i))
	Next
	GetFileHash = hashValue

	HWDebug("File: " & filePath & vbCrLf & _
		"Size: " & file_Size & vbCrLf & _
		"Attributes: " & fileAttributes & vbCrLf & _
		"Version: " & fileVersion & vbCrLf & _
		"MD5: " & hashValue) 
	
	Set objFileHash = Nothing
	Set objWI = Nothing
End Function

Function BigEndianHex(Int)
	Dim result
	Dim b1, b2, b3, b4
	result = Hex(Int)
	b1 = Mid(result, 7, 2)
	b2 = Mid(result, 5, 2)
	b3 = Mid(result, 3, 2)
	b4 = Mid(result, 1, 2)
	BigEndianHex = b1 & b2 & b3 & b4
End Function

'----�ж��ļ��Ƿ���ڣ����ڷ���True�����򷵻�False
Function HWIsFileExist(filePath)
	Dim ObjFileSystem
	Set ObjFileSystem = CreateObject("Scripting.FileSystemObject")
	If ObjFileSystem.FileExists(filePath) Then
		HWIsFileExist = True'������ֵ�ķ����Ǹ���������ֵ
	Else
		HWIsFileExist = False
	End If
	
	Set ObjFileSystem = nothing
End Function

Function GetUnixTime() 
	Dim objWMIService, colItems
	Set objWMIService = GetObject("winmgmts:\\.\root\cimv2") 
	Set colItems = objWMIService.ExecQuery("Select * from Win32_OperatingSystem",,48) 
	For Each objItem in colItems 
		TimeZone = objItem.CurrentTimeZone 
	Next 
	GetUnixTime = DateDiff("s", "01/01/1970 00:00:00", Now()) 
	GetUnixTime = GetUnixTime - TimeZone * 60 
	
	Set objWMIService = nothing
	Set colItems = nothing
End Function 

'----MsgBoxʽ��־��������HW_DEBUG_TYPE���ƴ����
Function HWDebug(content)
	If (1 = HW_DEBUG_TYPE) Then 
		MsgBox(content)
	ElseIf (2 = HW_DEBUG_TYPE) Then
		Dim cmd
		content = Date() & " " &Time() & ": " &content
		cmd = "cmd.exe /C @echo " &content &" >> " &STRING_PATH_VBS_LOG
		Set objWS = CreateObject("WScript.Shell")
		objWS.run cmd, 0, True
		
		if (HWGetFileSize(STRING_PATH_VBS_LOG) > VALUE_SIZE_LOG_MAX) Then
			cmd = "cmd.exe /C @echo --------------" &" > " &STRING_PATH_VBS_LOG
			objWS.run cmd, 0, True
		End If
		
		Set objWS = nothing
	End If
End Function

'----��ȡ��ǰ��������Ӧ�ó����ע�����Ϣ
Function GetCurrentAppExeRegStatus()
	Dim ObjWShShell, ObjRegOperate, nRet
	Set ObjWShShell = CreateObject("WScript.Shell")
	Set ObjRegOperate = GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & "." & "\root\default:StdRegProv")
	
	g_strCurrentAppExePath = ""
	Rem: ����0��ʾ�ɹ�������1��ʾִ��ʧ�ܣ����ܲ����ڶ�Ӧ�ļ�����
	nRet = ObjRegOperate.GetStringValue(HKEY_LOCAL_MACHINE, STRING_SUB_REG_RUN_APP, STRING_NAME_APP_ROOT_DIR, g_strCurrentAppExePath)
	Rem: ����һ�ֶ�ע���ķ��������ǳ���ᵯ��ϵͳ�Ի��򣬲��ÿ���--->
	Rem: g_strCurrentAppExePath = ObjWShShell.RegRead(STRING_KEY_REG_RUN_APP)
	
	If (1 = nRet) Then
		GetCurrentAppExeRegStatus = VALUE_REG_READ_ERROR
	ElseIf (Len(g_strCurrentAppExePath) <= 0) Then
		GetCurrentAppExeRegStatus = VALUE_REG_KEYVALUE_INVALID
	Else
		If (HWIsFileExist(g_strCurrentAppExePath)) Then
			'HWDebug("Ӧ�ó���������ļ����ڣ�" & g_strCurrentAppExePath)
			GetCurrentAppExeRegStatus = 0
		Else
			'HWDebug("Ӧ�ó���������ļ������ڣ�" & g_strCurrentAppExePath)
			Rem: ��Ҫ�ж���һ��Ŀ¼�Ƿ���ڣ�����������Ҫ���ز���װ
			GetCurrentAppExeRegStatus = VALUE_REG_KEYVALUE_INVALID
		End If
	End If
	
	Set ObjWShShell = nothing
	Set ObjRegOperate = nothing
End Function


'----HumanHealthIndicatorӦ�ó����ļ��������ļ��,����0��ʾ�У�����1��ʾ��
Function HumanHealthIndicatorFileDetect()
	Dim nRet
	nRet = GetCurrentAppExeRegStatus()
	If (0 = nRet) Then
		HWDebug("Ӧ�ó���������ļ����ڣ�"&g_strCurrentAppExePath)
		HumanHealthIndicatorFileDetect = 0
	ElseIf (HWIsFileExist(STRING_PATH_APP1_DEFAULT & STRING_NAME_APPLICATION_EXE)) Then
		g_strCurrentAppExePath = (STRING_PATH_APP1_DEFAULT & STRING_NAME_APPLICATION_EXE)
		HWDebug("�Զ�ѡȡʹ��appִ�г���Ϊ: " &g_strCurrentAppExePath)
		HumanHealthIndicatorFileDetect = 0
	ElseIf (HWIsFileExist(STRING_PATH_APP2_DEFAULT & STRING_NAME_APPLICATION_EXE)) Then
		g_strCurrentAppExePath = (STRING_PATH_APP2_DEFAULT & STRING_NAME_APPLICATION_EXE)
		HWDebug("�Զ�ѡȡʹ��appִ�г���Ϊ: " &g_strCurrentAppExePath)
		HumanHealthIndicatorFileDetect = 0
	Else
		g_strCurrentAppExePath = ""
		HWDebug("Ӧ�ó���������ļ������ڣ�"&g_strCurrentAppExePath)
		HumanHealthIndicatorFileDetect = 1
	End If

End Function

'----HWUpdate���������ļ��������ļ�⣬������Ҫ��Ӧ��ǰapp exe·������
Function HWUpdateFileDetect()
	If (Len(g_strCurrentAppExePath) > 0) Then
		If (InStr(g_strCurrentAppExePath, "app1") > 0) Then'�ж�·�������Ƿ���app1
			If (HWIsFileExist(STRING_PATH_APP1_DEFAULT & STRING_NAME_UPDATE_EXE)) Then
				g_strCurrentUpdateExePath = STRING_PATH_APP1_DEFAULT & STRING_NAME_UPDATE_EXE
				HWDebug("��⵽��ǰ���������·��Ϊ: "&g_strCurrentUpdateExePath)
				HWUpdateFileDetect = 0
			ElseIf (HWIsFileExist(STRING_PATH_APP2_DEFAULT & STRING_NAME_UPDATE_EXE)) Then
				g_strCurrentUpdateExePath = STRING_PATH_APP2_DEFAULT & STRING_NAME_UPDATE_EXE
				HWDebug("��⵽��ǰ���������·��Ϊ: "&g_strCurrentUpdateExePath)
				HWUpdateFileDetect = 0
			Else
				g_strCurrentUpdateExePath = ""
				HWUpdateFileDetect = VALUE_UPDATE_EXE_NOT_EXIT
			End If
		ElseIf (InStr(g_strCurrentAppExePath, "app2") > 0) Then
			If (HWIsFileExist(STRING_PATH_APP2_DEFAULT & STRING_NAME_UPDATE_EXE)) Then
				g_strCurrentUpdateExePath = STRING_PATH_APP2_DEFAULT & STRING_NAME_UPDATE_EXE
				HWDebug("��⵽��ǰ���������·��Ϊ: "&g_strCurrentUpdateExePath)
				HWUpdateFileDetect = 0
			ElseIf (HWIsFileExist(STRING_PATH_APP1_DEFAULT & STRING_NAME_UPDATE_EXE)) Then
				g_strCurrentUpdateExePath = STRING_PATH_APP1_DEFAULT & STRING_NAME_UPDATE_EXE
				HWDebug("��⵽��ǰ���������·��Ϊ: "&g_strCurrentUpdateExePath)
				HWUpdateFileDetect = 0
			Else
				g_strCurrentUpdateExePath = ""
				HWUpdateFileDetect = VALUE_UPDATE_EXE_NOT_EXIT
			End If
		Else
			HWDebug("û���ҵ�Ӧ�ó����������������ļ��Ĵ���.")
			g_strCurrentUpdateExePath = ""
			HWUpdateFileDetect = VALUE_UPDATE_EXE_NOT_EXIT
		End If
	Else
		g_strCurrentAppExePath = ""
		g_strCurrentUpdateExePath = ""
		HWUpdateFileDetect = VALUE_UPDATE_EXE_NOT_EXIT
		HWDebug("Ӧ�ó���·��Ϊ�գ���Ҫ���������޸�...")
	End If
End Function

'----�����Ƿ���ڵ��ж�
Function IsProcessExist(processName, bFullWords)
	Dim objWMI, objsProcess, strProcessName, bRet
	Set objWMI = GetObject("WinMgmts:")
	Set objsProcess = objWMI.InstancesOf("Win32_Process")
	strProcessName = ""
	bRet = False
	For Each obj In objsProcess
		strProcessName = obj.Description
		If (bFullWords) Then
			'If (LCase(strProcessName) = LCase(processName)) Then
			Rem: �Ƚ�ʱ���ִ�Сд
			If (strProcessName = processName) Then
				bRet = True
				If (strProcessName = STRING_NAME_APPLICATION_EXE) Then'ͬʱ��ȡӦ�ó����ļ�����·��
					g_strCurrentAppExePath = obj.ExecutablePath
					HWDebug("�������е�Ӧ�ó���·��Ϊ: " &g_strCurrentAppExePath)
				End If
				Exit For
			End If
		Else
			'If (InstrRev(LCase(strProcessName), LCase(processName)) > 0) Then
			If (InstrRev(strProcessName, processName) > 0) Then
				bRet = True
				Exit For
			End If
		End If
	Next
	
	IsProcessExist = bRet
	Set objWMI = nothing
	Set objsProcess = nothing
End Function

'----Ӧ�ó�����������������״̬�ж�
Function GetProcessStatus()
	if (IsProcessExist(STRING_NAME_APPLICATION_EXE, True)) Then
		HWDebug("��⵽Ӧ�ó�����������......")
		If (IsProcessExist(STRING_NAME_UPDATE_EXE, True)) Then
			HWDebug("Ӧ�ó�����������������У�һ������...")
			GetProcessStatus = VALUE_PROCESS_ALL_OK
		Else
			HWDebug("��⵽��������û������......")
			GetProcessStatus = VALUE_PROCESS_UPDATE_NOK
		End If
	Else
		HWDebug("��⵽Ӧ�ó���û������......")
		If (IsProcessExist(STRING_NAME_UPDATE_EXE, True)) Then
			HWDebug("��⵽����������������......")
			GetProcessStatus = VALUE_PROCESS_APP_NOK
		Else
			HWDebug("��⵽��������û������.......")
			GetProcessStatus = VALUE_PROCESS_ALL_NOK
		End If
	End If
End Function

'----Ӧ�ó��������������ļ�����״̬���ж�(��ʱû��ʹ��)
Function CheckProcessFilesStatus()
	Rem: ���ж�ע������濪���������ļ��Ƿ���ڼ�ֵ�Ƿ�����
	Dim nRet
	nRet = GetCurrentAppExeRegStatus()
	If (0 = nRet) Then
		Rem: Ӧ�ó������
		nRet = HWUpdateFileDetect()
		If (0 = nRet) Then
			Rem: ��������Ҳ����
			CheckProcessFilesStatus = 0
		Else
			CheckProcessFilesStatus = VALUE_UPDATE_EXE_NOT_EXIT
			g_strCurrentAppExePath = ""'����ʹ��Empty�����и�ֵ�������ж�ʱ��Ҫʹ��IsEmpty()
		End If
	Else
		Rem: �ж��Ƿ���app�İ�װĿ¼����س������û������Ҫ���أ�
		CheckProcessFilesStatus = VALUE_APP_EXE_NOT_EXIT
	End If
End Function

'----��ȡ�ļ���С
Function HWGetFileSize(strFilePath)
	Dim objFileSystem, file
	Set objFileSystem = CreateObject("Scripting.FileSystemObject")
	If (objFileSystem.FileExists(strFilePath)) Then
		Set file = objFileSystem.GetFile(strFilePath)
		HWGetFileSize = file.size
	Else
		HWGetFileSize = 0
	End If
End Function

Function HWKillProcess(strProcess)
	Dim objWS
	Set objWS = CreateObject("WScript.shell")
	objWS.run "cmd.exe /C Taskkill /f /im " & strProcess, 0'0�����ִ̨�У�1��ǰִ̨�У������Ƿ����ʾcmd.exe���棬Ĭ����ǰִ̨��
	Set objWS = nothing
End Function

'�еķ�������֧��http���ļ�ͷ����������ϸ�Ҫ��
Function GetFileSizeFromLog(strLogFilePath)
	Dim objWS, objExeResult, cmd, strResult, nFileSize
	
	Set objWS = CreateObject("WScript.shell")
	cmd = "cmd.exe /C findstr /I ""Length"" "&strLogFilePath
	Set objExeResult = objWS.exec(cmd)
	strResult = objExeResult.StdOut.ReadAll()
	strResult = replace(strResult, vbNewLine, "")'���з�ֻ��ʹ��vbNewLine�ſ�ʹ��ʹ��'\n'����Ч
	
	cmd = "cmd.exe /C For /F ""tokens=2 delims= "" %i in (""" &strResult &""") do @echo %i"
	Set objExeResult = objWS.exec(cmd)
	strResult = objExeResult.StdOut.ReadAll()
	strResult = replace(strResult, ",", "")
	
	If (IsNumeric(strResult)) Then
		nFileSize = Int(strResult)
		HWDebug("����־��ȡ������Ҫ���ص��ļ���С(byte)��" &nFileSize)
	Else
		nFileSize = 0
		HWDebug("����־�ļ��л�ȡ�����ļ���Сʧ�ܣ�")
	End If
	
	cmd = "cmd.exe /C del /F /Q " &strLogFilePath
	objWS.run cmd, 0, True
	GetFileSizeFromLog = nFileSize
	
	Set objWS = nothing
	Set objExeResult = nothing
End Function

Function GetFileSizeFromServer(strUrl)
	Dim objWS, objExeResult, cmd, strResult, nFileSize
	Set objWS = CreateObject("WScript.shell")
	
	'ȷ����ȫ����ɾ���������ݼ�¼�ļ�
	cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_LOG
	objWS.run cmd, 0, True
	
	'-o���������־���ļ�,--spider�����أ��Դ�ȡ���ļ���С
	HWDebug("�����ӷ�������ȡ�����ļ���С(byte)��"&strUrl)
	cmd = "cmd.exe /C " &g_strWgetExePath &" -o " &STRING_PATH_DOWNLOAD_LOG &" --spider " &strUrl
	objWS.run cmd, 0, True
	
	cmd = "cmd.exe /C findstr /I ""Length"" "&STRING_PATH_DOWNLOAD_LOG
	Set objExeResult = objWS.exec(cmd)
	strResult = objExeResult.StdOut.ReadAll()
	strResult = replace(strResult, vbNewLine, "")'���з�ֻ��ʹ��vbNewLine�ſ�ʹ��ʹ��'\n'����Ч
	
	cmd = "cmd.exe /C For /F ""tokens=2 delims= "" %i in (""" &strResult &""") do @echo %i"
	Set objExeResult = objWS.exec(cmd)
	strResult = objExeResult.StdOut.ReadAll()
	strResult = replace(strResult, ",", "")
	
	If (IsNumeric(strResult)) Then
		nFileSize = Int(strResult)
		HWDebug("�����ص��ļ���С(byte)��" &nFileSize)
	Else
		nFileSize = 0
		HWDebug("�ӷ�������ȡ�����ļ���Сʧ�ܣ�")
	End If
	
	cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_LOG
	objWS.run cmd, 0, True
	GetFileSizeFromServer = nFileSize
	
	Set objWS = nothing
	Set objExeResult = nothing
End Function

Function InstallHWApplication(strPath)
	Dim objWS
	HWKillProcess("update*")'�Ƚ��������ܴ��ڵİ�װ����
	HWKillProcess("CureHealth*")
	WScript.sleep(3000)
	Set objWS = CreateObject("WScript.shell")
	HWDebug("��װ��������ǰ�װ��...")
	InstallHWApplication = objWS.run(strPath, 0, True)
	
	Set objWS = nothing
End Function

Function DownloadAndInstall()
	Dim objWS, cmd, nFileSize, strSavePath, strUrl, nRet
	Dim strMessage
	
	strSavePath = STRING_PATH_DOWNLOAD_PREFIX &STRING_NAME_INSTALL_FILE
	strUrl = STRING_URL_INSTALL_PREFIX &STRING_NAME_INSTALL_FILE
	
	nFileSize = GetFileSizeFromServer(strUrl)
	If ((nFileSize < 1000) or (nFileSize > 200000000)) Then
		HWDebug("���󵽵������ļ���С�쳣(byte)��"&nFileSize)
		DownloadAndInstall = 1
		strMessage = "�޸��������������صİ�װ�ļ������쳣����ȡ���Ĵ�СΪ(byte)��"&nFileSize &"(" &strUrl &")"
		HWSendEmail(strMessage)
		Exit Function
	End If
	
	Set objWS = CreateObject("WScript.shell")
		
	If (nFileSize = HWGetFileSize(strSavePath)) Then
		HWDebug("��װ�ļ�֮ǰ�Ѿ�������أ���ֱ�Ӱ�װ......")
		DownloadAndInstall = InstallHWApplication(strSavePath)
		Set objWS = nothing
		Exit Function
	Else
		If (not HWIsFileExist(strSavePath)) Then
			'ɾ��֮ǰ���ص���Ч�ļ�
			HWDebug("ɾ��������Ч���ļ�......")
			cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_PREFIX &"CureHealth*"
			objWS.run cmd, 0, True
		End If
		HWDebug("׼�����أ�")
	End If
	

	cmd = "cmd.exe /C " &g_strWgetExePath &" -q -c -P " &STRING_PATH_DOWNLOAD_PREFIX &" " & strUrl
	objWS.run cmd, 0, True
	
	If (nFileSize = HWGetFileSize(strSavePath)) Then
		HWDebug("��װ�ļ����سɹ�����ʼ��װ.....")
		nRet = InstallHWApplication(strSavePath)
		strMessage = "��װ�޸����Ϊ��"&nRet &"(" &strUrl &")"
		HWSendEmail(strMessage)
		DownloadAndInstall = nRet
	Else
		HWDebug("��װ�ļ�����ʧ�ܣ�")
		strMessage = "��װ�ļ�����ʧ�ܣ��ܴ�СΪ(byte)��"&nFileSize &"(" &strUrl &")"
		HWSendEmail(strMessage)
		DownloadAndInstall = 1
	End If
	
	Set objWS = nothing
End Function

Function StartAppExe()
	'���Ǵ�ע�����ȡ����ǰHumanIndicator���ڵ�·��
	'���ע��������ݷǷ�������Ҫ��Ϊȥ���ң�������Ҫ�������°�װ
	If (0 = HumanHealthIndicatorFileDetect()) Then
		Dim objWS
		Set objWS = CreateObject("WScript.shell")
		HWDebug("����Ӧ�ó���...")
		g_strCurrentAppExePath = Replace(g_strCurrentAppExePath, "Program Files", "Progra~1")
		objWS.run "cmd.exe /C " &g_strCurrentAppExePath, 0
		Set objWS = nothing
		'������ʱû�п��ǳ��������쳣�ĳ���
	Else
		'���ذ�װ������װ
		DownloadAndInstall()
	End If
End Function

Function StartUpdateExe()
	If (0 = HWUpdateFileDetect()) Then
		Dim objWS
		Set objWS = CreateObject("WScript.shell")
		HWDebug("������������...")
		
		Dim strMessage
		strMessage = "��⵽�������������쳣������������������..."
		HWSendEmail(strMessage)
		
		g_strCurrentUpdateExePath = Replace(g_strCurrentUpdateExePath, "Program Files", "Progra~1")
		objWS.run "cmd.exe /C " &g_strCurrentUpdateExePath, 0
		Set objWS = nothing
		'������ʱû�п��ǳ��������쳣�ĳ���
	Else
		'���ذ�װ������װ
		DownloadAndInstall()
	End If
End Function

'----��⵽���н��̶������к�Ĵ���
Function HandleProcessAllOK()
	'�������������־�Ƿ�����������������־����û�����ӣ���˵���������������������Ҫ������������
	Dim nTemp, nStatus
	HWDebug("���г���������������ʼ�����־ϵͳ...")
	g_nUpdateLogSize = HWGetFileSize(STRING_PATH_LOG_UPDATE)
	HWDebug("��ǰ������־��С(byte)��"&g_nUpdateLogSize)
	If (g_nUpdateLogSize > 0) Then
		WScript.sleep(VALUE_TIME_LOG_DETECT_AGAIN)'�ȴ�40���ӣ���������һ��15����һ���������ڼ�����־���
		nStatus = GetProcessStatus()'�ٴ��жϣ���ֹ��������ά��Ա�ڵ���
		if (VALUE_PROCESS_ALL_OK = nStatus) Then
			nTemp = HWGetFileSize(STRING_PATH_LOG_UPDATE)
			HWDebug("��ǰ������־��С(byte)��"&nTemp)
			if (not (nTemp <> g_nUpdateLogSize)) Then'��־��Сδ�䣬���ﲻ����<=����Ϊ��־���ܻع�
				HWDebug("��־ϵͳ�쳣������������������...")
				
				Dim strMessage
				strMessage = "��⵽��־ϵͳ�쳣������������������..."
				HWSendEmail(strMessage)
				
				HWKillProcess(STRING_NAME_UPDATE_EXE)'ɱ���������̣�������Ӧ�ó�����������
			Else
				HWDebug("��־ϵͳ��������.")
			End If
		End If
	End If
End Function

'----��⵽���н��̶�û�����к�Ĵ���
Function HandleProcessAllNOK()
	If (IsProcessExist(STRING_NAME_UPDATE_INSTALL_EXE, False)) Then
		'���ܴ����������̵���
		WScript.sleep(300000)'�ȴ�5����    'sleep��ʵ�֣���λ��ms
	Else
		WScript.sleep(60000)'�ȴ�60��
	End If
	
	Dim nStatus
	nStatus = GetProcessStatus()
	If (VALUE_PROCESS_ALL_NOK = nStatus) Then
		'�����ǿ�������ά��Ա�ڶ��豸���а�װ����ԣ���Ҫ������ʾ
		Dim nRet, objWS, i, strInfo
		Set objWS = CreateObject("WScript.shell")
		'popup�ľ����÷����Բο��ٶȰٿƵ�WScript.shell
		For i = 60 To 0 Step -1
			strInfo = "�����˼�⵽����ǳ���δ�������У������˽���" &CStr(i)& "����Զ����������Ƿ�����"
			nRet = objWS.popup(strInfo, 1, "��Ҫ����", 4 + 32)
			
			If (7 = nRet) Then'7�������˰�����ȡ����ť,��ʱ���Զ���������
				HWDebug("�п�������ά��Ա����ά���豸��ȡ������������س������.")
				Exit Function
			ElseIf (6 = nRet) Then
				Exit For
			End If
		Next
		HWDebug("��⵽Ӧ�ó��������������δ���У�����취����Ӧ�ó���...")
		
		Dim strMessage
		strMessage = "��⵽Ӧ�ó�������������δ���У������������ܼ�����޸�..."
		HWSendEmail(strMessage)
		
		StartAppExe()
		WScript.sleep(3000)
		StartUpdateExe()
	Else
		HandleProcessStatus(nStatus)
	End If
End Function

'----��⵽Ӧ�ó���û�����еĴ���
Function HandleProcessAppNOK()
	WScript.sleep(VALUE_TIME_APPLICATION_DETECT_AGAIN)'�ȴ�3���ӣ�������ʱ��������
	Dim nStatus
	nStatus = GetProcessStatus()
    If (VALUE_PROCESS_APP_NOK = nStatus) Then
		HWDebug("��⵽Ӧ�ó���δ���У�����취����...")
		
		Dim strMessage
		strMessage = "��⵽Ӧ�ó���δ���У������������ܼ�����޸�..."
		HWSendEmail(strMessage)
		
		StartAppExe()
	End If
End Function

'----��⵽��������û�����к�Ĵ���
Function HandleProcessUpdateNOK()
	WScript.sleep(VALUE_TIME_UPDATE_DETECT_AGAIN)'sleep 20���ӣ������Ǹտ�����Ӧ�ó���û�н���������������
	Dim nRet
	nRet = GetProcessStatus()
	if (VALUE_PROCESS_UPDATE_NOK = nRet) Then
		HWDebug("��⵽��������û�����У�����취����...")
		
		Dim strMessage
		strMessage = "��⵽��������δ���У������������ܼ�����޸�..."
		HWSendEmail(strMessage)
		
		StartUpdateExe()
	End If
End Function

Function HandleProcessStatus(nProcessStatus)
	Select Case nProcessStatus
		Case VALUE_PROCESS_ALL_OK
			HandleProcessAllOK()
		Case VALUE_PROCESS_ALL_NOK
			HandleProcessAllNOK()
		Case VALUE_PROCESS_APP_NOK
			HandleProcessAppNOK()
		Case VALUE_PROCESS_UPDATE_NOK
			HandleProcessUpdateNOK()
	End Select
End Function

Function HWInit()
	If (HWIsFileExist(STRING_PATH_WGET_DEFAULT)) Then
		g_strWgetExePath = STRING_PATH_WGET_DEFAULT
	ElseIf (HWIsFileExist(STRING_PATH_WGET_app1)) Then
		g_strWgetExePath = STRING_PATH_WGET_app1
	ElseIf (HWIsFileExist(STRING_PATH_WGET_app2)) Then
		g_strWgetExePath = STRING_PATH_WGET_app2
	End If
	HWDebug("wget: "&g_strWgetExePath)
End Function

'----�ű��������(ͨ�����������������������ű��ļ�)
Function UpdateDetect()
	g_nUpdateDetectTimes = g_nUpdateDetectTimes + 1
	HWDebug(".........." &g_nUpdateDetectTimes)
	If ((g_nUpdateDetectTimes > 1) and (g_nUpdateDetectTimes <= VALUE_MAX_UPDATE_DETECT)) Then
		Exit Function
	Else
		g_nUpdateDetectTimes = 1
	End IF
	HWDebug("Start update detect: " &g_nUpdateDetectTimes)
	
	Dim MD5Current, downloadFileSize, strMD5SavePath, strVBSSavePath, objFileSystem, objFileOpen, strMD5ReadLine, strVBSUrl, strMD5Url
	strMD5SavePath = (STRING_PATH_DOWNLOAD_PREFIX &STRING_NAME_HWDETECT_MD5)
	strMD5Url = (STRING_URL_VBS_PREFIX &STRING_NAME_HWDETECT_MD5)
	
	MD5Current = GetFileHash(STRING_NAME_HWDETECT)
	MD5Current = LCase(MD5Current)
	
	Set objWS = CreateObject("WScript.shell")
	Set objFileSystem = CreateObject("Scripting.FileSystemObject")
	'md5�ļ�
	cmd = "cmd.exe /C del /F /Q " &strMD5SavePath
	objWS.run cmd, 0, True
	
	'�����Ƿ��жϺ���ִ�гɹ�
	'������MD5�ļ����ٸ���������־�ж��ļ��Ƿ����سɹ�
	'ɾ����־�ļ�
	cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_LOG
	objWS.run cmd, 0, True
	'����MD5�ļ��������ع����������־�ļ�
	cmd = "cmd.exe /C " &g_strWgetExePath &" -o " &STRING_PATH_DOWNLOAD_LOG &" -c -P " &STRING_PATH_DOWNLOAD_PREFIX &" " & strMD5Url
	objWS.run cmd, 0, True
	
	downloadFileSize = GetFileSizeFromLog(STRING_PATH_DOWNLOAD_LOG)
		
	If (not (HWGetFileSize(strMD5SavePath) <> downloadFileSize)) Then'md5�ļ����سɹ����
		Dim objExeResult
		cmd = "cmd.exe /C findstr ""."" "&strMD5SavePath
		Set objExeResult = objWS.exec(cmd)
		strMD5ReadLine = objExeResult.StdOut.ReadAll()
		strMD5ReadLine = FormatString(strMD5ReadLine)
		
		strVBSUrl = (STRING_URL_VBS_PREFIX &STRING_NAME_HWDETECT &"." &strMD5ReadLine)
		strVBSSavePath = (STRING_PATH_DOWNLOAD_PREFIX &STRING_NAME_HWDETECT &"." &strMD5ReadLine)
		If (MD5Current <> strMD5ReadLine) Then
			HWDebug("��ǰ�ű�hashΪ��" & MD5Current)
			HWDebug("���½ű�hashΪ��" & strMD5ReadLine)
			If (not HWIsFileExist(strVBSSavePath)) Then
				'ɾ��֮ǰ���ص�ʧЧ�ļ�
				cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_PREFIX &STRING_NAME_HWDETECT &".*"
				objWS.run cmd, 0, True
			End If
			
			'����VBS�ű��ļ�
			cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_LOG
			objWS.run cmd, 0, True
	
			cmd = "cmd.exe /C " &g_strWgetExePath &" -o " &STRING_PATH_DOWNLOAD_LOG &" -c -P " &STRING_PATH_DOWNLOAD_PREFIX &" " & strVBSUrl
			objWS.run cmd, 0, True
				
			downloadFileSize = GetFileSizeFromLog(STRING_PATH_DOWNLOAD_LOG)
			HWDebug("���󵽵�VBS�ļ���СΪ(byte): " &downloadFileSize)
			If (HWIsFileExist(strVBSSavePath)) Then
				Dim MD5Temp
				MD5Temp = GetFileHash(strVBSSavePath)
				If (MD5Temp <> strMD5ReadLine) Then
					objFileSystem.CopyFile strVBSSavePath, STRING_PATH_SYSTEM32 &STRING_NAME_HWDETECT, True
					HWDebug("VBS�ű��ļ����³ɹ���")
					g_bQuit = True'�ű��Զ��˳����ɼƻ���������
				Else
					HWDebug("VBS�ű��ļ����ظ���ʧ�ܣ�")
				End If
			End If
		Else
			HWDebug("��ǰ�ű������°汾������Ҫ���ظ���...")
		End If
	Else
		HWDebug("VBS MD5�ļ�����ʧ�ܣ�VBS���¼��ʧ�ܣ�")
	End If
	
	Set objWS = nothing
	Set objFileSystem = nothing
End Function

'----�ű�������
Function MainLoop()
	Set objWS = CreateObject("WScript.shell")
	host = WScript.FullName
	If LCase(right(host, len(host)-InStrRev(host,"\"))) = "wscript.exe" Then'����ͨ��WScript.exec��ʱ���д��ڵ���
		objWS.run "cscript """ & WScript.ScriptFullName & chr(34), 0
		WScript.Quit
	End If
	
	Dim nProcessStatus, nStartTime
	WScript.sleep(VALUE_TIME_BEFORE_DETECT)'�ȴ�3���ӣ��ر��ǿ���ʱ��
	nStartTime = GetUnixTime()
	HWDebug("")
	HWDebug("Welcome to HWDetect VBS.")
	While (not g_bQuit)
		HWDebug("VBS Detect start...")
		HWInit()
		nProcessStatus = GetProcessStatus()
		HandleProcessStatus(nProcessStatus)
		Dim nTempDate, nTempHour
		nTempDate = GetUnixTime()
		nTempHour = Hour(Now())
		if (nTempDate - nStartTime > 8640000) Then'���ܸտ�����ʱ���ȡ��ϵͳʱ���Ǵ�ģ������ݴ���'86400��3600*24����1���ʱ��
			nStartTime = nTempDate
		End If
		
		UpdateDetect()'���VBS�ű�����
		if ((nTempDate - nStartTime > VALUE_TIME_RESTART_PERIOD) and ((nTempHour >= VALUE_HOUR_MIN) and (nTempHour <= VALUE_HOUR_MAX))) Then
			'�����豸
			Dim objWS
			Set objWS = CreateObject("WScript.shell")
			HWDebug("Too long to restart system, restart now...")
			objWS.run "cmd.exe /C Shutdown /r /f /t 60 /c 'Ϊ�˸���Ϊ�����񣬻����˽�����һ�������Ż���ϵͳ���������Ժ�......'", 0
			Set objWS = nothing
		Else
			WScript.sleep(VALUE_TIME_DETECT_PERIOD)'�����Խ��м��
		End If

	WEnd
End Function

On Error Resume Next'�������(����������)ִ�г���������
MainLoop()
