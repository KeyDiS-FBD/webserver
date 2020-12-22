#include <http.h>


HTTPreq *parse_request(char *request) {
    HTTPreq *req = malloc(sizeof(HTTPreq));

    req->request = request;
    req->method = strtok(request, " \r\n");
    req->path = strtok(NULL, " ");
    req->protocol = strtok(NULL, " \r\n");

    return req;
}
