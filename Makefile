CC = gcc

CFLAGS += -Wall -Wextra -Werror 

DEPS += include/list.h

OBJFILES = src/main.o src/tar_options.o src/tar_archive.o\
						src/tar_util.o src/tar_extract.o\
						src/tar_append.o src/tar_update.o src/tar_list.o


TARGET = my_tar

all: $(TARGET)

$(TARGET): $(OBJFILES) $(DEPS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

.PHONY: clean
clean:
	rm -f *.o *.tar *.out $(TARGET)