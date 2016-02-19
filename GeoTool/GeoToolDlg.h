
// GeoToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "FileName.h"
#include "Params.h"

// CGeoToolDlg �Ի���
class CGeoToolDlg : public CDialogEx
{
// ����
public:
	CGeoToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GEOTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOpenButton();
	afx_msg void OnBnClickedSaveButton();

	CComboBox m_satellite_combo;
	afx_msg void OnCbnSelchangeAbort();
	CComboBox m_sensor_combo;
	afx_msg void OnBnClickedInputButton();
	afx_msg void OnCbnSelchangeSensorCombo();

	CString SatelliteName;
	CString SensorName;
	CString strPointFilePath;  
    CString strLinetimeFilePath;
	CString strMetaFilePath;
	CString strResultFilePath;
	afx_msg void OnBnClickedInstallButton();
	afx_msg void OnBnClickedLinetimeButton();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedDistortionButton();
	afx_msg void OnBnClickedResultInsatallButton();
	afx_msg void OnBnClickedResultDistortionButton();
	afx_msg void OnBnClickedImageButton();

	FileName filename;
	Params params;

};
