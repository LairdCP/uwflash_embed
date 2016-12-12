// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Copyright (c) 2016, Laird                                                  ++
//                                                                            ++
// Permission to use, copy, modify, and/or distribute this software for any   ++
// purpose with or without fee is hereby granted, provided that the above     ++
// copyright notice and this permission notice appear in all copies.          ++
//                                                                            ++
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES   ++
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF           ++
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR    ++
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES     ++
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN      ++
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR ++
// IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.                ++
//                                                                            ++
// SPDX-License-Identifier:ISC                                                ++
//                                                                            ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                                                                            ++
// Source to embed firmware upgrader functionality into a host system which   ++
// interacts with specific Laird modules. The host can be a microcontroller   ++
// or full blown OS based PC like Windows/Linex/Mac or other.                 ++
//                                                                            ++
// The source requires a C++ compiler and has been used to create utilities   ++
// supplied by Laird.                                                         ++
//                                                                            ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if !defined(_TARGETUWG_H)     /* prevent multiple inclusions */
#define _TARGETUWG_H

#if !defined(EXCLUDE_VERSION_CORE_HEADER)
#include "UwgVersionCore.h"  /* contains UwScript Core Version Number "AT I 33" */
#endif
        /*....................................................................
        ** This is used to add patterns at the beginning and end of malloc
        ** blocks, which are checked when a block is freed. It helps debug
        ** code which is writing to areas it shouldn't
        **...................................................................*/
#define notUWG_HEAP_DATA_ALLOC_FREE_CHECK

        /*....................................................................
        ** All mallocs/frees are through UwgMalloc_ and UwgFree_
        **...................................................................*/
#define UWG_SINGLE_HEAP_MANAGER
#define     SINGLE_HEAP_MANAGER   /* kept for legacy reason */

        /*....................................................................
        ** Keeps count of mallocs and frees
        **...................................................................*/
#define notUWG_MONITOR_MALLOCS

        /*....................................................................
        ** This enables a routine at compile time so that resultcodes are
        ** translated into verbose strings. See routine UwgResultCodeString()
        **...................................................................*/
#define notUWG_GENERATE_VERBOSE_RESULTCODES

        /*....................................................................
        ** Enables the following:-
        ** AT I 43 to return sizeof TOK Context Block
        ** AT I 44 to return sizeof RUN Context Block
        ** AT I 45 to return sizeof RUN Public Context Block
        ** AT I 46 to return sizeof IMM Context Block
        ** AT I 47 to return sizeof IMM Public Context Block
        ** AT I 48 to return sizeof APP Context Block
        **...................................................................*/
#define notUWG_SHOW_STRUCT_SIZES


#endif /* prevent multiple inclusions */
