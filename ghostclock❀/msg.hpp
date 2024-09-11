#include <ctime>
#include <graphics.h>
#include <thread>
#include <Windows.h>
#include "timer.hpp"
using namespace std;

class Msg
{
public:
    float scale = 1;
    const TCHAR* lastMessage; // 消息内容
    time_t lastSendTime = 0; // 发送时间 （10位UNIX时间戳）
    int lastX;
    int lastY;
    int lastFontHeight;
    int lastFontWidth;
    int lastCleanHeight;
    int lastCleanWidth;
    Timer* lastMessageTimer;

    void setScale(float s)
    {
        scale = s;
    }

    void showTestMessage()
    {
        setbkcolor(BLACK);
        settextstyle(40, 0, _T("Consolas"));
        settextcolor(LIGHTCYAN);
        outtextxy(0, 0, _T("TEST"));
    }

    void showMessage(const TCHAR* message, int x, int y, int fontHeight, int fontWidth = 0, int cleanHeight = NULL, int cleanWidth = NULL)
    {
        if (lastMessageTimer == NULL) lastMessageTimer = new Timer;

        if (cleanHeight == NULL) cleanHeight = fontHeight;
        if (cleanWidth == NULL) cleanWidth = 360;

        if (scale == NULL || scale == 1) {
            scale = 1;
        }
        else {
            x = std::floor(x * scale);
            y = std::floor(y * scale);
            fontHeight = std::floor(fontHeight * scale);
            fontWidth = std::floor(fontWidth * scale);
            cleanHeight = std::floor(cleanHeight * scale);
            cleanWidth = std::floor(cleanWidth * scale);
        }

        // 当新消息传入时销毁旧消息
        time_t now = time(0);
        // 检查 上次发送时间 是否>1s
        if ((lastSendTime > 0) && (lastSendTime + 1 < now)) {
            cleanDraw(lastX, lastY, lastX + lastCleanWidth, lastY + lastCleanHeight);
            // 检查定时器 如果有定时器则停止定时器并立即销毁上次消息
            if (lastMessageTimer->status == lastMessageTimer->TIMER_START) {
                lastMessageTimer->stop();
                // TODO 虽然不知道什么BUG，但是重新new一个Timer之后就解决了个奇怪的问题
                lastMessageTimer = new Timer;
            }
        }

        setbkcolor(BLACK);
        settextstyle(fontHeight, fontWidth, _T("Consolas"));
        settextcolor(LIGHTCYAN);
        outtextxy(x, y, message);
        // 设置定时器 1s 后销毁
        lastMessageTimer->start(1, [=]() {
            cleanDraw(x, y, x + cleanWidth, y + cleanHeight);
            });

        lastMessage = message;
        lastSendTime = time(0);
        lastX = x;
        lastY = y;
        lastFontHeight = fontHeight;
        lastFontWidth = fontWidth;
        lastCleanHeight = cleanHeight;
        lastCleanWidth = cleanWidth;

    }

    void drawTimer(int x, int y, int minutes, int seconds, int size) {
        settextstyle(std::floor(size * scale), 0, _T("Consolas"));
        setbkcolor(BLACK);
        settextcolor(LIGHTCYAN);
        _TCHAR str[16];
        _stprintf_s(str, _T("%02d:%02d"), minutes, seconds);
        outtextxy(std::floor(x * scale), std::floor(y * scale), str);
    }

    void cleanDraw(int x, int y, int width, int height) {
        setfillcolor(BLACK);
        solidrectangle(x, y, width, height);  // 清除提示信息
    }

    void cleanMessage(int x, int y, int nHeight, int nWidth, int delay) {
        std::thread t([=]() {
            std::this_thread::sleep_for(std::chrono::seconds(delay));
            cleanDraw(x, y, x + nWidth, y + nHeight);
            });
        t.detach();
    }
};