#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <curl/curl.h>

#include "email.h"

static void print_usage(void)
{
    puts("Usage: mailer [options]"); puts("Options:");
    puts("    -h, --help:            print this message and exit");
    puts("    -v, --verbose:         TODO");
    puts("    -s, --subject:         TODO");
    puts("    -t, --to:              TODO");
    puts("    -n, --to-name:         TODO");
    puts("    -f, --from:            TODO");
    puts("    -m, --from-name:       TODO");
    puts("    -b, --body:            TODO");
    puts("    -r, --smtp-server:     TODO");
    puts("    -u, --smtp-user:       TODO");
    puts("    -p, --smtp-password:   TODO");
    puts("    -o, --smtp-port:       TODO");
}

static int parse_arguments(int argc, char **argv, struct email *email)
{
    int arg;
    int idx = 0;

    static struct option opts[] = {
        { "subject",       required_argument, 0, 's' },
        { "to",            required_argument, 0, 't' },
        { "to-name",       required_argument, 0, 'n' },
        { "from",          required_argument, 0, 'f' },
        { "from-name",     required_argument, 0, 'm' },
        { "body",          required_argument, 0, 'b' },
        { "smtp-server",   required_argument, 0, 'r' },
        { "smtp-user",     required_argument, 0, 'u' },
        { "smtp-password", required_argument, 0, 'p' },
        { "smtp-port",     required_argument, 0, 'o' },
        { "verbose",       no_argument,       0, 'v' },
        { "help",          no_argument,       0, 'h' },
        { 0, 0, 0, 0 }
    };

    while (1)
    {
       arg = getopt_long(argc, argv, "hs:t:n:f:m:b:r:u:p:o:", opts, &idx);

       if (arg == -1) break;

       switch (arg)
       {
           case 'h':
           {
               print_usage();
               return 1;
           }
           case 'v':
           {
               email->verbose = 1;
               break;
           }
           case 't':
           {
               email->to_addr = strdup(optarg);
               break;
           }
           case 'n':
           {
               email->to_name = strdup(optarg);
               break;
           }
           case 'f':
           {
               email->from_addr = strdup(optarg);
               break;
           }
           case 'm':
           {
               email->from_name = strdup(optarg);
               break;
           }
           case 's':
           {
               email->subject = strdup(optarg);
               break;
           }
           case 'b':
           {
               email->body = strdup(optarg);
               break;
           }
           case 'r':
           {
               email->server = strdup(optarg);
               break;
           }
           case 'u':
           {
               email->user = strdup(optarg);
               break;
           }
           case 'p':
           {
               email->password = strdup(optarg);
               break;
           }
           case 'o':
           {
               email->port = optarg;
               break;
           }
           case '?':
           {
               print_usage();
               return 1;
           }
           default:
           {
               break;
           }
       }
    }

    if (!email->port)
    {
        email->port = DEFAULT_TLS_PORT;
    }
    else if (!atoi(email->port))
    {
        fprintf(stderr, "'%s' is not a valid TCP port\n", email->port);
        return 1;
    }

    if (!email->body)
    {
        email->body = " ";
    }

    return 0;
}

static int send_email(struct email *email)
{
    CURL *curl;
    char *smtp_url;
    FILE *email_body;

    CURLcode result = CURLE_OK;
    struct curl_slist *rec = NULL;

    curl = curl_easy_init();

    if (!curl)
    {
        return 1;
    }

    if (email->verbose)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }

    smtp_url = get_server_url(email);

    curl_easy_setopt(curl, CURLOPT_URL, smtp_url);

    curl_easy_setopt(curl, CURLOPT_USERNAME, email->user);

    curl_easy_setopt(curl, CURLOPT_PASSWORD, email->password);

    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, email->from_addr);

    rec = curl_slist_append(rec, email->to_addr);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, rec);

    email_body = fmemopen(email->body, strlen(email->body), "r");

    curl_easy_setopt(curl, CURLOPT_READDATA, email_body);

    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    result = curl_easy_perform(curl);

    free(smtp_url);
    fclose(email_body);
    curl_slist_free_all(rec);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        fprintf(stderr, "%s\n", curl_easy_strerror(result));
        return 1;
    }

    return 0;
}


int main(int argc, char **argv)
{
    int result;
    struct email *email;
    
    email = alloc_email();
    result = parse_arguments(argc, argv, email);

    if (result != 0)
    {
        free_email(email);
        return 1;
    }

    result = send_email(email);

    if (result != 0)
    {
        free_email(email);
        return 1;
    }

    free_email(email);

    return 0;
}
