# Lox

Another practicing interpreter project by follow Bob Nystrom's jlox in [craftinginterpreters](http://www.craftinginterpreters.com).
This is a C++ version.

# Caution

Please notice that I started this project when I'm rather unfamiliar with C++, so I am using raw pointers and incorrect access protections here.
If you are going to implement lox in C++ by yourself, please remember to use smart pointers, constant references, and initializer list.
Just like how Bob uses "final references" in java to prevent false assignment, which is very very important.

This project is also a warning for me, to remind me how bad codes will lower the effectiveness of debugging.

# Reference

[craftinginterpreters](http://www.craftinginterpreters.com)

[UNIX exit_code](https://www.freebsd.org/cgi/man.cgi?query=sysexits&apropos=0&sektion=0&manpath=FreeBSD+4.3-RELEASE&format=html)