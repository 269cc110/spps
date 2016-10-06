#ifndef BIC_H_
#define BIC_H_

#include <iostream>
#include <string>

#include "spps.h"
#include "util.h"

using std::string;
using std::vector;

namespace spps
{
	namespace builtin
	{

#define EXECUTOR_CLASS(a) class Default##a : public virtual CommandExecutor, public Destroyable \
{ \
	public: \
		~Default##a() {} \
		void execute(string full, vector<string> split); \
		string get_help(); \
};

		/*class DefaultExit : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultExit() {}
				void execute(string full, vector<string> split);
				string get_help();
		};*/

		EXECUTOR_CLASS(Exit)

		class DefaultHelp : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultHelp() {}
				void execute(string full, vector<string> split);
				string get_help();
		};

		class DefaultEcho : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultEcho() {}
				void execute(string full, vector<string> split);
				string get_help();
		};

		class DefaultSPPS : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultSPPS() {}
				void execute(string full, vector<string> split);
				string get_help();
		};

		class DefaultSystem : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultSystem() {}
				void execute(string full, vector<string> split);
				string get_help();
		};

		class DefaultPause : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultPause() {}
				void execute(string full, vector<string> split);
				string get_help();
		};

		class DefaultSleep : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultSleep() {}
				void execute(string full, vector<string> split);
				string get_help();
		};

		class DefaultPWD : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultPWD() {}
				void execute(string full, vector<string> split);
				string get_help();
		};

		class DefaultCD : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultCD() {}
				void execute(string full, vector<string> split);
				string get_help();
		};

		class DefaultScript : public virtual CommandExecutor, public Destroyable
		{
			public:
				~DefaultScript() {}
				void execute(string full, vector<string> split);
				string get_help();
		};

#undef EXECUTOR_CLASS

		void init();
	}
}

#endif
