#pragma once

#include "Dragon.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include "Build_in/GameObjects.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

class ExampleLayer : public Dragon::Layer
{

public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0 / 720.f),skybox()
	{
		//Framebuffer
		Dragon::FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Dragon::Framebuffer::Create(spec);

		//point light init
		for (int i = 0; i < m_PointLightNum; i++)
		{
			Dragon::PointLight pointLight;
			m_PointLights.push_back(pointLight);
		}

		//skybox init
		m_ShaderLibrary.Load("skybox", "assets/shaders/SkyBox.glsl");
		auto skyBoxShader = m_ShaderLibrary.Get("skybox");
		skybox.SetFaces(m_Faces);
		skybox.Init();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->UploadUniformInt("u_SkyBox", 0);

		//screenQuad init
		m_ShaderLibrary.Load("screenQuadShader", "assets/shaders/PostEffect.glsl");
		auto screenQuadShader = m_ShaderLibrary.Get("screenQuadShader");
		screenQuad.Init();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(screenQuadShader)->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(screenQuadShader)->UploadUniformInt("u_ScreenTexture", 0);

		//cube init
		m_ShaderLibrary.Load("cubeShader", "assets/shaders/Blinn_PhongTexture.glsl");
		auto cubeShader = m_ShaderLibrary.Get("cubeShader");
		m_Albedo = Dragon::Texture2D::Create("assets/textures/wall.jpg");
		m_Albedo->Bind();
		skybox.GetCubeMap()->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformInt("u_Albedo", 0);
		//std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformInt("u_SkyBox", 0);
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformInt("u_ShadowMap", 1);

		glEnable(GL_MULTISAMPLE);
	}

	void OnUpdate(Dragon::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);
		ButtonClicked();
		auto cubeShader = m_ShaderLibrary.Get("cubeShader");
		auto skyBoxShader = m_ShaderLibrary.Get("skybox");
		auto screenQuadShader = m_ShaderLibrary.Get("screenQuadShader");

		glEnable(GL_DEPTH_TEST);
		//MVP
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

		m_Framebuffer->Bind();
		Dragon::RenderCommand::Clear();
		Dragon::Renderer::BeginScene();
		//GameObject Init
		for (auto gameObjectName : m_GameObjectsName)
		{
			auto gameObject = m_GameObjectLibrary.Get(gameObjectName);
			if (gameObject->GetType() == "Cube")
				std::dynamic_pointer_cast<Dragon::Cube>(gameObject)->Init();
			//cube
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->Bind();
			//CameraPos
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_CameraPos", m_CameraController.GetCamera().GetPosition());
			//Material
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat("u_Shininess", 32.0f);
			//std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_Albedo", glm::vec3(0.5f, 0.3f, 0.2f));
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat("u_Specular", 0.5f);
			//MVP
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformMat4("u_Model", gameObject->GetModel());
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformMat4("u_Projection", m_CameraController.GetCamera().GetProjectionMatrix());
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformMat4("u_View", m_CameraController.GetCamera().GetViewMatrix());
			//dir light
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformMat4("u_LightSpaceMatrix", m_DirLight.GetLightSpaceMatrix());
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_DirLight.direction", glm::vec3(-1.0f, -1.0f, 0.0));
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_DirLight.diffuse", m_DirLight.GetDiffuse());
			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_DirLight.specular", m_DirLight.GetSpecular());

			std::dynamic_pointer_cast<Dragon::OpenGLShader>(cubeShader)->UploadUniformFloat3("u_Ambient", glm::vec3(0.5f, 0.3f, 0.2f));

			m_Albedo->Bind();
			skybox.GetCubeMap()->Bind();
		}
		//GameObject Rendering
		for (auto gameObjectName : m_GameObjectsName)
		{
			auto gameObject = m_GameObjectLibrary.Get(gameObjectName);
			if (gameObject->GetType() == "Cube")
				std::dynamic_pointer_cast<Dragon::Cube>(gameObject)->Render();
		}
		Dragon::Renderer::EndScene();
		

		//skybox
		glDepthFunc(GL_LEQUAL);
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->Bind();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->UploadUniformMat4("u_Projection", m_CameraController.GetCamera().GetProjectionMatrix());
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(skyBoxShader)->UploadUniformMat4("u_View", glm::mat4(glm::mat3(m_CameraController.GetCamera().GetViewMatrix())));
		skybox.Render();
		glDepthFunc(GL_LESS);
		m_Framebuffer->Unbind();
		Dragon::Renderer::EndScene();

		Dragon::Renderer::BeginScene();
		glDisable(GL_DEPTH_TEST);
		Dragon::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		Dragon::RenderCommand::Clear();
		std::dynamic_pointer_cast<Dragon::OpenGLShader>(screenQuadShader)->Bind();
		m_Framebuffer->BindColorTexture();
		screenQuad.Render();
		Dragon::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Inspector");
		if(m_GameObjectsName.size()!=0)
			GameObjectGui(m_GameObjectsName[m_GameObjectsItem]);
		ImGui::NewLine();

		ImGui::Text("Shader List");
		
		const char** shaderList = StringToChar(m_ShaderList);
		ImGui::Combo("", &m_ShaderListItem, shaderList, m_ShaderList.size());
		ShaderGui(m_ShaderList[m_ShaderListItem]);


		ImGui::End();

		ImGui::Begin("Game Object");
		ImGui::Text("Geometry");
		m_CubeButtonClicked = ImGui::Button("Cube");
		ImGui::SameLine();
		ImGui::Button("Sphere");
		ImGui::SameLine();
		ImGui::Button("Quad");
		ImGui::Button("Import Model");
		ImGui::NewLine();
		ImGui::Text("Light Source");
		ImGui::Button("Point Light");
		ImGui::SameLine();
		ImGui::Button("Directional Light");
		ImGui::SameLine();
		ImGui::Button("Spot Light");
		ImGui::SameLine();
		ImGui::Button("Area Light");
		ImGui::Button("Sky Box");
		ImGui::NewLine();
		ImGui::Button("Orthographic Camera");
		ImGui::SameLine();
		ImGui::Button("Perspective Camera");
		ImGui::End();

		ImGui::Begin("Outline View");
		const char** GameObjectList = StringToChar(m_GameObjectsName);
		ImGui::ListBox("", &m_GameObjectsItem, GameObjectList, m_GameObjectsName.size(), m_GameObjectsName.size());
		ImGui::End();

		ImGui::Begin("Console");
		ImGui::TextColored(ImVec4(1.0, 1.0, 1.0, 1.0), "lalala");
		ImGui::End();
	}

	void OnEvent(Dragon::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}
private:
	void ShaderGui(std::string shaderName)
	{
		ImGui::NewLine();
		ImGui::LabelText(shaderName.c_str(), "Shader Parameter");
		if (shaderName == "Lambert")
		{
			ImGui::ColorEdit3("Ambient", m_Color);
			ImGui::ColorEdit3("Diffuse", m_Color);
		}
		if (shaderName == "LambertTexture")
		{
			ImGui::ColorEdit3("Ambient", m_Color);
			ImGui::NewLine();
			ImGui::Text("Diffuse");
			ImGui::SameLine();
			testImage = Dragon::Texture2D::Create("assets/textures/wall.jpg");
			ImGui::ImageButton((ImTextureID)(intptr_t)testImage->GetTextureID(), ImVec2(100, 100));
		}
	}

	void GameObjectGui(std::string gameObjectName)
	{
		ImGui::Text("Transform");
		auto gameObject = m_GameObjectLibrary.Get(gameObjectName);
		float position[] = { gameObject->GetPosition().x, gameObject->GetPosition().y, gameObject->GetPosition().z };
		float rotation[] = { gameObject->GetRotation().x, gameObject->GetRotation().y, gameObject->GetRotation().z };
		float scale[] = { gameObject->GetScale().x, gameObject->GetScale().y, gameObject->GetScale().z };
		ImGui::DragFloat3("Position", position, 0.1f);
		ImGui::DragFloat3("Rotation", rotation, 0.1f);
		ImGui::DragFloat3("Scale", scale, 0.1f);
		gameObject->SetPosition(glm::vec3(position[0], position[1], position[2]));
		gameObject->SetRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));
		gameObject->SetScale(glm::vec3(scale[0], scale[1], scale[2]));
	}

	void ButtonClicked()
	{
		if (m_CubeButtonClicked)
		{
			std::shared_ptr<Dragon::Cube> cube = std::make_shared<Dragon::Cube>();
			cube->SetName("Cube" + std::to_string(m_CubeCounter));
			cube->SetType("Cube");
			m_GameObjectsName.push_back(cube->GetName());
			m_GameObjectLibrary.Add(cube->GetName(), cube);
			DG_INFO("{0} is created", m_GameObjectsName[m_CubeCounter]);
			m_CubeCounter++;
			m_CubeButtonClicked = false;
		}
	}

	const char** StringToChar(std::vector<std::string>& vector)
	{
		const char** list = new const char* [vector.size()];
		for (int i = 0; i < vector.size(); i++)
		{
			list[i] = new char[50];
		}
		for (int i = 0; i < vector.size(); i++)
		{
			list[i] = vector[i].c_str();
		}

		return list;
	}
private:
	//Framebuffer
	std::shared_ptr<Dragon::Framebuffer> m_Framebuffer;
	std::shared_ptr<Dragon::Framebuffer> m_ShadowFBO;

	//Shaders---------------------------------------
	Dragon::ShaderLibrary m_ShaderLibrary;
	std::vector<std::string> m_ShaderList = {
		"Lambert",
		"LambertTexture",
		"Phong",
		"PhongTexture",
		"Blinn",
		"BlinnTexture",
		"PBR",
		"PBRTexture",
		"AlphaTest",
		"AlphaBlending",
		"ShadowMapDir",
		"ShadowMapPoint",
		"SkyBox"
	};
	int m_ShaderListItem = 0;

	//Textures--------------------------------------
	std::shared_ptr<Dragon::Texture2D> m_Albedo;

	//GameObjects-----------------------------------
	Dragon::GameObjectLibrary m_GameObjectLibrary;
	Dragon::SkyBox skybox;
	std::vector<std::string> m_Faces = {
		"./assets/textures/skybox/right.jpg",
		"./assets/textures/skybox/left.jpg",
		"./assets/textures/skybox/top.jpg",
		"./assets/textures/skybox/bottom.jpg",
		"./assets/textures/skybox/front.jpg",
		"./assets/textures/skybox/back.jpg",
	};
	Dragon::ScreenQuad screenQuad;

	std::vector<std::string> m_GameObjectsName = {};
	int m_GameObjectsItem = 0;

	//cube button
	bool m_CubeButtonClicked = false;
	int m_CubeCounter = 0;

	//Textures--------------------------------------
	std::shared_ptr<Dragon::Texture2D> testImage;

	//Camera----------------------------------------
	Dragon::CameraController m_CameraController;
	Dragon::Camera m_Camera;

	//Light-----------------------------------------
	Dragon::DirLight m_DirLight;
	int m_PointLightNum = 1;
	std::vector<Dragon::PointLight> m_PointLights;

	bool isSelected = true;

	bool m_Check = false;
	float m_Metallic = 0.0f;
	float m_Roughness = 0.0f;
	float m_AO = 0.0f;
	float m_Color[3] = { 0.0f, 0.0f, 0.0f };
};
