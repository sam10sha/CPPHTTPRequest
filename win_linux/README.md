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
    libssl.lib
    libcrypto.lib
    GenericIO.lib
    Network.lib


FOR LINUX
Set the following environment variables:
    BOOST_HOME
    CPPHTTPREQUEST_HOME
Set include directories:
    $(BOOST_HOME)
    $(CPPHTTPREQUEST_HOME)/src/Headers/GenericIO
    $(CPPHTTPREQUEST_HOME)/src/Headers/Network
Link following libraries:
    libssl.a [flag: -lssl]
    libcrypto.a [flag: -lcrypto]
    libGenericIO.a | libGenericIO.so [flag: -lGenericIO]
    libNetwork.a | libNetwork.so [flag: -lNetwork]
NOTE: You may have to use 'readelf' and 'patchelf' in order to modify
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
    libssl.lib [flag: -lssl]
    libcrypto.lib [flag: -lcrypto]
    libGenericIO.a | libGenericIO.dylib [flag: -lGenericIO]
    libNetwork.a | libNetwork.so [flag: -lNetwork]
NOTE: You may have to use 'otool' and 'install_name_tool' in order to modify
      the rpath (r[un]path) for the binaries.
