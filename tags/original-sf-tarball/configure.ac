dnl Process this file with autoconf to produce a configure script.
AC_INIT(src/main.c)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(sdlroads, 0.1)

AM_CONFIG_HEADER(src/config.h)

dnl Checks for programs.
AC_PROG_CC

dnl Checks for header files.
AC_HEADER_STDC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Check for headers
AC_CHECK_HEADERS([jpeglib.h])

dnl Checks for libraries.
AC_CHECK_LIB([GL], [glBegin],, AC_MSG_ERROR([Error! You do not have OpenGL
installed. You will not be able to play this game.]))

AC_CHECK_LIB([GLU], [gluLookAt],, AC_MSG_ERROR([Error! You do not have OpenGL
installed. You will not be able to play this game.]))

AC_CHECK_LIB([jpeg], [jpeg_start_compress],, AC_MSG_ERROR([libjpeg not found!]))

AC_CHECK_LIB([SDL_mixer], [Mix_LoadMUS],, AC_MSG_ERROR([Error! You need
SDL_mixer. Check out http://www.libsdl.org/projects/SDL_mixer/index.html for
more info.]))

AC_CHECK_LIB([SDL_ttf], [TTF_OpenFont],, AC_MSG_ERROR([Error! You need SDL_ttf.
Check out http://www.libsdl.org/projects/SDL_ttf/ for more info.]))

AC_CHECK_LIB([SDL_image], [IMG_Load],, AC_MSG_ERROR([SDL_image not found]))

dnl Check for SDL
SDL_VERSION=1.2.4
AM_PATH_SDL($SDL_VERSION,
           :,
           AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
			)
dnl CFLAGS="$CFLAGS $SDL_CFLAGS"
CFLAGS="$CFLAGS $SDL_CFLAGS -DPATH_PREFIX=\\\"\$(datadir)/\$(PACKAGE)/\\\""
LIBS="$LIBS $SDL_LIBS"

dnl Checks for library functions.

AC_OUTPUT([Makefile src/Makefile images/Makefile tracks/Makefile tracks/skyroads/Makefile
sound/Makefile models/Makefile fonts/Makefile])
