Rem: *-------------------------------------------------------------------------*
Rem: * 该脚本为健康监测仪的守护脚本，仅在一定程度上对应用程序和升级程序进行守护
Rem: * 1、需要考虑升级过程和运维同事维护等场景。通过智能判断决定是否是升级过程及运维过程，如果是运维过程则进行倒计时提示的展示；
Rem: * 2、考虑日志系统可能存在异常，会导致升级程序阻塞，所以需要增加对日志文件大小判断来确定是否升级程序发生阻塞问题；或者考虑定时重启；
Rem: * 3、安装程序下载并进行安装功能
Rem: *-------------------------------------------------------------------------*

Rem:***注意事项
    '1、安装或升级脚本中需要对wget进行安装；
	'2、安装或升级脚本中需要对守护脚本进行安装；

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
Const HW_DEBUG_TYPE = 2'0: 无任何输出；1：弹窗输出；2：输出到日志

Const VALUE_REG_READ_ERROR = 1
Const VALUE_REG_KEYVALUE_INVALID = 2
Const VALUE_UPDATE_EXE_NOT_EXIT = 3
Const VALUE_APP_EXE_NOT_EXIT = 4
Const VALUE_PROCESS_ALL_OK = 5
Const VALUE_PROCESS_APP_NOK = 6
Const VALUE_PROCESS_UPDATE_NOK = 7
Const VALUE_PROCESS_ALL_NOK = 8

Const VALUE_TIME_BEFORE_DETECT = 180000'3000'(单位是毫秒)默认3分钟
Const VALUE_TIME_APPLICATION_DETECT_AGAIN = 180000'30000'(单位是毫秒)连续检测应用程序未运行的间隔时间，默认是3分钟
Const VALUE_TIME_UPDATE_DETECT_AGAIN = 1200000'30000'(单位是毫秒)连续检测升级程序未运行的间隔时间,默认20分钟
Const VALUE_TIME_LOG_DETECT_AGAIN = 2400000'60000'(单位是毫秒)连续对日志系统进行检测的间隔时间，默认是40分钟
Const VALUE_TIME_DETECT_PERIOD = 3600000'60000'(单位是毫秒)系统检测周期，默认是1小时

Const VALUE_TIME_RESTART_PERIOD = 86400'(单位是秒)86400 = 3600 * 24，一天
Const VALUE_HOUR_MIN = 1'用于逻辑判断的当天最小起始时间点，一天的范围为0~23
Const VALUE_HOUR_MAX = 5
Const VALUE_SIZE_LOG_MAX = 10000000'(单位是byte)
Const VALUE_SIZE_VBS_MAX = 3000000'(单位是byte)

Const VALUE_VERSION_VBS = "1.0"
Const VALUE_MAX_UPDATE_DETECT = 30

Rem: 全局变量定义
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
	strResult = replace(strInput, vbNewLine, "")'去掉回车与换行
	strResult = Trim(strResult)'去除空格
	FormatString = LCase(strResult)'转为小写
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
		cmd = "cmd.exe /C For /F ""tokens=2 delims=="" %i in (""" &strResult &""") do @echo %i"'注意in后面需有空格
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
	HWDebug("当前APP版本号为: "&nVersion)
End Function

Function GetRouterMac()
	Dim cmd, objResult, strResult
	Set objWS = CreateObject("WScript.shell")
	Set objResult = objWS.exec("cmd.exe /C arp -a | find /I ""00-e0-61""")
	strResult = objResult.StdOut.ReadAll()
	If (Len(strResult) > 0) Then'切记这里不能使用IsEmpty，否则一直为False
		strResult = replace(strResult, vbNewLine, "")'cmd返回的结果总会有换行符，只有使用VBS的常量vbNewLine才好使
		cmd = "cmd.exe /C For /F ""tokens=2 delims= "" %i in (""" &strResult &""") do @echo %i"
		Set objResult = objWS.exec(cmd)
		strResult = objResult.StdOut.ReadAll()
		strResult = FormatRouterMac(strResult)
		
		Dim lastChar, nValue'获取到的云碟LAN MAC后还需要作-1处理以取云碟WIFI的MAC
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
			Set objResult = objWS.exec("cmd.exe /C ipconfig -all | find ""物理地址""")
			strResult = objResult.StdOut.ReadAll()
			strResult = replace(strResult, vbNewLine, ":")
			cmd = "cmd.exe /C For /F ""tokens=2,4,6 delims=:"" %i in (""" &strResult &""") do @echo %i,%j,%k"'最多取三个可能的MAC地址
			Set objResult = objWS.exec(cmd)
			strResult = objResult.StdOut.ReadAll()
			strResult = FormatRouterMac(strResult)
			GetRouterMac = strResult
		End If
	End If
	HWDebug("获取到的入库MAC地址为: " &strResult)
End Function

function HWSendEmail(strEmailBody)
	HWDebug("Send email, message:" &strEmailBody)
	Set cdoMail = CreateObject("CDO.Message")  '创建CDO对象
	Set cdoConf = CreateObject("CDO.Configuration") '创建CDO配置文件对象
	cdoMail.From = "Cplusplus2017@163.com"
	cdoMail.To = "yangliwei@95051.com"
	'cdoMail.CC = "biyaxiong@95051.com"
	cdoMail.Subject = ("VBS Information ["&GetRouterMac() &"] [" &GetAppVersion &"] [" &VALUE_VERSION_VBS &"]")
	'邮件正文
	cdoMail.HTMLbody = strEmailBody & "</table></body></html>"
   
	cdoConf.Fields.Item ("http://schemas.microsoft.com/cdo/configuration/sendusing") = 2    '使用网络上的SMTP服务器而不是本地的SMTP服务器
	cdoConf.Fields.Item ("http://schemas.microsoft.com/cdo/configuration/smtpserver") = "smtp.163.com"    'SMTP服务器地址, 可以换成其他你要用的邮箱服务器或者ip
	cdoConf.Fields.Item ("http://schemas.microsoft.com/cdo/configuration/smtpserverport") = 25    '邮件服务器端口
	cdoConf.Fields.Item ("http://schemas.microsoft.com/cdo/configuration/smtpauthenticate") = 1    '服务器认证方式
	cdoConf.Fields.Item("http://schemas.microsoft.com/cdo/configuration/sendusername") = "Cplusplus2017@163.com" '发件人账号
	cdoConf.Fields.Item("http://schemas.microsoft.com/cdo/configuration/sendpassword") = "YouLe2017"    '发件人登陆邮箱密码
	cdoConf.Fields.Item("http://schemas.microsoft.com/cdo/configuration/smtpconnectiontimeout") = 60    '连接服务器的超时时间
	cdoConf.Fields.Update  
	Set cdoMail.Configuration = cdoConf
     
	'设置邮件的重要度和优先级
	cdoMail.Fields.Item("urn:schemas:mailheader:X-MSMail-Priority") = "High"
	cdoMail.Fields.Item("urn:schemas:mailheader:X-Priority") = 2 
	cdoMail.Fields.Item("urn:schemas:httpmail:importance") = 2 
	cdoMail.Fields.Update
     
	'发送邮件
	dim sleepSeconds     
	sleepSeconds = 5
	cdoMail.Send
	WScript.Sleep(1000 * sleepSeconds)
     
	Set cdoMail = nothing
	Set cdoConf = nothing
End function

Function GetFileHash(fileName)'奇怪的是这里不能用路径，只能用单一的文件名
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

'----判断文件是否存在，存在返回True，否则返回False
Function HWIsFileExist(filePath)
	Dim ObjFileSystem
	Set ObjFileSystem = CreateObject("Scripting.FileSystemObject")
	If ObjFileSystem.FileExists(filePath) Then
		HWIsFileExist = True'赋返回值的方法是给函数名赋值
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

'----MsgBox式日志弹窗，由HW_DEBUG_TYPE控制打开与否
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

'----获取当前开机启动应用程序的注册表信息
Function GetCurrentAppExeRegStatus()
	Dim ObjWShShell, ObjRegOperate, nRet
	Set ObjWShShell = CreateObject("WScript.Shell")
	Set ObjRegOperate = GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & "." & "\root\default:StdRegProv")
	
	g_strCurrentAppExePath = ""
	Rem: 返回0表示成功，返回1表示执行失败，可能不存在对应的键名等
	nRet = ObjRegOperate.GetStringValue(HKEY_LOCAL_MACHINE, STRING_SUB_REG_RUN_APP, STRING_NAME_APP_ROOT_DIR, g_strCurrentAppExePath)
	Rem: 另外一种读注册表的方法，但是出错会弹出系统对话框，不好控制--->
	Rem: g_strCurrentAppExePath = ObjWShShell.RegRead(STRING_KEY_REG_RUN_APP)
	
	If (1 = nRet) Then
		GetCurrentAppExeRegStatus = VALUE_REG_READ_ERROR
	ElseIf (Len(g_strCurrentAppExePath) <= 0) Then
		GetCurrentAppExeRegStatus = VALUE_REG_KEYVALUE_INVALID
	Else
		If (HWIsFileExist(g_strCurrentAppExePath)) Then
			'HWDebug("应用程序二进制文件存在：" & g_strCurrentAppExePath)
			GetCurrentAppExeRegStatus = 0
		Else
			'HWDebug("应用程序二进制文件不存在：" & g_strCurrentAppExePath)
			Rem: 需要判断另一个目录是否存在，不存在则需要下载并安装
			GetCurrentAppExeRegStatus = VALUE_REG_KEYVALUE_INVALID
		End If
	End If
	
	Set ObjWShShell = nothing
	Set ObjRegOperate = nothing
End Function


'----HumanHealthIndicator应用程序文件存在与否的检测,返回0表示有，返回1表示无
Function HumanHealthIndicatorFileDetect()
	Dim nRet
	nRet = GetCurrentAppExeRegStatus()
	If (0 = nRet) Then
		HWDebug("应用程序二进制文件存在："&g_strCurrentAppExePath)
		HumanHealthIndicatorFileDetect = 0
	ElseIf (HWIsFileExist(STRING_PATH_APP1_DEFAULT & STRING_NAME_APPLICATION_EXE)) Then
		g_strCurrentAppExePath = (STRING_PATH_APP1_DEFAULT & STRING_NAME_APPLICATION_EXE)
		HWDebug("自动选取使用app执行程序为: " &g_strCurrentAppExePath)
		HumanHealthIndicatorFileDetect = 0
	ElseIf (HWIsFileExist(STRING_PATH_APP2_DEFAULT & STRING_NAME_APPLICATION_EXE)) Then
		g_strCurrentAppExePath = (STRING_PATH_APP2_DEFAULT & STRING_NAME_APPLICATION_EXE)
		HWDebug("自动选取使用app执行程序为: " &g_strCurrentAppExePath)
		HumanHealthIndicatorFileDetect = 0
	Else
		g_strCurrentAppExePath = ""
		HWDebug("应用程序二进制文件不存在："&g_strCurrentAppExePath)
		HumanHealthIndicatorFileDetect = 1
	End If

End Function

'----HWUpdate升级程序文件存在与否的检测，这里需要对应当前app exe路径而言
Function HWUpdateFileDetect()
	If (Len(g_strCurrentAppExePath) > 0) Then
		If (InStr(g_strCurrentAppExePath, "app1") > 0) Then'判断路径当中是否有app1
			If (HWIsFileExist(STRING_PATH_APP1_DEFAULT & STRING_NAME_UPDATE_EXE)) Then
				g_strCurrentUpdateExePath = STRING_PATH_APP1_DEFAULT & STRING_NAME_UPDATE_EXE
				HWDebug("检测到当前升级程序的路径为: "&g_strCurrentUpdateExePath)
				HWUpdateFileDetect = 0
			ElseIf (HWIsFileExist(STRING_PATH_APP2_DEFAULT & STRING_NAME_UPDATE_EXE)) Then
				g_strCurrentUpdateExePath = STRING_PATH_APP2_DEFAULT & STRING_NAME_UPDATE_EXE
				HWDebug("检测到当前升级程序的路径为: "&g_strCurrentUpdateExePath)
				HWUpdateFileDetect = 0
			Else
				g_strCurrentUpdateExePath = ""
				HWUpdateFileDetect = VALUE_UPDATE_EXE_NOT_EXIT
			End If
		ElseIf (InStr(g_strCurrentAppExePath, "app2") > 0) Then
			If (HWIsFileExist(STRING_PATH_APP2_DEFAULT & STRING_NAME_UPDATE_EXE)) Then
				g_strCurrentUpdateExePath = STRING_PATH_APP2_DEFAULT & STRING_NAME_UPDATE_EXE
				HWDebug("检测到当前升级程序的路径为: "&g_strCurrentUpdateExePath)
				HWUpdateFileDetect = 0
			ElseIf (HWIsFileExist(STRING_PATH_APP1_DEFAULT & STRING_NAME_UPDATE_EXE)) Then
				g_strCurrentUpdateExePath = STRING_PATH_APP1_DEFAULT & STRING_NAME_UPDATE_EXE
				HWDebug("检测到当前升级程序的路径为: "&g_strCurrentUpdateExePath)
				HWUpdateFileDetect = 0
			Else
				g_strCurrentUpdateExePath = ""
				HWUpdateFileDetect = VALUE_UPDATE_EXE_NOT_EXIT
			End If
		Else
			HWDebug("没有找到应用程序和升级程序二制文件的存在.")
			g_strCurrentUpdateExePath = ""
			HWUpdateFileDetect = VALUE_UPDATE_EXE_NOT_EXIT
		End If
	Else
		g_strCurrentAppExePath = ""
		g_strCurrentUpdateExePath = ""
		HWUpdateFileDetect = VALUE_UPDATE_EXE_NOT_EXIT
		HWDebug("应用程序路径为空，需要进行智能修复...")
	End If
End Function

'----进程是否存在的判断
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
			Rem: 比较时区分大小写
			If (strProcessName = processName) Then
				bRet = True
				If (strProcessName = STRING_NAME_APPLICATION_EXE) Then'同时获取应用程序文件所在路径
					g_strCurrentAppExePath = obj.ExecutablePath
					HWDebug("正在运行的应用程序路径为: " &g_strCurrentAppExePath)
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

'----应用程序与升级程序运行状态判断
Function GetProcessStatus()
	if (IsProcessExist(STRING_NAME_APPLICATION_EXE, True)) Then
		HWDebug("检测到应用程序运行正常......")
		If (IsProcessExist(STRING_NAME_UPDATE_EXE, True)) Then
			HWDebug("应用程序和升级程序都在运行，一切正常...")
			GetProcessStatus = VALUE_PROCESS_ALL_OK
		Else
			HWDebug("检测到升级程序没有运行......")
			GetProcessStatus = VALUE_PROCESS_UPDATE_NOK
		End If
	Else
		HWDebug("检测到应用程序没有运行......")
		If (IsProcessExist(STRING_NAME_UPDATE_EXE, True)) Then
			HWDebug("检测到升级程序运行正常......")
			GetProcessStatus = VALUE_PROCESS_APP_NOK
		Else
			HWDebug("检测到升级程序没有运行.......")
			GetProcessStatus = VALUE_PROCESS_ALL_NOK
		End If
	End If
End Function

'----应用程序与升级程序文件存在状态的判断(暂时没有使用)
Function CheckProcessFilesStatus()
	Rem: 先判断注册表里面开机自启动的键是否存在及值是否正常
	Dim nRet
	nRet = GetCurrentAppExeRegStatus()
	If (0 = nRet) Then
		Rem: 应用程序存在
		nRet = HWUpdateFileDetect()
		If (0 = nRet) Then
			Rem: 升级程序也存在
			CheckProcessFilesStatus = 0
		Else
			CheckProcessFilesStatus = VALUE_UPDATE_EXE_NOT_EXIT
			g_strCurrentAppExePath = ""'或者使用Empty来进行赋值，但是判断时需要使用IsEmpty()
		End If
	Else
		Rem: 判断是否有app的安装目录及相关程序，如果没有则还需要下载；
		CheckProcessFilesStatus = VALUE_APP_EXE_NOT_EXIT
	End If
End Function

'----获取文件大小
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
	objWS.run "cmd.exe /C Taskkill /f /im " & strProcess, 0'0代表后台执行，1是前台执行，就是是否会显示cmd.exe界面，默认是前台执行
	Set objWS = nothing
End Function

'有的服务器不支持http的文件头部请求或有严格要求
Function GetFileSizeFromLog(strLogFilePath)
	Dim objWS, objExeResult, cmd, strResult, nFileSize
	
	Set objWS = CreateObject("WScript.shell")
	cmd = "cmd.exe /C findstr /I ""Length"" "&strLogFilePath
	Set objExeResult = objWS.exec(cmd)
	strResult = objExeResult.StdOut.ReadAll()
	strResult = replace(strResult, vbNewLine, "")'换行符只有使用vbNewLine才可使，使用'\n'不生效
	
	cmd = "cmd.exe /C For /F ""tokens=2 delims= "" %i in (""" &strResult &""") do @echo %i"
	Set objExeResult = objWS.exec(cmd)
	strResult = objExeResult.StdOut.ReadAll()
	strResult = replace(strResult, ",", "")
	
	If (IsNumeric(strResult)) Then
		nFileSize = Int(strResult)
		HWDebug("从日志获取到的需要下载的文件大小(byte)：" &nFileSize)
	Else
		nFileSize = 0
		HWDebug("从日志文件中获取下载文件大小失败！")
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
	
	'确保安全，先删除下载内容记录文件
	cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_LOG
	objWS.run cmd, 0, True
	
	'-o输出请求日志到文件,--spider不下载；以此取出文件大小
	HWDebug("即将从服务器获取下载文件大小(byte)："&strUrl)
	cmd = "cmd.exe /C " &g_strWgetExePath &" -o " &STRING_PATH_DOWNLOAD_LOG &" --spider " &strUrl
	objWS.run cmd, 0, True
	
	cmd = "cmd.exe /C findstr /I ""Length"" "&STRING_PATH_DOWNLOAD_LOG
	Set objExeResult = objWS.exec(cmd)
	strResult = objExeResult.StdOut.ReadAll()
	strResult = replace(strResult, vbNewLine, "")'换行符只有使用vbNewLine才可使，使用'\n'不生效
	
	cmd = "cmd.exe /C For /F ""tokens=2 delims= "" %i in (""" &strResult &""") do @echo %i"
	Set objExeResult = objWS.exec(cmd)
	strResult = objExeResult.StdOut.ReadAll()
	strResult = replace(strResult, ",", "")
	
	If (IsNumeric(strResult)) Then
		nFileSize = Int(strResult)
		HWDebug("将下载的文件大小(byte)：" &nFileSize)
	Else
		nFileSize = 0
		HWDebug("从服务器获取下载文件大小失败！")
	End If
	
	cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_LOG
	objWS.run cmd, 0, True
	GetFileSizeFromServer = nFileSize
	
	Set objWS = nothing
	Set objExeResult = nothing
End Function

Function InstallHWApplication(strPath)
	Dim objWS
	HWKillProcess("update*")'先结束掉可能存在的安装进程
	HWKillProcess("CureHealth*")
	WScript.sleep(3000)
	Set objWS = CreateObject("WScript.shell")
	HWDebug("安装健康监测仪安装包...")
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
		HWDebug("请求到的下载文件大小异常(byte)："&nFileSize)
		DownloadAndInstall = 1
		strMessage = "修复过程中请求下载的安装文件存在异常，获取到的大小为(byte)："&nFileSize &"(" &strUrl &")"
		HWSendEmail(strMessage)
		Exit Function
	End If
	
	Set objWS = CreateObject("WScript.shell")
		
	If (nFileSize = HWGetFileSize(strSavePath)) Then
		HWDebug("安装文件之前已经完成下载，将直接安装......")
		DownloadAndInstall = InstallHWApplication(strSavePath)
		Set objWS = nothing
		Exit Function
	Else
		If (not HWIsFileExist(strSavePath)) Then
			'删除之前下载的无效文件
			HWDebug("删除所有无效的文件......")
			cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_PREFIX &"CureHealth*"
			objWS.run cmd, 0, True
		End If
		HWDebug("准备下载！")
	End If
	

	cmd = "cmd.exe /C " &g_strWgetExePath &" -q -c -P " &STRING_PATH_DOWNLOAD_PREFIX &" " & strUrl
	objWS.run cmd, 0, True
	
	If (nFileSize = HWGetFileSize(strSavePath)) Then
		HWDebug("安装文件下载成功，开始安装.....")
		nRet = InstallHWApplication(strSavePath)
		strMessage = "安装修复结果为："&nRet &"(" &strUrl &")"
		HWSendEmail(strMessage)
		DownloadAndInstall = nRet
	Else
		HWDebug("安装文件下载失败！")
		strMessage = "安装文件下载失败！总大小为(byte)："&nFileSize &"(" &strUrl &")"
		HWSendEmail(strMessage)
		DownloadAndInstall = 1
	End If
	
	Set objWS = nothing
End Function

Function StartAppExe()
	'先是从注册表中取出当前HumanIndicator所在的路径
	'如果注册表中数据非法，则需要人为去查找，甚至需要下载重新安装
	If (0 = HumanHealthIndicatorFileDetect()) Then
		Dim objWS
		Set objWS = CreateObject("WScript.shell")
		HWDebug("拉起应用程序...")
		g_strCurrentAppExePath = Replace(g_strCurrentAppExePath, "Program Files", "Progra~1")
		objWS.run "cmd.exe /C " &g_strCurrentAppExePath, 0
		Set objWS = nothing
		'这里暂时没有考虑程序运行异常的场景
	Else
		'下载安装包并安装
		DownloadAndInstall()
	End If
End Function

Function StartUpdateExe()
	If (0 = HWUpdateFileDetect()) Then
		Dim objWS
		Set objWS = CreateObject("WScript.shell")
		HWDebug("拉起升级程序...")
		
		Dim strMessage
		strMessage = "检测到升级程序运行异常，即将拉起升级程序..."
		HWSendEmail(strMessage)
		
		g_strCurrentUpdateExePath = Replace(g_strCurrentUpdateExePath, "Program Files", "Progra~1")
		objWS.run "cmd.exe /C " &g_strCurrentUpdateExePath, 0
		Set objWS = nothing
		'这里暂时没有考虑程序运行异常的场景
	Else
		'下载安装包并安装
		DownloadAndInstall()
	End If
End Function

'----检测到所有进程都在运行后的处理
Function HandleProcessAllOK()
	'检查升级程序日志是否还在正常输出，如果日志内容没有增加，则说明升级程序可能死锁，需要重启升级程序
	Dim nTemp, nStatus
	HWDebug("所有程序运行正常，开始检查日志系统...")
	g_nUpdateLogSize = HWGetFileSize(STRING_PATH_LOG_UPDATE)
	HWDebug("当前升级日志大小(byte)："&g_nUpdateLogSize)
	If (g_nUpdateLogSize > 0) Then
		WScript.sleep(VALUE_TIME_LOG_DETECT_AGAIN)'等待40分钟，升级程序一般15分钟一次请求周期即会日志输出
		nStatus = GetProcessStatus()'再次判断，防止开发或运维人员在调试
		if (VALUE_PROCESS_ALL_OK = nStatus) Then
			nTemp = HWGetFileSize(STRING_PATH_LOG_UPDATE)
			HWDebug("当前升级日志大小(byte)："&nTemp)
			if (not (nTemp <> g_nUpdateLogSize)) Then'日志大小未变，这里不能用<=，因为日志可能回滚
				HWDebug("日志系统异常，即将重启升级程序...")
				
				Dim strMessage
				strMessage = "检测到日志系统异常，即将重启升级程序..."
				HWSendEmail(strMessage)
				
				HWKillProcess(STRING_NAME_UPDATE_EXE)'杀死升级进程，后续由应用程序主动拉起
			Else
				HWDebug("日志系统运行正常.")
			End If
		End If
	End If
End Function

'----检测到所有进程都没有运行后的处理
Function HandleProcessAllNOK()
	If (IsProcessExist(STRING_NAME_UPDATE_INSTALL_EXE, False)) Then
		'可能处于升级过程当中
		WScript.sleep(300000)'等待5分钟    'sleep的实现，单位是ms
	Else
		WScript.sleep(60000)'等待60秒
	End If
	
	Dim nStatus
	nStatus = GetProcessStatus()
	If (VALUE_PROCESS_ALL_NOK = nStatus) Then
		'可能是开发或运维人员在对设备进行安装或调试，需要弹出提示
		Dim nRet, objWS, i, strInfo
		Set objWS = CreateObject("WScript.shell")
		'popup的具体用法可以参考百度百科的WScript.shell
		For i = 60 To 0 Step -1
			strInfo = "机器人检测到监测仪程序并未正常运行，机器人将在" &CStr(i)& "秒后自动启动程序，是否允许？"
			nRet = objWS.popup(strInfo, 1, "重要提醒", 4 + 32)
			
			If (7 = nRet) Then'7代表有人按下了取消按钮,这时不自动启动程序
				HWDebug("有开发或运维人员正在维护设备，取消本次启动相关程序操作.")
				Exit Function
			ElseIf (6 = nRet) Then
				Exit For
			End If
		Next
		HWDebug("检测到应用程序与升级程序均未运行，将想办法启动应用程序...")
		
		Dim strMessage
		strMessage = "检测到应用程序和升级程序均未运行，即将进行智能检测与修复..."
		HWSendEmail(strMessage)
		
		StartAppExe()
		WScript.sleep(3000)
		StartUpdateExe()
	Else
		HandleProcessStatus(nStatus)
	End If
End Function

'----检测到应用程序没有运行的处理
Function HandleProcessAppNOK()
	WScript.sleep(VALUE_TIME_APPLICATION_DETECT_AGAIN)'等待3分钟，可能这时正在升级
	Dim nStatus
	nStatus = GetProcessStatus()
    If (VALUE_PROCESS_APP_NOK = nStatus) Then
		HWDebug("检测到应用程序未运行，将想办法启动...")
		
		Dim strMessage
		strMessage = "检测到应用程序未运行，即将进行智能检测与修复..."
		HWSendEmail(strMessage)
		
		StartAppExe()
	End If
End Function

'----检测到升级程序没有运行后的处理
Function HandleProcessUpdateNOK()
	WScript.sleep(VALUE_TIME_UPDATE_DETECT_AGAIN)'sleep 20分钟，可能是刚开机，应用程序还没有将升级程序拉起来
	Dim nRet
	nRet = GetProcessStatus()
	if (VALUE_PROCESS_UPDATE_NOK = nRet) Then
		HWDebug("检测到升级程序没有运行，将想办法启动...")
		
		Dim strMessage
		strMessage = "检测到升级程序未运行，即将进行智能检测与修复..."
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

'----脚本升级检测(通过网络请求升级，仅升级脚本文件)
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
	'md5文件
	cmd = "cmd.exe /C del /F /Q " &strMD5SavePath
	objWS.run cmd, 0, True
	
	'这里是否判断函数执行成功
	'先下载MD5文件，再根据下载日志判断文件是否下载成功
	'删除日志文件
	cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_LOG
	objWS.run cmd, 0, True
	'下载MD5文件并将下载过程输出到日志文件
	cmd = "cmd.exe /C " &g_strWgetExePath &" -o " &STRING_PATH_DOWNLOAD_LOG &" -c -P " &STRING_PATH_DOWNLOAD_PREFIX &" " & strMD5Url
	objWS.run cmd, 0, True
	
	downloadFileSize = GetFileSizeFromLog(STRING_PATH_DOWNLOAD_LOG)
		
	If (not (HWGetFileSize(strMD5SavePath) <> downloadFileSize)) Then'md5文件下载成功与否
		Dim objExeResult
		cmd = "cmd.exe /C findstr ""."" "&strMD5SavePath
		Set objExeResult = objWS.exec(cmd)
		strMD5ReadLine = objExeResult.StdOut.ReadAll()
		strMD5ReadLine = FormatString(strMD5ReadLine)
		
		strVBSUrl = (STRING_URL_VBS_PREFIX &STRING_NAME_HWDETECT &"." &strMD5ReadLine)
		strVBSSavePath = (STRING_PATH_DOWNLOAD_PREFIX &STRING_NAME_HWDETECT &"." &strMD5ReadLine)
		If (MD5Current <> strMD5ReadLine) Then
			HWDebug("当前脚本hash为：" & MD5Current)
			HWDebug("最新脚本hash为：" & strMD5ReadLine)
			If (not HWIsFileExist(strVBSSavePath)) Then
				'删除之前下载的失效文件
				cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_PREFIX &STRING_NAME_HWDETECT &".*"
				objWS.run cmd, 0, True
			End If
			
			'下载VBS脚本文件
			cmd = "cmd.exe /C del /F /Q " &STRING_PATH_DOWNLOAD_LOG
			objWS.run cmd, 0, True
	
			cmd = "cmd.exe /C " &g_strWgetExePath &" -o " &STRING_PATH_DOWNLOAD_LOG &" -c -P " &STRING_PATH_DOWNLOAD_PREFIX &" " & strVBSUrl
			objWS.run cmd, 0, True
				
			downloadFileSize = GetFileSizeFromLog(STRING_PATH_DOWNLOAD_LOG)
			HWDebug("请求到的VBS文件大小为(byte): " &downloadFileSize)
			If (HWIsFileExist(strVBSSavePath)) Then
				Dim MD5Temp
				MD5Temp = GetFileHash(strVBSSavePath)
				If (MD5Temp <> strMD5ReadLine) Then
					objFileSystem.CopyFile strVBSSavePath, STRING_PATH_SYSTEM32 &STRING_NAME_HWDETECT, True
					HWDebug("VBS脚本文件更新成功！")
					g_bQuit = True'脚本自动退出，由计划任务拉起
				Else
					HWDebug("VBS脚本文件下载更新失败！")
				End If
			End If
		Else
			HWDebug("当前脚本是最新版本，不需要下载更新...")
		End If
	Else
		HWDebug("VBS MD5文件下载失败，VBS更新检查失败！")
	End If
	
	Set objWS = nothing
	Set objFileSystem = nothing
End Function

'----脚本主函数
Function MainLoop()
	Set objWS = CreateObject("WScript.shell")
	host = WScript.FullName
	If LCase(right(host, len(host)-InStrRev(host,"\"))) = "wscript.exe" Then'避免通过WScript.exec的时候有窗口弹出
		objWS.run "cscript """ & WScript.ScriptFullName & chr(34), 0
		WScript.Quit
	End If
	
	Dim nProcessStatus, nStartTime
	WScript.sleep(VALUE_TIME_BEFORE_DETECT)'等待3分钟，特别是开机时刻
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
		if (nTempDate - nStartTime > 8640000) Then'可能刚开机的时候获取的系统时间是错的，进行容错处理'86400是3600*24，共1天的时间
			nStartTime = nTempDate
		End If
		
		UpdateDetect()'检测VBS脚本升级
		if ((nTempDate - nStartTime > VALUE_TIME_RESTART_PERIOD) and ((nTempHour >= VALUE_HOUR_MIN) and (nTempHour <= VALUE_HOUR_MAX))) Then
			'重启设备
			Dim objWS
			Set objWS = CreateObject("WScript.shell")
			HWDebug("Too long to restart system, restart now...")
			objWS.run "cmd.exe /C Shutdown /r /f /t 60 /c '为了更好为您服务，机器人将进行一次自我优化与系统重启，请稍候......'", 0
			Set objWS = nothing
		Else
			WScript.sleep(VALUE_TIME_DETECT_PERIOD)'周期性进行检测
		End If

	WEnd
End Function

On Error Resume Next'其后所有(包括函数内)执行出错后均跳过
MainLoop()
