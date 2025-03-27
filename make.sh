#!/bin/bash

# Paths
SAXON_DIR=/usr/local/lib/libsaxon-HEC-linux-amd64-v12.5.0
SAXON_API_DIR=${SAXON_DIR}/Saxon.C.API
SAXON_LIBS_DIR=${SAXON_DIR}/libs/nix

# Compile flags (taken from samples/cppTests/build64-linux.sh)
C_FLAGS="-g -fPIC -I${SAXON_API_DIR}/graalvm"
CPP_FLAGS="-std=c++17 -g -fPIC -I${SAXON_API_DIR} -I${SAXON_API_DIR}/graalvm"
LINK_FLAGS="-pthread -Wl,-rpath,${SAXON_LIBS_DIR} -L${SAXON_LIBS_DIR} -ldl -lc -lsaxon-hec-12.5.0"
ERROR_FLAGS="-Wno-error=unused-value -Wno-error=reorder -Wno-error=sign-compare -Wno-error=unused-variable -Wno-error=unused-parameter"

BIN_DIR="$HOME/saxon_bin"
rm -rf "$BIN_DIR" && mkdir -p "$BIN_DIR"

# Compile C files (taken from samples/cppTests/build64-linux.sh)
gcc ${C_FLAGS} -c ${SAXON_API_DIR}/SaxonCGlue.c -o "$BIN_DIR/SaxonCGlue.o"
gcc ${C_FLAGS} -c ${SAXON_API_DIR}/SaxonCXPath.c -o "$BIN_DIR/SaxonCXPath.o"

# cpp files needed (taken from samples/cppTests/build64-linux.sh)
CPP_FILES=(
  "XdmAtomicValue" "XdmArray" "XdmFunctionItem" "XdmItem" "XdmMap" "XdmNode" "XdmValue"
  "SaxonProcessor" "SaxonApiException" "Xslt30Processor" "XsltExecutable"
  "DocumentBuilder" "XPathProcessor" "XQueryProcessor" "SchemaValidator"
)

# Compile cpp files
for file in "${CPP_FILES[@]}"; do
  g++ ${CPP_FLAGS} -c ${SAXON_API_DIR}/${file}.cpp -o "$BIN_DIR/${file}.o"
done

# Compile Tim's program
g++ ${CPP_FLAGS} -c cpp/process_xml.cpp -o "$BIN_DIR/process_xml.o"

# Link object files
g++ ${CPP_FLAGS} -o "$BIN_DIR/process_xml" "$BIN_DIR"/*.o ${LINK_FLAGS} ${ERROR_FLAGS}
