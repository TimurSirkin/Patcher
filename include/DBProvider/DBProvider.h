#ifndef DBPROVIDER_H
#define DBPROVIDER_H

#include "DBProvider/Connection.h"
#include <pqxx/pqxx>
#include <string>

using namespace std;

struct ObjectData // Sctructure for containing objet data
{
	string name; // Name of object
	string type; // Type of object
	string schema; // Scheme of object
	vector<string> params; // Params of object

	ObjectData() = default;
	ObjectData(const string pName, const string pType, const string pScheme, const vector<string> pParamsVector = vector<string>())
	{
		name = pName;
		type = pType;
		schema = pScheme;
		params = pParamsVector;
	}

	bool operator == (ObjectData& object) const
	{
		return (this->name == object.name) && (this->type == object.type);
	}
};

struct ScriptData : ObjectData // Sctructure for containing script data
{
	std::string text; // Script text

	ScriptData() = default;

	ScriptData(const std::string pName, string pType, string pScheme, vector<string> pParamsVector, string pText = "") : ObjectData(pName, pType, pScheme, pParamsVector)
	{
		text = pText;
	}

	ScriptData(const ObjectData& objectData, string pText) : ScriptData(objectData.name, objectData.type, objectData.schema, objectData.params, pText)
	{
	}
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

struct Constraint
{
	string type;
	string name;
	string columnName;
	string checkClause;
	string foreignTableSchema;
	string foreignTableName;
	string foreignColumnName;
	string matchOption;
	string onDelete;
	string onUpdate;
};

struct PartittionTable
{
	string schema;
	string name;
	string partitionExpression;
};

struct Table // Sctructure for containing table structure information
{
public:
	string type;
	string owner;
	string description;
	string options;
	string space;
	string partitionExpression;

	void setPartitionTable(string shema, string name, string partitionExpression);
	PartittionTable getPartitionTable() const;
	bool isPartition() const;

	vector<Column> columns;
	vector<Constraint> constraints;
	vector<string> inheritTables;
	vector<string> indexCreateExpressions;

private:
	PartittionTable _partitionTable;
	bool _isPartition = false;
};

// Vector for containing object data
typedef vector<ObjectData> objectDataVector;

// Vector for containing script data
typedef vector<ScriptData> scriptDataVector;

class DBProvider
{
public:
	explicit DBProvider(string &loginStringPG);

	DBProvider() = default;

	~DBProvider();

	// Returns all objects of database
	vector<ObjectData> getObjects() const;

	// Returns script data by object data
	ScriptData getScriptData(const ObjectData& data, vector<ScriptData> &extraScriptDatas);

	// Checks if specified object exists in database
	bool doesCurrentObjectExists(const std::string& scheme, const std::string& signature, const std::string& type) const;

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
	static objectDataVector getType(ObjectData obj)
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

	bool tableExists(const std::string& schema, const std::string& tableName) const;

	bool sequenceExists(const std::string& schema, const std::string& sequenceName) const;

	bool functionExists(const std::string& schema, const std::string& funcSignatur) const;

	bool indexExists(const std::string& schema, const std::string& indexName) const;

	bool viewExists(const std::string& tableSchema, const std::string& tableName) const;

	bool triggerExists(const std::string& triggerSchema, const std::string& triggerName) const;


private:
	// DBConnectionPool Pool
	shared_ptr<DBConnection::Connection> currentConnection;

	// Getting information about object from database
	Table getTable(const ObjectData& data);

	// Get single value from query
	string getSingleValue(const string& queryString, const string& columnName) const;

	// Get ScriptData for current type
	ScriptData getTableData(const ObjectData& data, vector<ScriptData> &extraScriptDatas);
	ScriptData getFunctionData(const ObjectData& data) const;
	ScriptData getViewData(const ObjectData& data) const;
	ScriptData getSequenceData(const ObjectData& data) const;
	ScriptData getTriggerData(const ObjectData& data) const;
	ScriptData getIndexData(const ObjectData& data) const;

	// Methods for initialization of Table structure
	bool initializePartitionTable(Table& table, const ObjectData& data);
	void initializeType(Table& table, const ObjectData& data);
	void initializeOwner(Table& table, const ObjectData& data);
	void initializeDescription(Table& table, const ObjectData& data);
	void initializeOptions(Table& table, const ObjectData& data);
	void initializeSpace(Table& table, const ObjectData& data);
	void initializeColumns(Table& table, const ObjectData& data);
	void initializePartitionExpression(Table& table, const ObjectData& data);
	void initializeConstraints(Table& table, const ObjectData& data);
	void initializeInheritTables(Table& table, const ObjectData& data);
	void initializeIndexExpressions(Table& table, const ObjectData& data);
};

void printObjectsData(const pqxx::result& res);

#endif