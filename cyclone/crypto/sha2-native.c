/* sha2.c -- SHA-2 digest algorithms native implementations  */
/* Copyright (c) 2015 Alexei Lozovsky.  All rights reserved. */
/* BSD-style license: http://synthcode.com/license.txt       */

#include "cyclone/types.h"
#include "cyclone/runtime.h"

/*
 * SHA-2 algorithms are described in RFC 6234:
 *
 *    http://tools.ietf.org/html/rfc6234
 */

/* Initial hash vector for SHA-224 */
static const uint32_t h224[8] = {
  0xC1059ED8UL, 0x367CD507UL, 0x3070DD17UL, 0xF70E5939UL,
  0xFFC00B31UL, 0x68581511UL, 0x64F98FA7UL, 0xBEFA4FA4UL,
};

/* Initial hash vector for SHA-256 */
static const uint32_t h256[8] = {
  0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL, 0xA54FF53AUL,
  0x510E527FUL, 0x9B05688CUL, 0x1F83D9ABUL, 0x5BE0CD19UL,
};

/* Round constants for SHA-224/256 */
static const uint32_t k256[64] = {
  0x428A2F98UL, 0x71374491UL, 0xB5C0FBCFUL, 0xE9B5DBA5UL,
  0x3956C25BUL, 0x59F111F1UL, 0x923F82A4UL, 0xAB1C5ED5UL,
  0xD807AA98UL, 0x12835B01UL, 0x243185BEUL, 0x550C7DC3UL,
  0x72BE5D74UL, 0x80DEB1FEUL, 0x9BDC06A7UL, 0xC19BF174UL,
  0xE49B69C1UL, 0xEFBE4786UL, 0x0FC19DC6UL, 0x240CA1CCUL,
  0x2DE92C6FUL, 0x4A7484AAUL, 0x5CB0A9DCUL, 0x76F988DAUL,
  0x983E5152UL, 0xA831C66DUL, 0xB00327C8UL, 0xBF597FC7UL,
  0xC6E00BF3UL, 0xD5A79147UL, 0x06CA6351UL, 0x14292967UL,
  0x27B70A85UL, 0x2E1B2138UL, 0x4D2C6DFCUL, 0x53380D13UL,
  0x650A7354UL, 0x766A0ABBUL, 0x81C2C92EUL, 0x92722C85UL,
  0xA2BFE8A1UL, 0xA81A664BUL, 0xC24B8B70UL, 0xC76C51A3UL,
  0xD192E819UL, 0xD6990624UL, 0xF40E3585UL, 0x106AA070UL,
  0x19A4C116UL, 0x1E376C08UL, 0x2748774CUL, 0x34B0BCB5UL,
  0x391C0CB3UL, 0x4ED8AA4AUL, 0x5B9CCA4FUL, 0x682E6FF3UL,
  0x748F82EEUL, 0x78A5636FUL, 0x84C87814UL, 0x8CC70208UL,
  0x90BEFFFAUL, 0xA4506CEBUL, 0xBEF9A3F7UL, 0xC67178F2UL,
};

/* Supported digest types */
enum sha_type {
  SHA_TYPE_224 = 0,
  SHA_TYPE_256,
  SHA_TYPE_MAX
};

/* Intermediate digest computation state */
struct sha_context {
  enum sha_type type;
  char sealed;
  uint32_t len;
  uint32_t hash256[8];
  uint8_t buffer[128]; /* enough for all SHA-2 */
};

/* = SHA-224/256 implementation ===================================== */

#define ror32(v, a) (((v) >> (a)) | ((v) << (32 - (a))))

static void sha_224_256_round (const uint8_t chunk[64],
                               uint32_t hash[8]) {
  int i;
  uint32_t w[64];
  uint32_t tmp1, tmp2;
  uint32_t a, b, c, d, e, f, g, h;
  /* Initialize schedule array */
  for (i = 0; i < 16; i++) {
    w[i] = (chunk[4*i + 0] << 24)
         | (chunk[4*i + 1] << 16)
         | (chunk[4*i + 2] <<  8)
         | (chunk[4*i + 3] <<  0);
  }
  for (i = 16; i < 64; i++) {
    w[i] = w[i - 16]
         + (ror32(w[i-15], 7) ^ ror32(w[i-15], 18) ^ (w[i-15] >> 3))
         + w[i - 7]
         + (ror32(w[i-2], 17) ^ ror32(w[i-2], 19) ^ (w[i-2] >> 10));
  }
  /* Initialize working variables */
  a = hash[0]; b = hash[1]; c = hash[2]; d = hash[3];
  e = hash[4]; f = hash[5]; g = hash[6]; h = hash[7];
  /* Main loop */
  for (i = 0; i < 64; i++) {
    tmp1 = h
         + (ror32(e, 6) ^ ror32(e, 11) ^ ror32(e, 25))
         + ((e & f) ^ ((~e) & g))
         + k256[i]
         + w[i];
    tmp2 = (ror32(a, 2) ^ ror32(a, 13) ^ ror32(a, 22))
         + ((a & b) ^ (a & c) ^ (b & c));
    h = g; g = f; f = e; e = d + tmp1;
    d = c; c = b; b = a; a = tmp1 + tmp2;
  }
  /* Update hash values */
  hash[0] += a; hash[1] += b; hash[2] += c; hash[3] += d;
  hash[4] += e; hash[5] += f; hash[6] += g; hash[7] += h;
}

static void sha_224_256_remainder (uint8_t chunk[64], uint32_t offset,
                                   uint32_t len_bits, uint32_t hash[8]) {
  int i;
  /* Pad with '1' bit and zeros */
  chunk[offset] = 0x80;
  memset(chunk + offset + 1, 0, 64 - offset - 1);
  /* If we can't fit the length, use an additional chunk */
  if (offset >= 56) {
    sha_224_256_round(chunk, hash);
    memset(chunk, 0, 64);
  }
  /* Append the message length in bits as big-endian 64-bit integer */
  for (i = 63; i >= 56; i--) {
    chunk[i] = len_bits & 0xFF;
    len_bits >>= 8;
  }
  sha_224_256_round(chunk, hash);
}

/* = Allocating computation context ================================= */

static void start_sha (void *data, object cont, object type_arg) {
  Cyc_check_int(data, type_arg);
  struct sha_context *sha;
  int type = obj_obj2int(type_arg);
  if (type >= SHA_TYPE_MAX) {
    Cyc_rt_raise2(data, "SHA-2 digest type not supported", type_arg);
  }
  sha = calloc(1, sizeof(*sha));
  sha->type = type;
  switch (type) {
  case SHA_TYPE_224:
    memcpy(sha->hash256, h224, sizeof(h224));
    break;
  case SHA_TYPE_256:
    memcpy(sha->hash256, h256, sizeof(h256));
    break;
  default:
    break;
  }
  make_c_opaque(opq, sha);
  opaque_collect_ptr(&opq) = 1; // Cyclone GC will free this memory
  return_closcall1(data, cont, &opq);
}

/* = Processing incoming data ======================================= */

static object sha_224_256_add_bytes (void *data, object opq, //struct sha_context *sha,
                                     object chunk) { //const sexp_uint8_t *src, sexp_uint_t len) {
  Cyc_check_opaque(data, opq);
  Cyc_check_obj(data, string_tag, chunk);

  struct sha_context *sha = opaque_ptr(opq);
  const uint8_t *src = NULL;
  int len = 0;

  if (type_of(chunk) == string_tag) {
    src = (uint8_t *)string_str(chunk);
    len = string_len(chunk);
  } else if (type_of(chunk) == bytevector_tag) {
    bytevector_type *bv = chunk;
    src = (uint8_t *)bv->data;
    len = bv->len;
  } else {
    Cyc_rt_raise2(data, "Unhandled type for adding SHA data", chunk);
  }

  

  uint32_t src_offset, buf_offset;
  /* Realign (src + src_offset) to 64 bytes */
  src_offset = 0;
  buf_offset = sha->len % 64;
  sha->len += len;
  if (buf_offset) {
    while ((buf_offset < 64) && (src_offset < len))
      sha->buffer[buf_offset++] = src[src_offset++];
    if (buf_offset == 64)
      sha_224_256_round(sha->buffer, sha->hash256);
    else
      return boolean_t;
  }
  /* Process whole chunks without copying them */
  if (len >= 64) {
    for ( ; src_offset <= (len - 64); src_offset += 64)
      sha_224_256_round(src + src_offset, sha->hash256);
  }
  /* Copy the remainder into the buffer */
  if (src_offset < len)
    memcpy(sha->buffer + buf_offset, src + src_offset, len - src_offset);
  return boolean_t;
}
// 
// static sexp sha_add_bytes (sexp ctx, sexp self, struct sha_context *sha,
//                            const char* data, sexp_uint_t len) {
//   switch (sha->type) {
//   case SHA_TYPE_224:
//   case SHA_TYPE_256:
//     return sha_224_256_add_bytes(sha, (const sexp_uint8_t*) data, len);
//   default:
//     return sexp_xtype_exception(ctx, self, "unexpected context type",
//                                 sexp_make_fixnum(sha->type));
//   }
// }
// 
// sexp sexp_add_sha_data (sexp ctx, sexp self, struct sha_context *sha, sexp data) {
//   if (sha->sealed)
//     return sexp_xtype_exception(ctx, self, "cannot add to sealed context", data);
//   if (sexp_bytesp(data))
//     return sha_add_bytes(ctx, self, sha, sexp_bytes_data(data), sexp_bytes_length(data));
//   if (sexp_stringp(data))
//     return sha_add_bytes(ctx, self, sha, sexp_string_data(data), sexp_string_size(data));
//   return sexp_xtype_exception(ctx, self, "data type not supported", data);
// }

/* = Extracting computed digest ===================================== */

static const char *hex = "0123456789abcdef";

static void sha_224_256_hash_string (void *data, object k,
                                     const uint32_t hash[8], int count) {
  int i, j;
  uint32_t next_word;
  /* Allocate a string of target length */
  //res = sexp_make_string(ctx, sexp_make_fixnum(count * 8), SEXP_VOID);
  //if (sexp_exceptionp(res))
  //  return res;
  char *buf = alloca(count * 8);

  /* Write 32-bit words as nibbles in big-endian order */
  for (i = 0; i < count; i++) {
    next_word = hash[i];
    for (j = 7; j >= 0; j--) {
      buf[8*i + j] = hex[next_word & 0xF];
      next_word >>= 4;
    }
  }
  buf[count * 8] = '\0';
  make_string(str, buf);
  return_closcall1(data, k, &str);
}

static void get_sha (void *data, object k, object opq) {
  Cyc_check_opaque(data, opq);
  struct sha_context *sha = opaque_ptr(opq);
  if (!sha->sealed) {
    sha->sealed = 1;
    switch (sha->type) {
    case SHA_TYPE_224:
    case SHA_TYPE_256:
      sha_224_256_remainder(sha->buffer, sha->len % 64,
                            sha->len * 8, sha->hash256);
      break;
    default:
      break;
    }
  }
  switch (sha->type) {
  case SHA_TYPE_224:
    sha_224_256_hash_string(data, k, sha->hash256, 7);
  case SHA_TYPE_256:
    sha_224_256_hash_string(data, k, sha->hash256, 8);
  default:
    Cyc_rt_raise2(data, "Unexpected SHA type", obj_int2obj(sha->type));
  }
}
