
#include <fstream>
#include <locale.h>
#include <string>
#include <Windows.h>
#include "config.hpp"

using namespace std;

class GhostConfig
{


public:
    Config::Ini ini;
    string configFilePath = "./config.ini";

    string defaultConfig = R"d(# ɾ�����ļ�����������Ĭ������
[window]
    # ����һ�еȺź�����д���򴰿�λ��
    x = 10

    # ����һ�еȺź�����д���򴰿�λ��
    y = 10

[key]
    # ����һ�еȺź�����д�������ASCII��
    toggleKeyCode = 84

    # ����һ�еȺź�����д���ð�����ASCII��
    resetKeyCode = 89
)d";

    int64_t configWindowX;
    int64_t configWindowY;
    int64_t configToggleKeyCode;
    int64_t configResetKeyCode;

    void init() {

        if (not ini.Load(configFilePath)) {
            // δ�ҵ��ļ�ʱ��������Ĭ�������ļ�
            resetConfigFile();

            if (not ini.Load(configFilePath)) {
                throw "�޷���ȡĬ��config.ini�ļ�";
            }

        }

        ini.GetIntValue("window", "x", configWindowX, 10);
        ini.GetIntValue("window", "y", configWindowY, 10);
        ini.GetIntValue("key", "toggleKeyCode", configToggleKeyCode, 84);
        ini.GetIntValue("key", "resetKeyCode", configResetKeyCode, 89);

    }

    void resetConfigFile() {

        string curLocale = setlocale(LC_ALL, NULL);
        setlocale(LC_ALL, "chs");

        locale loc("zh_CN.UTF-8");
        locale::global(loc);

        wofstream writeDefaultConfig(configFilePath);

        if (!writeDefaultConfig) {
            throw "�޷�д��config.ini�ļ�";
        }

        int wDefaultConfigSize = MultiByteToWideChar(CP_ACP, 0, defaultConfig.c_str(), -1, NULL, 0);
        wchar_t* wDefaultConfig = new wchar_t[wDefaultConfigSize];
        MultiByteToWideChar(CP_ACP, 0, defaultConfig.c_str(), -1, wDefaultConfig, wDefaultConfigSize);

        writeDefaultConfig.imbue(loc);
        writeDefaultConfig << wDefaultConfig << endl;

        setlocale(LC_ALL, curLocale.c_str());

    }
};