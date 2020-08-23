# Resources
BOOST_HOME                  = /opt/cpp_packages/boost/boost_1_72_0

# Compiler
CC                          = g++
AR                          = ar rcs
RANLIB                      = ranlib
NOLINK                      = -c
CFLAGS                      = -std=c++11 -Wall
SYSINCLUDE                  = -isystem $(BOOST_HOME)
COMPILE                     = $(CC) $(NOLINK) $(CFLAGS) $(SYSINCLUDE)

# Directories
SRC_DIR                     = src/Sources
GENERICIO_SRC_DIR           = $(SRC_DIR)/GenericIO
NETWORK_SRC_DIR             = $(SRC_DIR)/Network
HEAD_DIR                    = src/Headers
GENERICIO_HEAD_DIR          = $(HEAD_DIR)/GenericIO
NETWORK_HEAD_DIR            = $(HEAD_DIR)/Network
OBJ_DIR                     = obj
GENERICIO_OBJ_DIR           = $(OBJ_DIR)/GenericIO
NETWORK_OBJ_DIR             = $(OBJ_DIR)/Network
LIB_DIR                     = lib
BIN_DIR                     = bin

# Files
MAIN                        = $(BIN_DIR)/main
DRIVER_O                    = $(OBJ_DIR)/driver.o
DRIVER_CPP                  = $(SRC_DIR)/driver.cpp
GENERICIO_A                 = $(LIB_DIR)/GenericIO.a
NETWORK_A                   = $(LIB_DIR)/libNetwork.a
GENERICIO_A_OBJ             = $(GENERICIO_OBJ_DIR)/GenericStreamBuf.o\
                                $(GENERICIO_OBJ_DIR)/GenericIStreamBuf.o
GENERICIO_A_SRC             = $(GENERICIO_SRC_DIR)/GenericStreamBuf.cpp\
                                $(GENERICIO_SRC_DIR)/GenericIStreamBuf.cpp
NETWORK_A_OBJ               = $(NETWORK_OBJ_DIR)/HttpByteContent.o\
                                $(NETWORK_OBJ_DIR)/HttpContent.o\
                                $(NETWORK_OBJ_DIR)/HttpFileStreamContent.o\
                                $(NETWORK_OBJ_DIR)/HttpRequestMessage.o\
                                $(NETWORK_OBJ_DIR)/HttpResponseMessage.o\
                                $(NETWORK_OBJ_DIR)/HttpStreamContent.o\
                                $(NETWORK_OBJ_DIR)/HttpStringContent.o\
                                $(NETWORK_OBJ_DIR)/WebClient.o
NETWORK_A_SRC               = $(NETWORK_SRC_DIR)/HttpByteContent.cpp\
                                $(NETWORK_SRC_DIR)/HttpContent.cpp\
                                $(NETWORK_SRC_DIR)/HttpFileStreamContent.cpp\
                                $(NETWORK_SRC_DIR)/HttpRequestMessage.cpp\
                                $(NETWORK_SRC_DIR)/HttpResponseMessage.cpp\
                                $(NETWORK_SRC_DIR)/HttpStreamContent.cpp\
                                $(NETWORK_SRC_DIR)/HttpStringContent.cpp\
                                $(NETWORK_SRC_DIR)/WebClient.cpp


main: $(MAIN)

# Executables
$(MAIN): $(DRIVER_O) $(NETWORK_A) $(GENERICIO_A) | $(BIN_DIR)
	$(CC) -L/usr/local/lib/boost/ $^ -lpthread -lcrypto -lssl -o $@

# Libraries
$(NETWORK_A): $(NETWORK_A_OBJ) | $(LIB_DIR)
	$(AR) $@ $^
$(GENERICIO_A): $(GENERICIO_A_OBJ) | $(LIB_DIR)
	${AR} $@ $^

# Object files
$(DRIVER_O): $(DRIVER_CPP) | $(OBJ_DIR)
	$(COMPILE) -I $(HEAD_DIR) -I $(GENERICIO_HEAD_DIR) -I $(NETWORK_HEAD_DIR) $(DRIVER_CPP) -o $@
$(GENERICIO_A_OBJ): $(GENERICIO_A_SRC) | $(GENERICIO_OBJ_DIR)
	for source in $^; do\
	    echo $(COMPILE) -I $(GENERICIO_HEAD_DIR) -o $(GENERICIO_OBJ_DIR)/$$(basename $${source%.*}).o $$source;\
	    $(COMPILE) -I $(GENERICIO_HEAD_DIR) -o $(GENERICIO_OBJ_DIR)/$$(basename $${source%.*}).o $$source;\
	done
$(NETWORK_A_OBJ): $(NETWORK_A_SRC) | $(NETWORK_OBJ_DIR)
	for source in $^; do\
	    echo $(COMPILE) -I $(NETWORK_HEAD_DIR) -I $(GENERICIO_HEAD_DIR) -o $(NETWORK_OBJ_DIR)/$$(basename $${source%.*}).o $$source;\
	    $(COMPILE) -I $(NETWORK_HEAD_DIR) -I $(GENERICIO_HEAD_DIR) -o $(NETWORK_OBJ_DIR)/$$(basename $${source%.*}).o $$source;\
	done


$(GENERICIO_OBJ_DIR):
	mkdir -p $@
$(NETWORK_OBJ_DIR):
	mkdir -p $@

$(LIB_DIR):
	mkdir $@

$(BIN_DIR):
	mkdir $@


clean:
	rm -f $(GENERICIO_OBJ_DIR)/*.o $(NETWORK_OBJ_DIR)/*.o obj/*.o lib/* bin/*





#LEGACY
#STDAFX_H                    = $(HEAD_DIR)/stdafx.h
#HTTPCONTENT_H               = $(HEAD_DIR)/HttpContent.h
#HTTPCONTENT_CPP             = $(SRC_DIR)/HttpContent.cpp
#HTTPCONTENT_O               = $(OBJ_DIR)/HttpContent.o
#HTTPBYTECONTENT_H           = $(HEAD_DIR)/HttpByteContent.h
#HTTPBYTECONTENT_CPP         = $(SRC_DIR)/HttpByteContent.cpp
#HTTPBYTECONTENT_O           = $(OBJ_DIR)/HttpByteContent.o
#HTTPFILESTREAMCONTENT_H     = $(HEAD_DIR)/HttpFileStreamContent.h
#HTTPFILESTREAMCONTENT_CPP   = $(SRC_DIR)/HttpFileStreamContent.cpp
#HTTPFILESTREAMCONTENT_O     = $(OBJ_DIR)/HttpFileStreamContent.o
#HTTPSTRINGCONTENT_H         = $(HEAD_DIR)/HttpStringContent.h
#HTTPSTRINGCONTENT_CPP       = $(SRC_DIR)/HttpStringContent.cpp
#HTTPSTRINGCONTENT_O         = $(OBJ_DIR)/HttpStringContent.o
#HTTPREQUESTMESSAGE_H        = $(HEAD_DIR)/HttpRequestMessage.h
#HTTPREQUESTMESSAGE_CPP      = $(SRC_DIR)/HttpRequestMessage.cpp
#HTTPREQUESTMESSAGE_O        = $(OBJ_DIR)/HttpRequestMessage.o
#HTTPRESPONSEMESSAGE_H       = $(HEAD_DIR)/HttpResponseMessage.h
#HTTPRESPONSEMESSAGE_CPP     = $(SRC_DIR)/HttpResponseMessage.cpp
#HTTPRESPONSEMESSAGE_O       = $(OBJ_DIR)/HttpResponseMessage.o
#WEBCLIENT_H                 = $(HEAD_DIR)/WebClient.h
#WEBCLIENT_CPP               = $(SRC_DIR)/WebClient.cpp
#WEBCLIENT_O                 = $(OBJ_DIR)/WebClient.o
#$(NETWORK_A): $(HTTPCONTENT_O) $(HTTPBYTECONTENT_O) $(HTTPFILESTREAMCONTENT_O) $(HTTPSTRINGCONTENT_O) $(HTTPREQUESTMESSAGE_O) $(HTTPRESPONSEMESSAGE_O) $(WEBCLIENT_O) | $(LIB_DIR)
#$(DRIVER_O): $(DRIVER_CPP) | $(OBJ_DIR)
#	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(DRIVER_CPP) -o $@

#$(HTTPCONTENT_O): $(HTTPCONTENT_H) $(HTTPCONTENT_CPP) | $(OBJ_DIR)
#	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPCONTENT_CPP) -o $@

#$(HTTPBYTECONTENT_O): $(HTTPBYTECONTENT_H) $(HTTPBYTECONTENT_CPP) | $(OBJ_DIR)
#	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPBYTECONTENT_CPP) -o $@

#$(HTTPSTRINGCONTENT_O): $(HTTPSTRINGCONTENT_H) $(HTTPSTRINGCONTENT_CPP) | $(OBJ_DIR)
#	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPSTRINGCONTENT_CPP) -o $@

#$(HTTPFILESTREAMCONTENT_O): $(HTTPFILESTREAMCONTENT_H) $(HTTPFILESTREAMCONTENT_CPP) | $(OBJ_DIR)
#	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPFILESTREAMCONTENT_CPP) -o $@

#$(HTTPREQUESTMESSAGE_O): $(HTTPREQUESTMESSAGE_H) $(HTTPREQUESTMESSAGE_CPP) | $(OBJ_DIR)
#	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPREQUESTMESSAGE_CPP) -o $@

#$(HTTPRESPONSEMESSAGE_O): $(HTTPRESPONSEMESSAGE_H) $(HTTPRESPONSEMESSAGE_CPP) | $(OBJ_DIR)
#	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(HTTPRESPONSEMESSAGE_CPP) -o $@

#$(WEBCLIENT_O): $(WEBCLIENT_H) $(WEBCLIENT_CPP) | $(OBJ_DIR)
#	$(CC) $(NOLINK) $(CFLAGS) -I$(HEAD_DIR) $(WEBCLIENT_CPP) -o $@
