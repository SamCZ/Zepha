//
// Created by aurailus on 11/01/19.
//

#include "ClientNetworkInterpreter.h"

#include "NetPlayerField.h"
#include "../world/Player.h"
#include "../../../util/net/NetHandler.h"

ClientNetworkInterpreter::ClientNetworkInterpreter(ServerConnection &connection, ClientGame &defs, Player& player) :
    player(player),
    connection(connection),
    playerModel(std::make_shared<Model>()) {
    playerModel->fromSerialized(defs.models.models["zeus:default:player"], {defs.textures["zeus:default:player"]});
}

void ClientNetworkInterpreter::init(LocalWorld *world, std::function<void(std::unique_ptr<Packet>)> invCallback) {
    this->world = world;
    this->onInvPacket = invCallback;
}

void ClientNetworkInterpreter::update() {
    recvPackets = 0;

    ENetEvent event;
    while (connection.pollEvents(&event)) {
        recvPackets++;

        switch (event.type) {
            default: break;
            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << Log::info << "Connected to server "
                          << NetHandler::intToIPString(event.peer->address.host)
                          << ":" << event.peer->address.port << "." << Log::endl;
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                std::unique_ptr<Packet> p = std::make_unique<Packet>(event.packet);
                receivedPacket(std::move(p));
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                std::cout << Log::info << "Disconnected from server "
                          << event.peer->address.host << ":" << event.peer->address.port << "." << Log::endl;
                break;
            }
        }
    }

    //Send Player Position
    Packet p(PacketType::PLAYER_INFO);
    p.data = Serializer()
            .append(player.getPos())
            .append(player.getPitch())
            .append(player.getYaw())
            .data;

    p.sendTo(connection.getPeer(), PacketChannel::PLAYER);
}

void ClientNetworkInterpreter::receivedPacket(std::unique_ptr<Packet> p) {
    Deserializer d(p->data);

    switch (p->type) {
        default: {
            std::cout << Log::err << "Received unknown packet of type " << static_cast<int>(p->type)
                      << ". Is the server on a different protocol version?" << Log::endl;
            break;
        }

        case PacketType::THIS_PLAYER_INFO: {
            while (!d.atEnd()) {
                switch (d.read<unsigned int>()) {
                    case static_cast<unsigned int>(NetPlayerField::ID): {
                        cid = d.read<unsigned int>();
                        break;
                    }
                    case static_cast<unsigned int>(NetPlayerField::POSITION): {
                        player.setPos(d.read<glm::vec3>());
                        break;
                    }
                    case static_cast<unsigned int>(NetPlayerField::PITCH): {
                        player.setPitch(d.read<float>());
                        break;
                    }
                    case static_cast<unsigned int>(NetPlayerField::YAW): {
                        player.setYaw(d.read<float>());
                        break;
                    }
                }
            }
            break;
        }
        case PacketType::PLAYER_INFO: {
            unsigned int cid = d.read<unsigned int>();
            if (this->cid == cid) break;

            bool found = false;
            for (auto& entity : world->dimension.playerEntities) {
                if (entity.getCid() == cid) {
                    // Update an existing PlayerEntity
                    entity.interpPos(d.read<glm::vec3>());
                    entity.interpRotateZ(-d.read<float>() + 90);
                    entity.interpRotateY(-d.read<float>() + 90);

                    found = true;
                    break;
                }
            }
            if (found) break;

            // Instantiate a new PlayerEntity
            world->dimension.playerEntities.emplace_back(d.read<glm::vec3>(), cid, playerModel);
            break;
        }
        case PacketType::ENTITY_INFO: {
            world->dimension.serverEntityInfo(*p);
            break;
        }
        case PacketType::ENTITY_REMOVED: {
            world->dimension.serverEntityRemoved(d.read<unsigned int>());
            break;
        }
        case PacketType::BLOCK_SET: {
            auto pos = d.read<glm::ivec3>();
            auto block = d.read<unsigned int>();
            world->setBlock(pos, block);
            break;
        }
        case PacketType::CHUNK: {
            world->loadChunkPacket(std::move(p));
            break;
        }
        case PacketType::SERVER_INFO: {
            serverSideChunkGens = d.read<unsigned int>();
            break;
        }
        case PacketType::INV_INVALID: {
            std::string source = d.read<std::string>();
            std::string list = d.read<std::string>();

            std::cout << Log::err << "Invalid inventory " << source << ":" << list << " was requested by client." << Log::endl;
            exit(1);
        }
        case PacketType::INV_DATA: {
            onInvPacket(std::move(p));
            break;
        }
    }
}

void ClientNetworkInterpreter::setBlock(glm::ivec3 pos, unsigned int block) {
    Serializer().append(pos).append(block)
        .packet(PacketType::BLOCK_SET).sendTo(connection.getPeer(), PacketChannel::BLOCK);
}

void ClientNetworkInterpreter::watchInv(const std::string& inv, const std::string& list) {
    Serializer().append(inv).append(list)
            .packet(PacketType::INV_WATCH).sendTo(connection.getPeer(), PacketChannel::INVENTORY);
}

void ClientNetworkInterpreter::unwatchInv(const std::string& inv, const std::string& list) {
    Serializer().append(inv).append(list)
            .packet(PacketType::INV_UNWATCH).sendTo(connection.getPeer(), PacketChannel::INVENTORY);
}

void ClientNetworkInterpreter::primaryInteract(const std::string &inv, const std::string &list, unsigned short ind) {
    Serializer().append<unsigned short>(0).append(inv).append(list).append<unsigned short>(ind)
            .packet(PacketType::INV_INTERACT).sendTo(connection.getPeer(), PacketChannel::INVENTORY);
}

void ClientNetworkInterpreter::secondaryInteract(const std::string &inv, const std::string &list, unsigned short ind) {
    Serializer().append<unsigned short>(1).append(inv).append(list).append<unsigned short>(ind)
            .packet(PacketType::INV_INTERACT).sendTo(connection.getPeer(), PacketChannel::INVENTORY);
}
