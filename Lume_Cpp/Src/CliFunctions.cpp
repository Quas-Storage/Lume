#include "CliFunctions.hpp"

// welcome message when you run command without any params or flags
void lumeWelcomeMessage() {
	std::string lumeVersionString = LUME_VERSION;

	std::cout << " " << std::endl;
	std::cout << "---------LUME---------" << std::endl;
	std::cout << "Version " + lumeVersionString << std::endl;
	std::cout << "                       " << std::endl;
	std::cout << "use \"Help\" for more information" << std::endl;
	std::cout << " " << std::endl;
}

// Displays Help command
void lumeHelp() {
	std::cout << " " << std::endl;
	std::cout << "\x1b[5mLume CLI guide\x1b[0m" << std::endl;
	std::cout << "help - provides instructions on the CLI" << std::endl;
	std::cout << "version - displays the installed version of Lume" << std::endl;
	std::cout << "build path_to_folder_or_file - builds a folder or file to .lmb files" << std::endl;
	std::cout << "   " << std::endl;
	std::cout << "\x1b[5mflags\x1b[0m" << std::endl;
	std::cout << "build --debug - enables the debug mode for the compiler for better profiling" << std::endl;
}

// Displays the current version of lume
void lumeVersion() {
	std::string lumeVersionString = LUME_VERSION;
	std::cout << "Currently running lume version " + lumeVersionString << std::endl;
}

// runs the lume compiler and executes the code
void runProgram(const char* path, bool debugMode) {
	std::string stringPath = static_cast<std::string>(path);

	if (!stringPath.ends_with(".lume")) {
		std::cout << "Invalid file type. Expected .lume" << std::endl;
		return;
	};
	std::replace(stringPath.begin(), stringPath.end(), '/', '\\');

	compilerOptions options;
	options.debug = debugMode;

	compiler compilerInstance = compiler(stringPath.c_str(), &options);
	compilerStatus status = compilerInstance.compile();

	if (status.status) {
		std::cout << "Execution succefull" << std::endl;
	}
}