#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION
#pragma clang diagnostic pop

#include "client/Client.h"

//std::string make_daytime_string() {
//    std::time_t now = std::time(nullptr);
//    return "HIfhjdsfjhglfgkhkdfjhbhnjgdsk,jfgbhdksijfghgjfkwaslodfjgbhfdsklofjghnbgkmdsk,lfkgbna";
////    return std::ctime(&now);
//}

//void server() {
//    try {
//        asio::io_context io_context;
//        udp::socket socket(io_context, udp::endpoint(udp::v4(), 12345));
//
//        for (;;) {
//            char recv_buf[128];
//            udp::endpoint remote_endpoint; //Populated by the next line
//
//            Timer t("owo");
//            socket.receive_from(asio::buffer(recv_buf), remote_endpoint);
//            t.printElapsedSeconds();
//            std::cout << recv_buf << std::endl;
//
//            std::string message = make_daytime_string();
//
//            asio::error_code ignored_error;
//            socket.send_to(asio::buffer(message), remote_endpoint, 0, ignored_error);
//
//            //Socket is garbage collected here, so like, don't do that?
//        }
//    }
//    catch (std::exception& e) {
//        std::cerr << e.what() << std::endl;
//    }
//}

int main(int argc, char* argv[]) {
//    auto serverThread = new std::thread(server);
//    this_thread::sleep_for(0.2s);
//
//    try {
//        asio::io_context io_context;
//        udp::resolver resolver(io_context);
//        udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "127.0.0.1", "12345").begin();
//
//        udp::socket socket(io_context);
//        socket.open(udp::v4());
//
//        char send_buf[3] = {'a', 'b', 'c'};
//        socket.send_to(asio::buffer(send_buf), receiver_endpoint);
//
//        char recv_buf[20];
//
//        udp::endpoint sender_endpoint; //Populated by the next line
//        size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
//
//        sender_endpoint.
//
//        std::cout.write(recv_buf, len);
////        std::cout << "Done " << len << std::endl;
//    }
//    catch (std::exception& e) {
//        std::cout << e.what() << std::endl;
//    }


    Client c(1366, 768);
    c.start();
    return 0;
}