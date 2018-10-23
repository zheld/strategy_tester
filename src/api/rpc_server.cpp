#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include <iostream>
#include "../api/api.h"

//the thread function
void* connection_handler(void*);

int start_server() {
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1303);

    //Bind
    if (bind(socket_desc, (struct sockaddr*) &server, sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 30);

//    //Accept and incoming connection
//    puts("Waiting for incoming connections...");
//    c = sizeof(struct sockaddr_in);
//
//
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    while ((client_sock = accept(socket_desc, (struct sockaddr*) &client, (socklen_t*) &c))) {
        puts("Connection accepted");

        if (pthread_create(&thread_id, NULL, connection_handler, (void*) &client_sock) < 0) {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }

    return 0;
}

void* connection_handler(void* socket_desc) {
    //Get the socket descriptor
    int sock = *(int*) socket_desc;
    int read_size;
    char* message, client_message[3000];
    std::cout << "NEW CONNECTION" << std::endl;

    //Send some messages to the client
//    message = "Greetings! I am your connection handler\n";
//    write(sock, message, strlen(message));

//    message = "Now type something and i shall repeat what you type \n";
//    write(sock, message, strlen(message));

    //Receive a message from client
    while ((read_size = recv(sock, client_message, 3000, 0)) > 0) {
        //end of string marker
//        client_message[read_size] = '\0';
//        std::cout << client_message << std::endl;

        auto resp_str = Execute(client_message);
        auto resp = resp_str.c_str();
        if (strlen(resp) == 2) {
            std::cout << resp_str << std::endl;
            std::cout << client_message << std::endl;
        }
//        std::cout << resp << "len: " << strlen(resp) << std::endl;

        //Send the message back to client
        write(sock, resp, strlen(resp));

        //clear the message buffer
        memset(client_message, 0, 3000);
    }

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }

    return 0;
}

void StartRPC() {
    start_server();
}

