/**
 * \file protocol.h
 */

void send_train(int socket, Train train);
void receive_train(int socket,Train *train);
void send_request(int socket, Request request);
void receive_request(int socket,Request *request);