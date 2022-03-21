#include "Render.h"
#include <iostream>
#include ".\ImGui\imgui.h"

Render& Render::Instance()
{
	static Render Instance;
	return Instance;
}

void Render::Initialize()
{
	ImGui::CreateContext();
	ImGuiIO& Io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
}

Render::Render()
{
}