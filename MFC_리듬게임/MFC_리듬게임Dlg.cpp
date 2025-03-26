// MFC_리듬게임Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_리듬게임.h"
#include "MFC_리듬게임Dlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Note 클래스 구현
Note::Note(int lane, double timing)
    : m_Lane(lane), m_Timing(timing), m_Position(0), m_Hit(false)
{
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    // 구현입니다.
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
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_START, &CMFC리듬게임Dlg::OnBnClickedButtonStart)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_SELECT_MUSIC, &CMFC리듬게임Dlg::OnBnClickedButtonSelectMusic)

END_MESSAGE_MAP()

// CMFC리듬게임Dlg 대화 상자

CMFC리듬게임Dlg::CMFC리듬게임Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFC__DIALOG, pParent)
    , m_GameTime(0.0)
    , m_Score(0)
    , m_GameStarted(false)
    , m_LastJudgment(Judgment::NONE)
    , m_JudgmentShowTime(0.0)
    , m_Combo(0)
    , m_MaxCombo(0)
    , m_MusicPlaying(false)
    , m_MusicDuration(0.0)
    , m_ComboAnimTime(0.0)           
    , m_ComboAnimDuration(0.3)       
    , m_LastAnimatedCombo(0)         
    , m_BackgroundLevel(0)           
    , m_LastBackgroundCombo(0)       
    , m_BackgroundTransitionTime(0.0) 
    , m_BackgroundTransitionDuration(1.0) 
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_MusicPath = _T("");
}



void CMFC리듬게임Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC리듬게임Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_START, &CMFC리듬게임Dlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CMFC리듬게임Dlg 메시지 처리기

BOOL CMFC리듬게임Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 대화상자 크기 설정
    SetWindowPos(NULL, 0, 0, 900, 800, SWP_NOMOVE | SWP_NOZORDER);

    // 화면 크기에 맞게 판정선 위치 조정
    CRect rect;
    GetClientRect(&rect);
    int webViewHeight = rect.Height() * 2 / 5;  // 40% 높이
    const_cast<int&>(JUDGMENT_LINE_Y) = webViewHeight + (rect.Height() - webViewHeight) * 2 / 3;

    // 시작 버튼 생성 및 스타일 설정
    CButton* pButton = new CButton();
    pButton->Create(_T("게임 시작"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(rect.Width() / 2 - 70, rect.Height() - 100, rect.Width() / 2 + 70, rect.Height() - 70),
        this, IDC_BUTTON_START);
   

    // 버튼 폰트 설정
    CFont* buttonFont = new CFont();
    buttonFont->CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("맑은 고딕"));

    // 노래 선택 버튼 생성
    CComboBox* pCombo = new CComboBox();
    pCombo->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        CRect(20, rect.Height() - 100, 180, rect.Height() - 70), this, IDC_COMBO_SONG_LIST);

    // 노래 목록 추가
    pCombo->AddString(_T("LOVE DIVE"));
    pCombo->AddString(_T("Whiplash"));
    pCombo->AddString(_T("HAPPY"));
    pCombo->AddString(_T("Carly Rae Jepsen - Call Me Maybe"));
    pCombo->AddString(_T("Dont Look Back In Anger"));
    pCombo->AddString(_T("Shape Of You"));
    pCombo->AddString(_T("Viva la Vida"));
    pCombo->AddString(_T("희망"));
    pCombo->AddString(_T("캐논"));
    pCombo->AddString(_T("비밀번호 486"));




    // 기본 선택
    pCombo->SetCurSel(0);

    CComboBox* pDiffCombo = new CComboBox();
    pDiffCombo->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        CRect(200, rect.Height() - 100, 300, rect.Height() - 70), this, 1004);

    pDiffCombo->AddString(_T("Easy"));
    pDiffCombo->AddString(_T("Normal"));
    pDiffCombo->AddString(_T("Hard"));
    pDiffCombo->SetCurSel(1); // 기본 선택: Normal

    pCombo->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    pDiffCombo->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    pButton->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // 게임 초기화
    InitGame();

    // WebView2 초기화 추가
    InitializeWebView2();

    return TRUE;
}


void CMFC리듬게임Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC리듬게임Dlg::OnPaint()
{
    if (IsIconic())
    {
        // (기존 아이콘 그리기 코드 유지)
    }
    else
    {
        CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

        // 더블 버퍼링을 위한 메모리 DC 생성
        CDC memDC;
        memDC.CreateCompatibleDC(&dc);

        CRect rect;
        GetClientRect(&rect);

        CBitmap memBitmap;
        memBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

        CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

        // 게임 화면 그리기
        DrawGame(&memDC);

        // 메모리 DC에서 화면으로 복사
        dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

        // 리소스 해제
        memDC.SelectObject(pOldBitmap);
    }
}

HCURSOR CMFC리듬게임Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 키보드 입력 처리
BOOL CMFC리듬게임Dlg::PreTranslateMessage(MSG* pMsg)
{
    // 키보드 메시지 처리
    if (pMsg->message == WM_KEYDOWN && m_GameStarted)
    {
        switch (pMsg->wParam)
        {
        case 'A': ProcessKeyInput(0); return TRUE;
        case 'S': ProcessKeyInput(1); return TRUE;
        case 'D': ProcessKeyInput(2); return TRUE;
        case 'F': ProcessKeyInput(3); return TRUE;
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

// 타이머 처리
void CMFC리듬게임Dlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1 && m_GameStarted)
    {
        // 음악 위치 확인
        if (m_MusicPlaying)
        {
            double position = GetMusicPosition();

            // 음악이 끝났는지 확인
            if (position >= m_MusicDuration - 0.5)  // 0.5초 버퍼
            {
                // 게임 종료
                KillTimer(1);
                StopMusic();
                m_GameStarted = false;

                // 버튼 텍스트 변경
                CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON_START);
                if (pButton)
                {
                    pButton->SetWindowText(_T("게임 시작"));
                }

                // 결과 메시지 표시
                CString resultMsg;
                resultMsg.Format(_T("게임 종료!\n최종 점수: %d\n최대 콤보: %d"), m_Score, m_MaxCombo);
                AfxMessageBox(resultMsg);

                // 화면 갱신
                Invalidate(FALSE);
                return;
            }

            // 게임 시간을 음악 위치와 동기화
            m_GameTime = position;
        }
        else
        {
            // 음악이 재생되지 않으면 직접 시간 계산
            m_GameTime += 0.016;  // 약 16ms
        }

        // 노트 위치 업데이트
        UpdateNotes();

        // 화면 다시 그리기
        Invalidate(FALSE);
    }

    CDialogEx::OnTimer(nIDEvent);
}
// PlayYouTubeVideo 함수 구현
BOOL CMFC리듬게임Dlg::PlayYouTubeVideo(const CString& youtubeURL)
{
    // 이미 재생 중인 브라우저가 있으면 중지
    StopYouTubeVideo();

    // 비디오 영역 설정
    GetClientRect(&m_VideoRect);
    m_VideoRect.bottom = m_VideoRect.bottom / 2;  // 상단 절반만 사용

    try {
        // 웹 브라우저 컨트롤 생성
        HRESULT hr = CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_ALL,
            IID_IWebBrowser2, (void**)&m_pWebBrowser);

        if (FAILED(hr) || !m_pWebBrowser) {
            AfxMessageBox(_T("웹 브라우저를 생성할 수 없습니다."));
            return FALSE;
        }

        // 유튜브 비디오 ID 추출
        CString videoID = ExtractYouTubeVideoID(youtubeURL);
        if (videoID.IsEmpty()) {
            AfxMessageBox(_T("유효한 YouTube URL이 아닙니다."));
            return FALSE;
        }

        // HTML 컨텐츠 생성 (유튜브 임베드)
        CString htmlContent;
        htmlContent.Format(
            _T("<html><head><style>body{margin:0;padding:0;overflow:hidden;background:black;}</style></head>")
            _T("<body><iframe width=\"100%%\" height=\"100%%\" ")
            _T("src=\"https://www.youtube.com/embed/%s?autoplay=1&controls=0&showinfo=0&rel=0\" ")
            _T("frameborder=\"0\" allowfullscreen></iframe></body></html>"),
            videoID);

        // 브라우저 속성 설정
        m_pWebBrowser->put_Silent(VARIANT_TRUE);  // 에러 메시지 숨기기
        m_pWebBrowser->put_AddressBar(VARIANT_FALSE);
        m_pWebBrowser->put_ToolBar(VARIANT_FALSE);
        m_pWebBrowser->put_MenuBar(VARIANT_FALSE);
        m_pWebBrowser->put_StatusBar(VARIANT_FALSE);
        m_pWebBrowser->put_Visible(VARIANT_TRUE);

        // 브라우저 위치 및 크기 설정
        CRect rectWindow;
        GetWindowRect(&rectWindow);
        m_pWebBrowser->put_Left(rectWindow.left);
        m_pWebBrowser->put_Top(rectWindow.top);
        m_pWebBrowser->put_Width(m_VideoRect.Width());
        m_pWebBrowser->put_Height(m_VideoRect.Height());

        // 브라우저 열기
        BSTR bstrURL = SysAllocString(L"about:blank");
        VARIANT varEmpty;
        VariantInit(&varEmpty);
        hr = m_pWebBrowser->Navigate(bstrURL, &varEmpty, &varEmpty, &varEmpty, &varEmpty);
        SysFreeString(bstrURL);

        if (FAILED(hr)) {
            AfxMessageBox(_T("웹 페이지 로드 실패"));
            return FALSE;
        }

        // 브라우저가 로드될 때까지 대기
        VARIANT_BOOL busy = VARIANT_TRUE;
        while (busy == VARIANT_TRUE) {
            m_pWebBrowser->get_Busy(&busy);
            Sleep(50);
        }

        // 문서 객체 가져오기
        CComPtr<IDispatch> pDisp;
        hr = m_pWebBrowser->get_Document(&pDisp);
        if (FAILED(hr) || !pDisp) {
            AfxMessageBox(_T("문서 객체 가져오기 실패"));
            return FALSE;
        }

        // HTML 문서 인터페이스 가져오기
        CComQIPtr<IHTMLDocument2> pHTMLDoc = pDisp;
        if (!pHTMLDoc) {
            AfxMessageBox(_T("HTML 문서 인터페이스 가져오기 실패"));
            return FALSE;
        }

        // HTML 문서에 내용 쓰기
        BSTR bstrHTML = htmlContent.AllocSysString();
        SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
        if (!psa) {
            SysFreeString(bstrHTML);
            AfxMessageBox(_T("SafeArray 생성 실패"));
            return FALSE;
        }

        VARIANT* pVar;
        hr = SafeArrayAccessData(psa, (void**)&pVar);
        if (SUCCEEDED(hr)) {
            pVar->vt = VT_BSTR;
            pVar->bstrVal = bstrHTML;
            SafeArrayUnaccessData(psa);

            hr = pHTMLDoc->write(psa);
            hr = pHTMLDoc->close();
        }

        SafeArrayDestroy(psa);
        // bstrHTML은 SafeArray가 파괴될 때 자동으로 해제됨

        TRACE(_T("YouTube 비디오 로드 성공: %s\n"), videoID);
        return TRUE;
    }
    catch (_com_error& e) {
        CString errorMsg;
        errorMsg.Format(_T("COM 오류: 0x%08x - %s"), e.Error(), e.ErrorMessage());
        AfxMessageBox(errorMsg);
    }
    catch (...) {
        AfxMessageBox(_T("알 수 없는 오류 발생"));
    }

    return FALSE;
}

// StopYouTubeVideo 함수 구현
void CMFC리듬게임Dlg::StopYouTubeVideo()
{
    if (m_pWebBrowser) {
        // 브라우저 닫기
        m_pWebBrowser->Quit();
        m_pWebBrowser.Release();
    }
}

// 유튜브 비디오 ID 추출 헬퍼 함수
CString CMFC리듬게임Dlg::ExtractYouTubeVideoID(const CString& youtubeURL)
{
    TRACE(_T("유튜브 URL 파싱: %s\n"), youtubeURL);

    // 단순화된 URL 형식 처리 (https://youtu.be/VIDEO_ID)
    int slashPos = youtubeURL.ReverseFind('/');
    if (slashPos != -1) {
        CString videoID = youtubeURL.Mid(slashPos + 1);

        // 물음표 이후 부분 제거
        int qPos = videoID.Find(_T('?'));
        if (qPos != -1) {
            videoID = videoID.Left(qPos);
        }

        TRACE(_T("추출된 비디오 ID: %s\n"), videoID);
        return videoID;
    }

    // 표준 유튜브 URL 형식 처리 (https://www.youtube.com/watch?v=VIDEO_ID)
    int vPos = youtubeURL.Find(_T("v="));
    if (vPos != -1) {
        CString videoID = youtubeURL.Mid(vPos + 2);
        int ampPos = videoID.Find(_T('&'));
        if (ampPos != -1) {
            videoID = videoID.Left(ampPos);
        }

        TRACE(_T("추출된 비디오 ID: %s\n"), videoID);
        return videoID;
    }

    // 비디오 ID를 찾을 수 없음
    AfxMessageBox(_T("유튜브 비디오 ID를 추출할 수 없습니다."));
    return _T("");
}

void CMFC리듬게임Dlg::OnBnClickedButtonStart()
{
    // 콤보박스에서 현재 선택된 곡 가져오기
    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SONG_LIST);
    int sel = pCombo->GetCurSel();
    CString songName;
    pCombo->GetLBText(sel, songName);

    // 선택된 노래에 따라 파일 경로와 YouTube URL 설정
    CString youtubeURL;

    // 선택된 노래에 따라 파일 경로 설정
    if (songName == _T("LOVE DIVE"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\LOVE DIVE.wav");
        youtubeURL = _T("https://youtu.be/Y8JFxS1HlDo?si=kilAhsMeG0bDCqBK");
    }
    else if (songName == _T("Whiplash"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\Whiplash.wav");
        youtubeURL = _T("https://youtu.be/jWQx2f-CErU?si=OX7xzVGjKtSNLSD5");
    }
    else if (songName == _T("HAPPY"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\HAPPY.wav");
        youtubeURL = _T("https://youtu.be/2dFwndi4ung?si=rfr4O9QvhrzyPDhL");
    }
    else if (songName == _T("Carly Rae Jepsen - Call Me Maybe"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\Carly Rae Jepsen - Call Me Maybe.wav");
        youtubeURL = _T("https://youtu.be/PPoHpGzcWLI?si=EfLuCVakVN3KvArL");
    }
    else if (songName == _T("Dont Look Back In Anger"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\Dont Look Back In Anger.wav");
        youtubeURL = _T("https://youtu.be/cmpRLQZkTb8?si=KgaNIfbgEZRiga_W");
    }
    else if (songName == _T("Shape Of You"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\Shape Of You.wav");
        youtubeURL = _T("https://youtu.be/JGwWNGJdvx8?si=u1i3QEX5_9_4vxZZ");
    }
    else if (songName == _T("Viva la Vida"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\Viva la Vida.wav");
        youtubeURL = _T("https://youtu.be/dvgZkm1xWPE?si=ff6rygwpmnQhZ2_l");
    }
    else if (songName == _T("희망"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\희망.wav");
        youtubeURL = _T("https://youtu.be/izfFPpO7-Zo?si=ZK1NUl3RNccwSFPH");
    }
    else if (songName == _T("캐논"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\캐논.wav");
        youtubeURL = _T("https://youtu.be/QjA5faZF1A8?si=qXlbQ5pPFLXoaC3s");
    }
    else if (songName == _T("비밀번호 486"))
    {
        m_MusicPath = _T("E:\\source\\MFC_RhythmGame\\비밀번호 486.wav");
        youtubeURL = _T("https://youtu.be/r5MM2iI8-58?si=ziyMWCtp8jmrl8f8");
    }

    // 모든 노래에 대해 YouTube URL 설정
    if (!youtubeURL.IsEmpty())
    {
        NavigateWebView2(youtubeURL);  // 이 부분을 추가합니다!
    }

    // 난이도 콤보박스에서 선택값 읽기
    CComboBox* pDiffCombo = (CComboBox*)GetDlgItem(1004);
    int diffSel = pDiffCombo->GetCurSel();

    if (diffSel == 0)      m_Difficulty = Difficulty::EASY;
    else if (diffSel == 1) m_Difficulty = Difficulty::NORMAL;
    else if (diffSel == 2) m_Difficulty = Difficulty::HARD;

    // 기존 로직 유지
    if (!m_GameStarted)
    {
        m_MusicDuration = GetMusicDuration(m_MusicPath);

        InitGame();
        if (PlayMusic(m_MusicPath))
        {
            SetTimer(1, 16, NULL);
            m_GameStarted = true;
        }
        else
        {
            AfxMessageBox(_T("음악 파일을 로드할 수 없습니다."));
        }
    }
    else
    {
        KillTimer(1);
        StopMusic();
        StopYouTubeVideo();
        m_GameStarted = false;
    }

    // 버튼 텍스트 변경
    CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON_START);
    if (pButton)
        pButton->SetWindowText(m_GameStarted ? _T("게임 중지") : _T("게임 시작"));
}


// 게임 초기화
void CMFC리듬게임Dlg::InitGame()
{
    // 변수 초기화
    m_GameTime = 0.0;
    m_Score = 0;
    m_LastJudgment = Judgment::NONE;
    m_JudgmentShowTime = 0.0;
    m_Combo = 0;
    m_MaxCombo = 0;
    m_MusicPlaying = false;

    // 콤보 애니메이션 초기화 
    m_ComboAnimTime = 0.0;
    m_LastAnimatedCombo = 0;


    // 배경 변화 초기화 
    m_BackgroundLevel = 0;
    m_LastBackgroundCombo = 0;
    m_BackgroundTransitionTime = 0.0;


    // 노트 데이터 로드
    LoadNotes();
}
// 노트 데이터 로드
void CMFC리듬게임Dlg::LoadNotes()
{
    m_Notes.clear();
    double startOffset = 2.0; // 음악 시작 후 몇 초부터 노트 생성
    int laneCount = 4;

    double interval;
    int maxNotesPerBeat;

    // 난이도에 따라 생성 패턴 달리함
    switch (m_Difficulty)
    {
    case Difficulty::EASY:
        interval = 1.5;
        maxNotesPerBeat = 1;
        break;
    case Difficulty::NORMAL:
        interval = 1.0;
        maxNotesPerBeat = 2;
        break;
    case Difficulty::HARD:
        interval = 0.5;
        maxNotesPerBeat = 3;
        break;
    }

    for (double t = startOffset; t < m_MusicDuration - 1.0; t += interval)
    {
        int notesThisTime = rand() % maxNotesPerBeat + 1;

        // 중복 없는 레인 번호 무작위 선택
        std::vector<int> lanes = { 0, 1, 2, 3 };
        std::random_shuffle(lanes.begin(), lanes.end());

        for (int i = 0; i < notesThisTime; i++)
        {
            m_Notes.push_back(Note(lanes[i], t));
        }
    }
}


// 노트 위치 업데이트
void CMFC리듬게임Dlg::UpdateNotes()
{
    // 웹뷰 영역의 높이 계산
    CRect rect;
    GetClientRect(&rect);
    int webViewHeight = rect.Height() / 4;  // 화면 높이의 1/4

    for (auto& note : m_Notes)
    {
        if (!note.IsHit())
        {
            // 노트의 Y 위치 계산 (시간에 따라 아래로 이동)
            double timeDiff = note.GetTiming() - m_GameTime;

            // 노트가 화면에 보이기 시작할 시간 차이
            double visibleTimeDiff = 3.0;  // 3초 전부터 노트 표시

            if (timeDiff <= visibleTimeDiff)
            {
                // 노트 위치 계산 - 웹뷰 아래부터 시작하도록 조정
                int startY = webViewHeight + 5;  // 레인 시작 위치와 동일하게
                double position = JUDGMENT_LINE_Y - (timeDiff / visibleTimeDiff) * (JUDGMENT_LINE_Y - startY);
                note.SetPosition(position);

                // 노트가 판정선을 지나쳤는지 확인
                if (timeDiff < -0.2)  // 0.2초 이상 지나면 자동으로 미스 처리
                {
                    note.SetHit(true);
                    m_LastJudgment = Judgment::MISS;
                    m_JudgmentShowTime = m_GameTime + 0.5;  // 0.5초간 판정 표시
                }
            }
        }
    }
    // 히트 이펙트 업데이트
    for (auto it = m_HitEffects.begin(); it != m_HitEffects.end(); ) {
        if (m_GameTime - it->startTime > it->duration) {
            it = m_HitEffects.erase(it);
        }
        else {
            ++it;
        }
    }
}

// 키 입력 처리
void CMFC리듬게임Dlg::ProcessKeyInput(int lane)
{
    // 해당 레인에서 가장 가까운 노트 찾기
    Note* closestNote = FindClosestNote(lane, m_GameTime);

    if (closestNote)
    {
        // 노트와 현재 시간의 차이 계산
        double timeDiff = std::abs(closestNote->GetTiming() - m_GameTime);

        // 판정
        Judgment judge = JudgeHit(closestNote->GetTiming(), m_GameTime);

        // 판정에 따른 점수 부여
        switch (judge)
        {
        case Judgment::PERFECT:
            m_Score += 100;
            m_Combo++;
            break;
        case Judgment::GREAT:
            m_Score += 80;
            m_Combo++;
            break;
        case Judgment::GOOD:
            m_Score += 50;
            m_Combo++;
            break;
        case Judgment::MISS:
            m_Combo = 0;  // 미스 시 콤보 초기화
            break;
        }
        // 콤보가 증가하면 애니메이션 시작
        if (m_Combo > m_LastAnimatedCombo) {
            m_ComboAnimTime = m_GameTime;
            m_LastAnimatedCombo = m_Combo;
        }

        // 10콤보마다 배경 변화 
        if (m_Combo >= m_LastBackgroundCombo + 10 && judge != Judgment::MISS) {
            m_BackgroundLevel = m_Combo / 10;  // 10콤보마다 레벨업
            m_LastBackgroundCombo = m_Combo - (m_Combo % 10);  // 현재 레벨에 해당하는 콤보 값
            m_BackgroundTransitionTime = m_GameTime;  // 배경 전환 시작 시간 설정
        }

        // 콤보가 0이 되면 배경도 초기화 
        if (m_Combo == 0) {
            m_BackgroundLevel = 0;
            m_LastBackgroundCombo = 0;
            m_BackgroundTransitionTime = m_GameTime;
        }

        // 최대 콤보 갱신
        if (m_Combo > m_MaxCombo)
            m_MaxCombo = m_Combo;

        // 노트 처리
        closestNote->SetHit(true);

        // 판정 결과 표시
        m_LastJudgment = judge;
        m_JudgmentShowTime = m_GameTime + 0.5;  // 0.5초간 판정 표시

        // 판정에 따른 점수 부여 부분 끝에 추가 (미스가 아닌 경우만 이펙트 추가)
        if (judge != Judgment::MISS) {
            // 히트 이펙트 추가
            HitEffect effect;
            effect.lane = lane;
            effect.startTime = m_GameTime;
            effect.duration = 0.2; // 0.2초로 짧게 설정
            effect.judgment = judge;
            m_HitEffects.push_back(effect);
        }
    }
}

// 판정 함수
Judgment CMFC리듬게임Dlg::JudgeHit(double noteTime, double pressTime)
{
    double diff = std::abs(noteTime - pressTime);

    if (diff < 0.05) return Judgment::PERFECT;
    else if (diff < 0.1) return Judgment::GREAT;
    else if (diff < 0.15) return Judgment::GOOD;
    else return Judgment::MISS;
}

// 가장 가까운 노트 찾기
Note* CMFC리듬게임Dlg::FindClosestNote(int lane, double time)
{
    Note* closestNote = nullptr;
    double minTimeDiff = 1.0;  // 판정 최대 시간 차이

    for (auto& note : m_Notes)
    {
        if (!note.IsHit() && note.GetLane() == lane)
        {
            double timeDiff = std::abs(note.GetTiming() - time);
            if (timeDiff < minTimeDiff)
            {
                minTimeDiff = timeDiff;
                closestNote = &note;
            }
        }
    }

    return closestNote;
}

// 게임 그리기
void CMFC리듬게임Dlg::DrawGame(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    // 배경 그리기
    DrawBackground(pDC, rect);

    // 레인 그리기
    DrawLanes(pDC, rect);

    // 판정선 그리기
    DrawJudgmentLine(pDC);

    // 노트 그리기
    DrawNotes(pDC);

    // 이펙트 그리기
    DrawHitEffects(pDC);

    // 점수 표시
    DrawScore(pDC);

    // 판정 결과 표시
    DrawJudgment(pDC);

    // 콤보 표시
    DrawCombo(pDC);
}

// 배경 그리기
void CMFC리듬게임Dlg::DrawBackground(CDC* pDC, const CRect& rect)
{
    // 전환 효과를 위한 알파값 계산 (0.0~1.0)
    double alpha = 1.0;
    if (m_GameTime - m_BackgroundTransitionTime < m_BackgroundTransitionDuration) {
        alpha = (m_GameTime - m_BackgroundTransitionTime) / m_BackgroundTransitionDuration;
    }

    // 배경 레벨에 따른 색상 설정
    COLORREF baseColor1, baseColor2;
    int starCount = 100;  // 기본 별 개수

    switch (m_BackgroundLevel) {
    case 0:  // 기본 배경 (어두운 파란색 -> 검은색)
        baseColor1 = RGB(0, 0, 50);
        baseColor2 = RGB(0, 0, 0);
        break;
    case 1:  // 10~19 콤보 (파란색 + 보라색 조합)
        baseColor1 = RGB(20, 0, 60);
        baseColor2 = RGB(0, 0, 40);
        starCount = 150;
        break;
    case 2:  // 20~29 콤보 (보라색 + 빨간색 조합)
        baseColor1 = RGB(40, 0, 70);
        baseColor2 = RGB(20, 0, 30);
        starCount = 200;
        break;
    case 3:  // 30~39 콤보 (빨간색 + 주황색 조합)
        baseColor1 = RGB(60, 0, 60);
        baseColor2 = RGB(30, 0, 30);
        starCount = 250;
        break;
    case 4:  // 40~49 콤보 (주황색 + 노란색 조합)
        baseColor1 = RGB(70, 20, 50);
        baseColor2 = RGB(35, 10, 30);
        starCount = 300;
        break;
    default:  // 50+ 콤보 (색상 변화가 더 화려해짐)
        // 시간에 따라 변화하는 화려한 색상
        int time = static_cast<int>(m_GameTime * 10) % 100;
        baseColor1 = RGB(
            80 + time / 2,
            40 + (100 - time) / 2,
            60 + time / 3
        );
        baseColor2 = RGB(
            40 + time / 4,
            20 + (100 - time) / 4,
            30 + time / 6
        );
        starCount = 350 + m_BackgroundLevel * 10;  // 더 많은 별
        break;
    }

    // 그라데이션 배경 그리기
    for (int y = 0; y < rect.Height(); y++) {
        double ratio = static_cast<double>(y) / rect.Height();

        int r = static_cast<int>(GetRValue(baseColor1) * (1.0 - ratio) + GetRValue(baseColor2) * ratio);
        int g = static_cast<int>(GetGValue(baseColor1) * (1.0 - ratio) + GetGValue(baseColor2) * ratio);
        int b = static_cast<int>(GetBValue(baseColor1) * (1.0 - ratio) + GetBValue(baseColor2) * ratio);

        COLORREF color = RGB(r, g, b);
        pDC->FillSolidRect(0, y, rect.Width(), 1, color);
    }

    // 별 효과 추가 (레벨에 따라 더 많은 별과 다양한 크기의 별 생성)
    srand(static_cast<unsigned int>(10));  // 고정된 시드 사용 (깜빡임 방지)

    for (int i = 0; i < starCount; i++) {
        int x = rand() % rect.Width();
        int y = rand() % rect.Height();
        int size = 1;

        // 레벨이 높을수록 큰 별이 나올 확률 증가
        int sizeRandom = rand() % 100;
        if (m_BackgroundLevel > 0 && sizeRandom < m_BackgroundLevel * 5) {
            size = (sizeRandom % 3) + 2;  // 2~4 크기의 별

            // 큰 별은 반짝이는 효과 (시간에 따라 밝기 변경)
            int brightness = 150 + static_cast<int>(50 * sin(m_GameTime * 3 + i));
            brightness = min(255, max(100, brightness));

            COLORREF starColor = RGB(brightness, brightness, brightness);

            // 별 그리기 (크기에 따라 점 또는 작은 원)
            if (size == 2) {
                pDC->SetPixel(x, y, starColor);
                pDC->SetPixel(x + 1, y, starColor);
                pDC->SetPixel(x, y + 1, starColor);
                pDC->SetPixel(x + 1, y + 1, starColor);
            }
            else {
                // 작은 원 그리기
                CBrush brush(starColor);
                CBrush* pOldBrush = pDC->SelectObject(&brush);
                pDC->Ellipse(x - size / 2, y - size / 2, x + size / 2, y + size / 2);
                pDC->SelectObject(pOldBrush);
            }
        }
        else {
            // 일반 별 (작은 흰색 점)
            pDC->SetPixel(x, y, RGB(255, 255, 255));
        }
    }

    // 레벨 5 이상에서 추가 효과: 파동 효과
    if (m_BackgroundLevel >= 5) {
        // 여러 개의 원형 파동 그리기
        int waveCount = 3;
        for (int w = 0; w < waveCount; w++) {
            // 파동의 중심점
            int centerX = rect.Width() / 2;
            int centerY = rect.Height() / 2;

            // 시간과 파동 인덱스에 따른 반지름 계산
            double phase = (m_GameTime * 0.5 + w * 1.0) - floor(m_GameTime * 0.5 + w * 1.0);
            int radius = static_cast<int>(phase * rect.Width() * 0.8);

            // 파동의 두께와 투명도 계산
            int thickness = 2;
            int alpha = static_cast<int>((1.0 - phase) * 100);

            // 파동 색상 (레벨에 따라 다른 색상)
            COLORREF waveColor;
            switch ((m_BackgroundLevel + w) % 6) {
            case 0: waveColor = RGB(255, 50, 50); break;   // 빨강
            case 1: waveColor = RGB(255, 150, 50); break;  // 주황
            case 2: waveColor = RGB(255, 255, 50); break;  // 노랑
            case 3: waveColor = RGB(50, 255, 50); break;   // 초록
            case 4: waveColor = RGB(50, 50, 255); break;   // 파랑
            case 5: waveColor = RGB(150, 50, 255); break;  // 보라
            }

            // 파동 그리기 (원 테두리)
            CPen pen(PS_SOLID, thickness, waveColor);
            CPen* pOldPen = pDC->SelectObject(&pen);
            pDC->SelectStockObject(NULL_BRUSH);  // 내부는 투명하게

            pDC->Ellipse(
                centerX - radius,
                centerY - radius * 0.75,  // 약간 타원형으로
                centerX + radius,
                centerY + radius * 0.75
            );

            pDC->SelectObject(pOldPen);
        }
    }
}

void CMFC리듬게임Dlg::OnBnClickedButtonSelectMusic()
{
    CFileDialog fileDlg(TRUE, _T("wav"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
        _T("음악 파일 (*.wav;*.mp3)|*.wav;*.mp3|모든 파일 (*.*)|*.*||"));

    if (fileDlg.DoModal() == IDOK)
    {
        m_MusicPath = fileDlg.GetPathName();
        AfxMessageBox(_T("선택된 노래: ") + m_MusicPath);
    }
}


// 레인 그리기
void CMFC리듬게임Dlg::DrawLanes(CDC* pDC, const CRect& rect)
{
    // 레인의 너비는 그대로 유지
    int totalLaneWidth = LANE_COUNT * LANE_WIDTH;

    // 화면 중앙에 레인 배치
    int centerX = rect.Width() / 2;
    int laneStartX = centerX - (totalLaneWidth / 2);

    // 웹뷰 영역의 높이 계산
    int webViewHeight = rect.Height() / 4;  // 화면 높이의 1/4

    // 레인 시작 Y 위치 - 웹뷰 바로 아래로 조정
    int laneStartY = webViewHeight + 5;  // 약간의 여백만 두고 바로 시작


    // 각 레인 그리기
    for (int i = 0; i < LANE_COUNT; i++)
    {
        int laneX = laneStartX + i * LANE_WIDTH;

        // 레인 배경 (그라데이션 효과)
        for (int y = laneStartY; y <= JUDGMENT_LINE_Y; y++)
        {
            int intensity = 30 + (y - laneStartY) * 20 / (JUDGMENT_LINE_Y - laneStartY);
            COLORREF color = RGB(intensity / 3, intensity / 2, intensity);
            pDC->FillSolidRect(laneX, y, LANE_WIDTH, 1, color);
        }

        // 레인 구분선 (밝은 색상)
        pDC->FillSolidRect(laneX, laneStartY, 1, JUDGMENT_LINE_Y - laneStartY, RGB(80, 80, 100));
        pDC->FillSolidRect(laneX + LANE_WIDTH - 1, laneStartY, 1, JUDGMENT_LINE_Y - laneStartY, RGB(80, 80, 100));

        // 키 표시 (둥근 버튼 형태)
        CString keyText;
        switch (i)
        {
        case 0: keyText = _T("A"); break;
        case 1: keyText = _T("S"); break;
        case 2: keyText = _T("D"); break;
        case 3: keyText = _T("F"); break;
        }

        // 키 버튼 그리기
        CRect keyRect(laneX + 5, JUDGMENT_LINE_Y + 10, laneX + LANE_WIDTH - 5, JUDGMENT_LINE_Y + 40);
        pDC->FillSolidRect(keyRect, RGB(60, 60, 70));
        pDC->Draw3dRect(keyRect, RGB(120, 120, 140), RGB(40, 40, 60));

        // 텍스트 설정
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(RGB(255, 255, 255));

        // 키 글자 폰트 설정
        CFont font;
        font.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

        CFont* pOldFont = pDC->SelectObject(&font);
        pDC->DrawText(keyText, keyRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);
    }
}

// 판정선 그리기
void CMFC리듬게임Dlg::DrawJudgmentLine(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    int centerX = rect.Width() / 2;
    int laneStartX = centerX - (LANE_COUNT * LANE_WIDTH) / 2;
    int laneEndX = laneStartX + LANE_COUNT * LANE_WIDTH;

    // 판정선 그리기 (빛나는 효과)
    int glowSize = 3;
    for (int i = -glowSize; i <= glowSize; i++)
    {
        int alpha = 255 - abs(i) * 50;
        COLORREF color = RGB(alpha, alpha, 255);
        pDC->FillSolidRect(laneStartX, JUDGMENT_LINE_Y + i, laneEndX - laneStartX, 1, color);
    }

    // 중앙 판정선 (밝은 색상)
    pDC->FillSolidRect(laneStartX, JUDGMENT_LINE_Y, laneEndX - laneStartX, 2, RGB(255, 255, 255));
}

// 노트 그리기
void CMFC리듬게임Dlg::DrawNotes(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    int centerX = rect.Width() / 2;
    int laneStartX = centerX - (LANE_COUNT * LANE_WIDTH) / 2;

    for (const auto& note : m_Notes)
    {
        if (!note.IsHit())
        {
            double timeDiff = note.GetTiming() - m_GameTime;

            // 노트가 화면에 보이기 시작할 시간 차이
            double visibleTimeDiff = 3.0;

            if (timeDiff <= visibleTimeDiff && timeDiff > -0.2)
            {
                int laneX = laneStartX + note.GetLane() * LANE_WIDTH;
                int noteY = static_cast<int>(note.GetPosition());

                // 노트 모양 (둥근 형태)
                int cornerRadius = 5;
                CRect noteRect(laneX + 10, noteY - NOTE_HEIGHT / 2, laneX + LANE_WIDTH - 10, noteY + NOTE_HEIGHT / 2);

                // 노트 색상 결정 (각 레인마다 다른 색상)
                COLORREF noteColor;
                switch (note.GetLane())
                {
                case 0: noteColor = RGB(255, 100, 100); break; // 빨강
                case 1: noteColor = RGB(100, 255, 100); break; // 초록
                case 2: noteColor = RGB(100, 100, 255); break; // 파랑
                case 3: noteColor = RGB(255, 255, 100); break; // 노랑
                default: noteColor = RGB(200, 200, 200); break;
                }

                // 노트 그림자 효과
                CRect shadowRect = noteRect;
                shadowRect.OffsetRect(2, 2);
                pDC->FillSolidRect(shadowRect, RGB(0, 0, 0));

                // 노트 내부 채우기
                pDC->FillSolidRect(noteRect, noteColor);

                // 노트 밝은 부분 (하이라이트 효과)
                CRect highlightRect(noteRect.left, noteRect.top, noteRect.right, noteRect.top + 3);
                pDC->FillSolidRect(highlightRect, RGB(255, 255, 255));

                // 노트 테두리
                pDC->Draw3dRect(noteRect, RGB(255, 255, 255), RGB(150, 150, 150));
            }
        }
    }
}


// 점수 표시
void CMFC리듬게임Dlg::DrawScore(CDC* pDC)
{
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 255));

    CRect rect;
    GetClientRect(&rect);

    // 웹뷰 높이 계산
    int webViewHeight = rect.Height() / 4;

    // 점수 배경 위치 수정 (웹뷰 아래로)
    CRect scorePanel(10, webViewHeight + 10, 150, webViewHeight + 50);
    pDC->FillSolidRect(scorePanel, RGB(50, 50, 70));
    pDC->Draw3dRect(scorePanel, RGB(100, 100, 120), RGB(30, 30, 50));

    // 점수 텍스트
    CString scoreStr;
    scoreStr.Format(_T("점수: %d"), m_Score);

    // 폰트 설정
    CFont font;
    font.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("맑은 고딕"));

    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->DrawText(scoreStr, scorePanel, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    pDC->SelectObject(pOldFont);
}

// 판정 결과 표시
void CMFC리듬게임Dlg::DrawJudgment(CDC* pDC)
{
    if (m_LastJudgment != Judgment::NONE && m_GameTime < m_JudgmentShowTime)
    {
        pDC->SetBkMode(TRANSPARENT);

        CString judgmentStr;
        COLORREF judgmentColor;

        switch (m_LastJudgment)
        {
        case Judgment::PERFECT:
            judgmentStr = _T("PERFECT");
            judgmentColor = RGB(255, 215, 0); // 골드
            break;
        case Judgment::GREAT:
            judgmentStr = _T("GREAT");
            judgmentColor = RGB(0, 255, 0); // 초록
            break;
        case Judgment::GOOD:
            judgmentStr = _T("GOOD");
            judgmentColor = RGB(0, 150, 255); // 파랑
            break;
        case Judgment::MISS:
            judgmentStr = _T("MISS");
            judgmentColor = RGB(255, 0, 0); // 빨강
            break;
        }

        CRect rect;
        GetClientRect(&rect);

        int centerX = rect.Width() / 2;
        CRect judgmentRect(centerX - 100, JUDGMENT_LINE_Y - 80, centerX + 100, JUDGMENT_LINE_Y - 40);

        // 판정 텍스트에 빛나는 효과 추가
        for (int i = -2; i <= 2; i++)
        {
            for (int j = -2; j <= 2; j++)
            {
                if (i != 0 || j != 0)
                {
                    CRect glowRect = judgmentRect;
                    glowRect.OffsetRect(i, j);

                    // 폰트 설정
                    CFont glowFont;
                    glowFont.CreateFont(35, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

                    CFont* pOldFont = pDC->SelectObject(&glowFont);
                    pDC->SetTextColor(RGB(50, 50, 50)); // 그림자 색상
                    pDC->DrawText(judgmentStr, glowRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    pDC->SelectObject(pOldFont);
                }
            }
        }

        // 메인 텍스트
        CFont font;
        font.CreateFont(35, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

        CFont* pOldFont = pDC->SelectObject(&font);
        pDC->SetTextColor(judgmentColor);
        pDC->DrawText(judgmentStr, judgmentRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);
    }
}
void CMFC리듬게임Dlg::DrawCombo(CDC* pDC)
{
    if (m_Combo > 0)
    {
        CRect rect;
        GetClientRect(&rect);

        // 콤보 표시 위치
        int rightX = rect.Width() - 20;
        int middleY = rect.Height() / 2;

        // 패널 배경 (더 크게 설정하여 두 텍스트를 충분히 담음)
        CRect panelRect(rightX - 140, middleY - 30, rightX, middleY + 40);
        pDC->FillSolidRect(panelRect, RGB(30, 30, 50));
        pDC->Draw3dRect(panelRect, RGB(70, 70, 90), RGB(20, 20, 40));

        // 콤보 숫자 영역 (상단에 배치)
        CRect comboRect(rightX - 140, middleY - 30, rightX, middleY);

        // 콤보 텍스트
        CString comboStr;
        comboStr.Format(_T("%d COMBO"), m_Combo);

        // 애니메이션 진행 상태 계산
        double animProgress = 1.0;

        if (m_GameTime - m_ComboAnimTime < m_ComboAnimDuration) {
            double progress = (m_GameTime - m_ComboAnimTime) / m_ComboAnimDuration;
            animProgress = 1.0 + 0.5 * sin(progress * 3.14159);
        }

        // 콤보 값에 따라 색상 변화
        COLORREF comboColor;
        if (m_Combo >= 50)
            comboColor = RGB(255, 50, 50);
        else if (m_Combo >= 30)
            comboColor = RGB(255, 100, 0);
        else if (m_Combo >= 10)
            comboColor = RGB(255, 160, 0);
        else
            comboColor = RGB(255, 200, 100);

        // 폰트 설정 - 애니메이션에 따라 크기 조절
        int fontSize = static_cast<int>(25 * animProgress);

        CFont font;
        font.CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("맑은 고딕"));

        CFont* pOldFont = pDC->SelectObject(&font);
        pDC->SetTextColor(comboColor);
        pDC->SetBkMode(TRANSPARENT);

        // 움직임 효과
        if (m_GameTime - m_ComboAnimTime < m_ComboAnimDuration) {
            double offsetY = -3.0 * sin((m_GameTime - m_ComboAnimTime) / m_ComboAnimDuration * 3.14159);
            comboRect.OffsetRect(0, static_cast<int>(offsetY));
        }

        pDC->DrawText(comboStr, comboRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // 추가 텍스트를 위한 구분선 추가 (선택 사항)
        if (m_Combo >= 10) {
            int lineY = middleY + 5;  // 중앙보다 약간 아래
            pDC->FillSolidRect(panelRect.left + 10, lineY, panelRect.Width() - 20, 1, RGB(60, 60, 80));
        }

        // 콤보 추가 텍스트 영역 (하단에 별도 배치)
        if (m_Combo >= 10) {
            CFont smallFont;
            smallFont.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

            pDC->SelectObject(&smallFont);

            CString bonusText;
            if (m_Combo >= 50)
                bonusText = _T("EXCELLENT!");
            else if (m_Combo >= 30)
                bonusText = _T("GREAT COMBO!");
            else if (m_Combo >= 10)
                bonusText = _T("NICE COMBO!");

            // 추가 텍스트 영역을 아래쪽에 별도 배치
            CRect bonusRect(rightX - 140, middleY + 10, rightX, middleY + 35);

            // 추가 텍스트 색상 (콤보 색상보다 약간 더 밝게)
            COLORREF bonusColor = RGB(
                min(255, GetRValue(comboColor) + 40),
                min(255, GetGValue(comboColor) + 40),
                min(255, GetBValue(comboColor) + 40)
            );

            pDC->SetTextColor(bonusColor);
            pDC->DrawText(bonusText, bonusRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }

        pDC->SelectObject(pOldFont);
    }
}

void CMFC리듬게임Dlg::DrawHitEffects(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    int centerX = rect.Width() / 2;
    int laneStartX = centerX - (LANE_COUNT * LANE_WIDTH) / 2;

    for (const auto& effect : m_HitEffects) {
        double progress = (m_GameTime - effect.startTime) / effect.duration;
        if (progress < 1.0) {
            int laneX = laneStartX + effect.lane * LANE_WIDTH;
            int laneCenter = laneX + LANE_WIDTH / 2;

            // 판정에 따른 색상 결정 - 흰색 성분 없이 순수 색상만 사용
            COLORREF color;
            switch (effect.judgment) {
            case Judgment::PERFECT: color = RGB(180, 80, 200); break;  // 보라색
            case Judgment::GREAT:   color = RGB(50, 180, 200); break;  // 청록색
            case Judgment::GOOD:    color = RGB(70, 120, 200); break;  // 파랑색
            default:                color = RGB(150, 150, 150); break; // 회색
            }

            // 효과 크기 계산
            double sizeProgress = sin(progress * 3.14159);
            int maxSize = LANE_WIDTH * 1.5;
            int size = static_cast<int>(maxSize * sizeProgress);

            // 원형 효과 - 밝은 흰색 효과 대신 색상 그라데이션 사용
            for (int i = 0; i < 3; i++) {
                int circleSize = size - i * 10;
                if (circleSize > 0) {
                    // 투명도 효과 (점점 사라지게)
                    int intensity = static_cast<int>((1.0 - progress) * 255);

                    // 각 원마다 색상 조절 (밝은 흰색 대신 색상의 밝기만 약간 조절)
                    int r = GetRValue(color) + (2 - i) * 20;
                    int g = GetGValue(color) + (2 - i) * 20;
                    int b = GetBValue(color) + (2 - i) * 20;

                    r = min(255, r);
                    g = min(255, g);
                    b = min(255, b);

                    COLORREF circleColor = RGB(r, g, b);

                    // 테두리와 내부 채우기 모두 동일 색상 사용
                    CPen pen(PS_SOLID, 1, circleColor);
                    CPen* pOldPen = pDC->SelectObject(&pen);

                    CBrush brush(circleColor);
                    CBrush* pOldBrush = pDC->SelectObject(&brush);

                    // 원 그리기 (내부 채우기)
                    pDC->Ellipse(
                        laneCenter - circleSize / 2, JUDGMENT_LINE_Y - circleSize / 2,
                        laneCenter + circleSize / 2, JUDGMENT_LINE_Y + circleSize / 2
                    );

                    pDC->SelectObject(pOldPen);
                    pDC->SelectObject(pOldBrush);
                }
            }

            // 빛줄기 효과는 기존 유지하되 색상만 변경
            CPen lightPen(PS_SOLID, 2, color);
            CPen* pOldPen = pDC->SelectObject(&lightPen);

            int lightLength = static_cast<int>(LANE_WIDTH * 1.0 * sizeProgress);
            int lightWidth = static_cast<int>(LANE_WIDTH * 0.6 * (1.0 - progress));

            // 위쪽 빛줄기
            pDC->MoveTo(laneCenter - lightWidth / 2, JUDGMENT_LINE_Y);
            pDC->LineTo(laneCenter, JUDGMENT_LINE_Y - lightLength);
            pDC->LineTo(laneCenter + lightWidth / 2, JUDGMENT_LINE_Y);

            // 아래쪽 빛줄기
            pDC->MoveTo(laneCenter - lightWidth / 2, JUDGMENT_LINE_Y);
            pDC->LineTo(laneCenter, JUDGMENT_LINE_Y + lightLength);
            pDC->LineTo(laneCenter + lightWidth / 2, JUDGMENT_LINE_Y);

            pDC->SelectObject(pOldPen);
        }
    }
}

bool CMFC리듬게임Dlg::InitializeWebView2()
{
    // 웹뷰 호스트 윈도우 생성
    CRect rect;
    GetClientRect(&rect);

    // 웹뷰 크기를 상단 1/4 영역으로 설정
    int webViewHeight = rect.Height() / 4;  // 화면 높이의 1/4

    m_webViewHwnd = ::CreateWindowEx(
        0, L"STATIC", L"", WS_CHILD | WS_VISIBLE,
        0, 0, rect.Width(), webViewHeight,  // 상단 전체 너비, 1/4 높이
        m_hWnd, NULL, AfxGetInstanceHandle(), NULL);

    if (!m_webViewHwnd) {
        AfxMessageBox(_T("웹뷰 호스트 윈도우 생성 실패"));
        return false;
    }

    m_webViewWnd.Attach(m_webViewHwnd);

    // WebView2 환경 생성
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* environment) -> HRESULT {
                if (FAILED(result)) {
                    AfxMessageBox(_T("WebView2 환경 생성 실패"));
                    return result;
                }

                // 환경 저장
                m_webViewEnvironment = environment;

                // WebView2 컨트롤러 생성
                environment->CreateCoreWebView2Controller(
                    m_webViewHwnd,
                    Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(result)) {
                                AfxMessageBox(_T("WebView2 컨트롤러 생성 실패"));
                                return result;
                            }

                            // 컨트롤러 저장
                            m_webViewController = controller;

                            // WebView2 가져오기
                            controller->get_CoreWebView2(&m_webView);

                            // WebView2 설정
                            ICoreWebView2Settings* settings;
                            m_webView->get_Settings(&settings);
                            settings->put_IsScriptEnabled(TRUE);
                            settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                            settings->put_IsWebMessageEnabled(TRUE);

                            // 초기 크기 설정
                            ResizeWebView();

                            // WebView2를 배경으로 설정 (클릭 이벤트가 투과되도록)
                            ::SetWindowLong(m_webViewHwnd, GWL_EXSTYLE,
                                GetWindowLong(m_webViewHwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);

                            // WebView2를 Z-order의 가장 아래로 설정
                            ::SetWindowPos(m_webViewHwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

                            // 컨트롤러 표시
                            controller->put_IsVisible(TRUE);

                            return S_OK;
                        }).Get());
                return S_OK;
            }).Get());

    if (FAILED(hr)) {
        AfxMessageBox(_T("WebView2 환경 생성 시작 실패"));
        return false;
    }

    return true;
}

void CMFC리듬게임Dlg::ResizeWebView()
{
    if (m_webViewController) {
        RECT bounds;
        ::GetClientRect(m_webViewHwnd, &bounds);
        m_webViewController->put_Bounds(bounds);
    }
}

bool CMFC리듬게임Dlg::NavigateWebView2(const CString& url)
{
    if (!m_webView) {
        AfxMessageBox(_T("WebView2가 초기화되지 않았습니다."));
        return false;
    }

    // 유튜브 임베드 URL 형식으로 변환 (기존 ExtractYouTubeVideoID 함수 활용)
    CString videoID = ExtractYouTubeVideoID(url);
    if (videoID.IsEmpty()) {
        AfxMessageBox(_T("유효한 YouTube URL이 아닙니다."));
        return false;
    }

    // 유튜브 임베드 HTML 생성
    CString html;
    html.Format(
        L"<html><head><style>body{margin:0;padding:0;overflow:hidden;background:black;}</style></head>"
        L"<body><iframe width=\"100%%\" height=\"100%%\" "
        L"src=\"https://www.youtube.com/embed/%s?autoplay=1&mute=1&controls=1\" "
        L"frameborder=\"0\" allow=\"autoplay; encrypted-media\" allowfullscreen></iframe></body></html>",
        videoID);

    // HTML 직접 설정 (임베드 URL로 직접 이동하는 대신 HTML 내용을 설정)
    m_webView->NavigateToString(html.GetBuffer());

    return true;
}

void CMFC리듬게임Dlg::CleanupWebView2()
{
    if (m_webViewController) {
        m_webViewController->Close();
    }
    m_webView = nullptr;
    m_webViewController = nullptr;
    m_webViewEnvironment = nullptr;

    if (m_webViewWnd) {
        m_webViewWnd.Detach();
    }

    if (m_webViewHwnd && ::IsWindow(m_webViewHwnd)) {
        ::DestroyWindow(m_webViewHwnd);
        m_webViewHwnd = NULL;
    }
}
bool CMFC리듬게임Dlg::PlayMusic(const CString& filePath)
{
    // 이미 재생 중인 음악 중지
    StopMusic();

    // MCI 명령 형식으로 재생
    CString command;
    command.Format(_T("open \"%s\" type mpegvideo alias BGM"), filePath);

    if (mciSendString(command, NULL, 0, NULL) != 0)
        return false;

    if (mciSendString(_T("play BGM"), NULL, 0, NULL) != 0)
    {
        mciSendString(_T("close BGM"), NULL, 0, NULL);
        return false;
    }

    m_MusicPlaying = true;
    m_MusicPath = filePath;
    m_MusicDuration = GetMusicDuration(filePath);

    return true;
}

// 음악 중지 함수
void CMFC리듬게임Dlg::StopMusic()
{
    if (m_MusicPlaying)
    {
        mciSendString(_T("stop BGM"), NULL, 0, NULL);
        mciSendString(_T("close BGM"), NULL, 0, NULL);
        m_MusicPlaying = false;
    }
}

// 음악 길이 가져오기
double CMFC리듬게임Dlg::GetMusicDuration(const CString& filePath)
{
    // 임시로 파일을 열어 길이 확인
    CString command;
    command.Format(_T("open \"%s\" type mpegvideo alias TempBGM"), filePath);

    if (mciSendString(command, NULL, 0, NULL) != 0)
        return 0.0;

    TCHAR buffer[128] = { 0 };
    if (mciSendString(_T("status TempBGM length"), buffer, sizeof(buffer), NULL) != 0)
    {
        mciSendString(_T("close TempBGM"), NULL, 0, NULL);
        return 0.0;
    }

    double duration = _ttoi(buffer) / 1000.0; // ms를 초로 변환
    mciSendString(_T("close TempBGM"), NULL, 0, NULL);

    return duration;
}

// 현재 재생 위치 가져오기
double CMFC리듬게임Dlg::GetMusicPosition()
{
    if (!m_MusicPlaying)
        return 0.0;

    TCHAR buffer[128] = { 0 };
    if (mciSendString(_T("status BGM position"), buffer, sizeof(buffer), NULL) != 0)
        return 0.0;

    return _ttoi(buffer) / 1000.0; // ms를 초로 변환
}

// 대화 상자 종료 시 호출
void CMFC리듬게임Dlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    // 음악 정리
    StopMusic();
    StopYouTubeVideo();

    CleanupWebView2();
}