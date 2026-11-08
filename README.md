## Assembly:

cmake -S . -B build

cmake --build build --config Release

## Run after build:

.\build\Release\homework_sql.exe



## I used my own configuration settings. See below

***Host:`localhost`***

***Port:`5432`***

***Database:`postgres`***

***User:`postgres`*** 

***Password:`zuzha2015A!`***



## If your configuration differs, you need to update the relevant constants in consts.h:

***inline constexpr std::string_view kDbHost = "your_localhost";***

***inline constexpr std::string_view kDbPort = "your_port";***

***inline constexpr std::string_view kDbName = "your_db_name";***

***inline constexpr std::string_view kDbUser = "your_username";***

***inline constexpr std::string_view kDbPassword = "your_password";***

