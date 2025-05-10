#ifndef VCDIR_H
#define VCDIR_H 1

// inicializa dir a partir de vcpath
int read_dir (const char *vcpath, struct directory *dir);

// grava dir no fim de vcpath
int write_dir (const char *vcpath, struct directory *dir);

// retorna o índice do membro de nome memname no vetor dir->memv
int mem_index (const char *memname, struct directory *dir);

#endif /* vcdir.h */