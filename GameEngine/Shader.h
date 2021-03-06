#pragma once

#include "ShaderProgram.h"

class Shader : public ShaderProgram {
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;

	Shader();

	Shader(std::string vertexShaderFilename, std::string fragmentShaderFilename);

	~Shader();

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(glm::mat4 matrix);

	void loadProjectionMatrix(glm::mat4);

	void loadViewMatrix();
};
