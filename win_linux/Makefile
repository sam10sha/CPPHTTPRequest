# Compiler
CC                          = g++
AR                          = ar rcs
RANLIB                      = ranlib
NOLINK                      = -c
CFLAGS                      = -std=c++11 -Wall
LIBPATH                     = -L/usr/local/lib/boost/ -Llib/

# Directories
SRC_DIR                     = src/Sources
HEAD_DIR                    = src/Headers
OBJ_DIR                     = obj
LIB_DIR                     = lib
BIN_DIR                     = bin

# Files
NETWORK_A                   = $(LIB_DIR)/libNetwork.a
STDAFX_H                    = $(HEAD_DIR)/stdafx.h
DRIVER_CPP                  = $(SRC_DIR)/driver.cpp
DRIVER_O                    = $(OBJ_DIR)/driver.o
HTTPCONTENT_H               = $(HEAD_DIR)/HttpContent.h
HTTPCONTENT_CPP             = $(SRC_DIR)/HttpContent.cpp
HTTPCONTENT_O               = $(OBJ_DIR)/HttpContent.o
HTTPBYTECONTENT_H           = $(HEAD_DIR)/HttpByteContent.h
HTTPBYTECONTENT_CPP         = $(SRC_DIR)/HttpByteContent.cpp
HTTPBYTECONTENT_O           = $(OBJ_DIR)/HttpByteContent.o
HTTPFILESTREAMCONTENT_H     = $(HEAD_DIR)/HttpFileStreamContent.h
HTTPFILESTREAMCONTENT_CPP   = $(SRC_DIR)/HttpFileStreamContent.cpp
HTTPFILESTREAMCONTENT_O     = $(OBJ_DIR)/HttpFileStreamContent.o
HTTPSTRINGCONTENT_H         = $(HEAD_DIR)/HttpStringContent.h
HTTPSTRINGCONTENT_CPP       = $(SRC_DIR)/HttpStringContent.cpp
HTTPSTRINGCONTENT_O         = $(OBJ_DIR)/HttpStringContent.o
HTTPREQUESTMESSAGE_H        = $(HEAD_DIR)/HttpRequestMessage.h
HTTPREQUESTMESSAGE_CPP      = $(SRC_DIR)/HttpRequestMessage.cpp
HTTPREQUESTMESSAGE_O        = $(OBJ_DIR)/HttpRequestMessage.o
HTTPRESPONSEMESSAGE_H       = $(HEAD_DIR)/HttpResponseMessage.h
HTTPRESPONSEMESSAGE_CPP     = $(SRC_DIR)/HttpResponseMessage.cpp
HTTPRESPONSEMESSAGE_O       = $(OBJ_DIR)/HttpResponseMessage.o
WEBCLIENT_H                 = $(HEAD_DIR)/WebClient.h
WEBCLIENT_CPP               = $(SRC_DIR)/WebClient.cpp
WEBCLIENT_O                 = $(OBJ_DIR)/WebClient.o
MAIN                        = $(BIN_DIR)/main


main: $(MAIN)

# Executables
$(MAIN): $(DRIVER_O) $(NETWORK_A) | $(BIN_DIR)
	$(CC) $(LIBPATH) $^ -lpthread -o $@

# Libraries
$(NETWORK_A): $(HTTPCONTENT_O) $(HTTPBYTECONTENT_O) $(HTTPFILESTREAMCONTENT_O) $(HTTPSTRINGCONTENT_O) $(HTTPREQUESTMESSAGE_O) $(HTTPRESPONSEMESSAGE_O) $(WEBCLIENT_O) | $(LIB_DIR)
	$(AR) $@ $^

# Object files
$(DRIVER_O): $(DRIVER_CPP) | $(OBJ_DIR)
	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(DRIVER_CPP) -o $@

$(HTTPCONTENT_O): $(HTTPCONTENT_H) $(HTTPCONTENT_CPP) | $(OBJ_DIR)
	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPCONTENT_CPP) -o $@

$(HTTPBYTECONTENT_O): $(HTTPBYTECONTENT_H) $(HTTPBYTECONTENT_CPP) | $(OBJ_DIR)
	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPBYTECONTENT_CPP) -o $@

$(HTTPSTRINGCONTENT_O): $(HTTPSTRINGCONTENT_H) $(HTTPSTRINGCONTENT_CPP) | $(OBJ_DIR)
	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPSTRINGCONTENT_CPP) -o $@

$(HTTPFILESTREAMCONTENT_O): $(HTTPFILESTREAMCONTENT_H) $(HTTPFILESTREAMCONTENT_CPP) | $(OBJ_DIR)
	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPFILESTREAMCONTENT_CPP) -o $@

$(HTTPREQUESTMESSAGE_O): $(HTTPREQUESTMESSAGE_H) $(HTTPREQUESTMESSAGE_CPP) | $(OBJ_DIR)
	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPREQUESTMESSAGE_CPP) -o $@

$(HTTPRESPONSEMESSAGE_O): $(HTTPRESPONSEMESSAGE_H) $(HTTPRESPONSEMESSAGE_CPP) | $(OBJ_DIR)
	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPRESPONSEMESSAGE_CPP) -o $@

$(WEBCLIENT_O): $(WEBCLIENT_H) $(WEBCLIENT_CPP) | $(OBJ_DIR)
	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(WEBCLIENT_CPP) -o $@
	


$(OBJ_DIR):
	mkdir $@

$(LIB_DIR):
	mkdir $@

$(BIN_DIR):
	mkdir $@


clean:
	rm -f obj/* lib/* bin/*


