#pragma once
#include <vector>
#include <string>

#include <pqxx/pqxx>

#include "consts.h"
namespace my_bd {
	struct Client {
		int id = consts::my_bd::kId;
		std::string name = std::string(consts::my_bd::kName);
		std::string surname = std::string(consts::my_bd::kSurname);
		std::string email = std::string(consts::my_bd::kEmail);
		std::vector<std::string> phoneNumbers = std::vector<std::string>{};
	};
	class DBWorker {
	public:
		explicit DBWorker(const std::string& conn_str);
		~DBWorker() = default;
		// bool - it is not necessary(except for ChangeClientData, DeleteClient, DeletePhoneNumber) for but i add it for convenience.
		bool Create(); // public for method Create - optional for task. i would make it private...
		bool AddClient(const Client& client);
		bool DeleteClient(int id);
		bool AddPhoneNumber(int client_id, const std::string& phone_number);
		bool DeletePhoneNumber(int client_id, const std::string& phone_number);
		bool ChangeClientData(int client_id, const Client& new_data);
		bool FindClient(const std::string& search_request);
		
		void ClearDB(); // for correct demonstration(test)
	private:
		void AddPhoneToTx(pqxx::work& tx, int client_id, const std::string& phone_number);
		void PrepareRequests();
		void PrintClientInfo(const pqxx::row_ref& row);
		template <typename T>
		T GetField(const pqxx::row_ref& row, std::string_view column_name) {
			return row[std::string(column_name)].as<T>();
		}
		pqxx::connection conn;
	};
}