# 🎵 MFC Rhythm Game 🎮

## 프로젝트 소개
MFC Rhythm Game은 C++ MFC 프레임워크를 활용하여 개발된 리듬 게임입니다. 다양한 장르의 인기 곡들을 선택하여 플레이할 수 있으며, 키보드를 사용해 화면에 표시되는 노트에 맞춰 타이밍을 맞추는 게임입니다.

## 주요 기능
- 10곡 이상의 다양한 장르 음악 제공
- 직관적인 사용자 인터페이스
- 실시간 점수 및 콤보 시스템
- YouTube 동영상 연동 (WebView2 활용)
- 3단계 난이도 설정 (Easy, Normal, Hard)
- 시각적 피드백 (노트 히트 이펙트, 배경 변화 등)

## 개발 환경
- 언어: C++
- 프레임워크: MFC (Microsoft Foundation Classes)
- IDE: Visual Studio
- 오디오 처리: Windows Media Control Interface (MCI)
- 웹 컨텐츠: WebView2 컴포넌트

## 설치 및 실행 방법
1. 프로젝트를 클론 또는 다운로드합니다.
2. Visual Studio에서 솔루션 파일(.sln)을 엽니다.
3. 프로젝트 내 음악 파일 경로를 현재 시스템에 맞게 수정합니다.
4. WebView2 런타임이 설치되어 있는지 확인합니다 (없을 경우 설치).
5. 빌드 및 실행합니다.

## 음악 파일 설정
게임 실행을 위해 아래와 같이 음악 파일 경로를 설정해야 합니다. 음악 파일(.wav)은 지정된 경로에 위치해야 합니다.

```cpp
if (songName == _T("LOVE DIVE"))
{
    m_MusicPath = _T("./Resources/Music/LOVE DIVE.wav");
    youtubeURL = _T("https://youtu.be/Y8JFxS1HlDo");
}
else if (songName == _T("Whiplash"))
{
    m_MusicPath = _T("./Resources/Music/Whiplash.wav");
    youtubeURL = _T("https://youtu.be/jWQx2f-CErU");
}
// ... 기타 음악 파일 경로
```

## 조작 방법
- **곡 선택**: 드롭다운 메뉴에서 원하는 곡 선택
- **난이도 설정**: Easy, Normal, Hard 중 선택
- **게임 시작**: "게임 시작" 버튼 클릭
- **노트 입력**: A, S, D, F 키를 사용하여 각 레인에 맞는 노트 입력
- **게임 중지**: 게임 중 "게임 중지" 버튼 클릭

## 게임 시스템
### 판정 시스템
- **Perfect**: 0.05초 이내 히트 (100점)
- **Great**: 0.1초 이내 히트 (80점)
- **Good**: 0.15초 이내 히트 (50점)
- **Miss**: 0.15초 이상 차이 또는 노트 놓침 (0점)

### 콤보 시스템
- 연속으로 노트를 성공적으로 히트할 때마다 콤보 증가
- Miss 판정 시 콤보 초기화
- 10콤보마다 배경 효과 변화
- 콤보에 따른 시각적 피드백 (애니메이션, 색상 변화)

## 시각적 요소
- 동적 배경 효과 (콤보에 따라 변화)
- 노트 히트 이펙트
- 판정 결과 표시
- 콤보 애니메이션
- YouTube 뮤직비디오 동시 재생

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
- 개발자: 김사무엘, 최창일
- 이메일: [이메일 주소]
- GitHub: [GitHub 프로필 링크]

## 라이선스
이 프로젝트는 [라이선스 유형]에 따라 라이선스가 부여됩니다. 자세한 내용은 LICENSE 파일을 참조하세요.

## 주의사항
- 이 게임에 사용된 모든 음악은 개인 학습 및 비상업적 용도로만 사용됩니다.
- 상업적 목적으로 사용하지 마십시오.
- WebView2 컴포넌트 사용을 위해 Microsoft Edge WebView2 런타임이 필요합니다.

## 향후 개발 계획
- 사용자 정의 노트 패턴 추가
- 추가 곡 업데이트
- 온라인 랭킹 시스템
- 더 다양한 시각적 효과
