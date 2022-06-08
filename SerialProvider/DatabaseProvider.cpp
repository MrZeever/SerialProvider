#include "DatabaseProvider.h"

database_provider::DatabaseProvider::DatabaseProvider()
{
	checkCurrentFolder(file_location);
}

bool database_provider::DatabaseProvider::getFile()
{
	system("git checkout main");
	system("git pull");

	return fileExist(file_location);
}

bool database_provider::DatabaseProvider::checkVersion()
{
	return false;
}

bool database_provider::DatabaseProvider::pushFile()
{
	auto giver = []() {
		system("git add SerialBase/Base.txt");
		system("git add SerialBase/Last.txt");
		system("git commit -m\"Updated database\" ");
		system("git push");
		return true;
	};

	std::future<bool> fut = std::async(std::launch::async, giver);
	return fut.get();
}

void database_provider::DatabaseProvider::checkCurrentFolder(std::string& loc)
{
	auto verify = []() {
		return std::filesystem::current_path().u8string();
	};
	std::future<std::string> fut = std::async(std::launch::async, verify);
	loc = fut.get();
	std::cout << "Current folder: " << loc << std::endl;
}

std::string database_provider::DatabaseProvider::getFileLocation()
{
	return std::filesystem::current_path().u8string() + "\\" + binaryName;
}

bool database_provider::DatabaseProvider::fileExist(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}
