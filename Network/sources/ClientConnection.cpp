// Copyright 2021 byteihq <kotov038@gmail.com>

#include <ClientConnection.h>

void ClientConnection::closeConnection() {
    socket_.close();
}
