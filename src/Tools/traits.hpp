#pragma once

#include <type_traits>

#include <glm/vec2.hpp>

namespace gl::traits {

	// Remove reference and qualifiers from <T>

	template <class T>
	struct remove_cvref {
		using type = std::remove_cv_t<std::remove_reference_t<T>>;
	};
	template <class T> using remove_cvref_t = typename remove_cvref<T>::type;
}