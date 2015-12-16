# RabbitMQ TCL

This library is a wrapper around [rabbitmq-c](https://github.com/alanxz/rabbitmq-c).

It exposes the C functions allowing to interact with a RabbitMQ server in TCL.

Work in progress, not yet ready for production or a for a technical preview.

## User starting guide

### Installation

```
git clone https://github.com/dereckson/rabbitmq-tcl.git
cd rabbitmq-tcl
make
```

### How to use it?

**To load the library:**

```
load rabbitmq.so
```

**You'll then got a mq command to interact with your broker:**

```
mq connect [broker.domain.tld[:port]] [username] [password] [vhost]
mq disconnect
```

**To publish a message:**

```
mq publish <target> [-key <routing key>] [-mandatory] [-immediate] <message>
```

The target could be directly an exchange name, or use the following syntax:
`[-exchange <exchange name>] [-queue <queue name>]`

**To get a message:**

The prefered way is asynchronously.

But you can also use the synchronous basic.get implementation:

```
mq get <queue> [-noack]
```

Currently, basic.ack isn't implemented, so add -noack if you don't want the
broker send again the same messages at the next session.

**To bind a queue to an exchange:**

Creates a temporary autodelete queue

```
mq bindqueue <exchange> [bindingKey]
```

If the exchange doesn't exist, it will be created (in mode topic).

With RabbitMQ, the queue will be named randomly starting by `amq.gen-` and be
be deleted if consumed.

Please note `mq get` doesn't consume, it provides direct access instead.

### Multiple connections

If you need to use more than one connectionn, you've also aliases mq1 to mq10:

```
mq3 connect
mq3 disconnect
```

## Customize the build

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
