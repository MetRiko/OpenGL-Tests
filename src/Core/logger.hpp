#pragma once

#include <iostream>
#include <sstream>
#include <utility>

namespace gl {

	struct {

		std::stringstream ss;

		template<typename... Args>
		void log(Args&&... args) {
			(ss << ... << std::forward<Args>(args));
			(std::cerr << ... << std::forward<Args>(args));
		}

		void print() {
			std::cerr << get();
		}

		std::string get() {
			return ss.str();
		}

	} Logger;

}