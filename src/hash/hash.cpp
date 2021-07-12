#include <openssl/sha.h>
#include <openssl/rand.h>

#include <string>
#include <iostream>
#include <fstream>

#include "../../inc/hash.h"
#include "../../inc/hexstr.h"

// read from file or raw      
std::string calc_hash (bool use_disk, std::string target) {
    // if use_disk is true, target is a file name
    // if use_disk isn't than it's raw text

    unsigned char outhash[SHA256_DIGEST_LENGTH];
    std::string outstr;

    // init sha
    SHA256_CTX context;
    SHA256_Init(&context);

    if (use_disk) { // target is filename (disk)
        std::ifstream fl(target);
        fl.seekg(0, std::ios::end);
        size_t len = fl.tellg();
        char *ret = new char[len];
        fl.seekg(0, std::ios::beg);
        fl.read(ret, len);
        fl.close();
        outstr = ret;
    } else { // target is raw message (mem)
        outstr = target;
    }
    // finishing sha
    SHA256_Update(&context, outstr.c_str(), outstr.size());
    SHA256_Final(outhash, &context);
    
    return to_hexstr(outhash, SHA256_DIGEST_LENGTH);
};

std::string gen_trip(size_t len) {
    unsigned char rawbytes[16];
    if (!RAND_bytes(rawbytes, 16)) return NULL;
    return to_hexstr(rawbytes, 16);
}