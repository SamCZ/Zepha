#include <client/gui/compound/GuiPerfGraph.h>
#include <client/gui/compound/GuiCellGraph.h>
#include "DebugGui.h"

#include "world/LocalWorld.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "client/graph/Font.h"
#include "world/dim/chunk/Chunk.h"
#include "client/gui/basic/GuiText.h"
#include "world/player/LocalPlayer.h"
#include "client/gui/compound/GuiLabelledGraph.h"

DebugGui::DebugGui(u16vec2 buffer, SubgamePtr game, LocalWorld& world, vec<string>& perfSections) :
	game(game), world(world) {

	auto fontRef = game.l()->textures["font"];
	auto fpsHistogramRef = game.l()->textures["histogram"];
	auto genericHistogramRef = game.l()->textures["histogram_white"];

	Font f(game.l()->textures, fontRef);

	auto crosshairText = make_shared<GuiText>("crosshairText");
	crosshairText->create({ 2, 2 }, {}, { 0.2, 0.2, 0.2, 0.5 }, { 1, 1, 1, 1 }, f);
	add(crosshairText);

	auto dataText = make_shared<GuiText>("dataText");
	dataText->create({ 2, 2 }, {}, { 0.2, 0.2, 0.2, 0.5 }, { 1, 1, 1, 1 }, f);
	add(dataText);

	auto interpGraph = make_shared<GuiLabelledGraph>("interpGraph");
	interpGraph->create({ 244, 64 }, {}, "Interp", 120, 256, genericHistogramRef, f);
	add(interpGraph);

	auto meshGraph = make_shared<GuiLabelledGraph>("meshGraph");
	meshGraph->create({ 244, 64 }, {}, "Mesh", 120, 32, genericHistogramRef, f);
	add(meshGraph);

	auto genGraph = make_shared<GuiLabelledGraph>("genGraph");
	genGraph->create({ 244, 64 }, {}, "Gen", 120, 16, genericHistogramRef, f);
	add(genGraph);

	auto packetGraph = make_shared<GuiLabelledGraph>("packetGraph");
	packetGraph->create({ 244, 64 }, {}, "Packets", 120, 32, genericHistogramRef, f);
	add(packetGraph);

	auto fpsGraph = make_shared<GuiLabelledGraph>("fpsGraph");
	fpsGraph->create({ 244, 64 }, {}, "FPS", 120, 60, fpsHistogramRef, f);
	add(fpsGraph);

	auto drawsGraph = make_shared<GuiLabelledGraph>("drawsGraph");
	drawsGraph->create({ 244, 64 }, {}, "Draw Calls", 120, 0, genericHistogramRef, f);
	add(drawsGraph);

	auto gpuGraph = make_shared<GuiLabelledGraph>("gpuGraph");
	gpuGraph->create({ 244, 64 }, {}, "GPU", 120, 1, genericHistogramRef, f);
	add(gpuGraph);

	auto perfGraph = make_shared<GuiPerfGraph>("perfGraph");
	perfGraph->create(344, {}, perfSections, "Performance", f);
	add(perfGraph);

	auto chunkStates = make_shared<GuiCellGraph>("chunkStates");
	chunkStates->create(6, vec4(4), CHUNK_RANGE, "Chunk Compression", f);
	chunkStates->refresh();
	add(chunkStates);

	positionElements(buffer);
}

void DebugGui::positionElements(u16vec2 buffer) {
	get<GuiText>("crosshairText")->setPos({ buffer.x / 2 + 22, buffer.y / 2 - 7 });
	get<GuiText>("dataText")->setPos({ 10, 10 });

	get<GuiLabelledGraph>("genGraph")->setPos({ buffer.x - 254, buffer.y - 70 - 160 });
	get<GuiLabelledGraph>("packetGraph")->setPos({ buffer.x - 254, buffer.y - 70 - 240 });
	get<GuiLabelledGraph>("meshGraph")->setPos({ buffer.x - 254, buffer.y - 70 - 80 });
	get<GuiLabelledGraph>("interpGraph")->setPos({ buffer.x - 254, buffer.y - 70 });

	get<GuiLabelledGraph>("fpsGraph")->setPos({ buffer.x - 254, 10 });
	get<GuiLabelledGraph>("drawsGraph")->setPos({ buffer.x - 254, 90 });
	get<GuiLabelledGraph>("gpuGraph")->setPos({ buffer.x - 254, 90 + 80 });
	get<GuiLabelledGraph>("perfGraph")->setPos({ buffer.x - 354 - 254, 10 });

	get<GuiLabelledGraph>("chunkStates")->setPos({ buffer.x - 264 - 300, buffer.y - 334 });
}

void DebugGui::update(sptr<LocalPlayer> player, f64 delta, u32 interpolatedChunks, u32 generatedChunks,
	u32 recievedPackets, vec<usize>& perfTimings, u32 drawnMeshChunks, u32 generatedMeshChunks) {

	Target target = player->getTarget();

	auto& onBiomeDef = game->getBiomes().biomeFromId(
		world.getActiveDimension()->getBiome(glm::floor(player->getPos())));

	// FPS and Draw calls graphs

	get<GuiPerfGraph>("perfGraph")->updateTimings(perfTimings);
	get<GuiLabelledGraph>("fpsGraph")->pushValue(1 / delta);
	get<GuiLabelledGraph>("drawsGraph")->pushValue(drawnMeshChunks);

	int videoMemAvail, videoMemTotal;

	glGetIntegerv(0x9048, &videoMemTotal);
	glGetIntegerv(0x9049, &videoMemAvail);

	get<GuiLabelledGraph>("gpuGraph")->pushValue(std::round(
		(videoMemTotal - videoMemAvail) / static_cast<f32>(videoMemTotal) * 100.0) / 100.0f);


	// Thread information graphs

	get<GuiLabelledGraph>("meshGraph")->pushValue(generatedMeshChunks);
	get<GuiLabelledGraph>("interpGraph")->pushValue(interpolatedChunks);
	get<GuiLabelledGraph>("genGraph")->pushValue(generatedChunks);
	get<GuiLabelledGraph>("packetGraph")->pushValue(recievedPackets);

	// Textual information

	vec3 playerPos = glm::floor(player->getPos());
	vec3 chunkPos = Space::Chunk::world::fromBlock(playerPos);
	vec3 mapBlockPos = Space::MapBlock::world::fromChunk(chunkPos);
	vec3 regionPos = Space::Region::world::fromChunk(chunkPos);

	vec3 posOffsetFromChunk = Space::Block::relative::toChunk(playerPos);
	vec3 posOffsetFromBlock = Space::Block::relative::toMapBlock(playerPos);
	vec3 posOffsetFromRegion = Space::Block::relative::toRegion(playerPos);

	std::ostringstream str;

	str << "Dimension: " << world.getActiveDimension()->getIdentifier()
	    << " [" << world.getActiveDimension()->getInd() << "]" << std::endl << std::endl

	    << "Pos: " << playerPos << " (" << player->getPos() << ")" << std::endl
		<< "Vel: " << player->getVel() << std::endl

		<< "Yaw: " << player->getYaw() << ", "
		<< "Pitch: " << player->getPitch() << std::endl << std::endl

		<< "C: " << posOffsetFromChunk << " [" << chunkPos << "]" << std::endl
		<< "M: " << posOffsetFromBlock << " [" << mapBlockPos << "]" << std::endl
		<< "R: " << posOffsetFromRegion << " [" << regionPos << "]" << std::endl
	    << std::endl

	    << "Texture Slots: " << game.l()->textures.textureSlotsUsed << " / " << game.l()->textures.maxTextureSlots
	    << " ("
	    << round(game.l()->textures.textureSlotsUsed / static_cast<float>(game.l()->textures.maxTextureSlots) * 100)
	    << "%)" << std::endl << std::endl

		<< "Biome: " << onBiomeDef.identifier << " [" << onBiomeDef.index << "]" << std::endl << std::endl;

	if (target.type == Target::Type::BLOCK) {
		string face =
			target.data.block.face == EVec::TOP ? "Top" :
			target.data.block.face == EVec::BOTTOM ? "Bottom" :
			target.data.block.face == EVec::LEFT ? "Left" :
			target.data.block.face == EVec::RIGHT ? "Right" :
			target.data.block.face == EVec::FRONT ? "Front" :
			target.data.block.face == EVec::BACK ? "Back" :
			"None (!)";

		const auto& def = game->getDefs().blockFromId(world.getActiveDimension()->getBlock(target.data.block.pos));

		str << "Pointing At: " << def.identifier << " [" << def.index << "]" << std::endl
			<< "Pointed Position: " << target.data.block.pos << std::endl
			<< "Pointed Face: " << face << std::endl;
	}
	else if (target.type == Target::Type::ENTITY) {
		const auto& entity = **world.getActiveDimension().l()->getEntityById(target.data.entity.id).entity;

		str << "Pointing At: " << (target.data.entity.id < 0 ? "Local" : "Server")
			<< " Entity #" << std::fabs(target.data.entity.id) << std::endl
			<< "Pointed Position: " << entity.getPos() << std::endl << std::endl;
	}
	else {
		str << "No Target" << std::endl << std::endl;
	}

//	for (usize i = 0; i < perfTimings.size(); i++) {
//		str << perfSections[i] << ": " << perfTimings[i] << " ns." << std::endl;
//	}

	get<GuiText>("dataText")->setText(str.str());

	// Chunk States

	if (chunkTimer == 0) {
		auto chunkStates = get<GuiCellGraph>("chunkStates");
		ivec3 off = { 0, 0, 0 };
		for (off.x = 0; off.x < CHUNK_RANGE; off.x++) {
			for (off.z = 0; off.z < CHUNK_RANGE; off.z++) {
				f32 existAmount = 0;
				f32 compressedAmount = 0;
				ivec3 check = ivec3(chunkPos) + off -
					glm::ivec3(floor(CHUNK_RANGE / 2), 0, floor(CHUNK_RANGE / 2));

				for (off.y = 0; off.y < CHUNK_VERT; off.y++) {
					check.y = static_cast<i32>(chunkPos.y) + off.y - CHUNK_VERT / 2;
					const auto chunk = world.getActiveDimension()->getChunk(check);
					if (chunk) {
						existAmount++;
						if (chunk->isCompressed()) compressedAmount++;
					}
				}

				const auto color = glm::mix(CHUNK_UNLOADED,
					glm::mix(CHUNK_UNCOMPRESSED, CHUNK_COMPRESSED,
						compressedAmount / CHUNK_VERT),existAmount / CHUNK_VERT);

				chunkStates->setCellColor(u16vec2(off.x, off.z), color);
			}
		}
		chunkStates->refresh();
	}
	
	chunkTimer = (chunkTimer + 1) % CHUNK_INTERVAL;

	// Crosshair information

	if (target.type == Target::Type::BLOCK) {
		const auto& def = game->getDefs().blockFromId(world.getActiveDimension()->getBlock(target.data.block.pos));
		get<GuiText>("crosshairText")->setText(def.name + " (" + def.identifier + ") [" + std::to_string(def.index) + "]");
	}
	else {
		get<GuiText>("crosshairText")->setText("");
	}
}

void DebugGui::bufferResized(u16vec2 buffer) {
	positionElements(buffer);
}

void DebugGui::changeVisibility(Visibility state) {
	setVisible(state == Visibility::ON);
	get<GuiLabelledGraph>("fpsGraph")->setVisible(state != Visibility::OFF);
}
