#ifndef SCR_H_
#define SCR_H_

#include <string>
#include <fstream>
#include <stack>

namespace spps
{
	class ScriptHandler
	{
		public:
			~ScriptHandler();

			static bool execute(std::string filename, bool warn_not_found = true);
			static int get_line();

		private:
			ScriptHandler(std::string filename);

			static std::stack<ScriptHandler *> handlers;

			std::ifstream *stream;
			int line_number = 0;

			void exec();
	};
}

#endif
