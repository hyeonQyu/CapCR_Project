// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// CapCRView.cpp : CCapCRView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "CapCR.h"
#endif

#include "CapCRDoc.h"
#include "CapCRView.h"
#include "MainFrm.h"
#include "CanvasDlg.h"
#include "OCR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCapCRView

IMPLEMENT_DYNCREATE(CCapCRView, CFormView)

BEGIN_MESSAGE_MAP(CCapCRView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CCapCRView::OnClickedButtonCapture)
	ON_WM_PAINT()
	ON_COMMAND(ID_BUTTON_CAPTURE, &CCapCRView::OnButtonCapture)
	ON_COMMAND(ID_BUTTON_RUNOCR, &CCapCRView::OnButtonRunocr)
	ON_COMMAND(ID_BUTTON_OPENIMAGE, &CCapCRView::OnButtonOpenimage)
	ON_COMMAND(ID_BUTTON_SAVEIMAGE, &CCapCRView::OnButtonSaveimage)
	ON_COMMAND(ID_BUTTON_SAVETEXT, &CCapCRView::OnButtonSavetext)
END_MESSAGE_MAP()

// CCapCRView 생성/소멸

CCapCRView::CCapCRView()
	: CFormView(IDD_CAPCR_FORM)
	, m_strTextbox(_T(""))
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CCapCRView::~CCapCRView()
{
}

void CCapCRView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_INPUT_IMG, m_strInput);
	//  DDX_Text(pDX, IDC_EDIT_OUTPUT_TXT, m_strOutput);
	//  DDX_Text(pDX, IDC_EXPLAIN, m_message);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strTextbox);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editTextbox);
}

BOOL CCapCRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CCapCRView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void CCapCRView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCapCRView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCapCRView 진단

#ifdef _DEBUG
void CCapCRView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCapCRView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCapCRDoc* CCapCRView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCapCRDoc)));
	return (CCapCRDoc*)m_pDocument;
}
#endif //_DEBUG


// CCapCRView 메시지 처리기



void CCapCRView::OnPaint()
{
	CPaintDC dc(this);
	
	if (IsIconic())
	{
		// 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
		if (Image.IsNull() == FALSE)			// 창 변화시 그림 사라지는거 방지
			Image.BitBlt(dc.m_hDC, 0, 0);
	}
	else
	{
		if (Image.IsNull() == FALSE)		// 창 변화시 그림 사라지는거 방지
			Image.BitBlt(dc.m_hDC, 0, 0);
		CFormView::OnPaint();
	}
	if (!openImage.IsNull())
	{
		openImage.BitBlt(dc.m_hDC, 0, 0);
		
	}
	
}


void CCapCRView::OnButtonCapture()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (Image != NULL)
		Image.Destroy();

	int editbox_Height = 150;
	CString imgName = _T("Desktop.jpg");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 현재 창 투명하게
	::SetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE, ::GetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	pFrame->SetLayeredWindowAttributes(0, 0, LWA_ALPHA);


	// Canvas 다이얼로그 호출
	CanvasDlg canvas;
	canvas.DoModal();




	area = canvas.GetClipRect();
	int nClipWidth = area.right - area.left;
	int nClipHeight = area.bottom - area.top;

	CWnd* pWndDesktop = GetDesktopWindow();
	// 바탕 화면 윈도우 DC 

	CWindowDC ScrDC(pWndDesktop);
	// 뷰 윈도우 DC
	CClientDC dc(this);
	CRect Rect;
	GetClientRect(&Rect);
	CWnd::GetWindowRect(&Rect);


	int sx = area.left;
	int sy = area.top;
	int cx = nClipWidth;
	int cy = nClipHeight;

	SIZE s;
	ZeroMemory(&s, sizeof(SIZE));
	s.cx = (LONG)::GetSystemMetrics(SM_CXFULLSCREEN);
	s.cy = (LONG)::GetSystemMetrics(SM_CYFULLSCREEN);


	if ((s.cy * 0.5 < cy)) {
		cx = cx * 0.5;
		cy = cy * 0.5;
	}


	Image.Create(cx, cy, ScrDC.GetDeviceCaps(BITSPIXEL));

	// 이미지 DC에 현재 작업 영역의 절대 좌표를 사용해 그 크기만큼 저장하게 한다. 

	CDC* pDC = CDC::FromHandle(Image.GetDC());
	pDC->BitBlt(0, 0, cx, cy, &ScrDC, sx, sy, SRCCOPY);
	Image.ReleaseDC();

	// 저장된 이미지를 원하는 파일명, 포멧타입을 지정해서 저장한다. 

	//Image.Save(imgName, Gdiplus::ImageFormatJPEG);


	//CStatic *staticSize = (CStatic*)GetDlgItem(AFX_IDC_PICTURE);
	// CRect rect2;

	// staticSize->GetClientRect(rect2);
	//Image.Load(imgName);


	Image.Save(imgName, Gdiplus::ImageFormatJPEG);

	if (cx > 520 && cy > 300)
		pFrame->SetWindowPos(NULL, (s.cx / 2) - (cx + 50) / 2, 0, cx + 50, cy + 200 + editbox_Height, SWP_NOREPOSITION);
	else if (cx < 520 && cy < 300)
		pFrame->SetWindowPos(NULL, s.cx / 2 - 285, 0, 570, 500+ editbox_Height, SWP_NOREPOSITION);
	else if (cx < 520)
		pFrame->SetWindowPos(NULL, (s.cx / 2) - 285, 0, 570, cy + 200+ editbox_Height, SWP_NOREPOSITION);
	else
		pFrame->SetWindowPos(NULL, s.cx / 2 - ((cx + 50) / 2), 0, cx + 50, 500+ editbox_Height, SWP_NOREPOSITION);


	// 바뀌는 다이얼로그에 따른 에디트 컨트롤 크기 및 위치조정
	if(cx < 520)
		m_editTextbox.MoveWindow(0, cy + 10, 520, editbox_Height);
	else m_editTextbox.MoveWindow(0, cy +10, cx, editbox_Height);


	// 창 투명화 해제
	pFrame->SetLayeredWindowAttributes(0, 255, LWA_ALPHA);

	// 이미지 뷰에 붙이기
	Image.BitBlt(dc.m_hDC, 0, 0);



	// 그 파일을 실행해 준다. 

	/*
	::ShellExecute(NULL,
	TEXT("open"), TEXT("Desktop.jpg"),
	NULL, NULL, SW_SHOW);
	*/
}


void CCapCRView::OnButtonRunocr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COCR *ocr = new COCR();
	ocr->RunOCR(&Image, "ConvertedText.txt", 50);
	UpdateData(FALSE);
}


void CCapCRView::OnButtonOpenimage()
{
	char szFilter[] = " All Files(*.*)|*.*|";
	sFilename = "";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (CString)szFilter, NULL);
	int cx, cy;
	int editbox_Height = 150;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	SIZE s;
	ZeroMemory(&s, sizeof(SIZE));
	s.cx = (LONG)::GetSystemMetrics(SM_CXFULLSCREEN);
	s.cy = (LONG)::GetSystemMetrics(SM_CYFULLSCREEN);


	if (IDOK == dlg.DoModal())
	{
		sFilename = dlg.GetPathName();
		if (!openImage.IsNull())
			openImage.Destroy();


		openImage.Load(sFilename);
		
		cx = openImage.GetWidth();
		cy = openImage.GetHeight();
		
		if (cx> 520 &&cy> 300)
			pFrame->SetWindowPos(NULL, (s.cx / 2) - (cx + 50) / 2, 0, cx + 50, cy + 200 + editbox_Height, SWP_NOREPOSITION);
		else if (cx < 520 && cy < 300)
			pFrame->SetWindowPos(NULL, s.cx / 2 - 285, 0, 570, 500 + editbox_Height, SWP_NOREPOSITION);
		else if (cx < 520)
			pFrame->SetWindowPos(NULL, (s.cx / 2) - 285, 0, 570, cy + 200 + editbox_Height, SWP_NOREPOSITION);
		else
			pFrame->SetWindowPos(NULL, s.cx / 2 - ((cx + 50) / 2), 0, cx + 50, 500 + editbox_Height, SWP_NOREPOSITION);

		if (cx < 520)
			m_editTextbox.MoveWindow(0, cy + 10, 520, editbox_Height);
		else m_editTextbox.MoveWindow(0, cy + 10, cx, editbox_Height);

		

		Invalidate();
		UpdateWindow();
	}

}


void CCapCRView::OnButtonSaveimage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
		

	



}


void CCapCRView::OnButtonSavetext()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	printf("[OnBnClickedBtnSave] \n");
	CString m_strPath;
	CStdioFile file;
	// CFile file;
	CFileException ex;
	CFileDialog dlg(FALSE, _T("*.txt"), NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, _T("TXT Files(*.txt)|*.txt|"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_strPath = dlg.GetPathName();
		if (m_strPath.Right(4) != ".txt")
		{
			m_strPath += ".txt";
		}
		file.Open(m_strPath, CFile::modeCreate | CFile::modeReadWrite, &ex);
		// 에디트 박스에 있는 것을 저장한다. 
		UpdateData(TRUE);
		file.WriteString(m_strTextbox);
		// 종료한다. 
		file.Close();
	}

}
