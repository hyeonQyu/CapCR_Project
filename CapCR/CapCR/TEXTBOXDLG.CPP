
#include "stdafx.h"
#include "CapCR.h"
#include "TextboxDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


IMPLEMENT_DYNAMIC(CTextboxDlg, CDialogEx)

CTextboxDlg::CTextboxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEXTBOX, pParent)
	, m_strTextbox(_T(""))
{

}

CTextboxDlg::~CTextboxDlg()
{
}

void CTextboxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXTBOX, m_editTextbox);
	DDX_Text(pDX, IDC_EDIT_TEXTBOX, m_strTextbox);
}


BEGIN_MESSAGE_MAP(CTextboxDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTEXTBOX �޽��� ó�����Դϴ�.


BOOL CTextboxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	RECT  rectParent;
	CRect rect;

	((CMainFrame*)AfxGetMainWnd())->GetWindowRect(&rectParent);
	CPoint parentSize;
	parentSize.x = rectParent.right - rectParent.left;
	parentSize.y = rectParent.bottom - rectParent.top;
	SetWindowPos(NULL, 0, 0, parentSize.x-13, 200, SWP_NOREPOSITION);
	GetClientRect(&rect); // ����� ���̾�α��� ������ ��´�.
	CPoint pos;

	// �������� ����
	pos.x = rectParent.left;
	pos.y = parentSize.y;

	// ǥ����ġ ����
	SetWindowPos(NULL, pos.x + 7, pos.y-7, 0, 0, SWP_NOSIZE);

	// ��Ʈ ����
//	m_font.CreatePointFont(150, "����");
	m_font.CreateFontA(21, 9, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("���� ��ü M"));
	m_editTextbox.SetFont(&m_font);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTextboxDlg::OnSize(UINT nType, int cx, int cy)
{
	// �̰� ������� ���� �̤Ф�
	CDialogEx::OnSize(nType, cx, cy);

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TEXT);
	if (!pEdit) { return; }

	CRect rect1;
	pEdit->GetWindowRect(&rect1);
	ScreenToClient(&rect1);
	AfxMessageBox(rect1.Width());
	pEdit->MoveWindow(rect1.left, rect1.top, rect1.right - rect1.left, rect1.bottom - rect1.top, TRUE);
	return;

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}