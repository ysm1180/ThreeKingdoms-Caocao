# 게임 엔진 내장 Lua 클래스 & 함수 목록 

C++ 문법 형태로 설명합니다.

## Class 목록

### Control Class
#### 공통 컨트롤
```C++
class CBaseControl {
    bool IsEnabled();
    bool IsVisible();
    number GetWidth();
    number GetHeight();
    number GetX();
    number GetY();
    string GetUserData();

    void SetEnabled(bool value);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetX(int x);
    void SetY(int y);
    void SetCreateEvent(LuaFunction fn);
    void SetDestroyEvent(LuaFunction fn);
    void SetMouseLButtonUpEvent(LuaFunction fn);
    void SetMouseLButtonDownEvent(LuaFunction fn);
    void SetMouseMoveEvent(LuaFunction fn);
    void SetMouseEnterEvent(LuaFunction fn);
    void SetMouseLeaveEvent(LuaFunction fn);
    void SetUserData(string data);

    void Refresh();
    void RefreshRegion(int left, number top, number right, number bottom);

    void Show();
    void Hide();
}

/*
@description 컨트롤의 활성화 여부를 반환합니다.
@return 활성화 여부
*/
bool CBaseControl::IsEnabled();

/*
@description 컨트롤의 Visible 여부를 반환합니다.
@return visible 여부
*/
bool CBaseControl::IsVisible();

/*
@description 컨트롤의 너비를 반환합니다.
@return 컨트롤 너비
*/
int CBaseControl::GetWidth();

/*
@description 컨트롤의 높이를 반환합니다.
@return 컨트롤 높이
*/
int CBaseControl::GetHeight();

/*
@description 컨트롤의 X 위치를 반환합니다.
@return 컨트롤 X 위치
*/
int CBaseControl::GetX();

/*
@description 컨트롤의 Y 위치를 반환합니다.
@return 컨트롤 Y 위치
*/
int CBaseControl::GetY();

/*
@description 컨트롤의 데이터를 반환합니다.
@return 컨트롤 데이터
*/
string CBaseControl::GetUserData();

/*
@description 컨트롤의 활성화 여부를 설정합니다.
@param value 활성화 여부
*/
void CBaseControl::SetEnabled(bool value);

void CBaseControl::SetWidth(int width);
void CBaseControl::SetHeight(int height);
void CBaseControl::SetX(int x);
void CBaseControl::SetY(int y);
void CBaseControl::SetCreateEvent(LuaFunction fn);
void CBaseControl::SetDestroyEvent(LuaFunction fn);
void CBaseControl::SetMouseLButtonUpEvent(LuaFunction fn);
void CBaseControl::SetMouseLButtonDownEvent(LuaFunction fn);
void CBaseControl::SetMouseMoveEvent(LuaFunction fn);
void CBaseControl::SetMouseEnterEvent(LuaFunction fn);
void CBaseControl::SetMouseLeaveEvent(LuaFunction fn);
void CBaseControl::SetUserData(string data);

void CBaseControl::Refresh();
void CBaseControl::RefreshRegion(int left, number top, number right, number bottom);

void CBaseControl::Show();
void CBaseControl::Hide();
```

### Manager Class

#### 파일 관리
```C++
class CFile;
class CFileManager {
    CFile LoadFile(string path);
    char ReadByte(CFile file, number position);
    void WriteByte(CFile file, number position, number byte);
    void CloseFile(CFile file);
};

/*
@description path 의 파일을 불러온다.
@param path : 불러올 파일의 경로
@return 불러온 파일을 CFile 클래스로 반환한다.
*/
CFile CFileManager::LoadFile(string path);

/*
@description 파일의 byte 를 읽는다.
@param file : 읽을 파일
@param position : 읽을 위치
@return 읽은 byte
*/
char CFileManager::ReadByte(CFile file, number position);

/*
@description 파일의 byte 를 write 한다.
@param file : Write 할 파일
@param position : Write 위치
@param byte : Write 할 byte
*/
void CFileManager::WriteByte(CFile file, number position, number byte);

/*
@description 파일을 닫는다. LoadFile 한 파일은 반드시 CloseFile로 닫아주어야 한다.
@param file : 닫을 파일
*/
void CFileManager::CloseFile(CFile file);
```

#### 게임 관리
```C++
class CGameManager {

}
```


## LUA 에서 실제로 사용되는 Singleton 변수 이름
+ Class CFileManager -> fileManager (in Lua)
+ Class CGameManager -> gameManager (in Lua)
+ Class CControlManager -> controlManager (in Lua)

### LUA 사용 예시
```lua
    control = controlManager:CreateWindow(parent)

    file = fileManager:LoadFile(path)

    gameManager:Quit()
```