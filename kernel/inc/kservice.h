/*
 * @Author: spaceman
 * @Date: 2021-03-05 17:16:16
 * @LastEditTime: 2021-03-05 17:39:11
 * @LastEditors: spaceman
 * @Description:
 * @FilePath: \XidianOS\kernel\inc\kservice.h
 */
#ifndef __K_SERVICE_H_
#define __K_SERVICE_H_

#include <xd_k.h>
#include <xd_def.h>


/* XidianOS version information */
#define XD_VERSION                      0L              /**< major version number */
#define XD_SUBVERSION                   0L              /**< minor version number */
#define XD_REVISION                     1L              /**< revise version number */

/* XidianOS version */
#define XIDIANOS_VERSION                ((XD_VERSION * 10000) + \
                                        (XD_SUBVERSION * 100) + XD_REVISION)

#define  KPRINTF_BUFSIZE                128

#define xd_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

void xd_kprintf(const char* fmt , ...);
void xd_show_version(void);
xd_int32_t xd_strncmp(const char *cs, const char *ct, xd_uint32_t count);

#endif
