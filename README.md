# CPPHTTPRequest

1. Global Include path:
   Add {boost_home_dir}
2. Additional Include paths:
   src/Headers/Network
   src/Headers/GenericIO
   {openssl_home_dir}/include
3. Link:
   Network.lib
   GenericIO.lib
   *libssl_static.lib or libssl.lib[include libssl-3.dll in output]
   *libcrypto_static.lib or libcrypto.lib[include libcrypto-3.dll in output]
   **libboost_date_time-vc*-mt-*-*.lib
   **libboost_regex-vc*-mt-*-*.lib

*  Follow instructions to build openssl
   HINT: Download portable version of Strawberry Perl and portable version of NASM
** Follow instructions to build boost
