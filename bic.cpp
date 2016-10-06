#include <cstdio>

#include <unistd.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(_CYGWIN32_) || defined(_WINDOWS_)
#include "windows.h"
#endif

#include "spps.h"
#include "bic.h"
#include "util.h"
#include "scr.h"

using namespace spps::builtin;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

#define EXECUTOR(a) void Default##a::execute(string full, vector<string> split)
#define HELP(a,b) string Default##a::get_help() { return b; }

EXECUTOR(Exit)
{
	SPPSSys::quit(0);
}

HELP(Exit, "Quits SPPS")

// DefaultHelp

EXECUTOR(Help)
{
	if(split.size() < 2)
	{
		vector<string> all_commands;
		ProviderManager::get_all_commands(all_commands);

		if(all_commands.size() > 0)
		{
			for(auto s : all_commands)
			{
				cout << s << " - " << ProviderManager::get_help(s) << endl;
			}
		}
		else cout << "No commands registered" << endl;
	}
	else cout << ProviderManager::get_help(split[1]) << endl;
}

HELP(Help, "Prints command help")

// DefaultEcho

EXECUTOR(Echo)
{
	if(split.size() < 2)
	{
		cout << endl;
		return;
	}

	cout << Util::trim_left(full.substr(4, full.length() - 4)) << endl;
}

HELP(Echo, "Echoes the provided string back to stdout")

// DefaultSPPS

EXECUTOR(SPPS)
{
	if(split.size() > 1 && split[1][0] == '/' && split[1].length() > 1)
	{
		switch(split[1][1])
		{
			case 'v':
				cout << spps::VERSION << endl;
				break;
			case 'n':
				cout << "SPPS" << endl;
				break;
			case 'e':
				cout << SPPSSys::errorlevel << endl;
				break;
			default:
				cout << "SPPS " << spps::VERSION << endl;
		}
	}
	else cout << "SPPS " << spps::VERSION << endl;
}

HELP(SPPS, "Prints the SPPS version")

// DefaultSystem

EXECUTOR(System)
{
	int exit = system(Util::trim_left(full.substr(6, full.length() - 6)).c_str());
	if(exit)
	{
		SPPSSys::set_errorlevel(exit < 0 ? -exit : exit);
		cout << "Process exited with non-zero exit code " << exit << endl;
	}
}

HELP(System, "Executes a command on the system shell")

// DefaultPause

EXECUTOR(Pause)
{
	cout << "Press enter to continue...";

	cin.get();
	cin.ignore(1);
}

HELP(Pause, "Prompts the user to press enter")

// DefaultSleep

EXECUTOR(Sleep)
{
	if(split.size() < 2)
	{
		cout << "Usage:\n\tsleep <time>" << endl;
		return;
	}

	try
	{
		sleep(std::stoi(split[1]));
	}
	catch(std::invalid_argument &e)
	{
		cerr << "Invalid integer" << endl;
	}
	catch(std::out_of_range &e)
	{
		cerr << "Out of range" << endl;
	}
}

HELP(Sleep, "Waits for the specified number of seconds")

// DefaultPWD

EXECUTOR(PWD)
{
	char *cwd = new char[513];

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(_CYGWIN32_) || defined(_WINDOWS_)
	GetCurrentDirectory(513, cwd);
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
	getcwd(cwd, 513);
#endif

	cout << cwd << endl;

	delete cwd;
}

HELP(PWD, "Prints the current working directory")

// DefaultCD

EXECUTOR(CD)
{
	if(split.size() < 2)
	{
		cout << "Usage:\n\tcd <dir>" << endl;
		return;
	}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(_CYGWIN32_) || defined(_WINDOWS_)
	SetCurrentDirectory(Util::trim_left(full.substr(2, full.length() - 2)).c_str());
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
	chdir(Util::trim_left(full.substr(2, full.length() - 2)).c_str());
#endif
}

HELP(CD, "Changes the current working directory")

// DefaultScript

EXECUTOR(Script)
{
	if(split.size() < 2)
	{
		cout << "Usage:\n\tscript <script>" << endl;
		return;
	}

	ScriptHandler::execute(split[1]);
}

HELP(Script, "Executes a script")

#undef EXECUTOR
#undef HELP

#define REGISTER(a,b) Default##a *d##b = new Default##a; \
	DefaultCommandProvider::register_command(#b, d##b); \
	Util::add_destroyable(d##b)

void spps::builtin::init()
{
	REGISTER(Exit, exit);
	REGISTER(Help, help);
	REGISTER(Echo, echo);
	REGISTER(SPPS, spps);
	REGISTER(System, system);
	REGISTER(Pause, pause);
	REGISTER(Sleep, sleep);
	REGISTER(PWD, pwd);
	REGISTER(CD, cd);
	REGISTER(Script, script);
}

#undef REGISTER
