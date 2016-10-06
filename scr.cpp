#include <iostream>

#include "scr.h"
#include "spps.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

using namespace spps;

std::stack<ScriptHandler *> ScriptHandler::handlers;

ScriptHandler::ScriptHandler(string filename)
{
	stream = new std::ifstream(filename);
}

ScriptHandler::~ScriptHandler()
{
	delete stream;
}

bool ScriptHandler::execute(string filename, bool warn_not_found)
{
	ScriptHandler handler(filename);

	if(!*handler.stream)
	{
		if(warn_not_found) cerr << "File not found" << endl;

		return false;
	}

	ProviderManager::script_level++;
	handlers.push(&handler);

	handler.exec();

	handlers.pop();
	ProviderManager::script_level--;

	return true;
}

int ScriptHandler::get_line()
{
	return handlers.top()->line_number;
}

void ScriptHandler::exec()
{
	string line;

	while(*stream)
	{
		std::getline(*stream, line);
		ProviderManager::execute(line);

		if(SPPSSys::errorlevel != 0) continue;
	}

	stream->close();
}
