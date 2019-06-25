#pragma once

#include <iostream>

#include <glm/vec2.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Core/logger.hpp>

namespace gl {
	struct Window {
		int major = 0;
		int minor = 0;
		glm::vec2 screenSize {100, 100};
		std::string title;
	
		GLFWwindow* window = nullptr;

		~Window() {
			glfwTerminate();
		}

		operator GLFWwindow*() const {
			return window;
		}

		void setScreen(glm::vec2 screenSize, const std::string& title) {
			this->screenSize = screenSize;
			this->title = title;
		}

		void setVersion(int major, int minor) {
			// this->major = major;
			// this->minor = minor;
		}

		void setTitle(const std::string& title) {
			this->title = title;
		}

		void setup() {
			if (not glfwInit()) {
				gl::Logger.log("ERROR::GLFW::COULD_NOT_START\n");
				return;
			} 

			if (major and minor) {
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
			}
			else {
				// glGetIntegerv(GL_MAJOR_VERSION, &major); 
				// glGetIntegerv(GL_MINOR_VERSION, &minor); 
				// std::cout<<major<<' '<<minor<<std::endl;
				// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
				// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
			}
			// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			window = glfwCreateWindow(screenSize.x, screenSize.y, title.c_str(), nullptr, nullptr);
			if (window == nullptr) {
				gl::Logger.log("ERROR::WINDOW::FAILED_TO_CREATE_WINDOW\n");
				glfwTerminate();
				return;
			}
			glfwMakeContextCurrent(window);

			glViewport(0.f, 0.f, screenSize.x, screenSize.y);
			glfwSetFramebufferSizeCallback(window, []([[maybe_unused]] auto* window, int width, int height){
				glViewport(0.f, 0.f, width, height);
			});  

			// GLEW 

			glewExperimental = GL_TRUE;
			glewInit();

			const GLubyte* renderer = glGetString(GL_RENDERER);
			const GLubyte* version = glGetString(GL_VERSION);
			gl::Logger.log("Renderer: ", renderer, '\n');
			gl::Logger.log("OpenGL version supported: ", version, '\n');

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}
	};	
}