#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <vector>
#include <regex>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::map;
using std::vector;
using std::regex;
using std::smatch;
using std::ssub_match;

#define SHORT_PORT_OPT "-p"
#define LONG_PORT_OPT "--port"
#define SHORT_HELP_OPT "-h"
#define LONG_HELP_OPT "--help"
#define QUES_HELP_OPT "-?"
#define SHORT_ABOUT_OPT "-!"
#define LONG_ABOUT_OPT "--about"
#define SHORT_VERS_OPT "-v"
#define LONG_VERS_OPT "--version"
#define SHORT_ENV_VAR_OPT "-e"
#define LONG_ENV_VAR_OPT "--environment"
#define DEFAULT_ENV_VAR "PORT"
#define VERSION "1.3"
#define USAGE_FILE "portsetter.usage_"
#define ABOUT_FILE "portsetter.about_"


enum msgPos{
    TO_MANY_ARGS, 
    INVALID_PORT, 
    PORT_OUT_OF_RANGE, 
    INVALID_FLAG, 
    MISSING_PORT_NUM, 
    INVALID_ENV_VAR, 
    LISTENING 
};

vector<string> msg;

string readFile(const string& fileName, bool insertIntoVector = false){
    ifstream resultsFile (fileName);
    string line;
    string completeOutput = "";
    
    if (resultsFile.is_open()) {
        while ( getline(resultsFile,line) ) {
            if (insertIntoVector) { 
                msg.push_back(line);
            }
            completeOutput += line + "\n"; 
        }
        resultsFile.close();
    }
    return completeOutput;
}

void displayUsage(const string lang){
    cout << readFile(USAGE_FILE+lang+".txt");
}

bool validNum(const string& portNum){
    if(portNum == ""){
        cout << msg[INVALID_PORT] << endl;
        return false;
    }
    
    if (portNum.find_first_not_of("0123456789") != string::npos) { 
        cout << msg[INVALID_PORT] << endl;
        return false; 
    }
    
    int portNumber = stoi(portNum);
    if (portNumber < 1 || portNumber > 65535) {
        cout << msg[PORT_OUT_OF_RANGE] << endl;
        return false;
    }
    
    return true;
}


bool validInput(int argc, char * args[]){
    map<string,string> flags = {{SHORT_PORT_OPT,""},{LONG_PORT_OPT,""}, {SHORT_HELP_OPT,""}, {LONG_HELP_OPT,""}, {QUES_HELP_OPT,""}, {SHORT_ABOUT_OPT,""}, {LONG_ABOUT_OPT,""}, {SHORT_VERS_OPT,""}, {LONG_VERS_OPT,""}, {SHORT_ENV_VAR_OPT,""}, {LONG_ENV_VAR_OPT,""}};
    
    if (argc <= 1) { return true; }
    
    if (argc > 4) { 
        cout << msg[TO_MANY_ARGS] << endl;
        return false; 
    }
    
    string flag = args[1];
    
    if (flags.count(flag) == 0 || flag == SHORT_ENV_VAR_OPT || flag == LONG_ENV_VAR_OPT) {
        cout << msg[INVALID_FLAG] << endl;
        return false; 
    }
    
    if (flag == SHORT_HELP_OPT || flag == LONG_HELP_OPT || flag == QUES_HELP_OPT || flag == SHORT_ABOUT_OPT || flag == LONG_ABOUT_OPT || flag == SHORT_VERS_OPT || flag == LONG_VERS_OPT) {
        if (argc > 2) { 
            cout << msg[TO_MANY_ARGS] << endl; 
            return false;
        }
        return true;
    }
    
    if (argc < 3) { 
        cout << msg[MISSING_PORT_NUM] << endl;
        return false; 
    }
    
    string port = args[2];
    string env_var = DEFAULT_ENV_VAR;
    string portNumber = "";
    
    if(argc == 4){
            env_var = args[3];
    }
    
    char * cStrPortNumber = getenv(env_var.c_str());
    
    if (port == SHORT_ENV_VAR_OPT || port == LONG_ENV_VAR_OPT) {
        if(cStrPortNumber == nullptr){
            cout << msg[INVALID_ENV_VAR] << endl;
            return false;
        }
        
        portNumber = cStrPortNumber;
        
        if (!validNum(portNumber)) { return false; }
        
        return true;
    }
    
    if (argc > 3) {
        cout << msg[TO_MANY_ARGS] << endl;
        return false;
    }
    
    if (!validNum(port)) { return false; }
    
    return true;
}


string getLang(){
    string env_var[4] = {"LANGUAGE", "LC_ALL", "LC_MESSAGES", "LANG"}; 
    string langErrBegin = "Bad language specification in environment variable ";
    string langErrEnd = ". Using English.";
    string langString = "";
    int i;
    for (i = 0; i < 4; ++i) {
        char * cStrLang = getenv(env_var[i].c_str());
        
        if (cStrLang == nullptr) { continue; }
        
        langString = cStrLang;
        
        if (langString == "null") { continue; }
        
        if (langString == "") { continue; }
        
        if (langString == "C" || langString == "c") { continue; }
        
        if (langString == "C.UTF-8" || langString == "c.utf-8") { continue; }
        
        if (langString.find_first_of("._",0) > 2 ) { continue; }
        
        break;
    }
   
    
    if(langString.size() < 2 || langString.substr(0,2) == "C."){
        return "en";
    }
    
    if (langString.find_first_of("._",0) > 2 || langString.find_first_of("._",0) == string::npos) {
        cout << langErrBegin << env_var[3] << "=" << langString << langErrEnd << endl;
        return "en";
    }
    
    string language = langString.substr(0,2);
    if (language == "en") {
        return language; 
    }
    
    if (language == "es") {
        return language;
    }
    
    cout << "Missing" << language << " translation files. Using English." << endl;
    
    return "en";
}


// string getLang(const string& langString){
//     try{
//     //regex language("([a-z]{2})(.*)", std::regex_constants::extended);
//     regex language("(.{2})_", std::regex_constants::extended);
//     smatch matches;
//     //if(regex_search(langString.begin(), langString.end(), matches, language)){
//     if(regex_search(langString, matches, language)){
//         cout << endl << matches[0] << endl << endl;
//     }
//     }catch(std::regex_error& e){
//         cout << e.code() << endl;
//     }
//     return "";
// }


int main(int argc, char * args[]) {
    string lang = getLang();
    readFile("portsetter.cmdoutput_" + lang + ".txt", true);
    
    if (!validInput(argc, args)) { 
        displayUsage(lang); 
        return 1; 
    }
    
    if (argc == 1) { 
        displayUsage(lang); 
        return 0;
    }
    
    string flag = args[1];
    
    if (flag == SHORT_HELP_OPT || flag == LONG_HELP_OPT || flag == QUES_HELP_OPT) { 
        displayUsage(lang); 
        return 0;
    }
    
    if(flag == SHORT_VERS_OPT || flag == LONG_VERS_OPT){
        cout << VERSION << endl;
        return 0;
    }
    
    if(flag == SHORT_ABOUT_OPT || flag == LONG_ABOUT_OPT){
        cout << readFile(ABOUT_FILE+lang+".txt") << endl;
        return 0;
    }
    
    string port = args[2];
    if (flag == SHORT_PORT_OPT || flag == LONG_PORT_OPT) { 
        if (argc == 4) { 
            port = getenv(args[3]) ;
        }else if (port == SHORT_ENV_VAR_OPT || port == LONG_ENV_VAR_OPT) {
            port = getenv(DEFAULT_ENV_VAR);
        }
        cout << msg[LISTENING] << port << endl;
    }
    
    
    return 0;
}