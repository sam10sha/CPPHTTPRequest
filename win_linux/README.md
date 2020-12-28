FOR WINDOWS
Set the following environment variables:
    BOOST_HOME
    OPENSSL_HOME
    CPPHTTPREQUEST_HOME
Set include directories:
    $(BOOST_HOME)
    $(OPENSSL_HOME)/include
    $(CPPHTTPREQUEST_HOME)/src/Headers/GenericIO
    $(CPPHTTPREQUEST_HOME)/src/Headers/Network
Link following libraries:
    Network.lib
    GenericIO.lib
    libssl.lib
    libcrypto.lib
NOTE: The order of the linked libraries may impact your build.


FOR LINUX
Set the following environment variables:
    BOOST_HOME
    CPPHTTPREQUEST_HOME
Set include directories:
    $(BOOST_HOME)
    $(CPPHTTPREQUEST_HOME)/src/Headers/GenericIO
    $(CPPHTTPREQUEST_HOME)/src/Headers/Network
Link following libraries:
    libNetwork.a | libNetwork.so [flag: -lNetwork]
    libGenericIO.a | libGenericIO.so [flag: -lGenericIO]
    libpthread.a [flag: -lpthread]
    libssl.a [flag: -lssl]
    libcrypto.a [flag: -lcrypto]
NOTE: The order of the linked libraries may impact your build.
      You may have to use 'readelf' and 'patchelf' in order to modify
      the rpath (r[un]path) for the binaries.


FOR MACOS
Set the following environment variables:
    BOOST_HOME
    OPENSSL_HOME
    CPPHTTPREQUEST_HOME
Set include directories:
    $(BOOST_HOME)
    $(OPENSSL_HOME)/include
    $(CPPHTTPREQUEST_HOME)/src/Headers/GenericIO
    $(CPPHTTPREQUEST_HOME)/src/Headers/Network
Link following libraries:
    libNetwork.a | libNetwork.so [flag: -lNetwork]
    libGenericIO.a | libGenericIO.dylib [flag: -lGenericIO]
    libpthread.a | libpthread.dylib [flag: -lpthread]
    libssl.a | libssl.dylib [flag: -lssl]
    libcrypto.a | libcrypto.dylib [flag: -lcrypto]
NOTE: The order of the linked libraries may impact your build.
      You may have to use 'otool' and 'install_name_tool' in order to modify
      the rpath (r[un]path) for the binaries.
