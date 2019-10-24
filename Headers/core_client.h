//
// Created by Valentin on 10/24/2019.
//

#ifndef CURSED_CHAT_CORE_CLIENT_H
#define CURSED_CHAT_CORE_CLIENT_H

/**
 * --------- DEFINES ----------
 */
// addresses to connect to
#define LOCAL_HOST "127.0.0.1"
#define LOCAL_PORT 10001

// versions
#define CLIENT_MAJOR 0
#define CLIENT_MINOR 1
#define CLIENT_PATCH 0

/** --------------------------- */

int client_initialize(void);

#endif //CURSED_CHAT_CORE_CLIENT_H
