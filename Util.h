#ifndef UTIL_H
#define UTIL_H

#include <string_view>
#include <array>
#include <functional>
#include <utility>
#include <iostream>
#include <tuple>
#include <ostream>
#include <sstream>
#include <regex>

namespace advcppfinal {
	namespace v1 {

		/* This file contains algorithms independent of the database,
		   or other part of the program. */

		/* Find length of a variadic pack.
		   Used in Menu for finding last index for exit. */
		template<typename T, typename... Ts>
		struct pack_size {
			static size_t constexpr size = 1 + pack_size<Ts...>::size;
		};
		template<typename T>
		struct pack_size<T> {
			static size_t constexpr size = 1;
		};

		/* Compile time length of string, constexpr function
		   would have been more easier, but though it works in GCC,
		   MSVC doesn't let me use that as a template argument. */
		template<typename> struct cs_size;
		template <size_t len>
		struct cs_size<char[len]> {
			static size_t constexpr size = len - 1;
		};

		/* Copy a string into another at compile time after adding index.
		   Also updates the index and pointer using references, so its
		   easily used in fold expressions without adding something else */
		constexpr inline void copy(char*& ptr, char const str[], size_t len, int& index) {
			*ptr++ = static_cast<char>('0' + index++);
			*ptr++ = ':'; *ptr++ = ' ';
			for (size_t i = 0; i < len; i++) *ptr++ = *str++;
			*ptr++ = '\n';
		}

		/* Merges the strings at compile time and adds index */
		template <typename... Cstring>
		constexpr auto menu_concat(Cstring const&... cstring) {
			/* Fold Expressions */
			int constexpr len = (2 + ... + (4 + cs_size<Cstring>::size));
			std::array<char, len> array{};
			char* ptr = array.data();
			*ptr++ = '\n';
			int index = 0;
			((copy(ptr, cstring, cs_size<Cstring>::size, index)), ...);
			*ptr = '\0';
			return array;
		}

		/* Calls the ask method on the i-th type.
		   Why "ask"? Because thats what I need. */
		template<typename T, typename... Ts>
		struct call_ith {
			template<typename... Args>
			static void call(int i, Args&&... args) {
				if (i == 0) std::invoke(T::ask, std::forward<Args>(args)...);
				else call_ith<Ts...>::call(i - 1, std::forward<Args>(args)...);
			}
		};
		template<typename T>
		struct call_ith<T> {
			template<typename... Args>
			static void call(int i, Args&&... args) {
				if (i == 0) std::invoke(T::ask, std::forward<Args>(args)...);
				else std::cout << "Invalid Option" << std::endl;
			}
		};

		/* Helper function to get input from user, handles different types,
		   and doesn't break when used with integers */
		template<typename T>
		T ask_field(std::string_view name) {
			while (true) {
				if (!name.empty()) std::cout << "Enter " << name << ": ";
				T value;
				std::cin >> value;
				if (std::cin.fail()) {
					std::cout << "Invalid Entry, Try Again" << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				else return value;
			}
		}

		/* Factory, CTAD and Aggregate Initialization!!!!
		   To use aggragate initialization with make_unique
		   directly. Creates the type using aggragate
		   initialization when cast as part of make_unique. */
		template<typename... Args>
		struct aggregate_init {
			std::tuple<Args...> args;
			aggregate_init(Args&&... args) : args{ std::forward<Args>(args)... } {}
			template<typename T>
			explicit operator T() const {
				return std::apply([](Args... args) {
					return T{ std::forward<Args>(args)... };
					}, std::move(args));
			}
		};
		template<typename T, typename... Args>
		std::unique_ptr<T> make_unique_with_aggregate(Args&&... args) {
			return std::make_unique<T>(aggregate_init(std::forward<Args>(args)...));
		}

		/* Indent stream for printing with indents. */
		template<typename CharT = char, typename Traits = std::char_traits<CharT>>
		class IndentStreamBuf : public std::basic_stringbuf<CharT, Traits> {
		private:
			std::basic_ostream<CharT, Traits>& ostream;
			int indent_size;
			typename std::basic_string<CharT>::size_type indent_count = 0;
		public:
			IndentStreamBuf(std::basic_ostream<CharT, Traits>& ostream, int indent_size = 4) : ostream(ostream), indent_size(indent_size) {}
			int overflow(int ch) override {
				CharT char_ch = static_cast<CharT>(ch);
				ostream << char_ch;
				if (char_ch == '\n')
					ostream << std::basic_string<CharT>(indent_count * indent_size, ' ');
				return ch;
			}
			void indent() { indent_count++; }
			void unindent() { indent_count--; }
		};
		/* Deduction Guide for IndentStreamBuf */
		template<typename CharT, typename Traits>
		IndentStreamBuf(std::basic_ostream<CharT, Traits>&, int())->IndentStreamBuf<CharT, Traits>;

		template<typename CharT = char, typename Traits = std::char_traits<CharT>>
		class IndentStream : public std::basic_ostream<CharT, Traits> {
			IndentStreamBuf<CharT, Traits> streambuf;
		public:
			IndentStream(std::basic_ostream<CharT, Traits>& ostream, int indent_size = 4) : streambuf(ostream, indent_size), std::basic_ostream<CharT, Traits>(&streambuf) {}
		};
		/* Deduction Guide for IndentStream */
		template<typename CharT, typename Traits>
		IndentStream(std::basic_ostream<CharT, Traits>&, int())->IndentStream<CharT, Traits>;

		template<typename CharT = char, typename Traits = std::char_traits<CharT>>
		std::basic_ostream<CharT, Traits>& indent(std::basic_ostream<CharT, Traits>& os) {
			static_cast<IndentStreamBuf<CharT, Traits>*>(os.rdbuf())->indent();
			return os;
		}
		template<typename CharT = char, typename Traits = std::char_traits<CharT>>
		std::basic_ostream<CharT, Traits>& unindent(std::basic_ostream<CharT, Traits>& os) {
			static_cast<IndentStreamBuf<CharT, Traits>*>(os.rdbuf())->unindent();
			return os;
		}


		/* General Purpose funtion to regex search a field returned by key and print
		   using the print callable when there's a match. */
		template<typename It,
			typename = std::enable_if<std::is_same_v<typename std::iterator_traits<It>::iterator_category, std::forward_iterator_tag>>>
		void iterator_regex_search(It begin, It end, std::string regex_str, std::function<std::string(It)> key, std::function<void(It)> print) {
			/* Using std::regex, since its runtime */
			try {
				std::regex search_regex(regex_str);
				for (auto iter = begin; iter != end; iter++) {
					if (std::regex_search(key(iter), search_regex))
						print(iter);
				}
			}
			catch (std::regex_error e) {
				std::cout << "Regex Error" << std::endl;
			}
		}
	};
	using namespace v1;
};


#endif // UTIL_H