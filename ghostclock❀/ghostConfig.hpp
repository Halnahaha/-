
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

    string defaultConfig = R"d(# 删除本文件可重新生成默认配置
[window]
    # 在下一行等号后面填写横向窗口位置
    x = 10

    # 在下一行等号后面填写纵向窗口位置
    y = 10

[key]
    # 在下一行等号后面填写激活按键的ASCII码
    toggleKeyCode = 84

    # 在下一行等号后面填写重置按键的ASCII码
    resetKeyCode = 89
)d";

    int64_t configWindowX;
    int64_t configWindowY;
    int64_t configToggleKeyCode;
    int64_t configResetKeyCode;

    void init() {

        if (not ini.Load(configFilePath)) {
            // 未找到文件时重新生成默认配置文件
            resetConfigFile();

            if (not ini.Load(configFilePath)) {
                throw "无法读取默认config.ini文件";
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
            throw "无法写入config.ini文件";
        }

        int wDefaultConfigSize = MultiByteToWideChar(CP_ACP, 0, defaultConfig.c_str(), -1, NULL, 0);
        wchar_t* wDefaultConfig = new wchar_t[wDefaultConfigSize];
        MultiByteToWideChar(CP_ACP, 0, defaultConfig.c_str(), -1, wDefaultConfig, wDefaultConfigSize);

        writeDefaultConfig.imbue(loc);
        writeDefaultConfig << wDefaultConfig << endl;

        setlocale(LC_ALL, curLocale.c_str());

    }
};