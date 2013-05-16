// kssl.h: definitions for the Keyless SSL protocol
//
// Copyright (c) 2013 CloudFlare, Inc.

#ifndef INCLUDED_KSSL
#define INCLUDED_KSSL 1

#include <stdint.h>

#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t

// These macros define the size of the kssl_header and kssl_item
// structures when they are serialized onto the wire (see
// kssl_helpers.c for seralization functions). These are not the same
// thing as sizeof(kssl_header) and sizeof(kssl_item) because
// of structure packing; the structure are used on deserialized
// data and are not direct representations of the wire protocol.
#define KSSL_HEADER_SIZE (sizeof(BYTE) + sizeof(BYTE) + sizeof(WORD) + sizeof(DWORD))
#define KSSL_ITEM_HEADER_SIZE (sizeof(BYTE) + sizeof(WORD))

typedef struct {
  BYTE version_maj; // Protocol major version (see KSSL_VERSION_MAJ)
  BYTE version_min; // Protocol minor version (see KSSL_VERSION_MIN)
  WORD length;      // Length of the payload
  DWORD id;         // Unique ID generated by client for this operation
  BYTE *data;       // Message contents TLVs
} kssl_header;

// The current KSSL protocol version
#define KSSL_VERSION_MAJ 0x01
#define KSSL_VERSION_MIN 0x00

// Possible item tags

#define KSSL_TAG_DIGEST     0x01 // An RSA key digest (see
                                 // digest_pubilc_modulus)
#define KSSL_TAG_SNI        0x02 // Server name (optional)
#define KSSL_TAG_CLIENT_IP  0x03 // Client IP (4 bytes for IPv4, 16 for IPv6)
#define KSSL_TAG_OPCODE     0x11 // Requested operation (one of KSSL_OP_*)
#define KSSL_TAG_PAYLOAD    0x12 // Payload

// This structure stores the value of a given tag
typedef struct {
  BYTE tag;       // Tag to identify contents of item
  WORD length;    // Length of the item data
  BYTE *data;     // The block of data to decrypt or sign
} kssl_item;

// Possible values for KSSL_TAG_OPCODE

// A test message which will be echoed with its payload with the
// operation changed to OP_PONG
#define KSSL_OP_PING 0xF1
#define KSSL_OP_PONG 0xF2

// Decrypt data encrypted using RSA with RSA_PKCS1_PADDING
#define KSSL_OP_RSA_DECRYPT 0x01

// Sign data using RSA
#define KSSL_OP_RSA_SIGN_MD5SHA1     0x02
#define KSSL_OP_RSA_SIGN_SHA1        0x03
#define KSSL_OP_RSA_SIGN_SHA224      0x04
#define KSSL_OP_RSA_SIGN_SHA256      0x05
#define KSSL_OP_RSA_SIGN_SHA384      0x06
#define KSSL_OP_RSA_SIGN_SHA512      0x07

// Used to send a block of data back to the client (in response, for
// example, to a KSSL_OP_RSA_DECRYPT)
#define KSSL_OP_RESPONSE             0xF0
#define KSSL_OP_ERROR                0xFF

// Some error occurred, explanation is single byte in payload
#define KSSL_ERROR_NONE              0x00
#define KSSL_ERROR_CRYPTO_FAILED     0x01
#define KSSL_ERROR_KEY_NOT_FOUND     0x02
#define KSSL_ERROR_READ              0x03
#define KSSL_ERROR_VERSION_MISMATCH  0x04
#define KSSL_ERROR_BAD_OPCODE        0x05
#define KSSL_ERROR_UNEXPECTED_OPCODE 0x06
#define KSSL_ERROR_FORMAT            0x07
#define KSSL_ERROR_INTERNAL          0x08

#endif // INCLUDED_KSSL
