#include "my_bd.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include <pqxx/pqxx>

#include "consts.h"
namespace my_bd {
	DBWorker::DBWorker(const std::string& conn_str) :
		conn(conn_str) {
		if (!Create()) { throw std::runtime_error(std::string(consts::sql::errs::kInitDBError)); }
		PrepareRequests(); 
	}
	bool DBWorker::Create() {
		try {
			pqxx::work tx{conn};
			tx.exec(consts::sql::req::kCreateClients);
			tx.exec(consts::sql::req::kCreatePhones);
			tx.commit();
			return true;
		}
		catch (const pqxx::sql_error& e) {
			std::cerr << consts::sql::errs::kCreateError << ": " << e.what() << "\n";
			return false;
		}
	}
	bool DBWorker::AddClient(const Client& client) {
		try {
			pqxx::work tx{ conn };
			pqxx::result result_client_id = tx.exec_prepared(
				std::string(consts::sql::templ_req::kTAddClient), 
				client.name, 
				client.surname, 
				client.email);
			int formatted_client_id = result_client_id[0][0].as<int>();
			for (const auto& phone : client.phoneNumbers) {
				AddPhoneToTx(tx, formatted_client_id, phone);
			}
			tx.commit();
			return true;
		}
		catch (const pqxx::sql_error& e) {
			std::cerr << consts::sql::errs::kAddClientError << ": " << e.what() << "\n";
			return false;
		}
	}
	bool DBWorker::DeleteClient(int id) {
		try {
			pqxx::work tx{ conn };
			pqxx::result result = tx.exec_prepared(
				std::string(consts::sql::templ_req::kTDeleteClient),
				id);
			tx.commit();
			// phones will delete automatically - id_client referense use CASCADE
			return result.affected_rows() > 0;
		}
		catch (const pqxx::sql_error& e) {
			std::cerr << consts::sql::errs::kDeleteClientError << ": " << e.what() << "\n";
			return false;
		}
	}
	bool DBWorker::AddPhoneNumber(int client_id, const std::string& phone_number) {
		try { 
			pqxx::work tx{ conn };
			AddPhoneToTx(tx, client_id, phone_number);
			tx.commit();
			return true;
		}
		catch (const pqxx::sql_error& e) {
			std::cerr << consts::sql::errs::kAddPhoneNumberError << ": " << e.what() << "\n";
			return false;
		}
	}
	bool DBWorker::DeletePhoneNumber(int client_id, const std::string& phone_number) {
		try {
			pqxx::work tx{ conn };
			pqxx::result result = tx.exec_prepared(
				std::string(consts::sql::templ_req::kTDeletePhoneNumber),
				client_id,
				phone_number);
			tx.commit();
			return result.affected_rows() > 0;
		}
		catch (const pqxx::sql_error& e) {
			std::cerr << consts::sql::errs::kDeletePhoneNumberError << ": " << e.what() << "\n";
			return false;
		}
	}
	bool DBWorker::ChangeClientData(int client_id, const Client& new_data) {
		try {
			pqxx::work tx{ conn };
			pqxx::result result = tx.exec_prepared(
				std::string(consts::sql::templ_req::kTChangeClientData),
				client_id,
				new_data.name,
				new_data.surname,
				new_data.email);
			tx.commit();
			return result.affected_rows() > 0;
		}
		catch (const pqxx::sql_error& e) {
			std::cerr << consts::sql::errs::kChangeClientDataError << ": " << e.what() << "\n";
			return false;
		}
	}
	bool DBWorker::FindClient(const std::string& search_request) {
		try {
			pqxx::work tx{ conn };
			pqxx::result search_result = tx.exec_prepared(
				std::string(consts::sql::templ_req::kTFindClient),
				search_request);
			if (search_result.empty()) {
				std::cout << consts::sql::req::helpers::kEmptySearchClientResult;
				return false;
			}
			for (const auto& row : search_result) {
				PrintClientInfo(row);
			}
			tx.commit();
			return true;
		}
		catch (const pqxx::sql_error& e) {
			std::cerr << consts::sql::errs::kFindClientError << ": " << e.what() << "\n";
			return false;
		}
	}
	void DBWorker::ClearDB() { // only for test
		try {
			pqxx::work tx{ conn };
			tx.exec(consts::sql::req::kTruncateClients);
			tx.commit();
			std::cout << " -- Database successfully cleared -- "; 
		}
		catch (const pqxx::sql_error& e) {
			std::cerr << consts::sql::errs::kTruncateClientsError << e.what() << "\n";
		}
	}
	void DBWorker::AddPhoneToTx(pqxx::work& tx, int client_id, const std::string& phone_number) {
		tx.exec_prepared(
			std::string(consts::sql::templ_req::kTAddPhoneNumber),
			client_id,
			phone_number
		);
	}
	void DBWorker::PrepareRequests() {
		conn.prepare(std::string(consts::sql::templ_req::kTAddClient), 
			std::string(consts::sql::req::kAddClient));
		conn.prepare(std::string(consts::sql::templ_req::kTAddPhoneNumber), 
			std::string(consts::sql::req::kAddPhoneNumber));
		conn.prepare(std::string(consts::sql::templ_req::kTDeleteClient), 
			std::string(consts::sql::req::kDeleteClient));
		conn.prepare(std::string(consts::sql::templ_req::kTDeletePhoneNumber), 
			std::string(consts::sql::req::kDeletePhoneNumber));
		conn.prepare(std::string(consts::sql::templ_req::kTChangeClientData), 
			std::string(consts::sql::req::kChangeClientData));
		conn.prepare(std::string(consts::sql::templ_req::kTFindClient), 
			std::string(consts::sql::req::kFindClient));
	}
	void DBWorker::PrintClientInfo(const pqxx::row_ref& row) {
		std::cout << consts::sql::req::helpers::kIDStr <<
			GetField<int>(row, consts::my_bd::kIDClientInDB) << // ID

			consts::sql::req::helpers::kNameStr <<
			GetField<std::string>(row, consts::my_bd::kNameInDB) << // Name

			consts::sql::req::helpers::kSurnameStr <<
			GetField<std::string>(row, consts::my_bd::kSurnameInDB) << // Surname

			consts::sql::req::helpers::kEmailStr <<
			GetField<std::string>(row, consts::my_bd::kEmailInDB);// Email

			if (!row[std::string(consts::my_bd::kPhoneNumberInDB)].is_null()) {
				std::cout << consts::sql::req::helpers::kPhoneNumberStr <<
					GetField<std::string>(row, consts::my_bd::kPhoneNumberInDB); // Phones
			}
			std::cout << "\n\n";
	}
}