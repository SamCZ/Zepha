//
// Created by aurailus on 11/01/19.
//

#pragma once

#include <memory>
#include <functional>
#include <glm/vec3.hpp>

#include "util/CovariantPtr.h"
#include "util/net/Serializer.h"
#include "client/conn/ServerConnection.h"

class Model;
class Target;
class LocalWorld;
class PacketView;
class LocalPlayer;
class LocalSubgame;

enum class NetPlayerField;

class ClientNetworkInterpreter {
public:
	ClientNetworkInterpreter(const ClientNetworkInterpreter& other) = delete;
	
	ClientNetworkInterpreter(ServerConnection& connection, LocalWorld& world);
	
	void init(std::function<void(uptr<PacketView>)> invCallback);
	
	void update();
	
	void blockHit(const Target& target);
	
	void blockPlace(const Target& target);
	
	void blockInteract(const Target& target);
	
	void blockPlaceOrInteract(const Target& target);
	
	void wieldItemUse(const Target& target);
	
	void invWatch(const string& inv, const string& list);
	
	void invUnwatch(const string& inv, const string& list);
	
	void invInteract(const string& inv, const string& list, bool primary, u16 ind);
	
	void sendPacket(const Packet& packet, Packet::Channel channel);
	
	u32 recvPackets = 0;
	u32 serverSideChunkGens = 0;

private:
	void receivedPacket(uptr<PacketView> ePacket);
	
	LocalWorld& world;
	ServerConnection& connection;
	
	std::function<void(uptr<PacketView>)> onInvPacket;
};

