#ifndef VCDEF_H
#define VCDEF_H 1

#include <stdint.h>
#include <sys/types.h>
#include <time.h>

// o tamanho máximo de um nome de arquivo é 255 caracteres na maioria dos
// sistemas de arquivos (ext4, ntfs, etc.).
// então não faz sentido usar um tamanho maior
#define MAX_NAME_LEN 255

struct member
{
  char name[MAX_NAME_LEN + 1]; // nome do membro
  uid_t uid;                   // user id
  uint32_t pos;                // posição no arquivo
  uint64_t osz;                // tamanho original
  uint64_t dsz;                // tamanho em disco
  uint64_t offset;             // offset em relação ao início do arquivo
  time_t mtime;                // momento da última modificação
} __attribute__ ((packed));

struct directory
{
  struct member *memv; // vetor de membros
  uint32_t memc;       // quantidade de membros em memv
};

#endif /* vcdef.h */