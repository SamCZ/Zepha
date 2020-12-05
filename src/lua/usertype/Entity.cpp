//
// Created by aurailus on 2020-08-04.
//

#include "Entity.h"

#include "world/World.h"
#include "game/LocalSubgame.h"
#include "game/def/BlockDef.h"
#include "world/dim/Dimension.h"
#include "game/def/CraftItemDef.h"
#include "game/atlas/DefinitionAtlas.h"
#include "world/dim/ent/ServerLuaEntity.h"

void Api::Usertype::Entity::bind(State, sol::state& lua, sol::table&) {
	lua.new_usertype<Entity>("Entity",
		"get_id", &Entity::get_id,
		"id", sol::property(&Entity::get_id),
		
		"anims", sol::property(&Entity::get_animation_manager),
		
		"get_pos", &Entity::get_pos,
		"set_pos", &Entity::set_pos,
		"snap_pos", &Entity::snap_pos,
		"pos", sol::property(&Entity::get_pos, &Entity::set_pos),
		"get_vel", &Entity::get_vel,
		"set_vel", &Entity::set_vel,
		"vel", sol::property(&Entity::get_vel, &Entity::set_vel),
		"get_pitch", &Entity::get_pitch,
		"set_pitch", &Entity::set_pitch,
		"snap_pitch", &Entity::snap_pitch,
		"pitch", sol::property(&Entity::get_pitch, &Entity::set_pitch),
		"get_yaw", &Entity::get_yaw,
		"set_yaw", &Entity::set_yaw,
		"snap_yaw", &Entity::snap_yaw,
		"yaw", sol::property(&Entity::get_yaw, &Entity::set_yaw),
		"get_roll", &Entity::get_roll,
		"set_roll", &Entity::set_roll,
		"snap_roll", &Entity::snap_roll,
		"roll", sol::property(&Entity::get_roll, &Entity::set_roll),
		"get_scale", &Entity::get_scale,
		"set_scale", &Entity::set_scale,
		"snap_scale", &Entity::snap_scale,
		"scale", sol::property(&Entity::get_scale, &Entity::set_scale),
		"get_visual_offset", &Entity::get_visual_offset,
		"set_visual_offset", &Entity::set_visual_offset,
		"snap_visual_offset", &Entity::snap_visual_offset,
		"visual_offset", sol::property(&Entity::get_visual_offset, &Entity::set_visual_offset),
		
		"get_dimension", &Entity::get_dimension,
		"set_dimension", &Entity::set_dimension,
		"dim", sol::property(&Entity::get_dimension),
		
		"set_display_type", &Entity::set_display_type,
		"get_collision_box", &Entity::get_collision_box,
		"set_collision_box", &Entity::set_collision_box,
		"collision_box", sol::property(&Entity::get_collision_box, &Entity::set_collision_box),
		"get_collides", &Entity::get_collides,
		"set_collides", &Entity::set_collides,
		"collides", sol::property(&Entity::get_collides, &Entity::set_collides),
		"get_gravity", &Entity::get_gravity,
		"set_gravity", &Entity::set_gravity,
		"gravity", sol::property(&Entity::get_gravity, &Entity::set_gravity)
	);
}

long long Api::Usertype::Entity::get_id() {
	return entity->getId();
}

sol::object Api::Usertype::Entity::get_animation_manager(sol::this_state s) {
	if (entity.isL()) return sol::make_object(s, std::static_pointer_cast<LocalAnimationManager>(animation));
	else return sol::make_object(s, std::static_pointer_cast<ServerAnimationManager>(animation));
}

glm::vec3 Api::Usertype::Entity::get_pos() {
	return entity->getPos();
}

void Api::Usertype::Entity::set_pos(glm::vec3 pos) {
	entity->setPos(pos);
}

void Api::Usertype::Entity::snap_pos(glm::vec3 pos) {
	entity->setPos(pos);
}

glm::vec3 Api::Usertype::Entity::get_vel() {
	return entity->getVel();
}

void Api::Usertype::Entity::set_vel(glm::vec3 vel) {
	entity->setVel(vel);
}

glm::vec3 Api::Usertype::Entity::get_visual_offset() {
	return entity->getVisualOffset();
}

void Api::Usertype::Entity::set_visual_offset(glm::vec3 vs) {
	entity->setVisualOffset(vs);
}

void Api::Usertype::Entity::snap_visual_offset(glm::vec3 vs) {
	entity->setVisualOffset(vs);
}

float Api::Usertype::Entity::get_pitch() {
	return entity->getRotateX();
}

void Api::Usertype::Entity::set_pitch(float rot) {
	entity->setRotateX(rot);
}

void Api::Usertype::Entity::snap_pitch(float rot) {
	entity->setRotateX(rot);
}

float Api::Usertype::Entity::get_yaw() {
	return entity->getRotateY();
}

void Api::Usertype::Entity::set_yaw(float rot) {
	entity->setRotateY(rot);
}

void Api::Usertype::Entity::snap_yaw(float rot) {
	entity->setRotateY(rot);
}

float Api::Usertype::Entity::get_roll() {
	return entity->getRotateZ();
}

void Api::Usertype::Entity::set_roll(float rot) {
	entity->setRotateZ(rot);
}

void Api::Usertype::Entity::snap_roll(float rot) {
	entity->setRotateZ(rot);
}

float Api::Usertype::Entity::get_scale() {
	return entity->getScale().x;
}

void Api::Usertype::Entity::set_scale(float scale) {
	entity->setScale(scale);
}

void Api::Usertype::Entity::snap_scale(float scale) {
	entity->setScale(scale);
}

Api::Usertype::Dimension Api::Usertype::Entity::get_dimension() {
	return Dimension(entity->getDim());
}

void Api::Usertype::Entity::set_dimension(const std::string& identifier) {
	// TODO: Entity::setDim only sets the internal entity pointer, it doesn't move it to the other dimension's list.
	// Additionally, the entity id would be invalidated if that happened, because entity ids are per-dimension.
	// They would have to become global for this function to be enabled.
	
	std::cout << "Refused to change Entity's dimension, as the function is not complete."
			  << "Read TODO in set_dimension for more details." << std::endl;
	
//	entity->setDim(entity->getDim()->getWorld().getDimension(identifier));
}

void Api::Usertype::Entity::set_display_type(const std::string& mode,
	const std::string& argA, sol::optional<std::string> argB) {
	
	if (mode == "gameobject") {
		ItemDef& def = entity->getGame()->getDefs().fromStr(argA);
		if (def.type == ItemDef::Type::BLOCK) entity->setAppearance("gameobject", "block", argA);
		else if (def.type == ItemDef::Type::CRAFTITEM) entity->setAppearance("gameobject", "craftitem", argA);
	}
	else if (mode == "model" && argB) {
		entity->setAppearance("model", argA, *argB);
	}
	else throw std::runtime_error("Invalid display type parameters.");
}

sol::object Api::Usertype::Entity::get_collision_box() {
	auto c = entity->getCollisionBox();
	if (c) {
		sol::table table {};
		table[0] = c->a;
		table[1] = c->b;
		return table;
	}
	return sol::nil;
}

void Api::Usertype::Entity::set_collision_box(sol::object box) {
	if (box.is<sol::table>()) entity->setCollisionBox(SelectionBox(box.as<sol::table>()[1], box.as<sol::table>()[2]));
	else entity->removeCollisionBox();
}

bool Api::Usertype::Entity::get_collides() {
	return entity->getCollides();
}

void Api::Usertype::Entity::set_collides(bool collides) {
	entity->setCollides(collides);
}

float Api::Usertype::Entity::get_gravity() {
	return entity->getGravity();
}

void Api::Usertype::Entity::set_gravity(float gravity) {
	entity->setGravity(gravity);
}
