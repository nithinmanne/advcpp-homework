#ifndef MENU_H
#define MENU_H

#include "Util.h"
#include "Database.h"

#include <vector>
#include <iostream>

namespace advcppfinal {
	namespace v1 {
		/* Auto create Menus at compile time using a list of form types.
		   Any type can be a From_Type if it has the required static
		   function and member */
		template <typename... Form_Types>
		class Menu {
		public:
			/* This Template argument is used to specify if it stays in the menu
			   after one operation. The logic for this is also done at compile
			   time itself using templates and if constexpr. */
			template<bool loop = false>
			static void show(Database& db) {
				int choice;
				int menu_size = pack_size<Form_Types...>::size;
				if constexpr (loop) {
					while (true) {
						std::cout << menu_concat(Form_Types::text..., "Exit").data();
						choice = ask_field<int>("choice");
						if (choice == menu_size) break;
						call_ith<Form_Types...>::call(choice, db);
					}
				}
				else {
					std::cout << menu_concat(Form_Types::text...).data();
					choice = ask_field<int>("choice");
					call_ith<Form_Types...>::call(choice, db);
				}
			}
		};
	};
	using namespace v1;
};


#endif // MENU_H