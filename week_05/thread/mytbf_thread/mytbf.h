#ifndef MYTBF_H__
#define MYTBF_H__

#include <stdio.h>
#include <stdlib.h>

#define TBF_MAX    1024

typedef void tbf_t;

tbf_t *tbf_init(int cps, int burst);

int tbf_fetch_token(tbf_t *tbf, int fet_num);

int tbf_return_token(tbf_t *tbf, int ret_num);

void tbf_destroy(tbf_t **tbf);

#endif