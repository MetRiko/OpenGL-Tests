#pragma once

#include <array>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string_view>
#include <unordered_map>
#include <type_traits>
#include <tuple>
#include <utility>
#include <functional>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Core/logger.hpp>


namespace gl {

	struct Shader {

		constexpr static const char* DEFAULT_VERTEX_SHADER = 
			"#version 400\n"
			"in vec3 vp;"
			"void main() {"
			"	gl_Position = vec4(vp, 1.0);"
			"}";

		constexpr static const char* DEFAULT_FRAGMENT_SHADER =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"	frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
			"}";

		static std::string getDefaultVertexSource() {
			return std::string(DEFAULT_VERTEX_SHADER); 
		}
		static std::string getDefaultFragmentSource() {
			return std::string(DEFAULT_FRAGMENT_SHADER); 
		}

		enum {
			Vertex, Fragment
		};

		unsigned int shaderId;

		bool compiled = false;

		unsigned int type = 0; 
		std::string src;

		Shader() {

		}

		Shader(const std::string& filename, bool setup = true) {
			load(filename);
			if (setup == true)
				this->setup();
		}

		Shader(const std::string& filename, int type, bool setup = true) {
			load(filename, type);	
			if (setup == true)
				this->setup();		
		}

		~Shader() {
			glDeleteShader(shaderId);
		}

		const std::string& getSource() {
			return src;
		}

		void create() {
			if (type == Vertex)
				shaderId = glCreateShader(GL_VERTEX_SHADER);
			else if (type == Fragment)
				shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		}

		void compile() {
			const char *str = src.c_str();
			glShaderSource(shaderId, 1, &str, NULL);
			glCompileShader(shaderId);

			compileDebugInfo();
		}

		void setup() {
			create();
			compile();
		}

		void compileDebugInfo() {
			int  success;
			char infoLog[512];
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

			if(not success)
			{
			    glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			    if (type == Vertex) 
				    gl::Logger.log("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", infoLog, '\n');
				else if (type == Fragment) 
				    gl::Logger.log("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", infoLog, '\n');
			}
		}

		void load(const std::string_view& filepath, int type = -1) {
			std::ifstream file;
			file.open(filepath.data());
			if (file.good()) 
			{
				if (type == -1) {					
					std::string_view ext = filepath.substr(filepath.find_last_of('.'));
					// gl::Logger.log(ext, "\n");
					if (ext == ".vert")
						this->type = Vertex;
					else if (ext == ".frag")
						this->type = Fragment;
				}
				else {
					this->type = type;
				}

				std::stringstream ss;
				ss << file.rdbuf();
				this->src = ss.str();

				// gl::Logger.log(this->src);

				file.close();
			}
			else
				gl::Logger.log("ERROR::SHADER::LOADING_SHADER_FILE ", filepath);
		}

		void loadAndSetup(const std::string_view& filepath, int type = -1) {
			load(filepath, type);
			setup();
		}

		void source(unsigned int type, const std::string& src) {
			this->type = type;
			this->src = src;
		}

		void source(unsigned int type) {
			this->type = type;
			if (type == Vertex)
				this->src = getDefaultVertexSource();
			else if (type == Fragment)
				this->src = getDefaultFragmentSource();
		}

		operator GLuint() {
			return shaderId;
		}
	};

	struct ShaderProgram {
		unsigned int programId;

		bool created = false;

		std::unordered_map<std::string, unsigned int> uniforms;

		void create() {
			programId = glCreateProgram();
			created = true;
		}

		template<typename... Args>
		void attach(Args&&... shaders) {
			
			if (not created) 
				create();

			glAttachShader(programId, (std::forward<Args>(shaders), ...));
		}

		void link() {			
			glLinkProgram(programId);

			linkDebugInfo();
		}

		// Load shaders from path : with auto deduction type [.frag/.vert]
		template<typename... Files>
		void loadAndAttach(Files&&... files) {	

			auto func = [this](auto&& filename){
				gl::Shader shader;
				shader.load(std::forward<decltype(filename)>(filename));
				shader.setup();
				attach(shader);
			};
			(func(std::forward<Files>(files)), ...);
		}

		template<typename... Files>
		void loadAndSetup(Files&&... files) {	
			loadAndAttach(files...);
			setup();
		}

		void setup() {
			link();
		}

		void linkDebugInfo() {
			int  success;
			char infoLog[512];

			glGetProgramiv(programId, GL_LINK_STATUS, &success);
			if(not success) {
			    glGetProgramInfoLog(programId, 512, NULL, infoLog);
			    gl::Logger.log("ERROR::SHADER_PROGRAM::LINK_FAILED\n", infoLog, '\n');
			}
		}

		void bind() {
			glUseProgram(programId);
		}

		// - - - Uniforms - - - //

		template <typename... Strings>
		void defineUniforms(Strings&&... names) {

			(uniforms.emplace(std::forward<Strings>(names), 
				glGetUniformLocation(programId, std::data(std::forward<Strings>(names)))), ...);

			// * * * Debug purposes * * * //
			// for (auto& key : uniforms) {
			// 	gl::Logger.log(key.first, ", ");
			// }
			// gl::Logger.log(":\n");
		}

		// Set uniform by passing : {a,b,c,...}
		template <typename String, typename Type, size_t Size, typename Indices = std::make_index_sequence<Size>>
		void uniform(String&& name, Type (&&list)[Size]) {
			_uniformAddr(std::forward<String>(name), std::forward<Type*>(list), Indices{});			
		}

		// Set uniform by passing : glm::vec
		template <typename String, int Size, typename Type, glm::qualifier Precision, typename Indices = std::make_index_sequence<Size>>
		void uniform(String&& name, glm::vec<Size, Type, Precision>&& vec) {
			_uniformAddr(std::forward<String>(name), std::forward<Type*>((Type*)&vec), Indices{});
		}

		// [support] Set uniform by passing : address of any struct (etc. struct{Type,Type,...})
		template <typename String, typename Type, std::size_t... I>
		void _uniformAddr(String&& name, Type*&& addr, std::index_sequence<I...>) {
			uniform(std::forward<String>(name), std::forward<Type>(*(addr + I))...);
		}

		// Set uniform by passing : args...
		template <typename String, typename... Types>
		void uniform(String&& name, Types&&... values) {

			// * * * Debug purposes * * * //
			// gl::Logger.log((typeid(values).name())...);
			// gl::Logger.log(values...);

			bind();

			using type = typename std::tuple_element<0, std::tuple<Types...>>::type;
			constexpr auto size = sizeof...(Types);
			
			unsigned int loc = uniforms[std::data(std::forward<String>(name))];


			// * * * Debug purposes * * * //
			auto debugLog = [&values...](auto&& sufix){
				gl::Logger.log("glUniform", sufix, " : ");
				for (auto&& v : {values...})
					gl::Logger.log(' ', v);
				gl::Logger.log('\n');
			};

			if constexpr (std::is_floating_point_v<type>) {
				if constexpr (size == 1) 		{ glUniform1f(loc, values...); debugLog("1f"); }
				else if constexpr (size == 2) 	{ glUniform2f(loc, values...); debugLog("2f"); }
				else if constexpr (size == 3) 	{ glUniform3f(loc, values...); debugLog("3f"); }
				else if constexpr (size == 4) 	{ glUniform4f(loc, values...); debugLog("4f"); }
			}
			else if constexpr (std::is_integral_v<type>) {
				if constexpr (std::is_unsigned_v<type>) { 
					if constexpr (size == 1) 		{ glUniform1ui(loc, values...); debugLog("1u"); }
					else if constexpr (size == 2) 	{ glUniform2ui(loc, values...); debugLog("2u"); }
					else if constexpr (size == 3) 	{ glUniform3ui(loc, values...); debugLog("3u"); }
					else if constexpr (size == 4) 	{ glUniform4ui(loc, values...); debugLog("4u"); }
				}
				else {
					if constexpr (size == 1) 		{ glUniform1i(loc, values...); debugLog("1i"); }
					else if constexpr (size == 2) 	{ glUniform2i(loc, values...); debugLog("2i"); }
					else if constexpr (size == 3) 	{ glUniform3i(loc, values...); debugLog("3i"); }
					else if constexpr (size == 4) 	{ glUniform4i(loc, values...); debugLog("4i"); }
				}
			}
		}

		// - - - Conversion - - - //

		operator GLuint() {
			return programId;
		}
	};
}
