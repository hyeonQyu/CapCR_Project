// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// CapCRView.h : CCapCRView Ŭ������ �������̽�
//

#pragma once
#include "atlimage.h"


class CCapCRView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CCapCRView();
	DECLARE_DYNCREATE(CCapCRView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_CAPCR_FORM };
#endif

// Ư���Դϴ�.
public:
	CCapCRDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	CImage Image;
	HICON m_hIcon;
	RECT area;
	virtual ~CCapCRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnClickedButtonCapture();
	afx_msg void OnPaint();
	afx_msg void OnButtonCapture();
};

#ifndef _DEBUG  // CapCRView.cpp�� ����� ����
inline CCapCRDoc* CCapCRView::GetDocument() const
   { return reinterpret_cast<CCapCRDoc*>(m_pDocument); }
#endif
