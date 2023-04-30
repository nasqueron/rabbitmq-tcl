# RabbitMQ TCL

This library is a wrapper around [rabbitmq-c](https://github.com/alanxz/rabbitmq-c).

It exposes the C functions allowing to interact with a RabbitMQ server in TCL.

## Documentation

* rabbitmq-tcl.c - the extension code
* makePkgIndex.c - a tool to prepare pkgIndex.tcl

## Customize the build

### config.h

If you want to be able to use different default values, you can edit the defines
at `src/config.h`, then rebuild the extension.

### E4C

This extension is a wrapper around librabbitmq, which is not bulletproof against
null pointer errors.

In a critical context, we suggest to enable e4c. If a command `mq quux` would
try to use a NULL pointer, instead of segfault, you would get a TCL error,
and then would be able to resume operations:

```
% mq quux
Segmentation violation.
%
```

## Licensing

(c) 2015, Nasqueron, some rights reserved.
Released under BSD-2-Clause license.

This work includes the exceptions4c library.
(c) 2013 Guillermo Calvo
Released under LGPL license.

### Note if you relicense a derivative work

If you create any derivative work of this library, you must do one of the
following actions:

    * drop exceptions4c support (rm vendor/e4c and the #ifdef USE_E4C blocks)
    * release the whole work under LGPLv3 or GPLv3
    * publish source code or object form (.o) so it's possible to relink
      against another version of exceptions4c [this is what we do]

According http://www.gnu.org/licenses/gpl-faq.html#LGPLStaticVsDynamic, you can
link staticly against a LGPL library as long as you provide the .o object of
your application stripped of the LGPL code or provide the source code and build
tools to achieve it.

## Misc

### Acknowledgment

* Alan Antonuk, rabbitmq-c maintainer
* Fastbase, which provides [fbsql](http://www.fastbase.co.nz/fbsql/fbsql.c),
  which is used as the model and starting point for this library.

### Colophon

This work has been created for Nasqueron, to allow their operations and
infrastructure TCL code to interact to our message broker.

**Editors used:** vim, Cloud9
**Tools used:** Arcanist, clang-format
**Compiler:** clang/llvm
