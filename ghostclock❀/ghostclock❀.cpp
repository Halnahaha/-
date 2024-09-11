#include "ghostConfig.hpp"
#include "msg.hpp"
using namespace std;



int main() {

    GhostConfig ghostConfig;

    ghostConfig.init();

    Msg msg;

    float_t scale = ghostConfig.configWindowScale;

    msg.setScale(scale);

    initgraph(std::floor(200 * scale), std::floor(150 * scale));

    HWND hwnd = GetHWnd();
    MoveWindow(hwnd, ghostConfig.configWindowX, ghostConfig.configWindowY, std::floor(200 * scale), std::floor(150 * scale), false);
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE, style & ~WS_CAPTION);  // 设置窗口为无标题栏
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);  // 将窗口置顶

    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
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
        if (GetAsyncKeyState(ghostConfig.configToggleKeyCode) & 0x8000) {
            if (!tildeKeyPressed) {
                activated = !activated;
                if (activated) {
                    msg.showMessage(_T("已激活"), 10, 100, 40, NULL, NULL, 111);
                }
                else {
                    msg.showMessage(_T("未激活"), 10, 100, 40, NULL, NULL, 111);
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
            if (GetAsyncKeyState(ghostConfig.configResetKeyCode) & 0x8000) {
                if (timerStarted) {
                    timerStarted = false;
                    rightButtonPressed = false;
                    msg.showMessage(_T("重置好咯"), 10, 110, 40, 0);
                }
            }

            if (timerStarted) {
                clock_t currentTime = clock();
                int elapsedSeconds = static_cast<int>((currentTime - startTime) / static_cast<double>(CLOCKS_PER_SEC));
                seconds = elapsedSeconds % 60;
                minutes = elapsedSeconds / 60;
                msg.drawTimer(10, 30, minutes, seconds, 80);

                if (elapsedSeconds >= 180) {
                    timerStarted = false;
                    rightButtonPressed = false;
                    msg.showMessage(_T("到三分钟"), 10, 110, 40, 0);
                    seconds = 0;
                    minutes = 0;
                    setfillcolor(BLACK);
                    solidrectangle(std::floor(10 * scale), std::floor(30 * scale), std::floor(100 * scale), std::floor(100 * scale)); // 清除之前的时间显示
                    msg.drawTimer(10, 30, minutes, seconds, 80);
                }
            }
        }

        // 修复无限循环导致的性能问题
        Sleep(8);
    }

    closegraph();
    return 0;
}