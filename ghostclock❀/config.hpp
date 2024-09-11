#pragma once
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>
namespace Config {
    class Ini {
    public:
        void Dump(std::function<void(const std::string&, const std::string&, const std::string&)> deal) {
            auto iter = cfg_.begin();
            while (iter != cfg_.end()) {
                auto kv_iter = iter->second.begin();
                while (kv_iter != iter->second.end()) {
                    deal(iter->first, kv_iter->first, kv_iter->second);
                    ++kv_iter;
                }
                ++iter;
            }
        }
        bool Load(std::string file_name) {
            if (file_name == "") return false;
            std::ifstream in;
            std::string line;
            in.open(file_name.c_str());
            if (not in.is_open()) return false;
            while (getline(in, line)) {
                std::string section, key, value;
                if (not parseLine(line, section, key, value)) {
                    continue;
                }
                set(section, key, value);
            }
            return true;
        }
        void GetStrValue(const std::string& section, const std::string& key, std::string& value, std::string default_value) {
            value = default_value;
            if (cfg_.find(section) == cfg_.end()) {
                return;
            }
            if (cfg_[section].find(key) == cfg_[section].end()) {
                return;
            }
            value = cfg_[section][key];
        }
        void GetInt32Value(const std::string& section, const std::string& key, int32_t& value, int32_t default_value) {
            value = default_value;
            if (cfg_.find(section) == cfg_.end()) {
                return;
            }
            if (cfg_[section].find(key) == cfg_[section].end()) {
                return;
            }
            value = atol(cfg_[section][key].c_str());
        }
        void GetIntValue(const std::string& section, const std::string& key, int64_t& value, int64_t default_value) {
            value = default_value;
            if (cfg_.find(section) == cfg_.end()) {
                return;
            }
            if (cfg_[section].find(key) == cfg_[section].end()) {
                return;
            }
            value = atol(cfg_[section][key].c_str());
        }
        void GetFloatValue(const std::string& section, const std::string& key, float_t& value, float_t default_value) {
            value = default_value;
            if (cfg_.find(section) == cfg_.end()) {
                return;
            }
            if (cfg_[section].find(key) == cfg_[section].end()) {
                return;
            }
            value = std::stof(cfg_[section][key].c_str());
        }
        void set(std::string& section, std::string& key, std::string& value) {
            if (cfg_.find(section) == cfg_.end()) {
                std::unordered_map<std::string, std::string> kv_map;
                cfg_[section] = kv_map;
            }
            if (key != "" && value != "") cfg_[section][key] = value;
        }
    private:
        void ltrim(std::string& str) {
            if (str.empty()) return;
            size_t len = 0;
            char* temp = (char*)str.c_str();
            while (*temp && isblank(*temp)) {
                ++len;
                ++temp;
            }
            if (len > 0) str.erase(0, len);
        }
        void rtrim(std::string& str) {
            if (str.empty()) return;
            size_t len = str.length();
            size_t pos = len;
            while (pos > 0) {
                if (not isblank(str[pos - 1])) {
                    break;
                }
                --pos;
            }
            if (pos != len) str.erase(pos);
        }
        void trim(std::string& str) {
            ltrim(str);
            rtrim(str);
        }
        bool parseLine(std::string& line, std::string& section, std::string& key, std::string& value) {
            static std::string cur_section = "";
            std::string nodes[2] = { "#", ";" };  //去掉注释的内容
            for (int i = 0; i < 2; ++i) {
                std::string::size_type pos = line.find(nodes[i]);
                if (pos != std::string::npos) line.erase(pos);
            }
            trim(line);
            if (line == "") return false;
            if (line[0] == '[' && line[line.size() - 1] == ']') {
                section = line.substr(1, line.size() - 2);
                trim(section);
                cur_section = section;
                return false;
            }
            if (cur_section == "") return false;
            bool is_key = true;
            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i] == '=') {
                    is_key = false;
                    continue;
                }
                if (is_key) {
                    key += line[i];
                }
                else {
                    value += line[i];
                }
            }
            section = cur_section;
            trim(key);
            trim(value);
            return true;
        }
    private:
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> cfg_;
    };  // ini格式配置文件的读取
}  // namespace Config