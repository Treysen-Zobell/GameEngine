
// OpenGL
#include <glad\glad.h>
#include <glfw\glfw3.h>

#include <openAL/al.h>
#include <openAL/alc.h>

// Standard
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <map>

// Include Libraries
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "stb_image.h"

// Headers
#include "TessellationShader.h"
#include "FrameBufferObject.h"
#include "ReflectionShader.h"
#include "OpenALFunctions.h"
#include "OpenGLFunctions.h"
#include "DisplayManager.h"
#include "StaticShader.h"
#include "NormalShader.h"
#include "SkyboxShader.h"
#include "SkyboxModel.h"
#include "BSDFShader.h"
#include "FPSShader.h"
#include "FpsModel.h"
#include "Listener.h"
#include "Texture.h"
#include "Source.h"
#include "Vertex.h"
#include "Shader.h"
#include "Config.h"
#include "Camera.h"
#include "Debug.h"
#include "Maths.h"
#include "Model.h"
#include "Light.h"
#include "Mesh.h"

int main() {
	Loader::loadSceneJSON("Resources/TestScene/testScene.json");
	Config::loadConfigs("Settings/settings.ini");

	Listener listener = Listener();

	Source source1 = Source("Resources/audio/ambientMono.wav", glm::vec3(-4.25f, 0.125f, -4.25f), glm::vec3(0.0f), 1.0f, 1.0f, 0.0f, 10.0f, 1.0f, AL_FALSE);
	Source source2 = Source("Resources/audio/heavy.wav");

	DisplayManager::createDisplay(Config::Display::WIDTH, Config::Display::HEIGHT);

	Shader shader = Shader(
		"Shaders/Shader/shader.vert",
		"Shaders/Shader/shader.frag");

	BSDFShader bsdfShader = BSDFShader(
		"Shaders/BSDFShader/shader.vert",
		"Shaders/BSDFShader/shader.frag");

	StaticShader staticShader = StaticShader(
		"Shaders/LightShader/shader.vert", 
		"Shaders/LightShader/shader.frag");

	NormalShader normalShader = NormalShader(
		"Shaders/NormalShader/shader.vert",
		"Shaders/NormalShader/shader.frag");

	TessellationShader tessShader = TessellationShader(
		"Shaders/tessellationShader/vertex.vert",
		"Shaders/tessellationShader/fragment.frag",
		"Shaders/tessellationShader/control.tesc",
		"Shaders/tessellationShader/evaluation.tese");
		// "shaders/tessellationShader/geometry.geom");

	ReflectionShader reflectionShader = ReflectionShader(
		"Shaders/reflectionShader/shader.vert",
		"Shaders/reflectionShader/shader.frag");

	SkyboxShader skyboxShader = SkyboxShader(
		"Shaders/skyboxShader/shader.vert",
		"Shaders/skyboxShader/shader.frag");

	FPSShader fpsShader = FPSShader(
		"Shaders/fpsShader/vertexShaderFPS.vert",
		"Shaders/fpsShader/fragmentShaderFPS.frag");

	Light light(glm::vec3(60.0f, 100.0f, 100.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	FpsModel fpsModel = FpsModel();
	SkyboxModel skyboxModel = SkyboxModel("Resources/skyboxDay");

	// Model model = Model("Resources/BSDFTest/test0001.obj");
	Model model = Model("Resources/TestScene/testScene.obj");

	DisplayManager::hideCursor();
	//DisplayManager::showCursor();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	FrameBufferObject fbo = FrameBufferObject();
	FrameBufferObject fbo2 = FrameBufferObject();

	source1.play();

	while (!glfwWindowShouldClose(DisplayManager::window) && !glfwGetKey(DisplayManager::window, GLFW_KEY_ESCAPE)) {

		if (glfwGetKey(DisplayManager::window, GLFW_KEY_0))
			source2.play();
		source2.setPosition(Camera::position);

		Camera::move();

		listener.updatePosition();

		// Buffered Shader Cycle
		fbo.bind();
		shader.start();
		model.draw(shader, glm::mat4(1.0f));
		shader.stop();

		skyboxShader.start();
		skyboxModel.draw(skyboxShader);
		skyboxShader.stop();
		fbo.unbind();

		// Buffered Shader Cycle 2
		//GLuint tempTexture = model.meshes[1].textures[0].ID;
		//model.meshes[1].textures[0].ID = fbo.textureColorID;

		fbo2.bind();
		shader.start();
		model.draw(shader, glm::mat4(1.0f));
		shader.stop();

		skyboxShader.start();
		skyboxModel.draw(skyboxShader);
		skyboxShader.stop();
		fbo2.unbind();

		//model.meshes[1].textures[0].ID = tempTexture;

		// Clear Screen Buffers
		glCall(glClearColor, 0.0f, 1.0f, 1.0f, 1.0f);
		glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shader Cycle
		//tempTexture = model.meshes[1].textures[0].ID;
		//model.meshes[1].textures[0].ID = fbo2.textureColorID;

		bsdfShader.start();
		model.draw(bsdfShader, glm::mat4(1.0f));
		bsdfShader.stop();

		//model.meshes[1].textures[0].ID = tempTexture;

		// Skybox Shader Cycle
		skyboxShader.start();
		skyboxModel.draw(skyboxShader);
		skyboxShader.stop();

		// FPS Shader Cycle
		fpsModel.update();
		fpsModel.render(fpsShader);

		// Show Display Buffer
		DisplayManager::updateDisplay();
	}

	fbo.destroy();
	shader.cleanUp();
	staticShader.cleanUp();
	normalShader.cleanUp();
	tessShader.cleanUp();
	fpsShader.cleanUp();
	Loader::destroy();
	DisplayManager::closeDisplay();
}
