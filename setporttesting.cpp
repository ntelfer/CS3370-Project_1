#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::stringstream;

const string programName = "setport";
const string outputFile = "output.txt";

string readFile(const string& file){
    ifstream resultsFile (file);
    string line;
    string completeOutput = "";
    
    if (resultsFile.is_open()) {
        while ( getline (resultsFile,line) ) {
            completeOutput += line; 
        }
        resultsFile.close();
    }
    return completeOutput;
}

string formatCommand(const string& command){
    string formatedCommand = "\"" + command + "\"";
    int tabsToAdd = 1;
    
    if (formatedCommand.size() <= 9) { 
        tabsToAdd += 2; 
    } else if (formatedCommand.size() <= 15) {
        tabsToAdd += 2;
    } else if (formatedCommand.size() <= 23) {
        tabsToAdd += 1;
    }
    
    for (int i = 0; i < tabsToAdd; ++i) {
        formatedCommand += "\t";
    }
    
    return formatedCommand;
}

void tester(const string& command, int expectedReturnValue, const string& expectedOutput, const string& outputRedirect){
    string pass = "Testing " + formatCommand(command) + "- pass";
    string fail = "Testing " + formatCommand(command) + "- fail";
    
    int returnValue = system((command + outputRedirect).c_str())/256;
    
    if (returnValue != expectedReturnValue) { 
        cout << fail << endl; 
        return;
    }
    
    string portsetterOutput = readFile(outputFile);
    
    if (portsetterOutput != expectedOutput) { 
        cout << fail << endl; 
        return;
    }
    
    cout << pass << endl;
    return;
}

int main(){
    string helpOutput = readFile("portsetter.usage_en.txt");
    string portNumber = "4040";
    string portSetOutput = "Listening on port ";
    string version = "1.3";
    string about = readFile("portsetter.about_en.txt");
    string unknownFlag = "Error: Flag not recognized.";
    string invalidPort = "Error: Port number is invalid.";
    string tooManyArgs = "Error: To many arguments.";
    string invalidPortRange = "Error: Port number is outside acceptable range.";
    string missingPort = "Error: Port number is missing.";
    string default_env_port_num = getenv("PORT");
    string badEnvVar = "Error: Invalid Environment Variable.";
    
    //happy paths
    tester(programName, 0, helpOutput, " > " + outputFile);
    tester(programName + " -h", 0, helpOutput, " > " + outputFile);
    tester(programName + " --help", 0, helpOutput, " > " + outputFile);
    tester(programName + " -p " + portNumber, 0, portSetOutput + portNumber, " > " + outputFile);
    tester(programName + " --port " + portNumber, 0, portSetOutput + portNumber, " > " + outputFile);
    tester(programName + " -p -e", 0, portSetOutput + default_env_port_num, " > " + outputFile);
    tester(programName + " -p -e PORT", 0, portSetOutput + default_env_port_num, " > " + outputFile);
    tester(programName + " --port -e", 0, portSetOutput + default_env_port_num, " > " + outputFile);
    tester(programName + " --port -e PORT", 0, portSetOutput + default_env_port_num, " > " + outputFile);
    tester(programName + " -?", 0, helpOutput, " > " + outputFile);
    tester(programName + " -v", 0, version, " > " + outputFile);
    tester(programName + " --version", 0, version, " > " + outputFile);
    tester(programName + " -!", 0, about, " > " + outputFile);
    tester(programName + " --about", 0, about, " > " + outputFile);
    
    //sad paths
    tester(programName + " help", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " -help", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " --h", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " -h --help", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " -hs", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " -p --port 9", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " -p 77 33", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " -p -21", 1, invalidPort + helpOutput, " > " + outputFile);
    tester(programName + " -p 0", 1, invalidPortRange + helpOutput, " > " + outputFile);
    tester(programName + " --port", 1, missingPort + helpOutput, " > " + outputFile);
    tester(programName + " -p 90642", 1, invalidPortRange + helpOutput, " > " + outputFile);
    tester(programName + " -x 45321", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " -P 714", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " -e", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " -e PORT", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " -e -p", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " -p -e akdflj", 1, badEnvVar + helpOutput, " > " + outputFile);
    tester(programName + " --port -e akdflj", 1, badEnvVar + helpOutput, " > " + outputFile);
    tester(programName + " -e --port", 1, unknownFlag + helpOutput, " > " + outputFile);
    tester(programName + " -p -e 123", 1, badEnvVar + helpOutput, " > " + outputFile);
    tester(programName + " --port -e 123", 1, badEnvVar + helpOutput, " > " + outputFile);
    tester(programName + " -v -h", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " -h -v", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " --version -h", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " --help -v", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " --version --help", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " --help --version", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " --help -!", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " --help --about", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " --version -!", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " --version --about", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " -! --about", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " -v --version", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " -? --help", 1, tooManyArgs + helpOutput, " > " + outputFile);
    tester(programName + " -? -h", 1, tooManyArgs + helpOutput, " > " + outputFile);
    
    return 0;
}