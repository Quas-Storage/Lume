#include "Cli.hpp"

// Checks if a certain arg is contained in the list
bool argExists(std::vector<std::string>* args, const char* pattern) {
	auto itterator = std::find(args->begin(), args->end(), pattern);
	if (itterator != args->end()) {
		return true;
	}

	return false;
}

// check if arg is path
bool argIsPath(const char* pattern) {
	return std::filesystem::exists(pattern);
}

// Proccesses the args and sorts them into flags / commands
void processArgs(char* argv[]) {
	std::vector<std::string> flags;
	std::vector<std::string> commands;
	std::vector<std::string> paths;

	size_t i = 0;
	while (true) {
		i++;
		if (!argv[i]) {
			break;
		}

		const std::string argString = static_cast<std::string>(argv[i]);

		if (argString.starts_with("--")) {
			flags.push_back(argString);
		}
		else {
			if (argIsPath(argString.c_str())) {
				paths.push_back(argString);
			}
			else {
				commands.push_back(argString);
			}
		}
	}

	if (argExists(&flags, "--help")) {
		lumeHelp();
	}
	else if (argExists(&flags, "--version")) {
		lumeVersion();
	}
	else if (paths.size() == 1 && commands.size() == 0) {
		bool debugMode = false;
		if (argExists(&flags, "--debug")) {
			debugMode = true;
		}
		runProgram(paths.at(0).c_str(), debugMode);
	}
	else {
		std::cout << "\x1b[91mInvalid Command\x1b[0m" << std::endl;
	}
}

// runs the actuall CLI
unsigned int runCli(char* argv[]) {
	if (!argv[1]) {
		lumeWelcomeMessage();
	}
	else {
		processArgs(argv);
	}

	return 1;
}