
// ZGLyricDecoderDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ZGLyricDecoder.h"
#include "ZGLyricDecoderDlg.h"
#include "afxdialogex.h"
#include "LyricDecoder.h"
#include <CommCtrl.h>
#include "aerosubc.h"
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"AeroGlassGDI.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CZGLyricDecoderDlg 对话框



CZGLyricDecoderDlg::CZGLyricDecoderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZGLYRICDECODER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZGLyricDecoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZGLyricDecoderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CZGLyricDecoderDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CZGLyricDecoderDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &CZGLyricDecoderDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON2, &CZGLyricDecoderDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CZGLyricDecoderDlg::OnBnClickedButton3)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

bool qrc_ok = true;

// CZGLyricDecoderDlg 消息处理程序

BOOL CZGLyricDecoderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	HMODULE hDLL = LoadLibrary(L"QQMusicCommon.dll");
	if (!hDLL) {
		TaskDialog(m_hWnd, NULL,
			L"提示",
			L"没有正确版本的QQMusicCommon.dll文件,将只能转换酷狗krc歌词!",
			NULL,
			TDCBF_OK_BUTTON,
			TD_WARNING_ICON,
			NULL);
		qrc_ok = false;
	}
	else FreeLibrary(hDLL);
	AllAero(m_hWnd);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CZGLyricDecoderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CZGLyricDecoderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CZGLyricDecoderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString mypath=L"",savepath=L"";

void CZGLyricDecoderDlg::OnEnChangeEdit1()
{
	GetDlgItem(IDC_EDIT1)->GetWindowText(mypath);
}

CString GetWorkDir()
{
	wchar_t pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
	CString csFullPath(pFileName);
	if (nPos < 0)
		return CString("");
	else
		return csFullPath;
}

void CZGLyricDecoderDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filter = L"加密的歌词文件 (*.krc; *.qrc)|*.krc;*.qrc|所有文件 (*.*)|*.*||";	//文件过虑的类型
	CFileDialog openFileDlg(TRUE, GetWorkDir(), NULL, OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK) {
		mypath = openFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT1)->SetWindowText(mypath);
	}
	if (!qrc_ok&&mypath.Find(L".qrc")!=-1) {
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	}
}

void CZGLyricDecoderDlg::OnEnChangeEdit2()
{
	GetDlgItem(IDC_EDIT2)->GetWindowText(savepath);
}

void CZGLyricDecoderDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filter = L"歌词文件 (*.lrc)|*.lrc|所有文件 (*.*)|*.*||";	//文件过虑的类型
	CFileDialog openFileDlg(FALSE, GetWorkDir(), NULL, OFN_READONLY|OFN_OVERWRITEPROMPT, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK) {
		savepath = openFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT2)->SetWindowText(savepath);
	}
}

void CZGLyricDecoderDlg::OnBnClickedButton3()
{
	HANDLE pFile = CreateFile(mypath, GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,        //打开已存在的文件 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	HANDLE pSaveFile = CreateFile(savepath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	bool failure = false;
	if (pFile == INVALID_HANDLE_VALUE)
	{
		failure = true;
		CloseHandle(pFile);
	}
	if (pSaveFile == INVALID_HANDLE_VALUE)
	{
		failure = true;
		CloseHandle(pSaveFile);
	}
	// TODO: 在此添加控件通知处理程序代码
	if (mypath.IsEmpty() || savepath.IsEmpty() || failure) {
		TaskDialog(m_hWnd, NULL,
			L"错误",
			L"文件名必须有效",
			NULL,
			TDCBF_OK_BUTTON,
			TD_ERROR_ICON,
			NULL);
	}
	else {
		DWORD filesize = GetFileSize(pFile, NULL);
		char* buffer = new char[filesize + 1]; // 最后一位为 '/0',C-Style 字符串的结束符
		DWORD readsize;
		ReadFile(pFile, buffer, filesize, &readsize, NULL);
		buffer[filesize] = 0;
		if (mypath.Find(L".qrc") != -1) {
			FileNode decoded = qrcdecode(buffer,readsize);
			if (!decoded.success) {
				TaskDialog(m_hWnd, NULL,
					L"错误",
					L"转换失败",
					NULL,
					TDCBF_OK_BUTTON,
					TD_ERROR_ICON,
					NULL);
			}
			else {
				SetFilePointer(pSaveFile, 0, 0, FILE_BEGIN);
				DWORD dwWritten;     //保存写了多少字节到文件中
				if (!WriteFile(pSaveFile, decoded.data, decoded.len, &dwWritten, 0)) {
					TaskDialog(m_hWnd, NULL,
						L"错误",
						L"写入失败",
						NULL,
						TDCBF_OK_BUTTON,
						TD_ERROR_ICON,
						NULL);
				}
				else {
					TaskDialog(m_hWnd, NULL,
						L"提示",
						L"转换成功",
						NULL,
						TDCBF_OK_BUTTON,
						TD_INFORMATION_ICON,
						NULL);
				}
			}
		}
		if (mypath.Find(L".krc") != -1) {
			FileNode decoded = krcdecode(buffer, readsize);
			if (!decoded.success) {
				TaskDialog(m_hWnd, NULL,
					L"错误",
					L"转换失败",
					NULL,
					TDCBF_OK_BUTTON,
					TD_ERROR_ICON,
					NULL);
			}
			else {
				SetFilePointer(pSaveFile, 0, 0, FILE_BEGIN);
				DWORD dwWritten;     //保存写了多少字节到文件中
				if (!WriteFile(pSaveFile, decoded.data, decoded.len, &dwWritten, 0)) {
					TaskDialog(m_hWnd, NULL,
						L"错误",
						L"写入失败",
						NULL,
						TDCBF_OK_BUTTON,
						TD_ERROR_ICON,
						NULL);
				}
				else {
					TaskDialog(m_hWnd, NULL,
						L"提示",
						L"转换成功",
						NULL,
						TDCBF_OK_BUTTON,
						TD_INFORMATION_ICON,
						NULL);
				}
			}
		}
		delete[] buffer; // 注意是delete[] 而不是 delete
	}
	CloseHandle(pFile);
	CloseHandle(pSaveFile);
}


void CZGLyricDecoderDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT count;
	wchar_t filePath[1024];
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (count==1) {
		//0是文件索引号
		int pathLen = DragQueryFile(hDropInfo, 0, filePath, sizeof(filePath));
		mypath = CString(filePath);
		GetDlgItem(IDC_EDIT1)->SetWindowText(mypath);
	}
	else {
		TaskDialog(m_hWnd, NULL,
			L"错误",
			L"一次只能处理一个文件!",
			NULL,
			TDCBF_OK_BUTTON,
			TD_ERROR_ICON,
			NULL);
	}

	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}
