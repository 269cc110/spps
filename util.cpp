#include "util.h"

using namespace spps;

using std::string;
using std::vector;

vector<Destroyable *> Util::destroyables;

void Util::split(vector<string> &result, string to_split)
{
	vector<char> buffer;

	bool in_quote = false;
	bool escape = false;

	unsigned int index = 0;

	while(index < to_split.length())
	{
		char c = to_split[index++];

		if(escape)
		{
			escape = false;
			buffer.push_back(c);
		}
		else
		{
			if(Util::is_whitespace(c))
			{
				if(in_quote) buffer.push_back(c);
				else if(!buffer.empty())
				{
					result.push_back(string(buffer.begin(), buffer.end()));
					buffer.clear();
				}
			}
			else if(c == '"')
			{
				in_quote = !in_quote;

				if(!buffer.empty())
				{
					result.push_back(string(buffer.begin(), buffer.end()));
					buffer.clear();
				}
			}
			else if(c == '\\') escape = true;
			else buffer.push_back(c);
		}
	}

	if(!buffer.empty()) result.push_back(string(buffer.begin(), buffer.end()));
}

void Util::join(string &out, vector<string> in, string delim)
{
	for(auto &s : in)
	{
		out.append((out.empty() ? "" : delim) + s);
	}
}

string Util::trim_left(string to_trim)
{
	unsigned int slen = to_trim.length();
	unsigned int fnw = 0;

	for(unsigned int i = 0; i < slen; i++)
	{
		if(!Util::is_whitespace(to_trim[i]))
		{
			fnw = i;
			break;
		}
	}

	return fnw > 0 ? to_trim.substr(fnw, slen - fnw) : to_trim;
}

string Util::trim_right(string to_trim)
{
	unsigned int slen = to_trim.length();
	unsigned int lnw = 0;

	for(unsigned int i = 0; i < slen; i++)
	{
		if(!Util::is_whitespace(to_trim[i])) lnw = i + 1;
	}

	return lnw < slen ? to_trim.substr(0, lnw) : to_trim;
}

string Util::trim(string to_trim)
{
	unsigned int slen = to_trim.length();
	unsigned int lnw = 0;
	unsigned int fnw = 0;

	bool hnw = false;

	for(unsigned int i = 0; i < slen; i++)
	{
		if(!Util::is_whitespace(to_trim[i]))
		{
			if(!hnw)
			{
				fnw = i;
				hnw = true;
			}

			lnw = i;
		}
	}

	unsigned int rlen = lnw == 0 ? 0 : (lnw - fnw) + 1;

	return rlen < slen ? (rlen == 0 ? "" : to_trim.substr(fnw, rlen)) : to_trim;
}

bool Util::is_whitespace(char c)
{
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

void Util::add_destroyable(Destroyable *destroyable)
{
	Util::destroyables.push_back(destroyable);
}

void Util::destroy_all()
{
	for(auto *p : Util::destroyables)
	{
		p->destroy();
	}
}
