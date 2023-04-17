const char *fallbackShader_tonemap_fp = "\x75\x6e\x69\x66\x6f\x72\x6d\x20\x73\x61\x6d\x70\x6c\x65\x72\x32\x44\x20\x75\x5f\x54\x65\x78\x74\x75\x72\x65\x4d\x61\x70\x3b\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x73\x61\x6d\x70\x6c\x65\x72\x32\x44\x20\x75\x5f\x4c\x65\x76\x65\x6c\x73\x4d\x61\x70\x3b\x0a\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x76\x65\x63\x34\x20\x20\x20\x20\x20\x20\x75\x5f\x43\x6f\x6c\x6f\x72\x3b\x0a\x0a\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x76\x65\x63\x32\x20\x20\x20\x20\x20\x20\x75\x5f\x41\x75\x74\x6f\x45\x78\x70\x6f\x73\x75\x72\x65\x4d\x69\x6e\x4d\x61\x78\x3b\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x76\x65\x63\x33\x20\x20\x20\x20\x20\x20\x75\x5f\x54\x6f\x6e\x65\x4d\x69\x6e\x41\x76\x67\x4d\x61\x78\x4c\x69\x6e\x65\x61\x72\x3b\x0a\x0a\x76\x61\x72\x79\x69\x6e\x67\x20\x76\x65\x63\x32\x20\x20\x20\x20\x20\x20\x76\x61\x72\x5f\x54\x65\x78\x43\x6f\x6f\x72\x64\x73\x3b\x0a\x76\x61\x72\x79\x69\x6e\x67\x20\x66\x6c\x6f\x61\x74\x20\x20\x20\x20\x20\x76\x61\x72\x5f\x49\x6e\x76\x57\x68\x69\x74\x65\x3b\x0a\x0a\x63\x6f\x6e\x73\x74\x20\x76\x65\x63\x33\x20\x20\x4c\x55\x4d\x49\x4e\x41\x4e\x43\x45\x5f\x56\x45\x43\x54\x4f\x52\x20\x3d\x20\x20\x20\x76\x65\x63\x33\x28\x30\x2e\x32\x31\x32\x35\x2c\x20\x30\x2e\x37\x31\x35\x34\x2c\x20\x30\x2e\x30\x37\x32\x31\x29\x3b\x20\x2f\x2f\x76\x65\x63\x33\x28\x30\x2e\x32\x39\x39\x2c\x20\x30\x2e\x35\x38\x37\x2c\x20\x30\x2e\x31\x31\x34\x29\x3b\x0a\x0a\x66\x6c\x6f\x61\x74\x20\x46\x69\x6c\x6d\x69\x63\x54\x6f\x6e\x65\x6d\x61\x70\x28\x66\x6c\x6f\x61\x74\x20\x78\x29\x0a\x7b\x0a\x09\x63\x6f\x6e\x73\x74\x20\x66\x6c\x6f\x61\x74\x20\x53\x53\x20\x20\x3d\x20\x30\x2e\x32\x32\x3b\x20\x2f\x2f\x20\x53\x68\x6f\x75\x6c\x64\x65\x72\x20\x53\x74\x72\x65\x6e\x67\x74\x68\x0a\x09\x63\x6f\x6e\x73\x74\x20\x66\x6c\x6f\x61\x74\x20\x4c\x53\x20\x20\x3d\x20\x30\x2e\x33\x30\x3b\x20\x2f\x2f\x20\x4c\x69\x6e\x65\x61\x72\x20\x53\x74\x72\x65\x6e\x67\x74\x68\x0a\x09\x63\x6f\x6e\x73\x74\x20\x66\x6c\x6f\x61\x74\x20\x4c\x41\x20\x20\x3d\x20\x30\x2e\x31\x30\x3b\x20\x2f\x2f\x20\x4c\x69\x6e\x65\x61\x72\x20\x41\x6e\x67\x6c\x65\x0a\x09\x63\x6f\x6e\x73\x74\x20\x66\x6c\x6f\x61\x74\x20\x54\x53\x20\x20\x3d\x20\x30\x2e\x32\x30\x3b\x20\x2f\x2f\x20\x54\x6f\x65\x20\x53\x74\x72\x65\x6e\x67\x74\x68\x0a\x09\x63\x6f\x6e\x73\x74\x20\x66\x6c\x6f\x61\x74\x20\x54\x41\x4e\x20\x3d\x20\x30\x2e\x30\x31\x3b\x20\x2f\x2f\x20\x54\x6f\x65\x20\x41\x6e\x67\x6c\x65\x20\x4e\x75\x6d\x65\x72\x61\x74\x6f\x72\x0a\x09\x63\x6f\x6e\x73\x74\x20\x66\x6c\x6f\x61\x74\x20\x54\x41\x44\x20\x3d\x20\x30\x2e\x33\x30\x3b\x20\x2f\x2f\x20\x54\x6f\x65\x20\x41\x6e\x67\x6c\x65\x20\x44\x65\x6e\x6f\x6d\x69\x6e\x61\x74\x6f\x72\x0a\x0a\x09\x72\x65\x74\x75\x72\x6e\x20\x28\x28\x78\x2a\x28\x53\x53\x2a\x78\x2b\x4c\x41\x2a\x4c\x53\x29\x2b\x54\x53\x2a\x54\x41\x4e\x29\x2f\x28\x78\x2a\x28\x53\x53\x2a\x78\x2b\x4c\x53\x29\x2b\x54\x53\x2a\x54\x41\x44\x29\x29\x20\x2d\x20\x54\x41\x4e\x2f\x54\x41\x44\x3b\x0a\x7d\x0a\x0a\x76\x6f\x69\x64\x20\x6d\x61\x69\x6e\x28\x29\x0a\x7b\x0a\x09\x76\x65\x63\x34\x20\x63\x6f\x6c\x6f\x72\x20\x3d\x20\x74\x65\x78\x74\x75\x72\x65\x32\x44\x28\x75\x5f\x54\x65\x78\x74\x75\x72\x65\x4d\x61\x70\x2c\x20\x76\x61\x72\x5f\x54\x65\x78\x43\x6f\x6f\x72\x64\x73\x29\x20\x2a\x20\x75\x5f\x43\x6f\x6c\x6f\x72\x3b\x0a\x0a\x23\x69\x66\x20\x64\x65\x66\x69\x6e\x65\x64\x28\x55\x53\x45\x5f\x50\x42\x52\x29\x0a\x09\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x20\x2a\x3d\x20\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x3b\x0a\x23\x65\x6e\x64\x69\x66\x0a\x0a\x09\x76\x65\x63\x33\x20\x6d\x69\x6e\x41\x76\x67\x4d\x61\x78\x20\x3d\x20\x74\x65\x78\x74\x75\x72\x65\x32\x44\x28\x75\x5f\x4c\x65\x76\x65\x6c\x73\x4d\x61\x70\x2c\x20\x76\x61\x72\x5f\x54\x65\x78\x43\x6f\x6f\x72\x64\x73\x29\x2e\x72\x67\x62\x3b\x0a\x09\x76\x65\x63\x33\x20\x6c\x6f\x67\x4d\x69\x6e\x41\x76\x67\x4d\x61\x78\x4c\x75\x6d\x20\x3d\x20\x63\x6c\x61\x6d\x70\x28\x6d\x69\x6e\x41\x76\x67\x4d\x61\x78\x20\x2a\x20\x32\x30\x2e\x30\x20\x2d\x20\x31\x30\x2e\x30\x2c\x20\x2d\x75\x5f\x41\x75\x74\x6f\x45\x78\x70\x6f\x73\x75\x72\x65\x4d\x69\x6e\x4d\x61\x78\x2e\x79\x2c\x20\x2d\x75\x5f\x41\x75\x74\x6f\x45\x78\x70\x6f\x73\x75\x72\x65\x4d\x69\x6e\x4d\x61\x78\x2e\x78\x29\x3b\x0a\x0a\x09\x66\x6c\x6f\x61\x74\x20\x69\x6e\x76\x41\x76\x67\x4c\x75\x6d\x20\x3d\x20\x75\x5f\x54\x6f\x6e\x65\x4d\x69\x6e\x41\x76\x67\x4d\x61\x78\x4c\x69\x6e\x65\x61\x72\x2e\x79\x20\x2a\x20\x65\x78\x70\x32\x28\x2d\x6c\x6f\x67\x4d\x69\x6e\x41\x76\x67\x4d\x61\x78\x4c\x75\x6d\x2e\x79\x29\x3b\x0a\x0a\x09\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x20\x3d\x20\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x20\x2a\x20\x69\x6e\x76\x41\x76\x67\x4c\x75\x6d\x20\x2d\x20\x75\x5f\x54\x6f\x6e\x65\x4d\x69\x6e\x41\x76\x67\x4d\x61\x78\x4c\x69\x6e\x65\x61\x72\x2e\x78\x78\x78\x3b\x0a\x09\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x20\x3d\x20\x6d\x61\x78\x28\x76\x65\x63\x33\x28\x30\x2e\x30\x29\x2c\x20\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x29\x3b\x0a\x0a\x09\x63\x6f\x6c\x6f\x72\x2e\x72\x20\x3d\x20\x46\x69\x6c\x6d\x69\x63\x54\x6f\x6e\x65\x6d\x61\x70\x28\x63\x6f\x6c\x6f\x72\x2e\x72\x29\x3b\x0a\x09\x63\x6f\x6c\x6f\x72\x2e\x67\x20\x3d\x20\x46\x69\x6c\x6d\x69\x63\x54\x6f\x6e\x65\x6d\x61\x70\x28\x63\x6f\x6c\x6f\x72\x2e\x67\x29\x3b\x0a\x09\x63\x6f\x6c\x6f\x72\x2e\x62\x20\x3d\x20\x46\x69\x6c\x6d\x69\x63\x54\x6f\x6e\x65\x6d\x61\x70\x28\x63\x6f\x6c\x6f\x72\x2e\x62\x29\x3b\x0a\x0a\x09\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x20\x3d\x20\x63\x6c\x61\x6d\x70\x28\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x20\x2a\x20\x76\x61\x72\x5f\x49\x6e\x76\x57\x68\x69\x74\x65\x2c\x20\x30\x2e\x30\x2c\x20\x31\x2e\x30\x29\x3b\x0a\x0a\x23\x69\x66\x20\x64\x65\x66\x69\x6e\x65\x64\x28\x55\x53\x45\x5f\x50\x42\x52\x29\x0a\x09\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x20\x3d\x20\x73\x71\x72\x74\x28\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x29\x3b\x0a\x23\x65\x6e\x64\x69\x66\x0a\x0a\x09\x2f\x2f\x20\x61\x64\x64\x20\x61\x20\x62\x69\x74\x20\x6f\x66\x20\x64\x69\x74\x68\x65\x72\x20\x74\x6f\x20\x72\x65\x64\x75\x63\x65\x20\x62\x61\x6e\x64\x69\x6e\x67\x0a\x09\x63\x6f\x6c\x6f\x72\x2e\x72\x67\x62\x20\x2b\x3d\x20\x76\x65\x63\x33\x28\x31\x2e\x30\x2f\x35\x31\x30\x2e\x30\x20\x2a\x20\x6d\x6f\x64\x28\x67\x6c\x5f\x46\x72\x61\x67\x43\x6f\x6f\x72\x64\x2e\x78\x20\x2b\x20\x67\x6c\x5f\x46\x72\x61\x67\x43\x6f\x6f\x72\x64\x2e\x79\x2c\x20\x32\x2e\x30\x29\x20\x2d\x20\x31\x2e\x30\x2f\x31\x30\x32\x30\x2e\x30\x29\x3b\x0a\x0a\x09\x67\x6c\x5f\x46\x72\x61\x67\x43\x6f\x6c\x6f\x72\x20\x3d\x20\x63\x6f\x6c\x6f\x72\x3b\x0a\x7d\x0a";
