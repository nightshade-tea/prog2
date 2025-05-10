#ifndef VCDIR_H
#define VCDIR_H 1

#include <stdint.h>

// inicializa dir a partir de vcpath
int read_dir (struct directory *dir, FILE *vcfp);

// grava dir no fim de vcpath
int write_dir (struct directory *dir, FILE *vcfp);

// retorna o índice do membro de nome memname no vetor dir->memv ou UINT32_MAX
// se não encontrado
uint32_t mem_index (struct directory *dir, const char *memname);

// remove o membro de índice idx do vetor dir->memv
int rm_mem (struct directory *dir, uint32_t idx);

// adiciona o membro mem ao final do vetor dir->memv
int add_mem (struct directory *dir, struct member *mem);

#endif /* vcdir.h */