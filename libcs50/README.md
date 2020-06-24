# CS50 Tiny Search Engine (TSE) utility library

This library contains many modules utiilzed by the rest of the TSE program. The data structures used, counters.c, set.c and hashtable.c, were written by Caroline Tornquist; memory.c, webpage.c, file.c and jhash.c were provided by Dartmouth faculty. 

## Usage

To build `libcs50.a`, run `make`. To clean up, run `make clean`.

## Overview

 * `bag` - the **bag** data structure from Lab 3
 * `counters` - the **counters** data structure from Lab 3
 * [`file`](file.html) - functions to read files (includes readlinep)
 * `hashtable` - the **hashtable** data structure from Lab 3
 * `jhash` - the Jenkins Hash function used by hashtable
 * [`memory`](memory.html) - handy wrappers for malloc/free
 * `set` - the **set** data structure from Lab 3
 * [`webpage`](webpage.html) - functions to load and scan web pages
