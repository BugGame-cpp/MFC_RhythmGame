# 🎵 MFC Rhythm Game 🎮

## 프로젝트 소개
MFC Rhythm Game은 C++ MFC 프레임워크를 활용하여 개발된 리듬 게임입니다. 다양한 장르의 인기 곡들을 선택하여 플레이할 수 있으며, 키보드를 사용해 화면에 표시되는 노트에 맞춰 타이밍을 맞추는 게임입니다.

## 주요 기능
- 10곡 이상의 다양한 장르 음악 제공
- 직관적인 사용자 인터페이스
- 실시간 점수 시스템
- YouTube 링크 연동
- 난이도 시스템

## 개발 환경
- 언어: C++
- 프레임워크: MFC (Microsoft Foundation Classes)
- IDE: Visual Studio
- 오디오 처리: Windows API

## 설치 및 실행 방법
1. 프로젝트를 클론 또는 다운로드합니다.
2. Visual Studio에서 솔루션 파일(.sln)을 엽니다.
3. 프로젝트 내 음악 파일 경로를 현재 시스템에 맞게 수정합니다.
4. 빌드 및 실행합니다.

## 음악 파일 설정
게임 실행을 위해 아래와 같이 음악 파일 경로를 설정해야 합니다. 음악 파일(.wav)은 프로젝트의 `Resources` 폴더에 위치해야 합니다.

```cpp
// 선택된 노래에 따라 파일 경로 설정
if (songName == _T("LOVE DIVE"))
{
    m_MusicPath = _T("./Resources/Music/LOVE DIVE.wav");
    youtubeURL = _T("https://youtu.be/Y8JFxS1HlDo?si=kilAhsMeG0bDCqBK");
}
else if (songName == _T("Whiplash"))
{
    m_MusicPath = _T("./Resources/Music/Whiplash.wav");
    youtubeURL = _T("https://youtu.be/jWQx2f-CErU?si=OX7xzVGjKtSNLSD5");
}
else if (songName == _T("HAPPY"))
{
    m_MusicPath = _T("./Resources/Music/HAPPY.wav");
    youtubeURL = _T("https://youtu.be/2dFwndi4ung?si=rfr4O9QvhrzyPDhL");
}
else if (songName == _T("Carly Rae Jepsen - Call Me Maybe"))
{
    m_MusicPath = _T("./Resources/Music/Carly Rae Jepsen - Call Me Maybe.wav");
    youtubeURL = _T("https://youtu.be/PPoHpGzcWLI?si=EfLuCVakVN3KvArL");
}
else if (songName == _T("Dont Look Back In Anger"))
{
    m_MusicPath = _T("./Resources/Music/Dont Look Back In Anger.wav");
    youtubeURL = _T("https://youtu.be/cmpRLQZkTb8?si=KgaNIfbgEZRiga*W");
}
else if (songName == _T("Shape Of You"))
{
    m_MusicPath = _T("./Resources/Music/Shape Of You.wav");
    youtubeURL = _T("https://youtu.be/JGwWNGJdvx8?si=u1i3QEX5*9_4vxZZ");
}
else if (songName == _T("Viva la Vida"))
{
    m_MusicPath = _T("./Resources/Music/Viva la Vida.wav");
    youtubeURL = _T("https://youtu.be/dvgZkm1xWPE?si=ff6rygwpmnQhZ2*l");
}
else if (songName == _T("희망"))
{
    m_MusicPath = _T("./Resources/Music/희망.wav");
    youtubeURL = _T("https://youtu.be/izfFPpO7-Zo?si=ZK1NUl3RNccwSFPH");
}
else if (songName == _T("캐논"))
{
    m_MusicPath = _T("./Resources/Music/캐논.wav");
    youtubeURL = _T("https://youtu.be/QjA5faZF1A8?si=qXlbQ5pPFLXoaC3s");
}
else if (songName == _T("비밀번호 486"))
{
    m_MusicPath = _T("./Resources/Music/비밀번호 486.wav");
    youtubeURL = _T("https://youtu.be/r5MM2iI8-58?si=ziyMWCtp8jmrl8f8");
}
```

## 조작 방법
- 메인 메뉴: 마우스 클릭으로 곡 선택
- 게임 플레이: A, S, D, F 키를 사용하여 노트 입력
- ESC: 게임 일시정지 또는 메인 메뉴로 돌아가기

## 점수 시스템
- Perfect: 100점
- Great: 80점
- Good: 50점
- Miss: 0점

## 수록곡 목록
1. IVE - LOVE DIVE
2. NCT 127 - Whiplash
3. TAEYEON - HAPPY
4. Carly Rae Jepsen - Call Me Maybe
5. Oasis - Don't Look Back In Anger
6. Ed Sheeran - Shape Of You
7. Coldplay - Viva la Vida
8. 부활 - 희망
9. Johann Pachelbel - 캐논
10. 윤도현 - 비밀번호 486

## 개발자 정보
- 개발자: 김사무엘,최창일
- 이메일: [이메일 주소]
- GitHub: [GitHub 프로필 링크]

## 라이선스
이 프로젝트는 [라이선스 유형]에 따라 라이선스가 부여됩니다. 자세한 내용은 LICENSE 파일을 참조하세요.

## 주의사항
- 이 게임에 사용된 모든 음악은 개인 학습 및 비상업적 용도로만 사용됩니다.
- 상업적 목적으로 사용하지 마십시오.
