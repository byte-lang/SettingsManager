#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
namespace SettingsManager
{
    struct Data {
        private:
            std::vector<std::string> split(std::string s,std::string delimiter) { std::vector<std::string> res;s+=delimiter;std::string word;int pos = s.find(delimiter);while (pos != std::string::npos) {    word = s.substr(0, pos);  res.push_back(word);   s.erase(0, pos + delimiter.length()); pos = s.find(delimiter);  }   res.push_back(s);  res.pop_back();return res;}

        public:
        std::vector<std::string> value;
        std::string operator[](std::string index) { 
            for(int i = 0; i < value.size(); i ++)
            {
                std::vector<std::string> data = split(value[i], "\n");
                if(data[0] == index) return data[1];
            }
            throw std::runtime_error("Settings::Error}> Name '" + index + "' not found.");
        }
        bool operator==(Data data)
            {
                int size = data.size();
                if(size != value.size()) return false;
                for(int i = 0; i < value.size(); i++)
                {    
                    std::cout << data[i] + ":" + value[i] << std::endl;
                    if(data[i] != value[i]) 
                    {
                        return false;
                    }
                }
                return true;
            }
        bool operator!=(Data data)
            {int size = data.size(); if(size != value.size()) return true;for(int i = 0; i < value.size(); i++){    if(data[i] != value[i]) return true;}return false;}
        bool operator<(int index)
            {if(value.size() < index) return true; else return false;}
        bool operator>(int index)
            {if(value.size() > index) return true; else return false;}
        bool operator<=(int index)
            {if(value.size() <= index) return true; else return false;}
        bool operator>=(int index)
            {if(value.size() >= index) return true; else return false;}
        bool operator<(Data data)
            {if(value.size() < data.size()) return true; else return false;}
        bool operator>(Data data)
            {if(value.size() > data.size()) return true; else return false;}
        bool operator<=(Data data)
            {if(value.size() <= data.size()) return true; else return false;}
        bool operator>=(Data data)
            {if(value.size() >= data.size()) return true; else return false;}
        std::string operator[](size_t index) 
        {
            if(index >= value.size()) throw std::runtime_error("Settings::Error}> Index is out of bounds.");
            return value[index].substr(value[index].find('\n') + 1);
        }
        void operator<<(std::string index) { value.push_back(index); }
        void print_details() {
            std::cout << "----------------------------------------------------------------" << std::endl;
            for(int i = 0; i < value.size(); i++)
            std::cout << value[i] << "\n----------------------------------------------------------------"<< std::endl;
        }
        Data(std::vector<std::string>);
        Data() : value() {}
        void clear() { value.clear(); }
        size_t size() { return value.size(); }
    };
    Data::Data(std::vector<std::string> x) : value(x) {}
    struct Parse {
        private:
            std::string file;
            std::vector<std::string> split(std::string s,std::string delimiter) { std::vector<std::string> res;s+=delimiter;std::string word;int pos = s.find(delimiter);while (pos != std::string::npos) {    word = s.substr(0, pos);  res.push_back(word);   s.erase(0, pos + delimiter.length()); pos = s.find(delimiter);  }   res.push_back(s);  res.pop_back();return res;}
        public:
            void Open(std::string filename)
            {
                std::fstream get(filename);
                if(!get.is_open()) throw std::runtime_error("Settings::Error}> File cannot be open.");
                else
                {
                    file = filename;
                }
            }
            Data Read()
            {
                if(file.empty()) throw std::runtime_error("Settings::Error}> No file was opened. Open a file using Open() function.");
                SettingsManager::Data res;
                std::ifstream filef(file);
                std::string str((std::istreambuf_iterator<char>(filef)), std::istreambuf_iterator<char>());
                std::vector<std::string> data = split(str, "\n");
                for(int i = 0; i < data.size(); i++) 
                {
                    std::string value = data[i];
                    if(value.find_first_not_of(" \t\r\n") == std::string::npos) continue;
                    else
                    {
                        int rawvalue = value.find_first_not_of(" \t\r\n");
                        value = value.substr(rawvalue);
                        if(value[0] == '$') continue;
                        int valuepos = value.find("=");
                        if(valuepos == 0)
                        {
                            std::string line = std::to_string(i + 1);
                            throw std::runtime_error("Settings::Error}> On line " + line + ", Name expected.");
                        }
                        if(valuepos == std::string::npos)
                        { 
                            std::string line = std::to_string(i + 1);
                            throw std::runtime_error("Settings::Error}> On line " + line + ", Value expected.");
                        }
                        std::string tempvalue = value.substr(0, valuepos);
                        tempvalue.erase(std::remove_if(tempvalue.begin(), tempvalue.end(), isspace), tempvalue.end());
                        std::string tempvaluesub = value.substr(valuepos + 1);
                        size_t valuestr = tempvaluesub.find_first_not_of(" \n\r\t");
                        std::string tores = tempvalue + "\n" + value.substr(valuepos + valuestr + 1);
                        res << tores;
                    }
                }
                return res;
            }
            void Create(std::string name, std::string default_value)
            {
                if(file.empty()) throw std::runtime_error("Settings::Error}> No file was opened. Open a file using Open() function.");
                std::ofstream filewrite(file, std::ios::app);
                filewrite << "\n" + name + "=" + default_value;
            }
            void Remove(std::string name)
            {
                if(file.empty()) throw std::runtime_error("Settings::Error}> No file was opened. Open a file using Open() function.");
                std::ifstream filef(file);
                std::string str((std::istreambuf_iterator<char>(filef)), std::istreambuf_iterator<char>());
                std::vector<std::string> data = split(str, "\n");
                std::string res;
                bool found = false;
                for(int i = 0; i < data.size(); i++) 
                {
                    std::string value = data[i];
                    int rawvalue = value.find_first_not_of(" \t\r\n");
                    value = value.substr(rawvalue);
                    if(value[0] == '$') { res.append(value + "\n"); continue;}
                    int valuepos = value.find("=");
                    if(valuepos == 0)
                    {
                        std::string line = std::to_string(i + 1);
                        throw std::runtime_error("Settings::Error}> On line " + line + ", Name expected.");
                    }
                    if(valuepos == std::string::npos)
                    { 
                        std::string line = std::to_string(i + 1);
                        throw std::runtime_error("Settings::Error}> On line " + line + ", Value expected.");
                    }
                    std::string tempvalue = value.substr(0, valuepos);
                    tempvalue.erase(std::remove_if(tempvalue.begin(), tempvalue.end(), isspace), tempvalue.end());
                    if(tempvalue == name)
                        found = true;
                    else res.append(value + "\n");
                }
                res.pop_back();
                if(!found) throw std::runtime_error("Settings::Error}> Name '" + name + "' was not found.");
                std::ofstream filewrite(file);
                filewrite << res;
            }
            void AddComment(std::string value, int line = 0) {
                if(line == 0) {
                    std::ofstream filewrite(file, std::ios::app);
                    filewrite << "\n$" + value;
                }
                else
                {
                    std::ifstream filef(file);
                    std::string str((std::istreambuf_iterator<char>(filef)), std::istreambuf_iterator<char>());
                    std::vector<std::string> data = split(str, "\n");
                    std::string res;
                    bool added = false;
                    for(int i = 0; i < data.size(); i++) {
                        if(i != line || !added)
                        {
                            if(i <= 0)
                                res.append(data[i]);
                            else
                                res.append("\n" + data[i]);
                        }
                        else
                        {
                            if(!added)
                            {
                                res.append("\n$" + value);
                                added = true;
                                i--;
                            }
                        }
                    }
                    std::ofstream filewrite(file);
                    filewrite << res;
                }
            }
            std::string GetName(std::string valuearg)
            {
                if(file.empty()) throw std::runtime_error("Settings::Error}> No file was opened. Open a file using Open() function.");
                std::ifstream filef(file);
                std::string str((std::istreambuf_iterator<char>(filef)), std::istreambuf_iterator<char>());
                std::vector<std::string> data = split(str, "\n");
                std::string res;
                bool found = false;
                for(int i = 0; i < data.size(); i++) 
                {
                    std::string value = data[i];
                    int rawvalue = value.find_first_not_of(" \t\r\n");
                    value = value.substr(rawvalue);
                    if(value[0] == '$') { res.append(value + "\n"); continue;}
                    int valuepos = value.find("=");
                    if(valuepos == 0)
                    {
                        std::string line = std::to_string(i + 1);
                        throw std::runtime_error("Settings::Error}> On line " + line + ", Name expected.");
                    }
                    if(valuepos == std::string::npos)
                    { 
                        std::string line = std::to_string(i + 1);
                        throw std::runtime_error("Settings::Error}> On line " + line + ", Value expected.");
                    }
                    std::string tempvaluesub = value.substr(valuepos + 1);
                    size_t valuestr = tempvaluesub.find_first_not_of(" \n\r\t");
                    if(value.substr(valuepos + valuestr + 1) == valuearg)
                    {
                        return value.substr(0, valuepos);
                    }

                }
                throw std::runtime_error("Settings::Error}> Value '" + valuearg + "' was not found.");
            }
            void ChangeName(std::string name, std::string newname)
            {
                if(file.empty()) throw std::runtime_error("Settings::Error}> No file was opened. Open a file using Open() function.");
                std::ifstream filef(file);
                std::string str((std::istreambuf_iterator<char>(filef)), std::istreambuf_iterator<char>());
                std::vector<std::string> data = split(str, "\n");
                std::string res;
                bool found = false;
                for(int i = 0; i < data.size(); i++) 
                {
                    std::string value = data[i];
                    int rawvalue = value.find_first_not_of(" \t\r\n");
                    value = value.substr(rawvalue);
                    if(value[0] == '$') { res.append(value + "\n"); continue;}
                    int valuepos = value.find("=");
                    if(valuepos == 0)
                    {
                        std::string line = std::to_string(i + 1);
                        throw std::runtime_error("Settings::Error}> On line " + line + ", Name expected.");
                    }
                    if(valuepos == std::string::npos)
                    { 
                        std::string line = std::to_string(i + 1);
                        throw std::runtime_error("Settings::Error}> On line " + line + ", Value expected.");
                    }
                    std::string tempvalue = value.substr(0, valuepos);
                    tempvalue.erase(std::remove_if(tempvalue.begin(), tempvalue.end(), isspace), tempvalue.end());
                    if(tempvalue == name)
                    {
                        found = true;
                        std::string temp = newname + "=" + value.substr(valuepos + 1);
                        value = temp;
                    }
                    res.append(value + "\n");
                }
                res.pop_back();
                if(!found) throw std::runtime_error("Settings::Error}> Name '" + name + "' was not found.");
                std::ofstream filewrite(file);
                filewrite << res;
            }
            void Update(std::string name, std::string valuechange)
            {
                if(file.empty()) throw std::runtime_error("Settings::Error}> No file was opened. Open a file using Open() function.");
                std::ifstream filef(file);
                std::string str((std::istreambuf_iterator<char>(filef)), std::istreambuf_iterator<char>());
                std::vector<std::string> data = split(str, "\n");
                std::string res;
                bool found = false;
                for(int i = 0; i < data.size(); i++) 
                {
                    std::string value = data[i];
                    int rawvalue = value.find_first_not_of(" \t\r\n");
                    value = value.substr(rawvalue);
                    if(value[0] == '$') { res.append(value + "\n"); continue;}
                    int valuepos = value.find("=");
                    if(valuepos == 0)
                    {
                        std::string line = std::to_string(i + 1);
                        throw std::runtime_error("Settings::Error}> On line " + line + ", Name expected.");
                    }
                    if(valuepos == std::string::npos)
                    { 
                        std::string line = std::to_string(i + 1);
                        throw std::runtime_error("Settings::Error}> On line " + line + ", Value expected.");
                    }
                    if(value.substr(0, valuepos) == name)
                    {
                        found = true;
                        std::string tempvalue = value.substr(0, valuepos);
                        tempvalue.erase(std::remove_if(tempvalue.begin(), tempvalue.end(), isspace), tempvalue.end());
                        std::string temp = tempvalue + "=" + valuechange;
                        value = temp;
                    }
                    res.append(value + "\n");
                }
                res.pop_back();
                if(!found) throw std::runtime_error("Settings::Error}> Name '" + name + "' was not found.");
                std::ofstream filewrite(file);
                filewrite << res;
            }
    };
}
#endif