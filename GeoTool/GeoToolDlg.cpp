
// GeoToolDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CGeoToolDlg �Ի���



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


// CGeoToolDlg ��Ϣ�������

BOOL CGeoToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_satellite_combo.AddString(_T("HJ1A"));
	m_satellite_combo.AddString(_T("ZY3A"));
	m_satellite_combo.AddString(_T("GF01"));
	m_satellite_combo.AddString(_T("GF02"));


	// Ĭ��ѡ���һ��   
//	m_satellite_combo.SetCurSel(0);   

	m_sensor_combo.AddString(_T("CCD1"));
	m_sensor_combo.AddString(_T("CCD2"));

//	m_sensor_combo.SetCurSel(0);   

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGeoToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGeoToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGeoToolDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CGeoToolDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CGeoToolDlg::OnCbnSelchangeAbort()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���ù�����   
	TCHAR szFilter[] = _T("�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");   
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);   

	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())   
	{   
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		strPointFilePath = fileDlg.GetPathName();   
		SetDlgItemText(IDC_INPUT_EDIT, strPointFilePath);   
	}   
}



void CGeoToolDlg::OnCbnSelchangeSensorCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sensor = m_sensor_combo.GetCurSel();
//	CString sen;
	m_sensor_combo.GetLBText(sensor,SensorName);
	//SensorName = (LPSTR)(LPCTSTR)sen;
	//SensorName = sen.GetBuffer(0);  
	//sen.ReleaseBuffer();
}

void CGeoToolDlg::OnBnClickedLinetimeButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���ù�����   
	TCHAR szFilter[] = _T("��ʱ�ļ�(*.dat)|*.dat|�����ļ�(*.*)|*.*||");   
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("dat"), NULL, 0, szFilter, this);   
 
	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())   
	{   
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		strLinetimeFilePath = fileDlg.GetPathName();   
		SetDlgItemText(IDC_LINETIME_EDIT, strLinetimeFilePath);   
	}   
}


void CGeoToolDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("�����ļ�(*.xml)|*.xml|�����ļ�(*.*)|*.*||");   
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("xml"), NULL, 0, szFilter, this);   

	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())   
	{   
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		strMetaFilePath = fileDlg.GetPathName();   
		SetDlgItemText(IDC_META_EDIT, strMetaFilePath);   
	}   
}

void CGeoToolDlg::OnBnClickedInstallButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (SatelliteName.IsEmpty())
	{
		MessageBox("��ѡ��������", "��ʾ", MB_OK);
		return;   
	}

	if (SensorName.IsEmpty())
	{
		MessageBox("��ѡ�񴫸�����", "��ʾ", MB_OK);
		return;   
	}

	if (strPointFilePath.IsEmpty())
	{
		MessageBox("��������Ƶ��ļ�·��", "��ʾ", MB_OK);
		return;   
	}

	if (strLinetimeFilePath.IsEmpty())
	{
		MessageBox("��������ʱ�ļ�·��", "��ʾ", MB_OK);
		return;   
	}
	if (strMetaFilePath.IsEmpty())
	{
		MessageBox("����������ļ�·��", "��ʾ", MB_OK);
		return;   
	}
	
	FileName filename;
	filename.linetimeName = strLinetimeFilePath.GetBuffer(0);
	filename.metaName = strMetaFilePath.GetBuffer(0);
	filename.pointName = strPointFilePath.GetBuffer(0);

	Install((LPSTR)(LPCTSTR)SatelliteName,(LPSTR)(LPCTSTR)SensorName,filename,params);
	CString str = "��װ�������\n\n";
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
	MessageBox(str, "��װ����", MB_OK);
	return;

}


void CGeoToolDlg::OnBnClickedDistortionButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (SatelliteName.IsEmpty())
	{
		MessageBox("��ѡ��������", "��ʾ", MB_OK);
		return;   
	}

	if (SensorName.IsEmpty())
	{
		MessageBox("��ѡ�񴫸�����", "��ʾ", MB_OK);
		return;   
	}

	if (strPointFilePath.IsEmpty())
	{
		MessageBox("��������Ƶ��ļ�·��", "��ʾ", MB_OK);
		return;   
	}

	if (strLinetimeFilePath.IsEmpty())
	{
		MessageBox("��������ʱ�ļ�·��", "��ʾ", MB_OK);
		return;   
	}
	if (strMetaFilePath.IsEmpty())
	{
		MessageBox("����������ļ�·��", "��ʾ", MB_OK);
		return;   
	}



	filename.linetimeName = strLinetimeFilePath.GetBuffer(0);
	filename.metaName = strMetaFilePath.GetBuffer(0);
	filename.pointName = strPointFilePath.GetBuffer(0);

	Distortion((LPSTR)(LPCTSTR)SatelliteName,(LPSTR)(LPCTSTR)SensorName,filename,params);
	CString str = "��Ԫ�������\n\n";
	str += "������䣺\n";
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
	str += "������䣺\n";
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
	MessageBox(str, "��װ����", MB_OK);
	return;
}


void CGeoToolDlg::OnBnClickedResultInsatallButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str = "��װ����:\n";
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
	MessageBox(str, "��װ����", MB_OK);
	return;
}


void CGeoToolDlg::OnBnClickedResultDistortionButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str = "������䣺\n";
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
	str += "������䣺\n";
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
	MessageBox(str, "��װ����", MB_OK);
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
