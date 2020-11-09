//
// Created by aurailus on 17/12/18.
//

#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"

Renderer::Renderer() : Renderer({ 1366, 768 }) {};

Renderer::Renderer(glm::ivec2 win) :
	activeTexture(nullptr),
	window(win),
	
	world(win, 2),
	entity(win, 2),
	ssao(win, 1, 24),
	blur(win, 1),
	light(win, 2) {
	
	camera.create(window.getSize().x, window.getSize().y, glm::vec3(0, 1, 0));
	
	ssao.createFromFile("../assets/shader/post/passThrough.vs", "../assets/shader/post/ssaoCalc.fs");
	blur.createFromFile("../assets/shader/post/passThrough.vs", "../assets/shader/post/ssaoBlur.fs");
	light.createFromFile("../assets/shader/post/passThrough.vs", "../assets/shader/post/deferredLighting.fs");
	world.createFromFile("../assets/shader/world/world.vs", "../assets/shader/world/world.fs",
		"../assets/shader/world/world.gs");
	entity.createFromFile("../assets/shader/world/entity.vs", "../assets/shader/world/entity.fs");
	
	guiShader = Shader();
	guiShader.createFromFile("../assets/shader/ortho/hud.vs", "../assets/shader/ortho/hud.fs");
	
	gu.matrix = camera.getOrthographicMatrix();
	gu.ortho = guiShader.get("ortho");
	gu.model = guiShader.get("model");
	gu.bones = guiShader.get("uBones");
	gu.clipBounds = guiShader.get("uClipBounds");
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	window.addResizeCallback("renderer", [&](glm::ivec2 win) {
		ssao.windowResized(win);
		blur.windowResized(win);
		light.windowResized(win);
		world.windowResized(win);
		
		camera.changeWindowDimensions(win);
		
		gu.matrix = camera.getOrthographicMatrix();
	});
}

void Renderer::update(double delta) {
	//VSync 1 = On, 0 = Off
	glfwSwapInterval(1);
	elapsedTime += delta;
	
	window.update();
	world.updateSwayMap(delta);
}

void Renderer::beginChunkDeferredCalls() {
	activeTexture = nullptr;
	currentModelUniform = world.uniforms.model;
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	
	glViewport(0, 0, static_cast<int>(world.windowSize.x * world.bufferScale),
		static_cast<int>(world.windowSize.y * world.bufferScale));
	
	glBindFramebuffer(GL_FRAMEBUFFER, light.gBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	const float skyColor[] = { clearColor.x, clearColor.y, clearColor.z, 1 };
	static const float clearTransparent[] = { 0, 0, 0, 1 };
	glClearBufferfv(GL_COLOR, 0, clearTransparent);
	glClearBufferfv(GL_COLOR, 1, clearTransparent);
	glClearBufferfv(GL_COLOR, 2, skyColor);
	
	setShader(world);
	world.set(world.uniforms.proj, camera.getProjectionMatrix());
	world.set(world.uniforms.view, camera.getViewMatrix());
	world.set(world.uniforms.time, static_cast<float>(elapsedTime));
	world.swayTex.use(1);
}

void Renderer::beginEntityDeferredCalls() {
	currentModelUniform = entity.uniforms.model;
	
	setShader(entity);
	entity.set(entity.uniforms.proj, camera.getProjectionMatrix());
	entity.set(entity.uniforms.view, camera.getViewMatrix());
}

void Renderer::endDeferredCalls() {
	activeTexture = nullptr;
	
	glBindFramebuffer(GL_FRAMEBUFFER, ssao.fbo);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	setShader(ssao);
	ssao.set(ssao.uniforms.proj, camera.getProjectionMatrix());
	ssao.set(ssao.uniforms.view, camera.getViewMatrix());
	ssao.set(ssao.uniforms.kernelCount, ssao.kernelCount);
	
	for (unsigned int i = 0; i < ssao.kernelCount; i++) {
		GLint uni = ssao.get("samples[" + std::to_string(i) + "]");
		ssao.set(uni, ssao.kernels[i]);
	}
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, light.gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, light.gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, ssao.tex);
	
	renderQuad();
	
	auto winSize = window.getSize();
	glViewport(0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y));
	
	glBindFramebuffer(GL_FRAMEBUFFER, blur.fbo);
	glClear(GL_COLOR_BUFFER_BIT);
	setShader(blur);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssao.colorBuffer);
	renderQuad();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setShader(light);
	light.set(light.uniforms.camPosition, camera.getPos());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, light.gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, light.gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, light.gColorSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, blur.colorBuffer);
	
	glEnable(GL_BLEND);
	renderQuad();
}

void Renderer::beginGUIDrawCalls() {
	activeTexture = nullptr;
	currentModelUniform = gu.model;
	
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	
	setShader(guiShader);
	guiShader.set(gu.ortho, gu.matrix);
}

void Renderer::swapBuffers() {
	Shader::clearShader();
	window.swapBuffers();
}

void Renderer::setShader(Shader& s) {
	s.use();
	this->currentShader = &s;
}

void Renderer::setClearColor(unsigned char r, unsigned char g, unsigned char b) {
	clearColor = { static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f, 1 };
}

void Renderer::toggleDepthTest(bool enable) {
	enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Renderer::clearDepthBuffer() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::setModelMatrix(const glm::mat4& modelMatrix) {
	glUniformMatrix4fv(currentModelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Renderer::setBones(std::vector<glm::mat4>& transforms) {
	if (transforms.empty()) return;
	currentShader->setArr((currentShader == &entity ? entity.uniforms.bones : gu.bones),
		static_cast<GLsizei>(transforms.size()), transforms.at(0));
}

void Renderer::setClipBounds(glm::vec4 bounds) {
	guiShader.set(gu.clipBounds, { bounds.x, window.getSize().y - bounds.w, bounds.z, window.getSize().y - bounds.y });
}

void Renderer::enableTexture(Texture* texture) {
	if (texture != activeTexture) {
		activeTexture = texture;
		texture->use(0);
	}
}

void Renderer::renderQuad() {
	if (quadVAO == 0) {
		float quadVertices[] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	}
	
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

