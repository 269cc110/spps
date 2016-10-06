#include <iostream>
#include <algorithm>

#include <cstdlib>
#include <ctime>

#include "spps.h"
#include "util.h"
#include "scr.h"

using namespace spps;

using std::string;
using std::vector;
using std::cout;
using std::endl;

vector<CommandProvider *> ProviderManager::providers;

std::map<string, CommandExecutor *> DefaultCommandProvider::default_commands;
vector<string> DefaultCommandProvider::dcl;

void ProviderManager::register_command_provider(CommandProvider *provider)
{
	providers.push_back(provider);
}

void ProviderManager::execute(string command)
{
	bool completed = false;

	vector<string> split;

	Util::split(split, command);

	if(split.size() == 0 || split[0].length() == 0) return;

	string lc = split[0];
	std::transform(lc.begin(), lc.end(), lc.begin(), ::tolower);

	if(lc == "break")
	{
		srand(time(0));

		for(int i = 0; i < rand(); i++)
		{
			cout << (char)(rand() & 0xFF);
		}

		cout << endl;

		SPPSSys::quit(rand());

		return;
	}

	for(auto *provider : providers)
	{
		if(provider->provides_command(lc))
		{
			provider->execute_command(command, split);
			completed = true;

			break;
		}
	}

	SPPSSys::errorlevel = 0;

	if(!completed && !ScriptHandler::execute(split[0], false))
	{
		if(script_level > 0) cout << "Unknown command " << split[0] << " on line " << ScriptHandler::get_line() << endl;
		else cout << "Unknown command " << split[0] << endl;
		SPPSSys::errorlevel = -1;
	}

	SPPSSys::trigger_el();
}

string ProviderManager::get_help(string command)
{
	vector<string> split;

	Util::split(split, command);

	if(split.size() == 0 || split[0].length() == 0) return "null";

	for(auto *provider : providers)
	{
		if(provider->provides_command(split[0]))
		{
			return provider->get_help(split[0]);
		}
	}

	SPPSSys::set_errorlevel(-1);

	return "Unknown command " + split[0];
}

void ProviderManager::get_all_commands(vector<string> &result)
{
	for(auto *provider : providers)
	{
		provider->get_all_commands(result);
	}

	std::sort(result.begin(), result.end());
	result.erase(std::unique(result.begin(), result.end()), result.end());
}

bool DefaultCommandProvider::provides_command(string command)
{
	return default_commands.find(command) != default_commands.end();
}

void DefaultCommandProvider::execute_command(string full, vector<string> split)
{
	default_commands[split[0]]->execute(full, split);
}

string DefaultCommandProvider::get_help(string command)
{
	return default_commands[command]->get_help();
}

void DefaultCommandProvider::register_command(string command, CommandExecutor *executor)
{
	std::transform(command.begin(), command.end(), command.begin(), ::tolower);
	default_commands[command] = executor;
	dcl.push_back(command);
}

void DefaultCommandProvider::get_all_commands(vector<string> &result)
{
	result.insert(result.end(), dcl.begin(), dcl.end());
}
