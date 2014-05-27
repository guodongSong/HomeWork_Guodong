
void *HCoreLoadBinaryStream(const char *filename, unsigned int *size = NULL);
void HCoreReleaseBinaryStream(const void *buffer);

void *HCoreLoadFileStream(const char *filename, unsigned int *size=NULL);
void HCoreReleaseFileStream(const void *buffer);
