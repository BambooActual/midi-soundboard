#include "gui.h"


// Remove once lsp is fixed or further modifications are not needed!!
// #include "audioloader.h"
#include "audioloader.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "soundplayer.h"
#include <string>


#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int createWindow()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1080, 720, "Bamboo MIDI Soundboard", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
	bool ImportingSound = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// std::vector<std::string> PossibleOutputs;
	std::vector<std::string> PossibleOutputs = getOutputDevices();
	std::vector<std::string> LoadedSoundEffects = getSoundNames();


    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		ImVec2 ViewportSize = ImGui::GetMainViewport()->WorkSize;
		ImVec2 LogBoxSize = ImVec2(0, 150);
		float ContentRelation = 0.2f;

	// ===================================================================================== //
	// =============================== Sound Effects Window ================================ //
	// ===================================================================================== //

		ImGuiWindowFlags ToolWindowFlags = 0;
		ToolWindowFlags |= ImGuiWindowFlags_MenuBar;
		ToolWindowFlags |= ImGuiWindowFlags_NoTitleBar;
		ToolWindowFlags |= ImGuiWindowFlags_NoResize;
		ToolWindowFlags |= ImGuiWindowFlags_NoCollapse;
		ToolWindowFlags |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::SetNextWindowPos(ImVec2(ViewportSize.x * ContentRelation, 0));
		ImGui::SetNextWindowSize(ImVec2(ViewportSize.x * (1.0f - ContentRelation), ViewportSize.y - LogBoxSize.y));

        {
            ImGui::Begin("Test Window", NULL, ToolWindowFlags);

			ImGui::BeginMenuBar();
			if (ImGui::BeginMenu("Sounds"))
			{
				if (ImGui::MenuItem("Import"))
				{
					// Import Sound
					importSound();
					LoadedSoundEffects = getSoundNames();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Output"))
			{
				// PossibleOutputs = getOutputDevices();
				if (ImGui::BeginMenu("Microphone"))
				{
					for (int i = 0; i < PossibleOutputs.size(); i++)
					{
						std::string MicOutName = PossibleOutputs[i] + "##Mic";
						if (ImGui::MenuItem(MicOutName.c_str()))
						{
							std::cout << "New Microphone Output: " << PossibleOutputs[i].c_str() << '\n';

							changeConfig("MicrophoneOutput", PossibleOutputs[i].c_str());
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Playback"))
				{
					for (int i = 0; i < PossibleOutputs.size(); i++)
					{
						std::string PlaybackName = PossibleOutputs[i] + "##Playback";
						if (ImGui::MenuItem(PlaybackName.c_str()))
						{
							std::cout << "New Playback Device: " << PossibleOutputs[i].c_str() << '\n';

							changeConfig("PlaybackOutput", PossibleOutputs[i].c_str());
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();

			// if (ImGui::TreeNode("Test Tree"))
			// {
			// 	ImGui::SeparatorText("Text Number 1");
			// 	ImGui::SeparatorText("Text Number 2");
			// 	ImGui::SeparatorText("Text Number 3");
			//
			// 	ImGui::TreePop();
			// }

			ImGui::BeginTable("Test Table", 1);

			// List of Loaded sound effects.
			ImGui::TableNextColumn();

			ImU32 HeaderColor = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImU32 ButtonColor = ImGui::GetColorU32(ImVec4(0.6f, 0.3f, 0.1f, 1.0f));
			ImU32 ButtonColorHover = ImGui::GetColorU32(ImVec4(0.4f, 0.2f, 0.0f, 1.0f));
			ImU32 ButtonColorActive = ImGui::GetColorU32(ImVec4(0.2f, 0.1f, 0.0f, 1.0f));

			// Set Color of the following elements.
			ImGui::PushStyleColor(ImGuiCol_Button, ButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ButtonColorHover);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ButtonColorActive);
			ImGui::PushStyleColor(ImGuiCol_Header, HeaderColor);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ButtonColorHover);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ButtonColorActive);

			ImGui::Text("Sound Effects");

			ImVec2 CursorPosition = ImVec2(15, 55);
			int SoundAmount = LoadedSoundEffects.size();
			for (int i = 0; i < SoundAmount; i++)
			{
				ImGui::SetCursorPos(CursorPosition);
				if (ImGui::CollapsingHeader(LoadedSoundEffects[i].c_str()))
				{
					// On Sound effect Clicked
					CursorPosition.y += 25;
					CursorPosition.x += 20;
					ImGui::SetCursorPos(CursorPosition);

					std::string ButtonName;
					ButtonName = "Bind Key##";
					ButtonName += std::to_string(i);
					if (ImGui::Button(ButtonName.c_str(), ImVec2(90, 50)))
					{
						// Bind key to sound effect
					}

					CursorPosition.x += 100;
					CursorPosition.y += 30;
					ImGui::SetCursorPos(CursorPosition);

					std::string SoundOption1;
					SoundOption1 = "Remove##";
					SoundOption1 += std::to_string(i);
					if (ImGui::Button(SoundOption1.c_str(), ImVec2(80, 20)))
					{

					}
					CursorPosition.x -= 120;
					CursorPosition.y += 20;

				}

				CursorPosition.y += 20;
				// CursorPosition.x -= 20;
			}

			// Apply changes to prior elements.
			ImGui::PopStyleColor(); ImGui::PopStyleColor();
			ImGui::PopStyleColor(); ImGui::PopStyleColor();
			ImGui::PopStyleColor(); ImGui::PopStyleColor();

			ImGui::EndTable();
            ImGui::End();
        }

	// ===================================================================================== //
	// ============================== Browser Content Window =============================== //
	// ===================================================================================== //
		
		ImGuiWindowFlags MainWindowFlags = 0;
		MainWindowFlags |= ImGuiWindowFlags_MenuBar;
		MainWindowFlags |= ImGuiWindowFlags_NoTitleBar;
		MainWindowFlags |= ImGuiWindowFlags_NoResize;
		MainWindowFlags |= ImGuiWindowFlags_NoCollapse;
		MainWindowFlags |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(ViewportSize.x * ContentRelation, ViewportSize.y));

		{
			ImGui::Begin("Main Window", NULL, MainWindowFlags);

			ImGui::BeginMenuBar();
			if (ImGui::BeginMenu("Sound Libraries"))
			{
				if (ImGui::MenuItem("Add"))
				{
					// Add reference directory
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();

            ImGui::End();
		}

	// ===================================================================================== //
	// ================================== Log Window ======================================= //
	// ===================================================================================== //

		ImGuiWindowFlags LogWindowFlags = 0;
		LogWindowFlags |= ImGuiWindowFlags_NoTitleBar;
		LogWindowFlags |= ImGuiWindowFlags_NoResize;
		LogWindowFlags |= ImGuiWindowFlags_NoCollapse;
		LogWindowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		LogWindowFlags |= ImGuiWindowFlags_AlwaysUseWindowPadding;

		ImGui::SetNextWindowPos(ImVec2(ViewportSize.x * ContentRelation, ViewportSize.y - LogBoxSize.y));
		ImGui::SetNextWindowSize(ImVec2(ViewportSize.x * (1.0f - ContentRelation), LogBoxSize.y));

		{
			ImGui::Begin("Log Window", NULL, LogWindowFlags);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			ImGui::Text("Available Region %.3f", ImGui::GetMainViewport()->WorkSize.y);
            ImGui::End();
		}

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

		if (Done == true)
			glfwSetWindowShouldClose(window, true);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

	Done = true;
    return 0;
}
