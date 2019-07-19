#pragma once
#include <string>
#include <vector>
#include <utility>
#include <pqxx/pqxx>

namespace ParsingTools
{
	using namespace std;
	vector<string> splitToVector(string str, const string &delimiter);
	string interpolate(string &original, const string &toBeReplaced, const string &replacement);
	string joinAsStrings(const vector<string> &vec, const char *delimiter);
	pair<vector<string>, string> parseCredentials(string &input);
}

class DBConnection
{
public:

	struct LoginData
	{
		std::string databaseName;
		std::string username;
		std::string password;
		std::string hostname;
		unsigned int portNumber;
		std::string result;
	} *info;

	DBConnection(std::string loginCredentials);
	~DBConnection();

private:
	// field stores current connection
	pqxx::connection *current = nullptr;
	void setConnection(const std::string &credentials);
	void disconnect();
};