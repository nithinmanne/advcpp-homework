#ifndef FORM_H
#define FORM_H

#include "Util.h"
#include "Database.h"
#include "Menu.h"

#include <string>
#include <vector>
#include <string_view>

namespace advcppfinal {
	namespace v1 {
		/* This file consists of multiple form types, each of which expose
		   a static text member and ask method. */

		/* inline static variable and function have different meanings */

		struct SearchBookByNameForm {
			inline static char const text[] = "Search Book By Name";
			inline static void ask(Database& db) {
				auto name = ask_field<std::string>("Name Regex");
				db.search_books_by_name(std::move(name));
			}
		};

		struct SearchBookByAuthorForm {
			inline static char const text[] = "Search Book By Author";
			inline static void ask(Database& db) {
				auto author = ask_field<std::string>("Author Regex");
				db.search_books_by_author(std::move(author));
			}
		};
		
		struct AddBooksForm {
			inline static char const text[] = "Add Books From File";
			inline static void ask(Database& db) {
				auto filename = ask_field<std::string>("Filename");
				db.add_books_from_file(std::move(filename));
			}
		};

		inline void login_success(Database& db, student_account_tag) {
			using LoginMenu = Menu<SearchBookByNameForm, SearchBookByAuthorForm>;
			LoginMenu::show<false>(db);
		}
		inline void login_success(Database& db, admin_account_tag) {
			/* Giving Admins the ability to search Books as trivial as
			   simply adding the required SearchBook... type in this list. */
			using LoginMenu = Menu<AddBooksForm>;
			LoginMenu::show<true>(db);
		}

		struct LoginForm {
			inline static char const text[] = "Login";
			inline static void ask(Database& db) {
				auto username = ask_field<std::string>("Username");
				auto password = ask_field<std::string>("Password");
				login_status login_check = db.login(std::move(username), std::move(password));
				if (login_check == login_status::student) login_success(db, student_account_tag{});
				else if (login_check == login_status::admin) login_success(db, admin_account_tag{});
			}
		};

		struct RegisterForm {
			inline static char const text[] = "Register";
			inline static void ask(Database& db) {
				auto username = ask_field<std::string>("Username");
				auto password = ask_field<std::string>("Password");
				auto type = ask_field<std::string>("Type(Student/Admin)");
				db.create_account(std::move(username), std::move(password), std::move(type));
			}
		};
	};
	using namespace v1;
};


#endif // FORM_H