#include "openssl/ssl.h"
#include "openssl/bio.h"
#include "openssl/err.h"

#include "stdio.h"
#include "string.h"

/* getPeerCert - Returns the peer certificate from an SSL connection, if one exists.
 *               The return code is the result from SSL_get_verify_result. It will
 *               retrieve the certificate regardless of what this code is - it's your
 *               job to determine what to do with a potentially bad certificate. If ssl
 *               is NULL, it will return -1.
 */

int getPeerCert(SSL * ssl, X509 ** peerCertificate)
{
    int returnCode;

    if(ssl == NULL) return -1;

    returnCode = SSL_get_verify_result(ssl);
    * peerCertificate = SSL_get_peer_certificate(ssl);
    return returnCode;
}

/* verifycert - Checks the Common Name field on a peer certificate against a
 *              host name. Returns positive if they match, zero if not, negative
 *              if either parameter is NULL.
 */
int verifycert(X509 * peerCertificate, const char * hostname)
{
    char commonName [512];
    X509_NAME * name;

    if(peerCertificate == NULL || hostname == NULL) return -1;

    /* Retrieve the Common Name from the certificate */
    name = X509_get_subject_name(peerCertificate);
    X509_NAME_get_text_by_NID(name, NID_commonName, commonName, 512);

    fprintf(stderr, "verifycert - Common Name on certificate: %s\n", commonName);
    
    /* More in-depth checks of the common name can be used if necessary. Here
     * we're only checking the common name and hostname to see if they match.
     */
    if(strcasecmp(commonName, hostname) == 0) return 1;
    else return 0;
}

int main()
{
    BIO * bio;
    SSL * ssl;
    SSL_CTX * ctx;
    X509 * peerCert;

    int p, totalRead;

    char * request = "GET / HTTP/1.1\x0D\x0AHost: www.sds-project.fr\x0D\x0A\x43onnection: Close\x0D\x0A\x0D\x0A";
    char r[1024];

    /* Set up the library */
    ERR_load_BIO_strings();
    SSL_load_error_strings();
    SSL_library_init();

    /* Set up the SSL context */
    ctx = SSL_CTX_new(SSLv23_client_method());

    /* Load the trust store - in this case, it's just a single
     * certificate that has been created for testing purposes.
     */
    if(! SSL_CTX_load_verify_locations(ctx, "cacert.pem", NULL))
    {
        fprintf(stderr, "Error loading trust store\n");
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return 0;
    }

    /* Setup the connection */
    bio = BIO_new_ssl_connect(ctx);

    /* Set the SSL_MODE_AUTO_RETRY flag */
    BIO_get_ssl(bio, & ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    /* Create and setup the connection */
    BIO_set_conn_hostname(bio, "www.sds-project.fr:https");
    fprintf(stderr, "Connecting to host www.sds-project.fr\n");

    if(BIO_do_connect(bio) <= 0)
    {
        fprintf(stderr, "Error attempting to connect\n");
        ERR_print_errors_fp(stderr);
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return 0;
    }

    /* Retrieve the peer certificate */
    fprintf(stderr, "Retrieving peer certificate\n");

    if(getPeerCert(ssl, & peerCert) != X509_V_OK)
    {
        /* Can be changed to better handle a suspect certificate. However,
         * for the purposes of this demonstration, we're aborting.
         */
        fprintf(stderr, "Certificate verification error: %li\n", SSL_get_verify_result(ssl));
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return 0;
    }

    /* Validate the peer certificate against the hostname */
    fprintf(stderr, "Validating peer certificate\n");

    if(! verifycert(peerCert, "www.sds-project.fr"))
    {
        /* Can be changed to better handle a suspect certificate. However,
         * for the purposes of this demonstration, we're aborting.
         */
        fprintf(stderr, "Hostname and Common Name do not match\n");
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        return 0;
    }

    /* Send the request */
    fprintf(stderr, "Sending request\n");
    BIO_write(bio, request, strlen(request));

    /* Read in the response */
    fprintf(stderr, "Reading response\n");
    totalRead = 0;
    for(;;)
    {
        p = BIO_read(bio, r, 1023);
        if(p <= 0) break;
        r[p] = 0; totalRead += p;
        /* Uncomment the next line to see the output on the screen */
        //printf("%s", r);
    }

    printf("\nTotal bytes read: %i\n", totalRead);
    /* Close the connection and free the context */

    BIO_free_all(bio);
    SSL_CTX_free(ctx);
    return 0;
}
