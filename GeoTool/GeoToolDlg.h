
// GeoToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "FileName.h"
#include "Params.h"

// CGeoToolDlg 对话框
class CGeoToolDlg : public CDialogEx
{
// 构造
public:
	CGeoToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GEOTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
