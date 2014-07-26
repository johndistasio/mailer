#pragma once

#define DEFAULT_TLS_PORT "587"

/**
 * Holds all state necessary for program operation.
 */
struct email
{
    char *to_addr;
    char *to_name;
    char *from_addr;
    char *from_name;
    char *subject;
    char *body;
    char *server;
    char *port;
    char *user;
    char *password;
    int verbose;
};

/**
 *
 */
char* get_server_url(struct email *email);

/**
 *
 */
char* get_email_body(struct email *email);

/**
 *
 */
struct email* alloc_email();

/**
 *
 */
int send_email(struct email *email);

/**
 *
 */
void free_email(struct email *email);

/**
 *
 */
void dump_email(FILE *stream, struct email *email);
