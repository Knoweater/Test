#include <windows.h>

// ���� ����
class Window {
public:
    Window(int width, int height, int posX, int posY, int borderWidth, COLORREF borderColor, const wchar_t* title, COLORREF titleColor);
    void Show(int nCmdShow);
    void Hide();
    void Minimize();
    void Maximize();
    void Close();
    void Move(int x, int y);
    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

private:
    HWND hwnd;
    HMENU hMenu; // ���� ����
    int width;
    int height;
    int posX;
    int posY;
    int borderWidth;
    COLORREF borderColor;
    const wchar_t* title;
    COLORREF titleColor;
};

// �����������
Window::Window(int width, int height, int posX, int posY, int borderWidth, COLORREF borderColor, const wchar_t* title, COLORREF titleColor)
    : width(width), height(height), posX(posX), posY(posY), borderWidth(borderWidth), borderColor(borderColor), title(title), titleColor(titleColor) {

    // ��������� ����� ����
    WNDCLASS wc = { 0 };
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MyWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    RegisterClass(&wc);

    // ��������� ����
    hMenu = CreateMenu();
    AppendMenu(hMenu, MF_STRING, 1, L"����");
    AppendMenu(hMenu, MF_STRING, 2, L"������");
    AppendMenu(hMenu, MF_STRING, 3, L"�����");
    AppendMenu(hMenu, MF_STRING, 4, L"������");

    // ��������� ����
    hwnd = CreateWindow(
        L"MyWindowClass", title, WS_OVERLAPPEDWINDOW,
        posX, posY, width, height, NULL, hMenu, GetModuleHandle(NULL), this);
}

void Window::Show(int nCmdShow) {
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
}

void Window::Hide() {
    ShowWindow(hwnd, SW_HIDE);
}

void Window::Minimize() {
    ShowWindow(hwnd, SW_MINIMIZE);
}

void Window::Maximize() {
    ShowWindow(hwnd, SW_MAXIMIZE);
}

void Window::Close() {
    PostMessage(hwnd, WM_CLOSE, 0, 0);
}

void Window::Move(int x, int y) {
    SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// ������� ����������
LRESULT CALLBACK Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static Window* pWindow;
    switch (msg) {
    case WM_CREATE: {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lp;
        pWindow = (Window*)pCreate->lpCreateParams;
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wp)) {
        case 1:
            MessageBox(hwnd, L"������� '����'", L"����", MB_OK);
            break;
        case 2:
            MessageBox(hwnd, L"������� '������'", L"����", MB_OK);
            break;
        case 3:
            MessageBox(hwnd, L"������� '�����'", L"����", MB_OK);
            break;
        case 4:
            MessageBox(hwnd, L"������� '������'", L"����", MB_OK);
            break;
        }
        break;
    }
    case WM_KEYDOWN: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        switch (wp) {
        case 'W': MoveWindow(hwnd, rect.left, rect.top - 10, rect.right - rect.left, rect.bottom - rect.top, TRUE); break;
        case 'S': MoveWindow(hwnd, rect.left, rect.top + 10, rect.right - rect.left, rect.bottom - rect.top, TRUE); break;
        case 'A': MoveWindow(hwnd, rect.left - 10, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE); break;
        case 'D': MoveWindow(hwnd, rect.left + 10, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE); break;
        case 'Z': pWindow->Minimize(); break; // ��������
        case 'X': pWindow->Maximize(); break; // ����������
        case 'C': pWindow->Close(); break;    // �������
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // ������� ���������
        RECT rect;
        GetClientRect(hwnd, &rect);
        rect.top = 0;
        rect.bottom = 40; // ������ ���������
        SetBkColor(hdc, pWindow->titleColor);
        SetTextColor(hdc, RGB(255, 255, 255)); // ����� �����
        FillRect(hdc, &rect, CreateSolidBrush(pWindow->titleColor));
        DrawText(hdc, pWindow->title, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// ������� ������� WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // ��������� ����
    Window myWindow(800, 600, 100, 100, 10, RGB(0, 0, 0), L"����������� ����", RGB(0, 128, 100)); // ���� ���������

    // �������� ����
    myWindow.Show(nCmdShow);

    // ���� ����������
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    int a;
    return 0;
}
