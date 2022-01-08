#include "Menu.h"
#include "Renderer/Lighting/PointLight.h"
static void ShowExampleAppDockSpace();
static void ShowExampleAppLog();

// Helper to wire demo markers located in code to a interactive browser
typedef void (*ImGuiDemoMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern ImGuiDemoMarkerCallback  GImGuiDemoMarkerCallback;
extern void* GImGuiDemoMarkerCallbackUserData;
ImGuiDemoMarkerCallback         GImGuiDemoMarkerCallback = NULL;
void* GImGuiDemoMarkerCallbackUserData = NULL;
#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)

void DisplayMenu()
{
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");
    ShowAppMainMenuBar();

    //bool show_demo_window = true;
    //bool show_another_window = true;
    //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    //{
    //    static float f = 0.0f;
    //    static int counter = 0;

    //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //    ImGui::Checkbox("Another Window", &show_another_window);

    //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //        counter++;
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);

    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //    ImGui::End();
    //}

    //const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
    //static int item_current_idx = 0; // Here we store our selection data as an index.
    //if (ImGui::BeginListBox("", { -FLT_MIN , 0}))
    //{
    //    for (int n = 0; n < IM_ARRAYSIZE(items); n++)
    //    {
    //        const bool is_selected = (item_current_idx == n);
    //        if (ImGui::Selectable(items[n], is_selected))
    //        {
    //            item_current_idx = n;
    //            std::cout << "pick element " + std::to_string(n) << std::endl;
    //        }

    //        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
    //        if (is_selected)
    //            ImGui::SetItemDefaultFocus();
    //    }
    //    ImGui::EndListBox();
    //}

    //// 3. Show another simple window.
    //if (show_another_window)
    //{
    //    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //    ImGui::Text("Hello from another window!");
    //    if (ImGui::Button("Close Me"))
    //        show_another_window = false;
    //    ImGui::End();
    //}

    //ShowExampleAppDockSpace();
    //ShowExampleAppLog();
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ShowAppMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ShowMenuFile()
{
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open...", "Ctrl+O")) 
    {
        OpenFile();
    }
    if (ImGui::MenuItem("Save As..", "Ctrl+S")) 
    {
        //SaveFile();
    }
    if (ImGui::MenuItem("Load Model"))
    {
        OpenFile();
    }

    if (ImGui::MenuItem("Quit", "Alt+F4")) 
    {
        Application::Get().Close();
    }
}

void OpenFile() 
{
    auto filePath = FileUtil::OpenFile("obj files (*.obj)\0*.obj\0");
    if (!filePath.empty())
    {
        logInfo("Open file: " + filePath);

        std::shared_ptr<Model> model = std::make_shared<Model>(filePath);
        model->loadModel();
        std::shared_ptr<Light> light = std::make_shared<PointLight>();
        std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
        model->UseLight(light);
        model->UseMaterial(material);

        Application::Get().GetContext()->AddModel(model);
    }
}

void SaveFile()
{
    auto filePath = FileUtil::SaveFile("obj files (*.obj)\0*.obj\0");
    if (!filePath.empty())
    {
        logDebug("Save file: " + filePath);
    }
}