#include "DebugDisplay.h"

#include "world/LocalWorld.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "world/dim/chunk/Chunk.h"
#include "client/gui/BoxElement.h"
#include "client/gui/TextElement.h"
#include "world/player/LocalPlayer.h"
//#include "client/gui/compound/GuiPerfGraph.h"
//#include "client/gui/compound/GuiCellGraph.h"
//#include "client/gui/compound/GuiLabelledGraph.h"

DebugDisplay::DebugDisplay(sptr<Gui::Element> root, SubgamePtr game, LocalWorld& world, vec<string>& perfSections) :
	root(root), game(game), world(world) {

	using Expr = Gui::Expression;
	
	crosshair = root->append<Gui::TextElement>({{
		{ Gui::Prop::TEXT_SIZE, Expr("2px") },
		{ Gui::Prop::POS, array<Expr, 2> { Expr("50cw + 8dp"), Expr("50ch - 3.333dp") } },
		{ Gui::Prop::BACKGROUND, vec4 { 0.2, 0.2, 0.2, 0.3 } }
	}});
	
	data = root->append<Gui::TextElement>({{
		{ Gui::Prop::TEXT_SIZE, Expr("2px") },
		{ Gui::Prop::POS, array<Expr, 2> { Expr("4dp"), Expr("4dp") } },
		{ Gui::Prop::BACKGROUND, vec4 { 0.2, 0.2, 0.2, 0.3 } }
	}});

//	auto interpGraph = make_shared<GuiLabelledGraph>(window, "interpGraph");
//	interpGraph->create({ 244, 64 }, {}, "Interp", 120, 256, genericHistogramRef, f);
//	add(interpGraph);
//
//	auto meshGraph = make_shared<GuiLabelledGraph>(window, "meshGraph");
//	meshGraph->create({ 244, 64 }, {}, "Mesh", 120, 32, genericHistogramRef, f);
//	add(meshGraph);
//
//	auto genGraph = make_shared<GuiLabelledGraph>(window, "genGraph");
//	genGraph->create({ 244, 64 }, {}, "Gen", 120, 16, genericHistogramRef, f);
//	add(genGraph);
//
//	auto packetGraph = make_shared<GuiLabelledGraph>(window, "packetGraph");
//	packetGraph->create({ 244, 64 }, {}, "Packets", 120, 32, genericHistogramRef, f);
//	add(packetGraph);
//
//	auto fpsGraph = make_shared<GuiLabelledGraph>(window, "fpsGraph");
//	fpsGraph->create({ 244, 64 }, {}, "FPS", 120, 60, fpsHistogramRef, f);
//	add(fpsGraph);
//
//	auto drawsGraph = make_shared<GuiLabelledGraph>(window, "drawsGraph");
//	drawsGraph->create({ 244, 64 }, {}, "Draw Calls", 120, 0, genericHistogramRef, f);
//	add(drawsGraph);
//
//	auto gpuGraph = make_shared<GuiLabelledGraph>(window, "gpuGraph");
//	gpuGraph->create({ 244, 64 }, {}, "GPU", 120, 1, genericHistogramRef, f);
//	add(gpuGraph);
//
//	auto perfGraph = make_shared<GuiPerfGraph>(window, "perfGraph");
//	perfGraph->create(344, {}, perfSections, "Performance", f);
//	add(perfGraph);
//
//	auto chunkStates = make_shared<GuiCellGraph>(window, "chunkStates");
//	chunkStates->create(12, vec4(4), MAPBLOCK_RANGE, "Mapblocks", f);
//	chunkStates->refresh();
//	add(chunkStates);
}

//void DebugGui::positionElements(u16vec2 buffer) {
//	get<GuiText>("crosshairText")->setPos({ buffer.x / 2 + 22, buffer.y / 2 - 7 });
//	get<GuiText>("dataText")->setPos({ 10, 10 });
//
//	get<GuiLabelledGraph>("genGraph")->setPos({ buffer.x - 254, buffer.y - 70 - 160 });
//	get<GuiLabelledGraph>("packetGraph")->setPos({ buffer.x - 254, buffer.y - 70 - 240 });
//	get<GuiLabelledGraph>("meshGraph")->setPos({ buffer.x - 254, buffer.y - 70 - 80 });
//	get<GuiLabelledGraph>("interpGraph")->setPos({ buffer.x - 254, buffer.y - 70 });
//
//	get<GuiLabelledGraph>("fpsGraph")->setPos({ buffer.x - 254, 10 });
//	get<GuiLabelledGraph>("drawsGraph")->setPos({ buffer.x - 254, 90 });
//	get<GuiLabelledGraph>("gpuGraph")->setPos({ buffer.x - 254, 90 + 80 });
//	get<GuiLabelledGraph>("perfGraph")->setPos({ buffer.x - 354 - 254, 10 });
//
//	get<GuiLabelledGraph>("chunkStates")->setPos({ buffer.x - 264 - 144, buffer.y - 178 });
//}

void DebugDisplay::update(sptr<LocalPlayer> player, f64 delta, u32 interpolatedChunks, u32 generatedChunks,
	u32 recievedPackets, vec<usize>& perfTimings, u32 drawnMeshChunks, u32 generatedMeshChunks) {

	Target target = player->getTarget();

	auto& onBiomeDef = game->getBiomes().biomeFromId(
		world.getActiveDimension()->getBiome(glm::floor(player->getPos())));

//	// FPS and Draw calls graphs
//
//	get<GuiPerfGraph>("perfGraph")->updateTimings(perfTimings);
//	get<GuiLabelledGraph>("fpsGraph")->pushValue(1 / delta);
//	get<GuiLabelledGraph>("drawsGraph")->pushValue(drawnMeshChunks);
//
//	int videoMemAvail, videoMemTotal;
//
//	glGetIntegerv(0x9048, &videoMemTotal);
//	glGetIntegerv(0x9049, &videoMemAvail);
//
//	get<GuiLabelledGraph>("gpuGraph")->pushValue(std::round(
//		(videoMemTotal - videoMemAvail) / static_cast<f32>(videoMemTotal) * 100.0) / 100.0f);
//
//
//	// Thread information graphs
//
//	get<GuiLabelledGraph>("meshGraph")->pushValue(generatedMeshChunks);
//	get<GuiLabelledGraph>("interpGraph")->pushValue(interpolatedChunks);
//	get<GuiLabelledGraph>("genGraph")->pushValue(generatedChunks);
//	get<GuiLabelledGraph>("packetGraph")->pushValue(recievedPackets);

	// Textual information

	ivec3 playerPos = glm::floor(player->getPos());
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

	data->setProp(Gui::Prop::CONTENT, str.str());
//	// Chunk States
//
//	auto chunkStates = get<GuiCellGraph>("chunkStates");
//	ivec3 off = { 0, 0, 0 };
//	for (off.x = 0; off.x < MAPBLOCK_RANGE; off.x++) {
//		if ((off.x - mapBlockScanX) % MAPBLOCK_SCAN_X_INTERVAL != 0) continue;
//
//		for (off.z = 0; off.z < MAPBLOCK_RANGE; off.z++) {
//			f32 existAmount = 0;
//			ivec3 check = ivec3(mapBlockPos) + off -
//				ivec3(MAPBLOCK_RANGE / 2, 0, MAPBLOCK_RANGE / 2);
//
//			for (off.y = 0; off.y < MAPBLOCK_VERT; off.y++) {
//				check.y = static_cast<i32>(mapBlockPos.y) + off.y - MAPBLOCK_VERT / 2;
//				const auto mapBlock = world.getActiveDimension()->getMapBlock(check);
//				if (mapBlock) existAmount++;
//			}
//
//			const auto color =
//				(off.x == MAPBLOCK_RANGE / 2 && off.z == MAPBLOCK_RANGE / 2) ? MAPBLOCK_CURRENT
//					: glm::mix(MAPBLOCK_UNLOADED, MAPBLOCK_LOADED, existAmount / MAPBLOCK_VERT);
//			chunkStates->setCellColor(u16vec2(off.x, off.z), color);
//		}
//	}
//	chunkStates->refresh();
//
//	mapBlockScanX = (mapBlockScanX + 1) % MAPBLOCK_SCAN_X_INTERVAL;

	if (target.type == Target::Type::BLOCK) {
		const auto& def = game->getDefs().blockFromId(world.getActiveDimension()->getBlock(target.data.block.pos));
		crosshair->setProp(Gui::Prop::CONTENT, "`b" + def.name + " (`r` `c1" +
			def.identifier + "`cr - " + std::to_string(def.index) + "` `b)");
	}
	else {
		crosshair->setProp(Gui::Prop::CONTENT, string(""));
	}
	
	root->updateElement();
}

//void DebugDisplay::changeVisibility(Visibility state) {
////	setVisible(state == Visibility::ON);
////	get<GuiLabelledGraph>("fpsGraph")->setVisible(state != Visibility::OFF);
//}
