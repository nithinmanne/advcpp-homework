/*
	Advanced C++ Final Project
	Naga Nithin Manne
*/

#include <iostream>
#include <string>

#include "Util.h"
#include "Database.h"
#include "Menu.h"
#include "Form.h"

using namespace advcppfinal;

int main() {
	/* Raw Strings!! */
	std::cout << R"(Library Management
by Naga Nithin Manne)" << std::endl;
	Database db;
	using MainMenu = Menu<LoginForm, RegisterForm>;
	MainMenu::show<true>(db);
}