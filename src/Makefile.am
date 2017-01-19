ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS     =

AM_CPPFLAGS += -I"@HASHPIPE_INCDIR@"

# AM_CFLAGS is used for all C compiles
AM_CFLAGS = -fPIC -O3 -Wall -Werror -fno-strict-aliasing -mavx

hw_databuf = hw_databuf.h \
             hw_databuf.c

hw_threads = hw_thread1.c \
             hw_thread2.c

# This is the flag_gpu plugin itself
lib_LTLIBRARIES = hw_hashpipe.la
hw_hashpipe_la_SOURCES  = $(hw_databuf) $(hw_threads)
hw_hashpipe_la_LIBADD   = -lrt -L/usr/local/cuda/lib64
hw_hashpipe_la_LDFLAGS  = -avoid-version -module -shared -export-dynamic --enable-shared
hw_hashpipe_la_LDFLAGS += -L"@HASHPIPE_LIBDIR@" -Wl,-rpath,"@HASHPIPE_LIBDIR@"

# vi: set ts=8 noet :
