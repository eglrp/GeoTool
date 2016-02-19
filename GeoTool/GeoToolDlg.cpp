
// GeoToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GeoTool.h"
#include "GeoToolDlg.h"
#include "afxdialogex.h"
#include "CORRECT.h"
//#include "CorrectFactory.h"
//#include "AbstractCorrect.h"
#include "plot.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGeoToolDlg 对话框



CGeoToolDlg::CGeoToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGeoToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGeoToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDABORT, m_satellite_combo);
	DDX_Control(pDX, IDC_SENSOR_COMBO, m_sensor_combo);
}

BEGIN_MESSAGE_MAP(CGeoToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGeoToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGeoToolDlg::OnBnClickedCancel)

	ON_CBN_SELCHANGE(IDABORT, &CGeoToolDlg::OnCbnSelchangeAbort)
	ON_BN_CLICKED(IDC_INPUT_BUTTON, &CGeoToolDlg::OnBnClickedInputButton)
	ON_CBN_SELCHANGE(IDC_SENSOR_COMBO, &CGeoToolDlg::OnCbnSelchangeSensorCombo)
	ON_BN_CLICKED(IDC_INSTALL_BUTTON, &CGeoToolDlg::OnBnClickedInstallButton)
	ON_BN_CLICKED(IDC_LINETIME_BUTTON, &CGeoToolDlg::OnBnClickedLinetimeButton)
	ON_BN_CLICKED(IDC_BUTTON8, &CGeoToolDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_DISTORTION_BUTTON, &CGeoToolDlg::OnBnClickedDistortionButton)
	ON_BN_CLICKED(IDC_RESULT_INSATALL_BUTTON, &CGeoToolDlg::OnBnClickedResultInsatallButton)
	ON_BN_CLICKED(IDC_RESULT_DISTORTION_BUTTON, &CGeoToolDlg::OnBnClickedResultDistortionButton)
	ON_BN_CLICKED(IDC_IMAGE_BUTTON, &CGeoToolDlg::OnBnClickedImageButton)
END_MESSAGE_MAP()


// CGeoToolDlg 消息处理程序

BOOL CGeoToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_satellite_combo.AddString(_T("HJ1A"));
	m_satellite_combo.AddString(_T("ZY3A"));
	m_satellite_combo.AddString(_T("GF01"));
	m_satellite_combo.AddString(_T("GF02"));


	// 默认选择第一项   
//	m_satellite_combo.SetCurSel(0);   

	m_sensor_combo.AddString(_T("CCD1"));
	m_sensor_combo.AddString(_T("CCD2"));

//	m_sensor_combo.SetCurSel(0);   

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGeoToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGeoToolDlg::OnPaint()
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
HCURSOR CGeoToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGeoToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CGeoToolDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CGeoToolDlg::OnCbnSelchangeAbort()
{
	// TODO: 在此添加控件通知处理程序代码
	int satellite = m_satellite_combo.GetCurSel();
//	CString sat;
	m_satellite_combo.GetLBText(satellite,SatelliteName);
//	SatelliteName = (LPSTR)(LPCTSTR)sat;
	
	if (satellite == 0)
	{
		m_sensor_combo.ResetContent();
		m_sensor_combo.AddString(_T("CCD1"));
		m_sensor_combo.AddString(_T("CCD2"));
//		m_sensor_combo.SetCurSel(0);   
	}
	if (satellite == 1)
	{
		m_sensor_combo.ResetContent();
		m_sensor_combo.AddString(_T("FWD"));
		m_sensor_combo.AddString(_T("BWD"));
		m_sensor_combo.AddString(_T("NAD"));
		m_sensor_combo.AddString(_T("MUX"));
//		m_sensor_combo.SetCurSel(0);   
	}
	if (satellite == 2)
	{
		m_sensor_combo.ResetContent();
		m_sensor_combo.AddString(_T("PAN"));
		m_sensor_combo.AddString(_T("MS"));
		m_sensor_combo.AddString(_T("WF"));
//		m_sensor_combo.SetCurSel(0);   
	}
	if (satellite == 3)
	{
		m_sensor_combo.ResetContent();
		m_sensor_combo.AddString(_T("PAN"));
		m_sensor_combo.AddString(_T("MS"));
//		m_sensor_combo.SetCurSel(0);   
	}
}


void CGeoToolDlg::OnBnClickedInputButton()
{
	// TODO: 在此添加控件通知处理程序代码
	// 设置过滤器   
	TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");   
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);   

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())   
	{   
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strPointFilePath = fileDlg.GetPathName();   
		SetDlgItemText(IDC_INPUT_EDIT, strPointFilePath);   
	}   
}



void CGeoToolDlg::OnCbnSelchangeSensorCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	int sensor = m_sensor_combo.GetCurSel();
//	CString sen;
	m_sensor_combo.GetLBText(sensor,SensorName);
	//SensorName = (LPSTR)(LPCTSTR)sen;
	//SensorName = sen.GetBuffer(0);  
	//sen.ReleaseBuffer();
}

void CGeoToolDlg::OnBnClickedLinetimeButton()
{
	// TODO: 在此添加控件通知处理程序代码
	// 设置过滤器   
	TCHAR szFilter[] = _T("行时文件(*.dat)|*.dat|所有文件(*.*)|*.*||");   
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("dat"), NULL, 0, szFilter, this);   
 
	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())   
	{   
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strLinetimeFilePath = fileDlg.GetPathName();   
		SetDlgItemText(IDC_LINETIME_EDIT, strLinetimeFilePath);   
	}   
}


void CGeoToolDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("参数文件(*.xml)|*.xml|所有文件(*.*)|*.*||");   
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("xml"), NULL, 0, szFilter, this);   

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())   
	{   
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strMetaFilePath = fileDlg.GetPathName();   
		SetDlgItemText(IDC_META_EDIT, strMetaFilePath);   
	}   
}

void CGeoToolDlg::OnBnClickedInstallButton()
{
	// TODO: 在此添加控件通知处理程序代码

	if (SatelliteName.IsEmpty())
	{
		MessageBox("请选择卫星名", "提示", MB_OK);
		return;   
	}

	if (SensorName.IsEmpty())
	{
		MessageBox("请选择传感器名", "提示", MB_OK);
		return;   
	}

	if (strPointFilePath.IsEmpty())
	{
		MessageBox("请输入控制点文件路径", "提示", MB_OK);
		return;   
	}

	if (strLinetimeFilePath.IsEmpty())
	{
		MessageBox("请输入行时文件路径", "提示", MB_OK);
		return;   
	}
	if (strMetaFilePath.IsEmpty())
	{
		MessageBox("请输入参数文件路径", "提示", MB_OK);
		return;   
	}
	
	FileName filename;
	filename.linetimeName = strLinetimeFilePath.GetBuffer(0);
	filename.metaName = strMetaFilePath.GetBuffer(0);
	filename.pointName = strPointFilePath.GetBuffer(0);

	Install((LPSTR)(LPCTSTR)SatelliteName,(LPSTR)(LPCTSTR)SensorName,filename,params);
	CString str = "安装矩阵完成\n\n";
	str += "yaw = ";
	CString Temp;
	Temp.Format("%lf",params.yaw);
	str += Temp +"\n";
	str += "pitch = ";
	Temp.Format("%lf",params.pitch);
	str += Temp +"\n";
	str += "roll = ";
	Temp.Format("%lf",params.roll);
	str += Temp +"\n";
	MessageBox(str, "安装矩阵", MB_OK);
	return;

}


void CGeoToolDlg::OnBnClickedDistortionButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (SatelliteName.IsEmpty())
	{
		MessageBox("请选择卫星名", "提示", MB_OK);
		return;   
	}

	if (SensorName.IsEmpty())
	{
		MessageBox("请选择传感器名", "提示", MB_OK);
		return;   
	}

	if (strPointFilePath.IsEmpty())
	{
		MessageBox("请输入控制点文件路径", "提示", MB_OK);
		return;   
	}

	if (strLinetimeFilePath.IsEmpty())
	{
		MessageBox("请输入行时文件路径", "提示", MB_OK);
		return;   
	}
	if (strMetaFilePath.IsEmpty())
	{
		MessageBox("请输入参数文件路径", "提示", MB_OK);
		return;   
	}



	filename.linetimeName = strLinetimeFilePath.GetBuffer(0);
	filename.metaName = strMetaFilePath.GetBuffer(0);
	filename.pointName = strPointFilePath.GetBuffer(0);

	Distortion((LPSTR)(LPCTSTR)SatelliteName,(LPSTR)(LPCTSTR)SensorName,filename,params);
	CString str = "像元畸变完成\n\n";
	str += "径向畸变：\n";
	str += "c0 = ";
	CString Temp;
	Temp.Format("%.12lf",params.c0);
	str += Temp +"\n";
	str += "c1 = ";
	Temp.Format("%.12lf",params.c1);
	str += Temp +"\n";
	str += "c2 = ";
	Temp.Format("%.12lf",params.c2);
	str += Temp +"\n";
	str += "c3 = ";
	Temp.Format("%.12lf",params.c3);
	str += Temp +"\n";
	str += "切向畸变：\n";
	str += "d0 = ";
	Temp.Format("%.12lf",params.d0);
	str += Temp +"\n";
	str += "d1 = ";
	Temp.Format("%.12lf",params.d1);
	str += Temp +"\n";
	str += "d2 = ";
	Temp.Format("%.12lf",params.d2);
	str += Temp +"\n";
	str += "d3 = ";
	Temp.Format("%.12lf",params.d3);
	str += Temp +"\n";
	MessageBox(str, "安装矩阵", MB_OK);
	return;
}


void CGeoToolDlg::OnBnClickedResultInsatallButton()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str = "安装矩阵:\n";
	str += "yaw = ";
	CString Temp;
	Temp.Format("%lf",params.yaw);
	str += Temp +"\n";
	str += "pitch = ";
	Temp.Format("%lf",params.pitch);
	str += Temp +"\n";
	str += "roll = ";
	Temp.Format("%lf",params.roll);
	str += Temp +"\n";
	MessageBox(str, "安装矩阵", MB_OK);
	return;
}


void CGeoToolDlg::OnBnClickedResultDistortionButton()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str = "径向畸变：\n";
	str += "c0 = ";
	CString Temp;
	Temp.Format("%.12lf",params.c0);
	str += Temp +"\n";
	str += "c1 = ";
	Temp.Format("%.12lf",params.c1);
	str += Temp +"\n";
	str += "c2 = ";
	Temp.Format("%.12lf",params.c2);
	str += Temp +"\n";
	str += "c3 = ";
	Temp.Format("%.12lf",params.c3);
	str += Temp +"\n";
	str += "切向畸变：\n";
	str += "d0 = ";
	Temp.Format("%.12lf",params.d0);
	str += Temp +"\n";
	str += "d1 = ";
	Temp.Format("%.12lf",params.d1);
	str += Temp +"\n";
	str += "d2 = ";
	Temp.Format("%.12lf",params.d2);
	str += Temp +"\n";
	str += "d3 = ";
	Temp.Format("%.12lf",params.d3);
	str += Temp +"\n";
	MessageBox(str, "安装矩阵", MB_OK);
	return;
}


void CGeoToolDlg::OnBnClickedImageButton()
{
	
	{
		double xishu[4];
		xishu[0] = params.c0;
		xishu[1] = params.c1;
		xishu[2] = params.c2;
		xishu[3] = params.c3;
		OCplot(params.ImgSize,xishu,params.xsample,params.ysample,params.size,"XXXX");
		xishu[0] = params.d0;
		xishu[1] = params.d1;
		xishu[2] = params.d2;
		xishu[3] = params.d3;
		OCplot(params.ImgSize,xishu,params.xsample,params.zsample,params.size,"YYYY");
	}
	
}
