#ifndef DBPROVIDER_H
#define DBPROVIDER_H

#include "DBProvider/DBConnection.h"
#include "Shared/ParsingTools.h"

#include <pqxx/pqxx>
#include <tuple>
#include <string>

using namespace std;

struct ObjectData // Sctructure for containing objet data
{
	string name; // Name of object
	string type; // Type of object
	string scheme; // Scheme of object
	vector<string> paramsVector; // Params of object

	ObjectData() = default;
	ObjectData(const string pName, const string pType, const string pScheme, const vector<string> pParamsVector = vector<string>())
	{
		name = pName;
		type = pType;
		scheme = pScheme;
		paramsVector = pParamsVector;
	}

	bool operator == (ObjectData &object) const
	{
		return (this->name == object.name) && (this->type == object.type);
	}
};

struct ScriptData : ObjectData // Sctructure for containing script data
{
	std::string text; // Script text

	ScriptData() = default;
	
	ScriptData(const std::string pName,string pType,string pScheme,vector<string> pParamsVector, string pText = "") : ObjectData(pName, pType, pScheme, pParamsVector)
	{
		text = pText;
	}
	
	ScriptData(const ObjectData objectData, string pText) : ScriptData(objectData.name, objectData.type, objectData.scheme, objectData.paramsVector, pText) {}
};

struct Column // Structure for containing information about column of table
{
public:
	string name;
	string type;
	string defaultValue;
	string description;
	bool isNullable() const;
	void setNullable(string nullable);
private:
	bool nullable_ = false;
};

struct Trigger
{
	string name;
	string timing;
	string manipulation;
	string orientation;
	string action;
};

struct ObjectInformation // Sctructure for containing object structure information
{
	vector<Column> columns;
	vector<Trigger> triggers;
	string owner;
	string description;

	Trigger *getTrigger(string triggerName)
	{
		for (Trigger &trigger : triggers)
		{
			if (trigger.name == triggerName)
			{
				return &trigger;
			}
		}
		return nullptr;
	}
};

// Vector for containing object data
typedef vector<ObjectData> objectDataVectorType;

// Vector for containing script data
typedef vector<ScriptData> scriptDataVectorType;

class DBProvider
{
public:
	explicit DBProvider(string args);
	
	~DBProvider();

	// Returns all objects of database
	vector<ObjectData> getObjects() const;
	
	// Returns script data by object data
	ScriptData getScriptData(const ObjectData &data) const;
	
	// Checks if specified object exists in database
	bool doesCurrentObjectExists(std::string scheme, std::string name, std::string type) const;

	pqxx::result query(std::string strSQL) const;

	// Inserts a new record into the content provider
	static void insertToDB(ObjectData obj)
	{
	}

	// Deletes an existing record from the content provider
	static void deleteFromDB(ObjectData obj)
	{
	}

	// Updates an existing record from the content provider
	static void update(ObjectData obj)
	{
	}

	// Returns the MIME type of the data at the given URI
	static objectDataVectorType getType(ObjectData obj)
	{
		return {};
	}

	// Uses specified view
	static std::vector<ObjectData> useViewToGetData(std::string nameOfView)
	{
		return {};
	}

	// Creates new view
	static std::vector<ObjectData> createAndUseView(std::string nameOfView, std::string bodyOfView)
	{
		return {};
	}

	bool tableExists(const std::string& tableSchema, const std::string& tableName) const;

	bool sequenceExists(const std::string& sequenceSchema, const std::string& sequenceName) const;

	static bool functionExists(const std::string& name);

	static bool indexExists(const std::string& name);

	bool viewExists(const std::string& tableSchema, const std::string& tableName) const;

	bool triggerExists(const std::string& triggerSchema, const std::string& triggerName) const;

private:
	DBConnection *_connection = nullptr;

	// Getting information about object from database
	ObjectInformation getObjectInformation(const ObjectData &data) const;

	// Get single value from query
	inline string getSingleValue(const string &queryString, const string &columnName) const;
};

void printObjectsData(pqxx::result res);

#endif