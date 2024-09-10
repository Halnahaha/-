#include <ctime>
#include <graphics.h>
#include <thread>
#include <Windows.h>
#include "timer.hpp"
using namespace std;

class Msg
{
public:
    const TCHAR* lastMessage; // ��Ϣ����
    time_t lastSendTime = 0; // ����ʱ�� ��10λUNIXʱ�����
    int lastX;
    int lastY;
    int lastHeight;
    int lastWidth;
    Timer* lastMessageTimer;

    void showTestMessage()
    {
        setbkcolor(BLACK);
        settextstyle(40, 0, _T("Consolas"));
        settextcolor(LIGHTCYAN);
        outtextxy(0, 0, _T("TEST"));
    }

    void showMessage(const TCHAR* message, int x, int y, int nHeight, int nWidth)
    {
        if (lastMessageTimer == NULL) lastMessageTimer = new Timer;

        if (nWidth == 0) nWidth = 360;

        // ������Ϣ����ʱ���پ���Ϣ
        time_t now = time(0);
        // ��� �ϴη���ʱ�� �Ƿ�>1s
        if ((lastSendTime > 0) && (lastSendTime + 1 < now)) {
            cleanDraw(lastX, lastY, lastX + lastWidth, lastY + lastHeight);
            // ��鶨ʱ�� ����ж�ʱ����ֹͣ��ʱ�������������ϴ���Ϣ
            if (lastMessageTimer->status == lastMessageTimer->TIMER_START) {
                lastMessageTimer->stop();
                // TODO ��Ȼ��֪��ʲôBUG����������newһ��Timer֮��ͽ���˸���ֵ�����
                lastMessageTimer = new Timer;
            }
        }

        setbkcolor(BLACK);
        settextstyle(nHeight, 0, _T("Consolas"));
        settextcolor(LIGHTCYAN);
        outtextxy(x, y, message);
        // ���ö�ʱ�� 1s ������
        lastMessageTimer->start(1, [=]() {
            cleanDraw(x, y, x + nWidth, y + nHeight);
            });

        lastMessage = message;
        lastSendTime = time(0);
        lastX = x;
        lastY = y;
        lastHeight = nHeight;
        lastWidth = nWidth;

    }

    void drawTimer(int x, int y, int minutes, int seconds, int size) {
        settextstyle(size, 0, _T("Consolas"));
        setbkcolor(BLACK);
        settextcolor(LIGHTCYAN);
        _TCHAR str[16];
        _stprintf_s(str, _T("%02d:%02d"), minutes, seconds);
        outtextxy(x, y, str);
    }

    void cleanDraw(int x, int y, int width, int height) {
        setfillcolor(BLACK);
        solidrectangle(x, y, width, height);  // �����ʾ��Ϣ
    }

    void cleanMessage(int x, int y, int nHeight, int nWidth, int delay) {
        std::thread t([=]() {
            std::this_thread::sleep_for(std::chrono::seconds(delay));
            cleanDraw(x, y, x + nWidth, y + nHeight);
            });
        t.detach();
    }
};