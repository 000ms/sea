## Chronicle of C compilers

*The GNU C dialect is comprised of the GNU C implementation along with additional extensions.*

In modern times, newly developed programming languages claim to be safer, citing Scope-Bound Resource Management (SBRM) as a recurring argument.

This overlooks the fact that this feature has existed in GNU C for decades.

The essence of portability lies not in the number of compilers supported, but in the number of platforms that can run the programs. With compilers that support GNU C, programs can be made to operate on a wide range of systems and architectures, making them portable.

The GNU's extensions are gradually being incorporated into the ISO C specification.

## Scope-Bound Resource Management (SBRM)
*Resource Acquisition is Initialization (RAII)*

Variables that are declared as automatic are created and destroyed automatically on the stack. However, due to its restricted size, commonly ranging from 512KB to 8MB, programmers might use the heap for memory allocation, with manual management.

Manual memory management involves a drawback in case of program abortion, when the instructions supposed to release the resources will not be carried out. The concept behind SBRM is to tie the lifespan of heap allocated memory to an owner allocated on the stack, associated with a callback. In case of programs throwing exceptions, which are guaranteed to go through the stack unwinding, the unrolling of the owner will trigger the callback and free the resources.

This is the C++ technique to call destructors, and a similar mechanism can be achieved using GNU C.

## Generics

Templates are a powerful C++ feature that allow for the creation of flexible, reusable source. With it, developers can write code that works with a variety of data types, rather than having to repeat boilerplate blocks. This reduces duplication and improves maintainability. Again, it can be achieved with GNU C.
