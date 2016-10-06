#include <iostream>
#include <cstdlib>

#include "spps.h"
#include "bic.h"
#include "util.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

using namespace spps;

bool SPPSSys::should_exit = false;
int SPPSSys::exit_code = 0;
int SPPSSys::errorlevel = 0;
int SPPSSys::perrorlevel = 0;

int ProviderManager::script_level = 0;

int main()
{
	cout << "SPPS " << VERSION << endl;

	builtin::init();

	DefaultCommandProvider *provider = new DefaultCommandProvider;
	ProviderManager::register_command_provider(provider);
	Util::add_destroyable(provider);

	string command;

	while(SPPSSys::running())
	{
		cout << "> ";

		std::getline(cin, command);

		ProviderManager::execute(command);
	}

	Util::destroy_all();

	return SPPSSys::get_exit_code();
}
