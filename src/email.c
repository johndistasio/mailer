#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "email.h"

struct email* alloc_email()
{
    struct email *email = malloc(sizeof(struct email));

    email->subject   = NULL;
    email->body      = NULL;
    email->to_addr   = NULL;
    email->to_name   = NULL;
    email->from_addr = NULL;
    email->from_name = NULL;
    email->server    = NULL;
    email->user      = NULL;
    email->password  = NULL;
    email->verbose   = 0;

    return email;
}

void free_email(struct email *email)
{
    if (email == NULL)
    {
        return;
    }

    free(email->subject);
    free(email->body);
    free(email->to_addr);
    free(email->to_name);
    free(email->from_addr);
    free(email->from_name);
    free(email->server);
    free(email->user);
    free(email->password);

    free(email);
}

char* get_server_url(struct email *email)
{
    size_t sz = sizeof(char) * (strlen(email->server) + strlen(email->port) + 9);
    char *url = malloc(sz);

    snprintf(url, sz, "smtp://%s:%s", email->server, email->port);

    return url;
}

char* get_email_body(struct email *email)
{
    return " ";
}

void dump_email(FILE *stream, struct email *email)
{
    fprintf(stream, "verbose: %d\n", email->verbose);
    fprintf(stream, "server: %s\n", email->server);
    fprintf(stream, "user: %s\n", email->user);
    fprintf(stream, "password: %s\n", email->password);
    fprintf(stream, "to: %s <%s>\n", email->to_name, email->to_addr);
    fprintf(stream, "from: %s <%s>\n", email->from_name, email->from_addr);
    fprintf(stream, "subject: %s\n", email->subject);
    fprintf(stream, "body: %s\n", email->body);
}
