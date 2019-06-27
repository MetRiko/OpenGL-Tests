#pragma once

#include <array>
#include <iostream>
#include <functional>

#include <glm/vec3.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace gl {

	struct RenderObject {

	};

	struct Canvas {

	};

	struct Engine {

		Window* window = nullptr;

		std::function<void()> eventsFunction;
		std::function<void(double)> updateFunction;
		std::function<void()> renderFunction;

		float deltaTime = 0.016667;

		void setup(Window& window) {
			this->window = &window;

			while(isWindowOpen()) {

				// FPS + Delta time
				updateFPS();

				// Events
				checkEscapeButton();
				eventsFunction();

				// Update
				updateFunction(deltaTime);

				// Render
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				renderFunction();

				// Finish
				glfwPollEvents();
				glfwSwapBuffers(window);
			}
		}

		void updateFPS() {
			static double prev = glfwGetTime();
			static int frame_count;
			static char title[128];

			double curr = glfwGetTime();
			deltaTime = curr - prev;
			// if (deltaTime > 0.25) {
			if (deltaTime > 0.016667) {
				prev = curr;
				double fps = frame_count / deltaTime;
				sprintf(title, " @ FPS: %.2f", fps);
				glfwSetWindowTitle(*window, (window->title + title).c_str());
				frame_count = 0;
			}
			frame_count++;
		}

		bool isWindowOpen() {
			return !glfwWindowShouldClose(*window);
		}

		void checkEscapeButton() {
			if (glfwGetKey(*window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(*window, true);
		}

		void quit() {
			glfwSetWindowShouldClose(*window, true);
		}

		void setFunctions(auto events, auto update, auto render) {
			eventsFunction = events;
			updateFunction = update;
			renderFunction = render;
		}
	};
}