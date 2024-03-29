//
// Created by Valentin on 10/24/2019.
//

#ifndef CURSED_CHAT_LOG_FILE_H
#define CURSED_CHAT_LOG_FILE_H

#define PREFIX_DEBUG "[DEBUG] "
#define PREFIX_INFO "[INFO] "
#define PREFIX_WARNING "[WARNING] "
#define PREFIX_ERROR "[ERROR] "

#define LOG_NAME "output.log"

void output_logs_str(const char prefix[], const char str[], ...)__attribute__((format(printf,2, 3)));
void close_file(void);
void get_log_file(void);

#endif //CURSED_CHAT_LOG_FILE_H
