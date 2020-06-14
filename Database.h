#ifndef DATABASE_H
#define DATABASE_H

#include "Util.h"
#include "ctre.hpp"
#include "factory.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <type_traits>
#include <fstream>

namespace advcppfinal {
	namespace v1 {
		/* Has most of the code related to data and structures. */

		/* Enum for returning login status */
		enum class login_status {
			failed, student, admin
		};

		/* Tags used to differentiate between student and admin accounts */
		struct student_account_tag {};
		struct admin_account_tag {};

		/* The account structures. They don't inherit, so I can use Type
		   Erasure for common things. */
		struct StudentAccount {
			std::string username;
			std::string password;
			int grad_year;
		};

		struct AdminAccount {
			std::string username;
			std::string password;
		};

		/* The structure for books, has an inheritence structure, is created using a 
		   factory, and has a visitor that I use for printing information. */
		struct AcademicBook;
		struct FictionBook;
		struct BookVisitor {
			virtual void visit(AcademicBook*) = 0;
			virtual void visit(FictionBook*) = 0;
		};
		struct Book {
			std::string name;
			std::string author;
			std::string genre;
			std::string location;
			Book(std::string name, std::string author, std::string genre, std::string location) :
				name(std::move(name)), author(std::move(author)), genre(std::move(genre)), location(std::move(location)) {}
			virtual ~Book() = default;
			inline virtual int cost() { return 10; }
			inline virtual void accept(BookVisitor&) {}
		};
		struct AcademicBook : public Book {
			using Book::Book;
			inline int cost() override { return 0; }
			inline void accept(BookVisitor& v) override { v.visit(this); }
		};
		struct FictionBook : public Book {
			using Book::Book;
			using Book::cost;
			using Book::accept;
			inline void accept(BookVisitor& v) override { v.visit(this); }
		};
		using factory = flexible_abstract_factory<Book(std::string, std::string, std::string, std::string)>;
		using academic_factory = concrete_factory<factory, AcademicBook>;
		using fiction_factory = concrete_factory<factory, FictionBook>;
		struct InfoPrintBookVisitor : public BookVisitor {
			virtual void visit(AcademicBook* book) {
				IndentStream incout(std::cout);
				incout << "Academic Book:" << indent << std::endl;
				incout << "Name: " << book->name << std::endl;
				incout << "Author: " << book->author << indent << std::endl;
				incout << "Subject: " << book->genre << unindent << std::endl;
				incout  << "Location: " << book->location << unindent << std::endl;
			}
			virtual void visit(FictionBook* book) {
				IndentStream incout(std::cout);
				incout << "Fiction Book:" << indent << std::endl;
				incout << "Name: " << book->name << std::endl;
				incout << "Author: " << book->author << std::endl;
				incout << "Genre: " << book->genre << std::endl;
				incout << "Location: " << book->location << unindent << std::endl;
			}
		};

		/* The common class for Accounts, uses Type Erasure and checks for password,
		   and returns based on the type of account. */
		class AuthChk {
		private:
			struct AbsAccount {
				virtual login_status check(std::string_view) const = 0;
				virtual ~AbsAccount() = default;
			};
			template<typename T>
			struct ConcAccount : AbsAccount {
				T const& account;
				ConcAccount(T const& account) : account(account) {}
				login_status check(std::string_view input_password) const override {
					if (account.password.compare(input_password) == 0) {
						if constexpr (std::is_same_v<T, StudentAccount>) return login_status::student;
						else return login_status::admin;
					}
					return login_status::failed;
				}
			};
			std::unique_ptr<AbsAccount> account;
		public:
			template<typename T>
			AuthChk(T const& account) {
				this->account = std::make_unique<ConcAccount<T>>(account);
			}
			inline login_status check(std::string_view input_password) const {
				return account->check(input_password);
			}

		};

		/* Main database that stores all information. The main student and admin accounts are stored in
		   different vectors since they are of different types. These vectors are not used for anything
		   except data storage, which is why I prefereed vector, since it has the best dynamic size
		   contigous insert at end. I have also used a map to combine both the types of accounts. This
		   map only stores a reference to the correct member by username, and not actually a copy again. 
		   The books are also stored in a vector, since I have to iterate over all of them when searching
		   and vector is contigous which is generally good for cache performance. */
		class Database {
		private:
			std::vector<std::unique_ptr<StudentAccount>> student_accounts;
			std::vector<std::unique_ptr<AdminAccount>> admin_accounts;
			std::map<std::string_view, AuthChk, std::less<>> pass_check;
			std::unique_ptr<factory> academic_book_factory = std::make_unique<academic_factory>();
			std::unique_ptr<factory> fiction_book_factory = std::make_unique<fiction_factory>();
			std::vector<std::unique_ptr<Book>> books;
		public:
			inline void create_account(std::string username, std::string password, std::string type) {
				if (pass_check.find(username) != pass_check.end()) {
					std::cout << "Username Already Exists" << std::endl;
					return;
				}
				if (type.compare("Student") == 0 ||
					type.compare("student") == 0) {
					auto grad_year = ask_field<int>("Graduation Year");
					student_accounts.push_back(make_unique_with_aggregate<StudentAccount>(
						std::move(username), std::move(password), std::move(grad_year)));
					auto const& account = *student_accounts.back();
					pass_check.emplace(account.username, account);
				}
				else if (type.compare("Admin") == 0 ||
					type.compare("admin") == 0) {
					admin_accounts.push_back(make_unique_with_aggregate<AdminAccount>(
						std::move(username), std::move(password)));
					auto const& account = *admin_accounts.back();
					pass_check.emplace(account.username, account);
				}
				else {
					std::cout << "Invalid Choice" << std::endl;
				}
			}

			inline login_status login(std::string username, std::string password) {
				if (pass_check.find(username) == pass_check.end()) {
					std::cout << "Incorrect Username" << std::endl;
					return login_status::failed;
				}
				login_status login_check = pass_check.at(username).check(std::move(password));
				if (login_check == login_status::failed) std::cout << "Incorrect Password" << std::endl;
				return login_check;
			}

			/* CTRE used to read book information from CSV file of format
			   <Name>,<Author>,<Fiction|Academic>,<Genre>,<Location>.
			   Since this won't change, CTRE is a much better choice than std::regex. */
			static constexpr auto books_file_regex = ctll::fixed_string{ "([^,]+),([^,]+),(Fiction|Academic),([^,]+),([^,]+)" };
			inline void add_books_from_file(std::string filename) {
				std::ifstream file(filename);
				if (file.fail()) {
					std::cout << "File " << filename << " Error" << std::endl;
					return;
				}
				std::string line;
				while (std::getline(file, line)) {
					auto match = ctre::match<books_file_regex>(line);
					if (match) {
						if (match.get<3>().str().compare("Fiction") == 0) {
							books.push_back(fiction_book_factory->create<Book>(match.get<1>().str(), match.get<2>().str(),
								match.get<4>().str(), match.get<5>().str()));
						}
						else {
							books.push_back(academic_book_factory->create<Book>(match.get<1>().str(), match.get<2>().str(),
								match.get<4>().str(), match.get<5>().str()));
						}
					}
				}
			}

			inline void search_books_by_name(std::string name) {
				InfoPrintBookVisitor v;
				std::function<std::string(decltype(books.begin()))> key = [](auto book) { return (*book)->name; };
				std::function<void(decltype(books.begin()))> print = [&v](auto book) { (*book)->accept(v); };
				iterator_regex_search(books.begin(), books.end(), std::move(name), key, print);
			}

			inline void search_books_by_author(std::string author) {
				InfoPrintBookVisitor v;
				std::function<std::string(decltype(books.begin()))> key = [](auto book) { return (*book)->author; };
				std::function<void(decltype(books.begin()))> print = [&v](auto book) { (*book)->accept(v); };
				iterator_regex_search(books.begin(), books.end(), std::move(author), key, print);
			}

		};
	};
	using namespace v1;
};


#endif // DATABASE_H