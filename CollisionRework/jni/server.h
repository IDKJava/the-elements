/*
 * server.h
 * --------------------------
 * Declares functions for building the buffer,
 * sending it to the server, and receiving and
 * storing the return data.
 */

#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#define SERVER_IP "71.244.112.67"
#define PORTNUM 2000

//Include the global variables
#include "app.h"
//Include the global macros
#include "macros.h"
//Socket includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//For bcopy and bzero
#include <strings.h>

void adduserpass(void);
void buildbuffer(int type);
int sendbuffer(void);

#endif //!SERVER_H_INCLUDED
