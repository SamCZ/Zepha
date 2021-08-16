#pragma once

#include "client/gui/basic/GuiContainer.h"

#include "util/Types.h"
#include "util/CovariantPtr.h"

class GuiRect;
class LocalWorld;
class LocalPlayer;
class LocalSubgame;

/**
 * Renders debug information, in the form of graphs and text.
 * Can be enabled, disabled, or displayed with FPS only.
 */

class DebugGui : public GuiContainer {
public:

	enum class Visibility { OFF, FPS_ONLY, ON };

	DebugGui(u16vec2 buffer, SubgamePtr game, LocalWorld& world, vec<string>& perfSections);

	/** Resizes elements when the screen buffer is resized. */
	void bufferResized(u16vec2 bufferSize);

	/** Sets which elements are visible based on the state provided. */
	void changeVisibility(Visibility state);

	/** Positions all elements based on the buffer size. */
	void positionElements(u16vec2 buffer);

	/** Updates the debug screen with the latest data. */
	void update(sptr<LocalPlayer> player, f64 delta, u32 interpolatedChunks, u32 generatedChunks,
		u32 recievedPackets, vec<usize>& perfTimings, u32 drawnMeshChunks, u32 generatedMeshChunks);

private:

	constexpr static vec4 MAPBLOCK_UNLOADED = { 1, 1, 1, 0.15 };
	constexpr static vec4 MAPBLOCK_LOADED = { 1, 1, 1, 0.75 };
	constexpr static vec4 MAPBLOCK_CURRENT = { 1, 0.93, 0.35, 1 };

	constexpr static i32 MAPBLOCK_VERT = 3;
	constexpr static i32 MAPBLOCK_RANGE = 11;
	
	u32 mapBlockScanX = 0;
	constexpr static u32 MAPBLOCK_SCAN_X_INTERVAL = 9;

	SubgamePtr game;
	LocalWorld& world;

	Visibility state = Visibility::ON;
};
