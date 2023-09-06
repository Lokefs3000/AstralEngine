#include "pch.h"
#include "EdEngine.h"

#include "Data/WindowData.h"
#include "Graphics/Window.h"
#include "SDL3/SDL.h"
#include "glad/gl.h"
#include <iostream>
#include <Graphics/GL/GLGraphicsContext.h>
#include <Graphics/GL/Managers/GLTextureManager.h>
#include <Data/ContextData.h>
#include <Utlities/PlatformUtils.h>
#include <Resources/RawAssetManager.h>

#include "ImGui/imgui.h"

#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void EdEngine::InitializeLoadWindow()
{
	
}

void EdEngine::LoadProject(std::string dir)
{
	m_ProjectManager.release();
	
	m_ProjectAssetManager = std::make_shared<RawAssetManager>();
	m_ProjectAssetManager->Initialize("projects\\" + dir + "\\");

	m_ContentBrowser = std::make_unique<ContentBrowser>("projects\\" + dir + "\\assets\\", m_ProjectAssetManager, m_Textures);
}

EdEngine::EdEngine()
{
}

EdEngine::~EdEngine()
{
}

void EdEngine::Initialize(std::vector<std::string> Arguments)
{
	Log("Loading init window");

#pragma region GLInit
	WindowData initWindowDesc{};
	initWindowDesc.Width = 180 * 4;
	initWindowDesc.Height = 71 * 4;
	initWindowDesc.IsOpenGL = true;
	initWindowDesc.IsBorderless = true;
	initWindowDesc.IsResizable = false;
	initWindowDesc.Title = "Engine initialize";

	Window* initWindow = new Window(initWindowDesc);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GLContext glc = SDL_GL_CreateContext(initWindow->GetWindowCore());
	SDL_GL_MakeCurrent(initWindow->GetWindowCore(), glc);
	gladLoadGL(SDL_GL_GetProcAddress);

#pragma region Buffer
	uint32_t VAO, VBO;

	float vertices[6][4] = {
			{ -1.0f, 1.0f, 0.0f, 0.0f },
			{ -1.0f, -1.0f, 0.0f, 1.0f },
			{ 1.0f, -1.0f, 1.0f, 1.0f },
			{ -1.0f, 1.0f, 0.0f, 0.0f },
			{ 1.0f, -1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 0.0f }
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
#pragma endregion

#pragma region Graphic
	int w, h;
	stbi_uc* pixels = stbi_load("editor\\assets\\textures\\loading_base.png", &w, &h, NULL, 4);
	uint32_t Tex;
	glGenTextures(1, &Tex);
	glBindTexture(GL_TEXTURE_2D, Tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);
#pragma endregion

#pragma region Shaders
	const char* vs_src =
		"#version 460\n"
		"layout(location = 0) in vec4 v;\n"
		"layout(location = 0) out vec2 t;\n"
		"void main() {\n"
		"gl_Position = vec4(v.xy, 0.0, 1.0);\n"
		"t = v.zw;\n"
		"}";

	const char* fs_src =
		"#version 460\n"
		"layout(location=0)out vec4 f;\n"
		"layout(location=0)in vec2 t;\n"
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"f = texture(tex, t);\n"
		"}";

	uint32_t vs, fs;

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_src, NULL);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_src, NULL);
	glCompileShader(fs);

	uint32_t program;

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
#pragma endregion

#pragma region Render
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	SDL_GL_SwapWindow(initWindow->GetWindowCore());

	SDL_Event Event;
	while (SDL_PollEvent(&Event));
#pragma endregion

	glDeleteTextures(1, &Tex);
	glDeleteProgram(program);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	SDL_GL_DeleteContext(glc);
	gladLoaderUnloadGL();
#pragma endregion

	bool isWine = PlatformUtils::IsWine();

	Log("Initiating EdEngine");

	WindowData mainWinData{};
	mainWinData.Width = 1280;
	mainWinData.Height = 826;
	mainWinData.IsOpenGL = true;
	mainWinData.IsHidden = true;
	mainWinData.Title = "AstralEditor | " + std::string(isWine ? "Wine" : "Windows");

	m_MainWindow = std::make_shared<Window>(mainWinData);

	m_Context = std::make_shared<GLGraphicsContext>();

	ContextData ctxData{};
	ctxData.MainWindow = m_MainWindow;

	m_Context->Initialize(ctxData);

	m_AssetManager = std::make_shared<RawAssetManager>();
	m_AssetManager->Initialize("editor\\");

	m_Textures = std::make_shared<GLTextureManager>();
	m_Textures->Initialize(m_AssetManager);

	m_MainWindow->ShowWindow();

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	if (mainWinData.IsOpenGL) {
		ImGui_ImplSDL3_InitForOpenGL(m_MainWindow->GetWindowCore(), ((GLGraphicsContext*)m_Context.get())->GetContext());
		ImGui_ImplOpenGL3_Init("#version 460");
	}
	else {

	}

	std::string loadDef = "";

	for (size_t i = 0; i < Arguments.size(); i++)
	{
		if (Arguments[i] == "-open") {
			loadDef = Arguments[++i];
		}
	}

	if (loadDef.empty())
		m_ProjectManager = std::make_unique<ProjectManager>();
	else
		LoadProject(loadDef);

	Log("Removing init window");

#pragma region GLEnd
	delete initWindow;
#pragma endregion
}

void EdEngine::Run()
{
	Log("Running app");

	while (!m_MainWindow->IsClosed())
	{
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

		SDL_Event Event;
		while (SDL_PollEvent(&Event))
		{
			m_MainWindow->SendEvent(Event);
			ImGui_ImplSDL3_ProcessEvent(&Event);
		}

		m_Context->ClearFrame();

		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);

		if (m_ProjectManager.get() != NULL)
			m_ProjectManager->Render([this](std::string dir)->void {
			Log("Opening project: ", dir);
			LoadProject(dir);
				});
		if (m_ContentBrowser.get() != NULL)
			m_ContentBrowser->Render(m_DeltaTime);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		m_Context->PresentFrame();

		m_DeltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(std::chrono::system_clock::now() - start).count() / 1000.0f;
	}
}

void EdEngine::Shutdown()
{
	Log("Shutting down");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	m_Context->Shutdown();
}
