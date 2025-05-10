#ifndef VCDIR_H
#define VCDIR_H 1

#include <stdint.h>

// inicializa dir a partir de vcpath
int read_dir (struct directory *dir, const char *vcpath);

// grava dir no fim de vcpath
int write_dir (struct directory *dir, const char *vcpath);

// retorna o índice do membro de nome memname no vetor dir->memv
int mem_index (struct directory *dir, const char *memname);

// remove o membro de índice idx do vetor dir->memv
int rm_mem (struct directory *dir, uint32_t idx);

// adiciona o membro mem ao final do vetor dir->memv
int add_mem (struct directory *dir, struct member *mem);

#endif /* vcdir.h */