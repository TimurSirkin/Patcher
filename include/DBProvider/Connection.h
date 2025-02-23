#ifndef CONNECTION_H
#define CONNECTION_H

#include "DBProvider/ConnectionBase.h"
#include "DBProvider/LoginData.h"
#include <string>
#include <memory>
#include <pqxx/pqxx>

namespace DBConnection
{
	class Connection : ConnectionBase
	{
	public:
		Connection();

		explicit Connection(std::string& pgpass_str);

		~Connection();

		void setConnection(std::string& pgpass_str) override;

		void setConnection();

		LoginData getParameters() override;

		std::shared_ptr<pqxx::connection_base> getConnection() override;

	private:
		void closeConnection() override;
	};
}

#endif
