#include <graphics.h>
#include <ctime>
#include <Windows.h>
#include <tchar.h>

void drawTimer(int x, int y, int minutes, int seconds) {
    settextstyle(100, 0, _T("Consolas"));
    setbkcolor(BLACK);  // ���ñ�����ɫΪ��ɫ
    settextcolor(LIGHTCYAN);  // ����������ɫΪ��ɫ
    RECT rect = { x, y, x + 200, y + 100 };  // ��ʱ����ʾ����
    _TCHAR str[16];
    _stprintf_s(str, _T("%02d:%02d"), minutes, seconds);
    outtextxy(x, y, str);  // ʹ���µ������С����ʽ��ʾ����ʱ
}

void showMessage(const TCHAR* message, int x, int y, int size) {
    settextstyle(size, 0, _T("Consolas"));
    outtextxy(x, y, message);  // �ڴ�������ʾ��ʾ��Ϣ
    Sleep(1000);
    setfillcolor(BLACK);
    solidrectangle(x, y, x + 250, y + 40);  // �����ʾ��Ϣ
}

int main() {
    initgraph(400, 300);  // ���ڴ�С400x300

    bool activated = false; // �Ƿ񼤻�
    bool tildeKeyPressed = false; // �Ƿ���Ʈ�ż�
    bool rightButtonPressed = false; // �Ƿ����Ҽ�
    bool altKeyPressed = false; // �Ƿ���Alt��
    bool timerStarted = false; // �Ƿ��Ѿ���ʼ����ʱ
    int totalTimeInSeconds = 180;  // ������1

    int initialSeconds = totalTimeInSeconds % 60;
    int initialMinutes = totalTimeInSeconds / 60;
    int seconds = initialSeconds;
    int minutes = initialMinutes;
    clock_t startTime = 0;

    while (!GetAsyncKeyState(0x50)) {  // ������P���˳�
        if (GetAsyncKeyState(VK_OEM_3) & 0x8000) {  // ���Ʈ�ż�����
            if (!tildeKeyPressed) {
                activated = !activated;
                if (activated) {
                    showMessage(_T("�Ѽ���"), 100, 150, 40);  // ��ʾ������ʾ��Ϣ
                }
                else {
                    showMessage(_T("δ����"), 100, 150, 40);  // ��ʾδ������ʾ��Ϣ
                }
                tildeKeyPressed = true;
            }
        }
        else {
            tildeKeyPressed = false;
        }

        if (activated) {
            if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {  // ����Ҽ�����
                if (!timerStarted) {
                    seconds = initialSeconds;
                    minutes = initialMinutes;
                    startTime = clock();  // ��ʼ��ʱ
                    timerStarted = true;
                }
            }

            if (GetAsyncKeyState(0x52) & 0x8000) {  // ���R������
                if (timerStarted) {
                    timerStarted = false;
                    showMessage(_T("��ʱ������"), 100, 180, 40);  // ��ʾ������ʾ��Ϣ
                }
            }

            if (timerStarted) {
                clock_t currentTime = clock();
                int elapsedSeconds = (int)(((currentTime - startTime) / (double)CLOCKS_PER_SEC));
                int remainingSeconds = totalTimeInSeconds - elapsedSeconds;
                if (remainingSeconds > 0) {
                    minutes = remainingSeconds / 60;
                    seconds = remainingSeconds % 60;
                    drawTimer(100, 50, minutes, seconds);  // ���µ���ʱ��ʾ
                }
                else {
                    timerStarted = false;
                    showMessage(_T("ʱ�䵽"), 100, 180, 40);  // ��ʾʱ�䵽��ʾ��Ϣ
                }
            }
        }
    }

    closegraph();
    return 0;
}