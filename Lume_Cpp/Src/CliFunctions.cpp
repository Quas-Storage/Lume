#include "CliFunctions.hpp"

// welcome message when you run command without any params or flags
void lumeWelcomeMessage() {
	string lumeVersionString = LUME_VERSION;

	cout << " " << endl;
	cout << "---------LUME---------" << endl;
	cout << "Version " + lumeVersionString << endl;
	cout << "                       " << endl;
	cout << "use \"Help\" for more information" << endl;
	cout << " " << endl;
}

// Displays Help command
void lumeHelp() {
	cout << " " << endl;
	cout << "\x1b[5mLume CLI guide\x1b[0m" << endl;
	cout << "help - provides instructions on the CLI" << endl;
	cout << "version - displays the installed version of Lume" << endl;
	cout << "build path_to_folder_or_file - builds a folder or file to .lmb files" << endl;
	cout << "   " << endl;
	cout << "\x1b[5mflags\x1b[0m" << endl;
	cout << "build --debug - enables the debug mode for the compiler for better profiling" << endl;
}

// Displays the current version of lume
void lumeVersion() {
	string lumeVersionString = LUME_VERSION;
	cout << "Currently running lume version " + lumeVersionString << endl;
}

// runs the lume compiler and executes the code
void runProgram(const char* path, bool debugMode) {
	string stringPath = static_cast<string>(path);

	if (!stringPath.ends_with(".lume")) {
		cout << "Invalid file type. Expected .lume" << endl;
		return;
	};
	replace(stringPath.begin(), stringPath.end(), '/', '\\');

	compilerOptions options;
	options.debug = debugMode;

	compiler compilerInstance = compiler(stringPath.c_str(), &options);
	compilerStatus status = compilerInstance.compile();

	if (status.status) {
		cout << "Execution succefull" << endl;
	}
}