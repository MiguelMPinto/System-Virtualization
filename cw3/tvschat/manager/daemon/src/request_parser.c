#include <string.h>
#include "request_parser.h"


 script_info_t parseRequest(const char *req) {
    script_info_t info;
    memset(&info, 0, sizeof(info));

    // match directo (SEM validação extra)
    if (strcmp(req, "run private") == 0) {
        strcpy(info.script, "/opt/isel/tvs/tvschat-run.sh");
        strcpy(info.arg, "private");
    }
    else if (strcmp(req, "run prod") == 0) {
        strcpy(info.script, "/opt/isel/tvs/tvschat-run.sh");
        strcpy(info.arg, "prod");
    }
    else if (strcmp(req, "run dev") == 0) {
        strcpy(info.script, "/opt/isel/tvs/tvschat-run.sh");
        strcpy(info.arg, "dev");
    }
    else if (strcmp(req, "stop") == 0) {
        strcpy(info.script, "/opt/isel/tvs/tvschat-stop.sh");
    }
    else if (strcmp(req, "stop -llm") == 0) {
        strcpy(info.script, "/opt/isel/tvs/tvschat-stop.sh");
        strcpy(info.arg, "-llm");
    }
    else if (strcmp(req, "status") == 0) {
        strcpy(info.script, "/opt/isel/tvs/tvschat-status.sh");
    }
    else {
        strcpy(info.script, ""); // indica erro
    }

    return info;
}
