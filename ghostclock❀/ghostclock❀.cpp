#include <chrono>
#include <thread>
#include "ghostConfig.hpp"
#include "msg.hpp"
using namespace std;

int main() {

    GhostConfig ghostConfig;

    ghostConfig.init();

    Msg msg;

    msg.setScale(ghostConfig.configWindowScale);

    initgraph(msg.ceilWithScale(200), msg.ceilWithScale(150));

    HWND hwnd = GetHWnd();
    MoveWindow(hwnd, ghostConfig.configWindowX, ghostConfig.configWindowY, msg.ceilWithScale(200), msg.ceilWithScale(150), true);
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
                    
                    // 到时间后重置是因为之前到时间后会出现一个额外的小时间的BUG，该BUG未复现所以暂时禁用看看
                    //setfillcolor(BLACK);
                    //solidrectangle(msg.ceilWithScale(10), msg.ceilWithScale(30), msg.ceilWithScale(100), msg.ceilWithScale(100)); // 清除之前的时间显示
                    //msg.drawTimer(10, 30, minutes, seconds, 80);

                }
            }
        }

        // 修复无限循环导致的性能问题
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }

    closegraph();
    return 0;
}