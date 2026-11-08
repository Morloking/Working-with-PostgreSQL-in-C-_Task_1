#include <stdexcept>
#include <iostream>
#include <format>
#include <string>

#include <pqxx/pqxx>

#include "consts.h"
#include "my_bd.h"

int main() {
	try {
		std::string conn_str = std::format(
			consts::sql::init::kParametersDeclaration,
			consts::sql::init::kDbHost,
			consts::sql::init::kDbPort,
			consts::sql::init::kDbName,
			consts::sql::init::kDbUser,
			consts::sql::init::kDbPassword
		);
		my_bd::DBWorker db(conn_str);
		std::cout << " -- Database successfully initialized -- \n";
		db.ClearDB(); // only for correct test

		std::cout << " -- Adding a clients -- \n";
		my_bd::Client client_1;
		client_1.name = "Ivan";
		client_1.surname = "Ivanov";
		client_1.email = "ivan101@mail.ru";
		client_1.phoneNumbers = { "79999999999","799999999922" };

		my_bd::Client client_2;
		client_2.name = "Petr";
		client_2.surname = "Petrov";
		client_2.email = "petr2000@mail.ru";
		client_2.phoneNumbers = { "78888888888","78888888885" };

		db.AddClient(client_1);
		db.AddClient(client_2);

		std::cout << " -- Looking for a client -- \n";
		db.FindClient("Petrov");

		std::cout << " -- Add the number -- \n";
		db.AddPhoneNumber(1, "767777777");
		std::cout << " -- Check result -- \n";
		db.FindClient("Ivan");

		std::cout << " -- Changing client info -- ";
		my_bd::Client updated_client_1;
		updated_client_1.name = "Ivan";
		updated_client_1.surname = "Ivanov";
		updated_client_1.email = "ivan666666666666@mail.ru";
		db.ChangeClientData(1, updated_client_1);
		std::cout << " -- Check result -- \n";
		db.FindClient("ivan666666666666@mail.ru");

		std::cout << " -- Adding new phone for client1 -- ";
		db.AddPhoneNumber(1,"444444444444");
		std::cout << " -- Check result -- \n";
		db.FindClient("Ivan");

		std::cout << " -- Deleting phone for client2 -- ";
		db.DeletePhoneNumber(2, "78888888885");
		std::cout << " -- Check result -- \n";
		db.FindClient("Petr");

		std::cout << " -- Deleting client2 -- ";
		db.DeleteClient(2);
		std::cout << " -- Check result -- \n";
		db.FindClient("Petr");

		std::cout << "\n--- Test - client doesn't exists ---\n";
		if (!db.DeleteClient(999)) {
			std::cout << "Client does't exist.\n";
		}
	}
	catch (const pqxx::sql_error e){
		std::cout << "Critical db error" << e.what() << "\n";
		return 1;
	}
	catch (...) {
		std::cerr << "Unknown error\n";
		return 2;
	}
	return 0;
}