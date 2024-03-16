#▒ ISC License

#▒ Copyright (c) 2024 Sugiton <git@sugiton.dev>

#▒ Permission to use, copy, modify, and/or distribute this software for any
#▒ purpose with or without fee is hereby granted, provided that the above
#▒ copyright notice and this permission notice appear in all copies.

#▒ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
#▒ REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
#▒ AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
#▒ INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
#▒ LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
#▒ OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#▒ PERFORMANCE OF THIS SOFTWARE.

CC := gcc

SRL := demo/list.c
SRA := demo/directory.c
EXL := demo_list
EXA := demo_directory

.RECIPEPREFIX := >
.PHONY        := list directory clean

exe :
> $(CC) -I. -o $(EXL) $(SRL)
> $(CC) -I. -o $(EXA) $(SRA)

all :| clean exe;

list :
> $(if $(wildcard $(EXL)),$(info ...executing $(EXL))@./$(EXL))

directory :
> $(if $(wildcard $(EXA)),$(info ...executing $(EXA))@./$(EXA))

clean :
> $(if $(wildcard $(EXL)),$(info ...removing  $(EXL))@rm -f $(EXL))
> $(if $(wildcard $(EXA)),$(info ...removing  $(EXA))@rm -f $(EXA))
