dnl $Id$
dnl config.m4 for extension luxio

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(luxio, for luxio support,
dnl Make sure that the comment is aligned:
dnl [  --with-luxio             Include luxio support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(luxio, whether to enable luxio support,
Make sure that the comment is aligned:
[  --enable-luxio           Enable luxio support])

if test "$PHP_LUXIO" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-luxio -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/luxio"  # you most likely want to change this
  if test -r $PHP_LUXIO/$SEARCH_FOR; then # path given as parameter
    LUXIO_DIR=$PHP_LUXIO
  else # search default path list
    AC_MSG_CHECKING([for luxio files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        LUXIO_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  if test -z "$LUXIO_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the luxio distribution])
  fi

  dnl # --with-luxio -> add include path
  PHP_ADD_INCLUDE($LUXIO_DIR/include)

  dnl # --with-luxio -> check for lib and symbol presence
  dnl LIBNAME=luxio # you may want to change this
  dnl LIBSYMBOL=luxio # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $LUXIO_DIR/lib, LUXIO_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_LUXIOLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong luxio lib version or lib not found])
  dnl ],[
  dnl   -L$LUXIO_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(LUXIO_SHARED_LIBADD)

  CC=/usr/bin/g++
  CXX=/usr/bin/g++
  PHP_NEW_EXTENSION(luxio, luxio.cpp, $ext_shared)
fi
