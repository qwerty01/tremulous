const char *fallbackShader_shadowfill_vp = "\x61\x74\x74\x72\x69\x62\x75\x74\x65\x20\x76\x65\x63\x33\x20\x20\x61\x74\x74\x72\x5f\x50\x6f\x73\x69\x74\x69\x6f\x6e\x3b\x0a\x61\x74\x74\x72\x69\x62\x75\x74\x65\x20\x76\x65\x63\x33\x20\x20\x61\x74\x74\x72\x5f\x4e\x6f\x72\x6d\x61\x6c\x3b\x0a\x61\x74\x74\x72\x69\x62\x75\x74\x65\x20\x76\x65\x63\x34\x20\x20\x61\x74\x74\x72\x5f\x54\x65\x78\x43\x6f\x6f\x72\x64\x30\x3b\x0a\x0a\x2f\x2f\x23\x69\x66\x20\x64\x65\x66\x69\x6e\x65\x64\x28\x55\x53\x45\x5f\x56\x45\x52\x54\x45\x58\x5f\x41\x4e\x49\x4d\x41\x54\x49\x4f\x4e\x29\x0a\x61\x74\x74\x72\x69\x62\x75\x74\x65\x20\x76\x65\x63\x33\x20\x20\x61\x74\x74\x72\x5f\x50\x6f\x73\x69\x74\x69\x6f\x6e\x32\x3b\x0a\x61\x74\x74\x72\x69\x62\x75\x74\x65\x20\x76\x65\x63\x33\x20\x20\x61\x74\x74\x72\x5f\x4e\x6f\x72\x6d\x61\x6c\x32\x3b\x0a\x2f\x2f\x23\x65\x6e\x64\x69\x66\x0a\x0a\x2f\x2f\x23\x69\x66\x20\x64\x65\x66\x69\x6e\x65\x64\x28\x55\x53\x45\x5f\x44\x45\x46\x4f\x52\x4d\x5f\x56\x45\x52\x54\x45\x58\x45\x53\x29\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x69\x6e\x74\x20\x20\x20\x20\x20\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x3b\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x66\x6c\x6f\x61\x74\x20\x20\x20\x20\x75\x5f\x44\x65\x66\x6f\x72\x6d\x50\x61\x72\x61\x6d\x73\x5b\x35\x5d\x3b\x0a\x2f\x2f\x23\x65\x6e\x64\x69\x66\x0a\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x66\x6c\x6f\x61\x74\x20\x20\x20\x75\x5f\x54\x69\x6d\x65\x3b\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x6d\x61\x74\x34\x20\x20\x20\x20\x75\x5f\x4d\x6f\x64\x65\x6c\x56\x69\x65\x77\x50\x72\x6f\x6a\x65\x63\x74\x69\x6f\x6e\x4d\x61\x74\x72\x69\x78\x3b\x0a\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x6d\x61\x74\x34\x20\x20\x20\x75\x5f\x4d\x6f\x64\x65\x6c\x4d\x61\x74\x72\x69\x78\x3b\x0a\x0a\x2f\x2f\x23\x69\x66\x20\x64\x65\x66\x69\x6e\x65\x64\x28\x55\x53\x45\x5f\x56\x45\x52\x54\x45\x58\x5f\x41\x4e\x49\x4d\x41\x54\x49\x4f\x4e\x29\x0a\x75\x6e\x69\x66\x6f\x72\x6d\x20\x66\x6c\x6f\x61\x74\x20\x20\x20\x75\x5f\x56\x65\x72\x74\x65\x78\x4c\x65\x72\x70\x3b\x0a\x2f\x2f\x23\x65\x6e\x64\x69\x66\x0a\x0a\x76\x61\x72\x79\x69\x6e\x67\x20\x76\x65\x63\x33\x20\x20\x20\x20\x76\x61\x72\x5f\x50\x6f\x73\x69\x74\x69\x6f\x6e\x3b\x0a\x0a\x76\x65\x63\x33\x20\x44\x65\x66\x6f\x72\x6d\x50\x6f\x73\x69\x74\x69\x6f\x6e\x28\x63\x6f\x6e\x73\x74\x20\x76\x65\x63\x33\x20\x70\x6f\x73\x2c\x20\x63\x6f\x6e\x73\x74\x20\x76\x65\x63\x33\x20\x6e\x6f\x72\x6d\x61\x6c\x2c\x20\x63\x6f\x6e\x73\x74\x20\x76\x65\x63\x32\x20\x73\x74\x29\x0a\x7b\x0a\x09\x69\x66\x20\x28\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x20\x3d\x3d\x20\x30\x29\x0a\x09\x7b\x0a\x09\x09\x72\x65\x74\x75\x72\x6e\x20\x70\x6f\x73\x3b\x0a\x09\x7d\x0a\x0a\x09\x66\x6c\x6f\x61\x74\x20\x62\x61\x73\x65\x20\x3d\x20\x20\x20\x20\x20\x20\x75\x5f\x44\x65\x66\x6f\x72\x6d\x50\x61\x72\x61\x6d\x73\x5b\x30\x5d\x3b\x0a\x09\x66\x6c\x6f\x61\x74\x20\x61\x6d\x70\x6c\x69\x74\x75\x64\x65\x20\x3d\x20\x75\x5f\x44\x65\x66\x6f\x72\x6d\x50\x61\x72\x61\x6d\x73\x5b\x31\x5d\x3b\x0a\x09\x66\x6c\x6f\x61\x74\x20\x70\x68\x61\x73\x65\x20\x3d\x20\x20\x20\x20\x20\x75\x5f\x44\x65\x66\x6f\x72\x6d\x50\x61\x72\x61\x6d\x73\x5b\x32\x5d\x3b\x0a\x09\x66\x6c\x6f\x61\x74\x20\x66\x72\x65\x71\x75\x65\x6e\x63\x79\x20\x3d\x20\x75\x5f\x44\x65\x66\x6f\x72\x6d\x50\x61\x72\x61\x6d\x73\x5b\x33\x5d\x3b\x0a\x09\x66\x6c\x6f\x61\x74\x20\x73\x70\x72\x65\x61\x64\x20\x3d\x20\x20\x20\x20\x75\x5f\x44\x65\x66\x6f\x72\x6d\x50\x61\x72\x61\x6d\x73\x5b\x34\x5d\x3b\x0a\x0a\x09\x69\x66\x20\x28\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x20\x3d\x3d\x20\x44\x47\x45\x4e\x5f\x42\x55\x4c\x47\x45\x29\x0a\x09\x7b\x0a\x09\x09\x70\x68\x61\x73\x65\x20\x2a\x3d\x20\x73\x74\x2e\x78\x3b\x0a\x09\x7d\x0a\x09\x65\x6c\x73\x65\x20\x2f\x2f\x20\x69\x66\x20\x28\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x20\x3c\x3d\x20\x44\x47\x45\x4e\x5f\x57\x41\x56\x45\x5f\x49\x4e\x56\x45\x52\x53\x45\x5f\x53\x41\x57\x54\x4f\x4f\x54\x48\x29\x0a\x09\x7b\x0a\x09\x09\x70\x68\x61\x73\x65\x20\x2b\x3d\x20\x64\x6f\x74\x28\x70\x6f\x73\x2e\x78\x79\x7a\x2c\x20\x76\x65\x63\x33\x28\x73\x70\x72\x65\x61\x64\x29\x29\x3b\x0a\x09\x7d\x0a\x0a\x09\x66\x6c\x6f\x61\x74\x20\x76\x61\x6c\x75\x65\x20\x3d\x20\x70\x68\x61\x73\x65\x20\x2b\x20\x28\x75\x5f\x54\x69\x6d\x65\x20\x2a\x20\x66\x72\x65\x71\x75\x65\x6e\x63\x79\x29\x3b\x0a\x09\x66\x6c\x6f\x61\x74\x20\x66\x75\x6e\x63\x3b\x0a\x0a\x09\x69\x66\x20\x28\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x20\x3d\x3d\x20\x44\x47\x45\x4e\x5f\x57\x41\x56\x45\x5f\x53\x49\x4e\x29\x0a\x09\x7b\x0a\x09\x09\x66\x75\x6e\x63\x20\x3d\x20\x73\x69\x6e\x28\x76\x61\x6c\x75\x65\x20\x2a\x20\x32\x2e\x30\x20\x2a\x20\x4d\x5f\x50\x49\x29\x3b\x0a\x09\x7d\x0a\x09\x65\x6c\x73\x65\x20\x69\x66\x20\x28\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x20\x3d\x3d\x20\x44\x47\x45\x4e\x5f\x57\x41\x56\x45\x5f\x53\x51\x55\x41\x52\x45\x29\x0a\x09\x7b\x0a\x09\x09\x66\x75\x6e\x63\x20\x3d\x20\x73\x69\x67\x6e\x28\x30\x2e\x35\x20\x2d\x20\x66\x72\x61\x63\x74\x28\x76\x61\x6c\x75\x65\x29\x29\x3b\x0a\x09\x7d\x0a\x09\x65\x6c\x73\x65\x20\x69\x66\x20\x28\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x20\x3d\x3d\x20\x44\x47\x45\x4e\x5f\x57\x41\x56\x45\x5f\x54\x52\x49\x41\x4e\x47\x4c\x45\x29\x0a\x09\x7b\x0a\x09\x09\x66\x75\x6e\x63\x20\x3d\x20\x61\x62\x73\x28\x66\x72\x61\x63\x74\x28\x76\x61\x6c\x75\x65\x20\x2b\x20\x30\x2e\x37\x35\x29\x20\x2d\x20\x30\x2e\x35\x29\x20\x2a\x20\x34\x2e\x30\x20\x2d\x20\x31\x2e\x30\x3b\x0a\x09\x7d\x0a\x09\x65\x6c\x73\x65\x20\x69\x66\x20\x28\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x20\x3d\x3d\x20\x44\x47\x45\x4e\x5f\x57\x41\x56\x45\x5f\x53\x41\x57\x54\x4f\x4f\x54\x48\x29\x0a\x09\x7b\x0a\x09\x09\x66\x75\x6e\x63\x20\x3d\x20\x66\x72\x61\x63\x74\x28\x76\x61\x6c\x75\x65\x29\x3b\x0a\x09\x7d\x0a\x09\x65\x6c\x73\x65\x20\x69\x66\x20\x28\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x20\x3d\x3d\x20\x44\x47\x45\x4e\x5f\x57\x41\x56\x45\x5f\x49\x4e\x56\x45\x52\x53\x45\x5f\x53\x41\x57\x54\x4f\x4f\x54\x48\x29\x0a\x09\x7b\x0a\x09\x09\x66\x75\x6e\x63\x20\x3d\x20\x28\x31\x2e\x30\x20\x2d\x20\x66\x72\x61\x63\x74\x28\x76\x61\x6c\x75\x65\x29\x29\x3b\x0a\x09\x7d\x0a\x09\x65\x6c\x73\x65\x20\x2f\x2f\x20\x69\x66\x20\x28\x75\x5f\x44\x65\x66\x6f\x72\x6d\x47\x65\x6e\x20\x3d\x3d\x20\x44\x47\x45\x4e\x5f\x42\x55\x4c\x47\x45\x29\x0a\x09\x7b\x0a\x09\x09\x66\x75\x6e\x63\x20\x3d\x20\x73\x69\x6e\x28\x76\x61\x6c\x75\x65\x29\x3b\x0a\x09\x7d\x0a\x0a\x09\x72\x65\x74\x75\x72\x6e\x20\x70\x6f\x73\x20\x2b\x20\x6e\x6f\x72\x6d\x61\x6c\x20\x2a\x20\x28\x62\x61\x73\x65\x20\x2b\x20\x66\x75\x6e\x63\x20\x2a\x20\x61\x6d\x70\x6c\x69\x74\x75\x64\x65\x29\x3b\x0a\x7d\x0a\x0a\x76\x6f\x69\x64\x20\x6d\x61\x69\x6e\x28\x29\x0a\x7b\x0a\x09\x76\x65\x63\x33\x20\x70\x6f\x73\x69\x74\x69\x6f\x6e\x20\x20\x3d\x20\x6d\x69\x78\x28\x61\x74\x74\x72\x5f\x50\x6f\x73\x69\x74\x69\x6f\x6e\x2c\x20\x61\x74\x74\x72\x5f\x50\x6f\x73\x69\x74\x69\x6f\x6e\x32\x2c\x20\x75\x5f\x56\x65\x72\x74\x65\x78\x4c\x65\x72\x70\x29\x3b\x0a\x09\x76\x65\x63\x33\x20\x6e\x6f\x72\x6d\x61\x6c\x20\x20\x20\x20\x3d\x20\x6d\x69\x78\x28\x61\x74\x74\x72\x5f\x4e\x6f\x72\x6d\x61\x6c\x2c\x20\x20\x20\x61\x74\x74\x72\x5f\x4e\x6f\x72\x6d\x61\x6c\x32\x2c\x20\x20\x20\x75\x5f\x56\x65\x72\x74\x65\x78\x4c\x65\x72\x70\x29\x3b\x0a\x0a\x09\x70\x6f\x73\x69\x74\x69\x6f\x6e\x20\x3d\x20\x44\x65\x66\x6f\x72\x6d\x50\x6f\x73\x69\x74\x69\x6f\x6e\x28\x70\x6f\x73\x69\x74\x69\x6f\x6e\x2c\x20\x6e\x6f\x72\x6d\x61\x6c\x2c\x20\x61\x74\x74\x72\x5f\x54\x65\x78\x43\x6f\x6f\x72\x64\x30\x2e\x73\x74\x29\x3b\x0a\x0a\x09\x67\x6c\x5f\x50\x6f\x73\x69\x74\x69\x6f\x6e\x20\x3d\x20\x75\x5f\x4d\x6f\x64\x65\x6c\x56\x69\x65\x77\x50\x72\x6f\x6a\x65\x63\x74\x69\x6f\x6e\x4d\x61\x74\x72\x69\x78\x20\x2a\x20\x76\x65\x63\x34\x28\x70\x6f\x73\x69\x74\x69\x6f\x6e\x2c\x20\x31\x2e\x30\x29\x3b\x0a\x09\x0a\x09\x76\x61\x72\x5f\x50\x6f\x73\x69\x74\x69\x6f\x6e\x20\x20\x3d\x20\x28\x75\x5f\x4d\x6f\x64\x65\x6c\x4d\x61\x74\x72\x69\x78\x20\x2a\x20\x76\x65\x63\x34\x28\x70\x6f\x73\x69\x74\x69\x6f\x6e\x2c\x20\x31\x2e\x30\x29\x29\x2e\x78\x79\x7a\x3b\x0a\x7d\x0a";