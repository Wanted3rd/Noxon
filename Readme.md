# Noxon

- [Rust](https://rust.facepunch.com/) 모작
- 팀원 : 김홍범, 박원서, 백승지


## Git Rule

### Branch 작성

1. Main Branch가 Release Branch 입니다.

2. "대분류" 기준으로 `[대분류]` 의 형태로 Branch를 만듭니다.

3. "중분류" 기준으로 역할분담이 되어 있으므로, `[대분류]/[대분류]-[중분류]` 의 형태로 Branch를 만들어 작업합니다.

4. "소분류" 로 Branch를 만드는 것은 개인의 판단에 맡깁니다.

5. 하위 Branch를 상위 Branch로 Merge 할 때는 무조건 아래의 사항들을 체크 후 Merge 합니다.
    - Build가 되는가. (Compile Successed)
    - Begin Play가 되는가.
    - Runtime Error는 예측 어려운 부분이므로 필수는 아니지만, 경계만 부탁 드립니다.

- (`5`의 예시) Player/Player-Move 브랜치를 Player Branch로 병합할 때, Player-Move Branch의 내용들이 Build에 문제가 없어야 합니다.


## Project Rule

### 디렉토리 구조 Rule

1. 대부분 대분류 기준으로 폴더를 나눕니다.

2. Content 폴더 구조와 동일하게 Source 구조도 맞춰주세요.

3. Unreal Class 생성 시 Public과 Private을 나눠서 생성해 주세요.
    - Class 생성할 때 "공개" 혹은 "Public"으로 설정하고 생성하면 됩니다.

### Blueprint 파일 관리

1. Map은 각자의 이름의 Map을 이용해 주세요.

2. Merge Map은 팀원과 일정 공유 후 이용해 주세요.
    - Merge Map이 Conflict(충돌)가 나는 경우는 최대한 피해주세요.

3. <b>본인이 만든 Blueprint 파일이 아니라면</b>, 직접 수정을 지양해 주세요.
    - 부득이하게 수정해야 하는 경우라면, 복제본을 만들고 본인의 이름을 뒤에 붙여서 이용해주세요.
    - 혹은 팀원과 이야기 하여 Conflict를 최대한 피해주세요.

