#!/bin/bash
set -e

CC="gcc"
CFLAGS="-Wall -Wextra -std=c11 -I./src"
BIN_DIR="bin"
SRC_DIR="src"
OBJ_DIR="$BIN_DIR/obj"

echo "Creating directories..."
mkdir -p $BIN_DIR
mkdir -p $OBJ_DIR/core
mkdir -p $OBJ_DIR/ui
mkdir -p $OBJ_DIR/storage
mkdir -p $OBJ_DIR/utils
mkdir -p data/forms
mkdir -p data/records
mkdir -p templates
mkdir -p exports
mkdir -p backups

CORE_SRC="$SRC_DIR/core/field.c $SRC_DIR/core/form.c $SRC_DIR/core/record.c"
UI_SRC="$SRC_DIR/ui/form_builder.c $SRC_DIR/ui/data_entry.c"
STORAGE_SRC="$SRC_DIR/storage/csv_handler.c"
UTILS_SRC="$SRC_DIR/utils/string_utils.c $SRC_DIR/utils/input_utils.c"
MAIN_SRC="$SRC_DIR/main.c"

# Explicitly list object files for clarity and control
ALL_OBJ="$OBJ_DIR/core/field.o $OBJ_DIR/core/form.o $OBJ_DIR/core/record.o \
         $OBJ_DIR/ui/form_builder.o $OBJ_DIR/ui/data_entry.o \
         $OBJ_DIR/storage/csv_handler.o \
         $OBJ_DIR/utils/string_utils.o $OBJ_DIR/utils/input_utils.o \
         $OBJ_DIR/main.o"

echo "Compiling source files..."
$CC $CFLAGS -c $SRC_DIR/core/field.c -o $OBJ_DIR/core/field.o
$CC $CFLAGS -c $SRC_DIR/core/form.c -o $OBJ_DIR/core/form.o
$CC $CFLAGS -c $SRC_DIR/core/record.c -o $OBJ_DIR/core/record.o

$CC $CFLAGS -c $SRC_DIR/ui/form_builder.c -o $OBJ_DIR/ui/form_builder.o
$CC $CFLAGS -c $SRC_DIR/ui/data_entry.c -o $OBJ_DIR/ui/data_entry.o

$CC $CFLAGS -c $SRC_DIR/storage/csv_handler.c -o $OBJ_DIR/storage/csv_handler.o

$CC $CFLAGS -c $SRC_DIR/utils/string_utils.c -o $OBJ_DIR/utils/string_utils.o
$CC $CFLAGS -c $SRC_DIR/utils/input_utils.c -o $OBJ_DIR/utils/input_utils.o

$CC $CFLAGS -c $MAIN_SRC -o $OBJ_DIR/main.o

echo "Linking executable..."
$CC $CFLAGS $ALL_OBJ -o $BIN_DIR/formdb

echo "Build successful! You can run the application with ./bin/formdb"
