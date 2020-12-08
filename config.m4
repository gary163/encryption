dnl config.m4 for extension encryption 
dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(encryption, for encryption support,
dnl Make sure that the comment is aligned:
dnl [  --with-encryption             Include encryption support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(encryption, whether to enable encryption support,
dnl Make sure that the comment is aligned:
[  --enable-encryption          Enable encryption support], no)

dnl PHP_ADD_LIBRARY_WITH_PATH(yiencrypt, /usr/local/lib/, ENCRYPTION_SHARED_LIBADD)
dnl PHP_SUBST(ENCRYPTION_SHARED_LIBADD)

if test "$PHP_ENCRYPTION" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, ENCRYPTION_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-encryption -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/encryption.h"  # you most likely want to change this
  dnl if test -r $PHP_ENCRYPTION/$SEARCH_FOR; then # path given as parameter
  dnl   ENCRYPTION_DIR=$PHP_ENCRYPTION
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for encryption files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ENCRYPTION_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ENCRYPTION_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the encryption distribution])
  dnl fi

  dnl # --with-encryption -> add include path
  dnl PHP_ADD_INCLUDE($ENCRYPTION_DIR/include)

  dnl # --with-encryption -> check for lib and symbol presence
  dnl LIBNAME=ENCRYPTION # you may want to change this
  dnl LIBSYMBOL=ENCRYPTION # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ENCRYPTION_DIR/$PHP_LIBDIR, ENCRYPTION_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ENCRYPTIONLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong encryption lib version or lib not found])
  dnl ],[
  dnl   -L$ENCRYPTION_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ENCRYPTION_SHARED_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_ENCRYPTION, 1, [ Have encryption support ])
  source_file="encryption.c\
        hmac_sha1.c\
        base64.c"

  PHP_NEW_EXTENSION(encryption, $source_file, $ext_shared)
fi
