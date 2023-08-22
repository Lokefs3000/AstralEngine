#include "PerformanceViewer.h"

#include <core/Engine.h>

float PerformanceViewer::DrawBar(ImDrawList* dList, uint32_t color, StopWatch watch, float& x, float full, uint8_t id)
{
	if (!activeWatches[id])
		return 0.0f;

	std::string delta = std::to_string(watch->GetDuration());

	float acW = watch->GetDuration() / full;
	if (watch->GetDuration() == 0.0f)
		return 0.0f;

	ImVec2 availSpace = ImGui::GetContentRegionAvail();
	float width = availSpace.x;
	ImVec2 start = ImGui::GetCursorScreenPos() + ImVec2(x, 0.0f);

	dList->AddRectFilled(start, start + ImVec2(acW * width, 30.0f), color, 2.0f);

	return acW * width;
}

void PerformanceViewer::DrawTextLine(StopWatch watch, std::string name, uint32_t color, ImDrawList* dList, uint8_t id)
{
	ImGuiContext& ctx = *ImGui::GetCurrentContext();

	ImVec2 sC = ImGui::GetCursorScreenPos();
	ImRect rect = ImRect(sC, sC + ImVec2(ctx.FontSize, ctx.FontSize));

	if (ImGui::IsMouseHoveringRect(rect.Min, rect.Max)) {
		rect.Min -= ImVec2(1.0f, 1.0f);
		rect.Max += ImVec2(1.0f, 1.0f);

		if (ImGui::IsMouseClicked(0))
			activeWatches[id] = !activeWatches[id];
	}
	if (!activeWatches[id])
		color -= 0xdd000000;

	dList->AddRectFilled(rect.Min, rect.Max, color, 2.0f);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + rect.Max.x - sC.x + ctx.Style.FramePadding.x);

	float bX = ImGui::GetCursorPosX();
	float fW = ImGui::GetContentRegionAvail().x;

	ImGui::Text(name.c_str());
	ImGui::SameLine();

	std::string delta = std::to_string(watch->GetDuration());
	delta = delta.substr(0, delta.find(".") + 3) + "ms";

	float dTW = ImGui::CalcTextSize(delta.c_str()).x;
	ImGui::SetCursorPosX(bX + fW - dTW);

	ImGui::Text(delta.c_str());
}

void PerformanceViewer::Render(Engine* engine)
{
	if (ImGui::Begin("Performance viewer")) {
		ImGuiContext& ctx = *ImGui::GetCurrentContext();
		ImDrawList* drawList = ctx.CurrentWindow->DrawList;

		ImVec2 availSpaceAll = ImGui::GetContentRegionAvail();

		if (ImGui::BeginChild("##PV_Diagram", ImVec2(availSpaceAll.x, 70.0f), true)) {
			float beginX = ImGui::GetCursorPosX();

			ImVec2 availSpace = ImGui::GetContentRegionAvail();
			float width = availSpace.x;
			ImVec2 start = ImGui::GetCursorScreenPos();

			if (m_IsGraph) {

			}
			else {
				drawList->AddRectFilled(start, start + ImVec2(availSpace.x, 30.0f), 0xff737373, 2.0f);

				float x = 0.0f;
				x += DrawBar(drawList, 0xffd94148, engine->Watches.RenderWatch, x, engine->Watches.DeltaWatch->GetDuration(), 0);
				x += DrawBar(drawList, 0xff1397ed, engine->Watches.AssetWatch, x, engine->Watches.DeltaWatch->GetDuration(), 1);
				x += DrawBar(drawList, 0xff17af5d, engine->Watches.TextureWatch, x, engine->Watches.DeltaWatch->GetDuration(), 2);
				x += DrawBar(drawList, 0xff450543, engine->Watches.LayerWatch, x, engine->Watches.DeltaWatch->GetDuration(), 3);
				x += DrawBar(drawList, 0xff278c43, engine->Watches.LateLayerWatch, x, engine->Watches.DeltaWatch->GetDuration(), 4);
				x += DrawBar(drawList, 0xfff0f599, engine->Watches.EventWatch, x, engine->Watches.DeltaWatch->GetDuration(), 5);

				ImGui::Dummy(ImVec2(availSpace.x, 30.0f));
			}

			ImGui::Checkbox("Is graph?", &m_IsGraph);

			ImGui::SameLine();
			std::string dt = std::to_string(engine->Watches.DeltaWatch->GetDuration());
			std::string deltaText = "Delta: " + dt.substr(0, dt.find(".") + 3) + "ms";
			float deltaW = ImGui::CalcTextSize(deltaText.c_str()).x;
			ImGui::SetCursorPosX(beginX + availSpace.x - deltaW);
			ImGui::Text(deltaText.c_str());
		}
		ImGui::EndChild();

		if (ImGui::BeginChild("##PV_Viewables", ImGui::GetContentRegionAvail(), true)) {
			DrawTextLine(engine->Watches.RenderWatch, "Render", 0xffd94148, drawList, 0);
			DrawTextLine(engine->Watches.AssetWatch, "Assets", 0xff1397ed, drawList, 1);
			DrawTextLine(engine->Watches.TextureWatch, "Textures", 0xff17af5d, drawList, 2);
			DrawTextLine(engine->Watches.LayerWatch, "Layers frame", 0xff450543, drawList, 3);
			DrawTextLine(engine->Watches.LateLayerWatch, "Layers late", 0xff278c43, drawList, 4);
			DrawTextLine(engine->Watches.EventWatch, "Events", 0xfff0f599, drawList, 5);
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
