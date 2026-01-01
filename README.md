## libasm

NASM x86_64 reimplementations of a small libc subset for the 42 Libasm subject (see `en.subject.pdf`). Code follows the System V AMD64 ABI, uses Intel syntax, and targets Linux with `nasm -f elf64`.

### Scope (mandatory)
| Symbol     | Prototype                                           | Notes |
|------------|------------------------------------------------------|-------|
| ft_strlen  | size_t ft_strlen(const char *s);                    | byte length before the first `\0` |
| ft_strcpy  | char *ft_strcpy(char *dst, const char *src);        | returns `dst` |
| ft_strcmp  | int ft_strcmp(const char *s1, const char *s2);      | lexicographic compare |
| ft_write   | ssize_t ft_write(int fd, const void *buf, size_t count); | wraps `write(2)`, sets `errno` |
| ft_read    | ssize_t ft_read(int fd, void *buf, size_t count);   | wraps `read(2)`, validates `buf`, sets `errno` |
| ft_strdup  | char *ft_strdup(const char *s);                     | allocates with `malloc` |

### Requirements
- Linux x86_64
- `nasm` (Intel syntax, `-f elf64`)
- `gcc`
- 64-bit calling convention; do not compile with `-no-pie`

### Build targets (Makefile)
- `make` / `make all` — build `libasm.a` from available mandatory objects
- `make bonus` — same archive with bonus objects if `ft_*_bonus.s` files exist
- `make shared` — build `libasm.so` (uses any present mandatory/bonus objects)
- `make test` — build `libasm.so`, then run the harness in `tests/run_tests` with `LD_LIBRARY_PATH=.`
- `make clean` / `make fclean` / `make re` — remove build artifacts or rebuild from scratch
- `make check` — report missing mandatory `.s` files

### Tests
- Harness lives in `tests/` and loads symbols via `dlopen`/`dlsym`.
- Run `make test` to compile the shared library and execute all bundled tests.

### Usage
- Static link: `gcc your.c -L. -lasm`
- Dynamic link: `gcc your.c -L. -lasm -Wl,-rpath,.` (or set `LD_LIBRARY_PATH=.` at runtime)
- Quick standalone demo for `ft_read` (no full build required):
  ```bash
  nasm -f elf64 ft_read.s -o ft_read.o
  gcc main.c ft_read.o -o demo
  ./demo   # reads foo.txt and writes it to stdout
  ```

### Behavior notes
- System V AMD64 ABI throughout; stack stays 16-byte aligned across calls.
- `ft_write` is a direct `write(2)` syscall wrapper; on errors, it stores `errno` via `__errno_location` and returns `-1`.
- `ft_read` returns 0 when `count == 0` and sets `errno = EFAULT` when `buf == NULL` with `count > 0`; negative syscall returns map to `errno` via `__errno_location`.
- `ft_strdup` uses `malloc(len + 1)` and `ft_strcpy`; it returns NULL on allocation failure.

### Bonus
Required prototypes per the subject:

- `int ft_atoi_base(char *str, char *base);`
- `void ft_list_push_front(t_list **begin_list, void *data);`
- `int ft_list_size(t_list *begin_list);`
- `void ft_list_sort(t_list **begin_list, int (*cmp)());`
- `void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));`

### Repository layout
- `ft_*.s` — assembly sources for mandatory symbols
- `tests/` — test sources and harness (`tests/run_tests` binary is built by `make test`)
- `en.subject.pdf` — original project brief
