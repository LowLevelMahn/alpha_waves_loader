#include <cstdint>
#include <vector>
#include <stack>
#include <array>
#include <cstring>
#include <cassert>
#include <cstdio>

const uint8_t compressed_example[] = {
    0x04, 0x01, 0xBA, 0x01, 0x0C, 0x15, 0x19, 0x15, 0x00, 0x0C, 0x15, 0x19, 0x00, 0x0C, 0x15, 0x19, 0xE9, 0x10, 0x0A,
    0x15, 0x15, 0x19, 0x52, 0x01, 0x40, 0x80, 0x15, 0x19, 0x00, 0x0B, 0x01, 0x16, 0x01, 0x19, 0x29, 0x01, 0x40, 0x15,
    0x19, 0x0C, 0x03, 0x01, 0x13, 0x01, 0x19, 0x04, 0x0C, 0x00, 0x08, 0x0C, 0x00, 0xFD, 0x04, 0xFD, 0x04, 0xA9, 0x05,
    0xFD, 0x04, 0x58, 0x05, 0xA7, 0x01, 0xD7, 0x01, 0x07, 0x02, 0x37, 0x02, 0x67, 0x02, 0x97, 0x02, 0xC7, 0x02, 0xF7,
    0x02, 0x27, 0x03, 0x57, 0x03, 0x87, 0x03, 0xB7, 0x03, 0x3A, 0x02, 0x38, 0x02, 0x37, 0x02, 0x35, 0x02, 0x34, 0x02,
    0x33, 0x02, 0x31, 0x02, 0x30, 0x02, 0x2F, 0x02, 0x2D, 0x02, 0x2C, 0x02, 0x2B, 0x02, 0x29, 0x02, 0x28, 0x02, 0x27,
    0x02, 0x25, 0x02, 0x24, 0x02, 0x23, 0x02, 0x21, 0x02, 0x20, 0x02, 0x1F, 0x02, 0x1D, 0x02, 0x1C, 0x02, 0x1B, 0x02,
    0x1A, 0x02, 0x18, 0x02, 0x17, 0x02, 0x16, 0x02, 0x14, 0x02, 0x13, 0x02, 0x12, 0x02, 0x11, 0x02, 0x0F, 0x02, 0x0E,
    0x02, 0x0D, 0x02, 0x0B, 0x02, 0x0A, 0x02, 0x09, 0x02, 0x08, 0x02, 0x06, 0x02, 0x05, 0x02, 0x04, 0x02, 0x03, 0x02,
    0x02, 0x02, 0x00, 0x02, 0xFF, 0x01, 0xFE, 0x01, 0xFD, 0x01, 0xFB, 0x01, 0xFA, 0x01, 0xF9, 0x01, 0xF8, 0x01, 0xF6,
    0x01, 0xF5, 0x01, 0xF4, 0x01, 0xF3, 0x01, 0xF2, 0x01, 0xF0, 0x01, 0xEF, 0x01, 0xEE, 0x01, 0xED, 0x01, 0xEC, 0x01,
    0xEA, 0x01, 0xE9, 0x01, 0xE8, 0x01, 0xE7, 0x01, 0xE6, 0x01, 0xE4, 0x01, 0xE3, 0x01, 0xE2, 0x01, 0xE1, 0x01, 0xE0,
    0x01, 0xDF, 0x01, 0xDE, 0x01, 0xDC, 0x01, 0xDB, 0x01, 0xDA, 0x01, 0xD9, 0x01, 0xD8, 0x01, 0xD7, 0x01, 0xD6, 0x01,
    0xD4, 0x01, 0xD3, 0x01, 0xD2, 0x01, 0xD1, 0x01, 0xD0, 0x01, 0xCF, 0x01, 0xCE, 0x01, 0xCD, 0x01, 0xCC, 0x01, 0xCA,
    0x01, 0xC9, 0x01, 0xC8, 0x01, 0xC7, 0x01, 0xC6, 0x01, 0xC5, 0x01, 0xC4, 0x01, 0xC3, 0x01, 0xC2, 0x01, 0xC1, 0x01,
    0xC0, 0x01, 0xBE, 0x01, 0xBD, 0x01, 0xBC, 0x01, 0xBB, 0x01, 0xBA, 0x01, 0xB9, 0x01, 0xB8, 0x01, 0xB7, 0x01, 0xB6,
    0x01, 0xB5, 0x01, 0xB4, 0x01, 0xB3, 0x01, 0xB2, 0x01, 0xB1, 0x01, 0xB0, 0x01, 0xAF, 0x01, 0xAE, 0x01, 0xAD, 0x01,
    0xAC, 0x01, 0xAB, 0x01, 0xA9, 0x01, 0xA8, 0x01, 0xA7, 0x01, 0xA6, 0x01, 0xA5, 0x01, 0xA4, 0x01, 0xA3, 0x01, 0xA2,
    0x01, 0xA1, 0x01, 0xA0, 0x01, 0x9F, 0x01, 0x9E, 0x01, 0x9D, 0x01, 0x9C, 0x01, 0x9B, 0x01, 0x9A, 0x01, 0x99, 0x01,
    0x98, 0x01, 0x97, 0x01, 0x96, 0x01, 0x96, 0x01, 0x95, 0x01, 0x94, 0x01, 0x93, 0x01, 0x92, 0x01, 0x91, 0x01, 0x90,
    0x01, 0x8F, 0x01, 0x8E, 0x01, 0x8D, 0x01, 0x8C, 0x01, 0x8B, 0x01, 0x8A, 0x01, 0x89, 0x01, 0x88, 0x01, 0x87, 0x01,
    0x86, 0x01, 0x85, 0x01, 0x84, 0x01, 0x83, 0x01, 0x82, 0x01, 0x81, 0x01, 0x80, 0x01, 0x80, 0x01, 0x7F, 0x01, 0x7E,
    0x01, 0x7D, 0x01, 0x7C, 0x01, 0x7B, 0x01, 0x7A, 0x01, 0x79, 0x01, 0x78, 0x01, 0x77, 0x01, 0x76, 0x01, 0x75, 0x01,
    0x75, 0x01, 0x74, 0x01, 0x73, 0x01, 0x72, 0x01, 0x71, 0x01, 0x70, 0x01, 0x6F, 0x01, 0x6E, 0x01, 0x6D, 0x01, 0x6D,
    0x01, 0x6C, 0x23, 0x01, 0x28, 0x04, 0x16, 0x6C, 0x6D, 0x6E, 0x6F, 0x71, 0x72, 0x76, 0x79, 0x7A, 0x7C, 0x7D, 0x7E,
    0x82, 0x84, 0x86, 0x87, 0x8E, 0x91, 0x92, 0x93, 0x94, 0x96, 0x94, 0x96, 0x97, 0x99, 0x9A, 0x9B, 0x9F, 0xA2, 0xA3,
    0xA2, 0xA4, 0xA5, 0x00, 0xF7, 0x47, 0x26, 0xD0, 0x45, 0x00, 0x00, 0xF7, 0x45, 0x88, 0xF7, 0x16, 0x6E, 0x88, 0x5E,
    0x00, 0x04, 0x80, 0x6C, 0xF7, 0x03, 0x94, 0x96, 0x87, 0x84, 0x8C, 0x8E, 0x52, 0x89, 0x83, 0xA2, 0x6F, 0x40, 0x8B,
    0x00, 0xC2, 0x04, 0x8A, 0xE9, 0x02, 0x74, 0x75, 0x05, 0x04, 0x4F, 0x6D, 0x16, 0x44, 0x45, 0xC3, 0x40, 0x2E, 0x74,
    0x00, 0x71, 0x55, 0x04, 0x86, 0x74, 0x07, 0x47, 0xA3, 0x01, 0x4F, 0x4F, 0x04, 0x6F, 0x76, 0x77, 0x01, 0x6B, 0x01,
    0x6A, 0x01, 0x69, 0x01, 0x68, 0x01, 0x67, 0x01, 0x67, 0x01, 0x66, 0x01, 0x65, 0x01, 0x64, 0x01, 0x63, 0x01, 0x62,
    0x01, 0x61, 0x01, 0x60, 0x01, 0x60, 0x01, 0x5F, 0x01, 0x5E, 0x01, 0x5D, 0x01, 0x5C, 0x01, 0x5B, 0x01, 0x5B, 0x01,
    0x5A, 0x01, 0x59, 0x01, 0x58, 0x01, 0x57, 0x01, 0x56, 0x01, 0x56, 0x01, 0x55, 0x01, 0x54, 0x01, 0x53, 0x01, 0x9B,
    0x9B, 0x51, 0x01, 0x50, 0x01, 0x4F, 0x01, 0x4E, 0x01, 0x4D, 0x01, 0x4D, 0x01, 0x4C, 0x01, 0x4B, 0x01, 0x4A, 0x01,
    0x49, 0x01, 0x49, 0x01, 0x48, 0x01, 0x47, 0x01, 0x46, 0x01, 0x46, 0x01, 0x45, 0x01, 0x44, 0x01, 0x43, 0x01, 0x42,
    0x01, 0x42, 0x01, 0x41, 0x01, 0x40, 0x01, 0x3F, 0x01, 0x3F, 0x01, 0x3E, 0x01, 0x3D, 0x01, 0x3C, 0x01, 0x3B, 0x01,
    0x3B, 0x01, 0x3A, 0x01, 0x39, 0x01, 0x38, 0x01, 0x38, 0x01, 0x37, 0x01, 0x36, 0x01, 0x35, 0x01, 0x35, 0x01, 0x34,
    0x01, 0x33, 0x01, 0x32, 0x01, 0x32, 0x01, 0x31, 0x01, 0x30, 0x01, 0x30, 0x01, 0x2F, 0x01, 0x2E, 0x01, 0x2D, 0x01,
    0x2D, 0x01, 0x2C, 0x01, 0x2B, 0x01, 0x2B, 0x01, 0x2A, 0x01, 0x29, 0x01, 0x28, 0x01, 0x28, 0x01, 0x27, 0x01, 0x26,
    0x01, 0x25, 0x01, 0x25, 0x01, 0x24, 0x01, 0x23, 0x01, 0x23, 0x01, 0x22, 0x01, 0x21, 0x01, 0x21, 0x01, 0x20, 0x01,
    0x1F, 0x01, 0x1F, 0x01, 0x1E, 0x01, 0x1D, 0x01, 0x80, 0xA0, 0xC0, 0x1D, 0x06, 0x78, 0x06, 0x47, 0x06, 0x8D, 0x06,
    0x8C, 0x06, 0x43, 0x06, 0x0C, 0x06, 0x79, 0x79, 0x79, 0x16, 0x00, 0x01, 0x01, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E,
    0x00, 0xCB, 0x57, 0x53, 0x33, 0xC0, 0x8A, 0xC1, 0x24, 0x0F, 0xD1, 0xE0, 0x8B, 0xF8, 0x2E, 0x8B, 0xBD, 0x8F, 0x01,
    0xF7, 0xC5, 0x80, 0x72, 0x11, 0xF6, 0xC1, 0x0F, 0x75, 0x0C, 0xBF, 0xE7, 0x03, 0x8A, 0xC1, 0xA8, 0x70, 0x74, 0x03,
    0x80, 0xE9, 0x10, 0xF6, 0xC1, 0x91, 0x03, 0xBA, 0x40, 0x00, 0x52, 0x8B, 0xC5, 0x98, 0xD1, 0xE0, 0x03, 0xF8, 0x8B,
    0x05, 0x80, 0xE1, 0x70, 0xA2, 0xA2, 0x80, 0xE9, 0x07, 0xF6, 0xD9, 0xD3, 0xE0, 0x59, 0xF7, 0xC1, 0xA4, 0x06, 0xB9,
    0x87, 0x5B, 0x5F, 0xC3, 0x33, 0xC9, 0x5B, 0x5F, 0xC3, 0xB0, 0xB6, 0xE6, 0x43, 0xE4, 0x61, 0x24, 0xFC, 0xE6, 0x61,
    0xC3, 0x79, 0x02, 0x72, 0x14, 0x6E, 0x64, 0x01, 0xF6, 0xC4, 0x80, 0x75, 0x01, 0xC3, 0x32, 0xC0, 0x89, 0x05, 0x6E,
    0x04, 0x88, 0x71, 0x79, 0x04, 0x76, 0x27, 0x8A, 0x71, 0x0A, 0xC0, 0x74, 0x06, 0xFE, 0xC8, 0x88, 0x71, 0xC3, 0x79,
    0x96, 0x08, 0x7D, 0x96, 0x01, 0xC3, 0x83, 0x0D, 0x04, 0x79, 0x00, 0x20, 0x74, 0x03, 0x83, 0x0D, 0x02, 0x83, 0xE2,
    0xFC, 0xC3, 0x93, 0x02, 0x72, 0x18, 0x6E, 0x64, 0x0C, 0xF6, 0xC4, 0x80, 0x75, 0x01, 0xC3, 0x32, 0xC0, 0xC7, 0x7A,
    0x16, 0xC7, 0x05, 0x16, 0x89, 0x71, 0x56, 0x8B, 0x2D, 0x2E, 0x03, 0xB6, 0x7B, 0x01, 0x2E, 0xFF, 0xA6, 0x85, 0x01,
    0x93, 0x01, 0x76, 0x20, 0x6E, 0x04, 0xA8, 0x40, 0x74, 0x09, 0xC7, 0x05, 0x08, 0x00, 0x94, 0x24, 0x3F, 0x84, 0x06,
    0x82, 0x02, 0x97, 0x83, 0x4D, 0x02, 0x05, 0x8A, 0x45, 0x07, 0x0A, 0xC0, 0x74, 0x0A, 0xFE, 0xC8, 0x97, 0x94, 0x8A,
    0x45, 0x06, 0x0A, 0xC0, 0x74, 0x2C, 0xFE, 0xC8, 0x84, 0x06, 0x82, 0x02, 0x97, 0x82, 0x01, 0x98, 0x03, 0x7A, 0x89,
    0x7A, 0x94, 0x93, 0x00, 0x20, 0x74, 0x04, 0x83, 0x4D, 0x02, 0x02, 0x94, 0x93, 0x04, 0x72, 0x2B, 0x83, 0x65, 0x02,
    0xFB, 0x26, 0xF6, 0x04, 0x91, 0x0D, 0x82, 0x03, 0x98, 0x03, 0x7A, 0x89, 0x7A, 0xEB, 0x9E, 0x82, 0x01, 0x98, 0x03,
    0x7A, 0x89, 0x7A, 0x82, 0x03, 0x97, 0xEB, 0x8A, 0x83, 0x65, 0x02, 0xFE, 0x8B, 0x05, 0x05, 0x02, 0x00, 0x89, 0x05,
    0x5E, 0xE9, 0x47, 0xFF, 0x93, 0x96, 0xEA, 0x7D, 0x96, 0xE3, 0x94, 0x56, 0x57, 0x8B, 0x6D, 0xA9, 0xA4, 0x2E, 0xA9,
    0x02, 0x76, 0x2D, 0xFF, 0x4F, 0x0E, 0x75, 0x24, 0x8B, 0x47, 0x10, 0x89, 0x47, 0x0E, 0xC4, 0x77, 0x0A, 0x26, 0xAD,
    0x89, 0x77, 0x0A, 0x99, 0x0C, 0x8A, 0xCC, 0x32, 0xED, 0x32, 0xE4, 0xD0, 0xE0, 0x8B, 0xF8, 0x9C, 0x2E, 0xFF, 0x95,
    0xEA, 0x03, 0x9D, 0x73, 0xE2, 0x5F, 0x86, 0xC3, 0xC4, 0x47, 0x06, 0x89, 0x47, 0x0A, 0x99, 0x0C, 0x83, 0x67, 0x04,
    0xFD, 0xC7, 0x47, 0x18, 0x16, 0xEB, 0xCA, 0x53, 0x1E, 0x51, 0x8B, 0x07, 0x50, 0x2E, 0xFF, 0x1E, 0xFE, 0x03, 0x83,
    0xC4, 0x04, 0x1F, 0x5B, 0xC3, 0xA3, 0x02, 0x7D, 0x20, 0x76, 0x1A, 0xC7, 0x47, 0x25, 0x19, 0x01, 0x8C, 0x4F, 0x27,
    0xA3, 0x40, 0x7D, 0x00, 0x91, 0x07, 0xA5, 0x02, 0x83, 0x64, 0x04, 0xFB, 0xC3, 0x7C, 0x17, 0xC3, 0xFF, 0x47, 0x18,
    0x9F, 0x15, 0x8A, 0x2F, 0x33, 0xED, 0x8B, 0x57, 0x04, 0xE8, 0xC6, 0xFD, 0x89, 0x47, 0x23, 0x8B, 0x6D, 0x25, 0xFF,
    0xBF, 0x0B, 0xC1, 0x89, 0x6D, 0xA9, 0x96, 0x0D, 0xA5, 0x1F, 0x83, 0x4C, 0x02, 0x02, 0xA5, 0x21, 0x83, 0x0C, 0x02,
    0xC3, 0x26, 0xAC, 0x8A, 0xE8, 0x03, 0x4F, 0x13, 0x9F, 0x10, 0x9F, 0x0E, 0x89, 0x77, 0x0A, 0x99, 0x0C, 0xC3, 0xC3,
    0x7C, 0x12, 0xB0, 0x10, 0xF6, 0xE1, 0x2E, 0xC4, 0x36, 0x18, 0x04, 0x03, 0xF0, 0x89, 0x77, 0x25, 0x99, 0x27, 0xC3,
    0x50, 0x52, 0x06, 0x1E, 0x56, 0x57, 0x55, 0x2E, 0xF6, 0x06, 0x00, 0x04, 0xFF, 0x74, 0x08, 0x5D, 0x5F, 0x5E, 0x1F,
    0x07, 0x5A, 0x58, 0xC3, 0xBB, 0x9B, 0x8B, 0xF3, 0x2E, 0xA1, 0x0C, 0x9A, 0x18, 0x8E, 0xA1, 0x0E, 0x9A, 0x1A, 0x04,
    0xE8, 0xEB, 0xFE, 0x7D, 0x04, 0x72, 0x1A, 0x2E, 0xA1, 0x10, 0x9A, 0x18, 0x8E, 0xA1, 0x12, 0x9A, 0x1A, 0x04, 0x53,
    0x8B, 0x5F, 0x02, 0x8B, 0xF3, 0xE8, 0xCB, 0xFE, 0x5B, 0x8B, 0xDE, 0xC4, 0x77, 0x25, 0x46, 0x8B, 0x57, 0x23, 0x26,
    0xF6, 0x44, 0x0C, 0x91, 0x06, 0x8B, 0x7F, 0x1F, 0xE8, 0xC9, 0xFD, 0x8A, 0xC2, 0xE6, 0x42, 0x8A, 0xC6, 0xE6, 0x42,
    0x83, 0xC6, 0x0D, 0x33, 0xD2, 0x26, 0xF6, 0x44, 0x01, 0x91, 0x09, 0xBA, 0x03, 0x00, 0x8B, 0x7F, 0x21, 0xE8, 0x61,
    0xFD, 0xE4, 0x61, 0x24, 0xFC, 0x0A, 0xC2, 0xE6, 0x61, 0x5D, 0x5F, 0x5E, 0x1F, 0x07, 0x5A, 0x58, 0xC3, 0xC3, 0x8C,
    0xC8, 0x8B, 0xD0, 0xB8, 0xFE, 0x03, 0xC3, 0x8B, 0xEE, 0xBE, 0xFF, 0xFF, 0x0B, 0xED, 0x75, 0x03, 0xBD, 0x01, 0x00,
    0xBB, 0x9B, 0xBF, 0x01, 0x00, 0x85, 0xEF, 0x74, 0x08, 0x8B, 0x47, 0x06, 0x0B, 0x47, 0x08, 0x75, 0x03, 0xE9, 0xC1,
    0x00, 0x92, 0x01, 0x74, 0x1F, 0x0B, 0xC9, 0x74, 0x09, 0x2E, 0xC6, 0x06, 0x00, 0x04, 0x00, 0xEB, 0x0A, 0x90, 0x2E,
    0xC6, 0x06, 0x00, 0x04, 0x01, 0xE8, 0x00, 0xFD, 0x92, 0x04, 0x74, 0x03, 0x9F, 0x13, 0x6C, 0x40, 0x72, 0x13, 0x7D,
    0xA4, 0x0C, 0xA3, 0x40, 0xC7, 0x47, 0x25, 0x19, 0x01, 0x8C, 0x4F, 0x27, 0x6C, 0x80, 0x72, 0x11, 0xC6, 0x47, 0x1D,
    0x0F, 0xC6, 0x47, 0x1A, 0x0F, 0xC6, 0x47, 0x1E, 0x00, 0xC7, 0x6D, 0x02, 0x00, 0x6C, 0x20, 0x72, 0x04, 0xA3, 0x20,
    0x92, 0x20, 0x74, 0x17, 0x6C, 0x10, 0x76, 0x11, 0x51, 0xA2, 0x6F, 0x83, 0xE1, 0x0F, 0x7C, 0x1D, 0x7C, 0x1A, 0x59,
    0x92, 0x91, 0x0E, 0x51, 0xA2, 0x6F, 0x83, 0xE1, 0x0F, 0x7C, 0x1A, 0x59, 0x92, 0x10, 0x74, 0x06, 0x7C, 0x1B, 0x7C,
    0x1C, 0x6C, 0x10, 0x72, 0x1F, 0x92, 0x20, 0x75, 0x12, 0x7D, 0xA4, 0x12, 0x3B, 0x77, 0x18, 0x7F, 0x0D, 0xA5, 0x18,
    0xEB, 0x08, 0x90, 0x38, 0x4F, 0x1D, 0x74, 0x02, 0x33, 0xF6, 0xD1, 0xE7, 0x83, 0xC3, 0x29, 0x81, 0x22, 0x00, 0xAD,
    0x02, 0x1B, 0x1C, 0x22, 0x24, 0x28, 0x2A, 0x2D, 0x30, 0x34, 0x37, 0x3D, 0x3F, 0x48, 0x4B, 0x4E, 0x4B, 0x58, 0x60,
    0x61, 0x62, 0x61, 0x63, 0x64, 0x63, 0x64, 0x63, 0x64, 0x65, 0x64, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x26, 0xF7, 0xD1,
    0x04, 0x04, 0x52, 0xD0, 0x1C, 0x19, 0x8B, 0x00, 0x83, 0xC7, 0x25, 0x4B, 0x89, 0xCD, 0x8B, 0x24, 0x61, 0x01, 0x3F,
    0x63, 0x64, 0x63, 0x64, 0x83, 0x64, 0xBB, 0x83, 0x89, 0x0B, 0x04, 0x22, 0x8B, 0xC2, 0xEA, 0x2E, 0x40, 0x01, 0xE9,
    0x00, 0x01, 0x77, 0x74, 0x4F, 0x47, 0x34, 0x8C, 0x47, 0x21, 0xFE, 0x8C, 0x06, 0x00, 0x28, 0x48, 0x4E, 0x4F, 0x27,
    0xC3, 0x29, 0x2A, 0x4C, 0x3E, 0xD0, 0x26, 0x22, 0xFB, 0x7B, 0x01, 0x72, 0x0C, 0x8B, 0xC6, 0x30, 0x02, 0x74, 0x03,
    0xE8, 0x00, 0xFF, 0xC3, 0xE9, 0x16, 0xFF, 0x8B, 0xEE, 0xBE, 0xFF, 0xFF, 0x0B, 0xED, 0x75, 0x03, 0xBD, 0x61, 0xBB,
    0x29, 0x01, 0xBF, 0x61, 0x85, 0xEF, 0x75, 0x03, 0xE9, 0xC7, 0x00, 0x30, 0x04, 0x74, 0x03, 0x89, 0x4F, 0x13, 0x1C,
    0x40, 0x3D, 0x0C, 0x63, 0x1C, 0x80, 0x3D, 0x46, 0x30, 0x40, 0x74, 0x07, 0xF7, 0x47, 0x28, 0x3D, 0x39, 0x56, 0x51,
    0x2E, 0xC4, 0x36, 0x14, 0x04, 0x8B, 0xC1, 0x8B, 0xCE, 0x8D, 0x74, 0x12, 0xD1, 0xE0, 0xD1, 0xE0, 0x03, 0xF0, 0x1B,
    0x04, 0x03, 0xC1, 0x4B, 0x06, 0x8C, 0x47, 0x08, 0x48, 0x04, 0x02, 0x80, 0xC6, 0x47, 0x1D, 0x0F, 0xC6, 0x47, 0x1A,
    0x0F, 0xC6, 0x47, 0x1E, 0x00, 0x37, 0x02, 0x66, 0x04, 0x04, 0x59, 0x5E, 0x1C, 0x20, 0x3D, 0x04, 0x3F, 0x04, 0x20,
    0x30, 0x20, 0x74, 0x17, 0x1C, 0x10, 0x00, 0x75, 0x11, 0x51, 0x2D, 0x2D, 0x2D, 0x83, 0xE1, 0x0F, 0x88, 0x4F, 0x1D,
    0x88, 0x4F, 0x1A, 0x59, 0x30, 0x80, 0x74, 0x0E, 0x51, 0x2D, 0x2D, 0x2D, 0x83, 0xE1, 0x0F, 0x88, 0x4F, 0x1A, 0x59,
    0x1C, 0x10, 0x3D, 0x1F, 0x30, 0x20, 0x75, 0x12, 0xF7, 0x47, 0x28, 0x00, 0x75, 0x12, 0x3B, 0x77, 0x18, 0x7F, 0x0D,
    0x37, 0x18, 0xEB, 0x08, 0x90, 0x38, 0x4F, 0x1D, 0x74, 0x02, 0x33, 0xF6, 0xD1, 0xE7, 0x65, 0x81, 0xFB, 0x2A, 0x72,
    0x03, 0x8B, 0xC6, 0xC3, 0xE9, 0x21, 0xFF, 0x8E, 0xC2, 0x33, 0xC9, 0xBD, 0x61, 0x64, 0x60, 0x8B, 0xD1, 0xD1, 0xE2,
    0xD1, 0xE2, 0x03, 0xFA, 0x1B, 0x45, 0x08, 0x1B, 0x55, 0x0A, 0x68, 0x75, 0x15, 0xD1, 0xE5, 0x83, 0xC7, 0x04, 0x41,
    0x83, 0xF9, 0x0B, 0x75, 0xE9, 0x33, 0xC0, 0x4B, 0x06, 0x4B, 0x08, 0xEB, 0x11, 0x2E, 0x85, 0x2E, 0x02, 0x04, 0x74,
    0xE4, 0x60, 0x03, 0xF8, 0x89, 0x7F, 0x06, 0x8C, 0x47, 0x08, 0x63, 0x41, 0x65, 0x83, 0xF9, 0x01, 0x72, 0xAF, 0x1B,
    0x44, 0x04, 0x1B, 0x54, 0x06, 0x68, 0x74, 0x1B, 0xFE, 0x03, 0xF8, 0x26, 0x80, 0x7D, 0x04, 0x42, 0x74, 0x09, 0x1B,
    0x05, 0x1B, 0x55, 0x02, 0xEB, 0xE8, 0x83, 0xC7, 0x24, 0x67, 0x0C, 0x62, 0x0E, 0x62, 0x1A, 0x04, 0xC3, 0x8E, 0xC2,
    0x60, 0x8B, 0xF7, 0x2E, 0x67, 0x14, 0x62, 0x16, 0x04, 0x1B, 0x44, 0x04, 0x1B, 0x54, 0x06, 0x68, 0x74, 0x17, 0x60,
    0x03, 0xF8, 0x26, 0x80, 0x7D, 0x04, 0x42, 0x74, 0x09, 0x1B, 0x05, 0x1B, 0x55, 0x02, 0xEB, 0xE8, 0x83, 0xC7, 0x24,
    0x67, 0x10, 0x62, 0x12, 0x04, 0xBB, 0x29, 0x01, 0x63, 0x65, 0x81, 0xFB, 0x2A, 0x72, 0xEB, 0xC3, 0xB8, 0x4D, 0x42,
    0xBA, 0x61, 0xC3, 0x33, 0xC0, 0x8E, 0xC0, 0xBE, 0xC0, 0x03, 0x2E, 0xA1, 0x08, 0x69, 0x89, 0x24, 0xA1, 0x0A, 0x69,
    0x89, 0x44, 0x02, 0x2E, 0x8E, 0x06, 0x2C, 0x00, 0xB4, 0x49, 0x58, 0x8C, 0xC8, 0x8E, 0xC0, 0xB4, 0x49, 0x58, 0xC3,
    0x64, 0x33, 0xC9, 0x63, 0x37, 0x02, 0x66, 0x28, 0xC7, 0x44, 0x4E, 0x4C, 0x27, 0x65, 0x41, 0x83, 0xF9, 0x01, 0x75,
    0xDC, 0xE8, 0x11, 0xFA, 0xC3, 0x64, 0x88, 0x57, 0x17, 0x32, 0xED, 0x89, 0x4F, 0x15, 0x8A, 0x2F, 0x33, 0xED, 0x8B,
    0x57, 0x04, 0x33, 0xD2, 0xE8, 0x9B, 0xF9, 0x4B, 0x23, 0x8B, 0x47, 0x04, 0x25, 0xFF, 0xBF, 0x0B, 0xC1, 0x4B, 0x04,
    0xA9, 0x00, 0x40, 0x74, 0x0D, 0x37, 0x21, 0x83, 0x0C, 0x02, 0x37, 0x1F, 0x66, 0x02, 0x02, 0xC3, 0xC3, 0x64, 0x0A,
    0xC0, 0x75, 0x1E, 0x8B, 0xC2, 0x0B, 0xC6, 0x75, 0x05, 0x8C, 0xCA, 0xBE, 0x34, 0x89, 0x57, 0x27, 0x89, 0x77, 0x25,
    0x88, 0x6F, 0x12, 0x37, 0x1F, 0x66, 0x02, 0x01, 0xC4, 0x77, 0x25, 0xC3, 0xA2, 0x06, 0x3E, 0x0A, 0x7B, 0x04, 0x2B,
    0x09, 0xB9, 0x09, 0x3E, 0x07, 0x3A, 0x08, 0x16, 0x0A, 0x0F, 0x0A, 0x36, 0x07, 0x35, 0x07, 0x6B, 0x0A, 0xA4, 0x0A,
    0xA3, 0x0A, 0xEB, 0x0E, 0x49, 0x46, 0x47, 0x4D, 0x20, 0x42, 0x55, 0x5A, 0x45, 0x52, 0x20, 0x50, 0x43, 0x00, 0x1E,
    0x06, 0x56, 0x57, 0x53, 0x51, 0x55, 0xFC, 0x8C, 0xCB, 0x8E, 0xDB, 0x33, 0xDB, 0x8A, 0xDC, 0x2E, 0xFF, 0x97, 0xCA,
    0x0A, 0x5D, 0x59, 0x5B, 0x5F, 0x5E, 0x07, 0x1F, 0xCF, 0xBC, 0x00, 0x01, 0xBA, 0x87, 0x0B, 0x6A, 0x6A, 0x42, 0xB4,
    0x4A, 0x58, 0x33, 0xC0, 0x8E, 0xC0, 0xBE, 0xC0, 0x03, 0x1B, 0x3C, 0x2E, 0x67, 0x08, 0x69, 0x8E, 0x5C, 0x02, 0x2E,
    0x8C, 0x1E, 0x0A, 0x04, 0x8B, 0xC7, 0x8C, 0xDA, 0x68, 0x74, 0x19, 0x8B, 0x4D, 0x02, 0x8B, 0x55, 0x04, 0x81, 0xF9,
    0x49, 0x46, 0x75, 0x0D, 0x81, 0xFA, 0x47, 0x4D, 0x75, 0x07, 0xB0, 0xFF, 0xB4, 0x4C, 0x58, 0xC3, 0x2E, 0xC7, 0x06,
    0x04, 0x04, 0x2A, 0x2E, 0x8C, 0x0E, 0x06, 0x04, 0xB8, 0xE6, 0x0A, 0x26, 0x89, 0x69, 0x8C, 0x4C, 0x02, 0x32, 0xC0,
    0xBA, 0x13, 0x0B, 0x6A, 0x6A, 0x42, 0xB4, 0x31, 0x58 };

const uint8_t uncompressed_example[] = {
    0xE9, 0x10, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x01, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x01, 0x16, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
    0x01, 0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0xFD, 0x04, 0xFD, 0x04, 0xA9, 0x05, 0xFD, 0x04, 0x58, 0x05, 0xA7, 0x01, 0xD7, 0x01, 0x07, 0x02, 0x37, 0x02, 0x67,
    0x02, 0x97, 0x02, 0xC7, 0x02, 0xF7, 0x02, 0x27, 0x03, 0x57, 0x03, 0x87, 0x03, 0xB7, 0x03, 0x3A, 0x02, 0x38, 0x02,
    0x37, 0x02, 0x35, 0x02, 0x34, 0x02, 0x33, 0x02, 0x31, 0x02, 0x30, 0x02, 0x2F, 0x02, 0x2D, 0x02, 0x2C, 0x02, 0x2B,
    0x02, 0x29, 0x02, 0x28, 0x02, 0x27, 0x02, 0x25, 0x02, 0x24, 0x02, 0x23, 0x02, 0x21, 0x02, 0x20, 0x02, 0x1F, 0x02,
    0x1D, 0x02, 0x1C, 0x02, 0x1B, 0x02, 0x1A, 0x02, 0x18, 0x02, 0x17, 0x02, 0x16, 0x02, 0x14, 0x02, 0x13, 0x02, 0x12,
    0x02, 0x11, 0x02, 0x0F, 0x02, 0x0E, 0x02, 0x0D, 0x02, 0x0B, 0x02, 0x0A, 0x02, 0x09, 0x02, 0x08, 0x02, 0x06, 0x02,
    0x05, 0x02, 0x04, 0x02, 0x03, 0x02, 0x02, 0x02, 0x00, 0x02, 0xFF, 0x01, 0xFE, 0x01, 0xFD, 0x01, 0xFB, 0x01, 0xFA,
    0x01, 0xF9, 0x01, 0xF8, 0x01, 0xF6, 0x01, 0xF5, 0x01, 0xF4, 0x01, 0xF3, 0x01, 0xF2, 0x01, 0xF0, 0x01, 0xEF, 0x01,
    0xEE, 0x01, 0xED, 0x01, 0xEC, 0x01, 0xEA, 0x01, 0xE9, 0x01, 0xE8, 0x01, 0xE7, 0x01, 0xE6, 0x01, 0xE4, 0x01, 0xE3,
    0x01, 0xE2, 0x01, 0xE1, 0x01, 0xE0, 0x01, 0xDF, 0x01, 0xDE, 0x01, 0xDC, 0x01, 0xDB, 0x01, 0xDA, 0x01, 0xD9, 0x01,
    0xD8, 0x01, 0xD7, 0x01, 0xD6, 0x01, 0xD4, 0x01, 0xD3, 0x01, 0xD2, 0x01, 0xD1, 0x01, 0xD0, 0x01, 0xCF, 0x01, 0xCE,
    0x01, 0xCD, 0x01, 0xCC, 0x01, 0xCA, 0x01, 0xC9, 0x01, 0xC8, 0x01, 0xC7, 0x01, 0xC6, 0x01, 0xC5, 0x01, 0xC4, 0x01,
    0xC3, 0x01, 0xC2, 0x01, 0xC1, 0x01, 0xC0, 0x01, 0xBE, 0x01, 0xBD, 0x01, 0xBC, 0x01, 0xBB, 0x01, 0xBA, 0x01, 0xB9,
    0x01, 0xB8, 0x01, 0xB7, 0x01, 0xB6, 0x01, 0xB5, 0x01, 0xB4, 0x01, 0xB3, 0x01, 0xB2, 0x01, 0xB1, 0x01, 0xB0, 0x01,
    0xAF, 0x01, 0xAE, 0x01, 0xAD, 0x01, 0xAC, 0x01, 0xAB, 0x01, 0xA9, 0x01, 0xA8, 0x01, 0xA7, 0x01, 0xA6, 0x01, 0xA5,
    0x01, 0xA4, 0x01, 0xA3, 0x01, 0xA2, 0x01, 0xA1, 0x01, 0xA0, 0x01, 0x9F, 0x01, 0x9E, 0x01, 0x9D, 0x01, 0x9C, 0x01,
    0x9B, 0x01, 0x9A, 0x01, 0x99, 0x01, 0x98, 0x01, 0x97, 0x01, 0x96, 0x01, 0x96, 0x01, 0x95, 0x01, 0x94, 0x01, 0x93,
    0x01, 0x92, 0x01, 0x91, 0x01, 0x90, 0x01, 0x8F, 0x01, 0x8E, 0x01, 0x8D, 0x01, 0x8C, 0x01, 0x8B, 0x01, 0x8A, 0x01,
    0x89, 0x01, 0x88, 0x01, 0x87, 0x01, 0x86, 0x01, 0x85, 0x01, 0x84, 0x01, 0x83, 0x01, 0x82, 0x01, 0x81, 0x01, 0x80,
    0x01, 0x80, 0x01, 0x7F, 0x01, 0x7E, 0x01, 0x7D, 0x01, 0x7C, 0x01, 0x7B, 0x01, 0x7A, 0x01, 0x79, 0x01, 0x78, 0x01,
    0x77, 0x01, 0x76, 0x01, 0x75, 0x01, 0x75, 0x01, 0x74, 0x01, 0x73, 0x01, 0x72, 0x01, 0x71, 0x01, 0x70, 0x01, 0x6F,
    0x01, 0x6E, 0x01, 0x6D, 0x01, 0x6D, 0x01, 0x6C, 0x01, 0x6B, 0x01, 0x6A, 0x01, 0x69, 0x01, 0x68, 0x01, 0x67, 0x01,
    0x67, 0x01, 0x66, 0x01, 0x65, 0x01, 0x64, 0x01, 0x63, 0x01, 0x62, 0x01, 0x61, 0x01, 0x60, 0x01, 0x60, 0x01, 0x5F,
    0x01, 0x5E, 0x01, 0x5D, 0x01, 0x5C, 0x01, 0x5B, 0x01, 0x5B, 0x01, 0x5A, 0x01, 0x59, 0x01, 0x58, 0x01, 0x57, 0x01,
    0x56, 0x01, 0x56, 0x01, 0x55, 0x01, 0x54, 0x01, 0x53, 0x01, 0x52, 0x01, 0x52, 0x01, 0x51, 0x01, 0x50, 0x01, 0x4F,
    0x01, 0x4E, 0x01, 0x4D, 0x01, 0x4D, 0x01, 0x4C, 0x01, 0x4B, 0x01, 0x4A, 0x01, 0x49, 0x01, 0x49, 0x01, 0x48, 0x01,
    0x47, 0x01, 0x46, 0x01, 0x46, 0x01, 0x45, 0x01, 0x44, 0x01, 0x43, 0x01, 0x42, 0x01, 0x42, 0x01, 0x41, 0x01, 0x40,
    0x01, 0x3F, 0x01, 0x3F, 0x01, 0x3E, 0x01, 0x3D, 0x01, 0x3C, 0x01, 0x3B, 0x01, 0x3B, 0x01, 0x3A, 0x01, 0x39, 0x01,
    0x38, 0x01, 0x38, 0x01, 0x37, 0x01, 0x36, 0x01, 0x35, 0x01, 0x35, 0x01, 0x34, 0x01, 0x33, 0x01, 0x32, 0x01, 0x32,
    0x01, 0x31, 0x01, 0x30, 0x01, 0x30, 0x01, 0x2F, 0x01, 0x2E, 0x01, 0x2D, 0x01, 0x2D, 0x01, 0x2C, 0x01, 0x2B, 0x01,
    0x2B, 0x01, 0x2A, 0x01, 0x29, 0x01, 0x28, 0x01, 0x28, 0x01, 0x27, 0x01, 0x26, 0x01, 0x25, 0x01, 0x25, 0x01, 0x24,
    0x01, 0x23, 0x01, 0x23, 0x01, 0x22, 0x01, 0x21, 0x01, 0x21, 0x01, 0x20, 0x01, 0x1F, 0x01, 0x1F, 0x01, 0x1E, 0x01,
    0x1D, 0x01, 0x80, 0xA0, 0xC0, 0x1D, 0x06, 0x78, 0x06, 0x47, 0x06, 0x8D, 0x06, 0x8C, 0x06, 0x43, 0x06, 0x0C, 0x06,
    0xF7, 0x05, 0xF7, 0x05, 0xF7, 0x05, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB, 0x57,
    0x53, 0x33, 0xC0, 0x8A, 0xC1, 0x24, 0x0F, 0xD1, 0xE0, 0x8B, 0xF8, 0x2E, 0x8B, 0xBD, 0x8F, 0x01, 0xF7, 0xC5, 0x80,
    0x00, 0x74, 0x11, 0xF6, 0xC1, 0x0F, 0x75, 0x0C, 0xBF, 0xE7, 0x03, 0x8A, 0xC1, 0xA8, 0x70, 0x74, 0x03, 0x80, 0xE9,
    0x10, 0xF6, 0xC1, 0x80, 0x74, 0x03, 0xBA, 0x40, 0x00, 0x52, 0x8B, 0xC5, 0x98, 0xD1, 0xE0, 0x03, 0xF8, 0x8B, 0x05,
    0x80, 0xE1, 0x70, 0xD0, 0xE9, 0xD0, 0xE9, 0xD0, 0xE9, 0xD0, 0xE9, 0x80, 0xE9, 0x07, 0xF6, 0xD9, 0xD3, 0xE0, 0x59,
    0xF7, 0xC1, 0x40, 0x00, 0x75, 0x06, 0xB9, 0x00, 0x40, 0x5B, 0x5F, 0xC3, 0x33, 0xC9, 0x5B, 0x5F, 0xC3, 0xB0, 0xB6,
    0xE6, 0x43, 0xE4, 0x61, 0x24, 0xFC, 0xE6, 0x61, 0xC3, 0xF7, 0x05, 0x02, 0x00, 0x74, 0x14, 0x26, 0x8A, 0x64, 0x01,
    0xF6, 0xC4, 0x80, 0x75, 0x01, 0xC3, 0x32, 0xC0, 0x89, 0x05, 0x26, 0x8A, 0x04, 0x88, 0x45, 0x02, 0xF7, 0x05, 0x04,
    0x00, 0x75, 0x27, 0x8A, 0x45, 0x02, 0x0A, 0xC0, 0x74, 0x06, 0xFE, 0xC8, 0x88, 0x45, 0x02, 0xC3, 0xF7, 0x05, 0x00,
    0x40, 0x74, 0x08, 0xF7, 0x47, 0x04, 0x00, 0x40, 0x74, 0x01, 0xC3, 0x83, 0x0D, 0x04, 0xF7, 0x05, 0x00, 0x20, 0x74,
    0x03, 0x83, 0x0D, 0x02, 0x83, 0xE2, 0xFC, 0xC3, 0xF7, 0x45, 0x02, 0x02, 0x00, 0x74, 0x18, 0x26, 0x8A, 0x64, 0x0C,
    0xF6, 0xC4, 0x80, 0x75, 0x01, 0xC3, 0x32, 0xC0, 0xC7, 0x45, 0x04, 0x00, 0x00, 0xC7, 0x05, 0x00, 0x00, 0x89, 0x45,
    0x02, 0x56, 0x8B, 0x2D, 0x2E, 0x03, 0xB6, 0x7B, 0x01, 0x2E, 0xFF, 0xA6, 0x85, 0x01, 0xF7, 0x45, 0x02, 0x01, 0x00,
    0x75, 0x20, 0x26, 0x8A, 0x04, 0xA8, 0x40, 0x74, 0x09, 0xC7, 0x05, 0x08, 0x00, 0x03, 0x55, 0x04, 0x5E, 0xC3, 0x24,
    0x3F, 0x88, 0x45, 0x06, 0x26, 0x8A, 0x44, 0x02, 0x88, 0x45, 0x07, 0x83, 0x4D, 0x02, 0x05, 0x8A, 0x45, 0x07, 0x0A,
    0xC0, 0x74, 0x0A, 0xFE, 0xC8, 0x88, 0x45, 0x07, 0x03, 0x55, 0x04, 0x5E, 0xC3, 0x8A, 0x45, 0x06, 0x0A, 0xC0, 0x74,
    0x2C, 0xFE, 0xC8, 0x88, 0x45, 0x06, 0x26, 0x8A, 0x44, 0x02, 0x88, 0x45, 0x07, 0x26, 0x8A, 0x44, 0x01, 0x98, 0x03,
    0x45, 0x04, 0x89, 0x45, 0x04, 0x03, 0x55, 0x04, 0x5E, 0xC3, 0xF7, 0x45, 0x02, 0x00, 0x20, 0x74, 0x04, 0x83, 0x4D,
    0x02, 0x02, 0x03, 0x55, 0x04, 0x5E, 0xC3, 0xF7, 0x45, 0x02, 0x04, 0x00, 0x74, 0x2B, 0x83, 0x65, 0x02, 0xFB, 0x26,
    0xF6, 0x04, 0x80, 0x74, 0x0D, 0x26, 0x8A, 0x44, 0x03, 0x98, 0x03, 0x45, 0x04, 0x89, 0x45, 0x04, 0xEB, 0x9E, 0x26,
    0x8A, 0x44, 0x01, 0x98, 0x03, 0x45, 0x04, 0x89, 0x45, 0x04, 0x26, 0x8A, 0x44, 0x03, 0x88, 0x45, 0x07, 0xEB, 0x8A,
    0x83, 0x65, 0x02, 0xFE, 0x8B, 0x05, 0x05, 0x02, 0x00, 0x89, 0x05, 0x5E, 0xE9, 0x47, 0xFF, 0xF7, 0x45, 0x02, 0x00,
    0x40, 0x74, 0xEA, 0xF7, 0x47, 0x04, 0x00, 0x40, 0x74, 0xE3, 0x03, 0x55, 0x04, 0x5E, 0xC3, 0x56, 0x57, 0x8B, 0x47,
    0x04, 0xA9, 0x40, 0x00, 0x75, 0x2E, 0xA9, 0x02, 0x00, 0x75, 0x2D, 0xFF, 0x4F, 0x0E, 0x75, 0x24, 0x8B, 0x47, 0x10,
    0x89, 0x47, 0x0E, 0xC4, 0x77, 0x0A, 0x26, 0xAD, 0x89, 0x77, 0x0A, 0x8C, 0x47, 0x0C, 0x8A, 0xCC, 0x32, 0xED, 0x32,
    0xE4, 0xD0, 0xE0, 0x8B, 0xF8, 0x9C, 0x2E, 0xFF, 0x95, 0xEA, 0x03, 0x9D, 0x73, 0xE2, 0x5F, 0x5E, 0xC3, 0xC3, 0xC4,
    0x47, 0x06, 0x89, 0x47, 0x0A, 0x8C, 0x47, 0x0C, 0x83, 0x67, 0x04, 0xFD, 0xC7, 0x47, 0x18, 0x00, 0x00, 0xEB, 0xCA,
    0x53, 0x1E, 0x51, 0x8B, 0x07, 0x50, 0x2E, 0xFF, 0x1E, 0xFE, 0x03, 0x83, 0xC4, 0x04, 0x1F, 0x5B, 0xC3, 0x83, 0x4F,
    0x04, 0x02, 0xF7, 0x47, 0x04, 0x20, 0x00, 0x75, 0x1A, 0xC7, 0x47, 0x25, 0x19, 0x01, 0x8C, 0x4F, 0x27, 0x83, 0x4F,
    0x04, 0x40, 0xF7, 0x47, 0x04, 0x00, 0x80, 0x74, 0x07, 0x8B, 0x77, 0x02, 0x83, 0x64, 0x04, 0xFB, 0xC3, 0x88, 0x4F,
    0x17, 0xC3, 0xFF, 0x47, 0x18, 0x89, 0x4F, 0x15, 0x8A, 0x2F, 0x33, 0xED, 0x8B, 0x57, 0x04, 0xE8, 0xC6, 0xFD, 0x89,
    0x47, 0x23, 0x8B, 0x47, 0x04, 0x25, 0xFF, 0xBF, 0x0B, 0xC1, 0x89, 0x47, 0x04, 0xA9, 0x00, 0x40, 0x74, 0x0D, 0x8B,
    0x77, 0x1F, 0x83, 0x4C, 0x02, 0x02, 0x8B, 0x77, 0x21, 0x83, 0x0C, 0x02, 0xC3, 0x26, 0xAC, 0x8A, 0xE8, 0x03, 0x4F,
    0x13, 0x89, 0x4F, 0x10, 0x89, 0x4F, 0x0E, 0x89, 0x77, 0x0A, 0x8C, 0x47, 0x0C, 0xC3, 0xC3, 0x88, 0x4F, 0x12, 0xB0,
    0x10, 0xF6, 0xE1, 0x2E, 0xC4, 0x36, 0x18, 0x04, 0x03, 0xF0, 0x89, 0x77, 0x25, 0x8C, 0x47, 0x27, 0xC3, 0x50, 0x52,
    0x06, 0x1E, 0x56, 0x57, 0x55, 0x2E, 0xF6, 0x06, 0x00, 0x04, 0xFF, 0x74, 0x08, 0x5D, 0x5F, 0x5E, 0x1F, 0x07, 0x5A,
    0x58, 0xC3, 0xBB, 0x52, 0x01, 0x8B, 0xF3, 0x2E, 0xA1, 0x0C, 0x04, 0x2E, 0xA3, 0x18, 0x04, 0x2E, 0xA1, 0x0E, 0x04,
    0x2E, 0xA3, 0x1A, 0x04, 0xE8, 0xEB, 0xFE, 0xF7, 0x47, 0x04, 0x04, 0x00, 0x74, 0x1A, 0x2E, 0xA1, 0x10, 0x04, 0x2E,
    0xA3, 0x18, 0x04, 0x2E, 0xA1, 0x12, 0x04, 0x2E, 0xA3, 0x1A, 0x04, 0x53, 0x8B, 0x5F, 0x02, 0x8B, 0xF3, 0xE8, 0xCB,
    0xFE, 0x5B, 0x8B, 0xDE, 0xC4, 0x77, 0x25, 0x46, 0x8B, 0x57, 0x23, 0x26, 0xF6, 0x44, 0x0C, 0x80, 0x74, 0x06, 0x8B,
    0x7F, 0x1F, 0xE8, 0xC9, 0xFD, 0x8A, 0xC2, 0xE6, 0x42, 0x8A, 0xC6, 0xE6, 0x42, 0x83, 0xC6, 0x0D, 0x33, 0xD2, 0x26,
    0xF6, 0x44, 0x01, 0x80, 0x74, 0x09, 0xBA, 0x03, 0x00, 0x8B, 0x7F, 0x21, 0xE8, 0x61, 0xFD, 0xE4, 0x61, 0x24, 0xFC,
    0x0A, 0xC2, 0xE6, 0x61, 0x5D, 0x5F, 0x5E, 0x1F, 0x07, 0x5A, 0x58, 0xC3, 0xC3, 0x8C, 0xC8, 0x8B, 0xD0, 0xB8, 0xFE,
    0x03, 0xC3, 0x8B, 0xEE, 0xBE, 0xFF, 0xFF, 0x0B, 0xED, 0x75, 0x03, 0xBD, 0x01, 0x00, 0xBB, 0x52, 0x01, 0xBF, 0x01,
    0x00, 0x85, 0xEF, 0x74, 0x08, 0x8B, 0x47, 0x06, 0x0B, 0x47, 0x08, 0x75, 0x03, 0xE9, 0xC1, 0x00, 0xF7, 0xC2, 0x00,
    0x01, 0x74, 0x1F, 0x0B, 0xC9, 0x74, 0x09, 0x2E, 0xC6, 0x06, 0x00, 0x04, 0x00, 0xEB, 0x0A, 0x90, 0x2E, 0xC6, 0x06,
    0x00, 0x04, 0x01, 0xE8, 0x00, 0xFD, 0xF7, 0xC2, 0x00, 0x04, 0x74, 0x03, 0x89, 0x4F, 0x13, 0xF7, 0xC2, 0x40, 0x00,
    0x74, 0x13, 0xF7, 0x47, 0x04, 0x40, 0x00, 0x75, 0x0C, 0x83, 0x4F, 0x04, 0x40, 0xC7, 0x47, 0x25, 0x19, 0x01, 0x8C,
    0x4F, 0x27, 0xF7, 0xC2, 0x80, 0x00, 0x74, 0x11, 0xC6, 0x47, 0x1D, 0x0F, 0xC6, 0x47, 0x1A, 0x0F, 0xC6, 0x47, 0x1E,
    0x00, 0xC7, 0x47, 0x04, 0x02, 0x00, 0xF7, 0xC2, 0x20, 0x00, 0x74, 0x04, 0x83, 0x4F, 0x04, 0x20, 0xF7, 0xC2, 0x00,
    0x20, 0x74, 0x17, 0xF7, 0xC2, 0x10, 0x00, 0x75, 0x11, 0x51, 0xD0, 0xE9, 0xD0, 0xE9, 0xD0, 0xE9, 0x83, 0xE1, 0x0F,
    0x88, 0x4F, 0x1D, 0x88, 0x4F, 0x1A, 0x59, 0xF7, 0xC2, 0x00, 0x80, 0x74, 0x0E, 0x51, 0xD0, 0xE9, 0xD0, 0xE9, 0xD0,
    0xE9, 0x83, 0xE1, 0x0F, 0x88, 0x4F, 0x1A, 0x59, 0xF7, 0xC2, 0x00, 0x10, 0x74, 0x06, 0x88, 0x4F, 0x1B, 0x88, 0x4F,
    0x1C, 0xF7, 0xC2, 0x10, 0x00, 0x74, 0x1F, 0xF7, 0xC2, 0x00, 0x20, 0x75, 0x12, 0xF7, 0x47, 0x04, 0x40, 0x00, 0x75,
    0x12, 0x3B, 0x77, 0x18, 0x7F, 0x0D, 0x8B, 0x77, 0x18, 0xEB, 0x08, 0x90, 0x38, 0x4F, 0x1D, 0x74, 0x02, 0x33, 0xF6,
    0xD1, 0xE7, 0x83, 0xC3, 0x29, 0x81, 0xFB, 0x7B, 0x01, 0x72, 0x0C, 0x8B, 0xC6, 0xF7, 0xC2, 0x00, 0x02, 0x74, 0x03,
    0xE8, 0x00, 0xFF, 0xC3, 0xE9, 0x16, 0xFF, 0x8B, 0xEE, 0xBE, 0xFF, 0xFF, 0x0B, 0xED, 0x75, 0x03, 0xBD, 0x01, 0x00,
    0xBB, 0x29, 0x01, 0xBF, 0x01, 0x00, 0x85, 0xEF, 0x75, 0x03, 0xE9, 0xC7, 0x00, 0xF7, 0xC2, 0x00, 0x04, 0x74, 0x03,
    0x89, 0x4F, 0x13, 0xF7, 0xC2, 0x40, 0x00, 0x74, 0x0C, 0x83, 0x4F, 0x04, 0x40, 0xC7, 0x47, 0x25, 0x19, 0x01, 0x8C,
    0x4F, 0x27, 0xF7, 0xC2, 0x80, 0x00, 0x74, 0x46, 0xF7, 0xC2, 0x00, 0x40, 0x74, 0x07, 0xF7, 0x47, 0x04, 0x40, 0x00,
    0x74, 0x39, 0x56, 0x51, 0x2E, 0xC4, 0x36, 0x14, 0x04, 0x8B, 0xC1, 0x8B, 0xCE, 0x8D, 0x74, 0x12, 0xD1, 0xE0, 0xD1,
    0xE0, 0x03, 0xF0, 0x26, 0x8B, 0x04, 0x03, 0xC1, 0x89, 0x47, 0x06, 0x8C, 0x47, 0x08, 0xC7, 0x47, 0x04, 0x02, 0x80,
    0xC6, 0x47, 0x1D, 0x0F, 0xC6, 0x47, 0x1A, 0x0F, 0xC6, 0x47, 0x1E, 0x00, 0x8B, 0x77, 0x02, 0x83, 0x4C, 0x04, 0x04,
    0x59, 0x5E, 0xF7, 0xC2, 0x20, 0x00, 0x74, 0x04, 0x83, 0x4F, 0x04, 0x20, 0xF7, 0xC2, 0x00, 0x20, 0x74, 0x17, 0xF7,
    0xC2, 0x10, 0x00, 0x75, 0x11, 0x51, 0xD0, 0xE9, 0xD0, 0xE9, 0xD0, 0xE9, 0x83, 0xE1, 0x0F, 0x88, 0x4F, 0x1D, 0x88,
    0x4F, 0x1A, 0x59, 0xF7, 0xC2, 0x00, 0x80, 0x74, 0x0E, 0x51, 0xD0, 0xE9, 0xD0, 0xE9, 0xD0, 0xE9, 0x83, 0xE1, 0x0F,
    0x88, 0x4F, 0x1A, 0x59, 0xF7, 0xC2, 0x10, 0x00, 0x74, 0x1F, 0xF7, 0xC2, 0x00, 0x20, 0x75, 0x12, 0xF7, 0x47, 0x04,
    0x40, 0x00, 0x75, 0x12, 0x3B, 0x77, 0x18, 0x7F, 0x0D, 0x8B, 0x77, 0x18, 0xEB, 0x08, 0x90, 0x38, 0x4F, 0x1D, 0x74,
    0x02, 0x33, 0xF6, 0xD1, 0xE7, 0x83, 0xC3, 0x29, 0x81, 0xFB, 0x52, 0x01, 0x72, 0x03, 0x8B, 0xC6, 0xC3, 0xE9, 0x21,
    0xFF, 0x8E, 0xC2, 0x33, 0xC9, 0xBD, 0x01, 0x00, 0xBB, 0x52, 0x01, 0x8B, 0xFE, 0x8B, 0xD1, 0xD1, 0xE2, 0xD1, 0xE2,
    0x03, 0xFA, 0x26, 0x8B, 0x45, 0x08, 0x26, 0x8B, 0x55, 0x0A, 0x0B, 0xD0, 0x75, 0x15, 0xD1, 0xE5, 0x83, 0xC7, 0x04,
    0x41, 0x83, 0xF9, 0x0B, 0x75, 0xE9, 0x33, 0xC0, 0x89, 0x47, 0x06, 0x89, 0x47, 0x08, 0xEB, 0x11, 0x2E, 0x85, 0x2E,
    0x02, 0x04, 0x74, 0xE4, 0x8B, 0xFE, 0x03, 0xF8, 0x89, 0x7F, 0x06, 0x8C, 0x47, 0x08, 0x83, 0x4F, 0x04, 0x40, 0xC7,
    0x47, 0x25, 0x19, 0x01, 0x8C, 0x4F, 0x27, 0x41, 0x83, 0xC3, 0x29, 0x83, 0xF9, 0x01, 0x72, 0xAF, 0x26, 0x8B, 0x44,
    0x04, 0x26, 0x8B, 0x54, 0x06, 0x0B, 0xD0, 0x74, 0x26, 0x8B, 0xFE, 0x03, 0xF8, 0x26, 0x80, 0x7D, 0x04, 0x42, 0x74,
    0x09, 0x26, 0x8B, 0x05, 0x26, 0x8B, 0x55, 0x02, 0xEB, 0xE8, 0x83, 0xC7, 0x04, 0x2E, 0x89, 0x3E, 0x0C, 0x04, 0x2E,
    0x8C, 0x06, 0x0E, 0x04, 0x2E, 0x8C, 0x06, 0x1A, 0x04, 0xC3, 0x8E, 0xC2, 0x8B, 0xFE, 0x8B, 0xF7, 0x2E, 0x89, 0x3E,
    0x14, 0x04, 0x2E, 0x8C, 0x06, 0x16, 0x04, 0x26, 0x8B, 0x44, 0x04, 0x26, 0x8B, 0x54, 0x06, 0x0B, 0xD0, 0x74, 0x17,
    0x8B, 0xFE, 0x03, 0xF8, 0x26, 0x80, 0x7D, 0x04, 0x42, 0x74, 0x09, 0x26, 0x8B, 0x05, 0x26, 0x8B, 0x55, 0x02, 0xEB,
    0xE8, 0x83, 0xC7, 0x04, 0x2E, 0x89, 0x3E, 0x10, 0x04, 0x2E, 0x8C, 0x06, 0x12, 0x04, 0xBB, 0x29, 0x01, 0x83, 0x4F,
    0x04, 0x40, 0xC7, 0x47, 0x25, 0x19, 0x01, 0x8C, 0x4F, 0x27, 0x83, 0xC3, 0x29, 0x81, 0xFB, 0x52, 0x01, 0x72, 0xEB,
    0xC3, 0xB8, 0x4D, 0x42, 0xBA, 0x01, 0x00, 0xC3, 0x33, 0xC0, 0x8E, 0xC0, 0xBE, 0xC0, 0x03, 0x2E, 0xA1, 0x08, 0x04,
    0x26, 0x89, 0x04, 0x2E, 0xA1, 0x0A, 0x04, 0x26, 0x89, 0x44, 0x02, 0x2E, 0x8E, 0x06, 0x2C, 0x00, 0xB4, 0x49, 0xCD,
    0x21, 0x8C, 0xC8, 0x8E, 0xC0, 0xB4, 0x49, 0xCD, 0x21, 0xC3, 0xBB, 0x52, 0x01, 0x33, 0xC9, 0x83, 0x4F, 0x04, 0x40,
    0xC7, 0x47, 0x25, 0x19, 0x01, 0x8C, 0x4F, 0x27, 0x8B, 0x77, 0x02, 0x83, 0x4C, 0x04, 0x40, 0xC7, 0x44, 0x25, 0x19,
    0x01, 0x8C, 0x4C, 0x27, 0x83, 0xC3, 0x29, 0x41, 0x83, 0xF9, 0x01, 0x75, 0xDC, 0xE8, 0x11, 0xFA, 0xC3, 0xBB, 0x52,
    0x01, 0x88, 0x57, 0x17, 0x32, 0xED, 0x89, 0x4F, 0x15, 0x8A, 0x2F, 0x33, 0xED, 0x8B, 0x57, 0x04, 0x33, 0xD2, 0xE8,
    0x9B, 0xF9, 0x89, 0x47, 0x23, 0x8B, 0x47, 0x04, 0x25, 0xFF, 0xBF, 0x0B, 0xC1, 0x89, 0x47, 0x04, 0xA9, 0x00, 0x40,
    0x74, 0x0D, 0x8B, 0x77, 0x21, 0x83, 0x0C, 0x02, 0x8B, 0x77, 0x1F, 0x83, 0x4C, 0x02, 0x02, 0xC3, 0xC3, 0xBB, 0x52,
    0x01, 0x0A, 0xC0, 0x75, 0x1E, 0x8B, 0xC2, 0x0B, 0xC6, 0x75, 0x05, 0x8C, 0xCA, 0xBE, 0x19, 0x01, 0x89, 0x57, 0x27,
    0x89, 0x77, 0x25, 0x88, 0x6F, 0x12, 0x8B, 0x77, 0x1F, 0x83, 0x4C, 0x02, 0x01, 0xC4, 0x77, 0x25, 0xC3, 0xA2, 0x06,
    0x3E, 0x0A, 0x7B, 0x04, 0x2B, 0x09, 0xB9, 0x09, 0x3E, 0x07, 0x3A, 0x08, 0x16, 0x0A, 0x0F, 0x0A, 0x36, 0x07, 0x35,
    0x07, 0x6B, 0x0A, 0xA4, 0x0A, 0xA3, 0x0A, 0xEB, 0x0E, 0x49, 0x46, 0x47, 0x4D, 0x20, 0x42, 0x55, 0x5A, 0x45, 0x52,
    0x20, 0x50, 0x43, 0x00, 0x1E, 0x06, 0x56, 0x57, 0x53, 0x51, 0x55, 0xFC, 0x8C, 0xCB, 0x8E, 0xDB, 0x33, 0xDB, 0x8A,
    0xDC, 0x2E, 0xFF, 0x97, 0xCA, 0x0A, 0x5D, 0x59, 0x5B, 0x5F, 0x5E, 0x07, 0x1F, 0xCF, 0xBC, 0x00, 0x01, 0xBA, 0x87,
    0x0B, 0xD1, 0xEA, 0xD1, 0xEA, 0xD1, 0xEA, 0xD1, 0xEA, 0x42, 0xB4, 0x4A, 0xCD, 0x21, 0x33, 0xC0, 0x8E, 0xC0, 0xBE,
    0xC0, 0x03, 0x26, 0x8B, 0x3C, 0x2E, 0x89, 0x3E, 0x08, 0x04, 0x26, 0x8E, 0x5C, 0x02, 0x2E, 0x8C, 0x1E, 0x0A, 0x04,
    0x8B, 0xC7, 0x8C, 0xDA, 0x0B, 0xD0, 0x74, 0x19, 0x8B, 0x4D, 0x02, 0x8B, 0x55, 0x04, 0x81, 0xF9, 0x49, 0x46, 0x75,
    0x0D, 0x81, 0xFA, 0x47, 0x4D, 0x75, 0x07, 0xB0, 0xFF, 0xB4, 0x4C, 0xCD, 0x21, 0xC3, 0x2E, 0xC7, 0x06, 0x04, 0x04,
    0x52, 0x01, 0x2E, 0x8C, 0x0E, 0x06, 0x04, 0xB8, 0xE6, 0x0A, 0x26, 0x89, 0x04, 0x26, 0x8C, 0x4C, 0x02, 0x32, 0xC0,
    0xBA, 0x13, 0x0B, 0xD1, 0xEA, 0xD1, 0xEA, 0xD1, 0xEA, 0xD1, 0xEA, 0x42, 0xB4, 0x31, 0xCD, 0x21 };

#pragma pack( push, 1 )
struct pack_block_t
{
    uint8_t packed_size{};
    uint8_t flag{};      // 1 = more data to uncompress, 0 = end
    uint16_t data_len{}; // (un)compressed data
                         /*
	if packed_size != 0
		table0[ 1 + pack_block.packed_size ]
		table1[ 1 + pack_block.packed_size ]
		table2[ 1 + pack_block.packed_size ]
	data[ pack_block.data_len ]
	*/
};
static_assert( sizeof( pack_block_t ) == 4, "wrong size" );
#pragma pack( pop )

struct tables_t
{
    std::vector<uint8_t> table1;
    std::vector<uint8_t> table2;
    std::vector<uint8_t> data;

    //prepared
    std::array<uint8_t, 256> table3{}; // needs to be 0 filled
    std::vector<uint8_t> table4;
};

tables_t prepare_tables( const uint8_t*& compressed, const uint8_t packed_size, const uint16_t data_len_ )
{
    tables_t tables;
    // from uncompressed data right after packed_block
    std::vector<uint8_t> table0( 1 + packed_size ); // only for initialization
    tables.table1.resize( 1 + packed_size );
    tables.table2.resize( 1 + packed_size );
    tables.data.resize( data_len_ );

    ::memcpy( &table0[1], compressed, packed_size );
    compressed += packed_size;

    ::memcpy( &tables.table1[1], compressed, packed_size );
    compressed += packed_size;

    ::memcpy( &tables.table2[1], compressed, packed_size );
    compressed += packed_size;

    ::memcpy( tables.data.data(), compressed, data_len_ );
    compressed += data_len_;

    // runtime filled
    tables.table4.resize( 1 + packed_size );

    for( uint16_t i = 0; i < packed_size; ++i )
    {
        const uint8_t ofs_0 = i + 1;
        const uint8_t ofs_1 = table0[ofs_0];
        tables.table4[ofs_0] = tables.table3[ofs_1];
        tables.table3[ofs_1] = ofs_0;
    }

    return tables;
}

void val_3_non_0( uint8_t*& uncompressed_, const tables_t& tables_, const uint8_t val_3_ )
{
    struct stack_vals_t
    {
        uint8_t val_0{};
        uint8_t val_1{};
    };

    std::stack<stack_vals_t> stack;

    auto helper1 = [&stack, &tables_]( const uint8_t val_7_ ) {
        stack.push( { val_7_, tables_.table2[val_7_] } );
        return tables_.table1[val_7_];
    };

    auto helper2 = [&stack]( uint8_t* val_7_, uint8_t* val_4_ ) {
        if( stack.empty() )
        {
            return true;
        }

        const stack_vals_t stack_val = stack.top();
        stack.pop();
        *val_7_ = stack_val.val_0;
        *val_4_ = stack_val.val_1;

        return false;
    };

    uint8_t val_7 = val_3_;
    uint8_t val_4 = helper1( val_7 );

    while( true )
    {
        const uint8_t val_5 = val_4;
        const uint8_t val_6 = tables_.table3[val_5];

        if( val_6 == 0 )
        {
            *uncompressed_++ = val_4;
            if( helper2( &val_7, &val_4 ) )
            {
                return;
            }
        }
        else if( val_7 > val_6 )
        {
            val_7 = val_6;
            val_4 = helper1( val_7 );
        }
        else
        {
            val_4 = val_7;
            val_7 = val_6;

            assert( stack.size() >= 0 );
            while( true )
            {
                val_7 = tables_.table4[val_7];

                if( val_7 == 0 )
                {
                    *uncompressed_++ = val_5;
                    if( helper2( &val_7, &val_4 ) )
                    {
                        return;
                    }
                    break;
                }
                else if( val_7 < val_4 )
                {
                    val_4 = helper1( val_7 );
                    break;
                }

                // another run
            }
        }
    }
}

void emu_GAME_START_sub_3( std::vector<uint8_t>& uncompressed_buffer_, const std::vector<uint8_t>& compressed_buffer_ )
{
    uint8_t* uncompressed = uncompressed_buffer_.data();
    const uint8_t* compressed = compressed_buffer_.data();

    while( true )
    {
        const pack_block_t pack_block = *reinterpret_cast<const pack_block_t*>( compressed );
        compressed += sizeof( pack_block );
        assert( pack_block.flag == 0 || pack_block.flag == 1 );

#if 1
        printf( "pack_block: packed_size: 0x%02X, flag: 0x%02X, data_len: 0x%04X\n", pack_block.packed_size,
                pack_block.flag, pack_block.data_len );
#endif

        if( pack_block.packed_size == 0 )
        {
            // uncompressed (just append the data)

            printf( " uncompressed data\n" );

            ::memcpy( uncompressed, compressed, pack_block.data_len );
            uncompressed += pack_block.data_len;
            compressed += pack_block.data_len;
            continue;
        }

        // compressed - unpack

        // some sort of offset/value maps
        tables_t tables = prepare_tables( compressed, pack_block.packed_size, pack_block.data_len );

        for( size_t i = 0; i < tables.data.size(); ++i )
        {
            const uint8_t val_2 = tables.data[i];
            const uint8_t val_3 = tables.table3[val_2];

            if( val_3 == 0 )
            {
                *uncompressed++ = val_2;
            }
            else
            {
                val_3_non_0( uncompressed, tables, val_3 );
            }
        }

        if( pack_block.flag == 0 )
        {
            return;
        }
    }
}

int main()
{
    const std::vector<uint8_t> uncompressed_reference( uncompressed_example,
                                                       uncompressed_example + sizeof( uncompressed_example ) );

    std::vector<uint8_t> uncompressed( sizeof( uncompressed_example ) );
    const std::vector<uint8_t> compressed( compressed_example, compressed_example + sizeof( compressed_example ) );

    emu_GAME_START_sub_3( uncompressed, compressed );

    assert( uncompressed == uncompressed_reference );

    return 0;
}
