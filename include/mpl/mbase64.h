#ifndef _MBASE64_H_
#define _MBASE64_H_

#include <stdint.h>
#include <openssl/bio.h>

namespace mpl
{
    int base64Encode(const uint8_t *in, int inlen, char *out, int *outlen);
    int base64Decode(const char *in, int inlen, uint8_t *out, int *outlen);
} // namespace mpl

#endif /* _MBASE64_H_ */
