#ifndef SPPS_H_
#define SPPS_H_

#include <string>
#include <vector>
#include <map>

#include "util.h"

using std::string;
using std::vector;

namespace spps
{
	const string VERSION = "0.1.2";

	class SPPSSys
	{
		public:
			static int errorlevel;

			static void quit(int exit)
			{
				should_exit = true;
				exit_code = exit;
			}

			static inline bool running()
			{
				return !should_exit;
			}

			static inline int get_exit_code()
			{
				return exit_code;
			}

			static inline void set_errorlevel(int el)
			{
				perrorlevel = el;
			}

			static void trigger_el()
			{
				errorlevel = perrorlevel;
				perrorlevel = 0;
			}

		private:
			static bool should_exit;
			static int exit_code, perrorlevel;
	};

	class CommandProvider
	{
		public:
			virtual ~CommandProvider() {}
			virtual bool provides_command(string command) = 0;
			virtual void execute_command(string full, vector<string> split) = 0;
			virtual string get_help(string command) = 0;
			virtual void get_all_commands(vector<string> &result) = 0;
	};

	class ProviderManager
	{
		public:
			static int script_level;

			static void register_command_provider(CommandProvider *provider);
			static void execute(string command);
			static string get_help(string command);
			static void get_all_commands(vector<string> &result);

		private:
			static vector<CommandProvider *> providers;
	};

	class CommandExecutor
	{
		public:
			virtual ~CommandExecutor() {}
			virtual void execute(string full, vector<string> split) = 0;
			virtual string get_help() = 0;
	};

	class DefaultCommandProvider : public virtual CommandProvider, public Destroyable
	{
		public:
			bool provides_command(string command);
			void execute_command(string full, vector<string> split);
			string get_help(string command);
			void get_all_commands(vector<string> &result);

			static void register_command(string command, CommandExecutor *executor);

		private:
			static std::map<string, CommandExecutor *> default_commands;
			static vector<string> dcl;
	};
}

#endif
