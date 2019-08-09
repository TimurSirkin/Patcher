#ifndef PATCHBUILDER_H
#define PATCHBUILDER_H

#include <string>
#include <vector>
#include <utility>
#include <regex>
#include <sstream>
#include "DBProvider/DBProvider.h"

// Templates code words
constexpr auto TYPE_CODE = "$type$"; // Type code
constexpr auto BEGIN_CODE = "$begin$"; // Begining of the block code
constexpr auto END_CODE = "$end$"; // Ending of the block code
constexpr auto NAME_CODE = "%name%"; // Object name code
constexpr auto NAME_LENGTH = 6; // Object name code length
constexpr auto SCHEMA_CODE = "%schema%"; // Object scheme code
constexpr auto SCHEMA_LENGTH = 8; // Object scheme code length
constexpr auto ANY_TYPE_CODE = "$any$"; // Type not present in the template code

constexpr auto DEPENDENCY_LIST_NAME = "DependencyList.dpn"; // Name of file that will be created. Contains list of objects and their type that required to install the patch
constexpr auto OBJECT_LIST_NAME = "ObjectList.txt"; // Name of ObjectList for list of all objects for installation
constexpr auto INSTALL_SCRIPT_NAME_BAT = "Install.bat"; // Name of install script with .bat format
constexpr auto INSTALL_SCRIPT_NAME_SH = "Install.sh"; // Name of install script with .shell format
constexpr auto LOG_NAME = "PatchBuilder_"; // Begining of the log file name
constexpr auto LOG_FOLDER = "logs"; // Folder name for logs
constexpr auto LOG_FORMAT = ".log"; // Folder name for logs

constexpr auto BLOCK_LINE = "----------------"; // Splitter for text outing
using namespace std;

class PatchBuilder 
{
public:
  PatchBuilder() = default;
  PatchBuilder(const string pPatchListFullName, DBProvider &pProvider, const string pTemplatesFullName = ""); // Constructor
  ~PatchBuilder(); // Distructor
  void buildPatch(const string directory); // Build patch in choosen directory
  void addLog(const string message) const; // Add message in log file

private:
	string patchListFullName; // Directory and name of PatchList
	string templateString = ""; // Teplate text
	string logFileFullName; // Directory and name of file for logs
	DBProvider *provider; // Ptr to provider class
	bool isWithErrors = false; // Become true after some error
	bool isWithWarnings = false; // Become true after some warning
	scriptDataVectorType getScriptDataVector(const objectDataVectorType &objectDataVector) const;// Getting vector of scripts, created by DBProvider
	objectDataVectorType getObjectDataVector() const; // Getting vector of objects from source database
	void createInstallPocket(const string directory, const scriptDataVectorType &scriptDataVector) const; // Creating sql files for scripts from scriptDataVector and creating install script file
	bool isContains(const ObjectData data, const string &scriptText); // Returns true, if the object was found in the script
	objectDataVectorType getPatchListVector() const; // Getting vector of objects that contains a patch
	void createObjectList(const scriptDataVectorType &objectDataVector, const string directory) const; // Creating of ObjectList
	static void remove(objectDataVectorType &objectDataVector_first, const objectDataVectorType &objectDataVector_second); // Removing elements of second vector from first vector
	static void removeComments(scriptDataVectorType &scriptDataVector); // Removing all commits from script text
	regex createExpression(const ObjectData &data); // Creating regular expression for data from params
	static string getCurrentDateTime(); // Get current date
};
#endif
