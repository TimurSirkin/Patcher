#include <fstream>
#include <iostream>
#include <array>
#include <time.h>
#include <filesystem>
#include <direct.h>

#include "PatchInstaller/PatchInstaller.h"
#include "PatchInstaller/DependenciesChecker.h"
#include "Shared/Logger.h"
#include "PatchInstaller/FileParser.h"

using namespace PatcherLogger;

PatchInstaller::PatchInstaller() {}
PatchInstaller::~PatchInstaller() {}

/** Creates log for both parts of PatchInstaller. */
void createLog(std::string path, Level level, std::string data) {
	Log log;
	log.setLogByPath(path);
	log.addLog(level, data);

}

/** Checking dependencies part. */
void passCheckLogsForGui(std::string data, bool resultCheck) {
	std::cerr << "DEPENDENCY CHECK PROCESS:\n";
	std::cerr << data;
	if (!resultCheck) {
		std::cerr << "Check completed. Some objects do not exist in current database.\n";
	}
	else {
		std::cerr << "Check completed. All objects exist in current database.\n";
	}
}

/** Main method of checking dependencies.
	Checks the presence of objects in the database according to the list of objects specified in the file. */
bool PatchInstaller::checkDependencyList(std::string nameOfFile, DBProvider *dbProvider) {
	FileParser parser;
	if (parser.checkInputCorrect(nameOfFile)) {
		//A list of objects with three parameters: schema-name-type is created from the file
		DBObjects objectsParameters = parser.getResultOfParsing(nameOfFile);
		DependenciesChecker checker;

		// result = true if all object exist in current database
		bool result = checker.getCheck(objectsParameters, dbProvider);
		// print in stdout list of existence objects: schema name type exists / schema name type does not exist
		passCheckLogsForGui(checker.getDataForLog(), result);
		// print in stdout the same list of existence objects: 0 if object does not exist, 1 if object exists
		checker.print();
		createLog("logs/CheckingDependenciesErrors.log", INFO, checker.getDataForLog());

		return result;
	}
	else {
		throw std::invalid_argument("Incorrect DependencyList.dpn file");
	}
}

/** Installation part. */

/** Inner methods. */
std::string readLogFromTempFile(std::string fileName) {
	std::string dataForLog;
	std::string buffer;
	std::ifstream temp(fileName, std::ios::in);

	while (getline(temp, buffer)) {
		dataForLog += buffer;
		dataForLog += "\n";
	}

	return dataForLog;
}

void passInstallLogsGui(std::string &dataForErrorLog, bool installWithErrors) {
	std::cerr << "INSTALLATION ERRORS:\n";
	if (dataForErrorLog.empty()) {	
		dataForErrorLog += "Installation completed.\n";
	}
	else {
		dataForErrorLog += "Installation completed WITH ERRORS.\n";
	}
	std::cerr << dataForErrorLog;
}

/** Figuring out the operating system. */
#if !defined(__WIN32__) && (defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__))
#  define __WIN32__
#endif

#if defined(__unix__) || defined(__unix) || defined(_UNIXWARE7)
#define __unix__
#endif

/** Main method of installing part.
	When the method starts, the dependency check is considered successful. */
bool PatchInstaller::startInstallation(LoginData p) {
	//build command with parameters: Install.bat username databaseName host port
#if defined(__WIN32__)
	std::string commandWithParametersConnection = "Install.bat " + p.username +
		+ " " + p.database + " " + p.hostname + " ";
	commandWithParametersConnection += std::to_string(p.port);
#endif
#if (defined(__unix__)) 
	std::string commandWithParametersConnection = "Install.sh " + p.username +
		+" " + p.database + " " + p.hostname + " ";
	commandWithParametersConnection += std::to_string(p.port);
#endif
	const char *cstr = commandWithParametersConnection.c_str();
	//start installation script with parameters
	system(cstr);

	//open temp file generated by installation script
	std::ifstream errors("tempError.txt", std::ios::in);
	std::ifstream info("tempInfo.txt", std::ios::in);

	std::string dataForInfoLog = readLogFromTempFile("tempInfo.txt");
	std::string dataForErrorLog = readLogFromTempFile("tempError.txt");
	bool installWithErrors = dataForErrorLog.empty();
	//print in stdout installation errors
	passInstallLogsGui(dataForErrorLog, installWithErrors);

	errors.close();
	info.close();
	remove("tempError.txt");
	remove("tempInfo.txt");

	createLog("logs/InstallationErrors.log", ERROR, dataForErrorLog);
	createLog("logs/InstallationInfo.log", INFO, dataForInfoLog);

	return installWithErrors;
}
