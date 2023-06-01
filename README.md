# LeagueProject
# Git, GitHub 사용법
## GitHub에서 가져오기
- https://git-scm.com/에서 git 다운로드
- 바탕화면 우클릭 -> git bash here
- git config --global user.name "John Doe"
- git config --global user.email johndoe@example.com
    - github이메일 사용할 것
- git config --list
    - 변경사항 확인
- git clone https://github.com/ymsong1209/LeagueProject.git
## 파일 올리는 순서
    1. git pull
    2. git status
    3. git add
    4. git commit -m "커밋메세지"
    5. git push origin my_branch
    6. git merge dev
        - merge 오류 수정
        - 오류 수정후 다시 git commit -m "오류 수정"
        - git push origin my_branch
    7. git checkout dev
    8. git merge my_branch

## Branch
- git `checkout` dev
    - dev branch로 `이동`
    - 현재 master내부에 dev 밑에 ymsong branch가 존재합니다. dev 밑에 각자 개인 이름으로 branch를 파서 dev를 수정하고, master은 최종 수정된 dev를 업데이트 하는 방식으로 하겠습니다.
- git branch ymsong
    - ymsong branch `생성`
    - 기타 branch를 확인하고 싶으면 git branch 명령어를 사용하면 됩니다.
## Git Status
- 현재 폴더 내부의 상황을 보여줍니다.
- 모든 파일들은 `Tracked`, `Untracked`, `Unmodified`, `Modified`, `Staged` 단계로 구성이 됩니다.
- 파일을 `수정`하면 `Modified`상태가 되어서 `add`명령어를 통해 `Staged`단계로 올려야지 파일을 업로드 할 수가 있습니다.

## Pull, Add, Commit, Push
- git pull
    - 원격저장소(github)에서 로컬저장소(개인pc)로 파일을 가져옵니다. 비슷한 명령어인 `fetch`는 pull과 달리 merge를 안합니다.
- git status
    - 현재 branch에 파일이 무슨 상태인지 확인할 수 있습니다.
    - 수정된 파일이 올라오지 않았으면 `unstaged 단계`라고 빨갛게 표시가 될 것입니다.
- git add
    - `unstaged 단계`에 있던 파일들을 `staged`단계로 올려줍니다.
    - 파일 전체를 올리고 싶으면 `git add .`를 입력하고, 특저 파일만 올리고 싶으면 `git add 파일명`을 입력하시면 됩니다.
- git rm
    - Staged단계에 있던 파일을 UnStaged로 내리는 명령어입니다.
    - `git rm --cached <file>`을 사용하거나
    - `git rm --cached .`으로 모든 파일을 내릴 수가 있습니다.
- git commit
    - commit 메세지 입력.
    - git commit -m "오류 수정"
        - "ㅁㄴㅇㄹ"같이 애매하게 적지 말고 "Collider.h 오류수정" 같이 적어주면 됩니다.
- git push
    - Staged된 파일들을 원격저장소(GitHub)쪽에 push해줍니다.
    - `git push origin my_branch`

## Git Merge
- Git Merge
    - 자신의 개인 branch랑 `dev` branch를 먼저 merge해서 충돌사항이 있는지 확인해야합니다.
    - git checkout my_branch
    - git pull origin
    - git merge dev
    - 오류 발생시
        - code .
            - 코드를 수정해주는 에디터를 엽니다.
            - 이후 오류 수정
        - git add .
        - git commit -m "merge 오류 수정"
        - 다시 git merge dev 수행
    - git checkout dev
    - git merge my_branch
    

