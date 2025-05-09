#ifndef VC_H
#define VC_H 1

#include <stdint.h>
#include <time.h>

#define MAX_NAME_LEN 1024

struct member
{
  char name[MAX_NAME_LEN + 1]; // nome do membro
  uint32_t uid;                // id único
  uint32_t pos;                // posição no arquivo
  uint64_t osz;                // tamanho original
  uint64_t dsz;                // tamanho em disco
  uint64_t offset;             // offset em relação ao início do arquivo
  time_t mtime;                // momento da última modificação
} __attribute__((packed));

struct directory
{
  struct member *memv; // vetor de membros
  uint32_t memc;       // quantidade de membros em memv
};

#endif /* vc.h */
