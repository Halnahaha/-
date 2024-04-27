#include <graphics.h>
#include <ctime>
#include <Windows.h>
#include <tchar.h>

void drawTimer(int x, int y, int minutes, int seconds) {
    settextstyle(100, 0, _T("Consolas"));
    setbkcolor(BLACK);  // 设置背景颜色为黑色
    settextcolor(LIGHTCYAN);  // 设置文字颜色为青色
    RECT rect = { x, y, x + 200, y + 100 };  // 定时器显示区域
    _TCHAR str[16];
    _stprintf_s(str, _T("%02d:%02d"), minutes, seconds);
    outtextxy(x, y, str);  // 使用新的字体大小和样式显示倒计时
}

void showMessage(const TCHAR* message, int x, int y, int size) {
    settextstyle(size, 0, _T("Consolas"));
    outtextxy(x, y, message);  // 在窗口中显示提示信息
    Sleep(1000);
    setfillcolor(BLACK);
    solidrectangle(x, y, x + 250, y + 40);  // 清除提示信息
}

int main() {
    initgraph(400, 300);  // 窗口大小400x300

    bool activated = false; // 是否激活
    bool tildeKeyPressed = false; // 是否按下飘号键
    bool rightButtonPressed = false; // 是否按下右键
    bool altKeyPressed = false; // 是否按下Alt键
    bool timerStarted = false; // 是否已经开始倒计时
    int totalTimeInSeconds = 180;  // 三分钟1

    int initialSeconds = totalTimeInSeconds % 60;
    int initialMinutes = totalTimeInSeconds / 60;
    int seconds = initialSeconds;
    int minutes = initialMinutes;
    clock_t startTime = 0;

    while (!GetAsyncKeyState(0x50)) {  // 当按下P键退出
        if (GetAsyncKeyState(VK_OEM_3) & 0x8000) {  // 检测飘号键按下
            if (!tildeKeyPressed) {
                activated = !activated;
                if (activated) {
                    showMessage(_T("已激活"), 100, 150, 40);  // 显示激活提示信息
                }
                else {
                    showMessage(_T("未激活"), 100, 150, 40);  // 显示未激活提示信息
                }
                tildeKeyPressed = true;
            }
        }
        else {
            tildeKeyPressed = false;
        }

        if (activated) {
            if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {  // 检测右键按下
                if (!timerStarted) {
                    seconds = initialSeconds;
                    minutes = initialMinutes;
                    startTime = clock();  // 开始计时
                    timerStarted = true;
                }
            }

            if (GetAsyncKeyState(0x52) & 0x8000) {  // 检测R键按下
                if (timerStarted) {
                    timerStarted = false;
                    showMessage(_T("计时已重置"), 100, 180, 40);  // 显示重置提示信息
                }
            }

            if (timerStarted) {
                clock_t currentTime = clock();
                int elapsedSeconds = (int)(((currentTime - startTime) / (double)CLOCKS_PER_SEC));
                int remainingSeconds = totalTimeInSeconds - elapsedSeconds;
                if (remainingSeconds > 0) {
                    minutes = remainingSeconds / 60;
                    seconds = remainingSeconds % 60;
                    drawTimer(100, 50, minutes, seconds);  // 更新倒计时显示
                }
                else {
                    timerStarted = false;
                    showMessage(_T("时间到"), 100, 180, 40);  // 显示时间到提示信息
                }
            }
        }
    }

    closegraph();
    return 0;
}