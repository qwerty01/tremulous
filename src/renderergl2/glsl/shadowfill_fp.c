const char *fallbackShader_shadowfill_fp = "\x75\x6e\x69\x66\x6f\x72\x6d\x20\x76\x65\x63\x34\x20\x20\x75\x5f\x4c\x69\x67\x68\x74\x4f\x72\x69\x67\x69\x6e\x3b\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x66\x6c\x6f\x61\x74\x20\x75\x5f\x4c\x69\x67\x68\x74\x52\x61\x64\x69\x75\x73\x3b\x0a\x0a\x76\x61\x72\x79\x69\x6e\x67\x20\x76\x65\x63\x33\x20\x20\x76\x61\x72\x5f\x50\x6f\x73\x69\x74\x69\x6f\x6e\x3b\x0a\x0a\x76\x6f\x69\x64\x20\x6d\x61\x69\x6e\x28\x29\x0a\x7b\x0a\x23\x69\x66\x20\x64\x65\x66\x69\x6e\x65\x64\x28\x55\x53\x45\x5f\x44\x45\x50\x54\x48\x29\x0a\x09\x66\x6c\x6f\x61\x74\x20\x64\x65\x70\x74\x68\x20\x3d\x20\x6c\x65\x6e\x67\x74\x68\x28\x75\x5f\x4c\x69\x67\x68\x74\x4f\x72\x69\x67\x69\x6e\x2e\x78\x79\x7a\x20\x2d\x20\x76\x61\x72\x5f\x50\x6f\x73\x69\x74\x69\x6f\x6e\x29\x20\x2f\x20\x75\x5f\x4c\x69\x67\x68\x74\x52\x61\x64\x69\x75\x73\x3b\x0a\x20\x23\x69\x66\x20\x30\x0a\x09\x2f\x2f\x20\x33\x32\x20\x62\x69\x74\x20\x70\x72\x65\x63\x69\x73\x69\x6f\x6e\x0a\x09\x63\x6f\x6e\x73\x74\x20\x76\x65\x63\x34\x20\x62\x69\x74\x53\x68\x20\x3d\x20\x76\x65\x63\x34\x28\x20\x32\x35\x36\x20\x2a\x20\x32\x35\x36\x20\x2a\x20\x32\x35\x36\x2c\x20\x20\x20\x32\x35\x36\x20\x2a\x20\x32\x35\x36\x2c\x20\x20\x20\x20\x20\x20\x20\x20\x20\x32\x35\x36\x2c\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x31\x29\x3b\x0a\x09\x63\x6f\x6e\x73\x74\x20\x76\x65\x63\x34\x20\x62\x69\x74\x4d\x73\x6b\x20\x3d\x20\x76\x65\x63\x34\x28\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x30\x2c\x20\x31\x2e\x30\x20\x2f\x20\x32\x35\x36\x2e\x30\x2c\x20\x31\x2e\x30\x20\x2f\x20\x32\x35\x36\x2e\x30\x2c\x20\x31\x2e\x30\x20\x2f\x20\x32\x35\x36\x2e\x30\x29\x3b\x0a\x09\x0a\x09\x76\x65\x63\x34\x20\x63\x6f\x6d\x70\x3b\x0a\x09\x63\x6f\x6d\x70\x20\x3d\x20\x64\x65\x70\x74\x68\x20\x2a\x20\x62\x69\x74\x53\x68\x3b\x0a\x09\x63\x6f\x6d\x70\x2e\x78\x79\x7a\x20\x3d\x20\x66\x72\x61\x63\x74\x28\x63\x6f\x6d\x70\x2e\x78\x79\x7a\x29\x3b\x0a\x09\x63\x6f\x6d\x70\x20\x2d\x3d\x20\x63\x6f\x6d\x70\x2e\x78\x78\x79\x7a\x20\x2a\x20\x62\x69\x74\x4d\x73\x6b\x3b\x0a\x09\x67\x6c\x5f\x46\x72\x61\x67\x43\x6f\x6c\x6f\x72\x20\x3d\x20\x63\x6f\x6d\x70\x3b\x0a\x20\x23\x65\x6e\x64\x69\x66\x0a\x0a\x20\x23\x69\x66\x20\x31\x0a\x09\x2f\x2f\x20\x32\x34\x20\x62\x69\x74\x20\x70\x72\x65\x63\x69\x73\x69\x6f\x6e\x0a\x09\x63\x6f\x6e\x73\x74\x20\x76\x65\x63\x33\x20\x62\x69\x74\x53\x68\x20\x3d\x20\x76\x65\x63\x33\x28\x20\x32\x35\x36\x20\x2a\x20\x32\x35\x36\x2c\x20\x20\x20\x20\x20\x20\x20\x20\x20\x32\x35\x36\x2c\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x31\x29\x3b\x0a\x09\x63\x6f\x6e\x73\x74\x20\x76\x65\x63\x33\x20\x62\x69\x74\x4d\x73\x6b\x20\x3d\x20\x76\x65\x63\x33\x28\x20\x20\x20\x20\x20\x20\x20\x20\x30\x2c\x20\x31\x2e\x30\x20\x2f\x20\x32\x35\x36\x2e\x30\x2c\x20\x31\x2e\x30\x20\x2f\x20\x32\x35\x36\x2e\x30\x29\x3b\x0a\x09\x0a\x09\x76\x65\x63\x33\x20\x63\x6f\x6d\x70\x3b\x0a\x09\x63\x6f\x6d\x70\x20\x3d\x20\x64\x65\x70\x74\x68\x20\x2a\x20\x62\x69\x74\x53\x68\x3b\x0a\x09\x63\x6f\x6d\x70\x2e\x78\x79\x20\x3d\x20\x66\x72\x61\x63\x74\x28\x63\x6f\x6d\x70\x2e\x78\x79\x29\x3b\x0a\x09\x63\x6f\x6d\x70\x20\x2d\x3d\x20\x63\x6f\x6d\x70\x2e\x78\x78\x79\x20\x2a\x20\x62\x69\x74\x4d\x73\x6b\x3b\x0a\x09\x67\x6c\x5f\x46\x72\x61\x67\x43\x6f\x6c\x6f\x72\x20\x3d\x20\x76\x65\x63\x34\x28\x63\x6f\x6d\x70\x2c\x20\x31\x2e\x30\x29\x3b\x0a\x20\x23\x65\x6e\x64\x69\x66\x0a\x0a\x20\x23\x69\x66\x20\x30\x0a\x09\x2f\x2f\x20\x38\x20\x62\x69\x74\x20\x70\x72\x65\x63\x69\x73\x69\x6f\x6e\x0a\x09\x67\x6c\x5f\x46\x72\x61\x67\x43\x6f\x6c\x6f\x72\x20\x3d\x20\x76\x65\x63\x34\x28\x64\x65\x70\x74\x68\x2c\x20\x64\x65\x70\x74\x68\x2c\x20\x64\x65\x70\x74\x68\x2c\x20\x31\x29\x3b\x0a\x20\x23\x65\x6e\x64\x69\x66\x0a\x23\x65\x6c\x73\x65\x0a\x09\x67\x6c\x5f\x46\x72\x61\x67\x43\x6f\x6c\x6f\x72\x20\x3d\x20\x76\x65\x63\x34\x28\x30\x2c\x20\x30\x2c\x20\x30\x2c\x20\x31\x29\x3b\x0a\x23\x65\x6e\x64\x69\x66\x0a\x7d\x0a";
