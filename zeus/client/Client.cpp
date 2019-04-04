//
// Created by aurailus on 06/01/19.
//

#include <zconf.h>
#include <signal.h>
#include "Client.h"
#include <asio.hpp>

using asio::ip::udp;

Client::Client() = default;

Client::Client(int width, int height) {
    renderer = new Renderer(width, height);
}

void Client::start(char* path) {
//    Start Local Server
    int pid = fork();
    if (pid == 0) {
        char* arr[] = {
                (char*) "xterm",
                (char*) "-iconic",
                (char*) "-e",
                        path,
                (char*) "server",
                (char*) nullptr};

        execvp("xterm", arr);
    }
    else {
        local_server_pid = pid;
    }

    //Create ClientState struct
    state = new ClientState {
            .renderer = renderer,
            .fps = 0,
            .deltaTime = 0
    };

    //Start the Main Menu
    Scene* m = new MenuScene(state);
    sceneManager.setScene(m);

    //Try to connect to a server
    try {
        asio::io_context io_context;
        udp::resolver resolver(io_context);
        udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "127.0.0.1", "12346").begin();

        udp::socket socket(io_context);
        socket.open(udp::v4());

        int attempts = 0;
        bool connected = false;
        while (!connected) {
            if (attempts > 5) {
                break;
            }

            char send_buf[4] = {'a', 'b', 'c', 0};
            socket.send_to(asio::buffer(send_buf), receiver_endpoint);
            attempts++;

            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            if (socket.available() > 0) {
                std::cout << "Connected" << std::endl;
                connected = true;
            }
            else {
                std::cout << "Failed to connect..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds((attempts < 3 ? 50 : 700)));
            }
        }
        if (!connected) {
            std::cout << "Connection timed out!" << std::endl;
            return;
        }

        char recv_buf[20];

        udp::endpoint sender_endpoint; //Populated by the next line
        size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);

        std::cout.write(recv_buf, len);
        std::cout << "Done " << len << std::endl;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }


    while (!renderer->getWindow()->getShouldClose()) loop();
}

void Client::loop() {
    Timer t("Client Loop");

    double now = glfwGetTime();
    state->deltaTime = now - timeElapsed;
    timeElapsed = now;

    count ++;
    if (count == 20) {
        Scene* g = new GameScene(state);
        sceneManager.setScene(g); //Main Menu Scene here eventually
    }

    glfwPollEvents();

    sceneManager.update();
    state->renderer->update();

    state->fps = 1000 / (t.elapsedNs() / 1000000.0);
}

void Client::cleanup() {
    kill(local_server_pid, SIGKILL);
    sceneManager.cleanupScene();
    delete state;
    delete renderer;
}

Client::~Client() {
    cleanup();
}