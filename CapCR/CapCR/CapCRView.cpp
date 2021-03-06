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
#include "ProgressDlg.h"
#include "TranslateDlg.h"
#include "SearchDlg.h"

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
	ON_COMMAND(ID_BUTTON_SAVETEXT, &CCapCRView::OnButtonSavetext)
	ON_COMMAND(ID_CHECK_IGNORE_FIX, &CCapCRView::OnCheckIgnoreFix)
	ON_COMMAND(ID_CHECK_IGNORE_SPACE, &CCapCRView::OnCheckIgnoreSpace)
	ON_UPDATE_COMMAND_UI(ID_CHECK_IGNORE_FIX, &CCapCRView::OnUpdateCheckIgnoreFix)
	ON_UPDATE_COMMAND_UI(ID_CHECK_IGNORE_SPACE, &CCapCRView::OnUpdateCheckIgnoreSpace)
	ON_COMMAND(ID_BUTTON_TRANSLATE, &CCapCRView::OnButtonTranslate)
	ON_COMMAND(ID_BUTTON_SEARCH, &CCapCRView::OnButtonSearch)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SAVETEXT, &CCapCRView::OnUpdateButtonSavetext)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SEARCH, &CCapCRView::OnUpdateButtonSearch)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TRANSLATE, &CCapCRView::OnUpdateButtonTranslate)
END_MESSAGE_MAP()

// CCapCRView 생성/소멸

CCapCRView::CCapCRView()
	: CFormView(IDD_CAPCR_FORM)
	, ocr(NULL)
	, m_bOcrEmpty(true)
	, m_bIgnoreFix(false)
	, m_bIgnoreSpace(false)
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
}

BOOL CCapCRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	return CFormView::PreCreateWindow(cs);
}

void CCapCRView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SetWindowPos(NULL, 0, 0, 560, 500, SWP_NOMOVE);
	pFrame->m_bCreated = true;
}

void CCapCRView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCapCRView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
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
	if (openImage != NULL)
		openImage.Destroy();

	CString imgName = _T("Desktop.jpg");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCapCRView* pView = (CCapCRView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 현재 창 투명하게
	::SetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE, ::GetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	pFrame->SetLayeredWindowAttributes(0, 0, LWA_ALPHA);

	// 텍스트박스 대화상자도 없앰(ocr객체 삭제)
	if (!m_bOcrEmpty)
	{
		if (!ocr->m_bTextboxEmpty)
			ocr->DestroyTextDialog();
		delete(ocr);
		m_bOcrEmpty = true;
	}


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


	//Image.Save(imgName, Gdiplus::ImageFormatJPEG);
	if (cx > 560 && cy > 300)
		pFrame->SetWindowPos(NULL, (s.cx / 2) - (cx + 50) / 2, 0, cx + 30, cy + 170, SWP_NOREPOSITION);
	else if (cx < 560 && cy < 300) {
		pFrame->SetWindowPos(NULL, s.cx / 2 - 285, 0, 560, 500, SWP_NOREPOSITION);
		Invalidate();
	}
	else if (cx < 560)
		pFrame->SetWindowPos(NULL, (s.cx / 2) - 285, 0, 560, cy + 170, SWP_NOREPOSITION);
	else
		pFrame->SetWindowPos(NULL, s.cx / 2 - ((cx + 50) / 2), 0, cx + 30, 500, SWP_NOREPOSITION);



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
	UpdateData(TRUE);
	if (Image == NULL)
	{
		AfxMessageBox(_T("변환할 이미지가 없습니다."));
		return;
	}

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	CProgressDlg *pDlg = new CProgressDlg;
	pDlg->Create(IDD_PROGRESS, this);
	pDlg->CenterWindow(this);
	pDlg->ShowWindow(SW_SHOW);
	
	if (m_bOcrEmpty == FALSE)
	{
		ocr->DestroyTextDialog();
		delete(ocr);
		m_bOcrEmpty = true;
	}
	ocr = new COCR();
	ocr->RunOCR(&Image, "ConvertedText.txt", pDlg);
	m_bOcrEmpty = FALSE;
	pDlg->ShowWindow(SW_HIDE);
	pDlg->DestroyWindow();
	UpdateData(FALSE);

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	Invalidate();
}


void CCapCRView::OnButtonOpenimage()
{
	char szFilter[] = " All Files(*.*)|*.*|";
	sFilename = "";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (CString)szFilter, NULL);
	
	int cx, cy;
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
		if (!Image.IsNull())
			Image.Destroy();


		openImage.Load(sFilename);
		
		HRESULT hResult = Image.Load(sFilename);



		// 오류처리

		if (FAILED(hResult))
		{
			CString strtmp = _T("ERROR:Failed to load");
			strtmp += sFilename + _T("\n");
			TRACE(strtmp);
			return;
		}

	

		cx = openImage.GetWidth();
		cy = openImage.GetHeight();
		
		

		if (cx> 560 &&cy> 300)
			pFrame->SetWindowPos(NULL, (s.cx / 2) - (cx + 50) / 2, 0, cx + 50, cy + 200, SWP_NOREPOSITION);
		else if (cx < 560 && cy < 300) {
			pFrame->SetWindowPos(NULL, s.cx / 2 - 285, 0, 560, 500, SWP_NOREPOSITION);
			Invalidate();
		}
		else if (cx < 560)
			pFrame->SetWindowPos(NULL, (s.cx / 2) - 285, 0, 560, cy + 200, SWP_NOREPOSITION);
		else
			pFrame->SetWindowPos(NULL, s.cx / 2 - ((cx + 50) / 2), 0, cx + 50, 500, SWP_NOREPOSITION);

		

		Invalidate();
		UpdateWindow();
	}

}



void CCapCRView::OnButtonSavetext()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	printf("[OnBnClickedBtnSave] \n");
	CString m_strPath;
	CStdioFile file;
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
		ocr->textbox->UpdateData(TRUE);
		file.WriteString(ocr->textbox->m_strTextbox);
		// 종료한다. 
		file.Close();
	}
}


void CCapCRView::OnCheckIgnoreFix()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (!m_bIgnoreFix)
		m_bIgnoreFix = true;
	else
		m_bIgnoreFix = false;
}


void CCapCRView::OnCheckIgnoreSpace()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (!m_bIgnoreSpace)
		m_bIgnoreSpace = true;
	else
		m_bIgnoreSpace = false;
}


void CCapCRView::OnUpdateCheckIgnoreFix(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (m_bIgnoreFix)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CCapCRView::OnUpdateCheckIgnoreSpace(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (m_bIgnoreSpace)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CCapCRView::OnButtonTranslate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CTranslateDlg dlg;
	dlg.DoModal();
}


void CCapCRView::OnButtonSearch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSearchDlg dlg;
	dlg.DoModal();
}


BOOL CCapCRView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));

	return TRUE;
}




COCR* CCapCRView::GetOcrObject()
{
	return ocr;
}


void CCapCRView::OnUpdateButtonSavetext(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(!m_bOcrEmpty);
}


void CCapCRView::OnUpdateButtonSearch(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(!m_bOcrEmpty);
}


void CCapCRView::OnUpdateButtonTranslate(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(!m_bOcrEmpty);
}
