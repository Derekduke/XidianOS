/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#ifndef __LIST_H_
#define __LIST_H_
#include <xd_def.h>

void xd_list_init(xd_list_t* l);
void xd_list_insert_after(xd_list_t* l , xd_list_t* n);
void xd_list_insert_before(xd_list_t* l , xd_list_t* n);
void xd_list_remove(xd_list_t* n);

#endif
