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

### Multiple connections

If you need to use more than one connectionn, you've also aliases mq1 to mq10:

```
mq3 connect
mq3 disconnect
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
