/* $Id: net.c,v 5.3 2001/07/08 10:09:06 bertg Exp $
 *
 * XPilot, a multiplayer gravity war game.  Copyright (C) 1991-2001 by
 *
 *      Bj�rn Stabell        <bjoern@xpilot.org>
 *      Ken Ronny Schouten   <ken@xpilot.org>
 *      Bert Gijsbers        <bert@xpilot.org>
 *      Dick Balaska         <dick@xpilot.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#ifndef _WINDOWS
# include <unistd.h>
# ifndef __hpux
#  include <sys/time.h>
# endif
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
#endif

#ifdef __sgi
# include <bstring.h>
#endif

#ifdef _WINDOWS
# include "NT/winNet.h"
# undef	va_start		/* there are bad versions in windows.h's "stdarg.h" */
# undef	va_end
# include <varargs.h>
#endif

#include "version.h"
#include "config.h"
#include "const.h"
#include "error.h"
#include "net.h"
#include "packet.h"
#include "bit.h"
#include "socklib.h"

char net_version[] = VERSION;

int last_packet_of_frame;

int Sockbuf_init(sockbuf_t *sbuf, sock_t *sock, int size, int state)
{
    if ((sbuf->buf = sbuf->ptr = (char *) malloc(size)) == NULL) {
	return -1;
    }
    if (sock != (sock_t *) NULL) {
	sbuf->sock = *sock;
    } else {
	sock_init(&sbuf->sock);
    }
    sbuf->state = state;
    sbuf->len = 0;
    sbuf->size = size;
    sbuf->ptr = sbuf->buf;
    sbuf->state = state;
    return 0;
}

int Sockbuf_cleanup(sockbuf_t *sbuf)
{
    if (sbuf->buf != NULL) {
	free(sbuf->buf);
    }
    sbuf->buf = sbuf->ptr = NULL;
    sbuf->size = sbuf->len = 0;
    sbuf->state = 0;
    return 0;
}

int Sockbuf_clear(sockbuf_t *sbuf)
{
    sbuf->len = 0;
    sbuf->ptr = sbuf->buf;
    return 0;
}

int Sockbuf_advance(sockbuf_t *sbuf, int len)
{
    /*
     * First do a few buffer consistency checks.
     */
    if (sbuf->ptr > sbuf->buf + sbuf->len) {
	errno = 0;
	error("Sockbuf pointer too far");
	sbuf->ptr = sbuf->buf + sbuf->len;
    }
    if (sbuf->ptr < sbuf->buf) {
	errno = 0;
	error("Sockbuf pointer bad");
	sbuf->ptr = sbuf->buf;
    }
    if (sbuf->len > sbuf->size) {
	errno = 0;
	error("Sockbuf len too far");
	sbuf->len = sbuf->size;
    }
    if (sbuf->len < 0) {
	errno = 0;
	error("Sockbuf len bad");
	sbuf->len = 0;
    }
    if (len <= 0) {
	if (len < 0) {
	    errno = 0;
	    error("Sockbuf advance negative (%d)", len);
	}
    }
    else if (len >= sbuf->len) {
	if (len > sbuf->len) {
	    errno = 0;
	    error("Sockbuf advancing too far");
	}
	sbuf->len = 0;
	sbuf->ptr = sbuf->buf;
    } else {
#if defined(__hpux) || defined(SVR4) || defined(_SEQUENT_) || defined(SYSV) || defined(_WINDOWS)
	memmove(sbuf->buf, sbuf->buf + len, sbuf->len - len);
#else
	bcopy(sbuf->buf + len, sbuf->buf, sbuf->len - len);
#endif
	sbuf->len -= len;
	if (sbuf->ptr - sbuf->buf <= len) {
	    sbuf->ptr = sbuf->buf;
	} else {
	    sbuf->ptr -= len;
	}
    }
    return 0;
}

int Sockbuf_flush(sockbuf_t *sbuf)
{
    int			len,
			i;

    if (BIT(sbuf->state, SOCKBUF_WRITE) == 0) {
	errno = 0;
	error("No flush on non-writable socket buffer");
	error("(state=%02x,buf=%08x,ptr=%08x,size=%d,len=%d,sock=%d)",
	    sbuf->state, sbuf->buf, sbuf->ptr, sbuf->size, sbuf->len,
	    sbuf->sock);
	return -1;
    }
	/*Trace("Sockbuf_flush: state=%02x,buf=%08x,ptr=%08x,size=%d,len=%d,sock=%d\n",
	    sbuf->state, sbuf->buf, sbuf->ptr, sbuf->size, sbuf->len,
	    sbuf->sock); */
    if (BIT(sbuf->state, SOCKBUF_LOCK) != 0) {
	errno = 0;
	error("No flush on locked socket buffer (0x%02x)", sbuf->state);
	return -1;
    }
    if (sbuf->len <= 0) {
	if (sbuf->len < 0) {
	    errno = 0;
	    error("Write socket buffer length negative");
	    sbuf->len = 0;
	    sbuf->ptr = sbuf->buf;
	}
	return 0;
    }

#if 0
    /* maintain a few statistics */
    {
	static int		max = 1024, avg, count;

	avg += sbuf->len;
	count++;
	if (sbuf->len > max) {
	    max = sbuf->len;
	    printf("Max packet = %d, avg = %d\n", max, avg / count);
	}
	else if (max > 1024 && (count & 0x03) == 0) {
	    max--;
	}
    }
#endif

    if (BIT(sbuf->state, SOCKBUF_DGRAM) != 0) {
	errno = 0;
	i = 0;
#if 0
	if (randomMT() % 12 == 0)	/* artificial packet loss */
	    len = sbuf->len;
	else
#endif
	while ((len = sock_write(&sbuf->sock, sbuf->buf, sbuf->len)) <= 0) {
	    if (len == 0
		|| errno == EWOULDBLOCK
		|| errno == EAGAIN) {
		Sockbuf_clear(sbuf);
		return 0;
	    }
	    if (errno == EINTR) {
		errno = 0;
		continue;
	    }
#if 0
	    if (errno == ECONNREFUSED) {
		error("Send refused");
		Sockbuf_clear(sbuf);
		return -1;
	    }
#endif
	    if (++i > MAX_SOCKBUF_RETRIES) {
		error("Can't send on socket (%d,%d)", sbuf->sock, sbuf->len);
		Sockbuf_clear(sbuf);
		return -1;
	    }
	    { static int send_err;
		if ((send_err++ & 0x3F) == 0) {
		    error("send (%d)", i);
		}
	    }
	    if (sock_get_error(&sbuf->sock) == -1) {
		error("sock_get_error send");
		return -1;
	    }
	    errno = 0;
	}
	if (len != sbuf->len) {
	    errno = 0;
	    error("Can't write complete datagram (%d,%d)", len, sbuf->len);
	}
	Sockbuf_clear(sbuf);
    } else {
	errno = 0;
	while ((len = sock_write(&sbuf->sock, sbuf->buf, sbuf->len)) <= 0) {
	    if (errno == EINTR) {
		errno = 0;
		continue;
	    }
	    if (errno != EWOULDBLOCK
		&& errno != EAGAIN) {
		error("Can't write on socket");
		return -1;
	    }
	    return 0;
	}
	Sockbuf_advance(sbuf, len);
    }
    return len;
}

int Sockbuf_write(sockbuf_t *sbuf, char *buf, int len)
{
    if (BIT(sbuf->state, SOCKBUF_WRITE) == 0) {
	errno = 0;
	error("No write to non-writable socket buffer");
	return -1;
    }
    if (sbuf->size - sbuf->len < len) {
	if (BIT(sbuf->state, SOCKBUF_LOCK | SOCKBUF_DGRAM) != 0) {
	    errno = 0;
	    error("No write to locked socket buffer (%d,%d,%d,%d)",
		sbuf->state, sbuf->size, sbuf->len, len);
	    return -1;
	}
	if (Sockbuf_flush(sbuf) == -1) {
	    return -1;
	}
	if (sbuf->size - sbuf->len < len) {
	    return 0;
	}
    }
    memcpy(sbuf->buf + sbuf->len, buf, len);
    sbuf->len += len;

    return len;
}

int Sockbuf_read(sockbuf_t *sbuf)
{
    int			max,
			i,
			len;

    if (BIT(sbuf->state, SOCKBUF_READ) == 0) {
	errno = 0;
	error("No read from non-readable socket buffer (%d)", sbuf->state);
	return -1;
    }
    if (BIT(sbuf->state, SOCKBUF_LOCK) != 0) {
	return 0;
    }
    if (sbuf->ptr > sbuf->buf) {
	Sockbuf_advance(sbuf, sbuf->ptr - sbuf->buf);
    }
    if ((max = sbuf->size - sbuf->len) <= 0) {
	static int before;
	if (before++ == 0) {
	    errno = 0;
	    error("Read socket buffer not big enough (%d,%d)",
		  sbuf->size, sbuf->len);
	}
	return -1;
    }
    if (BIT(sbuf->state, SOCKBUF_DGRAM) != 0) {
	errno = 0;
	i = 0;
#if 0
	if (randomMT() % 12 == 0)		/* artificial packet loss */
	    len = sbuf->len;
	else
#endif
	while ((len = sock_read(&sbuf->sock, sbuf->buf + sbuf->len, max)) <= 0) {
	    if (len == 0) {
		return 0;
	    }
#ifdef _WINDOWS
		errno = WSAGetLastError();
#endif
	    if (errno == EINTR) {
		errno = 0;
		continue;
	    }
	    if (errno == EWOULDBLOCK
		|| errno == EAGAIN) {
		return 0;
	    }
#if 0
	    if (errno == ECONNREFUSED) {
		error("Receive refused");
		return -1;
	    }
#endif
/*
		Trace("errno=%d (%s) len = %d during sock_read\n", 
			errno, _GetWSockErrText(errno), len);
*/			
	    if (++i > MAX_SOCKBUF_RETRIES) {
		error("Can't recv on socket");
		return -1;
	    }
	    { static int recv_err;
		if ((recv_err++ & 0x3F) == 0) {
		    error("recv (%d)", i);
		}
	    }
	    if (sock_get_error(&sbuf->sock) == -1) {
		error("GetSocketError recv");
		return -1;
	    }
	    errno = 0;
	}
	sbuf->len += len;
    } else {
	errno = 0;
	while ((len = sock_read(&sbuf->sock, sbuf->buf + sbuf->len, max)) <= 0) {
	    if (len == 0) {
		return 0;
	    }
	    if (errno == EINTR) {
		errno = 0;
		continue;
	    }
	    if (errno != EWOULDBLOCK
		&& errno != EAGAIN) {
		error("Can't read on socket");
		return -1;
	    }
	    return 0;
	}
/*	IFWINDOWS( Trace("Read stream %d bytes from %d\n", len, sbuf->sock); )*/
	sbuf->len += len;
    }

    return sbuf->len;
}

int Sockbuf_copy(sockbuf_t *dest, sockbuf_t *src, int len)
{
    if (len < dest->size - dest->len) {
	errno = 0;
	error("Not enough room in destination copy socket buffer");
	return -1;
    }
    if (len < src->len) {
	errno = 0;
	error("Not enough data in source copy socket buffer");
	return -1;
    }
    memcpy(dest->buf + dest->len, src->buf, len);
    dest->len += len;

    return len;
}

#if STDVA
int Packet_printf(sockbuf_t *sbuf, const char *fmt, ...)
#else
int Packet_printf(va_alist)
    va_dcl
#endif
{
#define PRINTF_FMT	1
#define PRINTF_IO	2
#define PRINTF_SIZE	3

    int			i,
			cval,
			ival,
			count,
			failure = 0,
			max_str_size;
    int*                iptr;
    unsigned		uval;
    short		sval;
    unsigned short	usval;
    long		lval;
    unsigned long	ulval;
    float               fval;
    char		*str,
			*end,
			*buf,
			*stop;
    va_list		ap;
#if !STDVA
    char		*fmt;
    sockbuf_t		*sbuf;

    va_start(ap);
    sbuf = va_arg(ap, sockbuf_t *);
    fmt = va_arg(ap, char *);
#else
    va_start(ap, fmt);
#endif

    /*
     * Stream socket buffers should flush the buffer if running
     * out of write space.  This is currently not needed cause
     * only datagram sockets are used or the buffer is locked.
     */

    /*
     * Mark the end of the available buffer space,
     * but keep a little room for a terminating packet.
     * This terminating packet will be from Send_end_of_frame()
     * in netserver.c.  This is a hack, sorry.
     * But we want to send frames even if they're bigger than
     * our available buffer space.
     */
    end = sbuf->buf + sbuf->size;
    if (last_packet_of_frame != 1) {
	end -= SOCKBUF_WRITE_SPARE;
    }
    buf = sbuf->buf + sbuf->len;
    for (i = 0; failure == 0 && fmt[i] != '\0'; i++) {
	if (fmt[i] == '%') {
	    switch (fmt[++i]) {
	    case 'c':
		if (buf + 1 >= end) {
		    failure = PRINTF_SIZE;
		    break;
		}
		cval = va_arg(ap, int);
		*buf++ = cval;
		break;
	    case 'd':
		if (buf + 4 >= end) {
		    failure = PRINTF_SIZE;
		    break;
		}
		ival = va_arg(ap, int);
		*buf++ = ival >> 24;
		*buf++ = ival >> 16;
		*buf++ = ival >> 8;
		*buf++ = ival;
		break;
	    case 'u':
		if (buf + 4 >= end) {
		    failure = PRINTF_SIZE;
		    break;
		}
		uval = va_arg(ap, unsigned);
		*buf++ = uval >> 24;
		*buf++ = uval >> 16;
		*buf++ = uval >> 8;
		*buf++ = uval;
		break;
	    case 'h':
		if (buf + 2 >= end) {
		    failure = PRINTF_SIZE;
		    break;
		}
		switch (fmt[++i]) {
		case 'd':
		    sval = va_arg(ap, int);
		    *buf++ = sval >> 8;
		    *buf++ = (char)sval;
		    break;
		case 'u':
		    usval = va_arg(ap, unsigned);
		    *buf++ = usval >> 8;
		    *buf++ = (char)usval;
		    break;
		default:
		    failure = PRINTF_FMT;
		    break;
		}
		break;
	    case 'l':
		if (buf + 4 >= end) {
		    failure = PRINTF_SIZE;
		    break;
		}
		switch (fmt[++i]) {
		case 'd':
		    lval = va_arg(ap, long);
		    *buf++ = (char)(lval >> 24);
		    *buf++ = (char)(lval >> 16);
		    *buf++ = (char)(lval >> 8);
		    *buf++ = (char)lval;
		    break;
		case 'u':
		    ulval = va_arg(ap, unsigned long);
		    *buf++ = (char)(ulval >> 24);
		    *buf++ = (char)(ulval >> 16);
		    *buf++ = (char)(ulval >> 8);
		    *buf++ = (char)ulval;
		    break;
		default:
		    failure = PRINTF_FMT;
		    break;
		}
		break;
            case 'f': //added float support
		if (buf + 4 >= end) {
		    failure = PRINTF_SIZE;
		    break;
		}
                //We search for the first double because floats are promoted
                fval = va_arg(ap, double);
                //Get an int pointer to point to the same data, because floats doesn't support shifts
                iptr = (float*)&fval;

                //Send it over the network, a byte at a time - copied from other case's
                *buf++ = (char)(*iptr >> 24);
                *buf++ = (char)(*iptr >> 16);
                *buf++ = (char)(*iptr >> 8);
                *buf++ = (char)*iptr;
                break;
	    case 'S':	/* Big strings */
	    case 's':	/* Small strings */
		max_str_size = (fmt[i] == 'S') ? MSG_LEN : MAX_CHARS;
		str = va_arg(ap, char *);
		if (buf + max_str_size >= end) {
		    stop = end;
		} else {
		    stop = buf + max_str_size;
		}
		/* Send the nul byte too */
		do {
		    if (buf >= stop) {
			break;
		    }
		} while ((*buf++ = *str++) != '\0');
		if (buf > stop) {
		    failure = PRINTF_SIZE;
		}
		break;
	    default:
		failure = PRINTF_FMT;
		break;
	    }
	} else {
	    failure = PRINTF_FMT;
	}
    }
    if (failure != 0) {
	count = -1;
	if (failure == PRINTF_SIZE) {
#if 0
	    static int before;
	    if ((before++ & 0x0F) == 0) {
		printf("Write socket buffer not big enough (%d,%d,\"%s\")\n",
		    sbuf->size, sbuf->len, fmt);
	    }
#endif
	    if (BIT(sbuf->state, SOCKBUF_DGRAM) != 0) {
		count = 0;
		failure = 0;
	    }
	}
	else if (failure == PRINTF_FMT) {
	    errno = 0;
	    error("Error in format string (\"%s\")", fmt);
	}
    } else {
	count = buf - (sbuf->buf + sbuf->len);
	sbuf->len += count;
    }

    va_end(ap);

    return count;
}

#if STDVA
int Packet_scanf(sockbuf_t *sbuf, const char *fmt, ...)
#else
int Packet_scanf(va_alist)
    va_dcl
#endif
{
    int			i,
			j,
			k,
			*iptr,
			count = 0,
			failure = 0,
			max_str_size;
    unsigned		*uptr;
    short		*sptr;
    unsigned short	*usptr;
    long		*lptr;
    unsigned long	*ulptr;
    char		*cptr,
			*str;
    float               *fptr;
    va_list		ap;
#if !STDVA
    char		*fmt;
    sockbuf_t		*sbuf;

    va_start(ap);
    sbuf = va_arg(ap, sockbuf_t *);
    fmt = va_arg(ap, char *);
#else
    va_start(ap, fmt);
#endif

    for (i = j = 0; failure == 0 && fmt[i] != '\0'; i++) {
	if (fmt[i] == '%') {
	    count++;
	    switch (fmt[++i]) {
	    case 'c':
		if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 1]) {
		    if (BIT(sbuf->state, SOCKBUF_DGRAM | SOCKBUF_LOCK) != 0) {
			failure = 3;
			break;
		    }
		    if (Sockbuf_read(sbuf) == -1) {
			failure = 2;
			break;
		    }
		    if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 1]) {
			failure = 3;
			break;
		    }
		}
		cptr = va_arg(ap, char *);
		*cptr = sbuf->ptr[j++];
		break;
	    case 'd':
		if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 4]) {
		    if (BIT(sbuf->state, SOCKBUF_DGRAM | SOCKBUF_LOCK) != 0) {
			failure = 3;
			break;
		    }
		    if (Sockbuf_read(sbuf) == -1) {
			failure = 2;
			break;
		    }
		    if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 4]) {
			failure = 3;
			break;
		    }
		}
		iptr = va_arg(ap, int *);
		*iptr = sbuf->ptr[j++] << 24;
		*iptr |= (sbuf->ptr[j++] & 0xFF) << 16;
		*iptr |= (sbuf->ptr[j++] & 0xFF) << 8;
		*iptr |= (sbuf->ptr[j++] & 0xFF);
		break;
	    case 'u':
		if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 4]) {
		    if (BIT(sbuf->state, SOCKBUF_DGRAM | SOCKBUF_LOCK) != 0) {
			failure = 3;
			break;
		    }
		    if (Sockbuf_read(sbuf) == -1) {
			failure = 2;
			break;
		    }
		    if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 4]) {
			failure = 3;
			break;
		    }
		}
		uptr = va_arg(ap, unsigned *);
		*uptr = (sbuf->ptr[j++] & 0xFF) << 24;
		*uptr |= (sbuf->ptr[j++] & 0xFF) << 16;
		*uptr |= (sbuf->ptr[j++] & 0xFF) << 8;
		*uptr |= (sbuf->ptr[j++] & 0xFF);
		break;
	    case 'h':
		if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 2]) {
		    if (BIT(sbuf->state, SOCKBUF_DGRAM | SOCKBUF_LOCK) != 0) {
			failure = 3;
			break;
		    }
		    if (Sockbuf_read(sbuf) == -1) {
			failure = 2;
			break;
		    }
		    if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 2]) {
			failure = 3;
			break;
		    }
		}
		switch (fmt[++i]) {
		case 'd':
		    sptr = va_arg(ap, short *);
		    *sptr = sbuf->ptr[j++] << 8;
		    *sptr |= (sbuf->ptr[j++] & 0xFF);
		    break;
		case 'u':
		    usptr = va_arg(ap, unsigned short *);
		    *usptr = (sbuf->ptr[j++] & 0xFF) << 8;
		    *usptr |= (sbuf->ptr[j++] & 0xFF);
		    break;
		default:
		    failure = 1;
		    break;
		}
		break;
	    case 'l':
		if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 4]) {
		    if (BIT(sbuf->state, SOCKBUF_DGRAM | SOCKBUF_LOCK) != 0) {
			failure = 3;
			break;
		    }
		    if (Sockbuf_read(sbuf) == -1) {
			failure = 2;
			break;
		    }
		    if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 4]) {
			failure = 3;
			break;
		    }
		}
		switch (fmt[++i]) {
		case 'd':
		    lptr = va_arg(ap, long *);
		    *lptr = sbuf->ptr[j++] << 24;
		    *lptr |= (sbuf->ptr[j++] & 0xFF) << 16;
		    *lptr |= (sbuf->ptr[j++] & 0xFF) << 8;
		    *lptr |= (sbuf->ptr[j++] & 0xFF);
		    break;
		case 'u':
		    ulptr = va_arg(ap, unsigned long *);
		    *ulptr = (sbuf->ptr[j++] & 0xFF) << 24;
		    *ulptr |= (sbuf->ptr[j++] & 0xFF) << 16;
		    *ulptr |= (sbuf->ptr[j++] & 0xFF) << 8;
		    *ulptr |= (sbuf->ptr[j++] & 0xFF);
		    break;
		default:
		    failure = 1;
		    break;
		}
		break;
            case 'f':
                //added float handling
                
                //this part is copied from other types
		if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 4]) {
		    if (BIT(sbuf->state, SOCKBUF_DGRAM | SOCKBUF_LOCK) != 0) {
			failure = 3;
			break;
		    }
		    if (Sockbuf_read(sbuf) == -1) {
			failure = 2;
			break;
		    }
		    if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 4]) {
			failure = 3;
			break;
		    }
		}

                //get a pointer to the variable we want to fill with data
                fptr = va_arg(ap, float *);
                {
                  //Need to create a tmp int variable because floats doesn't support |=
                  int tmp;

                  tmp = sbuf->ptr[j++] << 24;
                  tmp |= (sbuf->ptr[j++] & 0xFF) << 16;
                  tmp |= (sbuf->ptr[j++] & 0xFF) << 8;
                  tmp |= (sbuf->ptr[j++] & 0xFF);

                  //"Cast" the int to a float
                  *fptr = *((float*)&tmp);
                }
                break;
	    case 'S':	/* Big strings */
	    case 's':	/* Small strings */
		max_str_size = (fmt[i] == 'S') ? MSG_LEN : MAX_CHARS;
		str = va_arg(ap, char *);
		k = 0;
		for (;;) {
		    if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 1]) {
			if (BIT(sbuf->state, SOCKBUF_DGRAM | SOCKBUF_LOCK) != 0) {
			    failure = 3;
			    break;
			}
			if (Sockbuf_read(sbuf) == -1) {
			    failure = 2;
			    break;
			}
			if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j + 1]) {
			    failure = 3;
			    break;
			}
		    }
		    if ((str[k++] = sbuf->ptr[j++]) == '\0') {
			break;
		    }
		    else if (k >= max_str_size) {
			/*
			 * What to do now is unclear to me.
			 * The server should drop the packet, but
			 * the client has more difficulty with that
			 * if this is the reliable data buffer.
			 */
#ifndef SILENT
			errno = 0;
			error("String overflow while scanning (%d,%d)",
			      k, max_str_size);
#endif
			if (BIT(sbuf->state, SOCKBUF_LOCK) != 0) {
			    failure = 2;
			} else {
			    failure = 3;
			}
			break;
		    }
		}
		if (failure != 0) {
		    strcpy(str, "ErRoR");
		}
		break;
	    default:
		failure = 1;
		break;
	    }
	} else {
	    failure = 1;
	}
    }
    if (failure == 1) {
	errno = 0;
	error("Error in format string (%s)", fmt);
    }
    else if (failure == 3) {
	/* Not enough input for one complete packet */
	count = 0;
	failure = 0;
    }
    else if (failure == 0) {
	if (&sbuf->buf[sbuf->len] < &sbuf->ptr[j]) {
	    errno = 0;
	    error("Input buffer exceeded (%s)", fmt);
	    failure = 1;
	} else {
	    sbuf->ptr += j;
	}
    }

    va_end(ap);

    return (failure) ? -1 : count;
}

