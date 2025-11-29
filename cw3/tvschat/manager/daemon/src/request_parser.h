#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

typedef struct {
    char script[128];
    char arg[64];
} script_info_t;

script_info_t parseRequest(const char *req);

#endif
