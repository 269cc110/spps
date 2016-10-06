#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace spps
{
	class Destroyable
	{
		public:
			~Destroyable() {}
			void destroy()
			{
				delete this;
			}
	};

	class Util
	{
		public:
			static void split(vector<string> &out, string to_split);
			static void join(string &out, vector<string> in, string delim);
			static string trim_left(string to_trim);
			static string trim_right(string to_trim);
			static string trim(string to_trim);
			static bool is_whitespace(char c);
			static void add_destroyable(Destroyable *to_destroy);
			static void destroy_all();

		private:
			static vector<Destroyable *> destroyables;
	};
}

#endif
