//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"

#include "world/LocalWorld.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "client/graph/Font.h"
#include "client/gui/basic/GuiText.h"
#include "world/player/LocalPlayer.h"
#include "client/gui/compound/GuiLabelledGraph.h"

DebugGui::DebugGui(glm::vec2 bufferSize, SubgamePtr game, WorldPtr world) :
	game(game),
	world(world) {
	
	auto fontRef = game.l()->textures["font"];
	auto fpsHistogramRef = game.l()->textures["histogram"];
	auto genericHistogramRef = game.l()->textures["histogram_white"];
	
	Font f(game.l()->textures, fontRef);
	
	auto crosshairText = std::make_shared<GuiText>("crosshairText");
	crosshairText->create({ 2, 2 }, {}, { 0.2, 0.2, 0.2, 0.5 }, { 1, 1, 1, 1 }, f);
	add(crosshairText);
	
	auto dataText = std::make_shared<GuiText>("dataText");
	dataText->create({ 2, 2 }, {}, { 0.2, 0.2, 0.2, 0.5 }, { 1, 1, 1, 1 }, f);
	add(dataText);
	
	auto interpGraph = std::make_shared<GuiLabelledGraph>("interpGraph");
	interpGraph->create({ 244, 64 }, {}, "Interp", 120, 256, genericHistogramRef, f);
	add(interpGraph);
	
	auto meshGraph = std::make_shared<GuiLabelledGraph>("meshGraph");
	meshGraph->create({ 244, 64 }, {}, "Mesh", 120, 32, genericHistogramRef, f);
	add(meshGraph);
	
	auto genGraph = std::make_shared<GuiLabelledGraph>("genGraph");
	genGraph->create({ 244, 64 }, {}, "Gen", 120, 16, genericHistogramRef, f);
	add(genGraph);
	
	auto packetGraph = std::make_shared<GuiLabelledGraph>("packetGraph");
	packetGraph->create({ 244, 64 }, {}, "Packets", 120, 32, genericHistogramRef, f);
	add(packetGraph);
	
	auto fpsGraph = std::make_shared<GuiLabelledGraph>("fpsGraph");
	fpsGraph->create({ 244, 64 }, {}, "FPS", 120, 60, fpsHistogramRef, f);
	add(fpsGraph);
	
	auto drawsGraph = std::make_shared<GuiLabelledGraph>("drawsGraph");
	drawsGraph->create({ 244, 64 }, {}, "Draw Calls", 120, 0, genericHistogramRef, f);
	add(drawsGraph);
	
	auto gpuGraph = std::make_shared<GuiLabelledGraph>("gpuGraph");
	gpuGraph->create({ 244, 64 }, {}, "GPU", 120, 1, genericHistogramRef, f);
	add(gpuGraph);
	
	positionElements(bufferSize);
}

void DebugGui::positionElements(glm::vec2 bufferSize) {
	auto bufferWidth = static_cast<int>(bufferSize.x);
	auto bufferHeight = static_cast<int>(bufferSize.y);
	
	get<GuiText>("crosshairText")->setPos({ bufferWidth / 2 + 22, bufferHeight / 2 - 7 });
	get<GuiText>("dataText")->setPos({ 10, 10 });
	
	get<GuiLabelledGraph>("genGraph")->setPos({ bufferWidth - 254, bufferHeight - 70 - 160 });
	get<GuiLabelledGraph>("packetGraph")->setPos({ bufferWidth - 254, bufferHeight - 70 - 240 });
	get<GuiLabelledGraph>("meshGraph")->setPos({ bufferWidth - 254, bufferHeight - 70 - 80 });
	get<GuiLabelledGraph>("interpGraph")->setPos({ bufferWidth - 254, bufferHeight - 70 });
	
	get<GuiLabelledGraph>("fpsGraph")->setPos({ bufferWidth - 254, 10 });
	get<GuiLabelledGraph>("drawsGraph")->setPos({ bufferWidth - 254, 90 });
	get<GuiLabelledGraph>("gpuGraph")->setPos({ bufferWidth - 254, 90 + 80 });
}

void DebugGui::update(std::shared_ptr<LocalPlayer> player, double fps, int /*chunks*/, int drawCalls, int ssGen,
	int ssPack) {
	Target target = player->getTarget();
	
	auto& onBiomeDef = game->getBiomes().biomeFromId(
		world.l()->getActiveDimension()->getBiome(glm::floor(player->getPos())));
	
	/* Top-right Graphs */ {
		get<GuiLabelledGraph>("fpsGraph")->pushValue(static_cast<float>(fps));
		get<GuiLabelledGraph>("drawsGraph")->pushValue(drawCalls);
		
		int videoMemAvail, videoMemTotal;
		
		glGetIntegerv(0x9048, &videoMemTotal);
		glGetIntegerv(0x9049, &videoMemAvail);
		
		get<GuiLabelledGraph>("gpuGraph")->pushValue(static_cast<int>(std::round(
			(videoMemTotal - videoMemAvail) / static_cast<float>(videoMemTotal) * 100.0)) / 100.0f);
	}
	
	/* Bottom-right Graphs */ {
		get<GuiLabelledGraph>("meshGraph")->pushValue(world.l()->lastMeshUpdates);
		get<GuiLabelledGraph>("interpGraph")->pushValue(world.l()->mapBlocksInterpolated);
		get<GuiLabelledGraph>("genGraph")->pushValue(static_cast<float>(ssGen));
		get<GuiLabelledGraph>("packetGraph")->pushValue(static_cast<float>(ssPack));
	}
	
	/* Top-left Data */ {
		glm::vec3 playerPos = glm::floor(player->getPos());
		glm::vec3 chunkPos = Space::Chunk::world::fromBlock(playerPos);
		glm::vec3 mapBlockPos = Space::MapBlock::world::fromChunk(chunkPos);
		glm::vec3 regionPos = Space::Region::world::fromChunk(chunkPos);
		
		glm::vec3 posOffsetFromChunk = Space::Block::relative::toChunk(playerPos);
		glm::vec3 posOffsetFromBlock = Space::Block::relative::toMapBlock(playerPos);
		glm::vec3 posOffsetFromRegion = Space::Block::relative::toRegion(playerPos);
		
		std::ostringstream str;
		
		using namespace Util;
		
		str << "Dimension: " << world.l()->getActiveDimension()->getIdentifier()
		    << " [" << world.l()->getActiveDimension()->getInd() << "]" << std::endl << std::endl;
		
		str << "Pos: " << vecToString(playerPos) << " (" << floatVecToString(player->getPos()) << ")" << std::endl;
		str << "Vel: " << floatVecToString(player->getVel()) << std::endl;
		str << "Yaw: " << floatToString(player->getYaw()) << ", ";
		str << "Pitch: " << floatToString(player->getPitch()) << std::endl << std::endl;
		
		str << "C: " << vecToString(posOffsetFromChunk) << " [" << vecToString(chunkPos) << "]" << std::endl;
		str << "M: " << vecToString(posOffsetFromBlock) << " [" << vecToString(mapBlockPos) << "]" << std::endl;
		str << "R: " << vecToString(posOffsetFromRegion) << " [" << vecToString(regionPos) << "]" << std::endl
		    << std::endl;
		
		str << "Texture Slots: " << game.l()->textures.textureSlotsUsed << " / " << game.l()->textures.maxTextureSlots
		    << " ("
		    << round(game.l()->textures.textureSlotsUsed / static_cast<float>(game.l()->textures.maxTextureSlots) * 100)
		    << "%)" << std::endl << std::endl;
		
		str << "Biome: " << onBiomeDef.identifier << " [" << onBiomeDef.index << "]" << std::endl << std::endl;
		
		if (target.type == Target::Type::BLOCK) {
			std::string face =
				target.data.block.face == EVec::TOP ? "TOP" :
				target.data.block.face == EVec::BOTTOM ? "BOTTOM" :
				target.data.block.face == EVec::LEFT ? "LEFT" :
				target.data.block.face == EVec::RIGHT ? "RIGHT" :
				target.data.block.face == EVec::FRONT ? "FRONT" :
				target.data.block.face == EVec::BACK ? "BACK" :
				"NONE";
			
			const auto& def = game->getDefs().blockFromId(world.l()->getActiveDimension()->getBlock(target.data.block.pos));
			
			str << "Pointing At: " << def.identifier << " [" << def.index << "]" << std::endl;
			str << "Pointed Position: " << vecToString(target.data.block.pos) << std::endl;
			str << "Pointed Face: " << face << std::endl;
		}
		else if (target.type == Target::Type::ENTITY) {
			const auto& entity = **world.l()->getActiveDimension().l()->getEntityById(target.data.entity.id).entity;
			
			str << "Pointing At: " << (target.data.entity.id < 0 ? "Local" : "Server")
				<< " Entity #" << std::fabs(target.data.entity.id) << std::endl;
			str << "Pointed Position: " << floatVecToString(entity.getPos()) << std::endl;
		}
		else {
			str << "No Target";
		}
		
		get<GuiText>("dataText")->setText(str.str());
	}
	
	/* Crosshair Text */ {
		if (target.type == Target::Type::BLOCK) {
			const auto& def = game->getDefs().blockFromId(world.l()->getActiveDimension()->getBlock(target.data.block.pos));
			get<GuiText>("crosshairText")->setText(
				def.name + " (" + def.identifier + ") [" + std::to_string(def.index) + "]");
		}
		else get<GuiText>("crosshairText")->setText("");
	}
}

void DebugGui::bufferResized(glm::vec2 bufferSize) {
	positionElements(bufferSize);
}

// 0 = All, 1 = None, 2 = FPS
void DebugGui::changeVisibilityState(int state) {
	displayMode = state;
	
	setVisible(displayMode == 0);
	get<GuiLabelledGraph>("fpsGraph")->setVisible(displayMode != 1);
}
