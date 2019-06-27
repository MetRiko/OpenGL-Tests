#pragma once

#include <array>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string_view>
#include <unordered_map>

#include <glm/vec3.hpp>

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

	// std::string Shader::DEFAULT_VERTEX_SHADER = "asd";
	// std::string Shader::DEFAULT_FRAGMENT_SHADER = "asd";

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

		// Uniforms

		void defineUniform(const std::string& name) {
			uniforms[name] = glGetUniformLocation(programId, name.c_str());
		}

		void setUniform(const std::string& name, const glm::vec3& arg) {
			bind();
			glUniform3f(uniforms[name], arg.x, arg.y, arg.z);
		}

		// Conversion

		operator GLuint() {
			return programId;
		}
	};
}