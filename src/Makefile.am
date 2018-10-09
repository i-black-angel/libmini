lib_LTLIBRARIES = libmpl.la

# library version
libmpl_la_LDFLAGS=$(LTLDFLAGS)

libmpl_la_CPPFLAGS = -I$(top_srcdir)/include $(all_includes)
libmpl_la_CPPFLAGS += -DMG_ENABLE_CALLBACK_USERDATA=1
######################################
# Cflags and Libs
# libmpl_la_CXXFLAGS = $(LIBICONV_CFLAGS)
libmpl_la_LIBADD = -lpthread

libmpl_la_SOURCES =				\
mapplication.cpp				\
moptions.cpp					\
mdatetime.cpp					\
mdir.cpp					\
mfile.cpp					\
mfileinfo.cpp					\
mplstd.cpp					\
mhostaddress.cpp				\
mhttp.cpp					\
mlog.cpp					\
mmongodb.cpp					\
mprocess.cpp					\
mrand.cpp					\
mlockfile.cpp					\
msettings.cpp					\
msocket.cpp					\
mstring.cpp					\
msysinfo.cpp					\
mthread.cpp					\
mcondition.cpp					\
merror.cpp					\
muuid.cpp					\
murl.cpp					\
muuid_p.h					\
muuid_unix.cpp					\
mhash.cpp					\
mhash_md5.h					\
mhash_md5.c					\
msettings_utf.c					\
mongoose.h					\
mongoose.c					\
mserialport.cpp
