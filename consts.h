#pragma once

#include <string_view>

namespace consts {
	namespace my_bd {
		// base values
		inline constexpr int kId = 0;
		inline constexpr std::string_view kName = "empty";
		inline constexpr std::string_view kSurname = "empty";
		inline constexpr std::string_view kEmail = "empty";
		inline constexpr std::string_view kPhoneNumber = "empty";

		// for method Print Client Info
		inline constexpr std::string_view kIDClientInDB = "id_client";
		inline constexpr std::string_view kNameInDB = "name";
		inline constexpr std::string_view kSurnameInDB = "surname";
		inline constexpr std::string_view kEmailInDB = "email";
		inline constexpr std::string_view kPhoneNumberInDB = "all_phones";
	}
	namespace sql::init {
		inline constexpr std::string_view kParametersDeclaration = "host={} port={} dbname={} user={} password={}";
		inline constexpr std::string_view kDbHost = "localhost";
		inline constexpr std::string_view kDbPort = "5432";
		inline constexpr std::string_view kDbName = "postgres";
		inline constexpr std::string_view kDbUser = "postgres";
		inline constexpr std::string_view kDbPassword = "zuzha2015A!";
	}
	namespace sql::errs {
		inline constexpr std::string_view kCreateError = "Failed to create tables ";
		inline constexpr std::string_view kAddClientError = "Failed to add client ";
		inline constexpr std::string_view kDeleteClientError = "Failed to delete client ";
		inline constexpr std::string_view kAddPhoneNumberError = "Failed to add phone number ";
		inline constexpr std::string_view kDeletePhoneNumberError = "Failed to delete phone number ";
		inline constexpr std::string_view kChangeClientDataError = "Failed to change client data ";
		inline constexpr std::string_view kFindClientError = "Failed to find client ";
		inline constexpr std::string_view kInitDBError = "Critical error - can't init db ";
		inline constexpr std::string_view kTruncateClientsError = "Failed to clear db "; // only for test
	}
	namespace sql::req {
		inline constexpr std::string_view kCreateClients = 
			"CREATE TABLE IF NOT EXISTS clients ("
			"id_client SERIAL PRIMARY KEY, " 
			"name VARCHAR(300) NOT NULL, "
			"surname VARCHAR(300) NOT NULL, "
			"email VARCHAR(350) NOT NULL UNIQUE"
			");";
		inline constexpr std::string_view kCreatePhones = 
			"CREATE TABLE IF NOT EXISTS phones ("
			"id_phone SERIAL PRIMARY KEY, "
			"id_client INTEGER REFERENCES clients(id_client) ON DELETE CASCADE, "
			"content VARCHAR(20) NOT NULL"
			");";
		inline constexpr std::string_view kAddClient = 
			"INSERT INTO clients ("
			"name, "
			"surname, "
			"email) "
			"VALUES ($1, $2, $3) "
			"RETURNING id_client;";
		inline constexpr std::string_view kAddPhoneNumber = //sub req for add client (for dependent table)
			"INSERT INTO phones("
			"id_client, " 
			"content"
			") VALUES ("
			"$1, $2);"; 
		inline constexpr std::string_view kDeleteClient = 
			"DELETE FROM clients " 
			"WHERE id_client = $1;";
		inline constexpr std::string_view kDeletePhoneNumber = 
			"DELETE FROM phones "
			"WHERE id_client = $1 " 
			"AND content = $2;";
		inline constexpr std::string_view kChangeClientData =
			"UPDATE clients "
			"SET name = $2, "
			"surname = $3, "
			"email = $4 "
			"WHERE id_client = $1;";
		inline constexpr std::string_view kFindClient = 
			"SELECT c.id_client, "
			"c.name, "
			"c.surname, "
			"c.email, "
			"STRING_AGG(p.content, ', ') as all_phones " 
			"FROM clients as c "
			"LEFT JOIN phones as p ON p.id_client = c.id_client "
			"WHERE c.name = $1 "
			"OR c.surname = $1 "
			"OR c.email = $1 "
			"OR p.content = $1 "
			"GROUP BY c.id_client, c.name, c.surname, c.email;";
		inline constexpr std::string_view kTruncateClients =
			"TRUNCATE TABLE clients "
			"RESTART IDENTITY CASCADE;";
	}
	namespace sql::templ_req {
		inline constexpr std::string_view kTAddClient = "add_client";
		inline constexpr std::string_view kTAddPhoneNumber = "add_phone";
		inline constexpr std::string_view kTDeleteClient = "delete_client";
		inline constexpr std::string_view kTDeletePhoneNumber = "delete_phone";
		inline constexpr std::string_view kTChangeClientData = "change_client";
		inline constexpr std::string_view kTFindClient = "find_client";
	}
	namespace sql::req::helpers { // for method Print Client Info
		inline constexpr std::string_view kEmptySearchClientResult = "Client not found"; 

		inline constexpr std::string_view kIDStr = "ID: ";
		inline constexpr std::string_view kNameStr = "| Name: ";
		inline constexpr std::string_view kSurnameStr = "| Surname: ";
		inline constexpr std::string_view kEmailStr = "| Email: ";
		inline constexpr std::string_view kPhoneNumberStr = " | Phones: ";
	}
}
