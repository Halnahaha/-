#include <graphics.h>
#include <ctime>
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

void drawTimer(int x, int y, int minutes, int seconds, int size) {
    settextstyle(size, 0, _T("Consolas"));
    setbkcolor(BLACK);
    settextcolor(LIGHTCYAN);
    _TCHAR str[16];
    _stprintf_s(str, _T("%02d:%02d"), minutes, seconds);
    outtextxy(x, y, str);
}

void showMessage(const TCHAR* message, int x, int y, int size) {
    settextstyle(size, 0, _T("Consolas"));
    outtextxy(x, y, message);
    Sleep(1000);
    setfillcolor(BLACK);
    solidrectangle(x, y, x + 350, y + 40);
}

int main() {
    int activateKey;
    int resetKey;

    cout << "请输入激活键的ASCII码: ";
    cin >> activateKey;

    cout << "请输入重置键的ASCII码: ";
    cin >> resetKey;

    initgraph(200, 150);

    HWND hwnd = GetHWnd();
    LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 50, LWA_COLORKEY);  // 设置窗口透明

    bool activated = false;
    bool tildeKeyPressed = false;
    bool rightButtonPressed = false;
    bool altKeyPressed = false;
    bool timerStarted = false;
    int seconds = 0;
    int minutes = 0;
    clock_t startTime = 0;

    while (!GetAsyncKeyState(VK_END)) {
        if (GetAsyncKeyState(activateKey) & 0x8000) {
            if (!tildeKeyPressed) {
                activated = !activated;
                if (activated) {
                    showMessage(_T("已激活"), 10, 100, 40);
                }
                else {
                    showMessage(_T("未激活"), 10, 100, 40);
                }
                tildeKeyPressed = true;
            }
        }
        else {
            tildeKeyPressed = false;
        }

        if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) && !timerStarted) {
            if (!rightButtonPressed) {
                if (activated) {
                    if (!timerStarted) {
                        timerStarted = true;
                        startTime = clock();
                    }
                }
                rightButtonPressed = true;
            }
        }
        else {
            rightButtonPressed = false;
        }

        if (activated) {
            if (GetAsyncKeyState(resetKey) & 0x8000) {
                if (timerStarted) {
                    timerStarted = false;
                    showMessage(_T("重置好咯"), 10, 110, 40);
                }
            }

            if (timerStarted) {
                clock_t currentTime = clock();
                int elapsedSeconds = static_cast<int>(((currentTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)));
                seconds = elapsedSeconds % 60;
                minutes = elapsedSeconds / 60;
                drawTimer(10, 30, minutes, seconds, 80);

                if (elapsedSeconds >= 180) {
                    timerStarted = false;
                    showMessage(_T("到三分钟"), 10, 110, 40);
                    seconds = 0;
                    minutes = 0;
                    setfillcolor(BLACK);
                    solidrectangle(10, 30, 100, 100); // 清除之前的时间显示
                    drawTimer(10, 30, minutes, seconds, 40);
                }
            }
        }
    }

    closegraph();
    return 0;
}