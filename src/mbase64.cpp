#include "mbase64.h"
#include <string.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <openssl/ossl_typ.h>
#include <iostream>

namespace mpl {
    int base64Encode(const uint8_t *in, int inlen, char *out, int *outlen) {
        BIO *b64 = BIO_new(BIO_f_base64());
        BIO *bmem = BIO_new(BIO_s_mem());
        if (!b64 || !bmem) {
            return -1;
        }
        b64 = BIO_push(b64, bmem);
        *outlen = BIO_write(b64, in, inlen);
        if (*outlen <= 0 || *outlen != inlen) {
            return -1;
        }
        BIO_flush(b64);

        BUF_MEM *buf = nullptr;
        BIO_get_mem_ptr(b64, &buf);
        *outlen = buf->length;
        memcpy(out, buf->data, *outlen);
        BIO_free_all(b64);
        return 0;
    }

    int base64Decode(const char *in, int inlen, uint8_t *out, int *outlen) {
        BIO *b64 = BIO_new(BIO_f_base64());
        BIO *bmem = BIO_new_mem_buf(const_cast<char *>(in), inlen);
        if (!b64 || !bmem) {
            return -1;
        }
        b64 = BIO_push(b64, bmem);
        *outlen = BIO_read(b64, out, inlen);
        if (*outlen <= 0) {
            return -1;
        }
        BIO_free_all(b64);
        return 0;
    }
}   // namespace mpl
