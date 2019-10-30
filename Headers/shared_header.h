//
// Created by Valentin on 10/24/2019.
//

#ifndef CURSED_CHAT_SHARED_HEADER_H
#define CURSED_CHAT_SHARED_HEADER_H

#ifdef _WIN32 /// WINDOWS PROTOTYPES
int inet_pton(int af, const char *src, char *dst);
#endif

// protocols
#define PROTOC_NICK 	"NICKNAME:"
#define PROTOC_SEND_MSG	"SEND_MESG:"
#define PROTOC_RECV_MSG	"RECV_MSG:"

#endif //CURSED_CHAT_SHARED_HEADER_H
